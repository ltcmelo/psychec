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
#include "Type_Pointer.h"

#include <iostream>
#include <sstream>

using namespace psy;
using namespace C;

struct PointerType::PointerTypeImpl : TypeImpl
{
    PointerTypeImpl(const Type* refedTy)
        : TypeImpl(TypeKind::Pointer)
        , refedTy_(refedTy)
    {}

    const Type* refedTy_;
};

PointerType::PointerType(const Type* refedTy)
    : Type(new PointerTypeImpl(refedTy))
{}

void PointerType::resetReferencedType(const Type* refedTy) const
{
    P_CAST->refedTy_ = refedTy;
}

const Type* PointerType::referencedType() const
{
    return P_CAST->refedTy_;
}

bool PointerType::arisesFromArrayDecay() const
{
    return P->F_.arrayDecay_;
}

bool PointerType::arisesFromFunctionDecay() const
{
    return P->F_.funcDecay_;
}

void PointerType::markAsArisingFromArrayDecay()
{
    P->F_.arrayDecay_ = true;
}

void PointerType::markAsArisingFromFunctionDecay()
{
    P->F_.funcDecay_ = true;
}

namespace psy {
namespace C {

PSY_C_API std::ostream& operator<<(std::ostream& os, const PointerType* ptrTy)
{
    if (!ptrTy)
        return os << "<PointerType is null>";
    os << "<PointerType | ";
    os << " referenced-type:" << ptrTy->referencedType();
    os << ">";
    return os;
}

} // C
} // psy
