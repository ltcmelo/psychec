// Copyright (c) 2020/21 Leandro T. C. Melo <ltcmelo@gmail.com>
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

using namespace psy;
using namespace C;

/* Generic */
const std::string Parser::DiagnosticsReporter::ID_of_ExpectedFeature = "Parser-000";
const std::string Parser::DiagnosticsReporter::ID_of_ExpectedToken = "Parser-001";
const std::string Parser::DiagnosticsReporter::ID_of_ExpectedOneOfTokens = "Parser-002";
const std::string Parser::DiagnosticsReporter::ID_of_ExpectedTokenOfCategory = "Parser-003";
const std::string Parser::DiagnosticsReporter::ID_of_ExpectedConstant = "Parser-004";
const std::string Parser::DiagnosticsReporter::ID_of_ExpectedStringLiteral = "Parser-005";
const std::string Parser::DiagnosticsReporter::ID_of_ExpectedExpression = "Parser-006";
const std::string Parser::DiagnosticsReporter::ID_of_ExpectedEnumerationConstant= "Parser-007";

/* Expressions, declarations, and statements */
const std::string Parser::DiagnosticsReporter::ID_of_ExpectedFieldName = "Parser-200-6.5.2";
const std::string Parser::DiagnosticsReporter::ID_of_ExpectedBraceEnclosedInitializerList = "Parser-201-6.7.9";
const std::string Parser::DiagnosticsReporter::ID_of_ExpectedFieldDesignator = "Parser-202-6.7.9";
const std::string Parser::DiagnosticsReporter::ID_of_ExpetedEqualsFOLLOWingArrayDesignator = "Parser-203-6.7.9";
const std::string Parser::DiagnosticsReporter::ID_of_ExpectedFOLLOWofDeclarator = "Parser-204-6.7.6";
const std::string Parser::DiagnosticsReporter::ID_of_ExpectedFOLLOWofInitializedDeclarator = "Parser-205";
const std::string Parser::DiagnosticsReporter::ID_of_UnexpectedInitializerFOLLOWingDeclarator = "Parser-206-6.7.9";
const std::string Parser::DiagnosticsReporter::ID_of_UnexpectedStaticOrTypeQualifierInArrayDeclarator = "Parser-207-6.7.6";
const std::string Parser::DiagnosticsReporter::ID_of_UnexpectedPointerInArrayDeclarator = "Parser-308-6.7.6";
const std::string Parser::DiagnosticsReporter::ID_of_ExpectedFIRSTofDirectDeclarator = "Parser--309-6.7.6";
const std::string Parser::DiagnosticsReporter::ID_of_ExpectedFOLLOWofStructOrUnionOrEnum = "Parser-310-6.7.2.1";
const std::string Parser::DiagnosticsReporter::ID_of_ExpectedFOLLOWofEnum = "Parser-311-6.7.2.1";
const std::string Parser::DiagnosticsReporter::ID_of_ExpectedFIRSTofSpecifierQualifier = "Parser-312-6.7.2.1";
const std::string Parser::DiagnosticsReporter::ID_of_ExpectedNamedParameterBeforeEllipsis = "Parser-313-6.7.6.3";
const std::string Parser::DiagnosticsReporter::ID_of_ExpectedTypeSpecifier = "Parser-314-6.7";
const std::string Parser::DiagnosticsReporter::ID_of_UnexpectedCaseLabelOutsideSwitch = "Parser-315-6.8.1-2";
const std::string Parser::DiagnosticsReporter::ID_of_UnexpectedDefaultLabelOutsideSwitch = "Parser-315-6.8.1-2";
const std::string Parser::DiagnosticsReporter::ID_of_ExpectedIdentifier = "Parser-316-6.8.6.1";


std::string Parser::DiagnosticsReporter::joinTokenNames(const std::vector<SyntaxKind>& validTkKinds)
{
    std::string s;
    if (!validTkKinds.empty()) {
        auto syntaxK = validTkKinds.front();
        auto tkKindCat = kindCategory(syntaxK);
        if (tkKindCat == SyntaxKindCategory::KeywordOrPunctuatorToken)
            s += "`";
        s += to_string(syntaxK);
        if (tkKindCat == SyntaxKindCategory::KeywordOrPunctuatorToken)
            s += "'";
    }

    for (std::vector<int>::size_type i = 1; i < validTkKinds.size(); ++i) {
        auto syntaxK = validTkKinds[i];
        auto tkKindCat = kindCategory(syntaxK);
        s += " or ";
        if (tkKindCat == SyntaxKindCategory::KeywordOrPunctuatorToken)
            s += "`";
        s += to_string(syntaxK);
        if (tkKindCat == SyntaxKindCategory::KeywordOrPunctuatorToken)
            s += "'";
    }
    s += " ";

    return s;
}

void Parser::DiagnosticsReporter::diagnose(DiagnosticDescriptor&& desc)
{
    if (!parser_->inBactrackingMode())
        parser_->tree_->newDiagnostic(desc, parser_->curTkIdx_);
};

/* Generic */

void Parser::DiagnosticsReporter::ExpectedFeature(const std::string& name)
{
    diagnose(DiagnosticDescriptor(ID_of_ExpectedFeature,
                                  "[[unexpected extension or C dialect]]",
                                  name + " is either an extension or unsupported in this dialect",
                                  DiagnosticSeverity::Warning,
                                  DiagnosticCategory::Syntax));
}

void Parser::DiagnosticsReporter::ExpectedToken(SyntaxKind syntaxK)
{
    auto s = "expected `"
            + to_string(syntaxK)
            + "', got `"
            + parser_->peek().valueText_c_str()
            + "'";

    diagnose(DiagnosticDescriptor(ID_of_ExpectedToken,
                                  "[[expected an specific token]]",
                                  s,
                                  DiagnosticSeverity::Error,
                                  DiagnosticCategory::Syntax));
}

void Parser::DiagnosticsReporter::ExpectedOneOfTokens(const std::vector<SyntaxKind>& validTkKinds)
{
    std::string s = "expected "
            + joinTokenNames(validTkKinds)
            + ", got `"
            + parser_->peek().valueText()
            + "'";

    diagnose(DiagnosticDescriptor(ID_of_ExpectedOneOfTokens,
                                  "[[expected one token from a selection of tokens]]",
                                  s,
                                  DiagnosticSeverity::Error,
                                  DiagnosticCategory::Syntax));
}

void Parser::DiagnosticsReporter::ExpectedTokenOfCategory(const std::string& name,
                                                          const std::string& cat)
{
    std::string s = "expected "
                + name
                + " got `"
                + parser_->peek().valueText() + "'";

    diagnose(DiagnosticDescriptor(cat,
                                  "[[expected any token of a given category]]",
                                  s,
                                  DiagnosticSeverity::Error,
                                  DiagnosticCategory::Syntax));
}

void Parser::DiagnosticsReporter::ExpectedConstant()
{
    ExpectedTokenOfCategory("constant", ID_of_ExpectedConstant);
}

void Parser::DiagnosticsReporter::ExpectedStringLiteral()
{
    ExpectedTokenOfCategory("string-literal", ID_of_ExpectedStringLiteral);
}

void Parser::DiagnosticsReporter::ExpectedExpression()
{
    ExpectedTokenOfCategory("expression", ID_of_ExpectedExpression);
}

void Parser::DiagnosticsReporter::ExpectedEnumerationConstant()
{
    ExpectedTokenOfCategory("enumeration-constant", ID_of_ExpectedEnumerationConstant);
}

/* Expressions, declarations, and statements */

void Parser::DiagnosticsReporter::ExpectedFieldName()
{
    auto s = "expected field name, got `"
           + parser_->peek().valueText()
           + "'";

    diagnose(DiagnosticDescriptor(ID_of_ExpectedFieldName,
                                  "[[expected field name]]",
                                  s,
                                  DiagnosticSeverity::Error,
                                  DiagnosticCategory::Syntax));
}

void Parser::DiagnosticsReporter::ExpectedBraceEnclosedInitializerList()
{
    diagnose(DiagnosticDescriptor(ID_of_ExpectedBraceEnclosedInitializerList,
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

    diagnose(DiagnosticDescriptor(ID_of_ExpectedFieldDesignator,
                                  "[[expected field designator]]",
                                  s,
                                  DiagnosticSeverity::Error,
                                  DiagnosticCategory::Syntax));
}

void Parser::DiagnosticsReporter::UnexpectedInitializerFOLLOWingDeclarator()
{
    diagnose(DiagnosticDescriptor(ID_of_UnexpectedInitializerFOLLOWingDeclarator,
                                  "[[unexpected initializer for declarator]]",
                                  "declarator may not be initialized",
                                  DiagnosticSeverity::Error,
                                  DiagnosticCategory::Syntax));
}

void Parser::DiagnosticsReporter::ExpetedEqualsFOLLOWingArrayDesignator()
{
    diagnose(DiagnosticDescriptor(ID_of_ExpetedEqualsFOLLOWingArrayDesignator,
                                  "[[obsolete array designator syntax]]",
                                  "obsolete array designator without `='",
                                  DiagnosticSeverity::Warning,
                                  DiagnosticCategory::Syntax));
}

void Parser::DiagnosticsReporter::UnexpectedStaticOrTypeQualifiersInArrayDeclarator()
{
    diagnose(DiagnosticDescriptor(ID_of_UnexpectedStaticOrTypeQualifierInArrayDeclarator,
                                  "[[unexpected static or type qualifier in array declarator]]",
                                  "`static' and type-qualifiers are only allowed in array declarators "
                                  "within function parameters",
                                  DiagnosticSeverity::Error,
                                  DiagnosticCategory::Syntax));
}

void Parser::DiagnosticsReporter::UnexpectedPointerInArrayDeclarator()
{
    diagnose(DiagnosticDescriptor(ID_of_UnexpectedPointerInArrayDeclarator,
                                  "[[unexpected pointer in array declarator]]",
                                  "`*' is only allowed in array declarators "
                                  "within function parameters",
                                  DiagnosticSeverity::Error,
                                  DiagnosticCategory::Syntax));
}

void Parser::DiagnosticsReporter::ExpectedFOLLOWofDeclarator()
{
    auto validTkKinds = { CommaToken,
                          SemicolonToken,
                          EqualsToken };

    std::string s = "expected "
                + joinTokenNames(validTkKinds)
                + "after declarator, got `"
                + parser_->peek().valueText_c_str()
                + "'";

    diagnose(DiagnosticDescriptor(ID_of_ExpectedFOLLOWofDeclarator,
                                  "[[unexpected FOLLOW of declarator]]",
                                  s,
                                  DiagnosticSeverity::Error,
                                  DiagnosticCategory::Syntax));
}

void Parser::DiagnosticsReporter::ExpectedFOLLOWofInitializedDeclarator()
{
    auto validTkKinds = { CommaToken,
                          SemicolonToken };

    std::string s = "expected "
                + joinTokenNames(validTkKinds)
                + "after initialized declarator, got `"
                + parser_->peek().valueText_c_str()
                + "'";

    diagnose(DiagnosticDescriptor(ID_of_ExpectedFOLLOWofInitializedDeclarator,
                                  "[[unexpected FOLLOW of initialized declarator]]",
                                  s,
                                  DiagnosticSeverity::Error,
                                  DiagnosticCategory::Syntax));
}

void Parser::DiagnosticsReporter::ExpectedFIRSTofDirectDeclarator()
{
    auto validTkKinds = { IdentifierToken,
                          OpenParenToken };

    std::string s = "expected "
                + joinTokenNames(validTkKinds)
                + "starting direct-declarator, got `"
                + parser_->peek().valueText_c_str()
                + "'";

    diagnose(DiagnosticDescriptor(ID_of_ExpectedFIRSTofDirectDeclarator,
                                  "[[unexpected FIRST of direct-declarator]]",
                                  s,
                                  DiagnosticSeverity::Error,
                                  DiagnosticCategory::Syntax));
}

void Parser::DiagnosticsReporter::ExpectedFIRSTofSpecifierQualifier()
{
    std::string s = "expected specifier-qualifier-list, got `"
                + parser_->peek().valueText()
                + "'";

    diagnose(DiagnosticDescriptor(ID_of_ExpectedFIRSTofSpecifierQualifier,
                                  "[[unexpected FIRST of specifier-qualifier-list]]",
                                  s,
                                  DiagnosticSeverity::Error,
                                  DiagnosticCategory::Syntax));
}

void Parser::DiagnosticsReporter::ExpectedFOLLOWofStructOrUnionOrEnum()
{
    auto validTkKinds = { IdentifierToken,
                          OpenBraceToken };

    std::string s = "expected "
                + joinTokenNames(validTkKinds)
                + "following struct-or-union or <enum>, got `"
                + parser_->peek().valueText_c_str()
                + "'";

    diagnose(DiagnosticDescriptor(ID_of_ExpectedFOLLOWofStructOrUnionOrEnum,
                                  "[[unexpected struct-or-union or enum FOLLOW]]",
                                  s,
                                  DiagnosticSeverity::Error,
                                  DiagnosticCategory::Syntax));
}

void Parser::DiagnosticsReporter::ExpectedNamedParameterBeforeEllipsis()
{
    diagnose(DiagnosticDescriptor(ID_of_ExpectedNamedParameterBeforeEllipsis,
                                  "[[unexpected ellipsis before named parameter]]",
                                  "ISO C requires a named parameter before `...'",
                                  DiagnosticSeverity::Error,
                                  DiagnosticCategory::Syntax));
}

void Parser::DiagnosticsReporter::ExpectedTypeSpecifier()
{
    diagnose(DiagnosticDescriptor(ID_of_ExpectedTypeSpecifier,
                                  "[[declaration without type specifier]]",
                                  "missing type specifier, assume `int'",
                                  DiagnosticSeverity::Warning,
                                  DiagnosticCategory::Syntax));
}

void Parser::DiagnosticsReporter::UnexpectedCaseLabelOutsideSwitch()
{
    diagnose(DiagnosticDescriptor(ID_of_UnexpectedCaseLabelOutsideSwitch,
                                  "[[case label outside switch-statement]]",
                                  "`case' label not within a switch statement",
                                  DiagnosticSeverity::Error,
                                  DiagnosticCategory::Syntax));
}

void Parser::DiagnosticsReporter::UnexpectedDefaultLabelOutsideSwitch()
{
    diagnose(DiagnosticDescriptor(ID_of_UnexpectedDefaultLabelOutsideSwitch,
                                  "[[default label outside switch-statement]]",
                                  "`default' label not within a switch statement",
                                  DiagnosticSeverity::Error,
                                  DiagnosticCategory::Syntax));
}

void Parser::DiagnosticsReporter::ExpectedIdentifier()
{
    auto s = "expected identifier, got `"
           + parser_->peek().valueText()
           + "'";

    diagnose(DiagnosticDescriptor(ID_of_ExpectedIdentifier,
                                  "[[expected identifier]]",
                                  s,
                                  DiagnosticSeverity::Error,
                                  DiagnosticCategory::Syntax));
}
