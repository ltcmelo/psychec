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

#ifndef PSYCHE_C_SYNTAX_NODES_STATEMENTS_H__
#define PSYCHE_C_SYNTAX_NODES_STATEMENTS_H__

#include "SyntaxNode.h"
#include "SyntaxToken.h"
#include "SyntaxTree.h"
#include "SyntaxVisitor.h"

#include "parser/LexedTokens.h"

namespace psy {
namespace C {

//------------//
// Statements //
//------------//
/**
 * \brief The LabeledStatementSyntax class.
 *
 * \remark 6.8.1
 *
 * \note Similar to:
 * - \c clang::syntax::LabelStmt of Clang's Libtooling.
 * - \c Microsoft.CodeAnalysis.CSharp.Syntax.LabeledStatementSyntax of Roslyn.
 */
class PSY_C_API LabeledStatementSyntax final : public StatementSyntax
{
    AST_NODE_NK(LabeledStatement, Statement)

public:
    SyntaxToken labelToken() const { return tokenAtIndex(labelTkIdx_); }
    const ExpressionSyntax* expression() const { return expr_; }
    SyntaxToken colonToken() const { return tokenAtIndex(colonTkIdx_); }
    const StatementSyntax* statement() const { return stmt_; }

private:
    LexedTokens::IndexType labelTkIdx_ = LexedTokens::invalidIndex();
    ExpressionSyntax* expr_ = nullptr;
    LexedTokens::IndexType colonTkIdx_ = LexedTokens::invalidIndex();
    StatementSyntax* stmt_ = nullptr;
    AST_CHILD_LST4(labelTkIdx_,
                   expr_,
                   colonTkIdx_,
                   stmt_)
};

/**
 * \brief The CompoundStatementSyntax class.
 *
 * \remark 6.8.2
 *
 * \note Similar to:
 * - \c clang::CompoundStmt of LLVM/Clang.
 * - \c clang::syntax::CompoundStatement of Clang's Libtooling.
 */
class PSY_C_API CompoundStatementSyntax final : public StatementSyntax
{
    AST_NODE_1K(CompoundStatement, Statement)

public:
    SyntaxToken openBraceToken() const { return tokenAtIndex(openBraceTkIdx_); }
    const StatementListSyntax* statements() const { return stmts_; }
    SyntaxToken closeBraceToken() const { return tokenAtIndex(closeBraceTkIdx_); }

private:
    LexedTokens::IndexType openBraceTkIdx_ = LexedTokens::invalidIndex();
    StatementListSyntax* stmts_ = nullptr;
    LexedTokens::IndexType closeBraceTkIdx_ = LexedTokens::invalidIndex();
    AST_CHILD_LST3(openBraceTkIdx_,
                   stmts_,
                   closeBraceTkIdx_)
};

/**
 * \brief The DeclarationStatementSyntax class.
 *
 * Adapts a \a declaration into an \a statement in order to hold a
 * DeclarationSyntax within a CompoundStatementSyntax.
 *
 * \remark 6.8.3
 *
 * \note Similar to:
 * - \c clang::DeclStmt of LLVM/Clang.
 * - \c clang::syntax::DeclarationStatement of Clang's Libtooling.
 * - \c Microsoft.CodeAnalysis.CSharp.Syntax.LocalDeclarationStatementSyntax of Roslyn.
 */
class PSY_C_API DeclarationStatementSyntax final : public StatementSyntax
{
    AST_NODE_1K(DeclarationStatement, Statement)

public:
    const DeclarationSyntax* declaration() const { return decl_; }

private:
    DeclarationSyntax* decl_ = nullptr;
    AST_CHILD_LST1(decl_)
};

/**
 * \brief The ExpressionStatementSyntax class.
 *
 * \remark 6.8.3
 *
 * \note Similar to:
 * - \c clang::syntax::ExpressionStatement of Clang's Libtooling.
 * - \c Microsoft.CodeAnalysis.CSharp.Syntax.ExpressionStatementSyntax of Roslyn.
 */
class PSY_C_API ExpressionStatementSyntax : public StatementSyntax
{
    AST_NODE_1K(ExpressionStatement, Statement)

public:
    const ExpressionSyntax* expression() const { return expr_; }
    SyntaxToken semicolonToken() const { return tokenAtIndex(semicolonTkIdx_); }

private:
    ExpressionSyntax* expr_ = nullptr;
    LexedTokens::IndexType semicolonTkIdx_ = LexedTokens::invalidIndex();
    AST_CHILD_LST2(expr_, semicolonTkIdx_)
};

/**
 * \brief The AmbiguousExpressionOrDeclarationStatementSyntax class.
 *
 * Represents the ambiguous syntaxes \a declaration-statement and
 * \a expression-statement.
 *
 * The call expression or variable declaration:
 * \code
 * x ( y ) ;
 * \endcode
 *
 * The multiply expression or pointer declaration:
 * \code
 * x * y ;
 * \endcode
 */
class PSY_C_API AmbiguousExpressionOrDeclarationStatementSyntax final : public StatementSyntax
{
    AST_NODE_NK(AmbiguousExpressionOrDeclarationStatement, Statement)

public:
    const DeclarationStatementSyntax* declarationStatement() const { return declStmt_; }
    const ExpressionStatementSyntax* expressionStatement() const { return exprStmt_; }

private:
    DeclarationStatementSyntax* declStmt_ = nullptr;
    ExpressionStatementSyntax* exprStmt_ = nullptr;
    AST_CHILD_LST2(declStmt_, exprStmt_)
};

/**
 * \brief The IfStatementSyntax class.
 *
 * \remark 6.8.4.1
 *
 * \note Similar to:
 * - \c clang::IfStmt of LLVM/Clang.
 * - \c clang::syntax::IfStatement of Clang's Libtooling.
 * - \c Microsoft.CodeAnalysis.CSharp.Syntax.ExpressionStatementSyntax of Roslyn.
 */
class PSY_C_API IfStatementSyntax final : public StatementSyntax
{
    AST_NODE_1K(IfStatement, Statement)

public:
    SyntaxToken ifKeyword() const { return tokenAtIndex(ifKwTkIdx_); }
    SyntaxToken openParenthesisToken() const { return tokenAtIndex(openParenTkIdx_); }
    ExpressionSyntax* condition() const { return cond_; }
    SyntaxToken closeParenthesisToken() const { return tokenAtIndex(closeParenTkIdx_); }
    StatementSyntax* statement() const { return stmt_; }
    SyntaxToken elseKeyword() const { return tokenAtIndex(elseKwTkIdx_); }
    StatementSyntax* elseStatement() const { return elseStmt_; }

public:
    LexedTokens::IndexType ifKwTkIdx_ = LexedTokens::invalidIndex();
    LexedTokens::IndexType openParenTkIdx_ = LexedTokens::invalidIndex();
    ExpressionSyntax* cond_ = nullptr;
    LexedTokens::IndexType closeParenTkIdx_ = LexedTokens::invalidIndex();
    StatementSyntax* stmt_ = nullptr;
    LexedTokens::IndexType elseKwTkIdx_ = LexedTokens::invalidIndex();
    StatementSyntax* elseStmt_ = nullptr;
    AST_CHILD_LST7(ifKwTkIdx_,
                   openParenTkIdx_,
                   cond_,
                   closeParenTkIdx_,
                   stmt_,
                   elseKwTkIdx_,
                   elseStmt_)
};

/**
 * \brief The SwitchStatementSyntax class.
 *
 * \remark 6.8.4.2
 *
 * \note Similar to:
 * - \c clang::SwitchStmt of LLVM/Clang.
 * - \c clang::syntax::SwitchStatement of Clang's Libtooling.
 * - \c Microsoft.CodeAnalysis.CSharp.Syntax.SwitchStatementSyntax of Roslyn.
 */
class PSY_C_API SwitchStatementSyntax final : public StatementSyntax
{
    AST_NODE_1K(SwitchStatement, Statement)

public:
    SyntaxToken switchKeyword() const { return tokenAtIndex(switchKwTkIdx_); }
    SyntaxToken openParenthesisToken() const { return tokenAtIndex(openParenTkIdx_); }
    const ExpressionSyntax* condition() const { return cond_; }
    SyntaxToken closeParenthesisToken() const { return tokenAtIndex(closeParenTkIdx_); }
    const StatementSyntax* statement() const { return stmt_; }

public:
    LexedTokens::IndexType switchKwTkIdx_ = LexedTokens::invalidIndex();
    LexedTokens::IndexType openParenTkIdx_ = LexedTokens::invalidIndex();
    ExpressionSyntax* cond_ = nullptr;
    LexedTokens::IndexType closeParenTkIdx_ = LexedTokens::invalidIndex();
    StatementSyntax* stmt_  = nullptr;
    AST_CHILD_LST5(switchKwTkIdx_,
                   openParenTkIdx_,
                   cond_,
                   closeParenTkIdx_,
                   stmt_)
};

/**
 * \brief The WhileStatementSyntax class.
 *
 * \remark 6.8.5.1
 *
 * \note Similar to:
 * - \c clang::WhileStmt of LLVM/Clang.
 * - \c clang::syntax::WhileStatement of Clang's Libtooling.
 * - \c Microsoft.CodeAnalysis.CSharp.Syntax.WhileStatementSyntax of Roslyn.
 */
class PSY_C_API WhileStatementSyntax final : public StatementSyntax
{
    AST_NODE_1K(WhileStatement, Statement)

public:
    SyntaxToken whileKeyword() const { return tokenAtIndex(whileKwTkIdx_); }
    SyntaxToken openParenthesisToken() const { return tokenAtIndex(openParenTkIdx_); }
    const ExpressionSyntax* condition() const { return cond_; }
    SyntaxToken closeParenthesisToken() const { return tokenAtIndex(closeParenTkIdx_); }
    const StatementSyntax* statement() const { return stmt_; }

private:
    LexedTokens::IndexType whileKwTkIdx_ = LexedTokens::invalidIndex();
    LexedTokens::IndexType openParenTkIdx_ = LexedTokens::invalidIndex();
    ExpressionSyntax* cond_ = nullptr;
    LexedTokens::IndexType closeParenTkIdx_ = LexedTokens::invalidIndex();
    StatementSyntax* stmt_ = nullptr;
    AST_CHILD_LST5(whileKwTkIdx_,
                   openParenTkIdx_,
                   cond_,
                   closeParenTkIdx_,
                   stmt_)
};

/**
 * \brief The DoStatementSyntax class.
 *
 * \remark 6.8.5.2
 *
 * \note Similar to:
 * - \c clang::DoStmt of LLVM/Clang.
 * - \c Microsoft.CodeAnalysis.CSharp.Syntax.DoStatementSyntax of Roslyn.
 */
class PSY_C_API DoStatementSyntax final : public StatementSyntax
{
    AST_NODE_1K(DoStatement, Statement)

public:
    SyntaxToken doKeyword() const { return tokenAtIndex(doKwTkIdx_); }
    const StatementSyntax* statement() const { return stmt_; }
    SyntaxToken whileKeyword() const { return tokenAtIndex(whileKwTkIdx_); }
    SyntaxToken openParenthesisToken() const { return tokenAtIndex(openParenTkIdx_); }
    const ExpressionSyntax* condition() const { return cond_; }
    SyntaxToken closeParenthesisToken() const { return tokenAtIndex(closeParenTkIdx_); }
    SyntaxToken semicolonToken() const { return tokenAtIndex(semicolonTkIdx_); }

private:
    LexedTokens::IndexType doKwTkIdx_ = LexedTokens::invalidIndex();
    StatementSyntax* stmt_ = nullptr;
    LexedTokens::IndexType whileKwTkIdx_ = LexedTokens::invalidIndex();
    LexedTokens::IndexType openParenTkIdx_ = LexedTokens::invalidIndex();
    ExpressionSyntax* cond_ = nullptr;
    LexedTokens::IndexType closeParenTkIdx_ = LexedTokens::invalidIndex();
    LexedTokens::IndexType semicolonTkIdx_ = LexedTokens::invalidIndex();
    AST_CHILD_LST7(doKwTkIdx_,
                   stmt_,
                   whileKwTkIdx_,
                   openParenTkIdx_,
                   cond_,
                   closeParenTkIdx_,
                   semicolonTkIdx_)
};

/**
 * \brief The ForStatementSyntax class.
 *
 * \remark 6.8.5.3
 *
 * \note Similar to:
 * - \c clang::ForStmt of LLVM/Clang.
 * - \c clang::syntax::ForStatement of Clang's Libtooling.
 * - \c Microsoft.CodeAnalysis.CSharp.Syntax.ForStatementSyntax of Roslyn.
 */
class PSY_C_API ForStatementSyntax final : public StatementSyntax
{
    AST_NODE_1K(ForStatement, Statement)

public:
    SyntaxToken forKeyword() const { return tokenAtIndex(forKwTkIdx_); }
    SyntaxToken openParenthesisToken() const { return tokenAtIndex(openParenTkIdx_); }
    SyntaxToken extensionKeyword() const { return tokenAtIndex(extKwTkIdx_); }
    const StatementSyntax* initializer() const { return initStmt_; }
    const ExpressionSyntax* condition() const { return cond_; }
    SyntaxToken semicolonToken() const { return tokenAtIndex(semicolonTkIdx_); }
    const ExpressionSyntax* expression() const { return expr_; }
    SyntaxToken closeParenthesisToken() const { return tokenAtIndex(closeParenTkIdx_); }
    const StatementSyntax* statement() const { return stmt_; }

private:
    LexedTokens::IndexType forKwTkIdx_ = LexedTokens::invalidIndex();
    LexedTokens::IndexType openParenTkIdx_ = LexedTokens::invalidIndex();
    LexedTokens::IndexType extKwTkIdx_ = LexedTokens::invalidIndex();
    StatementSyntax* initStmt_ = nullptr;
    ExpressionSyntax* cond_ = nullptr;
    LexedTokens::IndexType semicolonTkIdx_ = LexedTokens::invalidIndex();
    ExpressionSyntax* expr_ = nullptr;
    LexedTokens::IndexType closeParenTkIdx_ = LexedTokens::invalidIndex();
    StatementSyntax* stmt_ = nullptr;
    AST_CHILD_LST9(forKwTkIdx_,
                   openParenTkIdx_,
                   extKwTkIdx_,
                   initStmt_,
                   cond_,
                   semicolonTkIdx_,
                   expr_,
                   closeParenTkIdx_,
                   stmt_)
};

/**
 * \brief The GotoStatementSyntax class.
 *
 * \remark 6.8.6.1
 *
 * \note Similar to:
 * - \c clang::GotoStmt of LLVM/Clang.
 * - \c Microsoft.CodeAnalysis.CSharp.Syntax.GotoStatementSyntax of Roslyn.
 */
class PSY_C_API GotoStatementSyntax final : public StatementSyntax
{
    AST_NODE_1K(GotoStatement, Statement)

public:
    SyntaxToken gotoKeyword() const { return tokenAtIndex(gotoKwTkIdx_); }
    SyntaxToken identifierToken() const { return tokenAtIndex(identTkIdx_); }
    SyntaxToken semicolonToken() const { return tokenAtIndex(semicolonTkIdx_); }

private:
    LexedTokens::IndexType gotoKwTkIdx_ = LexedTokens::invalidIndex();
    LexedTokens::IndexType identTkIdx_ = LexedTokens::invalidIndex();
    LexedTokens::IndexType semicolonTkIdx_ = LexedTokens::invalidIndex();
};

/**
 * \brief The ContinueStatementSyntax class.
 *
 * \remark 6.8.6.2
 *
 * \note Similar to:
 * - \c clang::ContinueStmt of LLVM/Clang.
 * - \c clang::syntax::ContinueStatement of Clang's Libtooling.
 * - \c Microsoft.CodeAnalysis.CSharp.Syntax.ContinueStatementSyntax of Roslyn.
 */
class PSY_C_API ContinueStatementSyntax final : public StatementSyntax
{
    AST_NODE_1K(ContinueStatement, Statement)

public:
    SyntaxToken continueKeyword() const { return tokenAtIndex(continueKwTkIdx_); }
    SyntaxToken semicolonToken() const { return tokenAtIndex(semicolonTkIdx_); }

public:
    LexedTokens::IndexType continueKwTkIdx_ = LexedTokens::invalidIndex();
    LexedTokens::IndexType semicolonTkIdx_ = LexedTokens::invalidIndex();
    AST_CHILD_LST2(continueKwTkIdx_,
                   semicolonTkIdx_)
};

/**
 * \brief The BreakStatementSyntax class.
 *
 * \remark 6.8.6.3
 *
 * \note Similar to:
 * - \c clang::BreakStmt of LLVM/Clang.
 * - \c clang::syntax::BreakStatement of Clang's Libtooling.
 * - \c Microsoft.CodeAnalysis.CSharp.Syntax.BreakStatementSyntax of Roslyn.
 */
class PSY_C_API BreakStatementSyntax final : public StatementSyntax
{
    AST_NODE_1K(BreakStatement, Statement)

public:
    SyntaxToken breakKeyword() const { return tokenAtIndex(breakKwTkIdx_); }
    SyntaxToken semicolonToken() const { return tokenAtIndex(semicolonTkIdx_); }

private:
    LexedTokens::IndexType breakKwTkIdx_ = LexedTokens::invalidIndex();
    LexedTokens::IndexType semicolonTkIdx_ = LexedTokens::invalidIndex();
    AST_CHILD_LST2(breakKwTkIdx_,
                   semicolonTkIdx_)
};

/**
 * \brief The ReturnStatementSyntax class.
 *
 * \remark 6.8.6.4
 *
 * \note Similar to:
 * - \c clang::ReturnStmt of LLVM/Clang.
 * - \c clang::syntax::ReturnStatement of Clang's Libtooling.
 * - \c Microsoft.CodeAnalysis.CSharp.Syntax.ReturnStatementSyntax of Roslyn.
 */
class PSY_C_API ReturnStatementSyntax final : public StatementSyntax
{
    AST_NODE_1K(ReturnStatement, Statement)

public:
    SyntaxToken returnKeyword() const { return tokenAtIndex(returnKwTkIdx_); }
    const ExpressionSyntax* expression() const { return expr_; }
    SyntaxToken semicolonToken() const { return tokenAtIndex(semicolonTkIdx_); }

public:
    LexedTokens::IndexType returnKwTkIdx_ = LexedTokens::invalidIndex();
    ExpressionSyntax* expr_ = nullptr;
    LexedTokens::IndexType semicolonTkIdx_ = LexedTokens::invalidIndex();
    AST_CHILD_LST3(returnKwTkIdx_,
                   expr_,
                   semicolonTkIdx_)
};

/**
 * \brief The ExtGNU_AsmOperandSyntax class.
 */
class PSY_C_API ExtGNU_AsmOperandSyntax final : public SyntaxNode
{
    AST_G_NODE_NK(ExtGNU_AsmOperand)

public:
    SyntaxToken openBracketToken() const { return tokenAtIndex(openBracketTkIdx_); }
    const ExpressionSyntax* identifier() const { return identExpr_; }
    SyntaxToken closeBracketToken() const { return tokenAtIndex(closeBracketTkIdx_); }
    const ExpressionSyntax* stringLiteral() const { return strLit_; }
    SyntaxToken openParenthesisToken() const { return tokenAtIndex(openParenTkIdx_); }
    const ExpressionSyntax* expression() const { return expr_; }
    SyntaxToken closeParenthesisToken() const { return tokenAtIndex(closeParenTkIdx_); }

private:
    LexedTokens::IndexType openBracketTkIdx_ = LexedTokens::invalidIndex();
    ExpressionSyntax* identExpr_ = nullptr;
    LexedTokens::IndexType closeBracketTkIdx_ = LexedTokens::invalidIndex();
    ExpressionSyntax* strLit_ = nullptr;
    LexedTokens::IndexType openParenTkIdx_ = LexedTokens::invalidIndex();
    ExpressionSyntax* expr_ = nullptr;
    LexedTokens::IndexType closeParenTkIdx_ = LexedTokens::invalidIndex();
};

/**
 * \brief The ExtGNU_AsmStatementSyntax class.
 *
 * \remark J.5.10
 *
 * \attention Thisis a GNU extension:
 * https://gcc.gnu.org/onlinedocs/gcc/Extended-Asm.html#AssemblerTemplate
 *
 * \see ExtGNU_AsmStatementDeclarationSyntax
 */
class PSY_C_API ExtGNU_AsmStatementSyntax final : public StatementSyntax
{
    AST_NODE_1K(ExtGNU_AsmStatement, Statement)

public:
    SyntaxToken asmKeyword() const { return tokenAtIndex(asmKwTkIdx_); }
    const SpecifierListSyntax* asmQualifiers() const { return asmQuals_; }
    SyntaxToken openParenthesisToken() const { return tokenAtIndex(openParenTkIdx_); }
    const ExpressionSyntax* stringLiteral() const { return strLit_; }
    SyntaxToken colon1Token() const { return tokenAtIndex(colon1TkIdx_); }
    const ExtGNU_AsmOperandListSyntax* outputOperands() const { return outOprds_; }
    SyntaxToken colon2Token() const { return tokenAtIndex(colon2TkIdx_); }
    const ExtGNU_AsmOperandListSyntax* inputOperands() const { return inOprds_; }
    SyntaxToken colon3Token() const { return tokenAtIndex(colon3TkIdx_); }
    const ExpressionListSyntax* clobbers() const { return clobs_; }
    SyntaxToken colon4Token() const { return tokenAtIndex(colon4TkIdx_); }
    const ExpressionListSyntax* gotoLabels() const { return labels_; }
    SyntaxToken closeParenthesisToken() const { return tokenAtIndex(closeParenTkIdx_); }
    SyntaxToken semicolonToken() const { return tokenAtIndex(semicolonTkIdx_); }

private:
    LexedTokens::IndexType asmKwTkIdx_ = LexedTokens::invalidIndex();
    SpecifierListSyntax* asmQuals_ = nullptr;
    LexedTokens::IndexType openParenTkIdx_ = LexedTokens::invalidIndex();
    ExpressionSyntax* strLit_ = nullptr;
    LexedTokens::IndexType colon1TkIdx_ = LexedTokens::invalidIndex();
    ExtGNU_AsmOperandListSyntax* outOprds_ = nullptr;
    LexedTokens::IndexType colon2TkIdx_ = LexedTokens::invalidIndex();
    ExtGNU_AsmOperandListSyntax* inOprds_ = nullptr;
    LexedTokens::IndexType colon3TkIdx_ = LexedTokens::invalidIndex();
    ExpressionListSyntax* clobs_ = nullptr;
    LexedTokens::IndexType colon4TkIdx_ = LexedTokens::invalidIndex();
    ExpressionListSyntax* labels_ = nullptr;
    LexedTokens::IndexType closeParenTkIdx_ = LexedTokens::invalidIndex();
    LexedTokens::IndexType semicolonTkIdx_ = LexedTokens::invalidIndex();

};

class PSY_C_API ExtGNU_AsmQualifierSyntax final : public TrivialSpecifierSyntax
{
    AST_NODE_NK(ExtGNU_AsmQualifier, TrivialSpecifier)

public:
    SyntaxToken asmQualifier() const { return tokenAtIndex(specTkIdx_); }
};

} // C
} // psy

#endif
