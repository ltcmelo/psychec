// Copyright (c) 2016-20 Leandro T. C. Melo <ltcmelo@gmail.com>
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

#ifndef PSYCHE_IDENTITY_MATCHER_H__
#define PSYCHE_IDENTITY_MATCHER_H__

#include "FrontendConfig.h"
#include "AST.h"
#include "ASTMatcher.h"
#include "Name.h"
#include "FrontendFwds.h"
#include "Literals.h"
#include "TranslationUnit.h"
#include <cstring>

namespace psyche {

class CFE_API ASTIdentityMatcher final : public psyche::ASTMatcher
{
public:
    ASTIdentityMatcher(TranslationUnit* unit)
        : unit_(unit)
    {}

    TranslationUnit* unit_;

    // Expressions
    bool match(ArrayAccessAST* node, ArrayAccessAST* pattern) override;
    bool match(ArrayInitializerAST* node, ArrayInitializerAST* pattern) override;
    bool match(BinaryExpressionAST* node, BinaryExpressionAST* pattern) override;
    bool match(BoolLiteralAST* node, BoolLiteralAST* pattern) override;
    bool match(BracedInitializerAST* node, BracedInitializerAST* pattern) override;
    bool match(BracketDesignatorAST* node, BracketDesignatorAST* pattern) override;
    bool match(DotDesignatorAST* node, DotDesignatorAST* pattern) override;
    bool match(CallAST* node, CallAST* pattern) override;
    bool match(CastExpressionAST* node, CastExpressionAST* pattern) override;
    bool match(ConditionalExpressionAST* node, ConditionalExpressionAST* pattern) override;
    bool match(IdExpressionAST* node, IdExpressionAST* pattern) override;
    bool match(MemberAccessAST* node, MemberAccessAST* pattern) override;
    bool match(NestedExpressionAST* node, NestedExpressionAST* pattern) override;
    bool match(NumericLiteralAST* node, NumericLiteralAST* pattern) override;
    bool match(StringLiteralAST* node, StringLiteralAST* pattern) override;
    bool match(PointerLiteralAST* node, PointerLiteralAST* pattern) override;
    bool match(PostIncrDecrAST* node, PostIncrDecrAST* pattern) override;
    bool match(SizeofExpressionAST* node, SizeofExpressionAST* pattern) override;
    bool match(TypeIdAST* node, TypeIdAST* pattern) override;
    bool match(UnaryExpressionAST* node, UnaryExpressionAST* pattern) override;

    // Specifiers
    bool match(ClassSpecifierAST* node, ClassSpecifierAST* pattern) override;
    bool match(ElaboratedTypeSpecifierAST* node, ElaboratedTypeSpecifierAST* pattern) override;
    bool match(EnumSpecifierAST* node, EnumSpecifierAST* pattern) override;
    bool match(GnuAttributeSpecifierAST* node, GnuAttributeSpecifierAST* pattern) override;
    bool match(NamedTypeSpecifierAST* node, NamedTypeSpecifierAST* pattern) override;
    bool match(SimpleSpecifierAST* node, SimpleSpecifierAST* pattern) override;
    bool match(TypeofSpecifierAST* node, TypeofSpecifierAST* pattern) override;

    // Declarators
    bool match(DeclaratorAST* node, DeclaratorAST* pattern) override;
    bool match(DeclaratorIdAST* node, DeclaratorIdAST* pattern) override;
    bool match(PointerAST* node, PointerAST* pattern) override;

    // Names
    bool match(SimpleNameAST* node, SimpleNameAST* pattern) override;
    bool match(TaggedNameAST* node, TaggedNameAST* pattern) override;
};

} // namespace psyche

#endif
