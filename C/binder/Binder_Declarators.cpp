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

#include "parser/Unparser.h"
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
#include <sstream>

using namespace psy;
using namespace C;

void Binder::nameDeclarationAtTop(const Identifier* name)
{
    DECL_TOP_SYM_ret(sym);
    auto nameableDecl = MIXIN_NameableSymbol::from(sym);
    PSY_ASSERT_2(nameableDecl, return);
    nameableDecl->setName(name);
}

void Binder::typeDeclarationAtTopWithTypeAtTop()
{
    DECL_TOP_SYM_ret(sym);
    auto typeableDecl = MIXIN_TypeableSymbol::from(sym);
    PSY_ASSERT_2(typeableDecl, return);

    DECL_TOP_TY_ret(ty);
    typeableDecl->setType(ty);

    if (!pendingFunTys_.empty()) {
        PSY_ASSERT_2(!pendingFunTys_.empty(), return);
        pendingFunTys_.top()->addParameterType(ty);
    }
}

void Binder::popTypesUntilNonDerivedDeclaratorType()
{
    DECL_TOP_TY_ret(ty);
    while (true) {
        switch (ty->kind()) {
            case TypeKind::Qualified: {
                auto unqualTyK = ty->asQualifiedType()->unqualifiedType()->kind();
                if (!(unqualTyK == TypeKind::Array
                        || unqualTyK == TypeKind::Function
                        || unqualTyK == TypeKind::Pointer)) {
                    return;
                }
                [[fallthrough]];
            }
            case TypeKind::Array:
            case TypeKind::Function:
            case TypeKind::Pointer:
                popType();
                TOP_TY_ret(ty);
                continue;
            default:
                return;
        }
    }
}

template <class NodeT>
SyntaxVisitor::Action Binder::visit_AtMultipleDeclarators_COMMON(
        const NodeT* node,
        Action (Binder::*visit_AtEnd)(const NodeT*))
{
    for (auto decltorIt = node->declarators(); decltorIt; decltorIt = decltorIt->next) {
        visit(decltorIt->value);
        DECL_TOP_SYM_retQ(sym);
        switch (sym->kind()) {
            case SymbolKind::Declaration: {
                auto decl = sym->asDeclaration();
                switch (decl->kind()) {
                    case DeclarationKind::Type:
                        PSY_ASSERT_1(
                            decl->asTypeDeclaration()->kind() == TypeDeclarationKind::Typedef);
                        [[fallthrough]];

                    case DeclarationKind::Function:
                    case DeclarationKind::Object: {
                        typeDeclarationAtTopWithTypeAtTop();
                        popTypesUntilNonDerivedDeclaratorType();
                        popSymbol();
                        DECL_TOP_SCOPE_retQ(scope);
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
    F_.inTydefDecltor_ = true;
    auto action = visit_AtMultipleDeclarators_COMMON(
                node,
                &Binder::visitTypedefDeclaration_AtEnd);
    F_.inTydefDecltor_ = false;
    return action;
}

SyntaxVisitor::Action Binder::visitVariableAndOrFunctionDeclaration_AtDeclarators(
        const VariableAndOrFunctionDeclarationSyntax* node)
{
    return visit_AtMultipleDeclarators_COMMON(
                node,
                &Binder::visitVariableAndOrFunctionDeclaration_AtEnd);
}

SyntaxVisitor::Action Binder::visitFieldDeclaration_AtDeclarators(const FieldDeclarationSyntax* node)
{
    return visit_AtMultipleDeclarators_COMMON(
                node,
                &Binder::visitFieldDeclaration_AtEnd);
}

SyntaxVisitor::Action Binder::visitEnumeratorDeclaration_AtDeclarator(
        const EnumeratorDeclarationSyntax* node)
{
    bindObjectOrFunctionAndPushSymbol(node);
    nameDeclarationAtTop(identifier(node->identifierToken()));
    typeDeclarationAtTopWithTypeAtTop();
    return visitEnumeratorDeclaration_AtEnd(node);
}

template <class NodeT>
SyntaxVisitor::Action Binder::visit_AtSingleDeclarator_COMMON(
        const NodeT* node,
        Action (Binder::*visit_AtEnd)(const NodeT*))
{
    visit(node->declarator());
    typeDeclarationAtTopWithTypeAtTop();
    popTypesUntilNonDerivedDeclaratorType();
    return ((this)->*visit_AtEnd)(node);
}

SyntaxVisitor::Action Binder::visitParameterDeclaration_AtDeclarator(
        const ParameterDeclarationSyntax* node)
{
    return visit_AtSingleDeclarator_COMMON(
                node,
                &Binder::visitParameterDeclaration_AtEnd);
}

SyntaxVisitor::Action Binder::visitFunctionDefinition_AtDeclarator(
        const FunctionDefinitionSyntax* node)
{
    return visit_AtSingleDeclarator_COMMON(
                node,
                &Binder::visitFunctionDefinition_AtEnd);
}

SyntaxVisitor::Action Binder::visitTypeName_AtDeclarator(const TypeNameSyntax* node)
{
    return visit_AtSingleDeclarator_COMMON(
                node,
                &Binder::visitTypeName_AtEnd);
}

SyntaxVisitor::Action Binder::visitArrayOrFunctionDeclarator(const ArrayOrFunctionDeclaratorSyntax* node)
{
    for (auto attrIt = node->attributes(); attrIt; attrIt = attrIt->next)
        visit(attrIt->value);

    switch (node->suffix()->kind()) {
        case SyntaxKind::SubscriptSuffix: {
            DECL_TOP_TY_retQ(ty);
            pushType(makeType<ArrayType>(ty));
            visit(node->innerDeclarator());
            for (auto attrIt = node->attributes_PostDeclarator(); attrIt; attrIt = attrIt->next)
                visit(attrIt->value);
            visit(node->initializer());
            return Action::Skip;
        }

        case SyntaxKind::ParameterSuffix: {
            DECL_TOP_TY_retQ(ty);
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

            visit(node->innerDeclarator());
            for (auto attrIt = node->attributes_PostDeclarator(); attrIt; attrIt = attrIt->next)
                visit(attrIt->value);
            visit(node->initializer());

            pushNewScope(node, ScopeKind::FunctionPrototype, true);
            visit(node->suffix());
            popAndStashScope();

            pendingFunTys_.pop();

            return Action::Skip;
        }

        default:
            PSY_ASSERT_2(false, return Action::Quit);
    }
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
    DECL_TOP_TY_retQ(ty);
    pushType(makeType<PointerType>(ty));

    for (auto specIt = node->qualifiersAndAttributes(); specIt; specIt = specIt->next)
        visit(specIt->value);
    visit(node->innerDeclarator());
    visit(node->initializer());

    return Action::Skip;
}

SyntaxVisitor::Action Binder::visitParenthesizedDeclarator(const ParenthesizedDeclaratorSyntax* node)
{
    visit(node->innerDeclarator());

    return Action::Skip;
}

SyntaxVisitor::Action Binder::visitBitfieldDeclarator(const BitfieldDeclaratorSyntax* node)
{
    if (node->innerDeclarator()) {
        visit(node->innerDeclarator());
    } else {
        bindObjectOrFunctionAndPushSymbol(node);
        nameDeclarationAtTop(tree_->findIdentifier("", 0));
    }

    return Action::Skip;
}

void Binder::bindObjectOrFunctionAndPushSymbol(const SyntaxNode* node)
{
    DECL_TOP_SCOPE_ret(scope);
    switch (scope->kind()) {
        case ScopeKind::File:
        case ScopeKind::Block: {
            DECL_TOP_TY_ret(ty);
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
                    DECL_TOP_SYM_ret(sym);
                    switch (sym->kind()) {
                        case SymbolKind::Declaration: {
                            auto decl = sym->asDeclaration();
                            switch (decl->kind()) {
                                case DeclarationKind::Type: {
                                    auto tyDecl = decl->asTypeDeclaration();
                                    switch (tyDecl->kind()) {
                                        case TypeDeclarationKind::Tag: {
                                            auto tagTyDecl = tyDecl->asTagTypeDeclaration();
                                            switch (tagTyDecl->kind()) {
                                                case TagTypeDeclarationKind::Union:
                                                case TagTypeDeclarationKind::Struct:
                                                    bindAndPushSymbol<Field>(node);
                                                    break;

                                                case TagTypeDeclarationKind::Enum:
                                                    bindAndPushSymbol<Enumerator>(node);
                                                    break;
                                            }
                                            break;
                                        }
                                        case TypeDeclarationKind::Typedef:
                                            PSY_ASSERT_2(false, return);
                                    }
                                    break;
                                }
                                case DeclarationKind::Object:
                                case DeclarationKind::Function:
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

        case ScopeKind::FunctionPrototype: {
            DECL_TOP_TY_ret(ty);
            switch (ty->kind()) {
                case TypeKind::Array: {
                    /*
                     * 6.7.6.3-7
                     * A declaration of a parameter as “array of type”
                     * shall be adjusted to “qualified pointer to type”...
                     */
                    popType();
                    DECL_TOP_TY_ret(otherTy);
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
    for (auto attrIt = node->attributes(); attrIt; attrIt = attrIt->next)
        visit(attrIt->value);

    if (F_.inTydefDecltor_) {
        auto ident = identifier(node->identifierToken());
        auto tydefTy = makeType<TypedefType>(ident);
        auto decl = bindAndPushSymbol<Typedef>(node, tydefTy);

        if (ident == ptrdiff_t_)
            semaModel_->set_ptrdiff_t_Typedef(decl);
        else if (ident == size_t_)
            semaModel_->set_size_t_Typedef(decl);
        else if (ident == max_align_t_)
            semaModel_->set_max_align_t_Typedef(decl);
        else if (ident == wchar_t_)
            semaModel_->set_wchar_t_Typedef(decl);
        else if (ident == char16_t_)
            semaModel_->set_char16_t_Typedef(decl);
        else if (ident == char32_t_)
            semaModel_->set_char32_t_Typedef(decl);
    }
    else {
        bindObjectOrFunctionAndPushSymbol(node);
        nameDeclarationAtTop(identifier(node->identifierToken()));
    }

    for (auto attrIt = node->attributes_PostIdentifier(); attrIt; attrIt = attrIt->next)
        visit(attrIt->value);

    visit(node->initializer());

    return Action::Skip;
}

SyntaxVisitor::Action Binder::visitAbstractDeclarator(const AbstractDeclaratorSyntax* node)
{
    bindObjectOrFunctionAndPushSymbol(node);
    nameDeclarationAtTop(tree_->findIdentifier("", 0));

    return Action::Skip;
}
