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
#include "Declaration_Function.h"

#include "symbols/Symbol_ALL.h"
#include "syntax/Lexeme_Identifier.h"
#include "types/Type_ALL.h"

#include <sstream>

using namespace psy;
using namespace C;

struct FunctionDeclarationSymbol::FunctionImpl : DeclarationImpl
{
    FunctionImpl(const Symbol* containingSym,
                 const SyntaxTree* tree,
                 const Scope* enclosingScope)
        : DeclarationImpl(SymbolKind::FunctionDeclaration,
                          containingSym,
                          tree,
                          enclosingScope,
                          NameSpace::OrdinaryIdentifiers)
        , name_(nullptr)
        , ty_(nullptr)
    {
        F_.isDef_ = 0;
    }

    const Identifier* name_;
    const Type* ty_;
};

FunctionDeclarationSymbol::FunctionDeclarationSymbol(
        const Symbol* containingSym,
        const SyntaxTree* tree,
        const Scope* enclosingScope)
    : DeclarationSymbol(
          new FunctionImpl(containingSym,
                           tree,
                           enclosingScope))
{}

const Identifier* FunctionDeclarationSymbol::name() const
{
    return P_CAST->name_;
}

void FunctionDeclarationSymbol::setName(const Identifier* name)
{
    P_CAST->name_ = name;
}

const Type* FunctionDeclarationSymbol::type() const
{
    return P_CAST->ty_;
}

Type* FunctionDeclarationSymbol::type()
{
    return const_cast<Type*>(P_CAST->ty_);
}

void FunctionDeclarationSymbol::setType(const Type* ty)
{
    P_CAST->ty_ = ty;
}

bool FunctionDeclarationSymbol::isDefinition() const
{
    return P_CAST->F_.isDef_;
}

void FunctionDeclarationSymbol::setIsDefinition(bool isDef)
{
    P_CAST->F_.isDef_ = static_cast<std::uint32_t>(isDef);
}

namespace psy {
namespace C {

std::ostream& operator<<(std::ostream& os, const FunctionDeclarationSymbol* func)
{
    if (!func)
        return os << "<Function is null>";
    os << "<Function |";
    os << " name:" << func->name()->valueText();
    os << " type:" << func->type();
    os << ">";
    return os;
}

} // C
} // psy
