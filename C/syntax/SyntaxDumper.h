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

#include "SyntaxLexemes.h"
#include "SyntaxNodes.h"
#include "SyntaxVisitor.h"

namespace psy {
namespace C {

class PSY_C_API SyntaxDumper : protected SyntaxVisitor
{
public:
    SyntaxDumper(SyntaxTree* unit)
        : SyntaxVisitor(unit)
    {}

protected:
    virtual void terminal(unsigned tk, const SyntaxNode* node) {} // TODO: remove
    virtual void terminal(const SyntaxToken& tk, const SyntaxNode* node) {}
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

    Action visitTypeDeclaration_Common(const TypeDeclarationSyntax* node)
    {
        if (node->typeSpec_)
            nonterminal(node->typeSpec_);
        terminal(node->semicolonTkIdx_, node);
        return Action::Skip;
    }

    virtual Action visitStructOrUnionDeclaration(const StructOrUnionDeclarationSyntax* node) override
    {
        return visitTypeDeclaration_Common(node);
    }

    virtual Action visitEnumDeclaration(const EnumDeclarationSyntax* node) override
    {
        return visitTypeDeclaration_Common(node);
    }

    /* Specifiers */

    virtual Action visitTrivialSpecifier_Common(const TrivialSpecifierSyntax* node)
    {
        if (node->specTkIdx_)
            terminal(node->specTkIdx_, node);
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

    virtual Action visitTaggedTypeSpecifier(const TaggedTypeSpecifierSyntax* node)
    {
        if (node->taggedKwTkIdx_)
            terminal(node->taggedKwTkIdx_, node);
        for (SpecifierListSyntax *iter = node->attrs1_; iter; iter = iter->next)
            nonterminal(iter->value);
        if (node->identTkIdx_)
            terminal(node->identTkIdx_, node);
        if (node->openBraceTkIdx_)
            terminal(node->openBraceTkIdx_, node);
        for (DeclarationListSyntax *iter = node->decls_; iter; iter = iter->next)
            nonterminal(iter->value);
        if (node->closeBraceTkIdx_)
            terminal(node->closeBraceTkIdx_, node);
        for (SpecifierListSyntax *iter = node->attrs2_; iter; iter = iter->next)
            nonterminal(iter->value);
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
        if (node->alignasKwTkIdx_)
            terminal(node->alignasKwTkIdx_, node);
        if (node->openParenTkIdx_)
            terminal(node->openParenTkIdx_, node);
        nonterminal(node->arg_);
        if (node->closeParenTkIdx_)
            terminal(node->closeParenTkIdx_, node);
        return Action::Skip;
    }

    virtual Action visitExtGNU_AttributeSpecifier(const ExtGNU_AttributeSpecifierSyntax* node) override
    {
        if (node->attrKwTkIdx_)
            terminal(node->attrKwTkIdx_, node);
        if (node->openOuterParenTkIdx_)
            terminal(node->openOuterParenTkIdx_, node);
        if (node->openInnerParenTkIdx_)
            terminal(node->openInnerParenTkIdx_, node);
        for (ExtGNU_AttributeListSyntax *iter = node->attrs_; iter; iter = iter->next)
            nonterminal(iter->value);
        if (node->closeInnerParenTkIdx_)
            terminal(node->closeInnerParenTkIdx_, node);
        if (node->closeOuterParenTkIdx_)
            terminal(node->closeOuterParenTkIdx_, node);
        return Action::Skip;
    }

    virtual Action visitExtGNU_Attribute(const ExtGNU_AttributeSyntax* node) override
    {
        if (node->kwOrIdentTkIdx_)
            terminal(node->kwOrIdentTkIdx_, node);
        if (node->openParenTkIdx_)
            terminal(node->openParenTkIdx_, node);
        for (ExpressionListSyntax *iter = node->exprs_; iter; iter = iter->next) {
            nonterminal(iter->value);
            if (iter->delimTkIdx_)
                terminal(iter->delimTkIdx_, node);
        }
        if (node->closeParenTkIdx_)
            terminal(node->closeParenTkIdx_, node);
        return Action::Skip;
    }

    virtual Action visitExtGNU_Typeof(const ExtGNU_TypeofSyntax* node) override
    {
        if (node->typeofKwTkIdx_)
            terminal(node->typeofKwTkIdx_, node);
        if (node->openParenTkIdx_)
            terminal(node->openParenTkIdx_, node);
        nonterminal(node->arg_);
        if (node->closeParenTkIdx_)
            terminal(node->closeParenTkIdx_, node);
        return Action::Skip;
    }

    /* Declarators */

    virtual Action visitArrayOrFunctionDeclarator(const ArrayOrFunctionDeclaratorSyntax* node) override
    {
        for (auto iter = node->attrs1_; iter; iter = iter->next)
            nonterminal(iter->value);
        nonterminal(node->innerDecltor_);
        nonterminal(node->suffix_);
        for (auto iter = node->attrs2_; iter; iter = iter->next)
            nonterminal(iter->value);
        if (node->equalsTkIdx_)
            terminal(node->equalsTkIdx_, node);
        nonterminal(node->init_);
        return Action::Skip;
    }

    virtual Action visitPointerDeclarator(const PointerDeclaratorSyntax* node)
    {
        for (auto iter = node->attrs_; iter; iter = iter->next)
            nonterminal(iter->value);
        if (node->asteriskTkIdx_)
            terminal(node->asteriskTkIdx_, node);
        for (auto iter = node->qualsAndAttrs_; iter; iter = iter->next)
            nonterminal(iter->value);
        nonterminal(node->innerDecltor_);
        if (node->equalsTkIdx_)
            terminal(node->equalsTkIdx_, node);
        nonterminal(node->init_);
        return Action::Skip;
    }

    virtual Action visitParenthesizedDeclarator(const ParenthesizedDeclaratorSyntax* node) override
    {
        if (node->openParenTkIdx_)
            terminal(node->openParenTkIdx_, node);
        nonterminal(node->innerDecltor_);
        if (node->closeParenTkIdx_)
            terminal(node->closeParenTkIdx_, node);
        return Action::Skip;
    }

    virtual Action visitIdentifierDeclarator(const IdentifierDeclaratorSyntax* node) override
    {
        if (node->identTkIdx_)
            terminal(node->identTkIdx_, node);
        for (SpecifierListSyntax *iter = node->attrs2_; iter; iter = iter->next)
            nonterminal(iter->value);
        if (node->equalsTkIdx_)
            terminal(node->equalsTkIdx_, node);
        nonterminal(node->init_);
        return Action::Skip;
    }

    virtual Action visitAbstractDeclarator(const AbstractDeclaratorSyntax* node) override
    {
        return Action::Skip;
    }

    virtual Action visitBitfieldDeclarator(const BitfieldDeclaratorSyntax* node) override
    {
        nonterminal(node->innerDecltor_);
        if (node->colonTkIdx_)
            terminal(node->colonTkIdx_, node);
        nonterminal(node->expr_);
        for (auto iter = node->attrs_; iter; iter = iter->next)
            nonterminal(iter->value);
        return Action::Skip;
    }

    virtual Action visitVariableAndOrFunctionDeclaration(const VariableAndOrFunctionDeclarationSyntax* node) override
    {
        for (auto iter = node->specs_; iter; iter = iter->next)
            nonterminal(iter->value);
        for (DeclaratorListSyntax *iter = node->decltors_; iter; iter = iter->next) {
            nonterminal(iter->value);
            if (iter->delimTkIdx_)
                terminal(iter->delimTkIdx_, node);
        }
        if (node->semicolonTkIdx_)
            terminal(node->semicolonTkIdx_, node);
        return Action::Skip;
    }

    virtual Action visitFieldDeclaration(const FieldDeclarationSyntax* node) override
    {
        for (auto iter = node->specs_; iter; iter = iter->next)
            nonterminal(iter->value);
        for (DeclaratorListSyntax *iter = node->decltors_; iter; iter = iter->next) {
            nonterminal(iter->value);
            if (iter->delimTkIdx_)
                terminal(iter->delimTkIdx_, node);
        }
        if (node->semicolonTkIdx_)
            terminal(node->semicolonTkIdx_, node);
        return Action::Skip;
    }

    virtual Action visitStaticAssertDeclaration(const StaticAssertDeclarationSyntax* node) override
    {
        if (node->staticAssertKwTkIdx_)
            terminal(node->staticAssertKwTkIdx_, node);
        if (node->openParenTkIdx_)
            terminal(node->openParenTkIdx_, node);
        nonterminal(node->expr_);
        if (node->commaTkIdx_)
            terminal(node->commaTkIdx_, node);
        nonterminal(node->strLit_);
        if (node->closeParenTkIdx_)
            terminal(node->closeParenTkIdx_, node);
        if (node->semicolonTkIdx_)
            terminal(node->semicolonTkIdx_, node);
        return Action::Skip;
    }

    virtual Action visitEmptyDeclaration(const EmptyDeclarationSyntax* node) override
    {
        for (auto it = node->specifiers(); it; it = it->next)
            nonterminal(it->value);
        if (!node->semicolonToken().isMissing())
            terminal(node->semicolonToken(), node);
        return Action::Skip;
    }

    virtual Action visitExtGNU_AsmStatementDeclaration(const ExtGNU_AsmStatementDeclarationSyntax* node) override
    {
        if (node->asmTkIdx_)
            terminal(node->asmTkIdx_, node);
        if (node->openParenTkIdx_)
            terminal(node->openParenTkIdx_, node);
        nonterminal(node->strLit_);
        if (node->closeParenTkIdx_)
            terminal(node->closeParenTkIdx_, node);
        return Action::Skip;
    }

    virtual Action visitExtGNU_AsmStatement(const ExtGNU_AsmStatementSyntax* node) override
    {
        if (node->asmKwTkIdx_)
            terminal(node->asmKwTkIdx_, node);
        for (auto it = node->asmQualifiers(); it; it = it->next)
            nonterminal(it->value);
        if (node->openParenTkIdx_)
            terminal(node->openParenTkIdx_, node);
        nonterminal(node->strLit_);
        if (node->closeParenTkIdx_)
            terminal(node->closeParenTkIdx_, node);
        if (node->semicolonTkIdx_)
            terminal(node->semicolonTkIdx_, node);
        return Action::Skip;
    }

    virtual Action visitExtGNU_AsmQualifier(const ExtGNU_AsmQualifierSyntax* node)
    {
        if (node->specTkIdx_)
            terminal(node->specTkIdx_, node);
        return Action::Skip;
    }

    //-------------//
    // Expressions //
    //-------------//
    virtual Action visitIdentifierExpression(const IdentifierExpressionSyntax* node) override
    {
        if (node->identTkIdx_)
            terminal(node->identTkIdx_, node);
        return Action::Skip;
    }

    virtual Action visitConstantExpression(const ConstantExpressionSyntax* node) override
    {
        if (node->constantTkIdx_)
            terminal(node->constantTkIdx_, node);
        return Action::Skip;
    }

    virtual Action visitStringLiteralExpression(const StringLiteralExpressionSyntax* node) override
    {
        if (node->litTkIdx_)
            terminal(node->litTkIdx_, node);
        nonterminal(node->adjacent_);
        return Action::Skip;
    }

    virtual Action visitParenthesizedExpression(const ParenthesizedExpressionSyntax* node) override
    {
        if (node->openParenTkIdx_)
            terminal(node->openParenTkIdx_, node);
        nonterminal(node->expr_);
        if (node->closeParenTkIdx_)
            terminal(node->closeParenTkIdx_, node);
        return Action::Skip;
    }

    virtual Action visitGenericSelectionExpression(const GenericSelectionExpressionSyntax*  node) override
    {
        if (node->genericKwTkIdx_)
            terminal(node->genericKwTkIdx_, node);
        if (node->openParenTkIdx_)
            terminal(node->openParenTkIdx_, node);
        nonterminal(node->expr_);
        if (node->commaTkIdx_)
            terminal(node->commaTkIdx_, node);
        for (auto it = node->assocs_; it; it = it->next) {
            nonterminal(it->value);
            if (it->delimTkIdx_)
                terminal(it->delimTkIdx_, node);
        }
        if (node->closeParenTkIdx_)
            terminal(node->closeParenTkIdx_, node);
        return Action::Skip;
    }

    virtual Action visitGenericAssociation(const GenericAssociationSyntax* node) override
    {
        nonterminal(node->typeName_or_default_);
        if (node->colonTkIdx_)
            terminal(node->colonTkIdx_, node);
        nonterminal(node->expr_);
        return Action::Skip;
    }

    virtual Action visitExtGNU_EnclosedCompoundStatementExpression(const ExtGNU_EnclosedCompoundStatementExpressionSyntax* node) override
    {
        if (node->openParenTkIdx_)
            terminal(node->openParenTkIdx_, node);
        nonterminal(node->stmt_);
        if (node->closeParenTkIdx_)
            terminal(node->closeParenTkIdx_, node);
        return Action::Skip;
    }

    virtual Action visitAmbiguousCastOrBinaryExpression(const AmbiguousCastOrBinaryExpressionSyntax* node) override
    {
        nonterminal(node->castExpr_);
        nonterminal(node->binExpr_);
        return Action::Skip;
    }

    virtual Action visitTypeName(const TypeNameSyntax* node) override
    {
        for (auto it = node->specs_; it; it = it->next)
            nonterminal(it->value);
        nonterminal(node->decltor_);
        return Action::Skip;
    }

    virtual Action visitCompoundLiteralExpression(const CompoundLiteralExpressionSyntax* node) override
    {
        if (node->openParenTkIdx_)
            terminal(node->openParenTkIdx_, node);
        nonterminal(node->typeName_);
        if (node->closeParenTkIdx_)
            terminal(node->closeParenTkIdx_, node);
        nonterminal(node->init_);
        return Action::Skip;
    }

    virtual Action visitBinaryExpression(const BinaryExpressionSyntax* node) override
    {
        nonterminal(node->leftExpr_);
        if (node->oprtrTkIdx_)
            terminal(node->oprtrTkIdx_, node);
        nonterminal(node->rightExpr_);
        return Action::Skip;
    }

    virtual Action visitSequencingExpression(const SequencingExpressionSyntax* node) override
    {
        nonterminal(node->leftExpr_);
        if (node->oprtrTkIdx_)
            terminal(node->oprtrTkIdx_, node);
        nonterminal(node->rightExpr_);
        return Action::Skip;
    }

    virtual Action visitAssignmentExpression(const AssignmentExpressionSyntax* node) override
    {
        nonterminal(node->leftExpr_);
        if (node->oprtrTkIdx_)
            terminal(node->oprtrTkIdx_, node);
        nonterminal(node->rightExpr_);
        return Action::Skip;
    }

    virtual Action visitCastExpression(const CastExpressionSyntax* node) override
    {
        if (node->openParenTkIdx_)
            terminal(node->openParenTkIdx_, node);
        nonterminal(node->typeName_);
        if (node->closeParenTkIdx_)
            terminal(node->closeParenTkIdx_, node);
        nonterminal(node->expr_);
        return Action::Skip;
    }

    virtual Action visitCompoundStatement(const CompoundStatementSyntax* node) override
    {
        if (node->openBraceTkIdx_)
            terminal(node->openBraceTkIdx_, node);
        for (StatementListSyntax *iter = node->stmts_; iter; iter = iter->next)
            nonterminal(iter->value);
        if (node->closeBraceTkIdx_)
            terminal(node->closeBraceTkIdx_, node);
        return Action::Skip;
    }

    virtual Action visitConditionalExpression(const ConditionalExpressionSyntax* node) override
    {
        nonterminal(node->condExpr_);
        if (node->questionTkIdx_)
            terminal(node->questionTkIdx_, node);
        nonterminal(node->whenTrueExpr_);
        if (node->colonTkIdx_)
            terminal(node->colonTkIdx_, node);
        nonterminal(node->whenFalseExpr_);
        return Action::Skip;
    }

    virtual Action visitDeclarationStatement(const DeclarationStatementSyntax* node) override
    {
        nonterminal(node->decl_);
        return Action::Skip;
    }

    virtual Action visitParameterSuffix(const ParameterSuffixSyntax* node) override
    {
        if (node->openParenTkIdx_)
            terminal(node->openParenTkIdx_, node);
        for (ParameterDeclarationListSyntax *iter = node->decls_; iter; iter = iter->next) {
            nonterminal(iter->value);
            if (iter->delimTkIdx_)
                terminal(iter->delimTkIdx_, node);
        }
        if (node->ellipsisTkIdx_)
            terminal(node->ellipsisTkIdx_, node);

        if (node->closeParenTkIdx_)
            terminal(node->closeParenTkIdx_, node);
        return Action::Skip;
    }

    virtual Action visitSubscriptSuffix(const SubscriptSuffixSyntax* node) override
    {
        if (node->openBracketTkIdx_)
            terminal(node->openBracketTkIdx_, node);
        for (auto iter = node->qualsAndAttrs1_; iter; iter = iter->next)
            nonterminal(iter->value);
        if (node->staticKwTkIdx_)
            terminal(node->staticKwTkIdx_, node);
        for (auto iter = node->qualsAndAttrs2_; iter; iter = iter->next)
            nonterminal(iter->value);
        nonterminal(node->expr_);
        if (node->asteriskTkIdx_)
            terminal(node->asteriskTkIdx_, node);
        if (node->closeBracketTkIdx_)
            terminal(node->closeBracketTkIdx_, node);
        return Action::Skip;
    }

    virtual Action visitDoStatement(const DoStatementSyntax* node) override
    {
        if (node->doKwTkIdx_)
            terminal(node->doKwTkIdx_, node);
        nonterminal(node->stmt_);
        if (node->whileKwTkIdx_)
            terminal(node->whileKwTkIdx_, node);
        if (node->openParenTkIdx_)
            terminal(node->openParenTkIdx_, node);
        nonterminal(node->cond_);
        if (node->closeParenTkIdx_)
            terminal(node->closeParenTkIdx_, node);
        if (node->semicolonTkIdx_)
            terminal(node->semicolonTkIdx_, node);
        return Action::Skip;
    }

    virtual Action visitTypedefName(const TypedefNameSyntax* node) override
    {
        if (node->identTkIdx_)
            terminal(node->identTkIdx_, node);
        return Action::Skip;
    }

    virtual Action visitExtPSY_QuantifiedTypeSpecifier(const ExtPSY_QuantifiedTypeSpecifierSyntax* node)
    {
        if (node->quantifierTkIdx_)
            terminal(node->quantifierTkIdx_, node);
        if (node->openParenTkIdx_)
            terminal(node->openParenTkIdx_, node);
        if (node->identTkIdx_)
            terminal(node->identTkIdx_, node);
        if (node->closeParenTkIdx_)
            terminal(node->closeParenTkIdx_, node);
        return Action::Skip;
    }

    virtual Action visitEnumMemberDeclaration(const EnumMemberDeclarationSyntax* node) override
    {
        if (node->identTkIdx_)
            terminal(node->identTkIdx_, node);
        for (SpecifierListSyntax *iter = node->attrs_; iter; iter = iter->next)
            nonterminal(iter->value);
        if (node->equalsTkIdx_)
            terminal(node->equalsTkIdx_, node);
        nonterminal(node->expr_);
        if (node->commaTkIdx_)
            terminal(node->commaTkIdx_, node);
        return Action::Skip;
    }

    virtual Action visitExpressionStatement(const ExpressionStatementSyntax* node) override
    {
        nonterminal(node->expr_);
        if (node->semicolonTkIdx_)
            terminal(node->semicolonTkIdx_, node);
        return Action::Skip;
    }

    virtual Action visitFunctionDefinition(const FunctionDefinitionSyntax* node) override
    {
        for (auto iter = node->specs_; iter; iter = iter->next)
            nonterminal(iter->value);
        nonterminal(node->decltor_);
        nonterminal(node->body_);
        return Action::Skip;
    }

    virtual Action visitForStatement(const ForStatementSyntax* node) override
    {
        if (node->forKwTkIdx_)
            terminal(node->forKwTkIdx_, node);
        if (node->openParenTkIdx_)
            terminal(node->openParenTkIdx_, node);
        nonterminal(node->initStmt_);
        nonterminal(node->cond_);
        if (node->semicolonTkIdx_)
            terminal(node->semicolonTkIdx_, node);
        nonterminal(node->expr_);
        if (node->closeParenTkIdx_)
            terminal(node->closeParenTkIdx_, node);
        nonterminal(node->stmt_);
        return Action::Skip;
    }

    virtual Action visitIfStatement(const IfStatementSyntax* node) override
    {
        if (node->ifKwTkIdx_)
            terminal(node->ifKwTkIdx_, node);
        if (node->openParenTkIdx_)
            terminal(node->openParenTkIdx_, node);
        nonterminal(node->cond_);
        if (node->closeParenTkIdx_)
            terminal(node->closeParenTkIdx_, node);
        nonterminal(node->stmt_);
        if (node->elseKwTkIdx_)
            terminal(node->elseKwTkIdx_, node);
        nonterminal(node->elseStmt_);
        return Action::Skip;
    }


    virtual Action visitLabeledStatement(const LabeledStatementSyntax* node) override
    {
        if (node->labelTkIdx_)
            terminal(node->labelTkIdx_, node);
        nonterminal(node->expr_);
        if (node->colonTkIdx_)
            terminal(node->colonTkIdx_, node);
        nonterminal(node->stmt_);
        return Action::Skip;
    }

    virtual Action visitParameterDeclaration(const ParameterDeclarationSyntax* node) override
    {
        for (SpecifierListSyntax *iter = node->specs_; iter; iter = iter->next)
            nonterminal(iter->value);
        nonterminal(node->decltor_);
        return Action::Skip;
    }

    virtual Action visitCallExpression(const CallExpressionSyntax* node) override
    {
        nonterminal(node->expr_);
        if (node->openParenTkIdx_)
            terminal(node->openParenTkIdx_, node);
        for (ExpressionListSyntax *iter = node->args_; iter; iter = iter->next) {
            nonterminal(iter->value);
            if (iter->delimTkIdx_)
                terminal(iter->delimTkIdx_, node);
        }
        if (node->closeParenTkIdx_)
            terminal(node->closeParenTkIdx_, node);
        return Action::Skip;
    }

    virtual Action visitArraySubscriptExpression(const ArraySubscriptExpressionSyntax* node) override
    {
        nonterminal(node->expr_);
        if (node->openBracketTkIdx_)
            terminal(node->openBracketTkIdx_, node);
        nonterminal(node->arg_);
        if (node->closeBracketTkIdx_)
            terminal(node->closeBracketTkIdx_, node);
        return Action::Skip;
    }

    virtual Action visitMemberAccessExpression(const MemberAccessExpressionSyntax* node) override
    {
        nonterminal(node->expr_);
        if (node->oprtrTkIdx_)
            terminal(node->oprtrTkIdx_, node);
        nonterminal(node->identExpr_);
        return Action::Skip;
    }

    virtual Action visitBreakStatement(const BreakStatementSyntax* node) override
    {
        if (node->breakKwTkIdx_)
            terminal(node->breakKwTkIdx_, node);
        if (node->semicolonTkIdx_)
            terminal(node->semicolonTkIdx_, node);
        return Action::Skip;
    }

    virtual Action visitContinueStatement(const ContinueStatementSyntax* node) override
    {
        if (node->continueKwTkIdx_)
            terminal(node->continueKwTkIdx_, node);
        if (node->semicolonTkIdx_)
            terminal(node->semicolonTkIdx_, node);
        return Action::Skip;
    }

    virtual Action visitGotoStatement(const GotoStatementSyntax* node) override
    {
        if (node->gotoKwTkIdx_)
            terminal(node->gotoKwTkIdx_, node);
        if (node->identTkIdx_)
            terminal(node->identTkIdx_, node);
        if (node->semicolonTkIdx_)
            terminal(node->semicolonTkIdx_, node);
        return Action::Skip;
    }

    virtual Action visitReturnStatement(const ReturnStatementSyntax* node) override
    {
        if (node->returnKwTkIdx_)
            terminal(node->returnKwTkIdx_, node);
        nonterminal(node->expr_);
        if (node->semicolonTkIdx_)
            terminal(node->semicolonTkIdx_, node);
        return Action::Skip;
    }

    virtual Action visitTypeTraitExpression(const TypeTraitExpressionSyntax* node) override
    {
        if (node->oprtrTkIdx_)
            terminal(node->oprtrTkIdx_, node);
        if (node->openParenTkIdx_)
            terminal(node->openParenTkIdx_, node);
        nonterminal(node->arg_);
        if (node->closeParenTkIdx_)
            terminal(node->closeParenTkIdx_, node);
        return Action::Skip;
    }

    virtual Action visitSwitchStatement(const SwitchStatementSyntax* node) override
    {
        if (node->switchKwTkIdx_)
            terminal(node->switchKwTkIdx_, node);
        if (node->openParenTkIdx_)
            terminal(node->openParenTkIdx_, node);
        nonterminal(node->cond_);
        if (node->closeParenTkIdx_)
            terminal(node->closeParenTkIdx_, node);
        nonterminal(node->stmt_);
        return Action::Skip;
    }

    virtual Action visitPrefixUnaryExpression(const PrefixUnaryExpressionSyntax* node) override
    {
        if (node->oprtrTkIdx_)
            terminal(node->oprtrTkIdx_, node);
        nonterminal(node->expr_);
        return Action::Skip;
    }

    virtual Action visitPostfixUnaryExpression(const PostfixUnaryExpressionSyntax* node) override
    {
        nonterminal(node->expr_);
        if (node->oprtrTkIdx_)
            terminal(node->oprtrTkIdx_, node);
        return Action::Skip;
    }

    virtual Action visitWhileStatement(const WhileStatementSyntax* node) override
    {
        if (node->whileKwTkIdx_)
            terminal(node->whileKwTkIdx_, node);
        if (node->openParenTkIdx_)
            terminal(node->openParenTkIdx_, node);
        nonterminal(node->cond_);
        if (node->closeParenTkIdx_)
            terminal(node->closeParenTkIdx_, node);
        nonterminal(node->stmt_);
        return Action::Skip;
    }

    virtual Action visitExpressionInitializer(const ExpressionInitializerSyntax* node) override
    {
        nonterminal(node->expr_);
        return Action::Skip;
    }

    virtual Action visitBraceEnclosedInitializer(const BraceEnclosedInitializerSyntax* node) override
    {
        if (node->openBraceTkIdx_)
            terminal(node->openBraceTkIdx_, node);
        for (InitializerListSyntax *iter = node->initList_; iter; iter = iter->next) {
            nonterminal(iter->value);
            if (iter->delimTkIdx_)
                terminal(iter->delimTkIdx_, node);
        }
        if (node->closeBraceTkIdx_)
            terminal(node->closeBraceTkIdx_, node);
        return Action::Skip;
    }

    virtual Action visitFieldDesignator(const FieldDesignatorSyntax* node) override
    {
        if (node->dotTkIdx_)
            terminal(node->dotTkIdx_, node);
        if (node->identTkIdx_)
            terminal(node->identTkIdx_, node);
        return Action::Skip;
    }

    virtual Action visitArrayDesignator(const ArrayDesignatorSyntax* node) override
    {
        if (node->openBracketTkIdx_)
            terminal(node->openBracketTkIdx_, node);
        nonterminal(node->expr_);
        if (node->closeBracketTkIdx_)
            terminal(node->closeBracketTkIdx_, node);
        return Action::Skip;
    }

    virtual Action visitDesignatedInitializer(const DesignatedInitializerSyntax* node) override
    {
        for (DesignatorListSyntax *iter = node->desigs_; iter; iter = iter->next)
            nonterminal(iter->value);
        if (node->equalsTkIdx_)
            terminal(node->equalsTkIdx_, node);
        nonterminal(node->init_);
        return Action::Skip;
    }

    virtual Action visitAmbiguousExpressionOrDeclarationStatement(const AmbiguousExpressionOrDeclarationStatementSyntax* node) override
    {
        nonterminal(node->declStmt_);
        nonterminal(node->exprStmt_);
        return Action::Skip;
    }
};

} // C
} // psy

#endif
