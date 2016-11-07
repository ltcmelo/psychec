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

#include "Utils.h"
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

namespace {

struct FindAmbiguousNode : public ASTVisitor
{
    FindAmbiguousNode(CPlusPlus::TranslationUnit *unit)
        : ASTVisitor(unit)
        , hasAmbiguousNode_(false)
    {}

    bool operator()(const CPlusPlus::TranslationUnitAST *ast)
    {
        if (ast) {
            for (DeclarationListAST *it = ast->declaration_list; it; it = it->next)
                accept(it->value);
        }
        return hasAmbiguousNode_;
    }

    bool visit(AmbiguousStatementAST *) override
    {
        hasAmbiguousNode_ = true;
        return false;
    }

    bool hasAmbiguousNode_;
};

} // namespace anonymous

namespace psyche {

ExpressionAST* stripParen(ExpressionAST *ast)
{
    ExpressionAST* striped = ast;
    while (striped->asNestedExpression())
        striped = ast->asNestedExpression()->expression;
    return striped;
}

bool isProgramAmbiguous(TranslationUnit *program, const TranslationUnitAST *ast)
{
    return FindAmbiguousNode(program)(ast);
}

} // namespace psyche
