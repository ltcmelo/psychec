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

#include "TypeDeclaration_Typedef.h"
#include "TypeDeclaration__IMPL__.inc"

#include "syntax/Lexeme_Identifier.h"
#include "types/Type_TypedefName.h"

#include "../common/infra/Assertions.h"

#include <sstream>

using namespace psy;
using namespace C;

TypedefDeclarationSymbol::TypedefDeclarationSymbol(const Symbol* containingSym,
                 const SyntaxTree* tree,
                 const Scope* enclosingScope,
                 TypedefNameType* tydefNameTy)
    : TypeDeclarationSymbol(
          new TypeDeclarationImpl(SymbolKind::TypedefDeclaration,
                                  containingSym,
                                  tree,
                                  enclosingScope,
                                  NameSpace::OrdinaryIdentifiers,
                                  tydefNameTy))
    , synonymizedTy_(nullptr)
{
    tydefNameTy->setDeclaration(this);
}

const Identifier* TypedefDeclarationSymbol::denotingIdentifier() const
{
    PSY_ASSERT_2(introducedType()->kind() == TypeKind::TypedefName, return nullptr);
    return P_CAST->ty_->asTypedefNameType()->typedefName();
}

const TypedefNameType* TypedefDeclarationSymbol::introducedSynonymType() const
{
    PSY_ASSERT_2(introducedType()->kind() == TypeKind::TypedefName, return nullptr);
    return P_CAST->ty_->asTypedefNameType();
}

const Type* TypedefDeclarationSymbol::synonymizedType() const
{
    return synonymizedTy_;
}

void TypedefDeclarationSymbol::setSynonymizedType(const Type* ty)
{
    synonymizedTy_ = ty;
}

namespace psy {
namespace C {

std::ostream& operator<<(std::ostream& os, const TypedefDeclarationSymbol* tydef)
{
    if (!tydef)
        return os << "<Typedef is null>";
    os << "<Typedef |";
    os << " defined-type:" << tydef->introducedSynonymType();
    os << " synonymized-type:" << tydef->synonymizedType();
    os << ">";
    return os;
}

} // C
} // psy
