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

#ifndef PSYCHE_C_SYNTAX_LEXEME_CONSTANT_H__
#define PSYCHE_C_SYNTAX_LEXEME_CONSTANT_H__

#include "SyntaxLexeme.h"

#include "../common/infra/InternalAccess.h"

#include <cstdint>
#include <string>

namespace psy {
namespace C {

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
    //!@{
    /**
     * Cast \c this SyntaxLexeme as an IntegerConstant.
     */
    virtual IntegerConstant* asIntegerConstant() override { return this; }
    //!@}

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

    // TODO: Make internal.
    IntegerConstant(const char* chars, unsigned int size);
};

/**
 * \brief The FloatingConstant class.
 */
class PSY_C_API FloatingConstant final : public Constant
{
public:
    //!@{
    /**
     * Cast \c this SyntaxLexeme as a FloatingConstant.
     */
    virtual FloatingConstant* asFloatingConstant() override { return this; }
    //!@}

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

    // TODO: Make internal.
    FloatingConstant(const char* chars, unsigned int size);
};

/**
 * \brief The CharacterConstant class.
 */
class PSY_C_API CharacterConstant final : public Constant
{
public:
    //!@{
    /**
     * Cast \c this SyntaxLexeme as a CharacterConstant.
     */
    virtual CharacterConstant* asCharacterConstant() override { return this; }
    //!@}

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

    // TODO: Make internal.
    CharacterConstant(const char* chars, unsigned int size);
};

/**
 * \brief The ImaginaryIntegerConstant class.
 */
class PSY_C_API ImaginaryIntegerConstant final : public Constant
{
public:
    //!@{
    /**
     * Cast \c this SyntaxLexeme as an ImaginaryIntegerConstant.
     */
    virtual ImaginaryIntegerConstant* asImaginaryIntegerConstant() override { return this; }
    //!@}

    /**
     * \brief The ImaginaryIntegerConstant::Signedness enumeration.
     */
    enum class Signedness : std::uint8_t
    {
        Signed,
        Unsigned,
    };

    /**
     * The Signedness of \c this ImaginaryIntegerConstant.
     */
    Signedness signedness() const;

    /**
     * \brief The ImaginaryIntegerConstant::Variant enumeration.
     */
    enum class Variant : std::uint8_t
    {
        Int,
        Long,
        LongLong
    };

    /**
     * The Variant of \c this ImaginaryIntegerConstant.
     */
    Variant variant() const;

    // TODO: Make internal.
    ImaginaryIntegerConstant(const char* chars, unsigned int size);
};

/**
 * \brief The ImaginaryFloatingConstant class.
 */
class PSY_C_API ImaginaryFloatingConstant final : public Constant
{
public:
    //!@{
    /**
     * Cast \c this SyntaxLexemeas an ImaginaryFloatingConstant.
     */
    virtual ImaginaryFloatingConstant* asImaginaryFloatingConstant() override { return this; }
    //!@}

    /**
     * \brief The ImaginaryFloatingConstant::Variant enumeration.
     */
    enum class Variant : std::uint8_t
    {
        Float,
        Double,
        LongDouble
    };

    /**
     * The Variant of \c this ImaginaryFloatingConstant.
     */
    Variant variant() const;

    // TODO: Make internal.
    ImaginaryFloatingConstant(const char* chars, unsigned int size);
};

} // C
} // psy

#endif
