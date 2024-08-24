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

#ifndef PSYCHE_C_TAG_TYPE_DECLARATION_KIND_H__
#define PSYCHE_C_TAG_TYPE_DECLARATION_KIND_H__

#include "API.h"
#include "Fwds.h"

#include <cstdint>
#include <string>

namespace psy {
namespace C {

/**
 * \brief The TagTypeDeclarationSymbolKind enum.
 */
enum class PSY_C_API TagTypeDeclarationSymbolKind : std::uint8_t
{
    Struct,
    Union,
    Enum,
};

inline std::string PSY_C_API to_string(TagTypeDeclarationSymbolKind tagTyDeclK)
{
    switch (tagTyDeclK) {
        case TagTypeDeclarationSymbolKind::Struct:
            return "Struct";
        case TagTypeDeclarationSymbolKind::Union:
            return "Union";
        case TagTypeDeclarationSymbolKind::Enum:
            return "Enum";
    }
}

} // C
} // psy

#endif
