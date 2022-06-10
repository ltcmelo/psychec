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

TagSymbolName::TagSymbolName(TagChoice tagChoice, std::string tag)
    : tagChoice_(tagChoice)
    , tag_(std::move(tag))
{}

TagSymbolName::TagChoice TagSymbolName::tagChoice() const
{
    return tagChoice_;
}

std::string TagSymbolName::text() const
{
    std::string prefix;
    switch (tagChoice_) {
        case TagChoice::Struct:
            prefix = "struct ";
            break;
        case TagChoice::Union:
            prefix = "union ";
            break;
        case TagChoice::Enum:
            prefix = "enum ";
            break;
        default:
            PSY_ESCAPE_VIA_RETURN("");
            return "<INVALID or UNSPECIFIED TagChoice>";
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
