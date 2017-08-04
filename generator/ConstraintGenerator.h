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

#ifndef PSYCHE_CONSTRAINTGENERATOR_H__
#define PSYCHE_CONSTRAINTGENERATOR_H__

#include "Config.h"
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
class Observer;

/*!
 * \brief The ConstraintGenerator class
 *
 * Responsible for generating the constraints.
 */
class PSYCHEC_API ConstraintGenerator final : public CPlusPlus::ASTVisitor
{
public:
    ConstraintGenerator(CPlusPlus::TranslationUnit *unit, ConstraintWriter* writer);

    void generate(CPlusPlus::TranslationUnitAST *ast, CPlusPlus::Scope *global);

    void employDomainLattice(const DomainLattice* lattice);

    void installObserver(Observer* observer);

    void addVariadic(const std::string& funcName, size_t varArgPos);

private:
    /*!
     * \brief switchScope
     * \param scope
     * \return
     *
     * Convenience to switch current scope. Works similar to a stack, the
     * caller is responsible to pop the previous scope and push it back later.
     */
    CPlusPlus::Scope *switchScope(CPlusPlus::Scope *scope);

    void visitName(CPlusPlus::NameAST* ast);

    // Declarations
    void visitDeclaration(CPlusPlus::DeclarationAST *ast);
    bool visit(CPlusPlus::SimpleDeclarationAST *ast) override;
    bool visit(CPlusPlus::FunctionDefinitionAST *ast) override;

    // Expressions
    void visitExpression(CPlusPlus::ExpressionAST* ast);
    bool visit(CPlusPlus::ArrayAccessAST* ast) override;
    bool visit(CPlusPlus::BinaryExpressionAST* ast) override;
    bool visit(CPlusPlus::CallAST *ast) override;
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

    // Specifiers
    void visitSpecifier(CPlusPlus::SpecifierAST *ast);
    bool visit(CPlusPlus::EnumSpecifierAST *ast) override;
    bool visit(CPlusPlus::ClassSpecifierAST *ast) override;

    // Statements
    void visitStatement(CPlusPlus::StatementAST *ast);
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

    // Symbol visits.
    void visitSymbol(CPlusPlus::Function* func, CPlusPlus::StatementAST* body);

    //! Scope we're in and the global scope.
    CPlusPlus::Scope *scope_;
    CPlusPlus::Scope *global_;

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
    DomainLattice::Domain domainOf(CPlusPlus::ExpressionAST* ast) const;
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
    static std::string paramPrefix_;
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
    void convertBoolExpression(CPlusPlus::ExpressionAST *ast);

    /*!
     * Encapsulates the steps to enter a new expression rule and generate its
     * corresponding constraints.
     */
    void collectExpression(const std::string& ty, CPlusPlus::ExpressionAST* expr);

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
    std::unordered_map<std::string, size_t> variadic_;

    //! Visitor observer.
    Observer* observer_;
};

} // namespace psyche

#endif
