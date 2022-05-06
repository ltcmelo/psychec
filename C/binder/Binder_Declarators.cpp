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
#include "binder/Semantics_TypeSpecifiers.h"
#include "compilation/SemanticModel.h"
#include "symbols/Symbol_ALL.h"
#include "symbols/SymbolName_ALL.h"
#include "syntax/SyntaxFacts.h"
#include "syntax/SyntaxNodes.h"
#include "syntax/SyntaxUtilities.h"

#include "../common/infra/Assertions.h"
#include "../common/infra/Traces.h"

#include <iostream>

using namespace psy;
using namespace C;

template <class DeclT>
SyntaxVisitor::Action Binder::visitDeclaration_AtDeclarators(
        const DeclT* node,
        Action (Binder::*visit_DONE)(const DeclT*))
{
    for (auto decltorIt = node->declarators(); decltorIt; decltorIt = decltorIt->next)
        actOnDeclarator(decltorIt->value);

    return ((this)->*(visit_DONE))(node);
}

SyntaxVisitor::Action Binder::visitVariableAndOrFunctionDeclaration_AtDeclarators(
        const VariableAndOrFunctionDeclarationSyntax* node)
{
    return visitDeclaration_AtDeclarators(
                node,
                &Binder::visitVariableAndOrFunctionDeclaration_DONE);
}

SyntaxVisitor::Action Binder::visitFieldDeclaration_AtDeclarators(const FieldDeclarationSyntax* node)
{
    return visitDeclaration_AtDeclarators(
                node,
                &Binder::visitFieldDeclaration_DONE);
}

SyntaxVisitor::Action Binder::visitParameterDeclaration_AtDeclarator(const ParameterDeclarationSyntax* node)
{
    actOnDeclarator(node->declarator());

    return visitParameterDeclaration_DONE(node);
}

SyntaxVisitor::Action Binder::visitFunctionDefinition_AtDeclarator(const FunctionDefinitionSyntax* node)
{
    actOnDeclarator(node->declarator());

    reopenStashedScope();
    scopes_.top()->morphFrom_FunctionPrototype_to_Block();

    auto body = node->body()->asCompoundStatement();
    for (auto stmtIt = body->statements(); stmtIt; stmtIt = stmtIt->next)
        visit(stmtIt->value);

    closeScope();

    return Binder::visitFunctionDefinition_DONE(node);
}

SyntaxVisitor::Action Binder::actOnDeclarator(const DeclaratorSyntax* decltor)
{
    visit(decltor);

    auto sym = syms_.top();
    popSym();

    auto tySym = tySyms_.top();

    if (!pendingFunTySyms_.empty())
        pendingFunTySyms_.top()->addParameter(tySym);

    switch (tySym->typeKind()) {
        case TypeKind::Array:
        case TypeKind::Function:
        case TypeKind::Pointer:
            popTySym();
            break;

        case TypeKind::Named:
            break;

        default:
            PSY_TRACE_ESCAPE_0(return Action::Quit);
    }

    auto typeableSym = TypeClass_TypeableSymbol::asInstance(sym);
    PSY_ASSERT_0(typeableSym, return Action::Quit);
    typeableSym->setType(tySym);

    return Action::Skip;
}

/* Declarators */

SyntaxVisitor::Action Binder::visitArrayOrFunctionDeclarator(const ArrayOrFunctionDeclaratorSyntax* node)
{
    for (auto specIt = node->attributes(); specIt; specIt = specIt->next)
        visit(specIt->value);

    switch (node->suffix()->kind()) {
        case SubscriptSuffix:
            makeTySymAndPushIt<ArrayTypeSymbol>(tySyms_.top());
            break;

        case ParameterSuffix:
            makeTySymAndPushIt<FunctionTypeSymbol>(tySyms_.top());
            break;

        default:
            PSY_TRACE_ESCAPE_0(return Action::Quit);
    }

    visit(node->innerDeclarator());

    switch (tySyms_.top()->typeKind()) {
        case TypeKind::Function:
            pendingFunTySyms_.push(tySyms_.top()->asFunctionType());
            break;

        case TypeKind::Array:
            break;

    default:
            PSY_TRACE_ESCAPE_0(return Action::Quit);
    }

    openScope(ScopeKind::FunctionPrototype);
    visit(node->suffix());
    closeScopeAndStashIt();

    switch (tySyms_.top()->typeKind()) {
        case TypeKind::Function:
            pendingFunTySyms_.pop();
            break;

        case TypeKind::Array:
            break;

        default:
            PSY_TRACE_ESCAPE_0(return Action::Quit);
    }

    return Action::Skip;
}

SyntaxVisitor::Action Binder::visitSubscriptSuffix(const SubscriptSuffixSyntax* node)
{
    return Action::Skip;
}

SyntaxVisitor::Action Binder::visitParameterSuffix(const ParameterSuffixSyntax* node)
{
    for (auto declIt = node->parameters(); declIt; declIt = declIt->next) {
        TySymContT tySyms;
        std::swap(tySyms_, tySyms);
        visit(declIt->value);
        std::swap(tySyms_, tySyms);
    }

    return Action::Skip;
}

SyntaxVisitor::Action Binder::visitPointerDeclarator(const PointerDeclaratorSyntax* node)
{
    makeTySymAndPushIt<PointerTypeSymbol>(tySyms_.top());

    for (auto specIt = node->qualifiersAndAttributes(); specIt; specIt = specIt->next)
        visit(specIt->value);

    visit(node->innerDeclarator());

    return Action::Skip;
}

SyntaxVisitor::Action Binder::visitParenthesizedDeclarator(const ParenthesizedDeclaratorSyntax* node)
{
    visit(node->innerDeclarator());

    return Action::Skip;
}

SyntaxVisitor::Action Binder::visitIdentifierDeclarator(const IdentifierDeclaratorSyntax* node)
{
    auto tySym = tySyms_.top();
    switch (tySym->typeKind()) {
        case TypeKind::Function:
            makeSymAndPushIt<FunctionSymbol>();
            break;

        case TypeKind::Array:
        case TypeKind::Named:
        case TypeKind::Pointer:
            switch (syms_.top()->kind())
            {
                case SymbolKind::Type:
                    makeSymAndPushIt<FieldSymbol>();
                    break;

                case SymbolKind::Library:
                    makeSymAndPushIt<VariableSymbol>();
                    break;

                case SymbolKind::Function:
                    switch (scopes_.top()->kind()) {
                        case ScopeKind::FunctionPrototype:
                            makeSymAndPushIt<ParameterSymbol>();
                            break;

                        case ScopeKind::Block:
                        case ScopeKind::File:
                            makeSymAndPushIt<VariableSymbol>();
                            break;

                        default:
                            PSY_TRACE_ESCAPE_0(return Action::Quit);
                    }
                    break;

                default:
                    PSY_TRACE_ESCAPE_0(return Action::Quit);
            }
            break;

        default:
            PSY_TRACE_ESCAPE_0(break);
    }

    Symbol* sym = syms_.top();
    auto nameableSym = TypeClass_NameableSymbol::asInstance(sym);

    std::unique_ptr<SymbolName> name(
                new PlainSymbolName(node->identifierToken().valueText_c_str()));
    nameableSym->setName(std::move(name));

    return Action::Skip;
}

SyntaxVisitor::Action Binder::visitAbstractDeclarator(const AbstractDeclaratorSyntax*)
{
    auto tySym = tySyms_.top();
    switch (tySym->typeKind()) {
        case TypeKind::Function:
            makeSymAndPushIt<FunctionSymbol>();
            break;

        case TypeKind::Array:
        case TypeKind::Named:
        case TypeKind::Pointer:
            switch (syms_.top()->kind())
            {
                case SymbolKind::Type:
                    makeSymAndPushIt<FieldSymbol>();
                    break;

                case SymbolKind::Library:
                    makeSymAndPushIt<VariableSymbol>();
                    break;

                case SymbolKind::Function:
                    switch (scopes_.top()->kind()) {
                        case ScopeKind::FunctionPrototype:
                            makeSymAndPushIt<ParameterSymbol>();
                            break;

                        case ScopeKind::Block:
                        case ScopeKind::File:
                            makeSymAndPushIt<VariableSymbol>();
                            break;

                        default:
                            PSY_TRACE_ESCAPE_0(return Action::Quit);
                    }
                    break;

                default:
                    PSY_TRACE_ESCAPE_0(return Action::Quit);
            }
            break;

        default:
            PSY_TRACE_ESCAPE_0(break);
    }

    Symbol* sym = syms_.top();
    auto nameableSym = TypeClass_NameableSymbol::asInstance(sym);

    std::unique_ptr<SymbolName> name(new EmptySymbolName);

    nameableSym->setName(std::move(name));

    return Action::Skip;
}
