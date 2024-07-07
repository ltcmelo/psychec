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

#include "TypeDeclarationSymbol_Typedef.h"
#include "TypeDeclarationSymbol__IMPL__.inc"

#include "syntax/Lexeme_Identifier.h"
#include "types/Type_Typedef.h"

#include <sstream>

using namespace psy;
using namespace C;

Typedef::Typedef(const SyntaxTree* tree,
                 const Symbol* containingSym,
                 const Scope* enclosingScope,
                 TypedefType* tydefTy)
    : TypeDeclarationSymbol(
          new TypeDeclarationSymbolImpl(tree,
                                        containingSym,
                                        enclosingScope,
                                        NameSpace::OrdinaryIdentifiers,
                                        tydefTy,
                                        TypeDeclarationSymbolKind::Typedef))
{
}

std::string Typedef::toDisplayString() const
{
    return P_CAST->ty_->asTypedefType()->typedefName()->valueText();
}

const Identifier* Typedef::identifier() const
{
    PSY_ASSERT(P_CAST->ty_->kind() == TypeKind::Typedef, return nullptr);
    return P_CAST->ty_->asTypedefType()->typedefName();
}

namespace psy {
namespace C {

std::string to_string(const Typedef& tydef)
{
    std::ostringstream oss;
    oss << "<Typedef | ";
    oss << "type:" << to_string(*tydef.specifiedType());
    oss << ">";
    return oss.str();
}

} // C
} // psy
