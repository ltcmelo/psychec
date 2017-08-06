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

#include "Config.h"
#include "ASTfwd.h"
#include "ASTVisitor.h"
#include "ASTIdentityMatcher.h"
#include "CPlusPlusForwardDeclarations.h"
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

namespace psyche {

/*!
 * \brief The DomainLattice class
 */
class PSYCHEC_API DomainLattice final : public CPlusPlus::ASTVisitor
{
public:
    DomainLattice(CPlusPlus::TranslationUnit* unit);

    void categorize(CPlusPlus::TranslationUnitAST* ast, CPlusPlus::Scope* global);

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
    Domain retrieveDomain(const CPlusPlus::Symbol* sym, const CPlusPlus::Scope*) const;
    Domain retrieveDomain(const CPlusPlus::ExpressionAST* ast, const CPlusPlus::Scope*) const;

    /*!
     * \brief typeDomain
     * \param ty
     * \return
     *
     * Return the domain of a type.
     */
    static Domain domainForType(const CPlusPlus::FullySpecifiedType& ty,
                                const CPlusPlus::Symbol* sym = nullptr);

    // TEMP: Make this a utility.
    std::string fetchText(CPlusPlus::AST* ast) const;

private:
    // Declarations
    bool visit(CPlusPlus::SimpleDeclarationAST* ast) override;
    bool visit(CPlusPlus::FunctionDefinitionAST* ast) override;

    // Statements
    void visitStatement(CPlusPlus::StatementAST* ast);
    bool visit(CPlusPlus::SwitchStatementAST *ast) override;
    bool visit(CPlusPlus::CaseStatementAST* ast) override;
    bool visit(CPlusPlus::CompoundStatementAST *ast) override;
    bool visit(CPlusPlus::DeclarationStatementAST *ast) override;
    bool visit(CPlusPlus::DoStatementAST *ast) override;
    bool visit(CPlusPlus::WhileStatementAST *ast) override;
    bool visit(CPlusPlus::ForStatementAST *ast) override;
    bool visit(CPlusPlus::IfStatementAST *ast) override;
    bool visit(CPlusPlus::ExpressionStatementAST *ast) override;
    bool visit(CPlusPlus::ReturnStatementAST *ast) override;

    // Expressions
    void visitExpression(CPlusPlus::ExpressionAST* ast);
    bool visit(CPlusPlus::ArrayAccessAST* ast) override;
    bool visit(CPlusPlus::BinaryExpressionAST* ast) override;
    bool visit(CPlusPlus::CallAST* ast) override;
    bool visit(CPlusPlus::CastExpressionAST *ast) override;
    bool visit(CPlusPlus::ConditionalExpressionAST *ast) override;
    bool visit(CPlusPlus::IdExpressionAST* ast) override;
    bool visit(CPlusPlus::MemberAccessAST* ast) override;
    bool visit(CPlusPlus::UnaryExpressionAST* ast) override;
    bool visit(CPlusPlus::NumericLiteralAST* ast) override;
    bool visit(CPlusPlus::BoolLiteralAST* ast) override;
    bool visit(CPlusPlus::StringLiteralAST* ast) override;
    bool visit(CPlusPlus::SizeofExpressionAST* ast) override;
    bool visit(CPlusPlus::PointerLiteralAST* ast) override;
    bool visit(CPlusPlus::BracedInitializerAST *ast) override;
    bool visit(CPlusPlus::PostIncrDecrAST* ast) override;
    bool visit(CPlusPlus::NestedExpressionAST* ast) override;

    Domain enforceLeastDomain(Domain);
    void assignDomain(CPlusPlus::ExpressionAST* ast);
    Domain lastDom_;

    std::vector<CPlusPlus::ExpressionAST*> knownAsts_;
    CPlusPlus::ExpressionAST* isKnownAST(const CPlusPlus::ExpressionAST*) const;
    mutable ASTIdentityMatcher matcher_;

    using SymbolMap = std::unordered_map<const CPlusPlus::Symbol*, Domain>;
    using AstMap = std::unordered_map<const CPlusPlus::ExpressionAST*, Domain>;
    using DB = std::pair<AstMap, SymbolMap>;
    std::unordered_map<const CPlusPlus::Scope*, DB> index_;

    template <class T, class MapT>
    Domain retrieveDomainCore(const T* v, const MapT DB::*, const CPlusPlus::Scope*) const;

    const Scope *enterScope(const CPlusPlus::Scope* scope);
    const CPlusPlus::Scope* scope_;

    void resetCutoffScope();
    const CPlusPlus::Scope* globalScope_;
    const CPlusPlus::Scope* cutoffScope_;

    const DB* searchDB(const CPlusPlus::Scope* scope) const;
    DB* findOrCreateDB(const CPlusPlus::Scope* scope);

    // Function argument domains, taken from the call with highest rank.
    struct ArgumentData
    {
        Domain dom_;
        std::vector<CPlusPlus::ExpressionAST*> instances_;
        const CPlusPlus::Scope* callScope_; // TODO: This information is for call, not for arg.
    };

    std::unordered_map<CPlusPlus::ExpressionAST*, std::vector<ArgumentData>> funcs_;
};

inline std::ostream& operator<<(std::ostream& os, const DomainLattice::Domain& h)
{
    os << h.name_;
    return os;
}

} // namespace psyche

#endif
