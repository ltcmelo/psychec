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

#ifndef PSYCHE_C_SYNTAX_LEXEME_H__
#define PSYCHE_C_SYNTAX_LEXEME_H__

#include "API.h"
#include "Fwds.h"

#include "../common/text/TextElement.h"

#include <string>

namespace psy {
namespace C {

/**
 * \brief The SyntaxLexeme class.
 */
class PSY_C_API SyntaxLexeme : public TextElement
{
public:
    virtual ~SyntaxLexeme();

    //!@{
    /**
     * Cast \c this SyntaxLexeme.
     */
    virtual Identifier* asIdentifier() { return nullptr; }
    virtual IntegerConstant* asIntegerConstant() { return nullptr; }
    virtual FloatingConstant* asFloatingConstant() { return nullptr; }
    virtual CharacterConstant* asCharacterConstant() { return nullptr; }
    virtual ImaginaryIntegerConstant* asImaginaryIntegerConstant() { return nullptr; }
    virtual ImaginaryFloatingConstant* asImaginaryFloatingConstant() { return nullptr; }
    virtual StringLiteral* asStringLiteralExpression() { return nullptr; }
    //!@}

    /**
     * \brief The SyntaxLexeme::Kind enumeraiton.
     */
    enum class Kind : std::uint16_t
    {
        UNSPECIFIED = 0,

        Identifier,
        IntegerConstant,
        FloatingConstant,
        CharacterConstant,
        ImaginaryIntegerConstant,
        ImaginaryFloatingConstant,
        StringLiteral
    };

    /**
     * The Kind of \c this SyntaxLexeme.
     */
    Kind kind() const;

    /**
     * The value of \c this SyntaxLexeme represented as text, by an \c std::string.
     *
     * \note
     * The return of this method is the same as that of SyntaxToken::valueText,
     * for the SyntaxToken from which \c this SyntaxLexeme was obtained.
     *
     * \sa SyntaxToken::valueText
     */
    std::string valueText() const;

    /**
     * The value of \c this SyntaxLexeme. Template parameter \c ValueT
     * must be substituted, as an argument, by the C type corresponding
     * to the lexeme in question. Therefore, prior to calling
     * SyntaxLexeme::value, one must check the speficic kind of a lexeme,
     * and any variant it might have. Consider the snippet below, which
     * stores in \c i the C value of \c 42UL of a given SyntaxLexeme,
     * named \c lexeme.
     *
     * \code
     * if (lexeme->asIntegerConstant()) {
     *     auto intLexeme = lexeme->asIntegerConstant();
     *     if (intLexeme->signedness() == IntegerConstant::Signedness::Unsigned
     *             && intLexeme->variant() == IntegerConstant::Variant::Long) {
     *         auto i = intLexeme->value<unsigned long>();
     *         // ...
     *     }
     * }
     * \endcode
     *
     * \remark 6.2.5
     * \remark 6.4
     */
    template <class ValueT> ValueT value() const;

protected:
    SyntaxLexeme(const char* chars,
                 unsigned int size,
                 Kind kind);

    // Unavailable
    SyntaxLexeme(const SyntaxLexeme&) = delete;
    SyntaxLexeme& operator=(const SyntaxLexeme&) = delete;

    void checkHexPrefix();
    void checkVariousPrefixesAndSuffixes();

    struct BitFields
    {
        std::uint16_t kind_   : 3;
        std::uint16_t hex_    : 1;
        std::uint16_t l_      : 1;
        std::uint16_t L_      : 1;
        std::uint16_t u_      : 1;
        std::uint16_t u8_     : 1;
        std::uint16_t U_      : 1;
        std::uint16_t llOrLL_ : 1;
        std::uint16_t fOrF_   : 1;
    };
    union
    {
        std::uint16_t BF_all_;
        BitFields BF_;
    };
};

} // C
} // psy

#endif
