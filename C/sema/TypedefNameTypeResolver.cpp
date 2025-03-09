// Copyright (c) 2025 Leandro T. C. Melo <ltcmelo@gmail.com>
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

#include "TypedefNameTypeResolver.h"

#include "sema/Compilation.h"
#include "sema/SemanticModel.h"
#include "symbols/Symbol_ALL.h"
#include "syntax/SyntaxNodes.h"
#include "syntax/SyntaxTree.h"
#include "syntax/SyntaxVisitor__MACROS__.inc"
#include "types/Type_ALL.h"

using namespace psy;
using namespace C;

TypedefNameTypeResolver::TypedefNameTypeResolver(
        SemanticModel* semaModel,
        const SyntaxTree* tree)
    : SyntaxVisitor(tree)
    , semaModel_(semaModel)
    , inTydefDecltor_(false)
{}

void TypedefNameTypeResolver::resolveTypedefNameTypes()
{
    visit(tree_->root());
}

SyntaxVisitor::Action TypedefNameTypeResolver::visitTypedefDeclaration(
        const TypedefDeclarationSyntax* node)
{
    inTydefDecltor_ = true;
    VISIT(node->declarators());
    inTydefDecltor_ = false;

    return Action::Skip;
}

SyntaxVisitor::Action TypedefNameTypeResolver::visitIdentifierDeclarator(
        const IdentifierDeclaratorSyntax* node)
{
    if (!inTydefDecltor_)
        return Action::Skip;

    auto decl = semaModel_->declarationBy(node);
    PSY_ASSERT_2(decl, return Action::Quit);
    PSY_ASSERT_2(decl->kind() == SymbolKind::TypedefDeclaration, return Action::Quit);
    auto tydefDecl = decl->asTypedefDeclaration();
    auto resolvedTy = resolve(tydefDecl->synonymizedType());
    auto tydefNameTy = tydefDecl->introducedSynonymType();
    const_cast<TypedefNameType*>(tydefNameTy)->setResolvedSynonymizedType(resolvedTy);

    return Action::Skip;
}

const Type* TypedefNameTypeResolver::resolve(const Type* ty)
{
    switch (ty->kind()) {
        case TypeKind::Basic:
        case TypeKind::Void:
        case TypeKind::Tag:
        case TypeKind::Error:
            break;

        case TypeKind::Array: {
            auto arrTy = ty->asArrayType();
            auto elemTy = arrTy->elementType();
            auto resolvedTy = resolve(elemTy);
            if (resolvedTy != elemTy)
                arrTy->resetElementType(resolvedTy);
            break;
        }

        case TypeKind::Function: {
            auto funcTy = ty->asFunctionType();
            auto retTy = funcTy->returnType();
            auto resolvedTy = resolve(retTy);
            if (resolvedTy != retTy)
                funcTy->setReturnType(resolvedTy);
            const auto parms = funcTy->parameterTypes();
            const auto parmsSize = parms.size();
            for (FunctionType::ParameterTypes::size_type idx = 0; idx < parmsSize; ++idx) {
                const Type* parmTy = parms[idx];
                resolvedTy = resolve(parmTy);
                if (resolvedTy != parmTy)
                    funcTy->setParameterType(idx, resolvedTy);
            }
            break;
        }

        case TypeKind::Pointer: {
            auto ptrTy = ty->asPointerType();
            auto refedTy = ptrTy->referencedType();
            auto resolvedTy = resolve(refedTy);
            if (resolvedTy != refedTy)
                ptrTy->resetReferencedType(resolvedTy);
            break;
        }

        case TypeKind::TypedefName: {
            auto tydefNameTy = ty->asTypedefNameType();
            auto tydefDecl = tydefNameTy->declaration();
            if (!tydefDecl)
                return semaModel_->compilation()->canonicalErrorType();
            return resolve(tydefDecl->synonymizedType());
        }

        case TypeKind::Qualified: {
            auto qualTy = ty->asQualifiedType();
            auto unqualTy = qualTy->unqualifiedType();
            auto resolvedTy = resolve(unqualTy);
            if (resolvedTy != unqualTy) {
                qualTy->resetUnqualifiedType(
                        resolvedTy->kind() == TypeKind::Qualified
                            ? resolvedTy->asQualifiedType()->unqualifiedType()
                            : resolvedTy);
            }
            break;
        }
    }

    return ty;
}
