// Copyright (c) 2021 Leandro T. C. Melo <ltcmelo@gmail.com>
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

#include "Binder.h"

#include "SyntaxTree.h"
#include "syntax/SyntaxNodes.h"

#include <iostream>

using namespace psy;
using namespace C;

Binder::Binder(const SyntaxTree* tree)
    : SyntaxVisitor(tree)
    , diagnosticsReporter_(this)
{}

Binder::~Binder()
{}

void Binder::bind()
{
    visit(tree_->root());
}

//--------------//
// Declarations //
//--------------//

SyntaxVisitor::Action Binder::visitTranslationUnit(const TranslationUnitSyntax* node)
{
    for (auto declIt = node->declarations(); declIt; declIt = declIt->next)
        visit(declIt->value);
    return Action::Skip;
}

SyntaxVisitor::Action Binder::visitIncompleteDeclaration(const IncompleteDeclarationSyntax* node)
{
    diagnosticsReporter_.UselessDeclaration(node->lastToken());

    for (auto specIt = node->specifiers(); specIt; specIt = specIt->next)
        ;
    return Action::Skip;
}

SyntaxVisitor::Action Binder::visitVariableAndOrFunctionDeclaration(const VariableAndOrFunctionDeclarationSyntax* node)
{
    for (auto specIt = node->specifiers(); specIt; specIt = specIt->next)
        ;

    for (auto decltorIt = node->declarators(); decltorIt; decltorIt = decltorIt->next) {
        switch (decltorIt->value->kind()) {
            case FunctionDeclarator:
                std::cout << "its a function declarator "<< std::endl;
                    break;

        default:
            std::cout << "its an object declarator \n";
                break;
        }
    }

    return Action::Skip;
}

/* Declarators */

SyntaxVisitor::Action Binder::visitIdentifierDeclarator(const IdentifierDeclaratorSyntax* node)
{
    return Action::Skip;
}
