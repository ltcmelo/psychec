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

#include "TypeSymbol__IMPL__.inc"

#include "symbols/Symbol_ALL.h"

#include <sstream>

using namespace psy;
using namespace C;

TypeSymbol::TypeSymbol(TypeSymbolImpl* p)
    : Symbol(p)
{}

TypeSymbol::~TypeSymbol()
{}

TypeKind TypeSymbol::typeKind() const
{
    return P_CAST->tyKind_;
}

bool TypeSymbol::isConstQualified() const
{
    return P->BF_.const_;
}

bool TypeSymbol::isVolatileQualified() const
{
    return P->BF_.volatile_;
}

bool TypeSymbol::isRestrictQualified() const
{
    return P->BF_.restrict_;
}

void TypeSymbol::qualifyWithConst()
{
    P->BF_.const_ = 1;
}

void TypeSymbol::qualifyWithVolatile()
{
    P->BF_.volatile_ = 1;
}

void TypeSymbol::qualifyWithRestrict()
{
    P->BF_.restrict_ = 1;
}

namespace psy {
namespace C {

std::string PSY_C_API to_string(const TypeSymbol& tySym)
{
    switch (tySym.typeKind()) {
        case TypeKind::Array:
            return to_string(*tySym.asArrayType());
        case TypeKind::Function:
            return to_string(*tySym.asFunctionType());
        case TypeKind::Named:
            return to_string(*tySym.asNamedType());
        case TypeKind::Pointer:
            return to_string(*tySym.asPointerType());
        default:
            PSY_ESCAPE_VIA_RETURN("");
            return "<unknown type kind>";
    }
}

} // C
} // psy
