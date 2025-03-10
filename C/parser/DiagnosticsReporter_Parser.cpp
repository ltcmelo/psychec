// Copyright (c) 2020/21/22 Leandro T. C. Melo <ltcmelo@gmail.com>
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

#include "Parser.h"

#include "syntax/SyntaxNodes.h"

using namespace psy;
using namespace C;

/* General */
const std::string Parser::DiagnosticsReporter::ID_of_ExpectedFeature = "Parser-000";

/* Terminal */
const std::string Parser::DiagnosticsReporter::ID_of_ExpectedToken = "Parser-101";
const std::string Parser::DiagnosticsReporter::ID_of_ExpectedTokenWithin = "Parser-102";
const std::string Parser::DiagnosticsReporter::ID_of_ExpectedTokenOfCategoryConstant = "Parser-104";
const std::string Parser::DiagnosticsReporter::ID_of_ExpectedTokenOfCategoryStringLiteral = "Parser-105";
const std::string Parser::DiagnosticsReporter::ID_of_ExpectedTokenOfCategoryIdentifier = "Parser-106";

/* Non-terminal */
const std::string Parser::DiagnosticsReporter::ID_of_ExpectedFIRSTofExpression = "Parser-200-6.5";
const std::string Parser::DiagnosticsReporter::ID_of_ExpectedFIRSTofEnumerationConstant= "Parser-201-6.7.2.2";
const std::string Parser::DiagnosticsReporter::ID_of_ExpectedFIRSTofDirectDeclarator = "Parser-202-6.7.6";
const std::string Parser::DiagnosticsReporter::ID_of_ExpectedFIRSTofParameterDeclaration = "Parser-203-6.7.6.3";
const std::string Parser::DiagnosticsReporter::ID_of_ExpectedFIRSTofSpecifierQualifier = "Parser-204-6.7.2.1";
const std::string Parser::DiagnosticsReporter::ID_of_ExpectedFollowOfDesignatedInitializer = "Parser-205-6.7.9";
const std::string Parser::DiagnosticsReporter::ID_of_ExpectedFollowOfDeclarator = "Parser-206-6.7.6";
const std::string Parser::DiagnosticsReporter::ID_of_ExpectedFollowOfDeclaratorAndInitializer = "Parser-207";
const std::string Parser::DiagnosticsReporter::ID_of_ExpectedFollowOfStructOrUnionOrEnum = "Parser-208-6.7.2.1";
const std::string Parser::DiagnosticsReporter::ID_of_ExpectedFollowOfStructDeclarator = "Parser-209-6.7.2.1-9";
const std::string Parser::DiagnosticsReporter::ID_of_ExpectedFollowOfEnum = "Parser-210-6.7.2.1";

/* Detailed */
const std::string Parser::DiagnosticsReporter::ID_of_ExpectedFieldName = "Parser-300-6.5.2";
const std::string Parser::DiagnosticsReporter::ID_of_ExpectedBraceEnclosedInitializerList = "Parser-301-6.7.9";
const std::string Parser::DiagnosticsReporter::ID_of_ExpectedFieldDesignator = "Parser-302-6.7.9";
const std::string Parser::DiagnosticsReporter::ID_of_UnexpectedInitializerOfDeclarator = "Parser-303-6.7.9";
const std::string Parser::DiagnosticsReporter::ID_of_UnexpectedStaticOrTypeQualifierInArrayDeclarator = "Parser-304-6.7.6";
const std::string Parser::DiagnosticsReporter::ID_of_UnexpectedPointerInArrayDeclarator = "Parser-305-6.7.6";
const std::string Parser::DiagnosticsReporter::ID_of_ExpectedNamedParameterBeforeEllipsis = "Parser-306-6.7.6.3";
const std::string Parser::DiagnosticsReporter::ID_of_UnexpectedCaseLabelOutsideSwitch = "Parser-308-6.8.1-2";
const std::string Parser::DiagnosticsReporter::ID_of_UnexpectedDefaultLabelOutsideSwitch = "Parser-309-6.8.1-2";
const std::string Parser::DiagnosticsReporter::ID_of_UnexpectedContinueOutsideLoop = "Parser-310-6.8.6.2-1";
const std::string Parser::DiagnosticsReporter::ID_of_UnexpectedBreakOutsideSwitchOrLoop = "Parser-311-6.8.6.3-1";
const std::string Parser::DiagnosticsReporter::ID_of_UnexpectedGNUExtensionFlag = "Parser-312-GNU";

/* Ambiguities */
const std::string Parser::DiagnosticsReporter::ID_of_AmbiguousTypeNameOrExpressionAsTypeReference = "Parser-A1";
const std::string Parser::DiagnosticsReporter::ID_of_AmbiguousCastOrBinaryExpression = "Parser-A2";
const std::string Parser::DiagnosticsReporter::ID_of_AmbiguousExpressionOrDeclarationStatement = "Parser-A3";


std::string Parser::DiagnosticsReporter::joinTokenNames(const std::vector<SyntaxKind>& validTkKinds)
{
    std::string s;
    if (!validTkKinds.empty()) {
        auto tkK = validTkKinds.front();
        auto tkCat = SyntaxToken::category(tkK);
        if (tkCat == SyntaxToken::Category::Keywords
                || tkCat == SyntaxToken::Category::Punctuators)
            s += "`";
        s += to_string(tkK);
        if (tkCat == SyntaxToken::Category::Keywords
                || tkCat == SyntaxToken::Category::Punctuators)
            s += "'";
    }

    for (std::vector<int>::size_type i = 1; i < validTkKinds.size(); ++i) {
        auto tkK = validTkKinds[i];
        auto tkCat = SyntaxToken::category(tkK);
        s += " or ";
        if (tkCat == SyntaxToken::Category::Keywords
                || tkCat == SyntaxToken::Category::Punctuators)
            s += "`";
        s += to_string(tkK);
        if (tkCat == SyntaxToken::Category::Keywords
                || tkCat == SyntaxToken::Category::Punctuators)
            s += "'";
    }
    s += " ";

    return s;
}

void Parser::DiagnosticsReporter::diagnoseOrDelayDiagnostic(DiagnosticDescriptor&& desc)
{
    if (parser_->willBacktrack())
        return;

    if (IDsForDelay_.find(desc.id()) != IDsForDelay_.end())
        delayedDiags_.push_back(std::make_pair(desc, parser_->curTkIdx_));
    else
        parser_->tree_->newDiagnostic(desc, parser_->curTkIdx_);
}

void Parser::DiagnosticsReporter::diagnoseDelayedDiagnostics()
{
    for (const auto& p : delayedDiags_)
        parser_->tree_->newDiagnostic(p.first, p.second);
}

void Parser::DiagnosticsReporter::retainAmbiguityDiagnostic(
        DiagnosticDescriptor&& desc,
        const SyntaxNode* node)
{
    retainedAmbiguityDiags_.push_back(std::make_tuple(desc, parser_->curTkIdx_ - 1, node));
}

void Parser::DiagnosticsReporter::ExpectedFeature(const std::string& name)
{
    diagnoseOrDelayDiagnostic(
                DiagnosticDescriptor(ID_of_ExpectedFeature,
                                     "[[unexpected extension or C dialect]]",
                                     name + " is either an extension or unsupported in this dialect",
                                     DiagnosticSeverity::Warning,
                                     DiagnosticCategory::Syntax));
}

void Parser::DiagnosticsReporter::ExpectedToken(SyntaxKind tkK)
{
    auto s = "expected `"
            + to_string(tkK)
            + "', got `"
            + parser_->peek().valueText_c_str()
            + "'";

    diagnoseOrDelayDiagnostic(
                DiagnosticDescriptor(ID_of_ExpectedToken,
                                     "[[expected token]]",
                                     s,
                                     DiagnosticSeverity::Error,
                                     DiagnosticCategory::Syntax));
}

void Parser::DiagnosticsReporter::ExpectedTokenWithin(const std::vector<SyntaxKind>& validTkKinds)
{
    std::string s = "expected "
            + joinTokenNames(validTkKinds)
            + ", got `"
            + parser_->peek().valueText()
            + "'";

    diagnoseOrDelayDiagnostic(
                DiagnosticDescriptor(ID_of_ExpectedTokenWithin,
                                     "[[expected one of tokens]]",
                                     s,
                                     DiagnosticSeverity::Error,
                                     DiagnosticCategory::Syntax));
}

void Parser::DiagnosticsReporter::ExpectedTokenOfCategory(SyntaxToken::Category category,
                                                          const std::string& id)
{
    std::string s = "expected "
            + to_string(category)
            + " got `"
            + parser_->peek().valueText() + "'";

    diagnoseOrDelayDiagnostic(
                DiagnosticDescriptor(id,
                                     "[[expected token of category]]",
                                     s,
                                     DiagnosticSeverity::Error,
                                     DiagnosticCategory::Syntax));
}

void Parser::DiagnosticsReporter::ExpectedTokenOfCategoryIdentifier()
{
    ExpectedTokenOfCategory(SyntaxToken::Category::Constants, ID_of_ExpectedTokenOfCategoryIdentifier);
}

void Parser::DiagnosticsReporter::ExpectedTokenOfCategoryConstant()
{
    ExpectedTokenOfCategory(SyntaxToken::Category::Constants, ID_of_ExpectedTokenOfCategoryConstant);
}

void Parser::DiagnosticsReporter::ExpectedTokenOfCategoryStringLiteral()
{
    ExpectedTokenOfCategory(SyntaxToken::Category::StringLiterals, ID_of_ExpectedTokenOfCategoryStringLiteral);
}

/* Expressions, declarations, and statements */

void Parser::DiagnosticsReporter::ExpectedFIRSTof(const std::string& rule,
                                                  const std::string& id)
{
    std::string s = "expected "
            + rule
            + " got `"
            + parser_->peek().valueText() + "'";

    diagnoseOrDelayDiagnostic(
                DiagnosticDescriptor(id,
                                     "[[expected FIRST of]]",
                                     s,
                                     DiagnosticSeverity::Error,
                                     DiagnosticCategory::Syntax));

}

void Parser::DiagnosticsReporter::ExpectedFIRSTofExpression()
{
    return ExpectedFIRSTof("expression", ID_of_ExpectedFIRSTofExpression);
}

void Parser::DiagnosticsReporter::ExpectedFIRSTofEnumerationConstant()
{
    return ExpectedFIRSTof("enumeration-constant", ID_of_ExpectedFIRSTofEnumerationConstant);
}

void Parser::DiagnosticsReporter::ExpectedFieldName()
{
    auto s = "expected field name, got `"
            + parser_->peek().valueText()
            + "'";

    diagnoseOrDelayDiagnostic(
                DiagnosticDescriptor(ID_of_ExpectedFieldName,
                                     "[[expected field name]]",
                                     s,
                                     DiagnosticSeverity::Error,
                                     DiagnosticCategory::Syntax));
}

void Parser::DiagnosticsReporter::ExpectedBraceEnclosedInitializerList()
{
    diagnoseOrDelayDiagnostic(
                DiagnosticDescriptor(ID_of_ExpectedBraceEnclosedInitializerList,
                                     "[[unexpected empty brace-enclosed initializer]]",
                                     "ISO C forbids empty initializer braces",
                                     DiagnosticSeverity::Error,
                                     DiagnosticCategory::Syntax));
}

void Parser::DiagnosticsReporter::ExpectedFieldDesignator()
{
    auto s = "expected field designator, got `"
            + parser_->peek().valueText()
            + "'";

    diagnoseOrDelayDiagnostic(
                DiagnosticDescriptor(ID_of_ExpectedFieldDesignator,
                                     "[[expected field designator]]",
                                     s,
                                     DiagnosticSeverity::Error,
                                     DiagnosticCategory::Syntax));
}

void Parser::DiagnosticsReporter::UnexpectedInitializerOfDeclarator()
{
    diagnoseOrDelayDiagnostic(
                DiagnosticDescriptor(ID_of_UnexpectedInitializerOfDeclarator,
                                     "[[unexpected initializer for declarator]]",
                                     "declarator may not be initialized",
                                     DiagnosticSeverity::Error,
                                     DiagnosticCategory::Syntax));
}

void Parser::DiagnosticsReporter::ExpectedFollowOfDesignatedInitializer()
{
    diagnoseOrDelayDiagnostic(DiagnosticDescriptor(ID_of_ExpectedFollowOfDesignatedInitializer,
                                                   "[[obsolete array designator syntax]]",
                                                   "obsolete array designator without `='",
                                                   DiagnosticSeverity::Warning,
                                                   DiagnosticCategory::Syntax));
}

void Parser::DiagnosticsReporter::UnexpectedStaticOrTypeQualifiersInArrayDeclarator()
{
    diagnoseOrDelayDiagnostic(
                DiagnosticDescriptor(ID_of_UnexpectedStaticOrTypeQualifierInArrayDeclarator,
                                     "[[unexpected static or type qualifier in array declarator]]",
                                     "`static' and type-qualifiers are only allowed in array declarators "
                                     "within function parameters",
                                     DiagnosticSeverity::Error,
                                     DiagnosticCategory::Syntax));
}

void Parser::DiagnosticsReporter::UnexpectedPointerInArrayDeclarator()
{
    diagnoseOrDelayDiagnostic(
                DiagnosticDescriptor(ID_of_UnexpectedPointerInArrayDeclarator,
                                     "[[unexpected pointer in array declarator]]",
                                     "`*' is only allowed in array declarators "
                                     "within function parameters",
                                     DiagnosticSeverity::Error,
                                     DiagnosticCategory::Syntax));
}

void Parser::DiagnosticsReporter::ExpectedFollowOfDeclarator()
{
    auto validTkKinds = { SyntaxKind::CommaToken,
                          SyntaxKind::SemicolonToken,
                          SyntaxKind::EqualsToken };

    std::string s = "expected "
            + joinTokenNames(validTkKinds)
            + "after declarator, got `"
            + parser_->peek().valueText_c_str()
            + "'";

    diagnoseOrDelayDiagnostic(
                DiagnosticDescriptor(ID_of_ExpectedFollowOfDeclarator,
                                     "[[unexpected Follow of declarator]]",
                                     s,
                                     DiagnosticSeverity::Error,
                                     DiagnosticCategory::Syntax));
}

void Parser::DiagnosticsReporter::ExpectedFollowOfStructDeclarator()
{
    auto validTkKinds = { SyntaxKind::CommaToken,
                          SyntaxKind::SemicolonToken,
                          SyntaxKind::ColonToken };

    std::string s = "expected "
            + joinTokenNames(validTkKinds)
            + "after field declarator, got `"
            + parser_->peek().valueText_c_str()
            + "'";

    diagnoseOrDelayDiagnostic(
                DiagnosticDescriptor(ID_of_ExpectedFollowOfStructDeclarator,
                                     "[[unexpected Follow of field declarator]]",
                                     s,
                                     DiagnosticSeverity::Error,
                                     DiagnosticCategory::Syntax));
}

void Parser::DiagnosticsReporter::ExpectedFollowOfDeclaratorAndInitializer()
{
    auto validTkKinds = { SyntaxKind::CommaToken,
                          SyntaxKind::SemicolonToken };

    std::string s = "expected "
            + joinTokenNames(validTkKinds)
            + "after initialized declarator, got `"
            + parser_->peek().valueText_c_str()
            + "'";

    diagnoseOrDelayDiagnostic(
                DiagnosticDescriptor(ID_of_ExpectedFollowOfDeclaratorAndInitializer,
                                     "[[unexpected Follow of initialized declarator]]",
                                     s,
                                     DiagnosticSeverity::Error,
                                     DiagnosticCategory::Syntax));
}

void Parser::DiagnosticsReporter::ExpectedFIRSTofDirectDeclarator()
{
    auto validTkKinds = { SyntaxKind::IdentifierToken,
                          SyntaxKind::OpenParenToken };

    std::string s = "expected "
            + joinTokenNames(validTkKinds)
            + "starting direct-declarator, got `"
            + parser_->peek().valueText_c_str()
            + "'";

    diagnoseOrDelayDiagnostic(
                DiagnosticDescriptor(ID_of_ExpectedFIRSTofDirectDeclarator,
                                     "[[unexpected FIRST of direct-declarator]]",
                                     s,
                                     DiagnosticSeverity::Error,
                                     DiagnosticCategory::Syntax));
}

void Parser::DiagnosticsReporter::ExpectedFIRSTofParameterDeclaration()
{
    std::string s = "expected declaration specifiers or '...', got `"
            + parser_->peek().valueText()
            + "'";

    diagnoseOrDelayDiagnostic(
                DiagnosticDescriptor(ID_of_ExpectedFIRSTofParameterDeclaration,
                                     "[[unexpected FIRST of parameter-declaration]]",
                                     s,
                                     DiagnosticSeverity::Error,
                                     DiagnosticCategory::Syntax));
}

void Parser::DiagnosticsReporter::ExpectedFIRSTofSpecifierQualifier()
{
    std::string s = "expected specifier-qualifier-list, got `"
            + parser_->peek().valueText()
            + "'";

    diagnoseOrDelayDiagnostic(
                DiagnosticDescriptor(ID_of_ExpectedFIRSTofSpecifierQualifier,
                                     "[[unexpected FIRST of specifier-qualifier-list]]",
                                     s,
                                     DiagnosticSeverity::Error,
                                     DiagnosticCategory::Syntax));
}

void Parser::DiagnosticsReporter::ExpectedFollowOfStructOrUnionOrEnum()
{
    auto validTkKinds = { SyntaxKind::IdentifierToken,
                          SyntaxKind::OpenBraceToken };

    std::string s = "expected "
            + joinTokenNames(validTkKinds)
            + "following struct-or-union or enum, got `"
            + parser_->peek().valueText_c_str()
            + "'";

    diagnoseOrDelayDiagnostic(
                DiagnosticDescriptor(ID_of_ExpectedFollowOfStructOrUnionOrEnum,
                                     "[[unexpected struct-or-union or enum Follow]]",
                                     s,
                                     DiagnosticSeverity::Error,
                                     DiagnosticCategory::Syntax));
}

void Parser::DiagnosticsReporter::ExpectedNamedParameterBeforeEllipsis()
{
    diagnoseOrDelayDiagnostic(
                DiagnosticDescriptor(ID_of_ExpectedNamedParameterBeforeEllipsis,
                                     "[[unexpected ellipsis before named parameter]]",
                                     "ISO C requires a named parameter before `...'",
                                     DiagnosticSeverity::Error,
                                     DiagnosticCategory::Syntax));
}

void Parser::DiagnosticsReporter::UnexpectedCaseLabelOutsideSwitch()
{
    diagnoseOrDelayDiagnostic(
                DiagnosticDescriptor(ID_of_UnexpectedCaseLabelOutsideSwitch,
                                     "[[case label outside switch-statement]]",
                                     "`case' label not within a switch",
                                     DiagnosticSeverity::Error,
                                     DiagnosticCategory::Syntax));
}

void Parser::DiagnosticsReporter::UnexpectedDefaultLabelOutsideSwitch()
{
    diagnoseOrDelayDiagnostic(
                DiagnosticDescriptor(ID_of_UnexpectedDefaultLabelOutsideSwitch,
                                     "[[default label outside switch-statement]]",
                                     "`default' label not within a switch",
                                     DiagnosticSeverity::Error,
                                     DiagnosticCategory::Syntax));
}

void Parser::DiagnosticsReporter::UnexpectedContinueOutsideLoop()
{
    diagnoseOrDelayDiagnostic(
                DiagnosticDescriptor(ID_of_UnexpectedContinueOutsideLoop,
                                     "[[continue outside iteration-statement]]",
                                     "`continue' not within a loop",
                                     DiagnosticSeverity::Error,
                                     DiagnosticCategory::Syntax));
}

void Parser::DiagnosticsReporter::UnexpectedBreakOutsideSwitchOrLoop()
{
    diagnoseOrDelayDiagnostic(
                DiagnosticDescriptor(ID_of_UnexpectedBreakOutsideSwitchOrLoop,
                                     "[[break outside iteration- or switch-statement]]",
                                     "`break' not within a loop or switch",
                                     DiagnosticSeverity::Error,
                                     DiagnosticCategory::Syntax));
}

void Parser::DiagnosticsReporter::UnexpectedGNUExtensionFlag()
{
    diagnoseOrDelayDiagnostic(
                DiagnosticDescriptor(ID_of_UnexpectedGNUExtensionFlag,
                                     "[[unexpected `__extension__']]",
                                     "unrecognized `__extension__'",
                                     DiagnosticSeverity::Error,
                                     DiagnosticCategory::Syntax));
}

void Parser::DiagnosticsReporter::AmbiguousTypeNameOrExpressionAsTypeReference(
        const AmbiguousTypeNameOrExpressionAsTypeReferenceSyntax* node)
{
    std::string s = "ambiguous type name or expression `"
            + parser_->peek().valueText()
            + "'";

    retainAmbiguityDiagnostic(
                DiagnosticDescriptor(ID_of_AmbiguousTypeNameOrExpressionAsTypeReference,
                                     "[[ambiguous type name or expression]]",
                                     s,
                                     DiagnosticSeverity::Error,
                                     DiagnosticCategory::Syntax),
                node);
}

void Parser::DiagnosticsReporter::AmbiguousCastOrBinaryExpression(
        const AmbiguousCastOrBinaryExpressionSyntax* node)
{
    std::string s = "ambiguous cast or binary expression `"
            + parser_->peek().valueText()
            + "'";

    retainAmbiguityDiagnostic(
                DiagnosticDescriptor(ID_of_AmbiguousCastOrBinaryExpression,
                                     "[[ambiguous cast or binary expression]]",
                                     s,
                                     DiagnosticSeverity::Error,
                                     DiagnosticCategory::Syntax),
                node);
}

void Parser::DiagnosticsReporter::AmbiguousExpressionOrDeclarationStatement(
        const AmbiguousExpressionOrDeclarationStatementSyntax* node)
{
    std::string s = "ambiguous expression- or declaration-statement `"
            + parser_->peek().valueText()
            + "'";

    retainAmbiguityDiagnostic(
                DiagnosticDescriptor(ID_of_AmbiguousExpressionOrDeclarationStatement,
                                     "[[ambiguous expression- or declaration-statement]]",
                                     s,
                                     DiagnosticSeverity::Error,
                                     DiagnosticCategory::Syntax),
                node);
}
