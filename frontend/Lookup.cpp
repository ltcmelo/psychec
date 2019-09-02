// Copyright (c) 2016 Leandro T. C. Melo <ltcmelo@gmail.com>
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

#include "AST.h"
#include "ASTVisitor.h"
#include "CoreTypes.h"
#include "Literals.h"
#include "Name.h"
#include "Symbol.h"
#include "Symbols.h"
#include "TypeVisitor.h"
#include <cassert>
#include <iostream>

using namespace psyche;

namespace psyche {

Symbol* lookupSymbol(const Identifier* ident, const Scope* scope)
{
    if (!ident)
        return nullptr;

    Symbol* sym = nullptr;
    while (scope) {
        if ((sym = scope->find(ident)))
            return sym;
        scope = scope->enclosingScope();
    }
    return sym;
}

Symbol* lookupSymbol(const Name* name, const Scope* scope)
{
    return name->identifier() ? lookupSymbol(name->identifier(), scope) : nullptr;
}

Symbol* lookupTypeSymbol(const Name* name, const Scope *scope)
{
    assert((name->isNameId() || name->isTaggedNameId()) && "expected trival or tagged name");

    Symbol* tySym = lookupSymbol(name->identifier(), scope);
    if (!tySym)
        return nullptr;

    if (name->asNameId())
        return tySym->isTypedef() ? tySym : nullptr;

    auto tag = name->asTaggedNameId()->tag();
    switch (tag) {
    case TaggedNameId::Struct:
    case TaggedNameId::Union:
        // TODO: Add tag to forward-decl and unify all "tag/key" enums.
        return (tySym->isForwardClassDeclaration()
                    || (tySym->isClass()
                            && tySym->asClass()->classKey() == (int)tag)) ? tySym : nullptr;

    case TaggedNameId::Enum:
        return tySym->isEnum() ? tySym : nullptr;

    default:
        assert(false && "unexpected name");
        return nullptr;
    }
}

Symbol* lookupValueSymbol(const Name* name, const Scope *scope)
{
    Symbol* valSym = lookupSymbol(name, scope);
    if (valSym
            && (valSym->isDeclaration()
                    || valSym->isArgument())
            && valSym->storage() != Symbol::Typedef) {
        return valSym;
    }

    return nullptr;
}

} // namespace psyche
