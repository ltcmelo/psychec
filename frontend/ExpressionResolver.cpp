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

#include "ExpressionResolver.h"

#include "AST.h"
#include "CoreTypes.h"
#include "Control.h"
#include "Literals.h"
#include "Lookup.h"
#include "Scope.h"
#include "Symbol.h"
#include "Symbols.h"
#include "TranslationUnit.h"
#include <cassert>

using namespace psyche;

ExpressionResolver::ExpressionResolver(TranslationUnit *unit)
    : ASTVisitor(unit)
    , sym_(nullptr)
{}

Symbol *ExpressionResolver::resolve(ExpressionAST *ast, Scope *scope)
{
    scope_ = scope;
    accept(ast);

    return sym_;
}

bool ExpressionResolver::visit(ArrayAccessAST *ast)
{
    accept(ast->base_expression);

    return false;
}

bool ExpressionResolver::visit(CallAST *ast)
{
    accept(ast->base_expression);

    return false;
}

bool ExpressionResolver::visit(IdExpressionAST *ast)
{
    sym_ = lookupSymbol(ast->name->name, scope_);

    if (sym_ && sym_->asScope())
        scope_ = sym_->asScope();

    return false;
}

bool ExpressionResolver::visit(MemberAccessAST *ast)
{
    accept(ast->base_expression);
    accept(ast->member_name);

    return false;
}
