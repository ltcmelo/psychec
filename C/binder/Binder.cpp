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

#include "binder/Scopes.h"
#include "binder/Semantics_TypeSpecifiers.h"
#include "compilation/SemanticModel.h"
#include "symbols/Symbols.h"
#include "symbols/SymbolNames.h"
#include "syntax/SyntaxFacts.h"
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
{
}

void Binder::bind()
{
    visit(tree_->root());
}

template <class SymT>
void Binder::makeAndPushSymDEF()
{
    std::unique_ptr<SymT> sym(new SymT(tree_,
                                       scopes_.top(),
                                       symDEFs_.top()));
    pushSymDEF(std::move(sym));
}

template <> void Binder::makeAndPushSymDEF<LinkUnitSymbol>()
{
    std::unique_ptr<LinkUnitSymbol> sym(new LinkUnitSymbol(tree_, nullptr, nullptr));
    pushSymDEF(std::move(sym));
}

template void Binder::makeAndPushSymDEF<FieldSymbol>();
template void Binder::makeAndPushSymDEF<FunctionSymbol>();
template void Binder::makeAndPushSymDEF<ParameterSymbol>();
template void Binder::makeAndPushSymDEF<VariableSymbol>();

void Binder::makeAndPushSymDEF(TypeKind tyKind)
{
    std::unique_ptr<NamedTypeSymbol> sym(
                new NamedTypeSymbol(tree_,
                                    scopes_.top(),
                                    symDEFs_.top(),
                                    tyKind));
    pushSymDEF(std::move(sym));
}

template <class SymT>
void Binder::pushSymDEF(std::unique_ptr<SymT> sym)
{
    symDEFs_.push(sym.get());
    semaModel_->storeSymDEF(std::move(sym));
}

template void Binder::pushSymDEF<FunctionSymbol>(std::unique_ptr<FunctionSymbol>);
template void Binder::pushSymDEF<FieldSymbol>(std::unique_ptr<FieldSymbol>);
template void Binder::pushSymDEF<ParameterSymbol>(std::unique_ptr<ParameterSymbol>);
template void Binder::pushSymDEF<VariableSymbol>(std::unique_ptr<VariableSymbol>);
template void Binder::pushSymDEF<ArrayTypeSymbol>(std::unique_ptr<ArrayTypeSymbol>);
template void Binder::pushSymDEF<NamedTypeSymbol>(std::unique_ptr<NamedTypeSymbol>);
template void Binder::pushSymDEF<PointerTypeSymbol>(std::unique_ptr<PointerTypeSymbol>);

void Binder::popSymDEF()
{
    symDEFs_.pop();
}

template <class TySymT>
void Binder::pushTySymUSE(std::unique_ptr<TySymT> tySym)
{
    tySymUSEs_.push(tySym.get());
    semaModel_->storeSymUSE(std::move(tySym));
}

template void Binder::pushTySymUSE<ArrayTypeSymbol>(std::unique_ptr<ArrayTypeSymbol>);
template void Binder::pushTySymUSE<NamedTypeSymbol>(std::unique_ptr<NamedTypeSymbol>);
template void Binder::pushTySymUSE<PointerTypeSymbol>(std::unique_ptr<PointerTypeSymbol>);

void Binder::popTySymUSE()
{
    tySymUSEs_.pop();
}

template <class TySymT>
void Binder::makeAndPushTySymUSE()
{
    std::unique_ptr<TySymT> tySym(new TySymT(tree_,
                                             scopes_.top(),
                                             symDEFs_.top(),
                                             tySymUSEs_.top()));
   pushTySymUSE(std::move(tySym));
}

void Binder::makeAndPushTySymUSE(TypeKind tyKind)
{
    std::unique_ptr<NamedTypeSymbol> tySym(
                new NamedTypeSymbol(tree_,
                                    scopes_.top(),
                                    symDEFs_.top(),
                                    tyKind));
    pushTySymUSE(std::move(tySym));
}

template void Binder::makeAndPushTySymUSE<ArrayTypeSymbol>();
template void Binder::makeAndPushTySymUSE<PointerTypeSymbol>();

template <class ScopeT>
void Binder::openScope()
{
    auto scope = symDEFs_.top()->makeScope<ScopeT>();
    scopes_.push(scope);
}

void Binder::openNestedScope()
{
    auto scope = scopes_.top()->makeNestedScope();
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
    makeAndPushSymDEF<LinkUnitSymbol>();
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

SyntaxVisitor::Action Binder::visitTypeDeclaration_COMMON(const TypeDeclarationSyntax* node)
{
    visit(node->typeSpecifier());

    return Action::Skip;
}

SyntaxVisitor::Action Binder::visitStructOrUnionDeclaration(const StructOrUnionDeclarationSyntax* node)
{
    return visitTypeDeclaration_COMMON(node);
}

SyntaxVisitor::Action Binder::visitEnumDeclaration(const EnumDeclarationSyntax* node)
{
    return visitTypeDeclaration_COMMON(node);
}

SyntaxVisitor::Action Binder::visitVariableAndOrFunctionDeclaration(
        const VariableAndOrFunctionDeclarationSyntax* node)
{
    return visitVariableAndOrFunctionDeclaration_AtSpecifiers(node);
}

SyntaxVisitor::Action Binder::visitVariableAndOrFunctionDeclaration_DONE(
        const VariableAndOrFunctionDeclarationSyntax*)
{
    popTySymUSE();

    return Action::Skip;
}

SyntaxVisitor::Action Binder::visitFieldDeclaration(const FieldDeclarationSyntax* node)
{
    return visitFieldDeclaration_AtSpecifiers(node);
}

SyntaxVisitor::Action Binder::visitFieldDeclaration_DONE(const FieldDeclarationSyntax*)
{
    popTySymUSE();

    return Action::Skip;
}

SyntaxVisitor::Action Binder::visitParameterDeclaration(const ParameterDeclarationSyntax* node)
{
    for (auto specIt = node->specifiers(); specIt; specIt = specIt->next)
        visit(specIt->value);

    visit(node->declarator());

    return Action::Skip;
}

SyntaxVisitor::Action Binder::visitParameterDeclaration_DONE(const ParameterDeclarationSyntax*)
{
    popTySymUSE();

    return Action::Skip;
}

SyntaxVisitor::Action Binder::visitStaticAssertDeclaration(const StaticAssertDeclarationSyntax*)
{
    return Action::Skip;
}

SyntaxVisitor::Action Binder::visitFunctionDefinition(const FunctionDefinitionSyntax* node)
{
    for (auto specIt = node->specifiers(); specIt; specIt = specIt->next)
        visit(specIt->value);

    visit(node->declarator());

    visit(node->body());

    return Action::Skip;
}

//------------//
// Statements //
//------------//
SyntaxVisitor::Action Binder::visitCompoundStatement(const CompoundStatementSyntax* node)
{
    openNestedScope();

    for (auto stmtIt = node->statements(); stmtIt; stmtIt = stmtIt->next)
        visit(stmtIt->value);

    closeScope();

    return Action::Skip;
}

SyntaxVisitor::Action Binder::visitDeclarationStatement(const DeclarationStatementSyntax* node)
{
    visit(node->declaration());

    return Action::Skip;
}
