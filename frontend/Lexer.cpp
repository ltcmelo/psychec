// Copyright (c) 2008 Roberto Raggi <roberto.raggi@gmail.com>
// Copyright (c) 2016-20 Leandro T. C. Melo <ltcmelo@gmail.com>
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
#include "Control.h"
#include "TranslationUnit.h"
#include "Literals.h"

#include <cctype>
#include <iostream>

using namespace psyche;

/*!
    \fn static void Lexer::yyinp_utf8(const char *&currentSourceChar, unsigned char &yychar, unsigned &utf16charCounter)

    Process a single unicode code point in an UTF-8 encoded source.

    \a currentSourceChar points to the UTF-8 encoded source.
    \a yychar must be the byte pointed to by \a currentSourceChar.

    Points \a currentSourceChar to the byte of the next code point
    and modifies \a yychar to the value pointed by the updated
    \a currentSourceChar. \a utf16charCounter will be incremented by
    the number of UTF-16 code units that were needed for that code
    point.
*/

Lexer::Lexer(TranslationUnit *unit)
    : unit_(unit)
    , control_(unit->control())
    , flags_(0)
    , state_(0)
    , curLine_(1)
{
    f.classifyKeywordsIdentifiers_ = true;
    setSource(unit_->firstSourceChar(),
              unit_->lastSourceChar());
}

Lexer::Lexer(const char *firstChar, const char *lastChar)
    : unit_(0)
    , control_(0)
    , flags_(0)
    , state_(0)
    , curLine_(1)
{
    f.classifyKeywordsIdentifiers_ = true;
    setSource(firstChar, lastChar);
}

Lexer::~Lexer()
{ }

void Lexer::setSource(const char *firstChar, const char *lastChar)
{
    firstChar_ = firstChar;
    lastChar_ = lastChar;
    curChar_ = firstChar_ - 1;
    _currentCharUtf16 = ~0;
    tokStart_ = curChar_;
    yy_ = '\n';
}

void Lexer::setStartWithNewline(bool enabled)
{
    if (enabled)
        yy_ = '\n';
    else
        yy_ = ' ';
}

int Lexer::state() const
{ return state_; }

void Lexer::setState(int state)
{ state_ = state; }

bool Lexer::keepComments() const
{ return f.keepComments_; }

void Lexer::setKeepComments(bool yes)
{ f.keepComments_ = yes; }

bool Lexer::classifyKeywordIdentifiers() const
{ return f.classifyKeywordsIdentifiers_; }

void Lexer::setClassifyKeywordIdentifiers(bool yes)
{ f.classifyKeywordsIdentifiers_ = yes; }

void Lexer::setRecognizeAngleBracketStrings(bool yes)
{ f.recognizeAngleBracketStrings_ = yes; }

void Lexer::pushLineStartOffset()
{
    ++curLine_;

    if (unit_)
        unit_->pushLineOffset(_currentCharUtf16);
}

void Lexer::lex(Token *tk)
{
    tk->reset();
    lex_helper(tk);
    tk->f.bytes = curChar_ - tokStart_;
    tk->f.utf16chars = _currentCharUtf16 - _tokenStartUtf16;
}

static bool isRawStringLiteral(unsigned char kind)
{
    return kind >= T_FIRST_RAW_STRING_LITERAL
        && kind <= T_LAST_RAW_STRING_LITERAL;
}

static bool isMultiLineToken(unsigned char kind)
{
    return kind == T_EOF_SYMBOL
        || kind == T_COMMENT
        || kind == T_DOXY_COMMENT
        || kind == T_PSYCHEC_OMISSION_MARKER
        || isRawStringLiteral(kind);
}

void Lexer::lex_helper(Token *tk)
{
  again:
    while (yy_ && std::isspace(yy_)) {
        if (yy_ == '\n') {
            tk->f.joined = s.newLineExpected_;
            tk->f.newline = !s.newLineExpected_;

            if (s.newLineExpected_)
                s.newLineExpected_ = false;
            else if (!isMultiLineToken(s.tokKind_))
                state_ = 0;
        } else {
            tk->f.whitespace = true;
        }
        yyinp();
    }

    if (! unit_)
        tk->lineno = curLine_;

    tokStart_ = curChar_;
    tk->byteOffset = curChar_ - firstChar_;

    _tokenStartUtf16 = _currentCharUtf16;
    tk->utf16charOffset = _currentCharUtf16;

    if (yy_) {
        s.newLineExpected_ = false;
    } else if (s.tokKind_) {
        tk->f.kind = T_EOF_SYMBOL;
        return;
    }

    if (s.tokKind_ == T_COMMENT
            || s.tokKind_ == T_DOXY_COMMENT
            || s.tokKind_ == T_PSYCHEC_OMISSION_MARKER) {
        const int tkKind = s.tokKind_;
        while (yy_) {
            if (yy_ != '*')
                yyinp();
            else {
                yyinp();
                if (yy_ == '/') {
                    yyinp();
                    state_ = 0;
                    break;
                }
            }
        }

        if (! f.keepComments_)
            goto again;

        tk->f.kind = tkKind;
        return;
    }

    if (s.tokKind_ == T_CPP_COMMENT
            || s.tokKind_ == T_CPP_DOXY_COMMENT) {
        const Kind tkKind = (Kind)s.tokKind_;
        tk->f.joined = true;
        if (f.keepComments_)
            tk->f.kind = tkKind;
        state_ = 0;
        lexCppComment(tkKind);
        return;
    }

    if (isRawStringLiteral(s.tokKind_)) {
        tk->f.kind = s.tokKind_;
        if (lexUntilRawStringLiteralEndSimple())
            state_ = 0;
        return;
    }

    if (s.tokKind_ != T_EOF_SYMBOL) {
        tk->f.joined = true;
        tk->f.kind = s.tokKind_;
        state_ = 0;
        lexUntilQuote(tk, '"');
        return;
    }

    if (!yy_) {
        tk->f.kind = T_EOF_SYMBOL;
        return;
    }

    unsigned char ch = yy_;
    yyinp();

    switch (ch) {
    case '\\':
        s.newLineExpected_ = true;
        goto again;

    case '"':
        lexStringLiteral(tk);
        break;

    case '\'':
        lexCharLiteral(tk);
        break;

    case '{':
        tk->f.kind = T_LBRACE;
        break;

    case '}':
        tk->f.kind = T_RBRACE;
        break;

    case '[':
        tk->f.kind = T_LBRACKET;
        break;

    case ']':
        tk->f.kind = T_RBRACKET;
        break;

    case '#':
        if (yy_ == '#') {
            tk->f.kind = T_POUND_POUND;
            yyinp();
        } else {
            tk->f.kind = T_POUND;
        }
        break;

    case '(':
        tk->f.kind = T_LPAREN;
        break;

    case ')':
        tk->f.kind = T_RPAREN;
        break;

    case ';':
        tk->f.kind = T_SEMICOLON;
        break;

    case ':':
        if (yy_ == ':') {
            yyinp();
            tk->f.kind = T_COLON_COLON;
        } else if (yy_ == '>') {
            yyinp();
            tk->f.kind = T_RBRACKET;
        } else {
            tk->f.kind = T_COLON;
        }
        break;

    case '.':
        if (yy_ == '*') {
            yyinp();
            tk->f.kind = T_DOT_STAR;
        } else if (yy_ == '.') {
            yyinp();
            // ### CPP_CHECK(_yychar);
            if (yy_ == '.') {
                yyinp();
                tk->f.kind = T_DOT_DOT_DOT;
            } else {
                tk->f.kind = T_ERROR;
            }
        } else if (std::isdigit(yy_)) {
            if (f.ppMode_) {
                lexPreprocessorNumber(tk, true);
                break;
            }

            const char *yytext = curChar_ - 2;
            yyinp();
            lexDigitSequence(); // this is optional: we already skipped over the first digit
            lexExponentPart();
            if (!lexOptionalFloatingSuffix())
                lexOptionalUserDefinedLiteral(tk);
            if (std::isalnum(yy_) || yy_ == '_') {
                do {
                    yyinp();
                } while (std::isalnum(yy_) || yy_ == '_');
                tk->f.kind = T_ERROR;
            } else {
                int yylen = curChar_ - yytext;
                tk->f.kind = T_NUMERIC_LITERAL;
                if (control())
                    tk->number = control()->numericLiteral(yytext, yylen);
            }
        } else {
            tk->f.kind = T_DOT;
        }
        break;

    case '?':
        if (yy_ == '?') {
            yyinp();
            if (yy_ == '(') {
                yyinp();
                tk->f.kind = T_LBRACKET;
            } else if (yy_ == ')') {
                yyinp();
                tk->f.kind = T_RBRACKET;
            } else if (yy_ == '<') {
                yyinp();
                tk->f.kind = T_LBRACE;
            } else if (yy_ == '>') {
                yyinp();
                tk->f.kind = T_RBRACE;
            }
        } else {
            tk->f.kind = T_QUESTION;
        }
        break;

    case '+':
        if (yy_ == '+') {
            yyinp();
            tk->f.kind = T_PLUS_PLUS;
        } else if (yy_ == '=') {
            yyinp();
            tk->f.kind = T_PLUS_EQUAL;
        } else {
            tk->f.kind = T_PLUS;
        }
        break;

    case '-':
        if (yy_ == '-') {
            yyinp();
            tk->f.kind = T_MINUS_MINUS;
        } else if (yy_ == '=') {
            yyinp();
            tk->f.kind = T_MINUS_EQUAL;
        } else if (yy_ == '>') {
            yyinp();
            if (yy_ == '*') {
                yyinp();
                tk->f.kind = T_ARROW_STAR;
            } else {
                tk->f.kind = T_ARROW;
            }
        } else {
            tk->f.kind = T_MINUS;
        }
        break;

    case '*':
        if (yy_ == '=') {
            yyinp();
            tk->f.kind = T_STAR_EQUAL;
        } else {
            tk->f.kind = T_STAR;
        }
        break;

    case '/':
        if (yy_ == '/') {
            yyinp();
            Kind tokKind = T_CPP_COMMENT;

            if (yy_ == '/' || yy_ == '!') {
                yyinp();
                tokKind = T_CPP_DOXY_COMMENT;
            }

            lexCppComment(tokKind);

            if (! f.keepComments_)
                goto again;

            tk->f.kind = tokKind;
        } else if (yy_ == '*') {
            yyinp();
            Kind tokKind = T_COMMENT;

            if (yy_ == '*' || yy_ == '!') {
                const char ch = yy_;
                yyinp();
                if (ch == '*' && yy_ == '/')
                    goto done;

                if (yy_ == '<')
                    yyinp();
                if (! yy_ || std::isspace(yy_))
                    tokKind = T_DOXY_COMMENT;
            } else if (yy_ == '.') {
                do {
                    yyinp();
                } while (yy_ == '.');
                tokKind = T_PSYCHEC_OMISSION_MARKER;
            }

            while (yy_) {
                if (yy_ != '*') {
                    yyinp();
                } else {
                    yyinp();
                    if (yy_ == '/')
                        break;
                }
            }

        done:
            if (yy_)
                yyinp();
            else
                s.tokKind_ = tokKind;

            if (! f.keepComments_)
                goto again;

            tk->f.kind = tokKind;
        } else if (yy_ == '=') {
            yyinp();
            tk->f.kind = T_SLASH_EQUAL;
        } else {
            tk->f.kind = T_SLASH;
        }
        break;

    case '%':
        if (yy_ == '=') {
            yyinp();
            tk->f.kind = T_PERCENT_EQUAL;
        } else if (yy_ == '>') {
            yyinp();
            tk->f.kind = T_RBRACE;
        } else if (yy_ == ':') {
            yyinp();
            tk->f.kind = T_POUND;
        } else {
            tk->f.kind = T_PERCENT;
        }
        break;

    case '^':
        if (yy_ == '=') {
            yyinp();
            tk->f.kind = T_CARET_EQUAL;
        } else {
            tk->f.kind = T_CARET;
        }
        break;

    case '&':
        if (yy_ == '&') {
            yyinp();
            tk->f.kind = T_AMPER_AMPER;
        } else if (yy_ == '=') {
            yyinp();
            tk->f.kind = T_AMPER_EQUAL;
        } else {
            tk->f.kind = T_AMPER;
        }
        break;

    case '|':
        if (yy_ == '|') {
            yyinp();
            tk->f.kind = T_PIPE_PIPE;
        } else if (yy_ == '=') {
            yyinp();
            tk->f.kind = T_PIPE_EQUAL;
        } else {
            tk->f.kind = T_PIPE;
        }
        break;

    case '~':
        if (yy_ == '=') {
            yyinp();
            tk->f.kind = T_TILDE_EQUAL;
        } else {
            tk->f.kind = T_TILDE;
        }
        break;

    case '!':
        if (yy_ == '=') {
            yyinp();
            tk->f.kind = T_EXCLAIM_EQUAL;
        } else {
            tk->f.kind = T_EXCLAIM;
        }
        break;

    case '=':
        if (yy_ == '=') {
            yyinp();
            tk->f.kind = T_EQUAL_EQUAL;
        } else {
            tk->f.kind = T_EQUAL;
        }
        break;

    case '<':
        if (f.recognizeAngleBracketStrings_) {
            const char *yytext = curChar_;
            while (yy_ && yy_ != '>')
                yyinp();
            int yylen = curChar_ - yytext;
            // ### CPP_CHECK(_yychar == '>');
            if (yy_ == '>')
                yyinp();
            if (control())
                tk->string = control()->stringLiteral(yytext, yylen);
            tk->f.kind = T_ANGLE_STRING_LITERAL;
        } else if (yy_ == '<') {
            yyinp();
            if (yy_ == '=') {
                yyinp();
                tk->f.kind = T_LESS_LESS_EQUAL;
            } else
                tk->f.kind = T_LESS_LESS;
        } else if (yy_ == '=') {
            yyinp();
            tk->f.kind = T_LESS_EQUAL;
        } else if (yy_ == ':') {
            yyinp();
            tk->f.kind = T_LBRACKET;
        } else if (yy_ == '%') {
            yyinp();
            tk->f.kind = T_LBRACE;
        } else {
            tk->f.kind = T_LESS;
        }
        break;

    case '>':
        if (yy_ == '>') {
            yyinp();
            if (yy_ == '=') {
                yyinp();
                tk->f.kind = T_GREATER_GREATER_EQUAL;
            } else
                tk->f.kind = T_LESS_LESS;
            tk->f.kind = T_GREATER_GREATER;
        } else if (yy_ == '=') {
            yyinp();
            tk->f.kind = T_GREATER_EQUAL;
        } else {
            tk->f.kind = T_GREATER;
        }
        break;

    case ',':
        tk->f.kind = T_COMMA;
        break;

    default: {
        if (ch == 'L' || ch == 'u' || ch == 'U' || ch == 'R') {
            // Either a literal or still an identifier.
            if (yy_ == '"') {
                yyinp();
                if (ch == 'R')
                    lexRawStringLiteral(tk);
                else
                    lexStringLiteral(tk, ch);
            } else if (yy_ == '\'') {
                yyinp();
                lexCharLiteral(tk, ch);
            } else if (ch != 'R' && yy_ == 'R') {
                yyinp();
                if (yy_ == '"') {
                    yyinp();
                    lexRawStringLiteral(tk, ch);
                } else {
                    lexIdentifier(tk, 1);
                }
            } else if (ch == 'u' && yy_ == '8') {
                yyinp();
                if (yy_ == '"') {
                    yyinp();
                    lexStringLiteral(tk, '8');
                } else if (yy_ == '\'') {
                    yyinp();
                    lexCharLiteral(tk, '8');
                } else if (yy_ == 'R') {
                    yyinp();
                    if (yy_ == '"') {
                        yyinp();
                        lexRawStringLiteral(tk, '8');
                    } else {
                        lexIdentifier(tk, 2);
                    }
                } else {
                    lexIdentifier(tk, 1);
                }
            } else {
                lexIdentifier(tk);
            }
        } else if (std::isalpha(ch) || ch == '_' || ch == '$' || isByteOfMultiByteCodePoint(ch)) {
            lexIdentifier(tk, curChar_ - tokStart_ - 1);
        } else if (std::isdigit(ch)) {
            if (f.ppMode_)
                lexPreprocessorNumber(tk, false);
            else
                lexNumericLiteral(tk);
        } else {
            tk->f.kind = T_ERROR;
        }
        break;
    } // default

    } // switch
}

void Lexer::lexStringLiteral(Token *tk, unsigned char hint)
{
    if (hint == 'L')
        tk->f.kind = T_WIDE_STRING_LITERAL;
    else if (hint == 'U')
        tk->f.kind = T_UTF32_STRING_LITERAL;
    else if (hint == 'u')
        tk->f.kind = T_UTF16_STRING_LITERAL;
    else if (hint == '8')
        tk->f.kind = T_UTF8_STRING_LITERAL;
    else if (hint == '@')
        tk->f.kind = T_AT_STRING_LITERAL;
    else
        tk->f.kind = T_STRING_LITERAL;

    lexUntilQuote(tk, '"');
    lexOptionalUserDefinedLiteral(tk);
}

void Lexer::lexRawStringLiteral(Token *tk, unsigned char hint)
{
    const char *yytext = curChar_;

    int delimLength = -1;
    const char *closingDelimCandidate = 0;
    while (yy_) {
        if (yy_ == '(' && delimLength == -1) {
            delimLength = curChar_ - yytext;
            yyinp();
        } else if (yy_ == ')') {
            yyinp();
            if (delimLength == -1)
                break;
            closingDelimCandidate = curChar_;
        } else {
            if (delimLength == -1) {
                if (yy_ == '\\' || std::isspace(yy_))
                    break;
                yyinp();
            } else {
                if (!closingDelimCandidate) {
                    yyinp();
                } else {
                    if (yy_ == '"') {
                        if (delimLength == curChar_ - closingDelimCandidate) {
                            // Got a matching closing delimiter.
                            break;
                        }
                    }

                    // Make sure this continues to be a valid candidate.
                    if (yy_ != *(yytext + (curChar_ - closingDelimCandidate)))
                        closingDelimCandidate = 0;

                    yyinp();
                }
            }
        }
    }

    int yylen = curChar_ - yytext;

    if (yy_ == '"')
        yyinp();

    if (control())
        tk->string = control()->stringLiteral(yytext, yylen);

    if (hint == 'L')
        tk->f.kind = T_RAW_WIDE_STRING_LITERAL;
    else if (hint == 'U')
        tk->f.kind = T_RAW_UTF32_STRING_LITERAL;
    else if (hint == 'u')
        tk->f.kind = T_RAW_UTF16_STRING_LITERAL;
    else if (hint == '8')
        tk->f.kind = T_RAW_UTF8_STRING_LITERAL;
    else
        tk->f.kind = T_RAW_STRING_LITERAL;

    if (!yy_)
        s.tokKind_ = tk->f.kind;
}

// In the highlighting case we don't have any further information
// like the delimiter or its length, so just match for: ...)..."
bool Lexer::lexUntilRawStringLiteralEndSimple()
{
    bool closingParenthesisPassed = false;

    while (yy_) {
        if (yy_ == ')') {
            yyinp();
            closingParenthesisPassed = true;
        } else {
            if (closingParenthesisPassed && yy_ == '"') {
                yyinp();
                return true;
            } else {
                yyinp();
            }
        }
    }

    return false;
}

void Lexer::lexCharLiteral(Token *tk, unsigned char hint)
{
    if (hint == 'L')
        tk->f.kind = T_WIDE_CHAR_LITERAL;
    else if (hint == 'U')
        tk->f.kind = T_UTF32_CHAR_LITERAL;
    else if (hint == 'u')
        tk->f.kind = T_UTF16_CHAR_LITERAL;
    else
        tk->f.kind = T_CHAR_LITERAL;

    lexUntilQuote(tk, '\'');
    lexOptionalUserDefinedLiteral(tk);
}

void Lexer::lexUntilQuote(Token *tk, const unsigned char quote)
{
    const char *yytext = curChar_;
    while (yy_
           && yy_ != quote
           && yy_ != '\n') {
        if (yy_ == '\\')
            lexBackslash((Kind)tk->f.kind);
        else
            yyinp();
    }
    int yylen = curChar_ - yytext;

    if (yy_ == quote)
        yyinp();

    if (control()) {
        if (quote == '\'')
            tk->number = control()->numericLiteral(yytext, yylen);
        else
            tk->string = control()->stringLiteral(yytext, yylen);
    }
}

bool Lexer::lexDigitSequence()
{
    if (!std::isdigit(yy_))
        return false;
    yyinp();
    while (std::isdigit(yy_))
        yyinp();
    return true;
}

bool Lexer::lexExponentPart()
{
    if (yy_ != 'e' && yy_ != 'E')
        return false;
    yyinp();
    if (yy_ == '+' || yy_ == '-')
        yyinp();
    return lexDigitSequence();
}

bool Lexer::lexOptionalFloatingSuffix()
{
    if (yy_ == 'f' || yy_ == 'l' || yy_ == 'F' || yy_ == 'L') {
        yyinp();
        return true;
    }
    return false;
}

bool Lexer::lexOptionalIntegerSuffix(bool allowU)
{
    switch(yy_) {
    case 'u':
    case 'U':
        if (allowU) {
            yyinp();
            lexOptionalIntegerSuffix(false);
        }
        return true;
    case 'l':
        yyinp();
        if (yy_ == 'l')
            yyinp();
        return true;
    case 'L':
        yyinp();
        if (yy_ == 'L')
            yyinp();
        return true;
    default:
        return false;
    }
}

void Lexer::lexOptionalUserDefinedLiteral(Token *tk)
{
    if (dialect_.cpp11 && yy_ == '_') {
        tk->f.userDefinedLiteral = true;
        while (std::isalnum(yy_) || yy_ == '_' || isByteOfMultiByteCodePoint(yy_))
            yyinp();
    }
}

void Lexer::lexNumericLiteral(Token *tk)
{
    const char *yytext = curChar_ - 1;
    if (*yytext == '0' && yy_) {
        if (yy_ == 'x' || yy_ == 'X') {
            yyinp();
            while (std::isdigit(yy_) ||
                   (yy_ >= 'a' && yy_ <= 'f') ||
                   (yy_ >= 'A' && yy_ <= 'F')) {
                yyinp();
            }
            if (!lexOptionalIntegerSuffix())
                lexOptionalUserDefinedLiteral(tk);
            goto theEnd;
        } else if (yy_ == 'b' || yy_ == 'B') { // see n3472
            yyinp();
            while (yy_ == '0' || yy_ == '1')
                yyinp();
            if (!lexOptionalIntegerSuffix())
                lexOptionalUserDefinedLiteral(tk);
            goto theEnd;
        } else if (yy_ >= '0' && yy_ <= '7') {
            do {
                yyinp();
            } while (yy_ >= '0' && yy_ <= '7');
            if (!lexOptionalIntegerSuffix())
                lexOptionalUserDefinedLiteral(tk);
            goto theEnd;
        }
    }

    while (yy_) {
        if (yy_ == '.') {
            yyinp();
            lexDigitSequence(); // this is optional: "1." is a valid floating point number
            lexExponentPart();
            if (!lexOptionalFloatingSuffix())
                lexOptionalUserDefinedLiteral(tk);
            break;
        } else if (yy_ == 'e' || yy_ == 'E') {
            if (lexExponentPart() && !lexOptionalFloatingSuffix())
                lexOptionalUserDefinedLiteral(tk);
            break;
        } else if (std::isdigit(yy_)) {
            yyinp();
        } else {
            if (!lexOptionalIntegerSuffix())
                lexOptionalUserDefinedLiteral(tk);
            break;
        }
    }

theEnd:
    if (std::isalnum(yy_) || yy_ == '_') {
        do {
            yyinp();
        } while (std::isalnum(yy_) || yy_ == '_');
        tk->f.kind = T_ERROR;
    } else {
        int yylen = curChar_ - yytext;
        tk->f.kind = T_NUMERIC_LITERAL;
        if (control())
            tk->number = control()->numericLiteral(yytext, yylen);
    }
}

void Lexer::lexPreprocessorNumber(Token *tk, bool dotAlreadySkipped)
{
    const char *yytext = curChar_ - (dotAlreadySkipped ? 2 : 1);
    if (dotAlreadySkipped &&
            (!yy_ || (yy_ && !std::isdigit(yy_)))) {
        tk->f.kind = T_DOT;
        return;
    }

    while (yy_) {
        if (yy_ == 'e' || yy_ == 'E') {
            yyinp();
            if (yy_ == '+' || yy_ == '-')
                yyinp();
        } else if (std::isalnum(yy_) || yy_ == '_' || yy_ == '.') {
            yyinp();
        } else {
            lexOptionalUserDefinedLiteral(tk);
            break;
        }
    }

    int yylen = curChar_ - yytext;
    tk->f.kind = T_NUMERIC_LITERAL;
    if (control())
        tk->number = control()->numericLiteral(yytext, yylen);
}

void Lexer::lexIdentifier(Token *tk, unsigned extraProcessedChars)
{
    const char *yytext = curChar_ - 1 - extraProcessedChars;
    while (std::isalnum(yy_) || yy_ == '_' || yy_ == '$'
            || isByteOfMultiByteCodePoint(yy_)) {
        yyinp();
    }
    int yylen = curChar_ - yytext;
    if (f.classifyKeywordsIdentifiers_)
        tk->f.kind = classify(yytext, yylen, dialect_);
    else
        tk->f.kind = T_IDENTIFIER;

    if (tk->f.kind == T_IDENTIFIER) {
        tk->f.kind = classifyOperator(yytext, yylen);

        if (control())
            tk->identifier = control()->identifier(yytext, yylen);
    }
}

void Lexer::lexBackslash(Kind type)
{
    yyinp(); // skip '\\'
    if (yy_ && !std::isspace(yy_)) {
        yyinp();
        return;
    }
    while (yy_ != '\n' && std::isspace(yy_))
        yyinp();
    if (!yy_) {
        s.tokKind_ = type;
        s.newLineExpected_ = true;
        return;
    }
    if (yy_ == '\n') {
        yyinp();
        while (yy_ != '\n' && std::isspace(yy_))
            yyinp();
        if (!yy_)
            s.tokKind_ = type;
    }
}

void Lexer::lexCppComment(Kind type)
{
    while (yy_ && yy_ != '\n') {
        if (yy_ == '\\')
            lexBackslash(type);
        else if (yy_)
            yyinp();
    }
}
