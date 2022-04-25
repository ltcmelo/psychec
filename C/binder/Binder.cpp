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
    // The outermost scope and symbol.
    scopes_.push(nullptr);
    syms_.push(nullptr);

    visit(tree_->root());

    PSYCHE_ASSERT(scopes_.top() == nullptr, return, "expected outermost scope");
    scopes_.pop();
    PSYCHE_ASSERT(scopes_.empty(), return, "unexpected remaining scope");

//    PSYCHE_ASSERT(symDEFs_.top() == nullptr, return, "expected outermost symbol");
    syms_.pop();
//    PSYCHE_ASSERT(symDEFs_.empty(), return, "unexpected remaining symbol");
}

template <class ScopeT>
void Binder::openScope()
{
    auto scope = syms_.top()->makeScope<ScopeT>();
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

template <class SymT>
SymT* Binder::pushSym(std::unique_ptr<SymT> sym)
{
    syms_.push(sym.get());
    return static_cast<SymT*>(semaModel_->storeSymDEF(std::move(sym)));
}

template FunctionSymbol* Binder::pushSym<FunctionSymbol>(std::unique_ptr<FunctionSymbol>);
template FieldSymbol* Binder::pushSym<FieldSymbol>(std::unique_ptr<FieldSymbol>);
template ParameterSymbol* Binder::pushSym<ParameterSymbol>(std::unique_ptr<ParameterSymbol>);
template VariableSymbol* Binder::pushSym<VariableSymbol>(std::unique_ptr<VariableSymbol>);
template ArrayTypeSymbol* Binder::pushSym<ArrayTypeSymbol>(std::unique_ptr<ArrayTypeSymbol>);
template NamedTypeSymbol* Binder::pushSym<NamedTypeSymbol>(std::unique_ptr<NamedTypeSymbol>);
template PointerTypeSymbol* Binder::pushSym<PointerTypeSymbol>(std::unique_ptr<PointerTypeSymbol>);

void Binder::popSym()
{
    syms_.pop();
}

template <class TySymT>
TySymT* Binder::pushTySym(std::unique_ptr<TySymT> tySym)
{
    tySyms_.push(tySym.get());
    return static_cast<TySymT*>(semaModel_->storeSymUSE(std::move(tySym)));
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
    makeSymAndPushIt<LinkUnitSymbol>();
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

    popSym();

    return Action::Skip;
}

SyntaxVisitor::Action Binder::visitStructOrUnionDeclaration(const StructOrUnionDeclarationSyntax* node)
{
    const TagTypeSpecifierSyntax* tySpec = node->typeSpecifier();
    TagSymbolName::NameSpace ns;
    switch (tySpec->kind()) {
        case StructTypeSpecifier:
            ns = TagSymbolName::NameSpace::Structures;
            break;

        case UnionTypeSpecifier:
            ns = TagSymbolName::NameSpace::Unions;
            break;

        default:
            PSYCHE_FAIL_0(return Action::Quit);
            return Action::Quit;
    }

    makeSymAndPushIt<NamedTypeSymbol>(ns, tySpec->tagToken().valueText_c_str());

    return visitTypeDeclaration_COMMON(node);
}

SyntaxVisitor::Action Binder::visitEnumDeclaration(const EnumDeclarationSyntax* node)
{
    makeSymAndPushIt<NamedTypeSymbol>(TagSymbolName::NameSpace::Enumerations,
                                      node->typeSpecifier()->tagToken().valueText_c_str());

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

SyntaxVisitor::Action Binder::visitParameterDeclaration(const ParameterDeclarationSyntax* node)
{
    for (auto specIt = node->specifiers(); specIt; specIt = specIt->next)
        visit(specIt->value);

    visit(node->declarator());

    return Action::Skip;
}

SyntaxVisitor::Action Binder::visitParameterDeclaration_DONE(const ParameterDeclarationSyntax*)
{
    popTySym();

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
