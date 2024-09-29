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

template <class TyDeclT>
SyntaxVisitor::Action Binder::visitTagDeclaration_AtInternalDeclarations_COMMON(
        const TyDeclT* node,
        Action (Binder::*visit_AtEnd)(const TyDeclT*))
{
    for (auto declIt = node->typeSpecifier()->declarations(); declIt; declIt = declIt->next)
        visit(declIt->value);
    return ((this)->*(visit_AtEnd))(node);
}

SyntaxVisitor::Action Binder::visitStructOrUnionDeclaration_AtSpecifier(
        const StructOrUnionDeclarationSyntax* node)
{
    const TagTypeSpecifierSyntax* tySpec = node->typeSpecifier();
    switch (tySpec->kind()) {
        case SyntaxKind::StructTypeSpecifier: {
            auto tagTy = makeType<TagType>(
                        TagTypeKind::Struct,
                        identifier(tySpec->tagToken()));
            bindAndPushSymbol<Struct>(node, tagTy);
            break;
        }
        case SyntaxKind::UnionTypeSpecifier:{
            auto tagTy = makeType<TagType>(
                        TagTypeKind::Union,
                        identifier(tySpec->tagToken()));
            bindAndPushSymbol<Union>(node, tagTy);
            break;
        }
        default:
            PSY_ASSERT_2(false, return Action::Quit);
    }
    return visitTagDeclaration_AtInternalDeclarations_COMMON(
                node,
                &Binder::visitStructOrUnionDeclaration_AtEnd);
}

SyntaxVisitor::Action Binder::visitEnumDeclaration_AtSpecifier(const EnumDeclarationSyntax* node)
{
    auto tagTy = makeType<TagType>(
                TagTypeKind::Enum,
                identifier(node->typeSpec_->tagToken()));
    bindAndPushSymbol<Enum>(node, tagTy);

    return visitTagDeclaration_AtInternalDeclarations_COMMON(
                node,
                &Binder::visitEnumDeclaration_AtEnd);
}

template <class DeclT>
SyntaxVisitor::Action Binder::visitDeclaration_AtSpecifiers_COMMON(
        const DeclT* node,
        Action (Binder::*visit_AtDeclarators)(const DeclT*))
{
    tySpecHasImplicit_int = false;
    tySpecHasImplicit_double = false;
    for (auto specIt = node->specifiers(); specIt; specIt = specIt->next)
        visitIfNotTypeQualifier(specIt->value);

    if (tys_.empty()) {
        diagReporter_.TypeSpecifierMissingDefaultsToInt(node->lastToken());
        pushType(makeType<BasicType>(BasicTypeKind::Int));
    }

    for (auto specIt = node->specifiers(); specIt; specIt = specIt->next)
        visitIfTypeQualifier(specIt->value);

    return ((this)->*(visit_AtDeclarators))(node);
}

SyntaxVisitor::Action Binder::visitTypedefDeclaration_AtSpecifier(const TypedefDeclarationSyntax* node)
{
    return visitDeclaration_AtSpecifiers_COMMON(
                node,
                &Binder::visitTypedefDeclaration_AtDeclarators);
}

SyntaxVisitor::Action Binder::visitVariableAndOrFunctionDeclaration_AtSpecifiers(
        const VariableAndOrFunctionDeclarationSyntax* node)
{
    return visitDeclaration_AtSpecifiers_COMMON(
                node,
                &Binder::visitVariableAndOrFunctionDeclaration_AtDeclarators);
}

SyntaxVisitor::Action Binder::visitFunctionDefinition_AtSpecifiers(const FunctionDefinitionSyntax* node)
{
    return visitDeclaration_AtSpecifiers_COMMON(
                node,
                &Binder::visitFunctionDefinition_AtDeclarator);
}

SyntaxVisitor::Action Binder::visitFieldDeclaration_AtSpecifiers(const FieldDeclarationSyntax* node)
{
    return visitDeclaration_AtSpecifiers_COMMON(
                node,
                &Binder::visitFieldDeclaration_AtDeclarators);
}

SyntaxVisitor::Action Binder::visitEnumeratorDeclaration_AtImplicitSpecifier(const EnumeratorDeclarationSyntax* node)
{
    pushType(makeType<BasicType>(BasicTypeKind::Int));
    return visitEnumeratorDeclaration_AtDeclarator(node);
}

SyntaxVisitor::Action Binder::visitParameterDeclaration_AtSpecifiers(const ParameterDeclarationSyntax* node)
{
    return visitDeclaration_AtSpecifiers_COMMON(
                node,
                &Binder::visitParameterDeclaration_AtDeclarator);
}

SyntaxVisitor::Action Binder::visitIfNotTypeQualifier(const SpecifierSyntax* spec)
{
    if (!spec->asTypeQualifier())
        visit(spec);

    return Action::Skip;
}

SyntaxVisitor::Action Binder::visitIfTypeQualifier(const SpecifierSyntax* spec)
{
    if (spec->asTypeQualifier())
        visit(spec);

    return Action::Skip;
}

SyntaxVisitor::Action Binder::visitBasicTypeSpecifier(const BasicTypeSpecifierSyntax* node)
{
    auto tySpecTk = node->specifierToken();
    if (tys_.empty()) {
        BasicTypeKind basicTyK;
        switch (tySpecTk.kind()) {
            case SyntaxKind::Keyword_char:
                basicTyK = BasicTypeKind::Char;
                break;
            case SyntaxKind::Keyword_short:
                basicTyK = BasicTypeKind::Short;
                tySpecHasImplicit_int = true;
                break;
            case SyntaxKind::Keyword_int:
                basicTyK = BasicTypeKind::Int;
                break;
            case SyntaxKind::Keyword_long:
                basicTyK = BasicTypeKind::Long;
                tySpecHasImplicit_int = true;
                break;
            case SyntaxKind::Keyword_float:
                basicTyK = BasicTypeKind::Float;
                break;
            case SyntaxKind::Keyword_double:
                basicTyK = BasicTypeKind::Double;
                break;
            case SyntaxKind::Keyword__Bool:
                basicTyK = BasicTypeKind::Bool;
                break;
            case SyntaxKind::Keyword__Complex:
                tySpecHasImplicit_double = true;
                basicTyK = BasicTypeKind::DoubleComplex;
                break;
            case SyntaxKind::Keyword_signed:
                tySpecHasImplicit_int = true;
                basicTyK = BasicTypeKind::Int_S;
                break;
            case SyntaxKind::Keyword_unsigned:
                tySpecHasImplicit_int = true;
                basicTyK = BasicTypeKind::Int_U;
                break;
            default:
                PSY_ASSERT_FAIL_1(return Action::Quit);
        }
        pushType(makeType<BasicType>(basicTyK));
        return Action::Skip;
    }

    DECL_TOP_TY_retQ(ty);
    if (ty->kind() == TypeKind::Basic) {
        BasicType* curBasicTy = ty->asBasicType();
        auto curBasicTyK = ty->asBasicType()->kind();
        switch (tySpecTk.kind()) {
            case SyntaxKind::Keyword_char:
                switch (curBasicTyK) {
                    case BasicTypeKind::Int_S:
                        if (tySpecHasImplicit_int) {
                            curBasicTy->resetBasicTypeKind(BasicTypeKind::Char_S);
                            return Action::Skip;
                        }
                        break;
                    case BasicTypeKind::Int_U:
                        if (tySpecHasImplicit_int) {
                            curBasicTy->resetBasicTypeKind(BasicTypeKind::Char_U);
                            return Action::Skip;
                        }
                        break;
                    default:
                        break;
                }
                break;

            case SyntaxKind::Keyword_short:
                switch (curBasicTyK) {
                    case BasicTypeKind::Int:
                        curBasicTy->resetBasicTypeKind(BasicTypeKind::Short);
                        return Action::Skip;
                    case BasicTypeKind::Int_S:
                        curBasicTy->resetBasicTypeKind(BasicTypeKind::Short_S);
                        return Action::Skip;
                    case BasicTypeKind::Int_U:
                        curBasicTy->resetBasicTypeKind(BasicTypeKind::Short_U);
                        return Action::Skip;
                    default:
                        break;
                }
                break;

            case SyntaxKind::Keyword_int: {
                auto allowIdentity = tySpecHasImplicit_int;
                tySpecHasImplicit_int = false;
                switch (curBasicTyK) {
                    case BasicTypeKind::Short:
                    case BasicTypeKind::Short_S:
                    case BasicTypeKind::Short_U:
                    case BasicTypeKind::Int_S:
                    case BasicTypeKind::Int_U:
                    case BasicTypeKind::Long_S:
                    case BasicTypeKind::Long_U:
                    case BasicTypeKind::Long:
                    case BasicTypeKind::LongLong:
                    case BasicTypeKind::LongLong_S:
                    case BasicTypeKind::LongLong_U:
                        if (allowIdentity)
                            return Action::Skip;
                        break;
                    default:
                        break;
                }
                break;
            }
            case SyntaxKind::Keyword_long:
                switch (curBasicTyK) {
                    case BasicTypeKind::Int:
                        curBasicTy->resetBasicTypeKind(BasicTypeKind::Long);
                        return Action::Skip;
                    case BasicTypeKind::Int_S:
                        curBasicTy->resetBasicTypeKind(BasicTypeKind::Long_S);
                        return Action::Skip;
                    case BasicTypeKind::Int_U:
                        curBasicTy->resetBasicTypeKind(BasicTypeKind::Long_U);
                        return Action::Skip;
                    case BasicTypeKind::Long:
                        curBasicTy->resetBasicTypeKind(BasicTypeKind::LongLong);
                        return Action::Skip;
                    case BasicTypeKind::Long_S:
                        curBasicTy->resetBasicTypeKind(BasicTypeKind::LongLong_S);
                        return Action::Skip;
                    case BasicTypeKind::Long_U:
                        curBasicTy->resetBasicTypeKind(BasicTypeKind::LongLong_U);
                        return Action::Skip;
                    case BasicTypeKind::Double:
                        curBasicTy->resetBasicTypeKind(BasicTypeKind::LongDouble);
                        return Action::Skip;
                    case BasicTypeKind::DoubleComplex:
                        curBasicTy->resetBasicTypeKind(BasicTypeKind::LongDoubleComplex);
                        return Action::Skip;
                    default:
                        break;
                }
                break;

            case SyntaxKind::Keyword_float:
                switch (curBasicTyK) {
                    case BasicTypeKind::DoubleComplex:
                        if (tySpecHasImplicit_double) {
                            curBasicTy->resetBasicTypeKind(BasicTypeKind::FloatComplex);
                            return Action::Skip;
                        }
                        break;
                    default:
                        break;
                }
                break;

            case SyntaxKind::Keyword_double: {
                bool allowIdentity = tySpecHasImplicit_double;
                tySpecHasImplicit_double = false;
                switch (curBasicTyK) {
                    case BasicTypeKind::Long:
                        if (tySpecHasImplicit_int) {
                            curBasicTy->resetBasicTypeKind(BasicTypeKind::LongDouble);
                            return Action::Skip;
                        }
                        break;
                    case BasicTypeKind::DoubleComplex:
                    case BasicTypeKind::LongDoubleComplex:
                        if (allowIdentity)
                            return Action::Skip;
                        break;
                    default:
                        break;
                }
                break;
            }
            case SyntaxKind::Keyword__Bool:
                break;

            case SyntaxKind::Keyword__Complex:
                switch (curBasicTyK) {
                    case BasicTypeKind::Long:
                    case BasicTypeKind::LongDouble:
                        curBasicTy->resetBasicTypeKind(BasicTypeKind::LongDoubleComplex);
                        return Action::Skip;
                    case BasicTypeKind::Float:
                        curBasicTy->resetBasicTypeKind(BasicTypeKind::FloatComplex);
                        return Action::Skip;
                    case BasicTypeKind::Double:
                        curBasicTy->resetBasicTypeKind(BasicTypeKind::DoubleComplex);
                        return Action::Skip;
                    default:
                        break;
                }
                break;

            case SyntaxKind::Keyword_signed:
                switch (curBasicTyK) {
                    case BasicTypeKind::Char:
                        curBasicTy->resetBasicTypeKind(BasicTypeKind::Char_S);
                        return Action::Skip;
                    case BasicTypeKind::Short:
                        curBasicTy->resetBasicTypeKind(BasicTypeKind::Short_S);
                        return Action::Skip;
                    case BasicTypeKind::Int:
                        curBasicTy->resetBasicTypeKind(BasicTypeKind::Int_S);
                        return Action::Skip;
                    case BasicTypeKind::Long:
                        curBasicTy->resetBasicTypeKind(BasicTypeKind::Long_S);
                        return Action::Skip;
                    case BasicTypeKind::LongLong:
                        curBasicTy->resetBasicTypeKind(BasicTypeKind::LongLong_S);
                        return Action::Skip;
                    default:
                        break;
                }
                break;

            case SyntaxKind::Keyword_unsigned:
                switch (curBasicTyK) {
                    case BasicTypeKind::Char:
                        curBasicTy->resetBasicTypeKind(BasicTypeKind::Char_U);
                        return Action::Skip;
                    case BasicTypeKind::Short:
                        curBasicTy->resetBasicTypeKind(BasicTypeKind::Short_U);
                        return Action::Skip;
                    case BasicTypeKind::Int:
                        curBasicTy->resetBasicTypeKind(BasicTypeKind::Int_U);
                        return Action::Skip;
                    case BasicTypeKind::Long:
                        curBasicTy->resetBasicTypeKind(BasicTypeKind::Long_U);
                        return Action::Skip;
                    case BasicTypeKind::LongLong:
                        curBasicTy->resetBasicTypeKind(BasicTypeKind::LongLong_U);
                        return Action::Skip;
                    default:
                        break;
                }
                break;

            default:
                PSY_ASSERT_FAIL_1(return Action::Quit);
        }
    }

    diagReporter_.InvalidType(tySpecTk);
    return Action::Skip;
}

SyntaxVisitor::Action Binder::visitVoidTypeSpecifier(const VoidTypeSpecifierSyntax* node)
{
    pushType(makeType<VoidType>());

    return Action::Skip;
}

SyntaxVisitor::Action Binder::visitTagTypeSpecifier(const TagTypeSpecifierSyntax* node)
{
    if (!node->declarations()) {
        TagTypeKind tagTyK;
        switch (node->kind()) {
            case SyntaxKind::StructTypeSpecifier:
                tagTyK = TagTypeKind::Struct;
                break;

            case SyntaxKind::UnionTypeSpecifier:
                tagTyK = TagTypeKind::Union;
                break;

            case SyntaxKind::EnumTypeSpecifier:
                tagTyK = TagTypeKind::Enum;
                break;

            default:
                PSY_ASSERT_2(false, return Action::Quit);
        }
        pushType(makeType<TagType>(tagTyK, identifier(node->tagToken())));
    }

    for (auto attrIt = node->attributes(); attrIt; attrIt = attrIt->next)
        visit(attrIt->value);

    for (auto declIt = node->declarations(); declIt; declIt = declIt->next) {
        TyContT tys;
        std::swap(tys_, tys);
        visit(declIt->value);
        std::swap(tys_, tys);
    }

    for (auto attrIt = node->attributes_PostCloseBrace(); attrIt; attrIt = attrIt->next)
        visit(attrIt->value);

    return Action::Skip;
}

SyntaxVisitor::Action Binder::visitTagDeclarationAsSpecifier(
        const TagDeclarationAsSpecifierSyntax* node)
{
    visit(node->tagDeclaration());

    const TagTypeSpecifierSyntax* tySpec = node->tagDeclaration()->typeSpecifier();
    TagType* tagTy = nullptr;
    switch (tySpec->kind()) {
        case SyntaxKind::StructTypeSpecifier:
            tagTy = makeType<TagType>(
                        TagTypeKind::Struct,
                        identifier(tySpec->tagToken()));
            break;

        case SyntaxKind::UnionTypeSpecifier:
            tagTy = makeType<TagType>(
                        TagTypeKind::Union,
                        identifier(tySpec->tagToken()));
            break;

        case SyntaxKind::EnumTypeSpecifier:
            tagTy = makeType<TagType>(
                        TagTypeKind::Enum,
                        identifier(tySpec->tagToken()));
            break;

        default:
            PSY_ASSERT_2(false, return Action::Quit);
    }
    pushType(tagTy);

    return Action::Skip;
}

SyntaxVisitor::Action Binder::visitTypedefName(const TypedefNameSyntax* node)
{
    if (tys_.empty())
        pushType(makeType<TypedefType>(identifier(node->identifierToken())));

    return Action::Skip;
}

SyntaxVisitor::Action Binder::visitTypeQualifier(const TypeQualifierSyntax* node)
{
    QualifiedType* qualTy = nullptr;
    DECL_TOP_TY_retQ(ty);
    switch (ty->kind()) {
        case TypeKind::Qualified:
            qualTy = ty->asQualifiedType();
            break;

        default:
            qualTy = makeType<QualifiedType>(ty);
            popType();
            pushType(qualTy);
            break;
    }
    PSY_ASSERT_2(qualTy, return Action::Quit);

    const auto tyQualTk = node->qualifierKeyword();
    switch (tyQualTk.kind()) {
        case SyntaxKind::Keyword_const:
            qualTy->qualifyWithConst();
            break;

        case SyntaxKind::Keyword_volatile:
            qualTy->qualifyWithVolatile();
            break;

        case SyntaxKind::Keyword_restrict:
            if (qualTy->unqualifiedType()->kind() == TypeKind::Pointer)
                qualTy->qualifyWithRestrict();
            else
                diagReporter_.InvalidUseOfRestrict(tyQualTk);
            break;

        case SyntaxKind::Keyword__Atomic:
            qualTy->qualifyWithAtomic();
            break;

        default:
            PSY_ASSERT_FAIL_1(return Action::Quit);
    }

    return Action::Skip;
}
