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

#include "Type_TypedefName.h"
#include "Type__IMPL__.inc"

#include "syntax/Lexeme_Identifier.h"

#include <iostream>
#include <sstream>

using namespace psy;
using namespace C;

struct TypedefNameType::TypedefNameTypeImpl : TypeImpl
{
    TypedefNameTypeImpl(const Identifier* typedefName)
        : TypeImpl(TypeKind::TypedefName)
        , tydefName_(typedefName)
        , tydef_(nullptr)
        , actualTy_(nullptr)
    {}

    const Identifier* tydefName_;
    const TypedefDeclarationSymbol* tydef_;
    const Type* actualTy_;
};

const Identifier* TypedefNameType::typedefName() const
{
    return P_CAST->tydefName_;
}

TypedefNameType::TypedefNameType(const Identifier* typedefName)
    : Type(new TypedefNameTypeImpl(typedefName))
{}

const TypedefDeclarationSymbol* TypedefNameType::declaration() const
{
    return P_CAST->tydef_;
}

void TypedefNameType::setDeclaration(const TypedefDeclarationSymbol* tydef)
{
    P_CAST->tydef_ = tydef;
}

const Type* TypedefNameType::resolvedSynonymizedType() const
{
    return P_CAST->actualTy_;
}

void TypedefNameType::setResolvedSynonymizedType(const Type* ty)
{
    P_CAST->actualTy_ = ty;
}

namespace psy {
namespace C {

PSY_C_API std::ostream& operator<<(std::ostream& os, const TypedefNameType* tydefNameTy)
{
    if (!tydefNameTy)
        return os << "<TypedefNameType is null>";
    os << "<TypedefNameType | ";
    os << "typedef-name:" << tydefNameTy->typedefName()->valueText();
    os << ">";
    return os;
}

} // C
} // psy
