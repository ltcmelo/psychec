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

#ifndef PSYCHE_TYPEOFEXPR_H__
#define PSYCHE_TYPEOFEXPR_H__

#include "ASTVisitor.h"
#include "CPlusPlusForwardDeclarations.h"
#include "FullySpecifiedType.h"
#include <vector>
#include <cstddef>

namespace psyche {

/*!
 * \brief The TypeOfExpr class
 */
class TypeOfExpr final : CPlusPlus::ASTVisitor
{
public:
    TypeOfExpr(CPlusPlus::TranslationUnit *unit);

    CPlusPlus::FullySpecifiedType resolve(CPlusPlus::ExpressionAST *ast,
                                          CPlusPlus::Scope *scope);

private:
    CPlusPlus::Scope *switchScope(CPlusPlus::Scope *scope);

    void process(const CPlusPlus::Identifier* id);

    bool visit(CPlusPlus::ArrayAccessAST* ast) override;
    bool visit(CPlusPlus::BinaryExpressionAST* ast) override;
    bool visit(CPlusPlus::CallAST *ast) override;
    bool visit(CPlusPlus::CastExpressionAST *ast) override;
    bool visit(CPlusPlus::ConditionalExpressionAST *ast) override;
    bool visit(CPlusPlus::IdExpressionAST* ast) override;
    bool visit(CPlusPlus::MemberAccessAST* ast) override;
    bool visit(CPlusPlus::NumericLiteralAST* ast) override;
    bool visit(CPlusPlus::BoolLiteralAST* ast) override;
    bool visit(CPlusPlus::StringLiteralAST* ast) override;
    bool visit(CPlusPlus::UnaryExpressionAST* ast) override;
    bool visit(CPlusPlus::SizeofExpressionAST* ast) override;
    bool visit(CPlusPlus::PointerLiteralAST* ast) override;

    std::vector<CPlusPlus::FullySpecifiedType> fullType_;
    CPlusPlus::Scope* scope_;
    std::size_t singleLookup_;
};

} // namespace psyche

#endif
