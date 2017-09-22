/******************************************************************************
 Copyright (c) 2016-17 Leandro T. C. Melo (ltcmelo@gmail.com)

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

#include "BasicStubber.h"
#include "AST.h"
#include "Assert.h"
#include "CoreTypes.h"
#include "CSyntax.h"
#include "FunctionWriter.h"
#include "Literals.h"
#include "TranslationUnit.h"
#include <iostream>

using namespace psyche;
using namespace psyche;

BasicStubber::BasicStubber(TranslationUnit* unit)
    : ASTVisitor(unit)
{}

void BasicStubber::apply(TranslationUnitAST* ast)
{
    if (!ast)
        return;

    for (DeclarationListAST *it = ast->declaration_list; it; it = it->next)
        accept(it->value);
}

bool BasicStubber::visit(SimpleDeclarationAST *ast)
{
    if (!ast->declarator_list)
        return false;

    for (const List<Symbol*> *symIt = ast->symbols; symIt; symIt = symIt->next) {
        Symbol *decl = symIt->value;
        if (decl->asDeclaration()
                && decl->asDeclaration()->type()
                && decl->asDeclaration()->type()->asFunctionType()) {
            create(decl->asDeclaration()->type()->asFunctionType());
        }
    }
    return false;
}

void BasicStubber::create(Function* func)
{
    PSYCHE_ASSERT(func->name() && func->name()->isNameId(), return, "expected simple name");
    std::string funcIdent(func->name()->asNameId()->identifier()->chars(),
                          func->name()->asNameId()->identifier()->size());

    static int cnt = 0;
    FunctionWriter writer;
    std::string text = writer.writeFunction(func, funcIdent);

    text.append(" {\n");
    if (func->returnType()->isVoidType()) {
        text.append("}\n\n");
        std::cout << text << std::endl;
        return;
    }

    TypeSpeller<CSyntax> speller;
    bool isPtr = func->returnType()->isPointerType();
    if (isPtr) {
        text.append("    static ");
        text.append(speller.spell(func->returnType()->asPointerType()->elementType(), nullptr));
    } else {
        text.append("    " + speller.spell(func->returnType(), nullptr));
    }
    const std::string& var = "fake" + std::to_string(cnt);
    text.append(" " + var + ";\n    return ");
    if (isPtr)
        text.append("&" + var);
    else
        text.append(var);
    text.append(";\n}\n\n");

    // TODO: Write to given stream.
}
