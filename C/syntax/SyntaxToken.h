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

#ifndef PSYCHE_C_SYNTAX_TOKEN_H__
#define PSYCHE_C_SYNTAX_TOKEN_H__

#include "API.h"
#include "Fwds.h"

#include "SyntaxKind.h"

#include "parser/LanguageDialect.h"
#include "parser/LanguageExtensions.h"

#include "../common/location/Location.h"
#include "../common/text/TextSpan.h"

#include <cstddef>
#include <cstdint>

namespace psy {
namespace C {

/**
 * \brief The SyntaxToken class.
 *
 * \note
 * This API is inspired by that of \c Microsoft.CodeAnalysis.SyntaxToken
 * from Roslyn, the .NET Compiler Platform.
 *
 * \note
 * Influence by the API of Clang/LLVM is present as well; specifically:
 * \c clang::Token and \c clang::Preprocessor.
 */
class PSY_C_API SyntaxToken
{
    friend bool operator==(const SyntaxToken& a, const SyntaxToken& b);

public:
    ~SyntaxToken();

    /**
     * The SyntaxKind of \c this SyntaxToken.
     */
    SyntaxKind kind() const { return SyntaxKind(rawSyntaxK_); }

    /**
     * Whether \c this SyntaxToken is of SyntaxKind \p k.
     */
    bool isKind(SyntaxKind k) { return kind() == k; }

    /**
     * The raw kind of \c this SyntaxToken.
     */
    unsigned int rawKind() const { return rawSyntaxK_; }

    /**
     * Whether \c this SyntaxToken is of the given \p rawKind.
     */
    bool isRawKind(unsigned int rawKind) const { return rawSyntaxK_ == rawKind; }

    /**
     * \brief The existing SyntaxToken categories.
     *
     * \remark 6.4-3
     */
    enum class Category
    {
        Keywords,
        Identifiers,
        Constants,
        StringLiterals,
        Punctuators,
        Unrecognized
    };

    /**
     * The Category of \c this SyntaxToken.
     */
    Category category() const;

    /**
     * The Category of the SyntaxToken of kind \p k.
     */
    static Category category(SyntaxKind k);

    /**
     * The value of \c this SyntaxToken represented by a lexeme, from which
     * the actual value (e.g., an integer such as \c 42) may be obtained.
     *
     * \see SyntaxLexeme::value
     */
    SyntaxLexeme* valueLexeme() const;

    /**
     * The value of \c this SyntaxToken represented as text, by an \c std::string.
     *
     * \sa valueText_c_str
     */
    std::string valueText() const;

    /**
     * The value of \c this SyntaxToken represented as text, by C-style string.
     *
     * \sa valueText
     */
    const char* valueText_c_str() const;

    /**
     * Whether \c this SyntaxToken is at the start of a line.
     */
    bool isAtStartOfLine() const { return BF_.atStartOfLine_; }

    /**
     * Whether \c this SyntaxToken has any leading trivia (e.g., a whitespace).
     */
    bool hasLeadingTrivia() const { return BF_.hasLeadingWS_; }

    /**
     * Whether \c this SyntaxToken is joined with the previous one.
     */
    bool isJoined() const { return BF_.joined_; }

    /**
     * Whether \c this SyntaxToken is the result of a preprocessor expansion.
     *
     * \see SyntaxToken::isPPGenerated
     */
    bool isPPExpanded() const { return BF_.expanded_; }

    /**
     * Whether \c this SyntaxToken is the result of a preprocessor expansion
     * and generated. Consider:
     *
     * \code{.c}
     * #define FOO(a, b) a + b;
     * FOO(1, 2)
     * \endcode
     *
     * After preprocessing, we have:
     *
     * \code
     * 1 + 2;
     * \endcode
     *
     * Tokens \c 1, \c +, \c 2, and \c ; are all preprocessor-expanded; but
     * only \c + and \c ; are both expanded and generated.
     *
     * \see SyntaxToken::isPPExpanded
     */
    bool isPPGenerated() const { return BF_.generated_; }

    /**
     * Whether \c this SyntaxToken is a comment.
     */
    bool isComment() const;

    /**
     * Whether \c this SyntaxToken is missing from the source.
     */
    bool isMissing() const { return BF_.missing_; }

    /**
     * Whether \c this SyntaxToken is valid.
     *
     * \see SyntaxToken::invalid
     */
    bool isValid() const;

    /**
     * The Location of \c this SyntaxToken.
     */
    Location location() const;

    /**
     * The text span of \c this SyntaxToken.
     */
    TextSpan span() const;

    /**
     * An invalid SyntaxToken.
     */
    static SyntaxToken invalid();

PSY_INTERNAL_AND_RESTRICTED:
    PSY_GRANT_ACCESS(SyntaxTree);
    PSY_GRANT_ACCESS(SyntaxTree);
    PSY_GRANT_ACCESS(SyntaxNode);
    PSY_GRANT_ACCESS(Lexer);
    PSY_GRANT_ACCESS(Parser);

    SyntaxToken(SyntaxTree* tree);

    void setup();

    unsigned int byteStart() const { return byteOffset_; }
    unsigned int byteEnd() const { return byteOffset_ + byteSize_; }

    unsigned int charStart() const { return charOffset_; }
    unsigned int charEnd() const { return charOffset_ + charSize_; }

    SyntaxTree* tree_;

    /*
     * Watch for data layout (size) before changing members or their order.
     */

    std::uint16_t rawSyntaxK_;  // Keep the same underlying type of SyntaxKind.
    std::uint16_t byteSize_;
    std::uint16_t charSize_;
    std::uint32_t byteOffset_;
    std::uint32_t charOffset_;  // UTF-16
    std::size_t matchingBracket_;

    struct BitFields
    {
        std::uint16_t atStartOfLine_ : 1;
        std::uint16_t hasLeadingWS_  : 1;
        std::uint16_t joined_        : 1;
        std::uint16_t expanded_      : 1;
        std::uint16_t generated_     : 1;
        std::uint16_t missing_       : 1;
    };
    union
    {
        std::uint16_t BF_all_;
        BitFields BF_;
    };

    unsigned int lineno_;
    unsigned int column_;

    union
    {
        SyntaxLexeme* lexeme_;
        const Identifier* identifier_;
        const IntegerConstant* integer_;
        const FloatingConstant* floating_;
        const CharacterConstant* character_;
        const ImaginaryIntegerConstant* imaginaryInteger_;
        const ImaginaryFloatingConstant* imaginaryFloating_;
        const StringLiteral* string_;
    };
};

/**
 * The SyntaxKind \p kind as a \c std::string.
 */
std::string PSY_C_API to_string(SyntaxKind kind); // Definition in SyntaxNode.cpp

/**
 * The SyntaxToken::Category \p category as a \c std::string.
 */
std::string PSY_C_API to_string(SyntaxToken::Category category);

bool operator==(const SyntaxToken& a, const SyntaxToken& b);
bool operator!=(const SyntaxToken& a, const SyntaxToken& b);

} // C
} // psy

#endif
