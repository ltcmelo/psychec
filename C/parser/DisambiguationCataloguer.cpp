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

#include "DisambiguationCataloguer.h"

using namespace psy;
using namespace C;

DisambiguationCataloguer::DisambiguationCataloguer(SyntaxTree* tree)
    : SyntaxVisitor(tree)
{}

SyntaxVisitor::Action DisambiguationCataloguer::visitIdentifierDeclarator(const IdentifierDeclaratorSyntax *)
{
    return Action::Skip;
}

SyntaxVisitor::Action DisambiguationCataloguer::visitIdentifierName(const IdentifierNameSyntax *)
{
    return Action::Skip;
}

SyntaxVisitor::Action DisambiguationCataloguer::visitParenthesizedExpression(const ParenthesizedExpressionSyntax* node)
{
    return Base::visitParenthesizedExpression(node);
}

SyntaxVisitor::Action DisambiguationCataloguer::visitGenericSelectionExpression(const GenericSelectionExpressionSyntax* node)
{
    return Base::visitGenericSelectionExpression(node);
}

SyntaxVisitor::Action DisambiguationCataloguer::visitGenericAssociation(const GenericAssociationSyntax* node)
{
    return Base::visitGenericAssociation(node);
}

SyntaxVisitor::Action DisambiguationCataloguer::visitExtGNU_EnclosedCompoundStatementExpression(const ExtGNU_EnclosedCompoundStatementExpressionSyntax* node)
{
    return Base::visitExtGNU_EnclosedCompoundStatementExpression(node);
}

SyntaxVisitor::Action DisambiguationCataloguer::visitExtGNU_ComplexValuedExpression(const ExtGNU_ComplexValuedExpressionSyntax* node)
{
    return Base::visitExtGNU_ComplexValuedExpression(node);
}

SyntaxVisitor::Action DisambiguationCataloguer::visitPrefixUnaryExpression(const PrefixUnaryExpressionSyntax* node)
{
    return Base::visitPrefixUnaryExpression(node);
}

SyntaxVisitor::Action DisambiguationCataloguer::visitPostfixUnaryExpression(const PostfixUnaryExpressionSyntax* node)
{
    return Base::visitPostfixUnaryExpression(node);
}

SyntaxVisitor::Action DisambiguationCataloguer::visitMemberAccessExpression(const MemberAccessExpressionSyntax* node)
{
    return Base::visitMemberAccessExpression(node);
}

SyntaxVisitor::Action DisambiguationCataloguer::visitArraySubscriptExpression(const ArraySubscriptExpressionSyntax* node)
{
    return Base::visitArraySubscriptExpression(node);
}

SyntaxVisitor::Action DisambiguationCataloguer::visitTypeTraitExpression(const TypeTraitExpressionSyntax* node)
{
    return Base::visitTypeTraitExpression(node);
}

SyntaxVisitor::Action DisambiguationCataloguer::visitCastExpression(const CastExpressionSyntax* node)
{
    return Base::visitCastExpression(node);
}

SyntaxVisitor::Action DisambiguationCataloguer::visitCallExpression(const CallExpressionSyntax* node)
{
    return Base::visitCallExpression(node);
}

SyntaxVisitor::Action DisambiguationCataloguer::visitVAArgumentExpression(const VAArgumentExpressionSyntax* node)
{
    return Base::visitVAArgumentExpression(node);
}

SyntaxVisitor::Action DisambiguationCataloguer::visitOffsetOfExpression(const OffsetOfExpressionSyntax* node)
{
    return Base::visitOffsetOfExpression(node);
}

SyntaxVisitor::Action DisambiguationCataloguer::visitCompoundLiteralExpression(const CompoundLiteralExpressionSyntax* node)
{
    return Base::visitCompoundLiteralExpression(node);
}

SyntaxVisitor::Action DisambiguationCataloguer::visitBinaryExpression(const BinaryExpressionSyntax* node)
{
    return Base::visitBinaryExpression(node);
}

SyntaxVisitor::Action DisambiguationCataloguer::visitConditionalExpression(const ConditionalExpressionSyntax* node)
{
    return Base::visitConditionalExpression(node);
}

SyntaxVisitor::Action DisambiguationCataloguer::visitAssignmentExpression(const AssignmentExpressionSyntax* node)
{
    return Base::visitAssignmentExpression(node);
}

SyntaxVisitor::Action DisambiguationCataloguer::visitSequencingExpression(const SequencingExpressionSyntax* node)
{
    return Base::visitSequencingExpression(node);
}

SyntaxVisitor::Action DisambiguationCataloguer::visitExtGNU_ChooseExpression(const ExtGNU_ChooseExpressionSyntax* node)
{
    return Base::visitExtGNU_ChooseExpression(node);
}

SyntaxVisitor::Action DisambiguationCataloguer::visitDeclarationStatement(const DeclarationStatementSyntax* node)
{
    return Base::visitDeclarationStatement(node);
}

SyntaxVisitor::Action DisambiguationCataloguer::visitExpressionStatement(const ExpressionStatementSyntax* node)
{
    return Base::visitExpressionStatement(node);
}

SyntaxVisitor::Action DisambiguationCataloguer::visitIfStatement(const IfStatementSyntax* node)
{
    return Base::visitIfStatement(node);
}

SyntaxVisitor::Action DisambiguationCataloguer::visitSwitchStatement(const SwitchStatementSyntax* node)
{
    return Base::visitSwitchStatement(node);
}

SyntaxVisitor::Action DisambiguationCataloguer::visitWhileStatement(const WhileStatementSyntax* node)
{
    return Base::visitWhileStatement(node);
}

SyntaxVisitor::Action DisambiguationCataloguer::visitDoStatement(const DoStatementSyntax* node)
{
    return Base::visitDoStatement(node);
}

SyntaxVisitor::Action DisambiguationCataloguer::visitForStatement(const ForStatementSyntax* node)
{
    return Base::visitForStatement(node);
}

SyntaxVisitor::Action DisambiguationCataloguer::visitReturnStatement(const ReturnStatementSyntax* node)
{
    return Base::visitReturnStatement(node);
}

SyntaxVisitor::Action DisambiguationCataloguer::visitExtGNU_AsmStatement(const ExtGNU_AsmStatementSyntax* node)
{
    return Base::visitExtGNU_AsmStatement(node);
}

SyntaxVisitor::Action DisambiguationCataloguer::visitExtGNU_AsmOperand(const ExtGNU_AsmOperandSyntax* node)
{
    return Base::visitExtGNU_AsmOperand(node);
}

SyntaxVisitor::Action DisambiguationCataloguer::visitExpressionAsTypeReference(const ExpressionAsTypeReferenceSyntax* node)
{
    return Base::visitExpressionAsTypeReference(node);
}

SyntaxVisitor::Action DisambiguationCataloguer::visitAmbiguousTypeNameOrExpressionAsTypeReference(const AmbiguousTypeNameOrExpressionAsTypeReferenceSyntax* node)
{
    return Action::Skip;
}

SyntaxVisitor::Action DisambiguationCataloguer::visitAmbiguousCastOrBinaryExpression(const AmbiguousCastOrBinaryExpressionSyntax* node)
{
    return Action::Skip;
}

SyntaxVisitor::Action DisambiguationCataloguer::visitAmbiguousExpressionOrDeclarationStatement(const AmbiguousExpressionOrDeclarationStatementSyntax* node)
{
    return Action::Skip;
}

