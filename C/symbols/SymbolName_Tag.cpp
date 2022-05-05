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

#include "SymbolName_Tag.h"

#include "../common/infra/Assertions.h"

using namespace psy;
using namespace C;

TagSymbolName::TagSymbolName(TagSymbolNameKind tagK, std::string tag)
    : tagK_(tagK)
    , tag_(std::move(tag))
{}

TagSymbolNameKind TagSymbolName::kind() const
{
    return tagK_;
}

std::string TagSymbolName::text() const
{
    std::string prefix;
    switch (tagK_) {
        case TagSymbolNameKind::Structure:
            prefix = "struct ";
            break;
        case TagSymbolNameKind::Union:
            prefix = "union ";
            break;
        case TagSymbolNameKind::Enumeration:
            prefix = "enum ";
            break;
        default:
            PSY_TRACE_ESCAPE_0(return "");
            return "<INVALID or UNSPECIFIED TagSymbolNameKind>";
    }
    return prefix + tag_;
}

namespace psy {
namespace C {

std::string to_string(const TagSymbolName& name)
{
    return name.text();
}

bool operator==(const TagSymbolName& a, const TagSymbolName& b)
{
    return a.kind() == b.kind() && a.tag_ == b.tag_;
}

bool operator!=(const TagSymbolName& a, const TagSymbolName& b)
{
    return !(a == b);
}

} // C
} // psy
