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

#ifndef PSYCHE_C_SYMBOL_ARRAY_TYPE_H__
#define PSYCHE_C_SYMBOL_ARRAY_TYPE_H__

#include "Symbol_Type.h"

namespace psy {
namespace C {

/**
 * \brief The ArrayTypeSymbol class.
 *
 * \remark 6.2.5-20
 *
 * \note
 * This API is inspired by that of \c Microsoft.CodeAnalysis.IArrayTypeSymbol
 * from Roslyn, the .NET Compiler Platform.
 */
class PSY_C_API ArrayTypeSymbol final : public TypeSymbol
{
public:
    //!@{
    /**
     * Cast \c this TypeSymbol as a ArrayTypeSymbol.
     */
    virtual ArrayTypeSymbol* asArrayType() override { return this; }
    virtual const ArrayTypeSymbol* asArrayType() const override { return this; }
    //!@}

    /**
     * The <em>element type</em> of \c this ArrayTypeSymbol.
     *
     * \remark 6.2.5-20
     */
    const TypeSymbol* elementType() const;

PSY_INTERNAL_AND_RESTRICTED:
    PSY_GRANT_ACCESS(Binder);

    ArrayTypeSymbol(const SyntaxTree* tree,
                    const Scope* scope,
                    const Symbol* containingSym,
                    const TypeSymbol* elemTySym);

private:
    DECL_PIMPL_SUB(ArrayTypeSymbol)
};

std::string PSY_C_API to_string(const ArrayTypeSymbol& tySym);

} // C
} // psy

#endif
