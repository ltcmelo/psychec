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

#include "Declaration__IMPL__.inc"
#include "Declaration_Member.h"
#include "symbols/Symbol_ALL.h"
#include "types/Type_ALL.h"

#include <sstream>

using namespace psy;
using namespace C;

struct MemberDeclarationSymbol::MemberDeclarationImpl : DeclarationImpl
{
    MemberDeclarationImpl(SymbolKind symK,
                          const Symbol* containingSym,
                          const SyntaxTree* tree,
                          const Scope* enclosingScope)
        : DeclarationImpl(symK,
                          containingSym,
                          tree,
                          enclosingScope,
                          NameSpace::Members)
        , name_(nullptr)
        , ty_(nullptr)
    {}

    const Identifier* name_;
    const Type* ty_;
};

MemberDeclarationSymbol::MemberDeclarationSymbol(SymbolKind symK,
                                                 const Symbol* containingSym,
                                                 const SyntaxTree* tree,
                                                 const Scope* enclosingScope)
    : DeclarationSymbol(
          new MemberDeclarationImpl(symK,
                                    containingSym,
                                    tree,
                                    enclosingScope))
{}

MemberDeclarationSymbol::~MemberDeclarationSymbol()
{}

MemberDeclarationCategory MemberDeclarationSymbol::category() const
{
    switch (kind()) {
        case SymbolKind::Program:
        case SymbolKind::TranslationUnit:
        case SymbolKind::FunctionDeclaration:
            break;
        case SymbolKind::EnumeratorDeclaration:
            return MemberDeclarationCategory::Enumerator;
        case SymbolKind::FieldDeclaration:
            return MemberDeclarationCategory::Field;
        case SymbolKind::VariableDeclaration:
        case SymbolKind::ParameterDeclaration:
        case SymbolKind::TypedefDeclaration:
        case SymbolKind::StructDeclaration:
        case SymbolKind::UnionDeclaration:
        case SymbolKind::EnumDeclaration:
            break;
    }
    PSY_ASSERT_1(false);
    return MemberDeclarationCategory(~0);
}

const Type* MemberDeclarationSymbol::type() const
{
    return P_CAST->ty_;
}

void MemberDeclarationSymbol::setType(const Type* ty)
{
    P_CAST->ty_ = ty;
}

const Identifier* MemberDeclarationSymbol::name() const
{
    return P_CAST->name_;
}

void MemberDeclarationSymbol::setName(const Identifier* name)
{
    P_CAST->name_ = name;
}

namespace psy {
namespace C {

std::ostream& operator<<(std::ostream& os, const MemberDeclarationSymbol* membDecl)
{
    if (!membDecl)
        return os << "<MemberDeclaration is null>";
    switch (membDecl->category()) {
        case MemberDeclarationCategory::Enumerator:
            return os << membDecl->asEnumeratorDeclaration();
        case MemberDeclarationCategory::Field:
            return os << membDecl->asFieldDeclaration();
    }
    PSY_ASSERT_1(false);
    return os << "<invalid MemberDeclaration>";
}

} // C
} // psy
