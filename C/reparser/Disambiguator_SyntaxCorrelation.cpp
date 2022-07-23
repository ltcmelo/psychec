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

#include "../common/infra/Assertions.h"
#include "../common/infra/Escape.h"

#include <iostream>

using namespace psy;
using namespace C;

SyntaxCorrelationDisambiguator::SyntaxCorrelationDisambiguator(SyntaxTree* tree)
    : Disambiguator(tree)
{}

void SyntaxCorrelationDisambiguator::acquireCatalog(std::unique_ptr<NameCatalog> catalog)
{
    catalog_ = std::move(catalog);
}

SyntaxVisitor::Action SyntaxCorrelationDisambiguator::visitTranslationUnit(const TranslationUnitSyntax* node)
{
    catalog_->enterLevel(node);

    for (auto iter = node->declarations(); iter; iter = iter->next)
        visit(iter->value);

    catalog_->exitLevel();

    return Action::Skip;
}

Disambiguator::Disambiguation SyntaxCorrelationDisambiguator::disambiguateExpression(
        const AmbiguousCastOrBinaryExpressionSyntax* node) const
{
    auto typeName = node->castExpression()->typeName();
    PSY_ASSERT(typeName->specifiers()
                   && typeName->specifiers()->value
                   && typeName->specifiers()->value->kind() == TypedefName,
               return Disambiguation::Inconclusive);

    auto typedefName = typeName->specifiers()->value->asTypedefName();
    auto name = typedefName->identifierToken().valueText();

    return recognizesTypeName(name)
            ? Disambiguation::KeepCastExpression
            : recognizesName(name)
                    ? Disambiguation::KeepBinaryExpression
                    : Disambiguation::Inconclusive;
}

Disambiguator::Disambiguation SyntaxCorrelationDisambiguator::disambiguateStatement(
        const AmbiguousExpressionOrDeclarationStatementSyntax* node) const
{
    auto decl = node->declarationStatement()->declaration();
    PSY_ASSERT(decl->kind() == VariableAndOrFunctionDeclaration, return Disambiguation::Inconclusive);

    auto varDecl = decl->asVariableAndOrFunctionDeclaration();
    PSY_ASSERT(varDecl->specifiers()
                   && varDecl->specifiers()->value
                   && varDecl->specifiers()->value->kind() == TypedefName,
               return Disambiguation::Inconclusive);

    auto typedefName = varDecl->specifiers()->value->asTypedefName();
    auto name = typedefName->identifierToken().valueText();

    return recognizesTypeName(name)
            ? Disambiguation::KeepDeclarationStatement
            : recognizesName(name)
                    ? Disambiguation::KeepExpressionStatement
                    : Disambiguation::Inconclusive;
}

Disambiguator::Disambiguation SyntaxCorrelationDisambiguator::disambiguateTypeReference(
        const AmbiguousTypeNameOrExpressionAsTypeReferenceSyntax* node) const
{
    auto typeName = node->typeNameAsTypeReference()->typeName();
    PSY_ASSERT(typeName->specifiers()
                   && typeName->specifiers()->value
                   && typeName->specifiers()->value->kind() == TypedefName,
               return Disambiguation::Inconclusive);

    auto typedefName = typeName->specifiers()->value->asTypedefName();
    auto name = typedefName->identifierToken().valueText();

    return recognizesTypeName(name)
            ? Disambiguation::KeepTypeName
            : recognizesName(name)
                    ? Disambiguation::KeepExpression
                    : Disambiguation::Inconclusive;
}

bool SyntaxCorrelationDisambiguator::recognizesTypeName(const std::string& name) const
{
    return catalog_->containsTypeName(name);
}

bool SyntaxCorrelationDisambiguator::recognizesName(const std::string& name) const
{
    return catalog_->containsName(name);
}
