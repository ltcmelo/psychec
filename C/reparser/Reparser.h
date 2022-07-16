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

class PSY_C_NON_API Reparser : public SyntaxVisitor
{
public:
    virtual ~Reparser();

protected:
    Reparser(SyntaxTree* tree);

    virtual unsigned int reparse() = 0;

    enum class ExpressionOrDeclarationStatement : std::int8_t
    {
        Expression,
        Declaration
    };

    virtual ExpressionOrDeclarationStatement keepExpressionOrDeclarationStatement(
            const std::string& maybeTyName) = 0;

    //--------------//
    // Declarations //
    //--------------//
    Action visitTranslationUnit(const TranslationUnitSyntax*) override;
    Action visitIncompleteDeclaration(const IncompleteDeclarationSyntax*) override;
    Action visitStructOrUnionDeclaration(const StructOrUnionDeclarationSyntax*) override;
    Action visitEnumDeclaration(const EnumDeclarationSyntax*) override;
    Action visitEnumeratorDeclaration(const EnumeratorDeclarationSyntax*) override;
    Action visitVariableAndOrFunctionDeclaration(const VariableAndOrFunctionDeclarationSyntax*) override;
    Action visitFieldDeclaration(const FieldDeclarationSyntax*) override;
    Action visitParameterDeclaration(const ParameterDeclarationSyntax*) override;
    Action visitStaticAssertDeclaration(const StaticAssertDeclarationSyntax*) override;
    Action visitFunctionDefinition(const FunctionDefinitionSyntax*) override;
    Action visitExtPSY_TemplateDeclaration(const ExtPSY_TemplateDeclarationSyntax*) override;
    Action visitExtGNU_AsmStatementDeclaration(const ExtGNU_AsmStatementDeclarationSyntax*) override;
    Action visitExtKR_ParameterDeclaration(const ExtKR_ParameterDeclarationSyntax*) override;

    /* Specifiers */
    Action visitStorageClass(const StorageClassSyntax*) override;
    Action visitBuiltinTypeSpecifier(const BuiltinTypeSpecifierSyntax*) override;
    Action visitTagTypeSpecifier(const TagTypeSpecifierSyntax*) override;
    Action visitAtomicTypeSpecifier(const AtomicTypeSpecifierSyntax*) override;
    Action visitTypeDeclarationAsSpecifier(const TypeDeclarationAsSpecifierSyntax*) override;
    Action visitTypedefName(const TypedefNameSyntax*) override;
    Action visitTypeQualifier(const TypeQualifierSyntax*) override;
    Action visitFunctionSpecifier(const FunctionSpecifierSyntax*) override;
    Action visitAlignmentSpecifier(const AlignmentSpecifierSyntax*) override;
    Action visitExtGNU_Typeof(const ExtGNU_TypeofSyntax*) override;
    Action visitExtGNU_AttributeSpecifier(const ExtGNU_AttributeSpecifierSyntax*) override;
    Action visitExtGNU_Attribute(const ExtGNU_AttributeSyntax*) override;
    Action visitExtGNU_AsmLabel(const ExtGNU_AsmLabelSyntax*) override;
    Action visitExtPSY_QuantifiedTypeSpecifier(const ExtPSY_QuantifiedTypeSpecifierSyntax*) override;

    /* Declarators */
    Action visitArrayOrFunctionDeclarator(const ArrayOrFunctionDeclaratorSyntax*) override;
    Action visitPointerDeclarator(const PointerDeclaratorSyntax*) override;
    Action visitParenthesizedDeclarator(const ParenthesizedDeclaratorSyntax*) override;
    Action visitIdentifierDeclarator(const IdentifierDeclaratorSyntax*) override;
    Action visitAbstractDeclarator(const AbstractDeclaratorSyntax*) override;
    Action visitSubscriptSuffix(const SubscriptSuffixSyntax*) override;
    Action visitParameterSuffix(const ParameterSuffixSyntax*) override;
    Action visitBitfieldDeclarator(const BitfieldDeclaratorSyntax*) override;

    /* Initializers */
    Action visitExpressionInitializer(const ExpressionInitializerSyntax*) override;
    Action visitBraceEnclosedInitializer(const BraceEnclosedInitializerSyntax*) override;
    Action visitDesignatedInitializer(const DesignatedInitializerSyntax*) override;
    Action visitFieldDesignator(const FieldDesignatorSyntax*) override;
    Action visitArrayDesignator(const ArrayDesignatorSyntax*) override;

    Action visitOffsetOfDesignator(const OffsetOfDesignatorSyntax*) override;

    //-------------//
    // Expressions //
    //-------------//
    Action visitIdentifierName(const IdentifierNameSyntax*) override;
    Action visitPredefinedName(const PredefinedNameSyntax*) override;
    Action visitConstantExpression(const ConstantExpressionSyntax*) override;
    Action visitStringLiteralExpression(const StringLiteralExpressionSyntax*) override;
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
    Action visitCompoundStatement(const CompoundStatementSyntax*) override;
    Action visitDeclarationStatement(const DeclarationStatementSyntax*) override;
    Action visitExpressionStatement(const ExpressionStatementSyntax*) override;
    Action visitLabeledStatement(const LabeledStatementSyntax*) override;
    Action visitIfStatement(const IfStatementSyntax*) override;
    Action visitSwitchStatement(const SwitchStatementSyntax*) override;
    Action visitWhileStatement(const WhileStatementSyntax*) override;
    Action visitDoStatement(const DoStatementSyntax*) override;
    Action visitForStatement(const ForStatementSyntax*) override;
    Action visitGotoStatement(const GotoStatementSyntax*) override;
    Action visitContinueStatement(const ContinueStatementSyntax*) override;
    Action visitBreakStatement(const BreakStatementSyntax*) override;
    Action visitReturnStatement(const ReturnStatementSyntax*) override;
    Action visitExtGNU_AsmStatement(const ExtGNU_AsmStatementSyntax*) override;
    Action visitExtGNU_AsmQualifier(const ExtGNU_AsmQualifierSyntax*) override;
    Action visitExtGNU_AsmOperand(const ExtGNU_AsmOperandSyntax*) override;

    //--------//
    // Common //
    //--------//
    Action visitTypeName(const TypeNameSyntax*) override;
    Action visitExpressionAsTypeReference(const ExpressionAsTypeReferenceSyntax*) override;
    Action visitTypeNameAsTypeReference(const TypeNameAsTypeReferenceSyntax*) override;

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
