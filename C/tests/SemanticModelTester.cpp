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
    tree_ = SyntaxTree::parseText(SourceText(s),
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
    auto identDecltor = decltor->asIdentifierDeclarator();

    auto sym = semaModel->declaredSymbol(identDecltor);
    PSY_EXPECT_TRUE(sym);
    PSY_EXPECT_TRUE(sym->kind() == SymbolKind::Value);
    PSY_EXPECT_TRUE(sym->asValue());
    PSY_EXPECT_TRUE(sym->asValue()->valueKind() == ValueKind::Variable);
    PSY_EXPECT_TRUE(sym->asValue()->asVariable());
    PSY_EXPECT_EQ_STR(sym->asValue()->asVariable()->name()->text(), "x");
    PSY_EXPECT_TRUE(sym->asValue()->asVariable()->type()->typeKind() == TypeKind::Named);
    PSY_EXPECT_EQ_STR(sym->asValue()->asVariable()->type()->asNamedType()->name()->text(), "int");
}

void SemanticModelTester::case0002()
{
    auto [varAndOrFunDecl, semaModel] =
            declAndSemaModel<VariableAndOrFunctionDeclarationSyntax>("int ( x ) ;");

    auto decltor = varAndOrFunDecl->declarators()->value;
    auto parenDecltor = decltor->asParenthesizedDeclarator();

    auto sym = semaModel->declaredSymbol(parenDecltor);
    PSY_EXPECT_TRUE(sym);
    PSY_EXPECT_TRUE(sym->kind() == SymbolKind::Value);
    PSY_EXPECT_TRUE(sym->asValue());
    PSY_EXPECT_TRUE(sym->asValue()->valueKind() == ValueKind::Variable);
    PSY_EXPECT_TRUE(sym->asValue()->asVariable());
    PSY_EXPECT_EQ_STR(sym->asValue()->asVariable()->name()->text(), "x");
    PSY_EXPECT_TRUE(sym->asValue()->asVariable()->type()->typeKind() == TypeKind::Named);
    PSY_EXPECT_EQ_STR(sym->asValue()->asVariable()->type()->asNamedType()->name()->text(), "int");
}

void SemanticModelTester::case0003()
{
    auto [varAndOrFunDecl, semaModel] =
            declAndSemaModel<VariableAndOrFunctionDeclarationSyntax>("int x ;");

    auto syms = semaModel->declaredSymbols(varAndOrFunDecl);
    PSY_EXPECT_EQ_INT(syms.size(), 1);

    auto sym1 = syms[0];
    PSY_EXPECT_TRUE(sym1);
    PSY_EXPECT_TRUE(sym1->kind() == SymbolKind::Value);
    PSY_EXPECT_TRUE(sym1->asValue());
    PSY_EXPECT_TRUE(sym1->asValue()->valueKind() == ValueKind::Variable);
    PSY_EXPECT_TRUE(sym1->asValue()->asVariable());
    PSY_EXPECT_EQ_STR(sym1->asValue()->name()->text(), "x");
    PSY_EXPECT_TRUE(sym1->asValue()->type()->typeKind() == TypeKind::Named);
    PSY_EXPECT_EQ_STR(sym1->asValue()->type()->asNamedType()->name()->text(), "int");
}

void SemanticModelTester::case0004()
{
    auto [varAndOrFunDecl, semaModel] =
            declAndSemaModel<VariableAndOrFunctionDeclarationSyntax>("x y , z ;");

    auto syms = semaModel->declaredSymbols(varAndOrFunDecl);
    PSY_EXPECT_EQ_INT(syms.size(), 2);

    auto sym1 = syms[0];
    PSY_EXPECT_TRUE(sym1);
    PSY_EXPECT_TRUE(sym1->kind() == SymbolKind::Value);
    PSY_EXPECT_TRUE(sym1->asValue());
    PSY_EXPECT_TRUE(sym1->asValue()->valueKind() == ValueKind::Variable);
    PSY_EXPECT_TRUE(sym1->asValue()->asVariable());
    PSY_EXPECT_EQ_STR(sym1->asValue()->name()->text(), "y");
    PSY_EXPECT_TRUE(sym1->asValue()->type()->typeKind() == TypeKind::Named);
    PSY_EXPECT_EQ_STR(sym1->asValue()->type()->asNamedType()->name()->text(), "x");

    auto sym2 = syms[1];
    PSY_EXPECT_TRUE(sym2);
    PSY_EXPECT_TRUE(sym2->kind() == SymbolKind::Value);
    PSY_EXPECT_TRUE(sym2->asValue());
    PSY_EXPECT_TRUE(sym2->asValue()->valueKind() == ValueKind::Variable);
    PSY_EXPECT_TRUE(sym2->asValue()->asVariable());
    PSY_EXPECT_EQ_STR(sym2->asValue()->name()->text(), "z");
    PSY_EXPECT_TRUE(sym2->asValue()->type()->typeKind() == TypeKind::Named);
    PSY_EXPECT_EQ_STR(sym2->asValue()->type()->asNamedType()->name()->text(), "x");
}

void SemanticModelTester::case0005(){}
void SemanticModelTester::case0006(){}
void SemanticModelTester::case0007(){}
void SemanticModelTester::case0008(){}
void SemanticModelTester::case0009(){}
void SemanticModelTester::case0010(){}

void SemanticModelTester::case0101()
{
    auto [funcDef, semaModel] =
            declAndSemaModel<FunctionDefinitionSyntax>("void x () { }");

    auto decltor = funcDef->declarator();
    auto arrOrFunDecltor = decltor->asArrayOrFunctionDeclarator();

    const Symbol* sym = semaModel->declaredSymbol(arrOrFunDecltor);
    PSY_EXPECT_TRUE(sym);
    PSY_EXPECT_TRUE(sym->kind() == SymbolKind::Function);
    PSY_EXPECT_TRUE(sym->asFunction());
    PSY_EXPECT_EQ_STR(sym->asFunction()->name()->text(), "x");
    PSY_EXPECT_TRUE(sym->asFunction()->type()->typeKind() == TypeKind::Function);
    PSY_EXPECT_TRUE(sym->asFunction()->type()->asFunctionType());
}

void SemanticModelTester::case0102()
{
    auto [funcDef, semaModel] =
            declAndSemaModel<FunctionDefinitionSyntax>("void x () { }");

    const FunctionSymbol* funcSym = semaModel->declaredSymbol(funcDef);
    PSY_EXPECT_TRUE(funcSym);
    PSY_EXPECT_TRUE(funcSym->kind() == SymbolKind::Function);
    PSY_EXPECT_TRUE(funcSym->asFunction());
    PSY_EXPECT_EQ_STR(funcSym->asFunction()->name()->text(), "x");
    PSY_EXPECT_TRUE(funcSym->asFunction()->type()->typeKind() == TypeKind::Function);
    PSY_EXPECT_TRUE(funcSym->asFunction()->type()->asFunctionType());
}

void SemanticModelTester::case0103(){}
void SemanticModelTester::case0104(){}
void SemanticModelTester::case0105(){}
void SemanticModelTester::case0106(){}
void SemanticModelTester::case0107(){}
void SemanticModelTester::case0108(){}
void SemanticModelTester::case0109(){}
void SemanticModelTester::case0110(){}

void SemanticModelTester::case0201()
{
    auto [funcDef, semaModel] =
            declAndSemaModel<FunctionDefinitionSyntax>("void x ( int y ) { }");

    auto decltor = funcDef->declarator();
    auto arrOrFunDecltor = decltor->asArrayOrFunctionDeclarator();
    auto parmDecls = arrOrFunDecltor->suffix()->asParameterSuffix()->parameters();
    auto parmDecl0 = parmDecls->value;
    auto identDecltor = parmDecl0->declarator()->asIdentifierDeclarator();

    auto sym = semaModel->declaredSymbol(identDecltor);
    PSY_EXPECT_TRUE(sym);
    PSY_EXPECT_TRUE(sym->kind() == SymbolKind::Value);
    PSY_EXPECT_TRUE(sym->asValue());
    PSY_EXPECT_TRUE(sym->asValue()->valueKind() == ValueKind::Parameter);
    PSY_EXPECT_TRUE(sym->asValue()->asParameter());
    PSY_EXPECT_EQ_STR(sym->asValue()->asParameter()->name()->text(), "y");
    PSY_EXPECT_TRUE(sym->asValue()->type()->typeKind() == TypeKind::Named);
    PSY_EXPECT_EQ_STR(sym->asValue()->type()->asNamedType()->name()->text(), "int");
}

void SemanticModelTester::case0202()
{
    auto [funcDef, semaModel] =
            declAndSemaModel<FunctionDefinitionSyntax>("void x ( int y ) { }");

    auto decltor = funcDef->declarator();
    auto arrOrFunDecltor = decltor->asArrayOrFunctionDeclarator();
    auto parmDecls = arrOrFunDecltor->suffix()->asParameterSuffix()->parameters();

    const ParameterSymbol* parmSym = semaModel->declaredSymbol(parmDecls->value);
    PSY_EXPECT_TRUE(parmSym);
    PSY_EXPECT_TRUE(parmSym->kind() == SymbolKind::Value);
    PSY_EXPECT_TRUE(parmSym->asValue());
    PSY_EXPECT_TRUE(parmSym->asValue()->valueKind() == ValueKind::Parameter);
    PSY_EXPECT_TRUE(parmSym->asValue()->asParameter());
    PSY_EXPECT_EQ_STR(parmSym->asValue()->asParameter()->name()->text(), "y");
    PSY_EXPECT_TRUE(parmSym->asValue()->type()->typeKind() == TypeKind::Named);
    PSY_EXPECT_EQ_STR(parmSym->asValue()->type()->asNamedType()->name()->text(), "int");
}

void SemanticModelTester::case0203()
{
    auto [funcDef, semaModel] =
            declAndSemaModel<FunctionDefinitionSyntax>("void x ( y z ) { }");

    auto decltor = funcDef->declarator();
    auto arrOrFunDecltor = decltor->asArrayOrFunctionDeclarator();
    auto parmDecls = arrOrFunDecltor->suffix()->asParameterSuffix()->parameters();

    const ParameterSymbol* parmSym = semaModel->declaredSymbol(parmDecls->value);
    PSY_EXPECT_TRUE(parmSym);
    PSY_EXPECT_TRUE(parmSym->kind() == SymbolKind::Value);
    PSY_EXPECT_TRUE(parmSym->asValue());
    PSY_EXPECT_TRUE(parmSym->asValue()->valueKind() == ValueKind::Parameter);
    PSY_EXPECT_TRUE(parmSym->asValue()->asParameter());
    PSY_EXPECT_EQ_STR(parmSym->asValue()->asParameter()->name()->text(), "z");
    PSY_EXPECT_TRUE(parmSym->asValue()->type()->typeKind() == TypeKind::Named);
    PSY_EXPECT_EQ_STR(parmSym->asValue()->type()->asNamedType()->name()->text(), "y");
}

void SemanticModelTester::case0204()
{
    auto [funcDef, semaModel] =
            declAndSemaModel<FunctionDefinitionSyntax>("void x ( y z , int w ) { }");

    auto decltor = funcDef->declarator();
    auto arrOrFunDecltor = decltor->asArrayOrFunctionDeclarator();
    auto parmDecls = arrOrFunDecltor->suffix()->asParameterSuffix()->parameters();

    const ParameterSymbol* parmSym0 = semaModel->declaredSymbol(parmDecls->value);
    PSY_EXPECT_TRUE(parmSym0);
    PSY_EXPECT_TRUE(parmSym0->kind() == SymbolKind::Value);
    PSY_EXPECT_TRUE(parmSym0->asValue());
    PSY_EXPECT_TRUE(parmSym0->asValue()->valueKind() == ValueKind::Parameter);
    PSY_EXPECT_TRUE(parmSym0->asValue()->asParameter());
    PSY_EXPECT_EQ_STR(parmSym0->asValue()->asParameter()->name()->text(), "z");
    PSY_EXPECT_TRUE(parmSym0->asValue()->type()->typeKind() == TypeKind::Named);
    PSY_EXPECT_EQ_STR(parmSym0->asValue()->type()->asNamedType()->name()->text(), "y");

    const ParameterSymbol* parmSym1 = semaModel->declaredSymbol(parmDecls->next->value);
    PSY_EXPECT_TRUE(parmSym1);
    PSY_EXPECT_TRUE(parmSym1->kind() == SymbolKind::Value);
    PSY_EXPECT_TRUE(parmSym1->asValue());
    PSY_EXPECT_TRUE(parmSym1->asValue()->valueKind() == ValueKind::Parameter);
    PSY_EXPECT_TRUE(parmSym1->asValue()->asParameter());
    PSY_EXPECT_EQ_STR(parmSym1->asValue()->asParameter()->name()->text(), "w");
    PSY_EXPECT_TRUE(parmSym1->asValue()->type()->typeKind() == TypeKind::Named);
    PSY_EXPECT_EQ_STR(parmSym1->asValue()->type()->asNamedType()->name()->text(), "int");
}

void SemanticModelTester::case0205(){}
void SemanticModelTester::case0206(){}
void SemanticModelTester::case0207(){}
void SemanticModelTester::case0208(){}
void SemanticModelTester::case0209(){}
void SemanticModelTester::case0210(){}
void SemanticModelTester::case0211(){}
void SemanticModelTester::case0212(){}
void SemanticModelTester::case0213(){}
void SemanticModelTester::case0214(){}
void SemanticModelTester::case0215(){}
void SemanticModelTester::case0216(){}
void SemanticModelTester::case0217(){}
void SemanticModelTester::case0218(){}
void SemanticModelTester::case0219(){}
void SemanticModelTester::case0220(){}

void SemanticModelTester::case0301(){}
void SemanticModelTester::case0302(){}
void SemanticModelTester::case0303(){}
void SemanticModelTester::case0304(){}
void SemanticModelTester::case0305(){}
void SemanticModelTester::case0306(){}
void SemanticModelTester::case0307(){}
void SemanticModelTester::case0308(){}
void SemanticModelTester::case0309(){}
void SemanticModelTester::case0310(){}
void SemanticModelTester::case0311(){}
void SemanticModelTester::case0312(){}
void SemanticModelTester::case0313(){}
void SemanticModelTester::case0314(){}
void SemanticModelTester::case0315(){}
void SemanticModelTester::case0316(){}
void SemanticModelTester::case0317(){}
void SemanticModelTester::case0318(){}
void SemanticModelTester::case0319(){}
void SemanticModelTester::case0320(){}

void SemanticModelTester::case0401(){}
void SemanticModelTester::case0402(){}
void SemanticModelTester::case0403(){}
void SemanticModelTester::case0404(){}
void SemanticModelTester::case0405(){}
void SemanticModelTester::case0406(){}
void SemanticModelTester::case0407(){}
void SemanticModelTester::case0408(){}
void SemanticModelTester::case0409(){}
void SemanticModelTester::case0410(){}
void SemanticModelTester::case0411(){}
void SemanticModelTester::case0412(){}
void SemanticModelTester::case0413(){}
void SemanticModelTester::case0414(){}
void SemanticModelTester::case0415(){}
void SemanticModelTester::case0416(){}
void SemanticModelTester::case0417(){}
void SemanticModelTester::case0418(){}
void SemanticModelTester::case0419(){}
void SemanticModelTester::case0420(){}
