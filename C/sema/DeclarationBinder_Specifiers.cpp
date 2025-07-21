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

#include "sema/Scope.h"
#include "sema/SemanticModel.h"
#include "symbols/Symbol_ALL.h"
#include "syntax/Lexeme_ALL.h"
#include "syntax/SyntaxFacts.h"
#include "syntax/SyntaxTree.h"
#include "syntax/SyntaxNodes.h"
#include "syntax/SyntaxUtilities.h"
#include "syntax/SyntaxVisitor__MACROS__.inc"
#include "types/Type_ALL.h"
#include "../common/infra/Assertions.h"

#include <iostream>

using namespace psy;
using namespace C;

template <class TyDeclT>
SyntaxVisitor::Action DeclarationBinder::visitTagDeclaration_AtMemberDeclarations(
        const TyDeclT* node,
        Action (DeclarationBinder::*visit_AtEnd)(const TyDeclT*))
{
    VISIT(node->typeSpecifier()->declarations());
    return ((this)->*(visit_AtEnd))(node);
}

SyntaxVisitor::Action DeclarationBinder::visitStructOrUnionDeclaration_AtSpecifier(
        const StructOrUnionDeclarationSyntax* node)
{
    const TagTypeSpecifierSyntax* tySpec = node->typeSpecifier();
    switch (tySpec->kind()) {
        case SyntaxKind::StructTypeSpecifier: {
            auto tagTy = makeType<TagType>(
                        TagTypeKind::Struct,
                        obtainTag(tySpec->tagToken()));
            bindDeclaration<StructDeclarationSymbol>(node, tagTy);
            break;
        }
        case SyntaxKind::UnionTypeSpecifier:{
            auto tagTy = makeType<TagType>(
                        TagTypeKind::Union,
                        obtainTag(tySpec->tagToken()));
            bindDeclaration<UnionDeclarationSymbol>(node, tagTy);
            break;
        }
        default:
            PSY_ASSERT_1(false);
            return Action::Quit;
    }

    return visitTagDeclaration_AtMemberDeclarations(
                node,
                &DeclarationBinder::visitStructOrUnionDeclaration_AtEnd);
}

SyntaxVisitor::Action DeclarationBinder::visitEnumDeclaration_AtSpecifier(const EnumDeclarationSyntax* node)
{
    auto tagTy = makeType<TagType>(
                TagTypeKind::Enum,
                obtainTag(node->typeSpec_->tagToken()));
    bindDeclaration<EnumDeclarationSymbol>(node, tagTy);

    return visitTagDeclaration_AtMemberDeclarations(
                node,
                &DeclarationBinder::visitEnumDeclaration_AtEnd);
}

template <class NodeT>
SyntaxVisitor::Action DeclarationBinder::visit_AtSpecifiers_COMMON(
        const NodeT* node,
        Action (DeclarationBinder::*visit_AtDeclarators)(const NodeT*))
{
    F_.inImplicitIntTySpec_ = false;
    F_.inImplicitDoubleTySpec_ = false;
    F_.inExplicitSignedOrUnsignedTySpec_ = false;

    for (auto specIt = node->specifiers(); specIt; specIt = specIt->next) {
        if (!specIt->value->asTypeQualifier())
            VISIT(specIt->value);
    }

    if (tys_.empty()) {
        diagReporter_.TypeSpecifierMissingDefaultsToInt(node->lastToken());
        pushType(makeType<BasicType>(BasicTypeKind::Int_S));
    }

    for (auto specIt = node->specifiers(); specIt; specIt = specIt->next) {
        if (specIt->value->asTypeQualifier())
            VISIT(specIt->value);
    }

    return ((this)->*(visit_AtDeclarators))(node);
}

SyntaxVisitor::Action DeclarationBinder::visitTypedefDeclaration_AtSpecifier(const TypedefDeclarationSyntax* node)
{
    return visit_AtSpecifiers_COMMON(
                node,
                &DeclarationBinder::visitTypedefDeclaration_AtDeclarators);
}

SyntaxVisitor::Action DeclarationBinder::visitVariableAndOrFunctionDeclaration_AtSpecifiers(
        const VariableAndOrFunctionDeclarationSyntax* node)
{
    return visit_AtSpecifiers_COMMON(
                node,
                &DeclarationBinder::visitVariableAndOrFunctionDeclaration_AtDeclarators);
}

SyntaxVisitor::Action DeclarationBinder::visitFunctionDefinition_AtSpecifiers(const FunctionDefinitionSyntax* node)
{
    return visit_AtSpecifiers_COMMON(
                node,
                &DeclarationBinder::visitFunctionDefinition_AtDeclarator);
}

SyntaxVisitor::Action DeclarationBinder::visitFieldDeclaration_AtSpecifiers(const FieldDeclarationSyntax* node)
{
    return visit_AtSpecifiers_COMMON(
                node,
                &DeclarationBinder::visitFieldDeclaration_AtDeclarators);
}

SyntaxVisitor::Action DeclarationBinder::visitEnumeratorDeclaration_AtImplicitSpecifier(
        const EnumeratorDeclarationSyntax* node)
{
    pushType(makeType<BasicType>(BasicTypeKind::Int_S));
    return visitEnumeratorDeclaration_AtDeclaratorLike(node);
}

SyntaxVisitor::Action DeclarationBinder::visitParameterDeclaration_AtSpecifiers(const ParameterDeclarationSyntax* node)
{
    return visit_AtSpecifiers_COMMON(
                node,
                &DeclarationBinder::visitParameterDeclaration_AtDeclarator);
}

SyntaxVisitor::Action DeclarationBinder::visitBasicTypeSpecifier(const BasicTypeSpecifierSyntax* node)
{
    auto tySpecTk = node->specifierToken();
    if (tys_.empty()) {
        BasicTypeKind basicTyK;
        switch (tySpecTk.kind()) {
            case SyntaxKind::Keyword_char:
                basicTyK = BasicTypeKind::Char;
                break;
            case SyntaxKind::Keyword_short:
                basicTyK = BasicTypeKind::Short_S;
                F_.inImplicitIntTySpec_ = true;
                break;
            case SyntaxKind::Keyword_int:
                basicTyK = BasicTypeKind::Int_S;
                break;
            case SyntaxKind::Keyword_long:
                basicTyK = BasicTypeKind::Long_S;
                F_.inImplicitIntTySpec_ = true;
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
            case SyntaxKind::Keyword_ExtGNU___complex__:
                F_.inImplicitDoubleTySpec_ = true;
                basicTyK = BasicTypeKind::DoubleComplex;
                break;
            case SyntaxKind::Keyword_signed:
                F_.inImplicitIntTySpec_ = true;
                F_.inExplicitSignedOrUnsignedTySpec_ = true;
                basicTyK = BasicTypeKind::Int_S;
                break;
            case SyntaxKind::Keyword_unsigned:
                F_.inImplicitIntTySpec_ = true;
                F_.inExplicitSignedOrUnsignedTySpec_ = true;
                basicTyK = BasicTypeKind::Int_U;
                break;
            default:
                PSY_ASSERT_1(false);
                return Action::Quit;
        }
        pushType(makeType<BasicType>(basicTyK));
        return Action::Skip;
    }

    TY_AT_TOP(auto ty, Action::Quit);
    if (ty->kind() == TypeKind::Basic) {
        BasicType* curBasicTy = ty->asBasicType();
        auto curBasicTyK = ty->asBasicType()->kind();
        switch (tySpecTk.kind()) {
            case SyntaxKind::Keyword_char:
                switch (curBasicTyK) {
                    case BasicTypeKind::Int_S:
                        if (F_.inImplicitIntTySpec_) {
                            curBasicTy->resetBasicTypeKind(BasicTypeKind::Char_S);
                            return Action::Skip;
                        }
                        break;
                    case BasicTypeKind::Int_U:
                        if (F_.inImplicitIntTySpec_) {
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
                auto allowIntTySpec = F_.inImplicitIntTySpec_;
                F_.inImplicitIntTySpec_ = false;
                switch (curBasicTyK) {
                    case BasicTypeKind::Short_S:
                    case BasicTypeKind::Short_U:
                    case BasicTypeKind::Int_S:
                    case BasicTypeKind::Int_U:
                    case BasicTypeKind::Long_S:
                    case BasicTypeKind::Long_U:
                    case BasicTypeKind::LongLong_S:
                    case BasicTypeKind::LongLong_U:
                        if (allowIntTySpec)
                            return Action::Skip;
                        break;
                    default:
                        break;
                }
                break;
            }
            case SyntaxKind::Keyword_long:
                switch (curBasicTyK) {
                    case BasicTypeKind::Int_S:
                        curBasicTy->resetBasicTypeKind(BasicTypeKind::Long_S);
                        return Action::Skip;
                    case BasicTypeKind::Int_U:
                        curBasicTy->resetBasicTypeKind(BasicTypeKind::Long_U);
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
                if (!F_.inExplicitSignedOrUnsignedTySpec_) {
                    switch (curBasicTyK) {
                        case BasicTypeKind::DoubleComplex:
                            if (F_.inImplicitDoubleTySpec_) {
                                curBasicTy->resetBasicTypeKind(BasicTypeKind::FloatComplex);
                                return Action::Skip;
                            }
                            break;
                        default:
                            break;
                    }
                }
                break;

            case SyntaxKind::Keyword_double: {
                bool allowDoubleTySpec = F_.inImplicitDoubleTySpec_;
                F_.inImplicitDoubleTySpec_ = false;
                if (!F_.inExplicitSignedOrUnsignedTySpec_) {
                    switch (curBasicTyK) {
                        case BasicTypeKind::Long_S:
                            if (F_.inImplicitIntTySpec_) {
                                curBasicTy->resetBasicTypeKind(BasicTypeKind::LongDouble);
                                return Action::Skip;
                            }
                            break;
                        case BasicTypeKind::DoubleComplex:
                        case BasicTypeKind::LongDoubleComplex:
                            if (allowDoubleTySpec)
                                return Action::Skip;
                            break;
                        default:
                            break;
                    }
                }
                break;
            }
            case SyntaxKind::Keyword__Bool:
                break;

            case SyntaxKind::Keyword__Complex:
            case SyntaxKind::Keyword_ExtGNU___complex__:
                switch (curBasicTyK) {
                    case BasicTypeKind::Long_S:
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
                if (!F_.inExplicitSignedOrUnsignedTySpec_) {
                    switch (curBasicTyK) {
                        case BasicTypeKind::Char:
                            curBasicTy->resetBasicTypeKind(BasicTypeKind::Char_S);
                            return Action::Skip;
                        case BasicTypeKind::Short_S:
                            curBasicTy->resetBasicTypeKind(BasicTypeKind::Short_S);
                            return Action::Skip;
                        case BasicTypeKind::Int_S:
                            curBasicTy->resetBasicTypeKind(BasicTypeKind::Int_S);
                            return Action::Skip;
                        case BasicTypeKind::Long_S:
                            curBasicTy->resetBasicTypeKind(BasicTypeKind::Long_S);
                            return Action::Skip;
                        case BasicTypeKind::LongLong_S:
                            curBasicTy->resetBasicTypeKind(BasicTypeKind::LongLong_S);
                            return Action::Skip;
                        default:
                            break;
                    }
                    F_.inExplicitSignedOrUnsignedTySpec_ = true;
                }
                break;

            case SyntaxKind::Keyword_unsigned:
                if (!F_.inExplicitSignedOrUnsignedTySpec_) {
                    switch (curBasicTyK) {
                        case BasicTypeKind::Char:
                            curBasicTy->resetBasicTypeKind(BasicTypeKind::Char_U);
                            return Action::Skip;
                        case BasicTypeKind::Short_S:
                            curBasicTy->resetBasicTypeKind(BasicTypeKind::Short_U);
                            return Action::Skip;
                        case BasicTypeKind::Int_S:
                            curBasicTy->resetBasicTypeKind(BasicTypeKind::Int_U);
                            return Action::Skip;
                        case BasicTypeKind::Long_S:
                            curBasicTy->resetBasicTypeKind(BasicTypeKind::Long_U);
                            return Action::Skip;
                        case BasicTypeKind::LongLong_S:
                            curBasicTy->resetBasicTypeKind(BasicTypeKind::LongLong_U);
                            return Action::Skip;
                        default:
                            break;
                    }
                    F_.inExplicitSignedOrUnsignedTySpec_ = true;
                }
                break;

            default:
                PSY_ASSERT_1(false);
                return Action::Quit;
        }
    }

    diagReporter_.InvalidType(tySpecTk);
    return Action::Skip;
}

SyntaxVisitor::Action DeclarationBinder::visitVoidTypeSpecifier(const VoidTypeSpecifierSyntax* node)
{
    pushType(makeType<VoidType>());

    return Action::Skip;
}

SyntaxVisitor::Action DeclarationBinder::visitTagTypeSpecifier(const TagTypeSpecifierSyntax* node)
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
                PSY_ASSERT_1(false);
                return Action::Quit;
        }
        pushType(makeType<TagType>(tagTyK, obtainTag(node->tagToken())));
    }

    VISIT(node->attributes());
    for (auto declIt = node->declarations(); declIt; declIt = declIt->next) {
        TypeStack tys;
        std::swap(tys_, tys);
        VISIT(declIt->value);
        std::swap(tys_, tys);
    }
    VISIT(node->attributes_PostCloseBrace());

    return Action::Skip;
}

SyntaxVisitor::Action DeclarationBinder::visitTagDeclarationAsSpecifier(
        const TagDeclarationAsSpecifierSyntax* node)
{
    VISIT(node->tagDeclaration());

    const TagTypeSpecifierSyntax* tySpec = node->tagDeclaration()->typeSpecifier();
    TagType* tagTy = nullptr;
    switch (tySpec->kind()) {
        case SyntaxKind::StructTypeSpecifier:
            tagTy = makeType<TagType>(
                        TagTypeKind::Struct,
                        obtainKnownTag(tySpec->tagToken()));
            break;

        case SyntaxKind::UnionTypeSpecifier:
            tagTy = makeType<TagType>(
                        TagTypeKind::Union,
                        obtainKnownTag(tySpec->tagToken()));
            break;

        case SyntaxKind::EnumTypeSpecifier:
            tagTy = makeType<TagType>(
                        TagTypeKind::Enum,
                        obtainKnownTag(tySpec->tagToken()));
            break;

        default:
            PSY_ASSERT_1(false);
            return Action::Quit;
    }
    pushType(tagTy);

    return Action::Skip;
}

SyntaxVisitor::Action DeclarationBinder::visitTypedefName(const TypedefNameSyntax* node)
{
    if (tys_.empty())
        pushType(makeType<TypedefNameType>(obtainName(node->identifierToken())));

    return Action::Skip;
}

SyntaxVisitor::Action DeclarationBinder::visitTypeQualifier(const TypeQualifierSyntax* node)
{
    QualifiedType* qualTy = nullptr;
    TY_AT_TOP(auto ty, Action::Quit);
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

SyntaxVisitor::Action DeclarationBinder::visitExtGNU_Attribute(const ExtGNU_AttributeSyntax* node)
{
    return Action::Skip;
}

SyntaxVisitor::Action DeclarationBinder::visitTypeName_AtSpecifier(const TypeNameSyntax* node)
{
    return visit_AtSpecifiers_COMMON(
                node,
                &DeclarationBinder::visitTypeName_AtDeclarator);
}
