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

#include <iostream>

using namespace psy;
using namespace C;

void Binder::nameDeclarationAtTop(const Identifier* name)
{
    SYM_AT_TOP_V(sym);
    auto nameableDecl = MIXIN_NameableSymbol::from(sym);
    PSY_ASSERT_2(nameableDecl, return);
    nameableDecl->setName(name);
}

void Binder::typeDeclarationAtTopWithTypeAtTop()
{
    SYM_AT_TOP_V(sym);
    auto typeableDecl = MIXIN_TypeableSymbol::from(sym);
    PSY_ASSERT_2(typeableDecl, return);

    TY_AT_TOP_V(ty);
    typeableDecl->setType(ty);

    if (!pendingFunTys_.empty()) {
        PSY_ASSERT_2(!pendingFunTys_.empty(), return);
        pendingFunTys_.top()->addParameterType(ty);
    }
}

void Binder::popTypesUntilNonDerivedDeclaratorType()
{
    PSY_ASSERT_2(!tys_.empty(), return);
    auto ty = tys_.top();
    while (true) {
        switch (ty->kind()) {
            case TypeKind::Array:
            case TypeKind::Function:
            case TypeKind::Pointer:
                popType();
                PSY_ASSERT_2(!tys_.empty(), return);
                ty = tys_.top();
                continue;
            default:
                return;
        }
    }
}

template <class DeclT>
SyntaxVisitor::Action Binder::visitDeclaration_AtMultipleDeclarators_COMMON(
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
                    case DeclarationSymbolKind::Type:
                        PSY_ASSERT_1(
                            decl->asTypeDeclarationSymbol()->kind() == TypeDeclarationSymbolKind::Typedef);
                        [[fallthrough]];

                    case DeclarationSymbolKind::Function:
                    case DeclarationSymbolKind::Object: {
                        typeDeclarationAtTopWithTypeAtTop();
                        popTypesUntilNonDerivedDeclaratorType();
                        popSymbol();
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
    auto action = visitDeclaration_AtMultipleDeclarators_COMMON(
                node,
                &Binder::visitTypedefDeclaration_AtEnd);
    decltorIsOfTydef_ = false;
    return action;
}

SyntaxVisitor::Action Binder::visitVariableAndOrFunctionDeclaration_AtDeclarators(
        const VariableAndOrFunctionDeclarationSyntax* node)
{
    return visitDeclaration_AtMultipleDeclarators_COMMON(
                node,
                &Binder::visitVariableAndOrFunctionDeclaration_AtEnd);
}

SyntaxVisitor::Action Binder::visitFieldDeclaration_AtDeclarators(const FieldDeclarationSyntax* node)
{
    return visitDeclaration_AtMultipleDeclarators_COMMON(
                node,
                &Binder::visitFieldDeclaration_AtEnd);
}

SyntaxVisitor::Action Binder::visitEnumeratorDeclaration_AtDeclarator(const EnumeratorDeclarationSyntax* node)
{
    bindObjectOrFunctionAndPushSymbol(node);
    nameDeclarationAtTop(identifier(node->identifierToken()));
    typeDeclarationAtTopWithTypeAtTop();
    return visitEnumeratorDeclaration_AtEnd(node);
}

SyntaxVisitor::Action Binder::visitParameterDeclaration_AtDeclarator(const ParameterDeclarationSyntax* node)
{
    visit(node->declarator());
    typeDeclarationAtTopWithTypeAtTop();
    popTypesUntilNonDerivedDeclaratorType();
    return visitParameterDeclaration_AtEnd(node);
}

SyntaxVisitor::Action Binder::visitFunctionDefinition_AtDeclarator(const FunctionDefinitionSyntax* node)
{
    visit(node->declarator());
    typeDeclarationAtTopWithTypeAtTop();
    popTypesUntilNonDerivedDeclaratorType();

    auto decl = popSymbolAsDeclaration();
    PSY_ASSERT_2(decl, return Action::Quit);
    SCOPE_AT_TOP(scope);
    scope->addDeclaration(decl);

    popType();

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
            pushType(makeType<ArrayType>(ty));
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
            auto funcTy = makeType<FunctionType>(ty);
            pushType(funcTy);
            pendingFunTys_.push(funcTy);
            break;
        }

        default:
            PSY_ASSERT_2(false, return Action::Quit);
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
    pushType(makeType<PointerType>(ty));

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

void Binder::bindObjectOrFunctionAndPushSymbol(const SyntaxNode* node)
{
    SCOPE_AT_TOP_V(scope);
    switch (scope->kind()) {
        case ScopeKind::File:
        case ScopeKind::Block: {
            TY_AT_TOP_V(ty);
            switch (ty->kind()) {
                case TypeKind::Function:
                    bindAndPushSymbol<Function>(node);
                    break;

                case TypeKind::Array:
                case TypeKind::Pointer:
                case TypeKind::Basic:
                case TypeKind::Void:
                case TypeKind::Qualified:
                case TypeKind::Typedef:
                case TypeKind::Tag: {
                    SYM_AT_TOP_V(sym);
                    switch (sym->kind()) {
                        case SymbolKind::Declaration: {
                            auto decl = sym->asDeclarationSymbol();
                            switch (decl->kind()) {
                                case DeclarationSymbolKind::Type: {
                                    auto tyDecl = decl->asTypeDeclarationSymbol();
                                    switch (tyDecl->kind()) {
                                        case TypeDeclarationSymbolKind::Tag: {
                                            auto tagTyDecl = tyDecl->asTagTypeDeclaration();
                                            switch (tagTyDecl->kind()) {
                                                case TagTypeDeclarationSymbolKind::Union:
                                                case TagTypeDeclarationSymbolKind::Struct:
                                                    bindAndPushSymbol<Field>(node);
                                                    break;

                                                case TagTypeDeclarationSymbolKind::Enum:
                                                    bindAndPushSymbol<Enumerator>(node);
                                                    break;
                                            }
                                            break;
                                        }
                                        case TypeDeclarationSymbolKind::Typedef:
                                            PSY_ASSERT_2(false, return);
                                    }
                                    break;
                                }
                                case DeclarationSymbolKind::Object:
                                case DeclarationSymbolKind::Function:
                                    bindAndPushSymbol<Variable>(node);
                                    break;
                            }
                            break;
                        }
                        case SymbolKind::TranslationUnit:
                            bindAndPushSymbol<Variable>(node);
                            break;

                        default:
                            PSY_ASSERT_2(false, return);
                    }
                    break;
                }
            }
            break;
        }

        case ScopeKind::Function:
            break;

        case ScopeKind::FunctionPrototype: {
            TY_AT_TOP_V(ty);
            switch (ty->kind()) {
                case TypeKind::Array: {
                    /*
                     * 6.7.6.3-7
                     * A declaration of a parameter as “array of type”
                     * shall be adjusted to “qualified pointer to type”...
                     */
                    popType();
                    TY_AT_TOP_V(otherTy);
                    auto ptrTy = makeType<PointerType>(otherTy);
                    pushType(ptrTy);
                    ptrTy->markAsArisingFromArrayDecay();
                    break;
                }

                case TypeKind::Function: {
                    /*
                     * 6.7.6.3-8
                     * A declaration of a parameter as “function returning type”
                     * shall be adjusted to “pointer to function returning type”...
                     */
                    auto ptrTy = makeType<PointerType>(ty);
                    pushType(ptrTy);
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
            bindAndPushSymbol<Parameter>(node);
            break;
        }

        default:
            PSY_ASSERT_FAIL_1(return);
    }
}

SyntaxVisitor::Action Binder::visitIdentifierDeclarator(const IdentifierDeclaratorSyntax* node)
{
    if (decltorIsOfTydef_) {
        auto tydefTy = makeType<TypedefType>(identifier(node->identifierToken()));
        bindAndPushSymbol<Typedef>(node, tydefTy);
    }
    else {
        bindObjectOrFunctionAndPushSymbol(node);
        nameDeclarationAtTop(identifier(node->identifierToken()));
    }

    return Action::Skip;
}

SyntaxVisitor::Action Binder::visitAbstractDeclarator(const AbstractDeclaratorSyntax* node)
{
    bindObjectOrFunctionAndPushSymbol(node);
    nameDeclarationAtTop(tree_->findIdentifier("", 0));

    return Action::Skip;
}
