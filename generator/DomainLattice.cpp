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
#include "Utils.h"
#include <algorithm>
#include <unordered_set>
#include <utility>

using namespace CPlusPlus;
using namespace psyche;

#define VISITOR_NAME "TypeLattice"

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
    , lastDom_(Undefined)
    , matcher_(unit)
{}

void DomainLattice::categorize(TranslationUnitAST* ast, Scope* global)
{
    scope_ = global;

    for (DeclarationListAST *it = ast->declaration_list; it; it = it->next)
        accept(it->value);

    // Use collected function arguments to normalize domains in every call.
    for (auto& p : funcs_) {
        auto& argsData = p.second;
        for (auto& argData : argsData) {
            Domain dom = argData.clazz_;
            bool needUpdate = false;
            for (ExpressionAST* arg : argData.instances_) {
                const Domain& argDom = retrieveDomain(arg);
                if (argDom > dom) {
                    dom = argDom;
                    needUpdate = true;
                }
            }

            if (needUpdate) {
                for (ExpressionAST* arg : argData.instances_) {
                    enforceBaseDomain(dom);
                    visitExpression(arg);
                }
            }
        }
    }
}

DomainLattice::Domain DomainLattice::retrieveDomain(const Symbol* sym) const
{
    auto it = symbolDB_.find(const_cast<Symbol*>(sym));
    if (it != symbolDB_.end())
        return it->second;
    return Undefined;
}

DomainLattice::Domain DomainLattice::retrieveDomain(const Symbol *sym, const Scope* scope) const
{
    // TODO
    return Undefined;
}

DomainLattice::Domain DomainLattice::retrieveDomain(ExpressionAST *ast) const
{
    auto equivalent = isKnownAST(ast);
    if (!equivalent)
        return Undefined;

    PSYCHE_ASSERT(astDB_.find(equivalent) != astDB_.end(),
                  return Undefined,
                  "expected equivalent ast in index");
    return astDB_.find(equivalent)->second;
}

DomainLattice::Domain DomainLattice::retrieveDomain(ExpressionAST *ast, const Scope* scope) const
{
    // TODO
    return Undefined;
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
        const Identifier* ident = ty->asNamedType()->name()->asNameId();
        return !strcmp(kSizeT, ident->chars()) ?
                    Domain(kIntegral, kSizeT) :
                    !strcmp(ptrdiff_t__, ident->chars()) ?
                        Domain(kIntegral, ptrdiff_t__) : Undefined;
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
        const NumericLiteral *numLit =
                numericLiteral(ast->asNumericLiteral()->literal_token);
        PSYCHE_ASSERT(numLit, return, "numeric literal must exist");
        checkTy = !numLit->isInt() || strcmp(numLit->chars(), kZero);
    }

    // Whenever we have typing information, we must use it.
    if (checkTy) {
        TypeOfExpression typeofExpr(translationUnit());
        FullySpecifiedType ty = typeofExpr.resolve(ast, const_cast<Scope*>(scope_));
        if (ty.isValid()) {
            // This AST can be of any form, but it it's an identifier-expression
            // we recover the symbol so we can associated the class with it too.
            const Symbol* valSym = nullptr;
            if (ast->asIdExpression())
                valSym = lookupValueSymbol(ast->asIdExpression()->name->name, scope_);

            auto clazz = domainForType(ty, valSym);
            if (clazz > lastDom_) {
                enforceBaseDomain(clazz);
                printDebug("Upgrade class to %s, based type of AST <%s>\n",
                           lastDom_.name_.c_str(), astText.c_str());
            }

            // The "extra" association of classes with symbols is needed only
            // in the case of declarations with initializers.
            if (valSym) {
                const auto& symName = SymbolPP().print(valSym);
                auto it = symbolDB_.find(valSym);
                if (it == symbolDB_.end()) {
                    symbolDB_.insert(std::make_pair(valSym, clazz));
                    printDebug("Symbol %s classified as %s in AST <%s>\n",
                               symName.c_str(), lastDom_.name_.c_str(), astText.c_str());
                } else {
                    it->second = clazz;
                    printDebug("Symbol %s classified as %s in AST <%s>\n",
                               symName.c_str(), lastDom_.name_.c_str(), astText.c_str());
                }
            }
        }
    }

    if (auto equivalent = isKnownAST(ast)) {
        PSYCHE_ASSERT(astDB_.find(equivalent) != astDB_.end(),
                      return,
                      "expected equivalent ast in index");

        if (astDB_[equivalent] > lastDom_) {
            enforceBaseDomain(astDB_[equivalent]);
            printDebug("Upgrade class to %s, from equivalent of AST <%s>\n",
                       lastDom_.name_.c_str(), astText.c_str());
            return;
        }
        if (lastDom_ > astDB_[equivalent]) {
            astDB_[equivalent] = lastDom_;
            printDebug("AST <%s> re-classified as %s\n",
                       astText.c_str(), lastDom_.name_.c_str());
            return;
        }
    }

    knownAsts_.push_back(ast);
    astDB_[ast] = lastDom_;
    printDebug("AST <%s> classified as %s\n",
               astText.c_str(), lastDom_.name_.c_str());
}

void DomainLattice::visitExpression(ExpressionAST *ast)
{
    accept(ast);
    assignDomain(ast);
}

bool DomainLattice::visit(FunctionDefinitionAST* ast)
{
    accept(ast->function_body);

    return false;
}

bool DomainLattice::visit(SwitchStatementAST *ast)
{
    enforceBaseDomain(Integral);
    visitExpression(ast->condition);

    return false;
}

bool DomainLattice::visit(CaseStatementAST *ast)
{
    enforceBaseDomain(Integral);
    visitExpression(ast->expression);

    return false;
}

bool DomainLattice::visit(CompoundStatementAST *ast)
{
    enforceBaseDomain(Undefined);

    const Scope *prevScope = switchScope(ast->symbol);
    for (StatementListAST *it = ast->statement_list; it; it = it->next)
        accept(it->value);
    switchScope(prevScope);

    return false;
}

bool DomainLattice::visit(DeclarationStatementAST *ast)
{
    accept(ast->declaration);

    return false;
}

bool DomainLattice::visit(DoStatementAST *ast)
{
    enforceBaseDomain(Scalar);
    visitExpression(ast->expression);
    accept(ast->statement);

    return false;
}

bool DomainLattice::visit(WhileStatementAST *ast)
{
    enforceBaseDomain(Scalar);
    visitExpression(ast->condition);
    accept(ast->statement);

    return false;
}

bool DomainLattice::visit(ForStatementAST *ast)
{
    enforceBaseDomain(Undefined);
    accept(ast->initializer);
    enforceBaseDomain(Scalar);
    visitExpression(ast->condition);
    enforceBaseDomain(Undefined);
    visitExpression(ast->expression);
    accept(ast->statement);

    return false;
}

bool DomainLattice::visit(IfStatementAST *ast)
{
    enforceBaseDomain(Scalar);
    visitExpression(ast->condition);
    accept(ast->statement);
    accept(ast->else_statement);

    return false;
}

bool DomainLattice::visit(ExpressionStatementAST *ast)
{
    enforceBaseDomain(Undefined);
    visitExpression(ast->expression);

    return false;
}

bool DomainLattice::visit(ReturnStatementAST *ast)
{
    enforceBaseDomain(Undefined);
    visitExpression(ast->expression);

    return false;
}

ExpressionAST *DomainLattice::isKnownAST(ExpressionAST *ast) const
{
    for (auto candidate : knownAsts_) {
        if (ast->match(candidate, &matcher_))
            return candidate;
    }
    return nullptr;
}

const Scope* DomainLattice::switchScope(const Scope* scope)
{
    if (!scope)
        return scope_;
    std::swap(scope_, scope);
    return scope;
}

DomainLattice::Domain DomainLattice::enforceBaseDomain(DomainLattice::Domain clazz)
{
    std::swap(lastDom_, clazz);
    return clazz;
}

bool DomainLattice::visit(ArrayAccessAST *ast)
{
    Domain prevClass = enforceBaseDomain(Pointer);
    visitExpression(ast->base_expression);
    enforceBaseDomain(Integral);
    visitExpression(ast->expression);
    enforceBaseDomain(prevClass);

    return false;
}

bool DomainLattice::visit(BinaryExpressionAST *ast)
{
    BinaryExpressionAST* expr = ast->asBinaryExpression();

    auto plusMinus = [this, expr] (Domain lhsDom, bool isMinus) {
        if (lastDom_ == Scalar) {
            if (lhsDom == Scalar || lhsDom == Pointer)
                enforceBaseDomain(isMinus ? Scalar : lhsDom);
        } else if (lastDom_ == Pointer) {
            if (lhsDom == Scalar) {
                enforceBaseDomain(isMinus ? Scalar : Pointer);
            } else if (lhsDom == Arithmetic
                            || lhsDom == Integral
                            || lhsDom == FloatingPoint) {
                enforceBaseDomain(Pointer);
            } else if (isMinus && lhsDom == Pointer) {
                enforceBaseDomain(Integral);
            } else {
                enforceBaseDomain(Scalar);
            }
        } else if (lastDom_ == Arithmetic
                        || lastDom_ == Integral
                        || lastDom_ == FloatingPoint) {
            if (lhsDom == Scalar || lhsDom == Pointer)
                enforceBaseDomain(lhsDom);
        }
    };

    switch (tokenKind(expr->binary_op_token)) {
    case T_PLUS: {
        Domain entryDom = Scalar > lastDom_ ? Scalar : lastDom_;
        enforceBaseDomain(entryDom);
        visitExpression(expr->left_expression);
        Domain lhsDom = lastDom_;
        if (lhsDom == Pointer && entryDom == Pointer)
            enforceBaseDomain(Scalar);
        else
            enforceBaseDomain(entryDom);
        visitExpression(expr->right_expression);

        plusMinus(lhsDom, false);
        return false;
    }

    case T_MINUS: {
        if (Scalar > lastDom_ || lastDom_ == Pointer) {
            Domain entryDom = Scalar > lastDom_ ? Scalar : Pointer;
            enforceBaseDomain(entryDom);
            visitExpression(expr->left_expression);
            Domain lhsDom = enforceBaseDomain(entryDom);
            visitExpression(expr->right_expression);

            plusMinus(lhsDom, true);
        } else {
            // Current domain is arithmetic or higher.
            Domain prevDom = enforceBaseDomain(Scalar);
            visitExpression(expr->left_expression);
            Domain lhsDom = enforceBaseDomain(Scalar);
            visitExpression(expr->right_expression);

            if (lhsDom == Scalar && lastDom_ != Scalar) {
                enforceBaseDomain(lastDom_);
                visitExpression(expr->left_expression);
            } else if (lastDom_ == Scalar && lhsDom != Scalar) {
                enforceBaseDomain(lhsDom);
                visitExpression(expr->right_expression);
            }
            enforceBaseDomain(prevDom);
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
        enforceBaseDomain(Arithmetic);
        visitExpression(expr->left_expression);
        enforceBaseDomain(Arithmetic);
        visitExpression(expr->right_expression);
        enforceBaseDomain(Arithmetic);
        return false;
    }

    // Comparisson
    case T_LESS:
    case T_LESS_EQUAL:
    case T_GREATER:
    case T_GREATER_EQUAL:
    case T_EQUAL_EQUAL:
    case T_EXCLAIM_EQUAL: {
        enforceBaseDomain(Scalar);
        visitExpression(expr->right_expression);
        Domain rhsClass = lastDom_;
        visitExpression(expr->left_expression);
        if (lastDom_ > rhsClass) {
            visitExpression(expr->right_expression); }
        enforceBaseDomain(Integral);
        return false;
    }

    // Logical
    case T_AMPER_AMPER:
    case T_PIPE_PIPE: {
        enforceBaseDomain(Scalar);
        visitExpression(expr->left_expression);
        enforceBaseDomain(Scalar);
        visitExpression(expr->right_expression);
        enforceBaseDomain(Arithmetic);
        return false;
    }

    // Plain assignment
    case T_EQUAL: {
        enforceBaseDomain(Undefined);
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
        enforceBaseDomain(Scalar);
        visitExpression(expr->right_expression);
        enforceBaseDomain(Scalar);
        accept(expr->left_expression);
        enforceBaseDomain(Scalar);
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
        enforceBaseDomain(Arithmetic);
        visitExpression(expr->left_expression);
        enforceBaseDomain(Arithmetic);
        visitExpression(expr->right_expression);
        return false;
    }

    case T_COMMA: {
        Domain dom = enforceBaseDomain(Undefined);
        visitExpression(expr->left_expression);
        enforceBaseDomain(Undefined);
        visitExpression(expr->right_expression);
        enforceBaseDomain(dom);
        return false;
    }

    default:
        PSYCHE_ASSERT(false, return false, "unrecognized operator");
        return false;
    }
}

bool DomainLattice::visit(ConditionalExpressionAST *ast)
{
    Domain prevClass = enforceBaseDomain(Scalar);
    visitExpression(ast->condition);
    enforceBaseDomain(prevClass);
    visitExpression(ast->left_expression);
    visitExpression(ast->right_expression);
    enforceBaseDomain(prevClass);

    return false;
}

bool DomainLattice::visit(IdExpressionAST *ast)
{
    return false;
}

bool DomainLattice::visit(MemberAccessAST *ast)
{
    Domain prevClass = lastDom_;
    if (tokenKind(ast->access_token) == T_ARROW)
        enforceBaseDomain(Pointer);
    else
        enforceBaseDomain(Undefined);
    visitExpression(ast->base_expression);
    enforceBaseDomain(prevClass);

    return false;
}

bool DomainLattice::visit(UnaryExpressionAST *ast)
{
    switch (tokenKind(ast->unary_op_token)) {
    case T_STAR: {
        Domain prevClass = enforceBaseDomain(Pointer);
        visitExpression(ast->expression);
        enforceBaseDomain(prevClass);
        return false;
    }

    case T_EXCLAIM:
        enforceBaseDomain(Scalar);
        visitExpression(ast->expression);
        enforceBaseDomain(Integral);
        return false;

    case T_PLUS:
    case T_MINUS:
        enforceBaseDomain(Arithmetic);
        visitExpression(ast->expression);
        if (Arithmetic > lastDom_)
            enforceBaseDomain(Arithmetic);
        return false;

    case T_PLUS_PLUS:
    case T_MINUS_MINUS:
        if (Scalar > lastDom_)
            enforceBaseDomain(Scalar);
        visitExpression(ast->expression);
        return false;

    case T_AMPER:
        visitExpression(ast->expression);
        enforceBaseDomain(Pointer);
        return false;

    case T_TILDE:
        enforceBaseDomain(Arithmetic);
        visitExpression(ast->expression);
        enforceBaseDomain(Arithmetic);
        return false;

    default:
        visitExpression(ast->expression);
        PSYCHE_ASSERT(false, return false, "unhandled unary operator");
        return false;
    }
}

bool DomainLattice::visit(NumericLiteralAST *ast)
{
    const Token& tk = tokenAt(ast->literal_token);
    if (tk.is(T_CHAR_LITERAL)) {
        enforceBaseDomain(Domain(kIntegral, kCharTy));
    } else {
        const NumericLiteral *numLit = numericLiteral(ast->literal_token);
        PSYCHE_ASSERT(numLit, return false, "numeric literal must exist");
        if (numLit->isDouble()) {
            enforceBaseDomain(Domain(kFloatingPoint, kDoubleTy));
        } else if (numLit->isFloat()) {
            enforceBaseDomain(Domain(kFloatingPoint, kFloatTy));
        } else if (numLit->isLongDouble()) {
            enforceBaseDomain(Domain(kFloatingPoint, kLongDouble));
        } else if (numLit->isLong()) {
            enforceBaseDomain(Domain(kIntegral, kLongTy));
        } else if (numLit->isLongLong()) {
            enforceBaseDomain(Domain(kIntegral, kLongLongTy));
        } else {
            if (!strcmp(numLit->chars(), kZero)) {
                printDebug("Found null constant, 0, an integer or a pointer\n");
                enforceBaseDomain(Scalar);
            } else {
                enforceBaseDomain(Integral);
            }
        }
    }

    return false;
}

bool DomainLattice::visit(BoolLiteralAST *ast)
{
    enforceBaseDomain(Integral); // Booleans are integrals.

    return false;
}

bool DomainLattice::visit(StringLiteralAST *ast)
{
    enforceBaseDomain(Pointer);

    return false;
}

bool DomainLattice::visit(SizeofExpressionAST *ast)
{
    enforceBaseDomain(Undefined);
    visitExpression(ast->expression);

    return false;
}

bool DomainLattice::visit(PointerLiteralAST *ast)
{
    enforceBaseDomain(Pointer);

    return false;
}

bool DomainLattice::visit(BracedInitializerAST *ast)
{
    for (auto it = ast->expression_list; it; it = it->next) {
        enforceBaseDomain(Undefined);
        visitExpression(it->value);
    }

    return false;
}

bool DomainLattice::visit(CallAST *ast)
{
    Domain prevClass = lastDom_;
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

        auto paramDom = DomainLattice::Undefined;
        if (func) {
            auto argSym = func->argumentAt(idx);
            if (argSym)
                paramDom = domainForType(argSym->type(), argSym);
        }

        enforceBaseDomain(paramDom);
        visitExpression(it->value);

        // If a higher rank is reached, update the argument data.
        if (lastDom_ > data[idx].clazz_) {
            data[idx].clazz_ = lastDom_;
        } else if (data[idx].clazz_ > lastDom_) {
            lastDom_ = data[idx].clazz_;
            visitExpression(it->value);
        }
    }
    enforceBaseDomain(prevClass);

    return false;
}

bool DomainLattice::visit(CastExpressionAST *ast)
{
    enforceBaseDomain(Undefined);
    visitExpression(ast->expression);

    return false;
}

bool DomainLattice::visit(PostIncrDecrAST* ast)
{
    enforceBaseDomain(Scalar);
    visitExpression(ast->base_expression);

    return false;
}

bool DomainLattice::visit(NestedExpressionAST* ast)
{
    visitExpression(ast->expression);

    return false;
}

bool DomainLattice::visit(SimpleDeclarationAST* ast)
{
    DeclaratorListAST *declIt = ast->declarator_list;
    for (const List<Symbol*>* symIt = ast->symbols;
            symIt;
            symIt = symIt->next, declIt = declIt->next) {
        PSYCHE_ASSERT(declIt->value, return false, "expected declarator");
        if (!declIt->value->initializer
                || declIt->value->initializer->asBracedInitializer())
            continue;

        Symbol* decl = symIt->value;
        auto clazz = domainForType(decl->type(), decl);
        enforceBaseDomain(clazz);
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
