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

#ifndef PSYCHE_C_REPARSER_H__
#define PSYCHE_C_REPARSER_H__

#include "API.h"

#include "syntax/SyntaxVisitor.h"

#include "../common/infra/InternalAccess.h"

#include <cstdint>
#include <string>

namespace psy {
namespace C {

class PSY_C_NON_API Disambiguator : public SyntaxVisitor
{
public:
    virtual ~Disambiguator();

PSY_INTERNAL_AND_RESTRICTED:
    PSY_GRANT_ACCESS(Reparser);

    unsigned int disambiguate();

PSY_INTERNAL_AND_EXTENSIBLE:
    Disambiguator(SyntaxTree* tree);

    enum class Disambiguation : std::int8_t
    {
        Inconclusive,
        KeepCastExpression,
        KeepBinaryExpression,
        KeepExpressionStatement,
        KeepDeclarationStatement,
        KeepTypeName,
        KeepExpression,
    };

    virtual Disambiguation disambiguateExpression(const AmbiguousCastOrBinaryExpressionSyntax*) const = 0;
    virtual Disambiguation disambiguateStatement(const AmbiguousExpressionOrDeclarationStatementSyntax*) const = 0;
    virtual Disambiguation disambiguateTypeReference(const AmbiguousTypeNameOrExpressionAsTypeReferenceSyntax*) const = 0;

private:
    template <class ExprT> Action visitMaybeAmbiguousExpression(ExprT* const&);
    template <class StmtT> Action visitMaybeAmbiguousStatement(StmtT* const&);
    template <class TypeRefT> Action visitMaybeAmbiguousTypeReference(TypeRefT* const&);

    unsigned int pendingAmbigs_;

protected:
    //--------------//
    // Declarations //
    //--------------//
    Action visitStaticAssertDeclaration(const StaticAssertDeclarationSyntax*) override;

    /* Specifiers */
    Action visitAlignmentSpecifier(const AlignmentSpecifierSyntax*) override;
    Action visitExtGNU_Typeof(const ExtGNU_TypeofSyntax*) override;

    /* Declarators */
    Action visitSubscriptSuffix(const SubscriptSuffixSyntax*) override;

    /* Initializers */
    Action visitExpressionInitializer(const ExpressionInitializerSyntax*) override;
    Action visitArrayDesignator(const ArrayDesignatorSyntax*) override;

    //-------------//
    // Expressions //
    //-------------//
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
    Action visitBinaryExpression(const BinaryExpressionSyntax*) override;
    Action visitConditionalExpression(const ConditionalExpressionSyntax*) override;
    Action visitAssignmentExpression(const AssignmentExpressionSyntax*) override;
    Action visitSequencingExpression(const SequencingExpressionSyntax*) override;
    Action visitExtGNU_ChooseExpression(const ExtGNU_ChooseExpressionSyntax*) override;

    //------------//
    // Statements //
    //------------//
    Action visitCompoundStatement(const CompoundStatementSyntax*) override;
    Action visitDeclarationStatement(const DeclarationStatementSyntax*) override;
    Action visitExpressionStatement(const ExpressionStatementSyntax*) override;
    Action visitLabeledStatement(const LabeledStatementSyntax*) override;
    Action visitIfStatement(const IfStatementSyntax*) override;
    Action visitSwitchStatement(const SwitchStatementSyntax*) override;
    Action visitWhileStatement(const WhileStatementSyntax*) override;
    Action visitDoStatement(const DoStatementSyntax*) override;
    Action visitForStatement(const ForStatementSyntax*) override;
    Action visitReturnStatement(const ReturnStatementSyntax*) override;
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
