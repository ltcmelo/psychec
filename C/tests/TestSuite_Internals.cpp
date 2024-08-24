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
#include "TypeResolverTester.h"

#include "../common/infra/Assertions.h"

#include <algorithm>
#include <cstring>
#include <iostream>
#include <unordered_set>
#include <string>
#include <sstream>

//#define DUMP_AST
//#define DBG_DIAGNOSTICS
//#define DBG_BINDING_SEARCH

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

    auto D = std::make_unique<TypeResolverTester>(this);
    D->testTypeResolver();

    auto res = std::make_tuple(P->totalPassed()
                                    + B->totalPassed()
                                    + C->totalPassed()
                                    + D->totalPassed(),
                               P->totalFailed()
                                    + B->totalFailed()
                                    + C->totalFailed()
                                    + D->totalFailed());

    testers_.emplace_back(P.release());
    testers_.emplace_back(B.release());
    testers_.emplace_back(C.release());
    testers_.emplace_back(D.release());

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

#ifdef DBG_DIAGNOSTICS
    if (!tree_->diagnostics().empty()) {
        for (auto& diagnostic : tree_->diagnostics()) {
            diagnostic.outputIndent_ = 2;
            std::cout << std::endl << diagnostic << std::endl;
        }
        std::cout << "\t";
    }
#endif

    if (X.numW_ != W_cnt || X.numE_ != E_cnt) {
#ifdef DBG_DIAGNOSTICS
        std::cout << "\n\t" << std::string(25, '%') << "\n\t";
#endif
        std::cout << "mismatch in ";
        if (X.numW_ != W_cnt)
            std::cout << "WARNING";
        else
            std::cout << "ERROR";
        std::cout << " count";

#ifdef DBG_DIAGNOSTICS
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

#ifdef DBG_DIAGNOSTICS
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
#ifdef DBG_BINDING_SEARCH
    std::cout << "\n\t\tREJECT " << to_string(*sym) << " DUE TO " << msg;
#endif
    return false;
}

void DETAIL_MISMATCH(std::string msg)
{
#ifdef DBG_BINDING_SEARCH
    std::cout << "\n\t\t\tmismatch detail: " << msg;
#endif
}

bool CVRMatches(const Type* ty, CVR cvr)
{
    switch (cvr) {
        case CVR::Const:
            if (ty->kind() != TypeKind::Qualified
                    || !ty->asQualifiedType()->isConstQualified()) {
                DETAIL_MISMATCH("missing const");
                return false;
            }
            break;

        case CVR::Volatile:
            if (ty->kind() != TypeKind::Qualified
                    || !ty->asQualifiedType()->isVolatileQualified()) {
                DETAIL_MISMATCH("missing volatile");
                return false;
            }
            break;

        case CVR::ConstAndVolatile:
            if (ty->kind() != TypeKind::Qualified
                    || !ty->asQualifiedType()->isConstQualified()
                    || !ty->asQualifiedType()->isVolatileQualified()) {
                DETAIL_MISMATCH("missing const volatile");
                return false;
            }
            break;

        case CVR::Restrict:
            if (ty->kind() != TypeKind::Qualified
                    || !ty->asQualifiedType()->isRestrictQualified()) {
                DETAIL_MISMATCH("missing restrict");
                return false;
            }
            break;

        case CVR::ConstAndRestrict:
            if (ty->kind() != TypeKind::Qualified
                    || !ty->asQualifiedType()->isConstQualified()
                    || !ty->asQualifiedType()->isRestrictQualified()) {
                DETAIL_MISMATCH("missing const restrict");
                return false;
            }
            break;

        case CVR::Atomic:
            if (ty->kind() != TypeKind::Qualified
                    || !ty->asQualifiedType()->isAtomicQualified()) {
                DETAIL_MISMATCH("missing _Atomic");
                return false;
            }
            break;

        case CVR::None:
            if (ty->kind() == TypeKind::Qualified) {
                DETAIL_MISMATCH("spurious qualified type");
                return false;
            }
            break;
    }

    return true;
}

bool typeMatches(const Type* ty, const Ty& t)
{
    PSY_ASSERT_2(t.derivTyKs_.size() == t.derivTyCVRs_.size()
               && t.derivTyKs_.size() == t.derivPtrTyDecay_.size(),
               return false);

    for (auto i = t.derivTyKs_.size(); i > 0; --i) {
        auto derivTyCVR = t.derivTyCVRs_[i - 1];
        if (!CVRMatches(ty, derivTyCVR)) {
            DETAIL_MISMATCH("derived type CVR");
            return false;
        }
        if (ty->kind() == TypeKind::Qualified)
            ty = ty->asQualifiedType()->unqualifiedType();

        auto derivTyK = t.derivTyKs_[i - 1];
        if (derivTyK != ty->kind()) {
            DETAIL_MISMATCH("derived array/pointer/function type kind");
            return false;
        }

        auto derivPtrTyDecay = t.derivPtrTyDecay_[i - 1];

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
                if (parmTys.size() != t.parmsTys_.size()) {
                    DETAIL_MISMATCH("number of parameters");
                    return false;
                }

                for (auto i = 0U; i < parmTys.size(); ++i) {
                    const Type* parmTy = parmTys[i];
                    if (!typeMatches(parmTy, t.parmsTys_[i])) {
                        DETAIL_MISMATCH("parameter type mismatch");
                        return false;
                    }
                }

                ty = funcTy->returnType();
                if (!t.nestedRetTy_)
                    break;

                if (!typeMatches(ty, *t.nestedRetTy_.get())) {
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

    if (!CVRMatches(ty, t.CVR_)) {
        DETAIL_MISMATCH("type CVR");
        return false;
    }
    if (ty->kind() == TypeKind::Qualified)
        ty = ty->asQualifiedType()->unqualifiedType();

    if (ty->kind() != t.tyK_) {
        DETAIL_MISMATCH("basic/void/typedef/tag type mismatch");
        return false;
    }

    switch (ty->kind()) {
        case TypeKind::Basic:
            if (ty->asBasicType()->kind() != t.basicTyK_) {
                DETAIL_MISMATCH("basic type mismatch");
                return false;
            }
            break;

        case TypeKind::Void:
            break;

        case TypeKind::Typedef:
            if (ty->asTypedefType()->typedefName()->valueText() != t.ident_) {
                DETAIL_MISMATCH("typedef name mismatch");
                return false;
            }
            break;

        case TypeKind::Tag:
            if (ty->asTagType()->kind() != t.tagTyK_) {
                DETAIL_MISMATCH("tag type mismatch");
                return false;
            }
            if (ty->asTagType()->tag()->valueText() != t.ident_) {
                DETAIL_MISMATCH("tag mismatch");
                return false;
            }
            break;

        default:
            PSY_ASSERT_2(false, return false);
    }

    return true;
}

bool functionMatchesBinding(const Function* funcDecl, const Decl& decl)
{
    if (!funcDecl->name())
        return REJECT_CANDIDATE(funcDecl, "empty name");

    if (funcDecl->name()->valueText() != decl.ident_)
        return REJECT_CANDIDATE(funcDecl, "name mismatch");

    if (funcDecl->type() == nullptr)
        return REJECT_CANDIDATE(funcDecl, "null type");

    if (funcDecl->type()->kind() != TypeKind::Function)
        return REJECT_CANDIDATE(funcDecl, "not a function type");

    if (!typeMatches(funcDecl->type(), decl.ty_))
        return REJECT_CANDIDATE(funcDecl, "type mismatch");

    return true;
}

bool valueMatchesBinding(const ObjectDeclaration* objDecl, const Decl& decl)
{
    if (objDecl->kind() != decl.objDeclK_)
        return REJECT_CANDIDATE(objDecl, "value kind mismatch");

    if (!objDecl->name())
        return REJECT_CANDIDATE(objDecl, "empty value name");

    if (objDecl->name()->valueText() != decl.ident_)
        return REJECT_CANDIDATE(objDecl, "name mismatch");

    if (objDecl->type() == nullptr)
        return REJECT_CANDIDATE(objDecl, "null type");

    if (!typeMatches(objDecl->type(), decl.ty_))
        return REJECT_CANDIDATE(objDecl, "type mismatch");

    return true;
}

bool typeMatchesBinding(const TypeDeclaration* tyDecl, const Decl& decl)
{
    if (tyDecl->kind() != decl.tyDeclK_)
        return REJECT_CANDIDATE(tyDecl, "type decl kind mismatch");

    switch (decl.tyDeclK_) {
        case TypeDeclarationKind::Tag: {
            if (tyDecl->kind() != TypeDeclarationKind::Tag)
                return REJECT_CANDIDATE(tyDecl, "not a tag type");
            auto tagTyDecl = tyDecl->asTagTypeDeclaration();
            if (!tagTyDecl->specifiedType())
                return REJECT_CANDIDATE(tyDecl, "no specified type");
            if (!tagTyDecl->identifier())
                return REJECT_CANDIDATE(tyDecl, "empty identifier");
            if (tagTyDecl->identifier()->valueText() != decl.ident_)
                return REJECT_CANDIDATE(tyDecl, "identifier mismatch");
            auto tagTy = tagTyDecl->specifiedType();
            if (!tagTy->tag())
                return REJECT_CANDIDATE(tyDecl, "empty tag");
            if (tagTy->tag()->valueText() != decl.ident_)
                return REJECT_CANDIDATE(tyDecl, "tag mismatch");
            switch (decl.tagTyDeclK_) {
                case TagTypeDeclarationKind::Struct:
                    if (tagTyDecl->kind() != TagTypeDeclarationKind::Struct)
                        return REJECT_CANDIDATE(tyDecl, "not a struct");
                    if (tagTy->kind() != TagTypeKind::Struct)
                        return REJECT_CANDIDATE(tyDecl, "not a struct type");
                    break;
                case TagTypeDeclarationKind::Union:
                    if (tagTyDecl->kind() != TagTypeDeclarationKind::Union)
                        return REJECT_CANDIDATE(tyDecl, "not a union");
                    if (tagTy->kind() != TagTypeKind::Union)
                        return REJECT_CANDIDATE(tyDecl, "not a union type");
                    break;
                case TagTypeDeclarationKind::Enum:
                    if (tagTyDecl->kind() != TagTypeDeclarationKind::Enum)
                        return REJECT_CANDIDATE(tyDecl, "not a enum");
                    if (tagTy->kind() != TagTypeKind::Enum)
                        return REJECT_CANDIDATE(tyDecl, "not a enum type");
                    break;
            }
            break;
        }

        case TypeDeclarationKind::Typedef: {
            auto tydefDecl = tyDecl->asTypedef();
            if (!tydefDecl->definedType())
                return REJECT_CANDIDATE(tyDecl, "no defined type");
            if (!tydefDecl->definedType()->asTypedefType())
                return REJECT_CANDIDATE(tyDecl, "not a typedef type");
            auto tydefTy = tydefDecl->definedType()->asTypedefType();
            if (!tydefTy->typedefName())
                return REJECT_CANDIDATE(tyDecl, "empty typedef name");
            if (tydefTy->typedefName()->valueText() != decl.ident_)
                return REJECT_CANDIDATE(tyDecl, "typedef name mismatch");
            if (!typeMatches(tydefDecl->synonymizedType(), decl.ty_))
                return REJECT_CANDIDATE(tyDecl, "synonymized type mismatch");
            break;
        }
    }

    return true;
}

bool symbolMatchesBinding(const Declaration* declSym, const Decl& decl)
{
    if (declSym->kind() != decl.declK_)
        return REJECT_CANDIDATE(declSym, "symbol kind mismatch");

    if (declSym->enclosingScope()->kind() != decl.scopeK_)
        return REJECT_CANDIDATE(declSym, "scope kind mismatch");

    if (declSym->nameSpace() != decl.ns_)
        return REJECT_CANDIDATE(declSym, "name space kind mismatch");

    switch (decl.declK_)
    {
        case DeclarationKind::Object:
            return valueMatchesBinding(declSym->asObjectDeclaration(), decl);

        case DeclarationKind::Type: {
            return typeMatchesBinding(declSym->asTypeDeclaration(), decl);
        }

        case DeclarationKind::Function:
            return functionMatchesBinding(declSym->asFunction(), decl);

        default:
            PSY__internals__FAIL("unknkown symbol kind");
            return false;
    }

    return false;
};

bool symbolMatchesBinding_(const std::unique_ptr<Declaration>& declSym, const Decl& decl)
{
    return symbolMatchesBinding(declSym.get(), decl);
};

} // anonymous

void InternalsTestSuite::matchDeclarations(
        std::unique_ptr<SemanticModel> semaModel,
        std::vector<Decl> decls)
{
    for (const auto& Decl : decls) {
#ifdef DBG_BINDING_SEARCH
        std::cout << "\n\t\t...";
#endif
        using namespace std::placeholders;
        auto pred = std::bind(symbolMatchesBinding_, _1, Decl);
        auto decl = semaModel->searchForDecl(pred);
        if (decl == nullptr) {
            std::ostringstream oss;
            oss << "no declaration matches the expectation: ";
            oss << " identifier: " << Decl.ident_;
            oss << " kind: " << to_string(Decl.declK_);
            PSY__internals__FAIL(oss.str());
        }
#ifdef DBG_BINDING_SEARCH
        std::cout << "\n\t\tmatch! ";
#endif
    }

}

void InternalsTestSuite::checkSemanticModel(
        std::unique_ptr<SemanticModel> semaModel,
        Expectation X)
{
    if (!checkErrorAndWarn(X))
        return;

    auto unit = semaModel->translationUnit();
    if (unit == nullptr)
        PSY__internals__FAIL("unit not found");

    for (const auto& Decl : X.declarations_) {
#ifdef DBG_BINDING_SEARCH
        std::cout << "\n\t\t...";
#endif
        using namespace std::placeholders;
        auto pred = std::bind(symbolMatchesBinding_, _1, Decl);
        auto decl = semaModel->searchForDecl(pred);
        if (decl == nullptr) {
            std::ostringstream oss;
            oss << "no declaration matches the expectation: ";
            oss << " identifier: " << Decl.ident_;
            oss << " kind: " << to_string(Decl.declK_);
            PSY__internals__FAIL(oss.str());
        }
#ifdef DBG_BINDING_SEARCH
        std::cout << "\n\t\tmatch! ";
#endif

        if (X.checkScope_) {
            auto scope = unit->enclosedScope();
            PSY_EXPECT_TRUE(scope);
            while (!X.scopePath_.empty()) {
                std::size_t idx = X.scopePath_.back();
                X.scopePath_.pop_back();
                const auto& innerScopes = scope->innerScopes();
                PSY_EXPECT_TRUE(innerScopes.size() > idx);
                scope = innerScopes[idx];
                PSY_EXPECT_TRUE(scope);
            }
            auto sameDecl = scope->searchForDeclaration(decl->identifier(), decl->nameSpace());
            PSY_EXPECT_TRUE(sameDecl);
            PSY_EXPECT_TRUE(symbolMatchesBinding(sameDecl, Decl));
        }
    }
}

std::unique_ptr<SemanticModel> InternalsTestSuite::semanticModel(std::string text)
{
    parse(text);
    auto compilation = Compilation::create(tree_->filePath());
    compilation->addSyntaxTrees({ tree_.get() });
    std::unique_ptr<SemanticModel> semaModel(
                new SemanticModel(tree_.get(), compilation.get()));
    return semaModel;
}

void InternalsTestSuite::bind(std::string text, Expectation X)
{
    auto semaModel = semanticModel(text);
    semaModel->applyBinder();
    checkSemanticModel(std::move(semaModel), X);
}

void InternalsTestSuite::resolveTypes(std::string text, Expectation X)
{
    auto semaModel = semanticModel(text);
    semaModel->applyBinder();
    semaModel->applyTypeResolver();
    checkSemanticModel(std::move(semaModel), X);
}

