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

#ifndef PSYCHE_C_SYNTAX_NODES_COMMON_H__
#define PSYCHE_C_SYNTAX_NODES_COMMON_H__

#include "SyntaxNode.h"
#include "SyntaxToken.h"
#include "SyntaxTree.h"
#include "SyntaxVisitor.h"

#include "parser/LexedTokens.h"

namespace psy {
namespace C {

//--------//
// Common //
//--------//

/**
 * \brief The TypeNameSyntax class.
 *
 * \remark 6.7.7
 */
class PSY_C_API TypeNameSyntax final : public SyntaxNode
{
    AST_G_NODE_1K(TypeName)

public:
    const SpecifierListSyntax* specifiers() const { return specs_; }
    const DeclaratorSyntax* declarator() const { return decltor_; }

private:
    SpecifierListSyntax* specs_ = nullptr;
    DeclaratorSyntax* decltor_ = nullptr;
    AST_CHILD_LST2(specs_, decltor_);
};

/**
 * \brief The ExpressionAsTypeReferenceSyntax class.
 */
class PSY_C_API ExpressionAsTypeReferenceSyntax final : public TypeReferenceSyntax
{
    AST_NODE_1K(ExpressionAsTypeReference, TypeReference)

public:
    const ExpressionSyntax* expression() const { return expr_; }

private:
    ExpressionSyntax* expr_ = nullptr;
    AST_CHILD_LST1(expr_)
};

/**
 * \brief The TypeNameAsTypeReferenceSyntax class.
 */
class PSY_C_API TypeNameAsTypeReferenceSyntax final : public TypeReferenceSyntax
{
    AST_NODE_1K(TypeNameAsTypeReference, TypeReference)

public:
    SyntaxToken openParenthesisToken() const { return tokenAtIndex(openParenTkIdx_); }
    const TypeNameSyntax* typeName() const { return typeName_; }
    SyntaxToken closeParenthesisToken() const { return tokenAtIndex(closeParenTkIdx_); }

private:
    LexedTokens::IndexType openParenTkIdx_ = LexedTokens::invalidIndex();
    TypeNameSyntax* typeName_ = nullptr;
    LexedTokens::IndexType closeParenTkIdx_ = LexedTokens::invalidIndex();
    AST_CHILD_LST3(openParenTkIdx_,
                   typeName_,
                   closeParenTkIdx_)
};

/**
 * \brief The AmbiguousTypeNameOrExpressionAsTypeReferenceSyntax class.
 *
 * Represents the ambiguous syntaxes for refering to a type in an
 * \a expression like a \c sizeof or in a \a _Alignas \a specifier.
 * Such ambiguous reference to a type may be:
 * - a parenthesized \a type-name;
 * - a parenthesized \a expression.
 * (Both with a single \a identifier enclosed in parenthesis.)
 *
 * \code
 * _Alignas (x)
 * sizeof (x)
 * \endcode
 */
class PSY_C_API AmbiguousTypeNameOrExpressionAsTypeReferenceSyntax final : public TypeReferenceSyntax
{
    AST_NODE_1K(AmbiguousTypeNameOrExpressionAsTypeReference, TypeReference)

public:
    const ExpressionAsTypeReferenceSyntax* expressionAsTypeReference() const { return exprAsTyRef_; }
    const TypeNameAsTypeReferenceSyntax* typeNameAsTypeReference() const { return tyNameAsTyRef_; }

private:
    ExpressionAsTypeReferenceSyntax* exprAsTyRef_ = nullptr;
    TypeNameAsTypeReferenceSyntax* tyNameAsTyRef_ = nullptr;
    AST_CHILD_LST2(exprAsTyRef_, tyNameAsTyRef_);
};

} // C
} // psy

#endif
