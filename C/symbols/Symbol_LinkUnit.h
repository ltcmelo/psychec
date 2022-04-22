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

#ifndef PSYCHE_C_SYMBOL_LINK_UNIT_H__
#define PSYCHE_C_SYMBOL_LINK_UNIT_H__

#include "Symbol.h"

namespace psy {
namespace C {

/**
 * \brief The LinkUnitSymbol class.
 *
 * \note
 * This API is inspired by that of \c Microsoft.CodeAnalysis.IModuleSymbol
 * from Roslyn, the .NET Compiler Platform.
 */
class PSY_C_API LinkUnitSymbol final : public Symbol
{
public:
    virtual LinkUnitSymbol* asLinkUnit() override { return this; }
    virtual const LinkUnitSymbol* asLinkUnit() const override { return this; }

private:
    friend class Binder;

    LinkUnitSymbol(const SyntaxTree* tree,
                   const Scope* outerScope,
                   const Symbol* containingSym);
};

std::string PSY_C_API to_string(const LinkUnitSymbol& sym);

} // C
} // psy

#endif
