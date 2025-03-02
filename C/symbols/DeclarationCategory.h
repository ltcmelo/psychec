// Copyright (c) 2024 Leandro T. C. Melo <ltcmelo@gmail.com>
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

#ifndef PSYCHE_C_DECLARATION_CATEGORY_H__
#define PSYCHE_C_DECLARATION_CATEGORY_H__

#include "API.h"
#include "Fwds.h"

#include "../common/infra/Assertions.h"

#include <cstdint>
#include <string>

namespace psy {
namespace C {

/**
 * \brief The DeclarationCategory enum.
 */
enum class PSY_C_API DeclarationCategory : std::uint8_t
{
    Function,
    Object,
    Member,
    Type
};

PSY_C_API inline std::ostream& operator<<(std::ostream& os, DeclarationCategory declK)
{
    switch (declK) {
        case DeclarationCategory::Function:
            return os << "Function";
        case DeclarationCategory::Object:
            return os << "Object";
        case DeclarationCategory::Member:
            return os << "Member";
        case DeclarationCategory::Type:
            return os << "Type";
    }
    PSY_ASSERT_1(false);
    return os << "<invalid DeclarationCategory>";
}

} // C
} // psy

#endif
