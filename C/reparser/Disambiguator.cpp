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

Disambiguator::Disambiguation Disambiguator::disambiguateAmbiguousTypeNameOrExpressionAsTypeReferenceSyntax(
        const AmbiguousTypeNameOrExpressionAsTypeReferenceSyntax*) const
{
    return Disambiguation::Undetermined;
}

Disambiguator::Disambiguation Disambiguator::disambiguateAmbiguousCastOrBinaryExpressionSyntax(
        const AmbiguousCastOrBinaryExpressionSyntax* node) const
{
    auto typeName = node->castExpression()->typeName();
    PSY_ASSERT(typeName->specifiers()
                   && typeName->specifiers()->value
                   && typeName->specifiers()->value->kind() == TypedefName,
               return Disambiguation::Undetermined);

    auto typedefName = typeName->specifiers()->value->asTypedefName();
    auto name = typedefName->identifierToken().valueText();

    return recognizesTypeName(name)
            ? Disambiguation::CastExpression
            : recognizesName(name)
                    ? Disambiguation::BinaryExpression
                    : Disambiguation::Undetermined;
}

Disambiguator::Disambiguation Disambiguator::disambiguateAmbiguousExpressionOrDeclarationStatement(
        const AmbiguousExpressionOrDeclarationStatementSyntax* node) const
{
    auto decl = node->declarationStatement()->declaration();
    PSY_ASSERT(decl->kind() == VariableAndOrFunctionDeclaration, return Disambiguation::Undetermined);

    auto varDecl = decl->asVariableAndOrFunctionDeclaration();
    PSY_ASSERT(varDecl->specifiers()
                   && varDecl->specifiers()->value
                   && varDecl->specifiers()->value->kind() == TypedefName,
               return Disambiguation::Undetermined);

    auto typedefName = varDecl->specifiers()->value->asTypedefName();
    auto name = typedefName->identifierToken().valueText();

    return recognizesTypeName(name)
            ? Disambiguation::DeclarationStatement
            : recognizesName(name)
                    ? Disambiguation::ExpressionStatement
                    : Disambiguation::Undetermined;
}

SyntaxVisitor::Action Disambiguator::visitVariableAndOrFunctionDeclaration(const VariableAndOrFunctionDeclarationSyntax* node) { return Action::Visit; }

SyntaxVisitor::Action Disambiguator::visitFieldDeclaration(const FieldDeclarationSyntax* node) { return Action::Visit; }

SyntaxVisitor::Action Disambiguator::visitParameterDeclaration(const ParameterDeclarationSyntax* node) { return Action::Visit; }

SyntaxVisitor::Action Disambiguator::visitStaticAssertDeclaration(const StaticAssertDeclarationSyntax* node) { return Action::Visit; }

SyntaxVisitor::Action Disambiguator::visitFunctionDefinition(const FunctionDefinitionSyntax* node) { return Action::Visit; }

SyntaxVisitor::Action Disambiguator::visitExtPSY_TemplateDeclaration(const ExtPSY_TemplateDeclarationSyntax* node) { return Action::Visit; }

SyntaxVisitor::Action Disambiguator::visitExtGNU_AsmStatementDeclaration(const ExtGNU_AsmStatementDeclarationSyntax* node) { return Action::Visit; }

SyntaxVisitor::Action Disambiguator::visitExtKR_ParameterDeclaration(const ExtKR_ParameterDeclarationSyntax* node) { return Action::Visit; }

SyntaxVisitor::Action Disambiguator::visitStorageClass(const StorageClassSyntax* node) { return Action::Visit; }

SyntaxVisitor::Action Disambiguator::visitBuiltinTypeSpecifier(const BuiltinTypeSpecifierSyntax* node) { return Action::Visit; }

SyntaxVisitor::Action Disambiguator::visitTagTypeSpecifier(const TagTypeSpecifierSyntax* node) { return Action::Visit; }

SyntaxVisitor::Action Disambiguator::visitAtomicTypeSpecifier(const AtomicTypeSpecifierSyntax* node) { return Action::Visit; }

SyntaxVisitor::Action Disambiguator::visitTypeDeclarationAsSpecifier(const TypeDeclarationAsSpecifierSyntax* node) { return Action::Visit; }

SyntaxVisitor::Action Disambiguator::visitTypedefName(const TypedefNameSyntax* node) { return Action::Visit; }

SyntaxVisitor::Action Disambiguator::visitTypeQualifier(const TypeQualifierSyntax* node) { return Action::Visit; }

SyntaxVisitor::Action Disambiguator::visitFunctionSpecifier(const FunctionSpecifierSyntax* node) { return Action::Visit; }

SyntaxVisitor::Action Disambiguator::visitAlignmentSpecifier(const AlignmentSpecifierSyntax* node) { return Action::Visit; }

SyntaxVisitor::Action Disambiguator::visitExtGNU_Typeof(const ExtGNU_TypeofSyntax* node) { return Action::Visit; }

SyntaxVisitor::Action Disambiguator::visitExtGNU_AttributeSpecifier(const ExtGNU_AttributeSpecifierSyntax* node) { return Action::Visit; }

SyntaxVisitor::Action Disambiguator::visitExtGNU_Attribute(const ExtGNU_AttributeSyntax* node) { return Action::Visit; }

SyntaxVisitor::Action Disambiguator::visitExtGNU_AsmLabel(const ExtGNU_AsmLabelSyntax* node) { return Action::Visit; }

SyntaxVisitor::Action Disambiguator::visitExtPSY_QuantifiedTypeSpecifier(const ExtPSY_QuantifiedTypeSpecifierSyntax* node) { return Action::Visit; }

SyntaxVisitor::Action Disambiguator::visitArrayOrFunctionDeclarator(const ArrayOrFunctionDeclaratorSyntax* node) { return Action::Visit; }

SyntaxVisitor::Action Disambiguator::visitPointerDeclarator(const PointerDeclaratorSyntax* node) { return Action::Visit; }

SyntaxVisitor::Action Disambiguator::visitParenthesizedDeclarator(const ParenthesizedDeclaratorSyntax* node) { return Action::Visit; }

SyntaxVisitor::Action Disambiguator::visitIdentifierDeclarator(const IdentifierDeclaratorSyntax* node) { return Action::Visit; }

SyntaxVisitor::Action Disambiguator::visitAbstractDeclarator(const AbstractDeclaratorSyntax* node) { return Action::Visit; }

SyntaxVisitor::Action Disambiguator::visitSubscriptSuffix(const SubscriptSuffixSyntax* node) { return Action::Visit; }

SyntaxVisitor::Action Disambiguator::visitParameterSuffix(const ParameterSuffixSyntax* node) { return Action::Visit; }

SyntaxVisitor::Action Disambiguator::visitBitfieldDeclarator(const BitfieldDeclaratorSyntax* node) { return Action::Visit; }

SyntaxVisitor::Action Disambiguator::visitExpressionInitializer(const ExpressionInitializerSyntax* node) { return Action::Visit; }

SyntaxVisitor::Action Disambiguator::visitBraceEnclosedInitializer(const BraceEnclosedInitializerSyntax* node) { return Action::Visit; }

SyntaxVisitor::Action Disambiguator::visitDesignatedInitializer(const DesignatedInitializerSyntax* node) { return Action::Visit; }

SyntaxVisitor::Action Disambiguator::visitFieldDesignator(const FieldDesignatorSyntax* node) { return Action::Visit; }

SyntaxVisitor::Action Disambiguator::visitArrayDesignator(const ArrayDesignatorSyntax* node) { return Action::Visit; }

SyntaxVisitor::Action Disambiguator::visitOffsetOfDesignator(const OffsetOfDesignatorSyntax* node) { return Action::Visit; }

SyntaxVisitor::Action Disambiguator::visitIdentifierName(const IdentifierNameSyntax* node) { return Action::Visit; }

SyntaxVisitor::Action Disambiguator::visitPredefinedName(const PredefinedNameSyntax* node) { return Action::Visit; }

SyntaxVisitor::Action Disambiguator::visitConstantExpression(const ConstantExpressionSyntax* node) { return Action::Visit; }

SyntaxVisitor::Action Disambiguator::visitStringLiteralExpression(const StringLiteralExpressionSyntax* node) { return Action::Visit; }

SyntaxVisitor::Action Disambiguator::visitParenthesizedExpression(const ParenthesizedExpressionSyntax* node) { return Action::Visit; }

SyntaxVisitor::Action Disambiguator::visitGenericSelectionExpression(const GenericSelectionExpressionSyntax* node) { return Action::Visit; }

SyntaxVisitor::Action Disambiguator::visitGenericAssociation(const GenericAssociationSyntax* node) { return Action::Visit; }

SyntaxVisitor::Action Disambiguator::visitExtGNU_EnclosedCompoundStatementExpression(const ExtGNU_EnclosedCompoundStatementExpressionSyntax* node) { return Action::Visit; }

SyntaxVisitor::Action Disambiguator::visitExtGNU_ComplexValuedExpression(const ExtGNU_ComplexValuedExpressionSyntax* node) { return Action::Visit; }

SyntaxVisitor::Action Disambiguator::visitPrefixUnaryExpression(const PrefixUnaryExpressionSyntax* node) { return Action::Visit; }

SyntaxVisitor::Action Disambiguator::visitPostfixUnaryExpression(const PostfixUnaryExpressionSyntax* node) { return Action::Visit; }

SyntaxVisitor::Action Disambiguator::visitMemberAccessExpression(const MemberAccessExpressionSyntax* node) { return Action::Visit; }

SyntaxVisitor::Action Disambiguator::visitArraySubscriptExpression(const ArraySubscriptExpressionSyntax* node) { return Action::Visit; }

SyntaxVisitor::Action Disambiguator::visitTypeTraitExpression(const TypeTraitExpressionSyntax* node) { return Action::Visit; }

SyntaxVisitor::Action Disambiguator::visitCastExpression(const CastExpressionSyntax* node) { return Action::Visit; }

SyntaxVisitor::Action Disambiguator::visitCallExpression(const CallExpressionSyntax* node) { return Action::Visit; }

SyntaxVisitor::Action Disambiguator::visitVAArgumentExpression(const VAArgumentExpressionSyntax* node) { return Action::Visit; }

SyntaxVisitor::Action Disambiguator::visitOffsetOfExpression(const OffsetOfExpressionSyntax* node) { return Action::Visit; }

SyntaxVisitor::Action Disambiguator::visitCompoundLiteralExpression(const CompoundLiteralExpressionSyntax* node) { return Action::Visit; }

SyntaxVisitor::Action Disambiguator::visitBinaryExpression(const BinaryExpressionSyntax* node) { return Action::Visit; }

SyntaxVisitor::Action Disambiguator::visitConditionalExpression(const ConditionalExpressionSyntax* node) { return Action::Visit; }

SyntaxVisitor::Action Disambiguator::visitAssignmentExpression(const AssignmentExpressionSyntax* node) { return Action::Visit; }

SyntaxVisitor::Action Disambiguator::visitSequencingExpression(const SequencingExpressionSyntax* node) { return Action::Visit; }

SyntaxVisitor::Action Disambiguator::visitExtGNU_ChooseExpression(const ExtGNU_ChooseExpressionSyntax* node) { return Action::Visit; }

//------------//
// Statements //
//------------//

SyntaxVisitor::Action Disambiguator::visitCompoundStatement(const CompoundStatementSyntax* node)
{
    for (auto iter = node->stmts_; iter; iter = iter->next) {
        switch (iter->value->kind()) {
            case AmbiguousMultiplicationOrPointerDeclaration:
            case AmbiguousCallOrVariableDeclaration: {
                auto iter_P = const_cast<StatementListSyntax*>(iter);
                auto ambigNode = iter->value->asAmbiguousExpressionOrDeclarationStatement();
                auto disambig = disambiguateAmbiguousExpressionOrDeclarationStatement(ambigNode);
                switch (disambig) {
                    case Disambiguation::DeclarationStatement:
                        iter_P->value = ambigNode->declStmt_;
                        break;

                    case Disambiguation::ExpressionStatement:
                        iter_P->value = ambigNode->exprStmt_;
                        break;

                    case Disambiguation::Undetermined:
                        ++pendingAmbigs_;
                        break;

                    default:
                        PSY_ESCAPE_VIA_RETURN(Action::Skip);
                }
                break;
            }

            default:
                visit(iter->value);
                break;
        }
    }

    return Action::Skip;
}

SyntaxVisitor::Action Disambiguator::visitDeclarationStatement(const DeclarationStatementSyntax* node) { return Action::Visit; }

SyntaxVisitor::Action Disambiguator::visitExpressionStatement(const ExpressionStatementSyntax* node)
{
    switch (node->expr_->kind()) {
        case AmbiguousCastOrBinaryExpression: {
            auto node_P = const_cast<ExpressionStatementSyntax*>(node);
            auto ambigNode = node_P->expr_->asAmbiguousCastOrBinaryExpression();
            auto disambig = disambiguateAmbiguousCastOrBinaryExpressionSyntax(ambigNode);
            switch (disambig) {
                case Disambiguation::CastExpression:
                    node_P->expr_ = ambigNode->castExpr_;
                    break;

                case Disambiguation::ExpressionStatement:
                    node_P->expr_ = ambigNode->binExpr_;
                    break;

                case Disambiguation::Undetermined:
                    ++pendingAmbigs_;
                    break;

                default:
                    PSY_ESCAPE_VIA_RETURN(Action::Skip);
            }
            break;
        }

        default:
            visit(node->expr_);
            break;
    }

    return Action::Skip;
}

SyntaxVisitor::Action Disambiguator::visitLabeledStatement(const LabeledStatementSyntax* node) { return Action::Visit; }

SyntaxVisitor::Action Disambiguator::visitIfStatement(const IfStatementSyntax* node) { return Action::Visit; }

SyntaxVisitor::Action Disambiguator::visitSwitchStatement(const SwitchStatementSyntax* node) { return Action::Visit; }

SyntaxVisitor::Action Disambiguator::visitWhileStatement(const WhileStatementSyntax* node) { return Action::Visit; }

SyntaxVisitor::Action Disambiguator::visitDoStatement(const DoStatementSyntax* node) { return Action::Visit; }

SyntaxVisitor::Action Disambiguator::visitForStatement(const ForStatementSyntax* node) { return Action::Visit; }

SyntaxVisitor::Action Disambiguator::visitGotoStatement(const GotoStatementSyntax* node) { return Action::Visit; }

SyntaxVisitor::Action Disambiguator::visitContinueStatement(const ContinueStatementSyntax* node) { return Action::Visit; }

SyntaxVisitor::Action Disambiguator::visitBreakStatement(const BreakStatementSyntax* node) { return Action::Visit; }

SyntaxVisitor::Action Disambiguator::visitReturnStatement(const ReturnStatementSyntax* node) { return Action::Visit; }

SyntaxVisitor::Action Disambiguator::visitExtGNU_AsmStatement(const ExtGNU_AsmStatementSyntax* node) { return Action::Visit; }

SyntaxVisitor::Action Disambiguator::visitExtGNU_AsmQualifier(const ExtGNU_AsmQualifierSyntax* node) { return Action::Visit; }

SyntaxVisitor::Action Disambiguator::visitExtGNU_AsmOperand(const ExtGNU_AsmOperandSyntax* node) { return Action::Visit; }

SyntaxVisitor::Action Disambiguator::visitTypeName(const TypeNameSyntax* node) { return Action::Visit; }

SyntaxVisitor::Action Disambiguator::visitExpressionAsTypeReference(const ExpressionAsTypeReferenceSyntax* node) { return Action::Visit; }

SyntaxVisitor::Action Disambiguator::visitTypeNameAsTypeReference(const TypeNameAsTypeReferenceSyntax* node) { return Action::Visit; }

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
