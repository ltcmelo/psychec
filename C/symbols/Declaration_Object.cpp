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
#include "Declaration_Object.h"

#include "symbols/Symbol_ALL.h"

#include <sstream>

using namespace psy;
using namespace C;

struct ObjectDeclaration::ObjectDeclarationImpl : SymbolImpl
{
    ObjectDeclarationImpl(const SyntaxTree* tree,
                                const Symbol* containingSym,
                                const Scope* enclosingScope,
                                ObjectDeclarationKind objDeclK)
        : SymbolImpl(tree,
                     containingSym,
                     enclosingScope,
                     NameSpace::OrdinaryIdentifiers,
                     SymbolKind::Declaration)
        , name_(nullptr)
        , ty_(nullptr)
    {
        BF_.objDeclK_ = static_cast<std::uint32_t>(objDeclK);
    }

    const Identifier* name_;
    const Type* ty_;
};

ObjectDeclaration::ObjectDeclaration(const SyntaxTree* tree,
                                                 const Symbol* containingSym,
                                                 const Scope* enclosingScope,
                                                 ObjectDeclarationKind objDeclK)
    : Declaration(
          new ObjectDeclarationImpl(tree,
                                          containingSym,
                                          enclosingScope,
                                          objDeclK),
          DeclarationKind::Object)
{}

ObjectDeclaration::~ObjectDeclaration()
{}

ObjectDeclarationKind ObjectDeclaration::kind() const
{
    return ObjectDeclarationKind(P_CAST->BF_.objDeclK_);
}

const Type* ObjectDeclaration::type() const
{
    return P_CAST->ty_;
}

const Type* ObjectDeclaration::retypeableType() const
{
    return type();
}

void ObjectDeclaration::setType(const Type* ty)
{
    P_CAST->ty_ = ty;
}

const Identifier* ObjectDeclaration::name() const
{
    return P_CAST->name_;
}

void ObjectDeclaration::setName(const Identifier* name)
{
    P_CAST->name_ = name;
}

namespace psy {
namespace C {

std::string to_string(const ObjectDeclaration& sym)
{
    switch (sym.kind()) {
        case ObjectDeclarationKind::Enumerator:
            return to_string(*sym.asEnumerator());
        case ObjectDeclarationKind::Field:
            return to_string(*sym.asField());
        case ObjectDeclarationKind::Parameter:
            return to_string(*sym.asParameter());
        case ObjectDeclarationKind::Variable:
            return to_string(*sym.asVariable());
    }
}

} // C
} // psy
