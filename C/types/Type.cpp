// Copyright (c) 2021/22/23/24 Leandro T. C. Melo <ltcmelo@gmail.com>
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

#include "Type__IMPL__.inc"
#include "Type_ALL.h"

#include "../common/infra/Assertions.h"

Type::Type(TypeImpl* p)
    : P(p)
{}

Type::~Type()
{}

TypeKind Type::kind() const
{
    return TypeKind(P->F_.tyK_);
}

namespace psy {
namespace C {

PSY_C_API std::ostream& operator<<(std::ostream& os, const Type* ty)
{
    if (!ty)
        return os << "<Type is null>";
    switch (ty->kind()) {
        case TypeKind::Array:
            return os << ty->asArrayType();
        case TypeKind::Basic:
            return os << ty->asBasicType();
        case TypeKind::Function:
            return os << ty->asFunctionType();
        case TypeKind::Pointer:
            return os << ty->asPointerType();
        case TypeKind::TypedefName:
            return os << ty->asTypedefNameType();
        case TypeKind::Tag:
            return os << ty->asTagType();
        case TypeKind::Void:
            return os << ty->asVoidType();
        case TypeKind::Qualified:
            return os << ty->asQualifiedType();
        case TypeKind::Error:
            return os << ty->asErrorType();
    }
    PSY_ASSERT_1(false);
    return os << "<invalid Type>";
}

bool isArithmeticType(const Type* ty)
{
    PSY_ASSERT_2(ty, return false);
    return ty->kind() == TypeKind::Basic;
}

bool isRealType(const Type* ty)
{
    PSY_ASSERT_2(ty, return false);
    return ty->kind() == TypeKind::Basic
            && isRealTypeKind(ty->asBasicType()->kind());
}

bool isSignedIntegerType(const Type* ty)
{
    PSY_ASSERT_2(ty, return false);
    return ty->kind() == TypeKind::Basic
            && isSignedIntegerTypeKind(ty->asBasicType()->kind());
}

bool isUnsignedIntegerType(const Type* ty)
{
    PSY_ASSERT_2(ty, return false);
    return ty->kind() == TypeKind::Basic
            && isUnsignedIntegerTypeKind(ty->asBasicType()->kind());
}

bool isIntegerType(const Type* ty)
{
    PSY_ASSERT_2(ty, return false);
    return ty->kind() == TypeKind::Basic
            && isIntegerTypeKind(ty->asBasicType()->kind());
}

bool isScalarType(const Type* ty)
{
    PSY_ASSERT_2(ty, return false);
    return isArithmeticType(ty) || ty->kind() == TypeKind::Pointer;
}

bool isStructureOrUnionType(const Type* ty)
{
    PSY_ASSERT_2(ty, return false);
    return ty->kind() == TypeKind::Tag
            && isStructureOrUnionTypeKind(ty->asTagType()->kind());
}

} // C
} // psy
