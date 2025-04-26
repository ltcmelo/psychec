// Copyright (c) 2024-2025 Leandro T. C. Melo <ltcmelo@gmail.com>
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

#include "TypeChecker.h"

#include "syntax/SyntaxTree.h"

#include "sema/Scope.h"
#include "sema/PlatformOptions.h"
#include "sema/Compilation.h"
#include "sema/SemanticModel.h"
#include "symbols/Symbol_ALL.h"
#include "syntax/Lexeme_ALL.h"
#include "syntax/SyntaxFacts.h"
#include "syntax/SyntaxNodes.h"
#include "syntax/SyntaxVisitor__MACROS__.inc"
#include "syntax/SyntaxToken.h"
#include "syntax/SyntaxUtilities.h"
#include "types/Type_ALL.h"

#include "../common/infra/Assertions.h"

#include <cstdint>

#define VISIT_EXPR(NODE) do { if (visitExpression(NODE) == Action::Quit) return Action::Quit; } while (0)

using namespace psy;
using namespace C;

namespace
{
const Type* pickType(const TypedefDeclarationSymbol* tydef, const Type* altTy)
{
    if (!tydef)
        return altTy;
    PSY_ASSERT_1(tydef->synonymizedType());
    return tydef->synonymizedType();
}
} // anonymous

TypeChecker::TypeChecker(SemanticModel* semaModel, const SyntaxTree* tree)
    : SyntaxVisitor(tree)
    , semaModel_(semaModel)
    , ty_(nullptr)
    , ptrdiffTy_(
          pickType(
              semaModel->ptrdiff_t_typedef(),
              semaModel->compilation()->canonicalBasicType(BasicTypeKind::Int_S)))
    , sizeTy_(
          pickType(
              semaModel->size_t_typedef(),
              semaModel->compilation()->canonicalBasicType(BasicTypeKind::Long_U)))
    , maxAlignTy_(
          pickType(
              semaModel->max_align_t_typedef(),
              semaModel->compilation()->canonicalBasicType(BasicTypeKind::LongLong_S)))
    , wcharTy_(
          pickType(
              semaModel->wchar_t_typedef(),
              semaModel->compilation()->canonicalBasicType(BasicTypeKind::Int_S)))
    , char16Ty_(
          pickType(
              semaModel->char16_t_typedef(),
              semaModel->compilation()->canonicalBasicType(BasicTypeKind::Short_U)))
    , char32Ty_(
          pickType(
              semaModel->char32_t_typedef(),
              semaModel->compilation()->canonicalBasicType(BasicTypeKind::Int_U)))
    , strLitTy_(
          semaModel_->keepType(
              std::unique_ptr<ArrayType>(new ArrayType(
                  semaModel->compilation()->canonicalBasicType(BasicTypeKind::Char_U)))))
    , u8StrLitTy_(strLitTy_)
    , uStrLitTy_(semaModel_->keepType(std::unique_ptr<ArrayType>(new ArrayType(char16Ty_))))
    , UStrLitTy_(semaModel_->keepType(std::unique_ptr<ArrayType>(new ArrayType(char32Ty_))))
    , LStrLitTy_(semaModel_->keepType(std::unique_ptr<ArrayType>(new ArrayType(wcharTy_))))
    , diagReporter_(this)
{
}

TypeChecker::~TypeChecker()
{}

void TypeChecker::checkTypes()
{
    visit(tree_->root());
}

const Type* TypeChecker::unqualifiedAndResolved(const Type* ty)
{
    do {
        switch (ty->kind()) {
            case TypeKind::Qualified:
                ty = ty->asQualifiedType()->unqualifiedType();
                break;
            case TypeKind::TypedefName:
                ty = ty->asTypedefNameType()->resolvedSynonymizedType();
                break;
            default:
                return ty;
        }
    } while (true);
}

bool TypeChecker::isAssignable(const SyntaxNode* node, const Type* ty)
{
    switch (ty->kind()) {
        case TypeKind::Qualified:
            diagReporter_.CannotAssignToExpressionOfConstQualifiedType(node->lastToken());
            return false;
        case TypeKind::TypedefName:
            return isAssignable(node, ty->asTypedefNameType()->resolvedSynonymizedType());
        case TypeKind::Array:
            diagReporter_.CannotAssignToExpressionOfArrayType(node->lastToken());
            return false;
        case TypeKind::Tag: {
            auto tagTy = ty->asTagType();
            auto tagTyDecl = tagTy->declaration();
            if (!tagTyDecl)
                return false;
            for (const auto& membDecl : tagTyDecl->members()) {
                auto membTy = membDecl->type();
                if (!isAssignable(node, membTy))
                    return false;
            }
            return true;
        }
        default:
            return true;
    }
}

SyntaxVisitor::Action TypeChecker::visitExtGNU_Attribute(const ExtGNU_AttributeSyntax*)
{
    return Action::Quit;
}

BasicTypeKind TypeChecker::performIntegerPromotion(BasicTypeKind basicTyK)
{
    switch (basicTyK) {
        case BasicTypeKind::Bool:
        case BasicTypeKind::Char_U:
        case BasicTypeKind::Short_U:
            return BasicTypeKind::Int_U;

        case BasicTypeKind::Char_S:
        case BasicTypeKind::Short_S:
            return BasicTypeKind::Int_S;

        case BasicTypeKind::Int_S:
        case BasicTypeKind::Int_U:
        case BasicTypeKind::Long_S:
        case BasicTypeKind::Long_U  :
        case BasicTypeKind::LongLong_S:
        case BasicTypeKind::LongLong_U:
            return basicTyK;

        default:
            PSY_ASSERT_1(false);
            return basicTyK;
    }
}

BasicTypeKind TypeChecker::performSignBasedIntegerConversion(
        BasicTypeKind signedTyK,
        BasicTypeKind unsignedTyK)
{
    switch (unsignedTyK) {
        case BasicTypeKind::LongLong_U:
            return unsignedTyK;
        case BasicTypeKind::Long_U:
            switch (signedTyK) {
                case BasicTypeKind::LongLong_S:
                    // TODO: Evaluate and determine.
                    return signedTyK;
                default:
                    return unsignedTyK;
            }
        case BasicTypeKind::Int_U:
            switch (signedTyK) {
                case BasicTypeKind::Long_S:
                case BasicTypeKind::LongLong_S:
                    // TODO: Evaluate and determine.
                    return signedTyK;
                default:
                    return unsignedTyK;
            }
        case BasicTypeKind::Short_U:
            switch (signedTyK) {
                case BasicTypeKind::Int_S:
                case BasicTypeKind::Long_S:
                case BasicTypeKind::LongLong_S:
                    // TODO: Evaluate and determine.
                    return signedTyK;
                default:
                    return unsignedTyK;
            }
        case BasicTypeKind::Char_U:
            switch (signedTyK) {
                case BasicTypeKind::Short_S:
                case BasicTypeKind::Int_S:
                case BasicTypeKind::Long_S:
                case BasicTypeKind::LongLong_S:
                    // TODO: Evaluate and determine.
                    return signedTyK;
                default:
                    return unsignedTyK;
            }
        case BasicTypeKind::Bool:
            return signedTyK;

        default:
            PSY_ASSERT_1(false);
            return unsignedTyK;
    }
}

BasicTypeKind TypeChecker::performArithmeticConversions(
        BasicTypeKind leftTyK,
        BasicTypeKind rightTyK)
{
    switch (leftTyK) {
        case BasicTypeKind::Float:
        case BasicTypeKind::Double:
        case BasicTypeKind::LongDouble:
        case BasicTypeKind::FloatComplex:
        case BasicTypeKind::DoubleComplex:
        case BasicTypeKind::LongDoubleComplex:
            return leftTyK;
        default:
            switch (rightTyK) {
                case BasicTypeKind::Float:
                case BasicTypeKind::Double:
                case BasicTypeKind::LongDouble:
                case BasicTypeKind::FloatComplex:
                case BasicTypeKind::DoubleComplex:
                case BasicTypeKind::LongDoubleComplex:
                    return rightTyK;
                default:
                    break;
            }
            break;
    }

    leftTyK = performIntegerPromotion(leftTyK);
    rightTyK = performIntegerPromotion(rightTyK);

    if (leftTyK == rightTyK)
        return rightTyK;

    if (isSignedIntegerTypeKind(leftTyK)) {
        if (isSignedIntegerTypeKind(rightTyK)) {
            switch (leftTyK) {
                case BasicTypeKind::LongLong_S:
                    return leftTyK;
                case BasicTypeKind::Long_S:
                    switch (rightTyK) {
                        case BasicTypeKind::LongLong_S:
                            return rightTyK;
                        default:
                            return leftTyK;
                    }
                default:
                    switch (rightTyK) {
                        case BasicTypeKind::LongLong_S:
                        case BasicTypeKind::Long_S:
                            return rightTyK;
                        default:
                            return leftTyK;
                    }
                    return leftTyK;
            }
        }
        return performSignBasedIntegerConversion(leftTyK, rightTyK);
    }

    if (isUnsignedIntegerTypeKind(rightTyK)) {
        switch (leftTyK) {
            case BasicTypeKind::LongLong_U:
                return leftTyK;
            case BasicTypeKind::Long_U:
                switch (rightTyK) {
                    case BasicTypeKind::LongLong_U:
                        return rightTyK;
                    default:
                        return leftTyK;
                }
            default:
                switch (rightTyK) {
                    case BasicTypeKind::LongLong_U:
                    case BasicTypeKind::Long_U:
                        return rightTyK;
                    default:
                        return leftTyK;
                }
                return leftTyK;
        }
    }

    return performSignBasedIntegerConversion(rightTyK, leftTyK);
}

const BasicType* TypeChecker::determineCommonRealType(
        const BasicType* leftTy,
        const BasicType* rightTy) const
{
    PSY_ASSERT_2(leftTy != nullptr, return nullptr);
    PSY_ASSERT_2(rightTy != nullptr, return nullptr);
    auto basicTyK = performArithmeticConversions(leftTy->kind(), rightTy->kind());
    return semaModel_->compilation()->canonicalBasicType(basicTyK);
}

bool TypeChecker::satisfyArithmeticTypeConstraint(const Type* ty, const SyntaxNode* node)
{
    if (!isArithmeticType(ty)) {
        diagReporter_.ExpectedExpressionOfArithmeticType(node->lastToken());
        return false;
    }
    return true;
}

bool TypeChecker::satisfyIntegerTypeConstraint(const Type* ty, const SyntaxNode* node)
{
    if (!isIntegerType(ty)) {
        diagReporter_.ExpectedExpressionOfIntegerType(node->lastToken());
        return false;
    }
    return true;
}

bool TypeChecker::satisfyRealTypeConstraint(const Type* ty, const SyntaxNode* node)
{
    if (!isRealType(ty)) {
        diagReporter_.ExpectedExpressionOfRealType(node->lastToken());
        return false;
    }
    return true;
}

bool TypeChecker::typesAreCompatible(
        const Type* oneTy,
        const Type* otherTy,
        bool treatVoidAsAny,
        bool ignoreQualifier)
{
    switch (oneTy->kind()) {
        case TypeKind::Array:
            switch (otherTy->kind()) {
                case TypeKind::Array:
                    return typesAreCompatible(
                                oneTy->asArrayType()->elementType(),
                                otherTy->asArrayType()->elementType(),
                                treatVoidAsAny,
                                ignoreQualifier);
                case TypeKind::Basic:
                case TypeKind::Function:
                    break;
                case TypeKind::Pointer:
                    return typesAreCompatible(
                                oneTy->asArrayType()->elementType(),
                                otherTy->asPointerType()->referencedType(),
                                treatVoidAsAny,
                                ignoreQualifier);
                case TypeKind::TypedefName:
                    return typesAreCompatible(
                                oneTy,
                                otherTy->asTypedefNameType()->resolvedSynonymizedType(),
                                treatVoidAsAny,
                                ignoreQualifier);
                case TypeKind::Tag:
                    break;
                case TypeKind::Void:
                    return treatVoidAsAny;
                case TypeKind::Qualified:
                    break;
                case TypeKind::Error:
                    break;
            }
            break;

        case TypeKind::Basic:
            switch (otherTy->kind()) {
                case TypeKind::Array:
                    break;
                case TypeKind::Basic:
                    return oneTy->asBasicType()->kind() == otherTy->asBasicType()->kind();
                case TypeKind::Function:
                case TypeKind::Pointer:
                    break;
                case TypeKind::TypedefName:
                    return typesAreCompatible(
                                oneTy,
                                otherTy->asTypedefNameType()->resolvedSynonymizedType(),
                                treatVoidAsAny,
                                ignoreQualifier);
                case TypeKind::Tag:
                case TypeKind::Void:
                    return treatVoidAsAny;
                case TypeKind::Qualified:
                    if (ignoreQualifier)
                        return typesAreCompatible(
                                    oneTy,
                                    otherTy->asQualifiedType()->unqualifiedType(),
                                    treatVoidAsAny,
                                    false);
                    break;
                case TypeKind::Error:
                    break;
            }
            break;

        case TypeKind::Function:
            switch (otherTy->kind()) {
                case TypeKind::Array:
                case TypeKind::Basic:
                case TypeKind::Function:
                case TypeKind::Pointer:
                    break;
                case TypeKind::TypedefName:
                    return typesAreCompatible(
                                oneTy,
                                otherTy->asTypedefNameType()->resolvedSynonymizedType(),
                                treatVoidAsAny,
                                ignoreQualifier);
                case TypeKind::Tag:
                    break;
                case TypeKind::Void:
                    return treatVoidAsAny;
                case TypeKind::Qualified:
                    break;
                case TypeKind::Error:
                    break;
            }
            break;

        case TypeKind::Pointer:
            switch (otherTy->kind()) {
                case TypeKind::Array:
                    return typesAreCompatible(
                                oneTy->asPointerType()->referencedType(),
                                otherTy->asArrayType()->elementType(),
                                treatVoidAsAny,
                                ignoreQualifier);
                case TypeKind::Basic:
                case TypeKind::Function:
                    break;
                case TypeKind::Pointer:
                    return typesAreCompatible(
                                oneTy->asPointerType()->referencedType(),
                                otherTy->asPointerType()->referencedType(),
                                treatVoidAsAny,
                                ignoreQualifier);
                case TypeKind::TypedefName:
                    return typesAreCompatible(
                                oneTy,
                                otherTy->asTypedefNameType()->resolvedSynonymizedType(),
                                treatVoidAsAny,
                                ignoreQualifier);
                case TypeKind::Tag:
                    break;
                case TypeKind::Void:
                    return treatVoidAsAny;
                case TypeKind::Qualified:
                    break;
                case TypeKind::Error:
                    break;
            }
            break;

        case TypeKind::TypedefName:
            PSY_ASSERT_1(false);
            break;

        case TypeKind::Tag:
            switch (otherTy->kind()) {
                case TypeKind::Array:
                case TypeKind::Basic:
                case TypeKind::Function:
                case TypeKind::Pointer:
                    break;
                case TypeKind::TypedefName:
                    return typesAreCompatible(
                                oneTy,
                                otherTy->asTypedefNameType()->resolvedSynonymizedType(),
                                treatVoidAsAny,
                                ignoreQualifier);
                case TypeKind::Tag: {
                    auto oneTagTy = oneTy->asTagType();
                    auto otherTagTy = otherTy->asTagType();
                    return oneTagTy->kind() == otherTagTy->kind()
                            && oneTy->asTagType()->tag() == otherTy->asTagType()->tag();
                }
                case TypeKind::Void:
                    return treatVoidAsAny;
                case TypeKind::Qualified:
                    break;
                case TypeKind::Error:
                    break;
            }
            break;

        case TypeKind::Void:
            switch (otherTy->kind()) {
                case TypeKind::Array:
                case TypeKind::Basic:
                case TypeKind::Function:
                case TypeKind::Pointer:
                    return treatVoidAsAny;
                case TypeKind::TypedefName:
                    return typesAreCompatible(
                                oneTy,
                                otherTy->asTypedefNameType()->resolvedSynonymizedType(),
                                treatVoidAsAny,
                                ignoreQualifier);
                case TypeKind::Tag:
                    break;
                case TypeKind::Void:
                    return true;
                case TypeKind::Qualified:
                    return treatVoidAsAny;
                case TypeKind::Error:
                    break;
            }
            break;

        case TypeKind::Qualified:
            if (ignoreQualifier)
                return typesAreCompatible(
                            oneTy->asQualifiedType()->unqualifiedType(),
                            otherTy,
                            treatVoidAsAny,
                            ignoreQualifier);
            switch (otherTy->kind()) {
                case TypeKind::Array:
                case TypeKind::Basic:
                case TypeKind::Function:
                case TypeKind::Pointer:
                    break;
                case TypeKind::TypedefName:
                    return typesAreCompatible(
                                oneTy,
                                otherTy->asTypedefNameType()->resolvedSynonymizedType(),
                                treatVoidAsAny,
                                ignoreQualifier);
                case TypeKind::Tag:
                    break;
                case TypeKind::Void:
                    return treatVoidAsAny;
                case TypeKind::Qualified: {
                    auto oneQualTy = oneTy->asQualifiedType();
                    auto otherQualTy = otherTy->asQualifiedType();
                    if (oneQualTy->qualifiers() != otherQualTy->qualifiers())
                        return false;
                    return typesAreCompatible(
                                oneQualTy->unqualifiedType(),
                                otherQualTy->unqualifiedType(),
                                treatVoidAsAny,
                                ignoreQualifier);
                }
                case TypeKind::Error:
                    break;
            }
            break;

        case TypeKind::Error:
            break;
    }
    return false;
}

bool TypeChecker::isNULLPointerConstant(const SyntaxNode* node)
{
    return node->kind() == SyntaxKind::IntegerConstantExpression
            && node->asConstantExpression()->constantToken().kind()
                == SyntaxKind::IntegerConstantToken
            && node->asConstantExpression()->constantToken().lexeme()->valueText() == "0";
}

//-------------//
// Expressions //
//-------------//

SyntaxVisitor::Action TypeChecker::visitExpression(const SyntaxNode* node)
{
    VISIT(node);
    PSY_ASSERT_2(ty_, return Action::Quit);

    return Action::Skip;
}

SyntaxVisitor::Action TypeChecker::visitIdentifierName(const IdentifierNameSyntax* node)
{
    auto scope = semaModel_->scopeOf(node);
    PSY_ASSERT_2(scope, return Action::Quit);
    auto decl = scope->searchForDeclaration(
                identifierFrom(node),
                NameSpace::OrdinaryIdentifiers);
    if (!decl)
        ty_ = semaModel_->compilation()->canonicalErrorType();
    else {
        switch (decl->category()) {
            case DeclarationCategory::Member:
            case DeclarationCategory::Function:
            case DeclarationCategory::Object: {
                auto typeableDecl = MIXIN_TypeableDeclarationSymbol::from(decl);
                PSY_ASSERT_2(typeableDecl, return Action::Quit);
                ty_ = typeableDecl->type();
                break;
            }
            case DeclarationCategory::Type:
                ty_ = semaModel_->compilation()->canonicalErrorType();
                break;
        }
    }

    return Action::Skip;
}

SyntaxVisitor::Action TypeChecker::visitPredefinedName(
        const PredefinedNameSyntax* node)
{
    switch (node->identifierToken().kind()) {
        case SyntaxKind::Keyword___func__:
        case SyntaxKind::Keyword_ExtGNU___FUNCTION__:
        case SyntaxKind::Keyword_ExtGNU___PRETTY_FUNCTION__: {
            auto charTy = semaModel_->compilation()->canonicalBasicType(BasicTypeKind::Char);
            std::unique_ptr<QualifiedType> qualTy(new QualifiedType(charTy));
            qualTy->qualifyWithConst();
            auto ty = semaModel_->keepType(std::move(qualTy));
            ty_ = semaModel_->keepType(std::unique_ptr<ArrayType>(new ArrayType(ty)));
            break;
        }
        case SyntaxKind::Keyword_ExtGNU___printf__:
        case SyntaxKind::Keyword_ExtGNU___scanf__:
        case SyntaxKind::Keyword_ExtGNU___strftime__:
        case SyntaxKind::Keyword_ExtGNU___strfmon__:
            ty_ = semaModel_->compilation()->canonicalBasicType(BasicTypeKind::Int_S);
            break;
        default:
            PSY_ASSERT_1(false);
            return Action::Quit;
    }

    return Action::Skip;
}

namespace
{
template <size_t N>
BasicTypeKind selectTypeForValue(
        const PlatformOptions& platfOpts,
        unsigned long long v,
        BasicTypeKind (&candidates)[N])
{
    for (size_t i = 0; i < N - 1; ++i) {
        unsigned long long max;
        BasicTypeKind basicTyK = candidates[i];
        switch (basicTyK) {
            case BasicTypeKind::Char:
                max = platfOpts.maxValueOf(PlatformOptions::ArithmeticIntegerType::Char);
                break;
            case BasicTypeKind::Char_S:
                max = platfOpts.maxValueOf(PlatformOptions::ArithmeticIntegerType::Char_S);
                break;
            case BasicTypeKind::Char_U:
                max = platfOpts.maxValueOf(PlatformOptions::ArithmeticIntegerType::Char_U);
                break;
            case BasicTypeKind::Short_S:
                max = platfOpts.maxValueOf(PlatformOptions::ArithmeticIntegerType::Short_S);
                break;
            case BasicTypeKind::Short_U:
                max = platfOpts.maxValueOf(PlatformOptions::ArithmeticIntegerType::Short_U);
                break;
            case BasicTypeKind::Int_S:
                max = platfOpts.maxValueOf(PlatformOptions::ArithmeticIntegerType::Int_S);
                break;
            case BasicTypeKind::Int_U:
                max = platfOpts.maxValueOf(PlatformOptions::ArithmeticIntegerType::Int_U);
                break;
            case BasicTypeKind::Long_S:
                max = platfOpts.maxValueOf(PlatformOptions::ArithmeticIntegerType::Long_S);
                break;
            case BasicTypeKind::Long_U:
                max = platfOpts.maxValueOf(PlatformOptions::ArithmeticIntegerType::Long_U);
                break;
            case BasicTypeKind::LongLong_S:
                max = platfOpts.maxValueOf(PlatformOptions::ArithmeticIntegerType::LongLong_S);
                break;
            case BasicTypeKind::LongLong_U:
                max = platfOpts.maxValueOf(PlatformOptions::ArithmeticIntegerType::LongLong_U);
                break;
            case BasicTypeKind::Bool:
                max = platfOpts.maxValueOf(PlatformOptions::ArithmeticIntegerType::Bool);
                break;
            default:
                PSY_ASSERT_1(false);
                return BasicTypeKind::Int_S;
        }
        if (v <= max)
            return basicTyK;
    }
    return candidates[N-1];
}
} // anonymous

SyntaxVisitor::Action TypeChecker::visitConstantExpression(const ConstantExpressionSyntax* node)
{
    auto constantTk = node->constantToken().lexeme();
    switch (constantTk->kind()) {
        case Lexeme::LexemeKind::IntegerConstant: {
            auto intTk = constantTk->asIntegerConstant();
            unsigned long long val;
            try {
                val = std::stoull(intTk->valueText(), nullptr, 0);
            }
            catch (const std::exception&) {
                return Action::Quit;
            }
            BasicTypeKind basicTyK;
            switch (intTk->representationSuffix()) {
                case IntegerConstant::RepresentationSuffix::None:
                    if (intTk->isOctalOrHexadecimal()) {
                        BasicTypeKind kinds[] = {BasicTypeKind::Int_S,
                                                 BasicTypeKind::Int_U,
                                                 BasicTypeKind::Long_S,
                                                 BasicTypeKind::Long_U,
                                                 BasicTypeKind::LongLong_S,
                                                 BasicTypeKind::LongLong_U};
                        basicTyK = selectTypeForValue(
                                    semaModel_->compilation()->platformOptions(),
                                    val,
                                    kinds);
                    } else {
                        BasicTypeKind kinds[] = {BasicTypeKind::Int_S,
                                                 BasicTypeKind::Long_S,
                                                 BasicTypeKind::LongLong_S};
                        basicTyK = selectTypeForValue(
                                    semaModel_->compilation()->platformOptions(),
                                    val,
                                    kinds);
                    }
                    break;

                case IntegerConstant::RepresentationSuffix::uOrU: {
                    BasicTypeKind kinds[] = {BasicTypeKind::Int_U,
                                             BasicTypeKind::Long_U,
                                             BasicTypeKind::LongLong_U};
                    basicTyK = selectTypeForValue(
                                semaModel_->compilation()->platformOptions(),
                                val,
                                kinds);
                    break;
                }
                case IntegerConstant::RepresentationSuffix::lOrL:
                    if (intTk->isOctalOrHexadecimal()) {
                        BasicTypeKind kinds[] = {BasicTypeKind::Long_S,
                                                 BasicTypeKind::Long_U,
                                                 BasicTypeKind::LongLong_S,
                                                 BasicTypeKind::LongLong_U};
                        basicTyK = selectTypeForValue(
                                    semaModel_->compilation()->platformOptions(),
                                    val,
                                    kinds);
                    } else {
                        BasicTypeKind kinds[] = {BasicTypeKind::Long_S, BasicTypeKind::LongLong_S};
                        basicTyK = selectTypeForValue(
                                    semaModel_->compilation()->platformOptions(),
                                    val,
                                    kinds);
                    }
                    break;

                case IntegerConstant::RepresentationSuffix::lOrLAnduOrU: {
                    BasicTypeKind kinds[] = {BasicTypeKind::Long_U, BasicTypeKind::LongLong_U};
                    basicTyK = selectTypeForValue(
                                semaModel_->compilation()->platformOptions(),
                                val,
                                kinds);
                    break;
                }
                case IntegerConstant::RepresentationSuffix::llOrLL:
                    if (intTk->isOctalOrHexadecimal()) {
                        BasicTypeKind kinds[] = {BasicTypeKind::LongLong_S, BasicTypeKind::LongLong_U};
                        basicTyK = selectTypeForValue(
                                    semaModel_->compilation()->platformOptions(),
                                    val,
                                    kinds);
                    } else {
                        BasicTypeKind kinds[] = {BasicTypeKind::LongLong_S};
                        basicTyK = selectTypeForValue(
                                    semaModel_->compilation()->platformOptions(),
                                    val,
                                    kinds);
                    }
                    break;
                case IntegerConstant::RepresentationSuffix::llOrLLAnduOrU:
                    BasicTypeKind kinds[] = {BasicTypeKind::LongLong_U};
                    basicTyK = selectTypeForValue(
                                semaModel_->compilation()->platformOptions(),
                                val,
                                kinds);
                    break;
            }
            ty_ = semaModel_->compilation()->canonicalBasicType(basicTyK);
            break;
        }
        case Lexeme::LexemeKind::FloatingConstant: {
            auto floatTk = constantTk->asFloatingConstant();
            BasicTypeKind basicTyK;
            switch (floatTk->representationSuffix()) {
                case FloatingConstant::RepresentationSuffix::fOrF:
                    basicTyK = BasicTypeKind::Float;
                    break;
                case FloatingConstant::RepresentationSuffix::lOrL:
                    basicTyK = BasicTypeKind::LongDouble;
                    break;
                case FloatingConstant::RepresentationSuffix::None:
                    basicTyK = BasicTypeKind::Double;
                    break;
            }
            ty_ = semaModel_->compilation()->canonicalBasicType(basicTyK);
            break;
        }
        case Lexeme::LexemeKind::CharacterConstant:
            switch (constantTk->asCharacterConstant()->encodingPrefix()) {
                case CharacterConstant::EncodingPrefix::None:
                    ty_ = semaModel_->compilation()->canonicalBasicType(BasicTypeKind::Char_U);
                    break;
                case CharacterConstant::EncodingPrefix::u:
                    ty_ = char16Ty_;
                    break;
                case CharacterConstant::EncodingPrefix::U:
                    ty_ = char32Ty_;
                case CharacterConstant::EncodingPrefix::L:
                    ty_ = wcharTy_;
                    break;
            }
            break;

        case Lexeme::LexemeKind::ImaginaryIntegerConstant:
            ty_ = semaModel_->compilation()->canonicalBasicType(BasicTypeKind::Int_S);
            break;
        case Lexeme::LexemeKind::ImaginaryFloatingConstant:
            ty_ = semaModel_->compilation()->canonicalBasicType(BasicTypeKind::Double);
            break;
        case Lexeme::LexemeKind::StringLiteral:
            ty_ = typeOfStringLiteral(constantTk->asStringLiteral()->encodingPrefix());
            break;
        default:
            PSY_ASSERT_1(false);
            return Action::Quit;
    }

    return Action::Skip;
}

SyntaxVisitor::Action TypeChecker::visitStringLiteralExpression(
        const StringLiteralExpressionSyntax* node)
{
    ty_ = typeOfStringLiteral(literalFrom(node)->encodingPrefix());

    return Action::Skip;
}

const Type* TypeChecker::typeOfStringLiteral(StringLiteral::EncodingPrefix encodingSuffix)
{
    switch (encodingSuffix) {
        case StringLiteral::EncodingPrefix::None:
            return strLitTy_;
        case StringLiteral::EncodingPrefix::u8:
            return u8StrLitTy_;
        case StringLiteral::EncodingPrefix::u:
            return uStrLitTy_;
        case StringLiteral::EncodingPrefix::U:
            return UStrLitTy_;
        case StringLiteral::EncodingPrefix::L:
            return LStrLitTy_;
        default:
            PSY_ASSERT_1(false);
            return nullptr;
    }
}

SyntaxVisitor::Action TypeChecker::visitParenthesizedExpression(
        const ParenthesizedExpressionSyntax* node)
{
    VISIT_EXPR(node->expression());

    return Action::Skip;
}

SyntaxVisitor::Action TypeChecker::visitGenericSelectionExpression(
        const GenericSelectionExpressionSyntax* node)
{
    VISIT_EXPR(node->expression());

    return Action::Skip;
}

SyntaxVisitor::Action TypeChecker::visitGenericAssociation(const GenericAssociationSyntax*)
{
    return Action::Skip;
}

SyntaxVisitor::Action TypeChecker::visitExtGNU_EnclosedCompoundStatementExpression(
        const ExtGNU_EnclosedCompoundStatementExpressionSyntax* node)
{
    visit(node->statement());

    return Action::Skip;
}

SyntaxVisitor::Action TypeChecker::visitExtGNU_ComplexValuedExpression(
        const ExtGNU_ComplexValuedExpressionSyntax* node)
{
    VISIT_EXPR(node->expression());

    return Action::Skip;
}

/* Operations */

SyntaxVisitor::Action TypeChecker::visitPrefixUnaryExpression(
        const PrefixUnaryExpressionSyntax* node)
{
    VISIT_EXPR(node->expression());

    switch (node->operatorToken().kind()) {
        case SyntaxKind::PlusPlusToken:
        case SyntaxKind::MinusMinusToken: {
            auto ty = unqualifiedAndResolved(ty_);
            if (!(isRealType(ty) || ty->kind() == TypeKind::Pointer)) {
                diagReporter_.InvalidOperator(node->operatorToken());
                return Action::Quit;
            }
            break;
        }
        case SyntaxKind::AmpersandToken: {
            std::unique_ptr<PointerType> ptrTy(new PointerType(ty_));
            ty_ = semaModel_->keepType(std::move(ptrTy));
            break;
        }
        case SyntaxKind::AsteriskToken: {
            auto ty = unqualifiedAndResolved(ty_);
            switch (ty->kind()) {
                case TypeKind::Array:
                    ty_ = ty->asArrayType()->elementType();
                    break;
                case TypeKind::Pointer:
                    ty_ = ty->asPointerType()->referencedType();
                    break;
                case TypeKind::Function:
                    ty_ = ty;
                    break;
                default:
                    diagReporter_.ExpectedExpressionOfPointerType(node->lastToken());
                    return Action::Quit;
            }
            break;
        }
        case SyntaxKind::PlusToken:
        case SyntaxKind::MinusToken: {
            auto ty = unqualifiedAndResolved(ty_);
            if (!isArithmeticType(ty)) {
                diagReporter_.ExpectedExpressionOfArithmeticType(node->operatorToken());
                return Action::Quit;
            }
            break;
        }
        case SyntaxKind::TildeToken: {
            auto ty = unqualifiedAndResolved(ty_);
            if (!isIntegerType(ty)) {
                diagReporter_.ExpectedExpressionOfIntegerType(node->operatorToken());
                return Action::Quit;
            }
            break;
        }
        case SyntaxKind::ExclamationToken: {
            auto ty = unqualifiedAndResolved(ty_);
            if (!isScalarType(ty)) {
                diagReporter_.ExpectedExpressionOfScalarType(node->operatorToken());
                return Action::Quit;
            }
            break;
        }
        case SyntaxKind::AmpersandAmpersandToken:
            break;

        default:
            PSY_ASSERT_1(false);
            return Action::Quit;
    }

    return Action::Skip;
}

SyntaxVisitor::Action TypeChecker::visitPostfixUnaryExpression(
        const PostfixUnaryExpressionSyntax* node)
{
    VISIT_EXPR(node->expression());

    switch (node->operatorToken().kind()) {
        case SyntaxKind::PlusPlusToken:
        case SyntaxKind::MinusMinusToken: {
            auto ty = unqualifiedAndResolved(ty_);
            if (!(isRealType(ty) || ty->kind() == TypeKind::Pointer)) {
                diagReporter_.InvalidOperator(node->operatorToken());
                return Action::Quit;
            }
            break;
        }
        default:
            PSY_ASSERT_1(false);
            return Action::Quit;
    }

    return Action::Skip;
}

SyntaxVisitor::Action TypeChecker::visitMemberAccessExpression(
        const MemberAccessExpressionSyntax* node)
{
    VISIT_EXPR(node->expression());
    auto ty = unqualifiedAndResolved(ty_);

    const TagType* tagTy = nullptr;
    switch (ty->kind()) {
        case TypeKind::Tag:
            if (node->kind() != SyntaxKind::DirectMemberAccessExpression) {
                diagReporter_.InvalidOperator(node->operatorToken());
                return Action::Quit;
            }
            tagTy = ty->asTagType();
            break;

        case TypeKind::Pointer: {
            if (node->kind() != SyntaxKind::IndirectMemberAccessExpression) {
                diagReporter_.InvalidOperator(node->operatorToken());
                return Action::Quit;
            }
            auto refedTy = ty->asPointerType()->referencedType();
            refedTy = unqualifiedAndResolved(refedTy);
            if (refedTy->kind() == TypeKind::Tag)
                tagTy = refedTy->asTagType();
            break;
        }

        default:
            break;
    }

    if (!tagTy) {
        switch (node->kind()) {
            case SyntaxKind::DirectMemberAccessExpression:
                diagReporter_.ExpectedExpressionOfStructOrUnionType(
                    node->expression()->lastToken());
                break;
            case SyntaxKind::IndirectMemberAccessExpression:
                diagReporter_.ExpectedExpressionOfPointerToStructOrUnionType(
                    node->expression()->lastToken());
                break;
            default:
                PSY_ASSERT_1(false);
                break;
        }
        return Action::Quit;
    }

    auto tagTyDecl = tagTy->declaration();
    if (!tagTyDecl)
        return Action::Quit;
    auto membDecl = tagTyDecl->member(identifierFrom(node->memberName()));
    if (!membDecl) {
        diagReporter_.UnknownMemberOfTag(node->memberName()->lastToken());
        return Action::Quit;
    }
    ty_ = membDecl->type();

    return Action::Skip;
}

SyntaxVisitor::Action TypeChecker::visitArraySubscriptExpression(
        const ArraySubscriptExpressionSyntax* node)
{
    VISIT_EXPR(node->argument());
    auto argTy = unqualifiedAndResolved(ty_);
    if (!isIntegerType(argTy)) {
        diagReporter_.ExpectedExpressionOfIntegerType(node->argument()->lastToken());
        return Action::Quit;
    }

    VISIT_EXPR(node->expression());
    auto exprTy = unqualifiedAndResolved(ty_);
    switch (exprTy->kind()) {
        case TypeKind::Array:
            ty_ = exprTy->asArrayType()->elementType();
            break;
        case TypeKind::Pointer:
            ty_ = exprTy->asPointerType()->referencedType();
            break;
        default:
            diagReporter_.ExpectedExpressionOfPointerOrArrayType(
                        node->expression()->lastToken());
            return Action::Quit;
    }

    return Action::Skip;
}

SyntaxVisitor::Action TypeChecker::visitTypeTraitExpression(
        const TypeTraitExpressionSyntax* node)
{
    VISIT_EXPR(node->tyReference());

    switch (node->operatorToken().kind()) {
        case SyntaxKind::Keyword_sizeof:
        case SyntaxKind::Keyword__Alignof:
            if (ty_->kind() == TypeKind::Function) {
                diagReporter_.InvalidOperator(node->operatorToken());
                return Action::Quit;
            }
            break;
        default:
            PSY_ASSERT_1(false);
            return Action::Quit;
    }
    ty_ = semaModel_->compilation()->canonicalBasicType(BasicTypeKind::Int_S);

    return Action::Skip;
}

SyntaxVisitor::Action TypeChecker::visitCastExpression(
        const CastExpressionSyntax* node)
{
    VISIT_EXPR(node->typeName());

    return Action::Skip;
}

SyntaxVisitor::Action TypeChecker::visitCallExpression(
        const CallExpressionSyntax* node)
{
    VISIT_EXPR(node->expression());
    auto exprTy = unqualifiedAndResolved(ty_);

    const FunctionType* funcTy = nullptr;
    switch (exprTy->kind()) {
        case TypeKind::Function:
            funcTy = exprTy->asFunctionType();
            break;
        case TypeKind::Pointer: {
            auto refedTy = unqualifiedAndResolved(exprTy->asPointerType()->referencedType());
            while (refedTy->kind() == TypeKind::Pointer)
                refedTy = unqualifiedAndResolved(refedTy->asPointerType()->referencedType());
            if (refedTy->kind() == TypeKind::Function)
                funcTy = refedTy->asFunctionType();
            break;
        }
        default:
            break;
    }

    if (!funcTy) {
        diagReporter_.ExpectedExpressionOfFunctionOrFunctionPointerType(
            node->expression()->lastToken());
        return Action::Skip;
    }

    std::vector<const Type*> argTys;
    for (auto iter = node->arguments(); iter; iter = iter->next) {
        VISIT_EXPR(iter->value);
        argTys.push_back(ty_);
    }
    const auto& parmTys = funcTy->parameterTypes();
    if (argTys.size() < parmTys.size()) {
        diagReporter_.TooFewArgumentsToFunctionCall(node->expression()->lastToken());
        return Action::Quit;
    }

    return Action::Skip;
}

SyntaxVisitor::Action TypeChecker::visitVAArgumentExpression(const VAArgumentExpressionSyntax*) { return Action::Skip; }
SyntaxVisitor::Action TypeChecker::visitOffsetOfExpression(const OffsetOfExpressionSyntax*) { return Action::Skip; }
SyntaxVisitor::Action TypeChecker::visitCompoundLiteralExpression(const CompoundLiteralExpressionSyntax*) { return Action::Skip; }

SyntaxVisitor::Action TypeChecker::visitBinaryExpression(const BinaryExpressionSyntax* node)
{
    VISIT_EXPR(node->left());
    auto leftTy = unqualifiedAndResolved(ty_);
    VISIT_EXPR(node->right());
    auto rightTy = unqualifiedAndResolved(ty_);

    switch (node->operatorToken().kind()) {
        case SyntaxKind::AsteriskToken:
        case SyntaxKind::SlashToken:
            return visitBinaryExpression_MultiplicationOrDivision(node, leftTy, rightTy);
        case SyntaxKind::PercentToken:
            return visitBinaryExpression_Remainder(node, leftTy, rightTy);
        case SyntaxKind::PlusToken:
            return visitBinaryExpression_Addition(node, leftTy, rightTy);
        case SyntaxKind::MinusToken:
            return visitBinaryExpression_Subtraction(node, leftTy, rightTy);
        case SyntaxKind::LessThanLessThanToken:
        case SyntaxKind::GreaterThanGreaterThanToken:
            return visitBinaryExpression_BitwiseShift(node, leftTy, rightTy);
        case SyntaxKind::GreaterThanToken:
        case SyntaxKind::LessThanToken:
        case SyntaxKind::LessThanEqualsToken:
        case SyntaxKind::GreaterThanEqualsToken:
            return visitBinaryExpression_Relational(node, leftTy, rightTy);
        case SyntaxKind::EqualsEqualsToken:
        case SyntaxKind::ExclamationEqualsToken:
            return visitBinaryExpression_Equality(node, leftTy, rightTy);
        case SyntaxKind::AmpersandToken:
        case SyntaxKind::CaretToken:
        case SyntaxKind::BarToken:
            return visitBinaryExpression_Bitwise(node, leftTy, rightTy);
        case SyntaxKind::AmpersandAmpersandToken:
        case SyntaxKind::BarBarToken:
            return visitBinaryExpression_Logical(node, leftTy, rightTy);
        default:
            PSY_ASSERT_1(false);
            return Action::Quit;
    }
}

template <class BinaryLikeExprNodeT>
SyntaxVisitor::Action TypeChecker::visitBinaryExpression_MultiplicationOrDivision(
        const BinaryLikeExprNodeT* node,
        const Type* leftTy,
        const Type* rightTy)
{
    if (!(satisfyArithmeticTypeConstraint(leftTy, node->left())
            && satisfyArithmeticTypeConstraint(rightTy, node->right()))) {
        return Action::Quit;
    }
    ty_ = determineCommonRealType(leftTy->asBasicType(), rightTy->asBasicType());

    return Action::Skip;
}

template <class BinaryLikeExprNodeT>
SyntaxVisitor::Action TypeChecker::visitBinaryExpression_Remainder(
        const BinaryLikeExprNodeT* node,
        const Type* leftTy,
        const Type* rightTy)
{
    if (!(satisfyIntegerTypeConstraint(leftTy, node->left())
            && satisfyIntegerTypeConstraint(rightTy, node->right()))) {
        return Action::Quit;
    }
    ty_ = determineCommonRealType(leftTy->asBasicType(), rightTy->asBasicType());

    return Action::Skip;
}

template <class BinaryLikeExprNodeT>
SyntaxVisitor::Action TypeChecker::visitBinaryExpression_Addition(
        const BinaryLikeExprNodeT* node,
        const Type* leftTy,
        const Type* rightTy)
{
    if (isArithmeticType(leftTy) && isArithmeticType(rightTy)) {
        ty_ = determineCommonRealType(leftTy->asBasicType(), rightTy->asBasicType());
    } else if (leftTy->kind() == TypeKind::Pointer
               && isIntegerType(rightTy)) {
        ty_ = leftTy;
    } else if (rightTy->kind() == TypeKind::Pointer
                && isIntegerType(leftTy)) {
        ty_ = rightTy;
    } else {
        diagReporter_.InvalidOperator(node->operatorToken());
        return Action::Quit;
    }

    return Action::Skip;
}

template <class BinaryLikeExprNodeT>
SyntaxVisitor::Action TypeChecker::visitBinaryExpression_Subtraction(
        const BinaryLikeExprNodeT* node,
        const Type* leftTy,
        const Type* rightTy)
{
    if (isArithmeticType(leftTy) && isArithmeticType(rightTy)) {
        ty_ = determineCommonRealType(leftTy->asBasicType(), rightTy->asBasicType());
    } else if (leftTy->kind() == TypeKind::Pointer) {
        if (rightTy->kind() == TypeKind::Pointer) {
            ty_ = ptrdiffTy_;
        } else if (isIntegerType(rightTy)) {
            ty_ = leftTy;
        } else {
            diagReporter_.InvalidOperator(node->operatorToken());
            return Action::Quit;
        }
    } else {
        diagReporter_.InvalidOperator(node->operatorToken());
        return Action::Quit;
    }

    return Action::Skip;
}

template <class BinaryLikeExprNodeT>
SyntaxVisitor::Action TypeChecker::visitBinaryExpression_BitwiseShift(
        const BinaryLikeExprNodeT* node,
        const Type* leftTy,
        const Type* rightTy)
{
    if (!(satisfyIntegerTypeConstraint(leftTy, node->left())
            && satisfyIntegerTypeConstraint(rightTy, node->right()))) {
        return Action::Quit;
    }
    auto promoTyK = performIntegerPromotion(leftTy->asBasicType()->kind());
    ty_ = semaModel_->compilation()->canonicalBasicType(promoTyK);

    return Action::Skip;
}

template <class BinaryLikeExprNodeT>
SyntaxVisitor::Action TypeChecker::visitBinaryExpression_Relational(
        const BinaryLikeExprNodeT* node,
        const Type* leftTy,
        const Type* rightTy)
{
    if (!((isRealType(leftTy) && isRealType(rightTy))
            || (leftTy->kind() == TypeKind::Pointer
                && rightTy->kind() == TypeKind::Pointer
                && typesAreCompatible(
                    leftTy->asPointerType()->referencedType(),
                    rightTy->asPointerType()->referencedType(),
                    true,
                    true)))) {
        diagReporter_.InvalidOperator(node->operatorToken());
        return Action::Quit;
    }
    ty_ = semaModel_->compilation()->canonicalBasicType(BasicTypeKind::Int_S);

    return Action::Skip;
}

template <class BinaryLikeExprNodeT>
SyntaxVisitor::Action TypeChecker::visitBinaryExpression_Equality(
        const BinaryLikeExprNodeT* node,
        const Type* leftTy,
        const Type* rightTy)
{
    if (!((isArithmeticType(leftTy) && isArithmeticType(rightTy))
            || (leftTy->kind() == TypeKind::Pointer
                && ((rightTy->kind() == TypeKind::Pointer
                        && typesAreCompatible(
                                leftTy->asPointerType()->referencedType(),
                                rightTy->asPointerType()->referencedType(),
                                true,
                                true))
                    || (rightTy->kind() == TypeKind::Basic
                        && rightTy->asBasicType()->kind() == BasicTypeKind::Int_S
                        && isNULLPointerConstant(node->right()))))
            || (leftTy->kind() == TypeKind::Basic
                    && leftTy->asBasicType()->kind() == BasicTypeKind::Int_S
                    && isNULLPointerConstant(node->left())
                && rightTy->kind() == TypeKind::Pointer))) {
        diagReporter_.InvalidOperator(node->operatorToken());
        return Action::Quit;
    }
    ty_ = semaModel_->compilation()->canonicalBasicType(BasicTypeKind::Int_S);

    return Action::Skip;
}

template <class BinaryLikeExprNodeT>
SyntaxVisitor::Action TypeChecker::visitBinaryExpression_Bitwise(
        const BinaryLikeExprNodeT* node,
        const Type* leftTy,
        const Type* rightTy)
{
    return Action::Skip;
}

template <class BinaryLikeExprNodeT>
SyntaxVisitor::Action TypeChecker::visitBinaryExpression_Logical(
        const BinaryLikeExprNodeT* node,
        const Type* leftTy,
        const Type* rightTy)
{
    return Action::Skip;
}

SyntaxVisitor::Action TypeChecker::visitConditionalExpression(const ConditionalExpressionSyntax*) { return Action::Skip; }

SyntaxVisitor::Action TypeChecker::visitAssignmentExpression(
        const AssignmentExpressionSyntax* node)
{
    VISIT_EXPR(node->left());
    if (!isAssignable(node->left(), ty_))
        return Action::Quit;
    auto leftTy = unqualifiedAndResolved(ty_);
    VISIT_EXPR(node->right());
    auto rightTy = unqualifiedAndResolved(ty_);

    switch (node->operatorToken().kind()) {
        case SyntaxKind::EqualsToken:
            return visitAssignmentExpression_Simple(node, leftTy, rightTy);
        case SyntaxKind::AsteriskEqualsToken:
        case SyntaxKind::SlashEqualsToken:
            return visitBinaryExpression_MultiplicationOrDivision(node, leftTy, rightTy);
        case SyntaxKind::PercentEqualsToken:
            return visitBinaryExpression_Remainder(node, leftTy, rightTy);
        case SyntaxKind::PlusEqualsToken:
            return visitBinaryExpression_Addition(node, leftTy, rightTy);
        case SyntaxKind::MinusEqualsToken:
            return visitBinaryExpression_Subtraction(node, leftTy, rightTy);
        case SyntaxKind::LessThanEqualsToken:
        case SyntaxKind::GreaterThanEqualsToken:
            return visitBinaryExpression_BitwiseShift(node, leftTy, rightTy);
        case SyntaxKind::AmpersandEqualsToken:
        case SyntaxKind::ExclamationEqualsToken:
        case SyntaxKind::CaretEqualsToken:
            return visitBinaryExpression_Bitwise(node, leftTy, rightTy);
        default:
            PSY_ASSERT_1(false);
            return Action::Quit;
    }

    return Action::Skip;
}

SyntaxVisitor::Action TypeChecker:: visitAssignmentExpression_Simple(
        const AssignmentExpressionSyntax* node,
        const Type* leftTy,
        const Type* rightTy)
{
    if (!((isArithmeticType(leftTy) && isArithmeticType(rightTy))
            || (isStructureOrUnionType(leftTy)
                && typesAreCompatible(leftTy, rightTy, false, false))
            || (leftTy->kind() == TypeKind::Pointer
                && ((rightTy->kind() == TypeKind::Pointer
                        && typesAreCompatible(
                                leftTy->asPointerType()->referencedType(),
                                rightTy->asPointerType()->referencedType(),
                                true,
                                true))
                    || (rightTy->kind() == TypeKind::Basic
                        && rightTy->asBasicType()->kind() == BasicTypeKind::Int_S
                        && isNULLPointerConstant(node->right()))))
            )) {
        diagReporter_.IncompatibleTypesInAssignment(node->operatorToken());
        return Action::Quit;
    }
    ty_ = leftTy;

    return Action::Skip;
}

SyntaxVisitor::Action TypeChecker::visitSequencingExpression(const SequencingExpressionSyntax*) { return Action::Skip; }
SyntaxVisitor::Action TypeChecker::visitExtGNU_ChooseExpression(const ExtGNU_ChooseExpressionSyntax*) { return Action::Skip; }

//------------//
// Statements //
//------------//

SyntaxVisitor::Action TypeChecker::visitStatement(const StatementSyntax* node)
{
    // Ignore action of default visit to continue type checking
    // at statement level despite of errors at expression level.
    visitChildNodes(node);

    return Action::Skip;
}

SyntaxVisitor::Action TypeChecker::visitDeclarationStatement(const DeclarationStatementSyntax* node)
{
    return visitStatement(node);
}

SyntaxVisitor::Action TypeChecker::visitExpressionStatement(const ExpressionStatementSyntax* node)
{
    return visitStatement(node);
}

SyntaxVisitor::Action TypeChecker::visitReturnStatement(const ReturnStatementSyntax* node)
{
    return visitStatement(node);
}

//--------//
// Common //
//--------//

SyntaxVisitor::Action TypeChecker::visitTypeName(const TypeNameSyntax* node)
{
    auto decl = semaModel_->declarationBy(node->declarator());
    PSY_ASSERT_2(decl, return Action::Quit);
    switch (decl->category()) {
        case DeclarationCategory::Function:
            ty_ = decl->asFunctionDeclaration()->type();
            break;
        case DeclarationCategory::Object:
            ty_ = decl->asObjectDeclaration()->type();
            break;
        default:
            PSY_ASSERT_1(false);
            return Action::Quit;
    }

    return Action::Skip;
}

#undef VISIT
