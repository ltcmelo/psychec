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

#include "../common/infra/Escape.h"

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
    UNSPECIFIED = 0,

    File,
    Function,
    FunctionPrototype,
    Block
};

inline std::string PSY_C_API to_string(ScopeKind scopeK)
{
    switch (scopeK) {
        case ScopeKind::File:
            return "File";
        case ScopeKind::Function:
            return "Function";
        case ScopeKind::FunctionPrototype:
            return "FunctionPrototype";
        case ScopeKind::Block:
            return "Block";
        default:
            PSY_ESCAPE_VIA_RETURN("<INVALID or UNSPECIFIED ScopeKind>");
    }
}

} // C
} // psy

#endif
