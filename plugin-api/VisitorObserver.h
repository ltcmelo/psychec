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

#ifndef PSYCHE_VISITOR_OBSERVER_H__
#define PSYCHE_VISITOR_OBSERVER_H__

#include "PluginConfig.h"
#include "FrontendFwds.h"
#include "ASTFwds.h"

namespace psyche {

class PLUGIN_API VisitorObserver {
public:
    virtual ~VisitorObserver() {}

    // Declarations
    virtual void enter(psyche::SimpleDeclarationAST*, psyche::Scope*) {}
    virtual void enter(psyche::FunctionDefinitionAST*, psyche::Scope*) {}

    virtual void leave(psyche::SimpleDeclarationAST*) {}
    virtual void leave(psyche::FunctionDefinitionAST*) {}

    // Expressions
    virtual void enter(psyche::ArrayAccessAST*, psyche::Scope*) {}
    virtual void enter(psyche::BinaryExpressionAST*, psyche::Scope*) {}
    virtual void enter(psyche::CallAST*, psyche::Scope*) {}
    virtual void enter(psyche::CastExpressionAST*, psyche::Scope*) {}
    virtual void enter(psyche::ConditionalExpressionAST*, psyche::Scope*) {}
    virtual void enter(psyche::IdExpressionAST*, psyche::Scope*) {}
    virtual void enter(psyche::MemberAccessAST*, psyche::Scope*) {}
    virtual void enter(psyche::NumericLiteralAST*, psyche::Scope*) {}
    virtual void enter(psyche::BoolLiteralAST*, psyche::Scope*) {}
    virtual void enter(psyche::StringLiteralAST*, psyche::Scope*) {}
    virtual void enter(psyche::UnaryExpressionAST*, psyche::Scope*) {}
    virtual void enter(psyche::SizeofExpressionAST*, psyche::Scope*) {}
    virtual void enter(psyche::PointerLiteralAST*, psyche::Scope*) {}
    virtual void enter(psyche::BracedInitializerAST*, psyche::Scope*) {}
    virtual void enter(psyche::PostIncrDecrAST*, psyche::Scope*) {}

    virtual void leave(psyche::ArrayAccessAST*) {}
    virtual void leave(psyche::BinaryExpressionAST*) {}
    virtual void leave(psyche::CallAST*) {}
    virtual void leave(psyche::CastExpressionAST*) {}
    virtual void leave(psyche::ConditionalExpressionAST*) {}
    virtual void leave(psyche::IdExpressionAST*) {}
    virtual void leave(psyche::MemberAccessAST*) {}
    virtual void leave(psyche::NumericLiteralAST*) {}
    virtual void leave(psyche::BoolLiteralAST*) {}
    virtual void leave(psyche::StringLiteralAST*) {}
    virtual void leave(psyche::UnaryExpressionAST*) {}
    virtual void leave(psyche::SizeofExpressionAST*) {}
    virtual void leave(psyche::PointerLiteralAST*) {}
    virtual void leave(psyche::BracedInitializerAST*) {}
    virtual void leave(psyche::PostIncrDecrAST*) {}

    // Specifiers
    virtual void enter(psyche::EnumSpecifierAST*, psyche::Scope*) {}
    virtual void enter(psyche::ClassSpecifierAST*, psyche::Scope*) {}

    virtual void leave(psyche::EnumSpecifierAST*) {}
    virtual void leave(psyche::ClassSpecifierAST*) {}

    // Statements
    virtual void enter(psyche::CaseStatementAST*, psyche::Scope*) {}
    virtual void enter(psyche::CompoundStatementAST*, psyche::Scope*) {}
    virtual void enter(psyche::DeclarationStatementAST*, psyche::Scope*) {}
    virtual void enter(psyche::DoStatementAST*, psyche::Scope*) {}
    virtual void enter(psyche::ForStatementAST*, psyche::Scope*) {}
    virtual void enter(psyche::ExpressionStatementAST*, psyche::Scope*) {}
    virtual void enter(psyche::IfStatementAST*, psyche::Scope*) {}
    virtual void enter(psyche::ReturnStatementAST*, psyche::Scope*) {}
    virtual void enter(psyche::SwitchStatementAST*, psyche::Scope*) {}
    virtual void enter(psyche::WhileStatementAST*, psyche::Scope*) {}

    virtual void leave(psyche::CaseStatementAST*) {}
    virtual void leave(psyche::CompoundStatementAST*) {}
    virtual void leave(psyche::DeclarationStatementAST*) {}
    virtual void leave(psyche::DoStatementAST*) {}
    virtual void leave(psyche::ForStatementAST*) {}
    virtual void leave(psyche::ExpressionStatementAST*) {}
    virtual void leave(psyche::IfStatementAST*) {}
    virtual void leave(psyche::ReturnStatementAST*) {}
    virtual void leave(psyche::SwitchStatementAST*) {}
    virtual void leave(psyche::WhileStatementAST*) {}

    // Miscellanea
    virtual void enter(psyche::TranslationUnitAST*, psyche::Scope*) {}
    virtual void enter(psyche::EnumeratorAST*, psyche::Scope*) {}

    virtual void leave(psyche::TranslationUnitAST*) {}
    virtual void leave(psyche::EnumeratorAST*) {}

    virtual void withinFunction() {}
    virtual void outsideFunction() {}
};

template <class AstT>
struct ObserverInvoker
{
    ObserverInvoker(VisitorObserver* o,
                    AstT* ast,
                    psyche::Scope* scope) : o_(o), ast_(ast)
    {
        if (o_)
            o_->enter(ast_, scope);
    }

    ~ObserverInvoker()
    {
        if (o_)
            o_->leave(ast_);
    }

    VisitorObserver* o_;
    AstT* ast_;
};

} // namespace psyche

#endif
