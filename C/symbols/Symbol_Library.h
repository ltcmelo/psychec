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

#ifndef PSYCHE_C_SYMBOL_LINK_UNIT_H__
#define PSYCHE_C_SYMBOL_LINK_UNIT_H__

#include "Symbol.h"

#include "../common/infra/InternalAccess.h"

namespace psy {
namespace C {

/**
 * \brief The LibrarySymbol class.
 *
 * A symbol that denotes a library of objects and function from a source file.
 *
 * \attention
 * This symbol does not denote a <em>translation unit</em>.
 *
 * \remark 5.1.1.2-1 (8)
 *
 * \note
 * This API is inspired by that of \c Microsoft.CodeAnalysis.IModuleSymbol
 * from Roslyn, the .NET Compiler Platform.
 */
class PSY_C_API LibrarySymbol final : public Symbol
{
public:
    //!@{
    /**
     * Cast \c this Symbol as a LibrarySymbol.
     */
    virtual LibrarySymbol* asLibrary() override { return this; }
    virtual const LibrarySymbol* asLibrary() const override { return this; }
    //!@}

PSY_INTERNAL_AND_RESTRICTED:
    PSY_GRANT_ACCESS(Binder);

    LibrarySymbol(const SyntaxTree* tree,
                  const Scope* scope,
                  const Symbol* containingSym);
};

std::string PSY_C_API to_string(const LibrarySymbol& sym);

} // C
} // psy

#endif
