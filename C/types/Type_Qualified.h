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

/**
 * \brief The QualifiedType class
 */
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
     * \brief The Qualifiers class
     */
    class PSY_C_API Qualifiers
    {
    public:
        /**
         * Whether \c this Qualifiers has the \c const qualifier.
         */
        bool hasConst() const;

        /**
         * Whether \c this Qualifiers has the \c volatile qualifier.
         */
        bool hasVolatile() const;

        /**
         * Whether \c this Qualifiers has the \c const restrict.
         */
        bool hasRestrict() const;

        /**
         * Whether \c this Qualifiers has the \c const _Atomic.
         */
        bool hasAtomic() const;

        /**
         * Wether \c this Qualifiers equals the \c other Qualifiers.
         */
        bool operator==(const QualifiedType::Qualifiers& other) const;

        /**
         * Wether \c this Qualifiers doesn't equal the \c other Qualifiers.
         */
        bool operator!=(const QualifiedType::Qualifiers& other) const;

    private:
        PSY_GRANT_INTERNAL_ACCESS(QualifiedType);

        Qualifiers();

        struct BD
        {
            std::uint32_t const_ : 1;
            std::uint32_t volatile_ : 1;
            std::uint32_t restrict_ : 1;
            std::uint32_t atomic_ : 1;
        };
        union
        {
            std::uint32_t BD_;
            BD F_;
        };
    };

    const Qualifiers qualifiers() const;

PSY_INTERNAL:
    PSY_GRANT_INTERNAL_ACCESS(Binder);
    PSY_GRANT_INTERNAL_ACCESS(TypeResolver);
    PSY_GRANT_INTERNAL_ACCESS(TypeChecker);

    QualifiedType(const Type* unqualTy);

    void qualifyWithConst();
    void qualifyWithVolatile();
    void qualifyWithRestrict();
    void qualifyWithAtomic();

    void resetUnqualifiedType(const Type*) const;

private:
    DECL_PIMPL_SUB(QualifiedType);
};

std::string PSY_C_API to_string(const QualifiedType* qualTy);

} // C
} // psy

#endif
