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

#ifndef PSYCHE_LEXER_H__
#define PSYCHE_LEXER_H__

#include "FrontendConfig.h"
#include "FrontendFwds.h"
#include "Token.h"

namespace psyche {

class CFE_API Lexer
{
    Lexer(const Lexer &other);
    void operator =(const Lexer &other);

public:
    Lexer(TranslationUnit *unit);
    Lexer(const char *firstChar, const char *lastChar);
    ~Lexer();

    Control *control() const { return control_; }

    void lex(Token *tk);

    inline void operator()(Token *tk)
    { lex(tk); }

    Dialect dialect() const { return dialect_; }
    void setDialect(Dialect dialect) { dialect_ = dialect; }

    bool classifyKeywordIdentifiers() const;
    void setClassifyKeywordIdentifiers(bool yes);

    bool keepComments() const;
    void setKeepComments(bool yes);

    bool recognizeAngleBracketStrings() const;
    void setRecognizeAngleBracketStrings(bool yes);

    void setPreprocessorMode(bool yes)
    { f.ppMode_ = yes; }

    void setStartWithNewline(bool enabled);

private:
    static int classify(const char *string, int length, Dialect features);
    static int classifyOperator(const char *string, int length);

    void setSource(const char *firstChar, const char *lastChar);

    int state() const;
    void setState(int state);

    void pushLineStartOffset();

    void lex_helper(Token *tk);
    void lexStringLiteral(Token *tk, unsigned char hint = 0);
    void lexRawStringLiteral(Token *tk, unsigned char hint = 0);
    bool lexUntilRawStringLiteralEndSimple();
    void lexCharLiteral(Token *tk, unsigned char hint = 0);
    void lexUntilQuote(Token *tk, const unsigned char quote);
    bool lexDigitSequence();
    bool lexExponentPart();
    bool lexOptionalFloatingSuffix();
    bool lexOptionalIntegerSuffix(bool allowU = true);
    void lexOptionalUserDefinedLiteral(Token *tk);
    void lexNumericLiteral(Token *tk);
    void lexPreprocessorNumber(Token *tk, bool dotAlreadySkipped);
    void lexIdentifier(Token *tk, unsigned extraProcessedChars = 0);
    void lexBackslash(Kind type);
    void lexCppComment(Kind type);

    TranslationUnit *unit_;
    Control *control_;

    struct Flags {
        unsigned keepComments_                 : 1;
        unsigned classifyKeywordsIdentifiers_  : 1;
        unsigned recognizeAngleBracketStrings_ : 1;
        unsigned ppMode_                       : 1;
    };
    union {
        unsigned flags_;
        Flags f;
    };

    struct State {
        unsigned char tokKind_ : 7;
        unsigned char newLineExpected_ : 1;
    };
    union {
        unsigned char state_;
        State s;
    };

    const char *firstChar_;
    const char *curChar_;
    const char *lastChar_;
    const char *tokStart_;

    unsigned char yy_;

    void yyinp()
    {
        yyinp_utf8(curChar_, yy_, _currentCharUtf16);
        if (UNLIKELY(yy_ == '\n'))
            pushLineStartOffset();
    }

    unsigned curLine_;
    Dialect dialect_;

    static void yyinp_utf8(const char *&currentSourceChar, unsigned char &yychar,
                           unsigned &utf16charCounter)
    {
        ++utf16charCounter;

        // Process multi-byte UTF-8 code point (non-latin1)
        if (UNLIKELY(isByteOfMultiByteCodePoint(yychar))) {
            unsigned trailingBytesCurrentCodePoint = 1;
            for (unsigned char c = yychar << 2; isByteOfMultiByteCodePoint(c); c <<= 1)
                ++trailingBytesCurrentCodePoint;
            // Code points >= 0x00010000 are represented by two UTF-16 code units
            if (trailingBytesCurrentCodePoint >= 3)
                ++utf16charCounter;
            yychar = *(currentSourceChar += trailingBytesCurrentCodePoint + 1);

            // Process single-byte UTF-8 code point (latin1)
        } else {
            yychar = *++currentSourceChar;
        }
    }

    static bool isByteOfMultiByteCodePoint(unsigned char byte)
    { return byte & 0x80; } // Check if most significant bit is set

    unsigned _currentCharUtf16;
    unsigned _tokenStartUtf16;
};

} // namespace psyche

#endif
