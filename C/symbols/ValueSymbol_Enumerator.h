// Copyright (c) 2022 Leandro T. C. Melo <ltcmelo@gmail.com>
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

#ifndef PSYCHE_C_SYMBOL_ENUMERATOR_H__
#define PSYCHE_C_SYMBOL_ENUMERATOR_H__

#include "Symbol_Value.h"

namespace psy {
namespace C {

/**
 * \brief The EnumeratorSymbol class.
 */
class PSY_C_API EnumeratorSymbol final : public ValueSymbol
{
public:
    //!@{
    /**
     * Cast \c this ValueSymbol as a EnumeratorSymbol.
     */
    virtual EnumeratorSymbol* asEnumerator() { return this; }
    virtual const EnumeratorSymbol* asEnumerator() const { return this; }
    //!@}

PSY_INTERNAL_AND_RESTRICTED:
    PSY_GRANT_ACCESS(Binder);

    EnumeratorSymbol(const SyntaxTree* tree,
                     const Scope* scope,
                     const Symbol* containingSym);
};

std::string PSY_C_API to_string(const EnumeratorSymbol& sym);

} // C
} // psy

#endif
