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

#include "Reparser.h"

#include "syntax/SyntaxNodes.h"

#include "../common/infra/Assertions.h"
#include "../common/infra/Escape.h"

using namespace psy;
using namespace C;

Reparser::~Reparser()
{}

Reparser::Reparser(SyntaxTree* tree)
    : SyntaxVisitor(tree)
{}

SyntaxVisitor::Action Reparser::visitTranslationUnit(const TranslationUnitSyntax* node)
{
    for (auto iter = node->declarations(); iter; iter = iter->next)
        visit(iter->value);

    return Action::Skip;
}

SyntaxVisitor::Action Reparser::visitIncompleteDeclaration(const IncompleteDeclarationSyntax* node) { return Action::Visit; }

SyntaxVisitor::Action Reparser::visitStructOrUnionDeclaration(const StructOrUnionDeclarationSyntax* node) { return Action::Visit; }

SyntaxVisitor::Action Reparser::visitEnumDeclaration(const EnumDeclarationSyntax* node) { return Action::Visit; }

SyntaxVisitor::Action Reparser::visitEnumeratorDeclaration(const EnumeratorDeclarationSyntax* node) { return Action::Visit; }

SyntaxVisitor::Action Reparser::visitVariableAndOrFunctionDeclaration(const VariableAndOrFunctionDeclarationSyntax* node) { return Action::Visit; }

SyntaxVisitor::Action Reparser::visitFieldDeclaration(const FieldDeclarationSyntax* node) { return Action::Visit; }

SyntaxVisitor::Action Reparser::visitParameterDeclaration(const ParameterDeclarationSyntax* node) { return Action::Visit; }

SyntaxVisitor::Action Reparser::visitStaticAssertDeclaration(const StaticAssertDeclarationSyntax* node) { return Action::Visit; }

SyntaxVisitor::Action Reparser::visitFunctionDefinition(const FunctionDefinitionSyntax* node) { return Action::Visit; }

SyntaxVisitor::Action Reparser::visitExtPSY_TemplateDeclaration(const ExtPSY_TemplateDeclarationSyntax* node) { return Action::Visit; }

SyntaxVisitor::Action Reparser::visitExtGNU_AsmStatementDeclaration(const ExtGNU_AsmStatementDeclarationSyntax* node) { return Action::Visit; }

SyntaxVisitor::Action Reparser::visitExtKR_ParameterDeclaration(const ExtKR_ParameterDeclarationSyntax* node) { return Action::Visit; }

SyntaxVisitor::Action Reparser::visitStorageClass(const StorageClassSyntax* node) { return Action::Visit; }

SyntaxVisitor::Action Reparser::visitBuiltinTypeSpecifier(const BuiltinTypeSpecifierSyntax* node) { return Action::Visit; }

SyntaxVisitor::Action Reparser::visitTagTypeSpecifier(const TagTypeSpecifierSyntax* node) { return Action::Visit; }

SyntaxVisitor::Action Reparser::visitAtomicTypeSpecifier(const AtomicTypeSpecifierSyntax* node) { return Action::Visit; }

SyntaxVisitor::Action Reparser::visitTypeDeclarationAsSpecifier(const TypeDeclarationAsSpecifierSyntax* node) { return Action::Visit; }

SyntaxVisitor::Action Reparser::visitTypedefName(const TypedefNameSyntax* node) { return Action::Visit; }

SyntaxVisitor::Action Reparser::visitTypeQualifier(const TypeQualifierSyntax* node) { return Action::Visit; }

SyntaxVisitor::Action Reparser::visitFunctionSpecifier(const FunctionSpecifierSyntax* node) { return Action::Visit; }

SyntaxVisitor::Action Reparser::visitAlignmentSpecifier(const AlignmentSpecifierSyntax* node) { return Action::Visit; }

SyntaxVisitor::Action Reparser::visitExtGNU_Typeof(const ExtGNU_TypeofSyntax* node) { return Action::Visit; }

SyntaxVisitor::Action Reparser::visitExtGNU_AttributeSpecifier(const ExtGNU_AttributeSpecifierSyntax* node) { return Action::Visit; }

SyntaxVisitor::Action Reparser::visitExtGNU_Attribute(const ExtGNU_AttributeSyntax* node) { return Action::Visit; }

SyntaxVisitor::Action Reparser::visitExtGNU_AsmLabel(const ExtGNU_AsmLabelSyntax* node) { return Action::Visit; }

SyntaxVisitor::Action Reparser::visitExtPSY_QuantifiedTypeSpecifier(const ExtPSY_QuantifiedTypeSpecifierSyntax* node) { return Action::Visit; }

SyntaxVisitor::Action Reparser::visitArrayOrFunctionDeclarator(const ArrayOrFunctionDeclaratorSyntax* node) { return Action::Visit; }

SyntaxVisitor::Action Reparser::visitPointerDeclarator(const PointerDeclaratorSyntax* node) { return Action::Visit; }

SyntaxVisitor::Action Reparser::visitParenthesizedDeclarator(const ParenthesizedDeclaratorSyntax* node) { return Action::Visit; }

SyntaxVisitor::Action Reparser::visitIdentifierDeclarator(const IdentifierDeclaratorSyntax* node) { return Action::Visit; }

SyntaxVisitor::Action Reparser::visitAbstractDeclarator(const AbstractDeclaratorSyntax* node) { return Action::Visit; }

SyntaxVisitor::Action Reparser::visitSubscriptSuffix(const SubscriptSuffixSyntax* node) { return Action::Visit; }

SyntaxVisitor::Action Reparser::visitParameterSuffix(const ParameterSuffixSyntax* node) { return Action::Visit; }

SyntaxVisitor::Action Reparser::visitBitfieldDeclarator(const BitfieldDeclaratorSyntax* node) { return Action::Visit; }

SyntaxVisitor::Action Reparser::visitExpressionInitializer(const ExpressionInitializerSyntax* node) { return Action::Visit; }

SyntaxVisitor::Action Reparser::visitBraceEnclosedInitializer(const BraceEnclosedInitializerSyntax* node) { return Action::Visit; }

SyntaxVisitor::Action Reparser::visitDesignatedInitializer(const DesignatedInitializerSyntax* node) { return Action::Visit; }

SyntaxVisitor::Action Reparser::visitFieldDesignator(const FieldDesignatorSyntax* node) { return Action::Visit; }

SyntaxVisitor::Action Reparser::visitArrayDesignator(const ArrayDesignatorSyntax* node) { return Action::Visit; }

SyntaxVisitor::Action Reparser::visitOffsetOfDesignator(const OffsetOfDesignatorSyntax* node) { return Action::Visit; }

SyntaxVisitor::Action Reparser::visitIdentifierName(const IdentifierNameSyntax* node) { return Action::Visit; }

SyntaxVisitor::Action Reparser::visitPredefinedName(const PredefinedNameSyntax* node) { return Action::Visit; }

SyntaxVisitor::Action Reparser::visitConstantExpression(const ConstantExpressionSyntax* node) { return Action::Visit; }

SyntaxVisitor::Action Reparser::visitStringLiteralExpression(const StringLiteralExpressionSyntax* node) { return Action::Visit; }

SyntaxVisitor::Action Reparser::visitParenthesizedExpression(const ParenthesizedExpressionSyntax* node) { return Action::Visit; }

SyntaxVisitor::Action Reparser::visitGenericSelectionExpression(const GenericSelectionExpressionSyntax* node) { return Action::Visit; }

SyntaxVisitor::Action Reparser::visitGenericAssociation(const GenericAssociationSyntax* node) { return Action::Visit; }

SyntaxVisitor::Action Reparser::visitExtGNU_EnclosedCompoundStatementExpression(const ExtGNU_EnclosedCompoundStatementExpressionSyntax* node) { return Action::Visit; }

SyntaxVisitor::Action Reparser::visitExtGNU_ComplexValuedExpression(const ExtGNU_ComplexValuedExpressionSyntax* node) { return Action::Visit; }

SyntaxVisitor::Action Reparser::visitPrefixUnaryExpression(const PrefixUnaryExpressionSyntax* node) { return Action::Visit; }

SyntaxVisitor::Action Reparser::visitPostfixUnaryExpression(const PostfixUnaryExpressionSyntax* node) { return Action::Visit; }

SyntaxVisitor::Action Reparser::visitMemberAccessExpression(const MemberAccessExpressionSyntax* node) { return Action::Visit; }

SyntaxVisitor::Action Reparser::visitArraySubscriptExpression(const ArraySubscriptExpressionSyntax* node) { return Action::Visit; }

SyntaxVisitor::Action Reparser::visitTypeTraitExpression(const TypeTraitExpressionSyntax* node) { return Action::Visit; }

SyntaxVisitor::Action Reparser::visitCastExpression(const CastExpressionSyntax* node) { return Action::Visit; }

SyntaxVisitor::Action Reparser::visitCallExpression(const CallExpressionSyntax* node) { return Action::Visit; }

SyntaxVisitor::Action Reparser::visitVAArgumentExpression(const VAArgumentExpressionSyntax* node) { return Action::Visit; }

SyntaxVisitor::Action Reparser::visitOffsetOfExpression(const OffsetOfExpressionSyntax* node) { return Action::Visit; }

SyntaxVisitor::Action Reparser::visitCompoundLiteralExpression(const CompoundLiteralExpressionSyntax* node) { return Action::Visit; }

SyntaxVisitor::Action Reparser::visitBinaryExpression(const BinaryExpressionSyntax* node) { return Action::Visit; }

SyntaxVisitor::Action Reparser::visitConditionalExpression(const ConditionalExpressionSyntax* node) { return Action::Visit; }

SyntaxVisitor::Action Reparser::visitAssignmentExpression(const AssignmentExpressionSyntax* node) { return Action::Visit; }

SyntaxVisitor::Action Reparser::visitSequencingExpression(const SequencingExpressionSyntax* node) { return Action::Visit; }

SyntaxVisitor::Action Reparser::visitExtGNU_ChooseExpression(const ExtGNU_ChooseExpressionSyntax* node) { return Action::Visit; }

SyntaxVisitor::Action Reparser::visitCompoundStatement(const CompoundStatementSyntax* node) { return Action::Visit; }

SyntaxVisitor::Action Reparser::visitDeclarationStatement(const DeclarationStatementSyntax* node) { return Action::Visit; }

SyntaxVisitor::Action Reparser::visitExpressionStatement(const ExpressionStatementSyntax* node) { return Action::Visit; }

SyntaxVisitor::Action Reparser::visitLabeledStatement(const LabeledStatementSyntax* node) { return Action::Visit; }

SyntaxVisitor::Action Reparser::visitIfStatement(const IfStatementSyntax* node) { return Action::Visit; }

SyntaxVisitor::Action Reparser::visitSwitchStatement(const SwitchStatementSyntax* node) { return Action::Visit; }

SyntaxVisitor::Action Reparser::visitWhileStatement(const WhileStatementSyntax* node) { return Action::Visit; }

SyntaxVisitor::Action Reparser::visitDoStatement(const DoStatementSyntax* node) { return Action::Visit; }

SyntaxVisitor::Action Reparser::visitForStatement(const ForStatementSyntax* node) { return Action::Visit; }

SyntaxVisitor::Action Reparser::visitGotoStatement(const GotoStatementSyntax* node) { return Action::Visit; }

SyntaxVisitor::Action Reparser::visitContinueStatement(const ContinueStatementSyntax* node) { return Action::Visit; }

SyntaxVisitor::Action Reparser::visitBreakStatement(const BreakStatementSyntax* node) { return Action::Visit; }

SyntaxVisitor::Action Reparser::visitReturnStatement(const ReturnStatementSyntax* node) { return Action::Visit; }

SyntaxVisitor::Action Reparser::visitExtGNU_AsmStatement(const ExtGNU_AsmStatementSyntax* node) { return Action::Visit; }

SyntaxVisitor::Action Reparser::visitExtGNU_AsmQualifier(const ExtGNU_AsmQualifierSyntax* node) { return Action::Visit; }

SyntaxVisitor::Action Reparser::visitExtGNU_AsmOperand(const ExtGNU_AsmOperandSyntax* node) { return Action::Visit; }

SyntaxVisitor::Action Reparser::visitTypeName(const TypeNameSyntax* node) { return Action::Visit; }

SyntaxVisitor::Action Reparser::visitExpressionAsTypeReference(const ExpressionAsTypeReferenceSyntax* node) { return Action::Visit; }

SyntaxVisitor::Action Reparser::visitTypeNameAsTypeReference(const TypeNameAsTypeReferenceSyntax* node) { return Action::Visit; }

SyntaxVisitor::Action Reparser::visitAmbiguousTypeNameOrExpressionAsTypeReference(
        const AmbiguousTypeNameOrExpressionAsTypeReferenceSyntax* node)
{
    SyntaxVisitor::visitAmbiguousTypeNameOrExpressionAsTypeReference(node);
    return Action::Skip;
}

SyntaxVisitor::Action Reparser::visitAmbiguousCastOrBinaryExpression(
        const AmbiguousCastOrBinaryExpressionSyntax* node)
{
    SyntaxVisitor::visitAmbiguousCastOrBinaryExpression(node);
    return Action::Skip;
}

SyntaxVisitor::Action Reparser::visitAmbiguousExpressionOrDeclarationStatement(
        const AmbiguousExpressionOrDeclarationStatementSyntax* node)
{
    auto expr = node->expressionStatement()->expression();
    switch (expr->kind()) {
        case MultiplyExpression: {
            auto decl = node->declarationStatement()->declaration();
            PSY_ASSERT(decl->kind() == VariableAndOrFunctionDeclaration, return Action::Skip);

            auto varDecl = decl->asVariableAndOrFunctionDeclaration();
            PSY_ASSERT(varDecl->specifiers()
                           && varDecl->specifiers()->value
                           && varDecl->specifiers()->value->kind() == TypedefName,
                       return Action::Skip);

            auto typedefName = varDecl->specifiers()->value->asTypedefName();
            auto ident = typedefName->identifierToken().valueText();

            auto exprOrDecl = keepExpressionOrDeclarationStatement(ident);
            break;
        }

        case CallExpression: {
            auto callExpr = expr->asCallExpression();
            visit(callExpr->arguments()->value);
            break;
        }

        default:
            PSY_ESCAPE_VIA_RETURN(Action::Skip);
    }

    return Action::Skip;
}
