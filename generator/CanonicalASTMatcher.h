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

#ifndef PSYCHE_CANONICALASTMATCHER_
#define PSYCHE_CANONICALASTMATCHER_

#include "AST.h"
#include "ASTMatcher.h"
#include "Name.h"
#include "Literals.h"
#include "TranslationUnit.h"
#include <cstring>

using namespace CPlusPlus;

namespace psyche {

struct CanonicalASTMatcher final : public CPlusPlus::ASTMatcher
{
    CanonicalASTMatcher(TranslationUnit* unit)
        : unit_(unit)
    {}

    TranslationUnit* unit_;

    bool match(ArrayAccessAST *node, ArrayAccessAST *pattern) override
    {
        if (!node || !pattern)
            return false;
        if (!AST::match(node->base_expression, pattern->base_expression, this))
            return false;
        return true;
    }

    bool match(ArrayInitializerAST *node, ArrayInitializerAST *pattern) override
    {
        if (!node || !pattern)
            return false;
        if (!AST::match(node->expression_list, pattern->expression_list, this))
            return false;
        return true;
    }

    bool match(BinaryExpressionAST *node, BinaryExpressionAST *pattern) override
    {
        if (!node || !pattern)
            return false;
        if (!AST::match(node->left_expression, pattern->left_expression, this))
            return false;
        if (unit_->tokenKind(node->binary_op_token) != unit_->tokenKind(pattern->binary_op_token))
            return false;
        if (!AST::match(node->right_expression, pattern->right_expression, this))
            return false;
        return true;
    }

    bool match(BoolLiteralAST *node, BoolLiteralAST *pattern) override
    {
        if (!node || !pattern)
            return false;
        if (unit_->tokenKind(node->literal_token) != unit_->tokenKind(pattern->literal_token))
            return false;
        return true;
    }

    bool match(BracedInitializerAST *node, BracedInitializerAST *pattern) override
    {
        if (!node || !pattern)
            return false;
        if (!AST::match(node->expression_list, pattern->expression_list, this))
            return false;
        return true;
    }

    bool match(BracketDesignatorAST *node, BracketDesignatorAST *pattern) override
    {
        if (!node || !pattern)
            return false;
        if (!AST::match(node->expression, pattern->expression, this))
            return false;
        return true;
    }

    bool match(CallAST *node, CallAST *pattern) override
    {
        if (!node || !pattern)
            return false;
        if (!AST::match(node->base_expression, pattern->base_expression, this))
            return false;
        return true;
    }

    bool match(CastExpressionAST *node, CastExpressionAST *pattern) override
    {
        if (!node || !pattern)
            return false;
        if (!AST::match(node->type_id, pattern->type_id, this))
            return false;
        if (!AST::match(node->expression, pattern->expression, this))
            return false;
        return true;
    }

    bool match(ConditionalExpressionAST *node, ConditionalExpressionAST *pattern) override
    {
        if (!node || !pattern)
            return false;
        if (!AST::match(node->condition, pattern->condition, this))
            return false;
        if (!AST::match(node->left_expression, pattern->left_expression, this))
            return false;
        if (!AST::match(node->right_expression, pattern->right_expression, this))
            return false;
        return true;
    }

    bool match(IdExpressionAST *node, IdExpressionAST *pattern) override
    {
        if (!node || !pattern)
            return false;
        if (!AST::match(node->name, pattern->name, this))
            return false;
        return true;
    }

    bool match(MemberAccessAST *node, MemberAccessAST *pattern) override
    {
        if (!node || !pattern)
            return false;
        if (!AST::match(node->base_expression, pattern->base_expression, this))
            return false;
        if (unit_->tokenKind(node->access_token) != unit_->tokenKind(pattern->access_token))
            return false;
        if (!AST::match(node->member_name, pattern->member_name, this))
            return false;
        return true;
    }

    bool match(NestedExpressionAST *node, NestedExpressionAST *pattern) override
    {
        if (!node || !pattern)
            return false;
        if (!AST::match(node->expression, pattern->expression, this))
            return false;
        return true;
    }

    bool match(NumericLiteralAST *node, NumericLiteralAST *pattern) override
    {
        if (!node || !pattern)
            return false;
        if (unit_->tokenKind(node->literal_token) != unit_->tokenKind(pattern->literal_token))
            return false;
        if (unit_->spell(node->literal_token) != unit_->spell(pattern->literal_token))
            return false;
        return true;
    }

    bool match(StringLiteralAST* node, StringLiteralAST* pattern) override
    {
        if (!node || !pattern)
            return false;
        if (unit_->tokenKind(node->literal_token) != unit_->tokenKind(pattern->literal_token))
            return false;
        if (unit_->spell(node->literal_token) != unit_->spell(pattern->literal_token))
            return false;
        if (!AST::match(node->next, pattern->next, this))
            return false;
        return true;
    }

    bool match(PointerLiteralAST *node, PointerLiteralAST *pattern) override
    {
        if (!node || !pattern)
            return false;
        if (unit_->tokenKind(node->literal_token) != unit_->tokenKind(pattern->literal_token))
            return false;
        return true;
    }

    bool match(PostIncrDecrAST *node, PostIncrDecrAST *pattern) override
    {
        if (!node || !pattern)
            return false;
        if (!AST::match(node->base_expression, pattern->base_expression, this))
            return false;
        if (unit_->tokenKind(node->incr_decr_token) != unit_->tokenKind(pattern->incr_decr_token))
            return false;
        return true;
    }

    bool match(SizeofExpressionAST *node, SizeofExpressionAST *pattern) override
    {
        if (!node || !pattern)
            return false;
        if (!AST::match(node->expression, pattern->expression, this))
            return false;
        return true;
    }

    bool match(UnaryExpressionAST *node, UnaryExpressionAST *pattern) override
    {
        if (!node || !pattern)
            return false;
        if (!AST::match(node->expression, pattern->expression, this))
            return false;
        if (unit_->tokenKind(node->unary_op_token) != unit_->tokenKind(pattern->unary_op_token))
            return false;
        return true;
    }

    bool match(SimpleNameAST* node, SimpleNameAST* pattern) override
    {
        if (!node || !pattern)
            return false;
        return !(strcmp(node->name->identifier()->begin(),
                        pattern->name->identifier()->begin()));
    }

    bool match(ElaboratedNameAST* node, ElaboratedNameAST* pattern) override
    {
        if (!node || !pattern)
            return false;
        return !(strcmp(node->name->identifier()->begin(),
                        pattern->name->identifier()->begin()));
    }
};

} // namespace psyche

#endif
