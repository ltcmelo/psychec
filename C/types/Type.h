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

#ifndef PSYCHE_C_TYPE_H__
#define PSYCHE_C_TYPE_H__

#include "API.h"
#include "Fwds.h"

#include "TypeKind.h"

#include "../common/infra/AccessSpecifiers.h"
#include "../common/infra/Pimpl.h"

namespace psy {
namespace C {

/**
 * \brief The Type class.
 *
 * \note Similar to:
 * - \c clang::Type and clang::QualType of LLMV/Clang.
 * - \c Microsoft.CodeAnalysis.ITypeDeclarationSymbol of Roslyn.
 *
 * \see TypeDeclarationSymbol
 */
class PSY_C_API Type
{
public:
    virtual ~Type();

    //!@{
    /**
     * Cast \c this Type.
     */
    virtual ArrayType* asArrayType() { return nullptr; }
    virtual const ArrayType* asArrayType() const { return nullptr; }
    virtual BasicType* asBasicType() { return nullptr; }
    virtual const BasicType* asBasicType() const { return nullptr; }
    virtual FunctionType* asFunctionType() { return nullptr; }
    virtual const FunctionType* asFunctionType() const { return nullptr; }
    virtual PointerType* asPointerType() { return nullptr; }
    virtual const PointerType* asPointerType() const { return nullptr; }
    virtual TypedefType* asTypedefType() { return nullptr; }
    virtual const TypedefType* asTypedefType() const { return nullptr; }
    virtual TagType* asTagType() { return nullptr; }
    virtual const TagType* asTagType() const { return nullptr; }
    virtual VoidType* asVoidType() { return nullptr; }
    virtual const VoidType* asVoidType() const { return nullptr; }
    //!@}

    /**
     * The TypeKind of \c this Type.
     */
    TypeKind kind() const;

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

    void qualifyWithConst();
    void qualifyWithVolatile();
    void qualifyWithRestrict();
    void qualifyWithAtomic();

protected:
    DECL_PIMPL(Type);
    Type(TypeImpl* p);
    Type(const Type&) = delete;
    Type& operator=(const Type&) = delete;
};

std::string PSY_C_API to_string(const Type& ty);

} // C
} // psy

#endif
