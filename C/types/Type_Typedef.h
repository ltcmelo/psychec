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

#ifndef PSYCHE_C_TYPE_TYPEDEF_H__
#define PSYCHE_C_TYPE_TYPEDEF_H__

#include "API.h"
#include "Fwds.h"

#include "Type.h"

namespace psy {
namespace C {

class PSY_C_API TypedefType final : public Type
{
public:
    //!@{
    /**
     * Cast \c this type as an TypedefType.
     */
    virtual TypedefType* asTypedefType() { return this; }
    virtual const TypedefType* asTypedefType() const { return this; }
    //!@}

    /**
     * The \a typedef-name Idnetifier of \c this TypedefType.
     */
    const Identifier* typedefName() const;

PSY_INTERNAL:
    PSY_GRANT_INTERNAL_ACCESS(Binder);

    TypedefType(const Identifier* typedefName);

private:
    DECL_PIMPL_SUB(TypedefType)
};

std::string PSY_C_API to_string(const TypedefType& ty);

} // C
} // psy

#endif
