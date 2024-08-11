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

#include "Type_Function.h"
#include "Type__IMPL__.inc"

#include <iostream>
#include <sstream>

using namespace psy;
using namespace C;

struct FunctionType::FunctionTypeImpl : TypeImpl
{
    FunctionTypeImpl(const Type* retTy)
        : TypeImpl(TypeKind::Function)
        , retTy_(retTy)
    {}

    const Type* retTy_;
    std::vector<const Type*> paramTys_;
};

FunctionType::FunctionType(const Type* retTy)
    : Type(new FunctionTypeImpl(retTy))
{}

const Type* FunctionType::returnType() const
{
    return P_CAST->retTy_;
}

void FunctionType::setReturnType(const Type* retTy) const
{
    P_CAST->retTy_ = retTy;
}

std::vector<const Type *> FunctionType::parameterTypes() const
{
    return P_CAST->paramTys_;
}

void FunctionType::addParameterType(const Type *paramTy)
{
    P_CAST->paramTys_.push_back(paramTy);
}

namespace psy {
namespace C {

std::string PSY_C_API to_string(const FunctionType& funcTy)
{
    std::ostringstream oss;
    oss << "<FunctionType |";
    oss << " return-type:" << to_string(*funcTy.returnType());
    for (const auto& paramTy : funcTy.parameterTypes()) {
        oss << " parameter-type:";
        oss << to_string(*paramTy);
    }
    oss << ">";
    return oss.str();
}

} // C
} // psy
