/******************************************************************************
 Copyright (c) 2016,17 Leandro T. C. Melo (ltcmelo@gmail.com)

 This library is free software; you can redistribute it and/or modify it under
 the terms of the GNU Lesser General Public License as published by the Free
 Software Foundation; either version 2.1 of the License, or (at your option)
 any later version.

 This library is distributed in the hope that it will be useful, but WITHOUT
 ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License
 for more details.

 You should have received a copy of the GNU Lesser General Public License along
 with this library; if not, write to the Free Software Foundation, Inc., 51
 Franklin Street, Fifth Floor, Boston, MA  02110-1301 USA
 *****************************************************************************/

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

Symbol *lookupSymbol(const Identifier* ident, const Scope* scope)
{
    if (!ident)
        return nullptr;

    Symbol *tySym = nullptr;
    while (scope) {
        if ((tySym = scope->find(ident)))
            break;
        scope = scope->enclosingScope();
    }

    return tySym;
}

Symbol *lookupSymbol(const Name* name, const Scope* scope)
{
    assert((name->isNameId() || name->isTaggedNameId()) && "expected trival or tagged name");

    return lookupSymbol(name->identifier(), scope);
}

Symbol *lookupTypeSymbol(const Name* name, const Scope *scope)
{
    assert((name->isNameId() || name->isTaggedNameId()) && "expected trival or tagged name");

    Symbol *tySym = lookupSymbol(name->identifier(), scope);
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

Symbol *lookupValueSymbol(const Name *name, const Scope *scope)
{
    Symbol *valSym = lookupSymbol(name, scope);
    if (valSym
            && (valSym->isDeclaration()
                    || valSym->isArgument())
            && valSym->storage() != Symbol::Typedef) {
        return valSym;
    }

    return nullptr;
}

} // namespace psyche
