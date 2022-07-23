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

#ifndef PSYCHE_C_SYMBOL_POINTER_TYPE_H__
#define PSYCHE_C_SYMBOL_POINTER_TYPE_H__

#include "Symbol_Type.h"

namespace psy {
namespace C {

/**
 * \brief The PointerTypeSymbol class.
 *
 * \remark 6.2.5-20
 *
 * \note
 * This API is inspired by that of \c Microsoft.CodeAnalysis.IPointerTypeSymbol
 * from Roslyn, the .NET Compiler Platform.
 */
class PSY_C_API PointerTypeSymbol final : public TypeSymbol
{
public:
    //!@{
    /**
     * Cast \c this TypeSymbol as a PointerTypeSymbol.
     */
    virtual PointerTypeSymbol* asPointerType() override { return this; }
    virtual const PointerTypeSymbol* asPointerType() const override { return this; }
    //!@}

    /**
     * The <em>referenced type</em> of \c this PointerTypeSymbol.
     *
     * \remark 6.2.5-20
     */
    const TypeSymbol* referencedType() const;

    /**
     * Whether \c this PointerTypeSymbol arises from an array of type to pointer to type decay.
     *
     * \remark 6.7.6.3-7
     */
    bool arisesFromArrayDecay() const;

    /**
     * Whether \c this PointerTypeSymbol arises from a function type to pointer to function type decay.
     *
     * \remark 6.7.6.3-8
     */
    bool arisesFromFunctionDecay() const;

PSY_INTERNAL_AND_RESTRICTED:
    PSY_GRANT_ACCESS(Binder);

    PointerTypeSymbol(const SyntaxTree* tree,
                      const Scope* scope,
                      const Symbol* containingSym,
                      const TypeSymbol* refedTySym);

    void markAsArisingFromArrayDecay();
    void markAsArisingFromFunctionDecay();

private:
    DECL_PIMPL_SUB(PointerTypeSymbol)
};

std::string PSY_C_API to_string(const PointerTypeSymbol& tySym);

} // C
} // psy

#endif
