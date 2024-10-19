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

//-------------//
// Expressions //
//-------------//

/**
 * Parse an \a expression.
 * <a href="https://docs.google.com/spreadsheets/d/1oGjtFaqLzSoBEp2aGNgHrbEHxSi4Ijv57mXMPymZEcQ/edit?usp=sharing">
 * This table
 * </a>
 * describes the choices taken by the parser as according to the grammar rules.
 *
 \verbatim
 expression:
     assignment-expression
     expression , assignment-expression
 \endverbatim
 *
 * \remark 6.5.17
 *
 * \note
 * The naming convention employed in certain expression-parsing methods,
 * e.g., in \c Parser::parseExpressionWithPrecedence_CAST, deviates a bit
 * from the usual convention employed in parsing methods. This difference
 * is due to the precedence-oriented way in which the grammar of
 * expressions is defined; had said method been named \c parseCastExpression,
 * after its rule name \a cast-expression, one could have expected that
 * its result would always be a CastExpressionSyntax node (a correspondence
 * that holds in general). But this is not true, given that a
 * \a cast-expression may actually derive an \a constant, whose node is a
 * ConstantExpressionSyntax.
 */
bool Parser::parseExpression(ExpressionSyntax*& expr)
{
    DBG_THIS_RULE();

    DepthControl _(DEPTH_OF_EXPRS_);
    return parseExpressionWithPrecedenceComma(expr);
}

/**
 * Parse an \a identifier.
 *
 * \remark 6.4.2 and 6.5.1
 */
bool Parser::parseIdentifierName(ExpressionSyntax*& expr)
{
    DBG_THIS_RULE();

    if (peek().kind() != SyntaxKind::IdentifierToken) {
        diagReporter_.ExpectedTokenOfCategoryIdentifier();
        return false;
    }

    parseIdentifierName_AtFirst(expr);
    return true;
}

/**
 * Parse an \a identifier as an \a expression, with LA(1) at FIRST.
 *
 * \remark 6.4.2 and 6.5.1
 */
void Parser::parseIdentifierName_AtFirst(ExpressionSyntax*& expr)
{
    DBG_THIS_RULE();
    PSY_ASSERT_3(peek().kind() == SyntaxKind::IdentifierToken,
                  return,
                  "assert failure: <identifier>");

    auto identExpr = makeNode<IdentifierNameSyntax>();
    expr = identExpr;
    identExpr->identTkIdx_ = consume();
}

/**
 * Parse a \a predefined name as an \a expression, with LA(1) at first.
 */
void Parser::parsePredefinedName_AtFirst(ExpressionSyntax*& expr)
{
    DBG_THIS_RULE();
    PSY_ASSERT_3(SyntaxFacts::isPredefinedToken(peek().kind()),
                  return,
                  "assert failure: <predefined-name>");

    auto predefExpr = makeNode<PredefinedNameSyntax>();
    expr = predefExpr;
    predefExpr->predefTkIdx_ = consume();
}

/**
 * Parse a \a constant.
 *
 * \remark 6.4.4 and 6.5.1
 */
template <class ExprT>
bool Parser::parseConstant(ExpressionSyntax*& expr, SyntaxKind exprK)
{
    DBG_THIS_RULE();

    if (!SyntaxFacts::isConstantToken(peek().kind())) {
        diagReporter_.ExpectedTokenOfCategoryConstant();
        return false;
    }

    parseConstant_AtFirst<ExprT>(expr, exprK);
    return true;
}

template bool Parser::parseConstant<ConstantExpressionSyntax>
(ExpressionSyntax*& expr, SyntaxKind exprK);

/**
 * Parse a \a constant, with LA(1) at FIRST.
 *
 * \remark 6.4.4 and 6.5.1
 */
template <class ExprT>
void Parser::parseConstant_AtFirst(ExpressionSyntax*& expr, SyntaxKind exprK)
{
    DBG_THIS_RULE();
    PSY_ASSERT_3(SyntaxFacts::isConstantToken(peek().kind()),
                  return,
                  "assert failure: <constant>");

    auto constExpr = makeNode<ExprT>(exprK);
    expr = constExpr;
    constExpr->constantTkIdx_  = consume();
}

template void Parser::parseConstant_AtFirst<ConstantExpressionSyntax>
(ExpressionSyntax*& expr, SyntaxKind exprK);

/**
 * Parse a \a string-literal.
 */
bool Parser::parseStringLiteral(ExpressionSyntax*& expr)
{
    DBG_THIS_RULE();

    if (!SyntaxFacts::isStringLiteralToken(peek().kind())) {
        diagReporter_.ExpectedTokenOfCategoryStringLiteral();
        return false;
    }

    parseStringLiteral_AtFirst(expr);
    return true;
}

/**
 * Parse a \a string-literal, with LA(1) at FIRST.
 *
 * \remark 6.4.5 and 6.5.1
 */
void Parser::parseStringLiteral_AtFirst(ExpressionSyntax*& expr)
{
    DBG_THIS_RULE();
    PSY_ASSERT_3(SyntaxFacts::isStringLiteralToken(peek().kind()),
                  return,
                  "assert failure: <string-literal>");

    StringLiteralExpressionSyntax* strLit = nullptr;
    StringLiteralExpressionSyntax** strLit_cur = &strLit;

    do {
        *strLit_cur = makeNode<StringLiteralExpressionSyntax>();
        (*strLit_cur)->litTkIdx_ = consume();
        strLit_cur = &(*strLit_cur)->adjacent_;
    }
    while (SyntaxFacts::isStringLiteralToken(peek().kind()));

    expr = strLit;
}

/**
 * Parse a \a parenthesized-expression, with LA(1) at FIRST.
 *
 * \remark 6.5.1
 */
bool Parser::parseParenthesizedExpression_AtFirst(ExpressionSyntax*& expr)
{
    DBG_THIS_RULE();
    PSY_ASSERT_3(peek().kind() == SyntaxKind::OpenParenToken,
                  return false,
                  "assert failure: `('");

    auto parenExpr = makeNode<ParenthesizedExpressionSyntax>();
    expr = parenExpr;
    parenExpr->openParenTkIdx_ = consume();
    return parseExpression(parenExpr->expr_)
        && matchOrSkipTo(SyntaxKind::CloseParenToken, &parenExpr->closeParenTkIdx_);
}

/**
 * Parse a GNU extension \a statements-and-declaration in \a expression,
 * with LA(1) at FIRST and LA(2) at FOLLOW.
 *
 * https://gcc.gnu.org/onlinedocs/gcc/Statement-Exprs.html#Statement-Exprs
 */
bool Parser::parseExtGNU_StatementExpression_AtFirst(ExpressionSyntax *&expr)
{
    DBG_THIS_RULE();
    PSY_ASSERT_3(peek().kind() == SyntaxKind::OpenParenToken
                        && peek(2).kind() == SyntaxKind::OpenBraceToken,
                  return false,
                  "assert failure: `(' then `{'");

    if (!tree_->parseOptions().languageExtensions().isEnabled_extGNU_StatementExpressions())
        diagReporter_.ExpectedFeature("GNU statement expressions");

    auto gnuExpr = makeNode<ExtGNU_EnclosedCompoundStatementExpressionSyntax>();
    expr = gnuExpr;
    gnuExpr->openParenTkIdx_ = consume();

    StatementSyntax* statement = nullptr;
    parseCompoundStatement_AtFirst(statement, StatementContext::None);
    if (statement->asCompoundStatement())
        gnuExpr->stmt_ = statement->asCompoundStatement();
    return matchOrSkipTo(SyntaxKind::CloseParenToken, &gnuExpr->closeParenTkIdx_);
}

/**
 * Parse a \a generic-selection, with LA(1) at FIRST.
 *
 \verbatim
 generic-selection:
     _Generic ( assignment-expression , generic-assoc-list )
 \endverbatim
 *
 * \remark 6.5.1.1
 */
bool Parser::parseGenericSelectionExpression_AtFirst(ExpressionSyntax*& expr)
{
    DBG_THIS_RULE();
    PSY_ASSERT_3(peek().kind() == SyntaxKind::Keyword__Generic,
                  return false,
                  "assert failure: `_Generic'");

    auto selExpr = makeNode<GenericSelectionExpressionSyntax>();
    expr = selExpr;
    selExpr->genericKwTkIdx_ = consume();

    return match(SyntaxKind::OpenParenToken, &selExpr->openParenTkIdx_)
            && parseExpressionWithPrecedenceAssignment(selExpr->expr_)
            && match(SyntaxKind::CommaToken, &selExpr->commaTkIdx_)
            && parseGenericAssociationList(selExpr->assocs_)
            && matchOrSkipTo(SyntaxKind::CloseParenToken, &selExpr->closeParenTkIdx_);
}

/**
 * Parse a \a generic-assoc-list.
 *
 \verbatim
 generic-assoc-list:
     generic-association
     generic-assoc-list , generic-association
 \endverbatim
 *
 * \remark 6.5.1.1
 */
bool Parser::parseGenericAssociationList(GenericAssociationListSyntax*& assocList)
{
    DBG_THIS_RULE();

    return parseCommaSeparatedItems<GenericAssociationSyntax>(
                assocList,
                &Parser::parseGenericAssociation);
}

/**
 * Parse a \a generic-association.
 *
 \verbatim
 generic-association:
     type-name : assignment-expression
     default: assignment-expression
 \endverbatim
 *
 * \remark 6.5.1.1
 */
bool Parser::parseGenericAssociation(GenericAssociationSyntax*& assoc,
                                     GenericAssociationListSyntax*&)
{
    DBG_THIS_RULE();

    switch (peek().kind()) {
        case SyntaxKind::Keyword_default: {
            assoc = makeNode<GenericAssociationSyntax>(SyntaxKind::DefaultGenericAssociation);
            auto defExpr = makeNode<IdentifierNameSyntax>();
            defExpr->identTkIdx_ = consume();
            assoc->typeName_or_default_ = defExpr;
            break;
        }

        default: {
            TypeNameSyntax* typeName = nullptr;
            if (!parseTypeName(typeName))
                return false;
            assoc = makeNode<GenericAssociationSyntax>(SyntaxKind::TypedGenericAssociation);
            assoc->typeName_or_default_ = typeName;
            break;
        }
    }

    return match(SyntaxKind::ColonToken, &assoc->colonTkIdx_)
            && parseExpressionWithPrecedenceAssignment(assoc->expr_);
}

/* Postfix */

/**
 * Parse a \a postfix-expression, or any expression that is subsumed by such rule.
 *
 * See note about naming convention in Parser::parseExpression.
 *
 \verbatim
 postfix-expression:
     primary-expression
     postfix-expression [ expression ]
     postfix-expression ( argument-expression-list_opt )
     postfix-expression . identifier
     postfix-expression -> identifier
     postfix-expression ++
     postfix-expression --
     ( type-name ) { initializer-list }
     ( type-name) { initializer-list, }

 primary-expression:
     identifier
     constant
     string-literal
     ( expression )
     generic-selection
     __builtin_va_arg ( assignment-expression , type-name )
 \endverbatim
 *
 * Adjusted grammar:
 *
 \verbatim
 postfix-expression:
     compound-literal-at-open-paren
     primary-expression postfix-expression-at-follow-of-primary
 \endverbatim
 *
 * \remark 6.5.2
 */
bool Parser::parseExpressionWithPrecedencePostfix(ExpressionSyntax*& expr)
{
    DBG_THIS_RULE();

    switch (peek().kind()) {
        case SyntaxKind::OpenParenToken:
            // postfix-expression -> primary-expression -> `(' expression `)'
            //                     | `(' type-name `)' `{' initializer-list `}'
            //                     | `(' `{' ... `}' `)'
            switch (peek(2).kind()) {
                // type-name ->* type-qualifier
                case SyntaxKind::Keyword_const:
                case SyntaxKind::Keyword_volatile:
                case SyntaxKind::Keyword_restrict:
                case SyntaxKind::Keyword__Atomic:

                // type-name ->* alignment-specifier
                case SyntaxKind::Keyword__Alignas:

                // type-name ->* GNU-typeof-specifier
                case SyntaxKind::Keyword_ExtGNU___typeof__:

                // type-name ->* type-specifier
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
                    return parseCompoundLiteral_AtOpenParen(expr);

                // GNU
                case SyntaxKind::OpenBraceToken:
                    return parseExtGNU_StatementExpression_AtFirst(expr);

                // type-name ->* typedef-name -> identifier
                // expression ->* identifier
                case SyntaxKind::IdentifierToken: {
                    Backtracker BT(this);
                    auto openParenTkIdx = consume();
                    TypeNameSyntax* typeName = nullptr;
                    if (parseTypeName(typeName)
                                && peek().kind() == SyntaxKind::CloseParenToken
                                && peek(2).kind() == SyntaxKind::OpenBraceToken) {
                        auto closeParenTkIdx = consume();
                        return parseCompoundLiteral_AtOpenBrace(expr,
                                                                openParenTkIdx,
                                                                typeName,
                                                                closeParenTkIdx);
                    }
                    BT.backtrack();
                    [[fallthrough]];
                }

                default:
                    return parseParenthesizedExpression_AtFirst(expr)
                        && parsePostfixExpression_AtFollowOfPrimary(expr);;
            }

        case SyntaxKind::IdentifierToken:
            parseIdentifierName_AtFirst(expr);
            break;

        case SyntaxKind::IntegerConstantToken:
            parseConstant_AtFirst<ConstantExpressionSyntax>(
                    expr,
                    SyntaxKind::IntegerConstantExpression);
            break;

        case SyntaxKind::FloatingConstantToken:
            parseConstant_AtFirst<ConstantExpressionSyntax>(
                    expr,
                    SyntaxKind::FloatingConstantExpression);
            break;

        case SyntaxKind::ImaginaryIntegerConstantToken:
            parseConstant_AtFirst<ConstantExpressionSyntax>(
                    expr,
                    SyntaxKind::ImaginaryIntegerConstantExpression);
            break;

        case SyntaxKind::ImaginaryFloatingConstantToken:
            parseConstant_AtFirst<ConstantExpressionSyntax>(
                    expr,
                    SyntaxKind::ImaginaryFloatingConstantExpression);
            break;

        case SyntaxKind::CharacterConstantToken:
        case SyntaxKind::CharacterConstant_L_Token:
        case SyntaxKind::CharacterConstant_u_Token:
        case SyntaxKind::CharacterConstant_U_Token:
            parseConstant_AtFirst<ConstantExpressionSyntax>(
                    expr,
                    SyntaxKind::CharacterConstantExpression);
            break;

        case SyntaxKind::Keyword_Ext_true:
        case SyntaxKind::Keyword_Ext_false:
            parseConstant_AtFirst<ConstantExpressionSyntax>(
                    expr,
                    SyntaxKind::BooleanConstantExpression);
            break;

        case SyntaxKind::Keyword_Ext_NULL:
        case SyntaxKind::Keyword_Ext_nullptr:
            parseConstant_AtFirst<ConstantExpressionSyntax>(
                    expr,
                    SyntaxKind::NULL_ConstantExpression);
            break;

        case SyntaxKind::StringLiteralToken:
        case SyntaxKind::StringLiteral_L_Token:
        case SyntaxKind::StringLiteral_u8_Token:
        case SyntaxKind::StringLiteral_u_Token:
        case SyntaxKind::StringLiteral_U_Token:
        case SyntaxKind::StringLiteral_R_Token:
        case SyntaxKind::StringLiteral_LR_Token:
        case SyntaxKind::StringLiteral_u8R_Token:
        case SyntaxKind::StringLiteral_uR_Token:
        case SyntaxKind::StringLiteral_UR_Token:
            parseStringLiteral_AtFirst(expr);
            break;

        case SyntaxKind::Keyword__Generic:
            if (!parseGenericSelectionExpression_AtFirst(expr))
                return false;
            break;

        case SyntaxKind::Keyword_ExtGNU___builtin_va_arg:
        case SyntaxKind::Keyword_MacroStd_va_arg:
            return parseVAArgumentExpression_AtFirst(expr);

        case SyntaxKind::Keyword_ExtGNU___builtin_offsetof:
        case SyntaxKind::Keyword_MacroStd_offsetof:
            return parseOffsetOfExpression_AtFirst(expr);

        case SyntaxKind::Keyword_ExtGNU___builtin_choose_expr:
            return parseExtGNU_ChooseExpression_AtFirst(expr);

        case SyntaxKind::Keyword___func__:
        case SyntaxKind::Keyword_ExtGNU___FUNCTION__:
        case SyntaxKind::Keyword_ExtGNU___PRETTY_FUNCTION__:
            parsePredefinedName_AtFirst(expr);
            break;

        case SyntaxKind::Keyword_ExtGNU___real__:
            return parseExtGNU_ComplexValuedExpression_AtFirst(expr, SyntaxKind::ExtGNU_RealExpression);

        case SyntaxKind::Keyword_ExtGNU___imag__:
            return parseExtGNU_ComplexValuedExpression_AtFirst(expr, SyntaxKind::ExtGNU_ImagExpression);

        default:
            diagReporter_.ExpectedFIRSTofExpression();
            return false;
    }

    return parsePostfixExpression_AtFollowOfPrimary(expr);
}

/**
 *
 * Adjusted grammar:
 *
 \verbatim
 postfix-expression-at-follow-of-primary:
     [ expression ] postfix-expression-at-follow-of-primary
     ( argument-expression-list_opt ) postfix-expression-at-follow-of-primary
      . identifier postfix-expression-at-follow-of-primary
     -> identifier postfix-expression-at-follow-of-primary
     ++ postfix-expression-at-follow-of-primary
     -- postfix-expression-at-follow-of-primary
     ε
 \endverbatim
 *
 * \remark 6.5.2
 */

bool Parser::parsePostfixExpression_AtFollowOfPrimary(ExpressionSyntax*& expr)
{
    while (true) {
        SyntaxKind exprK = SyntaxKind::UnknownSyntax;
        switch (peek().kind()) {
            /* 6.5.2.1 */
            case SyntaxKind::OpenBracketToken: {
                if (!parsePostfixExpression_AtFollowOfPrimary<ArraySubscriptExpressionSyntax>(
                            expr,
                            SyntaxKind::ElementAccessExpression,
                            [this] (ArraySubscriptExpressionSyntax*& arrExpr) {
                                arrExpr->openBracketTkIdx_ = consume();
                                return parseExpression(arrExpr->arg_)
                                        && matchOrSkipTo(SyntaxKind::CloseBracketToken, &arrExpr->closeBracketTkIdx_);
                            })) {
                   return false;
                }
                break;
            }

            /* 6.5.2.2 */
            case SyntaxKind::OpenParenToken: {
                if (!parsePostfixExpression_AtFollowOfPrimary<CallExpressionSyntax>(
                            expr,
                            SyntaxKind::CallExpression,
                            [this] (CallExpressionSyntax*& callExpr) {
                                callExpr->openParenTkIdx_ = consume();
                                if (peek().kind() == SyntaxKind::CloseParenToken) {
                                    callExpr->closeParenTkIdx_ = consume();
                                    return true;
                                }
                                return parseCallArguments(callExpr->args_)
                                        && matchOrSkipTo(SyntaxKind::CloseParenToken, &callExpr->closeParenTkIdx_);
                            })) {
                    return false;
                }
                break;
            }

            /* 6.5.2.3 */
            case SyntaxKind::DotToken:
                exprK = SyntaxKind::DirectMemberAccessExpression;
                [[fallthrough]];

            case SyntaxKind::ArrowToken: {
                if (exprK == SyntaxKind::UnknownSyntax)
                    exprK = SyntaxKind::IndirectMemberAccessExpression;
                if (!parsePostfixExpression_AtFollowOfPrimary<MemberAccessExpressionSyntax>(
                            expr,
                            exprK,
                            [this] (MemberAccessExpressionSyntax*& membAccess) {
                                membAccess->oprtrTkIdx_ = consume();
                                if (peek().kind() == SyntaxKind::IdentifierToken) {
                                    ExpressionSyntax* identExpr = nullptr;
                                    parseIdentifierName_AtFirst(identExpr);
                                    membAccess->identExpr_ = identExpr->asIdentifierName();
                                    return true;
                                }

                                diagReporter_.ExpectedFieldName();
                                return false;
                            })) {
                    return false;
                }
                break;
            }

            /* 6.5.2.4 */
            case SyntaxKind::PlusPlusToken:
                exprK = SyntaxKind::PostIncrementExpression;
                [[fallthrough]];

            case SyntaxKind::MinusMinusToken: {
                if (exprK == SyntaxKind::UnknownSyntax)
                    exprK = SyntaxKind::PostDecrementExpression;
                if (!parsePostfixExpression_AtFollowOfPrimary<PostfixUnaryExpressionSyntax>(
                            expr,
                            exprK,
                            [this] (PostfixUnaryExpressionSyntax*& incDecExpr) {
                                incDecExpr->oprtrTkIdx_ = consume();
                                return true;
                            })) {
                    return false;
                }
                break;
            }

            default:
                return true;
        }
    }
}

/**
 * Parse a \a postfix-expression, with LA(1) at the postfix start.
 *
 * In the adjusted grammar of Parser::parseExpressionWithPrecedencePostfix.
 *
 \verbatim
 postfix-expression-at-postfix:
     [ expression ]
     ( argument-expression-list_opt )
     . identifier
     -> identifier
     ++
     --
 \endverbatim
 */
template <class ExprT>
bool Parser::parsePostfixExpression_AtFollowOfPrimary(ExpressionSyntax*& expr,
                                              SyntaxKind exprK,
                                              std::function<bool(ExprT*&)> parsePostfix)
{
    DBG_THIS_RULE();
    PSY_ASSERT_3(peek().kind() == SyntaxKind::OpenBracketToken
                        || peek().kind() == SyntaxKind::OpenParenToken
                        || peek().kind() == SyntaxKind::DotToken
                        || peek().kind() == SyntaxKind::ArrowToken
                        || peek().kind() == SyntaxKind::PlusPlusToken
                        || peek().kind() == SyntaxKind::MinusMinusToken,
                  return false,
                  "assert failure: `[', `(', `.', `->', '++', or `--'");

    auto postfixExpr = makeNode<ExprT>(exprK);
    postfixExpr->expr_ = expr;
    expr = postfixExpr;
    return parsePostfix(postfixExpr);
}

/**
 * Parse an \a argument-expression-list.
 *
 \verbatim
 argument-expression-list:
     assignment-expression
     argument-expression-list , assignment-expression
 \endverbatim
 *
 * \remark 6.5.2
 */
bool Parser::parseCallArguments(ExpressionListSyntax*& exprList)
{
    DBG_THIS_RULE();

    return parseCommaSeparatedItems<ExpressionSyntax>(
                exprList,
                &Parser::parseCallArgument);
}

bool Parser::parseCallArgument(ExpressionSyntax*&expr, ExpressionListSyntax*&)
{
    return parseExpressionWithPrecedenceAssignment(expr);
}

/**
 * Parser the variable argument "macro" \c va_arg.
 *
 * \remark 7.16.1.1
 */
bool Parser::parseVAArgumentExpression_AtFirst(ExpressionSyntax*& expr)
{
    DBG_THIS_RULE();
    PSY_ASSERT_3(peek().kind() == SyntaxKind::Keyword_ExtGNU___builtin_va_arg
                    || peek().kind() == SyntaxKind::Keyword_MacroStd_va_arg,
                  return false,
                  "assert failure: `__builtin_va_arg'");

    if (!tree_->parseOptions().languageExtensions().isEnabled_extGNU_InternalBuiltins())
        diagReporter_.ExpectedFeature("GNU internal builtins");

    auto vaArgExpr = makeNode<VAArgumentExpressionSyntax>();
    expr = vaArgExpr;
    vaArgExpr->kwTkIdx_ = consume();
    return match(SyntaxKind::OpenParenToken, &vaArgExpr->openParenTkIdx_)
            && parseExpressionWithPrecedenceAssignment(vaArgExpr->expr_)
            && match(SyntaxKind::CommaToken, &vaArgExpr->commaTkIdx_)
            && parseTypeName(vaArgExpr->typeName_)
            && match(SyntaxKind::CloseParenToken, &vaArgExpr->closeParenTkIdx_);
}

/**
 * Parse GNU's \c __builtin_offsetof and Standard \c offsetof.
 */
bool Parser::parseOffsetOfExpression_AtFirst(ExpressionSyntax*& expr)
{
    DBG_THIS_RULE();
    PSY_ASSERT_3(peek().kind() == SyntaxKind::Keyword_ExtGNU___builtin_offsetof
                  || peek().kind() == SyntaxKind::Keyword_MacroStd_offsetof,
                  return false,
                  "assert failure: `__builtin_offsetof'");

    if (!tree_->parseOptions().languageExtensions().isEnabled_extGNU_InternalBuiltins())
        diagReporter_.ExpectedFeature("GNU internal builtins");

    auto offsetOfExpr = makeNode<OffsetOfExpressionSyntax>();
    expr = offsetOfExpr;
    offsetOfExpr->kwTkIdx_ = consume();
    return match(SyntaxKind::OpenParenToken, &offsetOfExpr->openParenTkIdx_)
        && parseTypeName(offsetOfExpr->typeName_)
        && match(SyntaxKind::CommaToken, &offsetOfExpr->commaTkIdx_)
        && parseOffsetOfDesignator(offsetOfExpr->offsetOfDesignator_)
        && match(SyntaxKind::CloseParenToken, &offsetOfExpr->closeParenTkIdx_);
}

/**
 * Parse the GNU built-in function \c __builtin_choose_expr.
 *
 */
bool Parser::parseExtGNU_ChooseExpression_AtFirst(ExpressionSyntax*& expr)
{
    DBG_THIS_RULE();
    PSY_ASSERT_3(peek().kind() == SyntaxKind::Keyword_ExtGNU___builtin_choose_expr,
                  return false,
                  "assert failure: `__builtin_choose_expr'");

    if (!tree_->parseOptions().languageExtensions().isEnabled_extGNU_InternalBuiltins())
        diagReporter_.ExpectedFeature("GNU internal builtins");

    auto chooseExpr = makeNode<ExtGNU_ChooseExpressionSyntax>();
    expr = chooseExpr;
    chooseExpr->kwTkIdx_ = consume();
    return match(SyntaxKind::OpenParenToken, &chooseExpr->openParenTkIdx_)
        && parseConstant<ConstantExpressionSyntax>(chooseExpr->constExpr_, SyntaxKind::IntegerConstantExpression)
        && match(SyntaxKind::CommaToken, &chooseExpr->commaTkIdx1_)
        && parseExpressionWithPrecedenceAssignment(chooseExpr->expr1_)
        && match(SyntaxKind::CommaToken, &chooseExpr->commaTkIdx2_)
        && parseExpressionWithPrecedenceAssignment(chooseExpr->expr2_)
        && match(SyntaxKind::CloseParenToken, &chooseExpr->closeParenTkIdx_);
}

/**
 * Parse the GNU __real__ and __imag__ expressions.
 */
bool Parser::parseExtGNU_ComplexValuedExpression_AtFirst(ExpressionSyntax*& expr, SyntaxKind exprK)
{
    DBG_THIS_RULE();
    PSY_ASSERT_3(peek().kind() == SyntaxKind::Keyword_ExtGNU___real__
                      || peek().kind() == SyntaxKind::Keyword_ExtGNU___imag__,
                  return false,
                  "assert failure: `__real__' or `__imag__'");

    auto complexValExpr = makeNode<ExtGNU_ComplexValuedExpressionSyntax>(exprK);
    expr = complexValExpr;
    complexValExpr->oprtrTkIdx_ = consume();
    return parseExpressionWithPrecedenceAssignment(complexValExpr->expr_);
}

/**
 * Parse a \a postfix-expression that is a compound literal,
 * with LA(1) at \c (.
 *
 * In the adjusted grammar of Parser::parseExpressionWithPrecedencePostfix.
 *
 \verbatim
 compound-literal-at-open-paren:
    ( type-name ) compound-literal-at-open-brace
 \endverbatim
 */
bool Parser::parseCompoundLiteral_AtOpenParen(ExpressionSyntax*& expr)
{
    DBG_THIS_RULE();
    PSY_ASSERT_3(peek().kind() == SyntaxKind::OpenParenToken,
                  return false,
                  "assert failure: `('");

    auto openParenTkIdx = consume();
    TypeNameSyntax* typeName = nullptr;
    if (!parseTypeName(typeName))
        return false;

    auto closeParenTkIdx = LexedTokens::invalidIndex();
    if (!match(SyntaxKind::CloseParenToken, &closeParenTkIdx))
        return false;

    if (peek().kind() != SyntaxKind::OpenBraceToken) {
        diagReporter_.ExpectedToken(SyntaxKind::OpenBraceToken);
        return false;
    }

    return parseCompoundLiteral_AtOpenBrace(expr, openParenTkIdx, typeName, closeParenTkIdx);
}

/**
 * Parse a \a postfix-expression that is a compound literal,
 * with LA(1) at \c {.
 *
 * In the adjusted grammar of Parser::parseCompoundLiteral_AtOpenParen.
 *
 \verbatim
 compound-literal-at-open-brace:
    { initializer-list }
    { initializer-list, }
 \endverbatim
 */
bool Parser::parseCompoundLiteral_AtOpenBrace(
        ExpressionSyntax*& expr,
        LexedTokens::IndexType openParenTkIdx,
        TypeNameSyntax* typeName,
        LexedTokens::IndexType closeParenTkIdx)
{
    DBG_THIS_RULE();
    PSY_ASSERT_3(peek().kind() == SyntaxKind::OpenBraceToken,
                  return false,
                  "assert failure: `{'");

    if (tree_->dialect().std() < LanguageDialect::Std::C99
            && !tree_->parseOptions().languageExtensions().isEnabled_extGNU_CompoundLiterals()) {
        diagReporter_.ExpectedFeature("GNU/C99 compound literals");
    }

    auto compLit = makeNode<CompoundLiteralExpressionSyntax>();
    expr = compLit;
    compLit->openParenTkIdx_ = openParenTkIdx;
    compLit->typeName_ = typeName;
    compLit->closeParenTkIdx_ = closeParenTkIdx;
    return parseInitializer(compLit->init_)
        && parsePostfixExpression_AtFollowOfPrimary(expr);
}

/* Unary */

/**
 * Parse a \a unary-expression, or any expression that is subsumed by such rule.
 *
 * See note about naming convention in Parser::parseExpression.
 *
 \verbatim
 unary-expression:
     postfix-expression
     ++ unary-expression
     -- unary-expression
     unary-operator cast-expression
     sizeof unary-expression
     sizeof ( type-name )
     _Alignof ( type-name )

 unary-operator: & * + - ~ !
 \endverbatim
 *
 * Adjusted grammar:
 *
 \verbatim
 unary-expression:
     postfix-expression
     prefix-unary-expression-at-first unary-expression
     prefix-unary-expression-at-first cast-expression
     type-trait-expression
 \endverbatim
 *
 * \remark 6.5.3
 */
bool Parser::parseExpressionWithPrecedenceUnary(ExpressionSyntax*& expr)
{
    DBG_THIS_RULE();

    switch (peek().kind()) {
        /* 6.5.3.1 */
        case SyntaxKind::PlusPlusToken:
            return parsePrefixUnaryExpression_AtFirst(
                        expr,
                        SyntaxKind::PreIncrementExpression,
                        &Parser::parseExpressionWithPrecedenceUnary);

        case SyntaxKind::MinusMinusToken:
            return parsePrefixUnaryExpression_AtFirst(
                        expr,
                        SyntaxKind::PreDecrementExpression,
                        &Parser::parseExpressionWithPrecedenceUnary);

        /* 6.5.3.2 */
        case SyntaxKind::AmpersandToken:
            return parsePrefixUnaryExpression_AtFirst(
                        expr,
                        SyntaxKind::AddressOfExpression,
                        &Parser::parseExpressionWithPrecedenceCast);

        case SyntaxKind::AsteriskToken:
            return parsePrefixUnaryExpression_AtFirst(
                        expr,
                        SyntaxKind::PointerIndirectionExpression,
                        &Parser::parseExpressionWithPrecedenceCast);

        /* 6.5.3.3 */
        case SyntaxKind::PlusToken:
            return parsePrefixUnaryExpression_AtFirst(
                        expr,
                        SyntaxKind::UnaryPlusExpression,
                        &Parser::parseExpressionWithPrecedenceCast);

        case SyntaxKind::MinusToken:
            return parsePrefixUnaryExpression_AtFirst(
                        expr,
                        SyntaxKind::UnaryMinusExpression,
                        &Parser::parseExpressionWithPrecedenceCast);

        case SyntaxKind::TildeToken:
            return parsePrefixUnaryExpression_AtFirst(
                        expr,
                        SyntaxKind::BitwiseNotExpression,
                        &Parser::parseExpressionWithPrecedenceCast);

        case SyntaxKind::ExclamationToken:
            return parsePrefixUnaryExpression_AtFirst(
                        expr,
                        SyntaxKind::LogicalNotExpression,
                        &Parser::parseExpressionWithPrecedenceCast);

        /* 6.5.3.4 */
        case SyntaxKind::Keyword_sizeof:
            return parseTypeTraitExpression_AtFirst(expr, SyntaxKind::SizeofExpression);

        case SyntaxKind::Keyword__Alignof:
            return parseTypeTraitExpression_AtFirst(expr, SyntaxKind::AlignofExpression);

        /* GNU labels as values */
        case SyntaxKind::AmpersandAmpersandToken:
            if (!tree_->parseOptions().languageExtensions().isEnabled_extGNU_LabelsAsValues())
                diagReporter_.ExpectedFeature("GNU labels as values");
            return parsePrefixUnaryExpression_AtFirst(
                        expr,
                        SyntaxKind::ExtGNU_LabelAddress,
                        &Parser::parseExpressionWithPrecedenceCast);

        default:
            return parseExpressionWithPrecedencePostfix(expr);
    }
}

/**
 * Parse a \a unary-expression that is a \b prefix \a unary expression,
 * with LA(1) at the operator.
 *
 * In the adjusted grammar of Parser::parseExpressionWithPrecedenceUnary.
 *
 \verbatim
 prefix-unary-expression-at-first:
     ++ unary-expression
     -- unary-expression
     unary-operator cast-expression

 unary-operator: & * + - ~ !
 \endverbatim
 *
 * \remark 6.5.3
 */
bool Parser::parsePrefixUnaryExpression_AtFirst(
        ExpressionSyntax*& expr,
        SyntaxKind exprK,
        bool (Parser::*parseOperand)(ExpressionSyntax*&))
{
    DBG_THIS_RULE();
    PSY_ASSERT_3(peek().kind() == SyntaxKind::PlusPlusToken
                    || peek().kind() == SyntaxKind::MinusMinusToken
                    || peek().kind() == SyntaxKind::AmpersandToken
                    || peek().kind() == SyntaxKind::AsteriskToken
                    || peek().kind() == SyntaxKind::PlusToken
                    || peek().kind() == SyntaxKind::MinusToken
                    || peek().kind() == SyntaxKind::TildeToken
                    || peek().kind() == SyntaxKind::ExclamationToken
                    || peek().kind() == SyntaxKind::AmpersandAmpersandToken,
                  return false,
                  "expected `[', `(', `.', `->', '++', or `--'");

    auto unaryExpr = makeNode<PrefixUnaryExpressionSyntax>(exprK);
    expr = unaryExpr;
    unaryExpr->oprtrTkIdx_ = consume();
    return ((this)->*parseOperand)(unaryExpr->expr_);
}

/**
 * Parse a \a unary-expression that is type-trait \a unary expression,
 * with LA(1) at FIRST.
 *
 * In the adjusted grammar of Parser::parseExpressionWithPrecedenceUnary.
 *
 \verbatim
 type-trait-expression:
     sizeof unary-expression
     sizeof ( type-name )
     _Alignof ( type-name )
 \endverbatim
 *
 * \remark 6.5.3
 */
bool Parser::parseTypeTraitExpression_AtFirst(ExpressionSyntax*& expr, SyntaxKind exprK)
{
    DBG_THIS_RULE();
    PSY_ASSERT_3(peek().kind() == SyntaxKind::Keyword_sizeof
                    || peek().kind() == SyntaxKind::Keyword__Alignof,
                  return false,
                  "assert failure: `sizeof' or `_Alignof'");

    auto traitExpr = makeNode<TypeTraitExpressionSyntax>(exprK);
    expr = traitExpr;
    traitExpr->oprtrTkIdx_ = consume();

    return parseParenthesizedTypeNameOrExpression(traitExpr->tyRef_);
}

/* Cast */

/**
 * Parse a \a cast-expression, or any expression that is subsumed by such rule.
 *
 * See note about naming convention in Parser::parseExpression.
 *
 \verbatim
 cast-expression:
     unary-expression
     ( type-name ) cast-expression
 \endverbatim
 *
 * \remark 6.5.4
 */
bool Parser::parseExpressionWithPrecedenceCast(ExpressionSyntax*& expr)
{
    DBG_THIS_RULE();

    switch (peek().kind()) {
        case SyntaxKind::OpenParenToken: {
            // cast-expression -> unary-expression ->* `(' expression `)'
            //                  | `(' type-name `)' cast-expression
            switch (peek(2).kind()) {
                // type-name ->* type-qualifier
                case SyntaxKind::Keyword_const:
                case SyntaxKind::Keyword_volatile:
                case SyntaxKind::Keyword_restrict:
                case SyntaxKind::Keyword__Atomic:

                // type-name ->* alignment-specifier
                case SyntaxKind::Keyword__Alignas:

                // type-name ->* GNU-typeof-specifier ->
                case SyntaxKind::Keyword_ExtGNU___typeof__:

                // // type-name ->* type-specifier
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
                    return parseCompoundLiteralOrCastExpression_AtFirst(expr);

                // type-name ->* typedef-name -> identifier
                // expression ->* identifier
                case SyntaxKind::IdentifierToken: {
                    Backtracker BT(this);
                    if (parseCompoundLiteralOrCastExpression_AtFirst(expr)) {
                        if (expr->kind() == SyntaxKind::CastExpression)
                            maybeAmbiguateCastExpression(expr);
                        return true;
                    }
                    BT.backtrack();
                    [[fallthrough]];
                }

                default:
                    return parseExpressionWithPrecedenceUnary(expr);
            }
        }

        case SyntaxKind::Keyword_ExtGNU___extension__: {
            auto extKwTkIdx = consume();
            if (!parseExpressionWithPrecedenceCast(expr))
                return false;
            PSY_ASSERT_3(expr, return false, "invalid expression");
            expr->extKwTkIdx_ = extKwTkIdx;
            return true;
        }

        default:
            return parseExpressionWithPrecedenceUnary(expr);
    }
}

bool Parser::parseCompoundLiteralOrCastExpression_AtFirst(ExpressionSyntax*& expr)
{
    DBG_THIS_RULE();
    PSY_ASSERT_3(peek().kind() == SyntaxKind::OpenParenToken,
                  return false,
                  "assert failure: `('");

    auto openParenTkIdx = consume();
    TypeNameSyntax* typeName = nullptr;
    if (!parseTypeName(typeName))
        return false;

    LexedTokens::IndexType closeParenTkIdx;
    if (!match(SyntaxKind::CloseParenToken, &closeParenTkIdx))
        return false;

    if (peek().kind() == SyntaxKind::OpenBraceToken)
        return parseCompoundLiteral_AtOpenBrace(expr,
                                                openParenTkIdx,
                                                typeName,
                                                closeParenTkIdx);

    auto castExpr = makeNode<CastExpressionSyntax>();
    expr = castExpr;
    castExpr->openParenTkIdx_ = openParenTkIdx;
    castExpr->typeName_ = typeName;
    castExpr->closeParenTkIdx_ = closeParenTkIdx;
    return parseExpressionWithPrecedenceCast(castExpr->expr_);
}

void Parser::maybeAmbiguateCastExpression(ExpressionSyntax*& expr)
{
    PSY_ASSERT_3(expr->kind() == SyntaxKind::CastExpression,
                  return, "");

    auto castExpr = expr->asCastExpression();
    auto prefixExpr = castExpr->expr_->asPrefixUnaryExpression();
    if (!(prefixExpr->asPrefixUnaryExpression()
            && (prefixExpr->kind() == SyntaxKind::AddressOfExpression
                    || prefixExpr->kind() == SyntaxKind::PointerIndirectionExpression
                    || prefixExpr->kind() == SyntaxKind::UnaryPlusExpression
                    || prefixExpr->kind() == SyntaxKind::UnaryMinusExpression)))
        return;

    TypeNameSyntax* typeName = castExpr->typeName_;
    if (!(typeName->specs_
            && typeName->specs_->value->kind() == SyntaxKind::TypedefName
            && !typeName->specs_->next
            && typeName->decltor_
            && typeName->decltor_->kind() == SyntaxKind::AbstractDeclarator))
        return;

    SyntaxKind binExprK;
    switch (prefixExpr->kind()) {
        case SyntaxKind::AddressOfExpression:
            binExprK = SyntaxKind::BitwiseANDExpression;
            break;

        case SyntaxKind::PointerIndirectionExpression:
            binExprK = SyntaxKind::MultiplyExpression;
            break;

        case SyntaxKind::UnaryPlusExpression:
            binExprK = SyntaxKind::AddExpression;
            break;

        case SyntaxKind::UnaryMinusExpression:
            binExprK = SyntaxKind::SubstractExpression;
            break;

        default:
            PSY_ASSERT_3(false, return, "");
    }

    auto nameExpr = makeNode<IdentifierNameSyntax>();
    nameExpr->identTkIdx_ =
            typeName->specs_->value->asTypedefName()->identTkIdx_;
    auto parenExpr = makeNode<ParenthesizedExpressionSyntax>();
    parenExpr->expr_ = nameExpr;
    parenExpr->openParenTkIdx_ = castExpr->openParenTkIdx_;
    auto binExpr = makeNode<BinaryExpressionSyntax>(binExprK);
    binExpr->leftExpr_ = parenExpr;
    parenExpr->closeParenTkIdx_ = castExpr->closeParenTkIdx_;
    binExpr->oprtrTkIdx_ = prefixExpr->oprtrTkIdx_;
    binExpr->rightExpr_ = prefixExpr->expr_;

    auto ambigExpr = makeNode<AmbiguousCastOrBinaryExpressionSyntax>(SyntaxKind::AmbiguousCastOrBinaryExpression);
    expr = ambigExpr;
    ambigExpr->castExpr_ = castExpr;
    ambigExpr->binExpr_ = binExpr;

    diagReporter_.AmbiguousCastOrBinaryExpression(ambigExpr);
}

/* N-ary */

namespace NAryPrecedence {

enum : std::uint8_t
{
    Undefined = 0,
    Sequencing,
    Assignment,
    Conditional,
    LogicalOR,
    LogicalAND,
    BitwiseOR,
    BitwiseXOR,
    BitwiseAND,
    Equality,
    Relational,
    Shift,
    Additive,
    Multiplicative
};

} // NAryPrecedence

std::uint8_t precedenceOf(SyntaxKind tkK)
{
    switch (tkK) {
        case SyntaxKind::CommaToken:
            return NAryPrecedence::Sequencing;

        case SyntaxKind::EqualsToken:
        case SyntaxKind::PlusEqualsToken:
        case SyntaxKind::MinusEqualsToken:
        case SyntaxKind::AsteriskEqualsToken:
        case SyntaxKind::SlashEqualsToken:
        case SyntaxKind::PercentEqualsToken:
        case SyntaxKind::LessThanLessThanEqualsToken:
        case SyntaxKind::GreaterThanGreaterThanEqualsToken:
        case SyntaxKind::AmpersandEqualsToken:
        case SyntaxKind::CaretEqualsToken:
        case SyntaxKind::BarEqualsToken:
            return NAryPrecedence::Assignment;

        case SyntaxKind::QuestionToken:
            return NAryPrecedence::Conditional;

        case SyntaxKind::BarBarToken:
            return NAryPrecedence::LogicalOR;

        case SyntaxKind::AmpersandAmpersandToken:
            return NAryPrecedence::LogicalAND;

        case SyntaxKind::BarToken:
            return NAryPrecedence::BitwiseOR;

        case SyntaxKind::CaretToken:
            return NAryPrecedence::BitwiseXOR;

        case SyntaxKind::AmpersandToken:
            return NAryPrecedence::BitwiseAND;

        case SyntaxKind::EqualsEqualsToken:
        case SyntaxKind::ExclamationEqualsToken:
            return NAryPrecedence::Equality;

        case SyntaxKind::GreaterThanToken:
        case SyntaxKind::LessThanToken:
        case SyntaxKind::LessThanEqualsToken:
        case SyntaxKind::GreaterThanEqualsToken:
            return NAryPrecedence::Relational;

        case SyntaxKind::LessThanLessThanToken:
        case SyntaxKind::GreaterThanGreaterThanToken:
            return NAryPrecedence::Shift;

        case SyntaxKind::PlusToken:
        case SyntaxKind::MinusToken:
            return NAryPrecedence::Additive;

        case SyntaxKind::AsteriskToken:
        case SyntaxKind::SlashToken:
        case SyntaxKind::PercentToken:
            return NAryPrecedence::Multiplicative;

        default:
            return NAryPrecedence::Undefined;
    }
}

bool isRightAssociative(SyntaxKind tkK)
{
    auto prec = precedenceOf(tkK);
    return prec == NAryPrecedence::Conditional || prec == NAryPrecedence::Assignment;
}

/**
 * Parse a \a multiplicative-expression, or any expression that is subsumed by such rule.
 *
 * See note about naming convention in Parser::parseExpression.
 *
 \verbatim
 multiplicative-expression:
     cast-expression
     multiplicative-expression * cast-expression
     multiplicative-expression / cast-expression
     multiplicative-expression % cast-expression
 \endverbatim
 *
 * \remark 6.5.5
 */
bool Parser::parseExpressionWithPrecedenceMultiplicative(ExpressionSyntax*& expr)
{
    DBG_THIS_RULE();

    return parseNAryExpression(expr, NAryPrecedence::Multiplicative);
}

/**
 * Parse a \a additive-expression, or any expression that is subsumed by such rule.
 *
 * See note about naming convention in Parser::parseExpression.
 *
 \verbatim
 additive-expression:
     multiplicative-expression
     additive-expression + multiplicative-expression
     additive-expression - multiplicative-expression
 \endverbatim
 *
 * \remark 6.5.6
 */
bool Parser::parseExpressionWithPrecedenceAdditive(ExpressionSyntax*& expr)
{
    DBG_THIS_RULE();

    return parseNAryExpression(expr, NAryPrecedence::Additive);
}

/**
 * Parse a \a shift-expression, or any expression that is subsumed by such rule.
 *
 * See note about naming convention in Parser::parseExpression.
 *
 \verbatim
 shift-expression:
     additive-expression
     shift-expression << additive-expression
     shift-expression >> additive-expression
 \endverbatim
 *
 * \remark 6.5.7
 */
bool Parser::parseExpressionWithPrecedenceShift(ExpressionSyntax*& expr)
{
    DBG_THIS_RULE();

    return parseNAryExpression(expr, NAryPrecedence::Shift);
}

/**
 * Parse a \a relational-expression, or any expression that is subsumed by such rule.
 *
 * See note about naming convention in Parser::parseExpression.
 *
 \verbatim
 relational-expression:
     shift-expression
     relational-expression < shift-expression
     relational-expression > shift-expression
     relational-expression <= shift-expression
     relational-expression >= shift-expression
 \endverbatim
 *
 * \remark 6.5.8
 */
bool Parser::parseExpressionWithPrecedenceRelational(ExpressionSyntax*& expr)
{
    DBG_THIS_RULE();

    return parseNAryExpression(expr, NAryPrecedence::Relational);
}

/**
 * Parse a \a multiplicative-expression, or any expression that is subsumed by such rule.
 *
 * See note about naming convention in Parser::parseExpression.
 *
 \verbatim
 equality-expression:
     relational-expression
     equality-expression == relational-expression
     equality-expression != relational-expression
 \endverbatim
 *
 * \remark 6.5.9
 */
bool Parser::parseExpressionWithPrecedenceEquality(ExpressionSyntax*& expr)
{
    DBG_THIS_RULE();

    return parseNAryExpression(expr, NAryPrecedence::Equality);
}

/**
 * Parse a \a AND-expression, or any expression that is subsumed by such rule.
 *
 * See note about naming convention in Parser::parseExpression.
 *
 \verbatim
 AND-expression:
     equality-expression
     AND-expression & equality-expression
 \endverbatim
 *
 * \remark 6.5.10
 */
bool Parser::parseExpressionWithPrecedenceBitwiseAND(ExpressionSyntax*& expr)
{
    DBG_THIS_RULE();

    return parseNAryExpression(expr, NAryPrecedence::BitwiseAND);
}

/**
 * Parse a \a exclusive-OR-expression, or any expression that is subsumed by such rule.
 *
 * See note about naming convention in Parser::parseExpression.
 *
 \verbatim
 exclusive-OR-expression:
     AND-expression
     exclusive-OR-expression ^ AND-expression
 \endverbatim
 *
 * \remark 6.5.11
 */
bool Parser::parseExpressionWithPrecedenceBitwiseXOR(ExpressionSyntax*& expr)
{
    DBG_THIS_RULE();

    return parseNAryExpression(expr, NAryPrecedence::BitwiseXOR);
}

/**
 * Parse a \a inclusive-OR-expression, or any expression that is subsumed by such rule.
 *
 * See note about naming convention in Parser::parseExpression.
 *
 \verbatim
 inclusive-OR-expression:
     exclusive-OR-expression
     inclusive-OR-expression | exclusive-OR-expression
 \endverbatim
 *
 * \remark 6.5.12
 */
bool Parser::parseExpressionWithPrecedenceBitwiseOR(ExpressionSyntax*& expr)
{
    DBG_THIS_RULE();

    return parseNAryExpression(expr, NAryPrecedence::BitwiseOR);
}

/**
 * Parse a \a logical-AND-expression, or any expression that is subsumed by such rule.
 *
 * See note about naming convention in Parser::parseExpression.
 *
 \verbatim
 logical-AND-expression:
     inclusive-OR-expression
     logical-AND-expression && inclusive-OR-expression
 \endverbatim
 *
 * \remark 6.5.13
 */
bool Parser::parseExpressionWithPrecedenceLogicalAND(ExpressionSyntax*& expr)
{
    DBG_THIS_RULE();

    return parseNAryExpression(expr, NAryPrecedence::LogicalAND);
}

/**
 * Parse a \a logical-OR-expression, or any expression that is subsumed by such rule.
 *
 * See note about naming convention in Parser::parseExpression.
 *
 \verbatim
 logical-OR-expression:
     logical-AND-expression
     logical-OR-expression || logical-AND-expression
 \endverbatim
 *
 * \remark 6.5.14
 */
bool Parser::parseExpressionWithPrecedenceLogicalOR(ExpressionSyntax*& expr)
{
    DBG_THIS_RULE();

    return parseNAryExpression(expr, NAryPrecedence::LogicalOR);
}

/**
 * Parse a \a conditional-expression, or any expression that is subsumed by such rule.
 *
 * See note about naming convention in Parser::parseExpression.
 *
 \verbatim
 conditional-expression:
     logical-OR-expression
     logical-OR-expression ? expression : conditional-expression
 \endverbatim
 *
 * \remark 6.5.15
 */
bool Parser::parseExpressionWithPrecedenceConditional(ExpressionSyntax*& expr)
{
    DBG_THIS_RULE();

    return parseNAryExpression(expr, NAryPrecedence::Conditional);
}

/**
 * Parse a \a assignment-expression, or any expression that is subsumed by such rule.
 *
 * See note about naming convention in Parser::parseExpression.
 *
 \verbatim
 assignment-expression:
     conditional-expression
     unary-expression assignment-operator assignment-expression

 assignment-operator: one of
     = *= /= %= += -= <<= >>= &= ^= |=
 \endverbatim
 *
 * \remark 6.5.16
 */
bool Parser::parseExpressionWithPrecedenceAssignment(ExpressionSyntax*& expr)
{
    DBG_THIS_RULE();

    return parseNAryExpression(expr, NAryPrecedence::Assignment);
}

/**
 * Parse the comman operator, which is a sequence of expressions.
 *
 * \remark 6.5.17
 * \remark Parser::parseExpression
 */
bool Parser::parseExpressionWithPrecedenceComma(ExpressionSyntax*& expr)
{
    DBG_THIS_RULE();

    return parseNAryExpression(expr, NAryPrecedence::Sequencing);
}

bool Parser::parseNAryExpression(ExpressionSyntax*& expr, std::uint8_t cutoffPrecedence)
{
    DBG_THIS_RULE();

    if (!parseExpressionWithPrecedenceCast(expr))
        return false;

    return parseNAryExpression_AtOperator(expr, cutoffPrecedence);
}

bool Parser::parseNAryExpression_AtOperator(ExpressionSyntax*& baseExpr,
                                            std::uint8_t cutoffPrecedence)
{
    DBG_THIS_RULE();

    auto CUR_DEPTH_OF_EXPR = DEPTH_OF_EXPRS_;

    while (precedenceOf(peek().kind()) >= cutoffPrecedence) {
        if (++CUR_DEPTH_OF_EXPR > MAX_DEPTH_OF_EXPRS)
            throw std::runtime_error("maximum depth of expressions reached");

        auto tkK = peek().kind();
        auto exprK = SyntaxFacts::NAryExpressionKind(tkK);
        auto oprtrTkIdx = consume();

        ConditionalExpressionSyntax* condExpr = nullptr;
        if (tkK == SyntaxKind::QuestionToken) {
            condExpr = makeNode<ConditionalExpressionSyntax>();
            condExpr->questionTkIdx_ = oprtrTkIdx;

            if (peek().kind() == SyntaxKind::ColonToken) {
                if (!tree_->parseOptions().languageExtensions().isEnabled_extGNU_StatementExpressions())
                    diagReporter_.ExpectedFeature("GNU conditionals");
                condExpr->whenTrueExpr_ = nullptr;
            }
            else {
                parseExpression(condExpr->whenTrueExpr_);
            }
            match(SyntaxKind::ColonToken, &condExpr->colonTkIdx_);
        }

        ExpressionSyntax* nextExpr = nullptr;
        if (!parseExpressionWithPrecedenceCast(nextExpr))
             return false;

        auto prevPrec = precedenceOf(tkK);
        tkK = peek().kind();
        auto precAhead = precedenceOf(tkK);

        while ((precAhead > prevPrec
                        && SyntaxFacts::isNAryOperatorToken(tkK))
                   || (precAhead == prevPrec
                        && isRightAssociative(tkK))) {
            if (!parseNAryExpression_AtOperator(nextExpr, precAhead))
                return false;

            tkK = peek().kind();
            precAhead = precedenceOf(tkK);
        }

        /*
         * An "usual" N-ary expression E can be produced with a FIRST that is another N-ary
         * expression with same precedence of E or a tighter one. An assignment expression
         * is different in that its LHS may not be a N-ary expression: it must a unary one.
         */
        if (precAhead == NAryPrecedence::Assignment && prevPrec > precAhead)
            return false;

        if (condExpr) {
            condExpr->condExpr_ = baseExpr;
            condExpr->whenFalseExpr_ = nextExpr;
            baseExpr = condExpr;
        }
        else {
            if (SyntaxFacts::isAssignmentExpression(exprK)) {
                baseExpr = fill_LeftOperandInfixOperatorRightOperand_MIXIN(
                                makeNode<AssignmentExpressionSyntax>(exprK),
                                baseExpr,
                                oprtrTkIdx,
                                nextExpr);
            }
            else if (SyntaxFacts::isBinaryExpression(exprK)) {
                baseExpr = fill_LeftOperandInfixOperatorRightOperand_MIXIN(
                                makeNode<BinaryExpressionSyntax>(exprK),
                                baseExpr,
                                oprtrTkIdx,
                                nextExpr);
            }
            else {
                baseExpr = fill_LeftOperandInfixOperatorRightOperand_MIXIN(
                                makeNode<SequencingExpressionSyntax>(),
                                baseExpr,
                                oprtrTkIdx,
                                nextExpr);
            }
        }
    }

    return true;
}

template <class NodeT>
NodeT* Parser::fill_LeftOperandInfixOperatorRightOperand_MIXIN(
        NodeT* expr,
        ExpressionSyntax* left,
        LexedTokens::IndexType opTkIdx,
        ExpressionSyntax* right)
{
    expr->leftExpr_ = left;
    expr->oprtrTkIdx_ = opTkIdx;
    expr->rightExpr_ = right;
    return expr;
}
