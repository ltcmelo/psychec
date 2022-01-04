// Copyright (c) 2021 Leandro T. C. Melo <ltcmelo@gmail.com>
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
#include "APIFwds.h"
#include "TypeKind.h"
#include "Symbol.h"

#include <memory>

namespace psy {
namespace C {

/**
 * \brief The TypeSymbol class.
 *
 * \note
 * This API is inspired by that of \c Microsoft.CodeAnalysis.ITypeSymbol
 * from Roslyn, the .NET Compiler Platform.
 */
class PSY_C_API TypeSymbol : public Symbol
{
public:
    virtual ~TypeSymbol();

    virtual TypeSymbol* asType() override { return this; }
    virtual const TypeSymbol* asType() const override { return this; }

    /**
     * The TypeKind of \c this type.
     */
    TypeKind typeKind() const;

    virtual ArrayTypeSymbol* asArrayType() { return nullptr; }
    virtual const ArrayTypeSymbol* asArrayType() const { return nullptr; }
    virtual NamedTypeSymbol* asNamedType() { return nullptr; }
    virtual const NamedTypeSymbol* asNamedType() const { return nullptr; }
    virtual PointerTypeSymbol* asPointerType() { return nullptr; }
    virtual const PointerTypeSymbol* asPointerType() const { return nullptr; }

protected:
    DECL_PIMPL_SUB(TypeSymbol);

    TypeSymbol(TypeSymbolImpl* p);
    TypeSymbol(const SyntaxTree* tree,
               const Scope* outerScope,
               const Symbol* containingSym,
               TypeKind tyKind);
};

} // C
} // psy

#endif
