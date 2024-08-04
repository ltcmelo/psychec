// Copyright (c) 2024 Leandro T. C. Melo <ltcmelo@gmail.com>
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

#ifndef PSYCHE_C_TYPE_QUALIFIED_H__
#define PSYCHE_C_TYPE_QUALIFIED_H__

#include "API.h"
#include "Fwds.h"

#include "Type.h"

namespace psy {
namespace C {

class PSY_C_API QualifiedType final : public Type
{
public:
    //!@{
    /**
     * Cast \c this Type as an QualifiedType.
     */
    virtual QualifiedType* asQualifiedType() override { return this; }
    virtual const QualifiedType* asQualifiedType() const override { return this; }
    //!@}

    /**
     * The Type that is the \a unqualified version of \c this QualifiedType.
     */
    const Type* unqualifiedType() const;

    /**
     * Whether \c this Type is \c const qualified.
     */
    bool isConstQualified() const;

    /**
     * Whether \c this Type is \c volatile qualified.
     */
    bool isVolatileQualified() const;

    /**
     * Whether \c this Type is \c restrict qualified.
     */
    bool isRestrictQualified() const;

    /**
     * Whether \c this Type is \c _Atomic qualified.
     */
    bool isAtomicQualified() const;

PSY_INTERNAL:
    PSY_GRANT_INTERNAL_ACCESS(Binder);
    PSY_GRANT_INTERNAL_ACCESS(TypeResolver);

    QualifiedType(const Type* unqualTy);

    void qualifyWithConst();
    void qualifyWithVolatile();
    void qualifyWithRestrict();
    void qualifyWithAtomic();

    void resetUnqualifiedType(const Type*) const;

private:
    DECL_PIMPL_SUB(QualifiedType);
};

std::string PSY_C_API to_string(const QualifiedType& qualTy);

} // C
} // psy

#endif
