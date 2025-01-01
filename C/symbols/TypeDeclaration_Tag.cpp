// Copyright (c) 2024 Leandro T. C. Melo <ltcmelo@gmail.com>
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

#include "TypeDeclaration__IMPL__.inc"
#include "TypeDeclaration_Tag.h"

#include "symbols/Symbol_ALL.h"
#include "types/Type_Tag.h"

using namespace psy;
using namespace C;

TagTypeDeclaration::TagTypeDeclaration(const Symbol* containingSym,
                                       const SyntaxTree* tree,
                                       const Scope* enclosingScope,
                                       TagType* tagTy,
                                       TagTypeDeclarationKind tagTyDeclK)
    : TypeDeclaration(
          new TypeDeclarationImpl(containingSym,
                                  tree,
                                  enclosingScope,
                                  NameSpace::Tags,
                                  TypeDeclarationKind::Tag,
                                  tagTy,
                                  tagTyDeclK))
{
}

TagTypeDeclarationKind TagTypeDeclaration::kind() const
{
    return TagTypeDeclarationKind(P_CAST->BF_.tagTyDeclK_);
}

const Identifier* TagTypeDeclaration::identifier() const
{
    PSY_ASSERT_2(P_CAST->ty_->kind() == TypeKind::Tag, return nullptr);
    return P_CAST->ty_->asTagType()->tag();
}

const TagType* TagTypeDeclaration::specifiedType() const
{
    PSY_ASSERT_2(P_CAST->ty_->kind() == TypeKind::Tag, return nullptr);
    return P_CAST->ty_->asTagType();
}

namespace psy {
namespace C {

std::string PSY_C_API to_string(const TagTypeDeclaration* tagTyDecl)
{
    if (!tagTyDecl)
        return "<TagTypeDeclaration is null>";
    switch (tagTyDecl->kind()) {
        case TagTypeDeclarationKind::Struct:
            return to_string(tagTyDecl->asStruct());
        case TagTypeDeclarationKind::Union:
            return to_string(tagTyDecl->asUnion());
        case TagTypeDeclarationKind::Enum:
            return to_string(tagTyDecl->asEnum());
    }
}

} // C
} // psy
