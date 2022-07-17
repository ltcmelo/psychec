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

#include "DisambiguationCataloger.h"

#include "../common/infra/Assertions.h"
#include "../common/infra/Escape.h"

#include "syntax/SyntaxNodes.h"

using namespace psy;
using namespace C;

DisambiguationCataloger::DisambiguationCataloger(SyntaxTree* tree)
    : SyntaxVisitor(tree)
    , catalog_(new DisambiguationCatalog)
{}

std::unique_ptr<DisambiguationCatalog> DisambiguationCataloger::catalogFor(const SyntaxNode* node)
{
    visit(node);
    return std::move(catalog_);
}

SyntaxVisitor::Action DisambiguationCataloger::visitTranslationUnit(const TranslationUnitSyntax* node)
{
    catalog_->createLevelAndEnter(node);

    for (auto iter = node->declarations(); iter; iter = iter->next)
        visit(iter->value);

    catalog_->exitLevel();

    std::cout << "CATALOG\n" << *catalog_ << std::endl;

    return Action::Skip;
}

SyntaxVisitor::Action DisambiguationCataloger::visitTypedefName(const TypedefNameSyntax* node)
{
    catalog_->catalogAsType(node->identifierToken().valueText());

    return Action::Skip;
}

SyntaxVisitor::Action DisambiguationCataloger::visitIdentifierDeclarator(const IdentifierDeclaratorSyntax* node)
{
    catalog_->catalogAsNonType(node->identifierToken().valueText());

    visit(node->initializer());

    return Action::Skip;
}

SyntaxVisitor::Action DisambiguationCataloger::visitIdentifierName(const IdentifierNameSyntax* node)
{
    catalog_->catalogAsNonType(node->identifierToken().valueText());

    return Action::Skip;
}

SyntaxVisitor::Action DisambiguationCataloger::visitAmbiguousTypeNameOrExpressionAsTypeReference(
        const AmbiguousTypeNameOrExpressionAsTypeReferenceSyntax* node)
{
    return Action::Skip;
}

SyntaxVisitor::Action DisambiguationCataloger::visitAmbiguousCastOrBinaryExpression(
        const AmbiguousCastOrBinaryExpressionSyntax* node)
{
    visit(node->binaryExpression()->right());

    return Action::Skip;
}

SyntaxVisitor::Action DisambiguationCataloger::visitAmbiguousExpressionOrDeclarationStatement(
        const AmbiguousExpressionOrDeclarationStatementSyntax* node)
{
    auto expr = node->expressionStatement()->expression();
    switch (expr->kind()) {
        case MultiplyExpression: {
            auto binExpr = expr->asBinaryExpression();
            visit(binExpr->right());
            break;
        }

        case CallExpression: {
            auto callExpr = expr->asCallExpression();
            visit(callExpr->arguments()->value);
            break;
        }

        default:
            PSY_ESCAPE_VIA_RETURN(Action::Skip);
    }

    return Action::Skip;
}

