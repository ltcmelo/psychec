// Copyright (c) 2016 Leandro T. C. Melo <ltcmelo@gmail.com>
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.

#ifndef PSYCHE_ASTNORMALIZER_H__
#define PSYCHE_ASTNORMALIZER_H__

#include "FrontendConfig.h"

#include "AST.h"
#include "ASTVisitor.h"
#include <iostream>

namespace psyche {

class CFE_API ASTNormalizer final : public psyche::ASTVisitor
{
public:
    ASTNormalizer(psyche::TranslationUnit *unit, bool employHeuristic);

    bool normalize(psyche::TranslationUnitAST* ast);

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
    bool visit(psyche::CompoundStatementAST* ast);
    bool visit(psyche::IfStatementAST* ast);
    bool visit(psyche::ForStatementAST* ast);
    bool visit(psyche::LabeledStatementAST* ast);
    bool visit(psyche::WhileStatementAST* ast);
    bool visit(psyche::SwitchStatementAST* ast);
    bool visit(psyche::CaseStatementAST* ast);
    bool visit(psyche::DoStatementAST* ast);

    void maybeFixAST(psyche::StatementAST* &ast);

    bool employHeuristic_; // On ambiguities not resolved by further syntax.
    Stats stats_;
};

std::ostream& operator<<(std::ostream& os, const ASTNormalizer::Stats& s);

} // namespace psyche

#endif
