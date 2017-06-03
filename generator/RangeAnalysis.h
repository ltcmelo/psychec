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

#ifndef PSYCHE_RANGEANALYSIS_H__
#define PSYCHE_RANGEANALYSIS_H__

#include "Config.h"
#include "ASTVisitor.h"
#include "Range.h"
#include "Symbol.h"
#include "VersionedMap.h"
#include <iostream>
#include <stack>
#include <unordered_map>

namespace psyche {

class PSYCHEC_API RangeAnalysis final : public CPlusPlus::ASTVisitor
{
public:
    RangeAnalysis(CPlusPlus::TranslationUnit *unit);

    void run(CPlusPlus::TranslationUnitAST *ast, CPlusPlus::Namespace *global);

private:
    /*!
     * \brief switchScope
     * \param scope
     * \return
     *
     * Convenience to switch current scope.
     */
    const CPlusPlus::Scope *switchScope(const CPlusPlus::Scope *scope);

    /*!
     * \brief switchSymbol
     * \param symbol
     * \return
     */
    const CPlusPlus::Symbol* switchSymbol(const CPlusPlus::Symbol* symbol);

    /*!
     * \brief resolve
     * \param name
     *
     * Resolve a name to a symbol.
     */
    void resolve(const CPlusPlus::Name* name);

    // Expressions
    bool visit(CPlusPlus::BinaryExpressionAST* ast) override;
    bool visit(CPlusPlus::IdExpressionAST* ast) override;
    bool visit(CPlusPlus::MemberAccessAST* ast) override;
    bool visit(CPlusPlus::NumericLiteralAST* ast) override;

    // Statements
    bool visit(CPlusPlus::DeclarationStatementAST *ast) override;
    bool visit(CPlusPlus::ExpressionStatementAST *ast) override;
    bool visit(CPlusPlus::IfStatementAST *ast) override;
    bool visit(CPlusPlus::WhileStatementAST *ast) override;

    void dumpRanges() const;

    //! Scope we're in.
    const CPlusPlus::Scope *scope_;

    //! Symbol just looked-up.
    const CPlusPlus::Symbol* symbol_;

    //! Range stack.
    std::stack<Range> stack_;

    std::unordered_map<const CPlusPlus::StatementAST*, int32_t> revisionMap_;
    VersionedMap<const CPlusPlus::Symbol*, Range> rangeMap_;
};

} // namespace psyche

#endif
