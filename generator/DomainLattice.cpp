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

#include "DomainLattice.h"
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
#include "TypeOfExpression.h"
#include "TranslationUnit.h"
#include <algorithm>
#include <unordered_set>
#include <utility>

using namespace psyche;
using namespace psyche;

#define VISITOR_NAME "TypeLattice"

#define EXPR_REGION_CTRL ExpressionRegion r(withinExpr_)

namespace {

const char* const kUndefined = "undefined";
const char* const kScalar = "scalar";
const char* const kPointer = "pointer";
const char* const kIntegral = "integral";
const char* const kFloatingPoint = "floating-point";
const char* const kArithmetic = "arithmetic";

const char* const kZero = "0";

} // anonymous

const DomainLattice::Domain DomainLattice::Undefined(kUndefined);
const DomainLattice::Domain DomainLattice::Scalar(kScalar);
const DomainLattice::Domain DomainLattice::Pointer(kPointer);
const DomainLattice::Domain DomainLattice::Integral(kIntegral);
const DomainLattice::Domain DomainLattice::FloatingPoint(kFloatingPoint);
const DomainLattice::Domain DomainLattice::Arithmetic(kArithmetic);

bool DomainLattice::Domain::operator>=(const DomainLattice::Domain& other) const
{
    return name_ == other.name_ || (*this > other);
}

bool DomainLattice::Domain::operator>(const DomainLattice::Domain& other) const
{
    if (other == Undefined) {
        return name_ == kScalar
                || name_ == kPointer
                || name_ == kIntegral
                || name_ == kFloatingPoint
                || name_ == kArithmetic;
    }

    if (other == Scalar) {
        return name_ == kPointer
                || name_ == kIntegral
                || name_ == kFloatingPoint
                || name_ == kArithmetic;
    }

    if (other == Arithmetic) {
        return name_ == kIntegral
                || name_ == kFloatingPoint;
    }

    return false;
}

bool DomainLattice::Domain::operator==(const DomainLattice::Domain& other) const
{
    return name_ == other.name_;
}

bool DomainLattice::Domain::operator!=(const DomainLattice::Domain& other) const
{
    return !(*this == other);
}

DomainLattice::DomainLattice(TranslationUnit *unit)
    : ASTVisitor(unit)
    , withinExpr_(0)
    , lastDom_(Undefined)
    , matcher_(unit)
    , scope_(nullptr)
    , globalScope_(nullptr)
    , cutoffScope_(nullptr)
{}

void DomainLattice::categorize(TranslationUnitAST* ast, Scope* global)
{
    enterScope(global);
    globalScope_ = scope_;

    for (DeclarationListAST *it = ast->declaration_list; it; it = it->next) {
        resetCutoffScope();
        accept(it->value);
    }

    // Use collected function arguments to normalize domains in every call.
    for (auto& p : funcs_) {
        auto& argsData = p.second;
        for (auto& argData : argsData) {
            Domain dom = argData.dom_;
            bool needUpdate = false;
            for (ExpressionAST* arg : argData.instances_) {
                const Domain& argDom = retrieveDomain(arg, argData.callScope_); // TODO: Store scope.
                if (argDom > dom) {
                    dom = argDom;
                    needUpdate = true;
                }
            }

            if (needUpdate) {
                for (ExpressionAST* arg : argData.instances_) {
                    resetCutoffScope();
                    enforceLeastDomain(dom);
                    visitExpression(arg);
                }
            }
        }
    }
}

const DomainLattice::DB* DomainLattice::searchDB(const Scope* scope) const
{
    PSYCHE_ASSERT(scope, return nullptr, "expected valid scope");

    auto it = index_.find(scope);
    while (it == index_.end()) {
        if (!scope->enclosingScope())
            return nullptr;
        return searchDB(scope->enclosingScope());
    }
    return &it->second;
}

DomainLattice::DB *DomainLattice::findOrCreateDB(const Scope *scope)
{
    DB* db = nullptr;
    auto it = index_.find(scope);
    if (it == index_.end())
        db = &index_[scope];
    else
        db = &it->second;
    PSYCHE_ASSERT(db, return nullptr, "expected valid db");

    return db;
}

template <class T, class MapT>
DomainLattice::Domain DomainLattice::retrieveDomainCore(const T* v,
                                                        const MapT DB::*m,
                                                        const psyche::Scope* scope) const
{
    while (scope) {
        if (const DB* db = searchDB(scope)) {
            auto it = (db->*m).find(v);
            if (it != (db->*m).end())
                return it->second;
        }
        scope = scope->enclosingScope();
    }
    return Undefined;
}

DomainLattice::Domain DomainLattice::retrieveDomain(const Symbol* sym, const Scope* scope) const
{
    return retrieveDomainCore(sym, &DB::second, scope);
}

DomainLattice::Domain DomainLattice::retrieveDomain(const ExpressionAST* ast, const Scope* scope) const
{
    auto equiv = isKnownAST(ast);
    if (!equiv)
        return Undefined;

    return retrieveDomainCore(equiv, &DB::first, scope);
}

DomainLattice::Domain DomainLattice::domainForType(const FullySpecifiedType& ty,
                                                   const Symbol* sym)
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
            return Domain(kIntegral, kIntTy);
        }

        if (ty->asIntegerType()->kind() == IntegerType::Bool) {
            debug("Symbol %s lookedup as boolean (taken as integer)\n");
            return Domain(kIntegral, kIntTy);
        }

        if (ty->asIntegerType()->kind() == IntegerType::Char) {
            debug("Symbol %s looked as char\n");
            return Domain(kIntegral, kCharTy);
        }

        if (ty->asIntegerType()->kind() == IntegerType::Short) {
            debug("Symbol %s looked as short\n");
            return Domain(kIntegral, kShortTy);
        }

        if (ty->asIntegerType()->kind() == IntegerType::Long) {
            debug("Symbol %s looked as long\n");
            return Domain(kIntegral, kLongTy);
        }

        debug("Symbol %s looked as unknown integral\n");
        return Domain(kIntegral, kIntTy);
    }

    if (ty->asFloatType()) {
        if (ty->asFloatType()->kind() == FloatType::Float) {
            debug("Symbol %s lookedup as float\n");
            return Domain(kFloatingPoint, kFloatTy);
        }

        debug("Symbol %s looked as double\n");
        return Domain(kFloatingPoint, kDoubleTy);
    }

    if (ty->asPointerType() || ty->asArrayType()) {
        debug("Symbol %s lookedup as pointer:\n");
        // TODO: We can improve this by looking at the underlying element type
        // and using the "exact name" member of the class to hold this info.
        return Pointer;
    }

    if (ty->asNamedType() && ty->asNamedType()->name()->asNameId()) {
        // TODO: Compare char-by-char, matching at each substring.
        const Identifier* ident = ty->asNamedType()->name()->asNameId();
        return !strcmp(kSizeTy, ident->chars()) ?
                    Domain(kIntegral, kSizeTy) :
                    !strcmp(kPtrDiffTy, ident->chars()) ?
                        Domain(kIntegral, kPtrDiffTy) :
                        !strcmp(kIntPtrTy, ident->chars()) ?
                            Domain(kIntegral, kIntPtrTy) :
                            !strcmp(kUIntPtrTy, ident->chars()) ?
                                Domain(kIntegral, kUIntPtrTy) : Undefined;
    }

    return Undefined;
}

void DomainLattice::assignDomain(ExpressionAST *ast)
{
    if (!ast)
        return;

    const auto& astText = fetchText(ast);

    bool checkTy = true;

    if (ast->asNumericLiteral()) {
        // We don't want to type 0: it's both an integer and the null pointer constant.
        const NumericLiteral *numLit = numericLiteral(ast->asNumericLiteral()->literal_token);
        PSYCHE_ASSERT(numLit, return, "numeric literal must exist");
        checkTy = !numLit->isInt() || strcmp(numLit->chars(), kZero);
    }

    if (checkTy) {
        TypeOfExpression exprType(translationUnit());
        FullySpecifiedType ty = exprType.resolve(ast, const_cast<Scope*>(scope_));

        if (ty.isValid()) {
            auto dom = domainForType(ty, nullptr);
            if (dom > lastDom_) {
                enforceLeastDomain(dom);
                printDebug("Upgrade domain to %s, based on type information of AST <%s>\n",
                           lastDom_.name_.c_str(), astText.c_str());
            }
        }
    }

    if (ast->asIdExpression()) {
        auto valSym = lookupValueSymbol(ast->asIdExpression()->name->name, scope_);

        // The domain of a symbol applies from its declaration onwards or, in the case
        // its declaration is absent, within the entire scope.
        if (valSym && !control()->isSymbolAnulled(valSym)) {
            auto symScope = valSym->enclosingScope();
            if (!symScope->encloses(cutoffScope_))
                cutoffScope_ = symScope;

            DB* db = findOrCreateDB(symScope);
            if (lastDom_ > db->second[valSym]) {
                const auto& symName = SymbolPP().print(valSym);
                printDebug("Symbol %s (re)categorized as <%s>\n", symName.c_str(), lastDom_.name_.c_str());
                db->second[valSym] = lastDom_;
            }
        } else {
            resetCutoffScope();
        }
    }

    // The domain of an expression applies up to the least scope where the declaration
    // of a symbol involved in its subexpressions appears.
    DB* db = findOrCreateDB(cutoffScope_);
    if (auto equiv = isKnownAST(ast)) {
        auto ait = db->first.find(equiv);
        if (ait != db->first.end()) {
            auto& astDom = ait->second;
            if (astDom > lastDom_) {
                // Upgrade least domain
                enforceLeastDomain(astDom);
            } else if (lastDom_ > astDom) {
                // Update ast domain
                printDebug("AST %s re-categorized as <%s>\n",
                           astText.c_str(), lastDom_.name_.c_str());
                astDom = lastDom_;
            }
        } else {
            printDebug("AST %s categorized as <%s> in deeper scope\n",
                       astText.c_str(), lastDom_.name_.c_str());
            db->first[equiv] = lastDom_;
        }
    } else {
        printDebug("New AST %s categorized as <%s>\n", astText.c_str(), lastDom_.name_.c_str());
        knownAsts_.push_back(ast);
        db->first[ast] = lastDom_;
    }
}

    //--- Statements

bool DomainLattice::visit(FunctionDefinitionAST* ast)
{
    const Scope *prevScope = enterScope(ast->symbol);
    visitStatement(ast->function_body);
    enterScope(prevScope);

    return false;
}

void DomainLattice::visitStatement(StatementAST *ast)
{
    resetCutoffScope();
    accept(ast);
}

bool DomainLattice::visit(SwitchStatementAST *ast)
{
    enforceLeastDomain(Integral);
    visitExpression(ast->condition);

    return false;
}

bool DomainLattice::visit(CaseStatementAST *ast)
{
    enforceLeastDomain(Integral);
    visitExpression(ast->expression);

    return false;
}

bool DomainLattice::visit(CompoundStatementAST *ast)
{
    enforceLeastDomain(Undefined);

    const Scope *prevScope = enterScope(ast->symbol);
    for (StatementListAST *it = ast->statement_list; it; it = it->next)
        visitStatement(it->value);
    enterScope(prevScope);

    return false;
}

bool DomainLattice::visit(DeclarationStatementAST *ast)
{
    resetCutoffScope();
    accept(ast->declaration);

    return false;
}

bool DomainLattice::visit(DoStatementAST *ast)
{
    enforceLeastDomain(Scalar);

    visitExpression(ast->expression);
    visitStatement(ast->statement);

    return false;
}

bool DomainLattice::visit(WhileStatementAST *ast)
{
    enforceLeastDomain(Scalar);

    const Scope* prevScope = enterScope(ast->symbol);
    visitExpression(ast->condition);
    visitStatement(ast->statement);
    enterScope(prevScope);

    return false;
}

bool DomainLattice::visit(ForStatementAST *ast)
{
    enforceLeastDomain(Undefined);

    const Scope* prevScope = enterScope(ast->symbol);
    accept(ast->initializer);
    enforceLeastDomain(Scalar);
    visitExpression(ast->condition);
    enforceLeastDomain(Undefined);
    visitExpression(ast->expression);
    visitStatement(ast->statement);
    enterScope(prevScope);

    return false;
}

bool DomainLattice::visit(IfStatementAST *ast)
{
    enforceLeastDomain(Scalar);

    const Scope* prevScope = enterScope(ast->symbol);
    visitExpression(ast->condition);
    visitStatement(ast->statement);
    visitStatement(ast->else_statement);
    enterScope(prevScope);

    return false;
}

bool DomainLattice::visit(ExpressionStatementAST *ast)
{
    enforceLeastDomain(Undefined);

    visitExpression(ast->expression);

    return false;
}

bool DomainLattice::visit(ReturnStatementAST *ast)
{
    enforceLeastDomain(Undefined);

    visitExpression(ast->expression);

    return false;
}

    //--- Expressions

void DomainLattice::visitExpression(ExpressionAST *ast)
{
    accept(ast);
    assignDomain(ast);
}

ExpressionAST *DomainLattice::isKnownAST(const ExpressionAST *ast) const
{
    for (auto candidate : knownAsts_) {
        if (const_cast<ExpressionAST*>(ast)->match(candidate, &matcher_))
            return candidate;
    }
    return nullptr;
}

const Scope* DomainLattice::enterScope(const Scope* scope)
{
    if (!scope)
        return scope_;
    std::swap(scope_, scope);
    return scope;
}

void DomainLattice::resetCutoffScope()
{
    cutoffScope_ = globalScope_;
}

DomainLattice::Domain DomainLattice::enforceLeastDomain(DomainLattice::Domain dom)
{
    std::swap(lastDom_, dom);
    return dom;
}

bool DomainLattice::visit(ArrayAccessAST *ast)
{
    EXPR_REGION_CTRL;

    Domain prevDom = enforceLeastDomain(Pointer);
    visitExpression(ast->base_expression);
    enforceLeastDomain(Integral);
    visitExpression(ast->expression);
    enforceLeastDomain(prevDom);

    return false;
}

bool DomainLattice::visit(BinaryExpressionAST *ast)
{
    EXPR_REGION_CTRL;

    BinaryExpressionAST* expr = ast->asBinaryExpression();

    auto plusMinus = [this, expr] (Domain lhsDom, bool isMinus) {
        if (lastDom_ == Scalar) {
            if (lhsDom == Scalar || lhsDom == Pointer)
                enforceLeastDomain(isMinus ? Scalar : lhsDom);
        } else if (lastDom_ == Pointer) {
            if (lhsDom == Scalar) {
                enforceLeastDomain(isMinus ? Scalar : Pointer);
            } else if (lhsDom == Arithmetic
                            || lhsDom == Integral
                            || lhsDom == FloatingPoint) {
                enforceLeastDomain(Pointer);
            } else if (isMinus && lhsDom == Pointer) {
                enforceLeastDomain(Integral);
            } else {
                enforceLeastDomain(Scalar);
            }
        } else if (lastDom_ == Arithmetic
                        || lastDom_ == Integral
                        || lastDom_ == FloatingPoint) {
            if (lhsDom == Scalar || lhsDom == Pointer)
                enforceLeastDomain(lhsDom);
        }
    };

    switch (tokenKind(expr->binary_op_token)) {
    case T_PLUS: {
        Domain entryDom = Scalar > lastDom_ ? Scalar : lastDom_;
        enforceLeastDomain(entryDom);
        visitExpression(expr->left_expression);
        Domain lhsDom = lastDom_;
        if (lhsDom == Pointer && entryDom == Pointer)
            enforceLeastDomain(Scalar);
        else
            enforceLeastDomain(entryDom);
        visitExpression(expr->right_expression);

        plusMinus(lhsDom, false);
        return false;
    }

    case T_MINUS: {
        if (Scalar > lastDom_ || lastDom_ == Pointer) {
            Domain entryDom = Scalar > lastDom_ ? Scalar : Pointer;
            enforceLeastDomain(entryDom);
            visitExpression(expr->left_expression);
            Domain lhsDom = enforceLeastDomain(entryDom);
            visitExpression(expr->right_expression);

            plusMinus(lhsDom, true);
        } else {
            // Current domain is arithmetic or higher.
            Domain prevDom = enforceLeastDomain(Scalar);
            visitExpression(expr->left_expression);
            Domain lhsDom = enforceLeastDomain(Scalar);
            visitExpression(expr->right_expression);

            if (lhsDom == Scalar && lastDom_ != Scalar) {
                enforceLeastDomain(lastDom_);
                visitExpression(expr->left_expression);
            } else if (lastDom_ == Scalar && lhsDom != Scalar) {
                enforceLeastDomain(lhsDom);
                visitExpression(expr->right_expression);
            }
            enforceLeastDomain(prevDom);
        }
        return false;
    }

    // Arithmetic
    case T_STAR:
    case T_SLASH:
    case T_PERCENT:
    case T_AMPER:
    case T_PIPE:
    case T_CARET:
    case T_LESS_LESS:
    case T_GREATER_GREATER: {
        enforceLeastDomain(Arithmetic);
        visitExpression(expr->left_expression);
        enforceLeastDomain(Arithmetic);
        visitExpression(expr->right_expression);
        enforceLeastDomain(Arithmetic);
        return false;
    }

    // Comparisson
    case T_LESS:
    case T_LESS_EQUAL:
    case T_GREATER:
    case T_GREATER_EQUAL:
    case T_EQUAL_EQUAL:
    case T_EXCLAIM_EQUAL: {
        enforceLeastDomain(Scalar);
        visitExpression(expr->right_expression);
        Domain rhsClass = lastDom_;
        visitExpression(expr->left_expression);
        if (lastDom_ > rhsClass) {
            visitExpression(expr->right_expression); }
        enforceLeastDomain(Integral);
        return false;
    }

    // Logical
    case T_AMPER_AMPER:
    case T_PIPE_PIPE: {
        enforceLeastDomain(Scalar);
        visitExpression(expr->left_expression);
        enforceLeastDomain(Scalar);
        visitExpression(expr->right_expression);
        enforceLeastDomain(Arithmetic);
        return false;
    }

    // Plain assignment
    case T_EQUAL: {
        enforceLeastDomain(Undefined);
        visitExpression(expr->right_expression);
        Domain rhsClass = lastDom_;
        visitExpression(expr->left_expression);
        if (lastDom_ > rhsClass)
            visitExpression(expr->right_expression);
        return false;
    }

    // Scalar assignment
    case T_PLUS_EQUAL:
    case T_MINUS_EQUAL: {
        enforceLeastDomain(Scalar);
        visitExpression(expr->right_expression);
        enforceLeastDomain(Scalar);
        accept(expr->left_expression);
        enforceLeastDomain(Scalar);
        return false;
    }

    // Compound assignment
    case T_STAR_EQUAL:
    case T_SLASH_EQUAL:
    case T_PERCENT_EQUAL:
    case T_AMPER_EQUAL:
    case T_PIPE_EQUAL:
    case T_CARET_EQUAL:
    case T_LESS_LESS_EQUAL:
    case T_GREATER_GREATER_EQUAL: {
        enforceLeastDomain(Arithmetic);
        visitExpression(expr->left_expression);
        enforceLeastDomain(Arithmetic);
        visitExpression(expr->right_expression);
        return false;
    }

    case T_COMMA: {
        Domain dom = enforceLeastDomain(Undefined);
        visitExpression(expr->left_expression);
        enforceLeastDomain(Undefined);
        visitExpression(expr->right_expression);
        enforceLeastDomain(dom);
        return false;
    }

    default:
        PSYCHE_ASSERT(false, return false, "unrecognized operator");
        return false;
    }
}

bool DomainLattice::visit(ConditionalExpressionAST *ast)
{
    EXPR_REGION_CTRL;

    Domain prevDom = enforceLeastDomain(Scalar);
    visitExpression(ast->condition);
    enforceLeastDomain(prevDom);
    visitExpression(ast->left_expression);
    visitExpression(ast->right_expression);
    enforceLeastDomain(prevDom);

    return false;
}

bool DomainLattice::visit(IdExpressionAST *ast)
{
    return false;
}

bool DomainLattice::visit(MemberAccessAST *ast)
{
    EXPR_REGION_CTRL;

    Domain prevDom = lastDom_;
    if (tokenKind(ast->access_token) == T_ARROW)
        enforceLeastDomain(Pointer);
    else
        enforceLeastDomain(Undefined);
    visitExpression(ast->base_expression);
    enforceLeastDomain(prevDom);

    return false;
}

bool DomainLattice::visit(UnaryExpressionAST *ast)
{
    EXPR_REGION_CTRL;

    switch (tokenKind(ast->unary_op_token)) {
    case T_STAR: {
        Domain prevDom = enforceLeastDomain(Pointer);
        visitExpression(ast->expression);
        enforceLeastDomain(prevDom);
        return false;
    }

    case T_EXCLAIM:
        enforceLeastDomain(Scalar);
        visitExpression(ast->expression);
        enforceLeastDomain(Integral);
        return false;

    case T_PLUS:
    case T_MINUS:
        enforceLeastDomain(Arithmetic);
        visitExpression(ast->expression);
        if (Arithmetic > lastDom_)
            enforceLeastDomain(Arithmetic);
        return false;

    case T_PLUS_PLUS:
    case T_MINUS_MINUS:
        if (Scalar > lastDom_)
            enforceLeastDomain(Scalar);
        visitExpression(ast->expression);
        return false;

    case T_AMPER:
        visitExpression(ast->expression);
        enforceLeastDomain(Pointer);
        return false;

    case T_TILDE:
        enforceLeastDomain(Arithmetic);
        visitExpression(ast->expression);
        enforceLeastDomain(Arithmetic);
        return false;

    default:
        visitExpression(ast->expression);
        PSYCHE_ASSERT(false, return false, "unhandled unary operator");
        return false;
    }
}

bool DomainLattice::visit(NumericLiteralAST *ast)
{
    EXPR_REGION_CTRL;

    const Token& tk = tokenAt(ast->literal_token);
    if (tk.is(T_CHAR_LITERAL)) {
        // TODO: char/int
        enforceLeastDomain(Domain(kIntegral, kCharTy));
    } else {
        const NumericLiteral *numLit = numericLiteral(ast->literal_token);
        PSYCHE_ASSERT(numLit, return false, "numeric literal must exist");
        if (numLit->isDouble()) {
            enforceLeastDomain(Domain(kFloatingPoint, kDoubleTy));
        } else if (numLit->isFloat()) {
            enforceLeastDomain(Domain(kFloatingPoint, kFloatTy));
        } else if (numLit->isLongDouble()) {
            enforceLeastDomain(Domain(kFloatingPoint, kLongDoubleTy));
        } else if (numLit->isLong()) {
            enforceLeastDomain(Domain(kIntegral, kLongTy));
        } else if (numLit->isLongLong()) {
            enforceLeastDomain(Domain(kIntegral, kLongLongTy));
        } else {
            if (!strcmp(numLit->chars(), kZero)) {
                printDebug("Found null constant, 0, an integer or a pointer\n");
                enforceLeastDomain(Scalar);
            } else {
                enforceLeastDomain(Integral);
            }
        }
    }

    return false;
}

bool DomainLattice::visit(BoolLiteralAST *ast)
{
    EXPR_REGION_CTRL;

    enforceLeastDomain(Integral); // Booleans are integrals.

    return false;
}

bool DomainLattice::visit(StringLiteralAST *ast)
{
    EXPR_REGION_CTRL;

    enforceLeastDomain(Pointer);

    return false;
}

bool DomainLattice::visit(SizeofExpressionAST *ast)
{
    EXPR_REGION_CTRL;

    enforceLeastDomain(Undefined);
    visitExpression(ast->expression);

    return false;
}

bool DomainLattice::visit(PointerLiteralAST *ast)
{
    enforceLeastDomain(Pointer);

    return false;
}

bool DomainLattice::visit(BracedInitializerAST *ast)
{
    EXPR_REGION_CTRL;

    for (auto it = ast->expression_list; it; it = it->next) {
        enforceLeastDomain(Undefined);
        visitExpression(it->value);
    }

    return false;
}

bool DomainLattice::visit(CallAST *ast)
{
    EXPR_REGION_CTRL;

    Domain prevDom = lastDom_;
    visitExpression(ast->base_expression);

    ExpressionAST* equivalent = isKnownAST(ast->base_expression);
    if (equivalent == nullptr)
        equivalent = ast->base_expression;
    auto& data = funcs_[equivalent];

    // Ensure the argument data has the proper size. This must also account for variadic
    // functions.
    auto argLength = 0u;
    for (ExpressionListAST* it = ast->expression_list; it; it = it->next)
        ++argLength;
    if (data.size() <= argLength)
        data.resize(argLength);


    Function* func = nullptr;
    if (ast->base_expression->asIdExpression()) {
        auto sym = lookupSymbol(ast->base_expression->asIdExpression()->name->name, scope_);
        if (sym)
            func = sym->asFunction();

        if (!func && sym && sym->asDeclaration() && sym->asDeclaration()->type())
            func = sym->asDeclaration()->type()->asFunctionType();
    }

    auto idx = 0u;
    for (ExpressionListAST* it = ast->expression_list; it; it = it->next, ++idx) {
        data[idx].instances_.push_back(it->value);
        data[idx].callScope_ = scope_; // TODO: Make this a call (not arg) data.

        auto paramDom = DomainLattice::Undefined;
        if (func) {
            auto argSym = func->argumentAt(idx);
            if (argSym)
                paramDom = domainForType(argSym->type(), argSym);
        }

        enforceLeastDomain(paramDom);
        visitExpression(it->value);

        // If a higher rank is reached, update the argument data.
        if (lastDom_ > data[idx].dom_) {
            data[idx].dom_ = lastDom_;
        } else if (data[idx].dom_ > lastDom_) {
            lastDom_ = data[idx].dom_;
            visitExpression(it->value);
        }
    }
    enforceLeastDomain(prevDom);

    return false;
}

bool DomainLattice::visit(CastExpressionAST *ast)
{
    Domain finalDom;
    if (withinExpr_) {
        EXPR_REGION_CTRL;
        enforceLeastDomain(Scalar);
        visitExpression(ast->type_id);
        finalDom = enforceLeastDomain(Scalar);
        visitExpression(ast->expression);
    } else {
        EXPR_REGION_CTRL;
        enforceLeastDomain(Undefined);
        visitExpression(ast->type_id);
        finalDom = enforceLeastDomain(Undefined);
        visitExpression(ast->expression);
    }
    enforceLeastDomain(finalDom);

    return false;
}

bool DomainLattice::visit(PostIncrDecrAST* ast)
{
    EXPR_REGION_CTRL;

    enforceLeastDomain(Scalar);
    visitExpression(ast->base_expression);

    return false;
}

bool DomainLattice::visit(NestedExpressionAST* ast)
{
    EXPR_REGION_CTRL;

    visitExpression(ast->expression);

    return false;
}

    //--- Declarations

bool DomainLattice::visit(SimpleDeclarationAST* ast)
{
    DeclaratorListAST *declIt = ast->declarator_list;
    for (const List<Symbol*>* symIt = ast->symbols;
            symIt;
            symIt = symIt->next, declIt = declIt->next) {
        if (symIt->value->asForwardClassDeclaration())
            break;

        PSYCHE_ASSERT(declIt->value, return false, "expected declarator");
        if (!declIt->value->initializer
                || declIt->value->initializer->asBracedInitializer())
            continue;

        Symbol* decl = symIt->value;
        auto clazz = domainForType(decl->type(), decl);
        enforceLeastDomain(clazz);
        visitExpression(declIt->value->initializer);
    }

    return false;
}

std::string DomainLattice::fetchText(AST *ast) const
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
