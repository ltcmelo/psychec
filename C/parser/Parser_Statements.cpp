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

#include "Parser__IMPL__.inc"

using namespace psy;
using namespace C;

//------------//
// Statements //
//------------//

/**
 * Parse a \a statement.
 *
 * \remark 6.8
 */
bool Parser::parseStatement(StatementSyntax*& stmt, StatementContext stmtCtx)
{
    DEBUG_THIS_RULE();

    switch (peek().kind()) {
        case SyntaxKind::Keyword__Static_assert:
            return parseDeclarationStatement(
                        stmt,
                        &Parser::parseStaticAssertDeclaration_AtFirst);

        // storage-class-specifier
        case SyntaxKind::Keyword_typedef:
        case SyntaxKind::Keyword_extern:
        case SyntaxKind::Keyword_static:
        case SyntaxKind::Keyword_auto:
        case SyntaxKind::Keyword_register:
        case SyntaxKind::Keyword__Thread_local:
        case SyntaxKind::Keyword_ExtGNU___thread:

        // type-qualifier
        case SyntaxKind::Keyword_const:
        case SyntaxKind::Keyword_volatile:
        case SyntaxKind::Keyword_restrict:
        case SyntaxKind::Keyword__Atomic:

        // function-specifier
        case SyntaxKind::Keyword_inline:
        case SyntaxKind::Keyword__Noreturn:

        // type-specifier
        case SyntaxKind::Keyword_void:
        case SyntaxKind::Keyword_char:
        case SyntaxKind::Keyword_short:
        case SyntaxKind::Keyword_int:
        case SyntaxKind::Keyword_long:
        case SyntaxKind::Keyword_float:
        case SyntaxKind::Keyword_double:
        case SyntaxKind::Keyword__Bool:
        case SyntaxKind::Keyword__Complex:
        case SyntaxKind::Keyword_signed:
        case SyntaxKind::Keyword_unsigned:
        case SyntaxKind::Keyword_Ext_char16_t:
        case SyntaxKind::Keyword_Ext_char32_t:
        case SyntaxKind::Keyword_Ext_wchar_t:
        case SyntaxKind::Keyword_struct:
        case SyntaxKind::Keyword_union:
        case SyntaxKind::Keyword_enum:
        case SyntaxKind::Keyword_ExtGNU___typeof__:
        case SyntaxKind::Keyword_ExtGNU___complex__:
            return parseDeclarationStatement(
                        stmt,
                        &Parser::parseDeclarationOrFunctionDefinition);

        case SyntaxKind::IdentifierToken: {
            if (peek(2).kind() == SyntaxKind::ColonToken)
                return parseLabeledStatement_AtFirst(stmt, stmtCtx);

            Backtracker BT(this);
            if (!parseExpressionStatement(stmt)) {
                BT.backtrack();
                return parseDeclarationStatement(
                            stmt,
                            &Parser::parseDeclarationOrFunctionDefinition);
            }
            maybeAmbiguateStatement(stmt);
            return true;
        }

        case SyntaxKind::OpenBraceToken:
            return parseCompoundStatement_AtFirst(stmt, stmtCtx);

        case SyntaxKind::Keyword_if:
            return parseIfStatement_AtFirst(stmt, stmtCtx);

        case SyntaxKind::Keyword_switch:
            return parseSwitchStatement_AtFirst(stmt, stmtCtx);

        case SyntaxKind::Keyword_case:
            if (stmtCtx != StatementContext::Switch
                    && stmtCtx != StatementContext::SwitchAndLoop)
                diagReporter_.UnexpectedCaseLabelOutsideSwitch();
            return parseLabeledStatement_AtFirst(stmt, stmtCtx);

        case SyntaxKind::Keyword_default:
            if (stmtCtx != StatementContext::Switch
                    && stmtCtx != StatementContext::SwitchAndLoop)
                diagReporter_.UnexpectedDefaultLabelOutsideSwitch();
            return parseLabeledStatement_AtFirst(stmt, stmtCtx);

        case SyntaxKind::Keyword_while:
            return parseWhileStatement_AtFirst(stmt, stmtCtx);

        case SyntaxKind::Keyword_do:
            return parseDoStatement_AtFirst(stmt, stmtCtx);

        case SyntaxKind::Keyword_for:
            return parseForStatement_AtFirst(stmt, stmtCtx);

        case SyntaxKind::Keyword_goto:
            return parseGotoStatement_AtFirst(stmt);

        case SyntaxKind::Keyword_continue:
            if (stmtCtx != StatementContext::Loop
                    && stmtCtx != StatementContext::SwitchAndLoop)
                diagReporter_.UnexpectedContinueOutsideLoop();
            return parseContinueStatement_AtFirst(stmt, stmtCtx);

        case SyntaxKind::Keyword_break:
            if (stmtCtx == StatementContext::None)
                diagReporter_.UnexpectedBreakOutsideSwitchOrLoop();
            return parseBreakStatement_AtFirst(stmt, stmtCtx);

        case SyntaxKind::Keyword_return:
            return parseReturnStatement_AtFirst(stmt);

        case SyntaxKind::SemicolonToken: {
            auto exprStmt = makeNode<ExpressionStatementSyntax>();
            stmt = exprStmt;
            exprStmt->semicolonTkIdx_ = consume();
            return true;
        }

        case SyntaxKind::Keyword_ExtGNU___asm__:
            return parseExtGNU_AsmStatement(stmt);

        case SyntaxKind::Keyword_ExtGNU___extension__: {
            auto extKwTkIdx = consume();
            if (!parseStatement(stmt, stmtCtx))
                return false;

            PSY_ASSERT_W_MSG(stmt, return false, "invalid declaration");
            switch (stmt->kind()) {
                case SyntaxKind::ExpressionStatement:
                    PSY_ASSERT_W_MSG(stmt->asExpressionStatement(),
                                     return false,
                                     "invalid expression-statement");
                    stmt->asExpressionStatement()->expr_->extKwTkIdx_ = extKwTkIdx;
                    break;

                case SyntaxKind::DeclarationStatement:
                    PSY_ASSERT_W_MSG(stmt->asDeclarationStatement(),
                                     return false,
                                     "invalid expression-statement");
                    stmt->asDeclarationStatement()->decl_->extKwTkIdx_ = extKwTkIdx;
                    break;

                default:
                    diagReporter_.UnexpectedGNUExtensionFlag();
                    return false;
            }
            return true;
        }

        default:
            return parseExpressionStatement(stmt);
    }
}

bool Parser::parseDeclarationStatement(StatementSyntax*& stmt,
                                       bool (Parser::*parseDecl)(DeclarationSyntax*& ))
{
    auto declStmt = makeNode<DeclarationStatementSyntax>();
    stmt = declStmt;
    return ((this)->*(parseDecl))(declStmt->decl_);
}

void Parser::maybeAmbiguateStatement(StatementSyntax*& stmt)
{
    PSY_ASSERT_W_MSG(stmt->kind() == SyntaxKind::ExpressionStatement,
                     return,
                     to_string(stmt->kind()));

    auto exprStmt = stmt->asExpressionStatement();
    auto expr = exprStmt->expr_;
    if (!(expr->kind() == SyntaxKind::MultiplyExpression
            || expr->kind() == SyntaxKind::CallExpression))
        return;

    SyntaxKind stmtK;
    TypedefNameSyntax* typedefName;
    DeclaratorSyntax* decltor;
    switch (expr->kind()) {
        case SyntaxKind::MultiplyExpression: {
            auto binExpr = expr->asBinaryExpression();
            if (binExpr->leftExpr_->kind() != SyntaxKind::IdentifierName)
                return;

            IdentifierDeclaratorSyntax* identDecltor = nullptr;
            std::stack<ExpressionSyntax*> exprs;
            auto expr = binExpr->rightExpr_;
            while (true) {
                switch (expr->kind()) {
                    case SyntaxKind::PointerIndirectionExpression:
                        exprs.push(expr);
                        expr = expr->asPrefixUnaryExpression()->expr_;
                        break;

                    case SyntaxKind::ElementAccessExpression:
                        exprs.push(expr);
                        expr = expr->asArraySubscriptExpression()->expr_;
                        break;

                    case SyntaxKind::IdentifierName:
                        identDecltor = makeNode<IdentifierDeclaratorSyntax>();
                        identDecltor->identTkIdx_ = expr->asIdentifierName()->identTkIdx_;
                        break;

                    default:
                        return;
                }

                if (identDecltor)
                    break;
            }

            typedefName = makeNode<TypedefNameSyntax>();
            typedefName->identTkIdx_ =
                    binExpr->leftExpr_->asIdentifierName()->identTkIdx_;

            DeclaratorSyntax* innerDecltor = identDecltor;
            while (!exprs.empty()) {
                auto expr = exprs.top();
                exprs.pop();
                switch (expr->kind()) {
                    case SyntaxKind::PointerIndirectionExpression: {
                        auto ptrDecltor = makeNode<PointerDeclaratorSyntax>();
                        ptrDecltor->asteriskTkIdx_ = expr->asPrefixUnaryExpression()->oprtrTkIdx_;
                        ptrDecltor->innerDecltor_ = innerDecltor;
                        innerDecltor = ptrDecltor;
                        break;
                    }

                    case SyntaxKind::ElementAccessExpression: {
                        auto arrDecltorSx = makeNode<SubscriptSuffixSyntax>();
                        arrDecltorSx->openBracketTkIdx_ =
                                expr->asArraySubscriptExpression()->openBracketTkIdx_;
                        arrDecltorSx->expr_ = expr->asArraySubscriptExpression()->arg_;
                        arrDecltorSx->closeBracketTkIdx_ =
                                expr->asArraySubscriptExpression()->closeBracketTkIdx_;
                        auto arrDecltor = makeNode<ArrayOrFunctionDeclaratorSyntax>(SyntaxKind::ArrayDeclarator);
                        arrDecltor->asArrayOrFunctionDeclarator()->suffix_ = arrDecltorSx;
                        arrDecltor->innerDecltor_ = innerDecltor;
                        innerDecltor = arrDecltor;
                        break;
                    }

                    default:
                        PSY_ASSERT(false, return);
                }
            }
            auto ptrDecltor = makeNode<PointerDeclaratorSyntax>();
            ptrDecltor->asteriskTkIdx_ = binExpr->oprtrTkIdx_;
            ptrDecltor->innerDecltor_ = innerDecltor;
            decltor = ptrDecltor;

            stmtK = SyntaxKind::AmbiguousMultiplicationOrPointerDeclaration;
            break;
        }

        case SyntaxKind::CallExpression: {
            auto callExpr = expr->asCallExpression();
            if (!(callExpr->expr_->kind() == SyntaxKind::IdentifierName
                    && callExpr->args_
                    && !callExpr->args_->next)) {
                return;
            }

            IdentifierDeclaratorSyntax* identDecltor = nullptr;
            std::stack<ExpressionSyntax*> exprs;
            auto expr = callExpr->args_->value;
            while (true) {
                switch (expr->kind()) {
                    case SyntaxKind::ParenthesizedExpression:
                        exprs.push(expr);
                        expr = expr->asParenthesizedExpression()->expr_;
                        break;

                    case SyntaxKind::IdentifierName:
                        identDecltor = makeNode<IdentifierDeclaratorSyntax>();
                        identDecltor->identTkIdx_ = expr->asIdentifierName()->identTkIdx_;
                        break;

                    default:
                        return;
                }

                if (identDecltor)
                    break;
            }

            typedefName = makeNode<TypedefNameSyntax>();
            typedefName->identTkIdx_ =
                    callExpr->expr_->asIdentifierName()->identTkIdx_;

            DeclaratorSyntax* innerDecltor = identDecltor;
            while (!exprs.empty()) {
                auto expr = exprs.top();
                exprs.pop();
                switch (expr->kind_) {
                    case SyntaxKind::ParenthesizedExpression: {
                        auto parenDecltor = makeNode<ParenthesizedDeclaratorSyntax>();
                        parenDecltor->openParenTkIdx_ =
                                expr->asParenthesizedExpression()->openParenTkIdx_;
                        parenDecltor->closeParenTkIdx_ =
                                expr->asParenthesizedExpression()->closeParenTkIdx_;
                        parenDecltor->innerDecltor_ = innerDecltor;
                        innerDecltor = parenDecltor;
                        break;
                    }

                    default:
                        PSY_ASSERT(false, return);
                }
            }
            auto parenDecltor = makeNode<ParenthesizedDeclaratorSyntax>();
            parenDecltor->openParenTkIdx_ = callExpr->openParenTkIdx_;
            parenDecltor->closeParenTkIdx_ = callExpr->closeParenTkIdx_;
            parenDecltor->innerDecltor_ = innerDecltor;
            decltor = parenDecltor;

            stmtK = SyntaxKind::AmbiguousCallOrVariableDeclaration;
            break;
        }

        default:
            PSY_ASSERT_W_MSG(false, return, "");
    }

    VariableAndOrFunctionDeclarationSyntax* varDecl = nullptr;
    varDecl = makeNode<VariableAndOrFunctionDeclarationSyntax>();
    varDecl->specs_ = makeNode<SpecifierListSyntax>(typedefName);
    varDecl->semicolonTkIdx_ = exprStmt->semicolonTkIdx_;
    varDecl->decltors_ = makeNode<DeclaratorListSyntax>(decltor);

    auto ambigStmt = makeNode<AmbiguousExpressionOrDeclarationStatementSyntax>(stmtK);
    stmt = ambigStmt;
    ambigStmt->exprStmt_ = exprStmt;
    auto declStmt = makeNode<DeclarationStatementSyntax>();
    declStmt->decl_ = varDecl;
    ambigStmt->declStmt_ = declStmt;

    diagReporter_.AmbiguousExpressionOrDeclarationStatement(ambigStmt);
}

/**
 * Parse a \a compound-statement.
 *
 * \remark 6.8.2
 */
bool Parser::parseCompoundStatement_AtFirst(StatementSyntax*& stmt,
                                            StatementContext stmtCtx)
{
    DEBUG_THIS_RULE();
    PSY_ASSERT_W_MSG(peek().kind() == SyntaxKind::OpenBraceToken,
                     return false,
                     "assert failure: `{'");

    DepthControl _(DEPTH_OF_STMTS_);

    auto block = makeNode<CompoundStatementSyntax>();
    stmt = block;
    block->openBraceTkIdx_ = consume();

    StatementListSyntax** stmtList_cur = &block->stmts_;

    while (true) {
        StatementSyntax* innerStmt = nullptr;
        switch (peek().kind()) {
            case SyntaxKind::EndOfFile:
                diagReporter_.ExpectedToken(SyntaxKind::CloseBraceToken);
                return false;

            case SyntaxKind::CloseBraceToken:
                block->closeBraceTkIdx_ = consume();
                return true;

            default: {
                if (!parseStatement(innerStmt, stmtCtx)) {
                    skipTo(SyntaxKind::CloseBraceToken);
                    continue;
                }
                break;
            }
        }

        *stmtList_cur = makeNode<StatementListSyntax>(innerStmt);
        stmtList_cur = &(*stmtList_cur)->next;
    }

    return matchOrSkipTo(SyntaxKind::CloseBraceToken, &block->closeBraceTkIdx_);
}

/**
 * Parse an \a expression-statement.
 *
 * \remark 6.8.3
 */
bool Parser::parseExpressionStatement(StatementSyntax*& stmt)
{
    DEBUG_THIS_RULE();

    if (peek().kind() == SyntaxKind::SemicolonToken) {
        auto exprStmt = makeNode<ExpressionStatementSyntax>();
        stmt = exprStmt;
        exprStmt->semicolonTkIdx_ = consume();
        return true;
    }

    ExpressionSyntax* expr = nullptr;
    if (!parseExpression(expr))
        return false;

    auto exprStmt = makeNode<ExpressionStatementSyntax>();
    stmt = exprStmt;
    exprStmt->expr_ = expr;

    return matchOrSkipTo(SyntaxKind::SemicolonToken, &exprStmt->semicolonTkIdx_);
}

/**
 * Parse a \a selection-statement, of the \c if \a statement,
 * with LA(1) at \c FIRST.
 *
 * \remark 6.8.4.1
 */
bool Parser::parseIfStatement_AtFirst(StatementSyntax*& stmt, StatementContext stmtCtx)
{
    DEBUG_THIS_RULE();
    PSY_ASSERT_W_MSG(peek().kind() == SyntaxKind::Keyword_if,
                     return false,
                     "assert failure: `if'");

    auto ifStmt = makeNode<IfStatementSyntax>();
    stmt = ifStmt;
    ifStmt->ifKwTkIdx_ = consume();

    if (!(match(SyntaxKind::OpenParenToken, &ifStmt->openParenTkIdx_)
            && parseExpression(ifStmt->cond_)
            && match(SyntaxKind::CloseParenToken, &ifStmt->closeParenTkIdx_)
            && parseStatement(ifStmt->stmt_, stmtCtx))) {
        return ignoreStatement();
    }

    if (peek().kind() == SyntaxKind::Keyword_else) {
        ifStmt->elseKwTkIdx_ = consume();
        return checkStatementParse(
                    parseStatement(ifStmt->elseStmt_, stmtCtx));
    }
    return true;
}

/**
 * Parse a \a selection-statement of the \c witch \a statement,
 * with LA(1) at \c FIRST.
 *
 * \remark 6.8.4.2
 */
bool Parser::parseSwitchStatement_AtFirst(StatementSyntax*& stmt,
                                          StatementContext stmtCtx)
{
    DEBUG_THIS_RULE();
    PSY_ASSERT_W_MSG(peek().kind() == SyntaxKind::Keyword_switch,
                     return false,
                     "assert failure: `switch'");

    auto switchStmt = makeNode<SwitchStatementSyntax>();
    stmt = switchStmt;
    switchStmt->switchKwTkIdx_ = consume();

    return checkStatementParse(
                match(SyntaxKind::OpenParenToken, &switchStmt->openParenTkIdx_)
                && parseExpression(switchStmt->cond_)
                && match(SyntaxKind::CloseParenToken, &switchStmt->closeParenTkIdx_)
                && parseStatement(switchStmt->stmt_,
                                  stmtCtx + StatementContext::Switch));
}

/**
 * Parse an \a labeled-statement, with LA(1) at \c FIRST.
 *
 * \remark 6.8.1
 */
bool Parser::parseLabeledStatement_AtFirst(StatementSyntax*& stmt,
                                           StatementContext stmtCtx)
{
    DEBUG_THIS_RULE();
    PSY_ASSERT_W_MSG(peek().kind() == SyntaxKind::IdentifierToken
                        || peek().kind() == SyntaxKind::Keyword_case
                        || peek().kind() == SyntaxKind::Keyword_default,
                     return false,
                     "assert failure: <identifier>, `case', or `default'");

    LabeledStatementSyntax* labelStmt = nullptr;

    switch (peek().kind()) {
        case SyntaxKind::IdentifierToken:
            PSY_ASSERT_W_MSG(peek(2).kind() == SyntaxKind::ColonToken,
                             return false,
                             "assert failure: `:'");

            labelStmt = makeNode<LabeledStatementSyntax>(SyntaxKind::IdentifierLabelStatement);
            stmt = labelStmt;
            labelStmt->labelTkIdx_ = consume();
            labelStmt->colonTkIdx_ = consume();
            break;

        case SyntaxKind::Keyword_default:
            labelStmt = makeNode<LabeledStatementSyntax>(SyntaxKind::DefaultLabelStatement);
            stmt = labelStmt;
            labelStmt->labelTkIdx_ = consume();
            if (!match(SyntaxKind::ColonToken, &labelStmt->colonTkIdx_))
                return false;
            break;

        case SyntaxKind::Keyword_case:
            labelStmt = makeNode<LabeledStatementSyntax>(SyntaxKind::DefaultLabelStatement);
            stmt = labelStmt;
            labelStmt->labelTkIdx_ = consume();
            if (!(parseExpressionWithPrecedenceConditional(labelStmt->expr_)
                    && match(SyntaxKind::ColonToken, &labelStmt->colonTkIdx_)))
                return false;
            break;

        default:
            break;
    }

    return checkStatementParse(
                parseStatement(labelStmt->stmt_, stmtCtx));
}

/**
 * Parse an \a iteration-statement of the \c while \a statement,
 * with LA(1) at \c FIRST.
 *
 * \remark 6.8.5.1
 */
bool Parser::parseWhileStatement_AtFirst(StatementSyntax*& stmt,
                                         StatementContext stmtCtx)
{
    DEBUG_THIS_RULE();
    PSY_ASSERT_W_MSG(peek().kind() == SyntaxKind::Keyword_while,
                     return false,
                     "assert failure: `while'");

    auto whileStmt = makeNode<WhileStatementSyntax>();
    stmt = whileStmt;
    whileStmt->whileKwTkIdx_ = consume();

    return checkStatementParse(
                match(SyntaxKind::OpenParenToken, &whileStmt->openParenTkIdx_)
                && parseExpression(whileStmt->cond_)
                && match(SyntaxKind::CloseParenToken, &whileStmt->closeParenTkIdx_)
                && parseStatement(whileStmt->stmt_,
                                  stmtCtx + StatementContext::Loop));
}

/**
 * Parse an \a iteration-statement of the \c do \a statement,
 * with LA(1) at \c FIRST.
 *
 * \remark 6.8.5.2
 */
bool Parser::parseDoStatement_AtFirst(StatementSyntax*& stmt,
                                      StatementContext stmtCtx)
{
    DEBUG_THIS_RULE();
    PSY_ASSERT_W_MSG(peek().kind() == SyntaxKind::Keyword_do,
                     return false,
                     "assert failure: `do'");

    auto doStmt = makeNode<DoStatementSyntax>();
    stmt = doStmt;
    doStmt->doKwTkIdx_ = consume();

    return checkStatementParse(
                parseStatement(doStmt->stmt_,
                               stmtCtx + StatementContext::Loop)
                && match(SyntaxKind::Keyword_while, &doStmt->whileKwTkIdx_)
                && match(SyntaxKind::OpenParenToken, &doStmt->openParenTkIdx_)
                && parseExpression(doStmt->cond_)
                && match(SyntaxKind::CloseParenToken, &doStmt->closeParenTkIdx_)
                && match(SyntaxKind::SemicolonToken, &doStmt->semicolonTkIdx_));
}

/**
 * Parse an \a iteration-statement of the \c for \a statement,
 * with LA(1) at \c FIRST.
 *
 * \remark 6.8.5.3
 */
bool Parser::parseForStatement_AtFirst(StatementSyntax*& stmt,
                                       StatementContext stmtCtx)
{
    DEBUG_THIS_RULE();
    PSY_ASSERT_W_MSG(peek().kind() == SyntaxKind::Keyword_for,
                     return false,
                     "assert failure: `for'");

    auto forStmt = makeNode<ForStatementSyntax>();
    stmt = forStmt;
    forStmt->forKwTkIdx_ = consume();

    if (!match(SyntaxKind::OpenParenToken, &forStmt->openParenTkIdx_)) {
        skipTo(SyntaxKind::CloseParenToken);
        return false;
    }

    if (peek().kind() == SyntaxKind::Keyword_ExtGNU___extension__)
        forStmt->extKwTkIdx_ = consume();

    switch (peek().kind()) {
        // storage-class-specifier
        case SyntaxKind::Keyword_typedef:
        case SyntaxKind::Keyword_extern:
        case SyntaxKind::Keyword_static:
        case SyntaxKind::Keyword_auto:
        case SyntaxKind::Keyword_register:
        case SyntaxKind::Keyword__Thread_local:
        case SyntaxKind::Keyword_ExtGNU___thread:

        // type-qualifier
        case SyntaxKind::Keyword_const:
        case SyntaxKind::Keyword_volatile:
        case SyntaxKind::Keyword_restrict:
        case SyntaxKind::Keyword__Atomic:

        // function-specifier
        case SyntaxKind::Keyword_inline:
        case SyntaxKind::Keyword__Noreturn:

        // type-specifier
        case SyntaxKind::Keyword_void:
        case SyntaxKind::Keyword_char:
        case SyntaxKind::Keyword_short:
        case SyntaxKind::Keyword_int:
        case SyntaxKind::Keyword_long:
        case SyntaxKind::Keyword_float:
        case SyntaxKind::Keyword_double:
        case SyntaxKind::Keyword__Bool:
        case SyntaxKind::Keyword__Complex:
        case SyntaxKind::Keyword_signed:
        case SyntaxKind::Keyword_unsigned:
        case SyntaxKind::Keyword_Ext_char16_t:
        case SyntaxKind::Keyword_Ext_char32_t:
        case SyntaxKind::Keyword_Ext_wchar_t:
        case SyntaxKind::Keyword_struct:
        case SyntaxKind::Keyword_union:
        case SyntaxKind::Keyword_enum:
        case SyntaxKind::Keyword_ExtGNU___complex__:
            if (!parseDeclarationStatement(
                        forStmt->initStmt_,
                        &Parser::parseDeclarationOrFunctionDefinition)) {
                skipTo(SyntaxKind::CloseParenToken);
                return false;
            }
            break;

        case SyntaxKind::IdentifierToken: {
            Backtracker BT(this);
            if (!parseExpressionStatement(forStmt->initStmt_)) {
                BT.backtrack();
                if (parseDeclarationStatement(
                            forStmt->initStmt_,
                            &Parser::parseDeclarationOrFunctionDefinition)) {
                    break;
                }
                skipTo(SyntaxKind::CloseParenToken);
                return false;
            }
            maybeAmbiguateStatement(forStmt->initStmt_);
            break;
        }

        default:
            if (!parseExpressionStatement(forStmt->initStmt_)) {
                skipTo(SyntaxKind::CloseParenToken);
                return false;
            }
    }

    if (peek().kind() != SyntaxKind::SemicolonToken
            && !parseExpression(forStmt->cond_)) {
        skipTo(SyntaxKind::CloseParenToken);
        return false;
    }

    forStmt->semicolonTkIdx_ = consume();
    if (peek().kind() == SyntaxKind::CloseParenToken)
        forStmt->closeParenTkIdx_ = consume();
    else if (!(parseExpression(forStmt->expr_)
                && match(SyntaxKind::CloseParenToken, &forStmt->closeParenTkIdx_))) {
        skipTo(SyntaxKind::CloseParenToken);
        return false;
    }

    return checkStatementParse(
                parseStatement(forStmt->stmt_,
                               stmtCtx + StatementContext::Loop));
}

/**
 * Parse a \a jump-statement of the \c goto \a statement,
 * with LA(1) at \c FIRST.
 *
 * \remark 6.8.6.1
 */
bool Parser::parseGotoStatement_AtFirst(StatementSyntax*& stmt)
{
    DEBUG_THIS_RULE();
    PSY_ASSERT_W_MSG(peek().kind() == SyntaxKind::Keyword_goto,
                     return false,
                     "assert failure: `goto'");

    auto gotoStmt = makeNode<GotoStatementSyntax>();
    stmt = gotoStmt;
    gotoStmt->gotoKwTkIdx_ = consume();

    if (peek().kind() != SyntaxKind::IdentifierToken) {
        diagReporter_.ExpectedTokenOfCategoryIdentifier();
        return ignoreStatement();
    }

    gotoStmt->identTkIdx_ = consume();

    return checkStatementParse(
                match(SyntaxKind::SemicolonToken, &gotoStmt->semicolonTkIdx_));
}

/**
 * Parse a \a jump-statement of the \c continue \a statement,
 * with LA(1) at \c FIRST.
 *
 * \remark 6.8.6.2
 */
bool Parser::parseContinueStatement_AtFirst(StatementSyntax*& stmt,
                                            StatementContext stmtCtx)
{
    DEBUG_THIS_RULE();
    PSY_ASSERT_W_MSG(peek().kind() == SyntaxKind::Keyword_continue,
                     return false,
                     "assert failure: `continue'");

    auto continueStmt = makeNode<ContinueStatementSyntax>();
    stmt = continueStmt;
    continueStmt->continueKwTkIdx_ = consume();

    return checkStatementParse(
                match(SyntaxKind::SemicolonToken, &continueStmt->semicolonTkIdx_));
}

/**
 * Parse a \a jump-statement of the \c break \a statement,
 * with LA(1) at \c FIRST.
 *
 * \remark 6.8.6.3
 */
bool Parser::parseBreakStatement_AtFirst(StatementSyntax*& stmt,
                                         StatementContext stmtCtx)
{
    DEBUG_THIS_RULE();
    PSY_ASSERT_W_MSG(peek().kind() == SyntaxKind::Keyword_break,
                     return false,
                     "assert failure: `break'");

    auto breakStmt = makeNode<BreakStatementSyntax>();
    stmt = breakStmt;
    breakStmt->breakKwTkIdx_ = consume();

    return checkStatementParse(
                match(SyntaxKind::SemicolonToken, &breakStmt->semicolonTkIdx_));
}

/**
 * Parse a \a jump-statement of the \c return \a statement,
 * with LA(1) at \c FIRST.
 *
 * \remark 6.8.6.4
 */
bool Parser::parseReturnStatement_AtFirst(StatementSyntax*& stmt)
{
    DEBUG_THIS_RULE();

    auto retStmt = makeNode<ReturnStatementSyntax>();
    stmt = retStmt;
    retStmt->returnKwTkIdx_ = consume();

    if (peek().kind() == SyntaxKind::SemicolonToken) {
        retStmt->semicolonTkIdx_ = consume();
        return true;
    }

    return checkStatementParse(
                parseExpression(retStmt->expr_)
                && match(SyntaxKind::SemicolonToken, &retStmt->semicolonTkIdx_));
}

/**
 * Parse an ASM instruction \a statement.
 *
 * \remark J.5.10.
 *
 * \see https://gcc.gnu.org/onlinedocs/gcc/Using-Program-Language-with-C.html#Using-Program-Language-with-C
 */
bool Parser::parseExtGNU_AsmStatement(StatementSyntax*& stmt)
{
    DEBUG_THIS_RULE();
    PSY_ASSERT_W_MSG(peek().kind() == SyntaxKind::Keyword_ExtGNU___asm__,
                     return false,
                     "assert failure: `asm'");

    if (!tree_->parseOptions().languageExtensions().isEnabled_extGNU_Asm())
        diagReporter_.ExpectedFeature("GNU asm");

    auto asmStmt = makeNode<ExtGNU_AsmStatementSyntax>();
    stmt = asmStmt;
    asmStmt->asmKwTkIdx_ = consume();

    parseExtGNU_AsmQualifiers(asmStmt->asmQuals_);

    if (!(match(SyntaxKind::OpenParenToken, &asmStmt->openParenTkIdx_)
              && parseStringLiteral(asmStmt->strLit_))) {
        return ignoreStatement();
    }

    if (peek().kind() == SyntaxKind::ColonToken) {
        asmStmt->colon1TkIdx_ = consume();
        if ((peek().kind() == SyntaxKind::StringLiteralToken
                    || peek().kind() == SyntaxKind::OpenBracketToken)
                && !parseCommaSeparatedItems<ExtGNU_AsmOperandSyntax>(
                        asmStmt->outOprds_,
                        &Parser::parseExtGNU_AsmOutputOperand_AtFirst)) {
            return ignoreStatement();
        }
    }

    if (peek().kind() == SyntaxKind::ColonToken) {
        asmStmt->colon2TkIdx_ = consume();
        if ((peek().kind() == SyntaxKind::StringLiteralToken
                    || peek().kind() == SyntaxKind::OpenBracketToken)
                && !parseCommaSeparatedItems<ExtGNU_AsmOperandSyntax>(
                        asmStmt->inOprds_,
                        &Parser::parseExtGNU_AsmInputOperand_AtFirst)) {
            return ignoreStatement();
        }
    }

    if (peek().kind() == SyntaxKind::ColonToken) {
        asmStmt->colon3TkIdx_ = consume();
        if (!parseExtGNU_AsmClobbers(asmStmt->clobs_))
            return ignoreStatement();
    }

    if (peek().kind() == SyntaxKind::ColonToken) {
        asmStmt->colon4TkIdx_ = consume();
        if (!parseExtGNU_AsmGotoLabels(asmStmt->labels_))
            return ignoreStatement();
    }

    return checkStatementParse(
                match(SyntaxKind::CloseParenToken, &asmStmt->closeParenTkIdx_)
                && match(SyntaxKind::SemicolonToken, &asmStmt->semicolonTkIdx_));
}

void Parser::parseExtGNU_AsmQualifiers(SpecifierListSyntax*& specList)
{
    DEBUG_THIS_RULE();

    SpecifierListSyntax** qualsList_cur = &specList;

    while (true) {
        SpecifierSyntax* qual = nullptr;
        switch (peek().kind()) {
            case SyntaxKind::Keyword_volatile:
                parseTrivialSpecifier_AtFirst<ExtGNU_AsmQualifierSyntax>(
                        qual,
                        SyntaxKind::ExtGNU_AsmVolatileQualifier);
                break;

            case SyntaxKind::Keyword_inline:
                parseTrivialSpecifier_AtFirst<ExtGNU_AsmQualifierSyntax>(
                        qual,
                        SyntaxKind::ExtGNU_AsmInlineQualifier);
                break;

            case SyntaxKind::Keyword_goto:
                parseTrivialSpecifier_AtFirst<ExtGNU_AsmQualifierSyntax>(
                        qual,
                        SyntaxKind::ExtGNU_AsmGotoQualifier);
                break;

            default:
                return;
        }

        *qualsList_cur = makeNode<SpecifierListSyntax>(qual);
        qualsList_cur = &(*qualsList_cur)->next;
    }
}

bool Parser::parseExtGNU_AsmOutputOperand_AtFirst(ExtGNU_AsmOperandSyntax*& asmOprd,
                                                  ExtGNU_AsmOperandListSyntax*&)
{
    return parseExtGNU_AsmOperand_AtFirst(asmOprd, SyntaxKind::ExtGNU_AsmOutputOperand);
}

bool Parser::parseExtGNU_AsmInputOperand_AtFirst(ExtGNU_AsmOperandSyntax*& asmOprd,
                                                 ExtGNU_AsmOperandListSyntax*&)
{
    return parseExtGNU_AsmOperand_AtFirst(asmOprd, SyntaxKind::ExtGNU_AsmInputOperand);
}

bool Parser::parseExtGNU_AsmOperand_AtFirst(ExtGNU_AsmOperandSyntax*& asmOprd,
                                            SyntaxKind oprdK)
{
    DEBUG_THIS_RULE();
    PSY_ASSERT_W_MSG(peek().kind() == SyntaxKind::StringLiteralToken
                        || peek().kind() == SyntaxKind::OpenBracketToken,
                     return false,
                     "assert failure: `[' or `<string-literal>'");

    auto oprd = makeNode<ExtGNU_AsmOperandSyntax>(oprdK);
    asmOprd = oprd;

    if (peek().kind() == SyntaxKind::OpenBracketToken) {
        oprd->openBracketTkIdx_ = consume();
        if (!(parseIdentifierName(oprd->identExpr_)
                && match(SyntaxKind::CloseBracketToken, &oprd->closeBracketTkIdx_)))
            return false;
    }

    return parseStringLiteral(oprd->strLit_)
        && match(SyntaxKind::OpenParenToken, &oprd->openParenTkIdx_)
        && parseExpression(oprd->expr_)
        && match(SyntaxKind::CloseParenToken, &oprd->closeParenTkIdx_);
}

bool Parser::parseExtGNU_AsmClobbers(ExpressionListSyntax*& clobList)
{
    DEBUG_THIS_RULE();

    if (peek().kind() != SyntaxKind::StringLiteralToken)
        return true;

    return parseCommaSeparatedItems<ExpressionSyntax>(
                clobList,
                &Parser::parseExtGNU_AsmClobber_AtFirst);
}

bool Parser::parseExtGNU_AsmClobber_AtFirst(ExpressionSyntax*& clob, ExpressionListSyntax*&)
{
    DEBUG_THIS_RULE();

    return parseStringLiteral(clob);
}

bool Parser::parseExtGNU_AsmGotoLabels(ExpressionListSyntax*& labelList)
{
    DEBUG_THIS_RULE();

    if (peek().kind() != SyntaxKind::IdentifierToken)
        return true;

    return parseCommaSeparatedItems<ExpressionSyntax>(
                labelList,
                &Parser::parseExtGNU_AsmGotoLabel_AtFirst);

}

bool Parser::parseExtGNU_AsmGotoLabel_AtFirst(ExpressionSyntax*& label,
                                              ExpressionListSyntax*&)
{
    DEBUG_THIS_RULE();

    parseIdentifierName_AtFirst(label);
    return true;
}

bool Parser::checkStatementParse(bool stmtParsed)
{
    if (!stmtParsed)
        return ignoreStatement();
    return true;
}

namespace psy {
namespace C {

Parser::StatementContext operator+(Parser::StatementContext a,
                                   Parser::StatementContext b)
{
    switch (a) {
        case Parser::StatementContext::None:
            return b;

        case Parser::StatementContext::Switch:
            if (b == Parser::StatementContext::Loop)
                return Parser::StatementContext::SwitchAndLoop;
            return a;

        case Parser::StatementContext::Loop:
            if (b == Parser::StatementContext::Switch)
                return Parser::StatementContext::SwitchAndLoop;
            return a;

        case Parser::StatementContext::SwitchAndLoop:
            return a;

        default:
            PSY_ASSERT_W_MSG(false, return Parser::StatementContext::None, "");
    }
}

} // C
} // psy
