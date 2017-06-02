/******************************************************************************
 Copyright (c) 2017 Leandro T. C. Melo (ltcmelo@gmail.com)

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

#ifndef PSYCHE_OBSERVER_H__
#define PSYCHE_OBSERVER_H__

#include "ASTfwd.h"

namespace psyche {

class Observer {
public:
    ~Observer() {}

    // Declarations
    virtual void enter(CPlusPlus::SimpleDeclarationAST*) {}
    virtual void enter(CPlusPlus::FunctionDefinitionAST*) {}

    // Expressions
    virtual void enter(CPlusPlus::ArrayAccessAST*) {}
    virtual void enter(CPlusPlus::BinaryExpressionAST*) {}
    virtual void enter(CPlusPlus::CallAST*) {}
    virtual void enter(CPlusPlus::CastExpressionAST*) {}
    virtual void enter(CPlusPlus::ConditionalExpressionAST*) {}
    virtual void enter(CPlusPlus::IdExpressionAST*) {}
    virtual void enter(CPlusPlus::MemberAccessAST*) {}
    virtual void enter(CPlusPlus::NumericLiteralAST*) {}
    virtual void enter(CPlusPlus::BoolLiteralAST*) {}
    virtual void enter(CPlusPlus::StringLiteralAST*) {}
    virtual void enter(CPlusPlus::UnaryExpressionAST*) {}
    virtual void enter(CPlusPlus::SizeofExpressionAST*) {}
    virtual void enter(CPlusPlus::PointerLiteralAST*) {}
    virtual void enter(CPlusPlus::BracedInitializerAST*) {}
    virtual void enter(CPlusPlus::PostIncrDecrAST*) {}

    // Specifiers
    virtual void enter(CPlusPlus::EnumSpecifierAST*) {}
    virtual void enter(CPlusPlus::ClassSpecifierAST*) {}

    // Statements
    virtual void enter(CPlusPlus::CaseStatementAST*) {}
    virtual void enter(CPlusPlus::CompoundStatementAST*) {}
    virtual void enter(CPlusPlus::DeclarationStatementAST*) {}
    virtual void enter(CPlusPlus::DoStatementAST*) {}
    virtual void enter(CPlusPlus::ForStatementAST*) {}
    virtual void enter(CPlusPlus::ExpressionStatementAST*) {}
    virtual void enter(CPlusPlus::IfStatementAST*) {}
    virtual void enter(CPlusPlus::ReturnStatementAST*) {}
    virtual void enter(CPlusPlus::SwitchStatementAST*) {}
    virtual void enter(CPlusPlus::WhileStatementAST*) {}

    // Declarations
    virtual void leave(CPlusPlus::SimpleDeclarationAST*) {}
    virtual void leave(CPlusPlus::FunctionDefinitionAST*) {}

    // Expressions
    virtual void leave(CPlusPlus::ArrayAccessAST*) {}
    virtual void leave(CPlusPlus::BinaryExpressionAST*) {}
    virtual void leave(CPlusPlus::CallAST*) {}
    virtual void leave(CPlusPlus::CastExpressionAST*) {}
    virtual void leave(CPlusPlus::ConditionalExpressionAST*) {}
    virtual void leave(CPlusPlus::IdExpressionAST*) {}
    virtual void leave(CPlusPlus::MemberAccessAST*) {}
    virtual void leave(CPlusPlus::NumericLiteralAST*) {}
    virtual void leave(CPlusPlus::BoolLiteralAST*) {}
    virtual void leave(CPlusPlus::StringLiteralAST*) {}
    virtual void leave(CPlusPlus::UnaryExpressionAST*) {}
    virtual void leave(CPlusPlus::SizeofExpressionAST*) {}
    virtual void leave(CPlusPlus::PointerLiteralAST*) {}
    virtual void leave(CPlusPlus::BracedInitializerAST*) {}
    virtual void leave(CPlusPlus::PostIncrDecrAST*) {}

    // Specifiers
    virtual void leave(CPlusPlus::EnumSpecifierAST*) {}
    virtual void leave(CPlusPlus::ClassSpecifierAST*) {}

    // Statements
    virtual void leave(CPlusPlus::CaseStatementAST*) {}
    virtual void leave(CPlusPlus::CompoundStatementAST*) {}
    virtual void leave(CPlusPlus::DeclarationStatementAST*) {}
    virtual void leave(CPlusPlus::DoStatementAST*) {}
    virtual void leave(CPlusPlus::ForStatementAST*) {}
    virtual void leave(CPlusPlus::ExpressionStatementAST*) {}
    virtual void leave(CPlusPlus::IfStatementAST*) {}
    virtual void leave(CPlusPlus::ReturnStatementAST*) {}
    virtual void leave(CPlusPlus::SwitchStatementAST*) {}
    virtual void leave(CPlusPlus::WhileStatementAST*) {}
};

template <class AstT>
struct ObserverInvoker
{
    ObserverInvoker(Observer* o, AstT* ast) : o_(o), ast_(ast) { o_->enter(ast_); }
    ~ObserverInvoker() { o_->leave(ast_); }

    Observer* o_;
    AstT* ast_;
};

} // namespace psyche

#endif
