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

#include "Disambiguator_SyntaxCorrelation.h"

#include "syntax/SyntaxNodes.h"
#include "syntax/SyntaxUtilities.h"

#include "../common/infra/Assertions.h"
#include "../common/infra/Escape.h"

#include <iostream>

using namespace psy;
using namespace C;

SyntaxCorrelationDisambiguator::SyntaxCorrelationDisambiguator(
        SyntaxTree* tree,
        std::unique_ptr<NameCatalog> catalog)
    : Disambiguator(tree)
{
    catalog_ = std::move(catalog);
}

SyntaxVisitor::Action SyntaxCorrelationDisambiguator::visitTranslationUnit(const TranslationUnitSyntax* node)
{
    catalog_->markMappedNodeAsEncloser(node);
    Disambiguator::visitTranslationUnit(node);
    catalog_->dropEncloser();

    return Action::Skip;
}

SyntaxVisitor::Action SyntaxCorrelationDisambiguator::visitCompoundStatement(const CompoundStatementSyntax* node)
{
    catalog_->markMappedNodeAsEncloser(node);
    Disambiguator::visitCompoundStatement(node);
    catalog_->dropEncloser();

    return Action::Skip;
}

Disambiguator::Disambiguation SyntaxCorrelationDisambiguator::disambiguateExpression(
        const AmbiguousCastOrBinaryExpressionSyntax* node) const
{
    PSY_ASSERT(node->kind() == SyntaxKind::AmbiguousCastOrBinaryExpression,
               return Disambiguation::Inconclusive);

    auto tyName = node->castExpression()->typeName();
    PSY_ASSERT(tyName->specifiers()
                   && tyName->specifiers()->value
                   && tyName->specifiers()->value->kind() == SyntaxKind::TypedefName,
               return Disambiguation::Inconclusive);

    auto tydefName = tyName->specifiers()->value->asTypedefName();
    auto name = tydefName->identifierToken().valueText();

    return catalog_->hasUseAsTypeName(name)
            ? Disambiguation::KeepCastExpression
            : catalog_->hasUseAsNonTypeName(name)
                    ? Disambiguation::KeepBinaryExpression
                    : Disambiguation::Inconclusive;
}

Disambiguator::Disambiguation SyntaxCorrelationDisambiguator::disambiguateStatement(
        const AmbiguousExpressionOrDeclarationStatementSyntax* node) const
{
    PSY_ASSERT(node->kind() == SyntaxKind::AmbiguousMultiplicationOrPointerDeclaration
                   || node->kind() == SyntaxKind::AmbiguousCallOrVariableDeclaration,
               return Disambiguation::Inconclusive);

    auto decl = node->declarationStatement()->declaration();
    PSY_ASSERT(decl->kind() == SyntaxKind::VariableAndOrFunctionDeclaration,
               return Disambiguation::Inconclusive);

    auto varDecl = decl->asVariableAndOrFunctionDeclaration();

    PSY_ASSERT(varDecl->specifiers()
                   && varDecl->specifiers()->value
                   && varDecl->specifiers()->value->kind() == SyntaxKind::TypedefName,
               return Disambiguation::Inconclusive);

    auto tydefName = varDecl->specifiers()->value->asTypedefName();
    auto lhsName = tydefName->identifierToken().valueText();

    if (catalog_->hasUseAsTypeName(lhsName))
        return Disambiguation::KeepDeclarationStatement;

    if (catalog_->hasUseAsNonTypeName(lhsName))
        return Disambiguation::KeepExpressionStatement;

    if (node->kind() == SyntaxKind::AmbiguousMultiplicationOrPointerDeclaration)
        return Disambiguation::Inconclusive;

    PSY_ASSERT(varDecl->declarators()
                   && varDecl->declarators()->value,
               return Disambiguation::Inconclusive);

    auto decltor = SyntaxUtilities::strippedDeclaratorOrSelf(varDecl->declarators()->value);
    PSY_ASSERT(decltor->kind() == SyntaxKind::IdentifierDeclarator,
               return Disambiguation::Inconclusive);

    auto rhsName = decltor->asIdentifierDeclarator()->identifierToken().valueText();

    if (catalog_->hasDefAsNonTypeName(rhsName))
        return Disambiguation::KeepExpressionStatement;

    return Disambiguation::Inconclusive;
}

Disambiguator::Disambiguation SyntaxCorrelationDisambiguator::disambiguateTypeReference(
        const AmbiguousTypeNameOrExpressionAsTypeReferenceSyntax* node) const
{
    PSY_ASSERT(node->kind() == SyntaxKind::AmbiguousTypeNameOrExpressionAsTypeReference,
               return Disambiguation::Inconclusive);

    auto typeName = node->typeNameAsTypeReference()->typeName();
    PSY_ASSERT(typeName->specifiers()
                   && typeName->specifiers()->value
                   && typeName->specifiers()->value->kind() == SyntaxKind::TypedefName,
               return Disambiguation::Inconclusive);

    auto typedefName = typeName->specifiers()->value->asTypedefName();
    auto name = typedefName->identifierToken().valueText();

    return catalog_->hasUseAsTypeName(name)
            ? Disambiguation::KeepTypeName
            : catalog_->hasUseAsNonTypeName(name)
                    ? Disambiguation::KeepExpression
                    : Disambiguation::Inconclusive;
}
