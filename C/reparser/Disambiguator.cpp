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

#include "Disambiguator.h"

#include "syntax/SyntaxNodes.h"

#include "../common/infra/Assertions.h"
#include "../common/infra/Escape.h"

using namespace psy;
using namespace C;

Disambiguator::~Disambiguator()
{}

Disambiguator::Disambiguator(SyntaxTree* tree)
    : SyntaxVisitor(tree)
    , pendingAmbigs_(0)
{}

unsigned int Disambiguator::disambiguate()
{
    visit(tree_->root());

    return pendingAmbigs_;
}

template <class ExprT>
SyntaxVisitor::Action Disambiguator::visitMaybeAmbiguousExpression(ExprT* const& node)
{
    ExprT*& node_P = const_cast<ExprT*&>(node);
    switch (node->kind()) {
        case AmbiguousCastOrBinaryExpression: {
            auto ambigNode = node->asAmbiguousCastOrBinaryExpression();
            auto disambig = disambiguateExpression(ambigNode);
            switch (disambig) {
                case Disambiguation::KeepCastExpression:
                    node_P = ambigNode->castExpr_;
                    visit(node_P);
                    break;

                case Disambiguation::KeepExpressionStatement:
                    node_P = ambigNode->binExpr_;
                    visit(node_P);
                    break;

                case Disambiguation::Inconclusive:
                    ++pendingAmbigs_;
                    break;

                default:
                    PSY_ESCAPE_VIA_RETURN(Action::Skip);
            }
            break;
        }

        default:
            visit(node);
            break;
    }

    return Action::Skip;
}

template <class StmtT>
SyntaxVisitor::Action Disambiguator::visitMaybeAmbiguousStatement(StmtT* const& node)
{
    StmtT*& node_P = const_cast<StmtT*&>(node);
    switch (node->kind()) {
        case AmbiguousMultiplicationOrPointerDeclaration:
        case AmbiguousCallOrVariableDeclaration: {
            auto ambigNode = node->asAmbiguousExpressionOrDeclarationStatement();
            auto disambig = disambiguateStatement(ambigNode);
            switch (disambig) {
                case Disambiguation::KeepDeclarationStatement:
                    node_P = ambigNode->declStmt_;
                    visit(node_P);
                    break;

                case Disambiguation::KeepExpressionStatement:
                    node_P = ambigNode->exprStmt_;
                    visit(node_P);
                    break;

                case Disambiguation::Inconclusive:
                    ++pendingAmbigs_;
                    break;

                default:
                    PSY_ESCAPE_VIA_RETURN(Action::Skip);
            }
            break;
        }

        default:
            visit(node);
            break;
    }

    return Action::Skip;
}

template <class TypeRefT>
SyntaxVisitor::Action Disambiguator::visitMaybeAmbiguousTypeReference(TypeRefT* const& node)
{
    TypeRefT*& node_P = const_cast<TypeRefT*&>(node);
    switch (node->kind()) {
        case AmbiguousTypeNameOrExpressionAsTypeReference: {
            auto ambigNode = node->asAmbiguousTypeNameOrExpressionAsTypeReference();
            auto disambig = disambiguateTypeReference(ambigNode);
            switch (disambig) {
                case Disambiguation::KeepTypeName:
                    node_P = ambigNode->tyNameAsTyRef_;
                    visit(node_P);
                    break;

                case Disambiguation::KeepExpressionStatement:
                    node_P = ambigNode->exprAsTyRef_;
                    visit(node_P);
                    break;

                case Disambiguation::Inconclusive:
                    ++pendingAmbigs_;
                    break;

                default:
                    PSY_ESCAPE_VIA_RETURN(Action::Skip);
            }
            break;
        }

        default:
            visit(node);
            break;
    }

    return Action::Skip;
}

//--------------//
// Declarations //
//--------------//

SyntaxVisitor::Action Disambiguator::visitStaticAssertDeclaration(const StaticAssertDeclarationSyntax* node)
{
    visitMaybeAmbiguousExpression(node->expr_);

    return Action::Skip;
}

SyntaxVisitor::Action Disambiguator::visitAlignmentSpecifier(const AlignmentSpecifierSyntax* node)
{
    visitMaybeAmbiguousTypeReference(node->tyRef_);

    return Action::Skip;
}

SyntaxVisitor::Action Disambiguator::visitExtGNU_Typeof(const ExtGNU_TypeofSyntax* node)
{
    visitMaybeAmbiguousTypeReference(node->tyRef_);

    return Action::Skip;
}

SyntaxVisitor::Action Disambiguator::visitSubscriptSuffix(const SubscriptSuffixSyntax* node)
{
    visitMaybeAmbiguousExpression(node->expr_);

    return Action::Skip;
}

SyntaxVisitor::Action Disambiguator::visitExpressionInitializer(const ExpressionInitializerSyntax* node)
{
    return visitMaybeAmbiguousExpression(node->expr_);
}

SyntaxVisitor::Action Disambiguator::visitArrayDesignator(const ArrayDesignatorSyntax* node)
{
    visitMaybeAmbiguousExpression(node->expr_);

    return Action::Skip;
}

//-------------//
// Expressions //
//-------------//

SyntaxVisitor::Action Disambiguator::visitParenthesizedExpression(const ParenthesizedExpressionSyntax* node)
{
    visitMaybeAmbiguousExpression(node->expr_);

    return Action::Skip;
}

SyntaxVisitor::Action Disambiguator::visitGenericSelectionExpression(const GenericSelectionExpressionSyntax* node)
{
    visitMaybeAmbiguousExpression(node->expr_);
    for (auto it = node->associations(); it; it = it->next)
        visit(it->value);

    return Action::Skip;
}

SyntaxVisitor::Action Disambiguator::visitGenericAssociation(const GenericAssociationSyntax* node)
{
    visitMaybeAmbiguousExpression(node->expr_);

    return Action::Skip;
}

SyntaxVisitor::Action Disambiguator::visitExtGNU_EnclosedCompoundStatementExpression(
        const ExtGNU_EnclosedCompoundStatementExpressionSyntax* node)
{
    visit(node->stmt_);

    return Action::Skip;
}

SyntaxVisitor::Action Disambiguator::visitExtGNU_ComplexValuedExpression(
        const ExtGNU_ComplexValuedExpressionSyntax* node)
{
    return Action::Skip;
}

SyntaxVisitor::Action Disambiguator::visitPrefixUnaryExpression(const PrefixUnaryExpressionSyntax* node)
{
    visitMaybeAmbiguousExpression(node->expr_);

    return Action::Skip;
}

SyntaxVisitor::Action Disambiguator::visitPostfixUnaryExpression(const PostfixUnaryExpressionSyntax* node)
{
    visitMaybeAmbiguousExpression(node->expr_);

    return Action::Skip;
}

SyntaxVisitor::Action Disambiguator::visitMemberAccessExpression(const MemberAccessExpressionSyntax* node)
{
    visitMaybeAmbiguousExpression(node->expr_);

    return Action::Skip;
}

SyntaxVisitor::Action Disambiguator::visitArraySubscriptExpression(const ArraySubscriptExpressionSyntax* node)
{
    visitMaybeAmbiguousExpression(node->expr_);

    return Action::Skip;
}

SyntaxVisitor::Action Disambiguator::visitTypeTraitExpression(const TypeTraitExpressionSyntax* node)
{
    return visitMaybeAmbiguousTypeReference(node->tyRef_);
}

SyntaxVisitor::Action Disambiguator::visitCastExpression(const CastExpressionSyntax* node)
{
    visitMaybeAmbiguousExpression(node->expr_);

    return Action::Skip;
}

SyntaxVisitor::Action Disambiguator::visitCallExpression(const CallExpressionSyntax* node)
{
    visitMaybeAmbiguousExpression(node->expr_);

    return Action::Skip;
}

SyntaxVisitor::Action Disambiguator::visitVAArgumentExpression(const VAArgumentExpressionSyntax* node)
{
    visitMaybeAmbiguousExpression(node->expr_);

    return Action::Skip;
}

SyntaxVisitor::Action Disambiguator::visitBinaryExpression(const BinaryExpressionSyntax* node)
{
    visitMaybeAmbiguousExpression(node->leftExpr_);
    visitMaybeAmbiguousExpression(node->rightExpr_);

    return Action::Skip;
}

SyntaxVisitor::Action Disambiguator::visitConditionalExpression(const ConditionalExpressionSyntax* node)
{
    visitMaybeAmbiguousExpression(node->condExpr_);
    visitMaybeAmbiguousExpression(node->whenTrueExpr_);
    visitMaybeAmbiguousExpression(node->whenFalseExpr_);

    return Action::Skip;
}

SyntaxVisitor::Action Disambiguator::visitAssignmentExpression(const AssignmentExpressionSyntax* node)
{
    visitMaybeAmbiguousExpression(node->leftExpr_);
    visitMaybeAmbiguousExpression(node->rightExpr_);

    return Action::Skip;
}

SyntaxVisitor::Action Disambiguator::visitSequencingExpression(const SequencingExpressionSyntax* node)
{
    visitMaybeAmbiguousExpression(node->leftExpr_);
    visitMaybeAmbiguousExpression(node->rightExpr_);

    return Action::Skip;
}

SyntaxVisitor::Action Disambiguator::visitExtGNU_ChooseExpression(const ExtGNU_ChooseExpressionSyntax* node)
{
    visitMaybeAmbiguousExpression(node->expr1_);
    visitMaybeAmbiguousExpression(node->expr2_);

    return Action::Skip;
}

//------------//
// Statements //
//------------//

SyntaxVisitor::Action Disambiguator::visitCompoundStatement(const CompoundStatementSyntax* node)
{
    for (auto iter = node->stmts_; iter; iter = iter->next)
        visitMaybeAmbiguousStatement(iter->value);

    return Action::Skip;
}

SyntaxVisitor::Action Disambiguator::visitDeclarationStatement(const DeclarationStatementSyntax* node)
{
    return Action::Skip;
}

SyntaxVisitor::Action Disambiguator::visitExpressionStatement(const ExpressionStatementSyntax* node)
{
    visitMaybeAmbiguousExpression(node->expr_);

    return Action::Skip;
}

SyntaxVisitor::Action Disambiguator::visitLabeledStatement(const LabeledStatementSyntax* node)
{
    visitMaybeAmbiguousStatement(node->stmt_);

    return Action::Skip;
}

SyntaxVisitor::Action Disambiguator::visitIfStatement(const IfStatementSyntax* node)
{
    visitMaybeAmbiguousExpression(node->cond_);
    visitMaybeAmbiguousStatement(node->stmt_);
    visitMaybeAmbiguousStatement(node->elseStmt_);

    return Action::Skip;
}

SyntaxVisitor::Action Disambiguator::visitSwitchStatement(const SwitchStatementSyntax* node)
{
    visitMaybeAmbiguousExpression(node->cond_);
    visitMaybeAmbiguousStatement(node->stmt_);

    return Action::Skip;
}

SyntaxVisitor::Action Disambiguator::visitWhileStatement(const WhileStatementSyntax* node)
{
    visitMaybeAmbiguousExpression(node->cond_);
    visitMaybeAmbiguousStatement(node->stmt_);

    return Action::Skip;
}

SyntaxVisitor::Action Disambiguator::visitDoStatement(const DoStatementSyntax* node)
{
    visitMaybeAmbiguousExpression(node->cond_);
    visitMaybeAmbiguousStatement(node->stmt_);

    return Action::Skip;
}

SyntaxVisitor::Action Disambiguator::visitForStatement(const ForStatementSyntax* node)
{
    visit(node->initStmt_);
    visitMaybeAmbiguousExpression(node->cond_);
    visitMaybeAmbiguousExpression(node->expr_);
    visitMaybeAmbiguousStatement(node->stmt_);

    return Action::Skip;
}

SyntaxVisitor::Action Disambiguator::visitReturnStatement(const ReturnStatementSyntax* node)
{
    visitMaybeAmbiguousExpression(node->expr_);

    return Action::Skip;
}

SyntaxVisitor::Action Disambiguator::visitExtGNU_AsmOperand(const ExtGNU_AsmOperandSyntax* node)
{
    visitMaybeAmbiguousExpression(node->expr_);

    return Action::Skip;
}

//--------//
// Common //
//--------//

SyntaxVisitor::Action Disambiguator::visitExpressionAsTypeReference(const ExpressionAsTypeReferenceSyntax* node)
{
    visitMaybeAmbiguousExpression(node->expr_);

    return Action::Skip;
}

//-------------//
// Ambiguities //
//-------------//

SyntaxVisitor::Action Disambiguator::visitAmbiguousTypeNameOrExpressionAsTypeReference(
        const AmbiguousTypeNameOrExpressionAsTypeReferenceSyntax*)
{
    PSY_ESCAPE_VIA_RETURN(Action::Quit);
}

SyntaxVisitor::Action Disambiguator::visitAmbiguousCastOrBinaryExpression(
        const AmbiguousCastOrBinaryExpressionSyntax*)
{
    PSY_ESCAPE_VIA_RETURN(Action::Quit);
}

SyntaxVisitor::Action Disambiguator::visitAmbiguousExpressionOrDeclarationStatement(
        const AmbiguousExpressionOrDeclarationStatementSyntax*)
{
    PSY_ESCAPE_VIA_RETURN(Action::Quit);
}
