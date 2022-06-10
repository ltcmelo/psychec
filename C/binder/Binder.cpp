// Copyright (c) 2021/22 Leandro T. C. Melo <ltcmelo@gmail.com>
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

#include "binder/Scope.h"
#include "binder/ConstraintsInDeclarations.h"
#include "compilation/SemanticModel.h"
#include "symbols/Symbol_ALL.h"
#include "symbols/SymbolName_ALL.h"
#include "syntax/SyntaxFacts.h"
#include "syntax/SyntaxNodes.h"
#include "syntax/SyntaxUtilities.h"

#include "../common/infra/Assertions.h"

#include <iostream>

using namespace psy;
using namespace C;

Binder::Binder(SemanticModel* semaModel, const SyntaxTree* tree)
    : SyntaxVisitor(tree)
    , semaModel_(semaModel)
    , stashedScope_(nullptr)
    , diagReporter_(this)
{}

Binder::~Binder()
{
}

void Binder::bind()
{
    // The outermost scope and symbol.
    scopes_.push(nullptr);
    syms_.push(nullptr);

    visit(tree_->root());

    PSY_ASSERT_W_MSG(scopes_.top() == nullptr, return, "expected outermost scope");
    scopes_.pop();
    PSY_ASSERT_W_MSG(scopes_.empty(), return, "unexpected remaining scope");

//    PSY_ASSERT_W_MSG(symDEFs_.top() == nullptr, return, "expected outermost symbol");
    syms_.pop();
//    PSY_ASSERT_W_MSG(symDEFs_.empty(), return, "unexpected remaining symbol");
}

void Binder::openScope(ScopeKind scopeK)
{
    std::unique_ptr<Scope> scope(new Scope(scopeK));
    scopes_.push(scope.get());

    auto enclosingScope = scopes_.top();
    enclosingScope->enclose(std::move(scope));
}

void Binder::reopenStashedScope()
{
    PSY_ASSERT(stashedScope_, return);

    scopes_.push(stashedScope_);
}

void Binder::closeScope()
{
    scopes_.pop();
}

void Binder::closeScopeAndStashIt()
{
    stashedScope_ = scopes_.top();
    scopes_.pop();
}

template <class SymT>
SymT* Binder::pushSym(const SyntaxNode* node, std::unique_ptr<SymT> sym)
{
    syms_.push(sym.get());
    return static_cast<SymT*>(semaModel_->storeDeclaredSym(node, std::move(sym)));
}

template FunctionSymbol* Binder::pushSym<FunctionSymbol>(const SyntaxNode*, std::unique_ptr<FunctionSymbol>);
template FieldSymbol* Binder::pushSym<FieldSymbol>(const SyntaxNode*, std::unique_ptr<FieldSymbol>);
template EnumeratorSymbol* Binder::pushSym<EnumeratorSymbol>(const SyntaxNode*, std::unique_ptr<EnumeratorSymbol>);
template ParameterSymbol* Binder::pushSym<ParameterSymbol>(const SyntaxNode*, std::unique_ptr<ParameterSymbol>);
template VariableSymbol* Binder::pushSym<VariableSymbol>(const SyntaxNode*, std::unique_ptr<VariableSymbol>);
template ArrayTypeSymbol* Binder::pushSym<ArrayTypeSymbol>(const SyntaxNode*, std::unique_ptr<ArrayTypeSymbol>);
template NamedTypeSymbol* Binder::pushSym<NamedTypeSymbol>(const SyntaxNode*, std::unique_ptr<NamedTypeSymbol>);
template PointerTypeSymbol* Binder::pushSym<PointerTypeSymbol>(const SyntaxNode*, std::unique_ptr<PointerTypeSymbol>);

void Binder::popSym()
{
    syms_.pop();
}

template <class TySymT>
TySymT* Binder::pushTySym(std::unique_ptr<TySymT> tySym)
{
    tySyms_.push(tySym.get());
    return static_cast<TySymT*>(semaModel_->storeUsedSym(std::move(tySym)));
}

template ArrayTypeSymbol* Binder::pushTySym<ArrayTypeSymbol>(std::unique_ptr<ArrayTypeSymbol>);
template FunctionTypeSymbol* Binder::pushTySym<FunctionTypeSymbol>(std::unique_ptr<FunctionTypeSymbol>);
template NamedTypeSymbol* Binder::pushTySym<NamedTypeSymbol>(std::unique_ptr<NamedTypeSymbol>);
template PointerTypeSymbol* Binder::pushTySym<PointerTypeSymbol>(std::unique_ptr<PointerTypeSymbol>);

void Binder::popTySym()
{
    tySyms_.pop();
}

    //--------------//
    // Declarations //
    //--------------//

SyntaxVisitor::Action Binder::visitTranslationUnit(const TranslationUnitSyntax* node)
{
    makeSymAndPushIt<LibrarySymbol>(node);

    openScope(ScopeKind::File);

    for (auto declIt = node->declarations(); declIt; declIt = declIt->next)
        visit(declIt->value);

    closeScope();

    return Action::Skip;
}

SyntaxVisitor::Action Binder::visitIncompleteDeclaration(const IncompleteDeclarationSyntax* node)
{
    ConstraintsInDeclarations::UselessDeclaration(node->lastToken(), &diagReporter_);

    for (auto specIt = node->specifiers(); specIt; specIt = specIt->next)
        ;

    return Action::Skip;
}

SyntaxVisitor::Action Binder::visitStructOrUnionDeclaration(const StructOrUnionDeclarationSyntax* node)
{
    return visitStructOrUnionDeclaration_AtSpecifier(node);
}

SyntaxVisitor::Action Binder::visitStructOrUnionDeclaration_DONE(const StructOrUnionDeclarationSyntax* node)
{
    return Action::Skip;
}

SyntaxVisitor::Action Binder::visitEnumDeclaration(const EnumDeclarationSyntax* node)
{
    return visitEnumDeclaration_AtSpecifier(node);
}

SyntaxVisitor::Action Binder::visitEnumDeclaration_DONE(const EnumDeclarationSyntax* node)
{
    return Action::Skip;
}

SyntaxVisitor::Action Binder::visitVariableAndOrFunctionDeclaration(
        const VariableAndOrFunctionDeclarationSyntax* node)
{
    return visitVariableAndOrFunctionDeclaration_AtSpecifiers(node);
}

SyntaxVisitor::Action Binder::visitVariableAndOrFunctionDeclaration_DONE(
        const VariableAndOrFunctionDeclarationSyntax*)
{
    popTySym();

    return Action::Skip;
}

SyntaxVisitor::Action Binder::visitFieldDeclaration(const FieldDeclarationSyntax* node)
{
    return visitFieldDeclaration_AtSpecifiers(node);
}

SyntaxVisitor::Action Binder::visitFieldDeclaration_DONE(const FieldDeclarationSyntax*)
{
    popTySym();

    return Action::Skip;
}

SyntaxVisitor::Action Binder::visitEnumeratorDeclaration(const EnumeratorDeclarationSyntax* node)
{
    return visitEnumeratorDeclaration_AtImplicitSpecifier(node);
}

SyntaxVisitor::Action Binder::visitEnumeratorDeclaration_DONE(const EnumeratorDeclarationSyntax* node)
{
    popTySym();

    return Action::Skip;
}

SyntaxVisitor::Action Binder::visitParameterDeclaration(const ParameterDeclarationSyntax* node)
{
    return visitParameterDeclaration_AtSpecifiers(node);
}

SyntaxVisitor::Action Binder::visitParameterDeclaration_DONE(const ParameterDeclarationSyntax*)
{
    return Action::Skip;
}

SyntaxVisitor::Action Binder::visitStaticAssertDeclaration(const StaticAssertDeclarationSyntax*)
{
    return Action::Skip;
}

SyntaxVisitor::Action Binder::visitFunctionDefinition(const FunctionDefinitionSyntax* node)
{
    return visitFunctionDefinition_AtSpecifiers(node);
}

SyntaxVisitor::Action Binder::visitFunctionDefinition_DONE(const FunctionDefinitionSyntax* node)
{
    popTySym();

    return Action::Skip;
}

    //------------//
    // Statements //
    //------------//

SyntaxVisitor::Action Binder::visitCompoundStatement(const CompoundStatementSyntax* node)
{
    openScope(ScopeKind::Block);

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
