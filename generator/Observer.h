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
#include "CPlusPlusForwardDeclarations.h"

namespace psyche {

class ConstraintWriter;

class Observer {
public:
    virtual ~Observer() {}

    virtual void configure(CPlusPlus::TranslationUnit* unit, ConstraintWriter* writer) {}

    // Miscellanea
    virtual void enter(CPlusPlus::TranslationUnitAST*, CPlusPlus::Scope*) {}
    virtual void enter(CPlusPlus::EnumeratorAST*, CPlusPlus::Scope*) {}

    virtual void leave(CPlusPlus::TranslationUnitAST*) {}
    virtual void leave(CPlusPlus::EnumeratorAST*) {}

    // Declarations
    virtual void enter(CPlusPlus::SimpleDeclarationAST*, CPlusPlus::Scope*) {}
    virtual void enter(CPlusPlus::FunctionDefinitionAST*, CPlusPlus::Scope*) {}

    virtual void leave(CPlusPlus::SimpleDeclarationAST*) {}
    virtual void leave(CPlusPlus::FunctionDefinitionAST*) {}

    // Expressions
    virtual void enter(CPlusPlus::ArrayAccessAST*, CPlusPlus::Scope*) {}
    virtual void enter(CPlusPlus::BinaryExpressionAST*, CPlusPlus::Scope*) {}
    virtual void enter(CPlusPlus::CallAST*, CPlusPlus::Scope*) {}
    virtual void enter(CPlusPlus::CastExpressionAST*, CPlusPlus::Scope*) {}
    virtual void enter(CPlusPlus::ConditionalExpressionAST*, CPlusPlus::Scope*) {}
    virtual void enter(CPlusPlus::IdExpressionAST*, CPlusPlus::Scope*) {}
    virtual void enter(CPlusPlus::MemberAccessAST*, CPlusPlus::Scope*) {}
    virtual void enter(CPlusPlus::NumericLiteralAST*, CPlusPlus::Scope*) {}
    virtual void enter(CPlusPlus::BoolLiteralAST*, CPlusPlus::Scope*) {}
    virtual void enter(CPlusPlus::StringLiteralAST*, CPlusPlus::Scope*) {}
    virtual void enter(CPlusPlus::UnaryExpressionAST*, CPlusPlus::Scope*) {}
    virtual void enter(CPlusPlus::SizeofExpressionAST*, CPlusPlus::Scope*) {}
    virtual void enter(CPlusPlus::PointerLiteralAST*, CPlusPlus::Scope*) {}
    virtual void enter(CPlusPlus::BracedInitializerAST*, CPlusPlus::Scope*) {}
    virtual void enter(CPlusPlus::PostIncrDecrAST*, CPlusPlus::Scope*) {}

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
    virtual void enter(CPlusPlus::EnumSpecifierAST*, CPlusPlus::Scope*) {}
    virtual void enter(CPlusPlus::ClassSpecifierAST*, CPlusPlus::Scope*) {}

    virtual void leave(CPlusPlus::EnumSpecifierAST*) {}
    virtual void leave(CPlusPlus::ClassSpecifierAST*) {}

    // Statements
    virtual void enter(CPlusPlus::CaseStatementAST*, CPlusPlus::Scope*) {}
    virtual void enter(CPlusPlus::CompoundStatementAST*, CPlusPlus::Scope*) {}
    virtual void enter(CPlusPlus::DeclarationStatementAST*, CPlusPlus::Scope*) {}
    virtual void enter(CPlusPlus::DoStatementAST*, CPlusPlus::Scope*) {}
    virtual void enter(CPlusPlus::ForStatementAST*, CPlusPlus::Scope*) {}
    virtual void enter(CPlusPlus::ExpressionStatementAST*, CPlusPlus::Scope*) {}
    virtual void enter(CPlusPlus::IfStatementAST*, CPlusPlus::Scope*) {}
    virtual void enter(CPlusPlus::ReturnStatementAST*, CPlusPlus::Scope*) {}
    virtual void enter(CPlusPlus::SwitchStatementAST*, CPlusPlus::Scope*) {}
    virtual void enter(CPlusPlus::WhileStatementAST*, CPlusPlus::Scope*) {}

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

    // Miscellanea
    virtual void withinFunction() {}
    virtual void outsideFunction() {}
};

template <class AstT>
struct ObserverInvoker
{
    ObserverInvoker(Observer* o,
                    AstT* ast,
                    CPlusPlus::Scope* scope) : o_(o), ast_(ast)
    {
        o_->enter(ast_, scope);
    }
    ~ObserverInvoker() { o_->leave(ast_); }

    Observer* o_;
    AstT* ast_;
};

} // namespace psyche

#endif
