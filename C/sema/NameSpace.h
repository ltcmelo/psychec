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

#ifndef PSYCHE_C_NAME_SPACE_H__
#define PSYCHE_C_NAME_SPACE_H__

#include "API.h"
#include "Fwds.h"

#include "../common/infra/Assertions.h"

#include <cstdint>
#include <string>
#include <iostream>

namespace psy {
namespace C {

/**
 * \brief The NameSpace enum.
 *
 * \remark 6.2.3
 */
enum class PSY_C_API NameSpace : std::uint8_t
{
    LabelNames,
    Tags,
    Members,
    OrdinaryIdentifiers,
};

inline PSY_C_API std::ostream& operator<<(std::ostream& os, NameSpace ns)
{
    switch (ns) {
        case NameSpace::LabelNames:
            return os << "LabelNames";
        case NameSpace::Tags:
            return os << "Tags";
        case NameSpace::Members:
            return os << "Members";
        case NameSpace::OrdinaryIdentifiers:
            return os << "OrdinaryIdentifiers";
    }
    PSY_ASSERT_1(false);
    return os << "<invalid namespace>";
}

} // C
} // psy

#endif
