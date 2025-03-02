// Copyright (c) 2025 Leandro T. C. Melo <ltcmelo@gmail.com>
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

#ifndef PSYCHE_C_PLATFORM_OPTIONS_H__
#define PSYCHE_C_PLATFORM_OPTIONS_H__

#include "API.h"

#include <cstdint>

namespace psy {
namespace C {

/**
 * \brief The PlatformOptions class.
 */
class PSY_C_API PlatformOptions
{
public:
    PlatformOptions();

    /**
     * The ArithmeticIntegerType alternatives of a PlatformOptions.
     */
    enum class ArithmeticIntegerType : std::uint8_t
    {
        Char,
        Char_S,
        Char_U,
        Short_S,
        Short_U,
        Int_S,
        Int_U,
        Long_S,
        Long_U,
        LongLong_S,
        LongLong_U,
        Bool,
    };

    //!@{
    /**
     * The maximum value of arithmetic integer types of \c this PlatformOptions.
     */
    void setMaxValueOf(ArithmeticIntegerType arithIntTy, unsigned long long v);
    unsigned long long maxValueOf(ArithmeticIntegerType arithIntTy) const;
    //!@}

private:
    unsigned long long maxValOfChar_;
    unsigned long long maxValOfChar_S_;
    unsigned long long maxValOfChar_U_;
    unsigned long long maxValOfShort_S_;
    unsigned long long maxValOfShort_U_;
    unsigned long long maxValOfInt_S_;
    unsigned long long maxValOfInt_U_;
    unsigned long long maxValOfLong_S_;
    unsigned long long maxValOfLong_U_;
    unsigned long long maxValOfLongLong_S_;
    unsigned long long maxValOfLongLong_U_;
    unsigned long long maxValOfBool_;
};

} // C
} // psy

#endif
