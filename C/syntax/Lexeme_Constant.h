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

#ifndef PSYCHE_C_CONSTANT_LEXEME_H__
#define PSYCHE_C_CONSTANT_LEXEME_H__

#include "Lexeme.h"

#include "../common/infra/AccessSpecifiers.h"

#include <cstdint>
#include <string>

namespace psy {
namespace C {

/**
 * \brief The IntegerConstant class.
 */
class PSY_C_API IntegerConstant final : public Lexeme
{
public:
    //!@{
    /**
     * Cast \c this Lexeme as an IntegerConstant.
     */
    virtual IntegerConstant* asIntegerConstant() override { return this; }
    virtual const IntegerConstant* asIntegerConstant() const override { return this; }
    //!@}

    /**
     * The RepresentationSuffix alternatives of an IntegerConstant.
     *
     * \remark 6.4.4.1-5
     */
    enum class RepresentationSuffix : std::uint8_t
    {
        None,
        uOrU,
        lOrL,
        lOrLAnduOrU,
        llOrLL,
        llOrLLAnduOrU
    };

    /**
     * The RepresentationSuffix of \c this IntegerConstant.
     */
    RepresentationSuffix representationSuffix() const;

    /**
     * Whether \c this IntegerConstant is \a octal or \a hexadecimal.
     */
    bool isOctalOrHexadecimal() const;

    // TODO: Make internal.
    IntegerConstant(const char* chars, unsigned int size);
};

/**
 * \brief The FloatingConstant class.
 */
class PSY_C_API FloatingConstant final : public Lexeme
{
public:
    //!@{
    /**
     * Cast \c this Lexeme as a FloatingConstant.
     */
    virtual FloatingConstant* asFloatingConstant() override { return this; }
    virtual const FloatingConstant* asFloatingConstant() const override { return this; }
    //!@}

    /**
     * The RepresentationSuffix alternatives of a FloatingConstant.
     *
     * \remark 6.4.4.1-5
     */
    enum class RepresentationSuffix : std::uint8_t
    {
        None,
        lOrL,
        fOrF,
    };

    /**
     * The RepresentationSuffix of \c this FloatingConstant.
     */
    RepresentationSuffix representationSuffix() const;

    // TODO: Make internal.
    FloatingConstant(const char* chars, unsigned int size);
};

/**
 * \brief The CharacterConstant class.
 */
class PSY_C_API CharacterConstant final : public Lexeme
{
public:
    //!@{
    /**
     * Cast \c this Lexeme as a CharacterConstant.
     */
    virtual CharacterConstant* asCharacterConstant() override { return this; }
    virtual const CharacterConstant* asCharacterConstant() const override { return this; }
    //!@}

    /**
     * \brief The EncodingPrefix alternatives of a CharacterConstant.
     */
    enum class EncodingPrefix : std::uint8_t
    {
        u,
        U,
        L,
        None
    };

    /**
     * The EncodingPrefix of \c this CharacterConstant.
     */
    EncodingPrefix encodingPrefix() const;

    // TODO: Make internal.
    CharacterConstant(const char* chars, unsigned int size);
};

/**
 * \brief The ImaginaryIntegerConstant class.
 */
class PSY_C_API ImaginaryIntegerConstant final : public Lexeme
{
public:
    //!@{
    /**
     * Cast \c this Lexeme as an ImaginaryIntegerConstant.
     */
    virtual ImaginaryIntegerConstant* asImaginaryIntegerConstant() override { return this; }
    virtual const ImaginaryIntegerConstant* asImaginaryIntegerConstant() const override { return this; }
    //!@}

    // TODO: Make internal.
    ImaginaryIntegerConstant(const char* chars, unsigned int size);
};

/**
 * \brief The ImaginaryFloatingConstant class.
 */
class PSY_C_API ImaginaryFloatingConstant final : public Lexeme
{
public:
    //!@{
    /**
     * Cast \c this Lexemeas an ImaginaryFloatingConstant.
     */
    virtual ImaginaryFloatingConstant* asImaginaryFloatingConstant() override { return this; }
    virtual const ImaginaryFloatingConstant* asImaginaryFloatingConstant() const override { return this; }
    //!@}

    // TODO: Make internal.
    ImaginaryFloatingConstant(const char* chars, unsigned int size);
};

} // C
} // psy

#endif
