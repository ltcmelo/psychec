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
#include "binder/ConstraintsInDeclarators.h"
#include "compilation/SemanticModel.h"
#include "symbols/Symbol_ALL.h"
#include "symbols/SymbolName_ALL.h"
#include "syntax/SyntaxFacts.h"
#include "syntax/SyntaxNodes.h"
#include "syntax/SyntaxUtilities.h"

#include "../common/infra/Assertions.h"
#include "../common/infra/Escape.h"

#include <iostream>

using namespace psy;
using namespace C;

SyntaxVisitor::Action Binder::nameSymAtTop(const char* s)
{
    PSY_ASSERT(!syms_.empty(), return Action::Quit);
    Symbol* sym = syms_.top();

    auto nameableSym = TypeClass_NameableSymbol::asInstance(sym);
    PSY_ASSERT(nameableSym, return Action::Quit);

    std::unique_ptr<SymbolName> name;
    if (s)
        name.reset(new PlainSymbolName(s));
    else
        name.reset(new EmptySymbolName);
    nameableSym->setName(std::move(name));

    return Action::Skip;
}

SyntaxVisitor::Action Binder::typeSymAtTopAndPopIt()
{
    PSY_ASSERT(!syms_.empty(), return Action::Quit);
    auto sym = syms_.top();

    auto typeableSym = TypeClass_TypeableSymbol::asInstance(sym);
    PSY_ASSERT(typeableSym, return Action::Quit);

    PSY_ASSERT(!tySyms_.empty(), return Action::Quit);
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
            PSY_ESCAPE_VIA_RETURN(Action::Quit);
    }

    typeableSym->setType(tySym);

    popSym();

    return Action::Skip;
}

template <class DeclT>
SyntaxVisitor::Action Binder::visitDeclaration_AtDeclarators_COMMON(
        const DeclT* node,
        Action (Binder::*visit_DONE)(const DeclT*))
{
    for (auto decltorIt = node->declarators(); decltorIt; decltorIt = decltorIt->next) {
        visit(decltorIt->value);
        typeSymAtTopAndPopIt();
    }

    return ((this)->*(visit_DONE))(node);
}

SyntaxVisitor::Action Binder::visitVariableAndOrFunctionDeclaration_AtDeclarators(
        const VariableAndOrFunctionDeclarationSyntax* node)
{
    return visitDeclaration_AtDeclarators_COMMON(
                node,
                &Binder::visitVariableAndOrFunctionDeclaration_DONE);
}

SyntaxVisitor::Action Binder::visitFieldDeclaration_AtDeclarators(const FieldDeclarationSyntax* node)
{
    return visitDeclaration_AtDeclarators_COMMON(
                node,
                &Binder::visitFieldDeclaration_DONE);
}

SyntaxVisitor::Action Binder::visitEnumeratorDeclaration_AtDeclarator(const EnumeratorDeclarationSyntax* node)
{
    determineContextAndMakeSym(node);
    nameSymAtTop(node->identifierToken().valueText_c_str());
    typeSymAtTopAndPopIt();

    return visitEnumeratorDeclaration_DONE(node);
}

SyntaxVisitor::Action Binder::visitParameterDeclaration_AtDeclarator(const ParameterDeclarationSyntax* node)
{
    visit(node->declarator());

    typeSymAtTopAndPopIt();

    return visitParameterDeclaration_DONE(node);
}

SyntaxVisitor::Action Binder::visitFunctionDefinition_AtDeclarator(const FunctionDefinitionSyntax* node)
{
    visit(node->declarator());

    typeSymAtTopAndPopIt();

    reopenStashedScope();
    scopes_.top()->morphFrom_FunctionPrototype_to_Block();

    auto body = node->body()->asCompoundStatement();
    for (auto stmtIt = body->statements(); stmtIt; stmtIt = stmtIt->next)
        visit(stmtIt->value);

    closeScope();

    return Binder::visitFunctionDefinition_DONE(node);
}

SyntaxVisitor::Action Binder::visitArrayOrFunctionDeclarator(const ArrayOrFunctionDeclaratorSyntax* node)
{
    for (auto specIt = node->attributes(); specIt; specIt = specIt->next)
        visit(specIt->value);

    switch (node->suffix()->kind()) {
        case SubscriptSuffix:
            makeTySymAndPushIt<ArrayTypeSymbol>(tySyms_.top());
            break;

        case ParameterSuffix: {
            auto tySym = tySyms_.top();
            switch (tySym->typeKind()) {
                case TypeKind::Function:
                    ConstraintsInDeclarators::FunctionReturningFunction(
                                node->innerDeclarator()->firstToken(),
                                &diagReporter_);
                    break;

                case TypeKind::Array:
                    ConstraintsInDeclarators::FunctionReturningArray(
                                node->innerDeclarator()->firstToken(),
                                &diagReporter_);
                    break;

                case TypeKind::Pointer:
                case TypeKind::Named:
                    break;

                default:
                    PSY_ESCAPE_VIA_RETURN(Action::Quit);
            }
            makeTySymAndPushIt<FunctionTypeSymbol>(tySyms_.top());
            pendingFunTySyms_.push(tySyms_.top()->asFunctionType());
            break;
        }

        default:
            PSY_ESCAPE_VIA_RETURN(Action::Quit);
    }

    visit(node->innerDeclarator());

    openScope(ScopeKind::FunctionPrototype);
    visit(node->suffix());
    closeScopeAndStashIt();

    if (node->suffix()->kind() == ParameterSuffix)
        pendingFunTySyms_.pop();

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

template <class DecltrT>
SyntaxVisitor::Action Binder::determineContextAndMakeSym(const DecltrT* node)
{
    switch (scopes_.top()->kind()) {
        case ScopeKind::File:
        case ScopeKind::Block: {
            TypeSymbol* tySym = tySyms_.top();
            switch (tySym->typeKind()) {
                case TypeKind::Function:
                    makeSymAndPushIt<FunctionSymbol>(node);
                    break;

                case TypeKind::Array:
                case TypeKind::Pointer:
                case TypeKind::Named: {
                    Symbol* sym = syms_.top();
                    switch (sym->kind()) {
                        case SymbolKind::Type:
                            PSY_ASSERT(sym->asType()->asNamedType()
                                            && sym->asType()->asNamedType()->name()
                                            && sym->asType()->asNamedType()->name()->asTagSymbolName(),
                                       return Action::Quit);

                            switch (sym->asType()->asNamedType()->name()->asTagSymbolName()->tagChoice()) {
                                case TagSymbolName::TagChoice::Union:
                                case TagSymbolName::TagChoice::Struct:
                                    makeSymAndPushIt<FieldSymbol>(node);
                                    break;

                                case TagSymbolName::TagChoice::Enum:
                                    makeSymAndPushIt<EnumeratorSymbol>(node);
                                    break;

                                default:
                                    PSY_ESCAPE_VIA_RETURN(Action::Quit);
                            }
                            break;

                        case SymbolKind::Value:
                        case SymbolKind::Function:
                        case SymbolKind::Library:
                            makeSymAndPushIt<VariableSymbol>(node);
                            break;

                        default:
                            PSY_ESCAPE_VIA_RETURN(Action::Quit);
                    }
                    break;
                }

                default:
                    PSY_ESCAPE_VIA_RETURN(Action::Quit);
            }
            break;
        }

        case ScopeKind::Function:
            break;

        case ScopeKind::FunctionPrototype: {
            TypeSymbol* tySym = tySyms_.top();
            switch (tySym->typeKind()) {
                case TypeKind::Array: {
                    /*
                     * 6.7.6.3-7
                     * A declaration of a parameter as “array of type”
                     * shall be adjusted to “qualified pointer to type”...
                     */
                    popTySym();
                    makeTySymAndPushIt<PointerTypeSymbol>(tySyms_.top());
                    auto ptrTySym = tySyms_.top()->asPointerType();
                    ptrTySym->markAsArisingFromArrayDecay();
                    break;
                }

                case TypeKind::Function: {
                    /*
                     * 6.7.6.3-8
                     * A declaration of a parameter as “function returning type”
                     * shall be adjusted to “pointer to function returning type”...
                     */
                    makeTySymAndPushIt<PointerTypeSymbol>(tySyms_.top());
                    auto ptrTySym = tySyms_.top()->asPointerType();
                    ptrTySym->markAsArisingFromFunctionDecay();
                    break;
                }

                case TypeKind::Pointer:
                case TypeKind::Named:
                    break;

                default:
                    PSY_ESCAPE_VIA_RETURN(Action::Quit);
            }
            makeSymAndPushIt<ParameterSymbol>(node);
            break;
        }

        default:
            PSY_ESCAPE_VIA_RETURN(Action::Quit);
    }

    return Action::Skip;
}

SyntaxVisitor::Action Binder::visitIdentifierDeclarator(const IdentifierDeclaratorSyntax* node)
{
    determineContextAndMakeSym(node);
    nameSymAtTop(node->identifierToken().valueText_c_str());

    return Action::Skip;
}

SyntaxVisitor::Action Binder::visitAbstractDeclarator(const AbstractDeclaratorSyntax* node)
{
    determineContextAndMakeSym(node);
    nameSymAtTop(nullptr);

    return Action::Skip;
}
