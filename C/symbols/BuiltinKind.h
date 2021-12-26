// Copyright (c) 2021 Leandro T. C. Melo <ltcmelo@gmail.com>
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

#ifndef PSYCHE_C_SPECIAL_TYPE_H__
#define PSYCHE_C_SPECIAL_TYPE_H__

#include "API.h"
#include "APIFwds.h"

#include <cstdint>

namespace psy {
namespace C {

/**
 * \brief The BuiltinKind enum.
 *
 * The unsigned integer types are suffixed with \c _U;
 * the signed integer types are suffixed with \c _S.
 * The signed integer types are equivalent to their "plain" versions, except for \c char:
 * \c char is a separate type from <tt>signed char</tt> and <tt>unsigned char</tt>;
 * its enumerator is \c Char only (without any suffix).
 *
 * \remark 6.2.5-3
 * \remark 6.2.5-17
 * \remark 6.2.5-15 (with footnote 45)
 * \remark 6.7.2-2
 *
 * \note
 * This API is inspired by that of \c Microsoft.CodeAnalysis.SpecialType
 * from Roslyn, the .NET Compiler Platform.
 */
enum class BuiltinKind : std::uint8_t
{
    None = 0,

    Void,

    Char,

    Char_S, // signed char
    Char_U, // unsigned char

    Short_S, // short, signed short, short int, or signed short int
    Short_U, // unsigned short, or unsigned short int

    Int_S, // int, signed, or signed int
    Int_U, // unsigned, or unsigned int

    Long_S, // long, signed long, long int, or signed long int
    Long_U, // unsigned long, or unsigned long int

    LongLong_S, // long long, signed long long, long long int, or signed long long int
    LongLong_U, // unsigned long long, or unsigned long long int

    Float,

    Double,

    LongDouble,

    Bool,

    FloatComplex,
    DoubleComplex,
    LongDoubleComplex
};

} // C
} // psy

#endif
