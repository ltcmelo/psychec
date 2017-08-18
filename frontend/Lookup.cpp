/******************************************************************************
 * Copyright (c) 2016 Leandro T. C. Melo (ltcmelo@gmail.com)
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301
 * USA
 *****************************************************************************/

#include "AST.h"
#include "ASTVisitor.h"
#include "Assert.h"
#include "CoreTypes.h"
#include "Literals.h"
#include "Name.h"
#include "Symbol.h"
#include "Symbols.h"
#include "TypeVisitor.h"
#include <iostream>

using namespace CPlusPlus;

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
    return lookupSymbol(name->asNameId(), scope);
}

Symbol *lookupTypeSymbol(const Name* name, const Scope *scope)
{
    Symbol *tySym = lookupSymbol(name, scope);
    if (tySym && (tySym->isClass()
                  || tySym->isForwardClassDeclaration()
                  || tySym->isEnum()
                  || tySym->isTypedef())) {
        return tySym;
    }

    return nullptr;
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
