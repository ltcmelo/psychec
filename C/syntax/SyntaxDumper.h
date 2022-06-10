/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** Modifications (apply on the LGPL usage):
** Copyright (c) 2016-20 Leandro T. C. Melo (ltcmelo@gmail.com)
**
** This file is part of Qt Creator.
**
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company.  For licensing terms and
** conditions see http://www.qt.io/terms-conditions.  For further information
** use the contact form at http://www.qt.io/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 or version 3 as published by the Free
** Software Foundation and appearing in the file LICENSE.LGPLv21 and
** LICENSE.LGPLv3 included in the packaging of this file.  Please review the
** following information to ensure the GNU Lesser General Public License
** requirements will be met: https://www.gnu.org/licenses/lgpl.html and
** http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, The Qt Company gives you certain additional
** rights.  These rights are described in The Qt Company LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
****************************************************************************/

/////////////////////////////////////////////////////////////////////
/////              Note about copyright/licesing                /////
/////                                                           /////
///// This file, which is copyrighed by NOKIA/Digia/Qt Company, /////
///// is licensed under LGPL, as opposed to most of the files   /////
///// of the Psyche project, which are licensed under BSD. This /////
///// version has been modified by Leandro T. C. Melo.          /////
/////////////////////////////////////////////////////////////////////

#ifndef PSYCHE_C_SYNTAX_DUMPER_H__
#define PSYCHE_C_SYNTAX_DUMPER_H__

#include "API.h"

#include "SyntaxLexeme_ALL.h"
#include "SyntaxNodes.h"
#include "SyntaxVisitor.h"

namespace psy {
namespace C {

class PSY_C_NON_API SyntaxDumper : protected SyntaxVisitor
{
public:
    SyntaxDumper(SyntaxTree* tree)
        : SyntaxVisitor(tree)
    {}

protected:
    virtual void terminal(const SyntaxToken&, const SyntaxNode*) {}
    virtual void nonterminal(const SyntaxNode* node) { visit(node); }

    //--------------//
    // Declarations //
    //--------------//
    virtual Action visitTranslationUnit(const TranslationUnitSyntax* node) override
    {
        for (auto iter = node->declarations(); iter; iter = iter->next)
            nonterminal(iter->value);
        return Action::Skip;
    }

    void traverseDeclaration(const DeclarationSyntax* node)
    {
        terminal(node->extensionKeyword(), node);
    }

    virtual Action visitIncompleteDeclaration(const IncompleteDeclarationSyntax* node) override
    {
        traverseDeclaration(node);
        for (auto it = node->specifiers(); it; it = it->next)
            nonterminal(it->value);
        terminal(node->semicolonToken(), node);
        return Action::Skip;
    }

    void traverseTypeDeclaration(const TypeDeclarationSyntax* node)
    {
        traverseDeclaration(node);
        nonterminal(node->typeSpecifier());
        terminal(node->semicolonToken(), node);
    }

    virtual Action visitStructOrUnionDeclaration(const StructOrUnionDeclarationSyntax* node) override
    {
        traverseTypeDeclaration(node);
        return Action::Skip;
    }

    virtual Action visitEnumDeclaration(const EnumDeclarationSyntax* node) override
    {
        traverseTypeDeclaration(node);
        return Action::Skip;
    }

    virtual Action visitEnumeratorDeclaration(const EnumeratorDeclarationSyntax* node) override
    {
        traverseDeclaration(node);
        terminal(node->identifierToken(), node);
        for (auto iter = node->attributes(); iter; iter = iter->next)
            nonterminal(iter->value);
        terminal(node->equalsToken(), node);
        nonterminal(node->expression());
        terminal(node->commaToken(), node);
        return Action::Skip;
    }

    virtual Action visitVariableAndOrFunctionDeclaration(const VariableAndOrFunctionDeclarationSyntax* node) override
    {
        traverseDeclaration(node);
        for (auto iter = node->specifiers(); iter; iter = iter->next)
            nonterminal(iter->value);
        for (auto iter = node->declarators(); iter; iter = iter->next) {
            nonterminal(iter->value);
            terminal(iter->delimiterToken(), node);
        }
        terminal(node->semicolonToken(), node);
        return Action::Skip;
    }

    virtual Action visitFieldDeclaration(const FieldDeclarationSyntax* node) override
    {
        traverseDeclaration(node);
        for (auto iter = node->specifiers(); iter; iter = iter->next)
            nonterminal(iter->value);
        for (auto iter = node->declarators(); iter; iter = iter->next) {
            nonterminal(iter->value);
            terminal(iter->delimiterToken(), node);
        }
        terminal(node->semicolonToken(), node);
        return Action::Skip;
    }

    virtual Action visitParameterDeclaration(const ParameterDeclarationSyntax* node) override
    {
        for (auto iter = node->specifiers(); iter; iter = iter->next)
            nonterminal(iter->value);
        nonterminal(node->declarator());
        return Action::Skip;
    }

    virtual Action visitStaticAssertDeclaration(const StaticAssertDeclarationSyntax* node) override
    {
        terminal(node->staticAssertKeyword(), node);
        terminal(node->openParenthesisToken(), node);
        nonterminal(node->expression());
        terminal(node->commaToken(), node);
        nonterminal(node->stringLiteral());
        terminal(node->closeParenthesisToken(), node);
        terminal(node->semicolonToken(), node);
        return Action::Skip;
    }

    virtual Action visitFunctionDefinition(const FunctionDefinitionSyntax* node) override
    {
        traverseDeclaration(node);
        for (auto iter = node->specifiers(); iter; iter = iter->next)
            nonterminal(iter->value);
        nonterminal(node->declarator());
        for (auto iter = node->extKR_params(); iter; iter = iter->next)
            nonterminal(iter->value);
        nonterminal(node->body());
        return Action::Skip;
    }

    virtual Action visitExtGNU_AsmStatementDeclaration(const ExtGNU_AsmStatementDeclarationSyntax* node) override
    {
        traverseDeclaration(node);
        terminal(node->asmKeyword(), node);
        terminal(node->openParenthesisToken(), node);
        nonterminal(node->stringLiteral());
        terminal(node->closeParenthesisToken(), node);
        return Action::Skip;
    }

    virtual Action visitExtKR_ParameterDeclaration(const ExtKR_ParameterDeclarationSyntax* node) override
    {
        for (auto iter = node->specifiers(); iter; iter = iter->next)
            nonterminal(iter->value);
        for (auto iter = node->declarators(); iter; iter = iter->next) {
            nonterminal(iter->value);
            terminal(iter->delimiterToken(), node);
        }
        terminal(node->semicolonToken(), node);
        return Action::Skip;
    }

    /* Specifiers */
    virtual Action visitTrivialSpecifier_Common(const TrivialSpecifierSyntax* node)
    {
        terminal(node->specifierToken(), node);
        return Action::Skip;
    }

    virtual Action visitStorageClass(const StorageClassSyntax* node) override
    {
        return visitTrivialSpecifier_Common(node);
    }

    virtual Action visitBuiltinTypeSpecifier(const BuiltinTypeSpecifierSyntax* node) override
    {
        return visitTrivialSpecifier_Common(node);
    }

    virtual Action visitTagTypeSpecifier(const TagTypeSpecifierSyntax* node) override
    {
        terminal(node->keyword(), node);
        for (auto iter = node->attributes(); iter; iter = iter->next)
            nonterminal(iter->value);
        terminal(node->tagToken(), node);
        terminal(node->openBraceToken(), node);
        for (auto iter = node->declarations(); iter; iter = iter->next)
            nonterminal(iter->value);
        terminal(node->closeBraceToken(), node);
        for (auto iter = node->attributes_PostCloseBrace(); iter; iter = iter->next)
            nonterminal(iter->value);
        return Action::Skip;
    }

    virtual Action visitAtomicTypeSpecifier(const AtomicTypeSpecifierSyntax* node) override
    {
        terminal(node->atomicKeyword(), node);
        terminal(node->openParenthesisToken(), node);
        nonterminal(node->typeName());
        terminal(node->closeParenthesisToken(), node);
        return Action::Skip;
    }

    virtual Action visitTypedefName(const TypedefNameSyntax* node) override
    {
        terminal(node->identifierToken(), node);
        return Action::Skip;
    }

    virtual Action visitTypeQualifier(const TypeQualifierSyntax* node) override
    {
        return visitTrivialSpecifier_Common(node);
    }

    virtual Action visitFunctionSpecifier(const FunctionSpecifierSyntax* node) override
    {
        return visitTrivialSpecifier_Common(node);
    }

    virtual Action visitAlignmentSpecifier(const AlignmentSpecifierSyntax* node) override
    {
        terminal(node->alignasKeyword(), node);
        nonterminal(node->tyReference());
        return Action::Skip;
    }

    virtual Action visitExtGNU_Typeof(const ExtGNU_TypeofSyntax* node) override
    {
        terminal(node->typeofKeyword(), node);
        nonterminal(node->tyReference());
        return Action::Skip;
    }

    virtual Action visitExtGNU_AttributeSpecifier(const ExtGNU_AttributeSpecifierSyntax* node) override
    {
        terminal(node->attributeKeyword(), node);
        terminal(node->openOuterParenthesisToken(), node);
        terminal(node->openInnerParenthesisToken(), node);
        for (auto iter = node->attributes(); iter; iter = iter->next)
            nonterminal(iter->value);
        terminal(node->closeInnerParenthesisToken(), node);
        terminal(node->closeOuterParenthesisToken(), node);
        return Action::Skip;
    }

    virtual Action visitExtGNU_Attribute(const ExtGNU_AttributeSyntax* node) override
    {
        terminal(node->keywordOrIdentifierToken(), node);
        terminal(node->openParenthesisToken(), node);
        for (auto iter = node->expressions(); iter; iter = iter->next) {
            nonterminal(iter->value);
            terminal(iter->delimiterToken(), node);
        }
        terminal(node->closeParenthesisToken(), node);
        return Action::Skip;
    }

    virtual Action visitExtGNU_AsmLabel(const ExtGNU_AsmLabelSyntax* node) override
    {
        terminal(node->asmKeyword(), node);
        terminal(node->openParenthesisToken(), node);
        nonterminal(node->stringLiteral());
        terminal(node->closeParenthesisToken(), node);
        return Action::Skip;
    }

    /* Declarators */
    virtual Action visitArrayOrFunctionDeclarator(const ArrayOrFunctionDeclaratorSyntax* node) override
    {
        for (auto iter = node->attributes(); iter; iter = iter->next)
            nonterminal(iter->value);
        nonterminal(node->innerDeclarator());
        nonterminal(node->suffix());
        for (auto iter = node->attributes_PostDeclarator(); iter; iter = iter->next)
            nonterminal(iter->value);
        terminal(node->equalsToken(), node);
        nonterminal(node->initializer());
        return Action::Skip;
    }

    virtual Action visitPointerDeclarator(const PointerDeclaratorSyntax* node) override
    {
        for (auto iter = node->attributes(); iter; iter = iter->next)
            nonterminal(iter->value);
        terminal(node->asteriskToken(), node);
        for (auto iter = node->qualifiersAndAttributes(); iter; iter = iter->next)
            nonterminal(iter->value);
        nonterminal(node->innerDeclarator());
        terminal(node->equalsToken(), node);
        nonterminal(node->initializer());
        return Action::Skip;
    }

    virtual Action visitParenthesizedDeclarator(const ParenthesizedDeclaratorSyntax* node) override
    {
        terminal(node->openParenthesisToken(), node);
        nonterminal(node->innerDeclarator());
        terminal(node->closeParenthesisToken(), node);
        return Action::Skip;
    }

    virtual Action visitIdentifierDeclarator(const IdentifierDeclaratorSyntax* node) override
    {
        terminal(node->identifierToken(), node);
        for (auto iter = node->attributes_PostIdentifier(); iter; iter = iter->next)
            nonterminal(iter->value);
        terminal(node->equalsToken(), node);
        nonterminal(node->initializer());
        return Action::Skip;
    }

    virtual Action visitSubscriptSuffix(const SubscriptSuffixSyntax* node) override
    {
        terminal(node->openBracketToken(), node);
        for (auto iter = node->qualifiersAndAttributes(); iter; iter = iter->next)
            nonterminal(iter->value);
        terminal(node->staticKeyword(), node);
        for (auto iter = node->qualifiersAndAttributes_PostStatic(); iter; iter = iter->next)
            nonterminal(iter->value);
        nonterminal(node->expression());
        terminal(node->asteriskToken(), node);
        terminal(node->closeBracketToken(), node);
        return Action::Skip;
    }

    virtual Action visitParameterSuffix(const ParameterSuffixSyntax* node) override
    {
        terminal(node->openParenthesisToken(), node);
        for (auto iter = node->parameters(); iter; iter = iter->next) {
            nonterminal(iter->value);
            terminal(iter->delimiterToken(), node);
        }
        terminal(node->ellipsisToken(), node);
        terminal(node->closeParenthesisToken(), node);
        return Action::Skip;
    }

    virtual Action visitBitfieldDeclarator(const BitfieldDeclaratorSyntax* node) override
    {
        nonterminal(node->innerDeclarator());
        terminal(node->colonToken(), node);
        nonterminal(node->expression());
        for (auto iter = node->attributes(); iter; iter = iter->next)
            nonterminal(iter->value);
        return Action::Skip;
    }

    /* Initializers */
    virtual Action visitExpressionInitializer(const ExpressionInitializerSyntax* node) override
    {
        nonterminal(node->expression());
        return Action::Skip;
    }

    virtual Action visitBraceEnclosedInitializer(const BraceEnclosedInitializerSyntax* node) override
    {
        terminal(node->openBraceToken(), node);
        for (auto iter = node->initializerList(); iter; iter = iter->next) {
            nonterminal(iter->value);
            terminal(iter->delimiterToken(), node);
        }
        terminal(node->closeBraceToken(), node);
        return Action::Skip;
    }

    virtual Action visitDesignatedInitializer(const DesignatedInitializerSyntax* node) override
    {
        for (auto iter = node->designators(); iter; iter = iter->next)
            nonterminal(iter->value);
        terminal(node->equalsToken(), node);
        nonterminal(node->initializer());
        return Action::Skip;
    }

    virtual Action visitFieldDesignator(const FieldDesignatorSyntax* node) override
    {
        terminal(node->dotToken(), node);
        terminal(node->identifierToken(), node);
        return Action::Skip;
    }

    virtual Action visitArrayDesignator(const ArrayDesignatorSyntax* node) override
    {
        terminal(node->openBracketToken(), node);
        nonterminal(node->expression());
        terminal(node->closeBracketToken(), node);
        return Action::Skip;
    }

    virtual Action visitOffsetOfDesignator(const OffsetOfDesignatorSyntax* node) override
    {
        terminal(node->identifierToken(), node);
        for (auto iter = node->designators(); iter; iter = iter->next)
            nonterminal(iter->value);
        return Action::Skip;
    }

    //-------------//
    // Expressions //
    //-------------//
    void traverseExpression(const ExpressionSyntax* node)
    {
        terminal(node->extensionKeyword(), node);
    }

    virtual Action visitIdentifierName(const IdentifierNameSyntax* node) override
    {
        traverseExpression(node);
        terminal(node->identifierToken(), node);
        return Action::Skip;
    }

    virtual Action visitPredefinedName(const PredefinedNameSyntax* node) override
    {
        traverseExpression(node);
        terminal(node->predefinedToken(), node);
        return Action::Skip;
    }

    virtual Action visitConstantExpression(const ConstantExpressionSyntax* node) override
    {
        traverseExpression(node);
        terminal(node->constantToken(), node);
        return Action::Skip;
    }

    virtual Action visitStringLiteralExpression(const StringLiteralExpressionSyntax* node) override
    {
        traverseExpression(node);
        terminal(node->literalToken(), node);
        nonterminal(node->adjacent());
        return Action::Skip;
    }

    virtual Action visitParenthesizedExpression(const ParenthesizedExpressionSyntax* node) override
    {
        traverseExpression(node);
        terminal(node->openParenthesisToken(), node);
        nonterminal(node->expression());
        terminal(node->closeParenthesisToken(), node);
        return Action::Skip;
    }

    virtual Action visitGenericSelectionExpression(const GenericSelectionExpressionSyntax*  node) override
    {
        traverseExpression(node);
        terminal(node->genericKeyword(), node);
        terminal(node->openParenthesisToken(), node);
        nonterminal(node->expression());
        terminal(node->commaToken(), node);
        for (auto it = node->associations(); it; it = it->next) {
            nonterminal(it->value);
            terminal(it->delimiterToken(), node);
        }
        terminal(node->closeParenthesisToken(), node);
        return Action::Skip;
    }

    virtual Action visitGenericAssociation(const GenericAssociationSyntax* node) override
    {
        nonterminal(node->typeName_or_default());
        terminal(node->colonToken(), node);
        nonterminal(node->expression());
        return Action::Skip;
    }

    virtual Action visitExtGNU_EnclosedCompoundStatementExpression(const ExtGNU_EnclosedCompoundStatementExpressionSyntax* node) override
    {
        terminal(node->openParenthesisToken(), node);
        nonterminal(node->statement());
        terminal(node->closeParenthesisToken(), node);
        return Action::Skip;
    }

    virtual Action visitExtGNU_ComplexValuedExpression(const ExtGNU_ComplexValuedExpressionSyntax* node) override
    {
        terminal(node->operatorToken(), node);
        nonterminal(node->expression());
        return Action::Skip;
    }

    /* Operations */
    virtual Action visitPrefixUnaryExpression(const PrefixUnaryExpressionSyntax* node) override
    {
        traverseExpression(node);
        terminal(node->operatorToken(), node);
        nonterminal(node->expression());
        return Action::Skip;
    }

    virtual Action visitPostfixUnaryExpression(const PostfixUnaryExpressionSyntax* node) override
    {
        traverseExpression(node);
        nonterminal(node->expression());
        terminal(node->operatorToken(), node);
        return Action::Skip;
    }

    virtual Action visitMemberAccessExpression(const MemberAccessExpressionSyntax* node) override
    {
        traverseExpression(node);
        nonterminal(node->expression());
        terminal(node->operatorToken(), node);
        nonterminal(node->identifier());
        return Action::Skip;
    }

    virtual Action visitArraySubscriptExpression(const ArraySubscriptExpressionSyntax* node) override
    {
        traverseExpression(node);
        nonterminal(node->expression());
        terminal(node->openBracketToken(), node);
        nonterminal(node->argument());
        terminal(node->closeBracketToken(), node);
        return Action::Skip;
    }


    virtual Action visitTypeTraitExpression(const TypeTraitExpressionSyntax* node) override
    {
        traverseExpression(node);
        terminal(node->operatorToken(), node);
        nonterminal(node->tyReference());
        return Action::Skip;
    }

    virtual Action visitCastExpression(const CastExpressionSyntax* node) override
    {
        traverseExpression(node);
        terminal(node->openParenthesisToken(), node);
        nonterminal(node->typeName());
        terminal(node->closeParenthesisToken(), node);
        nonterminal(node->expression());
        return Action::Skip;
    }

    virtual Action visitCallExpression(const CallExpressionSyntax* node) override
    {
        nonterminal(node->expression());
        terminal(node->openParenthesisToken(), node);
        for (auto iter = node->arguments(); iter; iter = iter->next) {
            nonterminal(iter->value);
            terminal(iter->delimiterToken(), node);
        }
        terminal(node->closeParenthesisToken(), node);
        return Action::Skip;
    }

    virtual Action visitVAArgumentExpression(const VAArgumentExpressionSyntax* node) override
    {
        terminal(node->keyword(), node);
        terminal(node->openParenthesisToken(), node);
        nonterminal(node->expression());
        terminal(node->commaToken(), node);
        nonterminal(node->typeName());
        terminal(node->closeParenthesisToken(), node);
        return Action::Skip;
    }

    virtual Action visitOffsetOfExpression(const OffsetOfExpressionSyntax* node) override
    {
        terminal(node->keyword(), node);
        terminal(node->openParenthesisToken(), node);
        nonterminal(node->typeName());
        terminal(node->commaToken(), node);
        nonterminal(node->offsetOfDesignator());
        terminal(node->closeParenthesisToken(), node);
        return Action::Skip;
    }

    virtual Action visitCompoundLiteralExpression(const CompoundLiteralExpressionSyntax* node) override
    {
        terminal(node->openParenthesisToken(), node);
        nonterminal(node->typeName());
        terminal(node->closeParenthesisToken(), node);
        nonterminal(node->initializer());
        return Action::Skip;
    }

    virtual Action visitBinaryExpression(const BinaryExpressionSyntax* node) override
    {
        nonterminal(node->left());
        terminal(node->operatorToken(), node);
        nonterminal(node->right());
        return Action::Skip;
    }

    virtual Action visitConditionalExpression(const ConditionalExpressionSyntax* node) override
    {
        nonterminal(node->condition());
        terminal(node->questionToken(), node);
        nonterminal(node->whenTrue());
        terminal(node->colonToken(), node);
        nonterminal(node->whenFalse());
        return Action::Skip;
    }

    virtual Action visitAssignmentExpression(const AssignmentExpressionSyntax* node) override
    {
        nonterminal(node->left());
        terminal(node->operatorToken(), node);
        nonterminal(node->right());
        return Action::Skip;
    }

    virtual Action visitSequencingExpression(const SequencingExpressionSyntax* node) override
    {
        nonterminal(node->left());
        terminal(node->operatorToken(), node);
        nonterminal(node->right());
        return Action::Skip;
    }

    virtual Action visitExtGNU_ChooseExpression(const ExtGNU_ChooseExpressionSyntax* node) override
    {
        terminal(node->keyword(), node);
        terminal(node->openParenthesisToken(), node);
        nonterminal(node->constantExpression());
        terminal(node->commaToken1(), node);
        nonterminal(node->expression1());
        terminal(node->commaToken2(), node);
        nonterminal(node->expression2());
        terminal(node->closeParenthesisToken(), node);
        return Action::Skip;
    }

    //------------//
    // Statements //
    //------------//
    virtual Action visitCompoundStatement(const CompoundStatementSyntax* node) override
    {
        terminal(node->openBraceToken(), node);
        for (auto iter = node->statements(); iter; iter = iter->next)
            nonterminal(iter->value);
        terminal(node->closeBraceToken(), node);
        return Action::Skip;
    }

    virtual Action visitDeclarationStatement(const DeclarationStatementSyntax* node) override
    {
        nonterminal(node->declaration());
        return Action::Skip;
    }

    virtual Action visitExpressionStatement(const ExpressionStatementSyntax* node) override
    {
        nonterminal(node->expression());
        terminal(node->semicolonToken(), node);
        return Action::Skip;
    }

    virtual Action visitLabeledStatement(const LabeledStatementSyntax* node) override
    {
        terminal(node->labelToken(), node);
        nonterminal(node->expression());
        terminal(node->colonToken(), node);
        nonterminal(node->statement());
        return Action::Skip;
    }

    virtual Action visitIfStatement(const IfStatementSyntax* node) override
    {
        terminal(node->ifKeyword(), node);
        terminal(node->openParenthesisToken(), node);
        nonterminal(node->condition());
        terminal(node->closeParenthesisToken(), node);
        nonterminal(node->statement());
        terminal(node->elseKeyword(), node);
        nonterminal(node->elseStatement());
        return Action::Skip;
    }

    virtual Action visitSwitchStatement(const SwitchStatementSyntax* node) override
    {
        terminal(node->switchKeyword(), node);
        terminal(node->openParenthesisToken(), node);
        nonterminal(node->condition());
        terminal(node->closeParenthesisToken(), node);
        nonterminal(node->statement());
        return Action::Skip;
    }

    virtual Action visitWhileStatement(const WhileStatementSyntax* node) override
    {
        terminal(node->whileKeyword(), node);
        terminal(node->openParenthesisToken(), node);
        nonterminal(node->condition());
        terminal(node->closeParenthesisToken(), node);
        nonterminal(node->statement());
        return Action::Skip;
    }

    virtual Action visitDoStatement(const DoStatementSyntax* node) override
    {
        terminal(node->doKeyword(), node);
        nonterminal(node->statement());
        terminal(node->whileKeyword(), node);
        terminal(node->openParenthesisToken(), node);
        nonterminal(node->condition());
        terminal(node->closeParenthesisToken(), node);
        terminal(node->semicolonToken(), node);
        return Action::Skip;
    }

    virtual Action visitForStatement(const ForStatementSyntax* node) override
    {
        terminal(node->forKeyword(), node);
        terminal(node->openParenthesisToken(), node);
        terminal(node->extensionKeyword(), node);
        nonterminal(node->initializer());
        nonterminal(node->condition());
        terminal(node->semicolonToken(), node);
        nonterminal(node->expression());
        terminal(node->closeParenthesisToken(), node);
        nonterminal(node->statement());
        return Action::Skip;
    }

    virtual Action visitGotoStatement(const GotoStatementSyntax* node) override
    {
        terminal(node->gotoKeyword(), node);
        terminal(node->identifierToken(), node);
        terminal(node->semicolonToken(), node);
        return Action::Skip;
    }

    virtual Action visitContinueStatement(const ContinueStatementSyntax* node) override
    {
        terminal(node->continueKeyword(), node);
        terminal(node->semicolonToken(), node);
        return Action::Skip;
    }

    virtual Action visitBreakStatement(const BreakStatementSyntax* node) override
    {
        terminal(node->breakKeyword(), node);
        terminal(node->semicolonToken(), node);
        return Action::Skip;
    }

    virtual Action visitReturnStatement(const ReturnStatementSyntax* node) override
    {
        terminal(node->returnKeyword(), node);
        nonterminal(node->expression());
        terminal(node->semicolonToken(), node);
        return Action::Skip;
    }

    virtual Action visitExtGNU_AsmStatement(const ExtGNU_AsmStatementSyntax* node) override
    {
        terminal(node->asmKeyword(), node);
        for (auto it = node->asmQualifiers(); it; it = it->next)
            nonterminal(it->value);
        terminal(node->openParenthesisToken(), node);
        nonterminal(node->stringLiteral());
        terminal(node->colon1Token(), node);
        for (auto it = node->outputOperands(); it; it = it->next) {
            nonterminal(it->value);
            terminal(it->delimiterToken(), node);
        }
        terminal(node->colon2Token(), node);
        for (auto it = node->inputOperands(); it; it = it->next) {
            nonterminal(it->value);
            terminal(it->delimiterToken(), node);
        }
        terminal(node->colon3Token(), node);
        for (auto it = node->clobbers(); it; it = it->next) {
            nonterminal(it->value);
            terminal(it->delimiterToken(), node);
        }
        terminal(node->colon4Token(), node);
        for (auto it = node->gotoLabels(); it; it = it->next) {
            nonterminal(it->value);
            terminal(it->delimiterToken(), node);
        }
        terminal(node->closeParenthesisToken(), node);
        terminal(node->semicolonToken(), node);
        return Action::Skip;
    }

    virtual Action visitExtGNU_AsmQualifier(const ExtGNU_AsmQualifierSyntax* node) override
    {
        terminal(node->asmQualifier(), node);
        return Action::Skip;
    }

    virtual Action visitExtGNU_AsmOperand(const ExtGNU_AsmOperandSyntax* node) override
    {
        terminal(node->openBracketToken(), node);
        nonterminal(node->identifier());
        terminal(node->closeBracketToken(), node);
        nonterminal(node->stringLiteral());
        terminal(node->openParenthesisToken(), node);
        nonterminal(node->expression());
        terminal(node->closeParenthesisToken(), node);
        return Action::Skip;
    }

    //--------//
    // Common //
    //--------//
    virtual Action visitTypeName(const TypeNameSyntax* node) override
    {
        for (auto it = node->specifiers(); it; it = it->next)
            nonterminal(it->value);
        nonterminal(node->declarator());
        return Action::Skip;
    }

    virtual Action visitExpressionAsTypeReference(const ExpressionAsTypeReferenceSyntax* node) override
    {
        nonterminal(node->expression());
        return Action::Skip;
    }

    virtual Action visitTypeNameAsTypeReference(const TypeNameAsTypeReferenceSyntax* node) override
    {
        terminal(node->openParenthesisToken(), node);
        nonterminal(node->typeName());
        terminal(node->closeParenthesisToken(), node);
        return Action::Skip;
    }

    //-------------//
    // Ambiguities //
    //-------------//
    virtual Action visitAmbiguousTypeNameOrExpressionAsTypeReference(const AmbiguousTypeNameOrExpressionAsTypeReferenceSyntax* node) override
    {
        nonterminal(node->expressionAsTypeReference());
        nonterminal(node->typeNameAsTypeReference());
        return Action::Skip;
    }

    virtual Action visitAmbiguousCastOrBinaryExpression(const AmbiguousCastOrBinaryExpressionSyntax* node) override
    {
        nonterminal(node->castExpression());
        nonterminal(node->binaryExpression());
        return Action::Skip;
    }

    virtual Action visitAmbiguousExpressionOrDeclarationStatement(const AmbiguousExpressionOrDeclarationStatementSyntax* node) override
    {
        nonterminal(node->declarationStatement());
        nonterminal(node->expressionStatement());
        return Action::Skip;
    }
};

} // C
} // psy

#endif
