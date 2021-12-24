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

#include "SymbolName_Tag.h"

#include "../common/infra/PsycheAssert.h"

using namespace psy;
using namespace C;

TagSymbolName::TagSymbolName(TypeKind tyKind, std::string tag)
    : tyKind_(tyKind)
    , tag_(std::move(tag))
{
    PSYCHE_ASSERT(tyKind == TypeKind::Struct
                    || tyKind == TypeKind::Union
                    || tyKind == TypeKind::Enum,
                  return,
                  "unexpected type kind");
}

TypeKind TagSymbolName::typeKind() const
{
    return tyKind_;
}

namespace psy {
namespace C {

std::string to_string(const TagSymbolName& name)
{
    return to_string(name.tyKind_) + " " + name.tag_;
}

bool operator==(const TagSymbolName& a, const TagSymbolName& b)
{
    return a.tyKind_ == b.tyKind_ && a.tag_ == b.tag_;
}

bool operator!=(const TagSymbolName& a, const TagSymbolName& b)
{
    return !(a == b);
}

} // C
} // psy
