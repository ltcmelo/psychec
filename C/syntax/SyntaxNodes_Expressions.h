// Copyright (c) 2016/17/18/19/20/21/22 Leandro T. C. Melo <ltcmelo@gmail.com>
// Copyright (c) 2008 Roberto Raggi <roberto.raggi@gmail.com>
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

#ifndef PSYCHE_C_SYNTAX_NODES_EXPRESSIONS_H__
#define PSYCHE_C_SYNTAX_NODES_EXPRESSIONS_H__

#include "SyntaxNode.h"
#include "SyntaxNodes_Statements.h"
#include "SyntaxNodes_MIXIN.h"
#include "SyntaxToken.h"
#include "SyntaxTree.h"

namespace psy {
namespace C {

//-------------//
// Expressions //
//-------------//
/**
 * \brief The ConstantExpressionSyntax class.
 *
 * An \a expression that is a \a constant of numeric value.
 *
 * \code
 * 42
 * 3.14
 * 'c'
 * \endcode
 *
 * \note
 * In the C++ Standard, primary expressions of an integer, floating, etc. are
 * all referred to as "literals" (e.g., \a integer-literal); in C, there is a
 * terminology distinction: a string is referred to as an \a string-literal,
 * but the numerics as a \a constant.
 *
 * \attention
 * A ConstantExpressionSyntax of kind NULL_ConstantExpression is only
 * produced if \c NULL is configured as a builtin in LanguageExtensions.
 * The grammar extension applies on 6.4.4 and, consequently, on 6.5.1 as well.
 *
 * \attention
 * A ConstantExpressionSyntax of kind BooleanConstantExpression, is
 * produced either C99 mode (despite of header \c <stdbool.h> being \c #included
 * or not) of if booleans are configued as native in LanguageExtensions.
 *
 * \remark 6.4.4, 6.5.1, and 7.19
 *
 * \see LanguageExtensions::enable_NULLAsBuiltin
 * \see LanguageExtensions::enable_NativeBooleans
 *
 * \note Similar to:
 * - \c clang::IntegerLiteral, \c clang::FloatingLiteral, \c clang::CharacterLiteral, and
 *   \c clang::CXXBoolLiteralExpr of LLVM/Clang.
 * - \c clang::syntax::IntegerLiteralExpression, \c clang::syntax::FloatingLiteralExpression,
 *   \c clang::syntax::CharacterLiteralExpression, and \c clang::syntax::BoolLiteralExpression
 *   of Clang's Libtooling.
 * - \c Microsoft.CodeAnalysis.CSharp.Syntax.LiteralExpressionSyntax of Roslyn.
 */
class PSY_C_API ConstantExpressionSyntax final : public ExpressionSyntax
{
    AST_NODE_NK(ConstantExpression, Expression)

public:
    SyntaxToken constantToken() const { return tokenAtIndex(constantTkIdx_); }

private:
    LexedTokens::IndexType constantTkIdx_ = LexedTokens::invalidIndex();
    AST_CHILD_LST1(constantTkIdx_)
};

/**
 * \brief The StringLiteralExpressionSyntax class.
 *
 * An \a expression that is a literal string of characters.
 *
 * \code
 * "abc"
 * u"abc"
 * \endcode
 *
 * \remark 6.4.5 and 6.5.1
 *
 * \note Similar to:
 * - \c clang::StringLiteral of LLVM/Clang.
 * - \c clang::syntax::StringLiteralExpression of Clang's Libtooling.
 * - \c Microsoft.CodeAnalysis.CSharp.Syntax.LiteralExpressionSyntax of Roslyn.
 */
class PSY_C_API StringLiteralExpressionSyntax final : public ExpressionSyntax
{
    AST_NODE_1K(StringLiteralExpression, Expression)

public:
    SyntaxToken literalToken() const { return tokenAtIndex(litTkIdx_); }
    StringLiteralExpressionSyntax* adjacent() const { return adjacent_; }

private:
    LexedTokens::IndexType litTkIdx_ = LexedTokens::invalidIndex();
    AST_CHILD_LST1(litTkIdx_)

    StringLiteralExpressionSyntax* adjacent_ = nullptr;
};

/**
 * \brief The NameSyntax class.
 *
 * The base class of every \a expression that is a name.
 *
 * \note Similar to:
 * - \c Microsoft.CodeAnalysis.CSharp.Syntax.NameSyntax of Roslyn.
 */
class PSY_C_API NameSyntax : public ExpressionSyntax
{
    AST_NODE(Name, Expression)
};

/**
 * \brief The IdentifierNameSyntax class.
 *
 * \code
 * x
 * \endcode
 *
 * \remark 6.4.2 and 6.5.1
 *
 * \note Similar to:
 * - \c clang::UnaryOperator of LLVM/Clang.
 * - \c clang::syntax::IdExpression of Clang's Libtooling.
 * - \c Microsoft.CodeAnalysis.CSharp.Syntax.IdentifierNameSyntax of Roslyn.
 */
class PSY_C_API IdentifierNameSyntax final : public NameSyntax
{
    AST_NODE_1K(IdentifierName, Name)

public:
    SyntaxToken identifierToken() const { return tokenAtIndex(identTkIdx_); }

private:
    LexedTokens::IndexType identTkIdx_ = LexedTokens::invalidIndex();
    AST_CHILD_LST1(identTkIdx_)
};

/**
 * \brief The PredefinedNameSyntax class.
 *
 * \code
 * __func__
 * __FUNCTION__
 * __PRETTY_FUNCTION__
 * \endcode
 */
class PSY_C_API PredefinedNameSyntax final : public NameSyntax
{
    AST_NODE_1K(PredefinedName, Name)

public:
    SyntaxToken predefinedToken() const { return tokenAtIndex(predefTkIdx_); }

private:
    LexedTokens::IndexType predefTkIdx_ = LexedTokens::invalidIndex();
    AST_CHILD_LST1(predefTkIdx_)
};

/**
 * \brief The ParenthesizedExpressionSyntax class.
 *
 * \code
 * (x)
 * (y())
 * (z.w)
 * \endcode
 *
 * \remark 6.5.1
 *
 * \note Similar to:
 * - \c clang::ParenExpr of LLVM/Clang.
 * - \c clang::syntax::ParenExpression of Clang's Libtooling.
 * - \c Microsoft.CodeAnalysis.CSharp.Syntax.ParenthesizedExpressionSyntax of Roslyn.
 */
class PSY_C_API ParenthesizedExpressionSyntax final : public ExpressionSyntax
{
    AST_NODE_1K(ParenthesizedExpression, Expression)

public:
    SyntaxToken openParenthesisToken() const { return tokenAtIndex(openParenTkIdx_); }
    const ExpressionSyntax* expression() const { return expr_; }
    SyntaxToken closeParenthesisToken() const { return tokenAtIndex(closeParenTkIdx_); }

private:
    LexedTokens::IndexType openParenTkIdx_ = LexedTokens::invalidIndex();
    ExpressionSyntax* expr_ = nullptr;
    LexedTokens::IndexType closeParenTkIdx_ = LexedTokens::invalidIndex();
    AST_CHILD_LST3(openParenTkIdx_,
                   expr_,
                   closeParenTkIdx_)
};

/**
 * \brief The GenericSelection class.
 *
 * \remark 6.5.1.1
 *
 * \note Similar to:
 * - \c clang:GenericSelectionExpr of LLVM/Clang.
 */
class PSY_C_API GenericSelectionExpressionSyntax final : public ExpressionSyntax
{
    AST_NODE_1K(GenericSelectionExpression, Expression)

public:
    SyntaxToken genericKeyword() const { return tokenAtIndex(genericKwTkIdx_); }
    SyntaxToken openParenthesisToken() const { return tokenAtIndex(openParenTkIdx_); }
    const ExpressionSyntax* expression() const { return expr_; }
    SyntaxToken commaToken() const { return tokenAtIndex(commaTkIdx_); }
    const GenericAssociationListSyntax* associations() const { return assocs_; }
    SyntaxToken closeParenthesisToken() const { return tokenAtIndex(closeParenTkIdx_); }

private:
    LexedTokens::IndexType genericKwTkIdx_ = LexedTokens::invalidIndex();
    LexedTokens::IndexType openParenTkIdx_ = LexedTokens::invalidIndex();
    ExpressionSyntax* expr_ = nullptr;
    LexedTokens::IndexType commaTkIdx_ = LexedTokens::invalidIndex();
    GenericAssociationListSyntax* assocs_ = nullptr;
    LexedTokens::IndexType closeParenTkIdx_ = LexedTokens::invalidIndex();
    AST_CHILD_LST6(genericKwTkIdx_,
                   openParenTkIdx_,
                   expr_,
                   commaTkIdx_,
                   assocs_,
                   closeParenTkIdx_)
};

/**
 * \brief The GenericAssociationSyntax class.
 *
 * \remark 6.5.1.1
 */
class PSY_C_API GenericAssociationSyntax final : public SyntaxNode
{
    AST_G_NODE_NK(GenericAssociation)

public:
    const SyntaxNode* typeName_or_default() const { return typeName_or_default_; }
    SyntaxToken colonToken() const { return tokenAtIndex(colonTkIdx_); }
    const ExpressionSyntax * expression() const { return expr_; }

private:
    SyntaxNode* typeName_or_default_ = nullptr;
    LexedTokens::IndexType colonTkIdx_ = LexedTokens::invalidIndex();
    ExpressionSyntax* expr_ = nullptr;
    AST_CHILD_LST3(typeName_or_default_, colonTkIdx_, expr_)
};

/**
 * \brief The ExtGNU_EnclosedCompoundStatementExpressionSyntax class.
 *
 * \attention This is a GNU extension:
 * https://gcc.gnu.org/onlinedocs/gcc/Statement-Exprs.html#Statement-Exprs
 */
class PSY_C_API ExtGNU_EnclosedCompoundStatementExpressionSyntax final : public ExpressionSyntax
{
    AST_NODE_1K(ExtGNU_EnclosedCompoundStatementExpression, Expression)

public:
    SyntaxToken openParenthesisToken() const { return tokenAtIndex(openParenTkIdx_); }
    const CompoundStatementSyntax* statement() const { return stmt_; }
    SyntaxToken closeParenthesisToken() const { return tokenAtIndex(closeParenTkIdx_); }

private:
    LexedTokens::IndexType openParenTkIdx_ = LexedTokens::invalidIndex();
    CompoundStatementSyntax* stmt_ = nullptr;
    LexedTokens::IndexType closeParenTkIdx_ = LexedTokens::invalidIndex();
    AST_CHILD_LST3(openParenTkIdx_, stmt_, closeParenTkIdx_)
};

/* Operations */
/**
 * \brief The UnaryExpressionSyntax class.
 *
 * The base class of every \a expression which consists of one operand and
 * one operator, regardless of whether the latter is prefix or postfix.
 * For instance, both variations of increment/decrement are represented
 * by a class that is derived from UnaryExpressionSyntax; respectively,
 * PrefixUnaryExpressionSyntax and PostfixUnaryExpressionSyntax.
 *
 * See note about the type hierarchy \a expressions in ExpressionSyntax.
 *
 * \remark 6.5.3
 *
 * \note Similar to:
 * - \c clang::UnaryOperator of LLVM/Clang.
 * - \c clang::syntax::UnaryOperatorExpression of Clang's Libtooling.
 */
class PSY_C_API UnaryExpressionSyntax : public ExpressionSyntax
{
    AST_NODE(UnaryExpression, Expression)
};

/**
 * \brief The PostfixUnaryExpressionSyntax class.
 *
 * See note about the type hierarchy of \a expressions in ExpressionSyntax.
 *
 * \code
 * i++
 * \endcode
 *
 * \note Similar to:
 * - \c clang::UnaryOperator of LLVM/Clang.
 * - \c clang::syntax::PostfixUnaryOperatorExpression of Clang's Libtooling.
 * - \c Microsoft.CodeAnalysis.CSharp.Syntax.PostfixUnaryExpressionSyntax of Roslyn.
 *
 * \remark 6.5.2.4
 */
class PSY_C_API PostfixUnaryExpressionSyntax final : public UnaryExpressionSyntax
{
    AST_NODE_NK(PostfixUnaryExpression, UnaryExpression)

public:
    const ExpressionSyntax* expression() const { return expr_; }
    SyntaxToken operatorToken() const { return tokenAtIndex(oprtrTkIdx_); }

private:
    ExpressionSyntax* expr_ = nullptr;
    LexedTokens::IndexType oprtrTkIdx_ = LexedTokens::invalidIndex();
    AST_CHILD_LST2(expr_, oprtrTkIdx_)
};

/**
 * \brief The PrefixUnaryExpressionSyntax class.
 *
 * See note about the type hierarchy of \a expressions in ExpressionSyntax.
 *
 * \code
 * ++i
 * -n
 * !b
 * \endcode
 *
 * \remark 6.5.3.1, 6.5.3.2, and 6.5.3.3
 *
 * \note Similar to:
 * - \c clang::UnaryOperator of LLVM/Clang.
 * - \c clang::syntax::PrefixUnaryOperatorExpression of Clang's Libtooling.
 * - \c Microsoft.CodeAnalysis.CSharp.Syntax.PrefixUnaryExpressionSyntax of Roslyn.
 */
class PSY_C_API PrefixUnaryExpressionSyntax final : public UnaryExpressionSyntax
{
    AST_NODE_NK(PrefixUnaryExpression, UnaryExpression)

public:
    SyntaxToken operatorToken() const { return tokenAtIndex(oprtrTkIdx_); }
    const ExpressionSyntax* expression() const { return expr_; }

private:
    LexedTokens::IndexType oprtrTkIdx_ = LexedTokens::invalidIndex();
    ExpressionSyntax* expr_ = nullptr;
    AST_CHILD_LST2(oprtrTkIdx_, expr_)
};

/**
 * \brief The ArraySubscriptExpressionSyntax class.
 *
 * \code
 * a[i]
 * \endcode
 *
 * \remark 6.5.2.1
 *
 * \note Similar to:
 * - \c clang::ArraySubscriptExpr of LLVM/Clang.
 * - \c clang::syntax::ArraySubscript of Clang's Libtooling.
 * - \c Microsoft.CodeAnalysis.CSharp.Syntax.ElementAccessExpressionSyntax of Roslyn.
 */
class PSY_C_API ArraySubscriptExpressionSyntax final : public ExpressionSyntax
{
    AST_NODE_NK(ArraySubscriptExpression, Expression)

public:
    const ExpressionSyntax* expression() const { return expr_; }
    SyntaxToken openBracketToken() const { return tokenAtIndex(openBracketTkIdx_); }
    const ExpressionSyntax* argument() const { return arg_; }
    SyntaxToken closeBracketToken() const { return tokenAtIndex(closeBracketTkIdx_); }

private:
    ExpressionSyntax* expr_ = nullptr;
    LexedTokens::IndexType openBracketTkIdx_ = LexedTokens::invalidIndex();
    ExpressionSyntax* arg_ = nullptr;
    LexedTokens::IndexType closeBracketTkIdx_ = LexedTokens::invalidIndex();
    AST_CHILD_LST4(expr_, openBracketTkIdx_, arg_, closeBracketTkIdx_)
};

/**
 * \brief The CallExpressionSyntax class.
 *
 * \code
 * f()
 * f(1, 'c')
 * \endcode
 *
 * \remark 6.5.2.2
 *
 * \note Similar to:
 * - \c clang::CallExpr in LLVM/Clang.
 * - \c clang::syntax::CallExpression in Clang's Libtooling.
 * - \c Microsoft.CodeAnalysis.CSharp.Syntax.InvocationExpressionSyntax of Roslyn.
 */
class PSY_C_API CallExpressionSyntax final : public ExpressionSyntax
{
    AST_NODE_NK(CallExpression, Expression)

public:
    const ExpressionSyntax* expression() const { return expr_; }
    SyntaxToken openParenthesisToken() const { return tokenAtIndex(openParenTkIdx_); }
    const ExpressionListSyntax* arguments() const { return args_; }
    SyntaxToken closeParenthesisToken() const { return tokenAtIndex(closeParenTkIdx_); }

private:
    ExpressionSyntax* expr_ = nullptr;
    LexedTokens::IndexType openParenTkIdx_ = LexedTokens::invalidIndex();
    ExpressionListSyntax* args_ = nullptr;
    LexedTokens::IndexType closeParenTkIdx_ = LexedTokens::invalidIndex();
    AST_CHILD_LST4(expr_, openParenTkIdx_, args_, closeParenTkIdx_)
};

/**
 * \brief The MemberAccessExpressionSyntax class.
 *
 * \code
 * v.m
 * v->m
 * \endcode
 *
 * \remark 6.5.2.3
 *
 * \note Similar to:
 * - \c clang::MemberExpr of LLVM/Clang.
 * - \c clang::syntax::MemberExpression of Clang's Libtooling.
 * - \c Microsoft.CodeAnalysis.CSharp.Syntax.MemberAccessExpressionSyntax of Roslyn.
 */
class PSY_C_API MemberAccessExpressionSyntax final : public ExpressionSyntax
{
    AST_NODE_NK(MemberAccessExpression, Expression)

public:
    const ExpressionSyntax* expression() const { return expr_; }
    SyntaxToken operatorToken() const { return tokenAtIndex(oprtrTkIdx_); }
    const IdentifierNameSyntax* identifier() const { return identExpr_; }

private:
    ExpressionSyntax* expr_ = nullptr;
    LexedTokens::IndexType oprtrTkIdx_ = LexedTokens::invalidIndex();
    IdentifierNameSyntax* identExpr_ = nullptr;
    AST_CHILD_LST3(expr_, oprtrTkIdx_, identExpr_)
};

/**
 * \brief The CompoundLiteralExpressionSyntax class.
 *
 * \remark 6.5.2.5
 *
 * \note Similar to:
 * - \c clang::CompoundLiteralExpr of LLVM/Clang.
 */
class PSY_C_API CompoundLiteralExpressionSyntax final : public ExpressionSyntax
{
    AST_NODE_1K(CompoundLiteralExpression, Expression)

public:
    SyntaxToken openParenthesisToken() const { return tokenAtIndex(openParenTkIdx_); }
    const TypeNameSyntax* typeName() const { return typeName_; }
    SyntaxToken closeParenthesisToken() const { return tokenAtIndex(closeParenTkIdx_); }
    const InitializerSyntax* initializer() const { return init_; }

private:
    LexedTokens::IndexType openParenTkIdx_ = LexedTokens::invalidIndex();
    TypeNameSyntax* typeName_ = nullptr;
    LexedTokens::IndexType closeParenTkIdx_ = LexedTokens::invalidIndex();
    InitializerSyntax* init_ = nullptr;
    AST_CHILD_LST4(openParenTkIdx_, typeName_, closeParenTkIdx_, init_)
};

/**
 * \brief The TypeTraitExpressionSyntax class.
 *
 * Represents a `sizeof' or an `_Alignof' expression. The operator's argument
 * is:
 *   - a TypeNameSyntax, if surrounded by parenthesis, or
 *   - an ExpressionSyntax, otherwise.
 *
 * \code
 * sizeof x
 * sizeof(x)
 * _Alignof(x)
 * \endcode
 *
 * \remark 6.5.3.4
 *
 * \note Similar to:
 * - \c clang::UnaryExprOrTypeTraitExpr of LLVM/Clang.
 * - \c clang::syntax::PrefixUnaryOperatorExpression of Clang's Libtooling.
 * - \c Microsoft.CodeAnalysis.CSharp.Syntax.SizeOfExpressionSyntax of Roslyn.
 */
class PSY_C_API TypeTraitExpressionSyntax final : public ExpressionSyntax
{
    AST_NODE_NK(TypeTraitExpression, Expression)

public:
    SyntaxToken operatorToken() const { return tokenAtIndex(oprtrTkIdx_); }
    const TypeReferenceSyntax* tyReference() const { return tyRef_; }

private:
    LexedTokens::IndexType oprtrTkIdx_ = LexedTokens::invalidIndex();
    TypeReferenceSyntax* tyRef_ = nullptr;
    AST_CHILD_LST2(oprtrTkIdx_, tyRef_);
};

/**
 * \brief The CastExpressionSyntax class.
 *
 * \code
 * (int)v
 * (t)v
 * \endcode
 *
 * \remark 6.5.4
 *
 * \note Similar to:
 * - \c clang::CStyleCastExpr of LLVM/Clang.
 * - \c Microsoft.CodeAnalysis.CSharp.Syntax.CastExpressionSyntax of Roslyn.
 */
class PSY_C_API CastExpressionSyntax final : public ExpressionSyntax
{
    AST_NODE_1K(CastExpression, Expression)

public:
    SyntaxToken openParenthesisToken() const { return tokenAtIndex(openParenTkIdx_); }
    const TypeNameSyntax* typeName() const { return typeName_; }
    SyntaxToken closeParenthesisToken() const { return tokenAtIndex(closeParenTkIdx_); }
    const ExpressionSyntax* expression() const { return expr_; }

private:
    LexedTokens::IndexType openParenTkIdx_ = LexedTokens::invalidIndex();
    TypeNameSyntax* typeName_ = nullptr;
    LexedTokens::IndexType closeParenTkIdx_ = LexedTokens::invalidIndex();;
    ExpressionSyntax* expr_ = nullptr;
};

/**
 * \brief The BinaryExpressionSyntax class.
 *
 * \code
 * a + b
 * a == b
 * a && b
 * \endcode
 *
 * \remark 6.5.5, 6.5.6, 6.5.7, 6.5.8, 6.5.9, 6.5.10,
 * 6.5.11, 6.5.12, 6.5.13, and 6.5.14.
 *
 * \note Similar to:
 * - \c clang::BinaryOperator of LLVM/Clang.
 * - \c clang::syntax::BinaryOperatorExpression of Clang's Libtooling.
 * - \c Microsoft.CodeAnalysis.CSharp.Syntax.BinaryExpressionSyntax of Roslyn.
 */
class PSY_C_API BinaryExpressionSyntax
        : public ExpressionSyntax
        , public MIXIN_LeftExpressionInfixOperatorRightExpression
{
    AST_NODE_NK(BinaryExpression, Expression)

public:
    const ExpressionSyntax* left() const override { return leftExpr_; }
    SyntaxToken operatorToken() const override { return tokenAtIndex(oprtrTkIdx_); }
    const ExpressionSyntax* right() const override { return rightExpr_; }

private:
    AST_CHILD_LST3(leftExpr_, oprtrTkIdx_, rightExpr_)
};

/**
 * \brief The ConditionalExpressionSyntax
 *
 * \code
 * a ? b : c
 * \endcode
 *
 * \remark 6.5.15
 *
 * \note Similar to:
 * - \c clang::ConditionalOperator of LLVM/Clang.
 * - \c Microsoft.CodeAnalysis.CSharp.Syntax.ConditionalExpressionSyntax of Roslyn.
 */
class PSY_C_API ConditionalExpressionSyntax : public ExpressionSyntax
{
    AST_NODE_1K(ConditionalExpression, Expression)

public:
    const ExpressionSyntax* condition() const { return condExpr_; }
    SyntaxToken questionToken() const { return tokenAtIndex(questionTkIdx_); }
    const ExpressionSyntax* whenTrue() const { return whenTrueExpr_; }
    SyntaxToken colonToken() const { return tokenAtIndex(colonTkIdx_); }
    const ExpressionSyntax* whenFalse() const { return whenFalseExpr_; }

private:
    ExpressionSyntax* condExpr_ = nullptr;
    LexedTokens::IndexType questionTkIdx_ = LexedTokens::invalidIndex();
    ExpressionSyntax* whenTrueExpr_ = nullptr;
    LexedTokens::IndexType colonTkIdx_ = LexedTokens::invalidIndex();
    ExpressionSyntax* whenFalseExpr_ = nullptr;
};

/**
 * \brief The AssignmentExpressionSyntax class.
 *
 * \code
 * a = b
 * a += b
 * a /= b
 * \endcode
 *
 * \remark 6.5.16
 *
 * \note Similar to:
 * - \c clang::BinaryOperator of LLVM/Clang.
 * - \c clang::syntax::BinaryOperatorExpression of Clang's Libtooling.
 * - \c Microsoft.CodeAnalysis.CSharp.Syntax.AssignmentExpressionSyntax of Roslyn.
 */
class PSY_C_API AssignmentExpressionSyntax final
        : public ExpressionSyntax
        , public MIXIN_LeftExpressionInfixOperatorRightExpression
{
    AST_NODE_NK(AssignmentExpression, Expression)

public:
    const ExpressionSyntax* left() const override { return leftExpr_; }
    SyntaxToken operatorToken() const override { return tokenAtIndex(oprtrTkIdx_); }
    const ExpressionSyntax* right() const override { return rightExpr_; }

private:
    AST_CHILD_LST3(leftExpr_, oprtrTkIdx_, rightExpr_)
};

/**
 * \brief The SequencingExpressionSyntax class.
 *
 * \code
 * a, b
 * a, b, c
 * \endcode
 *
 * \remark 6.5.17
 *
 * \note Similar to:
 * - \c clang::BinaryOperator of LLVM/Clang.
 * - \c clang::syntax::BinaryOperatorExpression of Clang's Libtooling.
 */
class PSY_C_API SequencingExpressionSyntax
        : public ExpressionSyntax
        , public MIXIN_LeftExpressionInfixOperatorRightExpression
{
    AST_NODE_1K(SequencingExpression, Expression)

public:
    const ExpressionSyntax* left() const override { return leftExpr_; }
    SyntaxToken operatorToken() const override { return tokenAtIndex(oprtrTkIdx_); }
    const ExpressionSyntax* right() const override { return rightExpr_; }

private:
    AST_CHILD_LST3(leftExpr_, oprtrTkIdx_, rightExpr_)
};

/**
 * \brief The AmbiguousCastOrBinaryExpressionSyntax class.
 *
 * Represents the ambiguous syntaxes \a cast-expression and \a binary-expression.
 *
 * \code
 * (x) + y
 * (x) * y
 * \endcode
 */
class PSY_C_API AmbiguousCastOrBinaryExpressionSyntax final : public ExpressionSyntax
{
    AST_NODE_NK(AmbiguousCastOrBinaryExpression, Expression)

public:
    const CastExpressionSyntax* castExpression() const { return castExpr_; }
    const BinaryExpressionSyntax* binaryExpression() const { return binExpr_; }

private:
    CastExpressionSyntax* castExpr_ = nullptr;
    BinaryExpressionSyntax* binExpr_ = nullptr;
};

/**
 * \brief The VAArgumentExpressionSyntax class.
 *
 * \remark 7.16
 *
 * \note Similar to:
 * - \c clang:VAArgExpr of LLVM/Clang.
 */
class PSY_C_API VAArgumentExpressionSyntax final : public ExpressionSyntax
{
    AST_NODE_1K(VAArgumentExpression, Expression)

public:
    SyntaxToken keyword() const { return tokenAtIndex(kwTkIdx_); }
    SyntaxToken openParenthesisToken() const { return tokenAtIndex(openParenTkIdx_); }
    const ExpressionSyntax* expression() const { return expr_; }
    SyntaxToken commaToken() const { return tokenAtIndex(commaTkIdx_); }
    const TypeNameSyntax* typeName() const { return typeName_; }
    SyntaxToken closeParenthesisToken() const { return tokenAtIndex(closeParenTkIdx_); }

private:
    LexedTokens::IndexType kwTkIdx_ = LexedTokens::invalidIndex();
    LexedTokens::IndexType openParenTkIdx_ = LexedTokens::invalidIndex();
    ExpressionSyntax* expr_ = nullptr;
    LexedTokens::IndexType commaTkIdx_ = LexedTokens::invalidIndex();
    TypeNameSyntax* typeName_ = nullptr;
    LexedTokens::IndexType closeParenTkIdx_ = LexedTokens::invalidIndex();

    AST_CHILD_LST2(expr_, typeName_)
};

/**
 * \brief The OffsetOfExpressionSyntax class.
 *
 * \code
 * offsetof( type-name, member-designator)
 * __builtin_offsetof( type-name, member-designator)
 * \endcode
 *
 * \remark 7.19
 *
 * \note Similar to:
 * - \c clang::OffsetOfExpr of LLVM/Clang.
 */
class PSY_C_API OffsetOfExpressionSyntax final : public ExpressionSyntax
{
    AST_NODE_1K(OffsetOfExpression, Expression)

public:
    SyntaxToken keyword() const { return tokenAtIndex(kwTkIdx_); }
    SyntaxToken openParenthesisToken() const { return tokenAtIndex(openParenTkIdx_); }
    const TypeNameSyntax* typeName() const { return typeName_; }
    SyntaxToken commaToken() const { return tokenAtIndex(commaTkIdx_); }
    const DesignatorSyntax* offsetOfDesignator() const { return offsetOfDesignator_; }
    SyntaxToken closeParenthesisToken() const { return tokenAtIndex(closeParenTkIdx_); }

private:
    LexedTokens::IndexType kwTkIdx_ = LexedTokens::invalidIndex();
    LexedTokens::IndexType openParenTkIdx_ = LexedTokens::invalidIndex();
    TypeNameSyntax* typeName_ = nullptr;
    LexedTokens::IndexType commaTkIdx_ = LexedTokens::invalidIndex();
    DesignatorSyntax* offsetOfDesignator_ = nullptr;
    LexedTokens::IndexType closeParenTkIdx_ = LexedTokens::invalidIndex();

    AST_CHILD_LST6(kwTkIdx_,
                   openParenTkIdx_,
                   typeName_,
                   commaTkIdx_,
                   offsetOfDesignator_,
                   closeParenTkIdx_)
};

/**
 * \brief The ExtGNU_ChooseExpressionSyntax class.
 *
 * \code
 * __builtin_choose_expr( const_expr, expr1, expr2 )
 * \endcode
 *
 * \remark \c const_expr is an integer constant expression.
 *
 * \note Similar to:
 * - \c clang::ChooseExpr of LLVM/Clang.
 */
class PSY_C_API ExtGNU_ChooseExpressionSyntax final : public ExpressionSyntax
{
    AST_NODE_1K(ExtGNU_ChooseExpression, Expression)

public:
    SyntaxToken keyword() const { return tokenAtIndex(kwTkIdx_); }
    SyntaxToken openParenthesisToken() const { return tokenAtIndex(openParenTkIdx_); }
    const ExpressionSyntax* constantExpression() const { return constExpr_; }
    SyntaxToken commaToken1() const { return tokenAtIndex(commaTkIdx1_); }
    const ExpressionSyntax* expression1() const { return expr1_; }
    SyntaxToken commaToken2() const { return tokenAtIndex(commaTkIdx2_); }
    const ExpressionSyntax* expression2() const { return expr2_; }
    SyntaxToken closeParenthesisToken() const { return tokenAtIndex(closeParenTkIdx_); }

private:
    LexedTokens::IndexType kwTkIdx_ = LexedTokens::invalidIndex();
    LexedTokens::IndexType openParenTkIdx_ = LexedTokens::invalidIndex();
    ExpressionSyntax* constExpr_ = nullptr;
    LexedTokens::IndexType commaTkIdx1_ = LexedTokens::invalidIndex();
    ExpressionSyntax* expr1_ = nullptr;
    LexedTokens::IndexType commaTkIdx2_ = LexedTokens::invalidIndex();
    ExpressionSyntax* expr2_ = nullptr;
    LexedTokens::IndexType closeParenTkIdx_ = LexedTokens::invalidIndex();

    AST_CHILD_LST8(kwTkIdx_,
                   openParenTkIdx_,
                   constExpr_,
                   commaTkIdx1_,
                   expr1_,
                   commaTkIdx2_,
                   expr2_,
                   closeParenTkIdx_)
};

/**
 * \brief The ExtGNU_ComplexValuedExpressionSyntax class.
 *
 * \code
 * __real__ expr
 * __imag__ expr
 * \endcode
 */
class PSY_C_API ExtGNU_ComplexValuedExpressionSyntax final : public ExpressionSyntax
{
    AST_NODE_NK(ExtGNU_ComplexValuedExpression, Expression)

public:
    SyntaxToken operatorToken() const { return tokenAtIndex(oprtrTkIdx_); }
    const ExpressionSyntax* expression() const { return expr_; }

private:
    LexedTokens::IndexType oprtrTkIdx_ = LexedTokens::invalidIndex();
    ExpressionSyntax* expr_ = nullptr;

    AST_CHILD_LST2(oprtrTkIdx_, expr_)
};

} // C
} // psy

#endif
