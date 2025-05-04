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

#include "DeclarationBinder.h"
#include "DeclarationBinder__MACROS__.inc"

#include "parser/Unparser.h"
#include "sema/Scope.h"
#include "sema/SemanticModel.h"
#include "symbols/Symbol_ALL.h"
#include "syntax/SyntaxFacts.h"
#include "syntax/Lexeme_ALL.h"
#include "syntax/SyntaxNodes.h"
#include "syntax/SyntaxTree.h"
#include "syntax/SyntaxNodes_Declarations.h"
#include "syntax/SyntaxUtilities.h"
#include "syntax/SyntaxVisitor__MACROS__.inc"
#include "types/Type_ALL.h"

#include "../common/infra/Assertions.h"

#include <iostream>
#include <sstream>

using namespace psy;
using namespace C;

void DeclarationBinder::nameDeclarationAtTop(const Identifier* name)
{
    DECL_AT_TOP(auto decl, );
    auto nameableDecl = MIXIN_NameableDeclarationSymbol::from(decl);
    PSY_ASSERT_2(nameableDecl, return);
    nameableDecl->setName(name);
}

void DeclarationBinder::typeDeclarationAtTopWithTypeAtTop()
{
    DECL_AT_TOP(auto decl, );
    auto typeableDecl = MIXIN_TypeableDeclarationSymbol::from(decl);
    PSY_ASSERT_2(typeableDecl, return);

    TY_AT_TOP(auto ty, );
    typeableDecl->setType(ty);

    if (!openFuncTys_.empty()) {
        PSY_ASSERT_2(openFuncTys_.top(), return);
        openFuncTys_.top()->addParameterType(ty);
    }
}

void DeclarationBinder::popTypesUntilNonDerivedDeclaratorType()
{
    TY_AT_TOP(auto ty, );
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
                TY_AT_TOP(ty, );
                continue;
            case TypeKind::Error:
                PSY_ASSERT_1(false);
                return;
            default:
                return;
        }
    }
}

template <class NodeT>
SyntaxVisitor::Action DeclarationBinder::visit_AtMultipleDeclarators_COMMON(
        const NodeT* node,
        Action (DeclarationBinder::*visit_AtEnd)(const NodeT*))
{
    for (auto decltorIt = node->declarators(); decltorIt; decltorIt = decltorIt->next) {
        VISIT(decltorIt->value);
        DECL_AT_TOP(auto decl, Action::Quit);
        switch (decl->category()) {
            case DeclarationCategory::Type: {
                PSY_ASSERT_2(decl->kind() == SymbolKind::TypedefDeclaration, return Action::Quit);
                TY_AT_TOP(auto ty, Action::Quit);
                decl->asTypedefDeclaration()->setSynonymizedType(ty);
                break;
            }
            case DeclarationCategory::Function:
            case DeclarationCategory::Member:
            case DeclarationCategory::Object:
                typeDeclarationAtTopWithTypeAtTop();
                break;

            default:
                PSY_ASSERT_1(false);
                return Action::Quit;
        }
        popTypesUntilNonDerivedDeclaratorType();
        finishDeclaration();
    }
    return ((this)->*(visit_AtEnd))(node);
}

SyntaxVisitor::Action DeclarationBinder::visitTypedefDeclaration_AtDeclarators(
        const TypedefDeclarationSyntax* node)
{
    F_.inTydefDecltor_ = true;
    auto action = visit_AtMultipleDeclarators_COMMON(
                node,
                &DeclarationBinder::visitTypedefDeclaration_AtEnd);
    F_.inTydefDecltor_ = false;
    return action;
}

SyntaxVisitor::Action DeclarationBinder::visitVariableAndOrFunctionDeclaration_AtDeclarators(
        const VariableAndOrFunctionDeclarationSyntax* node)
{
    return visit_AtMultipleDeclarators_COMMON(
                node,
                &DeclarationBinder::visitVariableAndOrFunctionDeclaration_AtEnd);
}

void DeclarationBinder::bindAnonymousFieldDeclaration(const SyntaxNode* node)
{
    DECL_AT_TOP(auto decl, );
    PSY_ASSERT_2(decl->kind() == SymbolKind::UnionDeclaration
                     || decl->kind() == SymbolKind::StructDeclaration,
                 return);
    auto tagTyDecl = decl->asTagTypeDeclaration();
    auto fldDecl = bindDeclaration<FieldDeclarationSymbol>(node);
    tagTyDecl->addMember(fldDecl);
    nameDeclarationAtTop(tree_->findIdentifier("", 0));
}

SyntaxVisitor::Action DeclarationBinder::visitFieldDeclaration_AtDeclarators(
        const FieldDeclarationSyntax* node)
{
    if (!node->declarators()) {
        TY_AT_TOP(auto ty, Action::Quit);
        PSY_ASSERT_2(ty->kind() == TypeKind::Tag, return Action::Quit);
        auto tagTy = ty->asTagType();
        if (tagTy->isUntagged()) {
            bindAnonymousFieldDeclaration(node);
            typeDeclarationAtTopWithTypeAtTop();
            popTypesUntilNonDerivedDeclaratorType();
            finishDeclaration();
            return visitFieldDeclaration_AtEnd(node);
        }
    }

    return visit_AtMultipleDeclarators_COMMON(
                node,
                &DeclarationBinder::visitFieldDeclaration_AtEnd);
}

SyntaxVisitor::Action DeclarationBinder::visitBitfieldDeclarator(
        const BitfieldDeclaratorSyntax* node)
{
    if (node->innerDeclarator())
        VISIT(node->innerDeclarator());
    else
        bindAnonymousFieldDeclaration(node);

    return Action::Skip;
}


SyntaxVisitor::Action DeclarationBinder::visitEnumeratorDeclaration_AtDeclaratorLike(
        const EnumeratorDeclarationSyntax* node)
{
    DECL_AT_TOP(auto decl, Action::Quit);
    PSY_ASSERT_2(decl->kind() == SymbolKind::EnumDeclaration,
                 return Action::Quit);
    auto enumeratorDecl = bindDeclaration<EnumeratorDeclarationSymbol>(node);
    auto tagTyDecl = decl->asTagTypeDeclaration();
    tagTyDecl->addMember(enumeratorDecl);
    nameDeclarationAtTop(obtainName(node->identifierToken()));
    typeDeclarationAtTopWithTypeAtTop();
    return visitEnumeratorDeclaration_AtEnd(node);
}

template <class NodeT>
SyntaxVisitor::Action DeclarationBinder::visit_AtSingleDeclarator_COMMON(
        const NodeT* node,
        Action (DeclarationBinder::*visit_AtEnd)(const NodeT*))
{
    VISIT(node->declarator());
    typeDeclarationAtTopWithTypeAtTop();
    popTypesUntilNonDerivedDeclaratorType();
    return ((this)->*visit_AtEnd)(node);
}

SyntaxVisitor::Action DeclarationBinder::visitParameterDeclaration_AtDeclarator(
        const ParameterDeclarationSyntax* node)
{
    return visit_AtSingleDeclarator_COMMON(
                node,
                &DeclarationBinder::visitParameterDeclaration_AtEnd);
}

SyntaxVisitor::Action DeclarationBinder::visitFunctionDefinition_AtDeclarator(
        const FunctionDefinitionSyntax* node)
{
    return visit_AtSingleDeclarator_COMMON(
                node,
                &DeclarationBinder::visitFunctionDefinition_AtEnd);
}

SyntaxVisitor::Action DeclarationBinder::visitTypeName_AtDeclarator(const TypeNameSyntax* node)
{
    return visit_AtSingleDeclarator_COMMON(
                node,
                &DeclarationBinder::visitTypeName_AtEnd);
}

SyntaxVisitor::Action DeclarationBinder::visitArrayOrFunctionDeclarator(const ArrayOrFunctionDeclaratorSyntax* node)
{
    VISIT(node->attributes());

    switch (node->suffix()->kind()) {
        case SyntaxKind::SubscriptSuffix: {
            TY_AT_TOP(auto ty, Action::Quit);
            pushType(makeType<ArrayType>(ty));
            VISIT(node->innerDeclarator());
            VISIT(node->attributes_PostDeclarator());
            VISIT(node->initializer());
            return Action::Skip;
        }

        case SyntaxKind::ParameterSuffix: {
            TY_AT_TOP(auto ty, Action::Quit);
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
                case TypeKind::TypedefName:
                    break;

                case TypeKind::Error:
                    PSY_ASSERT_1(false);
                    return Action::Quit;
            }
            auto funcTy = makeType<FunctionType>(ty);
            pushType(funcTy);
            openFuncTys_.push(funcTy);

            VISIT(node->innerDeclarator());
            VISIT(node->attributes_PostDeclarator());
            VISIT(node->initializer());
            pushNewScope(node, ScopeKind::FunctionPrototype, true);
            VISIT(node->suffix());
            popAndStashScope();

            openFuncTys_.pop();
            return Action::Skip;
        }

        default:
            PSY_ASSERT_1(false);
            return Action::Quit;
    }
}

SyntaxVisitor::Action DeclarationBinder::visitSubscriptSuffix(const SubscriptSuffixSyntax* node)
{
    return Action::Skip;
}

SyntaxVisitor::Action DeclarationBinder::visitParameterSuffix(const ParameterSuffixSyntax* node)
{
    for (auto declIt = node->parameters(); declIt; declIt = declIt->next) {
        TypeStack tys;
        std::swap(tys_, tys);
        VISIT(declIt->value);
        std::swap(tys_, tys);
    }

    if (node->ellipsisTkIdx_ != LexedTokens::invalidIndex()) {
        PSY_ASSERT_2(!openFuncTys_.empty(), return Action::Quit);
        PSY_ASSERT_2(openFuncTys_.top(), return Action::Quit);
        auto funcTy = openFuncTys_.top();
        funcTy->markAsVariadic();
    }

    return Action::Skip;
}

SyntaxVisitor::Action DeclarationBinder::visitPointerDeclarator(const PointerDeclaratorSyntax* node)
{
    TY_AT_TOP(auto ty, Action::Quit);
    pushType(makeType<PointerType>(ty));

    VISIT(node->qualifiersAndAttributes());
    VISIT(node->innerDeclarator());
    VISIT(node->initializer());

    return Action::Skip;
}

SyntaxVisitor::Action DeclarationBinder::visitParenthesizedDeclarator(
        const ParenthesizedDeclaratorSyntax* node)
{
    VISIT(node->innerDeclarator());

    return Action::Skip;
}

void DeclarationBinder::handleNonTypedefDeclarator(const DeclaratorSyntax* node)
{
    SCOPE_AT_TOP(auto scope, );
    switch (scope->kind()) {
        case ScopeKind::File:
        case ScopeKind::Block: {
            TY_AT_TOP(auto ty, );
            switch (ty->kind()) {
                case TypeKind::Function:
                    bindDeclaration<FunctionDeclarationSymbol>(node);
                    break;

                case TypeKind::Array:
                case TypeKind::Pointer:
                case TypeKind::Basic:
                case TypeKind::Void:
                case TypeKind::Qualified:
                case TypeKind::TypedefName:
                case TypeKind::Tag: {
                    auto sym = containingUnitOrDeclaration();
                    switch (sym->category()) {
                        case SymbolCategory::Declaration: {
                            auto decl = sym->asDeclaration();
                            switch (decl->category()) {
                                case DeclarationCategory::Type: {
                                    auto tyDecl = decl->asTypeDeclaration();
                                    switch (tyDecl->category()) {
                                        case TypeDeclarationCategory::Tag: {
                                            auto tagTyDecl = tyDecl->asTagTypeDeclaration();
                                            switch (tagTyDecl->category()) {
                                                case TagDeclarationCategory::StructOrUnion: {
                                                    auto fldDecl = bindDeclaration<FieldDeclarationSymbol>(node);
                                                    tagTyDecl->addMember(fldDecl);
                                                    break;
                                                }
                                                case TagDeclarationCategory::Enum:
                                                    PSY_ASSERT_1(false);
                                                    return;
                                            }
                                            break;
                                        }
                                        case TypeDeclarationCategory::Typedef:
                                            PSY_ASSERT_1(false);
                                            return;
                                    }
                                    break;
                                }
                                case DeclarationCategory::Function:
                                case DeclarationCategory::Object:
                                    bindDeclaration<VariableDeclarationSymbol>(node);
                                    break;

                                case DeclarationCategory::Member:
                                    PSY_ASSERT_1(false);
                                    return;
                            }
                            break;
                        }
                        case SymbolCategory::TranslationUnit:
                            bindDeclaration<VariableDeclarationSymbol>(node);
                            break;

                        default:
                            PSY_ASSERT_1(false);
                            return;
                    }
                    break;
                }

                case TypeKind::Error:
                    PSY_ASSERT_1(false);
                    return;
            }
            break;
        }

        case ScopeKind::FunctionPrototype: {
            TY_AT_TOP(auto ty, );
            switch (ty->kind()) {
                case TypeKind::Array: {
                    /*
                     * 6.7.6.3-7
                     * A declaration of a parameter as “array of type”
                     * shall be adjusted to “qualified pointer to type”...
                     */
                    popType();
                    TY_AT_TOP(auto otherTy, );
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
                case TypeKind::TypedefName:
                case TypeKind::Tag:
                    break;

                case TypeKind::Error:
                    PSY_ASSERT_1(false);
                    return;
            }
            bindDeclaration<ParameterDeclarationSymbol>(node);
            break;
        }

        default:
            PSY_ASSERT_1(false);
            return;
    }
}

SyntaxVisitor::Action DeclarationBinder::visitIdentifierDeclarator(const IdentifierDeclaratorSyntax* node)
{
    VISIT(node->attributes());

    if (F_.inTydefDecltor_) {
        auto name = obtainName(node->identifierToken());
        auto tydefNameTy = makeType<TypedefNameType>(name);
        auto decl = bindDeclaration<TypedefDeclarationSymbol>(node, tydefNameTy);

        if (name == ptrdiff_t_)
            semaModel_->set_ptrdiff_t_typedef(decl);
        else if (name == size_t_)
            semaModel_->set_size_t_typedef(decl);
        else if (name == max_align_t_)
            semaModel_->set_max_align_t_typedef(decl);
        else if (name == wchar_t_)
            semaModel_->set_wchar_t_typedef(decl);
        else if (name == char16_t_)
            semaModel_->set_char16_t_typedef(decl);
        else if (name == char32_t_)
            semaModel_->set_char32_t_typedef(decl);
    }
    else {
        handleNonTypedefDeclarator(node);
        nameDeclarationAtTop(obtainName(node->identifierToken()));
    }

    VISIT(node->attributes_PostIdentifier());
    VISIT(node->initializer());

    return Action::Skip;
}

SyntaxVisitor::Action DeclarationBinder::visitAbstractDeclarator(const AbstractDeclaratorSyntax* node)
{
    handleNonTypedefDeclarator(node);
    nameDeclarationAtTop(tree_->findIdentifier("", 0));

    return Action::Skip;
}
