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

#include "SymbolPP.h"
#include "Symbols.h"
#include "Assert.h"
#include "Names.h"
#include "Literals.h"

using namespace CPlusPlus;
using namespace psyche;

std::string SymbolPP::print(const Symbol *symbol)
{
    if (!symbol)
        return "<unnamed>";

    name_.clear();
    accept(const_cast<Symbol*>(symbol));
    return name_.str();
}

bool SymbolPP::visit(Declaration *symbol)
{
    appendName(symbol->name());
    return false;
}

bool SymbolPP::visit(FakeDeclaration *symbol)
{
    appendName(symbol->name());
    return false;
}

bool SymbolPP::visit(Argument *symbol)
{
    appendName(symbol->name());
    return false;
}

bool SymbolPP::visit(Function *symbol)
{
    appendName(symbol->name());
    return false;
}

bool SymbolPP::visit(Class *symbol)
{
    appendName(symbol->name());
    return false;
}

bool SymbolPP::visit(FakeClass *symbol)
{
    appendName(symbol->name());
    return false;
}

bool SymbolPP::visit(Block *symbol)
{
    name_ << "<block>";
    return false;
}

void SymbolPP::appendName(const Name *name)
{
    PSYCHE_ASSERT(name && name->asNameId(), return, "expected trivial name");
    if (name_.tellp())
        name_ << '.';
    name_ << std::string(name->asNameId()->identifier()->begin(),
                         name->asNameId()->identifier()->end());
}
