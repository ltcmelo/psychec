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

#ifndef PSYCHE_GENERICS_INSTANTIATIOR_H__
#define PSYCHE_GENERICS_INSTANTIATIOR_H__

#include "ASTVisitor.h"
#include "ExpressionResolver.h"
#include "ExpressionTypeEvaluator.h"
#include "Substitution.h"
#include "TypePP.h"
#include <stack>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

namespace psyche {

class GenericsInstantiatior final : public ASTVisitor
{
public:
    GenericsInstantiatior(TranslationUnit* unit);

    bool quantify(AST* ast, Scope* scope);
    std::string instantiate(const std::string& source) const;

private:
    using Base = ASTVisitor;

    ExpressionTypeEvaluator typeof_;
    ExpressionResolver resolver_;
    TypePP<CSyntax> typePP_;

    Scope* switchScope(Scope* scope);
    Scope* scope_;

    int recognize(Function* func, const std::vector<FullySpecifiedType>& argsTypes);

    std::vector<Substitution<std::string>> subs_;

    struct FunctionInstantions
    {
        FunctionInstantions(GenericsDeclarationAST* ast = nullptr)
            : ast_(ast)
        {}

        GenericsDeclarationAST* ast_;

        struct InstanceData
        {
            int label_;
            std::vector<Substitution<std::string>> subs_;
        };
        std::unordered_map<std::string, InstanceData> overloads_;
    };
    std::unordered_map<Function*, FunctionInstantions> funcTbl_;

    std::stack<GenericsDeclarationAST*> genericsCtx_;

    // Declarations
    bool visit(GenericsDeclarationAST* ast) override;
    bool visit(FunctionDefinitionAST* ast) override;
    bool visit(SimpleDeclarationAST* ast) override;

    // Specifiers
    bool visit(QuantifiedTypeSpecifierAST* ast) override;

    // Statements
    bool visit(CompoundStatementAST* ast) override;

    // Expressions
    bool visit(CallAST* ast) override;
};

} // namespace psyche

#endif
