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

#include "RangeAnalysis.h"
#include "AST.h"
#include "Assert.h"
#include "Literals.h"
#include "Lookup.h"
#include "Scope.h"
#include "Symbols.h"
#include "TranslationUnit.h"
#include <cstdlib>
#include <memory>

using namespace psyche;
using namespace psyche;

RangeAnalysis::RangeAnalysis(TranslationUnit *unit)
    : ASTVisitor(unit)
{}

void RangeAnalysis::run(TranslationUnitAST *ast, Namespace *global)
{
    switchScope(global);
    for (DeclarationListAST *it = ast->declaration_list; it; it = it->next)
        accept(it->value);
    dumpRanges();
}

void RangeAnalysis::dumpRanges() const
{
    // TEMP...
    for (auto p : rangeMap_) {
        if (p.first->name()
                && p.first->name()->asNameId()) {
            Range range = p.second;
            std::cout << p.first->name()->asNameId()->identifier()->chars()
                      << ": " << range << std::endl;
        }
    }
}

const Scope *RangeAnalysis::switchScope(const Scope *scope)
{
    PSYCHE_ASSERT(scope, return nullptr, "scope must be valid");
    std::swap(scope_, scope);
    return scope;
}

const Symbol *RangeAnalysis::switchSymbol(const Symbol *symbol)
{
    PSYCHE_ASSERT(symbol, return nullptr, "symbol must be valid");
    std::swap(symbol_, symbol);
    return symbol;
}

void RangeAnalysis::resolve(const Name *name)
{
    // We expect to see only simple names. Also, since range analysis assumes
    // well-formed code, symbol lookup must succeed.
    PSYCHE_ASSERT(name && name->asNameId(), return, "expected simple name");
    symbol_ = lookupValueSymbol(name, scope_);
    PSYCHE_ASSERT(symbol_, return, "expected successful lookup");
}

bool RangeAnalysis::visit(NumericLiteralAST *ast)
{
    const NumericLiteral *numLit = numericLiteral(ast->literal_token);
    PSYCHE_ASSERT(numLit, return false, "numeric literal must exist");

    // We're interested only on natural numbers. In the case this literal
    // is a floating-point, our interpretation is to truncate it.
    auto value = std::atoi(numLit->chars());
    Range range(std::make_unique<IntegerValue>(value),
                std::make_unique<IntegerValue>(value));
    stack_.push(std::move(range));

    return false;
}

bool RangeAnalysis::visit(IdExpressionAST *ast)
{
    resolve(ast->name->name);

    // When the symbol is a scope, we only need to enter it.
    if (symbol_->type()->asClassType()) {
        switchScope(symbol_->type()->asClassType());
        return false;
    }

    // We must push the range for this symbol onto the stack. If there's already
    // a range associated to it, we use it. Otherwise a symbolic range is created.
    const auto it = rangeMap_.find(symbol_);
    if (it != rangeMap_.end()) {
        stack_.push(it->second);
    } else {
        Range range(std::make_unique<SymbolValue>(symbol_),
                    std::make_unique<SymbolValue>(symbol_));
        stack_.push(std::move(range));
    }

    return false;
}

bool RangeAnalysis::visit(BinaryExpressionAST *ast)
{
    accept(ast->left_expression);

    unsigned op = tokenKind(ast->binary_op_token);

    // In the case of an assignment, we drop the left-hand-side's range, since
    // it will be overwritten by the one from the right-hand-side.
    if (op == T_EQUAL) {
        stack_.pop();
        const Symbol* lhsSymbol = symbol_;
        accept(ast->right_expression);
        rangeMap_.insertOrAssign(lhsSymbol, std::move(stack_.top()));
        stack_.pop();
        return false;
    }

    // When two expressions are added, we must correspondigly add their ranges
    // and push the result onto the stack.
    if (op == T_PLUS) {
        Range lhsRange = std::move(stack_.top());
        stack_.pop();
        accept(ast->right_expression);
        Range rhsRange = std::move(stack_.top());
        stack_.pop();
        auto lower = std::make_unique<CompositeValue>(std::move(lhsRange.lower_),
                                                      std::move(rhsRange.lower_),
                                                      CompositeValue::Addition);
        auto upper = std::make_unique<CompositeValue>(std::move(lhsRange.upper_),
                                                      std::move(rhsRange.upper_),
                                                      CompositeValue::Addition);
        stack_.emplace(std::move(lower), std::move(upper));
        return false;
    }

    return false;
}

bool RangeAnalysis::visit(MemberAccessAST *ast)
{
    const Scope* prevScope = scope_;
    accept(ast->base_expression);
    resolve(ast->member_name->name);
    switchScope(prevScope);
    return false;
}

bool RangeAnalysis::visit(DeclarationStatementAST *ast)
{
    return false;
}

bool RangeAnalysis::visit(ExpressionStatementAST *ast)
{
    accept(ast->expression);
    revisionMap_.insert(std::make_pair(ast, rangeMap_.revision()));
    return false;
}

bool RangeAnalysis::visit(IfStatementAST *ast)
{
    const auto revision = rangeMap_.revision();
    accept(ast->statement);
    rangeMap_.applyRevision(revision);
    accept(ast->else_statement);
    rangeMap_.applyRevision(revision);
    // TODO: Unite commands from both branches (retrieve them from revisions).
    return false;
}

bool RangeAnalysis::visit(WhileStatementAST *ast)
{
    return false;
}
