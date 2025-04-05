// Copyright (c) 2025 Leandro T. C. Melo <ltcmelo@gmail.com>
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

#include "TagDeclaration_StructOrUnion.h"

#include "symbols/Symbol_ALL.h"

#include <algorithm>
#include <iterator>

using namespace psy;
using namespace C;

StructOrUnionDeclarationSymbol::StructOrUnionDeclarationSymbol(
        SymbolKind symK,
        const Symbol* containingSym,
        const SyntaxTree* tree,
        const Scope* enclosingScope,
        TagType* tagTy)
    : TagDeclarationSymbol(
              symK,
              containingSym,
              tree,
              enclosingScope,
              tagTy)
{}

StructOrUnionDeclarationSymbol::Fields StructOrUnionDeclarationSymbol::fields() const
{
    Fields fldDecls;
    fldDecls.reserve(membDecls_.size());
    std::transform(membDecls_.begin(),
                   membDecls_.end(),
                   std::back_inserter(fldDecls),
                   [] (const MemberDeclarationSymbol* membDecl) {
                       PSY_ASSERT_2(
                           membDecl->kind() == SymbolKind::FieldDeclaration,
                           continue);
                       return membDecl->asFieldDeclaration();
                   });
    return fldDecls;
}

void StructOrUnionDeclarationSymbol::addField(const FieldDeclarationSymbol* fld)
{
    addMember(fld);
}
