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

#ifndef PSYCHE_TYPEOFEXPRESSION_H__
#define PSYCHE_TYPEOFEXPRESSION_H__

#include "FrontendConfig.h"
#include "ASTVisitor.h"
#include "FullySpecifiedType.h"
#include <vector>
#include <cstddef>

namespace psyche {

/*!
 * \brief The TypeOfExpression class
 */
class CFE_API TypeOfExpression final : psyche::ASTVisitor
{
public:
    TypeOfExpression(psyche::TranslationUnit* unit);

    psyche::FullySpecifiedType resolve(psyche::ExpressionAST* ast,
                                          psyche::Scope* scope);

    psyche::FullySpecifiedType commonRealType(const psyche::FullySpecifiedType&,
                                                 const psyche::FullySpecifiedType&) const;

    psyche::FullySpecifiedType promotedType(const psyche::FullySpecifiedType&) const;

private:
    void process(const psyche::Identifier* id);

    // Expressions
    bool visit(psyche::ArrayAccessAST* ast) override;
    bool visit(psyche::BinaryExpressionAST* ast) override;
    bool visit(psyche::CallAST *ast) override;
    bool visit(psyche::CastExpressionAST *ast) override;
    bool visit(psyche::ConditionalExpressionAST *ast) override;
    bool visit(psyche::IdExpressionAST* ast) override;
    bool visit(psyche::MemberAccessAST* ast) override;
    bool visit(psyche::NumericLiteralAST* ast) override;
    bool visit(psyche::BoolLiteralAST* ast) override;
    bool visit(psyche::StringLiteralAST* ast) override;
    bool visit(psyche::UnaryExpressionAST* ast) override;
    bool visit(psyche::SizeofExpressionAST* ast) override;
    bool visit(psyche::PointerLiteralAST* ast) override;
    bool visit(psyche::TypeIdAST* ast) override;

    // Specifiers
    bool visit(psyche::SimpleSpecifierAST* ast) override;
    bool visit(psyche::NamedTypeSpecifierAST* ast) override;
    bool visit(psyche::ElaboratedTypeSpecifierAST* ast) override;

    // Declarators
    bool visit(psyche::DeclaratorAST* ast) override;
    bool visit(psyche::PointerAST* ast) override;

    psyche::FullySpecifiedType fullType_;
    psyche::Scope* scope_;
    std::size_t searchMember_;
};

} // namespace psyche

#endif
