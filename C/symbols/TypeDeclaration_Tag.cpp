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

TagDeclarationSymbol::TagDeclarationSymbol(
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

TagDeclarationCategory TagDeclarationSymbol::category() const
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
        case SymbolKind::UnionDeclaration:
            return TagDeclarationCategory::StructOrUnion;
        case SymbolKind::EnumDeclaration:
            return TagDeclarationCategory::Enum;
    }
    PSY_ASSERT_1(false);
    return TagDeclarationCategory(~0);
}

const Identifier* TagDeclarationSymbol::denotingIdentifier() const
{
    PSY_ASSERT_2(introducedType()->kind() == TypeKind::Tag, return nullptr);
    return P_CAST->ty_->asTagType()->tag();
}

void TagDeclarationSymbol::addMember(const MemberDeclarationSymbol* memb)
{
    membDecls_.push_back(memb);
}

const MemberDeclarationSymbol* TagDeclarationSymbol::member(const Identifier* name) const
{
    auto it = std::find_if(membDecls_.begin(),
                           membDecls_.end(),
                           [name] (const MemberDeclarationSymbol* membDecl) {
                                return membDecl->name() == name;
                           });
    if (it != membDecls_.end())
        return *it;

    for (const auto membDecl : membDecls_) {
        if (membDecl->kind() != SymbolKind::FieldDeclaration)
            continue;
        auto fldDecl = membDecl->asFieldDeclaration();
        if (fldDecl->isAnonymousStructureOrUnion()) {
            PSY_ASSERT_2(fldDecl->type()->kind() == TypeKind::Tag, continue);
            auto tagTyDecl = fldDecl->type()->asTagType()->declaration();
            if (!tagTyDecl)
                continue;
            auto innerMembDecl = tagTyDecl->member(name);
            if (innerMembDecl)
                return innerMembDecl;
        }
    }
    return nullptr;
}

TagDeclarationSymbol::Members TagDeclarationSymbol::members() const
{
    return membDecls_;
}

const TagType* TagDeclarationSymbol::introducedNewType() const
{
    PSY_ASSERT_2(introducedType()->kind() == TypeKind::Tag, return nullptr);
    return P_CAST->ty_->asTagType();
}

namespace psy {
namespace C {

PSY_C_API std::ostream& operator<<(std::ostream& os, const TagDeclarationSymbol* tagTyDecl)
{
    if (!tagTyDecl)
        return os << "<TagTypeDeclaration is null>";
    switch (tagTyDecl->category()) {
        case TagDeclarationCategory::StructOrUnion:
            return os << tagTyDecl->asStructOrUnionDeclaration();
        case TagDeclarationCategory::Enum:
            return os << tagTyDecl->asEnumDeclaration();
    }
    PSY_ASSERT_1(false);
    return os << "<invalid TagTypeDeclaration>";
}

} // C
} // psy
