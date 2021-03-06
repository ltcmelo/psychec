// Copyright (c) 2016/17/18/19/20/21 Leandro T. C. Melo <ltcmelo@gmail.com>
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

#ifndef PSYCHE_C_SYNTAX_LEXEMES_H__
#define PSYCHE_C_SYNTAX_LEXEMES_H__

#include "SyntaxLexeme.h"

#include <cstdint>
#include <string>

namespace psy {
namespace C {

//---------------------//
// Identifier/keywords //
//---------------------//

/**
 * \brief The Identifier class.
 */
class PSY_C_API Identifier final : public SyntaxLexeme
{
public:
    Identifier(const char* chars, unsigned int size);

    virtual Identifier* asIdentifier() override { return this; }
};

//-----------//
// Constants //
//-----------//

/**
 * \brief The Constant class.
 *
 * The base class of every constant.
 */
class PSY_C_API Constant : public SyntaxLexeme
{
protected:
    Constant(const char* chars,
             unsigned int size,
             Kind kind);
};

/**
 * \brief The IntegerConstant class.
 */
class PSY_C_API IntegerConstant final : public Constant
{
public:
    IntegerConstant(const char* chars, unsigned int size);

    virtual IntegerConstant* asIntegerConstant() override { return this; }

    /**
     * \brief The IntegerConstant::Signedness enumeration.
     */
    enum class Signedness : std::uint8_t
    {
        Signed,
        Unsigned,
    };

    /**
     * The Signedness of \c this IntegerConstant.
     */
    Signedness signedness() const;

    /**
     * \brief The IntegerConstant::Variant enumeration.
     */
    enum class Variant : std::uint8_t
    {
        Int,
        Long,
        LongLong
    };

    /**
     * The Variant of \c this IntegerConstant.
     */
    Variant variant() const;
};

/**
 * \brief The FloatingConstant class.
 */
class PSY_C_API FloatingConstant final : public Constant
{
public:
    FloatingConstant(const char* chars, unsigned int size);

    virtual FloatingConstant* asFloatingConstant()  override { return this; }

    /**
     * \brief The FloatingConstant::Variant enumeration.
     */
    enum class Variant : std::uint8_t
    {
        Float,
        Double,
        LongDouble
    };

    /**
     * The Variant of \c this FloatingConstant.
     */
    Variant variant() const;
};


/**
 * \brief The CharacterConstant class.
 */
class PSY_C_API CharacterConstant final : public Constant
{
public:
    CharacterConstant(const char* chars, unsigned int size);

    virtual CharacterConstant* asCharacterConstant()  override { return this; }

    /**
     * \brief The CharacterConstant::Variant enumeration.
     */
    enum class Variant : std::uint8_t
    {
        Plain_u_char,
        L_wchar_t,
        u_char16_t,
        U_char32_t
    };

    /**
     * The Variant of \c this CharacterConstant.
     */
    Variant variant() const;
};

//----------------//
// String literal //
//----------------//

/**
 * \brief The StringLiteral class.
 */
class PSY_C_API StringLiteral final : public SyntaxLexeme
{
public:
    StringLiteral(const char* chars, unsigned int size);

    virtual StringLiteral* asStringLiteralExpression() { return nullptr; }

    /**
     * \brief The StringLiteral::Variant enumeration.
     */
    enum class Variant : std::uint8_t
    {
        Plain_char,
        L_wchar_t,
        u8_char,
        u_char16_t,
        U_char32_t
    };

    /**
     * The Variant of \c this StringLiteral.
     */
    Variant variant() const;
};

} // C
} // psy

#endif
