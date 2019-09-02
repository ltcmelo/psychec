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

#ifndef PSYCHE_EXPRESSION_TYPE_EVALUATOR_H__
#define PSYCHE_EXPRESSION_TYPE_EVALUATOR_H__

#include "FrontendConfig.h"

#include "ASTVisitor.h"
#include "FullySpecifiedType.h"
#include <cstddef>
#include <vector>

namespace psyche {

class CFE_API ExpressionTypeEvaluator final : ASTVisitor
{
public:
    ExpressionTypeEvaluator(TranslationUnit* unit);

    FullySpecifiedType evaluate(ExpressionAST* ast, Scope* scope);

    FullySpecifiedType commonRealType(const FullySpecifiedType&,
                                      const FullySpecifiedType&) const;

    FullySpecifiedType promotedType(const FullySpecifiedType&) const;

private:
    void process(const Identifier* id);

    // Expressions
    bool visit(ArrayAccessAST* ast) override;
    bool visit(BinaryExpressionAST* ast) override;
    bool visit(CallAST *ast) override;
    bool visit(CastExpressionAST *ast) override;
    bool visit(ConditionalExpressionAST *ast) override;
    bool visit(IdExpressionAST* ast) override;
    bool visit(MemberAccessAST* ast) override;
    bool visit(NumericLiteralAST* ast) override;
    bool visit(BoolLiteralAST* ast) override;
    bool visit(StringLiteralAST* ast) override;
    bool visit(UnaryExpressionAST* ast) override;
    bool visit(SizeofExpressionAST* ast) override;
    bool visit(PointerLiteralAST* ast) override;
    bool visit(TypeIdAST* ast) override;

    // Specifiers
    bool visit(SimpleSpecifierAST* ast) override;
    bool visit(NamedTypeSpecifierAST* ast) override;
    bool visit(QuantifiedTypeSpecifierAST* ast) override;
    bool visit(ElaboratedTypeSpecifierAST* ast) override;

    // Declarators
    bool visit(DeclaratorAST* ast) override;
    bool visit(PointerAST* ast) override;

    FullySpecifiedType type_;
    Scope* scope_;
    std::size_t _memberAccess;
};

}

#endif
