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

#include "LanguageDialect.h"

#include "common/infra/Assertions.h"

#include <type_traits>

namespace psy {
namespace C {

bool operator==(LanguageDialect::Std a, LanguageDialect::Std b)
{
    return std::underlying_type<LanguageDialect::Std>::type(a)
            == std::underlying_type<LanguageDialect::Std>::type(b);
}

bool operator!=(LanguageDialect::Std a, LanguageDialect::Std b)
{
    return !(a == b);
}

bool operator<(LanguageDialect::Std a, LanguageDialect::Std b)
{
    return std::underlying_type<LanguageDialect::Std>::type(a)
            < std::underlying_type<LanguageDialect::Std>::type(b);
}

bool operator<=(LanguageDialect::Std a, LanguageDialect::Std b)
{
    return a < b || a == b;
}

bool operator>(LanguageDialect::Std a, LanguageDialect::Std b)
{
    return !(a < b) && !(a == b);
}

bool operator>=(LanguageDialect::Std a, LanguageDialect::Std b)
{
    return !(a < b);
}

std::string to_string(LanguageDialect::Std std)
{
    switch (std) {
        case LanguageDialect::Std::C89_90:
            return "c89";

        case LanguageDialect::Std::C99:
            return "c99";

        case LanguageDialect::Std::C11:
            return "c11";

        case LanguageDialect::Std::C17_18:
            return "c17";

        default:
            PSY_ASSERT_W_MSG(false, return "", "");
    }
}

} // C
} // psy
