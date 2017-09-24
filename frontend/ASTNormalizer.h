/******************************************************************************
 Copyright (c) 2016 Leandro T. C. Melo (ltcmelo@gmail.com)

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

#ifndef PSYCHE_ASTNORMALIZER_H__
#define PSYCHE_ASTNORMALIZER_H__

#include "FrontendConfig.h"
#include "ASTVisitor.h"
#include <iostream>

namespace psyche {

class CFE_API ASTNormalizer final : public psyche::ASTVisitor
{
public:
    ASTNormalizer(psyche::TranslationUnit *unit, bool employHeuristic);

    bool normalize(psyche::TranslationUnitAST *ast);

    struct Stats
    {
        unsigned int resolvedAsDecl_ { 0 };
        unsigned int resolvedAsExpr_ { 0 };
        unsigned int guessedAsCall_ { 0 };
        unsigned int guessedAsPtrDecl_ { 0 };

        void reset();
    };

    Stats stats() const { return stats_; }

private:
    // Since we wanna replace one AST node for another, we need hands on the
    // parent of the node in question. Using one of the existing (pre)visit
    // methods only wouldn't work because all we get is a copy of the poitner.
    // Our approach is to overide the visit of all nodes which can hold an
    // ambiguous statement within it.
    bool visit(psyche::CompoundStatementAST *ast);
    bool visit(psyche::IfStatementAST *ast);
    bool visit(psyche::ForStatementAST *ast);
    bool visit(psyche::LabeledStatementAST *ast);
    bool visit(psyche::WhileStatementAST *ast);
    bool visit(psyche::SwitchStatementAST *ast);
    bool visit(psyche::CaseStatementAST *ast);
    bool visit(psyche::DoStatementAST *ast);

    void maybeFixAST(psyche::StatementAST *&ast);

    bool employHeuristic_; // On ambiguities not resolved by further syntax.
    Stats stats_;
};

std::ostream& operator<<(std::ostream& os, const ASTNormalizer::Stats& s);

} // namespace psyche

#endif
