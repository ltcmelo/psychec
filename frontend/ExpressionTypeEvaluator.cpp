// Copyright (c) 2016 Leandro T. C. Melo <ltcmelo@gmail.com>
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.

#include "ExpressionTypeEvaluator.h"

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

ExpressionTypeEvaluator::ExpressionTypeEvaluator(TranslationUnit *unit)
    : ASTVisitor(unit)
    , _memberAccess(0)
{}

FullySpecifiedType ExpressionTypeEvaluator::evaluate(ExpressionAST* ast, Scope *scope)
{
    scope_ = scope;
    accept(ast);
    return type_;
}

FullySpecifiedType ExpressionTypeEvaluator::commonRealType(const FullySpecifiedType& lhsTy,
                                              const FullySpecifiedType& rhsTy) const
{
    assert((lhsTy && isArithmetic(lhsTy.type())
                && rhsTy && isArithmetic(rhsTy.type()))
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

FullySpecifiedType ExpressionTypeEvaluator::promotedType(const FullySpecifiedType& baseTy) const
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

bool ExpressionTypeEvaluator::visit(ArrayAccessAST* ast)
{
    accept(ast->base_expression);

    if (type_->isArrayType())
        type_ = type_->asArrayType()->elementType();
    else if (type_->isPointerType())
        type_ = type_->asPointerType()->elementType();
    else
        type_ = FullySpecifiedType();

    return false;
}

bool ExpressionTypeEvaluator::visit(BinaryExpressionAST* ast)
{
    BinaryExpressionAST* expr = ast->asBinaryExpression();

    auto plusMinus = [this, expr] (bool isMinus) {
        accept(expr->left_expression);
        const FullySpecifiedType lhsTy = type_;
        accept(expr->right_expression);
        const FullySpecifiedType rhsTy = type_;

        if (!lhsTy || !rhsTy)
            return FullySpecifiedType();

        if (isArithmetic(lhsTy.type()) && isArithmetic(rhsTy.type()))
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
        type_ = plusMinus(false);
        return false;

    case T_MINUS:
        type_ = plusMinus(true);
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
        const FullySpecifiedType lhsTy = type_;
        accept(expr->right_expression);
        const FullySpecifiedType rhsTy = type_;

        if (!lhsTy) {
            type_ = rhsTy;
        } else {
            if (rhsTy && isArithmetic(rhsTy.type()) && isArithmetic(lhsTy.type()))
                type_ = commonRealType(lhsTy, rhsTy);
            else
                type_ = lhsTy;
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
        type_ = control()->integerType(IntegerType::Int);
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

bool ExpressionTypeEvaluator::visit(CallAST* ast)
{
    accept(ast->base_expression);
    return false;
}

bool ExpressionTypeEvaluator::visit(CastExpressionAST* ast)
{
    type_ = ast->expression_type;
    return false;
}

bool ExpressionTypeEvaluator::visit(ConditionalExpressionAST* ast)
{
    accept(ast->right_expression);
    return false;
}

void ExpressionTypeEvaluator::process(const Identifier *id)
{
    const Symbol* valSym;
    if (_memberAccess)
        valSym = scope_->find(id);
    else
        valSym = lookupValueSymbol(id, scope_);

    if (!valSym)
        type_ = FullySpecifiedType();
    else
        type_ = valSym->type();

    if (_memberAccess || !type_)
        return;

    FullySpecifiedType ty = type_;
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

bool ExpressionTypeEvaluator::visit(IdExpressionAST* ast)
{
    assert((ast->asIdExpression()->name->name->asNameId()) && "expected trivial identifier name");

    process(ast->name->name->identifier());
    return false;
}

bool ExpressionTypeEvaluator::visit(MemberAccessAST* ast)
{
    assert(ast->member_name->name->asNameId() && "expected trivial member name");

    accept(ast->base_expression);
    ++_memberAccess;
    process(ast->member_name->name->identifier());
    assert(_memberAccess > 0 && "expected member check");
    --_memberAccess;

    return false;
}

bool ExpressionTypeEvaluator::visit(NumericLiteralAST* ast)
{
    const Token& tk = tokenAt(ast->literal_token);
    if (tk.is(T_CHAR_LITERAL)) {
        // TODO: char/int
        type_ = control()->integerType(IntegerType::Char);
    } else {
        const NumericLiteral *numLit = numericLiteral(ast->literal_token);
        assert(numLit && "numeric literal must exist");
        if (numLit->isDouble()) {
            type_ = control()->floatType(FloatType::Double);
        } else if (numLit->isLongDouble()) {
            type_ = control()->floatType(FloatType::LongDouble);
        } else if (numLit->isFloat()) {
            type_ = control()->floatType(FloatType::Float);
        } else {
            type_ = control()->integerType(IntegerType::Int);
        }
    }

    return false;
}

bool ExpressionTypeEvaluator::visit(BoolLiteralAST*)
{
    type_ = control()->integerType(IntegerType::Int);
    return false;
}

bool ExpressionTypeEvaluator::visit(StringLiteralAST*)
{
    FullySpecifiedType baseType = control()->integerType(IntegerType::Char);
    type_ = control()->pointerType(baseType);
    return false;
}

bool ExpressionTypeEvaluator::visit(UnaryExpressionAST* ast)
{
    accept(ast->expression);

    switch (tokenKind(ast->unary_op_token)) {
    case T_AMPER:
        type_ = control()->pointerType(type_);
        return false;

    case T_STAR: {
        if (type_->isArrayType())
            type_ = type_->asArrayType()->elementType();
        else if (type_->isPointerType())
            type_ = type_->asPointerType()->elementType();
        else
            type_ = FullySpecifiedType();
        return false;
    }

    default:
        return false;
    }
}

bool ExpressionTypeEvaluator::visit(SizeofExpressionAST*)
{
    type_ = control()->integerType(IntegerType::Int);
    return false;
}

bool ExpressionTypeEvaluator::visit(PointerLiteralAST*)
{
    type_ = control()->pointerType(FullySpecifiedType());
    return false;
}

bool ExpressionTypeEvaluator::visit(TypeIdAST* ast)
{
    accept(ast->type_specifier_list);
    accept(ast->declarator);
    return false;
}

bool ExpressionTypeEvaluator::visit(SimpleSpecifierAST* ast)
{
    switch (tokenKind(ast->specifier_token)) {
    case T_CHAR:
        // TODO: char/int
        type_ = control()->integerType(IntegerType::Char);
        return false;
    case T_SHORT:
        type_ = control()->integerType(IntegerType::Short);
        return false;
    case T_INT:
        type_ = control()->integerType(IntegerType::Int);
        return false;
    case T_LONG:
        if (type_.type()
                && type_->asIntegerType()
                && type_->asIntegerType()->kind() == IntegerType::Long) {
            type_ = control()->integerType(IntegerType::LongLong);
        } else {
            type_ = control()->integerType(IntegerType::Long);
        }
        return false;
    case T_FLOAT:
        type_ = control()->floatType(FloatType::Float);
        return false;
    case T_DOUBLE:
        if (type_.type()
                && type_->asIntegerType()
                && type_->asIntegerType()->kind() == IntegerType::Long) {
            type_ = control()->floatType(FloatType::LongDouble);
        } else {
            type_ = control()->floatType(FloatType::Double);
        }
        return false;
    case T_VOID:
        return false;

    case T_UNSIGNED:
        type_.setUnsigned(true);
        return false;
    case T_SIGNED:
        type_.setSigned(true);
        return false;

    case T_CONST:
        type_.setConst(true);
        return false;
    case T_VOLATILE:
        type_.setVolatile(true);
        return false;

    default:
        assert(false && "unknown type specifier");
        return false;
    }
}

namespace {

FullySpecifiedType lookupTypeName(NameAST* name, const Scope* scope)
{
    Symbol* tySym = lookupTypeSymbol(name->name, scope);
    if (tySym)
        return tySym->type();
    return FullySpecifiedType();
}

} // anonymous

bool ExpressionTypeEvaluator::visit(NamedTypeSpecifierAST* ast)
{
    type_ = lookupTypeName(ast->name, scope_);
    return false;
}

bool ExpressionTypeEvaluator::visit(QuantifiedTypeSpecifierAST* ast)
{
    type_ = lookupTypeName(ast->name, scope_);
    return false;
}

bool ExpressionTypeEvaluator::visit(ElaboratedTypeSpecifierAST* ast)
{
    type_ = lookupTypeName(ast->name, scope_);
    return false;
}

bool ExpressionTypeEvaluator::visit(DeclaratorAST* ast)
{
    accept(ast->ptr_operator_list);
    return false;
}

bool ExpressionTypeEvaluator::visit(PointerAST*)
{
    type_ = control()->pointerType(type_);
    return false;
}
