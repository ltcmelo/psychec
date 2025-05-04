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

#include "../common/infra/Assertions.h"

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

std::vector<const Type*> FunctionType::parameterTypes() const
{
    return P_CAST->paramTys_;
}

void FunctionType::addParameterType(const Type *paramTy)
{
    P_CAST->paramTys_.push_back(paramTy);
}

void FunctionType::setParameterType(ParameterTypes::size_type idx, const Type* paramTy) const
{
    PSY_ASSERT_2(P_CAST->paramTys_.size() > idx, return);
    P_CAST->paramTys_[idx] = paramTy;
}

void FunctionType::setParameterListForm(ParameterListForm form)
{
    P_CAST->F_.parmListForm_ = static_cast<std::uint8_t>(form);
}

bool FunctionType::isVariadic() const
{
    return P_CAST->F_.isVariadic_;
}

void FunctionType::markAsVariadic()
{
    P_CAST->F_.isVariadic_ = true;
}

FunctionType::ParameterListForm FunctionType::parameterListForm() const
{
    return ParameterListForm(P_CAST->F_.parmListForm_);
}

namespace psy {
namespace C {

PSY_C_API std::ostream& operator<<(std::ostream& os, const FunctionType* funcTy)
{
    if (!funcTy)
        return os << "<FunctionType is null>";
    os << "<FunctionType | ";
    os << " return-type:" << funcTy->returnType();
    for (const auto& paramTy : funcTy->parameterTypes()) {
        os << " parameter-type:";
        os << paramTy;
    }
    os << ">";
    return os;
}

} // C
} // psy
