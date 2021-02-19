// Copyright (c) 2016/17/18/19/20/21 Leandro T. C. Melo <ltcmelo@gmail.com>
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
#include "APIFwds.h"

#include <cstdint>

namespace psy {
namespace C {

/**
 * \brief The SyntaxVisitor class.
 */
class PSY_C_API SyntaxVisitor
{
public:
    SyntaxVisitor(SyntaxTree* tree);
    SyntaxVisitor(const SyntaxVisitor&) = delete;
    void operator=(const SyntaxVisitor&) = delete;
    virtual ~SyntaxVisitor();

    /**
     * \brief The Action enumeration.
     */
    enum class Action : std::uint8_t
    {
        Continue,
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
    virtual Action visitTranslationUnit(const TranslationUnitSyntax*) { return Action::Continue; }
    virtual Action visitEmptyDeclaration(const EmptyDeclarationSyntax*) { return Action::Continue; }
    virtual Action visitStructOrUnionDeclaration(const StructOrUnionDeclarationSyntax*) { return Action::Continue; }
    virtual Action visitEnumDeclaration(const EnumDeclarationSyntax*) { return Action::Continue; }
    virtual Action visitEnumMemberDeclaration(const EnumMemberDeclarationSyntax*) { return Action::Continue; }
    virtual Action visitVariableAndOrFunctionDeclaration(const VariableAndOrFunctionDeclarationSyntax*) { return Action::Continue; }
    virtual Action visitFieldDeclaration(const FieldDeclarationSyntax*) { return Action::Continue; }
    virtual Action visitParameterDeclaration(const ParameterDeclarationSyntax*) { return Action::Continue; }
    virtual Action visitStaticAssertDeclaration(const StaticAssertDeclarationSyntax*) { return Action::Continue; }
    virtual Action visitFunctionDefinition(const FunctionDefinitionSyntax*) { return Action::Continue; }
    virtual Action visitExtPSY_TemplateDeclaration(const ExtPSY_TemplateDeclarationSyntax*) { return Action::Continue; }
    virtual Action visitExtGNU_AsmStatementDeclaration(const ExtGNU_AsmStatementDeclarationSyntax*) { return Action::Continue; }

    /* Specifiers */
    virtual Action visitStorageClass(const StorageClassSyntax*) { return Action::Continue; }
    virtual Action visitBuiltinTypeSpecifier(const BuiltinTypeSpecifierSyntax*) { return Action::Continue; }
    virtual Action visitTaggedTypeSpecifier(const TaggedTypeSpecifierSyntax*) { return Action::Continue; }
    virtual Action visitTypeDeclarationAsSpecifier(const TypeDeclarationAsSpecifierSyntax*) { return Action::Continue; }
    virtual Action visitTypedefName(const TypedefNameSyntax*) { return Action::Continue; }
    virtual Action visitTypeName(const TypeNameSyntax*) { return Action::Continue; }
    virtual Action visitTypeQualifier(const TypeQualifierSyntax*) { return Action::Continue; }
    virtual Action visitFunctionSpecifier(const FunctionSpecifierSyntax*) { return Action::Continue; }
    virtual Action visitAlignmentSpecifier(const AlignmentSpecifierSyntax*) { return Action::Continue; }
    virtual Action visitExtGNU_Typeof(const ExtGNU_TypeofSyntax*) { return Action::Continue; }
    virtual Action visitExtGNU_AttributeSpecifier(const ExtGNU_AttributeSpecifierSyntax*) { return Action::Continue; }
    virtual Action visitExtGNU_Attribute(const ExtGNU_AttributeSyntax*) { return Action::Continue; }
    virtual Action visitExtPSY_QuantifiedTypeSpecifier(const ExtPSY_QuantifiedTypeSpecifierSyntax*) { return Action::Continue; }

    /* Declarators */
    virtual Action visitDeclarator(const DeclaratorSyntax*) { return Action::Continue; }
    virtual Action visitArrayOrFunctionDeclarator(const ArrayOrFunctionDeclaratorSyntax*) { return Action::Continue; }
    virtual Action visitPointerDeclarator(const PointerDeclaratorSyntax*) { return Action::Continue; }
    virtual Action visitParenthesizedDeclarator(const ParenthesizedDeclaratorSyntax*) { return Action::Continue; }
    virtual Action visitIdentifierDeclarator(const IdentifierDeclaratorSyntax*) { return Action::Continue; }
    virtual Action visitAbstractDeclarator(const AbstractDeclaratorSyntax*) { return Action::Continue; }
    virtual Action visitSubscriptSuffix(const SubscriptSuffixSyntax*) { return Action::Continue; }
    virtual Action visitParameterSuffix(const ParameterSuffixSyntax*) { return Action::Continue; }
    virtual Action visitBitfieldDeclarator(const BitfieldDeclaratorSyntax*) { return Action::Continue; }

    /* Initializers */
    virtual Action visitExpressionInitializer(const ExpressionInitializerSyntax*) { return Action::Continue; }
    virtual Action visitBraceEnclosedInitializer(const BraceEnclosedInitializerSyntax*) { return Action::Continue; }
    virtual Action visitDesignatedInitializer(const DesignatedInitializerSyntax*) { return Action::Continue; }
    virtual Action visitFieldDesignator(const FieldDesignatorSyntax*) { return Action::Continue; }
    virtual Action visitArrayDesignator(const ArrayDesignatorSyntax*) { return Action::Continue; }

    //-------------//
    // Expressions //
    //-------------//
    virtual Action visitIdentifierExpression(const IdentifierExpressionSyntax*) { return Action::Continue; }
    virtual Action visitConstantExpression(const ConstantExpressionSyntax*) { return Action::Continue; }
    virtual Action visitStringLiteralExpression(const StringLiteralExpressionSyntax*) { return Action::Continue; }
    virtual Action visitParenthesizedExpression(const ParenthesizedExpressionSyntax*) { return Action::Continue; }
    virtual Action visitGenericSelectionExpression(const GenericSelectionExpressionSyntax*) { return Action::Continue; }
    virtual Action visitGenericAssociation(const GenericAssociationSyntax*) { return Action::Continue; }
    virtual Action visitExtGNU_EnclosedCompoundStatementExpression(const ExtGNU_EnclosedCompoundStatementExpressionSyntax*) { return Action::Continue; }

    /* Operations */
    virtual Action visitPrefixUnaryExpression(const PrefixUnaryExpressionSyntax*) { return Action::Continue; }
    virtual Action visitPostfixUnaryExpression(const PostfixUnaryExpressionSyntax*) { return Action::Continue; }
    virtual Action visitMemberAccessExpression(const MemberAccessExpressionSyntax*) { return Action::Continue; }
    virtual Action visitArraySubscriptExpression(const ArraySubscriptExpressionSyntax*) { return Action::Continue; }
    virtual Action visitTypeTraitExpression(const TypeTraitExpressionSyntax*) { return Action::Continue; }
    virtual Action visitCastExpression(const CastExpressionSyntax*) { return Action::Continue; }
    virtual Action visitCallExpression(const CallExpressionSyntax*) { return Action::Continue; }
    virtual Action visitCompoundLiteralExpression(const CompoundLiteralExpressionSyntax*) { return Action::Continue; }
    virtual Action visitBinaryExpression(const BinaryExpressionSyntax*) { return Action::Continue; }
    virtual Action visitConditionalExpression(const ConditionalExpressionSyntax*) { return Action::Continue; }
    virtual Action visitAssignmentExpression(const AssignmentExpressionSyntax*) { return Action::Continue; }
    virtual Action visitSequencingExpression(const SequencingExpressionSyntax*) { return Action::Continue; }

    //------------//
    // Statements //
    //------------//
    virtual Action visitCompoundStatement(const CompoundStatementSyntax*) { return Action::Continue; }
    virtual Action visitDeclarationStatement(const DeclarationStatementSyntax*) { return Action::Continue; }
    virtual Action visitExpressionStatement(const ExpressionStatementSyntax*) { return Action::Continue; }
    virtual Action visitLabeledStatement(const LabeledStatementSyntax*) { return Action::Continue; }
    virtual Action visitIfStatement(const IfStatementSyntax*) { return Action::Continue; }
    virtual Action visitSwitchStatement(const SwitchStatementSyntax*) { return Action::Continue; }
    virtual Action visitWhileStatement(const WhileStatementSyntax*) { return Action::Continue; }
    virtual Action visitDoStatement(const DoStatementSyntax*) { return Action::Continue; }
    virtual Action visitForStatement(const ForStatementSyntax*) { return Action::Continue; }
    virtual Action visitGotoStatement(const GotoStatementSyntax*) { return Action::Continue; }
    virtual Action visitContinueStatement(const ContinueStatementSyntax*) { return Action::Continue; }
    virtual Action visitBreakStatement(const BreakStatementSyntax*) { return Action::Continue; }
    virtual Action visitReturnStatement(const ReturnStatementSyntax*) { return Action::Continue; }
    virtual Action visitExtGNU_AsmStatement(const ExtGNU_AsmStatementSyntax*) { return Action::Continue; }
    virtual Action visitExtGNU_AsmQualifier(const ExtGNU_AsmQualifierSyntax*) { return Action::Continue; }
    virtual Action visitExtGNU_AsmOperand(const ExtGNU_AsmOperandSyntax*) { return Action::Continue; }

    //-------------//
    // Ambiguities //
    //-------------//
    virtual Action visitAmbiguousTypedefNameOrIdentifierExpression(const AmbiguousTypedefNameOrIdentifierExpressionSyntax*) { return Action::Continue; }
    virtual Action visitAmbiguousCastOrBinaryExpression(const AmbiguousCastOrBinaryExpressionSyntax*) { return Action::Continue; }
    virtual Action visitAmbiguousExpressionOrDeclarationStatement(const AmbiguousExpressionOrDeclarationStatementSyntax*) { return Action::Continue; }

protected:
    SyntaxTree* tree_;
};

} // C
} // psy

#endif
