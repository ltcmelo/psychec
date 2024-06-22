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

#include "Type_Basic.h"
#include "Type__IMPL__.inc"

#include <iostream>
#include <sstream>

using namespace psy;
using namespace C;

BasicType::BasicType(BasicTypeKind basicTyK)
    : Type(new TypeImpl(TypeKind::Basic))
{
    resetBasicTypeKind(basicTyK);
}

BasicTypeKind BasicType::kind() const
{
    return BasicTypeKind(P->BF_.basicTyK_);
}

void BasicType::resetBasicTypeKind(BasicTypeKind basicTyK)
{
    P->BF_.basicTyK_ = static_cast<std::uint32_t>(basicTyK);
}

namespace psy {
namespace C {

std::string PSY_C_API to_string(const BasicType& basicTy)
{
    std::ostringstream oss;
    oss << "<BasicType | ";
    oss << to_string(basicTy.kind());
    oss << ">";
    return oss.str();
}

} // C
} // psy
