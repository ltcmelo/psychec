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

#ifndef PSYCHE_C_SYNTAX_VISITOR_H__
#define PSYCHE_C_SYNTAX_VISITOR_H__

#include "API.h"
#include "Fwds.h"

#include <cstdint>

namespace psy {
namespace C {

/**
 * \brief The SyntaxVisitor class.
 */
class PSY_C_API SyntaxVisitor
{
public:
    SyntaxVisitor(const SyntaxTree* tree);
    virtual ~SyntaxVisitor();

    /**
     * \brief The Action enumeration.
     */
    enum class Action : std::uint8_t
    {
        Visit,
        Skip,
        Quit
    };

    virtual bool preVisit(const SyntaxNode*) { return true; }
    virtual void postVisit(const SyntaxNode*) {}

    /**
     * Visit the SyntaxNode \p node.
     */
    void visit(const SyntaxNode* node);

    /**
     * Visit the SyntaxNodePlainList \p it.
     */
    template <class PtrT, class DerivedListT>
    void visit(CoreSyntaxNodeList<PtrT, DerivedListT>* it)
    {
        for (; it; it = it->next)
            visit(it->value);
    }

    //--------------//
    // Declarations //
    //--------------//
    virtual Action visitTranslationUnit(const TranslationUnitSyntax*) { return Action::Visit; }
    virtual Action visitIncompleteDeclaration(const IncompleteDeclarationSyntax*) { return Action::Visit; }
    virtual Action visitStructOrUnionDeclaration(const StructOrUnionDeclarationSyntax*) { return Action::Visit; }
    virtual Action visitEnumDeclaration(const EnumDeclarationSyntax*) { return Action::Visit; }
    virtual Action visitEnumeratorDeclaration(const EnumeratorDeclarationSyntax*) { return Action::Visit; }
    virtual Action visitVariableAndOrFunctionDeclaration(const VariableAndOrFunctionDeclarationSyntax*) { return Action::Visit; }
    virtual Action visitFieldDeclaration(const FieldDeclarationSyntax*) { return Action::Visit; }
    virtual Action visitParameterDeclaration(const ParameterDeclarationSyntax*) { return Action::Visit; }
    virtual Action visitStaticAssertDeclaration(const StaticAssertDeclarationSyntax*) { return Action::Visit; }
    virtual Action visitFunctionDefinition(const FunctionDefinitionSyntax*) { return Action::Visit; }
    virtual Action visitExtPSY_TemplateDeclaration(const ExtPSY_TemplateDeclarationSyntax*) { return Action::Visit; }
    virtual Action visitExtGNU_AsmStatementDeclaration(const ExtGNU_AsmStatementDeclarationSyntax*) { return Action::Visit; }
    virtual Action visitExtKR_ParameterDeclaration(const ExtKR_ParameterDeclarationSyntax*) { return Action::Visit; }

    /* Specifiers */
    virtual Action visitStorageClass(const StorageClassSyntax*) { return Action::Visit; }
    virtual Action visitBuiltinTypeSpecifier(const BuiltinTypeSpecifierSyntax*) { return Action::Visit; }
    virtual Action visitTagTypeSpecifier(const TagTypeSpecifierSyntax*) { return Action::Visit; }
    virtual Action visitAtomicTypeSpecifier(const AtomicTypeSpecifierSyntax*) { return Action::Visit; }
    virtual Action visitTypeDeclarationAsSpecifier(const TypeDeclarationAsSpecifierSyntax*) { return Action::Visit; }
    virtual Action visitTypedefName(const TypedefNameSyntax*) { return Action::Visit; }
    virtual Action visitTypeQualifier(const TypeQualifierSyntax*) { return Action::Visit; }
    virtual Action visitFunctionSpecifier(const FunctionSpecifierSyntax*) { return Action::Visit; }
    virtual Action visitAlignmentSpecifier(const AlignmentSpecifierSyntax*) { return Action::Visit; }
    virtual Action visitExtGNU_Typeof(const ExtGNU_TypeofSyntax*) { return Action::Visit; }
    virtual Action visitExtGNU_AttributeSpecifier(const ExtGNU_AttributeSpecifierSyntax*) { return Action::Visit; }
    virtual Action visitExtGNU_Attribute(const ExtGNU_AttributeSyntax*) { return Action::Visit; }
    virtual Action visitExtGNU_AsmLabel(const ExtGNU_AsmLabelSyntax*) { return Action::Visit; }
    virtual Action visitExtPSY_QuantifiedTypeSpecifier(const ExtPSY_QuantifiedTypeSpecifierSyntax*) { return Action::Visit; }

    /* Declarators */
    virtual Action visitArrayOrFunctionDeclarator(const ArrayOrFunctionDeclaratorSyntax*) { return Action::Visit; }
    virtual Action visitPointerDeclarator(const PointerDeclaratorSyntax*) { return Action::Visit; }
    virtual Action visitParenthesizedDeclarator(const ParenthesizedDeclaratorSyntax*) { return Action::Visit; }
    virtual Action visitIdentifierDeclarator(const IdentifierDeclaratorSyntax*) { return Action::Visit; }
    virtual Action visitAbstractDeclarator(const AbstractDeclaratorSyntax*) { return Action::Visit; }
    virtual Action visitSubscriptSuffix(const SubscriptSuffixSyntax*) { return Action::Visit; }
    virtual Action visitParameterSuffix(const ParameterSuffixSyntax*) { return Action::Visit; }
    virtual Action visitBitfieldDeclarator(const BitfieldDeclaratorSyntax*) { return Action::Visit; }

    /* Initializers */
    virtual Action visitExpressionInitializer(const ExpressionInitializerSyntax*) { return Action::Visit; }
    virtual Action visitBraceEnclosedInitializer(const BraceEnclosedInitializerSyntax*) { return Action::Visit; }
    virtual Action visitDesignatedInitializer(const DesignatedInitializerSyntax*) { return Action::Visit; }
    virtual Action visitFieldDesignator(const FieldDesignatorSyntax*) { return Action::Visit; }
    virtual Action visitArrayDesignator(const ArrayDesignatorSyntax*) { return Action::Visit; }
    virtual Action visitOffsetOfDesignator(const OffsetOfDesignatorSyntax*) { return Action::Visit; }

    //-------------//
    // Expressions //
    //-------------//
    virtual Action visitIdentifierName(const IdentifierNameSyntax*) { return Action::Visit; }
    virtual Action visitPredefinedName(const PredefinedNameSyntax*) { return Action::Visit; }
    virtual Action visitConstantExpression(const ConstantExpressionSyntax*) { return Action::Visit; }
    virtual Action visitStringLiteralExpression(const StringLiteralExpressionSyntax*) { return Action::Visit; }
    virtual Action visitParenthesizedExpression(const ParenthesizedExpressionSyntax*) { return Action::Visit; }
    virtual Action visitGenericSelectionExpression(const GenericSelectionExpressionSyntax*) { return Action::Visit; }
    virtual Action visitGenericAssociation(const GenericAssociationSyntax*) { return Action::Visit; }
    virtual Action visitExtGNU_EnclosedCompoundStatementExpression(const ExtGNU_EnclosedCompoundStatementExpressionSyntax*) { return Action::Visit; }
    virtual Action visitExtGNU_ComplexValuedExpression(const ExtGNU_ComplexValuedExpressionSyntax*) { return Action::Visit; }

    /* Operations */
    virtual Action visitPrefixUnaryExpression(const PrefixUnaryExpressionSyntax*) { return Action::Visit; }
    virtual Action visitPostfixUnaryExpression(const PostfixUnaryExpressionSyntax*) { return Action::Visit; }
    virtual Action visitMemberAccessExpression(const MemberAccessExpressionSyntax*) { return Action::Visit; }
    virtual Action visitArraySubscriptExpression(const ArraySubscriptExpressionSyntax*) { return Action::Visit; }
    virtual Action visitTypeTraitExpression(const TypeTraitExpressionSyntax*) { return Action::Visit; }
    virtual Action visitCastExpression(const CastExpressionSyntax*) { return Action::Visit; }
    virtual Action visitCallExpression(const CallExpressionSyntax*) { return Action::Visit; }
    virtual Action visitVAArgumentExpression(const VAArgumentExpressionSyntax*) { return Action::Visit; }
    virtual Action visitOffsetOfExpression(const OffsetOfExpressionSyntax*) { return Action::Visit; }
    virtual Action visitCompoundLiteralExpression(const CompoundLiteralExpressionSyntax*) { return Action::Visit; }
    virtual Action visitBinaryExpression(const BinaryExpressionSyntax*) { return Action::Visit; }
    virtual Action visitConditionalExpression(const ConditionalExpressionSyntax*) { return Action::Visit; }
    virtual Action visitAssignmentExpression(const AssignmentExpressionSyntax*) { return Action::Visit; }
    virtual Action visitSequencingExpression(const SequencingExpressionSyntax*) { return Action::Visit; }
    virtual Action visitExtGNU_ChooseExpression(const ExtGNU_ChooseExpressionSyntax*) { return Action::Visit; }

    //------------//
    // Statements //
    //------------//
    virtual Action visitCompoundStatement(const CompoundStatementSyntax*) { return Action::Visit; }
    virtual Action visitDeclarationStatement(const DeclarationStatementSyntax*) { return Action::Visit; }
    virtual Action visitExpressionStatement(const ExpressionStatementSyntax*) { return Action::Visit; }
    virtual Action visitLabeledStatement(const LabeledStatementSyntax*) { return Action::Visit; }
    virtual Action visitIfStatement(const IfStatementSyntax*) { return Action::Visit; }
    virtual Action visitSwitchStatement(const SwitchStatementSyntax*) { return Action::Visit; }
    virtual Action visitWhileStatement(const WhileStatementSyntax*) { return Action::Visit; }
    virtual Action visitDoStatement(const DoStatementSyntax*) { return Action::Visit; }
    virtual Action visitForStatement(const ForStatementSyntax*) { return Action::Visit; }
    virtual Action visitGotoStatement(const GotoStatementSyntax*) { return Action::Visit; }
    virtual Action visitContinueStatement(const ContinueStatementSyntax*) { return Action::Visit; }
    virtual Action visitBreakStatement(const BreakStatementSyntax*) { return Action::Visit; }
    virtual Action visitReturnStatement(const ReturnStatementSyntax*) { return Action::Visit; }
    virtual Action visitExtGNU_AsmStatement(const ExtGNU_AsmStatementSyntax*) { return Action::Visit; }
    virtual Action visitExtGNU_AsmQualifier(const ExtGNU_AsmQualifierSyntax*) { return Action::Visit; }
    virtual Action visitExtGNU_AsmOperand(const ExtGNU_AsmOperandSyntax*) { return Action::Visit; }

    //--------//
    // Common //
    //--------//
    virtual Action visitTypeName(const TypeNameSyntax*) { return Action::Visit; }
    virtual Action visitExpressionAsTypeReference(const ExpressionAsTypeReferenceSyntax*) { return Action::Visit; }
    virtual Action visitTypeNameAsTypeReference(const TypeNameAsTypeReferenceSyntax*) { return Action::Visit; }

    //-------------//
    // Ambiguities //
    //-------------//
    virtual Action visitAmbiguousTypeNameOrExpressionAsTypeReference(const AmbiguousTypeNameOrExpressionAsTypeReferenceSyntax*) { return Action::Visit; }
    virtual Action visitAmbiguousCastOrBinaryExpression(const AmbiguousCastOrBinaryExpressionSyntax*) { return Action::Visit; }
    virtual Action visitAmbiguousExpressionOrDeclarationStatement(const AmbiguousExpressionOrDeclarationStatementSyntax*) { return Action::Visit; }

protected:
    // Unavailable.
    SyntaxVisitor(const SyntaxVisitor&) = delete;
    void operator=(const SyntaxVisitor&) = delete;

    const SyntaxTree* tree_;
};

} // C
} // psy

#endif
