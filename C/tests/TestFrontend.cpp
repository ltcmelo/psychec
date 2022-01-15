// Copyright (c) 2020/21 Leandro T. C. Melo <ltcmelo@gmail.com>
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

#include "TestFrontend.h"

#include "Unparser.h"

#include "compilation/Assembly.h"
#include "compilation/Compilation.h"
#include "compilation/SemanticModel.h"
#include "symbols/Symbol.h"
#include "symbols/Symbols.h"
#include "syntax/SyntaxLexemes.h"
#include "syntax/SyntaxNamePrinter.h"
#include "syntax/SyntaxNodes.h"

#include <algorithm>
#include <cstring>
#include <iostream>
#include <unordered_set>
#include <string>
#include <sstream>

#define DEBUG_DIAGNOSTICS
//#define DUMP_AST

using namespace psy;
using namespace C;

TestFrontend::TestFrontend()
{}

TestFrontend::~TestFrontend()
{}

void TestFrontend::Expectation::setErrorCnt(int numE)
{
    numE_ = numE;
}

void TestFrontend::Expectation::setWarnCnt(int numW)
{
    numW_ = numW;
}

TestFrontend::Expectation& TestFrontend::Expectation::replicateAmbiguity(const std::string& s)
{
    ambiguousText_ = s;
    hasAmbiguity_ = true;
    return *this;
}

TestFrontend::Expectation& TestFrontend::Expectation::AST(std::vector<SyntaxKind>&& v)
{
    syntaxKinds_ = std::move(v);
    return *this;
}

TestFrontend::Expectation&
TestFrontend::Expectation::obj(const std::string& valSymName,
                               ValueKind valKind,
                               const std::string& tySymName,
                               TypeKind tyKind,
                               BuiltinTypeKind builtTyKind)
{
    objs_.push_back(std::make_tuple(valSymName,
                                    valKind,
                                    tySymName,
                                    tyKind,
                                    builtTyKind));
    return *this;
}

TestFrontend::Expectation&
TestFrontend::Expectation::qualObj(const std::string& valSymName,
                     ValueKind valKind,
                     const std::string& tySymName,
                     Qual qual,
                     TypeKind tyKind,
                     BuiltinTypeKind builtTyKind)
{
    qualObjs_.push_back(std::make_tuple(valSymName,
                                        valKind,
                                        tySymName,
                                        qual,
                                        tyKind,
                                        builtTyKind));
    return *this;
}

TestFrontend::Expectation&
TestFrontend::Expectation::objPtr_1(const std::string& valSymName,
                                    ValueKind valKind,
                                    TypeKind refedTyKind)
{
    objsPtr_1_.push_back(std::make_tuple(valSymName,
                                         valKind,
                                         refedTyKind,
                                         TypeKind::None));
    return *this;
}

TestFrontend::Expectation& TestFrontend::Expectation::addDiagnostic(ErrorOrWarn v, std::string descriptorId)
{
    if (v == ErrorOrWarn::Error) {
        ++numE_;
        if (!descriptorId.empty())
            descriptorsE_.push_back(descriptorId);
    }
    else {
        ++numW_;
        if (!descriptorId.empty())
            descriptorsW_.push_back(descriptorId);
    }
    return *this;
}

void TestFrontend::parseDeclaration(std::string source, Expectation X)
{
    parse(source, X, SyntaxTree::SyntaxCategory::Declarations);
}

void TestFrontend::parseExpression(std::string source, Expectation X)
{
    parse(source, X, SyntaxTree::SyntaxCategory::Expressions);
}

void TestFrontend::parseStatement(std::string source, Expectation X)
{
    parse(source, X, SyntaxTree::SyntaxCategory::Statements);
}

void TestFrontend::parse(std::string source,
                         Expectation X,
                         SyntaxTree::SyntaxCategory cat)
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

    tree_ = SyntaxTree::parseText(text, ParseOptions(), "", cat);

    int E_cnt = 0;
    int W_cnt = 0;
    std::unordered_set<std::string> E_IDs;
    std::unordered_set<std::string> W_IDs;
    for (const auto& diagnostic : tree_->diagnostics()) {
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

    PSYCHE_EXPECT_INT_EQ(X.numW_, W_cnt);
    PSYCHE_EXPECT_INT_EQ(X.numE_, E_cnt);

    for (const auto& id : X.descriptorsW_) {
        if (!W_IDs.count(id)) {
            std::string msg = "WARNING " + id + " not found, got:";
            for (const auto& idP : W_IDs)
                msg += "\n\t\t- " + idP;
            PSYCHE_TEST_FAIL(msg);
        }
    }

    for (const auto& id : X.descriptorsE_) {
        if (!E_IDs.count(id)) {
            std::string msg = "ERROR " + id + " not found, got:";
            for (const auto& idP : E_IDs)
                msg += "\n\t\t- " + idP;
            PSYCHE_TEST_FAIL(msg);
        }
    }

    if (X.numE_)
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

    if (X.hasAmbiguity_) {
        if (X.ambiguousText_.empty())
            PSYCHE_EXPECT_STR_EQ(text + text, textP);
        else {
            X.ambiguousText_.erase(
                        std::remove_if(X.ambiguousText_.begin(),
                                       X.ambiguousText_.end(), ::isspace),
                        X.ambiguousText_.end());
            PSYCHE_EXPECT_STR_EQ(X.ambiguousText_, textP);
        }
    }
    else
        PSYCHE_EXPECT_STR_EQ(text, textP);

    if (X.syntaxKinds_.empty())
        return;

    std::string names;
    for (auto k : X.syntaxKinds_)
        names += to_string(k);

    std::string namesP = ossTree.str();
    namesP.erase(std::remove_if(namesP.begin(), namesP.end(), ::isspace), namesP.end());
    PSYCHE_EXPECT_STR_EQ(names, namesP);
}

void TestFrontend::bind(std::string text,
                        Expectation X)
{
    parse(text);

    auto compilation = Compilation::create(tree_->filePath());
    compilation->addSyntaxTrees({ tree_.get() });
    compilation->semanticModel(tree_.get());







    int E_cnt = 0;
    int W_cnt = 0;
    std::unordered_set<std::string> E_IDs;
    std::unordered_set<std::string> W_IDs;
    for (const auto& diagnostic : tree_->diagnostics()) {
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

    PSYCHE_EXPECT_INT_EQ(X.numW_, W_cnt);
    PSYCHE_EXPECT_INT_EQ(X.numE_, E_cnt);

    for (const auto& id : X.descriptorsW_) {
        if (!W_IDs.count(id)) {
            std::string msg = "WARNING " + id + " not found, got:";
            for (const auto& idP : W_IDs)
                msg += "\n\t\t- " + idP;
            PSYCHE_TEST_FAIL(msg);
        }
    }

    for (const auto& id : X.descriptorsE_) {
        if (!E_IDs.count(id)) {
            std::string msg = "ERROR " + id + " not found, got:";
            for (const auto& idP : E_IDs)
                msg += "\n\t\t- " + idP;
            PSYCHE_TEST_FAIL(msg);
        }
    }

    if (X.numE_)
        return;










    auto sym = compilation->assembly()->findSymDEF(
                [] (const auto& sym) {
                    return sym->kind() == SymbolKind::LinkUnit;
                });
    if (sym == nullptr)
        PSYCHE_TEST_FAIL("link unit not found");

    for (auto objData : X.objs_) {
        auto valSymName = std::get<0>(objData);
        auto valKind = std::get<1>(objData);
        auto tySymName = std::get<2>(objData);
        auto tyKind = std::get<3>(objData);
        auto builtTyKind = std::get<4>(objData);

        auto sym = compilation->assembly()->findSymDEF(
                [&] (const auto& v) {
                    const Symbol* sym = v.get();
                    if (sym->kind() != SymbolKind::Value)
                        return false;

                    const ValueSymbol* actualSym = sym->asValue();
                    if (!(actualSym->name() != nullptr
                           && to_string(*actualSym->name()) == valSymName
                           && actualSym->valueKind() == valKind
                           && actualSym->type() != nullptr
                           && actualSym->type()->name() != nullptr
                           && to_string(*actualSym->type()->name()) == tySymName
                           && actualSym->type()->typeKind() == tyKind))
                        return false;

                    if (tyKind == TypeKind::Builtin) {
                        const NamedTypeSymbol* namedTySym = actualSym->type()->asNamedType();
                        if (!(namedTySym->builtinTypeKind() == builtTyKind))
                            return false;
                    }
                    else {
                        if (builtTyKind != BuiltinTypeKind::None)
                            return false;
                    }

                    return true;
                });

        if (sym == nullptr) {
            auto s = "cannot find "
                    + to_string(valKind) + " " + valSymName + " "
                    + to_string(tyKind) + " " + to_string(builtTyKind) + " "
                    + (tySymName == "" ? "<unnamed>" : tySymName);
            PSYCHE_TEST_FAIL(s);
        }
    }

    for (auto qualObjData : X.qualObjs_) {
        auto valSymName = std::get<0>(qualObjData);
        auto valKind = std::get<1>(qualObjData);
        auto tySymName = std::get<2>(qualObjData);
        auto qual = std::get<3>(qualObjData);
        auto tyKind = std::get<4>(qualObjData);
        auto builtTyKind = std::get<5>(qualObjData);

        auto sym = compilation->assembly()->findSymDEF(
                [&] (const auto& v) {
                    const Symbol* sym = v.get();
                    if (sym->kind() != SymbolKind::Value)
                        return false;

                    const ValueSymbol* actualSym = sym->asValue();
                    if (!(actualSym->name() != nullptr
                           && to_string(*actualSym->name()) == valSymName
                           && actualSym->valueKind() == valKind
                           && actualSym->type() != nullptr
                           && actualSym->type()->name() != nullptr
                           && to_string(*actualSym->type()->name()) == tySymName
                           && actualSym->type()->typeKind() == tyKind))
                        return false;

                    switch (qual) {
                        case Expectation::Qual::Const:
                            if (!actualSym->type()->isConstQualified())
                                return false;
                            break;

                        case Expectation::Qual::Volatile:
                            if (!actualSym->type()->isVolatileQualified())
                                return false;
                            break;

                        case Expectation::Qual::ConstAndVolatile:
                            if (!(actualSym->type()->isConstQualified())
                                    || !(actualSym->type()->isVolatileQualified()))
                            break;

                        case Expectation::Qual::Restrict:
                            if (!actualSym->type()->isRestrictQualified())
                                return false;
                            break;
                    }

                    if (tyKind == TypeKind::Builtin) {
                        const NamedTypeSymbol* namedTySym = actualSym->type()->asNamedType();
                        if (!(namedTySym->builtinTypeKind() == builtTyKind))
                            return false;
                    }
                    else {
                        if (builtTyKind != BuiltinTypeKind::None)
                            return false;
                    }

                    return true;
                });

        if (sym == nullptr) {
            auto s = "cannot find "
                    + to_string(valKind) + " " + valSymName + " "
                    + std::to_string((int)qual) + " "
                    + to_string(tyKind) + " " + to_string(builtTyKind) + " "
                    + (tySymName == "" ? "<unnamed>" : tySymName);
            PSYCHE_TEST_FAIL(s);
        }
    }

    for (auto objPtr_1_Data : X.objsPtr_1_) {
        auto valSymName = std::get<0>(objPtr_1_Data);
        auto valKind = std::get<1>(objPtr_1_Data);
        auto refedTyKind1 = std::get<2>(objPtr_1_Data);
        auto refedTyKind2 = std::get<3>(objPtr_1_Data);

        auto sym = compilation->assembly()->findSymDEF(
                [&] (const auto& v) {
                    const Symbol* sym = v.get();
                    if (sym->kind() != SymbolKind::Value)
                        return false;

                    const ValueSymbol* actualSym = sym->asValue();
                    if (!(actualSym->name() != nullptr
                           && to_string(*actualSym->name()) == valSymName
                           && actualSym->valueKind() == valKind
                           && actualSym->type() != nullptr
                           && actualSym->type()->typeKind() == TypeKind::Pointer))
                        return false;

                    return true;
                });

        if (sym == nullptr) {
            auto s = "cannot find "
                    + to_string(valKind) + " " + valSymName + " "
                    + to_string(refedTyKind1) + " " + to_string(refedTyKind2);
            PSYCHE_TEST_FAIL(s);
        }
    }
}

void TestFrontend::typeCheck(std::string text)
{
}
