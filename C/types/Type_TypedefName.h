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

#ifndef PSYCHE_C_TYPE_TYPEDEF_NAME_H__
#define PSYCHE_C_TYPE_TYPEDEF_NAME_H__

#include "API.h"
#include "Fwds.h"

#include "Type.h"

namespace psy {
namespace C {

/**
 * \brief The TypedefNameType class.
 */
class PSY_C_API TypedefNameType final : public Type
{
public:
    //!@{
    /**
     * Cast \c this Type as a TypedefNameType.
     */
    virtual TypedefNameType* asTypedefNameType() { return this; }
    virtual const TypedefNameType* asTypedefNameType() const { return this; }
    //!@}

    /**
     * The \a typedef-name of \c this TypedefNameType.
     */
    const Identifier* typedefName() const;

    /**
     * The (resolved) \a synonymized \a type of \c this TypedefNameType.
     */
    const Type* resolvedSynonymizedType() const;

    /**
     * The \a declaration of \c this TypedefNameType, if one exists.
     */
    const TypedefDeclarationSymbol* declaration() const;

PSY_INTERNAL:
    PSY_GRANT_INTERNAL_ACCESS(DeclarationBinder);
    PSY_GRANT_INTERNAL_ACCESS(TypeCanonicalizer);
    PSY_GRANT_INTERNAL_ACCESS(TypedefNameTypeResolver);
    PSY_GRANT_INTERNAL_ACCESS(TypedefDeclarationSymbol);
    PSY_GRANT_INTERNAL_ACCESS(TypeChecker);

    TypedefNameType(const Identifier* typedefName);

    void setDeclaration(const TypedefDeclarationSymbol* tydef);
    void setResolvedSynonymizedType(const Type* ty);

private:
    DECL_PIMPL_SUB(TypedefNameType)
};

PSY_C_API std::ostream& operator<<(std::ostream& os, const TypedefNameType* tydefNameTy);

} // C
} // psy

#endif
