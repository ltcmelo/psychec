// Copyright (c) 2016/17/18/19/20/21/22 Leandro T. C. Melo <ltcmelo@gmail.com>
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

#include "SyntaxTree.h"

#include "binder/Scope.h"
#include "compilation/Compilation.h"
#include "compilation/SemanticModel.h"
#include "symbols/Symbol_ALL.h"
#include "syntax/Lexeme_ALL.h"
#include "syntax/SyntaxFacts.h"
#include "syntax/SyntaxNodes.h"
#include "types/Type_ALL.h"

#include "../common/infra/Assertions.h"

#include <cstdint>
#include <limits>

#define V_(v) if (v == Action::Quit) return Action::Quit

using namespace psy;
using namespace C;

// TODO: move to separate file
class PlatformOptions
{
public:
    PlatformOptions()
        : maxVal_Char(std::numeric_limits<char>::max())
        , maxVal_Char_S(std::numeric_limits<char signed>::max())
        , maxVal_Char_U(std::numeric_limits<char unsigned>::max())
        , maxVal_Short_S(std::numeric_limits<short>::max())
        , maxVal_Short_U(std::numeric_limits<short unsigned>::max())
        , maxVal_Int_S(std::numeric_limits<int>::max())
        , maxVal_Int_U(std::numeric_limits<int unsigned>::max())
        , maxVal_Long_S(std::numeric_limits<long>::max())
        , maxVal_Long_U(std::numeric_limits<long unsigned>::max())
        , maxVal_LongLong_S(std::numeric_limits<long long>::max())
        , maxVal_LongLong_U(std::numeric_limits<long long unsigned>::max())
        , maxVal_Bool(std::numeric_limits<bool>::max())
    {}

    enum class ArithmeticIntegerType
    {
        Char,
        Char_S,
        Char_U,
        Short_S,
        Short_U,
        Int_S,
        Int_U,
        Long_S,
        Long_U,
        LongLong_S,
        LongLong_U,
        Bool,
    };

    void setMaxValue(ArithmeticIntegerType arithIntTy, unsigned long long v)
    {
        switch (arithIntTy) {
            case ArithmeticIntegerType::Char:
                maxVal_Char = v;
                break;
            case ArithmeticIntegerType::Char_S:
                maxVal_Char_S = v;
                break;
            case ArithmeticIntegerType::Char_U:
                maxVal_Char_U = v;
                break;
            case ArithmeticIntegerType::Short_S:
                maxVal_Short_S = v;
                break;
            case ArithmeticIntegerType::Short_U:
                maxVal_Short_U = v;
                break;
            case ArithmeticIntegerType::Int_S:
                maxVal_Int_S = v;
                break;
            case ArithmeticIntegerType::Int_U:
                maxVal_Int_U = v;
                break;
            case ArithmeticIntegerType::Long_S:
                maxVal_Long_S = v;
                break;
            case ArithmeticIntegerType::Long_U:
                maxVal_Long_U = v;
                break;
            case ArithmeticIntegerType::LongLong_S:
                maxVal_LongLong_S = v;
                break;
            case ArithmeticIntegerType::LongLong_U:
                maxVal_LongLong_U = v;
                break;
            case ArithmeticIntegerType::Bool:
                maxVal_Bool = v;
                break;
        }
    }

    unsigned long long maxValue(ArithmeticIntegerType arithIntTy)
    {
        switch (arithIntTy) {
            case ArithmeticIntegerType::Char:
                return maxVal_Char;
            case ArithmeticIntegerType::Char_S:
                return maxVal_Char_S;
            case ArithmeticIntegerType::Char_U:
                return maxVal_Char_U;
            case ArithmeticIntegerType::Short_S:
                return maxVal_Short_S;
            case ArithmeticIntegerType::Short_U:
                return maxVal_Short_U;
            case ArithmeticIntegerType::Int_S:
                return maxVal_Int_S;
            case ArithmeticIntegerType::Int_U:
                return maxVal_Int_U;
            case ArithmeticIntegerType::Long_S:
                return maxVal_Long_S;
            case ArithmeticIntegerType::Long_U:
                return maxVal_Long_U;
            case ArithmeticIntegerType::LongLong_S:
                return maxVal_LongLong_S;
            case ArithmeticIntegerType::LongLong_U:
                return maxVal_LongLong_U;
            case ArithmeticIntegerType::Bool:
                return maxVal_Bool;
        }
        PSY_ASSERT_1(false);
        return 0;
    }

private:
    unsigned long long maxVal_Char;
    unsigned long long maxVal_Char_S;
    unsigned long long maxVal_Char_U;
    unsigned long long maxVal_Short_S;
    unsigned long long maxVal_Short_U;
    unsigned long long maxVal_Int_S;
    unsigned long long maxVal_Int_U;
    unsigned long long maxVal_Long_S;
    unsigned long long maxVal_Long_U;
    unsigned long long maxVal_LongLong_S;
    unsigned long long maxVal_LongLong_U;
    unsigned long long maxVal_Bool;
};

namespace
{
const Type* pickType(const Typedef* tydefDecl, const Type* altTy)
{
    if (!tydefDecl)
        return altTy;
    PSY_ASSERT_1(tydefDecl->synonymizedType());
    return tydefDecl->synonymizedType();
}
} // anonymous

TypeChecker::TypeChecker(SemanticModel* semaModel, const SyntaxTree* tree)
    : SyntaxVisitor(tree)
    , semaModel_(semaModel)
    , scope_(nullptr)
    , ty_(nullptr)
    , ptrdiffTy_(
          pickType(
              semaModel->ptrdiff_t_Typedef(),
              semaModel->compilation()->canonicalBasicType(BasicTypeKind::Int_S)))
    , sizeTy_(
          pickType(
              semaModel->size_t_Typedef(),
              semaModel->compilation()->canonicalBasicType(BasicTypeKind::Long_U)))
    , maxAlignTy_(
          pickType(
              semaModel->max_align_t_Typedef(),
              semaModel->compilation()->canonicalBasicType(BasicTypeKind::LongLong_S)))
    , wcharTy_(
          pickType(
              semaModel->wchar_t_Typedef(),
              semaModel->compilation()->canonicalBasicType(BasicTypeKind::Int_S)))
    , char16Ty_(
          pickType(
              semaModel->char16_t_Typedef(),
              semaModel->compilation()->canonicalBasicType(BasicTypeKind::Short_U)))
    , char32Ty_(
          pickType(
              semaModel->char32_t_Typedef(),
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

void TypeChecker::typeCheck()
{
    // TODO
    return;

    visit(tree_->root());
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
        bool treatVoidAsAny)
{
    switch (oneTy->kind()) {
        case TypeKind::Array:
            switch (otherTy->kind()) {
                case TypeKind::Array:
                    return typesAreCompatible(
                                oneTy->asArrayType()->elementType(),
                                otherTy->asArrayType()->elementType(),
                                treatVoidAsAny);
                case TypeKind::Basic:
                case TypeKind::Function:
                    break;
                case TypeKind::Pointer:
                    return typesAreCompatible(
                                oneTy->asArrayType()->elementType(),
                                otherTy->asPointerType()->referencedType(),
                                treatVoidAsAny);
                case TypeKind::Typedef:
                    PSY_ASSERT_1(false);
                    break;
                case TypeKind::Tag:
                    break;
                case TypeKind::Void:
                    return treatVoidAsAny;
                case TypeKind::Qualified:
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
                case TypeKind::Typedef:
                    PSY_ASSERT_1(false);
                    break;
                case TypeKind::Tag:
                case TypeKind::Void:
                    return treatVoidAsAny;
                case TypeKind::Qualified:
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
                case TypeKind::Typedef:
                    PSY_ASSERT_1(false);
                    break;
                case TypeKind::Tag:
                    break;
                case TypeKind::Void:
                    return treatVoidAsAny;
                case TypeKind::Qualified:
                    break;
            }
            break;

        case TypeKind::Pointer:
            switch (otherTy->kind()) {
                case TypeKind::Array:
                    return typesAreCompatible(
                                oneTy->asPointerType()->referencedType(),
                                otherTy->asArrayType()->elementType(),
                                treatVoidAsAny);
                case TypeKind::Basic:
                case TypeKind::Function:
                    break;
                case TypeKind::Pointer:
                    return typesAreCompatible(
                                oneTy->asPointerType()->referencedType(),
                                otherTy->asPointerType()->referencedType(),
                                treatVoidAsAny);
                case TypeKind::Typedef:
                    PSY_ASSERT_1(false);
                    break;
                case TypeKind::Tag:
                    break;
                case TypeKind::Void:
                    return treatVoidAsAny;
                case TypeKind::Qualified:
                    break;
            }
            break;

        case TypeKind::Typedef:
            PSY_ASSERT_1(false);
            break;

        case TypeKind::Tag:
            switch (otherTy->kind()) {
                case TypeKind::Array:
                case TypeKind::Basic:
                case TypeKind::Function:
                case TypeKind::Pointer:
                    break;
                case TypeKind::Typedef:
                    PSY_ASSERT_1(false);
                    break;
                case TypeKind::Tag:
                    break;
                case TypeKind::Void:
                    return treatVoidAsAny;
                case TypeKind::Qualified:
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
                case TypeKind::Typedef:
                    PSY_ASSERT_1(false);
                    break;
                case TypeKind::Tag:
                    break;
                case TypeKind::Void:
                    return true;
                case TypeKind::Qualified:
                    return treatVoidAsAny;
            }
            break;

        case TypeKind::Qualified:
            switch (otherTy->kind()) {
                case TypeKind::Array:
                case TypeKind::Basic:
                case TypeKind::Function:
                case TypeKind::Pointer:
                    break;
                case TypeKind::Typedef:
                    PSY_ASSERT_1(false);
                    break;
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
                                treatVoidAsAny);
                }
            }
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

SyntaxVisitor::Action TypeChecker::visit_COMMON(const SyntaxNode *node)
{
    visit(node);
    PSY_ASSERT_2(ty_ != nullptr, return Action::Quit);
    return Action::Skip;
}

//--------------//
// Declarations //
//--------------//

SyntaxVisitor::Action TypeChecker::visitTranslationUnit(const TranslationUnitSyntax* node)
{
    scope_ = semaModel_->associatedScope(node);
    PSY_ASSERT_2(scope_ != nullptr, return Action::Skip);
    for (auto declIt = node->declarations(); declIt; declIt = declIt->next)
        visit(declIt->value);
    scope_ = scope_->outerScope();

    return Action::Skip;
}

SyntaxVisitor::Action TypeChecker::visitFunctionDefinition(const FunctionDefinitionSyntax* node)
{
    scope_ = semaModel_->associatedScope(node);
    PSY_ASSERT_2(scope_ != nullptr, return Action::Skip);
    for (auto stmtIt = node->body()->statements(); stmtIt; stmtIt = stmtIt->next)
        visit(stmtIt->value);
    scope_ = scope_->outerScope();

    return Action::Skip;
}

//-------------//
// Expressions //
//-------------//

SyntaxVisitor::Action TypeChecker::visitIdentifierName(const IdentifierNameSyntax* node)
{
    auto decl = scope_->searchForDeclaration(
        node->identifierToken().lexeme()->asIdentifier(),
        NameSpace::OrdinaryIdentifiers);
    PSY_ASSERT_2(decl, return Action::Quit);
    switch (decl->kind()) {
        case DeclarationKind::Function:
            ty_ = decl->asFunction()->type();
            break;
        case DeclarationKind::Object: {
            ty_ = decl->asObjectDeclaration()->type();
            break;
        }
        case DeclarationKind::Type:
            PSY_ASSERT_1(false);
            return Action::Quit;
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
BasicTypeKind selectTypeForValue(unsigned long long v, BasicTypeKind (&candidates)[N])
{
    PlatformOptions platfOpts; // TODO: use field

    for (size_t i = 0; i < N - 1; ++i) {
        unsigned long long max;
        BasicTypeKind basicTyK = candidates[i];
        switch (basicTyK) {
            case BasicTypeKind::Char:
                max = platfOpts.maxValue(PlatformOptions::ArithmeticIntegerType::Char);
                break;
            case BasicTypeKind::Char_S:
                max = platfOpts.maxValue(PlatformOptions::ArithmeticIntegerType::Char_S);
                break;
            case BasicTypeKind::Char_U:
                max = platfOpts.maxValue(PlatformOptions::ArithmeticIntegerType::Char_U);
                break;
            case BasicTypeKind::Short_S:
                max = platfOpts.maxValue(PlatformOptions::ArithmeticIntegerType::Short_S);
                break;
            case BasicTypeKind::Short_U:
                max = platfOpts.maxValue(PlatformOptions::ArithmeticIntegerType::Short_U);
                break;
            case BasicTypeKind::Int_S:
                max = platfOpts.maxValue(PlatformOptions::ArithmeticIntegerType::Int_S);
                break;
            case BasicTypeKind::Int_U:
                max = platfOpts.maxValue(PlatformOptions::ArithmeticIntegerType::Int_U);
                break;
            case BasicTypeKind::Long_S:
                max = platfOpts.maxValue(PlatformOptions::ArithmeticIntegerType::Long_S);
                break;
            case BasicTypeKind::Long_U:
                max = platfOpts.maxValue(PlatformOptions::ArithmeticIntegerType::Long_U);
                break;
            case BasicTypeKind::LongLong_S:
                max = platfOpts.maxValue(PlatformOptions::ArithmeticIntegerType::LongLong_S);
                break;
            case BasicTypeKind::LongLong_U:
                max = platfOpts.maxValue(PlatformOptions::ArithmeticIntegerType::LongLong_U);
                break;
            case BasicTypeKind::Bool:
                max = platfOpts.maxValue(PlatformOptions::ArithmeticIntegerType::Bool);
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
                        basicTyK = selectTypeForValue(val, kinds);
                    } else {
                        BasicTypeKind kinds[] = {BasicTypeKind::Int_S,
                                                 BasicTypeKind::Long_S,
                                                 BasicTypeKind::LongLong_S};
                        basicTyK = selectTypeForValue(val, kinds);
                    }
                    break;

                case IntegerConstant::RepresentationSuffix::uOrU: {
                    BasicTypeKind kinds[] = {BasicTypeKind::Int_U,
                                             BasicTypeKind::Long_U,
                                             BasicTypeKind::LongLong_U};
                    basicTyK = selectTypeForValue(val, kinds);
                    break;
                }
                case IntegerConstant::RepresentationSuffix::lOrL:
                    if (intTk->isOctalOrHexadecimal()) {
                        BasicTypeKind kinds[] = {BasicTypeKind::Long_S,
                                                 BasicTypeKind::Long_U,
                                                 BasicTypeKind::LongLong_S,
                                                 BasicTypeKind::LongLong_U};
                        basicTyK = selectTypeForValue(val, kinds);
                    } else {
                        BasicTypeKind kinds[] = {BasicTypeKind::Long_S, BasicTypeKind::LongLong_S};
                        basicTyK = selectTypeForValue(val, kinds);
                    }
                    break;

                case IntegerConstant::RepresentationSuffix::lOrLAnduOrU: {
                    BasicTypeKind kinds[] = {BasicTypeKind::Long_U, BasicTypeKind::LongLong_U};
                    basicTyK = selectTypeForValue(val, kinds);
                    break;
                }
                case IntegerConstant::RepresentationSuffix::llOrLL:
                    if (intTk->isOctalOrHexadecimal()) {
                        BasicTypeKind kinds[] = {BasicTypeKind::LongLong_S, BasicTypeKind::LongLong_U};
                        basicTyK = selectTypeForValue(val, kinds);
                    } else {
                        BasicTypeKind kinds[] = {BasicTypeKind::LongLong_S};
                        basicTyK = selectTypeForValue(val, kinds);
                    }
                    break;
                case IntegerConstant::RepresentationSuffix::llOrLLAnduOrU:
                    BasicTypeKind kinds[] = {BasicTypeKind::LongLong_U};
                    basicTyK = selectTypeForValue(val, kinds);
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
            return visitStringLiteral(constantTk->asStringLiteral()->encodingPrefix());
        default:
            PSY_ASSERT_1(false);
            return Action::Quit;
    }

    return Action::Skip;
}

SyntaxVisitor::Action TypeChecker::visitStringLiteralExpression(
        const StringLiteralExpressionSyntax* node)
{
    return visitStringLiteral(node->literalToken().lexeme()->asStringLiteral()->encodingPrefix());
}

SyntaxVisitor::Action TypeChecker::visitStringLiteral(StringLiteral::EncodingPrefix encodingSuffix)
{
    switch (encodingSuffix) {
        case StringLiteral::EncodingPrefix::None:
            ty_ = strLitTy_;
            break;
        case StringLiteral::EncodingPrefix::u8:
            ty_ = u8StrLitTy_;
            break;
        case StringLiteral::EncodingPrefix::u:
            ty_ = uStrLitTy_;
            break;
        case StringLiteral::EncodingPrefix::U:
            ty_ = UStrLitTy_;
            break;
        case StringLiteral::EncodingPrefix::L:
            ty_ = LStrLitTy_;
            break;
    }

    return Action::Skip;
}

SyntaxVisitor::Action TypeChecker::visitParenthesizedExpression(
        const ParenthesizedExpressionSyntax* node)
{
    V_(visit_COMMON(node->expression()));

    return Action::Skip;
}

SyntaxVisitor::Action TypeChecker::visitGenericSelectionExpression(const GenericSelectionExpressionSyntax*) { return Action::Skip; }
SyntaxVisitor::Action TypeChecker::visitGenericAssociation(const GenericAssociationSyntax*) { return Action::Skip; }
SyntaxVisitor::Action TypeChecker::visitExtGNU_EnclosedCompoundStatementExpression(const ExtGNU_EnclosedCompoundStatementExpressionSyntax*) { return Action::Skip; }
SyntaxVisitor::Action TypeChecker::visitExtGNU_ComplexValuedExpression(const ExtGNU_ComplexValuedExpressionSyntax*) { return Action::Skip; }

/* Operations */

SyntaxVisitor::Action TypeChecker::visitPrefixUnaryExpression(
        const PrefixUnaryExpressionSyntax* node)
{
    V_(visit_COMMON(node->expression()));

    switch (node->operatorToken().kind()) {
        case SyntaxKind::PlusPlusToken:
        case SyntaxKind::MinusMinusToken:
            if (!(isRealType(ty_) || ty_->kind() == TypeKind::Pointer)) {
                diagReporter_.InvalidOperator(node->operatorToken());
                return Action::Quit;
            }
            break;
        case SyntaxKind::AmpersandToken: {
            std::unique_ptr<PointerType> ptrTy(new PointerType(ty_));
            ty_ = semaModel_->keepType(std::move(ptrTy));
            break;
        }
        case SyntaxKind::AsteriskToken:
            if (ty_->kind() != TypeKind::Pointer) {
                diagReporter_.ExpectedExpressionOfPointerType(node->lastToken());
                return Action::Quit;
            }
            ty_ = ty_->asPointerType()->referencedType();
            break;
        case SyntaxKind::PlusToken:
        case SyntaxKind::MinusToken:
            if (!isArithmeticType(ty_)) {
                diagReporter_.ExpectedExpressionOfArithmeticType(node->operatorToken());
                return Action::Quit;
            }
            break;
        case SyntaxKind::TildeToken:
            if (!isIntegerType(ty_)) {
                diagReporter_.ExpectedExpressionOfIntegerType(node->operatorToken());
                return Action::Quit;
            }
            break;
        case SyntaxKind::ExclamationToken:
            if (!isScalarType(ty_)) {
                diagReporter_.ExpectedExpressionOfScalarType(node->operatorToken());
                return Action::Quit;
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
    V_(visit_COMMON(node->expression()));

    switch (node->operatorToken().kind()) {
        case SyntaxKind::PlusPlusToken:
        case SyntaxKind::MinusMinusToken:
            if (!(isRealType(ty_) || ty_->kind() == TypeKind::Pointer)) {
                diagReporter_.InvalidOperator(node->operatorToken());
                return Action::Quit;
            }
            break;
        default:
            PSY_ASSERT_1(false);
            return Action::Quit;
    }

    return Action::Skip;
}

SyntaxVisitor::Action TypeChecker::visitMemberAccessExpression(const MemberAccessExpressionSyntax*) { return Action::Skip; }
SyntaxVisitor::Action TypeChecker::visitArraySubscriptExpression(const ArraySubscriptExpressionSyntax*) { return Action::Skip; }

SyntaxVisitor::Action TypeChecker::visitTypeTraitExpression(
        const TypeTraitExpressionSyntax* node)
{
    V_(visit_COMMON(node->tyReference()));

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
    V_(visit_COMMON(node->typeName()));

    return Action::Skip;
}

SyntaxVisitor::Action TypeChecker::visitCallExpression(const CallExpressionSyntax*) { return Action::Skip; }
SyntaxVisitor::Action TypeChecker::visitVAArgumentExpression(const VAArgumentExpressionSyntax*) { return Action::Skip; }
SyntaxVisitor::Action TypeChecker::visitOffsetOfExpression(const OffsetOfExpressionSyntax*) { return Action::Skip; }
SyntaxVisitor::Action TypeChecker::visitCompoundLiteralExpression(const CompoundLiteralExpressionSyntax*) { return Action::Skip; }

SyntaxVisitor::Action TypeChecker::visitBinaryExpression(const BinaryExpressionSyntax* node)
{
    V_(visit_COMMON(node->left()));
    auto leftTy = unqualifyType(ty_);
    V_(visit_COMMON(node->right()));
    auto rightTy = unqualifyType(ty_);

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

SyntaxVisitor::Action TypeChecker::visitBinaryExpression_MultiplicationOrDivision(
        const BinaryExpressionSyntax* node,
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

SyntaxVisitor::Action TypeChecker::visitBinaryExpression_Remainder(
        const BinaryExpressionSyntax* node,
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

SyntaxVisitor::Action TypeChecker::visitBinaryExpression_Addition(
        const BinaryExpressionSyntax* node,
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

SyntaxVisitor::Action TypeChecker::visitBinaryExpression_Subtraction(
        const BinaryExpressionSyntax* node,
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

SyntaxVisitor::Action TypeChecker::visitBinaryExpression_BitwiseShift(
        const BinaryExpressionSyntax* node,
        const Type* leftTy,
        const Type* rightTy)
{
    if (!(satisfyIntegerTypeConstraint(leftTy, node->left())
            && satisfyIntegerTypeConstraint(rightTy, node->right()))) {
        return Action::Quit;
    }
    auto promotedTyK = performIntegerPromotion(leftTy->asBasicType()->kind());
    ty_ = semaModel_->compilation()->canonicalBasicType(promotedTyK);

    return Action::Skip;
}

SyntaxVisitor::Action TypeChecker::visitBinaryExpression_Relational(
        const BinaryExpressionSyntax* node,
        const Type* leftTy,
        const Type* rightTy)
{
    if (!((isRealType(leftTy) && isRealType(rightTy))
            || (leftTy->kind() == TypeKind::Pointer
                && rightTy->kind() == TypeKind::Pointer
                && typesAreCompatible(
                    leftTy->asPointerType()->referencedType(),
                    rightTy->asPointerType()->referencedType(),
                    true)))) {
        diagReporter_.InvalidOperator(node->operatorToken());
        return Action::Quit;
    }
    ty_ = semaModel_->compilation()->canonicalBasicType(BasicTypeKind::Int_S);

    return Action::Skip;
}

SyntaxVisitor::Action TypeChecker::visitBinaryExpression_Equality(
        const BinaryExpressionSyntax* node,
        const Type* leftTy,
        const Type* rightTy)
{
    if (!((isArithmeticType(leftTy) && isArithmeticType(rightTy))
            || (leftTy->kind() == TypeKind::Pointer
                && ((rightTy->kind() == TypeKind::Pointer
                        && typesAreCompatible(
                                leftTy->asPointerType()->referencedType(),
                                rightTy->asPointerType()->referencedType(),
                                true))
                    || (rightTy->kind() == TypeKind::Basic
                        && rightTy->asBasicType()->kind() == BasicTypeKind::Int_S
                        && isNULLPointerConstant(node->right()))))
            || (false))) {
        diagReporter_.InvalidOperator(node->operatorToken());
        return Action::Quit;
    }
    ty_ = semaModel_->compilation()->canonicalBasicType(BasicTypeKind::Int_S);

    return Action::Skip;
}

SyntaxVisitor::Action TypeChecker::visitBinaryExpression_Bitwise(
        const BinaryExpressionSyntax* node,
        const Type* leftTy,
        const Type* rightTy)
{
    return Action::Skip;
}

SyntaxVisitor::Action TypeChecker::visitBinaryExpression_Logical(
        const BinaryExpressionSyntax* node,
        const Type* leftTy,
        const Type* rightTy)
{
    return Action::Skip;
}

SyntaxVisitor::Action TypeChecker::visitConditionalExpression(const ConditionalExpressionSyntax*) { return Action::Skip; }
SyntaxVisitor::Action TypeChecker::visitAssignmentExpression(const AssignmentExpressionSyntax* node) { return Action::Skip; }
SyntaxVisitor::Action TypeChecker::visitSequencingExpression(const SequencingExpressionSyntax*) { return Action::Skip; }
SyntaxVisitor::Action TypeChecker::visitExtGNU_ChooseExpression(const ExtGNU_ChooseExpressionSyntax*) { return Action::Skip; }

//------------//
// Statements //
//------------//

SyntaxVisitor::Action TypeChecker::visitCompoundStatement(const CompoundStatementSyntax* node)
{
    scope_ = semaModel_->associatedScope(node);
    PSY_ASSERT_2(scope_ != nullptr, return Action::Skip);
    for (auto stmtIt = node->statements(); stmtIt; stmtIt = stmtIt->next)
        visit(stmtIt->value);
    scope_ = scope_->outerScope();

    return Action::Skip;
}

//--------//
// Common //
//--------//

SyntaxVisitor::Action TypeChecker::visitTypeName(const TypeNameSyntax* node)
{
    auto decl = semaModel_->declaredDeclaration(node->declarator());
    PSY_ASSERT_2(decl, return Action::Quit);
    switch (decl->kind()) {
        case DeclarationKind::Function:
            ty_ = decl->asFunction()->type();
            break;
        case DeclarationKind::Object:
            ty_ = decl->asObjectDeclaration()->type();
            break;
        default:
            PSY_ASSERT_1(false);
            return Action::Quit;
    }

    return Action::Skip;
}

#undef V_
