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

#ifndef PSYCHE_C_SCOPE_KIND_H__
#define PSYCHE_C_SCOPE_KIND_H__

#include "API.h"
#include "Fwds.h"

#include "../common/infra/Assertions.h"

#include <cstdint>
#include <string>

namespace psy {
namespace C {

/**
 * \brief The ScopeKind enum.
 *
 * \remark 6.2.1-4
 */
enum class PSY_C_API ScopeKind : uint8_t
{
    File,
    Function,
    FunctionPrototype,
    Block
};

inline PSY_C_API std::ostream& operator<<(std::ostream& os, ScopeKind K)
{
    switch (K) {
        case ScopeKind::File:
            return os << "File";
        case ScopeKind::Function:
            return os << "Function";
        case ScopeKind::FunctionPrototype:
            return os << "FunctionPrototype";
        case ScopeKind::Block:
            return os << "Block";
    }
    PSY_ASSERT_1(false);
    return os << "<invalid ScopeKind>";
}

} // C
} // psy

#endif
