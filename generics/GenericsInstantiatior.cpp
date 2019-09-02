/******************************************************************************
 Copyright (c) 2019 Leandro T. C. Melo (ltcmelo@gmail.com)

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

#include "GenericsInstantiatior.h"

#include "AST.h"
#include "Assert.h"
#include "BuiltinNames.h"
#include "Control.h"
#include "CoreTypes.h"
#include "DeclarationInterceptor.h"
#include "Debug.h"
#include "Literals.h"
#include "PrintfScanner.h"
#include "Scope.h"
#include "Symbols.h"
#include "TranslationUnit.h"
#include "VisitorObserver.h"
#include <algorithm>
#include <iostream>
#include <sstream>

using namespace psyche;

GenericsInstantiatior::GenericsInstantiatior(TranslationUnit* unit)
    : ASTDumper(unit)
    , isLineStart_(true)
    , typeof_(unit)
    , resolver_(unit)
    , scope_(nullptr)
{}

int GenericsInstantiatior::expand(AST* ast, psyche::Scope* global, std::ostream& os)
{
    os_ = &os;
    switchScope(global);
    accept(ast);

    if (funcTbl_.empty())
        return 0;

    *os_ << "\n/* psychec: generics expansion */\n";
    for (const auto& f : funcTbl_) {
        std::ostringstream oss;

        std::ostream* prevStream = os_;
        os_ = &oss;
        FunctionDefinitionAST* ast = f.second.ast_;
        for (SpecifierListAST* it = ast->decl_specifier_list; it; it = it->next)
            nonterminal(it->value);
        nonterminal(ast->declarator);
        nonterminal(ast->function_body);
        os_ = prevStream;

        for (const auto& data : f.second.overloads_) {
            std::string substituted = oss.str();
            for (const auto& sub : data.second.subs_)
                substituted = apply(sub, substituted);
            *os_ << substituted << "\n";
        }
    }
    *os_ << "/* psychec: end */\n";

    return 1;
}

void GenericsInstantiatior::terminal(unsigned tok, AST* ast)
{
    auto tokk = translationUnit()->tokenKind(tok);
    if (tokk == T_EOF_SYMBOL)
        return;

    if (!isLineStart_)
        *os_ << " ";
    *os_ <<  spell(tok);
    if (tokk == T_RBRACE
            || tokk == T_LBRACE
            || tokk == T_SEMICOLON) {
        *os_ << "\n";
        isLineStart_ = true;
    } else {
        isLineStart_ = false;
    }
}

int GenericsInstantiatior::recognize(Function* func, const std::vector<FullySpecifiedType>& argsTypes)
{
    std::string key;
    std::for_each(argsTypes.begin(), argsTypes.end(),
                  [&] (const auto& s) { key += typePP_.print(s, scope_) + ','; });

    auto it = funcTbl_.find(func);
    PSYCHE_ASSERT(it != funcTbl_.end(), return -1, "expected known function");

    auto it2 = it->second.overloads_.find(key);
    if (it2 == it->second.overloads_.end()) {
        auto cnt = it->second.overloads_.size();
        it->second.overloads_[key].label_ = cnt;

        std::string funcName(func->name()->identifier()->chars());
        it->second.overloads_[key].subs_.emplace_back(funcName, funcName + std::to_string(cnt));

        int cntU = func->argumentCount();
        int cntE = argsTypes.size();
        PSYCHE_ASSERT(cntU == cntE, return 0, "expected matching argument count");

        for (auto i = 0L; i < func->argumentCount(); ++i) {
            const FullySpecifiedType& tyU = func->argumentAt(i)->asArgument()->type().coreType();
            const FullySpecifiedType& tyE = argsTypes[i].coreType();
            it->second.overloads_[key].subs_.emplace_back(typePP_.print(tyU, scope_),
                                                          typePP_.print(tyE, scope_));
        }
    }

    return it->second.overloads_[key].label_;
}

Scope* GenericsInstantiatior::switchScope(Scope* scope)
{
    if (!scope)
        return scope_;
    std::swap(scope_, scope);
    return scope;
}

bool GenericsInstantiatior::visit(FunctionDefinitionAST* ast)
{
    Function* func = ast->symbol;
    if (!func)
        return false;

    for (SpecifierListAST* it = ast->decl_specifier_list; it; it = it->next)
        nonterminal(it->value);
    nonterminal(ast->declarator);

    Scope* prevScope = switchScope(func->asScope());
    nonterminal(ast->function_body);
    switchScope(prevScope);

    if (func->isGeneric() && ast->function_body) {
        if (funcTbl_.find(func) != funcTbl_.end())
            translationUnit()->error(ast->firstToken(), "redefinition of generic function");
        FunctionInstantions f(ast);
        funcTbl_[func] = f;
    }

    return false;
}

bool GenericsInstantiatior::visit(SimpleDeclarationAST* ast)
{
    FullySpecifiedType coreTy;
    if (ast->symbols && ast->symbols->value->asDeclaration()) {
        Declaration* decl = ast->symbols->value->asDeclaration();
        coreTy = decl->type().coreType();
    }

    for (SpecifierListAST *it = ast->decl_specifier_list; it; it = it->next) {
        if (it->value->asQuantifiedTypeSpecifier())
            *os_ << "  " << typePP_.print(coreTy, scope_);
        else
            nonterminal(it->value);
    }

    for (DeclaratorListAST *it = ast->declarator_list; it; it = it->next) {
        nonterminal(it->value);
        if (it->delim_token)
            terminal(it->delim_token, ast);
    }

    if (ast->semicolon_token)
        terminal(ast->semicolon_token, ast);

    return false;
}

bool GenericsInstantiatior::visit(QuantifiedTypeSpecifierAST* ast)
{
    nonterminal(ast->name);

    return false;
}

bool GenericsInstantiatior::visit(CompoundStatementAST* ast)
{
    if (ast->lbrace_token)
        terminal(ast->lbrace_token, ast);

    Scope* prevScope = switchScope(ast->symbol);
    for (StatementListAST *it = ast->statement_list; it; it = it->next)
        nonterminal(it->value);
    switchScope(prevScope);

    if (ast->rbrace_token)
        terminal(ast->rbrace_token, ast);

    return false;
}

bool GenericsInstantiatior::visit(CallAST* ast)
{
    Symbol* sym = resolver_.resolve(ast->base_expression, scope_);
    if (!sym || !sym->asFunction() || !sym->asFunction()->isGeneric()) {
        Base::visit(ast);
        return false;
    }

    std::vector<FullySpecifiedType> paramsTypes;
    for (ExpressionListAST* it = ast->expression_list; it; it = it->next) {
        FullySpecifiedType ty = typeof_.evaluate(it->value, scope_);
        paramsTypes.push_back(ty);
    }

    Function* func = sym->asFunction();
    int n = recognize(func, paramsTypes);

    nonterminal(ast->base_expression);
    *os_ << std::to_string(n);

    if (ast->lparen_token)
        terminal(ast->lparen_token, ast);
    for (ExpressionListAST* it = ast->expression_list; it; it = it->next) {
        nonterminal(it->value);
        if (it->delim_token)
            terminal(it->delim_token, ast);
    }
    if (ast->rparen_token)
        terminal(ast->rparen_token, ast);

    return false;
}
