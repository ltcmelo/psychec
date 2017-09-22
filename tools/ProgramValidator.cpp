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

#include "ProgramValidator.h"
#include "AST.h"
#include "Control.h"
#include "Token.h"
#include "TranslationUnit.h"
#include <iostream>

using namespace psyche;
using namespace psyche;

ProgramValidator::ProgramValidator(TranslationUnit *unit, bool forbidTypeDecl)
    : ASTVisitor(unit)
    , forbidTypeDecl_(forbidTypeDecl)
{}

void ProgramValidator::validate(TranslationUnitAST *ast)
{
    if (!ast || !checksRegistered())
        return;

    for (DeclarationListAST *it = ast->declaration_list; it; it = it->next)
        accept(it->value);
}

bool ProgramValidator::checksRegistered() const
{
    return forbidTypeDecl_;
}

bool ProgramValidator::visit(FunctionDefinitionAST *ast)
{
    accept(ast->function_body);

    return false;
}

bool ProgramValidator::visit(CompoundStatementAST *ast)
{
    for (StatementListAST* it = ast->statement_list; it; it = it->next)
        accept(it->value);

    return false;
}

bool ProgramValidator::visit(SimpleDeclarationAST *ast)
{
    for (SpecifierListAST *it = ast->decl_specifier_list; it; it = it->next)
        accept(it->value);

    return false;
}

bool ProgramValidator::visit(ClassSpecifierAST *ast)
{
    if (forbidTypeDecl_) {
        translationUnit()->error(ast->firstToken(),
                                 "struct or union not allowed in this mode");
    }

    return false;
}

bool ProgramValidator::visit(SimpleSpecifierAST *ast)
{
    if (forbidTypeDecl_
            && tokenKind(ast->specifier_token) == T_TYPEDEF) {
        translationUnit()->error(ast->firstToken(), "typedef not allowed in this mode");
    }

    return false;
}
