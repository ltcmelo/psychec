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

Type::Type(TypeImpl* p)
    : P(p)
{}

Type::~Type()
{}

TypeKind Type::kind() const
{
    return TypeKind(P->BF_.tyK_);
}

bool Type::isConstQualified() const
{
    return P->BF_.const_;
}

bool Type::isVolatileQualified() const
{
    return P->BF_.volatile_;
}

bool Type::isRestrictQualified() const
{
    return P->BF_.restrict_;
}

bool Type::isAtomicQualified() const
{
    return P->BF_.atomic_;
}

void Type::qualifyWithConst()
{
    P->BF_.const_ = 1;
}

void Type::qualifyWithVolatile()
{
    P->BF_.volatile_ = 1;
}

void Type::qualifyWithRestrict()
{
    P->BF_.restrict_ = 1;
}

void Type::qualifyWithAtomic()
{
    P->BF_.atomic_ = 1;
}

namespace psy {
namespace C {

std::string PSY_C_API to_string(const Type& ty)
{
    switch (ty.kind()) {
        case TypeKind::Array:
            return to_string(*ty.asArrayType());
        case TypeKind::Basic:
            return to_string(*ty.asBasicType());
        case TypeKind::Function:
            return to_string(*ty.asFunctionType());
        case TypeKind::Pointer:
            return to_string(*ty.asPointerType());
        case TypeKind::Typedef:
            return to_string(*ty.asTypedefType());
        case TypeKind::Tag:
            return to_string(*ty.asTagType());
        case TypeKind::Void:
            return to_string(*ty.asVoidType());
    }
}

} // C
} // psy
