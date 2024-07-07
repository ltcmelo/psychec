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

#include "Symbol__IMPL__.inc"
#include "DeclarationSymbol_Object.h"

#include "symbols/Symbol_ALL.h"

#include <sstream>

using namespace psy;
using namespace C;

struct ObjectDeclarationSymbol::ObjectDeclarationSymbolImpl : SymbolImpl
{
    ObjectDeclarationSymbolImpl(const SyntaxTree* tree,
                                const Symbol* containingSym,
                                const Scope* enclosingScope,
                                ObjectDeclarationSymbolKind objDeclSymK)
        : SymbolImpl(tree,
                     containingSym,
                     enclosingScope,
                     NameSpace::OrdinaryIdentifiers,
                     SymbolKind::Declaration)
        , name_(nullptr)
    {
        BF_.objDeclSymK_ = static_cast<std::uint32_t>(objDeclSymK);
    }

    const Identifier* name_;
    const Type* ty_;
};

ObjectDeclarationSymbol::ObjectDeclarationSymbol(const SyntaxTree* tree,
                                                 const Symbol* containingSym,
                                                 const Scope* enclosingScope,
                                                 ObjectDeclarationSymbolKind objDeclSymK)
    : DeclarationSymbol(
          new ObjectDeclarationSymbolImpl(tree,
                                          containingSym,
                                          enclosingScope,
                                          objDeclSymK),
          DeclarationSymbolKind::Object)
{}

ObjectDeclarationSymbol::~ObjectDeclarationSymbol()
{}

ObjectDeclarationSymbolKind ObjectDeclarationSymbol::kind() const
{
    return ObjectDeclarationSymbolKind(P_CAST->BF_.objDeclSymK_);
}

const Type* ObjectDeclarationSymbol::type() const
{
    return P_CAST->ty_;
}

void ObjectDeclarationSymbol::setType(const Type* ty)
{
    P_CAST->ty_ = ty;
}

const Identifier* ObjectDeclarationSymbol::name() const
{
    return P_CAST->name_;
}

void ObjectDeclarationSymbol::setName(const Identifier* name)
{
    P_CAST->name_ = name;
}

namespace psy {
namespace C {

std::string to_string(const ObjectDeclarationSymbol& sym)
{
    switch (sym.kind()) {
        case ObjectDeclarationSymbolKind::Enumerator:
            return to_string(*sym.asEnumerator());
        case ObjectDeclarationSymbolKind::Field:
            return to_string(*sym.asField());
        case ObjectDeclarationSymbolKind::Parameter:
            return to_string(*sym.asParameter());
        case ObjectDeclarationSymbolKind::Variable:
            return to_string(*sym.asVariable());
    }
}

} // C
} // psy
