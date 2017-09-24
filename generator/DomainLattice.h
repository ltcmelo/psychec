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

#ifndef PSYCHE_DOMAIN_LATTICE_H__
#define PSYCHE_DOMAIN_LATTICE_H__

#include "ASTFwds.h"
#include "ASTVisitor.h"
#include "ASTIdentityMatcher.h"
#include "FrontendConfig.h"
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

namespace psyche {

/*!
 * \brief The DomainLattice class
 */
class DomainLattice final : public psyche::ASTVisitor
{
public:
    DomainLattice(psyche::TranslationUnit* unit);

    void categorize(psyche::TranslationUnitAST* ast, psyche::Scope* global);

    /*!
     * \brief The domain
     */
    struct Domain
    {
        explicit Domain(const std::string& name = "undefined")
            : name_(name)
        {}

        Domain(const std::string& name, const std::string& tyName)
            : name_(name), ty_(tyName)
        {}

        bool operator>(const Domain& other) const;
        bool operator>=(const Domain& other) const;
        bool operator==(const Domain& other) const;
        bool operator!=(const Domain& other) const;

        std::string name_;
        std::string ty_;
    };

    // The domains.
    const static Domain Undefined;
    const static Domain Scalar;
    const static Domain Pointer;
    const static Domain Integral;
    const static Domain FloatingPoint;
    const static Domain Arithmetic; // Integral or floating-point.

    /*!
     * \brief retrieveDomain
     * \return
     *
     * Return the domain of the given AST/symbol.
     */
    Domain retrieveDomain(const psyche::Symbol* sym, const psyche::Scope*) const;
    Domain retrieveDomain(const psyche::ExpressionAST* ast, const psyche::Scope*) const;

    /*!
     * \brief typeDomain
     * \param ty
     * \return
     *
     * Return the domain of a type.
     */
    static Domain domainForType(const psyche::FullySpecifiedType& ty,
                                const psyche::Symbol* sym = nullptr);

    // TEMP: Make this a utility.
    std::string fetchText(psyche::AST* ast) const;

private:
    // Declarations
    bool visit(psyche::SimpleDeclarationAST* ast) override;
    bool visit(psyche::FunctionDefinitionAST* ast) override;

    // Statements
    void visitStatement(psyche::StatementAST* ast);
    bool visit(psyche::SwitchStatementAST *ast) override;
    bool visit(psyche::CaseStatementAST* ast) override;
    bool visit(psyche::CompoundStatementAST *ast) override;
    bool visit(psyche::DeclarationStatementAST *ast) override;
    bool visit(psyche::DoStatementAST *ast) override;
    bool visit(psyche::WhileStatementAST *ast) override;
    bool visit(psyche::ForStatementAST *ast) override;
    bool visit(psyche::IfStatementAST *ast) override;
    bool visit(psyche::ExpressionStatementAST *ast) override;
    bool visit(psyche::ReturnStatementAST *ast) override;

    // Expressions
    void visitExpression(psyche::ExpressionAST* ast);
    bool visit(psyche::ArrayAccessAST* ast) override;
    bool visit(psyche::BinaryExpressionAST* ast) override;
    bool visit(psyche::CallAST* ast) override;
    bool visit(psyche::CastExpressionAST *ast) override;
    bool visit(psyche::ConditionalExpressionAST *ast) override;
    bool visit(psyche::IdExpressionAST* ast) override;
    bool visit(psyche::MemberAccessAST* ast) override;
    bool visit(psyche::UnaryExpressionAST* ast) override;
    bool visit(psyche::NumericLiteralAST* ast) override;
    bool visit(psyche::BoolLiteralAST* ast) override;
    bool visit(psyche::StringLiteralAST* ast) override;
    bool visit(psyche::SizeofExpressionAST* ast) override;
    bool visit(psyche::PointerLiteralAST* ast) override;
    bool visit(psyche::BracedInitializerAST *ast) override;
    bool visit(psyche::PostIncrDecrAST* ast) override;
    bool visit(psyche::NestedExpressionAST* ast) override;

    unsigned int withinExpr_;
    struct ExpressionRegion
    {
        ExpressionRegion(unsigned int& cnt) : cnt_(cnt) { ++cnt_; }
        ~ExpressionRegion() { --cnt_; }
        unsigned int& cnt_;
    };

    Domain enforceLeastDomain(Domain);
    void assignDomain(psyche::ExpressionAST* ast);
    Domain lastDom_;

    std::vector<psyche::ExpressionAST*> knownAsts_;
    psyche::ExpressionAST* isKnownAST(const psyche::ExpressionAST*) const;
    mutable ASTIdentityMatcher matcher_;

    using SymbolMap = std::unordered_map<const psyche::Symbol*, Domain>;
    using AstMap = std::unordered_map<const psyche::ExpressionAST*, Domain>;
    using DB = std::pair<AstMap, SymbolMap>;
    std::unordered_map<const psyche::Scope*, DB> index_;

    template <class T, class MapT>
    Domain retrieveDomainCore(const T* v, const MapT DB::*, const psyche::Scope*) const;

    const Scope *enterScope(const psyche::Scope* scope);
    const psyche::Scope* scope_;

    void resetCutoffScope();
    const psyche::Scope* globalScope_;
    const psyche::Scope* cutoffScope_;

    const DB* searchDB(const psyche::Scope* scope) const;
    DB* findOrCreateDB(const psyche::Scope* scope);

    // Function argument domains, taken from the call with highest rank.
    struct ArgumentData
    {
        Domain dom_;
        std::vector<psyche::ExpressionAST*> instances_;
        const psyche::Scope* callScope_; // TODO: This information is for call, not for arg.
    };

    std::unordered_map<psyche::ExpressionAST*, std::vector<ArgumentData>> funcs_;
};

inline std::ostream& operator<<(std::ostream& os, const DomainLattice::Domain& h)
{
    os << h.name_;
    return os;
}

} // namespace psyche

#endif
