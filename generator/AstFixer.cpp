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

#include "AstFixer.h"
#include "AST.h"
#include "Assert.h"
#include "CoreTypes.h"
#include "Debug.h"
#include "Literals.h"
#include "Scope.h"
#include "Symbols.h"
#include "SyntaxAmbiguity.h"
#include "TranslationUnit.h"

#define VISITOR_NAME "AstFixer"

using namespace CPlusPlus;
using namespace psyche;

AstFixer::AstFixer(TranslationUnit *unit)
    : ASTVisitor(unit)
{}

void AstFixer::Stats::reset()
{
    resolvedAsDecl_ = 0;
    resolvedAsExpr_ = 0;
    guessedAsCall_ = 0;
    guessedAsPtrDecl_ = 0;
}

void AstFixer::fix(TranslationUnitAST *ast)
{
    if (!ast)
        return;

    for (DeclarationListAST *it = ast->declaration_list; it; it = it->next)
        accept(it->value);
}

bool AstFixer::visit(CompoundStatementAST *ast)
{
    for (StatementListAST *it = ast->statement_list; it; it = it->next) {
        maybeFixAST(it->value);
        accept(it->value);
    }
    return false;
}

bool AstFixer::visit(IfStatementAST *ast)
{
    maybeFixAST(ast->statement);
    accept(ast->statement);
    maybeFixAST(ast->else_statement);
    accept(ast->else_statement);
    return false;
}

bool AstFixer::visit(ForStatementAST *ast)
{
    maybeFixAST(ast->statement);
    accept(ast->statement);
    return false;
}

bool AstFixer::visit(LabeledStatementAST *ast)
{
    maybeFixAST(ast->statement);
    accept(ast->statement);
    return false;
}

bool AstFixer::visit(WhileStatementAST *ast)
{
    maybeFixAST(ast->statement);
    accept(ast->statement);
    return false;
}

bool AstFixer::visit(SwitchStatementAST *ast)
{
    maybeFixAST(ast->statement);
    accept(ast->statement);
    return false;
}

bool AstFixer::visit(CaseStatementAST *ast)
{
    maybeFixAST(ast->statement);
    accept(ast->statement);
    return false;
}

bool AstFixer::visit(DoStatementAST *ast)
{
    maybeFixAST(ast->statement);
    accept(ast->statement);
    return false;
}

void AstFixer::maybeFixAST(StatementAST *&ast)
{
    if (!ast || !ast->asAmbiguousStatement())
        return;

    AmbiguityInfo::Resolution resolution = ast->asAmbiguousStatement()->info->resolution();
    auto line = ast->asAmbiguousStatement()->info->line();
    if (resolution == AmbiguityInfo::Resolution::DefinitelyExpression) {
        ast = ast->asAmbiguousStatement()->expressionStmt;
        printDebug("Ambiguity at %d can be disambiguated as expression\n", line);
        ++stats_.resolvedAsExpr_;
    } else if (resolution == AmbiguityInfo::Resolution::DefinitelyDeclaration) {
        ast = ast->asAmbiguousStatement()->declarationStmt;
        printDebug("Ambiguity at %d can be disambiguated as declaration\n", line);
        ++stats_.resolvedAsDecl_;
    } else {
        printDebug("Ambiguity at %d cannot be disambiguated, apply heuristics\n", line);
        AmbiguityInfo::Variety variety = ast->asAmbiguousStatement()->info->variety();
        if (variety == AmbiguityInfo::Variety::MulExpr_X_PointerDecl) {
            ast = ast->asAmbiguousStatement()->declarationStmt;
            ++stats_.guessedAsPtrDecl_;
        } else if (variety == AmbiguityInfo::Variety::OneArgCall_X_VarDecl) {
            ast = ast->asAmbiguousStatement()->expressionStmt;
            ++stats_.guessedAsCall_;
        }
    }
}

namespace psyche {

std::ostream& operator<<(std::ostream& os, const AstFixer::Stats& s)
{
    os << "  Total ambiguities  : " << s.resolvedAsDecl_
          + s.resolvedAsExpr_
          + s.guessedAsCall_
          + s.guessedAsPtrDecl_ << std::endl
       << "  Resolved as decl   : " << s.resolvedAsDecl_ << std::endl
       << "  Resolved as expr   : " << s.resolvedAsExpr_ << std::endl
       << "  Guessed as call    : " << s.guessedAsCall_ << std::endl
       << "  Guessed as ptr decl: " << s.guessedAsPtrDecl_;
    return os;
}

} // namespace psyche
