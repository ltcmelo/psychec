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

#ifndef PSYCHE_C_SYMBOL_KIND_H__
#define PSYCHE_C_SYMBOL_KIND_H__

#include "API.h"
#include "Fwds.h"

#include "../common/infra/Escape.h"

#include <cstdint>
#include <string>

namespace psy {
namespace C {

/**
 * \brief The SymbolKind enum.
 *
 * \note
 * This API is inspired by that of \c Microsoft.CodeAnalysis.SymbolKind
 * from Roslyn, the .NET Compiler Platform.
 */
enum class PSY_C_API SymbolKind : std::uint8_t
{
    UNSPECIFIED = 0,

    Library,
    Function,
    Value,
    Type,
};

inline std::string PSY_C_API to_string(SymbolKind kind)
{
    switch (kind) {
        case SymbolKind::Library:
            return "Library";
        case SymbolKind::Function:
            return "Function";
        case SymbolKind::Value:
            return "Value";
        case SymbolKind::Type:
            return "Type";
        default:
            PSY_ESCAPE_VIA_RETURN("<INVALID or UNSPECIFIED SymbolKind>");
    }
}

} // C
} // psy

#endif
