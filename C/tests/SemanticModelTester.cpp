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
                                  TextCompleteness::Fragment,
                                  ParseOptions(),
                                  "<test>");

    auto TU = tree_->translationUnitRoot();
    PSY_EXPECT_TRUE(TU);
    PSY_EXPECT_TRUE(TU->declarations());

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

    const Symbol* sym = semaModel->declaredSymbol(identDecltor);
    PSY_EXPECT_TRUE(sym);
    PSY_EXPECT_EQ_ENU(sym->kind(), SymbolKind::Value, SymbolKind);
    PSY_EXPECT_EQ_ENU(sym->asValue()->valueKind(), ValueKind::Variable, ValueKind);
    const VariableSymbol* varsym = sym->asValue()->asVariable();
    PSY_EXPECT_EQ_STR(varsym->name()->text(), "x");
    PSY_EXPECT_EQ_ENU(varsym->type()->typeKind(), TypeKind::Named, TypeKind);
}

void SemanticModelTester::case0002()
{
    auto [varAndOrFunDecl, semaModel] =
            declAndSemaModel<VariableAndOrFunctionDeclarationSyntax>("int ( x ) ;");

    auto decltor = varAndOrFunDecl->declarators()->value;
    auto parenDecltor = decltor->asParenthesizedDeclarator();

    const Symbol* sym = semaModel->declaredSymbol(parenDecltor);
    PSY_EXPECT_TRUE(sym);
    PSY_EXPECT_EQ_ENU(sym->kind(), SymbolKind::Value, SymbolKind);
    PSY_EXPECT_EQ_ENU(sym->asValue()->valueKind(), ValueKind::Variable, ValueKind);
    const VariableSymbol* varsym = sym->asValue()->asVariable();
    PSY_EXPECT_EQ_STR(varsym->name()->text(), "x");
    PSY_EXPECT_EQ_ENU(varsym->type()->typeKind(), TypeKind::Named, TypeKind);
}

void SemanticModelTester::case0003()
{
    auto [varAndOrFunDecl, semaModel] =
            declAndSemaModel<VariableAndOrFunctionDeclarationSyntax>("int x ;");

    auto syms = semaModel->declaredSymbols(varAndOrFunDecl);
    PSY_EXPECT_EQ_INT(syms.size(), 1);

    const Symbol* sym0 = syms[0];
    PSY_EXPECT_TRUE(sym0);
    PSY_EXPECT_EQ_ENU(sym0->kind(), SymbolKind::Value, SymbolKind);
    PSY_EXPECT_EQ_ENU(sym0->asValue()->valueKind(), ValueKind::Variable, ValueKind);
    const VariableSymbol* varSym1 = sym0->asValue()->asVariable();
    PSY_EXPECT_EQ_STR(varSym1->name()->text(), "x");
    PSY_EXPECT_EQ_ENU(varSym1->type()->typeKind(), TypeKind::Named, TypeKind);
}

void SemanticModelTester::case0004()
{
    auto [varAndOrFunDecl, semaModel] =
            declAndSemaModel<VariableAndOrFunctionDeclarationSyntax>("x y , z ;");

    auto syms = semaModel->declaredSymbols(varAndOrFunDecl);
    PSY_EXPECT_EQ_INT(syms.size(), 2);

    const Symbol* sym1 = syms[0];
    PSY_EXPECT_TRUE(sym1);
    PSY_EXPECT_EQ_ENU(sym1->kind(), SymbolKind::Value, SymbolKind);
    PSY_EXPECT_EQ_ENU(sym1->asValue()->valueKind(), ValueKind::Variable, ValueKind);
    const VariableSymbol* varSym1 = sym1->asValue()->asVariable();
    PSY_EXPECT_EQ_STR(varSym1->name()->text(), "y");
    PSY_EXPECT_EQ_ENU(varSym1->type()->typeKind(), TypeKind::Named, TypeKind);

    const Symbol* sym2 = syms[1];
    PSY_EXPECT_TRUE(sym2);
    PSY_EXPECT_EQ_ENU(sym2->kind(), SymbolKind::Value, SymbolKind);
    PSY_EXPECT_EQ_ENU(sym2->asValue()->valueKind(), ValueKind::Variable, ValueKind);
    const VariableSymbol* varSym2 = sym2->asValue()->asVariable();
    PSY_EXPECT_EQ_STR(varSym2->name()->text(), "z");
    PSY_EXPECT_EQ_ENU(varSym2->type()->typeKind(), TypeKind::Named, TypeKind);
}

void SemanticModelTester::case0090()
{
    auto [varAndOrFunDecl, semaModel] =
            declAndSemaModel<VariableAndOrFunctionDeclarationSyntax>("int x , y ( ) ;");

    auto syms = semaModel->declaredSymbols(varAndOrFunDecl);
    PSY_EXPECT_EQ_INT(syms.size(), 2);

    const Symbol* sym0 = syms[0];
    PSY_EXPECT_TRUE(sym0);
    PSY_EXPECT_EQ_ENU(sym0->kind(), SymbolKind::Value, SymbolKind);
    PSY_EXPECT_EQ_ENU(sym0->asValue()->valueKind(), ValueKind::Variable, ValueKind);
    const VariableSymbol* varSym1 = sym0->asValue()->asVariable();
    PSY_EXPECT_EQ_STR(varSym1->name()->text(), "x");
    PSY_EXPECT_EQ_ENU(varSym1->type()->typeKind(), TypeKind::Named, TypeKind);

    const Symbol* sym1 = syms[1];
    PSY_EXPECT_TRUE(sym1);
    PSY_EXPECT_EQ_ENU(sym1->kind(), SymbolKind::Function, SymbolKind);
    const FunctionSymbol* funcSym2 = sym1->asFunction();
    PSY_EXPECT_TRUE(funcSym2);
    PSY_EXPECT_EQ_STR(funcSym2->name()->text(), "y");
    PSY_EXPECT_EQ_ENU(funcSym2->type()->typeKind(), TypeKind::Function, TypeKind);
}

void SemanticModelTester::case0091()
{
    auto [varAndOrFunDecl, semaModel] =
            declAndSemaModel<VariableAndOrFunctionDeclarationSyntax>("int x , y ( int z ) ;");

    auto syms = semaModel->declaredSymbols(varAndOrFunDecl);
    PSY_EXPECT_EQ_INT(syms.size(), 2);

    const Symbol* sym1 = syms[0];
    PSY_EXPECT_TRUE(sym1);
    PSY_EXPECT_EQ_ENU(sym1->kind(), SymbolKind::Value, SymbolKind);
    PSY_EXPECT_EQ_ENU(sym1->asValue()->valueKind(), ValueKind::Variable, ValueKind);
    const VariableSymbol* varSym1 = sym1->asValue()->asVariable();
    PSY_EXPECT_EQ_STR(varSym1->name()->text(), "x");
    PSY_EXPECT_EQ_ENU(varSym1->type()->typeKind(), TypeKind::Named, TypeKind);

    const Symbol* sym2 = syms[1];
    PSY_EXPECT_TRUE(sym2);
    PSY_EXPECT_EQ_ENU(sym2->kind(), SymbolKind::Function, SymbolKind);
    const FunctionSymbol* funcSym2 = sym2->asFunction();
    PSY_EXPECT_TRUE(funcSym2);
    PSY_EXPECT_EQ_STR(funcSym2->name()->text(), "y");
    PSY_EXPECT_EQ_ENU(funcSym2->type()->typeKind(), TypeKind::Function, TypeKind);
}

void SemanticModelTester::case0092()
{
    auto [varAndOrFunDecl, semaModel] =
            declAndSemaModel<VariableAndOrFunctionDeclarationSyntax>("x y , z ( ) ;");

    auto syms = semaModel->declaredSymbols(varAndOrFunDecl);
    PSY_EXPECT_EQ_INT(syms.size(), 2);

    const Symbol* sym0 = syms[0];
    PSY_EXPECT_TRUE(sym0);
    PSY_EXPECT_EQ_ENU(sym0->kind(), SymbolKind::Value, SymbolKind);
    PSY_EXPECT_EQ_ENU(sym0->asValue()->valueKind(), ValueKind::Variable, ValueKind);
    const VariableSymbol* varSym1 = sym0->asValue()->asVariable();
    PSY_EXPECT_EQ_STR(varSym1->name()->text(), "y");
    PSY_EXPECT_EQ_ENU(varSym1->type()->typeKind(), TypeKind::Named, TypeKind);

    const Symbol* sym1 = syms[1];
    PSY_EXPECT_TRUE(sym1);
    PSY_EXPECT_EQ_ENU(sym1->kind(), SymbolKind::Function, SymbolKind);
    const FunctionSymbol* funcSym2 = sym1->asFunction();
    PSY_EXPECT_TRUE(funcSym2);
    PSY_EXPECT_EQ_STR(funcSym2->name()->text(), "z");
    PSY_EXPECT_EQ_ENU(funcSym2->type()->typeKind(), TypeKind::Function, TypeKind);
}

void SemanticModelTester::case0093(){}
void SemanticModelTester::case0094(){}
void SemanticModelTester::case0095(){}
void SemanticModelTester::case0096(){}
void SemanticModelTester::case0097(){}
void SemanticModelTester::case0098(){}
void SemanticModelTester::case0099(){}

void SemanticModelTester::case0005(){}
void SemanticModelTester::case0006(){}
void SemanticModelTester::case0007(){}
void SemanticModelTester::case0008(){}
void SemanticModelTester::case0009(){}
void SemanticModelTester::case0010(){}

void SemanticModelTester::case0101()
{
    auto [varAndOrFunDecl, semaModel] =
            declAndSemaModel<VariableAndOrFunctionDeclarationSyntax>("void x ( ) ;");

    auto syms = semaModel->declaredSymbols(varAndOrFunDecl);
    PSY_EXPECT_EQ_INT(syms.size(), 1);

    const Symbol* sym0 = syms[0];
    PSY_EXPECT_TRUE(sym0);
    PSY_EXPECT_EQ_ENU(sym0->kind(), SymbolKind::Function, SymbolKind);
    const FunctionSymbol* funcSym = sym0->asFunction();
    PSY_EXPECT_TRUE(funcSym);
    PSY_EXPECT_EQ_STR(funcSym->name()->text(), "x");
    PSY_EXPECT_EQ_ENU(funcSym->type()->typeKind(), TypeKind::Function, TypeKind);

    auto decltor = varAndOrFunDecl->declarators()->value;
    auto arrOrFunDecltor = decltor->asArrayOrFunctionDeclarator();

    const Symbol* sym = semaModel->declaredSymbol(arrOrFunDecltor);
    PSY_EXPECT_TRUE(sym);
    PSY_EXPECT_TRUE(sym->kind() == SymbolKind::Function);
    PSY_EXPECT_EQ_PTR(sym, funcSym);
}

void SemanticModelTester::case0102()
{
}

void SemanticModelTester::case0103()
{

}

void SemanticModelTester::case0104(){}
void SemanticModelTester::case0105(){}
void SemanticModelTester::case0106(){}
void SemanticModelTester::case0107(){}
void SemanticModelTester::case0108(){}
void SemanticModelTester::case0109(){}
void SemanticModelTester::case0110(){}

void SemanticModelTester::case0150()
{
    auto [funcDef, semaModel] =
            declAndSemaModel<FunctionDefinitionSyntax>("void x ( ) { }");

    const FunctionSymbol* funcSym = semaModel->declaredSymbol(funcDef);
    PSY_EXPECT_TRUE(funcSym);
    PSY_EXPECT_EQ_STR(funcSym->name()->text(), "x");
    PSY_EXPECT_EQ_ENU(funcSym->type()->typeKind(), TypeKind::Function, TypeKind);

    auto decltor = funcDef->declarator();
    auto arrOrFunDecltor = decltor->asArrayOrFunctionDeclarator();

    const Symbol* sym = semaModel->declaredSymbol(arrOrFunDecltor);
    PSY_EXPECT_TRUE(sym);
    PSY_EXPECT_TRUE(sym->kind() == SymbolKind::Function);
    PSY_EXPECT_EQ_PTR(sym, funcSym);
}

void SemanticModelTester::case0151()
{
}

void SemanticModelTester::case0152(){}
void SemanticModelTester::case0153(){}
void SemanticModelTester::case0154(){}
void SemanticModelTester::case0155(){}
void SemanticModelTester::case0156(){}
void SemanticModelTester::case0157(){}
void SemanticModelTester::case0158(){}
void SemanticModelTester::case0159(){}


void SemanticModelTester::case0201()
{
    auto [funcDef, semaModel] =
            declAndSemaModel<FunctionDefinitionSyntax>("void x ( int y ) { }");

    auto decltor = funcDef->declarator();
    auto arrOrFunDecltor = decltor->asArrayOrFunctionDeclarator();
    auto parmDecls = arrOrFunDecltor->suffix()->asParameterSuffix()->parameters();
    auto parmDecl0 = parmDecls->value;
    auto identDecltor = parmDecl0->declarator()->asIdentifierDeclarator();

    const Symbol* sym = semaModel->declaredSymbol(identDecltor);
    PSY_EXPECT_TRUE(sym);
    PSY_EXPECT_EQ_ENU(sym->kind(), SymbolKind::Value, SymbolKind);
    PSY_EXPECT_EQ_ENU(sym->asValue()->valueKind(), ValueKind::Parameter, ValueKind);
    const ParameterSymbol* parmSym = sym->asValue()->asParameter();
    PSY_EXPECT_TRUE(parmSym);
    PSY_EXPECT_EQ_STR(parmSym->name()->text(), "y");
    PSY_EXPECT_EQ_ENU(parmSym->type()->typeKind(), TypeKind::Named, TypeKind);
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
    PSY_EXPECT_EQ_STR(parmSym->name()->text(), "y");
    PSY_EXPECT_EQ_ENU(parmSym->type()->typeKind(), TypeKind::Named, TypeKind);
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
    PSY_EXPECT_EQ_STR(parmSym->name()->text(), "z");
    PSY_EXPECT_EQ_ENU(parmSym->type()->typeKind(), TypeKind::Named, TypeKind);
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
    PSY_EXPECT_EQ_STR(parmSym0->name()->text(), "z");
    PSY_EXPECT_EQ_ENU(parmSym0->type()->typeKind(), TypeKind::Named, TypeKind);

    const ParameterSymbol* parmSym1 = semaModel->declaredSymbol(parmDecls->next->value);
    PSY_EXPECT_TRUE(parmSym1);
    PSY_EXPECT_EQ_STR(parmSym1->name()->text(), "w");
    PSY_EXPECT_EQ_ENU(parmSym1->type()->typeKind(), TypeKind::Named, TypeKind);
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

void SemanticModelTester::case0300()
{
    auto [tyDecl, semaModel] =
            declAndSemaModel<StructOrUnionDeclarationSyntax>("struct x { int _ ; } ;");

    const NamedTypeSymbol* namedTySym = semaModel->declaredSymbol(tyDecl);
    PSY_EXPECT_TRUE(namedTySym);
    PSY_EXPECT_EQ_STR(namedTySym->name()->text(), "struct x");
}

void SemanticModelTester::case0301()
{
    auto [tyDecl, semaModel] =
            declAndSemaModel<StructOrUnionDeclarationSyntax>("union x { int _ ; } ;");

    const NamedTypeSymbol* namedTySym = semaModel->declaredSymbol(tyDecl);
    PSY_EXPECT_TRUE(namedTySym);
    PSY_EXPECT_EQ_STR(namedTySym->name()->text(), "union x");
}

void SemanticModelTester::case0302()
{
    auto [varAndOrFunDecl, semaModel] =
            declAndSemaModel<VariableAndOrFunctionDeclarationSyntax>("struct x { int _ ; } y ;");

    auto syms = semaModel->declaredSymbols(varAndOrFunDecl);
    PSY_EXPECT_EQ_INT(syms.size(), 1);

    auto spec = varAndOrFunDecl->specifiers()->value->asSpecifier();
    auto tyDecl = spec->asTypeDeclarationAsSpecifier()->typeDeclaration();

    const NamedTypeSymbol* namedTySym = semaModel->declaredSymbol(tyDecl);
    PSY_EXPECT_TRUE(namedTySym);
    PSY_EXPECT_EQ_STR(namedTySym->name()->text(), "struct x");
}

void SemanticModelTester::case0303()
{
    auto [tyDecl, semaModel] =
            declAndSemaModel<StructOrUnionDeclarationSyntax>("struct x { struct y { int _ ; } z ; } ;");

    auto tySpec = tyDecl->typeSpecifier();
    auto fldDecl0 = tySpec->declarations()->value->asFieldDeclaration();
    auto nestedTyDecl = fldDecl0->specifiers()->value->asTypeDeclarationAsSpecifier()->typeDeclaration();

    const NamedTypeSymbol* namedTySym = semaModel->declaredSymbol(nestedTyDecl);
    PSY_EXPECT_TRUE(namedTySym);
    PSY_EXPECT_EQ_STR(namedTySym->name()->text(), "struct y");
}

void SemanticModelTester::case0304()
{
    auto [tyDecl, semaModel] =
            declAndSemaModel<StructOrUnionDeclarationSyntax>(R"(
struct x
{
    struct y
    {
        struct z { int _ ; } w;
    } u ;
} ;
)");

    auto tySpec = tyDecl->typeSpecifier();
    auto fldDecl0 = tySpec->declarations()->value->asFieldDeclaration();
    auto nestedTyDecl = fldDecl0->specifiers()->value->asTypeDeclarationAsSpecifier()->typeDeclaration();

    auto nestedTySpec = nestedTyDecl->typeSpecifier();
    auto nestedFldDecl0 = nestedTySpec->declarations()->value->asFieldDeclaration();
    auto nestedNestedTyDecl = nestedFldDecl0->specifiers()->value->asTypeDeclarationAsSpecifier()->typeDeclaration();

    const NamedTypeSymbol* namedTySym = semaModel->declaredSymbol(nestedNestedTyDecl);
    PSY_EXPECT_TRUE(namedTySym);
    PSY_EXPECT_EQ_STR(namedTySym->name()->text(), "struct z");

}

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
void SemanticModelTester::case0321(){}
void SemanticModelTester::case0322(){}
void SemanticModelTester::case0323(){}
void SemanticModelTester::case0324(){}
void SemanticModelTester::case0325(){}
void SemanticModelTester::case0326(){}
void SemanticModelTester::case0327(){}
void SemanticModelTester::case0328(){}
void SemanticModelTester::case0329(){}
void SemanticModelTester::case0330(){}
void SemanticModelTester::case0331(){}
void SemanticModelTester::case0332(){}
void SemanticModelTester::case0333(){}
void SemanticModelTester::case0334(){}
void SemanticModelTester::case0335(){}
void SemanticModelTester::case0336(){}
void SemanticModelTester::case0337(){}
void SemanticModelTester::case0338(){}
void SemanticModelTester::case0339(){}
void SemanticModelTester::case0340(){}
void SemanticModelTester::case0341(){}
void SemanticModelTester::case0342(){}
void SemanticModelTester::case0343(){}
void SemanticModelTester::case0344(){}
void SemanticModelTester::case0345(){}
void SemanticModelTester::case0346(){}
void SemanticModelTester::case0347(){}
void SemanticModelTester::case0348(){}
void SemanticModelTester::case0349(){}

void SemanticModelTester::case0350()
{
    auto [tyDecl, semaModel] =
            declAndSemaModel<StructOrUnionDeclarationSyntax>("struct x { int y ; } ;");

    auto tySpec = tyDecl->typeSpecifier();

    auto fldDecl0 = tySpec->declarations()->value->asFieldDeclaration();
    std::vector<const FieldSymbol*> fldSyms0 = semaModel->declaredSymbols(fldDecl0);
    PSY_EXPECT_EQ_INT(fldSyms0.size(), 1);

    const FieldSymbol* fldSym0_0 = fldSyms0[0];
    PSY_EXPECT_TRUE(fldSym0_0);
    PSY_EXPECT_EQ_STR(fldSym0_0->name()->text(), "y");
    PSY_EXPECT_EQ_ENU(fldSym0_0->type()->typeKind(), TypeKind::Named, TypeKind);
}

void SemanticModelTester::case0351()
{
    auto [tyDecl, semaModel] =
            declAndSemaModel<StructOrUnionDeclarationSyntax>("struct x { int y , z ; } ;");

    auto tySpec = tyDecl->typeSpecifier();

    auto fldDecl0 = tySpec->declarations()->value->asFieldDeclaration();
    std::vector<const FieldSymbol*> fldSyms0 = semaModel->declaredSymbols(fldDecl0);
    PSY_EXPECT_EQ_INT(fldSyms0.size(), 2);

    const FieldSymbol* fldSym0_0 = fldSyms0[0];
    PSY_EXPECT_TRUE(fldSym0_0);
    PSY_EXPECT_EQ_STR(fldSym0_0->name()->text(), "y");
    PSY_EXPECT_EQ_ENU(fldSym0_0->type()->typeKind(), TypeKind::Named, TypeKind);

    const FieldSymbol* fldSym0_1 = fldSyms0[1];
    PSY_EXPECT_TRUE(fldSym0_1);
    PSY_EXPECT_EQ_STR(fldSym0_1->name()->text(), "z");
    PSY_EXPECT_EQ_ENU(fldSym0_1->type()->typeKind(), TypeKind::Named, TypeKind);
}

void SemanticModelTester::case0352()
{
    auto [tyDecl, semaModel] =
            declAndSemaModel<StructOrUnionDeclarationSyntax>("struct x { int y ; double * z ; } ;");

    auto tySpec = tyDecl->typeSpecifier();

    auto fldDecl0 = tySpec->declarations()->value->asFieldDeclaration();
    std::vector<const FieldSymbol*> fldSyms0 = semaModel->declaredSymbols(fldDecl0);
    PSY_EXPECT_EQ_INT(fldSyms0.size(), 1);

    const FieldSymbol* fldSym0_0 = fldSyms0[0];
    PSY_EXPECT_TRUE(fldSym0_0);
    PSY_EXPECT_EQ_STR(fldSym0_0->name()->text(), "y");
    PSY_EXPECT_EQ_ENU(fldSym0_0->type()->typeKind(), TypeKind::Named, TypeKind);

    auto fldDecl1 = tySpec->declarations()->next->value->asFieldDeclaration();
    std::vector<const FieldSymbol*> fldSyms1 = semaModel->declaredSymbols(fldDecl1);
    PSY_EXPECT_EQ_INT(fldSyms1.size(), 1);

    const FieldSymbol* fldSym1_0 = fldSyms1[0];
    PSY_EXPECT_TRUE(fldSym1_0);
    PSY_EXPECT_EQ_STR(fldSym1_0->name()->text(), "z");
    PSY_EXPECT_EQ_ENU(fldSym1_0->type()->typeKind(), TypeKind::Pointer, TypeKind);
}

void SemanticModelTester::case0353()
{
    auto [tyDecl, semaModel] =
            declAndSemaModel<StructOrUnionDeclarationSyntax>("struct x { int y , z ; double * z, w ; } ;");

    auto tySpec = tyDecl->typeSpecifier();

    auto fldDecl0 = tySpec->declarations()->value->asFieldDeclaration();
    std::vector<const FieldSymbol*> fldSyms0 = semaModel->declaredSymbols(fldDecl0);
    PSY_EXPECT_EQ_INT(fldSyms0.size(), 2);

    const FieldSymbol* fldSym0_0 = fldSyms0[0];
    PSY_EXPECT_TRUE(fldSym0_0);
    PSY_EXPECT_EQ_STR(fldSym0_0->name()->text(), "y");
    PSY_EXPECT_EQ_ENU(fldSym0_0->type()->typeKind(), TypeKind::Named, TypeKind);

    const FieldSymbol* fldSym0_1 = fldSyms0[1];
    PSY_EXPECT_TRUE(fldSym0_1);
    PSY_EXPECT_EQ_STR(fldSym0_1->name()->text(), "z");
    PSY_EXPECT_EQ_ENU(fldSym0_1->type()->typeKind(), TypeKind::Named, TypeKind);

    auto fldDecl1 = tySpec->declarations()->next->value->asFieldDeclaration();
    std::vector<const FieldSymbol*> fldSyms1 = semaModel->declaredSymbols(fldDecl1);
    PSY_EXPECT_EQ_INT(fldSyms1.size(), 2);

    const FieldSymbol* fldSym1_0 = fldSyms1[0];
    PSY_EXPECT_TRUE(fldSym1_0);
    PSY_EXPECT_EQ_STR(fldSym1_0->name()->text(), "z");
    PSY_EXPECT_EQ_ENU(fldSym1_0->type()->typeKind(), TypeKind::Pointer, TypeKind);

    const FieldSymbol* fldSym1_1 = fldSyms1[1];
    PSY_EXPECT_TRUE(fldSym1_1);
    PSY_EXPECT_EQ_STR(fldSym1_1->name()->text(), "w");
    PSY_EXPECT_EQ_ENU(fldSym1_1->type()->typeKind(), TypeKind::Named, TypeKind);
}

void SemanticModelTester::case0354(){}
void SemanticModelTester::case0355(){}
void SemanticModelTester::case0356(){}
void SemanticModelTester::case0357(){}
void SemanticModelTester::case0358(){}
void SemanticModelTester::case0359(){}
void SemanticModelTester::case0360(){}
void SemanticModelTester::case0361(){}
void SemanticModelTester::case0362(){}
void SemanticModelTester::case0363(){}
void SemanticModelTester::case0364(){}
void SemanticModelTester::case0365(){}
void SemanticModelTester::case0366(){}
void SemanticModelTester::case0367(){}
void SemanticModelTester::case0368(){}
void SemanticModelTester::case0369(){}
void SemanticModelTester::case0370(){}
void SemanticModelTester::case0371(){}
void SemanticModelTester::case0372(){}
void SemanticModelTester::case0373(){}
void SemanticModelTester::case0374(){}
void SemanticModelTester::case0375(){}
void SemanticModelTester::case0376(){}
void SemanticModelTester::case0377(){}
void SemanticModelTester::case0378(){}
void SemanticModelTester::case0379(){}
void SemanticModelTester::case0380(){}
void SemanticModelTester::case0381(){}
void SemanticModelTester::case0382(){}
void SemanticModelTester::case0383(){}
void SemanticModelTester::case0384(){}
void SemanticModelTester::case0385(){}
void SemanticModelTester::case0386(){}
void SemanticModelTester::case0387(){}
void SemanticModelTester::case0388(){}
void SemanticModelTester::case0389(){}
void SemanticModelTester::case0390(){}
void SemanticModelTester::case0391(){}
void SemanticModelTester::case0392(){}
void SemanticModelTester::case0393(){}
void SemanticModelTester::case0394(){}
void SemanticModelTester::case0395(){}
void SemanticModelTester::case0396(){}
void SemanticModelTester::case0397(){}
void SemanticModelTester::case0398(){}
void SemanticModelTester::case0399(){}

void SemanticModelTester::case0400()
{
    auto [tyDecl, semaModel] =
            declAndSemaModel<EnumDeclarationSyntax>("enum x { y } ;");

    const NamedTypeSymbol* namedTySym = semaModel->declaredSymbol(tyDecl);
    PSY_EXPECT_TRUE(namedTySym);
    PSY_EXPECT_EQ_STR(namedTySym->name()->text(), "enum x");
}

void SemanticModelTester::case0401()
{
}

void SemanticModelTester::case0402()
{
}

void SemanticModelTester::case0403()
{
}

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
void SemanticModelTester::case0421(){}
void SemanticModelTester::case0422(){}
void SemanticModelTester::case0423(){}
void SemanticModelTester::case0424(){}
void SemanticModelTester::case0425(){}
void SemanticModelTester::case0426(){}
void SemanticModelTester::case0427(){}
void SemanticModelTester::case0428(){}
void SemanticModelTester::case0429(){}
void SemanticModelTester::case0430(){}
void SemanticModelTester::case0431(){}
void SemanticModelTester::case0432(){}
void SemanticModelTester::case0433(){}
void SemanticModelTester::case0434(){}
void SemanticModelTester::case0435(){}
void SemanticModelTester::case0436(){}
void SemanticModelTester::case0437(){}
void SemanticModelTester::case0438(){}
void SemanticModelTester::case0439(){}
void SemanticModelTester::case0440(){}
void SemanticModelTester::case0441(){}
void SemanticModelTester::case0442(){}
void SemanticModelTester::case0443(){}
void SemanticModelTester::case0444(){}
void SemanticModelTester::case0445(){}
void SemanticModelTester::case0446(){}
void SemanticModelTester::case0447(){}
void SemanticModelTester::case0448(){}
void SemanticModelTester::case0449(){}

void SemanticModelTester::case0450()
{
    auto [tyDecl, semaModel] =
            declAndSemaModel<EnumDeclarationSyntax>("enum x { y } ;");

    auto tySpec = tyDecl->typeSpecifier();

    auto enumtrDecl0 = tySpec->declarations()->value->asEnumeratorDeclaration();

    const EnumeratorSymbol* enumtrSym = semaModel->declaredSymbol(enumtrDecl0);
    PSY_EXPECT_TRUE(enumtrSym);
    PSY_EXPECT_EQ_STR(enumtrSym->name()->text(), "y");
    PSY_EXPECT_EQ_ENU(enumtrSym->type()->typeKind(), TypeKind::Named, TypeKind);
}

void SemanticModelTester::case0451()
{
    auto [tyDecl, semaModel] =
            declAndSemaModel<EnumDeclarationSyntax>("enum x { y = 0 } ;");

    auto tySpec = tyDecl->typeSpecifier();

    auto enumtrDecl0 = tySpec->declarations()->value->asEnumeratorDeclaration();

    const EnumeratorSymbol* enumtrSym = semaModel->declaredSymbol(enumtrDecl0);
    PSY_EXPECT_TRUE(enumtrSym);
    PSY_EXPECT_EQ_STR(enumtrSym->name()->text(), "y");
    PSY_EXPECT_EQ_ENU(enumtrSym->type()->typeKind(), TypeKind::Named, TypeKind);
}

void SemanticModelTester::case0452()
{
    auto [tyDecl, semaModel] =
            declAndSemaModel<EnumDeclarationSyntax>("enum x { y , z } ;");

    auto tySpec = tyDecl->typeSpecifier();

    auto enumtrDecl0 = tySpec->declarations()->value->asEnumeratorDeclaration();

    const EnumeratorSymbol* enumtrSym0 = semaModel->declaredSymbol(enumtrDecl0);
    PSY_EXPECT_TRUE(enumtrSym0);
    PSY_EXPECT_EQ_STR(enumtrSym0->name()->text(), "y");
    PSY_EXPECT_EQ_ENU(enumtrSym0->type()->typeKind(), TypeKind::Named, TypeKind);

    auto enumtrDecl1 = tySpec->declarations()->next->value->asEnumeratorDeclaration();

    const EnumeratorSymbol* enumtrSym1 = semaModel->declaredSymbol(enumtrDecl1);
    PSY_EXPECT_TRUE(enumtrSym1);
    PSY_EXPECT_EQ_STR(enumtrSym1->name()->text(), "z");
    PSY_EXPECT_EQ_ENU(enumtrSym1->type()->typeKind(), TypeKind::Named, TypeKind);
}

void SemanticModelTester::case0453()
{
    auto [tyDecl, semaModel] =
            declAndSemaModel<EnumDeclarationSyntax>("enum x { y = 0 , z } ;");

    auto tySpec = tyDecl->typeSpecifier();

    auto enumtrDecl0 = tySpec->declarations()->value->asEnumeratorDeclaration();

    const EnumeratorSymbol* enumtrSym0 = semaModel->declaredSymbol(enumtrDecl0);
    PSY_EXPECT_TRUE(enumtrSym0);
    PSY_EXPECT_EQ_STR(enumtrSym0->name()->text(), "y");
    PSY_EXPECT_EQ_ENU(enumtrSym0->type()->typeKind(), TypeKind::Named, TypeKind);

    auto enumtrDecl1 = tySpec->declarations()->next->value->asEnumeratorDeclaration();

    const EnumeratorSymbol* enumtrSym1 = semaModel->declaredSymbol(enumtrDecl1);
    PSY_EXPECT_TRUE(enumtrSym1);
    PSY_EXPECT_EQ_STR(enumtrSym1->name()->text(), "z");
    PSY_EXPECT_EQ_ENU(enumtrSym1->type()->typeKind(), TypeKind::Named, TypeKind);
}

void SemanticModelTester::case0454()
{
    auto [tyDecl, semaModel] =
            declAndSemaModel<EnumDeclarationSyntax>("enum x { y = 0 , z = 1 } ;");

    auto tySpec = tyDecl->typeSpecifier();

    auto enumtrDecl0 = tySpec->declarations()->value->asEnumeratorDeclaration();

    const EnumeratorSymbol* enumtrSym0 = semaModel->declaredSymbol(enumtrDecl0);
    PSY_EXPECT_TRUE(enumtrSym0);
    PSY_EXPECT_EQ_STR(enumtrSym0->name()->text(), "y");
    PSY_EXPECT_EQ_ENU(enumtrSym0->type()->typeKind(), TypeKind::Named, TypeKind);

    auto enumtrDecl1 = tySpec->declarations()->next->value->asEnumeratorDeclaration();

    const EnumeratorSymbol* enumtrSym1 = semaModel->declaredSymbol(enumtrDecl1);
    PSY_EXPECT_TRUE(enumtrSym1);
    PSY_EXPECT_EQ_STR(enumtrSym1->name()->text(), "z");
    PSY_EXPECT_EQ_ENU(enumtrSym1->type()->typeKind(), TypeKind::Named, TypeKind);
}

void SemanticModelTester::case0455()
{
    auto [tyDecl, semaModel] =
            declAndSemaModel<EnumDeclarationSyntax>("enum x { y , z = 1 } ;");

    auto tySpec = tyDecl->typeSpecifier();

    auto enumtrDecl0 = tySpec->declarations()->value->asEnumeratorDeclaration();

    const EnumeratorSymbol* enumtrSym0 = semaModel->declaredSymbol(enumtrDecl0);
    PSY_EXPECT_TRUE(enumtrSym0);
    PSY_EXPECT_EQ_STR(enumtrSym0->name()->text(), "y");
    PSY_EXPECT_EQ_ENU(enumtrSym0->type()->typeKind(), TypeKind::Named, TypeKind);

    auto enumtrDecl1 = tySpec->declarations()->next->value->asEnumeratorDeclaration();

    const EnumeratorSymbol* enumtrSym1 = semaModel->declaredSymbol(enumtrDecl1);
    PSY_EXPECT_TRUE(enumtrSym1);
    PSY_EXPECT_EQ_STR(enumtrSym1->name()->text(), "z");
    PSY_EXPECT_EQ_ENU(enumtrSym1->type()->typeKind(), TypeKind::Named, TypeKind);
}

void SemanticModelTester::case0456(){}
void SemanticModelTester::case0457(){}
void SemanticModelTester::case0458(){}
void SemanticModelTester::case0459(){}
void SemanticModelTester::case0460(){}
void SemanticModelTester::case0461(){}
void SemanticModelTester::case0462(){}
void SemanticModelTester::case0463(){}
void SemanticModelTester::case0464(){}
void SemanticModelTester::case0465(){}
void SemanticModelTester::case0466(){}
void SemanticModelTester::case0467(){}
void SemanticModelTester::case0468(){}
void SemanticModelTester::case0469(){}
void SemanticModelTester::case0470(){}
void SemanticModelTester::case0471(){}
void SemanticModelTester::case0472(){}
void SemanticModelTester::case0473(){}
void SemanticModelTester::case0474(){}
void SemanticModelTester::case0475(){}
void SemanticModelTester::case0476(){}
void SemanticModelTester::case0477(){}
void SemanticModelTester::case0478(){}
void SemanticModelTester::case0479(){}
void SemanticModelTester::case0480(){}
void SemanticModelTester::case0481(){}
void SemanticModelTester::case0482(){}
void SemanticModelTester::case0483(){}
void SemanticModelTester::case0484(){}
void SemanticModelTester::case0485(){}
void SemanticModelTester::case0486(){}
void SemanticModelTester::case0487(){}
void SemanticModelTester::case0488(){}
void SemanticModelTester::case0489(){}
void SemanticModelTester::case0490(){}
void SemanticModelTester::case0491(){}
void SemanticModelTester::case0492(){}
void SemanticModelTester::case0493(){}
void SemanticModelTester::case0494(){}
void SemanticModelTester::case0495(){}
void SemanticModelTester::case0496(){}
void SemanticModelTester::case0497(){}
void SemanticModelTester::case0498(){}
void SemanticModelTester::case0499(){}

void SemanticModelTester::case0500()
{
    auto s = "int x ; ";

    tree_ = SyntaxTree::parseText(SourceText(s),
                                  TextPreprocessingState::Preprocessed,
                                  TextCompleteness::Fragment,
                                  ParseOptions(),
                                  "<test>");

    auto TU = tree_->translationUnitRoot();
    PSY_EXPECT_TRUE(TU);

    compilation_ = Compilation::create(tree_->filePath());
    compilation_->addSyntaxTrees({ tree_.get() });
    auto semaModel = compilation_->semanticModel(tree_.get());
    PSY_EXPECT_TRUE(semaModel);

    const LibrarySymbol* libSym = semaModel->declaredSymbol(TU);
    PSY_EXPECT_TRUE(libSym);

}
void SemanticModelTester::case0501(){}
void SemanticModelTester::case0502(){}
void SemanticModelTester::case0503(){}
void SemanticModelTester::case0504(){}
void SemanticModelTester::case0505(){}
void SemanticModelTester::case0506(){}
void SemanticModelTester::case0507(){}
void SemanticModelTester::case0508(){}
void SemanticModelTester::case0509(){}
