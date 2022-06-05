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

#include "C/SyntaxTree.h"
#include "C/syntax/SyntaxNodes.h"
#include "C/compilation/SemanticModel.h"




#include <type_traits>

using namespace psy;
using namespace C;

const std::string SemanticModelTester::Name = "SEMANTIC MODEL";


void SemanticModelTester::testSemanticModel()
{
    return run<SemanticModelTester>(tests_);
}

void SemanticModelTester::case0001()
{
    auto tree = SyntaxTree::parseText(SourceText("int x ;"),
                                      TextPreprocessingState::Preprocessed,
                                      ParseOptions(),
                                      "<test>");

    auto TU = tree->translationUnitRoot();
    PSY_EXPECT_TRUE(TU);

    auto decl = TU->declarations()->value;
    PSY_EXPECT_TRUE(decl);

    auto varAndOrFunDecl = decl->asVariableAndOrFunctionDeclaration();
    PSY_EXPECT_TRUE(varAndOrFunDecl);

    auto decltor = varAndOrFunDecl->declarators()->value;
    PSY_EXPECT_TRUE(decltor);

    auto identDecltor = decltor->asIdentifierDeclarator();
    PSY_EXPECT_TRUE(identDecltor);

    auto compilation = Compilation::create(tree->filePath());
    compilation->addSyntaxTrees({ tree.get() });
    auto semaModel = compilation->semanticModel(tree.get());

    auto sym = semaModel->declaredSymbol(identDecltor);
    PSY_EXPECT_TRUE(sym);

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
