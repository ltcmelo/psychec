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

#ifndef PSYCHE_C_STRING_LITERAL_LEXEME_H__
#define PSYCHE_C_STRING_LITERAL_LEXEME_H__

#include "Lexeme.h"

#include <cstdint>
#include <string>

namespace psy {
namespace C {

/**
 * \brief The StringLiteral class.
 */
class PSY_C_API StringLiteral final : public Lexeme
{
public:
    //!@{
    /**
     * Cast \c this Lexeme as a StringLiteral.
     */
    virtual StringLiteral* asStringLiteral() override { return this; }
    virtual const StringLiteral* asStringLiteral() const override { return this; }

    /**
     * \brief The EncodingPrefix alternatives of a StringLiteral.
     */
    enum class EncodingPrefix : std::uint8_t
    {
        u8,
        u,
        U,
        L,
        None
    };

    /**
     * The EncodingPrefix of \c this StringLiteral.
     */
    EncodingPrefix encodingPrefix() const;

    // TODO: Make internal.
    StringLiteral(const char* chars, unsigned int size);
};

} // C
} // psy

#endif
