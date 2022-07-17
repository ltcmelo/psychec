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

#ifndef PSYCHE_C_SYNTAX_NODES_H__
#define PSYCHE_C_SYNTAX_NODES_H__

#include "SyntaxNode.h"
#include "SyntaxNodes_MIXIN.h"
#include "SyntaxToken.h"
#include "SyntaxTree.h"

#define AST__COMMON__(NODE, BASE_NODE) \
    friend class Parser; \
    friend class Disambiguator; \
    friend class Binder; \
    public: \
        virtual NODE##Syntax* as##NODE() override { return this; } \
        virtual const NODE##Syntax* as##NODE() const override { return this; }

/*
 * There are 2 coarse variations of AST nodes:
 *
 *   1 - those that inherit directly from `SyntaxNode', creating a syntactic
 *       category (6.1-1) for the syntax in question;
 *   2 - those that belong to an specific category.
 *
 * Macros prefixed with `AST_G_NODE' apply on 1, while macros prefixed
 * with `AST_NODE' apply on 2.
 */
#define AST_G_NODE__COMMON__(NODE) \
    AST__COMMON__(NODE,) \
    using BaseSyntax = SyntaxNode;

#define AST_NODE__COMMON__(NODE, BASE_NODE) \
    AST__COMMON__(NODE, BASE_NODE) \
    using BaseSyntax = BASE_NODE##Syntax;

/*
 * The class of an AST node further varies in respect to whether:
 *
 *  - it serves exclusively as base type (not visited during AST traversal);
 *  - it is a type for which a single concrete syntax kind exists;
 *  - it is a type for which multiple concrete syntax kinds exist.
 *
 * In the last case, the construct for the AST node in question requires
 * an specific `SyntaxKind' argument.
 */
#define AST_G_NODE(NODE) \
    AST_G_NODE__COMMON__(NODE) \
    using SyntaxNode::SyntaxNode;
#define AST_G_NODE_1K(NODE) \
    AST_G_NODE__COMMON__(NODE) \
    NODE##Syntax(SyntaxTree* tree) : SyntaxNode(tree, NODE) {} \
    DISPATCH_VISIT(NODE)
#define AST_G_NODE_NK(NODE) \
    AST_G_NODE__COMMON__(NODE) \
    NODE##Syntax(SyntaxTree* tree, SyntaxKind kind) : SyntaxNode(tree, kind) {} \
    DISPATCH_VISIT(NODE)

#define AST_NODE(NODE, BASE_NODE) \
    AST_NODE__COMMON__(NODE, BASE_NODE) \
    using BASE_NODE##Syntax::BASE_NODE##Syntax;
#define AST_NODE_1K(NODE, BASE_NODE) \
    AST_NODE__COMMON__(NODE, BASE_NODE) \
    NODE##Syntax(SyntaxTree* tree) : BASE_NODE##Syntax(tree, NODE) {} \
    DISPATCH_VISIT(NODE)
#define AST_NODE_NK(NODE, BASE_NODE) \
    AST_NODE__COMMON__(NODE, BASE_NODE) \
    NODE##Syntax(SyntaxTree* tree, SyntaxKind kind) : BASE_NODE##Syntax(tree, kind) {} \
    DISPATCH_VISIT(NODE)

/*
 * The children, either nodes or tokens, of an AST node.
 */
#define AST_CHILD_LST1(NAME1) \
    CHILD_NODES_AND_TOKENS(CHILD_NAME_1(NAME1))
#define AST_CHILD_LST2(NAME1, NAME2) \
    CHILD_NODES_AND_TOKENS(CHILD_NAME_2(NAME1, NAME2))
#define AST_CHILD_LST3(NAME1, NAME2, NAME3) \
    CHILD_NODES_AND_TOKENS(CHILD_NAME_3(NAME1, NAME2, NAME3))
#define AST_CHILD_LST4(NAME1, NAME2, NAME3, NAME4) \
    CHILD_NODES_AND_TOKENS(CHILD_NAME_4(NAME1, NAME2, NAME3, NAME4))
#define AST_CHILD_LST5(NAME1, NAME2, NAME3, NAME4, NAME5) \
    CHILD_NODES_AND_TOKENS(CHILD_NAME_5(NAME1, NAME2, NAME3, NAME4, NAME5))
#define AST_CHILD_LST6(NAME1, NAME2, NAME3, NAME4, NAME5, NAME6) \
    CHILD_NODES_AND_TOKENS(CHILD_NAME_6(NAME1, NAME2, NAME3, NAME4, NAME5, NAME6))
#define AST_CHILD_LST7(NAME1, NAME2, NAME3, NAME4, NAME5, NAME6, NAME7) \
    CHILD_NODES_AND_TOKENS(CHILD_NAME_7(NAME1, NAME2, NAME3, NAME4, NAME5, NAME6, NAME7))
#define AST_CHILD_LST8(NAME1, NAME2, NAME3, NAME4, NAME5, NAME6, NAME7, NAME8) \
    CHILD_NODES_AND_TOKENS(CHILD_NAME_8(NAME1, NAME2, NAME3, NAME4, NAME5, NAME6, NAME7, NAME8))
#define AST_CHILD_LST9(NAME1, NAME2, NAME3, NAME4, NAME5, NAME6, NAME7, NAME8, NAME9) \
    CHILD_NODES_AND_TOKENS(CHILD_NAME_9(NAME1, NAME2, NAME3, NAME4, NAME5, NAME6, NAME7, NAME8, NAME9))

#define CHILD_NAME_1(NAME1) \
    SyntaxHolder(NAME1)
#define CHILD_NAME_2(NAME1, NAME2) \
    CHILD_NAME_1(NAME1), \
    SyntaxHolder(NAME2)
#define CHILD_NAME_3(NAME1, NAME2, NAME3) \
    CHILD_NAME_2(NAME1, NAME2), \
    SyntaxHolder(NAME3)
#define CHILD_NAME_4(NAME1, NAME2, NAME3, NAME4) \
    CHILD_NAME_3(NAME1, NAME2, NAME3), \
    SyntaxHolder(NAME4)
#define CHILD_NAME_5(NAME1, NAME2, NAME3, NAME4, NAME5) \
    CHILD_NAME_4(NAME1, NAME2, NAME3, NAME4), \
    SyntaxHolder(NAME5)
#define CHILD_NAME_6(NAME1, NAME2, NAME3, NAME4, NAME5, NAME6) \
    CHILD_NAME_5(NAME1, NAME2, NAME3, NAME4, NAME5), \
    SyntaxHolder(NAME6)
#define CHILD_NAME_7(NAME1, NAME2, NAME3, NAME4, NAME5, NAME6, NAME7) \
    CHILD_NAME_6(NAME1, NAME2, NAME3, NAME4, NAME5, NAME6), \
    SyntaxHolder(NAME7)
#define CHILD_NAME_8(NAME1, NAME2, NAME3, NAME4, NAME5, NAME6, NAME7, NAME8) \
    CHILD_NAME_7(NAME1, NAME2, NAME3, NAME4, NAME5, NAME6, NAME7), \
    SyntaxHolder(NAME8)
#define CHILD_NAME_9(NAME1, NAME2, NAME3, NAME4, NAME5, NAME6, NAME7, NAME8, NAME9) \
    CHILD_NAME_8(NAME1, NAME2, NAME3, NAME4, NAME5, NAME6, NAME7, NAME8), \
    SyntaxHolder(NAME9)

/*
 * The default implementation of the visitor dispatching function for
 * vising the `this' node.
 */
#define DISPATCH_VISIT(NODE) \
    protected: \
        virtual SyntaxVisitor::Action dispatchVisit(SyntaxVisitor* visitor) const override \
            { return visitor->visit##NODE(this); }

/*
 * The default implementation of the function that gather the child
 * nodes and tokens of the `this' node.
 */
#define CHILD_NODES_AND_TOKENS(CHILDREN_SYNTAX) \
    public: \
        virtual std::vector<SyntaxHolder> childNodesAndTokens() const override \
            { auto self = { CHILDREN_SYNTAX }; \
              return merge(BaseSyntax::childNodesAndTokens(), self); }

using namespace psy;
using namespace C;

inline std::vector<SyntaxHolder> merge(std::vector<SyntaxHolder>&& a,
                                       std::vector<SyntaxHolder>&& b)
{
    a.insert(a.end(), b.begin(), b.end());
    return std::move(a);
}

namespace psy {
namespace C {


/**
 * \brief The DeclarationSyntax class.
 *
 * The base class of every \a declaration.
 *
 * \remark 6.7
 *
 * \note Similar to:
 * - \c clang::Decl of LLVM/Clang.
 * - \c Microsoft.CodeAnalysis.CSharp.Syntax.MemberDeclarationSyntax and
 * - \c Microsoft.CodeAnalysis.CSharp.Syntax.VariableDeclarationSyntax of Roslyn.
 */
class PSY_C_API DeclarationSyntax : public SyntaxNode
                                  , public MIXIN_GNUExtensionFlag
{
    AST_G_NODE(Declaration)

public:
    SyntaxToken extensionKeyword() const override { return tokenAtIndex(extKwTkIdx_); }

private:
    AST_CHILD_LST1(extKwTkIdx_);
};

/**
 * \brief The SpecifierSyntax class.
 *
 * The base class of every \a declaration-specifier.
 *
 * \remark 6.7.1, 6.7.2, 6.7.3, 6.7.4, and 6.7.5.
 */
class PSY_C_API SpecifierSyntax : public SyntaxNode
{
    AST_G_NODE(Specifier)
};

/**
 * \brief The DeclaratorSyntax class.
 *
 * The base class of every \a declarator.
 *
 * \remark 6.7.6
 *
 * \note Similar to:
 * - \c clang::Declarator of LLVM/Clang.
 * - \c clang::syntax::Declarator of Clang's Libtooling.
 */
class PSY_C_API DeclaratorSyntax : public SyntaxNode
{
    AST_G_NODE(Declarator)
};

/**
 * \brief The DeclaratorSuffixSyntax class.
 *
 * The base class of every \a declarator suffix.
 *
 * \remark 6.7.6
 */
class PSY_C_API DeclaratorSuffixSyntax : public SyntaxNode
{
    AST_G_NODE(DeclaratorSuffix)
};

/**
 * \brief The InitializerSyntax class.
 *
 * \remark 6.7.9
 *
 * \note Similar to:
 * - \c Microsoft.CodeAnalysis.CSharp.Syntax.InitializerExpressionSyntax of Roslyn.
 */
class PSY_C_API InitializerSyntax : public SyntaxNode
{
    AST_G_NODE(Initializer)
};

/**
 * \brief The ExpressionSyntax class.
 *
 * The base class of every \a expression.
 *
 * The type hierarchy of ExpressionSyntax generally corresponds to the
 * grammar of expressions, but there are exceptions to this correspondence.
 * Consider the prefix and postfix variations of the increment/decrement
 * operations: both consist of one operand and one operator, however,
 * the former is a sentence of a \a unary-expression and the latter,
 * of \a postfix-expression (an asymmetry that is observed in the parser
 * as well, see note in Parser:parseExpression); for the AST, we opt for
 * a structure that is closer to the meaning of the constructs in the language
 * semantics, regardless of the grammar non-terminal that derived them.
 *
 * \remark 6.5
 *
 * \note Similar to:
 * - \c clang::Expr of LLVM/Clang.
 * - \c clang::syntax::Expression of Clang's Libtooling.
 * - \c Microsoft.CodeAnalysis.CSharp.Syntax.ExpressionSyntax of Roslyn.
 */
class PSY_C_API ExpressionSyntax : public SyntaxNode
                                 , public MIXIN_GNUExtensionFlag
{
    AST_G_NODE(Expression)

public:
    SyntaxToken extensionKeyword() const override { return tokenAtIndex(extKwTkIdx_); }

private:
    AST_CHILD_LST1(extKwTkIdx_);
};

/**
 * \brief The StatementSyntax class: The base class for all statements.
 */
class PSY_C_API StatementSyntax : public SyntaxNode
{
    AST_G_NODE(Statement)
};

/**
 * \brief The TypeReferenceSyntax class.
 */
class PSY_C_API TypeReferenceSyntax : public SyntaxNode
{
    AST_G_NODE(TypeReference)
};

} // C
} // psy

#include "SyntaxNodes_Common.h"
#include "SyntaxNodes_Declarations.h"
#include "SyntaxNodes_Expressions.h"
#include "SyntaxNodes_Statements.h"

#undef AST__COMMON__
#undef AST_G_NODE__COMMON__
#undef AST_NODE__COMMON__
#undef AST_G_NODE
#undef AST_G_NODE_1K
#undef AST_G_NODE_NK
#undef AST_NODE
#undef AST_NODE_1K
#undef AST_NODE_NK

#undef AST_CHILD_LST1
#undef AST_CHILD_LST2
#undef AST_CHILD_LST3
#undef AST_CHILD_LST4
#undef AST_CHILD_LST5
#undef AST_CHILD_LST6
#undef AST_CHILD_LST7
#undef AST_CHILD_LST8
#undef CHILD_NAME_1
#undef CHILD_NAME_2
#undef CHILD_NAME_3
#undef CHILD_NAME_4
#undef CHILD_NAME_5
#undef CHILD_NAME_6
#undef CHILD_NAME_7
#undef CHILD_NAME_8

#undef DISPATCH_VISIT
#undef CHILD_NODES_AND_TOKENS

#endif
