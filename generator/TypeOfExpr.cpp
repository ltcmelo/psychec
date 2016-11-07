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

#include "TypeOfExpr.h"
#include "AST.h"
#include "Assert.h"
#include "CoreTypes.h"
#include "Control.h"
#include "Debug.h"
#include "Literals.h"
#include "Lookup.h"
#include "Scope.h"
#include "Symbol.h"
#include "Symbols.h"
#include "TranslationUnit.h"
#include "Utils.h"

using namespace CPlusPlus;
using namespace psyche;

TypeOfExpr::TypeOfExpr(TranslationUnit *unit)
    : ASTVisitor(unit)
    , singleLookup_(0)
{}

FullySpecifiedType TypeOfExpr::resolve(ExpressionAST *ast, Scope *scope)
{
    scope_ = scope;
    accept(ast);
    if (!fullType_.empty())
        return fullType_.back();
    return FullySpecifiedType();
}

Scope *TypeOfExpr::switchScope(Scope *scope)
{
    if (!scope)
        return scope_;
    std::swap(scope_, scope);
    return scope;
}

bool TypeOfExpr::visit(ArrayAccessAST *ast)
{
    accept(ast->base_expression);

    FullySpecifiedType elemTy;
    if (fullType_.back()->isArrayType())
        elemTy = fullType_.back()->asArrayType()->elementType();
    else if (fullType_.back()->isPointerType())
        elemTy = fullType_.back()->asPointerType()->elementType();
    fullType_.push_back(std::move(elemTy));

    return false;
}

bool TypeOfExpr::visit(BinaryExpressionAST *ast)
{
    // TODO: This is not exactly correct, we need to work here. But for now
    // let's just use the type of the LHS.
    accept(ast->left_expression);
    return false;
}

bool TypeOfExpr::visit(CallAST *ast)
{
    accept(ast->base_expression);
    return false;
}

bool TypeOfExpr::visit(CastExpressionAST *ast)
{
    fullType_.push_back(ast->expression_type);
    return false;
}

bool TypeOfExpr::visit(ConditionalExpressionAST *ast)
{
    accept(ast->right_expression);
    return false;
}
void TypeOfExpr::process(const Identifier *id)
{
    const Symbol* valSym;
    if (singleLookup_)
        valSym = scope_->find(id);
    else
        valSym = lookupValueSymbol(id, scope_);

    if (!valSym) {
        fullType_.push_back(FullySpecifiedType());
        return;
    }

    FullySpecifiedType ty = valSym->type();
    if (!ty.isValid())
        return;

    fullType_.push_back(ty);
    while (ty.type()->isPointerType())
        ty = ty.type()->asPointerType()->elementType();

    if (ty.type()->isNamedType()) {
        id = ty.type()->asNamedType()->name()->identifier();
    } else if (ty.type()->isClassType()) {
        id = ty.type()->asClassType()->name()->identifier();
    } else {
        return;
    }

    Symbol* tySym = lookupTypeSymbol(id, scope_);
    if (tySym && tySym->type()->asClassType())
        scope_ = tySym->type()->asClassType();
}

bool TypeOfExpr::visit(IdExpressionAST *ast)
{
    PSYCHE_ASSERT(ast->asIdExpression()->name->name->asNameId(),
                  return false,
                  "expected trivial identifier name");
    process(ast->name->name->identifier());
    return false;
}

bool TypeOfExpr::visit(MemberAccessAST *ast)
{
    PSYCHE_ASSERT(ast->member_name->name->asNameId(),
                  return false,
                  "expected trivial member name");

    accept(ast->base_expression);
    ++singleLookup_;
    process(ast->member_name->name->identifier());
    PSYCHE_ASSERT(singleLookup_ > 0, return false, "expected member check");
    --singleLookup_;

    return false;
}

bool TypeOfExpr::visit(NumericLiteralAST *ast)
{
    const NumericLiteral *numLit =
            numericLiteral(ast->asNumericLiteral()->literal_token);
    PSYCHE_ASSERT(numLit, return false, "numeric literal must exist");
    if (numLit->isDouble()) {
        fullType_.push_back(control()->floatType(FloatType::Double));
    } else if (numLit->isLongDouble()) {
        fullType_.push_back(control()->floatType(FloatType::LongDouble));
    } else if (numLit->isFloat()) {
        fullType_.push_back(control()->floatType(FloatType::Float));
    } else {
        fullType_.push_back(control()->integerType(IntegerType::Int));
    }

    return false;
}

bool TypeOfExpr::visit(BoolLiteralAST *ast)
{
    fullType_.push_back(control()->integerType(IntegerType::Int));
    return false;
}

bool TypeOfExpr::visit(StringLiteralAST *ast)
{
    FullySpecifiedType baseType = control()->integerType(IntegerType::Char);
    fullType_.push_back(control()->pointerType(baseType));
    return false;
}

bool TypeOfExpr::visit(UnaryExpressionAST *ast)
{
    accept(ast->expression);

    switch (tokenKind(ast->unary_op_token)) {
    case T_AMPER:
        fullType_.push_back(control()->pointerType(fullType_.back()));
        return false;

    case T_STAR: {
        FullySpecifiedType elemTy;
        if (fullType_.back()->isArrayType())
            elemTy = fullType_.back()->asArrayType()->elementType();
        else if (fullType_.back()->isPointerType())
            elemTy = fullType_.back()->asPointerType()->elementType();
        fullType_.push_back(std::move(elemTy));
        return false;
    }

    default:
        return false;
    }
}

bool TypeOfExpr::visit(SizeofExpressionAST *ast)
{
    fullType_.push_back(control()->integerType(IntegerType::Int));
    return false;
}

bool TypeOfExpr::visit(PointerLiteralAST *ast)
{
    fullType_.push_back(control()->pointerType(FullySpecifiedType()));
    return false;
}
