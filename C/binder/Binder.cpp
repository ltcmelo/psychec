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

#include "compilation/SemanticModel.h"
#include "binder/Scopes.h"
#include "symbols/Symbols.h"
#include "syntax/SyntaxNodes.h"
#include "syntax/SyntaxUtilities.h"

#include "../common/infra/PsycheAssert.h"

#include <iostream>

using namespace psy;
using namespace C;

Binder::Binder(SemanticModel* semaModel, const SyntaxTree* tree)
    : SyntaxVisitor(tree)
    , semaModel_(semaModel)
    , diagReporter_(this)
{}

Binder::~Binder()
{}

void Binder::bind()
{
    visit(tree_->root());
}

template <class SymbolT>
SymbolT* Binder::newSymbol_COMMON(std::unique_ptr<SymbolT> sym)
{
    syms_.push(sym.get());
    return static_cast<SymbolT*>(semaModel_->storeSymbol(std::move(sym)));
}

template <class SymbolT>
SymbolT* Binder::newSymbol()
{
    std::unique_ptr<SymbolT> sym(new SymbolT(tree_,
                                             scopes_.top(),
                                             syms_.top()));
    return newSymbol_COMMON(std::move(sym));
}

template FieldSymbol* Binder::newSymbol<FieldSymbol>();
template FunctionSymbol* Binder::newSymbol<FunctionSymbol>();
template ParameterSymbol* Binder::newSymbol<ParameterSymbol>();
template VariableSymbol* Binder::newSymbol<VariableSymbol>();

template <>
LinkUnitSymbol* Binder::newSymbol<LinkUnitSymbol>()
{
    std::unique_ptr<LinkUnitSymbol> sym(new LinkUnitSymbol(tree_, nullptr, nullptr));
    return newSymbol_COMMON(std::move(sym));
}

template <class ScopeT>
void Binder::openScope()
{
    auto scope = syms_.top()->newScope<ScopeT>();
    scopes_.push(scope);
}

void Binder::closeScope()
{
    scopes_.pop();
}

//--------------//
// Declarations //
//--------------//

SyntaxVisitor::Action Binder::visitTranslationUnit(const TranslationUnitSyntax* node)
{
    newSymbol<LinkUnitSymbol>();
    openScope<FileScope>();

    for (auto declIt = node->declarations(); declIt; declIt = declIt->next)
        visit(declIt->value);

    closeScope();

    return Action::Skip;
}

SyntaxVisitor::Action Binder::visitIncompleteDeclaration(const IncompleteDeclarationSyntax* node)
{
    diagReporter_.UselessDeclaration(node->lastToken());

    for (auto specIt = node->specifiers(); specIt; specIt = specIt->next)
        ;

    return Action::Skip;
}

SyntaxVisitor::Action Binder::visitVariableAndOrFunctionDeclaration(const VariableAndOrFunctionDeclarationSyntax* node)
{
    for (auto specIt = node->specifiers(); specIt; specIt = specIt->next)
        ;

    for (auto decltorIt = node->declarators(); decltorIt; decltorIt = decltorIt->next) {
        Symbol* sym;
        auto decltor = SyntaxUtilities::strippedDeclarator(decltorIt->value);
        switch (decltor->kind()) {
            case FunctionDeclarator:
                sym = newSymbol<FunctionSymbol>();
                break;

            case ArrayDeclarator:
            case IdentifierDeclarator:
                break;

            case BitfieldDeclarator:
                sym = newSymbol<FieldSymbol>();
                break;

            default:
                std::cout << "declarator " << to_string(decltorIt->value->kind()) << std::endl;
                PSYCHE_FAIL(return Action::Quit, "unknown declarator");
                break;
        }
        visit(decltorIt->value);
    }

    return Action::Skip;
}

/* Declarators */

SyntaxVisitor::Action Binder::visitIdentifierDeclarator(const IdentifierDeclaratorSyntax* node)
{
    return Action::Skip;
}
