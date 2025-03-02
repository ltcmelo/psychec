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

#include "PlatformOptions.h"

#include "../common/infra/Assertions.h"

#include <limits>

using namespace psy;
using namespace C;

PlatformOptions::PlatformOptions()
    : maxValOfChar_(std::numeric_limits<char>::max())
    , maxValOfChar_S_(std::numeric_limits<char signed>::max())
    , maxValOfChar_U_(std::numeric_limits<char unsigned>::max())
    , maxValOfShort_S_(std::numeric_limits<short>::max())
    , maxValOfShort_U_(std::numeric_limits<short unsigned>::max())
    , maxValOfInt_S_(std::numeric_limits<int>::max())
    , maxValOfInt_U_(std::numeric_limits<int unsigned>::max())
    , maxValOfLong_S_(std::numeric_limits<long>::max())
    , maxValOfLong_U_(std::numeric_limits<long unsigned>::max())
    , maxValOfLongLong_S_(std::numeric_limits<long long>::max())
    , maxValOfLongLong_U_(std::numeric_limits<long long unsigned>::max())
    , maxValOfBool_(std::numeric_limits<bool>::max())
{}

void PlatformOptions::setMaxValueOf(ArithmeticIntegerType arithIntTy, unsigned long long v)
{
    switch (arithIntTy) {
        case ArithmeticIntegerType::Char:
            maxValOfChar_ = v;
            break;
        case ArithmeticIntegerType::Char_S:
            maxValOfChar_S_ = v;
            break;
        case ArithmeticIntegerType::Char_U:
            maxValOfChar_U_ = v;
            break;
        case ArithmeticIntegerType::Short_S:
            maxValOfShort_S_ = v;
            break;
        case ArithmeticIntegerType::Short_U:
            maxValOfShort_U_ = v;
            break;
        case ArithmeticIntegerType::Int_S:
            maxValOfInt_S_ = v;
            break;
        case ArithmeticIntegerType::Int_U:
            maxValOfInt_U_ = v;
            break;
        case ArithmeticIntegerType::Long_S:
            maxValOfLong_S_ = v;
            break;
        case ArithmeticIntegerType::Long_U:
            maxValOfLong_U_ = v;
            break;
        case ArithmeticIntegerType::LongLong_S:
            maxValOfLongLong_S_ = v;
            break;
        case ArithmeticIntegerType::LongLong_U:
            maxValOfLongLong_U_ = v;
            break;
        case ArithmeticIntegerType::Bool:
            maxValOfBool_ = v;
            break;
    }
}

unsigned long long PlatformOptions::maxValueOf(ArithmeticIntegerType arithIntTy) const
{
    switch (arithIntTy) {
        case ArithmeticIntegerType::Char:
            return maxValOfChar_;
        case ArithmeticIntegerType::Char_S:
            return maxValOfChar_S_;
        case ArithmeticIntegerType::Char_U:
            return maxValOfChar_U_;
        case ArithmeticIntegerType::Short_S:
            return maxValOfShort_S_;
        case ArithmeticIntegerType::Short_U:
            return maxValOfShort_U_;
        case ArithmeticIntegerType::Int_S:
            return maxValOfInt_S_;
        case ArithmeticIntegerType::Int_U:
            return maxValOfInt_U_;
        case ArithmeticIntegerType::Long_S:
            return maxValOfLong_S_;
        case ArithmeticIntegerType::Long_U:
            return maxValOfLong_U_;
        case ArithmeticIntegerType::LongLong_S:
            return maxValOfLongLong_S_;
        case ArithmeticIntegerType::LongLong_U:
            return maxValOfLongLong_U_;
        case ArithmeticIntegerType::Bool:
            return maxValOfBool_;
    }
    PSY_ASSERT_1(false);
    return 0;
}

