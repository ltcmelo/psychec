// Copyright (c) 2016/17/18/19/20/21 Leandro T. C. Melo <ltcmelo@gmail.com>
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

#ifndef PSYCHE_VISITOR_OBSERVER_H__
#define PSYCHE_VISITOR_OBSERVER_H__

#include "PluginConfig.h"
#include "APIFwds.h"

namespace psy {
namespace C {

class PLUGIN_API VisitorObserver {
public:
    virtual ~VisitorObserver() {}

    // Declarations
    virtual void enter(DeclaratorDeclarationSyntax*, ScopeSymbol*) {}
    virtual void enter(FunctionDefinitionSyntax*, ScopeSymbol*) {}

    virtual void leave(DeclaratorDeclarationSyntax*) {}
    virtual void leave(FunctionDefinitionSyntax*) {}

    // Expressions
    virtual void enter(ArraySubscriptExpressionSyntax*, ScopeSymbol*) {}
    virtual void enter(BinaryExpressionSyntax*, ScopeSymbol*) {}
    virtual void enter(CallExpressionSyntax*, ScopeSymbol*) {}
    virtual void enter(CastExpressionSyntax*, ScopeSymbol*) {}
    virtual void enter(ConditionalExpressionSyntax*, ScopeSymbol*) {}
    virtual void enter(IdentifierExpressionSyntax*, ScopeSymbol*) {}
    virtual void enter(MemberAccessExpressionSyntax*, ScopeSymbol*) {}
    virtual void enter(ConstantExpressionSyntax*, ScopeSymbol*) {}
    virtual void enter(StringLiteralExpressionSyntax*, ScopeSymbol*) {}
    virtual void enter(UnaryExpressionSyntax*, ScopeSymbol*) {}
    virtual void enter(TypeTraitExpressionSyntax*, ScopeSymbol*) {}
    virtual void enter(BraceEnclosedInitializerSyntax*, ScopeSymbol*) {}
//    virtual void enter(PostIncrDecrSyntax*, ScopeSymbol*) {}

    virtual void leave(ArraySubscriptExpressionSyntax*) {}
    virtual void leave(BinaryExpressionSyntax*) {}
    virtual void leave(CallExpressionSyntax*) {}
    virtual void leave(CastExpressionSyntax*) {}
    virtual void leave(ConditionalExpressionSyntax*) {}
    virtual void leave(IdentifierExpressionSyntax*) {}
    virtual void leave(MemberAccessExpressionSyntax*) {}
    virtual void leave(ConstantExpressionSyntax*) {}
    virtual void leave(StringLiteralExpressionSyntax*) {}
    virtual void leave(UnaryExpressionSyntax*) {}
    virtual void leave(TypeTraitExpressionSyntax*) {}
    virtual void leave(BraceEnclosedInitializerSyntax*) {}
//    virtual void leave(PostIncrDecrSyntax*) {}

    // Specifiers
    virtual void enter(TaggedTypeSpecifierSyntax*, ScopeSymbol*) {}

    virtual void leave(TaggedTypeSpecifierSyntax*) {}

    // Statements
    virtual void enter(CompoundStatementSyntax*, ScopeSymbol*) {}
    virtual void enter(DeclarationStatementSyntax*, ScopeSymbol*) {}
    virtual void enter(DoStatementSyntax*, ScopeSymbol*) {}
    virtual void enter(ForStatementSyntax*, ScopeSymbol*) {}
    virtual void enter(ExpressionStatementSyntax*, ScopeSymbol*) {}
    virtual void enter(IfStatementSyntax*, ScopeSymbol*) {}
    virtual void enter(ReturnStatementSyntax*, ScopeSymbol*) {}
    virtual void enter(SwitchStatementSyntax*, ScopeSymbol*) {}
    virtual void enter(WhileStatementSyntax*, ScopeSymbol*) {}

    virtual void leave(CompoundStatementSyntax*) {}
    virtual void leave(DeclarationStatementSyntax*) {}
    virtual void leave(DoStatementSyntax*) {}
    virtual void leave(ForStatementSyntax*) {}
    virtual void leave(ExpressionStatementSyntax*) {}
    virtual void leave(IfStatementSyntax*) {}
    virtual void leave(ReturnStatementSyntax*) {}
    virtual void leave(SwitchStatementSyntax*) {}
    virtual void leave(WhileStatementSyntax*) {}

    // Miscellanea
    virtual void enter(TranslationUnitSyntax*, ScopeSymbol*) {}
    virtual void enter(EnumMemberDeclarationSyntax*, ScopeSymbol*) {}

    virtual void leave(TranslationUnitSyntax*) {}
    virtual void leave(EnumMemberDeclarationSyntax*) {}

    virtual void withinFunction() {}
    virtual void outsideFunction() {}
};

template <class AstT>
struct ObserverInvoker
{
    ObserverInvoker(VisitorObserver* o,
                    AstT* ast,
                    ScopeSymbol* scope) : o_(o), ast_(ast)
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

} // C
} // psy

#endif
