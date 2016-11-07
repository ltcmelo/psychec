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

#ifndef PSYCHE_SCALARTYPELATTICE_H__
#define PSYCHE_SCALARTYPELATTICE_H__

#include "ASTfwd.h"
#include "ASTVisitor.h"
#include "CPlusPlusForwardDeclarations.h"
#include "CanonicalASTMatcher.h"
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

namespace psyche {

/*!
 * \brief The ScalarTypeLattice class
 */
class ScalarTypeLattice final : public CPlusPlus::ASTVisitor
{
public:
    ScalarTypeLattice(CPlusPlus::TranslationUnit* unit);

    /*!
     * \brief The class structure
     */
    struct Class
    {
        explicit Class(const std::string& name = "undefined")
            : name_(name)
        {}

        Class(const std::string& name, const std::string& tyName)
            : name_(name), arithName_(tyName)
        {}

        bool operator>(const Class& other) const;
        bool operator==(const Class& other) const;
        bool operator!=(const Class& other) const;

        std::string name_;
        std::string arithName_;
    };

    // The classes.
    const static Class Undefined;
    const static Class Scalar;
    const static Class Pointer;
    const static Class Integral;
    const static Class FloatingPoint;
    const static Class Arithmetic; // Integer or floating-point.

    /*!
     * \brief totalize
     *
     * Classify the expression parts as according to the lattice.
     */
    void totalize(CPlusPlus::ExpressionAST*, const CPlusPlus::Scope*);

    /*!
     * \brief createBindings
     */
    void createBindings();

    /*!
     * \brief recover
     * \return
     *
     * Return the class of the given identifier.
     */
    Class recover(const CPlusPlus::Symbol *sym) const;

    /*!
     * \brief recover
     * \param ast
     * \return
     */
    Class recover(CPlusPlus::ExpressionAST* ast) const;

    /*!
     * \brief classOf
     * \param ty
     * \return
     */
    static Class classOf(const CPlusPlus::FullySpecifiedType& ty,
                         const CPlusPlus::Symbol* sym = nullptr);

    // TEMP: Make this a utility.
    std::string fetchText(CPlusPlus::AST* ast) const;

private:
    void classify(CPlusPlus::ExpressionAST* ast);

    void enter(CPlusPlus::ExpressionAST* ast);
    bool visit(CPlusPlus::ArrayAccessAST* ast) override;
    bool visit(CPlusPlus::BinaryExpressionAST* ast) override;
    bool visit(CPlusPlus::ConditionalExpressionAST *ast) override;
    bool visit(CPlusPlus::IdExpressionAST* ast) override;
    bool visit(CPlusPlus::MemberAccessAST* ast) override;
    bool visit(CPlusPlus::NestedExpressionAST* ast) override;
    bool visit(CPlusPlus::UnaryExpressionAST* ast) override;
    bool visit(CPlusPlus::NumericLiteralAST* ast) override;
    bool visit(CPlusPlus::BoolLiteralAST* ast) override;
    bool visit(CPlusPlus::StringLiteralAST* ast) override;
    bool visit(CPlusPlus::PointerLiteralAST* ast) override;
    bool visit(CPlusPlus::CallAST* ast) override;
    bool visit(CPlusPlus::PostIncrDecrAST* ast) override;

    Class switchClass(Class);
    CPlusPlus::ExpressionAST* isKnownAST(CPlusPlus::ExpressionAST*) const;

    using SymbolMap = std::unordered_map<const CPlusPlus::Symbol*, Class>;
    using AstMap = std::unordered_map<CPlusPlus::ExpressionAST*, Class>;

    Class clazz_;
    SymbolMap symbolDB_;
    AstMap astDB_;
    std::vector<CPlusPlus::ExpressionAST*> knownAsts_;
    const CPlusPlus::Scope* scope_;
    mutable CanonicalASTMatcher matcher_;
};

inline std::ostream& operator<<(std::ostream& os, const ScalarTypeLattice::Class& h)
{
    os << h.name_;
    return os;
}

} // namespace psyche

#endif
