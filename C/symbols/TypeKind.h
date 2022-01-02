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

#ifndef PSYCHE_C_TYPE_KIND_H__
#define PSYCHE_C_TYPE_KIND_H__

#include "API.h"
#include "APIFwds.h"

#include <cstdint>
#include <string>

namespace psy {
namespace C {

/**
 * \brief The TypeKind enum.
 *
 * \note
 * This API is inspired by that of \c Microsoft.CodeAnalysis.TypeKind
 * from Roslyn, the .NET Compiler Platform.
 */
enum class TypeKind : std::uint8_t
{
    None = 0,
    Builtin,
    Array,
    Enum,
    Pointer,
    Struct,
    Union,
    Synonym,
};

inline std::string PSY_C_API to_string(TypeKind tyKind)
{
    switch (tyKind) {
        case TypeKind::Builtin:
            return "Builtin";
        case TypeKind::Array:
            return "Array";
        case TypeKind::Enum:
            return "Enum";
        case TypeKind::Pointer:
            return "Pointer";
        case TypeKind::Struct:
            return "Struct";
        case TypeKind::Union:
            return "Union";
        case TypeKind::Synonym:
            return "Synonym";

        default:
            return "<unknown type kind>";
    }
}

} // C
} // psy

#endif
