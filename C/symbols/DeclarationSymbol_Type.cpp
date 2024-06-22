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

#include "TypeDeclarationSymbol__IMPL__.inc"

#include "symbols/Symbol_ALL.h"

#include <sstream>

using namespace psy;
using namespace C;

TypeDeclarationSymbol::TypeDeclarationSymbol(TypeDeclarationSymbolImpl* p)
    : DeclarationSymbol(p, DeclarationSymbolKind::Type)
{}

TypeDeclarationSymbol::~TypeDeclarationSymbol()
{}

TypeDeclarationSymbolKind TypeDeclarationSymbol::kind() const
{
    return TypeDeclarationSymbolKind(P_CAST->BF_.tyDeclK_);
}

const Type* TypeDeclarationSymbol::specifiedType() const
{
    return P_CAST->ty_;
}

namespace psy {
namespace C {

std::string PSY_C_API to_string(const TypeDeclarationSymbol& tyDecl)
{
    switch (tyDecl.kind()) {
        case TypeDeclarationSymbolKind::Struct:
            return to_string(*tyDecl.asStruct());
        case TypeDeclarationSymbolKind::Union:
            return to_string(*tyDecl.asUnion());
        case TypeDeclarationSymbolKind::Enum:
            return to_string(*tyDecl.asEnum());
        case TypeDeclarationSymbolKind::Typedef:
            return to_string(*tyDecl.asTypedef());
    }
}

} // C
} // psy
