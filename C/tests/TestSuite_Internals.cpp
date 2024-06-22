// Copyright (c) 2020/21/22 Leandro T. C. Melo <ltcmelo@gmail.com>
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.

#include "TestSuite_Internals.h"

#include "compilation/Compilation.h"
#include "compilation/SemanticModel.h"
#include "symbols/Symbol.h"
#include "parser/Unparser.h"
#include "symbols/Symbol_ALL.h"
#include "syntax/Lexeme_ALL.h"
#include "syntax/SyntaxNamePrinter.h"
#include "syntax/SyntaxNodes.h"

#include "BinderTester.h"
#include "ParserTester.h"
#include "ReparserTester.h"

#include "../common/infra/Assertions.h"

#include <algorithm>
#include <cstring>
#include <iostream>
#include <unordered_set>
#include <string>
#include <sstream>

//#define DUMP_AST
#define DEBUG_DIAGNOSTICS
#define DEBUG_BINDING_SEARCH

using namespace psy;
using namespace C;

InternalsTestSuite::InternalsTestSuite()
{}

InternalsTestSuite::~InternalsTestSuite()
{}

std::tuple<int, int> InternalsTestSuite::testAll()
{
    auto P = std::make_unique<ParserTester>(this);
    P->testParser();

    auto B = std::make_unique<ReparserTester>(this);
    B->testReparser();

    auto C = std::make_unique<BinderTester>(this);
    C->testBinder();

    auto res = std::make_tuple(P->totalPassed()
                                    + B->totalPassed()
                                    + C->totalPassed(),
                               P->totalFailed()
                                    + B->totalFailed()
                                    + C->totalFailed());

    testers_.emplace_back(P.release());
    testers_.emplace_back(B.release());
    testers_.emplace_back(C.release());

    return res;
}

std::string InternalsTestSuite::description() const
{
    return "C internals test suite";
}

void InternalsTestSuite::printSummary() const
{
    for (auto const& tester : testers_) {
        std::cout << "    " << tester->name() << std::endl
                  << "        passed: " << tester->totalPassed() << std::endl
                  << "        failed: " << tester->totalFailed() << std::endl;
    }
}

bool InternalsTestSuite::checkErrorAndWarn(Expectation X)
{
    int E_cnt = 0;
    int W_cnt = 0;
    std::unordered_set<std::string> E_IDs;
    std::unordered_set<std::string> W_IDs;
    for (const auto& diagnostic : tree_->diagnostics()) {
        if (X.containsAmbiguity_
                && isDiagnosticDescriptorIdOfSyntaxAmbiguity(diagnostic.descriptor().id()))
            continue;

        if (diagnostic.severity() == DiagnosticSeverity::Error) {
            ++E_cnt;
            E_IDs.insert(diagnostic.descriptor().id());
        }
        else if (diagnostic.severity() == DiagnosticSeverity::Warning) {
            ++W_cnt;
            W_IDs.insert(diagnostic.descriptor().id());
        }
    }

#ifdef DEBUG_DIAGNOSTICS
    if (!tree_->diagnostics().empty()) {
        for (auto& diagnostic : tree_->diagnostics()) {
            diagnostic.outputIndent_ = 2;
            std::cout << std::endl << diagnostic << std::endl;
        }
        std::cout << "\t";
    }
#endif

    if (X.numW_ != W_cnt || X.numE_ != E_cnt) {
#ifdef DEBUG_DIAGNOSTICS
        std::cout << "\n\t" << std::string(25, '%') << "\n\t";
#endif
        std::cout << "mismatch in ";
        if (X.numW_ != W_cnt)
            std::cout << "WARNING";
        else
            std::cout << "ERROR";
        std::cout << " count";

#ifdef DEBUG_DIAGNOSTICS
        std::cout << "\n\t" << std::string(25, '%');
#endif
    }

    PSY_EXPECT_EQ_INT(W_cnt, X.numW_);
    PSY_EXPECT_EQ_INT(E_cnt, X.numE_);

    for (const auto& id : X.descriptorsW_) {
        if (!W_IDs.count(id)) {
            std::string msg = "WARNING " + id + " not found, got:";
            for (const auto& idP : W_IDs)
                msg += "\n\t\t- " + idP;
            PSY__internals__FAIL(msg);
        }
    }

    for (const auto& id : X.descriptorsE_) {
        if (!E_IDs.count(id)) {
            std::string msg = "ERROR " + id + " not found, got:";
            for (const auto& idP : E_IDs)
                msg += "\n\t\t- " + idP;
            PSY__internals__FAIL(msg);
        }
    }

    if (X.numE_ && !X.continueTestDespiteOfErrors_)
        return false;

    return true;
}

void InternalsTestSuite::parseDeclaration(std::string source, Expectation X)
{
    parse(source, X, SyntaxTree::SyntaxCategory::Declarations);
}

void InternalsTestSuite::parseExpression(std::string source, Expectation X)
{
    parse(source, X, SyntaxTree::SyntaxCategory::Expressions);
}

void InternalsTestSuite::parseStatement(std::string source, Expectation X)
{
    parse(source, X, SyntaxTree::SyntaxCategory::Statements);
}

void InternalsTestSuite::parse(std::string source,
                               Expectation X,
                               SyntaxTree::SyntaxCategory syntaxCat)
{
    auto text = source;

#ifdef DEBUG_DIAGNOSTICS
    if (X.numW_ > 0 || X.numE_ > 0) {
        std::cout << std::endl;
        if (X.numW_ > 0)
            std::cout << "\t\t[expect (parser) WARNING]\n";
        if (X.numE_ > 0)
            std::cout << "\t\t[expect (parser) ERROR]\n";
    }
#endif

    ParseOptions parseOpts;
    if (X.containsAmbiguity_)
        parseOpts.setAmbiguityMode(ParseOptions::AmbiguityMode::Diagnose);

    tree_ = SyntaxTree::parseText(text,
                                  TextPreprocessingState::Unknown,
                                  TextCompleteness::Fragment,
                                  parseOpts,
                                  "",
                                  syntaxCat);

    if (X.numE_ == 0 && X.numW_ == 0 && tree_->parseExitedEarly()) {
        PSY_EXPECT_TRUE(X.unfinishedParse_);
        return;
    }

    if (!checkErrorAndWarn(X))
        return;

    std::ostringstream ossTree;
    SyntaxNamePrinter printer(tree_.get());
    printer.print(tree_->root(),
                  SyntaxNamePrinter::Style::Plain,
                  ossTree);

#ifdef DUMP_AST
    std::cout << "\n\n"
              << "========================== AST ==================================\n"
              << source << "\n"
              << "-----------------------------------------------------------------"
              << ossTree.str()
              << "=================================================================\n";
#endif

    std::ostringstream ossText;
    Unparser unparser(tree_.get());
    unparser.unparse(tree_->root(), ossText);

    std::string textP = ossText.str();
    textP.erase(std::remove_if(textP.begin(), textP.end(), ::isspace), textP.end());
    text.erase(std::remove_if(text.begin(), text.end(), ::isspace), text.end());

    if (X.containsAmbiguity_) {
        if (X.ambiguityText_.empty())
            PSY_EXPECT_EQ_STR(textP, text + text);
        else {
            X.ambiguityText_.erase(
                        std::remove_if(X.ambiguityText_.begin(),
                                       X.ambiguityText_.end(), ::isspace),
                        X.ambiguityText_.end());
            PSY_EXPECT_EQ_STR(textP, X.ambiguityText_);
        }
    }
    else
        PSY_EXPECT_EQ_STR(textP, text);

    if (X.syntaxKinds_.empty())
        return;

    std::string names;
    for (auto k : X.syntaxKinds_)
        names += to_string(k);

    std::string namesP = ossTree.str();
    namesP.erase(std::remove_if(namesP.begin(), namesP.end(), ::isspace), namesP.end());
    PSY_EXPECT_EQ_STR(namesP, names);
}

void InternalsTestSuite::reparse(std::string source,
                                 Reparser::DisambiguationStrategy strategy,
                                 Expectation X)
{
    auto text = source;

    ParseOptions parseOpts;
    TextCompleteness textCompleness;
    switch (strategy) {
        case Reparser::DisambiguationStrategy::SyntaxCorrelation:
            parseOpts.setAmbiguityMode(
                        ParseOptions::AmbiguityMode::DisambiguateAlgorithmically);
            textCompleness = TextCompleteness::Fragment;
            break;

        case Reparser::DisambiguationStrategy::TypeSynonymsVerification:
            parseOpts.setAmbiguityMode(
                        ParseOptions::AmbiguityMode::DisambiguateAlgorithmically);
            textCompleness = TextCompleteness::Full;
            break;

        case Reparser::DisambiguationStrategy::GuidelineImposition:
            parseOpts.setAmbiguityMode(
                        ParseOptions::AmbiguityMode::DisambiguateHeuristically);
            textCompleness = TextCompleteness::Unknown;
            break;

        default:
            PSY__internals__FAIL("unknown strategy");
    }

    tree_ = SyntaxTree::parseText(text,
                                  TextPreprocessingState::Unknown,
                                  textCompleness,
                                  parseOpts,
                                  "");

    if (!X.containsAmbiguity_ && !checkErrorAndWarn(X))
        return;

    std::ostringstream ossTree;
    SyntaxNamePrinter printer(tree_.get());
    printer.print(tree_->root(),
                  SyntaxNamePrinter::Style::Plain,
                  ossTree);

#ifdef DUMP_AST
    std::cout << "\n\n"
              << "========================== AST ==================================\n"
              << source << "\n"
              << "-----------------------------------------------------------------"
              << ossTree.str()
              << "=================================================================\n";
#endif

    std::ostringstream ossText;
    Unparser unparser(tree_.get());
    unparser.unparse(tree_->root(), ossText);

    std::string textP = ossText.str();
    textP.erase(std::remove_if(textP.begin(), textP.end(), ::isspace), textP.end());
    text.erase(std::remove_if(text.begin(), text.end(), ::isspace), text.end());

    if (X.containsAmbiguity_) {
        if (X.ambiguityText_.empty())
            PSY_EXPECT_EQ_STR(textP, text + text);
        else {
            X.ambiguityText_.erase(
                        std::remove_if(X.ambiguityText_.begin(),
                                       X.ambiguityText_.end(), ::isspace),
                        X.ambiguityText_.end());
            PSY_EXPECT_EQ_STR(textP, X.ambiguityText_);
        }
    }
    else
        PSY_EXPECT_EQ_STR(textP, text);

    if (X.syntaxKinds_.empty())
        return;

    std::string names;
    for (auto k : X.syntaxKinds_)
        names += to_string(k);

    std::string namesP = ossTree.str();
    namesP.erase(std::remove_if(namesP.begin(), namesP.end(), ::isspace), namesP.end());
    PSY_EXPECT_EQ_STR(namesP, names);
}

void InternalsTestSuite::reparse_withSyntaxCorrelation(std::string text, Expectation X)
{
    reparse(text, Reparser::DisambiguationStrategy::SyntaxCorrelation, X);
}

void InternalsTestSuite::reparse_withTypeSynonymVerification(std::string text, Expectation X)
{
    reparse(text, Reparser::DisambiguationStrategy::TypeSynonymsVerification, X);
}

void InternalsTestSuite::reparse_withGuidelineImposition(std::string text, Expectation X)
{
    reparse(text, Reparser::DisambiguationStrategy::GuidelineImposition, X);
}

namespace {

bool REJECT_CANDIDATE(const Symbol* sym, std::string msg)
{
#ifdef DEBUG_BINDING_SEARCH
    std::cout << "\n\t\tREJECT " << to_string(*sym) << " DUE TO " << msg;
#endif
    return false;
}

void DETAIL_MISMATCH(std::string msg)
{
#ifdef DEBUG_BINDING_SEARCH
    std::cout << "\n\t\t\tmismatch detail: " << msg;
#endif
}

bool CVRMatches(const Type* ty, CVR cvr)
{
    switch (cvr) {
        case CVR::Const:
            if (!ty->isConstQualified()) {
                DETAIL_MISMATCH("missing const");
                return false;
            }
            break;

        case CVR::Volatile:
            if (!ty->isVolatileQualified()) {
                DETAIL_MISMATCH("missing volatile");
                return false;
            }
            break;

        case CVR::ConstAndVolatile:
            if (!(ty->isConstQualified())
                    || !(ty->isVolatileQualified())) {
                DETAIL_MISMATCH("missing const volatile");
                return false;
            }
            break;

        case CVR::Restrict:
            if (!ty->isRestrictQualified()) {
                DETAIL_MISMATCH("missing restrict");
                return false;
            }
            break;

        case CVR::ConstAndRestrict:
            if (!(ty->isConstQualified())
                    || !(ty->isRestrictQualified())) {
                DETAIL_MISMATCH("missing const restrict");
                return false;
            }
            break;

        case CVR::Atomic:
            if (!ty->isAtomicQualified()) {
                DETAIL_MISMATCH("missing _Atomic");
                return false;
            }
            break;

        case CVR::None:
            if (ty->isConstQualified()) {
                DETAIL_MISMATCH("spurious const");
                return false;
            }
            if (ty->isVolatileQualified()) {
                DETAIL_MISMATCH("spurious volatile");
                return false;
            }
            if (ty->isRestrictQualified()) {
                DETAIL_MISMATCH("spurious restrict");
                return false;
            }
            break;
    }

    return true;
}

bool typeMatches(const Type* ty, const TySummary& tySummary)
{
    PSY_ASSERT(tySummary.derivTyKs_.size() == tySummary.derivTyCVRs_.size()
               && tySummary.derivTyKs_.size() == tySummary.derivPtrTyDecay_.size(),
               return false);

    for (auto i = tySummary.derivTyKs_.size(); i > 0; --i) {
        auto derivTyK = tySummary.derivTyKs_[i - 1];
        if (derivTyK != ty->kind()) {
            DETAIL_MISMATCH("derived array/pointer/function type kind");
            return false;
        }

        auto derivTyCVR = tySummary.derivTyCVRs_[i - 1];
        if (!CVRMatches(ty, derivTyCVR)) {
            DETAIL_MISMATCH("derived type CVR");
            return false;
        }

        auto derivPtrTyDecay = tySummary.derivPtrTyDecay_[i - 1];

        switch (ty->kind()) {
            case TypeKind::Array:
                if (derivPtrTyDecay != Decay::None) {
                    DETAIL_MISMATCH("can't happen for array");
                    return false;
                }
                ty = ty->asArrayType()->elementType();
                break;

            case TypeKind::Pointer:
                switch (derivPtrTyDecay) {
                    case Decay::None:
                        if (ty->asPointerType()->arisesFromFunctionDecay()
                                    || ty->asPointerType()->arisesFromArrayDecay()) {
                            DETAIL_MISMATCH("pointer type is decayed from array/function");
                            return false;
                        }
                        break;

                    case Decay::FromFunctionToFunctionPointer:
                        if (!ty->asPointerType()->arisesFromFunctionDecay()) {
                            DETAIL_MISMATCH("pointer type isn't (function) decayed");
                            return false;
                        }
                        break;

                    case Decay::FromArrayToPointer:
                        if (!ty->asPointerType()->arisesFromArrayDecay()) {
                            DETAIL_MISMATCH("pointer type isn't (array) decayed");
                            return false;
                        }
                        break;
                }
                ty = ty->asPointerType()->referencedType();
                break;

            case TypeKind::Function: {
                if (derivPtrTyDecay != Decay::None) {
                    DETAIL_MISMATCH("can't happen for function");
                    return false;
                }

                auto funcTy = ty->asFunctionType();
                auto parmTys = funcTy->parameterTypes();
                if (parmTys.size() != tySummary.parmsTys_.size()) {
                    DETAIL_MISMATCH("number of parameters");
                    return false;
                }

                for (auto i = 0U; i < parmTys.size(); ++i) {
                    const Type* parmTy = parmTys[i];
                    if (!typeMatches(parmTy, tySummary.parmsTys_[i])) {
                        DETAIL_MISMATCH("parameter type mismatch");
                        return false;
                    }
                }

                ty = funcTy->returnType();
                if (!tySummary.nestedRetTy_)
                    break;

                if (!typeMatches(ty, *tySummary.nestedRetTy_.get())) {
                    DETAIL_MISMATCH("nested return type mismatch");
                    return false;
                }
                return true;
            }

            default:
                PSY__internals__FAIL("unexpected");
                return false;
        }
    }

    if (ty->kind() != tySummary.tyK_) {
        DETAIL_MISMATCH("basic/void/typedef/tag type mismatch");
        return false;
    }

    switch (ty->kind()) {
        case TypeKind::Basic:
            if (ty->asBasicType()->kind() != tySummary.basicTyK_) {
                DETAIL_MISMATCH("basic type mismatch");
                return false;
            }
            break;

        case TypeKind::Void:
            break;

        case TypeKind::Typedef:
            if (ty->asTypedefType()->typedefName()->valueText() != tySummary.nameOrTag_) {
                DETAIL_MISMATCH("typedef name mismatch");
                return false;
            }
            break;

        case TypeKind::Tag:
            if (ty->asTagType()->kind() != tySummary.tagTyK_) {
                DETAIL_MISMATCH("tag type mismatch");
                return false;
            }
            if (ty->asTagType()->tag()->valueText() != tySummary.nameOrTag_) {
                DETAIL_MISMATCH("tag mismatch");
                return false;
            }
            break;

        default:
            PSY_ASSERT(false, return false);
    }

    if (!CVRMatches(ty, tySummary.CVR_)) {
        DETAIL_MISMATCH("type CVR");
        return false;
    }

    return true;
}

bool functionMatchesBinding(const Function* funcSym, const DeclSummary& decl)
{
    if (!funcSym->name())
        return REJECT_CANDIDATE(funcSym, "empty name");

    if (funcSym->name()->valueText() != decl.id_)
        return REJECT_CANDIDATE(funcSym, "name mismatch");

    if (funcSym->type() == nullptr)
        return REJECT_CANDIDATE(funcSym, "null type");

    if (funcSym->type()->kind() != TypeKind::Function)
        return REJECT_CANDIDATE(funcSym, "not a function type");

    if (!typeMatches(funcSym->type(), decl.Ty))
        return REJECT_CANDIDATE(funcSym, "type mismatch");

    return true;
}

bool valueMatchesBinding(const ObjectDeclarationSymbol* valSym, const DeclSummary& decl)
{
    if (valSym->kind() != decl.valDeclK_)
        return REJECT_CANDIDATE(valSym, "value kind mismatch");

    if (!valSym->name())
        return REJECT_CANDIDATE(valSym, "empty value name");

    if (valSym->name()->valueText() != decl.id_)
        return REJECT_CANDIDATE(valSym, "name mismatch");

    if (valSym->type() == nullptr)
        return REJECT_CANDIDATE(valSym, "null type");

    if (!typeMatches(valSym->type(), decl.Ty))
        return REJECT_CANDIDATE(valSym, "type mismatch");

    return true;
}

bool typeMatchesBinding(const TypeDeclarationSymbol* tySym, const DeclSummary& declSummary)
{
    if (tySym->kind() != declSummary.tyDeclSymK_)
        return REJECT_CANDIDATE(tySym, "type decl kind mismatch");

    if (!tySym->specifiedType())
        return REJECT_CANDIDATE(tySym, "no specified type");

    auto checkTag = [=]() {
        if (!tySym->specifiedType()->asTagType())
            return REJECT_CANDIDATE(tySym, "not a tag type");
        auto tagTy = tySym->specifiedType()->asTagType();
        if (!tagTy->tag())
            return REJECT_CANDIDATE(tySym, "empty tag");
        if (tagTy->tag()->valueText() != declSummary.id_)
            return REJECT_CANDIDATE(tySym, "tag mismatch");
        return true;
    };

    switch (declSummary.tyDeclSymK_) {
        case TypeDeclarationSymbolKind::Struct:
            if (!checkTag())
                return false;
            if (tySym->specifiedType()->asTagType()->kind() != TagTypeKind::Struct)
                return REJECT_CANDIDATE(tySym, "specified type struct mismatch");
            break;

        case TypeDeclarationSymbolKind::Union:
            if (!checkTag())
                return false;
            if (tySym->specifiedType()->asTagType()->kind() != TagTypeKind::Union)
                return REJECT_CANDIDATE(tySym, "specified type union mismatch");
            break;

        case TypeDeclarationSymbolKind::Enum:
            if (!checkTag())
                return false;
            if (tySym->specifiedType()->asTagType()->kind() != TagTypeKind::Enum)
                return REJECT_CANDIDATE(tySym, "specified type enum mismatch");
            break;

        case TypeDeclarationSymbolKind::Typedef: {
            if (!tySym->specifiedType()->asTypedefType())
                return REJECT_CANDIDATE(tySym, "not a typedef type");
            auto typedefTy = tySym->specifiedType()->asTypedefType();
            if (!typedefTy->typedefName())
                return REJECT_CANDIDATE(tySym, "empty typedef name");
            if (typedefTy->typedefName()->valueText() != declSummary.id_)
                return REJECT_CANDIDATE(tySym, "typedef name mismatch");
            break;
        }
    }

    return true;
}

bool symbolMatchesBinding(const std::unique_ptr<DeclarationSymbol>& sym, const DeclSummary& summary)
{
    const DeclarationSymbol* candSym = sym.get();

    if (candSym->kind() != summary.declK_)
        return REJECT_CANDIDATE(candSym, "symbol kind mismatch");

    if (candSym->scope()->kind() != summary.scopeK_)
        return REJECT_CANDIDATE(candSym, "scope kind mismatch");

    if (candSym->nameSpace() != summary.ns_)
        return REJECT_CANDIDATE(candSym, "name space kind mismatch");

    switch (summary.declK_)
    {
        case DeclarationSymbolKind::Object:
            return valueMatchesBinding(candSym->asObjectDeclarationSymbol(), summary);

        case DeclarationSymbolKind::Type: {
            return typeMatchesBinding(candSym->asTypeDeclarationSymbol(), summary);
        }

        case DeclarationSymbolKind::Function:
            return functionMatchesBinding(candSym->asFunction(), summary);

        default:
            PSY__internals__FAIL("unknkown symbol kind");
            return false;
    }

    return false;
};

} // anonymous

void InternalsTestSuite::bind(std::string text, Expectation X)
{
    parse(text);
    auto compilation = Compilation::create(tree_->filePath());
    compilation->addSyntaxTrees({ tree_.get() });
    auto semaModel = compilation->computeSemanticModel(tree_.get());

    if (!checkErrorAndWarn(X))
        return;

    auto unitSym = semaModel->translationUnit();
    if (unitSym == nullptr)
        PSY__internals__FAIL("unit not found");

    for (const auto& binding : X.bindings_) {
#ifdef DEBUG_BINDING_SEARCH
        std::cout << "\n\t\t...";
#endif
        using namespace std::placeholders;

        auto pred = std::bind(symbolMatchesBinding, _1, binding);
        auto declSym = semaModel->searchForDecl(pred);
        if (declSym == nullptr) {
            std::ostringstream oss;
            oss << "no symbol matches the expectation: ";
            oss << " name or tag: " << binding.id_;
            oss << " decl kind: " << to_string(binding.declK_);
            PSY__internals__FAIL(oss.str());
        }

#ifdef DEBUG_BINDING_SEARCH
        std::cout << "\n\t\tmatch! ";
#endif

    }
}
