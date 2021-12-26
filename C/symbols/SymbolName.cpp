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

#include "SymbolName.h"

#include "SymbolNames.h"

#include "common/infra/PsycheAssert.h"

using namespace psy;
using namespace C;

SymbolName::SymbolName()
{}

SymbolName::~SymbolName()
{}

SymbolName::Kind SymbolName::kind() const
{
    if (asPlainSymbolName())
        return Kind::Plain;
    if (asTagSymbolName())
        return Kind::Tag;
    if (asEmptySymbolName())
        return Kind::Empty;

    PSYCHE_ASSERT(false, return Kind::Plain, "");
}

namespace psy {
namespace C {

std::string to_string(const SymbolName& name)
{
    switch (name.kind()) {
        case SymbolName::Kind::Plain:
            return to_string(static_cast<const PlainSymbolName&>(name));

        case SymbolName::Kind::Tag:
            return to_string(static_cast<const TagSymbolName&>(name));

        default:
            PSYCHE_FAIL(return "", "unknown symbol name");
            return "";
    }
}

bool operator==(const SymbolName& a, const SymbolName& b)
{
    // TODO
    return false;
}

bool operator!=(const SymbolName& a, const SymbolName& b)
{
    return !(a == b);
}

} // C
} // psy
