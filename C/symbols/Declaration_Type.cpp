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

#include "TypeDeclaration__IMPL__.inc"
#include "symbols/Symbol_ALL.h"

#include <iostream>

using namespace psy;
using namespace C;

TypeDeclarationSymbol::TypeDeclarationSymbol(TypeDeclarationImpl* p)
    : DeclarationSymbol(p)
{}

TypeDeclarationSymbol::~TypeDeclarationSymbol()
{}

TypeDeclarationCategory TypeDeclarationSymbol::category() const
{
    switch (kind()) {
        case SymbolKind::Program:
        case SymbolKind::TranslationUnit:
        case SymbolKind::FunctionDeclaration:
        case SymbolKind::EnumeratorDeclaration:
        case SymbolKind::FieldDeclaration:
        case SymbolKind::VariableDeclaration:
        case SymbolKind::ParameterDeclaration:
            break;
        case SymbolKind::TypedefDeclaration:
            return TypeDeclarationCategory::Typedef;
        case SymbolKind::StructDeclaration:
        case SymbolKind::UnionDeclaration:
        case SymbolKind::EnumDeclaration:
            return TypeDeclarationCategory::Tag;
    }
    PSY_ASSERT_1(false);
    return TypeDeclarationCategory(~0);
}

const Type* TypeDeclarationSymbol::introducedType() const
{
    return P_CAST->ty_;
}

namespace psy {
namespace C {

PSY_C_API std::ostream& operator<<(std::ostream& os, const TypeDeclarationSymbol* tyDecl)
{
    if (!tyDecl)
        return os << "<TypeDeclaration is null>";
    switch (tyDecl->category()) {
        case TypeDeclarationCategory::Tag:
            return os << tyDecl->asTagTypeDeclaration();
        case TypeDeclarationCategory::Typedef:
            return os << tyDecl->asTypedefDeclaration();
    }
    PSY_ASSERT_1(false);
    return os << "<invalid TypeDeclaration>";
}

} // C
} // psy
