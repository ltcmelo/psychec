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

#ifndef PSYCHE_C_LEXER_H__
#define PSYCHE_C_LEXER_H__

#include "API.h"
#include "Fwds.h"

#include "syntax/SyntaxToken.h"

#include "../common/infra/InternalAccess.h"

#include <cstdint>
#include <functional>
#include <string>

namespace psy {
namespace C {

class ParseOptions;

/**
 * \brief The C Lexer class.
 */
class PSY_C_NON_API Lexer
{
public:
    ~Lexer();

    void lex();

PSY_INTERNAL_AND_RESTRICTED:
    PSY_GRANT_ACCESS(SyntaxTree);

    Lexer(SyntaxTree* tree);

private:
    // Unavailable
    Lexer(const Lexer&) = delete;
    void operator=(const Lexer&) = delete;

    void yylex(SyntaxToken* tk);
    void yylex_core(SyntaxToken* tk);
    void yyinput();
    void yyinput_core(const char*& yy,
                      unsigned char& yychar,
                      unsigned int& yycolumn,
                      unsigned int& offset);

    /* 6.4.2 Identifiers */
    void lexIdentifier(SyntaxToken* tk, int advanced = 0);

    /* 6.4.4 Constants */
    void lexCharacterConstant(SyntaxToken* tk, unsigned char prefix = 0);

    void lexIntegerOrFloatingConstant(SyntaxToken* tk);
    void lexIntegerOrFloating_AtFollowOfSuffix(SyntaxToken* tk, std::function<void ()>);

    void lexIntegerOrImaginaryIntegerSuffix(SyntaxToken* tk, unsigned int accLeng);
    void lexIntegerSuffix(int suffixCnt = 2);
    void lexImaginaryIntegerSuffix(SyntaxToken* tk);
    void lexImaginaryIntegerSuffix_AtFirst(SyntaxToken* tk);

    void lexFloatingOrImaginaryFloating_AtFollowOfPeriod(SyntaxToken* tk, unsigned int accLeng);
    void lexFloatingOrImaginaryFloating_AtExponent(SyntaxToken* tk, unsigned int accLeng);
    void lexFloatingOrImaginaryFloatingSuffix(SyntaxToken* tk, unsigned int accLeng);
    void lexFloatingSuffix();
    void lexImaginaryFloatingSuffix(SyntaxToken* tk);
    void lexImaginaryFloatingSuffix_AtFirst(SyntaxToken* tk);

    void lexDigitSequence();
    void lexHexadecimalDigitSequence();
    void lexExponentPart();
    void lexBinaryExponentPart();
    void lexSign();

    /* 6.4.5 String literals */
    void lexStringLiteral(SyntaxToken* tk, unsigned char prefix = 0);
    void lexRawStringLiteral(SyntaxToken* tk, unsigned char prefix = 0);
    bool lexContinuedRawStringLiteral();

    void lexUntilQuote(SyntaxToken* tk, unsigned char quote, unsigned int accLeng);
    void lexBackslash(std::uint16_t rawSyntaxK);
    void lexSingleLineComment(std::uint16_t rawSyntaxK);

    static SyntaxKind classify(const char* ident,
                               int size,
                               const ParseOptions& options);
    static SyntaxKind classifyOperator(const char* indent,
                                       int size,
                                       const ParseOptions& options);

    SyntaxTree* tree_;
    std::string text_;
    const char* c_strBeg_;
    const char* c_strEnd_;

    const char* yytext_;
    const char* yy_;
    unsigned char yychar_;
    unsigned int yylineno_;
    unsigned int yycolumn_;

    unsigned int offset_;
    unsigned int offsetMarker_;

    // Line breaks and continuations aren't strictly correct... (see quirks
    // at https://gcc.gnu.org/onlinedocs/cppinternals/Lexer.html).
    bool withinLogicalLine_;
    std::uint16_t rawSyntaxK_splitTk;

    struct DiagnosticsReporter
    {
        DiagnosticsReporter(Lexer* lexer) : lexer_(lexer) {}
        Lexer* lexer_;

        void IncompatibleLanguageDialect(const std::string& feature, LanguageDialect::Std expectedStd);
        void IncompatibleLanguageExtension(const std::string& feature, LanguageExtensions::Ext expectedExt);

        static const std::string ID_of_IncompatibleLanguageDialect;
        static const std::string ID_of_IncompatibleLanguageExtension;
    };
    friend struct DiagnosticsReporter;

    DiagnosticsReporter diagReporter_;
};

} // C
} // psy

#endif
