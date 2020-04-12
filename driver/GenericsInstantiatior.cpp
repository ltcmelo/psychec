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
    : ASTVisitor(unit)
    , typeof_(unit)
    , resolver_(unit)
    , scope_(nullptr)
{}

bool GenericsInstantiatior::quantify(AST* ast, Scope* scope)
{
    switchScope(scope);
    accept(ast);

    if (genericFuncsTbl_.empty())
        return false;

    auto unusedTyNameCnt = 0;
    const auto funcTbl = genericFuncsTbl_;
    for (const auto& variation : funcTbl) {
        if (!variation.second.overloads_.empty())
            continue;

        Function* func = variation.first;

        std::vector<FullySpecifiedType> argsTypes;
        for (unsigned i = 0; i < func->argumentCount(); ++i) {
            auto tyName = "__unused_" + std::to_string(unusedTyNameCnt++);
            NamedType* namedTy = control()->namedType(
                control()->identifier(tyName.c_str(), tyName.size()));
            argsTypes.emplace_back(namedTy);
        }

        recognize(func, argsTypes);
    }

    return true;
}

std::string GenericsInstantiatior::instantiate(const std::string& source) const
{
    std::string newSource = source;

    for (const auto& variation : genericFuncsTbl_) {
        GenericsDeclarationAST* ast = variation.second.ast_;

        const std::string& funcText = translationUnit()->fetchSource(ast);
        for (const auto& data : variation.second.overloads_) {
            std::string newFuncText = funcText;
            for (const auto& sub : data.second.subs_)
                newFuncText = apply(sub, newFuncText);
            newSource += newFuncText + "\n";
        }
    }

    newSource = applyOnce(subs_, newSource);

    return newSource;
}

int GenericsInstantiatior::recognize(Function* func,
                                     const std::vector<FullySpecifiedType>& argsTypes)
{
    std::string key;
    std::for_each(argsTypes.begin(), argsTypes.end(),
                  [&] (const auto& s) { key += typePP_.print(s, scope_) + ','; });

    auto it = genericFuncsTbl_.find(func);
    PSYCHE_ASSERT(it != genericFuncsTbl_.end(), return -1, "expected known function");

    auto it2 = it->second.overloads_.find(key);
    if (it2 == it->second.overloads_.end()) {
        auto cnt = it->second.overloads_.size();
        it->second.overloads_[key].label_ = cnt;

        std::string funcName(func->name()->identifier()->chars());
        it->second.overloads_[key].subs_.emplace_back(funcName, funcName + std::to_string(cnt));
        it->second.overloads_[key].subs_.emplace_back("_Template", "");

        int cntU = func->argumentCount();
        int cntE = argsTypes.size();
        PSYCHE_ASSERT(cntU == cntE, return -1, "expected matching argument count");

        for (auto i = 0L; i < func->argumentCount(); ++i) {
            if (!func->argumentAt(i)->asArgument()->type().coreType()->asQuantifiedType())
                continue;

            auto tyU = typePP_.print(func->argumentAt(i)->asArgument()->type().coreType(), scope_);
            it->second.overloads_[key].subs_.emplace_back("_Forall(" + tyU + ")", tyU);

            auto tyE = typePP_.print(argsTypes[i].coreType(), scope_);
            it->second.overloads_[key].subs_.emplace_back(tyU, tyE);
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

bool GenericsInstantiatior::visit(GenericsDeclarationAST* ast)
{
    auto funcText = translationUnit()->fetchSource(ast);
    subs_.emplace_back(funcText, "");

    genericsCtx_.push(ast);
    accept(ast->declaration);
    genericsCtx_.pop();

    return false;
}

bool GenericsInstantiatior::visit(FunctionDefinitionAST* ast)
{
    Function* func = ast->symbol;
    if (!func)
        return false;

    for (SpecifierListAST* it = ast->decl_specifier_list; it; it = it->next)
        accept(it->value);
    accept(ast->declarator);

    Scope* prevScope = switchScope(func->asScope());
    accept(ast->function_body);
    switchScope(prevScope);

    if (func->isGeneric() && ast->function_body) {
        if (genericFuncsTbl_.find(func) != genericFuncsTbl_.end())
            translationUnit()->error(ast->firstToken(), "redefinition of generic function");
        FunctionInstantions f(genericsCtx_.top());
        genericFuncsTbl_[func] = f;
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
        if (it->value->asQuantifiedTypeSpecifier()) {
            QuantifiedTypeSpecifierAST* specifier = it->value->asQuantifiedTypeSpecifier();
            subs_.emplace_back(translationUnit()->fetchSource(specifier),
                               typePP_.print(coreTy, scope_));
        }
    }

    return false;
}

bool GenericsInstantiatior::visit(QuantifiedTypeSpecifierAST* ast)
{
    accept(ast->name);

    return false;
}

bool GenericsInstantiatior::visit(CompoundStatementAST* ast)
{
    Scope* prevScope = switchScope(ast->symbol);
    for (StatementListAST *it = ast->statement_list; it; it = it->next)
        accept(it->value);
    switchScope(prevScope);

    return false;
}

bool GenericsInstantiatior::visit(CallAST* ast)
{
    Symbol* sym = resolver_.resolve(ast->base_expression, scope_);
    if (!sym || !sym->asFunction() || !sym->asFunction()->isGeneric())
        return true;

    Function* func = sym->asFunction();
    if (genericFuncsTbl_.find(func) == genericFuncsTbl_.end())
        return true;

    std::vector<FullySpecifiedType> argsTypes;
    for (ExpressionListAST* it = ast->expression_list; it; it = it->next) {
        FullySpecifiedType ty = typeof_.evaluate(it->value, scope_);
        argsTypes.push_back(ty);
    }

    int cnt = recognize(sym->asFunction(), argsTypes);

    std::string funcName(func->name()->identifier()->chars());
    subs_.emplace_back(translationUnit()->fetchSource(ast->base_expression),
                       funcName + std::to_string(cnt));

    return true;
}
