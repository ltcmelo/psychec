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
#include "Binder__MACROS__.inc"

#include "SyntaxTree.h"

#include "binder/Scope.h"
#include "compilation/SemanticModel.h"
#include "symbols/Symbol_ALL.h"
#include "syntax/SyntaxFacts.h"
#include "syntax/Lexeme_ALL.h"
#include "syntax/SyntaxNodes.h"
#include "syntax/SyntaxUtilities.h"
#include "types/Type_ALL.h"

#include "../common/infra/Assertions.h"
#include "../common/infra/Escape.h"

#include <iostream>

using namespace psy;
using namespace C;

SyntaxVisitor::Action Binder::nameSymAtTop(const Identifier* name)
{
    SYM_AT_TOP(sym);
    auto nameableSym = MIXIN_NameableSymbol::from(sym);
    PSY_ASSERT(nameableSym, return Action::Quit);

    nameableSym->setName(name);

    return Action::Skip;
}

SyntaxVisitor::Action Binder::typeSymAtTopAndPopIt()
{
    SYM_AT_TOP(sym);
    auto typeableSym = MIXIN_TypeableSymbol::from(sym);
    PSY_ASSERT(typeableSym, return Action::Quit);

    TY_AT_TOP(ty);

    if (!pendingFunTys_.empty()) {
        PSY_ASSERT(!pendingFunTys_.empty(), return Action::Quit);
        pendingFunTys_.top()->addParameterType(ty);
    }

    switch (ty->kind()) {
        case TypeKind::Array:
        case TypeKind::Function:
        case TypeKind::Pointer:
            popTy();
            break;

        case TypeKind::Typedef:
        case TypeKind::Tag:
        case TypeKind::Basic:
        case TypeKind::Void:
        case TypeKind::Qualified:
            break;
    }

    typeableSym->setType(ty);

    return Action::Skip;
}

template <class DeclT>
SyntaxVisitor::Action Binder::visitDeclaration_AtDeclarators_COMMON(
        const DeclT* node,
        Action (Binder::*visit_AtEnd)(const DeclT*))
{
    for (auto decltorIt = node->declarators(); decltorIt; decltorIt = decltorIt->next) {
        visit(decltorIt->value);

        SYM_AT_TOP(sym);
        switch (sym->kind()) {
            case SymbolKind::Declaration: {
                auto decl = sym->asDeclarationSymbol();
                switch (decl->kind()) {
                    case DeclarationSymbolKind::Function:
                    case DeclarationSymbolKind::Object: {
                        typeSymAtTopAndPopIt();
                        popSym();
                        SCOPE_AT_TOP(scope);
                        scope->addDeclaration(decl);
                        break;
                    }

                    default:
                        break;
                }
                break;
            }

            default:
                break;
        }
    }

    return ((this)->*(visit_AtEnd))(node);
}

SyntaxVisitor::Action Binder::visitTypedefDeclaration_AtDeclarators(
        const TypedefDeclarationSyntax* node)
{
    decltorIsOfTydef_ = true;
    auto action = visitDeclaration_AtDeclarators_COMMON(
                node,
                &Binder::visitTypedefDeclaration_AtEnd);
    decltorIsOfTydef_ = false;
    return action;
}

SyntaxVisitor::Action Binder::visitVariableAndOrFunctionDeclaration_AtDeclarators(
        const VariableAndOrFunctionDeclarationSyntax* node)
{
    return visitDeclaration_AtDeclarators_COMMON(
                node,
                &Binder::visitVariableAndOrFunctionDeclaration_AtEnd);
}

SyntaxVisitor::Action Binder::visitFieldDeclaration_AtDeclarators(const FieldDeclarationSyntax* node)
{
    return visitDeclaration_AtDeclarators_COMMON(
                node,
                &Binder::visitFieldDeclaration_AtEnd);
}

SyntaxVisitor::Action Binder::visitEnumeratorDeclaration_AtDeclarator(const EnumeratorDeclarationSyntax* node)
{
    RETURN_IF_QUIT(visitSimpleDeclarator_COMMON(node));
    RETURN_IF_QUIT(nameSymAtTop(lexemeOrEmptyIdent(node->identifierToken())));
    RETURN_IF_QUIT(typeSymAtTopAndPopIt());
    return visitEnumeratorDeclaration_AtEnd(node);
}

SyntaxVisitor::Action Binder::visitParameterDeclaration_AtDeclarator(const ParameterDeclarationSyntax* node)
{
    visit(node->declarator());
    RETURN_IF_QUIT(typeSymAtTopAndPopIt());
    return visitParameterDeclaration_AtEnd(node);
}

SyntaxVisitor::Action Binder::visitFunctionDefinition_AtDeclarator(const FunctionDefinitionSyntax* node)
{
    visit(node->declarator());

    typeSymAtTopAndPopIt();
    auto decl = popSymAsDecl();
    PSY_ASSERT(decl, return Action::Quit);
    SCOPE_AT_TOP(scope);
    scope->addDeclaration(decl);

    popTy();

    nestStashedScope();
    scopes_.top()->morphFrom_FunctionPrototype_to_Block();
    auto body = node->body()->asCompoundStatement();
    for (auto stmtIt = body->statements(); stmtIt; stmtIt = stmtIt->next)
        visit(stmtIt->value);
    unnestScope();

    return Binder::visitFunctionDefinition_AtEnd(node);
}

SyntaxVisitor::Action Binder::visitArrayOrFunctionDeclarator(const ArrayOrFunctionDeclaratorSyntax* node)
{
    for (auto specIt = node->attributes(); specIt; specIt = specIt->next)
        visit(specIt->value);

    switch (node->suffix()->kind()) {
        case SyntaxKind::SubscriptSuffix: {
            TY_AT_TOP(ty);
            pushTy(makeTy<ArrayType>(ty));
            break;
        }

        case SyntaxKind::ParameterSuffix: {
            TY_AT_TOP(ty);
            switch (ty->kind()) {
                case TypeKind::Function:
                    diagReporter_.FunctionReturningFunction(
                                node->innerDeclarator()->firstToken());
                    break;

                case TypeKind::Array:
                    diagReporter_.FunctionReturningArray(
                                node->innerDeclarator()->firstToken());
                    break;

                case TypeKind::Pointer:
                case TypeKind::Basic:
                case TypeKind::Void:
                case TypeKind::Qualified:
                case TypeKind::Tag:
                case TypeKind::Typedef:
                    break;
            }
            auto funcTy = makeTy<FunctionType>(ty);
            pushTy(funcTy);
            pendingFunTys_.push(funcTy);
            break;
        }

        default:
            PSY_ASSERT(false, return Action::Quit);
    }

    visit(node->innerDeclarator());

    nestNewScope(ScopeKind::FunctionPrototype);
    visit(node->suffix());
    unnestAndStashScope();

    if (node->suffix()->kind() == SyntaxKind::ParameterSuffix)
        pendingFunTys_.pop();

    return Action::Skip;
}

SyntaxVisitor::Action Binder::visitSubscriptSuffix(const SubscriptSuffixSyntax* node)
{
    return Action::Skip;
}

SyntaxVisitor::Action Binder::visitParameterSuffix(const ParameterSuffixSyntax* node)
{
    for (auto declIt = node->parameters(); declIt; declIt = declIt->next) {
        TyContT tys;
        std::swap(tys_, tys);
        visit(declIt->value);
        std::swap(tys_, tys);
    }

    return Action::Skip;
}

SyntaxVisitor::Action Binder::visitPointerDeclarator(const PointerDeclaratorSyntax* node)
{
    TY_AT_TOP(ty);
    pushTy(makeTy<PointerType>(ty));

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

SyntaxVisitor::Action Binder::visitSimpleDeclarator_COMMON(const SyntaxNode* node)
{
    SCOPE_AT_TOP(scope);
    switch (scope->kind()) {
        case ScopeKind::File:
        case ScopeKind::Block: {
            TY_AT_TOP(ty);
            switch (ty->kind()) {
                case TypeKind::Function:
                    makeBindAndPushSym<Function>(node);
                    break;

                case TypeKind::Array:
                case TypeKind::Pointer:
                case TypeKind::Basic:
                case TypeKind::Void:
                case TypeKind::Qualified:
                case TypeKind::Typedef:
                case TypeKind::Tag: {
                    SYM_AT_TOP(sym);
                    switch (sym->kind()) {
                        case SymbolKind::Declaration: {
                            auto declSym = sym->asDeclarationSymbol();
                            switch (declSym->kind()) {
                                case DeclarationSymbolKind::Type:
                                    switch (declSym->asTypeDeclarationSymbol()->kind()) {
                                        case TypeDeclarationSymbolKind::Union:
                                        case TypeDeclarationSymbolKind::Struct:
                                            makeBindAndPushSym<Field>(node);
                                            break;

                                        case TypeDeclarationSymbolKind::Enum:
                                            makeBindAndPushSym<Enumerator>(node);
                                            break;

                                        case TypeDeclarationSymbolKind::Typedef:
                                            PSY_ASSERT(false, return Action::Quit);
                                    }
                                    break;

                                case DeclarationSymbolKind::Object:
                                case DeclarationSymbolKind::Function:
                                    makeBindAndPushSym<Variable>(node);
                                    break;
                            }
                            break;
                        }

                        case SymbolKind::TranslationUnit:
                            makeBindAndPushSym<Variable>(node);
                            break;

                        default:
                            PSY_ASSERT(false, return Action::Quit);
                    }
                    break;
                }
            }
            break;
        }

        case ScopeKind::Function:
            break;

        case ScopeKind::FunctionPrototype: {
            TY_AT_TOP(ty);
            switch (ty->kind()) {
                case TypeKind::Array: {
                    /*
                     * 6.7.6.3-7
                     * A declaration of a parameter as “array of type”
                     * shall be adjusted to “qualified pointer to type”...
                     */
                    popTy();
                    TY_AT_TOP(otherTy);
                    auto ptrTy = makeTy<PointerType>(otherTy);
                    pushTy(ptrTy);
                    ptrTy->markAsArisingFromArrayDecay();
                    break;
                }

                case TypeKind::Function: {
                    /*
                     * 6.7.6.3-8
                     * A declaration of a parameter as “function returning type”
                     * shall be adjusted to “pointer to function returning type”...
                     */
                    auto ptrTy = makeTy<PointerType>(ty);
                    pushTy(ptrTy);
                    ptrTy->markAsArisingFromFunctionDecay();
                    break;
                }

                case TypeKind::Pointer:
                case TypeKind::Basic:
                case TypeKind::Void:
                case TypeKind::Qualified:
                case TypeKind::Typedef:
                case TypeKind::Tag:
                    break;
            }
            makeBindAndPushSym<Parameter>(node);
            break;
        }

        default:
            PSY_ESCAPE_VIA_RETURN(Action::Quit);
    }

    return Action::Skip;
}

SyntaxVisitor::Action Binder::visitIdentifierDeclarator(const IdentifierDeclaratorSyntax* node)
{
    if (decltorIsOfTydef_) {
        TY_AT_TOP(ty);
        SCOPE_AT_TOP(scope);
        auto tydefTy = makeTy<TypedefType>(lexemeOrEmptyIdent(node->identifierToken()));
        auto tydef = makeAndBindSym<Typedef>(node, tydefTy, ty);
        scope->addDeclaration(tydef);
    }
    else {
        RETURN_IF_QUIT(visitSimpleDeclarator_COMMON(node));
        nameSymAtTop(lexemeOrEmptyIdent(node->identifierToken()));
    }

    return Action::Skip;
}

SyntaxVisitor::Action Binder::visitAbstractDeclarator(const AbstractDeclaratorSyntax* node)
{
    RETURN_IF_QUIT(visitSimpleDeclarator_COMMON(node));
    nameSymAtTop(tree_->findIdentifier("", 0));

    return Action::Skip;
}
