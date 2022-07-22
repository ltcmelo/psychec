// Copyright (c) 2021/22 Leandro T. C. Melo <ltcmelo@gmail.com>
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

#ifndef PSYCHE_C_TYPE_SYMBOL_H__
#define PSYCHE_C_TYPE_SYMBOL_H__

#include "API.h"
#include "Fwds.h"

#include "TypeKind.h"
#include "Symbol.h"

#include "../common/infra/InternalAccess.h"

#include <memory>

namespace psy {
namespace C {

/**
 * \brief The TypeSymbol class.
 *
 * \note
 * This API is inspired by that of \c Microsoft.CodeAnalysis.ITypeSymbol
 * from Roslyn, the .NET Compiler Platform.
 *
 * \note
 * Influence by the API of Clang/LLVM is present as well; specifically:
 * \c clang::Type and \c clang::QualType.
 */
class PSY_C_API TypeSymbol : public Symbol
{
public:
    virtual ~TypeSymbol();

    //!@{
    /**
     * Cast \c this Symbol as a TypeSymbol.
     */
    virtual TypeSymbol* asType() override { return this; }
    virtual const TypeSymbol* asType() const override { return this; }
    //!@}

    //!@{
    /**
     * Cast \c this TypeSymbol.
     */
    virtual ArrayTypeSymbol* asArrayType() { return nullptr; }
    virtual const ArrayTypeSymbol* asArrayType() const { return nullptr; }
    virtual FunctionTypeSymbol* asFunctionType() { return nullptr; }
    virtual const FunctionTypeSymbol* asFunctionType() const { return nullptr; }
    virtual NamedTypeSymbol* asNamedType() { return nullptr; }
    virtual const NamedTypeSymbol* asNamedType() const { return nullptr; }
    virtual PointerTypeSymbol* asPointerType() { return nullptr; }
    virtual const PointerTypeSymbol* asPointerType() const { return nullptr; }
    //!@}

    /**
     * The TypeKind of \c this type.
     */
    TypeKind typeKind() const;

    /**
     * Whether the type is \c const qualified.
     */
    bool isConstQualified() const;

    /**
     * Whether the type is \c volatile qualified.
     */
    bool isVolatileQualified() const;

    /**
     * Whether the type is \c restrict qualified.
     */
    bool isRestrictQualified() const;

PSY_INTERNAL_AND_RESTRICTED:
    PSY_GRANT_ACCESS(SemanticsOfTypeQualifiers);

    void qualifyWithConst();
    void qualifyWithVolatile();
    void qualifyWithRestrict();

protected:
    DECL_PIMPL_SUB(TypeSymbol);

    TypeSymbol(TypeSymbolImpl* p);
};

std::string PSY_C_API to_string(const TypeSymbol& tySym);

} // C
} // psy

#endif
