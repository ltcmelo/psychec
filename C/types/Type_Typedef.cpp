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

#include "Type_Typedef.h"
#include "Type__IMPL__.inc"

#include "syntax/Lexeme_Identifier.h"

#include <iostream>
#include <sstream>

using namespace psy;
using namespace C;

struct TypedefType::TypedefTypeImpl : TypeImpl
{
    TypedefTypeImpl(const Identifier* typedefName)
        : TypeImpl(TypeKind::Typedef)
        , tydefName_(typedefName)
        , tydef_(nullptr)
    {}

    const Identifier* tydefName_;
    const Typedef* tydef_;
};

const Identifier *TypedefType::typedefName() const
{
    return P_CAST->tydefName_;
}

TypedefType::TypedefType(const Identifier* typedefName)
    : Type(new TypedefTypeImpl(typedefName))
{}

const Typedef* TypedefType::declaration() const
{
    return P_CAST->tydef_;
}

void TypedefType::setDeclaration(const Typedef* tydef)
{
    P_CAST->tydef_ = tydef;
}

namespace psy {
namespace C {

std::string PSY_C_API to_string(const TypedefType* tydefTy)
{
    if (!tydefTy)
        return "<TypedefType is null>";
    std::ostringstream oss;
    oss << "<TypedefType | ";
    oss << "typedef-name:" << tydefTy->typedefName()->valueText();
    oss << ">";
    return oss.str();
}

} // C
} // psy
