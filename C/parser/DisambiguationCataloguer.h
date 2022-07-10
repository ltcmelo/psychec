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

#ifndef PSYCHE_C_DISAMBIGUATION_CATALOGUER_H__
#define PSYCHE_C_DISAMBIGUATION_CATALOGUER_H__

#include "API.h"

#include "syntax/SyntaxVisitor.h"

#include <ostream>

namespace psy {
namespace C {

class PSY_C_NON_API DisambiguationCataloguer : public SyntaxVisitor
{
public:
    DisambiguationCataloguer(SyntaxTree* tree);

protected:
    using Base = SyntaxVisitor;

    //--------------//
    // Declarations //
    //--------------//

    /* Declarators */
    Action visitIdentifierDeclarator(const IdentifierDeclaratorSyntax*) override;

    //-------------//
    // Expressions //
    //-------------//

    Action visitIdentifierName(const IdentifierNameSyntax*) override;
    Action visitParenthesizedExpression(const ParenthesizedExpressionSyntax*) override;
    Action visitGenericSelectionExpression(const GenericSelectionExpressionSyntax*) override;
    Action visitGenericAssociation(const GenericAssociationSyntax*) override;
    Action visitExtGNU_EnclosedCompoundStatementExpression(const ExtGNU_EnclosedCompoundStatementExpressionSyntax*) override;
    Action visitExtGNU_ComplexValuedExpression(const ExtGNU_ComplexValuedExpressionSyntax*) override;

    /* Operations */
    Action visitPrefixUnaryExpression(const PrefixUnaryExpressionSyntax*) override;
    Action visitPostfixUnaryExpression(const PostfixUnaryExpressionSyntax*) override;
    Action visitMemberAccessExpression(const MemberAccessExpressionSyntax*) override;
    Action visitArraySubscriptExpression(const ArraySubscriptExpressionSyntax*) override;
    Action visitTypeTraitExpression(const TypeTraitExpressionSyntax*) override;
    Action visitCastExpression(const CastExpressionSyntax*) override;
    Action visitCallExpression(const CallExpressionSyntax*) override;
    Action visitVAArgumentExpression(const VAArgumentExpressionSyntax*) override;
    Action visitOffsetOfExpression(const OffsetOfExpressionSyntax*) override;
    Action visitCompoundLiteralExpression(const CompoundLiteralExpressionSyntax*) override;
    Action visitBinaryExpression(const BinaryExpressionSyntax*) override;
    Action visitConditionalExpression(const ConditionalExpressionSyntax*) override;
    Action visitAssignmentExpression(const AssignmentExpressionSyntax*) override;
    Action visitSequencingExpression(const SequencingExpressionSyntax*) override;
    Action visitExtGNU_ChooseExpression(const ExtGNU_ChooseExpressionSyntax*) override;

    //------------//
    // Statements //
    //------------//

    Action visitDeclarationStatement(const DeclarationStatementSyntax*) override;
    Action visitExpressionStatement(const ExpressionStatementSyntax*) override;
    Action visitIfStatement(const IfStatementSyntax*) override;
    Action visitSwitchStatement(const SwitchStatementSyntax*) override;
    Action visitWhileStatement(const WhileStatementSyntax*) override;
    Action visitDoStatement(const DoStatementSyntax*) override;
    Action visitForStatement(const ForStatementSyntax*) override;
    Action visitReturnStatement(const ReturnStatementSyntax*) override;
    Action visitExtGNU_AsmStatement(const ExtGNU_AsmStatementSyntax*) override;
    Action visitExtGNU_AsmOperand(const ExtGNU_AsmOperandSyntax*) override;

    //--------//
    // Common //
    //--------//

    Action visitExpressionAsTypeReference(const ExpressionAsTypeReferenceSyntax*) override;

    //-------------//
    // Ambiguities //
    //-------------//

    Action visitAmbiguousTypeNameOrExpressionAsTypeReference(const AmbiguousTypeNameOrExpressionAsTypeReferenceSyntax*) override;
    Action visitAmbiguousCastOrBinaryExpression(const AmbiguousCastOrBinaryExpressionSyntax*) override;
    Action visitAmbiguousExpressionOrDeclarationStatement(const AmbiguousExpressionOrDeclarationStatementSyntax*) override;
};

} // C
} // psy

#endif
