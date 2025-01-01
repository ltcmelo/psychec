// Copyright (c) 2021/22/23/24 Leandro T. C. Melo <ltcmelo@gmail.com>
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

#include "Type_Tag.h"
#include "Type__IMPL__.inc"

#include "syntax/Lexeme_Identifier.h"

#include <iostream>
#include <sstream>

using namespace psy;
using namespace C;

struct TagType::TagTypeImpl : TypeImpl
{
    TagTypeImpl(TagTypeKind tagTyK, const Identifier* tag)
        : TypeImpl(TypeKind::Tag)
        , tag_(tag)
    {}

    const Identifier* tag_;
};

TagTypeKind TagType::kind() const
{
    return TagTypeKind(P->F_.tagTyK_);
}

const Identifier *TagType::tag() const
{
    return P_CAST->tag_;
}

TagType::TagType(TagTypeKind tagTyK, const Identifier* tag)
    : Type(new TagTypeImpl(tagTyK, tag))
{
    P->F_.tagTyK_ = static_cast<std::uint32_t>(tagTyK);
}

namespace psy {
namespace C {

std::string PSY_C_API to_string(const TagType* tagTy)
{
    if (!tagTy)
        return "<TagType is null>";
    std::ostringstream oss;
    oss << "<TagType | ";
    oss << to_string(tagTy->kind());
    oss << " tag:" << tagTy->tag()->valueText();
    oss << ">";
    return oss.str();
}

} // C
} // psy
