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

#include "syntax/SyntaxLexemes.h"
#include "Unparser.h"

#include "common/diagnostics/DiagnosticCollector.h"
#include "syntax/SyntaxNamePrinter.h"
#include "syntax/SyntaxNodes.h"

#include <algorithm>
#include <cstring>
#include <iostream>
#include <unordered_set>
#include <string>
#include <sstream>

#define DEBUG_DIAGNOSTICS
#define DUMP_AST

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
    syntax_ = std::move(v);
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
            std::cout << "\t\t[expect (syntax) WARNING]\n";
        if (X.numE_ > 0)
            std::cout << "\t\t[expect (syntax) ERROR]\n";
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
    printer.print(tree_->root(), ossTree);

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

    if (X.syntax_.empty())
        return;

    std::string names;
    for (auto k : X.syntax_)
        names += to_string(k);

    std::string namesP = ossTree.str();
    namesP.erase(std::remove_if(namesP.begin(), namesP.end(), ::isspace), namesP.end());
    PSYCHE_EXPECT_STR_EQ(names, namesP);
}

void TestFrontend::parseAndBind(std::string text, int errorCnt, int warnCnt)
{
    parse(text);

    if (errorCnt > 0)
        std::cout << " [expect (binding) error] ";
    if (warnCnt > 0)
        std::cout << " [expect (binding) warning] ";

    //tree_->bind();

    PSYCHE_EXPECT_INT_EQ(warnCnt, diagnosticCollector_.warningsCount());
    PSYCHE_EXPECT_INT_EQ(errorCnt, diagnosticCollector_.errorsCount());
}

void TestFrontend::parseAndBindAndCheck(std::string text, int errorCnt, int warnCnt)
{
    parseAndBind(text);

    if (errorCnt > 0)
        std::cout << " [expect (typing) error] ";
    if (warnCnt > 0)
        std::cout << " [expect (typing) warning] ";

    //tree_->typeCheck();

    PSYCHE_EXPECT_INT_EQ(warnCnt, diagnosticCollector_.warningsCount());
    PSYCHE_EXPECT_INT_EQ(errorCnt, diagnosticCollector_.errorsCount());
}
