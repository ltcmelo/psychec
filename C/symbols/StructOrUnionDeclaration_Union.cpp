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
#include "StructOrUnionDeclaration_Union.h"

#include "sema/Scope.h"
#include "symbols/Symbol_ALL.h"
#include "syntax/Lexeme_Identifier.h"
#include "types/Type_Tag.h"

#include <iostream>
#include <sstream>

using namespace psy;
using namespace C;

UnionDeclarationSymbol::UnionDeclarationSymbol(
        const Symbol* containingSym,
        const SyntaxTree* tree,
        const Scope* enclosingScope,
        TagType* tagTy)
    : StructOrUnionDeclarationSymbol(
          SymbolKind::UnionDeclaration,
          containingSym,
          tree,
          enclosingScope,
          tagTy)
{
}

namespace psy {
namespace C {

std::ostream& operator<<(std::ostream& os, const UnionDeclarationSymbol* uniom)
{
    if (!uniom)
        return os << "<Union is null>";
    os << "<Union |";
    os << " type:" << uniom->introducedNewType();
    os << " scope:" << uniom->enclosingScope()->kind();
    for (const auto& fld : uniom->fields())
        os << " field:" << fld;
    os << ">";
    return os;
}

} // C
} // psy
