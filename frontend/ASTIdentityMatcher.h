/******************************************************************************
 Copyright (c) 2016,17 Leandro T. C. Melo (ltcmelo@gmail.com)

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
    bool match(ArrayAccessAST *node, ArrayAccessAST *pattern) override;
    bool match(ArrayInitializerAST *node, ArrayInitializerAST *pattern) override;
    bool match(BinaryExpressionAST *node, BinaryExpressionAST *pattern) override;
    bool match(BoolLiteralAST *node, BoolLiteralAST *pattern) override;
    bool match(BracedInitializerAST *node, BracedInitializerAST *pattern) override;
    bool match(BracketDesignatorAST *node, BracketDesignatorAST *pattern) override;
    bool match(DotDesignatorAST* node, DotDesignatorAST* pattern) override;
    bool match(CallAST *node, CallAST *pattern) override;
    bool match(CastExpressionAST *node, CastExpressionAST *pattern) override;
    bool match(ConditionalExpressionAST *node, ConditionalExpressionAST *pattern) override;
    bool match(IdExpressionAST *node, IdExpressionAST *pattern) override;
    bool match(MemberAccessAST *node, MemberAccessAST *pattern) override;
    bool match(NestedExpressionAST *node, NestedExpressionAST *pattern) override;
    bool match(NumericLiteralAST *node, NumericLiteralAST *pattern) override;
    bool match(StringLiteralAST* node, StringLiteralAST* pattern) override;
    bool match(PointerLiteralAST *node, PointerLiteralAST *pattern) override;
    bool match(PostIncrDecrAST *node, PostIncrDecrAST *pattern) override;
    bool match(SizeofExpressionAST *node, SizeofExpressionAST *pattern) override;
    bool match(TypeIdAST* node, TypeIdAST* pattern) override;
    bool match(UnaryExpressionAST *node, UnaryExpressionAST *pattern) override;

    // Specifiers
    bool match(ClassSpecifierAST *node, ClassSpecifierAST *pattern) override;
    bool match(ElaboratedTypeSpecifierAST *node, ElaboratedTypeSpecifierAST *pattern) override;
    bool match(EnumSpecifierAST *node, EnumSpecifierAST *pattern) override;
    bool match(GnuAttributeSpecifierAST *node, GnuAttributeSpecifierAST *pattern) override;
    bool match(NamedTypeSpecifierAST *node, NamedTypeSpecifierAST *pattern) override;
    bool match(SimpleSpecifierAST *node, SimpleSpecifierAST *pattern) override;
    bool match(TypeofSpecifierAST *node, TypeofSpecifierAST *pattern) override;

    // Declarators
    bool match(DeclaratorAST *node, DeclaratorAST *pattern) override;
    bool match(DeclaratorIdAST *node, DeclaratorIdAST *pattern) override;
    bool match(PointerAST *node, PointerAST *pattern) override;

    // Names
    bool match(SimpleNameAST* node, SimpleNameAST* pattern) override;
    bool match(TaggedNameAST* node, TaggedNameAST* pattern) override;
};

} // namespace psyche

#endif
