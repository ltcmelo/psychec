/******************************************************************************
 Copyright (c) 2017 Leandro T. C. Melo (ltcmelo@gmail.com)

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

#include "StdLibInterceptor.h"

#include "AST.h"
#include "Literals.h"
#include "Names.h"
#include "Symbols.h"
#include <iostream>
#include <cstring>

using namespace psyche;

bool StdLibInterceptor::intercept(SimpleDeclarationAST* ast)
{
    for (const List<Symbol*> *symIt = ast->symbols; symIt; symIt = symIt->next) {
        Symbol *decl = symIt->value;
        if (decl && isStdName(decl->name()))
            return true;
    }

    return false;
}

bool StdLibInterceptor::intercept(FunctionDefinitionAST* ast)
{
    return ast->symbol && isStdName(ast->symbol->name());
}

bool StdLibInterceptor::isStdName(const Name* name) const
{
    // TODO: In theory, functions starting with __ or with _Capital could also be ignored.

    if (!name || !name->asNameId())
        return false;

    return index_.recognizes(name->asNameId()->identifier()->chars());
}
