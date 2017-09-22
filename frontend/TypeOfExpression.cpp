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
#include "CoreTypes.h"
#include "Control.h"
#include "Literals.h"
#include "Lookup.h"
#include "Scope.h"
#include "Symbol.h"
#include "Symbols.h"
#include "TranslationUnit.h"
#include <cassert>

using namespace psyche;

TypeOfExpression::TypeOfExpression(TranslationUnit *unit)
    : ASTVisitor(unit)
    , searchMember_(0)
{}

FullySpecifiedType TypeOfExpression::resolve(ExpressionAST *ast, Scope *scope)
{
    scope_ = scope;
    accept(ast);
    return fullType_;
}

FullySpecifiedType TypeOfExpression::commonRealType(const FullySpecifiedType& lhsTy,
                                              const FullySpecifiedType& rhsTy) const
{
    assert((lhsTy && Type::isArithmetic(lhsTy.type())
                && rhsTy && Type::isArithmetic(rhsTy.type()))
            && "expected arithmetic type");

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

    if (fullType_->isArrayType())
        fullType_ = fullType_->asArrayType()->elementType();
    else if (fullType_->isPointerType())
        fullType_ = fullType_->asPointerType()->elementType();
    else
        fullType_ = FullySpecifiedType();

    return false;
}

bool TypeOfExpression::visit(BinaryExpressionAST *ast)
{
    BinaryExpressionAST* expr = ast->asBinaryExpression();

    auto plusMinus = [this, expr] (bool isMinus) {
        accept(expr->left_expression);
        const FullySpecifiedType lhsTy = fullType_;
        accept(expr->right_expression);
        const FullySpecifiedType rhsTy = fullType_;

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
        fullType_ = plusMinus(false);
        return false;

    case T_MINUS:
        fullType_ = plusMinus(true);
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
        const FullySpecifiedType lhsTy = fullType_;
        accept(expr->right_expression);
        const FullySpecifiedType rhsTy = fullType_;

        if (!lhsTy) {
            fullType_ = rhsTy;
        } else {
            if (rhsTy && Type::isArithmetic(rhsTy.type()) && Type::isArithmetic(lhsTy.type()))
                fullType_ = commonRealType(lhsTy, rhsTy);
            else
                fullType_ = lhsTy;
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
        fullType_ = control()->integerType(IntegerType::Int);
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
        assert(false && "unrecognized operator");
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
    fullType_ = ast->expression_type;
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
    if (searchMember_)
        valSym = scope_->find(id);
    else
        valSym = lookupValueSymbol(id, scope_);

    if (!valSym)
        fullType_ = FullySpecifiedType();
    else
        fullType_ = valSym->type();

    if (searchMember_ || !fullType_)
        return;

    FullySpecifiedType ty = fullType_;
    while (true) {
        if (ty.type()->isPointerType())
            ty = ty.type()->asPointerType()->elementType();
        else if (ty.type()->isArrayType())
            ty = ty.type()->asArrayType()->elementType();
        else break;
    }

    if (ty.type()->isClassType()) {
        scope_ = ty.type()->asClassType();
        return;
    }

    if (ty.type()->isNamedType()) {
        Symbol* tySym = lookupTypeSymbol(ty.type()->asNamedType()->name(), scope_);
        if (tySym && tySym->type() && tySym->type()->asClassType())
            scope_ = tySym->type()->asClassType();
    }

    // TODO: Stop the visit entirely.
}

bool TypeOfExpression::visit(IdExpressionAST *ast)
{
    assert((ast->asIdExpression()->name->name->asNameId()) && "expected trivial identifier name");

    process(ast->name->name->identifier());
    return false;
}

bool TypeOfExpression::visit(MemberAccessAST *ast)
{
    assert(ast->member_name->name->asNameId() && "expected trivial member name");

    accept(ast->base_expression);
    ++searchMember_;
    process(ast->member_name->name->identifier());
    assert(searchMember_ > 0 && "expected member check");
    --searchMember_;

    return false;
}

bool TypeOfExpression::visit(NumericLiteralAST *ast)
{
    const Token& tk = tokenAt(ast->literal_token);
    if (tk.is(T_CHAR_LITERAL)) {
        // TODO: char/int
        fullType_ = control()->integerType(IntegerType::Char);
    } else {
        const NumericLiteral *numLit = numericLiteral(ast->literal_token);
        assert(numLit && "numeric literal must exist");
        if (numLit->isDouble()) {
            fullType_ = control()->floatType(FloatType::Double);
        } else if (numLit->isLongDouble()) {
            fullType_ = control()->floatType(FloatType::LongDouble);
        } else if (numLit->isFloat()) {
            fullType_ = control()->floatType(FloatType::Float);
        } else {
            fullType_ = control()->integerType(IntegerType::Int);
        }
    }

    return false;
}

bool TypeOfExpression::visit(BoolLiteralAST*)
{
    fullType_ = control()->integerType(IntegerType::Int);
    return false;
}

bool TypeOfExpression::visit(StringLiteralAST*)
{
    FullySpecifiedType baseType = control()->integerType(IntegerType::Char);
    fullType_ = control()->pointerType(baseType);
    return false;
}

bool TypeOfExpression::visit(UnaryExpressionAST *ast)
{
    accept(ast->expression);

    switch (tokenKind(ast->unary_op_token)) {
    case T_AMPER:
        fullType_ = control()->pointerType(fullType_);
        return false;

    case T_STAR: {
        if (fullType_->isArrayType())
            fullType_ = fullType_->asArrayType()->elementType();
        else if (fullType_->isPointerType())
            fullType_ = fullType_->asPointerType()->elementType();
        else
            fullType_ = FullySpecifiedType();
        return false;
    }

    default:
        return false;
    }
}

bool TypeOfExpression::visit(SizeofExpressionAST*)
{
    fullType_ = control()->integerType(IntegerType::Int);
    return false;
}

bool TypeOfExpression::visit(PointerLiteralAST*)
{
    fullType_ = control()->pointerType(FullySpecifiedType());
    return false;
}

bool psyche::TypeOfExpression::visit(psyche::TypeIdAST *ast)
{
    accept(ast->type_specifier_list);
    accept(ast->declarator);
    return false;
}

bool TypeOfExpression::visit(SimpleSpecifierAST *ast)
{
    switch (tokenKind(ast->specifier_token)) {
    case T_CHAR:
        // TODO: char/int
        fullType_ = control()->integerType(IntegerType::Char);
        return false;
    case T_SHORT:
        fullType_ = control()->integerType(IntegerType::Short);
        return false;
    case T_INT:
        fullType_ = control()->integerType(IntegerType::Int);
        return false;
    case T_LONG:
        if (fullType_.type()
                && fullType_->asIntegerType()
                && fullType_->asIntegerType()->kind() == IntegerType::Long) {
            fullType_ = control()->integerType(IntegerType::LongLong);
        } else {
            fullType_ = control()->integerType(IntegerType::Long);
        }
        return false;
    case T_FLOAT:
        fullType_ = control()->floatType(FloatType::Float);
        return false;
    case T_DOUBLE:
        if (fullType_.type()
                && fullType_->asIntegerType()
                && fullType_->asIntegerType()->kind() == IntegerType::Long) {
            fullType_ = control()->floatType(FloatType::LongDouble);
        } else {
            fullType_ = control()->floatType(FloatType::Double);
        }
        return false;
    case T_VOID:
        return false;

    case T_UNSIGNED:
        fullType_.setUnsigned(true);
        return false;
    case T_SIGNED:
        fullType_.setSigned(true);
        return false;

    case T_CONST:
        fullType_.setConst(true);
        return false;
    case T_VOLATILE:
        fullType_.setVolatile(true);
        return false;

    default:
        assert(false && "unknown type specifier");
        return false;
    }
}

namespace {

FullySpecifiedType lookupTypeName(NameAST *name, const Scope* scope)
{
    Symbol* tySym = lookupTypeSymbol(name->name, scope);
    if (tySym)
        return tySym->type();
    return FullySpecifiedType();
}

} // anonymous

bool TypeOfExpression::visit(NamedTypeSpecifierAST *ast)
{
    fullType_ = lookupTypeName(ast->name, scope_);
    return false;
}

bool TypeOfExpression::visit(ElaboratedTypeSpecifierAST *ast)
{
    fullType_ = lookupTypeName(ast->name, scope_);
    return false;
}

bool TypeOfExpression::visit(DeclaratorAST *ast)
{
    accept(ast->ptr_operator_list);
    return false;
}

bool TypeOfExpression::visit(PointerAST*)
{
    fullType_ = control()->pointerType(fullType_);
    return false;
}
