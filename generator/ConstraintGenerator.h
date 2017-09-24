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

#ifndef PSYCHE_CONSTRAINT_GENERATOR_H__
#define PSYCHE_CONSTRAINT_GENERATOR_H__

#include "ASTVisitor.h"
#include "FreshVarSupply.h"
#include "FullySpecifiedType.h"
#include "ConstraintSyntax.h"
#include "DomainLattice.h"
#include "TypeSpeller.h"
#include <stack>
#include <string>
#include <unordered_map>
#include <vector>

namespace psyche {

class ConstraintWriter;
class DeclarationInterceptor;
class VisitorObserver;

/*!
 * \brief The ConstraintGenerator class
 *
 * Responsible for generating the constraints.
 */
class ConstraintGenerator final : public psyche::ASTVisitor
{
public:
    ConstraintGenerator(psyche::TranslationUnit *unit, ConstraintWriter* writer);

    void generate(psyche::TranslationUnitAST *ast, psyche::Scope *global);

    void employDomainLattice(const DomainLattice* lattice);

    void installObserver(VisitorObserver* observer);

    void installInterceptor(DeclarationInterceptor* interceptor);

    /*!
     * \brief addPrintfLike
     * \param funcName
     * \param varArgPos
     *
     * Add a printf-like function, one that accepts a format-specifier argument that can be used
     * to type other arguments.
     *
     * Variadic functions in general don't need to be added through this method, they are detected
     * automatically by our unificaiton algorithm.
     */
    void addPrintfLike(const std::string& funcName, size_t varArgPos);

private:
    /*!
     * \brief switchScope
     * \param scope
     * \return
     *
     * Convenience to switch current scope. Works similar to a stack, the
     * caller is responsible to pop the previous scope and push it back later.
     */
    psyche::Scope *switchScope(psyche::Scope *scope);

    void visitName(psyche::NameAST* ast);

    // Declarations
    void visitDeclaration(psyche::DeclarationAST *ast);
    bool visit(psyche::SimpleDeclarationAST *ast) override;
    bool visit(psyche::FunctionDefinitionAST *ast) override;

    // Expressions
    void visitExpression(psyche::ExpressionAST* ast);
    bool visit(psyche::ArrayAccessAST* ast) override;
    bool visit(psyche::BinaryExpressionAST* ast) override;
    bool visit(psyche::CallAST *ast) override;
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

    // Specifiers
    void visitSpecifier(psyche::SpecifierAST *ast);
    bool visit(psyche::EnumSpecifierAST *ast) override;
    bool visit(psyche::ClassSpecifierAST *ast) override;
    bool visit(psyche::GnuAttributeSpecifierAST *ast) override;

    // Statements
    void visitStatement(psyche::StatementAST *ast);
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

    // Miscellanea
    bool visit(psyche::EnumeratorAST *ast) override;

    // Symbol visits.
    void visitSymbol(psyche::Function* func, psyche::StatementAST* body);

    //! Scope we're in and the global scope.
    psyche::Scope *scope_;
    psyche::Scope *global_;

    //! Writer we use to generate the constraints.
    ConstraintWriter *writer_;

    //! Type name speller.
    TypeSpeller<ConstraintSyntax> typeSpeller_;

    //! Type variables supply and properties.
    FreshVarSupply supply_;

    //! Domain lattice for "pre inference".
    const DomainLattice* lattice_;

    //! Inside static initialization.
    bool staticDecl_;

    //!@{
    /*!
     * Domain lattice analysis.
     */
    void employLattice(const DomainLattice::Domain &lhsDom, const DomainLattice::Domain &rhsDom,
                       const std::string &lhsTy, const std::string &rhsTy,
                       int op);
    DomainLattice::Domain domainOf(psyche::ExpressionAST* ast) const;
    //!@}

    //!@{
    /*!
     * Type currently being considered for a given rule. It's responsability
     * of the caller rule to push the given type onto the stack so it can
     * be retrieved by the callee rule. The caller rule must pop it afterwards.
     *
     * \sa pushType, popType
     */
    std::stack<std::string> types_;
    void pushType(const std::string& ty);
    std::string popType();
    //!@}

    /*!
     * Whether the function currently being traversed has any valued returns.
     */
    std::stack<bool> valuedRets_;

    //!@{
    /*!
     * Prefix and counter for anonymous stuff.
     */
    int unnamedCount_;
    std::string createUnnamed(const std::string &prefix);
    static std::string declPrefix_;
    static std::string stubPrefix_;
    //!@}

    /*!
     * Assign the name to type on top of the stack.
     */
    void assignTop(const std::string& name);

    // Helpers
    void castExpressionHelper(const std::string &inputTy, std::string& resultTy);

    /*!
     * Convert boolean expressions to int.
     */
    void treatAsBool(psyche::ExpressionAST *ast);

    /*!
     * Encapsulates the steps to enter a new expression rule and generate its
     * corresponding constraints.
     */
    void collectExpression(const std::string& ty, psyche::ExpressionAST* expr);

    /*!
     * \brief ensureTypeIsKnown
     * \return
     *
     * Typedef a new type and return the associated type variable. This function
     * should be called to make sure a given type is known to the solver. In
     * particular, for the return type of a function, for a cast expression, and
     * for a declaration.
     */
    std::string ensureTypeIsKnown(std::string& tyName);

    //!@{
    /*!
     * Undeclared functions require special treatment because they can still
     * be defined later in the code. In such case, the function declaration
     * we infer must match the function definition present. This is not a
     * problem for the parameters but because return values may be discarded
     * (i.e. expression statement) we keep track of the ones we know.
     */
    std::unordered_map<std::string, std::string> knownFuncNames_;
    std::unordered_map<std::string, std::vector<std::string>> knownFuncRets_;
    //!@}

    std::stack<std::string> structs_;
    std::vector<std::string> field_;

    /*!
     * Variadic functions in printf-style in which a format-specifier is used to type
     * arguments.
     */
    std::unordered_map<std::string, size_t> printfs_;

    //! Generation's observer.
    VisitorObserver* observer_;

    //! Generation's interceptor.
    DeclarationInterceptor* interceptor_;
};

} // namespace psyche

#endif
