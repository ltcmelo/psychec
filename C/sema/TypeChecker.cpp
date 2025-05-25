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
#include "syntax/SyntaxToken.h"
#include "syntax/SyntaxUtilities.h"
#include "types/Type_ALL.h"

#include "../common/infra/Assertions.h"

#include <cstdint>

using namespace psy;
using namespace C;

#define VISIT(NODE) \
    do { \
        if (visit(NODE) == Action::Quit) return Action::Quit; \
        PSY_ASSERT_2(ty_, return Action::Quit); \
    } while (0)

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

const Type* TypeChecker::resolved(const Type* ty)
{
    if (ty->kind() == TypeKind::TypedefName)
        return ty->asTypedefNameType()->resolvedSynonymizedType();
    return ty;
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

bool TypeChecker::isAssignableType(const Type* ty, const SyntaxNode* node)
{
    switch (ty->kind()) {
        case TypeKind::Qualified:
            diagReporter_.CannotAssignToExpressionOfConstQualifiedType(node->lastToken());
            return false;
        case TypeKind::TypedefName:
            return isAssignableType(ty->asTypedefNameType()->resolvedSynonymizedType(), node);
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
                if (!isAssignableType(membTy, node))
                    return false;
            }
            return true;
        }
        default:
            return true;
    }
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

void TypeChecker::createTypeInfo(
        const SyntaxNode* node,
        const Type* ty,
        TypeInfo::TypeOrigin tyOrig)
{
    ty_ = ty;
    TypeInfo tyInfo(ty_, tyOrig);
    semaModel_->setTypeInfoOf(node, std::move(tyInfo));
}

SyntaxVisitor::Action TypeChecker::typeChecked(
        const ExpressionSyntax* node,
        const Type* ty,
        TypeInfo::TypeOrigin tyOrig)
{
    PSY_ASSERT_2(ty, return Action::Quit);
    createTypeInfo(node, ty, tyOrig);
    return Action::Skip;
}

SyntaxVisitor::Action TypeChecker::typeCheckError(const SyntaxNode* node)
{
    createTypeInfo(node,
             semaModel_->compilation()->canonicalErrorType(),
             TypeInfo::TypeOrigin::Error);
    return Action::Quit;
}

//--------------//
// Declarations //
//--------------//

void TypeChecker::determineParameterListForm(FunctionDeclarationSymbol* func)
{
    PSY_ASSERT_2(func->type(), return);
    PSY_ASSERT_2(func->type()->kind() == TypeKind::Function, return);

    auto funcTy = func->type()->asFunctionType();
    auto parmTys = funcTy->parameterTypes();
    if (parmTys.size() == 0)
        funcTy->setParameterListForm(
            FunctionType::ParameterListForm::Unspecified);
    else {
        if (parmTys.size() == 1) {
            auto resolvedParmTy = resolved(parmTys[0]);
            if (resolvedParmTy && resolvedParmTy->kind() == TypeKind::Void)
                funcTy->setParameterListForm(
                    FunctionType::ParameterListForm::SpecifiedAsEmpty);
            else
                funcTy->setParameterListForm(
                    FunctionType::ParameterListForm::NonEmpty);
        }
        else
            funcTy->setParameterListForm(
                FunctionType::ParameterListForm::NonEmpty);
    }
}

SyntaxVisitor::Action TypeChecker::visitVariableAndOrFunctionDeclaration(
        const VariableAndOrFunctionDeclarationSyntax* node)
{
    auto decls = semaModel_->variableAndOrFunctionsFor(node);
    for (auto decl : decls) {
        PSY_ASSERT_2(decl, continue);
        switch (decl->kind()) {
            case SymbolKind::FunctionDeclaration:
                determineParameterListForm(decl->asFunctionDeclaration());
                break;
            default:
                break;
        }
    }

    return Action::Visit;
}

SyntaxVisitor::Action TypeChecker::visitFunctionDefinition(
        const FunctionDefinitionSyntax* node)
{
    auto func = semaModel_->functionFor(node);
    PSY_ASSERT_2(func, return Action::Quit);
    determineParameterListForm(func);

    return Action::Visit;
}

SyntaxVisitor::Action TypeChecker::visitExtGNU_Attribute(const ExtGNU_AttributeSyntax*)
{
    return Action::Quit;
}

//-------------//
// Expressions //
//-------------//

SyntaxVisitor::Action TypeChecker::visitIdentifierName(const IdentifierNameSyntax* node)
{
    auto scope = semaModel_->scopeOf(node);
    PSY_ASSERT_2(scope, return Action::Quit);
    auto decl = scope->searchForDeclaration(
                identifierFrom(node),
                NameSpace::OrdinaryIdentifiers);
    if (!decl)
        return typeCheckError(node);

    const Type* ty = nullptr;
    switch (decl->category()) {
        case DeclarationCategory::Member:
        case DeclarationCategory::Function:
        case DeclarationCategory::Object: {
            auto typeableDecl = MIXIN_TypeableDeclarationSymbol::from(decl);
            PSY_ASSERT_2(typeableDecl, return Action::Quit);
            ty = typeableDecl->type();
            break;
        }
        case DeclarationCategory::Type:
            return typeCheckError(node);
    }
    return typeChecked(node, ty);
}

SyntaxVisitor::Action TypeChecker::visitPredefinedName(
        const PredefinedNameSyntax* node)
{
    const Type* ty = nullptr;
    switch (node->identifierToken().kind()) {
        case SyntaxKind::Keyword___func__:
        case SyntaxKind::Keyword_ExtGNU___FUNCTION__:
        case SyntaxKind::Keyword_ExtGNU___PRETTY_FUNCTION__: {
            auto charTy = semaModel_->compilation()->canonicalBasicType(BasicTypeKind::Char);
            std::unique_ptr<QualifiedType> qualTy(new QualifiedType(charTy));
            qualTy->qualifyWithConst();
            auto qualTy_RAW = semaModel_->keepType(std::move(qualTy));
            ty = semaModel_->keepType(std::unique_ptr<ArrayType>(new ArrayType(qualTy_RAW)));
            break;
        }
        case SyntaxKind::Keyword_ExtGNU___printf__:
        case SyntaxKind::Keyword_ExtGNU___scanf__:
        case SyntaxKind::Keyword_ExtGNU___strftime__:
        case SyntaxKind::Keyword_ExtGNU___strfmon__:
            ty = semaModel_->compilation()->canonicalBasicType(BasicTypeKind::Int_S);
            break;
        default:
            PSY_ASSERT_1(false);
            return Action::Quit;
    }
    return typeChecked(node, ty);
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
    const Type* ty = nullptr;
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
                    }
                    else {
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
                    }
                    else {
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
                    }
                    else {
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
            ty =  semaModel_->compilation()->canonicalBasicType(basicTyK);
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
            ty = semaModel_->compilation()->canonicalBasicType(basicTyK);
            break;
        }
        case Lexeme::LexemeKind::CharacterConstant:
            switch (constantTk->asCharacterConstant()->encodingPrefix()) {
                case CharacterConstant::EncodingPrefix::None:
                    ty = semaModel_->compilation()->canonicalBasicType(BasicTypeKind::Char_U);
                    break;
                case CharacterConstant::EncodingPrefix::u:
                    ty = char16Ty_;
                    break;
                case CharacterConstant::EncodingPrefix::U:
                    ty = char32Ty_;
                case CharacterConstant::EncodingPrefix::L:
                    ty = wcharTy_;
                    break;
            }
            break;

        case Lexeme::LexemeKind::ImaginaryIntegerConstant:
            ty = semaModel_->compilation()->canonicalBasicType(BasicTypeKind::Int_S);
            break;
        case Lexeme::LexemeKind::ImaginaryFloatingConstant:
            ty = semaModel_->compilation()->canonicalBasicType(BasicTypeKind::Double);
            break;
        case Lexeme::LexemeKind::StringLiteral:
            ty = typeOfStringLiteral(constantTk->asStringLiteral()->encodingPrefix());
            break;
        default:
            PSY_ASSERT_1(false);
            return Action::Quit;
    }
    return typeChecked(node, ty);
}

SyntaxVisitor::Action TypeChecker::visitStringLiteralExpression(
        const StringLiteralExpressionSyntax* node)
{
    return typeChecked(
                node,
                typeOfStringLiteral(literalFrom(node)->encodingPrefix()));
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
    VISIT(node->expression());

    return typeChecked(node, ty_);
}

SyntaxVisitor::Action TypeChecker::visitGenericSelectionExpression(
        const GenericSelectionExpressionSyntax* node)
{
    VISIT(node->expression());

    return typeChecked(node, ty_);
}

SyntaxVisitor::Action TypeChecker::visitGenericAssociation(const GenericAssociationSyntax*)
{
    return Action::Skip;
}

SyntaxVisitor::Action TypeChecker::visitExtGNU_EnclosedCompoundStatementExpression(
        const ExtGNU_EnclosedCompoundStatementExpressionSyntax* node)
{
    // TODO
    visit(node->statement());
    return Action::Skip;
}

SyntaxVisitor::Action TypeChecker::visitExtGNU_ComplexValuedExpression(
        const ExtGNU_ComplexValuedExpressionSyntax* node)
{
    VISIT(node->expression());

    return typeChecked(node, ty_);
}

/* Operations */

SyntaxVisitor::Action TypeChecker::visitPrefixUnaryExpression(
        const PrefixUnaryExpressionSyntax* node)
{
    VISIT(node->expression());

    const Type* ty = nullptr;
    switch (node->operatorToken().kind()) {
        case SyntaxKind::PlusPlusToken:
        case SyntaxKind::MinusMinusToken: {
            auto coreTy = unqualifiedAndResolved(ty_);
            if (!(isRealType(coreTy) || coreTy->kind() == TypeKind::Pointer)) {
                diagReporter_.InvalidOperator(node->operatorToken());
                return typeCheckError(node);
            }
            ty = ty_;
            break;
        }
        case SyntaxKind::AmpersandToken: {
            std::unique_ptr<PointerType> ptrTy(new PointerType(ty_));
            ty = semaModel_->keepType(std::move(ptrTy));
            break;
        }
        case SyntaxKind::AsteriskToken: {
            auto coreTy = unqualifiedAndResolved(ty_);
            switch (coreTy->kind()) {
                case TypeKind::Array:
                    ty = coreTy->asArrayType()->elementType();
                    break;
                case TypeKind::Pointer:
                    ty = coreTy->asPointerType()->referencedType();
                    break;
                case TypeKind::Function:
                    ty = coreTy;
                    break;
                default:
                    diagReporter_.ExpectedExpressionOfPointerType(node->lastToken());
                    return typeCheckError(node);
            }
            break;
        }
        case SyntaxKind::PlusToken:
        case SyntaxKind::MinusToken: {
            auto coreTy = unqualifiedAndResolved(ty_);
            if (!isArithmeticType(coreTy)) {
                diagReporter_.ExpectedExpressionOfArithmeticType(node->operatorToken());
                return typeCheckError(node);
            }
            ty = ty_;
            break;
        }
        case SyntaxKind::TildeToken: {
            auto coreTy = unqualifiedAndResolved(ty_);
            if (!isIntegerType(coreTy)) {
                diagReporter_.ExpectedExpressionOfIntegerType(node->operatorToken());
                return typeCheckError(node);
            }
            ty = ty_;
            break;
        }
        case SyntaxKind::ExclamationToken: {
            auto coreTy = unqualifiedAndResolved(ty_);
            if (!isScalarType(coreTy)) {
                diagReporter_.ExpectedExpressionOfScalarType(node->operatorToken());
                return typeCheckError(node);
            }
            ty = ty_;
            break;
        }
        case SyntaxKind::AmpersandAmpersandToken:
            ty = semaModel_->compilation()->canonicalBasicType(BasicTypeKind::Int_S);
            break;

        default:
            PSY_ASSERT_1(false);
            return Action::Quit;
    }
    return typeChecked(node, ty);
}

SyntaxVisitor::Action TypeChecker::visitPostfixUnaryExpression(
        const PostfixUnaryExpressionSyntax* node)
{
    VISIT(node->expression());

    switch (node->operatorToken().kind()) {
        case SyntaxKind::PlusPlusToken:
        case SyntaxKind::MinusMinusToken: {
            auto coreTy = unqualifiedAndResolved(ty_);
            if (!(isRealType(coreTy) || coreTy->kind() == TypeKind::Pointer)) {
                diagReporter_.InvalidOperator(node->operatorToken());
                return typeCheckError(node);
            }
            break;
        }
        default:
            PSY_ASSERT_1(false);
            return Action::Quit;
    }
    return typeChecked(node, ty_);
}

SyntaxVisitor::Action TypeChecker::visitMemberAccessExpression(
        const MemberAccessExpressionSyntax* node)
{
    VISIT(node->expression());

    const TagType* tagTy = nullptr;
    auto coreTy = unqualifiedAndResolved(ty_);
    switch (coreTy->kind()) {
        case TypeKind::Tag:
            if (node->kind() != SyntaxKind::DirectMemberAccessExpression) {
                diagReporter_.InvalidOperator(node->operatorToken());
                return typeCheckError(node);
            }
            tagTy = coreTy->asTagType();
            break;

        case TypeKind::Pointer: {
            if (node->kind() != SyntaxKind::IndirectMemberAccessExpression) {
                diagReporter_.InvalidOperator(node->operatorToken());
                return typeCheckError(node);
            }
            auto refedTy = coreTy->asPointerType()->referencedType();
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
                return Action::Quit;
        }
        return typeCheckError(node);
    }

    auto tagTyDecl = tagTy->declaration();
    if (!tagTyDecl)
        return typeCheckError(node);
    auto membDecl = tagTyDecl->member(identifierFrom(node->memberName()));
    if (!membDecl) {
        diagReporter_.UnknownMemberOfTag(node->memberName()->lastToken());
        return typeCheckError(node);
    }
    return typeChecked(node, membDecl->type());
}

SyntaxVisitor::Action TypeChecker::visitArraySubscriptExpression(
        const ArraySubscriptExpressionSyntax* node)
{
    VISIT(node->argument());

    auto argTy = unqualifiedAndResolved(ty_);
    if (!isIntegerType(argTy)) {
        diagReporter_.ExpectedExpressionOfIntegerType(node->argument()->lastToken());
        return typeCheckError(node);
    }

    VISIT(node->expression());

    const Type* ty = nullptr;
    auto coreTy = unqualifiedAndResolved(ty_);
    switch (coreTy->kind()) {
        case TypeKind::Array:
            ty = coreTy->asArrayType()->elementType();
            break;
        case TypeKind::Pointer:
            ty = coreTy->asPointerType()->referencedType();
            break;
        default:
            diagReporter_.ExpectedExpressionOfPointerOrArrayType(
                        node->expression()->lastToken());
            return typeCheckError(node);
    }
    return typeChecked(node, ty);
}

SyntaxVisitor::Action TypeChecker::visitTypeTraitExpression(
        const TypeTraitExpressionSyntax* node)
{
    switch (node->tyReference()->kind()) {
        case SyntaxKind::ExpressionAsTypeReference: {
            auto exprAsTy = node->tyReference()->asExpressionAsTypeReference();
            VISIT(exprAsTy->expression());
            break;
        }
        case SyntaxKind::TypeNameAsTypeReference: {
            auto tyNameAsTy = node->tyReference()->asTypeNameAsTypeReference();
            VISIT(tyNameAsTy->typeName());
            break;
        }
        default:
            PSY_ASSERT_1(false);
            return Action::Quit;
    }

    switch (node->operatorToken().kind()) {
        case SyntaxKind::Keyword_sizeof:
        case SyntaxKind::Keyword__Alignof:
            if (ty_->kind() == TypeKind::Function) {
                diagReporter_.InvalidOperator(node->operatorToken());
                return typeCheckError(node);
            }
            break;
        default:
            PSY_ASSERT_1(false);
            return Action::Quit;
    }
    return typeChecked(
        node,
        semaModel_->compilation()->canonicalBasicType(BasicTypeKind::Int_S));
}

SyntaxVisitor::Action TypeChecker::visitCastExpression(
        const CastExpressionSyntax* node)
{
    VISIT(node->typeName());

    return typeChecked(node, ty_);
}

SyntaxVisitor::Action TypeChecker::visitCallExpression(
        const CallExpressionSyntax* node)
{
    VISIT(node->expression());

    const FunctionType* funcTy = nullptr;
    auto coreTy = unqualifiedAndResolved(ty_);
    switch (coreTy->kind()) {
        case TypeKind::Function:
            funcTy = coreTy->asFunctionType();
            break;
        case TypeKind::Pointer: {
            auto refedTy = unqualifiedAndResolved(coreTy->asPointerType()->referencedType());
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
        return typeCheckError(node);
    }

    std::vector<std::pair<const Type*, const ExpressionSyntax*>> argTysWithNode;
    for (auto iter = node->arguments(); iter; iter = iter->next) {
        VISIT(iter->value);
        argTysWithNode.push_back(std::make_pair(ty_, iter->value));
    }
    const auto& parmTys = funcTy->parameterTypes();
    switch (funcTy->parameterListForm()) {
        case FunctionType::ParameterListForm::SpecifiedAsEmpty:
            if (argTysWithNode.size() > 0) {
                diagReporter_.TooManyArgumentsToFunctionCall(
                    node->expression()->lastToken());
                return typeCheckError(node);
            }
            break;
        case FunctionType::ParameterListForm::Unspecified:
            if (argTysWithNode.size() > 0) {
                // TODO
            }
            break;
        case FunctionType::ParameterListForm::NonEmpty: {
            if (argTysWithNode.size() < parmTys.size()) {
                diagReporter_.TooFewArgumentsToFunctionCall(
                    node->expression()->lastToken());
                return typeCheckError(node);
            }
            auto parmTyIdx = 0U;
            for (; parmTyIdx < parmTys.size(); ++parmTyIdx) {
                auto parmTy = parmTys[parmTyIdx];
                auto argTyWithNode = argTysWithNode[parmTyIdx];
                if (!isTypeAssignableFromOtherType(
                        parmTy,
                        argTyWithNode.first,
                        argTyWithNode.second)) {
                    diagReporter_.IncompatibleTypesInArgumentToParameterAssignment(
                        argTyWithNode.second->firstToken());
                    return typeCheckError(node);
                }
            }
            if (parmTyIdx < argTysWithNode.size() && !funcTy->isVariadic()) {
                diagReporter_.TooManyArgumentsToFunctionCall(
                    node->expression()->lastToken());
                return typeCheckError(node);
            }
            break;
        }
        default:
            PSY_ASSERT_1(false);
            return Action::Quit;
    }
    return typeChecked(node, funcTy->returnType());
}

SyntaxVisitor::Action TypeChecker::visitVAArgumentExpression(const VAArgumentExpressionSyntax*) { return Action::Skip; }
SyntaxVisitor::Action TypeChecker::visitOffsetOfExpression(const OffsetOfExpressionSyntax*) { return Action::Skip; }
SyntaxVisitor::Action TypeChecker::visitCompoundLiteralExpression(const CompoundLiteralExpressionSyntax*) { return Action::Skip; }

SyntaxVisitor::Action TypeChecker::visitBinaryExpression(const BinaryExpressionSyntax* node)
{
    VISIT(node->left());
    auto leftTy = unqualifiedAndResolved(ty_);
    VISIT(node->right());
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
        return typeCheckError(node);
    }
    auto ty = determineCommonRealType(leftTy->asBasicType(), rightTy->asBasicType());
    return typeChecked(node, ty);
}

template <class BinaryLikeExprNodeT>
SyntaxVisitor::Action TypeChecker::visitBinaryExpression_Remainder(
        const BinaryLikeExprNodeT* node,
        const Type* leftTy,
        const Type* rightTy)
{
    if (!(satisfyIntegerTypeConstraint(leftTy, node->left())
            && satisfyIntegerTypeConstraint(rightTy, node->right()))) {
        return typeCheckError(node);
    }
    auto ty = determineCommonRealType(leftTy->asBasicType(), rightTy->asBasicType());
    return typeChecked(node, ty);
}

template <class BinaryLikeExprNodeT>
SyntaxVisitor::Action TypeChecker::visitBinaryExpression_Addition(
        const BinaryLikeExprNodeT* node,
        const Type* leftTy,
        const Type* rightTy)
{
    const Type* ty = nullptr;
    if (isArithmeticType(leftTy) && isArithmeticType(rightTy)) {
        ty = determineCommonRealType(leftTy->asBasicType(), rightTy->asBasicType());
    }
    else if (leftTy->kind() == TypeKind::Pointer
               && isIntegerType(rightTy)) {
        ty = leftTy;
    }
    else if (rightTy->kind() == TypeKind::Pointer
                && isIntegerType(leftTy)) {
        ty = rightTy;
    }
    else {
        diagReporter_.InvalidOperator(node->operatorToken());
        return typeCheckError(node);
    }
    return typeChecked(node, ty);
}

template <class BinaryLikeExprNodeT>
SyntaxVisitor::Action TypeChecker::visitBinaryExpression_Subtraction(
        const BinaryLikeExprNodeT* node,
        const Type* leftTy,
        const Type* rightTy)
{
    const Type* ty = nullptr;
    if (isArithmeticType(leftTy) && isArithmeticType(rightTy)) {
        ty = determineCommonRealType(leftTy->asBasicType(), rightTy->asBasicType());
    }
    else if (leftTy->kind() == TypeKind::Pointer) {
        if (rightTy->kind() == TypeKind::Pointer) {
            ty = ptrdiffTy_;
        }
        else if (isIntegerType(rightTy)) {
            ty = leftTy;
        }
        else {
            diagReporter_.InvalidOperator(node->operatorToken());
            return typeCheckError(node);
        }
    }
    else {
        diagReporter_.InvalidOperator(node->operatorToken());
        return typeCheckError(node);
    }
    return typeChecked(node, ty);
}

template <class BinaryLikeExprNodeT>
SyntaxVisitor::Action TypeChecker::visitBinaryExpression_BitwiseShift(
        const BinaryLikeExprNodeT* node,
        const Type* leftTy,
        const Type* rightTy)
{
    if (!(satisfyIntegerTypeConstraint(leftTy, node->left())
            && satisfyIntegerTypeConstraint(rightTy, node->right()))) {
        return typeCheckError(node);
    }
    auto promoTyK = performIntegerPromotion(leftTy->asBasicType()->kind());
    auto ty = semaModel_->compilation()->canonicalBasicType(promoTyK);
    return typeChecked(node, ty);
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
        return typeCheckError(node);
    }
    auto ty = semaModel_->compilation()->canonicalBasicType(BasicTypeKind::Int_S);
    return typeChecked(node, ty);
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
        return typeCheckError(node);
    }
    auto ty = semaModel_->compilation()->canonicalBasicType(BasicTypeKind::Int_S);
    return typeChecked(node, ty);
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
    VISIT(node->left());
    if (!isAssignableType(ty_, node->left()))
        return Action::Quit;
    auto leftTy = unqualifiedAndResolved(ty_);
    VISIT(node->right());
    auto rightTy = unqualifiedAndResolved(ty_);

    switch (node->operatorToken().kind()) {
        case SyntaxKind::EqualsToken:
            if (!isTypeAssignableFromOtherType(leftTy, rightTy, node->right())) {
                diagReporter_.IncompatibleTypesInAssignment(node->operatorToken());
                return typeCheckError(node);
            }
            return typeChecked(node, leftTy);
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
}

bool TypeChecker::isTypeAssignableFromOtherType(
        const Type* ty,
        const Type* otherTy,
        const SyntaxNode* node)
{
    return ((isArithmeticType(ty) && isArithmeticType(otherTy))
            || (isStructureOrUnionType(ty)
                && typesAreCompatible(ty, otherTy, false, false))
            || (ty->kind() == TypeKind::Pointer
                && ((otherTy->kind() == TypeKind::Pointer
                        && typesAreCompatible(
                                ty->asPointerType()->referencedType(),
                                otherTy->asPointerType()->referencedType(),
                                true,
                                true))
                    || (otherTy->kind() == TypeKind::Basic
                        && otherTy->asBasicType()->kind() == BasicTypeKind::Int_S
                        && isNULLPointerConstant(node)))));
}

SyntaxVisitor::Action TypeChecker::visitSequencingExpression(const SequencingExpressionSyntax*) { return Action::Skip; }
SyntaxVisitor::Action TypeChecker::visitExtGNU_ChooseExpression(const ExtGNU_ChooseExpressionSyntax*) { return Action::Skip; }

//------------//
// Statements //
//------------//

SyntaxVisitor::Action TypeChecker::visitDeclarationStatement(const DeclarationStatementSyntax* node)
{
    visitChildNodes(node);

    return Action::Skip;
}

SyntaxVisitor::Action TypeChecker::visitReturnStatement(const ReturnStatementSyntax* node)
{
    visitChildNodes(node);

    return Action::Skip;
}

SyntaxVisitor::Action TypeChecker::visitExpressionStatement(const ExpressionStatementSyntax* node)
{
    visitChildNodes(node);

    return Action::Skip;
}

//--------//
// Common //
//--------//

SyntaxVisitor::Action TypeChecker::visitTypeName(const TypeNameSyntax* node)
{
    const Type* ty = nullptr;
    auto decl = semaModel_->declarationBy(node->declarator());
    PSY_ASSERT_2(decl, return Action::Quit);
    switch (decl->category()) {
        case DeclarationCategory::Function:
            ty = decl->asFunctionDeclaration()->type();
            break;
        case DeclarationCategory::Object:
            ty = decl->asObjectDeclaration()->type();
            break;
        default:
            PSY_ASSERT_1(false);
            return Action::Quit;
    }
    PSY_ASSERT_2(ty, return Action::Quit);
    createTypeInfo(node, ty, TypeInfo::TypeOrigin::TypeName);

    return Action::Skip;
}

#undef VISIT
