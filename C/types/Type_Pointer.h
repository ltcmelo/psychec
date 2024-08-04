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

#ifndef PSYCHE_C_TYPE_POINTER_H__
#define PSYCHE_C_TYPE_POINTER_H__

#include "API.h"
#include "Fwds.h"

#include "Type.h"

namespace psy {
namespace C {

class PSY_C_API PointerType final : public Type
{
public:
    //!@{
    /**
     * Cast \c this Type as an PointerType.
     */
    virtual PointerType* asPointerType() override { return this; }
    virtual const PointerType* asPointerType() const override { return this; }
    //!@}

    /**
     * The <em>referenced type</em> of \c this PointerType.
     *
     * \remark 6.2.5-20
     */
    const Type* referencedType() const;

    /**
     * Whether \c this PointerType arises from an array of type to pointer to type decay.
     *
     * \remark 6.7.6.3-7
     */
    bool arisesFromArrayDecay() const;

    /**
     * Whether \c this PointerType arises from a function type to pointer to function type decay.
     *
     * \remark 6.7.6.3-8
     */
    bool arisesFromFunctionDecay() const;

PSY_INTERNAL:
    PSY_GRANT_INTERNAL_ACCESS(Binder);
    PSY_GRANT_INTERNAL_ACCESS(TypeResolver);

    PointerType(const Type* refedTy);

    void markAsArisingFromArrayDecay();
    void markAsArisingFromFunctionDecay();
    void resetReferencedType(const Type*) const;

private:
    DECL_PIMPL_SUB(PointerType)
};

std::string PSY_C_API to_string(const PointerType& ptrTy);

} // C
} // psy

#endif
