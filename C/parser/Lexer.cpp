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

#include "Lexer.h"

#include "SyntaxTree.h"

#include "syntax/SyntaxLexeme_ALL.h"

#include <cctype>
#include <cstring>
#include <iostream>
#include <stack>

#ifndef UNLIKELY
  #ifdef __GNUC__
    #define UNLIKELY(expr) __builtin_expect(!!(expr), false)
  #else
    #define UNLIKELY(expr) (expr)
  #endif
#endif

using namespace psy;
using namespace C;

namespace {

const char* const kLine = "line";

// These are markers that Qt Creator's preprocessor creates (well... at least
// in some in the past) to keep track of macros; so keeping them for now.
const char* const kBegin = "begin";
const char* const kEnd = "end";
const char* const kExpansion = "expansion";

} // anonymous

Lexer::Lexer(SyntaxTree* tree)
    : tree_(tree)
    , text_(tree->text().rawText())
    , c_strBeg_(text_.c_str())
    , c_strEnd_(text_.c_str() + text_.size())
    , yytext_(c_strBeg_ - 1)
    , yy_(yytext_)
    , yychar_('\n')
    , yylineno_(1)
    , yycolumn_(0)
    , offset_(~0)  // Start immediately "before" 0.
    , withinLogicalLine_(false)
    , rawSyntaxK_splitTk(0)
    , diagReporter_(this)
{}

Lexer::~Lexer()
{}

namespace {

bool isByteOfMultiByteCP(unsigned char byte)
{
    // Check if most significant bit is set
    return byte & 0x80;
}

bool isRawStringLiteral(std::uint16_t rawSyntaxK)
{
    switch (rawSyntaxK) {
        case StringLiteral_R_Token:
        case StringLiteral_LR_Token:
        case StringLiteral_u8R_Token:
        case StringLiteral_uR_Token:
        case StringLiteral_UR_Token:
            return true;
        default:
            return false;
    }
}

bool isMultiLineToken(std::uint16_t rawSyntaxK)
{
    return rawSyntaxK == EndOfFile
            || rawSyntaxK == MultiLineCommentTrivia
            || rawSyntaxK == MultiLineDocumentationCommentTrivia
            || rawSyntaxK == Keyword_ExtPSY_omission
            || isRawStringLiteral(rawSyntaxK);
}

} // anonymous

void Lexer::lex()
{
    // Marker (invalid) token.
    tree_->addToken(SyntaxToken(nullptr));

    // Line and column...
    tree_->relayLineDirective(0, 1, tree_->filePath());
    tree_->relayLineStart(0);
    std::vector<std::pair<unsigned int, unsigned int>> expansions;
    unsigned int curExpansionIdx = 0;

    // Open/close brace tracking.
    std::stack<unsigned> braces;

    SyntaxToken tk(tree_);

    do {
        yylex(&tk);

LexEntry:
        if (tk.isAtStartOfLine() && tk.isKind(HashToken)) {
            auto offset = tk.charOffset_;
            yylex(&tk);

            if (!tk.isAtStartOfLine()
                    && tk.isKind(IdentifierToken)
                    && !strcmp(tk.identifier_->c_str(), kExpansion)) {
                // A Qt Creator-specific macro mark.
                yylex(&tk);

                if (!tk.isAtStartOfLine() && tk.isKind(IdentifierToken)) {
                    if (!strcmp(tk.identifier_->c_str(), kBegin)) {
                        // The start of an expansion section.
                        yylex(&tk);

                        // Gather where it happens and its length.
                        yylex(&tk);
                        yylex(&tk); // Skip the separating comma.
                        yylex(&tk);

                        // Gather the real line and column from the upcoming tokens; only
                        // relevant for tokens which are expanded but not generated.
                        while (!tk.isKind(EndOfFile)
                                    && !tk.isAtStartOfLine()) {
                            // A ~ means that the a number of generated tokens follows;
                            // otherwise, what follows is data.
                            if (tk.isKind(TildeToken)) {
                                yylex(&tk);

                                // Get the total number of generated tokens and specify "null"
                                // information for them.
                                auto all = strtoul(tk.valueText_c_str(), 0, 0);
                                auto prevSize = expansions.size();
                                expansions.resize(prevSize + all);
                                std::fill(expansions.begin() + prevSize,
                                          expansions.end(),
                                          std::make_pair(0, 0));

                                yylex(&tk);
                            }
                            else if (tk.isKind(IntegerConstantToken)) {
                                auto lineno = strtoul(tk.valueText_c_str(), 0, 0);
                                yylex(&tk);
                                yylex(&tk); // Skip the separating colon.
                                auto column = strtoul(tk.valueText_c_str(), 0, 0);
                                yylex(&tk);

                                // Store line and column for this non-generated token.
                                expansions.push_back(std::make_pair(lineno, column));
                            }
                        }
                    }
                    else if (!strcmp(tk.identifier_->c_str(), kEnd)) {
                        // The end of an expansion section.
                        expansions.clear();
                        curExpansionIdx = 0;
                        yylex(&tk);
                    }
                }
            }
            else {
                // A regular preprocessor directive.
                if (!tk.isAtStartOfLine()
                        && tk.isKind(IdentifierToken)
                        && !strcmp(tk.identifier_->c_str(), kLine)) {
                    yylex(&tk);
                }

                if (!tk.isAtStartOfLine()
                        && tk.isKind(IntegerConstantToken)) {
                    auto lineno = strtoul(tk.valueText_c_str(), 0, 0);
                    yylex(&tk);

                    if (!tk.isAtStartOfLine()
                            && tk.isKind(StringLiteralToken)) {
                        auto fileName = tree_->stringLiteral(tk.string_->c_str(), tk.string_->size());
                        tree_->relayLineDirective(offset, lineno, fileName->c_str());
                        yylex(&tk);
                    }
                }

                while (!tk.isAtStartOfLine() && !tk.isKind(EndOfFile)) {
                    // Skip the remaining of the line, ignoring a possible include.
                    yylex(&tk);
                }
            }
            goto LexEntry;
        }
        else if (tk.kind() == OpenBraceToken) {
            braces.push(tree_->tokenCount());
        }
        else if (tk.kind() == CloseBraceToken && !braces.empty()) {
            auto idx = braces.top();
            braces.pop();
            if (idx < tree_->tokenCount())
                tree_->tokenAt(idx).matchingBracket_ = tree_->tokenCount();
        }
        else if (tk.isComment()) {
            tree_->comments_.push_back(tk);
            if (tk.kind() != Keyword_ExtPSY_omission)
                continue;
        }

        bool isExpanded = false;
        bool isGenerated = false;
        if (!expansions.empty() && curExpansionIdx < expansions.size()) {
            isExpanded = true;
            const std::pair<unsigned int, unsigned int>& p = expansions[curExpansionIdx];
            if (p.first)
                tree_->relayExpansion(tk.charStart(), p);
            else
                isGenerated = true;
            ++curExpansionIdx;
        }
        tk.BF_.expanded_ = isExpanded;
        tk.BF_.generated_ = isGenerated;

        tree_->addToken(tk);
    }
    while (tk.kind());

    for (; !braces.empty(); braces.pop()) {
        auto idx = braces.top();
        tree_->tokenAt(idx).matchingBracket_ = tree_->tokenCount();
    }
}

void Lexer::yylex_core(SyntaxToken* tk)
{
LexEntry:
    while (yychar_ && std::isspace(yychar_)) {
        if (yychar_ == '\n') {
            tk->BF_.atStartOfLine_ = !withinLogicalLine_;
            tk->BF_.joined_ = withinLogicalLine_;

            if (withinLogicalLine_)
                withinLogicalLine_ = false;
            else if (!isMultiLineToken(rawSyntaxK_splitTk))
                withinLogicalLine_ = 0;
        }
        else {
            tk->BF_.hasLeadingWS_ = true;
        }
        yyinput();
    }

    yy_ = yytext_;

    tk->lineno_ = yylineno_;
    tk->column_ = yycolumn_ - 1;
    tk->byteOffset_ = yytext_ - c_strBeg_;
    tk->charOffset_ = offset_;

    if (yychar_)
        withinLogicalLine_ = false;
    else if (rawSyntaxK_splitTk) {
        tk->rawSyntaxK_ = EndOfFile;
        return;
    }

    if (rawSyntaxK_splitTk == MultiLineCommentTrivia
            || rawSyntaxK_splitTk == MultiLineDocumentationCommentTrivia
            || rawSyntaxK_splitTk == Keyword_ExtPSY_omission) {
        auto tkRawKind = rawSyntaxK_splitTk;
        while (yychar_) {
            if (yychar_ != '*')
                yyinput();
            else {
                yyinput();
                if (yychar_ == '/') {
                    yyinput();
                    withinLogicalLine_ = 0;
                    break;
                }
            }
        }

        if (tree_->parseOptions().treatmentOfComments() == ParseOptions::TreatmentOfComments::None)
            goto LexEntry;

        tk->rawSyntaxK_ = tkRawKind;
        return;
    }

    if (rawSyntaxK_splitTk == SingleLineCommentTrivia
            || rawSyntaxK_splitTk == SingleLineDocumentationCommentTrivia) {
        auto syntaxK = rawSyntaxK_splitTk;
        tk->BF_.joined_ = true;
        if (tree_->parseOptions().treatmentOfComments() != ParseOptions::TreatmentOfComments::None)
            tk->rawSyntaxK_ = syntaxK;
        withinLogicalLine_ = false;
        lexSingleLineComment(syntaxK);
        return;
    }

    if (isRawStringLiteral(rawSyntaxK_splitTk)) {
        tk->rawSyntaxK_ = rawSyntaxK_splitTk;
        if (lexContinuedRawStringLiteral())
            withinLogicalLine_ = false;
        return;
    }

    if (rawSyntaxK_splitTk != EndOfFile) {
        tk->BF_.joined_ = true;
        tk->rawSyntaxK_ = rawSyntaxK_splitTk;
        withinLogicalLine_ = false;
        lexUntilQuote(tk, '"', 1);
        return;
    }

    if (!yychar_) {
        tk->rawSyntaxK_ = EndOfFile;
        return;
    }

    unsigned char ch = yychar_;
    yyinput();

    switch (ch) {
        case '\\':
            withinLogicalLine_ = true;
            goto LexEntry;

        case '"':
            lexStringLiteral(tk);
            break;

        case '\'':
            lexCharacterConstant(tk);
            break;

        case '{':
            tk->rawSyntaxK_ = OpenBraceToken;
            break;

        case '}':
            tk->rawSyntaxK_ = CloseBraceToken;
            break;

        case '[':
            tk->rawSyntaxK_ = OpenBracketToken;
            break;

        case ']':
            tk->rawSyntaxK_ = CloseBracketToken;
            break;

        case '#':
            if (yychar_ == '#') {
                tk->rawSyntaxK_ = HashHashToken;
                yyinput();
            }
            else {
                tk->rawSyntaxK_ = HashToken;
            }
            break;

        case '(':
            tk->rawSyntaxK_ = OpenParenToken;
            break;

        case ')':
            tk->rawSyntaxK_ = CloseParenToken;
            break;

        case ';':
            tk->rawSyntaxK_ = SemicolonToken;
            break;

        case ':':
            if (yychar_ == '>') {
                // Digraph: 6.4.6-3.
                tk->rawSyntaxK_ = CloseBracketToken;
                yyinput();
            }
            else {
                tk->rawSyntaxK_ = ColonToken;
            }
            break;

        case '.':
            if (yychar_ == '.') {
                yyinput();
                if (yychar_ == '.') {
                    tk->rawSyntaxK_ = EllipsisToken;
                    yyinput();
                }
                else {
                    tk->rawSyntaxK_ = Error;
                }
            }
            else if (std::isdigit(yychar_)) {
                lexFloatingOrImaginaryFloating_AtFollowOfPeriod(tk, 1);
            }
            else {
                tk->rawSyntaxK_ = DotToken;
            }
            break;

        case '?':
            if (yychar_ == '?') {
                yyinput();
                if (yychar_ == '(') {
                    tk->rawSyntaxK_ = OpenBracketToken;
                    yyinput();
                }
                else if (yychar_ == ')') {
                    tk->rawSyntaxK_ = CloseBracketToken;
                    yyinput();
                }
                else if (yychar_ == '<') {
                    tk->rawSyntaxK_ = OpenBraceToken;
                    yyinput();
                }
                else if (yychar_ == '>') {
                    tk->rawSyntaxK_ = CloseBraceToken;
                    yyinput();
                }
            }
            else {
                tk->rawSyntaxK_ = QuestionToken;
            }
            break;

        case '+':
            if (yychar_ == '+') {
                tk->rawSyntaxK_ = PlusPlusToken;
                yyinput();
            }
            else if (yychar_ == '=') {
                tk->rawSyntaxK_ = PlusEqualsToken;
                yyinput();
            }
            else {
                tk->rawSyntaxK_ = PlusToken;
            }
            break;

        case '-':
            if (yychar_ == '-') {
                tk->rawSyntaxK_ = MinusMinusToken;
                yyinput();
            }
            else if (yychar_ == '=') {
                tk->rawSyntaxK_ = MinusEqualsToken;
                yyinput();
            }
            else if (yychar_ == '>') {
                tk->rawSyntaxK_ = ArrowToken;
                yyinput();
            }
            else {
                tk->rawSyntaxK_ = MinusToken;
            }
            break;

        case '*':
            if (yychar_ == '=') {
                tk->rawSyntaxK_ = AsteriskEqualsToken;
                yyinput();
            }
            else {
                tk->rawSyntaxK_ = AsteriskToken;
            }
            break;

        case '/':
            if (yychar_ == '/') {
                SyntaxKind syntaxK = SingleLineCommentTrivia;

                yyinput();
                if (yychar_ == '/' || yychar_ == '!') {
                    yyinput();
                    syntaxK = SingleLineDocumentationCommentTrivia;
                }
                lexSingleLineComment(syntaxK);

                if (tree_->parseOptions().treatmentOfComments() == ParseOptions::TreatmentOfComments::None)
                    goto LexEntry;

                tk->rawSyntaxK_ = syntaxK;
            }
            else if (yychar_ == '*') {
                SyntaxKind syntaxK = MultiLineCommentTrivia;

                yyinput();
                if (yychar_ == '*' || yychar_ == '!') {
                    const char ch = yychar_;
                    yyinput();
                    if (ch == '*' && yychar_ == '/')
                        goto LocalExit;

                    if (yychar_ == '<')
                        yyinput();

                    if (!yychar_ || std::isspace(yychar_))
                        syntaxK = MultiLineDocumentationCommentTrivia;
                }
                else if (yychar_ == '.') {
                    do {
                        yyinput();
                    }
                    while (yychar_ == '.');
                    syntaxK = Keyword_ExtPSY_omission;
                }

                while (yychar_) {
                    if (yychar_ != '*') {
                        yyinput();
                    }
                    else {
                        yyinput();
                        if (yychar_ == '/')
                            break;
                    }
                }

LocalExit:
                if (yychar_)
                    yyinput();
                else
                    rawSyntaxK_splitTk = syntaxK;

                if (tree_->parseOptions().treatmentOfComments() == ParseOptions::TreatmentOfComments::None)
                    goto LexEntry;

                tk->rawSyntaxK_ = syntaxK;
            }
            else if (yychar_ == '=') {
                tk->rawSyntaxK_ = SlashEqualsToken;
                yyinput();
            }
            else {
                tk->rawSyntaxK_ = SlashToken;
            }
            break;

        case '%':
            if (yychar_ == '=') {
                tk->rawSyntaxK_ = PercentEqualsToken;
                yyinput();
            }
            else if (yychar_ == '>') {
                // Digraph: 6.4.6-3.
                tk->rawSyntaxK_ = CloseBraceToken;
                yyinput();
            }
            else if (yychar_ == ':') {
                // Digraph: 6.4.6-3.
                tk->rawSyntaxK_ = HashToken;
                yyinput();
            }
            else {
                tk->rawSyntaxK_ = PercentToken;
            }
            break;

        case '^':
            if (yychar_ == '=') {
                tk->rawSyntaxK_ = CaretEqualsToken;
                yyinput();
            }
            else {
                tk->rawSyntaxK_ = CaretToken;
            }
            break;

        case '&':
            if (yychar_ == '&') {
                tk->rawSyntaxK_ = AmpersandAmpersandToken;
                yyinput();
            }
            else if (yychar_ == '=') {
                tk->rawSyntaxK_ = AmpersandEqualsToken;
                yyinput();
            }
            else {
                tk->rawSyntaxK_ = AmpersandToken;
            }
            break;

        case '|':
            if (yychar_ == '|') {
                tk->rawSyntaxK_ = BarBarToken;
                yyinput();
            }
            else if (yychar_ == '=') {
                tk->rawSyntaxK_ = BarEqualsToken;
                yyinput();
            }
            else {
                tk->rawSyntaxK_ = BarToken;
            }
            break;

        case '~':
            tk->rawSyntaxK_ = TildeToken;
            break;

        case '!':
            if (yychar_ == '=') {
                tk->rawSyntaxK_ = ExclamationEqualsToken;
                yyinput();
            }
            else {
                tk->rawSyntaxK_ = ExclamationToken;
            }
            break;

        case '=':
            if (yychar_ == '=') {
                tk->rawSyntaxK_ = EqualsEqualsToken;
                yyinput();
            }
            else {
                tk->rawSyntaxK_ = EqualsToken;
            }
            break;

        case '<':
            if (yychar_ == '<') {
                yyinput();
                if (yychar_ == '=') {
                    tk->rawSyntaxK_ = LessThanLessThanEqualsToken;
                    yyinput();
                }
                else
                    tk->rawSyntaxK_ = LessThanLessThanToken;
            }
            else if (yychar_ == '=') {
                tk->rawSyntaxK_ = LessThanEqualsToken;
                yyinput();
            }
            else if (yychar_ == ':') {
                // Digraph: 6.4.6-3.
                tk->rawSyntaxK_ = OpenBracketToken;
                yyinput();
            }
            else if (yychar_ == '%') {
                // Digraph: 6.4.6-3.
                tk->rawSyntaxK_ = OpenBraceToken;
                yyinput();
            }
            else {
                tk->rawSyntaxK_ = LessThanToken;
            }
            break;

        case '>':
            if (yychar_ == '>') {
                yyinput();
                if (yychar_ == '=') {
                    yyinput();
                    tk->rawSyntaxK_ = GreaterThanGreaterThanEqualsToken;
                }
                else
                    tk->rawSyntaxK_ = LessThanLessThanToken;
                tk->rawSyntaxK_ = GreaterThanGreaterThanToken;
            }
            else if (yychar_ == '=') {
                tk->rawSyntaxK_ = GreaterThanEqualsToken;
                yyinput();
            }
            else {
                tk->rawSyntaxK_ = GreaterThanToken;
            }
            break;

        case ',':
            tk->rawSyntaxK_ = CommaToken;
            break;

        default: {
            if (ch == 'L'
                    || ch == 'u'
                    || ch == 'U'
                    || ch == 'R') {
                if (yychar_ == '"') {
                    yyinput();
                    if (ch == 'R')
                        lexRawStringLiteral(tk);
                    else
                        lexStringLiteral(tk, ch);
                }
                else if (yychar_ == '\'') {
                    yyinput();
                    lexCharacterConstant(tk, ch);
                }
                else if (ch != 'R'
                            && yychar_ == 'R') {
                    yyinput();
                    if (yychar_ == '"') {
                        yyinput();
                        lexRawStringLiteral(tk, ch);
                    }
                    else {
                        lexIdentifier(tk, 1);
                    }
                }
                else if (ch == 'u'
                            && yychar_ == '8') {
                    yyinput();
                    if (yychar_ == '"') {
                        yyinput();
                        lexStringLiteral(tk, '8');
                    }
                    else if (yychar_ == '\'') {
                        yyinput();
                        lexCharacterConstant(tk, '8');
                    }
                    else if (yychar_ == 'R') {
                        yyinput();
                        if (yychar_ == '"') {
                            yyinput();
                            lexRawStringLiteral(tk, '8');
                        } else {
                            lexIdentifier(tk, 2);
                        }
                    }
                    else {
                        lexIdentifier(tk, 1);
                    }
                }
                else {
                    lexIdentifier(tk);
                }
            }
            else if (std::isalpha(ch)
                         || ch == '_'
                         || ch == '$'
                         || isByteOfMultiByteCP(ch)) {
                lexIdentifier(tk, yytext_ - yy_ - 1);
            }
            else if (std::isdigit(ch)) {
                lexIntegerOrFloatingConstant(tk);
            }
            else {
                tk->rawSyntaxK_ = Error;
            }
            break;
        }
    }
}

void Lexer::yylex(SyntaxToken* tk)
{
    tk->setup();

    yylex_core(tk);

    tk->byteSize_ = yytext_ - yy_;
    tk->charSize_ = offset_ - tk->charOffset_;
}

/**
 * Process a single unicode code point in an UTF-8 encoded source.
 * Points \c yychar to the byte of the next code point and modifies \a yy
 * to the value pointed by the updated \c yychar; \c offset will be
 * incremented by the number of UTF-16 code units that were needed.
 */
void Lexer::yyinput_core(const char*& yy,
                         unsigned char& yychar,
                         unsigned int& yycolumn,
                         unsigned int& offset)
{
    ++yycolumn;
    ++offset;

    if (UNLIKELY(isByteOfMultiByteCP(yychar))) {
        // Process multi-byte UTF-8 code point.
        unsigned int trailBytesCurCP = 1;
        for (unsigned char c = yychar << 2; isByteOfMultiByteCP(c); c <<= 1)
            ++trailBytesCurCP;

        // Code points >= 0x00010000 are represented by two UTF-16 code units.
        if (trailBytesCurCP >= 3) {
            ++yycolumn;
            ++offset;
        }

        yychar = *(yy += trailBytesCurCP + 1);
    }
    else {
        yychar = *++yy;
    }
}

void Lexer::yyinput()
{
    yyinput_core(yytext_, yychar_, yycolumn_, offset_);

    if (UNLIKELY(yychar_ == '\n')) {
        ++yylineno_;
        tree_->relayLineStart(offset_ + 1);
    }
}

/**
 * Lex an \a identifier.
 *
 * \remark 6.4.2.1
 */
void Lexer::lexIdentifier(SyntaxToken* tk, int advanced)
{
    const char* yytext = yytext_ - 1 - advanced;

    while (std::isalnum(yychar_)
            || yychar_ == '_'
            || yychar_ == '$'
            || isByteOfMultiByteCP(yychar_)) {
        yyinput();
    }

    int yyleng = yytext_ - yytext;

    if (tree_->parseOptions().treatmentOfIdentifiers() == ParseOptions
            ::TreatmentOfIdentifiers::Classify) {
        tk->rawSyntaxK_ = classify(yytext, yyleng, tree_->parseOptions());
    }
    else
        tk->rawSyntaxK_ = IdentifierToken;

    if (tk->rawSyntaxK_ == IdentifierToken) {
        tk->rawSyntaxK_ = classifyOperator(yytext, yyleng, tree_->parseOptions());
        tk->identifier_ = tree_->identifier(yytext, yyleng);
    }
}

/**
 * Lex a \a integer-constant or a \a floating-constant
 *
 * \remark 6.4.4.1, and 6.4.4.2
 */
void Lexer::lexIntegerOrFloatingConstant(SyntaxToken* tk)
{
    const char* yytext = yytext_ - 1;

    if (*yytext == '0' && yychar_) {
        if (yychar_ == 'x'|| yychar_ == 'X') {
            yyinput();
            lexHexadecimalDigitSequence();

            if (yychar_ == '.') {
                tk->rawSyntaxK_ = FloatingConstantToken;
                yyinput();

                if (tree_->parseOptions().dialect().std() < LanguageDialect::Std::C99) {
                    diagReporter_.IncompatibleLanguageDialect(
                                "hexadecimal floating-point constant",
                                LanguageDialect::Std::C99);
                }

                lexHexadecimalDigitSequence();
                lexBinaryExponentPart();
                lexFloatingOrImaginaryFloatingSuffix(tk, yytext_ - yytext);
                return;
            }
            lexIntegerOrImaginaryIntegerSuffix(tk, yytext_ - yytext);
            return;
        }

        if (yychar_ == 'b' || yychar_ == 'B') {
            yyinput();
            while (yychar_ == '0' || yychar_ == '1')
                yyinput();
            lexIntegerOrImaginaryIntegerSuffix(tk, yytext_ - yytext);
            return;
        }

        if (yychar_ >= '0' && yychar_ <= '7') {
            do {
                yyinput();
            }
            while (yychar_ >= '0' && yychar_ <= '7');

            lexIntegerOrImaginaryIntegerSuffix(tk, yytext_ - yytext);
            return;
        }
    }

    while (yychar_) {
        if (yychar_ == '.') {
            yyinput();
            lexFloatingOrImaginaryFloating_AtFollowOfPeriod(tk, yytext_ - yytext);
            return;
        }

        if (yychar_ == 'e' || yychar_ == 'E') {
            lexFloatingOrImaginaryFloating_AtExponent(tk, yytext_ - yytext);
            return;
        }

        if (!std::isdigit(yychar_))
            break;
        yyinput();
    }

    lexIntegerOrImaginaryIntegerSuffix(tk, yytext_ - yytext);
}

void Lexer::lexIntegerOrFloating_AtFollowOfSuffix(SyntaxToken* tk,
                                                  std::function<void ()> makeLexeme)
{
    if (std::isalnum(yychar_) || yychar_ == '_') {
        tk->rawSyntaxK_ = Error;
        do {
            yyinput();
        }
        while (std::isalnum(yychar_) || yychar_ == '_');
        return;
    }

    makeLexeme();
}

void Lexer::lexIntegerOrImaginaryIntegerSuffix(SyntaxToken* tk, unsigned int accLeng)
{
    const char* yytext = yytext_ - accLeng;
    if (yychar_ == 'i' || yychar_ == 'j') {
        lexImaginaryIntegerSuffix_AtFirst(tk);
        lexIntegerSuffix();
    }
    else {
        lexIntegerSuffix();
        lexImaginaryIntegerSuffix(tk);
    }

    lexIntegerOrFloating_AtFollowOfSuffix(
            tk,
            [&] () {
                if (tk->rawSyntaxK_ == ImaginaryIntegerConstantToken) {
                    tk->imaginaryInteger_ = tree_->imaginaryIntegerConstant(yytext, yytext_ - yytext);
                }
                else {
                    tk->rawSyntaxK_ = IntegerConstantToken;
                    tk->integer_ = tree_->integerConstant(yytext, yytext_ - yytext);
                }
            });
}

/**
 * Lex an \a integer-suffix.
 *
 * \remark 6.4.4.1
 */
void Lexer::lexIntegerSuffix(int suffixCnt)
{
    if (!suffixCnt)
        return;

    switch(yychar_) {
        case 'u':
        case 'U':
            yyinput();
            switch (yychar_) {
                case 'l':
                case 'L':
                    lexIntegerSuffix(--suffixCnt);
                    break;
            }
            break;

        case 'l':
            yyinput();
            if (yychar_ == 'l')
                yyinput();
            switch (yychar_) {
                case 'u':
                case 'U':
                    lexIntegerSuffix(--suffixCnt);
                    break;
            }
            break;

        case 'L':
            yyinput();
            if (yychar_ == 'L')
                yyinput();
            switch (yychar_) {
                case 'u':
                case 'U':
                    lexIntegerSuffix(--suffixCnt);
                    break;
            }
            break;
    }
}

void Lexer::lexImaginaryIntegerSuffix(SyntaxToken* tk)
{
    if (yychar_ == 'i' || yychar_ == 'j')
        lexImaginaryIntegerSuffix_AtFirst(tk);
}

void Lexer::lexImaginaryIntegerSuffix_AtFirst(SyntaxToken* tk)
{
    if (!tree_->parseOptions().extensions().isEnabled_ExtGNU_Complex()) {
        diagReporter_.IncompatibleLanguageExtension(
                    "imaginary constant",
                    LanguageExtensions::Ext::GNU_Complex);
    }

    yyinput();
    tk->rawSyntaxK_ = ImaginaryIntegerConstantToken;
}

void Lexer::lexFloatingOrImaginaryFloating_AtFollowOfPeriod(SyntaxToken* tk, unsigned int accLeng)
{
    const char* yytext = yytext_ - accLeng;
    lexDigitSequence();
    lexFloatingOrImaginaryFloating_AtExponent(tk, yytext_ - yytext);
}

void Lexer::lexFloatingOrImaginaryFloating_AtExponent(SyntaxToken* tk, unsigned int accLeng)
{
    const char* yytext = yytext_ - accLeng;
    lexExponentPart();
    lexFloatingOrImaginaryFloatingSuffix(tk, yytext_ - yytext);
}

void Lexer::lexFloatingOrImaginaryFloatingSuffix(SyntaxToken* tk, unsigned int accLeng)
{
    const char* yytext = yytext_ - accLeng;
    if (yychar_ == 'i' || yychar_ == 'j') {
        lexImaginaryFloatingSuffix_AtFirst(tk);
        lexFloatingSuffix();
    }
    else {
        lexFloatingSuffix();
        lexImaginaryFloatingSuffix(tk);
    }

    lexIntegerOrFloating_AtFollowOfSuffix(
            tk,
            [&] () {
                if (tk->rawSyntaxK_ == ImaginaryFloatingConstantToken) {
                    tk->imaginaryFloating_ = tree_->imaginaryFloatingConstant(yytext, yytext_ - yytext);
                }
                else {
                    tk->rawSyntaxK_ = FloatingConstantToken;
                    tk->floating_ = tree_->floatingConstant(yytext, yytext_ - yytext);
                }
            });
}

void Lexer::lexImaginaryFloatingSuffix(SyntaxToken* tk)
{
    if (yychar_ == 'i' || yychar_ == 'j')
        lexImaginaryFloatingSuffix_AtFirst(tk);
}

void Lexer::lexImaginaryFloatingSuffix_AtFirst(SyntaxToken* tk)
{
    if (!tree_->parseOptions().extensions().isEnabled_ExtGNU_Complex()) {
        diagReporter_.IncompatibleLanguageExtension(
                   "imaginary constant",
                   LanguageExtensions::Ext::GNU_Complex);
    }

    yyinput();
    tk->rawSyntaxK_ = ImaginaryFloatingConstantToken;
}

/**
 * Lex a \a digit-sequence
 *
 * \remark 6.4.4.2
 */
void Lexer::lexDigitSequence()
{
    if (!std::isdigit(yychar_))
        return;

    yyinput();
    while (std::isdigit(yychar_))
        yyinput();
}

/**
 * Lex an \a exponent-part
 *
 * \remark 6.4.4.2
 */
void Lexer::lexExponentPart()
{
    if (yychar_ == 'e' || yychar_ == 'E') {
        yyinput();
        lexSign();
        lexDigitSequence();
    }
}

/**
 * Lex a \a binary-exponent-part
 *
 * \remark 6.4.4.2
 */
void Lexer::lexBinaryExponentPart()
{
    if (yychar_ == 'p' || yychar_ == 'P') {
        yyinput();
        lexSign();
        lexDigitSequence();
    }
}

/**
 * Lex a \a sign
 *
 * \remark 6.4.4.2
 */
void Lexer::lexSign()
{
    if (yychar_ == '+' || yychar_ == '-')
        yyinput();
}

/**
 * Lex a \a floating-suffix
 *
 * \remark 6.4.4.2
 */
void Lexer::lexFloatingSuffix()
{
    if (yychar_ == 'f'
            || yychar_ == 'l'
            || yychar_ == 'F'
            || yychar_ == 'L') {
        yyinput();
    }
}

/**
 * Lex a \a hexadecimal-digit-sequence.
 *
 * \remark 6.4.4.2
 */
void Lexer::lexHexadecimalDigitSequence()
{
    while (std::isdigit(yychar_)
            || (yychar_ >= 'a' && yychar_ <= 'f')
            || (yychar_ >= 'A' && yychar_ <= 'F')) {
        yyinput();
    }
}

/**
 * Lex a \a character-constant.
 *
 * \remark 6.4.4.4
 */
void Lexer::lexCharacterConstant(SyntaxToken* tk, unsigned char prefix)
{
    unsigned int prefixSize = 1;
    if (prefix == 'L')
        tk->rawSyntaxK_ = CharacterConstant_L_Token;
    else if (prefix == 'U')
        tk->rawSyntaxK_ = CharacterConstant_U_Token;
    else if (prefix == 'u')
        tk->rawSyntaxK_ = CharacterConstant_u_Token;
    else {
        tk->rawSyntaxK_ = CharacterConstantToken;
        prefixSize = 0;
    }

    lexUntilQuote(tk, '\'', prefixSize);
}

/**
 * Lex a \a string-literal
 *
 * \remark 6.4.5
 */
void Lexer::lexStringLiteral(SyntaxToken* tk, unsigned char prefix)
{
    unsigned int prefixSize = 1;
    if (prefix == 'L')
        tk->rawSyntaxK_ = StringLiteral_L_Token;
    else if (prefix == 'U')
        tk->rawSyntaxK_ = StringLiteral_U_Token;
    else if (prefix == 'u')
        tk->rawSyntaxK_ = StringLiteral_u_Token;
    else if (prefix == '8')
        tk->rawSyntaxK_ = StringLiteral_u8_Token;
    else {
        tk->rawSyntaxK_ = StringLiteralToken;
        prefixSize = 0;
    }

    lexUntilQuote(tk, '"', prefixSize);
}

void Lexer::lexRawStringLiteral(SyntaxToken* tk, unsigned char prefix)
{
    const char* yytext = yytext_;
    int delimLeng = -1;
    const char* delimCandidate = nullptr;

    while (yychar_) {
        if (yychar_ == '(' && delimLeng == -1) {
            delimLeng = yytext_ - yytext;
            yyinput();
        }
        else if (yychar_ == ')') {
            yyinput();
            if (delimLeng == -1)
                break;
            delimCandidate = yytext_;
        }
        else {
            if (delimLeng == -1) {
                if (yychar_ == '\\' || std::isspace(yychar_))
                    break;
                yyinput();
            }
            else {
                if (!delimCandidate) {
                    yyinput();
                }
                else {
                    if (yychar_ == '"') {
                        if (delimLeng == yytext_ - delimCandidate) {
                            // Got a matching closing delimiter.
                            break;
                        }
                    }

                    // Make sure this continues to be a valid candidate.
                    if (yychar_ != *(yytext + (yytext_ - delimCandidate)))
                        delimCandidate = nullptr;

                    yyinput();
                }
            }
        }
    }

    int yyleng = yytext_ - yytext;

    if (yychar_ == '"')
        yyinput();

    tk->string_ = tree_->stringLiteral(yytext, yyleng);

    if (prefix == 'L')
        tk->rawSyntaxK_ = StringLiteral_LR_Token;
    else if (prefix == 'U')
        tk->rawSyntaxK_ = StringLiteral_UR_Token;
    else if (prefix == 'u')
        tk->rawSyntaxK_ = StringLiteral_uR_Token;
    else if (prefix == '8')
        tk->rawSyntaxK_ = StringLiteral_u8R_Token;
    else
        tk->rawSyntaxK_ = StringLiteral_R_Token;

    if (!yychar_)
        rawSyntaxK_splitTk = tk->rawSyntaxK_;
}

bool Lexer::lexContinuedRawStringLiteral()
{
    bool closeParen = false;

    while (yychar_) {
        if (yychar_ == ')') {
            yyinput();
            closeParen = true;
        }
        else {
            if (closeParen && yychar_ == '"') {
                yyinput();
                return true;
            }
            yyinput();
        }
    }

    return false;
}

void Lexer::lexBackslash(std::uint16_t rawSyntaxK)
{
    yyinput();
    if (yychar_ && !std::isspace(yychar_)) {
        yyinput();
        return;
    }

    while (yychar_ != '\n' && std::isspace(yychar_))
        yyinput();

    if (!yychar_) {
        rawSyntaxK_splitTk = rawSyntaxK;
        withinLogicalLine_ = true;
        return;
    }

    if (yychar_ == '\n') {
        yyinput();
        while (yychar_ != '\n' && std::isspace(yychar_))
            yyinput();

        if (!yychar_)
            rawSyntaxK_splitTk = rawSyntaxK;
    }
}

void Lexer::lexUntilQuote(SyntaxToken* tk, unsigned char quote, unsigned int accLeng)
{
    const char* yytext = yytext_ - 1;
    yytext -= accLeng;

    while (yychar_
               && yychar_ != quote
               && yychar_ != '\n') {
        if (yychar_ == '\\')
            lexBackslash(tk->rawSyntaxK_);
        else
            yyinput();
    }

    int yyleng = yytext_ - yytext + 1;
    yyleng += accLeng;

    if (yychar_ == quote)
        yyinput();

    if (quote == '\'')
        tk->character_ = tree_->characterConstant(yytext, yyleng);
    else
        tk->string_ = tree_->stringLiteral(yytext, yyleng);
}

void Lexer::lexSingleLineComment(std::uint16_t rawSyntaxK)
{
    while (yychar_ && yychar_ != '\n') {
        if (yychar_ == '\\')
            lexBackslash(rawSyntaxK);
        else if (yychar_)
            yyinput();
    }
}
