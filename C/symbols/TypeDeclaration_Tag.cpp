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

#include <algorithm>

using namespace psy;
using namespace C;

TagTypeDeclarationSymbol::TagTypeDeclarationSymbol(
        SymbolKind symK,
        const Symbol* containingSym,
        const SyntaxTree* tree,
        const Scope* enclosingScope,
        TagType* tagTy)
    : TypeDeclarationSymbol(
          new TypeDeclarationImpl(symK,
                                  containingSym,
                                  tree,
                                  enclosingScope,
                                  NameSpace::Tags,
                                  tagTy))
{
    tagTy->setDeclaration(this);
}

TagTypeDeclarationCategory TagTypeDeclarationSymbol::category() const
{
    switch (kind()) {
        case SymbolKind::Program:
        case SymbolKind::TranslationUnit:
        case SymbolKind::FunctionDeclaration:
        case SymbolKind::EnumeratorDeclaration:
        case SymbolKind::FieldDeclaration:
        case SymbolKind::VariableDeclaration:
        case SymbolKind::ParameterDeclaration:
        case SymbolKind::TypedefDeclaration:
            break;
        case SymbolKind::StructDeclaration:
            return TagTypeDeclarationCategory::Struct;
        case SymbolKind::UnionDeclaration:
            return TagTypeDeclarationCategory::Union;
        case SymbolKind::EnumDeclaration:
            return TagTypeDeclarationCategory::Enum;
    }
    PSY_ASSERT_1(false);
    return TagTypeDeclarationCategory(~0);
}

const Identifier* TagTypeDeclarationSymbol::denotingIdentifier() const
{
    PSY_ASSERT_2(introducedType()->kind() == TypeKind::Tag, return nullptr);
    return P_CAST->ty_->asTagType()->tag();
}

void TagTypeDeclarationSymbol::addMember(const MemberDeclarationSymbol* memb)
{
    membs_.push_back(memb);
}

const MemberDeclarationSymbol* TagTypeDeclarationSymbol::member(const Identifier* name) const
{
    auto it = std::find_if(membs_.begin(),
                           membs_.end(),
                           [name] (const MemberDeclarationSymbol* memb) {
                                return memb->name() == name;
                           });
    return it != membs_.end() ? *it : nullptr;
}

const TagType* TagTypeDeclarationSymbol::introducedNewType() const
{
    PSY_ASSERT_2(introducedType()->kind() == TypeKind::Tag, return nullptr);
    return P_CAST->ty_->asTagType();
}

namespace psy {
namespace C {

PSY_C_API std::ostream& operator<<(std::ostream& os, const TagTypeDeclarationSymbol* tagTyDecl)
{
    if (!tagTyDecl)
        return os << "<TagTypeDeclaration is null>";
    switch (tagTyDecl->category()) {
        case TagTypeDeclarationCategory::Struct:
            return os << tagTyDecl->asStructDeclaration();
        case TagTypeDeclarationCategory::Union:
            return os << tagTyDecl->asUnionDeclaration();
        case TagTypeDeclarationCategory::Enum:
            return os << tagTyDecl->asEnumDeclaration();
    }
    PSY_ASSERT_1(false);
    return os << "<invalid TagTypeDeclaration>";
}

} // C
} // psy
