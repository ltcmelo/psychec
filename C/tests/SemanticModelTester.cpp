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

#include "C/parser/Unparser.h"
#include "C/symbols/Symbol_ALL.h"
#include "C/syntax/Lexeme_ALL.h"
#include "C/syntax/SyntaxVisitor__MACROS__.inc"
#include "C/types/Type_ALL.h"

#include <algorithm>
#include <sstream>
#include <unordered_map>

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
}

class ExpressionCollector : public SyntaxVisitor
{
public:
    ExpressionCollector(const SyntaxTree* tree)
        : SyntaxVisitor(tree)
    {}

    std::unordered_map<std::string, const ExpressionSyntax*> m;

    Action visitExpression(const ExpressionSyntax* node)
    {
        Unparser unparser(this->tree_);
        std::ostringstream oss;
        unparser.unparse(node, oss);
        auto s = oss.str();
        s.erase(std::find_if(s.rbegin(),
                             s.rend(),
                             [] (int c) { return !std::isspace(c); }).base(),
                s.end());
        //std::cout << "[["<< s << "]]\n";
        m[s] = node;
        return Action::Visit;
    }

    virtual Action visitIdentifierName(const IdentifierNameSyntax* node) {  return visitExpression(node); }
    virtual Action visitPredefinedName(const PredefinedNameSyntax* node) {  return visitExpression(node); }
    virtual Action visitConstantExpression(const ConstantExpressionSyntax* node) {  return visitExpression(node); }
    virtual Action visitStringLiteralExpression(const StringLiteralExpressionSyntax* node) {  return visitExpression(node); }
    virtual Action visitParenthesizedExpression(const ParenthesizedExpressionSyntax* node) {  return visitExpression(node); }
    virtual Action visitGenericSelectionExpression(const GenericSelectionExpressionSyntax* node) {  return visitExpression(node); }
    //virtual Action visitGenericAssociation(const GenericAssociationSyntax* node) {  return visitExpression(node); }
    virtual Action visitExtGNU_EnclosedCompoundStatementExpression(const ExtGNU_EnclosedCompoundStatementExpressionSyntax* node) {  return visitExpression(node); }
    virtual Action visitExtGNU_ComplexValuedExpression(const ExtGNU_ComplexValuedExpressionSyntax* node) {  return visitExpression(node); }

    /* Operations */
    virtual Action visitPrefixUnaryExpression(const PrefixUnaryExpressionSyntax* node) {  return visitExpression(node); }
    virtual Action visitPostfixUnaryExpression(const PostfixUnaryExpressionSyntax* node) {  return visitExpression(node); }
    virtual Action visitMemberAccessExpression(const MemberAccessExpressionSyntax* node) {  return visitExpression(node); }
    virtual Action visitArraySubscriptExpression(const ArraySubscriptExpressionSyntax* node) {  return visitExpression(node); }
    virtual Action visitTypeTraitExpression(const TypeTraitExpressionSyntax* node) {  return visitExpression(node); }
    virtual Action visitCastExpression(const CastExpressionSyntax* node) {  return visitExpression(node); }
    virtual Action visitCallExpression(const CallExpressionSyntax* node) {  return visitExpression(node); }
    virtual Action visitVAArgumentExpression(const VAArgumentExpressionSyntax* node) {  return visitExpression(node); }
    virtual Action visitOffsetOfExpression(const OffsetOfExpressionSyntax* node) {  return visitExpression(node); }
    virtual Action visitCompoundLiteralExpression(const CompoundLiteralExpressionSyntax* node) {  return visitExpression(node); }
    virtual Action visitBinaryExpression(const BinaryExpressionSyntax* node) {  return visitExpression(node); }
    virtual Action visitConditionalExpression(const ConditionalExpressionSyntax* node) {  return visitExpression(node); }
    virtual Action visitAssignmentExpression(const AssignmentExpressionSyntax* node) {  return visitExpression(node); }
    virtual Action visitSequencingExpression(const SequencingExpressionSyntax* node) {  return visitExpression(node); }
    virtual Action visitExtGNU_ChooseExpression(const ExtGNU_ChooseExpressionSyntax* node) {  return visitExpression(node); }
};

template <class DeclNodeT>
std::tuple<const DeclNodeT*, const SemanticModel*>
SemanticModelTester::compileTestSymbols(
        const std::string& srcText,
        const std::string& tydefDeclSrcText)
{
    auto tree = SyntaxTree::parseText(SourceText(tydefDeclSrcText.empty()
                                                  ? srcText
                                                  : tydefDeclSrcText + srcText),
                                       TextPreprocessingState::Preprocessed,
                                       TextCompleteness::Fragment,
                                       ParseOptions(),
                                       "<test>");

    auto TU = tree->translationUnit();
    PSY_EXPECT_TRUE(TU);
    PSY_EXPECT_TRUE(TU->declarations());

    DeclarationSyntax* anyDeclNode = nullptr;
    if (tydefDeclSrcText.empty())
        anyDeclNode = TU->declarations()->value;
    else
        anyDeclNode = TU->declarations()->next->value;
    PSY_EXPECT_TRUE(anyDeclNode);
    PSY_EXPECT_TRUE(anyDeclNode->asDeclaration());

    auto decl = dynamic_cast<DeclNodeT*>(anyDeclNode);
    PSY_EXPECT_TRUE(decl);

    compilation_ = Compilation::create(tree->filePath());

    auto tree_RAW = tree.get();
    compilation_->addSyntaxTree(std::move(tree));
    compilation_->computeSemanticModel(tree_RAW);
    auto semaModel = compilation_->semanticModel(tree_RAW);
    PSY_EXPECT_TRUE(semaModel);

    return std::make_tuple(decl, semaModel);
}

std::tuple<std::unordered_map<std::string, const ExpressionSyntax*>,
           const SemanticModel*>
SemanticModelTester::compileTestTypes(const std::string& srcText)
{
    auto tree = SyntaxTree::parseText(SourceText(srcText),
                                      TextPreprocessingState::Preprocessed,
                                      TextCompleteness::Fragment,
                                      ParseOptions(),
                                      "<test>");

    ExpressionCollector v(tree.get());
    v.visit(tree->translationUnit());

    compilation_ = Compilation::create(tree->filePath());

    auto tree_RAW = tree.get();
    compilation_->addSyntaxTree(std::move(tree));
    compilation_->computeSemanticModel(tree_RAW);
    auto semaModel = compilation_->semanticModel(tree_RAW);
    PSY_EXPECT_TRUE(semaModel);

    return std::make_tuple(v.m, semaModel);
}

void SemanticModelTester::testSemanticModel()
{
    return run<SemanticModelTester>(tests_);
}

void SemanticModelTester::case0001()
{
    auto [varAndOrFunDeclNode, semaModel] =
            compileTestSymbols<VariableAndOrFunctionDeclarationSyntax>("int x ;");

    auto decltor = varAndOrFunDeclNode->declarators()->value;
    auto identDecltor = decltor->asIdentifierDeclarator();
    const DeclarationSymbol* declSym = semaModel->declarationBy(identDecltor);
    PSY_EXPECT_TRUE(declSym);
    PSY_EXPECT_EQ_ENU(declSym->category(), DeclarationCategory::Object, DeclarationCategory);
    PSY_EXPECT_EQ_ENU(declSym->kind(), SymbolKind::VariableDeclaration, SymbolKind);
    const VariableDeclarationSymbol* varDeclSym = declSym->asVariableDeclaration();
    PSY_EXPECT_EQ_STR(varDeclSym->name()->valueText(), "x");
    PSY_EXPECT_EQ_ENU(varDeclSym->type()->kind(), TypeKind::Basic, TypeKind);
    PSY_EXPECT_EQ_ENU(varDeclSym->type()->asBasicType()->kind(), BasicTypeKind::Int_S, BasicTypeKind);
}

void SemanticModelTester::case0002()
{
    auto [varAndOrFunDeclNode, semaModel] =
            compileTestSymbols<VariableAndOrFunctionDeclarationSyntax>("int ( x ) ;");

    auto decltor = varAndOrFunDeclNode->declarators()->value;
    auto parenDecltor = decltor->asParenthesizedDeclarator();
    const DeclarationSymbol* declSym = semaModel->declarationBy(parenDecltor);
    PSY_EXPECT_TRUE(declSym);
    PSY_EXPECT_EQ_ENU(declSym->category(), DeclarationCategory::Object, DeclarationCategory);
    PSY_EXPECT_EQ_ENU(declSym->kind(), SymbolKind::VariableDeclaration, SymbolKind);
    const VariableDeclarationSymbol* varDeclSym = declSym->asVariableDeclaration();
    PSY_EXPECT_EQ_STR(varDeclSym->name()->valueText(), "x");
    PSY_EXPECT_EQ_ENU(varDeclSym->type()->kind(), TypeKind::Basic, TypeKind);
    PSY_EXPECT_EQ_ENU(varDeclSym->type()->asBasicType()->kind(), BasicTypeKind::Int_S, BasicTypeKind);
}

void SemanticModelTester::case0003()
{
    auto [varAndOrFunDeclNode, semaModel] =
            compileTestSymbols<VariableAndOrFunctionDeclarationSyntax>("int x ;");

    auto syms = semaModel->variablesAndOrFunctionsFor(varAndOrFunDeclNode);
    PSY_EXPECT_EQ_INT(syms.size(), 1);

    const DeclarationSymbol* declSym = syms[0];
    PSY_EXPECT_TRUE(declSym);
    PSY_EXPECT_EQ_ENU(declSym->category(), DeclarationCategory::Object, DeclarationCategory);
    PSY_EXPECT_EQ_ENU(declSym->kind(), SymbolKind::VariableDeclaration, SymbolKind);
    const VariableDeclarationSymbol* varDeclSym = declSym->asVariableDeclaration();
    PSY_EXPECT_EQ_STR(varDeclSym->name()->valueText(), "x");
    PSY_EXPECT_EQ_ENU(varDeclSym->type()->kind(), TypeKind::Basic, TypeKind);
    PSY_EXPECT_EQ_ENU(varDeclSym->type()->asBasicType()->kind(), BasicTypeKind::Int_S, BasicTypeKind);
}

void SemanticModelTester::case0004()
{
    auto [varAndOrFunDeclNode, semaModel] =
            compileTestSymbols<VariableAndOrFunctionDeclarationSyntax>("x y , z ;", "typedef int x ;");

    auto syms = semaModel->variablesAndOrFunctionsFor(varAndOrFunDeclNode);
    PSY_EXPECT_EQ_INT(syms.size(), 2);

    const DeclarationSymbol* declSym0 = syms[0];
    PSY_EXPECT_TRUE(declSym0);
    PSY_EXPECT_EQ_ENU(declSym0->category(), DeclarationCategory::Object, DeclarationCategory);
    PSY_EXPECT_EQ_ENU(declSym0->kind(), SymbolKind::VariableDeclaration, SymbolKind);
    const VariableDeclarationSymbol* varDeclSym1 = declSym0->asVariableDeclaration();
    PSY_EXPECT_EQ_STR(varDeclSym1->name()->valueText(), "y");
    PSY_EXPECT_EQ_ENU(varDeclSym1->type()->kind(), TypeKind::TypedefName, TypeKind);

    const DeclarationSymbol* declSym1 = syms[1];
    PSY_EXPECT_TRUE(declSym1);
    PSY_EXPECT_EQ_ENU(declSym1->category(), DeclarationCategory::Object, DeclarationCategory);
    PSY_EXPECT_EQ_ENU(declSym1->kind(), SymbolKind::VariableDeclaration, SymbolKind);
    const VariableDeclarationSymbol* varDeclSym2 = declSym1->asVariableDeclaration();
    PSY_EXPECT_EQ_STR(varDeclSym2->name()->valueText(), "z");
    PSY_EXPECT_EQ_ENU(varDeclSym2->type()->kind(), TypeKind::TypedefName, TypeKind);
}

void SemanticModelTester::case0005()
{
    auto [varAndOrFunDeclNode, semaModel] =
            compileTestSymbols<VariableAndOrFunctionDeclarationSyntax>("x y , z ;");

    auto syms = semaModel->variablesAndOrFunctionsFor(varAndOrFunDeclNode);
    PSY_EXPECT_EQ_INT(syms.size(), 2);

    const DeclarationSymbol* declSym0 = syms[0];
    PSY_EXPECT_TRUE(declSym0);
    PSY_EXPECT_EQ_ENU(declSym0->category(), DeclarationCategory::Object, DeclarationCategory);
    PSY_EXPECT_EQ_ENU(declSym0->kind(), SymbolKind::VariableDeclaration, SymbolKind);
    const VariableDeclarationSymbol* varDeclSym1 = declSym0->asVariableDeclaration();
    PSY_EXPECT_EQ_STR(varDeclSym1->name()->valueText(), "y");
    PSY_EXPECT_EQ_ENU(varDeclSym1->type()->kind(), TypeKind::Error, TypeKind);

    const DeclarationSymbol* declSym1 = syms[1];
    PSY_EXPECT_TRUE(declSym1);
    PSY_EXPECT_EQ_ENU(declSym1->category(), DeclarationCategory::Object, DeclarationCategory);
    PSY_EXPECT_EQ_ENU(declSym1->kind(), SymbolKind::VariableDeclaration, SymbolKind);
    const VariableDeclarationSymbol* varDeclSym2 = declSym1->asVariableDeclaration();
    PSY_EXPECT_EQ_STR(varDeclSym2->name()->valueText(), "z");
    PSY_EXPECT_EQ_ENU(varDeclSym2->type()->kind(), TypeKind::Error, TypeKind);
}

void SemanticModelTester::case0006(){}
void SemanticModelTester::case0007(){}
void SemanticModelTester::case0008(){}
void SemanticModelTester::case0009(){}
void SemanticModelTester::case0010(){}

void SemanticModelTester::case0090()
{
    auto [varAndOrFunDeclNode, semaModel] =
            compileTestSymbols<VariableAndOrFunctionDeclarationSyntax>("int x , y ( ) ;");

    auto syms = semaModel->variablesAndOrFunctionsFor(varAndOrFunDeclNode);
    PSY_EXPECT_EQ_INT(syms.size(), 2);

    const DeclarationSymbol* declSym0 = syms[0];
    PSY_EXPECT_TRUE(declSym0);
    PSY_EXPECT_EQ_ENU(declSym0->category(), DeclarationCategory::Object, DeclarationCategory);
    PSY_EXPECT_EQ_ENU(declSym0->kind(), SymbolKind::VariableDeclaration, SymbolKind);
    const VariableDeclarationSymbol* varDeclSym0 = declSym0->asVariableDeclaration();
    PSY_EXPECT_EQ_STR(varDeclSym0->name()->valueText(), "x");
    PSY_EXPECT_EQ_ENU(varDeclSym0->type()->kind(), TypeKind::Basic, TypeKind);
    PSY_EXPECT_EQ_ENU(varDeclSym0->type()->asBasicType()->kind(), BasicTypeKind::Int_S, BasicTypeKind);

    const DeclarationSymbol* declSym1 = syms[1];
    PSY_EXPECT_TRUE(declSym1);
    PSY_EXPECT_EQ_ENU(declSym1->category(), DeclarationCategory::Function, DeclarationCategory);
    PSY_EXPECT_EQ_ENU(declSym1->kind(), SymbolKind::FunctionDeclaration, SymbolKind);
    const FunctionDeclarationSymbol* funcDeclSym1 = declSym1->asFunctionDeclaration();
    PSY_EXPECT_TRUE(funcDeclSym1);
    PSY_EXPECT_EQ_STR(funcDeclSym1->name()->valueText(), "y");
    PSY_EXPECT_EQ_ENU(funcDeclSym1->type()->kind(), TypeKind::Function, TypeKind);
}

void SemanticModelTester::case0091()
{
    auto [varAndOrFunDeclNode, semaModel] =
            compileTestSymbols<VariableAndOrFunctionDeclarationSyntax>("int x , y ( int z ) ;");

    auto syms = semaModel->variablesAndOrFunctionsFor(varAndOrFunDeclNode);
    PSY_EXPECT_EQ_INT(syms.size(), 2);

    const DeclarationSymbol* declSym0 = syms[0];
    PSY_EXPECT_TRUE(declSym0);
    PSY_EXPECT_EQ_ENU(declSym0->category(), DeclarationCategory::Object, DeclarationCategory);
    PSY_EXPECT_EQ_ENU(declSym0->kind(), SymbolKind::VariableDeclaration, SymbolKind);
    const VariableDeclarationSymbol* varDeclSym0 = declSym0->asVariableDeclaration();
    PSY_EXPECT_EQ_STR(varDeclSym0->name()->valueText(), "x");
    PSY_EXPECT_EQ_ENU(varDeclSym0->type()->kind(), TypeKind::Basic, TypeKind);
    PSY_EXPECT_EQ_ENU(varDeclSym0->type()->asBasicType()->kind(), BasicTypeKind::Int_S, BasicTypeKind);

    const DeclarationSymbol* declSym1 = syms[1];
    PSY_EXPECT_TRUE(declSym1);
    PSY_EXPECT_EQ_ENU(declSym1->category(), DeclarationCategory::Function, DeclarationCategory);
    PSY_EXPECT_EQ_ENU(declSym1->kind(), SymbolKind::FunctionDeclaration, SymbolKind);
    const FunctionDeclarationSymbol* funcDeclSym1 = declSym1->asFunctionDeclaration();
    PSY_EXPECT_TRUE(funcDeclSym1);
    PSY_EXPECT_EQ_STR(funcDeclSym1->name()->valueText(), "y");
    PSY_EXPECT_EQ_ENU(funcDeclSym1->type()->kind(), TypeKind::Function, TypeKind);
}

void SemanticModelTester::case0092()
{
    auto [varAndOrFunDeclNode, semaModel] =
            compileTestSymbols<VariableAndOrFunctionDeclarationSyntax>("x y , z ( ) ;", "typedef int x ;");

    auto syms = semaModel->variablesAndOrFunctionsFor(varAndOrFunDeclNode);
    PSY_EXPECT_EQ_INT(syms.size(), 2);

    const DeclarationSymbol* declSym0 = syms[0];
    PSY_EXPECT_TRUE(declSym0);
    PSY_EXPECT_EQ_ENU(declSym0->category(), DeclarationCategory::Object, DeclarationCategory);
    PSY_EXPECT_EQ_ENU(declSym0->kind(), SymbolKind::VariableDeclaration, SymbolKind);
    const VariableDeclarationSymbol* varDeclSym0 = declSym0->asVariableDeclaration();
    PSY_EXPECT_EQ_STR(varDeclSym0->name()->valueText(), "y");
    PSY_EXPECT_EQ_ENU(varDeclSym0->type()->kind(), TypeKind::TypedefName, TypeKind);

    const DeclarationSymbol* declSym1 = syms[1];
    PSY_EXPECT_TRUE(declSym1);
    PSY_EXPECT_EQ_ENU(declSym1->category(), DeclarationCategory::Function, DeclarationCategory);
    PSY_EXPECT_EQ_ENU(declSym1->kind(), SymbolKind::FunctionDeclaration, SymbolKind);
    const FunctionDeclarationSymbol* funcDeclSym1 = declSym1->asFunctionDeclaration();
    PSY_EXPECT_TRUE(funcDeclSym1);
    PSY_EXPECT_EQ_STR(funcDeclSym1->name()->valueText(), "z");
    PSY_EXPECT_EQ_ENU(funcDeclSym1->type()->kind(), TypeKind::Function, TypeKind);
}

void SemanticModelTester::case0093(){}
void SemanticModelTester::case0094(){}
void SemanticModelTester::case0095(){}
void SemanticModelTester::case0096(){}
void SemanticModelTester::case0097(){}
void SemanticModelTester::case0098(){}
void SemanticModelTester::case0099(){}

void SemanticModelTester::case0101()
{
    auto [varAndOrFunDeclNode, semaModel] =
            compileTestSymbols<VariableAndOrFunctionDeclarationSyntax>("void x ( ) ;");

    auto syms = semaModel->variablesAndOrFunctionsFor(varAndOrFunDeclNode);
    PSY_EXPECT_EQ_INT(syms.size(), 1);
    const DeclarationSymbol* declSym0 = syms[0];
    PSY_EXPECT_TRUE(declSym0);
    PSY_EXPECT_EQ_ENU(declSym0->category(), DeclarationCategory::Function, DeclarationCategory);
    PSY_EXPECT_EQ_ENU(declSym0->kind(), SymbolKind::FunctionDeclaration, SymbolKind);
    const FunctionDeclarationSymbol* funcDecl = declSym0->asFunctionDeclaration();
    PSY_EXPECT_TRUE(funcDecl);
    PSY_EXPECT_EQ_STR(funcDecl->name()->valueText(), "x");
    PSY_EXPECT_EQ_ENU(funcDecl->type()->kind(), TypeKind::Function, TypeKind);

    auto decltor = varAndOrFunDeclNode->declarators()->value;
    auto arrOrFunDecltor = decltor->asArrayOrFunctionDeclarator();
    const DeclarationSymbol* declSym = semaModel->declarationBy(arrOrFunDecltor);
    PSY_EXPECT_TRUE(declSym);
    PSY_EXPECT_TRUE(declSym->category() == DeclarationCategory::Function);
    PSY_EXPECT_TRUE(declSym->kind() == SymbolKind::FunctionDeclaration);
    PSY_EXPECT_EQ_PTR(declSym, funcDecl);
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
    auto [funcDefNode, semaModel] =
            compileTestSymbols<FunctionDefinitionSyntax>("void x ( ) { }");

    const FunctionDeclarationSymbol* funcDeclSym = semaModel->functionFor(funcDefNode);
    PSY_EXPECT_TRUE(funcDeclSym);
    PSY_EXPECT_EQ_STR(funcDeclSym->name()->valueText(), "x");
    PSY_EXPECT_EQ_ENU(funcDeclSym->type()->kind(), TypeKind::Function, TypeKind);

    auto decltor = funcDefNode->declarator();
    auto arrOrFunDecltor = decltor->asArrayOrFunctionDeclarator();
    const DeclarationSymbol* declSym = semaModel->declarationBy(arrOrFunDecltor);
    PSY_EXPECT_TRUE(declSym);
    PSY_EXPECT_TRUE(declSym->category() == DeclarationCategory::Function);
    PSY_EXPECT_TRUE(declSym->kind() == SymbolKind::FunctionDeclaration);
    PSY_EXPECT_EQ_PTR(declSym, funcDeclSym);
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
    auto [funcDefNode, semaModel] =
            compileTestSymbols<FunctionDefinitionSyntax>("void x ( int y ) { }");

    auto decltor = funcDefNode->declarator();
    auto arrOrFunDecltor = decltor->asArrayOrFunctionDeclarator();
    auto parmDecls = arrOrFunDecltor->suffix()->asParameterSuffix()->parameters();
    auto parmDecl0 = parmDecls->value;
    auto identDecltor = parmDecl0->declarator()->asIdentifierDeclarator();

    const DeclarationSymbol* declSym = semaModel->declarationBy(identDecltor);
    PSY_EXPECT_TRUE(declSym);
    PSY_EXPECT_EQ_ENU(declSym->category(), DeclarationCategory::Object, DeclarationCategory);
    PSY_EXPECT_EQ_ENU(declSym->kind(), SymbolKind::ParameterDeclaration, SymbolKind);
    const ParameterDeclarationSymbol* parmDeclSym = declSym->asParameterDeclaration();
    PSY_EXPECT_TRUE(parmDeclSym);
    PSY_EXPECT_EQ_STR(parmDeclSym->name()->valueText(), "y");
    PSY_EXPECT_EQ_ENU(parmDeclSym->type()->kind(), TypeKind::Basic, TypeKind);
    PSY_EXPECT_EQ_ENU(parmDeclSym->type()->asBasicType()->kind(), BasicTypeKind::Int_S, BasicTypeKind);
}

void SemanticModelTester::case0202()
{
    auto [funcDefNode, semaModel] =
            compileTestSymbols<FunctionDefinitionSyntax>("void x ( int y ) { }");

    auto decltor = funcDefNode->declarator();
    auto arrOrFunDecltor = decltor->asArrayOrFunctionDeclarator();
    auto parmDecls = arrOrFunDecltor->suffix()->asParameterSuffix()->parameters();

    const ParameterDeclarationSymbol* parmDeclSym = semaModel->parameterFor(parmDecls->value);
    PSY_EXPECT_TRUE(parmDeclSym);
    PSY_EXPECT_EQ_STR(parmDeclSym->name()->valueText(), "y");
    PSY_EXPECT_EQ_ENU(parmDeclSym->type()->kind(), TypeKind::Basic, TypeKind);
    PSY_EXPECT_EQ_ENU(parmDeclSym->type()->asBasicType()->kind(), BasicTypeKind::Int_S, BasicTypeKind);
}

void SemanticModelTester::case0203()
{
    auto [funcDefNode, semaModel] =
            compileTestSymbols<FunctionDefinitionSyntax>("void x ( y z ) { }");

    auto decltor = funcDefNode->declarator();
    auto arrOrFunDecltor = decltor->asArrayOrFunctionDeclarator();
    auto parmDecls = arrOrFunDecltor->suffix()->asParameterSuffix()->parameters();

    const ParameterDeclarationSymbol* parmDeclSym = semaModel->parameterFor(parmDecls->value);
    PSY_EXPECT_TRUE(parmDeclSym);
    PSY_EXPECT_EQ_STR(parmDeclSym->name()->valueText(), "z");
    PSY_EXPECT_EQ_ENU(parmDeclSym->type()->kind(), TypeKind::TypedefName, TypeKind);
}

void SemanticModelTester::case0204()
{
    auto [funcDefNode, semaModel] =
            compileTestSymbols<FunctionDefinitionSyntax>("void x ( y z , int w ) { }");

    auto decltor = funcDefNode->declarator();
    auto arrOrFunDecltor = decltor->asArrayOrFunctionDeclarator();
    auto parmDecls = arrOrFunDecltor->suffix()->asParameterSuffix()->parameters();

    const ParameterDeclarationSymbol* parmDeclSym0 = semaModel->parameterFor(parmDecls->value);
    PSY_EXPECT_TRUE(parmDeclSym0);
    PSY_EXPECT_EQ_STR(parmDeclSym0->name()->valueText(), "z");
    PSY_EXPECT_EQ_ENU(parmDeclSym0->type()->kind(), TypeKind::TypedefName, TypeKind);

    const ParameterDeclarationSymbol* parmDeclSym1 = semaModel->parameterFor(parmDecls->next->value);
    PSY_EXPECT_TRUE(parmDeclSym1);
    PSY_EXPECT_EQ_STR(parmDeclSym1->name()->valueText(), "w");
    PSY_EXPECT_EQ_ENU(parmDeclSym1->type()->kind(), TypeKind::Basic, TypeKind);
    PSY_EXPECT_EQ_ENU(parmDeclSym1->type()->asBasicType()->kind(), BasicTypeKind::Int_S, BasicTypeKind);
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
    auto [tyDeclNode, semaModel] =
            compileTestSymbols<StructOrUnionDeclarationSyntax>("struct x { int _ ; } ;");

    PSY_EXPECT_EQ_ENU(tyDeclNode->kind(), SyntaxKind::StructDeclaration, SyntaxKind);
    const StructDeclarationSymbol* strukt = semaModel->structFor(tyDeclNode->asStructOrUnionDeclaration());
    PSY_EXPECT_TRUE(strukt);
    PSY_EXPECT_EQ_STR(strukt->introducedNewType()->tag()->valueText(), "x");
}

void SemanticModelTester::case0301()
{
    auto [tyDeclNode, semaModel] =
            compileTestSymbols<StructOrUnionDeclarationSyntax>("union x { int _ ; } ;");

    PSY_EXPECT_EQ_ENU(tyDeclNode->kind(), SyntaxKind::UnionDeclaration, SyntaxKind);
    const UnionDeclarationSymbol* uniom = semaModel->unionFor(tyDeclNode->asStructOrUnionDeclaration());
    PSY_EXPECT_TRUE(uniom);
    PSY_EXPECT_EQ_STR(uniom->introducedNewType()->tag()->valueText(), "x");
}

void SemanticModelTester::case0302()
{
    auto [varAndOrFunDeclNode, semaModel] =
            compileTestSymbols<VariableAndOrFunctionDeclarationSyntax>("struct x { int _ ; } y ;");

    auto declSyms = semaModel->variablesAndOrFunctionsFor(varAndOrFunDeclNode);
    PSY_EXPECT_EQ_INT(declSyms.size(), 1);

    auto spec = varAndOrFunDeclNode->specifiers()->value->asSpecifier();
    auto tyDeclNode = spec->asTagDeclarationAsSpecifier()->tagDeclaration();
    PSY_EXPECT_EQ_ENU(tyDeclNode->kind(), SyntaxKind::StructDeclaration, SyntaxKind);

    const StructDeclarationSymbol* strukt = semaModel->structFor(tyDeclNode->asStructOrUnionDeclaration());
    PSY_EXPECT_TRUE(strukt);
    PSY_EXPECT_EQ_STR(strukt->introducedNewType()->tag()->valueText(), "x");
}

void SemanticModelTester::case0303()
{
    auto [tyDeclNode, semaModel] =
            compileTestSymbols<StructOrUnionDeclarationSyntax>("struct x { struct y { int _ ; } z ; } ;");

    auto tySpec = tyDeclNode->typeSpecifier();
    auto fldDecl0 = tySpec->declarations()->value->asFieldDeclaration();
    auto nestedtyDeclNode = fldDecl0->specifiers()->value->asTagDeclarationAsSpecifier()->tagDeclaration();
    PSY_EXPECT_EQ_ENU(nestedtyDeclNode->kind(), SyntaxKind::StructDeclaration, SyntaxKind);

    const StructDeclarationSymbol* strukt = semaModel->structFor(nestedtyDeclNode->asStructOrUnionDeclaration());
    PSY_EXPECT_TRUE(strukt);
    PSY_EXPECT_EQ_STR(strukt->introducedNewType()->tag()->valueText(), "y");
}

void SemanticModelTester::case0304()
{
    auto [tyDeclNode, semaModel] =
            compileTestSymbols<StructOrUnionDeclarationSyntax>(R"(
struct x
{
    struct y
    {
        struct z { int _ ; } w;
    } u ;
} ;
)");

    auto tySpec = tyDeclNode->typeSpecifier();
    auto fldDecl0 = tySpec->declarations()->value->asFieldDeclaration();
    auto nestedtyDeclNode = fldDecl0->specifiers()->value->asTagDeclarationAsSpecifier()->tagDeclaration();

    auto nestedTySpec = nestedtyDeclNode->typeSpecifier();
    auto nestedFldDecl0 = nestedTySpec->declarations()->value->asFieldDeclaration();
    auto nestedNestedtyDeclNode = nestedFldDecl0->specifiers()->value->asTagDeclarationAsSpecifier()->tagDeclaration();
    PSY_EXPECT_EQ_ENU(nestedNestedtyDeclNode->kind(),
                      SyntaxKind::StructDeclaration,
                      SyntaxKind);

    const StructDeclarationSymbol* strukt =
            semaModel->structFor(nestedNestedtyDeclNode->asStructOrUnionDeclaration());
    PSY_EXPECT_TRUE(strukt);
    PSY_EXPECT_EQ_STR(strukt->introducedNewType()->tag()->valueText(), "z");
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
    auto [tyDeclNode, semaModel] =
            compileTestSymbols<StructOrUnionDeclarationSyntax>("struct x { int y ; } ;");

    auto tySpec = tyDeclNode->typeSpecifier();
    auto fldDeclNode = tySpec->declarations()->value->asFieldDeclaration();
    std::vector<const FieldDeclarationSymbol*> fldDeclSyms = semaModel->fieldsFor(fldDeclNode);
    PSY_EXPECT_EQ_INT(fldDeclSyms.size(), 1);
    const FieldDeclarationSymbol* fldDeclSym = fldDeclSyms[0];
    PSY_EXPECT_TRUE(fldDeclSym);
    PSY_EXPECT_EQ_STR(fldDeclSym->name()->valueText(), "y");
    PSY_EXPECT_EQ_ENU(fldDeclSym->type()->kind(), TypeKind::Basic, TypeKind);
    PSY_EXPECT_EQ_ENU(fldDeclSym->type()->asBasicType()->kind(), BasicTypeKind::Int_S, BasicTypeKind);
}

void SemanticModelTester::case0351()
{
    auto [tyDeclNode, semaModel] =
            compileTestSymbols<StructOrUnionDeclarationSyntax>("struct x { int y , z ; } ;");

    auto tySpec = tyDeclNode->typeSpecifier();
    auto fldDeclNode = tySpec->declarations()->value->asFieldDeclaration();
    std::vector<const FieldDeclarationSymbol*> fldDeclSyms = semaModel->fieldsFor(fldDeclNode);
    PSY_EXPECT_EQ_INT(fldDeclSyms.size(), 2);

    const FieldDeclarationSymbol* fldDeclSym0 = fldDeclSyms[0];
    PSY_EXPECT_TRUE(fldDeclSym0);
    PSY_EXPECT_EQ_STR(fldDeclSym0->name()->valueText(), "y");
    PSY_EXPECT_EQ_ENU(fldDeclSym0->type()->kind(), TypeKind::Basic, TypeKind);
    PSY_EXPECT_EQ_ENU(fldDeclSym0->type()->asBasicType()->kind(), BasicTypeKind::Int_S, BasicTypeKind);

    const FieldDeclarationSymbol* fldDeclSym1 = fldDeclSyms[1];
    PSY_EXPECT_TRUE(fldDeclSym1);
    PSY_EXPECT_EQ_STR(fldDeclSym1->name()->valueText(), "z");
    PSY_EXPECT_EQ_ENU(fldDeclSym1->type()->kind(), TypeKind::Basic, TypeKind);
    PSY_EXPECT_EQ_ENU(fldDeclSym1->type()->asBasicType()->kind(), BasicTypeKind::Int_S, BasicTypeKind);
}

void SemanticModelTester::case0352()
{
    auto [tyDeclNode, semaModel] =
            compileTestSymbols<StructOrUnionDeclarationSyntax>("struct x { int y ; double * z ; } ;");

    auto tySpec = tyDeclNode->typeSpecifier();

    auto fldDeclNode = tySpec->declarations()->value->asFieldDeclaration();
    std::vector<const FieldDeclarationSymbol*> fldDeclSyms = semaModel->fieldsFor(fldDeclNode);
    PSY_EXPECT_EQ_INT(fldDeclSyms.size(), 1);
    const FieldDeclarationSymbol* fldDeclSym = fldDeclSyms[0];
    PSY_EXPECT_TRUE(fldDeclSym);
    PSY_EXPECT_EQ_STR(fldDeclSym->name()->valueText(), "y");
    PSY_EXPECT_EQ_ENU(fldDeclSym->type()->kind(), TypeKind::Basic, TypeKind);
    PSY_EXPECT_EQ_ENU(fldDeclSym->type()->asBasicType()->kind(), BasicTypeKind::Int_S, BasicTypeKind);

    auto nextFldDeclNode = tySpec->declarations()->next->value->asFieldDeclaration();
    std::vector<const FieldDeclarationSymbol*> nextFldDeclSyms = semaModel->fieldsFor(nextFldDeclNode);
    PSY_EXPECT_EQ_INT(nextFldDeclSyms.size(), 1);
    const FieldDeclarationSymbol* nextFldDeclSym = nextFldDeclSyms[0];
    PSY_EXPECT_TRUE(nextFldDeclSym);
    PSY_EXPECT_EQ_STR(nextFldDeclSym->name()->valueText(), "z");
    PSY_EXPECT_EQ_ENU(nextFldDeclSym->type()->kind(), TypeKind::Pointer, TypeKind);
}

void SemanticModelTester::case0353()
{
    auto [tyDeclNode, semaModel] =
            compileTestSymbols<StructOrUnionDeclarationSyntax>("struct x { int y , z ; double * k, w ; } ;");

    auto tySpec = tyDeclNode->typeSpecifier();

    auto fldDeclNode0 = tySpec->declarations()->value->asFieldDeclaration();
    std::vector<const FieldDeclarationSymbol*> fldDeclSyms0 = semaModel->fieldsFor(fldDeclNode0);
    PSY_EXPECT_EQ_INT(fldDeclSyms0.size(), 2);

    const FieldDeclarationSymbol* fldDeclSym0_0 = fldDeclSyms0[0];
    PSY_EXPECT_TRUE(fldDeclSym0_0);
    PSY_EXPECT_EQ_STR(fldDeclSym0_0->name()->valueText(), "y");
    PSY_EXPECT_EQ_ENU(fldDeclSym0_0->type()->kind(), TypeKind::Basic, TypeKind);
    PSY_EXPECT_EQ_ENU(fldDeclSym0_0->type()->asBasicType()->kind(), BasicTypeKind::Int_S, BasicTypeKind);

    const FieldDeclarationSymbol* fldDeclSym0_1 = fldDeclSyms0[1];
    PSY_EXPECT_EQ_STR(fldDeclSym0_1->name()->valueText(), "z");
    PSY_EXPECT_EQ_ENU(fldDeclSym0_1->type()->kind(), TypeKind::Basic, TypeKind);
    PSY_EXPECT_EQ_ENU(fldDeclSym0_1->type()->asBasicType()->kind(), BasicTypeKind::Int_S, BasicTypeKind);

    auto fldDeclNode1 = tySpec->declarations()->next->value->asFieldDeclaration();
    std::vector<const FieldDeclarationSymbol*> fldDeclSyms1 = semaModel->fieldsFor(fldDeclNode1);
    PSY_EXPECT_EQ_INT(fldDeclSyms1.size(), 2);

    const FieldDeclarationSymbol* fldDeclSym1_0 = fldDeclSyms1[0];
    PSY_EXPECT_TRUE(fldDeclSym1_0);
    PSY_EXPECT_EQ_STR(fldDeclSym1_0->name()->valueText(), "k");
    PSY_EXPECT_EQ_ENU(fldDeclSym1_0->type()->kind(), TypeKind::Pointer, TypeKind);

    const FieldDeclarationSymbol* fldDeclSym1_1 = fldDeclSyms1[1];
    PSY_EXPECT_TRUE(fldDeclSym1_1);
    PSY_EXPECT_EQ_STR(fldDeclSym1_1->name()->valueText(), "w");
    PSY_EXPECT_EQ_ENU(fldDeclSym1_1->type()->kind(), TypeKind::Basic, TypeKind);
    PSY_EXPECT_EQ_ENU(fldDeclSym1_1->type()->asBasicType()->kind(), BasicTypeKind::Double, BasicTypeKind);
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
    auto [enumDeclNode, semaModel] =
            compileTestSymbols<EnumDeclarationSyntax>("enum x { y } ;");

    const EnumDeclarationSymbol* enun = semaModel->enumFor(enumDeclNode);
    PSY_EXPECT_TRUE(enun);
    PSY_EXPECT_EQ_STR(enun->introducedNewType()->tag()->valueText(), "x");
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
    auto [tyDeclNode, semaModel] =
            compileTestSymbols<EnumDeclarationSyntax>("enum x { y } ;");

    auto tySpec = tyDeclNode->typeSpecifier();

    auto enumeratorDecl0 = tySpec->declarations()->value->asEnumeratorDeclaration();

    const EnumeratorDeclarationSymbol* enumeratorSym = semaModel->enumeratorFor(enumeratorDecl0);
    PSY_EXPECT_TRUE(enumeratorSym);
    PSY_EXPECT_EQ_STR(enumeratorSym->name()->valueText(), "y");
    PSY_EXPECT_EQ_ENU(enumeratorSym->type()->kind(), TypeKind::Basic, TypeKind);
    PSY_EXPECT_EQ_ENU(enumeratorSym->type()->asBasicType()->kind(), BasicTypeKind::Int_S, BasicTypeKind);
}

void SemanticModelTester::case0451()
{
    auto [tyDeclNode, semaModel] =
            compileTestSymbols<EnumDeclarationSyntax>("enum x { y = 0 } ;");

    auto tySpec = tyDeclNode->typeSpecifier();

    auto enumeratorDecl0 = tySpec->declarations()->value->asEnumeratorDeclaration();

    const EnumeratorDeclarationSymbol* enumeratorSym = semaModel->enumeratorFor(enumeratorDecl0);
    PSY_EXPECT_TRUE(enumeratorSym);
    PSY_EXPECT_EQ_STR(enumeratorSym->name()->valueText(), "y");
    PSY_EXPECT_EQ_ENU(enumeratorSym->type()->kind(), TypeKind::Basic, TypeKind);
    PSY_EXPECT_EQ_ENU(enumeratorSym->type()->asBasicType()->kind(), BasicTypeKind::Int_S, BasicTypeKind);
}

void SemanticModelTester::case0452()
{
    auto [tyDeclNode, semaModel] =
            compileTestSymbols<EnumDeclarationSyntax>("enum x { y , z } ;");

    auto tySpec = tyDeclNode->typeSpecifier();

    auto enumeratorDecl0 = tySpec->declarations()->value->asEnumeratorDeclaration();

    const EnumeratorDeclarationSymbol* enumeratorSym0 = semaModel->enumeratorFor(enumeratorDecl0);
    PSY_EXPECT_TRUE(enumeratorSym0);
    PSY_EXPECT_EQ_STR(enumeratorSym0->name()->valueText(), "y");
    PSY_EXPECT_EQ_ENU(enumeratorSym0->type()->kind(), TypeKind::Basic, TypeKind);
    PSY_EXPECT_EQ_ENU(enumeratorSym0->type()->asBasicType()->kind(), BasicTypeKind::Int_S, BasicTypeKind);

    auto enumeratorDecl1 = tySpec->declarations()->next->value->asEnumeratorDeclaration();

    const EnumeratorDeclarationSymbol* enumeratorSym1 = semaModel->enumeratorFor(enumeratorDecl1);
    PSY_EXPECT_TRUE(enumeratorSym1);
    PSY_EXPECT_EQ_STR(enumeratorSym1->name()->valueText(), "z");
    PSY_EXPECT_EQ_ENU(enumeratorSym1->type()->kind(), TypeKind::Basic, TypeKind);
    PSY_EXPECT_EQ_ENU(enumeratorSym1->type()->asBasicType()->kind(), BasicTypeKind::Int_S, BasicTypeKind);
}

void SemanticModelTester::case0453()
{
    auto [tyDeclNode, semaModel] =
            compileTestSymbols<EnumDeclarationSyntax>("enum x { y = 0 , z } ;");

    auto tySpec = tyDeclNode->typeSpecifier();

    auto enumeratorDecl0 = tySpec->declarations()->value->asEnumeratorDeclaration();

    const EnumeratorDeclarationSymbol* enumeratorSym0 = semaModel->enumeratorFor(enumeratorDecl0);
    PSY_EXPECT_TRUE(enumeratorSym0);
    PSY_EXPECT_EQ_STR(enumeratorSym0->name()->valueText(), "y");
    PSY_EXPECT_EQ_ENU(enumeratorSym0->type()->kind(), TypeKind::Basic, TypeKind);
    PSY_EXPECT_EQ_ENU(enumeratorSym0->type()->asBasicType()->kind(), BasicTypeKind::Int_S, BasicTypeKind);

    auto enumeratorDecl1 = tySpec->declarations()->next->value->asEnumeratorDeclaration();

    const EnumeratorDeclarationSymbol* enumeratorSym1 = semaModel->enumeratorFor(enumeratorDecl1);
    PSY_EXPECT_TRUE(enumeratorSym1);
    PSY_EXPECT_EQ_STR(enumeratorSym1->name()->valueText(), "z");
    PSY_EXPECT_EQ_ENU(enumeratorSym1->type()->kind(), TypeKind::Basic, TypeKind);
    PSY_EXPECT_EQ_ENU(enumeratorSym1->type()->asBasicType()->kind(), BasicTypeKind::Int_S, BasicTypeKind);
}

void SemanticModelTester::case0454()
{
    auto [tyDeclNode, semaModel] =
            compileTestSymbols<EnumDeclarationSyntax>("enum x { y = 0 , z = 1 } ;");

    auto tySpec = tyDeclNode->typeSpecifier();

    auto enumeratorDecl0 = tySpec->declarations()->value->asEnumeratorDeclaration();

    const EnumeratorDeclarationSymbol* enumeratorSym0 = semaModel->enumeratorFor(enumeratorDecl0);
    PSY_EXPECT_TRUE(enumeratorSym0);
    PSY_EXPECT_EQ_STR(enumeratorSym0->name()->valueText(), "y");
    PSY_EXPECT_EQ_ENU(enumeratorSym0->type()->kind(), TypeKind::Basic, TypeKind);
    PSY_EXPECT_EQ_ENU(enumeratorSym0->type()->asBasicType()->kind(), BasicTypeKind::Int_S, BasicTypeKind);

    auto enumeratorDecl1 = tySpec->declarations()->next->value->asEnumeratorDeclaration();

    const EnumeratorDeclarationSymbol* enumeratorSym1 = semaModel->enumeratorFor(enumeratorDecl1);
    PSY_EXPECT_TRUE(enumeratorSym1);
    PSY_EXPECT_EQ_STR(enumeratorSym1->name()->valueText(), "z");
    PSY_EXPECT_EQ_ENU(enumeratorSym1->type()->kind(), TypeKind::Basic, TypeKind);
    PSY_EXPECT_EQ_ENU(enumeratorSym1->type()->asBasicType()->kind(), BasicTypeKind::Int_S, BasicTypeKind);
}

void SemanticModelTester::case0455()
{
    auto [tyDeclNode, semaModel] =
            compileTestSymbols<EnumDeclarationSyntax>("enum x { y , z = 1 } ;");

    auto tySpec = tyDeclNode->typeSpecifier();

    auto enumeratorDecl0 = tySpec->declarations()->value->asEnumeratorDeclaration();

    const EnumeratorDeclarationSymbol* enumeratorSym0 = semaModel->enumeratorFor(enumeratorDecl0);
    PSY_EXPECT_TRUE(enumeratorSym0);
    PSY_EXPECT_EQ_STR(enumeratorSym0->name()->valueText(), "y");
    PSY_EXPECT_EQ_ENU(enumeratorSym0->type()->kind(), TypeKind::Basic, TypeKind);
    PSY_EXPECT_EQ_ENU(enumeratorSym0->type()->asBasicType()->kind(), BasicTypeKind::Int_S, BasicTypeKind);

    auto enumeratorDecl1 = tySpec->declarations()->next->value->asEnumeratorDeclaration();

    const EnumeratorDeclarationSymbol* enumeratorSym1 = semaModel->enumeratorFor(enumeratorDecl1);
    PSY_EXPECT_TRUE(enumeratorSym1);
    PSY_EXPECT_EQ_STR(enumeratorSym1->name()->valueText(), "z");
    PSY_EXPECT_EQ_ENU(enumeratorSym1->type()->kind(), TypeKind::Basic, TypeKind);
    PSY_EXPECT_EQ_ENU(enumeratorSym1->type()->asBasicType()->kind(), BasicTypeKind::Int_S, BasicTypeKind);
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
    auto [exprNodeByText, semaModel] = compileTestTypes("void f() { int x; x = 1; }");

    auto exprNode = exprNodeByText["x"];
    PSY_EXPECT_TRUE(exprNode);
    auto tyInfo = semaModel->typeInfoOf(exprNode);
    auto ty = tyInfo.type();
    PSY_EXPECT_TRUE(ty);
    PSY_EXPECT_EQ_ENU(ty->kind(), TypeKind::Basic, TypeKind);
    PSY_EXPECT_EQ_ENU(ty->asBasicType()->kind(), BasicTypeKind::Int_S, BasicTypeKind);

    exprNode = exprNodeByText["1"];
    PSY_EXPECT_TRUE(exprNode);
    tyInfo = semaModel->typeInfoOf(exprNode);
    ty = tyInfo.type();
    PSY_EXPECT_TRUE(ty);
    PSY_EXPECT_EQ_ENU(ty->kind(), TypeKind::Basic, TypeKind);
    PSY_EXPECT_EQ_ENU(ty->asBasicType()->kind(), BasicTypeKind::Int_S, BasicTypeKind);

    exprNode = exprNodeByText["x = 1"];
    PSY_EXPECT_TRUE(exprNode);
    tyInfo = semaModel->typeInfoOf(exprNode);
    ty = tyInfo.type();
    PSY_EXPECT_TRUE(ty);
    PSY_EXPECT_EQ_ENU(ty->kind(), TypeKind::Basic, TypeKind);
    PSY_EXPECT_EQ_ENU(ty->asBasicType()->kind(), BasicTypeKind::Int_S, BasicTypeKind);
}

void SemanticModelTester::case0501()
{
    auto [exprNodeByText, semaModel] =
            compileTestTypes("void f() { int x, y; y = x = 1; }");

    auto exprNode = exprNodeByText["x"];
    PSY_EXPECT_TRUE(exprNode);
    auto tyInfo = semaModel->typeInfoOf(exprNode);
    auto ty = tyInfo.type();
    PSY_EXPECT_TRUE(ty);
    PSY_EXPECT_EQ_ENU(ty->kind(), TypeKind::Basic, TypeKind);
    PSY_EXPECT_EQ_ENU(ty->asBasicType()->kind(), BasicTypeKind::Int_S, BasicTypeKind);

    exprNode = exprNodeByText["1"];
    PSY_EXPECT_TRUE(exprNode);
    tyInfo = semaModel->typeInfoOf(exprNode);
    ty = tyInfo.type();
    PSY_EXPECT_TRUE(ty);
    PSY_EXPECT_EQ_ENU(ty->kind(), TypeKind::Basic, TypeKind);
    PSY_EXPECT_EQ_ENU(ty->asBasicType()->kind(), BasicTypeKind::Int_S, BasicTypeKind);

    exprNode = exprNodeByText["x = 1"];
    PSY_EXPECT_TRUE(exprNode);
    tyInfo = semaModel->typeInfoOf(exprNode);
    ty = tyInfo.type();
    PSY_EXPECT_TRUE(ty);
    PSY_EXPECT_EQ_ENU(ty->kind(), TypeKind::Basic, TypeKind);
    PSY_EXPECT_EQ_ENU(ty->asBasicType()->kind(), BasicTypeKind::Int_S, BasicTypeKind);

    exprNode = exprNodeByText["y = x = 1"];
    PSY_EXPECT_TRUE(exprNode);
    tyInfo = semaModel->typeInfoOf(exprNode);
    ty = tyInfo.type();
    PSY_EXPECT_TRUE(ty);
    PSY_EXPECT_EQ_ENU(ty->kind(), TypeKind::Basic, TypeKind);
    PSY_EXPECT_EQ_ENU(ty->asBasicType()->kind(), BasicTypeKind::Int_S, BasicTypeKind);
}

void SemanticModelTester::case0502()
{
    auto [exprNodeByText, semaModel] =
            compileTestTypes("void f() { int x, y, z; z = y = x = 1; }");

    auto exprNode = exprNodeByText["x"];
    PSY_EXPECT_TRUE(exprNode);
    auto tyInfo = semaModel->typeInfoOf(exprNode);
    auto ty = tyInfo.type();
    PSY_EXPECT_TRUE(ty);
    PSY_EXPECT_EQ_ENU(ty->kind(), TypeKind::Basic, TypeKind);
    PSY_EXPECT_EQ_ENU(ty->asBasicType()->kind(), BasicTypeKind::Int_S, BasicTypeKind);

    exprNode = exprNodeByText["1"];
    PSY_EXPECT_TRUE(exprNode);
    tyInfo = semaModel->typeInfoOf(exprNode);
    ty = tyInfo.type();
    PSY_EXPECT_TRUE(ty);
    PSY_EXPECT_EQ_ENU(ty->kind(), TypeKind::Basic, TypeKind);
    PSY_EXPECT_EQ_ENU(ty->asBasicType()->kind(), BasicTypeKind::Int_S, BasicTypeKind);

    exprNode = exprNodeByText["x = 1"];
    PSY_EXPECT_TRUE(exprNode);
    tyInfo = semaModel->typeInfoOf(exprNode);
    ty = tyInfo.type();
    PSY_EXPECT_TRUE(ty);
    PSY_EXPECT_EQ_ENU(ty->kind(), TypeKind::Basic, TypeKind);
    PSY_EXPECT_EQ_ENU(ty->asBasicType()->kind(), BasicTypeKind::Int_S, BasicTypeKind);

    exprNode = exprNodeByText["y = x = 1"];
    PSY_EXPECT_TRUE(exprNode);
    tyInfo = semaModel->typeInfoOf(exprNode);
    ty = tyInfo.type();
    PSY_EXPECT_TRUE(ty);
    PSY_EXPECT_EQ_ENU(ty->kind(), TypeKind::Basic, TypeKind);
    PSY_EXPECT_EQ_ENU(ty->asBasicType()->kind(), BasicTypeKind::Int_S, BasicTypeKind);

    exprNode = exprNodeByText["z = y = x = 1"];
    PSY_EXPECT_TRUE(exprNode);
    tyInfo = semaModel->typeInfoOf(exprNode);
    ty = tyInfo.type();
    PSY_EXPECT_TRUE(ty);
    PSY_EXPECT_EQ_ENU(ty->kind(), TypeKind::Basic, TypeKind);
    PSY_EXPECT_EQ_ENU(ty->asBasicType()->kind(), BasicTypeKind::Int_S, BasicTypeKind);
}

void SemanticModelTester::case0503()
{
    auto [exprNodeByText, semaModel] =
            compileTestTypes("void f() { f(); }");

    auto exprNode = exprNodeByText["f"];
    PSY_EXPECT_TRUE(exprNode);
    auto tyInfo = semaModel->typeInfoOf(exprNode);
    auto ty = tyInfo.type();
    PSY_EXPECT_TRUE(ty);
    PSY_EXPECT_EQ_ENU(ty->kind(), TypeKind::Function, TypeKind);

    exprNode = exprNodeByText["f ( )"];
    PSY_EXPECT_TRUE(exprNode);
    tyInfo = semaModel->typeInfoOf(exprNode);
    ty = tyInfo.type();
    PSY_EXPECT_TRUE(ty);
    PSY_EXPECT_EQ_ENU(ty->kind(), TypeKind::Void, TypeKind);
}

void SemanticModelTester::case0504()
{
    auto [exprNodeByText, semaModel] =
            compileTestTypes("double f() { return f(); }");

    auto exprNode = exprNodeByText["f"];
    PSY_EXPECT_TRUE(exprNode);
    auto tyInfo = semaModel->typeInfoOf(exprNode);
    auto ty = tyInfo.type();
    PSY_EXPECT_TRUE(ty);
    PSY_EXPECT_EQ_ENU(ty->kind(), TypeKind::Function, TypeKind);

    exprNode = exprNodeByText["f ( )"];
    PSY_EXPECT_TRUE(exprNode);
    tyInfo = semaModel->typeInfoOf(exprNode);
    ty = tyInfo.type();
    PSY_EXPECT_TRUE(ty);
    PSY_EXPECT_EQ_ENU(ty->kind(), TypeKind::Basic, TypeKind);
    PSY_EXPECT_EQ_ENU(ty->asBasicType()->kind(), BasicTypeKind::Double, BasicTypeKind);
}

void SemanticModelTester::case0505()
{
    auto [exprNodeByText, semaModel] =
            compileTestTypes("void f() { double x; x = 1 + 2; }");

    auto exprNode = exprNodeByText["1"];
    PSY_EXPECT_TRUE(exprNode);
    auto tyInfo = semaModel->typeInfoOf(exprNode);
    auto ty = tyInfo.type();
    PSY_EXPECT_TRUE(ty);
    PSY_EXPECT_EQ_ENU(ty->kind(), TypeKind::Basic, TypeKind);
    PSY_EXPECT_EQ_ENU(ty->asBasicType()->kind(), BasicTypeKind::Int_S, BasicTypeKind);

    exprNode = exprNodeByText["2"];
    PSY_EXPECT_TRUE(exprNode);
    tyInfo = semaModel->typeInfoOf(exprNode);
    ty = tyInfo.type();
    PSY_EXPECT_TRUE(ty);
    PSY_EXPECT_EQ_ENU(ty->kind(), TypeKind::Basic, TypeKind);
    PSY_EXPECT_EQ_ENU(ty->asBasicType()->kind(), BasicTypeKind::Int_S, BasicTypeKind);

    exprNode = exprNodeByText["1 + 2"];
    PSY_EXPECT_TRUE(exprNode);
    tyInfo = semaModel->typeInfoOf(exprNode);
    ty = tyInfo.type();
    PSY_EXPECT_TRUE(ty);
    PSY_EXPECT_EQ_ENU(ty->kind(), TypeKind::Basic, TypeKind);
    PSY_EXPECT_EQ_ENU(ty->asBasicType()->kind(), BasicTypeKind::Int_S, BasicTypeKind);
}

void SemanticModelTester::case0506()
{
    auto [exprNodeByText, semaModel] =
            compileTestTypes("void f() { double x; x = 1 + 1.0; }");

    auto exprNode = exprNodeByText["1"];
    PSY_EXPECT_TRUE(exprNode);
    auto tyInfo = semaModel->typeInfoOf(exprNode);
    auto ty = tyInfo.type();
    PSY_EXPECT_TRUE(ty);
    PSY_EXPECT_EQ_ENU(ty->kind(), TypeKind::Basic, TypeKind);
    PSY_EXPECT_EQ_ENU(ty->asBasicType()->kind(), BasicTypeKind::Int_S, BasicTypeKind);

    exprNode = exprNodeByText["1.0"];
    PSY_EXPECT_TRUE(exprNode);
    tyInfo = semaModel->typeInfoOf(exprNode);
    ty = tyInfo.type();
    PSY_EXPECT_TRUE(ty);
    PSY_EXPECT_EQ_ENU(ty->kind(), TypeKind::Basic, TypeKind);
    PSY_EXPECT_EQ_ENU(ty->asBasicType()->kind(), BasicTypeKind::Double, BasicTypeKind);

    exprNode = exprNodeByText["1 + 1.0"];
    PSY_EXPECT_TRUE(exprNode);
    tyInfo = semaModel->typeInfoOf(exprNode);
    ty = tyInfo.type();
    PSY_EXPECT_TRUE(ty);
    PSY_EXPECT_EQ_ENU(ty->kind(), TypeKind::Basic, TypeKind);
    PSY_EXPECT_EQ_ENU(ty->asBasicType()->kind(), BasicTypeKind::Double, BasicTypeKind);
}

void SemanticModelTester::case0507()
{
    auto [exprNodeByText, semaModel] =
            compileTestTypes("void f() { double x = 1 + 1.0; }");

    auto exprNode = exprNodeByText["1"];
    PSY_EXPECT_TRUE(exprNode);
    auto tyInfo = semaModel->typeInfoOf(exprNode);
    auto ty = tyInfo.type();
    PSY_EXPECT_TRUE(ty);
    PSY_EXPECT_EQ_ENU(ty->kind(), TypeKind::Basic, TypeKind);
    PSY_EXPECT_EQ_ENU(ty->asBasicType()->kind(), BasicTypeKind::Int_S, BasicTypeKind);

    exprNode = exprNodeByText["1.0"];
    PSY_EXPECT_TRUE(exprNode);
    tyInfo = semaModel->typeInfoOf(exprNode);
    ty = tyInfo.type();
    PSY_EXPECT_TRUE(ty);
    PSY_EXPECT_EQ_ENU(ty->kind(), TypeKind::Basic, TypeKind);
    PSY_EXPECT_EQ_ENU(ty->asBasicType()->kind(), BasicTypeKind::Double, BasicTypeKind);

    exprNode = exprNodeByText["1 + 1.0"];
    PSY_EXPECT_TRUE(exprNode);
    tyInfo = semaModel->typeInfoOf(exprNode);
    ty = tyInfo.type();
    PSY_EXPECT_TRUE(ty);
    PSY_EXPECT_EQ_ENU(ty->kind(), TypeKind::Basic, TypeKind);
    PSY_EXPECT_EQ_ENU(ty->asBasicType()->kind(), BasicTypeKind::Double, BasicTypeKind);
}

void SemanticModelTester::case0508(){}
void SemanticModelTester::case0509(){}
