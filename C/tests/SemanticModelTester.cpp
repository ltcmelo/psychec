// Copyright (c) 2022 Leandro T. C. Melo <ltcmelo@gmail.com>
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

#include "SemanticModelTester.h"

#include "TestSuite_API.h"

#include "C/symbols/Symbol_ALL.h"

using namespace psy;
using namespace C;

const std::string SemanticModelTester::Name = "SEMANTIC MODEL";


APITestSuite* SemanticModelTester::suite()
{
    return static_cast<APITestSuite*>(suite_);
}

void SemanticModelTester::setUp()
{
}

void SemanticModelTester::tearDown()
{
    compilation_.reset(nullptr);
    tree_.reset(nullptr);
}

template <class DeclT>
std::tuple<const DeclT*, const SemanticModel*>
SemanticModelTester::declAndSemaModel(const std::string& s)
{
    tree_ = SyntaxTree::parseText(SourceText("int x ;"),
                                  TextPreprocessingState::Preprocessed,
                                  ParseOptions(),
                                  "<test>");

    auto TU = tree_->translationUnitRoot();
    PSY_EXPECT_TRUE(TU);

    auto anyDecl = TU->declarations()->value;
    PSY_EXPECT_TRUE(anyDecl);
    PSY_EXPECT_TRUE(anyDecl->asDeclaration());

    auto decl = dynamic_cast<DeclT*>(anyDecl);
    PSY_EXPECT_TRUE(decl);

    compilation_ = Compilation::create(tree_->filePath());
    compilation_->addSyntaxTrees({ tree_.get() });
    auto semaModel = compilation_->semanticModel(tree_.get());
    PSY_EXPECT_TRUE(semaModel);

    return std::make_tuple(decl, semaModel);
}

void SemanticModelTester::testSemanticModel()
{
    return run<SemanticModelTester>(tests_);
}

void SemanticModelTester::case0001()
{
    auto [varAndOrFunDecl, semaModel] =
            declAndSemaModel<VariableAndOrFunctionDeclarationSyntax>("int x ;");

    auto decltor = varAndOrFunDecl->declarators()->value;
    PSY_EXPECT_TRUE(decltor);

    auto identDecltor = decltor->asIdentifierDeclarator();
    PSY_EXPECT_TRUE(identDecltor);

    auto sym = semaModel->declaredSymbol(identDecltor);
    PSY_EXPECT_TRUE(sym);
    PSY_EXPECT_TRUE(sym->kind() == SymbolKind::Value);
    PSY_EXPECT_TRUE(sym->asValue()->valueKind() == ValueKind::Variable);
    PSY_EXPECT_TRUE(sym->asValue()->asVariable());
}

void SemanticModelTester::case0002(){}
void SemanticModelTester::case0003(){}
void SemanticModelTester::case0004(){}
void SemanticModelTester::case0005(){}
void SemanticModelTester::case0006(){}
void SemanticModelTester::case0007(){}
void SemanticModelTester::case0008(){}
void SemanticModelTester::case0009(){}
void SemanticModelTester::case0010(){}
