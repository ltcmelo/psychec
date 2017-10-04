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

/*
 Contributors:
   - Marcus Rodrigues (demaroar@gmail.com)
*/

#include "ConstraintGenerator.h"
#include "ConstraintWriter.h"
#include "AST.h"
#include "Assert.h"
#include "BuiltinNames.h"
#include "Control.h"
#include "CoreTypes.h"
#include "DeclarationInterceptor.h"
#include "Debug.h"
#include "Literals.h"
#include "PrintfScanner.h"
#include "Scope.h"
#include "Symbols.h"
#include "SymbolPP.h"
#include "TranslationUnit.h"
#include "TypeOfExpression.h"
#include "VisitorObserver.h"
#include <iostream>
#include <algorithm>

#define VISITOR_NAME "ConstraintGenerator"

#define OBSERVE(AST_NAME) ObserverInvoker<AST_NAME> invoker(observer_, ast, scope_)

#define ENSURE_NONEMPTY_TYPE_STACK(CODE) \
    PSYCHE_ASSERT(!types_.empty(), CODE, "type stack must be nonempty")

#define ENSURE_NONEMPTY_ALPHA_RET_STACK(CODE) \
    PSYCHE_ASSERT(!valuedRets_.empty(), CODE, "alpha return stack must be nonempty")

using namespace psyche;
using namespace psyche;

namespace {

// Extract the identifier of a name. Use this only when it's guaranteed that
// the underlying name is indeed a simple one.
std::string extractId(const Name* name)
{
    PSYCHE_ASSERT(name && (name->isNameId() || name->isTaggedNameId()),
                  return std::string(),
                  "expected either trival or elaborated name");

    const Identifier *id = name->identifier();
    return std::string(id->chars(), id->size());
}

}

std::string ConstraintGenerator::declPrefix_ = "decl";
std::string ConstraintGenerator::stubPrefix_ = "stub";

ConstraintGenerator::ConstraintGenerator(TranslationUnit *unit,
                                         ConstraintWriter* writer)
    : ASTVisitor(unit)
    , scope_(nullptr)
    , writer_(writer)
    , lattice_(nullptr)
    , staticDecl_(false)
    , unnamedCount_(0)
    , observer_(nullptr)
    , interceptor_(nullptr)
{
    addPrintfLike("printf", 0);
    addPrintfLike("printf_s", 0);
    addPrintfLike("wprintf", 0);
    addPrintfLike("wprintf_s", 0);
    addPrintfLike("fprintf", 1);
    addPrintfLike("fprintf_s", 1);
    addPrintfLike("fwprintf", 1);
    addPrintfLike("fwprintf_s", 1);
    addPrintfLike("sprintf", 1);
    addPrintfLike("snprintf", 2);
    addPrintfLike("snprintf_s", 2);
    addPrintfLike("sprintf_s", 2);
    addPrintfLike("swprintf", 2);
    addPrintfLike("swprintf_s", 2);
    addPrintfLike("snwprintf_s", 2);
}

void ConstraintGenerator::generate(TranslationUnitAST *ast, Scope *global)
{
    if (!ast)
        return;

    global_ = global;

    printDebug("Let's generate constraints!!!\n");
    OBSERVE(TranslationUnitAST);
    switchScope(global_);
    for (DeclarationListAST *it = ast->declaration_list; it; it = it->next)
        visitDeclaration(it->value);
}

void ConstraintGenerator::employDomainLattice(const DomainLattice* lattice)
{
    lattice_ = lattice;
}

void ConstraintGenerator::installObserver(VisitorObserver *observer)
{
    observer_ = observer;
    //observer_->configure(translationUnit(), writer_);
}

void ConstraintGenerator::installInterceptor(DeclarationInterceptor *interceptor)
{
    interceptor_ = interceptor;
}

void ConstraintGenerator::addPrintfLike(const std::string &funcName, size_t varArgPos)
{
    printfs_.insert(std::make_pair(funcName, varArgPos));
}

Scope *ConstraintGenerator::switchScope(Scope *scope)
{
    if (!scope)
        return scope_;
    std::swap(scope_, scope);
    return scope;
}

void ConstraintGenerator::pushType(const std::string &ty)
{
    types_.push(ty);
}

std::string ConstraintGenerator::popType()
{
    std::string ty = types_.top();
    types_.pop();
    return ty;
}

std::string ConstraintGenerator::createUnnamed(const std::string& prefix)
{
    int count = ++unnamedCount_;
    return prefix + std::to_string(count);
}

void ConstraintGenerator::assignTop(const std::string& name)
{
    writer_->beginSection();
    writer_->writeTypeof(name);
    writer_->writeEquivMark();
    ENSURE_NONEMPTY_TYPE_STACK(return);
    writer_->writeTypeSection(types_.top());
    writer_->endSection();
}

void ConstraintGenerator::collectExpression(const std::string &ty,
                                            ExpressionAST *expr)
{
    pushType(ty);
    visitExpression(expr);
    ENSURE_NONEMPTY_TYPE_STACK(return);
    popType();
}

    //--- Declarations

void ConstraintGenerator::visitDeclaration(DeclarationAST *ast)
{
    accept(ast);
}

bool ConstraintGenerator::visit(FunctionDefinitionAST *ast)
{
    if (interceptor_ && interceptor_->intercept(ast))
        return false;

    DEBUG_VISIT(FunctionDefinitionAST);
    OBSERVE(FunctionDefinitionAST);

    Function* func = ast->symbol;
    if (!func->name()->isNameId())
        return false;

    visitSymbol(func, ast->function_body);

    return false;
}

void ConstraintGenerator::visitSymbol(Function *func, StatementAST* body)
{
    std::vector<std::string> paramTyVars;
    for (auto i = 0u; i < func->argumentCount(); i++) {
        std::string alpha = supply_.createTypeVar1();
        writer_->writeExists(alpha);

        Symbol *sym = func->argumentAt(i);
        const std::string& ty = typeSpeller_.spell(sym->type(), scope_);
        writer_->writeTypedef(ty, alpha);
        paramTyVars.push_back(ty);

        writer_->writeEquivRel(alpha, ty);
    }

    // Write the function prototype. When no return type is specified, adopt old-style C
    // rule and assume `int'.
    std::string funcRet;
    if (func->returnType())
        funcRet = typeSpeller_.spell(func->returnType(), scope_);
    else
        funcRet = kDefaultIntTy;

    const std::string& alpha = ensureTypeIsKnown(funcRet);
    const Identifier *id = func->name()->asNameId()->identifier();
    const std::string funcName(id->begin(), id->end());
    writer_->writeFuncDecl(funcName, paramTyVars, funcRet);

    if (func->storage() == Symbol::Static)
        writer_->writeStatic(funcName);

    // The function name and parameter's type names are visible in the outer scope, but parameter
    // themselves are visible within the function body only.
    writer_->openScope();
    if (observer_)
        observer_->withinFunction();

    for (auto i = 0u; i < func->argumentCount(); i++) {
        std::string param;
        if(func->argumentAt(i)
                && func->argumentAt(i)->name()) {
            const Name* paramName = func->argumentAt(i)->name();
            const Identifier* id = paramName->asNameId()->identifier();
            param.assign(id->begin(), id->end());
        } else {
            param = createUnnamed(declPrefix_);
        }
        writer_->writeVarDecl(param, paramTyVars[i]);
    }

    valuedRets_.push(false);
    if (body) {
        pushType(funcRet);
        Scope *prevScope = switchScope(func->asScope());
        visitStatement(body);
        switchScope(prevScope);
        if (valuedRets_.top())
            writer_->writeEquivRel(alpha, types_.top());
        popType();
    }

    if (observer_)
        observer_->outsideFunction();
    writer_->closeScope();

    // If no valued return was detected, this alpha will be orphanized.
    ENSURE_NONEMPTY_ALPHA_RET_STACK(return);
    if (! valuedRets_.top())
        writer_->writeEquivRel(alpha, funcRet);
    valuedRets_.pop();

    // Keep track of function returns.
    knownFuncNames_.insert(std::make_pair(funcName, funcRet));
    auto it = knownFuncRets_.find(funcName);
    if (it != knownFuncRets_.end()) {
        for (const auto& ignored : it->second)
            writer_->writeEquivRel(ignored, funcRet);
    }
}

bool ConstraintGenerator::visit(SimpleDeclarationAST *ast)
{
    if (interceptor_ && interceptor_->intercept(ast))
        return false;

    DEBUG_VISIT(SimpleDeclarationAST);
    OBSERVE(SimpleDeclarationAST);

    for (SpecifierListAST *it = ast->decl_specifier_list; it; it = it->next)
        visitSpecifier(it->value);

    if (!ast->declarator_list)
        return false;

    // For each symbol annotated in the AST, there must exist a corresponding
    // declarator, and we can uniformily iterate over the two lists.
    DeclaratorListAST *declIt = ast->declarator_list;
    for (const List<Symbol*> *symIt = ast->symbols; symIt; symIt = symIt->next) {
        if (symIt->value->asForwardClassDeclaration())
            break;
        PSYCHE_ASSERT(declIt->value, return false, "expected declarator");

        Symbol *decl = symIt->value;

        // Type a function declaration just as we do for a function definition.
        if (decl->asDeclaration()
                && decl->asDeclaration()->type()
                && decl->asDeclaration()->type()->asFunctionType()) {
            visitSymbol(decl->asDeclaration()->type()->asFunctionType()->asFunction(), nullptr);
            return false;
        }

        std::string declName;
        if (decl->name())
            declName = extractId(decl->name());
        else
            declName = createUnnamed(declPrefix_);
        std::string declTy = typeSpeller_.spell(decl->type(), scope_);

        // Altough a `typedef` is parsed as a simple declaration, its contraint
        // rule is different. We process it and break out, since there cannot
        // exist multiple typedefs within one declaration.
        if (decl->storage() == Symbol::Typedef) {
            writer_->writeTypedef(declName, declTy);
            PSYCHE_ASSERT(!symIt->next, return false,
                          "multiple symbols within typedef cannot exist");
            return false;
        }

        const std::string& alpha = ensureTypeIsKnown(declTy);
        writer_->writeEquivRel(alpha, declTy);

        // If an initializer is provided, visit the expression. Unless, if
        // contains braces. If the declaration is of a record, we'll only
        // generate constraints if one of its fields are accessed. If the
        // declaration if of an array, we'll deal with it further down.
        if (declIt->value->initializer
                && !declIt->value->initializer->asBracedInitializer()) {
            const std::string& rhsAlpha = supply_.createTypeVar1();
            writer_->writeExists(rhsAlpha);
            collectExpression(rhsAlpha, declIt->value->initializer);

            const std::string& dummy = supply_.createTypeVar1();
            writer_->writeExists(dummy);
            pushType(dummy);
            employLattice(lattice_->retrieveDomain(decl, scope_), domainOf(declIt->value->initializer),
                          alpha, rhsAlpha, T_EQUAL);
            popType();
        }

        writer_->writeVarDecl(declName, alpha);

        // If this is the declaration of a struct member, we need to generate
        // a containment relationship for it.
        if (!structs_.empty()) {
            const std::string& structTy = structs_.top();
            writer_->writeMemberRel(structTy, declName, alpha);
        }

        // When we have an array with an initializer, we use the individual
        // expressions within the brace to determine the underlying element
        // type of the array. There's an apparent limitation to this approach
        // because C allows one to omit "internal" braces of composite types.
        // A `typedef struct Pair_ { int x; int y} Pair;' can be initialized
        // with any of the following forms:
        //     Pair array1[] = { { 1, 2 }, { 3, 4 } };
        //     Pair array1[] = {   1, 2  ,   3, 4   };
        // Looking at this syntax only we cannot distinguish between a pair
        // type array with two elements or a int type array. We handle this
        // through an pre-generation lattice classificaiton, just like we do
        // for the pointer x integrals conversions issue.
        // The first expression to process in an array declarator is its size
        // specification withing the brakets.
        if (declIt->value->postfix_declarator_list
                && declIt->value->postfix_declarator_list->value->asArrayDeclarator()) {
            auto decltr = declIt->value->postfix_declarator_list;
            do {
                auto size = decltr->value->asArrayDeclarator()->expression;
                if (size)
                    collectExpression(kDefaultIntTy, size);
                decltr = decltr->next;
            } while (decltr && decltr->value->asArrayDeclarator());

            if (declIt->value->initializer
                    && declIt->value->initializer->asBracedInitializer()
                    && declIt->value->initializer->asBracedInitializer()->expression_list) {
                std::string elem = supply_.createTypeVar1();
                writer_->writeExists(elem);

                auto init = declIt->value->initializer->asBracedInitializer()->expression_list;
                if (scope_ == global_ || decl->storage() == Symbol::Static)
                    staticDecl_ = true;
                do {
                    collectExpression(elem, init->value);
                    init = init->next;
                    if (!init)
                        break;
                } while (true);
                staticDecl_ = false; // Just make it false, cannot nest.

                auto decltrs = declIt->value->postfix_declarator_list;
                do {
                    const std::string& ptr = supply_.createTypeVar1();
                    writer_->writeExists(ptr);
                    writer_->writePtrRel(ptr, elem);
                    elem = ptr;
                    decltrs = decltrs->next;
                } while (decltrs && decltrs->value->asArrayDeclarator());
                writer_->writeEquivRel(alpha, elem);
            }
        } else if (declIt->value->initializer
                   && declIt->value->initializer->asBracedInitializer()
                   && declIt->value->initializer->asBracedInitializer()->expression_list) {
            // TODO: When this is an array hidden by a missing typedef.
            pushType(alpha);
            visitExpression(declIt->value->initializer);
            ENSURE_NONEMPTY_TYPE_STACK(return false);
            popType();
        }

        declIt = declIt->next;
    }

    return false;
}

    //--- Names

void ConstraintGenerator::visitName(NameAST *ast)
{
    accept(ast);
}

std::string ConstraintGenerator::ensureTypeIsKnown(std::string& tyName)
{
    std::string alpha = supply_.createTypeVar1();
    writer_->writeExists(alpha);
    writer_->writeTypedef(tyName, alpha);
    return alpha;
}

    //--- Expressions

void ConstraintGenerator::visitExpression(ExpressionAST *ast)
{
    accept(ast);
}

void ConstraintGenerator::employLattice(const DomainLattice::Domain& lhsDom,
                                        const DomainLattice::Domain& rhsDom,
                                        const std::string& lhsTy,
                                        const std::string& rhsTy,
                                        int op)
{
    printDebug("Binary expression  %s x %s\n", lhsDom.name_.c_str(), rhsDom.name_.c_str());

    auto isInconsistent = [&lhsDom, &rhsDom] () {
        return ((lhsDom == DomainLattice::Pointer
                    && (rhsDom == DomainLattice::Integral
                        || rhsDom == DomainLattice::FloatingPoint
                        || rhsDom == DomainLattice::Arithmetic))
                || (rhsDom == DomainLattice::Pointer
                    && (lhsDom == DomainLattice::Integral
                        || lhsDom == DomainLattice::FloatingPoint
                        || lhsDom == DomainLattice::Arithmetic)));
    };

    switch (op) {
    case T_MINUS:
        if (lhsDom == DomainLattice::Pointer && rhsDom == DomainLattice::Pointer) {
            writer_->writeEquivRel(lhsTy, rhsTy);
            writer_->writeEquivRel(types_.top(), kDefaultArithTy);
            break;
        }
        // Fallthrough

    case T_PLUS: {
        auto handlePtr = [this] (const std::string& ptrTy,
                                 const std::string& otherTy,
                                 const DomainLattice::Domain& otherDom) {
            writer_->writeEquivRel(types_.top(), ptrTy);
            if (otherDom == DomainLattice::Scalar) {
                writer_->writeEquivRel(otherTy, kIntTy);
            } else if (otherDom == DomainLattice::Arithmetic
                        || otherDom == DomainLattice::Integral
                        || otherDom == DomainLattice::FloatingPoint) {
                writer_->writeEquivRel(otherTy, otherDom.ty_.empty() ? kIntTy : otherDom.ty_);
            } else {
                // TODO: report error.
            }
        };

        auto handleArith = [this] (const std::string& arithTy,
                                   const std::string& otherTy,
                                   const DomainLattice::Domain& otherDom) {
            if (otherDom == DomainLattice::Arithmetic
                    || otherDom == DomainLattice::Integral
                    || otherDom == DomainLattice::FloatingPoint) {
                writer_->writeEquivRel(arithTy, otherTy);
                writer_->writeEquivRel(types_.top(), arithTy); // TODO: Use side with higher rank.
            } else {
                PSYCHE_ASSERT(otherDom == DomainLattice::Scalar, return, "expected scalar");
                writer_->writeEquivRel(types_.top(), otherTy);
                writer_->writeSubtypeRel(otherTy, kScalarTy);
            }
        };

        if (lhsDom == DomainLattice::Pointer) {
            handlePtr(lhsTy, rhsTy, rhsDom);
        } else if (rhsDom == DomainLattice::Pointer) {
            handlePtr(rhsTy, lhsTy, lhsDom);
        } else if (lhsDom == DomainLattice::Arithmetic
                || lhsDom == DomainLattice::Integral
                || lhsDom == DomainLattice::FloatingPoint) {
            handleArith(lhsTy, rhsTy, rhsDom);
        } else if (rhsDom == DomainLattice::Arithmetic
                || rhsDom == DomainLattice::Integral
                || rhsDom == DomainLattice::FloatingPoint) {
            handleArith(rhsTy, lhsTy, lhsDom);
        } else {
            // Scalars
            writer_->writeEquivRel(types_.top(), lhsTy);
            writer_->writeEquivRel(lhsTy, rhsTy);
            writer_->writeSubtypeRel(lhsTy, kScalarTy);
            writer_->writeSubtypeRel(rhsTy, kScalarTy);
        }
        break;
    }

    // Arithmetic
    case T_STAR:
    case T_SLASH:
    case T_PERCENT:
    case T_AMPER:
    case T_PIPE:
    case T_CARET:
    case T_LESS_LESS:
    case T_GREATER_GREATER:
        writer_->writeEquivRel(lhsTy, rhsTy);
        writer_->writeEquivRel(types_.top(), lhsTy); // TODO: Use side with higher rank.
        writer_->writeSubtypeRel(types_.top(), kIntTy);
        break;

    // Comparison
    case T_LESS:
    case T_LESS_EQUAL:
    case T_GREATER:
    case T_GREATER_EQUAL:
    case T_EQUAL_EQUAL:
    case T_EXCLAIM_EQUAL:
        writer_->writeEquivRel(lhsTy, rhsTy);
        writer_->writeEquivRel(types_.top(), kIntTy);
        if (lhsDom == DomainLattice::Scalar && rhsDom == DomainLattice::Scalar) {
            writer_->writeEquivRel(lhsTy, kScalarTy);
            writer_->writeEquivRel(rhsTy, kScalarTy);
        }
        break;

    // Logical
    case T_AMPER_AMPER:
    case T_PIPE_PIPE:
        if (!isInconsistent()) {
            writer_->writeEquivRel(types_.top(), kIntTy);
            if (lhsDom == DomainLattice::Scalar)
                writer_->writeSubtypeRel(lhsTy, kScalarTy);
            if (rhsDom == DomainLattice::Scalar)
                writer_->writeSubtypeRel(rhsTy, kScalarTy);
        }
        break;

    case T_EQUAL:
        if (!isInconsistent()) {
            writer_->writeSubtypeRel(lhsTy, rhsTy);
            writer_->writeEquivRel(types_.top(), lhsTy);
            if (lhsDom == DomainLattice::Scalar && rhsDom == DomainLattice::Scalar) {
                writer_->writeSubtypeRel(lhsTy, kScalarTy);
                writer_->writeSubtypeRel(rhsTy, kScalarTy);
            }
        }
        break;

    case T_PLUS_EQUAL:
    case T_MINUS_EQUAL:
        if (!isInconsistent()) {
            if (lhsDom == DomainLattice::Pointer) {
                writer_->writeEquivRel(types_.top(), lhsTy);
            } else if (rhsDom == DomainLattice::Pointer) {
                writer_->writeEquivRel(types_.top(), rhsTy);
            } else if (lhsDom == DomainLattice::Scalar) {
                writer_->writeEquivRel(types_.top(), lhsTy);
                writer_->writeSubtypeRel(lhsTy, kScalarTy);
                if (rhsDom == DomainLattice::Scalar)
                    writer_->writeEquivRel(lhsTy, rhsTy);
            } else if (rhsDom == DomainLattice::Scalar) {
                writer_->writeEquivRel(types_.top(), rhsTy);
                writer_->writeSubtypeRel(rhsTy, kScalarTy);
                if (lhsDom == DomainLattice::Scalar)
                    writer_->writeEquivRel(lhsTy, rhsTy);
            } else {
                writer_->writeEquivRel(types_.top(), lhsTy); // TODO: Use the highest rank.
                writer_->writeEquivRel(lhsTy, rhsTy);
            }
        }
        break;

    case T_STAR_EQUAL:
    case T_SLASH_EQUAL:
    case T_PERCENT_EQUAL:
    case T_AMPER_EQUAL:
    case T_PIPE_EQUAL:
    case T_CARET_EQUAL:
    case T_LESS_LESS_EQUAL:
    case T_GREATER_GREATER_EQUAL:
        writer_->writeEquivRel(lhsTy, rhsTy);
        if (lhsDom > DomainLattice::Arithmetic)
            writer_->writeEquivRel(types_.top(), lhsTy);
        else if (rhsDom > DomainLattice::Arithmetic)
            writer_->writeEquivRel(types_.top(), rhsTy);
        else
            writer_->writeEquivRel(types_.top(), kIntTy);
        break;;

    default:
        PSYCHE_ASSERT(false, return, "unrecognized operator");
        break;
    }
}

DomainLattice::Domain ConstraintGenerator::domainOf(ExpressionAST *ast) const
{
    DomainLattice::Domain dom = DomainLattice::Undefined;
    if (lattice_) {
        dom = lattice_->retrieveDomain(ast, scope_);
        if (dom != DomainLattice::Undefined) {
            const auto& s = lattice_->fetchText(ast);
            printDebug("Retrieved AST %s as %s\n", s.c_str(), dom.name_.c_str());
        }
    }

    if (dom == DomainLattice::Undefined) {
        TypeOfExpression typeofExpr(translationUnit());
        FullySpecifiedType ty = typeofExpr.resolve(ast, scope_);
        dom = DomainLattice::domainForType(ty);
        // TODO: Fetch text utility.
        const auto& s = lattice_->fetchText(ast);
        printDebug("Typed AST %s as %s\n", s.c_str(), dom.name_.c_str());
    }

    return dom;
}

bool ConstraintGenerator::visit(ArrayAccessAST *ast)
{
    DEBUG_VISIT(ArrayAccessAST);
    OBSERVE(ArrayAccessAST);

    std::tuple<std::string, std::string, std::string> a1a2a3 = supply_.createTypeVar3();
    writer_->writeExists(std::get<0>(a1a2a3));
    writer_->writeExists(std::get<1>(a1a2a3));
    writer_->writeExists(std::get<2>(a1a2a3));
    collectExpression(std::get<0>(a1a2a3), ast->base_expression);
    collectExpression(std::get<1>(a1a2a3), ast->expression);

    writer_->writePtrRel(std::get<0>(a1a2a3), std::get<2>(a1a2a3));
    ENSURE_NONEMPTY_TYPE_STACK(return false);
    writer_->writeEquivRel(types_.top(), std::get<2>(a1a2a3));

    writer_->writeEquivRel(std::get<1>(a1a2a3), kSizeTy);

    return false;
}

bool ConstraintGenerator::visit(BinaryExpressionAST *ast)
{
    DEBUG_VISIT(BinaryExpressionAST);
    OBSERVE(BinaryExpressionAST);

    ExpressionAST* lexpr = ast->left_expression;
    ExpressionAST* rexpr = ast->right_expression;

    // A comma expression is just a  sequence of unrelated expressions.
    unsigned op = tokenKind(ast->binary_op_token);
    if (op == T_COMMA) {
        visitExpression(lexpr);

        const std::string& alpha = supply_.createTypeVar1();
        writer_->writeExists(alpha);
        collectExpression(alpha, rexpr);
        return false;
    }

    // With NULL, we don't know the underlying element type of the pointer, but we can
    // constrain the left-hand-side as a pointer.
    if (rexpr->asPointerLiteral()) {
        std::tuple<std::string, std::string> a1a2 = supply_.createTypeVar2();
        writer_->writeExists(std::get<0>(a1a2));
        writer_->writeExists(std::get<1>(a1a2));
        collectExpression(std::get<0>(a1a2), lexpr);
        writer_->writePtrRel(std::get<0>(a1a2), std::get<1>(a1a2));
        return false;
    }

    std::tuple<std::string, std::string> a1a2 = supply_.createTypeVar2();
    writer_->writeExists(std::get<0>(a1a2));
    writer_->writeExists(std::get<1>(a1a2));
    collectExpression(std::get<0>(a1a2), lexpr);
    collectExpression(std::get<1>(a1a2), rexpr);

    employLattice(domainOf(lexpr), domainOf(rexpr), std::get<0>(a1a2), std::get<1>(a1a2), op);

    return false;
}

namespace {

const std::string trivialName(IdExpressionAST* idExpr)
{
    PSYCHE_ASSERT(idExpr
                  && idExpr->name->name->asNameId()
                  && idExpr->name->name->asNameId()->identifier(),
                  return std::string(),
                  "expected a trivial name");

    const Identifier *id = idExpr->name->name->asNameId()->identifier();
    return std::string(id->chars(), id->size());
}

} // anonymous

bool ConstraintGenerator::visit(CallAST *ast)
{
    DEBUG_VISIT(CallAST);
    OBSERVE(CallAST);

    std::string funcName;
    int varArgPos = -1;
    if (ast->base_expression->asIdExpression()) {
        funcName = trivialName(ast->base_expression->asIdExpression());

        // Detect whether the function is registered as a printf-style one.
        auto fit = printfs_.find(funcName);
        if (fit != printfs_.end())
            varArgPos = fit->second;
    } else {
        const std::string& funcVar = supply_.createTypeVar1();
        writer_->writeExists(funcVar);
        collectExpression(funcVar, ast->base_expression);
        funcName = createUnnamed(stubPrefix_);
        writer_->beginSection();
        writer_->writeTypeof(funcName);
        writer_->writeEquivMark();
        writer_->writeTypeSection(funcVar);
        writer_->endSection();
    }

    // Deal with "regular" functions, for which we generate constraints through
    // the normal expression inspection process.
    std::vector<std::string> typeVars;
    for (ExpressionListAST* it = ast->expression_list; it; it = it->next) {
        const std::string& typeVar = supply_.createTypeVar1();
        writer_->writeExists(typeVar);
        collectExpression(typeVar, it->value);
        typeVars.push_back(typeVar);
    }
    ENSURE_NONEMPTY_TYPE_STACK(return false);
    typeVars.push_back(types_.top());

    writer_->beginSection();
    writer_->writeTypeof(funcName);
    writer_->writeEquivMark();
    writer_->enterGroup();
    writer_->writeTypesSection(typeVars);
    writer_->leaveGroup();
    writer_->endSection();


    if (varArgPos == -1)
        return false;

    // Deal with printf family of functions, for which we generate constraints
    // based on format specifiers.
    int argCnt = 0;
    std::vector<PrintfScanner::FormatSpec> specs;
    for (ExpressionListAST* it = ast->expression_list; it; it = it->next, ++argCnt) {
        if (argCnt < varArgPos) {
            const std::string& typeVar = supply_.createTypeVar1();
            writer_->writeExists(typeVar);
            collectExpression(typeVar, it->value);
            continue;
        }

        if (argCnt == varArgPos) {
            // We expect the formatting string as a literal expression. But in
            // order to be more robust we also accept a literal wrapped in
            // parenthesis or within a call expression in which the base is an
            // identifier expression. The reason for this approach is because
            // many projects definte "translation macros" for string literals.
            // But those function-like macros are possibly unexpanded in the
            // code we analyse, so our parser will parse them as regular functions.
            ExpressionAST* litExpr = it->value;
            while (litExpr->asNestedExpression())
                litExpr = litExpr->asNestedExpression()->expression;
            if (litExpr->asCall() && litExpr->asCall()->expression_list)
                litExpr = litExpr->asCall()->expression_list->value;
            if (litExpr->asStringLiteral()) {
                specs.resize(argCnt);
                specs.push_back(PrintfScanner::String);

                do {
                    const StringLiteral* lit =
                        stringLiteral(litExpr->asStringLiteral()->literal_token);
                    std::string format(lit->begin(), lit->end());
                    PrintfScanner scanner(format);
                    // Although we already know the format is a string, we still
                    // add it to the evaluation for the case in which there are
                    // no type specifiers in it - we need a constraint out of this
                    // due to workflow reasons.
                    for (auto spec : scanner.evaluate())
                        specs.push_back(spec);

                    // Our lexer doesn't concatenate adjacent string literal
                    // tokens, they are instead stored in the literal itself.
                    litExpr = litExpr->asStringLiteral()->next;
                } while (litExpr);
            } else {
                // When the format is not a string literal, there's not much
                // we can do, except an string equivalence on the expression.
                collectExpression(kDefaultStrTy, it->value);
                return false;
            }
        }

        if (argCnt >= varArgPos) {
            PSYCHE_ASSERT(it->value && argCnt < static_cast<int>(specs.size()), return false,
                          "expected matching number of exprs/specs");
            switch (specs[argCnt]) {
            case PrintfScanner::Char:
            case PrintfScanner::Int:
                collectExpression(kDefaultIntTy, it->value);
                break;

            case PrintfScanner::FloatingPoint:
                collectExpression(kDefaultFloatPointTy, it->value);
                break;

            case PrintfScanner::String:
                collectExpression(kDefaultStrTy, it->value);
                break;

            case PrintfScanner::Pointer:
                // We don't know the underlying pointer type, nothing interesting to do.
                break;

            default:
                PSYCHE_ASSERT(false, return false, "unknown type spec");
                break;
            }
        }
    }

    return false;
}

void ConstraintGenerator::castExpressionHelper(const std::string& inputTy,
                                               std::string& resultTy)
{
    if (resultTy != kCharTy
            && resultTy != kShortTy
            && resultTy != kIntTy
            && resultTy != kLongTy
            && resultTy != kLongLongTy
            && resultTy != kFloatTy
            && resultTy != kDoubleTy
            && resultTy != kLongDoubleTy
            && resultTy != kVoidTy
            && resultTy != kDefaultStrTy) {
        ensureTypeIsKnown(resultTy);
    }
    ENSURE_NONEMPTY_TYPE_STACK(return );
    writer_->writeEquivRel(inputTy, resultTy);
}

bool ConstraintGenerator::visit(CastExpressionAST *ast)
{
    DEBUG_VISIT(CastExpressionAST);
    OBSERVE(CastExpressionAST);

    std::string ty = typeSpeller_.spell(ast->expression_type, scope_);
    castExpressionHelper(types_.top(), ty);

    const std::string& alpha = supply_.createTypeVar1();
    writer_->writeExists(alpha);
    collectExpression(alpha, ast->expression);

    auto targetDom = domainOf(ast->type_id);

    if (targetDom != DomainLattice::Undefined)
        writer_->writeSubtypeRel(alpha, kScalarTy);

    return false;
}

void ConstraintGenerator::treatAsBool(ExpressionAST *ast)
{
    if (!ast)
        return;

    std::string ty;
    auto dom = domainOf(ast);
    if (!dom.ty_.empty()) {
        ty = dom.ty_;
    } else {
        if (dom == DomainLattice::Arithmetic) {
            ty = kDefaultArithTy;
        } else if (dom == DomainLattice::Integral) {
            ty = kDefaultIntTy;
        } else if (dom == DomainLattice::FloatingPoint) {
            ty = kDefaultFloatPointTy;
        } else {
            ty = supply_.createTypeVar1();
            writer_->writeExists(ty);
            if (dom == DomainLattice::Scalar)
                writer_->writeSubtypeRel(ty, kScalarTy);
        }
    }

    collectExpression(ty, ast);
}

bool ConstraintGenerator::visit(ConditionalExpressionAST *ast)
{
    DEBUG_VISIT(ConditionalExpressionAST);
    OBSERVE(ConditionalExpressionAST);

    treatAsBool(ast->condition);
    visitExpression(ast->left_expression);
    visitExpression(ast->right_expression);

    return false;
}

bool ConstraintGenerator::visit(IdExpressionAST *ast)
{
    DEBUG_VISIT(IdExpressionAST);
    OBSERVE(IdExpressionAST);

    assignTop(extractId(ast->name->name));

    // Static initialization requires a compile-time constant.
    if (staticDecl_)
        writer_->writeConstantExpression(extractId(ast->name->name));

    return false;
}

bool ConstraintGenerator::visit(NumericLiteralAST *ast)
{
    DEBUG_VISIT(NumericLiteralAST);
    OBSERVE(NumericLiteralAST);

    const NumericLiteral *numLit = numericLiteral(ast->literal_token);
    PSYCHE_ASSERT(numLit, return false, "numeric literal must exist");

    std::string ty;
    if (numLit->isUnsigned())
        ty.append(" unsigned ");

    writer_->beginSection();
    if (numLit->isDouble()) {
        ty.append(kDoubleTy);
    } else if (numLit->isFloat()) {
        ty.append(kFloatTy);
    } else if (numLit->isLongDouble()) {
        ty.append(kLongDoubleTy);
    } else {
        if (!strcmp(numLit->chars(), "0")) {
            const std::string& alpha = supply_.createTypeVar1();
            writer_->writeExists(alpha);
            ty.append(alpha);
        } else if (tokenKind(ast->literal_token) == T_CHAR_LITERAL) {
            // TODO: char/int
            ty.append(kCharTy);
        } else if (numLit->isLong()) {
            ty.append(kLongTy);
        } else if (numLit->isLongLong()) {
            ty.append(kLongLongTy);
        } else {
            ty.append(kDefaultIntTy);
        }
    }
    writer_->writeTypeSection(ty);
    writer_->writeEquivMark();
    ENSURE_NONEMPTY_TYPE_STACK(return false);
    writer_->writeTypeSection(types_.top());
    writer_->endSection();

    return false;
}

bool ConstraintGenerator::visit(BoolLiteralAST *ast)
{
    DEBUG_VISIT(BoolLiteralAST);
    OBSERVE(BoolLiteralAST);

    // Treated as integer. It's relatively common to have C code defining `true`
    // and `false` through macros. Their meaning is obvious in such cases.
    ENSURE_NONEMPTY_TYPE_STACK(return false);
    writer_->writeEquivRel(kDefaultIntTy, types_.top());

    return false;
}

bool ConstraintGenerator::visit(StringLiteralAST *ast)
{
    DEBUG_VISIT(StringLiteralAST);
    OBSERVE(StringLiteralAST);

    ENSURE_NONEMPTY_TYPE_STACK(return false);
    writer_->writeEquivRel(kDefaultStrTy, types_.top());

    return false;
}

bool ConstraintGenerator::visit(MemberAccessAST *ast)
{
    DEBUG_VISIT(MemberAccessAST);
    OBSERVE(MemberAccessAST);

    std::tuple<std::string, std::string> a1a2 = supply_.createTypeVar2();
    writer_->writeExists(std::get<0>(a1a2));
    writer_->writeExists(std::get<1>(a1a2));

    std::string alpha3;
    unsigned accessTk = tokenKind(ast->access_token);
    if (accessTk == T_ARROW)  {
        alpha3 = supply_.createTypeVar1();
        writer_->writeExists(alpha3);
    }

    collectExpression(std::get<0>(a1a2), ast->base_expression);

    // For a pointer access we need to insert an additional constraint.
    if (accessTk == T_ARROW)
        writer_->writePtrRel(std::get<0>(a1a2), std::get<1>(a1a2));

    std::string sym = extractId(ast->member_name->name);
    if (accessTk == T_ARROW)
        writer_->writeMemberRel(std::get<1>(a1a2), sym, alpha3);
    else
        writer_->writeMemberRel(std::get<0>(a1a2), sym, std::get<1>(a1a2));

    ENSURE_NONEMPTY_TYPE_STACK(return false);
    if (accessTk == T_ARROW)
        writer_->writeEquivRel(types_.top(), alpha3);
    else
        writer_->writeEquivRel(types_.top(), std::get<1>(a1a2));

    return false;
}

bool ConstraintGenerator::visit(BracedInitializerAST *ast)
{
    DEBUG_VISIT(BracedInitializerAST);
    OBSERVE(BracedInitializerAST);

    const std::string& alpha = supply_.createTypeVar1();
    writer_->writeExists(alpha);

    auto cnt = 0;
    for (auto it = ast->expression_list; it; it = it->next, ++cnt) {
        std::string alphaField = supply_.createTypeVar1();
        writer_->writeExists(alphaField);
        collectExpression(alphaField, it->value);

        if (it->value->asDesignatedInitializer()) {
            auto init = it->value->asDesignatedInitializer();

            // Traverse the designators list from right to left, creating, for each name,
            // a corresponding type. The first type comes from the intializer expression.
            std::vector<DesignatorAST*> designators;
            for (DesignatorListAST* it2 = init->designator_list; it2; it2 = it2->next)
                designators.push_back(it2->value);
            std::reverse(designators.begin(), designators.end());

            std::string alphaDesig;
            for (auto i = 0u; i < designators.size(); ++i) {
                auto design = designators[i];
                if (design->asDotDesignator()) {
                    alphaDesig = supply_.createTypeVar1();
                    writer_->writeExists(alphaDesig);

                    const Identifier* field =
                        identifier(design->asDotDesignator()->identifier_token);
                    writer_->writeMemberRel(alphaDesig, std::string(field->chars(),
                                                                    field->size()), alphaField);
                    alphaField = alphaDesig;
                } else {
                    // TODO: Arrays.
                }
            }
            writer_->writeEquivRel(alphaDesig, alpha);
        } else {
            writer_->writeMemberRel(alpha, "member_" + std::to_string(cnt), alphaField);
        }
    }

    ENSURE_NONEMPTY_TYPE_STACK(return false);
    writer_->writeEquivRel(types_.top(), alpha);

    return false;
}

bool ConstraintGenerator::visit(PostIncrDecrAST *ast)
{
    DEBUG_VISIT(PostIncrDecrAST);
    OBSERVE(PostIncrDecrAST);

    const std::string& alpha = supply_.createTypeVar1();
    writer_->writeExists(alpha);
    collectExpression(alpha, ast->base_expression);

    return false;
}

bool ConstraintGenerator::visit(UnaryExpressionAST* ast)
{
    DEBUG_VISIT(UnaryExpressionAST);
    OBSERVE(UnaryExpressionAST);

    switch(tokenKind(ast->unary_op_token)) {
    case T_AMPER: {
        std::tuple<std::string, std::string> a1a2 = supply_.createTypeVar2();
        writer_->writeExists(std::get<0>(a1a2));
        writer_->writeExists(std::get<1>(a1a2));
        collectExpression(std::get<1>(a1a2), ast->expression);

        writer_->writePtrRel(std::get<0>(a1a2), std::get<1>(a1a2));

        ENSURE_NONEMPTY_TYPE_STACK(return false);
        writer_->writeEquivRel(types_.top(), std::get<0>(a1a2));
        break;
    }

    case T_STAR: {
        const std::string& alpha = supply_.createTypeVar1();
        writer_->writeExists(alpha);
        collectExpression(alpha, ast->expression);

        ENSURE_NONEMPTY_TYPE_STACK(return false);
        writer_->writePtrRel(alpha, types_.top());
        break;
    }

    case T_EXCLAIM: {
        const std::string& alpha = supply_.createTypeVar1();
        writer_->writeExists(alpha);
        collectExpression(alpha, ast->expression);

        ENSURE_NONEMPTY_TYPE_STACK(return false);
        writer_->writeEquivRel(types_.top(), kDefaultIntTy);
        break;
    }

    default:
        // Let's visit the base expression.
        visitExpression(ast->expression);
        break;
    }

    return false;
}

bool ConstraintGenerator::visit(SizeofExpressionAST *ast)
{
    DEBUG_VISIT(SizeofExpressionAST);
    OBSERVE(SizeofExpressionAST);

    // When sizeof's argument is a type, we need to make sure it exists.
    if (ast->expression->asTypeId()) {
        const std::string& alpha = supply_.createTypeVar1();
        writer_->writeExists(alpha);
        const std::string& ty = typeSpeller_.spell(ast->expression_type, scope_);
        writer_->writeTypedef(ty, alpha);
    }

    // TODO: Make sizeof and related type as size_t.
    ENSURE_NONEMPTY_TYPE_STACK(return false);
    writer_->writeEquivRel(types_.top(), kDefaultIntTy);

    return false;
}

bool ConstraintGenerator::visit(psyche::PointerLiteralAST* ast)
{
    DEBUG_VISIT(PointerLiteralAST);

    // We don't know the underlying element type, but we know it's a pointer.
    const std::string& alpha = supply_.createTypeVar1();
    writer_->writeExists(alpha);
    ENSURE_NONEMPTY_TYPE_STACK(return false);
    writer_->writePtrRel(types_.top(), alpha);

    return false;
}

    //--- Specifiers

void ConstraintGenerator::visitSpecifier(SpecifierAST *ast)
{
    accept(ast);
}

bool ConstraintGenerator::visit(EnumSpecifierAST *ast)
{
    DEBUG_VISIT(EnumSpecifierAST);
    OBSERVE(EnumSpecifierAST);

    for (SpecifierListAST *it = ast->type_specifier_list; it; it = it->next)
        visitSpecifier(it->value);

    Scope *prevScope = switchScope(ast->symbol);
    for (EnumeratorListAST *it = ast->enumerator_list; it; it = it->next)
        accept(it->value);
    switchScope(prevScope);

    return false;
}

bool ConstraintGenerator::visit(ClassSpecifierAST* ast)
{
    DEBUG_VISIT(ClassSpecifierAST);
    OBSERVE(ClassSpecifierAST);

    std::string classTy = typeSpeller_.spell(ast->symbol->type(), scope_);
    std::string tyName;
    if (ast->name->name->asAnonymousNameId()) {
        tyName = classTy;
    } else {
        std::string head = "struct ";
        if (tokenKind(ast->classkey_token) == T_UNION)
            head = "union ";
        tyName = head + extractId(ast->name->name);
    }

    writer_->writeTypedef(tyName, classTy);

    const std::string& alpha = supply_.createTypeVar1();
    writer_->writeExists(alpha);
    writer_->writeEquivRel(alpha, tyName);

    structs_.push(alpha);
    Scope *prevScope = switchScope(ast->symbol);
    for (DeclarationListAST *it = ast->member_specifier_list; it; it = it->next)
        visitDeclaration(it->value);
    switchScope(prevScope);
    structs_.pop();

    return false;
}

bool ConstraintGenerator::visit(GnuAttributeSpecifierAST *ast)
{
    return false;
}

    //--- Statements

void ConstraintGenerator::visitStatement(StatementAST *ast)
{
    accept(ast);
}

bool ConstraintGenerator::visit(CompoundStatementAST *ast)
{
    DEBUG_VISIT(CompoundStatementAST);
    OBSERVE(CompoundStatementAST);

    writer_->openScope();
    Scope *prevScope = switchScope(ast->symbol);
    for (StatementListAST *it = ast->statement_list; it; it = it->next)
        visitStatement(it->value);
    switchScope(prevScope);
    writer_->closeScope();

    return false;
}

bool ConstraintGenerator::visit(DeclarationStatementAST *ast)
{
    DEBUG_VISIT(DeclarationStatementAST);
    OBSERVE(DeclarationStatementAST);

    visitDeclaration(ast->declaration);

    return false;
}

bool ConstraintGenerator::visit(ExpressionStatementAST *ast)
{
    DEBUG_VISIT(ExpressionStatementAST);
    OBSERVE(ExpressionStatementAST);

    if (!ast->expression)
        return false;

    // Deal with assignment.
    if (ast->expression->asBinaryExpression()) {
        const std::string& alpha = supply_.createTypeVar1();
        writer_->writeExists(alpha);
        collectExpression(alpha, ast->expression);
        return false;
    }

    // Deal with a pointer dereference.
    if (ast->expression->asUnaryExpression()
            && (tokenKind(ast->expression->asUnaryExpression()->unary_op_token)
            == T_STAR)) {
        std::tuple<std::string, std::string> a1a2 = supply_.createTypeVar2();
        writer_->writeExists(std::get<0>(a1a2));
        writer_->writeExists(std::get<1>(a1a2));
        collectExpression(std::get<0>(a1a2), ast->expression->asUnaryExpression()->expression);
        writer_->writePtrRel(std::get<0>(a1a2), std::get<1>(a1a2));
        return false;
    }

    const std::string& alpha = supply_.createTypeVar1();
    writer_->writeExists(alpha);
    collectExpression(alpha, ast->expression);

    // In the case of a function call, we try to find out what's the actual
    // return type because, even though the function declaration may be absent,
    // the function definition may be present in later in the code. In such
    // case we use the return type for functions we know about.
    if (ast->expression->asCall()
            && ast->expression->asCall()->base_expression->asIdExpression()) {
        const std::string& funcName =
                trivialName(ast->expression->asCall()->base_expression->asIdExpression());
        const auto it = knownFuncNames_.find(funcName);
        if (it != knownFuncNames_.end())
            writer_->writeEquivRel(alpha, it->second);
        else
            knownFuncRets_[funcName].push_back(alpha);
    }

    return false;
}

bool ConstraintGenerator::visit(IfStatementAST *ast)
{
    DEBUG_VISIT(IfStatementAST);
    OBSERVE(IfStatementAST);

    Scope* prevScope = switchScope(ast->symbol);
    treatAsBool(ast->condition);
    visitStatement(ast->statement);
    visitStatement(ast->else_statement);
    switchScope(prevScope);

    return false;
}

bool ConstraintGenerator::visit(ReturnStatementAST *ast)
{
    DEBUG_VISIT(ReturnStatementAST);
    OBSERVE(ReturnStatementAST);

    if (ast->expression) {
        visitExpression(ast->expression);

        ENSURE_NONEMPTY_ALPHA_RET_STACK(return false);
        valuedRets_.top() = true;
    }

    return false;
}

bool ConstraintGenerator::visit(EnumeratorAST *ast)
{
    DEBUG_VISIT(EnumeratorAST);
    OBSERVE(EnumeratorAST);

    const Identifier* ident = identifier(ast->identifier_token);
    writer_->writeVarDecl(ident->chars(), kIntTy);

    if (ast->expression) {
        collectExpression(kIntTy, ast->expression);

        // TODO: Extend...
        if (ast->expression->asIdExpression())
            writer_->writeConstantExpression(extractId(ast->expression->asIdExpression()->name->name));
    }

    return false;
}

bool ConstraintGenerator::visit(SwitchStatementAST *ast)
{
    DEBUG_VISIT(SwitchStatementAST);
    OBSERVE(SwitchStatementAST);

    treatAsBool(ast->condition);
    visitStatement(ast->statement);

    return false;
}

bool ConstraintGenerator::visit(CaseStatementAST *ast)
{
    DEBUG_VISIT(CaseStatementAST);
    OBSERVE(CaseStatementAST);

    collectExpression(kDefaultIntTy, ast->expression);

    // TODO: Extend...
    if (ast->expression->asIdExpression())
        writer_->writeConstantExpression(extractId(ast->expression->asIdExpression()->name->name));

    visitStatement(ast->statement);

    return false;
}

bool ConstraintGenerator::visit(DoStatementAST *ast)
{
    DEBUG_VISIT(DoStatementAST);
    OBSERVE(DoStatementAST);

    treatAsBool(ast->expression);
    visitStatement(ast->statement);

    return false;
}

bool ConstraintGenerator::visit(WhileStatementAST *ast)
{
    DEBUG_VISIT(WhileStatementAST);
    OBSERVE(WhileStatementAST);

    Scope* prevScope = switchScope(ast->symbol);
    treatAsBool(ast->condition);
    visitStatement(ast->statement);
    switchScope(prevScope);

    return false;
}

bool ConstraintGenerator::visit(ForStatementAST *ast)
{
    DEBUG_VISIT(ForStatementAST);
    OBSERVE(ForStatementAST);

    Scope* prevScope = switchScope(ast->symbol);
    if (ast->initializer && ast->initializer->asDeclarationStatement())
        visitDeclaration(ast->initializer->asDeclarationStatement()->declaration);
    else
        visitStatement(ast->initializer);

    treatAsBool(ast->condition);

    if (ast->expression) {
        const std::string& alpha = supply_.createTypeVar1();
        writer_->writeExists(alpha);
        collectExpression(alpha, ast->expression);
    }

    visitStatement(ast->statement);
    switchScope(prevScope);

    return false;
}
