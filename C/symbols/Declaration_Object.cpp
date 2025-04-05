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
#include "Declaration_Object.h"

#include "symbols/Symbol_ALL.h"
#include "types/Type_ALL.h"

#include <sstream>

using namespace psy;
using namespace C;

struct ObjectDeclarationSymbol::ObjectDeclarationImpl : DeclarationImpl
{
    ObjectDeclarationImpl(SymbolKind symK,
                          const Symbol* containingSym,
                          const SyntaxTree* tree,
                          const Scope* enclosingScope)
        : DeclarationImpl(symK,
                          containingSym,
                          tree,
                          enclosingScope,
                          NameSpace::OrdinaryIdentifiers)
        , name_(nullptr)
        , ty_(nullptr)
    {}

    const Identifier* name_;
    const Type* ty_;
};

ObjectDeclarationSymbol::ObjectDeclarationSymbol(SymbolKind symK,
                                                 const Symbol* containingSym,
                                                 const SyntaxTree* tree,
                                                 const Scope* enclosingScope)
    : DeclarationSymbol(
          new ObjectDeclarationImpl(symK,
                                    containingSym,
                                    tree,
                                    enclosingScope))
{}

ObjectDeclarationSymbol::~ObjectDeclarationSymbol()
{}

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

std::ostream& operator<<(std::ostream& os, const ObjectDeclarationSymbol* objDecl)
{
    if (!objDecl)
        return os << "<ObjectDeclaration is null>";
    switch (objDecl->kind()) {
        case SymbolKind::ParameterDeclaration:
            return os << objDecl->asParameterDeclaration();
        case SymbolKind::VariableDeclaration:
            return os << objDecl->asVariableDeclaration();
        default:
            PSY_ASSERT_1(false);
            break;
    }
    return os << "<invalid ObjectDeclaration>";
}

} // C
} // psy
