/******************************************************************************
 * Copyright (c) 2016 Leandro T. C. Melo (ltcmelo@gmail.com)
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301
 * USA
 *****************************************************************************/

#ifndef PSYCHE_CONSTRAINTGENERATOR_H__
#define PSYCHE_CONSTRAINTGENERATOR_H__

#include "ASTVisitor.h"
#include "FreshVarSupply.h"
#include "FullySpecifiedType.h"
#include "ScalarTypeLattice.h"
#include "TypeNameSpeller.h"
#include <stack>
#include <string>
#include <unordered_map>
#include <vector>

namespace psyche {

class ConstraintStreamWriter;

/*!
 * \brief The ConstraintGenerator class
 *
 * Responsible for generating the constraints.
 */
class ConstraintGenerator final : public CPlusPlus::ASTVisitor
{
public:
    ConstraintGenerator(CPlusPlus::TranslationUnit *unit,
                        ConstraintStreamWriter* writer);

    void generate(CPlusPlus::TranslationUnitAST *ast,
                  CPlusPlus::Scope *global);

    void addPrintfVariety(const std::string& funcName, size_t varArgPos);

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

    // Top-level visitation entry points.
    void visitDeclaration(CPlusPlus::DeclarationAST *ast);
    void visitExpression(CPlusPlus::ExpressionAST* ast);
    void visitName(CPlusPlus::NameAST* ast);
    void visitSpecifier(CPlusPlus::SpecifierAST *ast);
    void visitStatement(CPlusPlus::StatementAST *ast);

    // Declarations
    bool visit(CPlusPlus::SimpleDeclarationAST *ast) override;
    bool visit(CPlusPlus::FunctionDefinitionAST *ast) override;

    // Expressions
    bool visit(CPlusPlus::ArrayAccessAST* ast) override;
    bool visit(CPlusPlus::BinaryExpressionAST* ast) override;
    bool visit(CPlusPlus::CallAST *ast) override;
    bool visit(CPlusPlus::CastExpressionAST *ast) override;
    bool visit(CPlusPlus::ConditionalExpressionAST *ast) override;
    bool visit(CPlusPlus::IdExpressionAST* ast) override;
    bool visit(CPlusPlus::MemberAccessAST* ast) override;
    bool visit(CPlusPlus::NumericLiteralAST* ast) override;
    bool visit(CPlusPlus::BoolLiteralAST* ast) override;
    bool visit(CPlusPlus::StringLiteralAST* ast) override;
    bool visit(CPlusPlus::UnaryExpressionAST* ast) override;
    bool visit(CPlusPlus::SizeofExpressionAST* ast) override;
    bool visit(CPlusPlus::PointerLiteralAST* ast) override;
    bool visit(CPlusPlus::BracedInitializerAST *ast) override;
    bool visit(CPlusPlus::PostIncrDecrAST* ast) override;

    // Specifiers
    bool visit(CPlusPlus::EnumSpecifierAST *ast) override;
    bool visit(CPlusPlus::ClassSpecifierAST *ast) override;

    // Statements
    bool visit(CPlusPlus::CaseStatementAST* ast) override;
    bool visit(CPlusPlus::CompoundStatementAST *ast) override;
    bool visit(CPlusPlus::DeclarationStatementAST *ast) override;
    bool visit(CPlusPlus::DoStatementAST *ast) override;
    bool visit(CPlusPlus::ForStatementAST *ast) override;
    bool visit(CPlusPlus::ExpressionStatementAST *ast) override;
    bool visit(CPlusPlus::IfStatementAST *ast) override;
    bool visit(CPlusPlus::ReturnStatementAST *ast) override;
    bool visit(CPlusPlus::SwitchStatementAST *ast) override;
    bool visit(CPlusPlus::WhileStatementAST *ast) override;

    //!@{
    /*!
     * The constraints rules requires that statements are `and`ed one to
     * another. But through the visitor we can't see ahead: either 1) we look
     * behind if there's been a valid statement or 2) we mark whenever a valid
     * statement has been visited. The approach originally implemented was
     * option 1, but it can get tricky. Current implementaiton is option 2.
     */
    void maybeFollowStmt();
    bool seenStmt_;
    //!@}

    //! Scope we're in and the global scope.
    CPlusPlus::Scope *scope_;
    CPlusPlus::Scope *global_;

    //! Writer we use to generate the constraints.
    ConstraintStreamWriter *writer_;

    //! Type name speller.
    TypeNameSpeller typeSpeller_;

    //! Type variables supply and properties.
    FreshVarSupply supply_;

    //! Pointer x integers lattice.
    ScalarTypeLattice lattice_;

    //! Inside static initialization area.
    bool staticDecl_;

    /*!
     * The generator performs two passes over the input, the first one is a
     * sort of preprocessing step (not to be confused with the compiler's
     * macro preprocessor) in which the scalar lattice is constructed.
     */
    bool preprocess_;

    //!@{
    /*!
     * Pointer x scalar lattice related processing.
     */
    void applyTypeLattice(const ScalarTypeLattice::Class &lhsClass,
                          const ScalarTypeLattice::Class &rhsClass,
                          const std::string &lhsAlpha,
                          const std::string &rhsAlpha,
                          int opTk);
    ScalarTypeLattice::Class classOfExpr(CPlusPlus::ExpressionAST* ast) const;
    //!@}

    /*!
     * Overall initialization.
     */
    void prepareForRun();

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
     * Prefix and counter for unamed names.
     */
    int unnamedCount_;
    std::string createUnnamed();
    static std::string unnamedParamPrefix_;
    //!@}

    /*!
     * There's a little "operational inconvenience" when generating constraints
     * for a declaration: the program's remaining statements come in the middle,
     * instead of in the end of the rule. Then we must accumulate all type
     * equivalences from the declarations and write then down later.
     */
    using EquivPair = std::pair<std::string, std::string>;
    std::stack<EquivPair> pendingEquivs_;

    /*!
     * Convenience function to process a program symbol we're interested on,
     * whever we need to write a type equivalence for it. This will return
     * the symbol name as a string.
     */
    std::string processSymbol(const std::string& name);

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

    /*!
     * Functions with "printf style" we understand.
     */
    std::unordered_map<std::string, size_t> printfs_;
};

} // namespace psyche

#endif
