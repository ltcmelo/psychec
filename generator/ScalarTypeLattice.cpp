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

#include "ScalarTypeLattice.h"
#include "AST.h"
#include "Assert.h"
#include "BuiltinNames.h"
#include "Control.h"
#include "CoreTypes.h"
#include "Debug.h"
#include "Literals.h"
#include "Lookup.h"
#include "Name.h"
#include "Symbol.h"
#include "Symbols.h"
#include "SymbolPP.h"
#include "Token.h"
#include "TypeOfExpr.h"
#include "TranslationUnit.h"
#include "Utils.h"
#include <algorithm>
#include <unordered_set>
#include <utility>

using namespace CPlusPlus;
using namespace psyche;

#define VISITOR_NAME "TypeLattice"

namespace {

const char* const kUndefinedClassName = "undefined";
const char* const kScalarClassName = "scalar";
const char* const kPointerClassName = "pointer";
const char* const kIntegralClassName = "integral";
const char* const kFloatPointClassName = "floating-point";
const char* const kArithmeticClassName = "arithmetic";

const char* const kZero = "0";

} // anonymous

const ScalarTypeLattice::Class ScalarTypeLattice::Undefined(kUndefinedClassName);
const ScalarTypeLattice::Class ScalarTypeLattice::Scalar(kScalarClassName);
const ScalarTypeLattice::Class ScalarTypeLattice::Pointer(kPointerClassName);
const ScalarTypeLattice::Class ScalarTypeLattice::Integral(kIntegralClassName);
const ScalarTypeLattice::Class ScalarTypeLattice::FloatingPoint(kFloatPointClassName);
const ScalarTypeLattice::Class ScalarTypeLattice::Arithmetic(kArithmeticClassName);

bool ScalarTypeLattice::Class::operator>(const ScalarTypeLattice::Class& other) const
{
    if (other == Undefined) {
        return name_ == kScalarClassName
                || name_ == kPointerClassName
                || name_ == kIntegralClassName
                || name_ == kFloatPointClassName
                || name_ == kArithmeticClassName;
    }

    if (other == Scalar) {
        return name_ == kPointerClassName
                || name_ == kIntegralClassName
                || name_ == kFloatPointClassName
                || name_ == kArithmeticClassName;
    }

    if (other == Arithmetic) {
        return name_ == kIntegralClassName
                || name_ == kFloatPointClassName;
    }

    return false;
}

bool ScalarTypeLattice::Class::operator==(const ScalarTypeLattice::Class& other) const
{
    return name_ == other.name_;
}

bool ScalarTypeLattice::Class::operator!=(const ScalarTypeLattice::Class& other) const
{
    return !(this == &other);
}

ScalarTypeLattice::ScalarTypeLattice(TranslationUnit *unit)
    : ASTVisitor(unit)
    , clazz_(Undefined)
    , matcher_(unit)
{}

void ScalarTypeLattice::totalize(ExpressionAST* ast, const Scope *scope)
{
    scope_ = scope;
    switchClass(Undefined);
    enter(ast);
}

ScalarTypeLattice::Class ScalarTypeLattice::recover(const Symbol* sym) const
{
    auto it = symbolDB_.find(const_cast<Symbol*>(sym));
    if (it != symbolDB_.end())
        return it->second;
    return Undefined;
}

ScalarTypeLattice::Class ScalarTypeLattice::recover(ExpressionAST *ast) const
{
    auto equivalent = isKnownAST(ast);
    if (!equivalent)
        return Undefined;

    PSYCHE_ASSERT(astDB_.find(equivalent) != astDB_.end(),
                  return Undefined,
                  "expected equivalent ast in index");
    return astDB_.find(equivalent)->second;
}

ScalarTypeLattice::Class ScalarTypeLattice::classOf(const FullySpecifiedType &ty,
                                                    const Symbol *sym)
{
    auto debug = [sym] (const char* m) {
        if (sym) {
            const auto& s = SymbolPP().print(sym);
            printDebug(m, s.begin());
        }
    };

    if (ty->asIntegerType()) {
        if (ty->asIntegerType()->kind() == IntegerType::Int) {
            debug("Symbol %s lookedup as integer\n");
            return Class(kIntegralClassName, kIntTy);
        }

        if (ty->asIntegerType()->kind() == IntegerType::Bool) {
            debug("Symbol %s lookedup as boolean (taken as integer)\n");
            return Class(kIntegralClassName, kIntTy);
        }

        if (ty->asIntegerType()->kind() == IntegerType::Char) {
            debug("Symbol %s looked as char\n");
            return Class(kIntegralClassName, kCharTy);
        }

        if (ty->asIntegerType()->kind() == IntegerType::Short) {
            debug("Symbol %s looked as short\n");
            return Class(kIntegralClassName, kShortTy);
        }

        if (ty->asIntegerType()->kind() == IntegerType::Long) {
            debug("Symbol %s looked as long\n");
            return Class(kIntegralClassName, kLongTy);
        }

        debug("Symbol %s looked as unknown integral\n");
        return Class(kIntegralClassName, kIntTy);
    }

    if (ty->asFloatType()) {
        if (ty->asFloatType()->kind() == FloatType::Float) {
            debug("Symbol %s lookedup as float\n");
            return Class(kFloatPointClassName, kFloatTy);
        }

        debug("Symbol %s looked as double\n");
        return Class(kFloatPointClassName, kDoubleTy);
    }

    if (ty->asPointerType() || ty->asArrayType()) {
        debug("Symbol %s lookedup as pointer:\n");
        // TODO: We can improve this by looking at the underlying element type
        // and using the "exact name" member of the class to hold this info.
        return Pointer;
    }

    return Undefined;
}

void ScalarTypeLattice::classify(ExpressionAST *ast)
{
    const auto& astText = fetchText(ast);

    bool checkTy = true;
    if (ast->asNumericLiteral()) {
        const NumericLiteral *numLit =
                numericLiteral(ast->asNumericLiteral()->literal_token);
        PSYCHE_ASSERT(numLit, return, "numeric literal must exist");
        checkTy = !numLit->isInt() || strcmp(numLit->chars(), kZero);
    }

    // Whenever we have typing information, we must use it.
    if (checkTy) {
        TypeOfExpr typeofExpr(translationUnit());
        FullySpecifiedType ty = typeofExpr.resolve(ast, const_cast<Scope*>(scope_));
        if (ty.isValid()) {
            // This AST can be of any form, but it it's an identifier-expression
            // we recover the symbol so we can associated the class with it too.
            const Symbol* valSym = nullptr;
            if (ast->asIdExpression())
                valSym = lookupValueSymbol(ast->asIdExpression()->name->name, scope_);

            auto clazz = classOf(ty, valSym);
            if (clazz > clazz_) {
                switchClass(clazz);
                printDebug("Upgrade class to %s, based type of AST <%s>\n",
                           clazz_.name_.c_str(), astText.c_str());
            }

            // The "extra" association of classes with symbols is needed only
            // in the case of declarations with initializers.
            if (valSym) {
                const auto& symName = SymbolPP().print(valSym);
                auto it = symbolDB_.find(valSym);
                if (it == symbolDB_.end()) {
                    symbolDB_.insert(std::make_pair(valSym, clazz));
                    printDebug("Symbol %s classified as %s in AST <%s>\n",
                               symName.c_str(), clazz_.name_.c_str(), astText.c_str());
                } else {
                    it->second = clazz;
                    printDebug("Symbol %s classified as %s in AST <%s>\n",
                               symName.c_str(), clazz_.name_.c_str(), astText.c_str());
                }
            }
        }
    }

    if (auto equivalent = isKnownAST(ast)) {
        PSYCHE_ASSERT(astDB_.find(equivalent) != astDB_.end(),
                      return,
                      "expected equivalent ast in index");

        if (astDB_[equivalent] > clazz_) {
            switchClass(astDB_[equivalent]);
            printDebug("Upgrade class to %s, from equivalent of AST <%s>\n",
                       clazz_.name_.c_str(), astText.c_str());
            return;
        }
        if (clazz_ > astDB_[equivalent]) {
            astDB_[equivalent] = clazz_;
            printDebug("AST <%s> re-classified as %s\n",
                       astText.c_str(), clazz_.name_.c_str());
            return;
        }
    }

    knownAsts_.push_back(ast);
    astDB_[ast] = clazz_;
    printDebug("AST <%s> classified as %s\n",
               astText.c_str(), clazz_.name_.c_str());
}

ExpressionAST *ScalarTypeLattice::isKnownAST(ExpressionAST *ast) const
{
    for (auto candidate : knownAsts_) {
        if (ast->match(candidate, &matcher_))
            return candidate;
    }
    return nullptr;
}

ScalarTypeLattice::Class ScalarTypeLattice::switchClass(ScalarTypeLattice::Class clazz)
{
    std::swap(clazz_, clazz);
    return clazz;
}

void ScalarTypeLattice::enter(ExpressionAST *ast)
{
    accept(ast);
    classify(ast);
}

bool ScalarTypeLattice::visit(ArrayAccessAST *ast)
{
    Class prevClass = switchClass(Pointer);
    enter(ast->base_expression);
    switchClass(Integral);
    enter(ast->expression);
    switchClass(prevClass);

    return false;
}

bool ScalarTypeLattice::visit(BinaryExpressionAST *ast)
{
    BinaryExpressionAST* bin = ast->asBinaryExpression();

    auto handleGeneric = [this, bin] (Class entryClass, Class exitClass) {
        switchClass(entryClass);
        enter(bin->left_expression);
        switchClass(entryClass);
        enter(bin->right_expression);
        switchClass(exitClass);
    };

    switch (tokenKind(bin->binary_op_token)) {
        // Operators + and - are valid on both arithmetic and pointer types.
    case T_PLUS:
    case T_MINUS: {
        switchClass(Scalar);
        enter(bin->left_expression);
        Class lhsClass = switchClass(Scalar);
        enter(bin->right_expression);
        if ((lhsClass == Pointer
             && (clazz_ == Scalar
                 || clazz_ == Arithmetic
                 || clazz_ == Integral
                 || clazz_ == FloatingPoint))
                || (clazz_ == Pointer
                    && (lhsClass == Scalar
                        || lhsClass == Arithmetic
                        || lhsClass == Integral
                        || lhsClass == FloatingPoint))) {
            switchClass(Pointer);
        } else if (lhsClass == Pointer && clazz_ == Pointer) {
            switchClass(Integral);
        } else if (lhsClass > clazz_) {
            switchClass(lhsClass);
        }
        return false;
    }

        // The following operators are valid only on arithmetic types.
    case T_STAR:
    case T_SLASH:
    case T_PERCENT:
    case T_AMPER:
    case T_PIPE:
    case T_CARET:
    case T_LESS_LESS:
    case T_GREATER_GREATER: {
        handleGeneric(Arithmetic, Arithmetic);
        return false;
    }

        // Logical operators are valid on both arithmetic and pointer types.
    case T_LESS:
    case T_GREATER:
    case T_LESS_EQUAL:
    case T_GREATER_EQUAL:
    case T_AMPER_AMPER:
    case T_PIPE_PIPE: {
        handleGeneric(Scalar, Arithmetic);
        return false;
    }

        // Valid on any type.
    case T_EQUAL:
    case T_PLUS_EQUAL:
    case T_MINUS_EQUAL:
    case T_STAR_EQUAL:
    case T_SLASH_EQUAL:
    case T_PERCENT_EQUAL:
    case T_LESS_LESS_EQUAL:
    case T_GREATER_GREATER_EQUAL:
    case T_CARET_EQUAL:
    case T_AMPER_EQUAL:
    case T_PIPE_EQUAL: {
        switchClass(Undefined);
        enter(bin->right_expression);
        Class rhsClass = clazz_;
        accept(bin->left_expression);
        // The RHS's class is used to classify the LHS.
        switchClass(rhsClass);
        classify(bin->left_expression);
        // Assignment leaves the class of its LHS.
        return false;
    }

    default:
        Class prevClass = switchClass(Undefined);
        enter(bin->left_expression);
        switchClass(Undefined);
        enter(bin->right_expression);
        switchClass(prevClass);
        return false;
    }
}

bool ScalarTypeLattice::visit(ConditionalExpressionAST *ast)
{
    Class prevClass = switchClass(Scalar);
    enter(ast->condition);
    switchClass(Undefined);
    enter(ast->left_expression);
    switchClass(Undefined);
    enter(ast->right_expression);
    switchClass(prevClass);

    return false;
}

bool ScalarTypeLattice::visit(IdExpressionAST *ast)
{
    return false;
}

bool ScalarTypeLattice::visit(MemberAccessAST *ast)
{
    Class prevClass = clazz_;
    if (tokenKind(ast->access_token) == T_ARROW)
        switchClass(Pointer);
    else
        switchClass(Undefined);
    enter(ast->base_expression);
    switchClass(prevClass);

    return false;
}

bool ScalarTypeLattice::visit(NestedExpressionAST *ast)
{
    enter(ast->expression);

    return false;
}

bool ScalarTypeLattice::visit(UnaryExpressionAST *ast)
{
    switch (tokenKind(ast->unary_op_token)) {
    case T_STAR: {
        Class prevClass = switchClass(Pointer);
        enter(ast->expression);
        switchClass(prevClass);
        return false;
    }

    case T_EXCLAIM:
        if (Scalar > clazz_)
            switchClass(Scalar);
        switchClass(Scalar);
        enter(ast->expression);
        switchClass(Integral);
        return false;

    case T_PLUS:
    case T_MINUS:
        switchClass(Arithmetic);
        enter(ast->expression);
        if (Arithmetic > clazz_)
            switchClass(Arithmetic);
        return false;

    case T_PLUS_PLUS:
    case T_MINUS_MINUS:
        if (Scalar > clazz_)
            switchClass(Scalar);
        enter(ast->expression);
        return false;

    case T_AMPER:
        enter(ast->expression);
        switchClass(Pointer);
        return false;

    case T_TILDE:
        switchClass(Arithmetic);
        enter(ast->expression);
        switchClass(Arithmetic);
        return false;

    default:
        enter(ast->expression);
        PSYCHE_ASSERT(false, return false, "unhandled unary operator");
        return false;
    }
}

bool ScalarTypeLattice::visit(NumericLiteralAST *ast)
{
    const NumericLiteral *numLit =
            numericLiteral(ast->asNumericLiteral()->literal_token);
    PSYCHE_ASSERT(numLit, return false, "numeric literal must exist");
    if (numLit->isDouble()
            || numLit->isFloat()
            || numLit->isLongDouble()) {
        switchClass(FloatingPoint);
    } else {
        if (!strcmp(numLit->chars(), kZero)) {
            printDebug("Found null constant, 0, an integer or a pointer\n");
            switchClass(Scalar);
        } else {
            switchClass(Integral);
        }
    }

    return false;
}

bool ScalarTypeLattice::visit(BoolLiteralAST *ast)
{
    switchClass(Integral); // Booleans are integrals.
    return false;
}

bool ScalarTypeLattice::visit(StringLiteralAST *ast)
{
    switchClass(Pointer);
    return false;
}

bool ScalarTypeLattice::visit(PointerLiteralAST *ast)
{
    switchClass(Pointer);
    return false;
}

bool ScalarTypeLattice::visit(CallAST *ast)
{
    Class prevClass = clazz_;
    enter(ast->base_expression);
    for (ExpressionListAST* it = ast->expression_list; it; it = it->next) {
        switchClass(Undefined);
        enter(it->value);
    }
    switchClass(prevClass);

    return false;
}

bool ScalarTypeLattice::visit(PostIncrDecrAST* ast)
{
    switchClass(Scalar);
    enter(ast->base_expression);
    return false;
}

std::string ScalarTypeLattice::fetchText(AST *ast) const
{
    const Token first = tokenAt(ast->firstToken());
    const Token last = tokenAt(ast->lastToken());
    const char* begin = translationUnit()->firstSourceChar();
    const char* cur = begin + first.byteOffset;
    const char* end = begin + last.byteOffset;

    std::string s;
    s.reserve(end - cur);
    while (cur < end) {
        s.push_back(*cur);
        ++cur;
    }
    return s;
}
