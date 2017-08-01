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

#include "TypeOfExpression.h"
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

TypeOfExpression::TypeOfExpression(TranslationUnit *unit)
    : ASTVisitor(unit)
    , singleLookup_(0)
{}

FullySpecifiedType TypeOfExpression::resolve(ExpressionAST *ast, Scope *scope)
{
    scope_ = scope;
    accept(ast);
    if (!fullType_.empty())
        return fullType_.back();
    return FullySpecifiedType();
}

FullySpecifiedType TypeOfExpression::commonRealType(const FullySpecifiedType& lhsTy,
                                              const FullySpecifiedType& rhsTy) const
{
    PSYCHE_ASSERT(lhsTy && Type::isArithmetic(lhsTy.type())
                        && rhsTy && Type::isArithmetic(rhsTy.type()),
                  return FullySpecifiedType(),
                  "expected arithmetic type");

    if (lhsTy == rhsTy)
        return lhsTy;

    auto check = [] (const FullySpecifiedType& ty,
                     bool (Type::*is) () const,
                     auto (Type::*as) () const,
                     int kind) {
        if (((*ty).*is)() && ((*ty).*as)()->kind() ==  kind)
            return true;
        return false;
    };

    auto ty =
        check(lhsTy, &Type::isFloatType, &Type::asFloatType, FloatType::LongDouble) ?
            lhsTy :
            check(rhsTy, &Type::isFloatType, &Type::asFloatType, FloatType::LongDouble) ?
                rhsTy :
        check(lhsTy, &Type::isFloatType, &Type::asFloatType, FloatType::Double) ?
            lhsTy :
            check(rhsTy, &Type::isFloatType, &Type::asFloatType, FloatType::Double) ?
                rhsTy :
        check(lhsTy, &Type::isFloatType, &Type::asFloatType, FloatType::Float) ?
            lhsTy :
            check(rhsTy, &Type::isFloatType, &Type::asFloatType, FloatType::Float) ?
                rhsTy : FullySpecifiedType();

    if (ty)
        return ty;

    // Both operands are integers.

    auto lhsPromoTy = promotedType(lhsTy);
    auto rhsPromoTy = promotedType(rhsTy);

    if (lhsPromoTy == rhsPromoTy)
        return lhsPromoTy;

    if (lhsPromoTy->asIntegerType()->rank() > rhsPromoTy->asIntegerType()->rank())
        return lhsPromoTy;
    return rhsPromoTy;
}

FullySpecifiedType TypeOfExpression::promotedType(const FullySpecifiedType& baseTy) const
{
    if (!baseTy || !baseTy->isIntegerType())
        return baseTy;

    switch (baseTy->asIntegerType()->kind()) {
    case IntegerType::Bool:
    case IntegerType::Char:
    case IntegerType::Short: {
        FullySpecifiedType promoTy;
        promoTy.setUnsigned(baseTy.isUnsigned());
        promoTy.setType(control()->integerType(IntegerType::Int));
        return promoTy;
    }

    default:
        return baseTy;
    }
}

bool TypeOfExpression::visit(ArrayAccessAST *ast)
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

bool TypeOfExpression::visit(BinaryExpressionAST *ast)
{
    BinaryExpressionAST* expr = ast->asBinaryExpression();

    auto plusMinus = [this, expr] (bool isMinus) {
        accept(expr->left_expression);
        const FullySpecifiedType lhsTy = fullType_.back();
        accept(expr->right_expression);
        const FullySpecifiedType rhsTy = fullType_.back();

        if (!lhsTy || !rhsTy)
            return FullySpecifiedType();

        if (Type::isArithmetic(lhsTy.type()) && Type::isArithmetic(rhsTy.type()))
            return commonRealType(lhsTy, rhsTy);

        if (lhsTy->isPointerType() && !rhsTy->isPointerType())
            return lhsTy;

        if (rhsTy->isPointerType() && !lhsTy->isPointerType())
            return rhsTy;

        if (isMinus && rhsTy->isPointerType() && lhsTy->isPointerType()) {
            FullySpecifiedType ty(control()->integerType(IntegerType::Long));
            return ty;
        }

        return FullySpecifiedType();
    };

    switch (tokenKind(expr->binary_op_token)) {
    case T_PLUS:
        fullType_.push_back(plusMinus(false));
        return false;

    case T_MINUS:
        fullType_.push_back(plusMinus(true));
        return false;

    case T_STAR:
    case T_SLASH:
    case T_PERCENT:
    case T_AMPER:
    case T_PIPE:
    case T_CARET:
    case T_LESS_LESS:
    case T_GREATER_GREATER: {
        accept(expr->left_expression);
        const FullySpecifiedType lhsTy = fullType_.back();
        accept(expr->right_expression);
        const FullySpecifiedType rhsTy = fullType_.back();

        if (!lhsTy) {
            fullType_.push_back(rhsTy);
        } else {
            if (rhsTy && Type::isArithmetic(rhsTy.type()) && Type::isArithmetic(lhsTy.type()))
                fullType_.push_back(commonRealType(lhsTy, rhsTy));
            else
                fullType_.push_back(lhsTy);
        }
        return false;
    }

    // Comparison and logical
    case T_LESS:
    case T_LESS_EQUAL:
    case T_GREATER:
    case T_GREATER_EQUAL:
    case T_EQUAL_EQUAL:
    case T_EXCLAIM_EQUAL:
    case T_AMPER_AMPER:
    case T_PIPE_PIPE:
        fullType_.push_back(control()->integerType(IntegerType::Int));
        return false;

    // Assignment
    case T_EQUAL:
    case T_PLUS_EQUAL:
    case T_MINUS_EQUAL:
    case T_STAR_EQUAL:
    case T_SLASH_EQUAL:
    case T_PERCENT_EQUAL:
    case T_AMPER_EQUAL:
    case T_PIPE_EQUAL:
    case T_CARET_EQUAL:
    case T_LESS_LESS_EQUAL:
    case T_GREATER_GREATER_EQUAL:
        accept(expr->left_expression);
        return false;

    case T_COMMA:
        accept(expr->right_expression);
        return false;

    default:
        PSYCHE_ASSERT(false, return false, "unrecognized operator");
        return false;
    }
}

bool TypeOfExpression::visit(CallAST *ast)
{
    accept(ast->base_expression);
    return false;
}

bool TypeOfExpression::visit(CastExpressionAST *ast)
{
    fullType_.push_back(ast->expression_type);
    return false;
}

bool TypeOfExpression::visit(ConditionalExpressionAST *ast)
{
    accept(ast->right_expression);
    return false;
}

void TypeOfExpression::process(const Identifier *id)
{
    const Symbol* valSym;
    if (singleLookup_)
        valSym = scope_->find(id);
    else
        valSym = lookupValueSymbol(id, scope_);

    if (!valSym || !valSym->type()) {
        fullType_.push_back(FullySpecifiedType());
        return;
    }

    FullySpecifiedType ty = valSym->type();
    fullType_.push_back(ty);

    while (ty.type()->isPointerType())
        ty = ty.type()->asPointerType()->elementType();

    if (ty.type()->isNamedType()) {
        id = ty.type()->asNamedType()->name()->identifier();
    } else if (ty.type()->isClassType()
               && (ty.type()->asClassType()->name()->asNameId()
                   || ty.type()->asClassType()->name()->asElaboratedNameId())) {
        id = ty.type()->asClassType()->name()->identifier();
    } else {
        return;
    }

    PSYCHE_ASSERT(id, return, "expected valid id");
    Symbol* tySym = lookupTypeSymbol(id, scope_);
    if (tySym && tySym->type()->asClassType())
        scope_ = tySym->type()->asClassType();
}

bool TypeOfExpression::visit(IdExpressionAST *ast)
{
    PSYCHE_ASSERT(ast->asIdExpression()->name->name->asNameId(),
                  return false,
                  "expected trivial identifier name");
    process(ast->name->name->identifier());
    return false;
}

bool TypeOfExpression::visit(MemberAccessAST *ast)
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

bool TypeOfExpression::visit(NumericLiteralAST *ast)
{
    const Token& tk = tokenAt(ast->literal_token);
    if (tk.is(T_CHAR_LITERAL)) {
        // TODO
        fullType_.push_back(control()->integerType(IntegerType::Char));
    } else {
        const NumericLiteral *numLit = numericLiteral(ast->literal_token);
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
    }

    return false;
}

bool TypeOfExpression::visit(BoolLiteralAST *ast)
{
    fullType_.push_back(control()->integerType(IntegerType::Int));
    return false;
}

bool TypeOfExpression::visit(StringLiteralAST *ast)
{
    FullySpecifiedType baseType = control()->integerType(IntegerType::Char);
    fullType_.push_back(control()->pointerType(baseType));
    return false;
}

bool TypeOfExpression::visit(UnaryExpressionAST *ast)
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

bool TypeOfExpression::visit(SizeofExpressionAST *ast)
{
    fullType_.push_back(control()->integerType(IntegerType::Int));
    return false;
}

bool TypeOfExpression::visit(PointerLiteralAST *ast)
{
    fullType_.push_back(control()->pointerType(FullySpecifiedType()));
    return false;
}
