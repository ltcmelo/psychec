// Copyright (c) 2021/22 Leandro T. C. Melo <ltcmelo@gmail.com>
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

//--------------//
// Declarations //
//--------------//

/**
 * Parse a \a translation-unit.
 */
void Parser::parseTranslationUnit(TranslationUnitSyntax*& unit)
{
    DBG_THIS_RULE();

    DeclarationListSyntax** declList_cur = &unit->decls_;

    while (true) {
        DeclarationSyntax* decl = nullptr;
        switch (peek().kind()) {
            case SyntaxKind::EndOfFile:
                return;

            case SyntaxKind::Keyword_ExtGNU___extension__: {
                auto extKwTkIdx = consume();
                if (!parseExternalDeclaration(decl))
                    break;
                PSY_ASSERT_2(decl, break);
                decl->extKwTkIdx_ = extKwTkIdx;
                break;
            }

            default:
                if (parseExternalDeclaration(decl))
                    break;
                ignoreDeclarationOrDefinition();
                continue;
        }

        *declList_cur = makeNode<DeclarationListSyntax>(decl);
        declList_cur = &(*declList_cur)->next;
    }

    diagReporter_.diagnoseDelayedDiagnostics();
}

/**
 * Parse an \a external-declaration.
 *
 \verbatim
 external-declaration:
     function-definition
     declaration
 \endverbatim
 *
 * \remark 6.9
 */
bool Parser::parseExternalDeclaration(DeclarationSyntax*& decl)
{
    DBG_THIS_RULE();

    switch (peek().kind()) {
        case SyntaxKind::SemicolonToken:
            consume();
            break;

        case SyntaxKind::Keyword__Static_assert:
            return parseStaticAssertDeclaration_AtFirst(decl);

        case SyntaxKind::Keyword_ExtGNU___asm__:
            return parseExtGNU_AsmStatementDeclaration_AtFirst(decl);

        case SyntaxKind::Keyword_ExtPSY__Template:
            parseExtPSY_TemplateDeclaration_AtFirst(decl);
            break;

        default:
            return parseDeclarationOrFunctionDefinition(decl);
    }

    return true;
}

void Parser::parseIncompleteDeclaration_AtFirst(DeclarationSyntax*& decl,
                                                const SpecifierListSyntax* specList)
{
    DBG_THIS_RULE();
    PSY_ASSERT_3(peek().kind() == SyntaxKind::SemicolonToken,
                  return,
                  "assert failure: `;'");

    auto incompDecl = makeNode<IncompleteDeclarationSyntax>();
    decl = incompDecl;
    incompDecl->specs_ = specList;
    incompDecl->semicolonTkIdx_ = consume();
}

/**
 * Parse a \a static_assert-declaration, with LA(1) at FIRST.
 *
 \verbatim
 static_assert-declaration:
     _Static_assert ( constant-expression , string-literal ) ;
 \endverbatim
 *
 * \remark 6.7.10
 */
bool Parser::parseStaticAssertDeclaration_AtFirst(DeclarationSyntax*& decl)
{
    DBG_THIS_RULE();
    PSY_ASSERT_3(peek().kind() == SyntaxKind::Keyword__Static_assert,
                  return false,
                  "assert failure: `_Static_assert'");

    auto assertDecl = makeNode<StaticAssertDeclarationSyntax>();
    decl = assertDecl;
    assertDecl->staticAssertKwTkIdx_ = consume();

    if (match(SyntaxKind::OpenParenToken, &assertDecl->openParenTkIdx_)
            && parseExpressionWithPrecedenceConditional(assertDecl->expr_)
            && match(SyntaxKind::CommaToken, &assertDecl->commaTkIdx_)
            && parseStringLiteral(assertDecl->strLit_)
            && match(SyntaxKind::CloseParenToken, &assertDecl->closeParenTkIdx_)
            && match(SyntaxKind::SemicolonToken, &assertDecl->semicolonTkIdx_))
        return true;

    skipTo(SyntaxKind::ColonToken);
    return false;
}

/**
 * Parse a GNU extension file-scope assembly \a statement as a \a declaration,
 * with LA(1) at FIRST.
 */
bool Parser::parseExtGNU_AsmStatementDeclaration_AtFirst(DeclarationSyntax*& decl)
{
    DBG_THIS_RULE();
    PSY_ASSERT_3(peek().kind() == SyntaxKind::Keyword_ExtGNU___asm__,
                  return false,
                  "assert failure: `asm'");

    if (!tree_->parseOptions().languageExtensions().isEnabled_extGNU_Asm())
        diagReporter_.ExpectedFeature("GNU assembly in C");

    auto asmDecl = makeNode<ExtGNU_AsmStatementDeclarationSyntax>();
    decl = asmDecl;
    asmDecl->asmTkIdx_ = consume();

    if (match(SyntaxKind::OpenParenToken, &asmDecl->openParenTkIdx_)
            && parseStringLiteral(asmDecl->strLit_)
            && match(SyntaxKind::CloseParenToken, &asmDecl->closeParenTkIdx_)
            && match(SyntaxKind::SemicolonToken, &asmDecl->semicolonTkIdx_))
        return true;

    skipTo(SyntaxKind::SemicolonToken);
    return false;
}

bool Parser::parseDeclaration(
        DeclarationSyntax*& decl,
        bool (Parser::*parse_AtDeclarator)(DeclarationSyntax*&, const SpecifierListSyntax*),
        DeclarationContext declCtx)
{
    SpecifierListSyntax* specList = nullptr;
    if (!parseDeclarationSpecifiers(decl, specList, declCtx)) {
        skipTo(SyntaxKind::SemicolonToken);
        return false;
    }

    return parseDeclarationOrStructDeclaration_AtFollowOfSpecifiers(
                decl,
                specList,
                parse_AtDeclarator);
}

bool Parser::parseDeclarationOrStructDeclaration_AtFollowOfSpecifiers(
        DeclarationSyntax*& decl,
        SpecifierListSyntax*& specList,
        bool (Parser::*parse_AtDeclarator)(DeclarationSyntax*&, const SpecifierListSyntax*))
{
    if (decl) {
        if (parse_AtDeclarator != &Parser::parseStructDeclaration_AtDeclarator
                && peek().kind() == SyntaxKind::SemicolonToken) {
            auto tagDecl = static_cast<TagDeclarationSyntax*>(decl);
            tagDecl->semicolonTkIdx_ = consume();
            return true;
        }

        auto tyDeclSpec = makeNode<TagDeclarationAsSpecifierSyntax>();
        tyDeclSpec->tagDecl_ = static_cast<TagDeclarationSyntax*>(decl);
        decl = nullptr;
        if (!specList)
            specList = makeNode<SpecifierListSyntax>(tyDeclSpec);
        else {
            for (auto iter = specList; iter; iter = iter->next) {
                if (iter->value->asTagTypeSpecifier()
                        && iter->value == tyDeclSpec->tagDecl_->typeSpec_) {
                    iter->value = tyDeclSpec;
                    break;
                }
            }
        }
    } else if (peek().kind() == SyntaxKind::SemicolonToken) {
        parseIncompleteDeclaration_AtFirst(decl, specList);
        return true;
    }

    return ((this)->*(parse_AtDeclarator))(decl, specList);
}

/**
 * Parse a (specifier-prefixed) \a declaration or a \a function-definition.
 *
 \verbatim
 declaration:
     declaration-specifiers init-decltor-list_opt ;
     static_assert-declaration

 function-definition:
     declaration-specifiers declarator declaration-list_opt compound-statement
 \endverbatim
 *
 * \remark 6.7, 6.9.1
 */
bool Parser::parseDeclarationOrFunctionDefinition(DeclarationSyntax*& decl)
{
    return parseDeclaration(
                decl,
                &Parser::parseDeclarationOrFunctionDefinition_AtDeclarator,
                DeclarationContext::Unspecified);
}

bool Parser::parseDeclarationOrFunctionDefinition_AtDeclarator(
        DeclarationSyntax*& decl,
        const SpecifierListSyntax* specList)
{
    DeclaratorListSyntax* decltorList = nullptr;
    DeclaratorListSyntax** decltorList_cur = &decltorList;

    while (true) {
        DeclaratorSyntax* decltor = nullptr;
        if (!parseDeclarator(decltor, DeclarationContext::Unspecified))
            return false;

        *decltorList_cur = makeNode<DeclaratorListSyntax>(decltor);

        InitializerSyntax** init = nullptr;
        if (peek().kind() == SyntaxKind::EqualsToken) {
            DeclaratorSyntax* unparenDecltor = const_cast<DeclaratorSyntax*>(
                    SyntaxUtilities::unparenthesizeDeclarator(decltor));
            switch (unparenDecltor->kind()) {
                case SyntaxKind::IdentifierDeclarator: {
                    auto identDecltor = unparenDecltor->asIdentifierDeclarator();
                    identDecltor->equalsTkIdx_ = consume();
                    init = &identDecltor->init_;
                    break;
                }

                case SyntaxKind::PointerDeclarator: {
                    auto ptrDecltor = unparenDecltor->asPointerDeclarator();
                    ptrDecltor->equalsTkIdx_ = consume();
                    init = &ptrDecltor->init_;
                    break;
                }

                case SyntaxKind::ArrayDeclarator: {
                    auto arrDecltor = unparenDecltor->asArrayOrFunctionDeclarator();
                    arrDecltor->equalsTkIdx_ = consume();
                    init = &arrDecltor->init_;
                    break;
                }

                case SyntaxKind::FunctionDeclarator: {
                    auto funcDecltor = unparenDecltor->asArrayOrFunctionDeclarator();
                    if (funcDecltor->innerDecltor_) {
                        auto unparenInnerDecltor = const_cast<DeclaratorSyntax*>(
                                    SyntaxUtilities::unparenthesizeDeclarator(funcDecltor->innerDecltor_));
                        if (unparenInnerDecltor->kind() == SyntaxKind::PointerDeclarator) {
                            funcDecltor->equalsTkIdx_ = consume();
                            init = &funcDecltor->init_;
                            break;
                        }
                    }
                    [[fallthrough]];
                }

                default:
                    diagReporter_.UnexpectedInitializerOfDeclarator();
                    return ignoreDeclarator();
            }
            if (!parseInitializer(*init))
                return false;
        }

        switch (peek().kind()) {
            case SyntaxKind::CommaToken:
                (*decltorList_cur)->delimTkIdx_ = consume();
                break;

            case SyntaxKind::SemicolonToken: {
                for (auto iter = specList; iter; iter = iter->next) {
                    PSY_ASSERT_2(iter->value, return false);
                    if (iter->value->kind() == SyntaxKind::TypedefStorageClass) {
                        auto tydefDecl = makeNode<TypedefDeclarationSyntax>();
                        decl = tydefDecl;
                        tydefDecl->semicolonTkIdx_ = consume();
                        tydefDecl->specs_ = const_cast<SpecifierListSyntax*>(specList);
                        tydefDecl->decltors_ = decltorList;
                        return true;
                    }
                }

                auto varAndOrFuncDecl = makeNode<VariableAndOrFunctionDeclarationSyntax>();
                decl = varAndOrFuncDecl;
                varAndOrFuncDecl->semicolonTkIdx_ = consume();
                varAndOrFuncDecl->specs_ = const_cast<SpecifierListSyntax*>(specList);
                varAndOrFuncDecl->decltors_ = decltorList;
                return true;
            }

            case SyntaxKind::OpenBraceToken:
                if (parseFunctionDefinition_AtOpenBrace(decl, specList, decltor, nullptr))
                    return true;
                [[fallthrough]];

            default: {
                if (init) {
                    diagReporter_.ExpectedFollowOfDeclaratorAndInitializer();
                    return false;
                }

                Backtracker BT(this);
                ExtKR_ParameterDeclarationListSyntax* paramKRList = nullptr;
                if (parseExtKR_ParameterDeclarationList(paramKRList)) {
                    BT.discard();
                    if (parseFunctionDefinition_AtOpenBrace(decl, specList, decltor, paramKRList)) {
                        diagReporter_.delayedDiags_.clear();
                        return true;
                    }
                    return false;
                }
                BT.backtrack();

                diagReporter_.ExpectedFollowOfDeclarator();
                return false;
            }
        }

        decltorList_cur = &(*decltorList_cur)->next;
    }
}

bool Parser::parseFunctionDefinition_AtOpenBrace(
        DeclarationSyntax*& decl,
        const SpecifierListSyntax* specList,
        DeclaratorSyntax*& decltor,
        ExtKR_ParameterDeclarationListSyntax* paramKRList)
{
    DBG_THIS_RULE();
    PSY_ASSERT_3(peek().kind() == SyntaxKind::OpenBraceToken,
                  return false,
                  "assert failure: `{'");

    const DeclaratorSyntax* prevDecltor = nullptr;
    const DeclaratorSyntax* outerDecltor = SyntaxUtilities::unparenthesizeDeclarator(decltor);
    while (outerDecltor) {
        const DeclaratorSyntax* innerDecltor =
                SyntaxUtilities::innerDeclaratorOf(outerDecltor);
        if (innerDecltor == outerDecltor)
            break;
        prevDecltor = outerDecltor;
        outerDecltor = SyntaxUtilities::unparenthesizeDeclarator(innerDecltor);
    }

    if (prevDecltor
            && prevDecltor->kind() == SyntaxKind::FunctionDeclarator) {
        auto funcDef = makeNode<FunctionDefinitionSyntax>();
        decl = funcDef;
        funcDef->specs_ = const_cast<SpecifierListSyntax*>(specList);
        funcDef->decltor_ = decltor;
        funcDef->extKR_params_ = paramKRList;
        StatementSyntax* stmt = nullptr;
        if (!parseCompoundStatement_AtFirst(stmt, StatementContext::None))
            return false;
        PSY_ASSERT_2(stmt->kind() == SyntaxKind::CompoundStatement, return false);
        funcDef->body_ = stmt->asCompoundStatement();
        return true;
    }

    return false;
}

Parser::IdentifierRole Parser::guessRoleOfIdentifier(DeclarationContext declCtx) const
{
    auto LA = 2;
    while (true) {
        switch (peek(LA).kind()) {
            case SyntaxKind::IdentifierToken:
                return IdentifierRole::TypedefName;

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
                return IdentifierRole::Declarator;

            // storage-class-specifier
            case SyntaxKind::Keyword_typedef:
            case SyntaxKind::Keyword_extern:
            case SyntaxKind::Keyword_static:
            case SyntaxKind::Keyword_auto:
            case SyntaxKind::Keyword_register:
            case SyntaxKind::Keyword__Thread_local:
            case SyntaxKind::Keyword_ExtGNU___thread:
                return IdentifierRole::TypedefName;

            // type-qualifier
            case SyntaxKind::Keyword_const:
            case SyntaxKind::Keyword_volatile:
            case SyntaxKind::Keyword_restrict:
            case SyntaxKind::Keyword__Atomic:
                return IdentifierRole::TypedefName;

            // function-specifier
            case SyntaxKind::Keyword_inline:
            case SyntaxKind::Keyword__Noreturn:
                return IdentifierRole::TypedefName;

            // alignment-specifier
            case SyntaxKind::Keyword__Alignas:
                return IdentifierRole::TypedefName;

            // attribute-specifier
            case SyntaxKind::Keyword_ExtGNU___attribute__:
                return IdentifierRole::TypedefName;

            // pointer-declarator
            case SyntaxKind::AsteriskToken:
                return IdentifierRole::TypedefName;

            case SyntaxKind::OpenParenToken: {
                if (declCtx == DeclarationContext::Parameter)
                    return IdentifierRole::TypedefName;
                auto parens = 1;
                auto check = 0;
                while (true) {
                    switch (peek(LA + 1).kind()) {
                        case SyntaxKind::OpenParenToken:
                            ++parens;
                            ++LA;
                            continue;
                        case SyntaxKind::CloseParenToken:
                            --parens;
                            if (!parens)
                                break;
                            ++LA;
                            continue;
                        case SyntaxKind::IdentifierToken:
                            if (check == 0)
                                check = -1;
                            else
                                check = 1;
                            ++LA;
                            continue;
                        case SyntaxKind::AsteriskToken:
                            ++LA;
                            continue;
                        case SyntaxKind::SemicolonToken:
                        case SyntaxKind::EndOfFile:
                            return IdentifierRole::Declarator;
                        default:
                            ++check;
                            ++LA;
                            continue;
                    }
                    break;
                }
                return check == -1
                        ? IdentifierRole::TypedefName
                        : IdentifierRole::Declarator;
            }

            case SyntaxKind::CloseParenToken:
                return peek(LA + 1).kind() == SyntaxKind::OpenBraceToken
                        ? IdentifierRole::Declarator
                        : isWithinKandRFuncDef_
                            ? IdentifierRole::Declarator
                            : IdentifierRole::TypedefName;

            case SyntaxKind::OpenBracketToken: {
                if (declCtx == DeclarationContext::Parameter)
                    return IdentifierRole::TypedefName;
                auto brackets = 1;
                auto check = 0;
                while (true) {
                    switch (peek(LA + 1).kind()) {
                        case SyntaxKind::OpenBracketToken:
                            ++brackets;
                            ++LA;
                            continue;
                        case SyntaxKind::CloseBracketToken:
                            --brackets;
                            if (!brackets)
                                break;
                            ++LA;
                            continue;
                        case SyntaxKind::IdentifierToken:
                            if (check == 0)
                                check = -1;
                            else
                                check = 1;
                            ++LA;
                            continue;
                        case SyntaxKind::AsteriskToken:
                            ++LA;
                            continue;
                        case SyntaxKind::SemicolonToken:
                        case SyntaxKind::EndOfFile:
                            return IdentifierRole::Declarator;
                        default:
                            ++check;
                            ++LA;
                            continue;
                    }
                    break;
                }
                return check == -1
                        ? IdentifierRole::TypedefName
                        : IdentifierRole::Declarator;
            }

            case SyntaxKind::CloseBracketToken:
                return peek(LA + 1).kind() == SyntaxKind::OpenBraceToken
                        ? IdentifierRole::Declarator
                        : isWithinKandRFuncDef_
                            ? IdentifierRole::Declarator
                            : IdentifierRole::TypedefName;

            case SyntaxKind::CommaToken:
                return isWithinKandRFuncDef_
                        ? IdentifierRole::Declarator
                        : IdentifierRole::TypedefName;

            default:
                return IdentifierRole::Declarator;
        }
    }
}

bool Parser::parseStructDeclaration_AtDeclarator(
        DeclarationSyntax*& decl,
        const SpecifierListSyntax* specList)
{
    DeclaratorListSyntax* decltorList = nullptr;
    DeclaratorListSyntax** decltorList_cur = &decltorList;

    if (peek().kind() == SyntaxKind::SemicolonToken)
        goto FieldsParsed;

    while (true) {
        DeclaratorSyntax* decltor = nullptr;
        if (!parseDeclarator(decltor, DeclarationContext::StructOrUnion))
            return false;

        *decltorList_cur = makeNode<DeclaratorListSyntax>(decltor);

        switch (peek().kind()) {
            case SyntaxKind::CommaToken:
                (*decltorList_cur)->delimTkIdx_ = consume();
                break;

            case SyntaxKind::SemicolonToken:
                goto FieldsParsed;

            default:
                diagReporter_.ExpectedFollowOfStructDeclarator();
                return false;
        }

        decltorList_cur = &(*decltorList_cur)->next;
    }

FieldsParsed:
    auto membDecl = makeNode<FieldDeclarationSyntax>();
    decl = membDecl;
    membDecl->semicolonTkIdx_ = consume();
    membDecl->specs_ = const_cast<SpecifierListSyntax*>(specList);
    membDecl->decltors_ = decltorList;
    return true;
}

/**
 * Parse a \a struct-declaration.
 *
 \verbatim
 struct-declaration:
     specifier-qualifier-list struct-declarator-list_opt ;
     static_assert-declaration
 \endverbatim
 *
 * \remark 6.7.2.1
 */
bool Parser::parseStructDeclaration(DeclarationSyntax*& decl)
{
    DBG_THIS_RULE();

    auto extKwTkIdx = peek().kind() == SyntaxKind::Keyword_ExtGNU___extension__
            ? consume()
            : LexedTokens::invalidIndex();

    switch (peek().kind()) {
        case SyntaxKind::Keyword__Static_assert:
            return parseStaticAssertDeclaration_AtFirst(decl);

        default: {
            SpecifierListSyntax* specList = nullptr;
            if (!parseSpecifierQualifierList(decl, specList)) {
                skipTo(SyntaxKind::SemicolonToken);
                return false;
            }
            if (!parseDeclarationOrStructDeclaration_AtFollowOfSpecifiers(
                        decl,
                        specList,
                        &Parser::parseStructDeclaration_AtDeclarator)) {
                return false;
            }
            if (extKwTkIdx != LexedTokens::invalidIndex()) {
                PSY_ASSERT_2(decl, return false);
                decl->extKwTkIdx_ = extKwTkIdx;
            }
            return true;
        }
    }
}

/**
 * Parse an \a enumerator.
 *
 \verbatim
 enumerator:
     enumeration-constant
     enumeration-constant = constant-expression
 \endverbatim
 *
 * \remark 6.7.2.2
 */
bool Parser::parseEnumerator(DeclarationSyntax*& decl)
{
    DBG_THIS_RULE();

    EnumeratorDeclarationSyntax* enumMembDecl = nullptr;

    switch (peek().kind()) {
        case SyntaxKind::IdentifierToken: {
            enumMembDecl = makeNode<EnumeratorDeclarationSyntax>();
            decl = enumMembDecl;
            enumMembDecl->identTkIdx_ = consume();
            break;
        }

        default:
            diagReporter_.ExpectedFIRSTofEnumerationConstant();
            return false;
    }

    if (peek().kind() == SyntaxKind::Keyword_ExtGNU___attribute__)
        parseExtGNU_AttributeSpecifierList_AtFirst(enumMembDecl->attrs_);

    switch (peek().kind()) {
        case SyntaxKind::EqualsToken:
            enumMembDecl->equalsTkIdx_ = consume();
            if (!parseExpressionWithPrecedenceConditional(enumMembDecl->expr_))
                return false;
            if (peek().kind() != SyntaxKind::CommaToken)
                break;
            [[fallthrough]];

        case SyntaxKind::CommaToken:
            enumMembDecl->commaTkIdx_ = consume();
            [[fallthrough]];

        default:
            break;
    }

    return true;
}

/**
 * Parse a \a parameter-type-list; or, informally, a "parameter-declaration-
 * list-and-or-ellipsis".
 *
 \verbatim
 parameter-type-list:
     parameter-list
     parameter-list , ...
 \endverbatim
 *
 * \remark 6.7.6
 */
bool Parser::parseParameterDeclarationListAndOrEllipsis(ParameterSuffixSyntax*& paramDecltorSfx)
{
    DBG_THIS_RULE();

    if (tree_->parseOptions().languageExtensions().isEnabled_extC_KandRStyle()) {
        auto LA = 1;
        while (true) {
            switch (peek(LA).kind()) {
                case SyntaxKind::CloseParenToken:
                    ++LA;
                    switch (peek(LA).kind()) {
                        case SyntaxKind::Keyword_const:
                        case SyntaxKind::Keyword_volatile:
                        case SyntaxKind::Keyword_restrict:
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
                        case SyntaxKind::Keyword_ExtGNU___complex__:
                        case SyntaxKind::Keyword_struct:
                        case SyntaxKind::Keyword_union:
                        case SyntaxKind::Keyword_enum:
                        case SyntaxKind::IdentifierToken:
                            isWithinKandRFuncDef_ = true;
                            break;
                        default:
                            break;
                    }
                    break;
                case SyntaxKind::EndOfFile:
                    break;
                default:
                    ++LA;
                    continue;
            }
            break;
        }
    }

    switch (peek().kind()) {
        case SyntaxKind::CloseParenToken:
            break;

        case SyntaxKind::EllipsisToken:
            if (!paramDecltorSfx->decls_)
                diagReporter_.ExpectedNamedParameterBeforeEllipsis();
            paramDecltorSfx->ellipsisTkIdx_ = consume();
            break;

        default:
            if (!parseParameterDeclarationList(paramDecltorSfx->decls_)) {
                isWithinKandRFuncDef_ = false;
                return false;
            }

            switch (peek().kind()) {
                case SyntaxKind::CommaToken:
                    paramDecltorSfx->decls_->delimTkIdx_ = consume();
                    match(SyntaxKind::EllipsisToken, &paramDecltorSfx->ellipsisTkIdx_);
                    break;

                case SyntaxKind::EllipsisToken:
                    paramDecltorSfx->ellipsisTkIdx_ = consume();
                    [[fallthrough]];

                default:
                    break;
            }
            break;
    }

    isWithinKandRFuncDef_ = false;

    return true;
}

/**
 * Parse a \a parameter-list; or, informally, a "parameter-declaration-
 * list".
 *
 \verbatim
 parameter-list:
     parameter-declaration
     parameter-list , parameter-declaration
 \endverbatim
 *
 * \remark 6.7.6
 */
bool Parser::parseParameterDeclarationList(ParameterDeclarationListSyntax*& paramList)
{
    DBG_THIS_RULE();

    ParameterDeclarationListSyntax** paramList_cur = &paramList;

    ParameterDeclarationSyntax* paramDecl = nullptr;
    if (!parseParameterDeclaration(paramDecl))
        return false;

    *paramList_cur = makeNode<ParameterDeclarationListSyntax>(paramDecl);

    while (peek().kind() == SyntaxKind::CommaToken) {
        (*paramList_cur)->delimTkIdx_ = consume();
        paramList_cur = &(*paramList_cur)->next;

        switch (peek().kind()) {
            case SyntaxKind::EllipsisToken:
                return true;

            default:
                *paramList_cur = makeNode<ParameterDeclarationListSyntax>();
                if (!parseParameterDeclaration((*paramList_cur)->value))
                    return false;
                break;
        }
    }

    return true;
}

/**
 * Parse a \a parameter-declaration.
 *
 \verbatim
 parameter-declaration:
     declaration-specifiers declarator
     declaration-specifiers abstract-decltor_opt
 \endverbatim
 *
 * \remark 6.7.6
 */
bool Parser::parseParameterDeclaration(ParameterDeclarationSyntax*& paramDecl)
{
    DBG_THIS_RULE();

    DeclarationSyntax* decl = nullptr;
    SpecifierListSyntax* specList = nullptr;
    if (!parseDeclarationSpecifiers(decl, specList, DeclarationContext::Parameter))
        return false;

    if (!specList) {
        switch (peek().kind()) {
            case SyntaxKind::IdentifierToken:
                break;

            default:
                diagReporter_.ExpectedFIRSTofParameterDeclaration();
                return false;
        }
    }

    paramDecl = makeNode<ParameterDeclarationSyntax>();
    paramDecl->specs_ = specList;

    Backtracker BT(this);
    if (!parseDeclarator(paramDecl->decltor_, DeclarationContext::Parameter)) {
        BT.backtrack();
        return parseAbstractDeclarator(paramDecl->decltor_);
    }
    BT.discard();

    return true;
}

bool Parser::parseExtPSY_TemplateDeclaration_AtFirst(DeclarationSyntax*& decl)
{
    DBG_THIS_RULE();
    PSY_ASSERT_3(peek().kind() == SyntaxKind::Keyword_ExtPSY__Template,
                  return false,
                  "assert failure: `_Template'");

    auto tmplDecl = makeNode<ExtPSY_TemplateDeclarationSyntax>();
    decl = tmplDecl;
    tmplDecl->templateTkIdx_ = consume();

    return parseDeclarationOrFunctionDefinition(tmplDecl->decl_);
}

bool Parser::parseExtKR_ParameterDeclarationList(
    ExtKR_ParameterDeclarationListSyntax *&paramList)
{
    DBG_THIS_RULE();

    ExtKR_ParameterDeclarationListSyntax** paramList_cur = &paramList;

    ExtKR_ParameterDeclarationSyntax* paramDecl = nullptr;
    if (!parseExtKR_ParameterDeclaration(paramDecl))
        return false;

    *paramList_cur = makeNode<ExtKR_ParameterDeclarationListSyntax>(paramDecl);

    while (peek().kind() != SyntaxKind::OpenBraceToken) {
        paramList_cur = &(*paramList_cur)->next;

        ExtKR_ParameterDeclarationSyntax* nextParamDecl = nullptr;
        if (!parseExtKR_ParameterDeclaration(nextParamDecl))
            return false;

        *paramList_cur = makeNode<ExtKR_ParameterDeclarationListSyntax>(nextParamDecl);
    }

    return true;
}

bool Parser::parseExtKR_ParameterDeclaration(ExtKR_ParameterDeclarationSyntax*& paramDecl)
{
    DBG_THIS_RULE();

    DeclarationSyntax* decl = nullptr;
    SpecifierListSyntax* specList = nullptr;
    if (!parseDeclarationSpecifiers(decl, specList, DeclarationContext::Unspecified))
        return false;

    paramDecl = makeNode<ExtKR_ParameterDeclarationSyntax>();
    paramDecl->specs_ = specList;

    DeclaratorListSyntax** decltorList_cur = &paramDecl->decltors_;
    while (true) {
        DeclaratorSyntax* decltor = nullptr;
        if (!parseDeclarator(decltor, DeclarationContext::Parameter))
            return false;

        *decltorList_cur = makeNode<DeclaratorListSyntax>(decltor);
        switch (peek().kind()) {
            case SyntaxKind::CommaToken:
                (*decltorList_cur)->delimTkIdx_ = consume();
                decltorList_cur = &(*decltorList_cur)->next;
                break;

            case SyntaxKind::SemicolonToken:
                paramDecl->semicolonTkIdx_ = consume();
                return true;

            default:
                return false;
        }
    }
}

/* Specifiers */
/**
 * Parse a \a declaration-specifiers.
 *
 \verbatim
 declaration-specifiers:
     storage-class-specifier declaration-specifiers_opt
     type-specifier declaration-specifiers_opt
     type-qualifier declaration-specifiers_opt
     function-specifier declaration-specifiers_opt
     alignment-specifier declaration-specifiers_opt
 \endverbatim
 *
 * \remark 6.7.1, 6.7.2, 6.7.3, 6.7.4, and 6.7.5
 */
bool Parser::parseDeclarationSpecifiers(DeclarationSyntax*& decl,
                                        SpecifierListSyntax*& specList,
                                        DeclarationContext declCtx)
{
    DBG_THIS_RULE();

    SpecifierListSyntax** specList_cur = &specList;
    bool seenType = false;
    while (true) {
        SpecifierSyntax* spec = nullptr;
        switch (peek().kind()) {
            // declaration-specifiers -> storage-class-specifier
            case SyntaxKind::Keyword_typedef:
                parseTrivialSpecifier_AtFirst<StorageClassSyntax>(
                            spec,
                            SyntaxKind::TypedefStorageClass);
                break;

            case SyntaxKind::Keyword_extern:
                parseTrivialSpecifier_AtFirst<StorageClassSyntax>(
                            spec,
                            SyntaxKind::ExternStorageClass);
                break;

            case SyntaxKind::Keyword_static:
                parseTrivialSpecifier_AtFirst<StorageClassSyntax>(
                            spec,
                            SyntaxKind::StaticStorageClass);
                break;

            case SyntaxKind::Keyword_auto:
                parseTrivialSpecifier_AtFirst<StorageClassSyntax>(
                            spec,
                            SyntaxKind::AutoStorageClass);
                break;

            case SyntaxKind::Keyword_register:
                parseTrivialSpecifier_AtFirst<StorageClassSyntax>(
                            spec,
                            SyntaxKind::RegisterStorageClass);
                break;

            case SyntaxKind::Keyword__Thread_local:
            case SyntaxKind::Keyword_ExtGNU___thread:
                parseTrivialSpecifier_AtFirst<StorageClassSyntax>(
                            spec,
                            SyntaxKind::ThreadLocalStorageClass);
                break;

            // declaration-specifiers -> type-qualifier
            case SyntaxKind::Keyword_const:
                parseTrivialSpecifier_AtFirst<TypeQualifierSyntax>(
                            spec,
                            SyntaxKind::ConstQualifier);
                break;

            case SyntaxKind::Keyword_volatile:
                parseTrivialSpecifier_AtFirst<TypeQualifierSyntax>(
                            spec,
                            SyntaxKind::VolatileQualifier);
                break;

            case SyntaxKind::Keyword_restrict:
                parseTrivialSpecifier_AtFirst<TypeQualifierSyntax>(
                            spec,
                            SyntaxKind::RestrictQualifier);
                break;

            // declaration-specifiers -> type-qualifier -> `_Atomic'
            // declaration-specifiers -> type-specifier -> `_Atomic' `('
            case SyntaxKind::Keyword__Atomic:
                if (peek(2).kind() == SyntaxKind::OpenParenToken) {
                    if (!parseAtomicTypeSpecifier_AtFirst(spec))
                        return false;
                    seenType = true;
                }
                else
                    parseTrivialSpecifier_AtFirst<TypeQualifierSyntax>(
                                spec,
                                SyntaxKind::AtomicQualifier);
                break;

            // declaration-specifiers -> function-specifier
            case SyntaxKind::Keyword_inline:
                parseTrivialSpecifier_AtFirst<FunctionSpecifierSyntax>(
                            spec,
                            SyntaxKind::InlineSpecifier);
                break;

            case SyntaxKind::Keyword__Noreturn:
                parseTrivialSpecifier_AtFirst<FunctionSpecifierSyntax>(
                            spec,
                            SyntaxKind::NoReturnSpecifier);
                break;

            // declaration-specifiers -> type-specifier -> void
            case SyntaxKind::Keyword_void:
                seenType = true;
                parseTrivialSpecifier_AtFirst<VoidTypeSpecifierSyntax>(spec);
                break;

            // declaration-specifiers -> type-specifier -> basic types
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
            case SyntaxKind::Keyword_ExtGNU___complex__:
                seenType = true;
                parseTrivialSpecifier_AtFirst<BasicTypeSpecifierSyntax>(
                            spec,
                            SyntaxKind::BasicTypeSpecifier);
                break;

            // declaration-specifiers -> type-specifier ->* `struct'
            case SyntaxKind::Keyword_struct:
                seenType = true;
                if (!parseTagTypeSpecifier_AtFirst<StructOrUnionDeclarationSyntax>(
                            decl,
                            spec,
                            SyntaxKind::StructDeclaration,
                            SyntaxKind::StructTypeSpecifier,
                            &Parser::parseStructDeclaration))
                    return false;
                break;

            // declaration-specifiers -> type-specifier ->* `union'
            case SyntaxKind::Keyword_union:
                seenType = true;
                if (!parseTagTypeSpecifier_AtFirst<StructOrUnionDeclarationSyntax>(
                            decl,
                            spec,
                            SyntaxKind::UnionDeclaration,
                            SyntaxKind::UnionTypeSpecifier,
                            &Parser::parseStructDeclaration))
                    return  false;
                break;

            // declaration-specifiers -> type-specifier -> enum-specifier
            case SyntaxKind::Keyword_enum:
                seenType = true;
                if (!parseTagTypeSpecifier_AtFirst<EnumDeclarationSyntax>(
                            decl,
                            spec,
                            SyntaxKind::EnumDeclaration,
                            SyntaxKind::EnumTypeSpecifier,
                            &Parser::parseEnumerator))
                    return false;
                break;

            // declaration-specifiers -> type-specifier -> typedef-name
            case SyntaxKind::IdentifierToken: {
                if (seenType)
                    return true;
                if (guessRoleOfIdentifier(declCtx) == IdentifierRole::Declarator)
                    return true;
                seenType = true;
                parseTypedefName_AtFirst(spec);
                break;
            }

            // declaration-specifiers -> alignment-specifier
            case SyntaxKind::Keyword__Alignas:
                if (!parseAlignmentSpecifier_AtFirst(spec))
                    return false;
                break;

            // declaration-specifiers -> GNU-attribute-specifier
            case SyntaxKind::Keyword_ExtGNU___attribute__:
                if (!parseExtGNU_AttributeSpecifier_AtFirst(spec))
                    return false;
                break;

            // declaration-specifiers -> GNU-typeof-specifier
            case SyntaxKind::Keyword_ExtGNU___typeof__:
                if (!parseExtGNU_Typeof_AtFirst(spec))
                    return false;
                break;

            // declaration-specifiers -> PsycheC
            case SyntaxKind::Keyword_ExtPSY__Forall:
            case SyntaxKind::Keyword_ExtPSY__Exists:
                if (!parseExtPSY_QuantifiedTypeSpecifier_AtFirst(spec))
                    return false;
                break;

            default:
                return true;
        }

        *specList_cur = makeNode<SpecifierListSyntax>(spec);
        specList_cur = &(*specList_cur)->next;

        if (decl)
            return parseTypeQualifiersAndAttributes(*specList_cur);
    }
}

/**
 * Parse a \a specifier-qualifier-list.
 *
 \verbatim
 specifier-qualifier-list:
     type-specifier specifier-qualifier-list_opt
     type-qualifier specifier-qualifier-list_opt
 \endverbatim
 *
 * \remark 6.7.2.1
 */
bool Parser::parseSpecifierQualifierList(DeclarationSyntax*& decl,
                                         SpecifierListSyntax*& specList)
{
    DBG_THIS_RULE();

    SpecifierListSyntax** specList_cur = &specList;
    bool seenType = false;
    while (true) {
        SpecifierSyntax* spec = nullptr;
        switch (peek().kind()) {
            // declaration-specifiers -> type-qualifier
            case SyntaxKind::Keyword_const:
                parseTrivialSpecifier_AtFirst<TypeQualifierSyntax>(
                            spec,
                            SyntaxKind::ConstQualifier);
                break;

            case SyntaxKind::Keyword_volatile:
                parseTrivialSpecifier_AtFirst<TypeQualifierSyntax>(
                            spec,
                            SyntaxKind::VolatileQualifier);
                break;

            case SyntaxKind::Keyword_restrict:
                parseTrivialSpecifier_AtFirst<TypeQualifierSyntax>(
                            spec,
                            SyntaxKind::RestrictQualifier);
                break;

            // declaration-specifiers -> type-qualifier -> `_Atomic'
            // declaration-specifiers -> type-specifier -> `_Atomic' `('
            case SyntaxKind::Keyword__Atomic:
                if (peek(2).kind() == SyntaxKind::OpenParenToken) {
                    if (!parseAtomicTypeSpecifier_AtFirst(spec))
                        return false;
                    seenType = true;
                }
                else
                    parseTrivialSpecifier_AtFirst<TypeQualifierSyntax>(
                                spec,
                                SyntaxKind::AtomicQualifier);
                break;

            // declaration-specifiers -> type-specifier -> void
            case SyntaxKind::Keyword_void:
                seenType = true;
                parseTrivialSpecifier_AtFirst<VoidTypeSpecifierSyntax>(spec);
                break;

            // declaration-specifiers -> type-specifier -> basic types
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
            case SyntaxKind::Keyword_ExtGNU___complex__:
                seenType = true;
                parseTrivialSpecifier_AtFirst<BasicTypeSpecifierSyntax>(
                            spec,
                            SyntaxKind::BasicTypeSpecifier);
                break;

            // declaration-specifiers -> type-specifier ->* `struct'
            case SyntaxKind::Keyword_struct:
                seenType = true;
                if (!parseTagTypeSpecifier_AtFirst<StructOrUnionDeclarationSyntax>(
                            decl,
                            spec,
                            SyntaxKind::StructDeclaration,
                            SyntaxKind::StructTypeSpecifier,
                            &Parser::parseStructDeclaration))
                    return false;
                break;

            // declaration-specifiers -> type-specifier ->* `union'
            case SyntaxKind::Keyword_union:
                seenType = true;
                if (!parseTagTypeSpecifier_AtFirst<StructOrUnionDeclarationSyntax>(
                            decl,
                            spec,
                            SyntaxKind::UnionDeclaration,
                            SyntaxKind::UnionTypeSpecifier,
                            &Parser::parseStructDeclaration))
                    return false;
                break;

            // declaration-specifiers -> type-specifier -> enum-specifier
            case SyntaxKind::Keyword_enum:
                seenType = true;
                if (!parseTagTypeSpecifier_AtFirst<EnumDeclarationSyntax>(
                            decl,
                            spec,
                            SyntaxKind::EnumDeclaration,
                            SyntaxKind::EnumTypeSpecifier,
                            &Parser::parseEnumerator))
                    return false;
                break;

            // declaration-specifiers -> type-specifier -> typedef-name
            case SyntaxKind::IdentifierToken: {
                if (seenType)
                    return true;
                seenType = true;
                parseTypedefName_AtFirst(spec);
                break;
            }

            // declaration-specifiers -> alignment-specifier
            case SyntaxKind::Keyword__Alignas:
                if (!parseAlignmentSpecifier_AtFirst(spec))
                    return false;
                break;

            // declaration-specifiers -> GNU-attribute-specifier
            case SyntaxKind::Keyword_ExtGNU___attribute__:
                if (!parseExtGNU_AttributeSpecifier_AtFirst(spec))
                    return false;
                break;

            // declaration-specifiers -> GNU-typeof-specifier
            case SyntaxKind::Keyword_ExtGNU___typeof__:
                if (!parseExtGNU_Typeof_AtFirst(spec))
                    return false;
                break;

            default:
                if (specList_cur == &specList) {
                    diagReporter_.ExpectedFIRSTofSpecifierQualifier();
                    return false;
                }
                return true;
        }

        *specList_cur = makeNode<SpecifierListSyntax>(spec);
        specList_cur = &(*specList_cur)->next;

        if (decl)
            return parseTypeQualifiersAndAttributes(*specList_cur);
    }
}

/**
 * Parse a "trivial" specifier, which is one of:
 *
 *  a \a storage-class-specifier,
 *  a \a basic type-specifer,
 *  a \a type-qualifier,
 *  a \a function-specifier, or
 *  a \a GNU ext asm-qualifier.
 *
 * \remark 6.7.1, 6.7.2, 6.7.3, and 6.7.4
 */
template <class SpecT>
void Parser::parseTrivialSpecifier_AtFirst(SpecifierSyntax*& spec, SyntaxKind specK)
{
    DBG_THIS_RULE();
    PSY_ASSERT_3(SyntaxFacts::isStorageClassSyntax(peek())
                        || SyntaxFacts::isBasicTypeSpecifierSyntax(peek())
                        || SyntaxFacts::isTypeQualifierSyntax(peek())
                        || SyntaxFacts::isFunctionSpecifierSyntax(peek())
                        || SyntaxFacts::isExtGNU_AsmQualifierSyntax(peek()),
                  return,
                  "assert failure: <storage-class-specifier>, "
                                  "(builtin) <type-specifier>, "
                                  "<function-specifier>, "
                                  "<type-qualifier>, or"
                                  "<GNU-ext-asm-qualifier>");

    auto trivSpec = makeNode<SpecT>(specK);
    spec = trivSpec;
    trivSpec->specTkIdx_ = consume();
}

template <class SpecT>
void Parser::parseTrivialSpecifier_AtFirst(SpecifierSyntax*& spec)
{
    DBG_THIS_RULE();
    PSY_ASSERT_3(peek().kind() == SyntaxKind::Keyword_void,
                     return,
                    "assert failure: void type-specifier");

    auto trivSpec = makeNode<VoidTypeSpecifierSyntax>();
    spec = trivSpec;
    trivSpec->specTkIdx_ = consume();
}

template void Parser::parseTrivialSpecifier_AtFirst<ExtGNU_AsmQualifierSyntax>
(SpecifierSyntax*& spec, SyntaxKind specK);

/**
 * Parse an \a alignment-specifier.
 *
 \verbatim
 alignment-specifier:
     _Alignas ( type-name )
     _Alignas ( constant-expression )
 \endverbatim
 *
 * \remark 6.7.5
 */
bool Parser::parseAlignmentSpecifier_AtFirst(SpecifierSyntax*& spec)
{
    DBG_THIS_RULE();
    PSY_ASSERT_3(peek().kind() == SyntaxKind::Keyword__Alignas,
                  return false,
                  "assert failure: `_Alignas'");

    auto alignSpec = makeNode<AlignmentSpecifierSyntax>();
    spec = alignSpec;
    alignSpec->alignasKwTkIdx_ = consume();
    return parseParenthesizedTypeNameOrExpression(alignSpec->tyRef_);
}

/**
 * Parse a GNU extension \c typeof \a specifier.
 */
bool Parser::parseExtGNU_Typeof_AtFirst(SpecifierSyntax*& spec)
{
    DBG_THIS_RULE();
    PSY_ASSERT_3(peek().kind() == SyntaxKind::Keyword_ExtGNU___typeof__,
                  return false,
                  "assert failure: `typeof'");

    auto typeofSpec = makeNode<ExtGNU_TypeofSyntax>();
    spec = typeofSpec;
    typeofSpec->typeofKwTkIdx_ = consume();
    return parseParenthesizedTypeNameOrExpression(typeofSpec->tyRef_);
}

/**
 * Parse a \a typedef-name specifier.
 *
 * \remark 6.7.8
 */
void Parser::parseTypedefName_AtFirst(SpecifierSyntax*& spec)
{
    DBG_THIS_RULE();
    PSY_ASSERT_3(peek().kind() == SyntaxKind::IdentifierToken,
                  return,
                  "assert failure: <identifier>");

    auto tydefName = makeNode<TypedefNameSyntax>();
    spec = tydefName;
    tydefName->identTkIdx_ = consume();
}

/**
 * Parse an \a atomic-type-specifier.
 *
 * \remark 6.7.2.4
 */
bool Parser::parseAtomicTypeSpecifier_AtFirst(SpecifierSyntax*& spec)
{
    DBG_THIS_RULE();
    PSY_ASSERT_3(peek().kind() == SyntaxKind::Keyword__Atomic,
                  return false,
                  "assert failure: `_Atomic'");

    auto atomTySpec = makeNode<AtomicTypeSpecifierSyntax>();
    spec = atomTySpec;
    atomTySpec->atomicKwTkIdx_ = consume();

    return match(SyntaxKind::OpenParenToken, &atomTySpec->openParenTkIdx_)
        && parseTypeName(atomTySpec->typeName_)
        && matchOrSkipTo(SyntaxKind::CloseParenToken, &atomTySpec->closeParenTkIdx_);
}

/**
 * Parse a \a struct-or-union-specifier or a \a enum-specifier. The parsing
 * of the declaration of the members of either the \c struct (and \c union)
 * or the \c enum is specified through a parameter of function type.
 *
 \verbatim
 struct-or-union-specifier:
     struct-or-union identifier_opt { struct-declaration-list }
     struct-or-union identifier

 struct-declaration-list:
     struct-declaration

  struct-declaration:
     struct-declaration-list struct-declaration

 enum-specifier:
     enum identifier_opt { enumerator-list }
     enum identifier_opt { enumerator-list , }
     enum identifier
 \endverbatim
 *
 * \remark 6.7.2.1
 * \remark 6.7.2.3-6, 6.7.2.3-8, and 6.7.2.3-8.
 */
template <class TypeDeclT>
bool Parser::parseTagTypeSpecifier_AtFirst(
        DeclarationSyntax*& decl,
        SpecifierSyntax*& spec,
        SyntaxKind declK,
        SyntaxKind specK,
        bool (Parser::*parseMember)(DeclarationSyntax*&))
{
    DBG_THIS_RULE();
    PSY_ASSERT_3((peek().kind() == SyntaxKind::Keyword_struct
                        || peek().kind() == SyntaxKind::Keyword_union
                        || peek().kind() == SyntaxKind::Keyword_enum)
                    && (declK == SyntaxKind::StructDeclaration
                        || declK == SyntaxKind::UnionDeclaration
                        || declK == SyntaxKind::EnumDeclaration)
                    && (specK == SyntaxKind::StructTypeSpecifier
                        || specK == SyntaxKind::UnionTypeSpecifier
                        || specK == SyntaxKind::EnumTypeSpecifier),
                  return false,
                  "assert failure: `struct', `union', or `enum'");

    auto tySpec = makeNode<TagTypeSpecifierSyntax>(specK);
    spec = tySpec;
    tySpec->kwTkIdx_ = consume();

    if (peek().kind() == SyntaxKind::Keyword_ExtGNU___attribute__)
        parseExtGNU_AttributeSpecifierList_AtFirst(tySpec->attrs1_);

    auto wrapTySpecInTyDecl = [&]() {
        auto tyDecl = makeNode<TypeDeclT>(declK);
        decl = tyDecl;
        tyDecl->typeSpec_ = tySpec;
    };

    switch (peek().kind()) {
        case SyntaxKind::OpenBraceToken:
            tySpec->openBraceTkIdx_ = consume();
            break;

        case SyntaxKind::IdentifierToken:
            tySpec->tagTkIdx_ = consume();
            switch (peek().kind()) {
                case SyntaxKind::OpenBraceToken:
                    tySpec->openBraceTkIdx_ = consume();
                    break;

                case SyntaxKind::SemicolonToken:
                    if (specK == SyntaxKind::StructTypeSpecifier
                            || specK == SyntaxKind::UnionTypeSpecifier) {
                        wrapTySpecInTyDecl();
                        return true;
                    }
                    [[fallthrough]];

                default:
                    return true;
            }
            break;

        default:
            diagReporter_.ExpectedFollowOfStructOrUnionOrEnum();
            return false;
    }

    wrapTySpecInTyDecl();

    DeclarationListSyntax** declList_cur = &tySpec->decls_;

    while (true) {
        DeclarationSyntax* membDecl = nullptr;
        switch (peek().kind()) {
            case SyntaxKind::CloseBraceToken:
                tySpec->closeBraceTkIdx_ = consume();
                goto MembersParsed;

            default:
                if (!((this)->*(parseMember))(membDecl)) {
                    ignoreMemberDeclaration();
                    if (peek().kind() == SyntaxKind::EndOfFile)
                        return false;
                }
                break;
        }
        *declList_cur = makeNode<DeclarationListSyntax>(membDecl);
        declList_cur = &(*declList_cur)->next;
    }

MembersParsed:
    if (peek().kind() == SyntaxKind::Keyword_ExtGNU___attribute__)
        parseExtGNU_AttributeSpecifierList_AtFirst(tySpec->attrs2_);

    return true;
}

/**
 * Parse a GNU extension \a attribute-specifier list.
 */
bool Parser::parseExtGNU_AttributeSpecifierList_AtFirst(SpecifierListSyntax*& specList)
{
    DBG_THIS_RULE();
    PSY_ASSERT_3(peek().kind() == SyntaxKind::Keyword_ExtGNU___attribute__,
                  return false,
                  "assert failure: `__attribute__'");

    SpecifierListSyntax** specList_cur = &specList;

    do {
        SpecifierSyntax* spec = nullptr;
        if (!parseExtGNU_AttributeSpecifier_AtFirst(spec))
            return false;

        *specList_cur = makeNode<SpecifierListSyntax>(spec);
        specList_cur = &(*specList_cur)->next;
    }
    while (peek().kind() == SyntaxKind::Keyword_ExtGNU___attribute__);

    return true;
}

/**
 * Parse an a GNU extension \a attribute-specifier.
 */
bool Parser::parseExtGNU_AttributeSpecifier_AtFirst(SpecifierSyntax*& spec)
{
    DBG_THIS_RULE();
    PSY_ASSERT_3(peek().kind() == SyntaxKind::Keyword_ExtGNU___attribute__,
                  return false,
                  "assert failure: `__attribute__'");

    auto attrSpec = makeNode<ExtGNU_AttributeSpecifierSyntax>();
    spec = attrSpec;
    attrSpec->attrKwTkIdx_ = consume();

    if (match(SyntaxKind::OpenParenToken, &attrSpec->openOuterParenTkIdx_)
            && match(SyntaxKind::OpenParenToken, &attrSpec->openInnerParenTkIdx_)
            && parseExtGNU_AttributeList(attrSpec->attrs_)
            && match(SyntaxKind::CloseParenToken, &attrSpec->closeInnerParenTkIdx_)
            && match(SyntaxKind::CloseParenToken, &attrSpec->closeOuterParenTkIdx_))
        return true;

    skipTo(SyntaxKind::CloseParenToken);
    return false;
}

/**
 * Parse an \a attribute-list of GNU extension \a attribute-specifier.
 */
bool Parser::parseExtGNU_AttributeList(ExtGNU_AttributeListSyntax*& attrList)
{
    DBG_THIS_RULE();

    ExtGNU_AttributeListSyntax** attrList_cur = &attrList;

    while (true) {
        ExtGNU_AttributeSyntax* attr = nullptr;
        if (!parseExtGNU_Attribute(attr))
            return false;

        *attrList_cur = makeNode<ExtGNU_AttributeListSyntax>(attr);

        switch (peek().kind()) {
            case SyntaxKind::CommaToken:
                (*attrList_cur)->delimTkIdx_ = consume();
                attrList_cur = &(*attrList_cur)->next;
                break;

            case SyntaxKind::CloseParenToken:
                return true;

            default:
                diagReporter_.ExpectedTokenWithin(
                    { SyntaxKind::CommaToken,
                      SyntaxKind::CloseParenToken });
                return false;
        }
    }
    return true;
}

/**
 * Parse a GNU extension \a attribute.
 */
bool Parser::parseExtGNU_Attribute(ExtGNU_AttributeSyntax*& attr)
{
    DBG_THIS_RULE();

    switch (peek().kind()) {
        case SyntaxKind::IdentifierToken:
        case SyntaxKind::Keyword_const:
            attr = makeNode<ExtGNU_AttributeSyntax>();
            attr->kwOrIdentTkIdx_ = consume();
            break;

        case SyntaxKind::CommaToken:
        case SyntaxKind::CloseParenToken:
            // An empty attribute is valid.
            attr = makeNode<ExtGNU_AttributeSyntax>();
            return true;

        default:
            diagReporter_.ExpectedTokenWithin(
                { SyntaxKind::IdentifierToken,
                  SyntaxKind::Keyword_const,
                  SyntaxKind::CommaToken,
                  SyntaxKind::CloseParenToken });
            return false;
    }

    if (peek().kind() != SyntaxKind::OpenParenToken)
        return true;

    attr->openParenTkIdx_ = consume();

    auto ident = tree_->tokenAt(attr->kwOrIdentTkIdx_).identifier_;
    bool (Parser::*parseAttrArg)(ExpressionListSyntax*&);
    if (ident && !strcmp(ident->c_str(), "availability"))
        parseAttrArg = &Parser::parseExtGNU_AttributeArgumentsLLVM;
    else
        parseAttrArg = &Parser::parseExtGNU_AttributeArguments;

    return ((this->*(parseAttrArg))(attr->exprs_))
        && matchOrSkipTo(SyntaxKind::CloseParenToken, &attr->closeParenTkIdx_);
}

/**
 * Parse the arguments of a GNU extension \a attribute.
 */
bool Parser::parseExtGNU_AttributeArguments(ExpressionListSyntax*& exprList)
{
    return parseCallArguments(exprList);
}

/**
 * Parse the arguments of a GNU extension \a attribute of LLVM.
 *
 * The default parsing for the arguments of an \a attribute is that of an
 * \a expression-list, but LLVM's \c availability argument requires
 * "special" handling: the clauses \c introduced, \c obsolete, etc.
 * contain \c version specifier, which may be a tuple of three separated
 * integers (which don't make up for floating point).
 *
 * \code
 * __attribute__((availability(macosx,introduced=10.12.1))) void f();
 * \endcode
 *
 * \see https://clang.llvm.org/docs/AttributeReference.html#availability
 */
bool Parser::parseExtGNU_AttributeArgumentsLLVM(ExpressionListSyntax*& exprList)
{
    DBG_THIS_RULE();

    if (!tree_->parseOptions().languageExtensions().isEnabled_extGNU_AttributeSpecifiersLLVM())
        diagReporter_.ExpectedFeature("GNU attributes of LLVM");

    ExpressionListSyntax** exprList_cur = &exprList;

    ExpressionSyntax* platName = nullptr;
    if (!parseIdentifierName(platName))
        return false;

    *exprList_cur = makeNode<ExpressionListSyntax>(platName);

    while (peek().kind() == SyntaxKind::CommaToken) {
        (*exprList_cur)->delimTkIdx_ = consume();
        exprList_cur = &(*exprList_cur)->next;

        ExpressionSyntax* expr = nullptr;
        if (!parseIdentifierName(expr))
            return false;

        switch (peek().kind()) {
            case SyntaxKind::EqualsToken: {
                auto equalsTkIdx_ = consume();
                ExpressionSyntax* versionExpr = nullptr;
                if (peek().kind() == SyntaxKind::StringLiteralToken)
                    parseStringLiteral_AtFirst(versionExpr);
                else {
                    if (!parseConstant<ConstantExpressionSyntax>(
                                versionExpr,
                                SyntaxKind::FloatingConstantExpression))
                        return false;

                    // Discard any (possible) "patch" component of a version.
                    if (peek().kind() == SyntaxKind::IntegerConstantToken)
                        consume();
                }

                auto assign = makeNode<BinaryExpressionSyntax>(SyntaxKind::BasicAssignmentExpression);
                assign->leftExpr_ = expr;
                assign->oprtrTkIdx_ = equalsTkIdx_;
                assign->rightExpr_ = versionExpr;
                expr = assign;
                break;
            }

            default:
                break;
        }
        *exprList_cur = makeNode<ExpressionListSyntax>(expr);
    }

    return true;
}

bool Parser::parseExtGNU_AsmLabel_AtFirst(SpecifierSyntax*& attr)
{
    DBG_THIS_RULE();
    PSY_ASSERT_3(peek().kind() == SyntaxKind::Keyword_ExtGNU___asm__,
                  return false,
                  "assert failure: `asm'");

    auto asmAttr = makeNode<ExtGNU_AsmLabelSyntax>();
    attr = asmAttr;
    asmAttr->asmKwTkIdx_ = consume();

    if (match(SyntaxKind::OpenParenToken, &asmAttr->openParenTkIdx_)
            && parseStringLiteral(asmAttr->strLit_)
            && match(SyntaxKind::CloseParenToken, &asmAttr->closeParenTkIdx_))
        return true;

    return false;
}

bool Parser::parseExtPSY_QuantifiedTypeSpecifier_AtFirst(SpecifierSyntax*& spec)
{
    DBG_THIS_RULE();
    PSY_ASSERT_3(peek().kind() == SyntaxKind::Keyword_ExtPSY__Exists
                    || peek().kind() == SyntaxKind::Keyword_ExtPSY__Forall,
                  return false,
                  "assert failure: `_Exists' or `_Forall'");

    auto typeSpec = makeNode<ExtPSY_QuantifiedTypeSpecifierSyntax>();
    spec = typeSpec;
    typeSpec->quantifierTkIdx_ = consume();

    if (match(SyntaxKind::OpenParenToken, &typeSpec->openParenTkIdx_)
            && match(SyntaxKind::IdentifierToken, &typeSpec->identTkIdx_)
            && match(SyntaxKind::CloseParenToken, &typeSpec->closeParenTkIdx_))
        return true;

    return false;
}

/* Declarators */

bool Parser::parseAbstractDeclarator(DeclaratorSyntax*& decltor)
{
    DBG_THIS_RULE();

    return parseDeclarator(decltor,
                           DeclarationContext::Parameter,
                           DeclaratorForm::Abstract);
}

bool Parser::parseDeclarator(DeclaratorSyntax*& decltor,
                             DeclarationContext declCtx)
{
    DBG_THIS_RULE();

    return parseDeclarator(decltor, declCtx, DeclaratorForm::Concrete);
}

bool Parser::parseDeclarator(DeclaratorSyntax*& decltor,
                             DeclarationContext declCtx,
                             DeclaratorForm decltorForm)

{
    DBG_THIS_RULE();

    SpecifierListSyntax* attrList = nullptr;
    if (peek().kind() == SyntaxKind::Keyword_ExtGNU___attribute__)
        parseExtGNU_AttributeSpecifierList_AtFirst(attrList);

    if (peek().kind() == SyntaxKind::AsteriskToken) {
        auto ptrDecltor = makeNode<PointerDeclaratorSyntax>();
        decltor = ptrDecltor;
        ptrDecltor->attrs_ = attrList;
        ptrDecltor->asteriskTkIdx_ = consume();
        if (!parseTypeQualifiersAndAttributes(ptrDecltor->qualsAndAttrs_))
            return false;
        return parseDeclarator(ptrDecltor->innerDecltor_, declCtx, decltorForm);
    }

    return parseDirectDeclarator(decltor, declCtx, decltorForm, attrList);
}

bool Parser::parseDirectDeclarator(DeclaratorSyntax*& decltor,
                                   DeclarationContext declCtx,
                                   DeclaratorForm decltorForm,
                                   SpecifierListSyntax* attrList)
{
    DBG_THIS_RULE();

    switch (peek().kind()) {
        case SyntaxKind::IdentifierToken: {
            if (decltorForm == DeclaratorForm::Abstract)
                return false;

            auto identDecltor = makeNode<IdentifierDeclaratorSyntax>();
            decltor = identDecltor;
            identDecltor->identTkIdx_ = consume();
            identDecltor->attrs1_ = attrList;
            if (!parseDirectDeclaratorSuffix(
                        decltor,
                        declCtx,
                        decltorForm,
                        attrList,
                        identDecltor))
                return false;

            SpecifierListSyntax** specList_cur = &identDecltor->attrs2_;

            switch (peek().kind()) {
                case SyntaxKind::Keyword_ExtGNU___asm__: {
                    SpecifierSyntax* spec = nullptr;
                    if (!parseExtGNU_AsmLabel_AtFirst(spec))
                        return false;

                    *specList_cur = makeNode<SpecifierListSyntax>(spec);
                    specList_cur = &(*specList_cur)->next;

                    if (peek().kind() != SyntaxKind::Keyword_ExtGNU___attribute__)
                        break;
                    [[fallthrough]];
                }

                case SyntaxKind::Keyword_ExtGNU___attribute__:
                    if (!parseExtGNU_AttributeSpecifierList_AtFirst(*specList_cur))
                        return false;
                    break;

                default:
                    break;
            }
            break;
        }

        case SyntaxKind::OpenParenToken: {
            if (decltorForm == DeclaratorForm::Abstract) {
                auto absDecltor = makeNode<AbstractDeclaratorSyntax>();
                decltor = absDecltor;
                absDecltor->attrs_ = attrList;
                if (peek(2).kind() == SyntaxKind::CloseParenToken) {
                    if (!parseDirectDeclaratorSuffix(
                                decltor,
                                declCtx,
                                decltorForm,
                                attrList,
                                absDecltor))
                        return false;
                    break;
                }
                else {
                    Backtracker BT(this);
                    auto openParenTkIdx = consume();
                    DeclaratorSyntax* innerDecltor = nullptr;
                    if (!parseAbstractDeclarator(innerDecltor)
                            || peek().kind() != SyntaxKind::CloseParenToken) {
                        BT.backtrack();
                        if (!parseDirectDeclaratorSuffix(
                                    decltor,
                                    declCtx,
                                    decltorForm,
                                    attrList,
                                    absDecltor))
                            return false;
                        break;
                    }
                    BT.discard();

                    auto parenDecltor = makeNode<ParenthesizedDeclaratorSyntax>();
                    decltor = parenDecltor;
                    parenDecltor->openParenTkIdx_ = openParenTkIdx;
                    parenDecltor->innerDecltor_ = innerDecltor;
                    parenDecltor->closeParenTkIdx_ = consume();
                    if (!parseDirectDeclaratorSuffix(
                                decltor,
                                declCtx,
                                decltorForm,
                                attrList,
                                parenDecltor))
                        return false;
                    break;
                }
            }

            auto parenDecltor = makeNode<ParenthesizedDeclaratorSyntax>();
            parenDecltor->openParenTkIdx_ = consume();
            if (!parseDeclarator(parenDecltor->innerDecltor_, declCtx, decltorForm)
                    || !match(SyntaxKind::CloseParenToken, &parenDecltor->closeParenTkIdx_)
                    || !parseDirectDeclaratorSuffix(decltor,
                                                    declCtx,
                                                    decltorForm,
                                                    attrList,
                                                    parenDecltor))
                return false;

            if (!decltor)
                decltor = parenDecltor;
            break;
        }

        case SyntaxKind::OpenBracketToken:
            if (decltorForm == DeclaratorForm::Abstract) {
                auto absDecltor = makeNode<AbstractDeclaratorSyntax>();
                decltor = absDecltor;
                absDecltor->attrs_ = attrList;
                if (!parseDirectDeclaratorSuffix(
                            decltor,
                            declCtx,
                            decltorForm,
                            attrList,
                            absDecltor))
                    return false;
                break;
            }
            diagReporter_.ExpectedFIRSTofDirectDeclarator();
            return false;

        case SyntaxKind::ColonToken:
            if (decltorForm == DeclaratorForm::Concrete
                    && declCtx == DeclarationContext::StructOrUnion) {
                auto bitFldDecltor = makeNode<BitfieldDeclaratorSyntax>();
                decltor = bitFldDecltor;
                bitFldDecltor->colonTkIdx_ = consume();
                if (!parseExpressionWithPrecedenceConditional(bitFldDecltor->expr_))
                    return false;
                break;
            }
            [[fallthrough]];

        default: {
            if (decltorForm == DeclaratorForm::Abstract) {
                auto absDecltor = makeNode<AbstractDeclaratorSyntax>();
                decltor = absDecltor;
                absDecltor->attrs_ = attrList;
                break;
            }
            diagReporter_.ExpectedFIRSTofDirectDeclarator();
            return false;
        }
    }

    if (peek().kind() == SyntaxKind::ColonToken
            && decltorForm == DeclaratorForm::Concrete
            && declCtx == DeclarationContext::StructOrUnion) {
        auto bitFldDecltor = makeNode<BitfieldDeclaratorSyntax>();
        bitFldDecltor->innerDecltor_ = decltor;
        decltor = bitFldDecltor;
        bitFldDecltor->colonTkIdx_ = consume();
        if (!parseExpressionWithPrecedenceConditional(bitFldDecltor->expr_))
            return false;

        if (peek().kind() == SyntaxKind::Keyword_ExtGNU___attribute__)
            parseExtGNU_AttributeSpecifierList_AtFirst(bitFldDecltor->attrs_);
    }

    return true;
}

/**
 * Parse a \a direct-declarator.
 *
 \verbatim
 direct-declarator:
    identifier
    ( declarator )
    direct-declarator [ ... ]
    direct-declarator ( ... )
 \endverbatim
 *
 * \remark 6.7.6
 */
bool Parser::parseDirectDeclaratorSuffix(DeclaratorSyntax*& decltor,
                                         DeclarationContext declCtx,
                                         DeclaratorForm decltorForm,
                                         SpecifierListSyntax* attrList,
                                         DeclaratorSyntax* innerDecltor)
{
    auto validateContext =
            [this, declCtx] (void (Parser::DiagnosticsReporter::*report)()) {
                if (declCtx != DeclarationContext::Parameter) {
                    ((diagReporter_).*(report))();
                    skipTo(SyntaxKind::CloseBracketToken);
                    return false;
                }
                return true;
            };

    auto checkDialect =
            [this] () {
                if (tree_->dialect().std() < LanguageDialect::Std::C99) {
                    diagReporter_.ExpectedFeature(
                            "C99 array declarators with `*', `static', and type-qualifiers "
                            "within function parameters");
                }
            };

    switch (peek().kind()) {
        case SyntaxKind::OpenParenToken: {
            auto funcDecltorSfx = makeNode<ParameterSuffixSyntax>();
            funcDecltorSfx->openParenTkIdx_ = consume();
            if (!parseParameterDeclarationListAndOrEllipsis(funcDecltorSfx)
                    || !match(SyntaxKind::CloseParenToken, &funcDecltorSfx->closeParenTkIdx_))
                return false;

            if (peek().kind() == SyntaxKind::Keyword_ExtPSY_omission)
                funcDecltorSfx->psyOmitTkIdx_ = consume();

            decltor = makeNode<ArrayOrFunctionDeclaratorSyntax>(SyntaxKind::FunctionDeclarator);
            decltor->asArrayOrFunctionDeclarator()->suffix_ = funcDecltorSfx;
            break;
        }

        case SyntaxKind::OpenBracketToken: {
            auto arrDecltorSx = makeNode<SubscriptSuffixSyntax>();
            arrDecltorSx->openBracketTkIdx_ = consume();
            switch (peek().kind()) {
                case SyntaxKind::CloseBracketToken:
                    break;

                case SyntaxKind::AsteriskToken:
                    checkDialect();
                    if (!validateContext(&Parser::DiagnosticsReporter::
                                         UnexpectedPointerInArrayDeclarator)) {
                        skipTo(SyntaxKind::CloseBracketToken);
                        return false;
                    }
                    arrDecltorSx->asteriskTkIdx_ = consume();
                    break;

                case SyntaxKind::Keyword_const:
                case SyntaxKind::Keyword_volatile:
                case SyntaxKind::Keyword_restrict:
                case SyntaxKind::Keyword__Atomic:
                case SyntaxKind::Keyword_ExtGNU___attribute__: {
                    checkDialect();
                    if (!validateContext(&Parser::DiagnosticsReporter::
                                         UnexpectedStaticOrTypeQualifiersInArrayDeclarator)
                            || !parseTypeQualifiersAndAttributes(arrDecltorSx->qualsAndAttrs1_)) {
                        skipTo(SyntaxKind::CloseBracketToken);
                        return false;
                    }

                    auto tkK = peek().kind();
                    if (tkK == SyntaxKind::AsteriskToken) {
                        arrDecltorSx->asteriskTkIdx_ = consume();
                        break;
                    }
                    else if (tkK != SyntaxKind::Keyword_static) {
                        if (!parseExpressionWithPrecedenceAssignment(arrDecltorSx->expr_)) {
                            skipTo(SyntaxKind::CloseBracketToken);
                            return false;
                        }
                        break;
                    }
                    [[fallthrough]];
                }

                case SyntaxKind::Keyword_static:
                    checkDialect();
                    if (!validateContext(&Parser::DiagnosticsReporter::
                                         UnexpectedStaticOrTypeQualifiersInArrayDeclarator)) {
                        skipTo(SyntaxKind::CloseBracketToken);
                        return false;
                    }

                    arrDecltorSx->staticKwTkIdx_ = consume();
                    switch (peek().kind()) {
                        case SyntaxKind::Keyword_const:
                        case SyntaxKind::Keyword_volatile:
                        case SyntaxKind::Keyword_restrict:
                        case SyntaxKind::Keyword_ExtGNU___attribute__:
                            if (!parseTypeQualifiersAndAttributes(arrDecltorSx->qualsAndAttrs2_)) {
                                skipTo(SyntaxKind::CloseBracketToken);
                                return false;
                            }
                            [[fallthrough]];

                        default:
                            break;
                    }
                    [[fallthrough]];

                default:
                    if (!parseExpressionWithPrecedenceAssignment(arrDecltorSx->expr_)) {
                        skipTo(SyntaxKind::CloseBracketToken);
                        return false;
                    }
                    break;
            }

            if (!matchOrSkipTo(SyntaxKind::CloseBracketToken, &arrDecltorSx->closeBracketTkIdx_))
                return false;

            decltor = makeNode<ArrayOrFunctionDeclaratorSyntax>(SyntaxKind::ArrayDeclarator);
            decltor->asArrayOrFunctionDeclarator()->suffix_ = arrDecltorSx;
            break;
        }

        default:
            return true;
    }

    auto arrayOrFuncDecltor = static_cast<ArrayOrFunctionDeclaratorSyntax*>(decltor);
    arrayOrFuncDecltor->attrs1_ = attrList;
    arrayOrFuncDecltor->innerDecltor_ = innerDecltor;

    SpecifierListSyntax** specList_cur = &arrayOrFuncDecltor->attrs2_;

    switch (peek().kind()) {
        case SyntaxKind::Keyword_ExtGNU___asm__: {
            SpecifierSyntax* spec = nullptr;
            if (!parseExtGNU_AsmLabel_AtFirst(spec))
                return false;

            *specList_cur = makeNode<SpecifierListSyntax>(spec);
            specList_cur = &(*specList_cur)->next;

            if (peek().kind() != SyntaxKind::Keyword_ExtGNU___attribute__)
                break;
            [[fallthrough]];
        }

        case SyntaxKind::Keyword_ExtGNU___attribute__:
            if (!parseExtGNU_AttributeSpecifierList_AtFirst(*specList_cur))
                return false;
            break;

        default:
            break;
    }

    switch (peek().kind()) {
        case SyntaxKind::OpenParenToken:
        case SyntaxKind::OpenBracketToken: {
            innerDecltor = decltor;
            return parseDirectDeclaratorSuffix(decltor,
                                               declCtx,
                                               decltorForm,
                                               nullptr,
                                               innerDecltor);
        }

        default:
            return true;
    }

    return true;
}

/**
 * Parse a \a pointer \a declarator.
 *
 \verbatim
 pointer:
     * type-qualifier-list_opt
     * type_qualifier-list_opt pointer
 \endverbatim
 *
 * \remark 6.7.6.1.
 */
bool Parser::parseTypeQualifiersAndAttributes(SpecifierListSyntax*& specList)
{
    DBG_THIS_RULE();

    SpecifierListSyntax** specList_cur = &specList;

    while (true) {
        SpecifierSyntax* spec = nullptr;
        switch (peek().kind()) {
            case SyntaxKind::Keyword_ExtGNU___attribute__:
                return parseExtGNU_AttributeSpecifierList_AtFirst(specList);

            case SyntaxKind::Keyword_ExtGNU___asm__:
                if (parseExtGNU_AsmLabel_AtFirst(spec))
                    return false;
                break;

            case SyntaxKind::Keyword_const:
                parseTrivialSpecifier_AtFirst<TypeQualifierSyntax>(
                            spec,
                            SyntaxKind::ConstQualifier);
                break;

            case SyntaxKind::Keyword_volatile:
                parseTrivialSpecifier_AtFirst<TypeQualifierSyntax>(
                            spec,
                            SyntaxKind::VolatileQualifier);
                break;

            case SyntaxKind::Keyword_restrict:
                parseTrivialSpecifier_AtFirst<TypeQualifierSyntax>(
                            spec,
                            SyntaxKind::RestrictQualifier);
                break;

            case SyntaxKind::Keyword__Atomic:
                parseTrivialSpecifier_AtFirst<TypeQualifierSyntax>(
                            spec,
                            SyntaxKind::AtomicQualifier);
                break;

            default:
                return true;
        }

        *specList_cur = makeNode<SpecifierListSyntax>(spec);
        specList_cur = &(*specList_cur)->next;
    }
}

/* Initializers */

/**
 * Parse an \a initializer.
 *
 \verbatim
 initializer:
     assignment-expression
     { initializer-list }
     { initializer-list, }
 \endverbatim
 *
 * Adjusted grammar:
 *
 \verbatim
 initializer:
     expression-initializer
     brace-enclosed-initializer
 \endverbatim
 *
 * \remark 6.7.9
 */
bool Parser::parseInitializer(InitializerSyntax*& init)
{
    DBG_THIS_RULE();

    switch (peek().kind()) {
        case SyntaxKind::OpenBraceToken:
            return parseBraceEnclosedInitializer_AtFirst(init);

        default:
            return parseExpressionInitializer(init);
    }
}

/**
 * Parse an \a initializer that is an \a assignment-expression.
 *
 * In the adjusted grammar of Parser::parseInitializer.
 *
 \verbatim
 expression-initializer:
     assignment-expression
 \endverbatim
 */
bool Parser::parseExpressionInitializer(InitializerSyntax*& init)
{
    ExpressionSyntax* expr = nullptr;
    if (!parseExpressionWithPrecedenceAssignment(expr))
        return false;

    auto exprInit = makeNode<ExpressionInitializerSyntax>();
    init = exprInit;
    exprInit->expr_ = expr;
    return true;
}

/**
 * Parse an \a initializer that is an \a initializer-list enclosed in braces.
 *
 * In the adjusted grammar of Parser::parseInitializer.
 *
 \verbatim
 brace-enclosed-initializer
     { initializer-list }
     { initializer-list, }
 \endverbatim
 */
bool Parser::parseBraceEnclosedInitializer_AtFirst(InitializerSyntax*& init)
{
    DBG_THIS_RULE();
    PSY_ASSERT_3(peek().kind() == SyntaxKind::OpenBraceToken,
                  return false,
                  "expected `{'");

    auto braceInit = makeNode<BraceEnclosedInitializerSyntax>();
    init = braceInit;
    braceInit->openBraceTkIdx_ = consume();

    if (peek().kind() == SyntaxKind::CloseBraceToken) {
        diagReporter_.ExpectedBraceEnclosedInitializerList();
        braceInit->closeBraceTkIdx_ = consume();
        return true;
    }

    if (!parseInitializerList(braceInit->initList_)) {
        skipTo(SyntaxKind::CloseBraceToken);
        consume();
        return false;
    }

    return matchOrSkipTo(SyntaxKind::CloseBraceToken, &braceInit->closeBraceTkIdx_);
}

bool Parser::parseInitializerList(InitializerListSyntax*& initList)
{
    DBG_THIS_RULE();

    return parseCommaSeparatedItems<InitializerSyntax>(
                initList,
                &Parser::parseInitializerListItem);
}

bool Parser::parseInitializerListItem(InitializerSyntax*& init, InitializerListSyntax*& initList)
{
    DBG_THIS_RULE();

    switch (peek().kind()) {
        case SyntaxKind::CloseBraceToken:
            return true;

        case SyntaxKind::CommaToken:
            if (peek(2).kind() == SyntaxKind::CloseBraceToken) {
                initList->delimTkIdx_ = consume();
                return true;
            }
            diagReporter_.ExpectedFIRSTofExpression();
            return false;

        case SyntaxKind::DotToken:
            return parseDesignatedInitializer_AtFirst(
                        init,
                        &Parser::parseFieldDesignator_AtFirst);

        case SyntaxKind::OpenBracketToken:
            return parseDesignatedInitializer_AtFirst(
                        init,
                        &Parser::parseArrayDesignator_AtFirst);

        case SyntaxKind::OpenBraceToken:
            return parseBraceEnclosedInitializer_AtFirst(init);

        default:
            return parseExpressionInitializer(init);
    }
}

bool Parser::parseDesignatedInitializer_AtFirst(InitializerSyntax*& init,
                                                bool (Parser::*parseDesig)(DesignatorSyntax*& desig))
{
    DBG_THIS_RULE();
    PSY_ASSERT_3(peek().kind() == SyntaxKind::DotToken
                        || peek().kind() == SyntaxKind::OpenBracketToken,
                  return false,
                  "assert failure: `.' or `['");

    if (tree_->dialect().std() < LanguageDialect::Std::C99
            && !tree_->parseOptions().languageExtensions().isEnabled_extGNU_DesignatedInitializers())
        diagReporter_.ExpectedFeature("GNU/C99 designated initializers");

    DesignatorListSyntax* desigList = nullptr;
    if (!parseDesignatorList_AtFirst(desigList, parseDesig))
        return false;

    auto desigInit = makeNode<DesignatedInitializerSyntax>();
    init = desigInit;
    desigInit->desigs_ = desigList;

    switch (peek().kind()) {
        case SyntaxKind::EqualsToken:
            desigInit->equalsTkIdx_ = consume();
            return parseInitializer(desigInit->init_);

        default:
            diagReporter_.ExpectedFollowOfDesignatedInitializer();
            return parseInitializer(desigInit->init_);
    }
}

bool Parser::parseDesignatorList_AtFirst(DesignatorListSyntax*& desigList,
                                         bool (Parser::*parseDesig)(DesignatorSyntax*& desig))
{
    DBG_THIS_RULE();
    PSY_ASSERT_3(peek().kind() == SyntaxKind::DotToken
                        || peek().kind() == SyntaxKind::OpenBracketToken,
                  return false,
                  "assert failure: `.' or `['");

    DesignatorListSyntax** desigs_cur = &desigList;

    while (true) {
        DesignatorSyntax* desig = nullptr;
        if (!(((this)->*(parseDesig))(desig)))
            return false;

        *desigs_cur = makeNode<DesignatorListSyntax>(desig);
        desigs_cur = &(*desigs_cur)->next;

        switch (peek().kind()) {
            case SyntaxKind::DotToken:
                parseDesig = &Parser::parseFieldDesignator_AtFirst;
                break;

            case SyntaxKind::OpenBracketToken:
                parseDesig = &Parser::parseArrayDesignator_AtFirst;
                break;

            default:
                return true;
        }
    }
}

bool Parser::parseFieldDesignator_AtFirst(DesignatorSyntax*& desig)
{
    DBG_THIS_RULE();
    PSY_ASSERT_3(peek().kind() == SyntaxKind::DotToken,
                  return false,
                  "assert failure: `.'");

    auto fldDesig = makeNode<FieldDesignatorSyntax>();
    desig = fldDesig;
    fldDesig->dotTkIdx_ = consume();

    if (peek().kind() == SyntaxKind::IdentifierToken) {
        fldDesig->identTkIdx_ = consume();
        return true;
    }

    diagReporter_.ExpectedFieldDesignator();
    return false;
}

bool Parser::parseArrayDesignator_AtFirst(DesignatorSyntax*& desig)
{
    DBG_THIS_RULE();
    PSY_ASSERT_3(peek().kind() == SyntaxKind::OpenBracketToken,
                  return false,
                  "assert failure: `['");

    auto arrDesig = makeNode<ArrayDesignatorSyntax>();
    desig = arrDesig;
    arrDesig->openBracketTkIdx_ = consume();

    return parseExpressionWithPrecedenceConditional(arrDesig->expr_)
                && matchOrSkipTo(SyntaxKind::CloseBracketToken, &arrDesig->closeBracketTkIdx_);
}

/**
 * Parse Standard's \c offsetof and GNU's \c __builtin_offsetof \a member-designator.
 */
bool Parser::parseOffsetOfDesignator(DesignatorSyntax*& desig)
{
    DBG_THIS_RULE();

    if (peek().kind() != SyntaxKind::IdentifierToken) {
        diagReporter_.ExpectedTokenOfCategoryIdentifier();
        return false;
    }

    auto offsetOfDesig = makeNode<OffsetOfDesignatorSyntax>();
    desig = offsetOfDesig;
    offsetOfDesig->identTkIdx_ = consume();

    DesignatorListSyntax* desigList = nullptr;
    switch (peek().kind()) {
        case SyntaxKind::DotToken:
            if (!parseDesignatorList_AtFirst(desigList, &Parser::parseFieldDesignator_AtFirst))
                return false;
            break;

        case SyntaxKind::OpenBracketToken:
            if (!parseDesignatorList_AtFirst(desigList, &Parser::parseArrayDesignator_AtFirst))
                return false;
            break;

        default:
            return true;
    }

    offsetOfDesig->desigs_ = desigList;
    return true;
}
