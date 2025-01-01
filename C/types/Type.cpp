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

std::string PSY_C_API to_string(const Type* ty)
{
    if (!ty)
        return "<Type is null>";
    switch (ty->kind()) {
        case TypeKind::Array:
            return to_string(ty->asArrayType());
        case TypeKind::Basic:
            return to_string(ty->asBasicType());
        case TypeKind::Function:
            return to_string(ty->asFunctionType());
        case TypeKind::Pointer:
            return to_string(ty->asPointerType());
        case TypeKind::Typedef:
            return to_string(ty->asTypedefType());
        case TypeKind::Tag:
            return to_string(ty->asTagType());
        case TypeKind::Void:
            return to_string(ty->asVoidType());
        case TypeKind::Qualified:
            return to_string(ty->asQualifiedType());
    }
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

const Type* unqualifyType(const Type* ty)
{
    PSY_ASSERT_2(ty, return nullptr);
    return ty->kind() == TypeKind::Qualified
            ? ty->asQualifiedType()->unqualifiedType()
            : ty;
}

} // C
} // psy
