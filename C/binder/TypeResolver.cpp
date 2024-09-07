// Copyright (c) 2024 Leandro T. C. Melo <ltcmelo@gmail.com>
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

#include "TypeResolver.h"

#include "binder/Scope.h"
#include "compilation/Compilation.h"
#include "compilation/SemanticModel.h"
#include "symbols/Symbol_ALL.h"
#include "syntax/Lexeme_Identifier.h"
#include "syntax/SyntaxNodes.h"
#include "types/Type_ALL.h"

#include "../common/infra/Assertions.h"

#include <iostream>

using namespace psy;
using namespace C;

TypeResolver::TypeResolver(SemanticModel* semaModel, const SyntaxTree* tree)
    : SyntaxVisitor(tree)
    , semaModel_(semaModel)
{}

void TypeResolver::resolveTypes()
{
    visit(tree_->root());

    for (const auto& ty : discardedTys_)
        semaModel_->dropType(ty);
}

SyntaxVisitor::Action TypeResolver::visitDeclarator_COMMON(const DeclaratorSyntax* node)
{
    auto decl = semaModel_->declaredDeclaration(node);
    if (decl) {
        switch (decl->kind()) {
            case DeclarationKind::Type:
            case DeclarationKind::Function:
            case DeclarationKind::Object: {
                auto typeableDecl = MIXIN_TypeableSymbol::from(decl);
                auto ty = typeableDecl->retypeableType();
                auto resolvedTy = resolveType(ty, decl->enclosingScope());
                typeableDecl->setType(resolvedTy);
                break;
            }
        }
    }

    return Action::Skip;
}

SyntaxVisitor::Action TypeResolver::visitPointerDeclarator(
        const PointerDeclaratorSyntax* node)
{
    return visitDeclarator_COMMON(node);
}

SyntaxVisitor::Action TypeResolver::visitParenthesizedDeclarator(
        const ParenthesizedDeclaratorSyntax* node)
{
    return visitDeclarator_COMMON(node->innerDeclarator());
}

SyntaxVisitor::Action TypeResolver::visitIdentifierDeclarator(
        const IdentifierDeclaratorSyntax* node)
{
    return visitDeclarator_COMMON(node);
}

const Type* TypeResolver::resolveType(const Type* ty, const Scope* scope) const
{
    switch (ty->kind()) {
        case TypeKind::Array: {
            auto arrTy = ty->asArrayType();
            auto elemTy = arrTy->elementType();
            auto resolvedTy = resolveType(elemTy, scope);
            if (resolvedTy != elemTy) {
                arrTy->resetElementType(resolvedTy);
                discardedTys_.insert(elemTy);
            }
            break;
        }

        case TypeKind::Basic: {
            auto basicTy = ty->asBasicType();
            auto resolvedTy =
                semaModel_->compilation()->program()->canonicalBasicType(basicTy->kind());
            if (resolvedTy != basicTy) {
                discardedTys_.insert(ty);
                return resolvedTy;
            }
            break;
        }

        case TypeKind::Void: {
            auto voidTy = ty->asVoidType();
            auto resolvedTy = semaModel_->compilation()->program()->canonicalVoidType();
            if (resolvedTy != voidTy) {
                discardedTys_.insert(ty);
                return voidTy;
            }
            break;
        }

        case TypeKind::Function: {
            auto funcTy = ty->asFunctionType();
            auto retTy = funcTy->returnType();
            auto resolvedTy = resolveType(retTy, scope);
            if (resolvedTy != retTy)
                funcTy->setReturnType(resolvedTy);
            const auto parms = funcTy->parameterTypes();
            const auto parmsSize = parms.size();
            for (FunctionType::ParameterTypes::size_type idx = 0; idx < parmsSize; ++idx) {
                const Type* parmTy = parms[idx];
                resolvedTy = resolveType(parmTy, scope);
                if (resolvedTy != parmTy) {
                    funcTy->setParameterType(idx, resolvedTy);
                    discardedTys_.insert(parmTy);
                }
            }
            break;
        }

        case TypeKind::Pointer: {
            auto ptrTy = ty->asPointerType();
            auto refedTy = ptrTy->referencedType();
            auto resolvedTy = resolveType(refedTy, scope);
            if (resolvedTy != refedTy) {
                ptrTy->resetReferencedType(resolvedTy);
                discardedTys_.insert(refedTy);
            }
            break;
        }

        case TypeKind::Typedef: {
            auto tydefName = ty->asTypedefType()->typedefName();
            auto decl = scope->searchForDeclaration(
                        tydefName,
                        NameSpace::OrdinaryIdentifiers);
            if (decl) {
                if (decl->kind() == DeclarationKind::Type) {
                    auto tyDecl = decl->asTypeDeclaration();
                    PSY_ASSERT_2(tyDecl->kind() == TypeDeclarationKind::Typedef,
                                 return nullptr);
                    auto tydef = tyDecl->asTypedef();
                    return tydef->synonymizedType();
                } else {
                    // report: not a type declaration
                }
            } else {
                // report: declaration not found
            }
            //return nullptr;
            return ty;
        }

        case TypeKind::Tag:
            break;

        case TypeKind::Qualified: {
            auto qualTy = ty->asQualifiedType();
            auto unqualTy = qualTy->unqualifiedType();
            auto resolvedTy = resolveType(unqualTy, scope);
            if (resolvedTy != unqualTy) {
                qualTy->resetUnqualifiedType(
                            resolvedTy->kind() == TypeKind::Qualified
                            ? resolvedTy->asQualifiedType()->unqualifiedType()
                            : resolvedTy);
                discardedTys_.insert(unqualTy);
            }
            break;
        }
    }
    return ty;
}
