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

#include "Reparser_SyntaxCorrelation.h"

#include "syntax/SyntaxNodes.h"

#include "../common/infra/Assertions.h"
#include "../common/infra/Escape.h"

#include <iostream>

using namespace psy;
using namespace C;

SyntaxCorrelationReparser::SyntaxCorrelationReparser(SyntaxTree* tree)
    : Reparser(tree)
    , pendingAmbigs_(0)
{}

void SyntaxCorrelationReparser::acquireCatalog(std::unique_ptr<DisambiguationCatalog> catalog)
{
    catalog_ = std::move(catalog);
}

unsigned int SyntaxCorrelationReparser::reparse()
{
    visit(tree_->root());

    return pendingAmbigs_;
}

SyntaxVisitor::Action SyntaxCorrelationReparser::visitTranslationUnit(const TranslationUnitSyntax* node)
{
    catalog_->enterLevel(node);

    for (auto iter = node->declarations(); iter; iter = iter->next)
        visit(iter->value);

    catalog_->exitLevel();

    return Action::Skip;
}

Reparser::ExpressionOrDeclarationStatement
SyntaxCorrelationReparser::keepExpressionOrDeclarationStatement(const std::string& maybeTyName)
{
    if (catalog_->isCatalogedAsType(maybeTyName)) {
        std::cout << "catalog contains as type " << maybeTyName << std::endl;
        return Reparser::ExpressionOrDeclarationStatement::Declaration;
    }

    std::cout << "catalog DOES NOT contain as type " << maybeTyName << std::endl;
    return Reparser::ExpressionOrDeclarationStatement::Expression;
}
