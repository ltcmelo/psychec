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
        , tagTyDecl_(nullptr)
    {}

    const Identifier* tag_;
    const TagTypeDeclarationSymbol* tagTyDecl_;
};

TagType::TagType(TagTypeKind tagTyK, const Identifier* tag)
    : Type(new TagTypeImpl(tagTyK, tag))
{
    P->F_.tagTyK_ = static_cast<std::uint32_t>(tagTyK);
}

TagTypeKind TagType::kind() const
{
    return TagTypeKind(P->F_.tagTyK_);
}

const Identifier* TagType::tag() const
{
    return P_CAST->tag_;
}

bool TagType::isUntagged() const
{
    return P_CAST->tag_->valueText()[0] == '#';
}

const TagTypeDeclarationSymbol* TagType::declaration() const
{
    return P_CAST->tagTyDecl_;
}

void TagType::setDeclaration(const TagTypeDeclarationSymbol* tagTyDecl)
{
    P_CAST->tagTyDecl_ = tagTyDecl;
}

namespace psy {
namespace C {

PSY_C_API std::ostream& operator<<(std::ostream& os, const TagType* tagTy)
{
    if (!tagTy)
        return os << "<TagType is null>";
    os << "<TagType | ";
    os << tagTy->kind();
    os << " tag:" << tagTy->tag()->valueText();
    os << ">";
    return os;
}

} // C
} // psy
