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

#include <cstdint>
#include <string>

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

inline std::string PSY_C_API to_string(NameSpace ns)
{
    switch (ns) {
        case NameSpace::LabelNames:
            return "LabelNames";
        case NameSpace::Tags:
            return "Tags";
        case NameSpace::Members:
            return "Members";
        case NameSpace::OrdinaryIdentifiers:
            return "OrdinaryIdentifiers";
    }
}

} // C
} // psy

#endif
