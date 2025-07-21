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

#include "TypeCanonicalizer.h"

#include "sema/Scope.h"
#include "sema/Compilation.h"
#include "sema/SemanticModel.h"
#include "symbols/Symbol_ALL.h"
#include "syntax/Lexeme_Identifier.h"
#include "syntax/SyntaxNodes.h"
#include "syntax/SyntaxVisitor__MACROS__.inc"
#include "types/Type_ALL.h"
#include "../common/infra/Assertions.h"

#include <cstring>
#include <iostream>

using namespace psy;
using namespace C;

TypeCanonicalizer::TypeCanonicalizer(SemanticModel* semaModel, const SyntaxTree* tree)
    : SyntaxVisitor(tree)
    , semaModel_(semaModel)
    , tySpecNode_(nullptr)
    , diagReporter_(this)
{
    const char* internals[] = { "__builtin_va_list" };
    for (const auto s : internals) {
        auto ident = semaModel_->syntaxTree()->findIdentifier(s, strlen(s));
        if (ident)
            internalTydefNameIdents_.insert(ident);
    }
}

void TypeCanonicalizer::canonicalizeTypes()
{
    visit(tree_->rootNode());
    for (const auto& ty : discardedTys_)
        semaModel_->dropType(ty);
}

SyntaxVisitor::Action TypeCanonicalizer::visitTranslationUnit(const TranslationUnitSyntax* node)
{
    syms_.push(semaModel_->translationUnit());
    VISIT(node->declarations());
    syms_.pop();

    return Action::Skip;
}

SyntaxVisitor::Action TypeCanonicalizer::visitFunctionDefinition(const FunctionDefinitionSyntax* node)
{
    auto decl = semaModel_->functionFor(node);
    PSY_ASSERT_2(decl, return Action::Quit);
    syms_.push(decl);
    VISIT(node->body());
    syms_.pop();

    return Action::Skip;
}

void TypeCanonicalizer::canonicalizeAnonymousFields(FieldDeclarationSymbol* fldDecl)
{
    auto canonTy = canonicalize(fldDecl->type(), fldDecl->enclosingScope());
    fldDecl->setType(canonTy);

    if (isStructureOrUnionType(canonTy)
            && canonTy->asTagType()->isUntagged()) {
        auto tagTyDecl = canonTy->asTagType()->declaration();
        if (tagTyDecl) {
            auto struOrUnioDecl = tagTyDecl->asStructOrUnionDeclaration();
            for (auto outerFldDecl : struOrUnioDecl->fields())
                canonicalizeAnonymousFields(
                        const_cast<FieldDeclarationSymbol*>(outerFldDecl));
        }
    }
}

SyntaxVisitor::Action TypeCanonicalizer::visitFieldDeclaration(
        const FieldDeclarationSyntax* node)
{
    if (node->declarators())
        return Action::Visit;

    const auto& fldDecls = semaModel_->fieldsFor(node);
    PSY_ASSERT_2(fldDecls.size() == 1, return Action::Quit);
    auto fldDecl = fldDecls[0];
    canonicalizeAnonymousFields(fldDecl);

    return Action::Skip;
}

SyntaxVisitor::Action TypeCanonicalizer::visitTagTypeSpecifier(const TagTypeSpecifierSyntax* node)
{
    tySpecNode_ = node;

    return Action::Visit;
}

SyntaxVisitor::Action TypeCanonicalizer::visitTypedefName(const TypedefNameSyntax* node)
{
    tySpecNode_ = node;

    return Action::Skip;
}

SyntaxVisitor::Action TypeCanonicalizer::visitDeclarator_COMMON(const DeclaratorSyntax* node)
{
    auto decl = semaModel_->declarationBy(node);
    PSY_ASSERT_2(decl, return Action::Quit);
    switch (decl->category()) {
        case DeclarationCategory::Type: {
            PSY_ASSERT_2(decl->kind() == SymbolKind::TypedefDeclaration, return Action::Quit);
            auto tydefDecl = decl->asTypedefDeclaration();
            auto canonTy = canonicalize(tydefDecl->synonymizedType(), decl->enclosingScope());
            tydefDecl->setSynonymizedType(canonTy);
            break;
        }
        case DeclarationCategory::Member:
        case DeclarationCategory::Function:
        case DeclarationCategory::Object: {
            auto typeableDecl = MIXIN_TypeableDeclarationSymbol::from(decl);
            PSY_ASSERT_2(typeableDecl, return Action::Quit);
            auto ty = typeableDecl->type();
            auto canonTy = canonicalize(ty, decl->enclosingScope());
            typeableDecl->setType(canonTy);
            break;
        }
    }

    return Action::Skip;
}

SyntaxVisitor::Action TypeCanonicalizer::visitPointerDeclarator(
        const PointerDeclaratorSyntax* node)
{
    return visitDeclarator_COMMON(node);
}

SyntaxVisitor::Action TypeCanonicalizer::visitParenthesizedDeclarator(
        const ParenthesizedDeclaratorSyntax* node)
{
    return visitDeclarator_COMMON(node->innerDeclarator());
}

SyntaxVisitor::Action TypeCanonicalizer::visitIdentifierDeclarator(
        const IdentifierDeclaratorSyntax* node)
{
    return visitDeclarator_COMMON(node);
}

const Type* TypeCanonicalizer::canonicalize(const Type* ty, const Scope* scope)
{
    switch (ty->kind()) {
        case TypeKind::Array: {
            auto arrTy = ty->asArrayType();
            auto elemTy = arrTy->elementType();
            auto canonTy = canonicalize(elemTy, scope);
            if (canonTy != elemTy) {
                arrTy->resetElementType(canonTy);
                discardedTys_.insert(elemTy);
            }
            break;
        }

        case TypeKind::Basic: {
            auto basicTy = ty->asBasicType();
            auto canonTy =
                semaModel_->compilation()->canonicalBasicType(basicTy->kind());
            if (canonTy != basicTy) {
                discardedTys_.insert(ty);
                return canonTy;
            }
            break;
        }

        case TypeKind::Void: {
            auto voidTy = ty->asVoidType();
            auto canonTy = semaModel_->compilation()->canonicalVoidType();
            if (canonTy != voidTy) {
                discardedTys_.insert(ty);
                return canonTy;
            }
            break;
        }

        case TypeKind::Function: {
            auto funcTy = ty->asFunctionType();
            auto retTy = funcTy->returnType();
            auto canonTy = canonicalize(retTy, scope);
            if (canonTy != retTy) {
                funcTy->setReturnType(canonTy);
                discardedTys_.insert(retTy);
            }
            const auto parms = funcTy->parameterTypes();
            const auto parmsSize = parms.size();
            for (FunctionType::ParameterTypes::size_type idx = 0; idx < parmsSize; ++idx) {
                const Type* parmTy = parms[idx];
                canonTy = canonicalize(parmTy, scope);
                if (canonTy != parmTy) {
                    funcTy->setParameterType(idx, canonTy);
                    discardedTys_.insert(parmTy);
                }
            }
            break;
        }

        case TypeKind::Pointer: {
            auto ptrTy = ty->asPointerType();
            auto refedTy = ptrTy->referencedType();
            auto canonTy = canonicalize(refedTy, scope);
            if (canonTy != refedTy) {
                ptrTy->resetReferencedType(canonTy);
                discardedTys_.insert(refedTy);
            }
            break;
        }

        case TypeKind::TypedefName: {
            auto tydefNameTy = ty->asTypedefNameType();
            if (tydefNameTy->declaration())
                return tydefNameTy->declaration()->introducedSynonymType();
            auto tydefName = tydefNameTy->typedefName();
            auto decl = scope->searchForDeclaration(
                        tydefName,
                        NameSpace::OrdinaryIdentifiers);
            if (decl) {
                if (decl->category() == DeclarationCategory::Type) {
                    auto tyDecl = decl->asTypeDeclaration();
                    // A type declaration found under the ordinary identifiers
                    // namespace must be that of a typedef.
                    PSY_ASSERT_2(tyDecl->category() == TypeDeclarationCategory::Typedef, return ty);
                    auto tydef = tyDecl->asTypedefDeclaration();
                    PSY_ASSERT_2(tydef->introducedSynonymType() != tydefNameTy, return ty);
                    discardedTys_.insert(tydefNameTy);
                    return tydef->introducedSynonymType();
                }
                //if (tree_->completeness() == TextCompleteness::Full)
                diagReporter_.ExpectedTypedefDeclaration(tySpecNode_->lastToken());
            }
            else {
                //if (tree_->completeness() == TextCompleteness::Full)
                if (!internalTydefNameIdents_.count(tydefName))
                    diagReporter_.TypeDeclarationNotFound(tySpecNode_->lastToken());
            }
            return semaModel_->compilation()->canonicalErrorType();
        }

        case TypeKind::Tag: {
            auto tagTy = ty->asTagType();
            if (tagTy->declaration())
                return tagTy->declaration()->introducedNewType();
            auto tag = tagTy->tag();
            auto decl = scope->searchForDeclaration(
                        tag,
                        NameSpace::Tags);
            if (decl) {
                // A declaration found under the tags name space must be that
                // of a tag type declaration.
                PSY_ASSERT_2(decl->category() == DeclarationCategory::Type, return ty);
                auto tyDecl = decl->asTypeDeclaration();
                PSY_ASSERT_2(tyDecl->category() == TypeDeclarationCategory::Tag, return ty);
                auto tagDecl = tyDecl->asTagTypeDeclaration();
                PSY_ASSERT_2(tagDecl->introducedNewType() != tagTy, return ty);
                if (tagTy->kind() == tagDecl->introducedNewType()->kind()) {
                    discardedTys_.insert(tagTy);
                    return canonicalize(tagDecl->introducedNewType(), scope);
                }
                //if (tree_->completeness() == TextCompleteness::Full)
                diagReporter_.TagTypeDoesNotMatchTagDeclaration(tySpecNode_->lastToken());
            }
            else {
                //if (tree_->completeness() == TextCompleteness::Full)
                diagReporter_.TypeDeclarationNotFound(tySpecNode_->lastToken());
            }
            return semaModel_->compilation()->canonicalErrorType();
        }

        case TypeKind::Qualified: {
            auto qualTy = ty->asQualifiedType();
            auto unqualTy = qualTy->unqualifiedType();
            auto canonTy = canonicalize(unqualTy, scope);
            if (canonTy != unqualTy) {
                qualTy->resetUnqualifiedType(
                        canonTy->kind() == TypeKind::Qualified
                            ? canonTy->asQualifiedType()->unqualifiedType()
                            : canonTy);
                discardedTys_.insert(unqualTy);
            }
            break;
        }

        case TypeKind::Error:
            break;
    }

    return ty;
}
