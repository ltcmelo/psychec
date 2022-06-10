// Copyright (c) 2016/17/18/19/20/21/22 Leandro T. C. Melo <ltcmelo@gmail.com>
// Copyright (c) 2008 Roberto Raggi <roberto.raggi@gmail.com>
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

#ifndef PSYCHE_C_SYNTAX_NODE_H__
#define PSYCHE_C_SYNTAX_NODE_H__

#include "API.h"
#include "Fwds.h"

#include "SyntaxKind.h"
#include "SyntaxNodeList.h"
#include "SyntaxToken.h"
#include "SyntaxVisitor.h"
#include "SyntaxHolder.h"

#include "infra/Managed.h"
#include "parser/LexedTokens.h"

#include <iostream>
#include <memory>
#include <variant>
#include <vector>

namespace psy {
namespace C {

/**
 * \brief The SyntaxNode class.
 *
 * The base class of every AST node.
 *
 * \note
 * This API is inspired by that of \c Microsoft.CodeAnalysis.SyntaxNode
 * from Roslyn, the .NET Compiler Platform.
 */
class PSY_C_API SyntaxNode : public Managed
{
public:
    virtual ~SyntaxNode();

    /**
     * The SyntaxTree to which \c this SyntaxNode belongs to.
     */
    const SyntaxTree* syntaxTree() const;

    /**
     * The SyntaxKind of \c this SyntaxNode.
     */
    SyntaxKind kind() const;

    /**
     * Whether \c this SyntaxNode is of SyntaxKind \p k.
     */
    bool isKind(SyntaxKind k) { return kind() == k; }

    /**
     * The first token of \c this SyntaxNode.
     */
    SyntaxToken firstToken() const;

    /**
     * The last token of \c this SyntaxNode.
     */
    SyntaxToken lastToken() const;

    //!@{
    /**
     * Accept \c this SyntaxNode for traversal by the given \p visitor.
     */
    void acceptVisitor(SyntaxVisitor* visitor) const;

    /**
     * Accept the SyntaxNode \p node for traversal by the given \p visitor.
     */
    static void acceptVisitor(const SyntaxNode* node, SyntaxVisitor* visitor)
    {
        if (node)
            node->acceptVisitor(visitor);
    }

    /**
     * Accept the syntax list \p it for traversal by the given \p visitor.
     */
    template <class PtrT, class DerivedListT>
    static void acceptVisitor(List<PtrT, DerivedListT>* it, SyntaxVisitor* visitor)
    {
        for (; it; it = it->next)
            acceptVisitor(it->value, visitor);
    }
    //!@}

    //--------------//
    // Declarations //
    //--------------//
    virtual TranslationUnitSyntax* asTranslationUnit() { return nullptr; }
    virtual const TranslationUnitSyntax* asTranslationUnit() const { return nullptr; }
    virtual DeclarationSyntax* asDeclaration() { return nullptr; }
    virtual const DeclarationSyntax* asDeclaration() const { return nullptr; }
    virtual IncompleteDeclarationSyntax* asIncompleteDeclaration() { return nullptr; }
    virtual const IncompleteDeclarationSyntax* asIncompleteDeclaration() const { return nullptr; }
    virtual NamedDeclarationSyntax* asNamedDeclaration() { return nullptr; }
    virtual const NamedDeclarationSyntax* asNamedDeclaration() const { return nullptr; }
    virtual TypeDeclarationSyntax* asTypeDeclaration() { return nullptr; }
    virtual const TypeDeclarationSyntax* asTypeDeclaration() const { return nullptr; }
    virtual TagDeclarationSyntax* asTagDeclaration() { return nullptr; }
    virtual const TagDeclarationSyntax* asTagDeclaration() const { return nullptr; }
    virtual StructOrUnionDeclarationSyntax* asStructOrUnionDeclaration() { return nullptr; }
    virtual const StructOrUnionDeclarationSyntax* asStructOrUnionDeclaration() const { return nullptr; }
    virtual EnumDeclarationSyntax* asEnumDeclaration() { return nullptr; }
    virtual const EnumDeclarationSyntax* asEnumDeclaration() const { return nullptr; }
    virtual EnumeratorDeclarationSyntax* asEnumeratorDeclaration() { return nullptr; }
    virtual const EnumeratorDeclarationSyntax* asEnumeratorDeclaration() const { return nullptr; }
    virtual ValueDeclarationSyntax* asValueDeclaration() { return nullptr; }
    virtual const ValueDeclarationSyntax* asValueDeclaration() const { return nullptr; }
    virtual DeclaratorDeclarationSyntax* asDeclaratorDeclaration() { return nullptr; }
    virtual const DeclaratorDeclarationSyntax* asDeclaratorDeclaration() const { return nullptr; }
    virtual VariableAndOrFunctionDeclarationSyntax* asVariableAndOrFunctionDeclaration() { return nullptr; }
    virtual const VariableAndOrFunctionDeclarationSyntax* asVariableAndOrFunctionDeclaration() const { return nullptr; }
    virtual ParameterDeclarationSyntax* asParameterDeclaration() { return nullptr; }
    virtual const ParameterDeclarationSyntax* asParameterDeclaration() const { return nullptr; }
    virtual FieldDeclarationSyntax* asFieldDeclaration() { return nullptr; }
    virtual const FieldDeclarationSyntax* asFieldDeclaration() const { return nullptr; }
    virtual StaticAssertDeclarationSyntax* asStaticAssertDeclaration() { return nullptr; }
    virtual const StaticAssertDeclarationSyntax* asStaticAssertDeclaration() const { return nullptr; }
    virtual FunctionDefinitionSyntax* asFunctionDefinition() { return nullptr; }
    virtual const FunctionDefinitionSyntax* asFunctionDefinition() const { return nullptr; }
    virtual ExtPSY_TemplateDeclarationSyntax* asExtPSY_TemplateDeclaration() { return nullptr; }
    virtual const ExtPSY_TemplateDeclarationSyntax* asExtPSY_TemplateDeclaration() const { return nullptr; }
    virtual ExtGNU_AsmStatementDeclarationSyntax* asExtGNU_AsmStatementDeclaration() { return nullptr; }
    virtual const ExtGNU_AsmStatementDeclarationSyntax* asExtGNU_AsmStatementDeclaration() const { return nullptr; }
    virtual ExtKR_ParameterDeclarationSyntax* asExtKR_ParameterDeclaration() { return nullptr; }
    virtual const ExtKR_ParameterDeclarationSyntax* asExtKR_ParameterDeclaration() const { return nullptr; }

    /* Specifiers */
    virtual SpecifierSyntax* asSpecifier() { return nullptr; }
    virtual const SpecifierSyntax* asSpecifier() const { return nullptr; }
    virtual TrivialSpecifierSyntax* asTrivialSpecifier() { return nullptr; }
    virtual const TrivialSpecifierSyntax* asTrivialSpecifier() const { return nullptr; }
    virtual StorageClassSyntax* asStorageClass() { return nullptr; }
    virtual const StorageClassSyntax* asStorageClass() const { return nullptr; }
    virtual BuiltinTypeSpecifierSyntax* asBuiltinTypeSpecifier() { return nullptr; }
    virtual const BuiltinTypeSpecifierSyntax* asBuiltinTypeSpecifier() const { return nullptr; }
    virtual TagTypeSpecifierSyntax* asTagTypeSpecifier() { return nullptr; }
    virtual const TagTypeSpecifierSyntax* asTagTypeSpecifier() const { return nullptr; }
    virtual TypeDeclarationAsSpecifierSyntax* asTypeDeclarationAsSpecifier() { return nullptr; }
    virtual AtomicTypeSpecifierSyntax* asAtomicTypeSpecifier() { return nullptr; }
    virtual const AtomicTypeSpecifierSyntax* asAtomicTypeSpecifier() const { return nullptr; }
    virtual const TypeDeclarationAsSpecifierSyntax* asTypeDeclarationAsSpecifier() const { return nullptr; }
    virtual TypeQualifierSyntax* asTypeQualifier() { return nullptr; }
    virtual const TypeQualifierSyntax* asTypeQualifier() const { return nullptr; }
    virtual FunctionSpecifierSyntax* asFunctionSpecifier() { return nullptr; }
    virtual const FunctionSpecifierSyntax* asFunctionSpecifier() const { return nullptr; }
    virtual AlignmentSpecifierSyntax* asAlignmentSpecifier() { return nullptr; }
    virtual const AlignmentSpecifierSyntax* asAlignmentSpecifier() const { return nullptr; }
    virtual TypedefNameSyntax* asTypedefName() { return nullptr; }
    virtual const TypedefNameSyntax* asTypedefName() const { return nullptr; }
    virtual ExtGNU_AttributeSpecifierSyntax* asExtGNU_AttributeSpecifier() { return nullptr; }
    virtual const ExtGNU_AttributeSpecifierSyntax* asExtGNU_AttributeSpecifier() const { return nullptr; }
    virtual ExtGNU_AttributeSyntax* asExtGNU_Attribute() { return nullptr; }
    virtual const ExtGNU_AttributeSyntax* asExtGNU_Attribute() const { return nullptr; }
    virtual ExtGNU_AsmLabelSyntax* asExtGNU_AsmLabel() { return nullptr; }
    virtual const ExtGNU_AsmLabelSyntax* asExtGNU_AsmLabel() const { return nullptr; }
    virtual ExtGNU_TypeofSyntax* asExtGNU_Typeof() { return nullptr; }
    virtual const ExtGNU_TypeofSyntax* asExtGNU_Typeof() const { return nullptr; }
    virtual ExtPSY_QuantifiedTypeSpecifierSyntax* asExtPSY_QuantifiedTypeSpecifier() { return nullptr; }
    virtual const ExtPSY_QuantifiedTypeSpecifierSyntax* asExtPSY_QuantifiedTypeSpecifier() const { return nullptr; }

    /* Declarators */
    virtual DeclaratorSyntax* asDeclarator() { return nullptr; }
    virtual const DeclaratorSyntax* asDeclarator() const { return nullptr; }
    virtual PointerDeclaratorSyntax* asPointerDeclarator() { return nullptr; }
    virtual const PointerDeclaratorSyntax* asPointerDeclarator() const { return nullptr; }
    virtual IdentifierDeclaratorSyntax* asIdentifierDeclarator() { return nullptr; }
    virtual const IdentifierDeclaratorSyntax* asIdentifierDeclarator() const { return nullptr; }
    virtual AbstractDeclaratorSyntax* asAbstractDeclarator() { return nullptr; }
    virtual const AbstractDeclaratorSyntax* asAbstractDeclarator() const { return nullptr; }
    virtual ParenthesizedDeclaratorSyntax* asParenthesizedDeclarator() { return nullptr; }
    virtual const ParenthesizedDeclaratorSyntax* asParenthesizedDeclarator() const { return nullptr; }
    virtual ArrayOrFunctionDeclaratorSyntax* asArrayOrFunctionDeclarator() { return nullptr; }
    virtual const ArrayOrFunctionDeclaratorSyntax* asArrayOrFunctionDeclarator() const { return nullptr; }
    virtual DeclaratorSuffixSyntax* asDeclaratorSuffix() { return nullptr; }
    virtual const DeclaratorSuffixSyntax* asDeclaratorSuffix() const { return nullptr; }
    virtual SubscriptSuffixSyntax* asSubscriptSuffix() { return nullptr; }
    virtual const SubscriptSuffixSyntax* asSubscriptSuffix() const { return nullptr; }
    virtual ParameterSuffixSyntax* asParameterSuffix() { return nullptr; }
    virtual const ParameterSuffixSyntax* asParameterSuffix() const { return nullptr; }
    virtual BitfieldDeclaratorSyntax* asBitfieldDeclarator() { return  nullptr; }
    virtual const BitfieldDeclaratorSyntax* asBitfieldDeclarator() const { return  nullptr; }

    /* Initializers */
    virtual InitializerSyntax* asInitializer() { return nullptr; }
    virtual const InitializerSyntax* asInitializer() const { return nullptr; }
    virtual ExpressionInitializerSyntax* asExpressionInitializer() { return nullptr; }
    virtual const ExpressionInitializerSyntax* asExpressionInitializer() const { return nullptr; }
    virtual BraceEnclosedInitializerSyntax* asBraceEnclosedInitializer() { return nullptr; }
    virtual const BraceEnclosedInitializerSyntax* asBraceEnclosedInitializer() const { return nullptr; }
    virtual DesignatedInitializerSyntax* asDesignatedInitializer() { return nullptr; }
    virtual const DesignatedInitializerSyntax* asDesignatedInitializer() const { return nullptr; }
    virtual DesignatorSyntax* asDesignator() { return nullptr; }
    virtual const DesignatorSyntax* asDesignator() const { return nullptr; }
    virtual ArrayDesignatorSyntax* asArrayDesignator() { return nullptr; }
    virtual const ArrayDesignatorSyntax* asArrayDesignator() const { return nullptr; }
    virtual FieldDesignatorSyntax* asFieldDesignator() { return nullptr; }
    virtual const FieldDesignatorSyntax* asFieldDesignator() const { return nullptr; }

    virtual OffsetOfDesignatorSyntax* asOffsetOfDesignator() { return nullptr; }
    virtual const OffsetOfDesignatorSyntax* asOffsetOfDesignator() const { return nullptr; }

    //-------------//
    // Expressions //
    //-------------//
    virtual ExpressionSyntax* asExpression() { return nullptr; }
    virtual const ExpressionSyntax* asExpression() const { return nullptr; }
    virtual ConstantExpressionSyntax* asConstantExpression() { return nullptr; }
    virtual const ConstantExpressionSyntax* asConstantExpression() const { return nullptr; }
    virtual StringLiteralExpressionSyntax* asStringLiteralExpression() { return nullptr; }
    virtual const StringLiteralExpressionSyntax* asStringLiteralExpression() const { return nullptr; }
    virtual ParenthesizedExpressionSyntax* asParenthesizedExpression() { return nullptr; }
    virtual const ParenthesizedExpressionSyntax* asParenthesizedExpression() const { return nullptr; }
    virtual GenericSelectionExpressionSyntax* asGenericSelectionExpression() { return nullptr; }
    virtual const GenericSelectionExpressionSyntax* asGenericSelectionExpression() const { return nullptr; }
    virtual GenericAssociationSyntax* asGenericAssociation() { return nullptr; }
    virtual const GenericAssociationSyntax* asGenericAssociation() const { return nullptr; }
    virtual ExtGNU_EnclosedCompoundStatementExpressionSyntax* asExtGNU_EnclosedCompoundStatementExpression() { return nullptr; }
    virtual const ExtGNU_EnclosedCompoundStatementExpressionSyntax* asExtGNU_EnclosedCompoundStatementExpression() const { return nullptr; }
    virtual ExtGNU_ComplexValuedExpressionSyntax* asExtGNU_ComplexValuedExpression() { return nullptr; }
    virtual const ExtGNU_ComplexValuedExpressionSyntax* asExtGNU_ComplexValuedExpression() const { return nullptr; }

    /* Names */
    virtual NameSyntax* asName() { return nullptr; }
    virtual const NameSyntax* asName() const { return nullptr; }
    virtual IdentifierNameSyntax* asIdentifierName() { return nullptr; }
    virtual const IdentifierNameSyntax* asIdentifierName() const { return nullptr; }
    virtual PredefinedNameSyntax* asPredefinedName() { return nullptr; }
    virtual const PredefinedNameSyntax* asPredefinedName() const { return nullptr; }

    /* Operations */
    virtual UnaryExpressionSyntax* asUnaryExpression() { return nullptr; }
    virtual const UnaryExpressionSyntax* asUnaryExpression() const { return nullptr; }
    virtual PostfixUnaryExpressionSyntax* asPostfixUnaryExpression() { return nullptr; }
    virtual const PostfixUnaryExpressionSyntax* asPostfixUnaryExpression() const { return nullptr; }
    virtual PrefixUnaryExpressionSyntax* asPrefixUnaryExpression() { return nullptr; }
    virtual const PrefixUnaryExpressionSyntax* asPrefixUnaryExpression() const { return nullptr; }
    virtual MemberAccessExpressionSyntax* asMemberAccessExpression() { return nullptr; }
    virtual const MemberAccessExpressionSyntax* asMemberAccessExpression() const { return nullptr; }
    virtual ArraySubscriptExpressionSyntax* asArraySubscriptExpression() { return nullptr; }
    virtual const ArraySubscriptExpressionSyntax* asArraySubscriptExpression() const { return nullptr; }
    virtual TypeTraitExpressionSyntax* asTypeTraitExpression() { return nullptr; }
    virtual const TypeTraitExpressionSyntax* asTypeTraitExpression() const { return nullptr; }
    virtual CallExpressionSyntax* asCallExpression() { return nullptr; }
    virtual const CallExpressionSyntax* asCallExpression() const { return nullptr; }
    virtual CompoundLiteralExpressionSyntax* asCompoundLiteralExpression() { return nullptr; }
    virtual const CompoundLiteralExpressionSyntax* asCompoundLiteralExpression() const { return nullptr; }
    virtual CastExpressionSyntax* asCastExpression() { return nullptr; }
    virtual const CastExpressionSyntax* asCastExpression() const { return nullptr; }
    virtual VAArgumentExpressionSyntax* asVAArgumentExpression() { return nullptr; }
    virtual const VAArgumentExpressionSyntax* asVAArgumentExpression() const { return nullptr; }
    virtual OffsetOfExpressionSyntax* asOffsetOfExpression() { return nullptr; }
    virtual const OffsetOfExpressionSyntax* asOffsetOfExpression() const { return nullptr; }
    virtual BinaryExpressionSyntax* asBinaryExpression() { return nullptr; }
    virtual const BinaryExpressionSyntax* asBinaryExpression() const { return nullptr; }
    virtual ConditionalExpressionSyntax* asConditionalExpression() { return nullptr; }
    virtual const ConditionalExpressionSyntax* asConditionalExpression() const { return nullptr; }
    virtual AssignmentExpressionSyntax* asAssignmentExpression() { return nullptr; }
    virtual const AssignmentExpressionSyntax* asAssignmentExpression() const { return nullptr; }
    virtual SequencingExpressionSyntax* asSequencingExpression() { return nullptr; }
    virtual const SequencingExpressionSyntax* asSequencingExpression() const { return nullptr; }
    virtual ExtGNU_ChooseExpressionSyntax* asExtGNU_ChooseExpression() { return nullptr; }
    virtual const ExtGNU_ChooseExpressionSyntax* asExtGNU_ChooseExpression() const { return nullptr; }

    //------------//
    // Statements //
    //------------//
    virtual StatementSyntax* asStatement() { return nullptr; }
    virtual const StatementSyntax* asStatement() const { return nullptr; }
    virtual CompoundStatementSyntax* asCompoundStatement() { return nullptr; }
    virtual const CompoundStatementSyntax* asCompoundStatement() const { return nullptr; }
    virtual DeclarationStatementSyntax* asDeclarationStatement() { return nullptr; }
    virtual const DeclarationStatementSyntax* asDeclarationStatement() const { return nullptr; }
    virtual ExpressionStatementSyntax* asExpressionStatement() { return nullptr; }
    virtual const ExpressionStatementSyntax* asExpressionStatement() const { return nullptr; }
    virtual LabeledStatementSyntax* asLabeledStatement() { return nullptr; }
    virtual const LabeledStatementSyntax* asLabeledStatement() const { return nullptr; }
    virtual IfStatementSyntax* asIfStatement() { return nullptr; }
    virtual const IfStatementSyntax* asIfStatement() const { return nullptr; }
    virtual SwitchStatementSyntax* asSwitchStatement() { return nullptr; }
    virtual const SwitchStatementSyntax* asSwitchStatement() const { return nullptr; }
    virtual WhileStatementSyntax* asWhileStatement() { return nullptr; }
    virtual const WhileStatementSyntax* asWhileStatement() const { return nullptr; }
    virtual DoStatementSyntax* asDoStatement() { return nullptr; }
    virtual const DoStatementSyntax* asDoStatement() const { return nullptr; }
    virtual ForStatementSyntax* asForStatement() { return nullptr; }
    virtual const ForStatementSyntax* asForStatement() const { return nullptr; }
    virtual GotoStatementSyntax* asGotoStatement() { return nullptr; }
    virtual const GotoStatementSyntax* asGotoStatement() const { return nullptr; }
    virtual ContinueStatementSyntax* asContinueStatement() { return nullptr; }
    virtual const ContinueStatementSyntax* asContinueStatement() const { return nullptr; }
    virtual BreakStatementSyntax* asBreakStatement() { return nullptr; }
    virtual const BreakStatementSyntax* asBreakStatement() const { return nullptr; }
    virtual ReturnStatementSyntax* asReturnStatement() { return nullptr; }
    virtual const ReturnStatementSyntax* asReturnStatement() const { return nullptr; }
    virtual ExtGNU_AsmStatementSyntax* asExtGNU_AsmStatement() { return nullptr; }
    virtual const ExtGNU_AsmStatementSyntax* asExtGNU_AsmStatement() const { return nullptr; }
    virtual ExtGNU_AsmQualifierSyntax* asExtGNU_AsmQualifier() { return nullptr; }
    virtual const ExtGNU_AsmQualifierSyntax* asExtGNU_AsmQualifier() const { return nullptr; }
    virtual ExtGNU_AsmOperandSyntax* asExtGNU_AsmOperand() { return nullptr; }
    virtual const ExtGNU_AsmOperandSyntax* asExtGNU_AsmOperand() const { return nullptr; }

    //--------//
    // Common //
    //--------//
    virtual TypeNameSyntax* asTypeName() { return nullptr; }
    virtual const TypeNameSyntax* asTypeName() const { return nullptr; }
    virtual TypeReferenceSyntax* asTypeReference() { return nullptr; }
    virtual const TypeReferenceSyntax* asTypeReference() const { return nullptr; }
    virtual ExpressionAsTypeReferenceSyntax* asExpressionAsTypeReference() { return nullptr; }
    virtual const ExpressionAsTypeReferenceSyntax* asExpressionAsTypeReference() const { return nullptr; }
    virtual TypeNameAsTypeReferenceSyntax* asTypeNameAsTypeReference() { return nullptr; }
    virtual const TypeNameAsTypeReferenceSyntax* asTypeNameAsTypeReference() const { return nullptr; }

    //-------------//
    // Ambiguities //
    //-------------//
    virtual AmbiguousTypeNameOrExpressionAsTypeReferenceSyntax* asAmbiguousTypeNameOrExpressionAsTypeReference() { return nullptr; }
    virtual const AmbiguousTypeNameOrExpressionAsTypeReferenceSyntax* asAmbiguousTypeNameOrExpressionAsTypeReference() const { return nullptr; }
    virtual AmbiguousCastOrBinaryExpressionSyntax* asAmbiguousCastOrBinaryExpression() { return nullptr; }
    virtual const AmbiguousCastOrBinaryExpressionSyntax* asAmbiguousCastOrBinaryExpression() const { return nullptr; }
    virtual AmbiguousExpressionOrDeclarationStatementSyntax* asAmbiguousExpressionOrDeclarationStatement() { return nullptr; }
    virtual const AmbiguousExpressionOrDeclarationStatementSyntax* asAmbiguousExpressionOrDeclarationStatement() const { return nullptr; }

protected:
    SyntaxNode(SyntaxTree* tree, SyntaxKind kind = Error);

    // Unavailable
    SyntaxNode(const SyntaxNode& other) = delete;
    SyntaxNode& operator=(const SyntaxNode& other) = delete;

    SyntaxToken tokenAtIndex(LexedTokens::IndexType tkIdx) const;
    SyntaxToken findValidToken(const std::vector<SyntaxHolder>& syntaxHolders) const;
    void visitChildren(SyntaxVisitor* visitor) const;

    virtual std::vector<SyntaxHolder> childNodesAndTokens() const { return {}; }
    virtual SyntaxVisitor::Action dispatchVisit(SyntaxVisitor* visitor) const = 0;

    SyntaxTree* tree_;
    SyntaxKind kind_;
};

/**
 * The SyntaxKind of a given syntax \p kind as a \c std::string.
 */
std::string PSY_C_API to_string(SyntaxKind kind);

} // C
} // psy

#endif
