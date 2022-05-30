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

#ifndef PSYCHE_C_LANGUAGE_DIALECT_H__
#define PSYCHE_C_LANGUAGE_DIALECT_H__

#include "API.h"

#include "../common/infra/InternalAccess.h"

#include <cstdint>
#include <string>

namespace psy {
namespace C {

/**
 * \brief The LanguageDialect class.
 *
 * The C dialectd, based on the Standard version: C89/90, C99, C11, or C17/18.
 *
 * \note
 * Additional information at https://gcc.gnu.org/onlinedocs/gcc/C-Dialect-Options.html.
 */
class PSY_C_API LanguageDialect
{
public:
    /**
     * \brief The C Standard enumeration.
     */
    enum class Std : std::uint8_t
    {
        C89_90,
        C99,
        C11,
        C17_18
    };

    LanguageDialect(Std std = Std::C11)
        : std_(std)
    {}

    /**
     * The C Standard of \c this LanguageDialect.
     */
    Std std() const { return std_; }

private:
    Std std_;
};

bool PSY_C_API operator==(LanguageDialect::Std a, LanguageDialect::Std b);
bool PSY_C_API operator!=(LanguageDialect::Std a, LanguageDialect::Std b);
bool PSY_C_API operator<(LanguageDialect::Std a, LanguageDialect::Std b);
bool PSY_C_API operator<=(LanguageDialect::Std a, LanguageDialect::Std b);
bool PSY_C_API operator>(LanguageDialect::Std a, LanguageDialect::Std b);
bool PSY_C_API operator>=(LanguageDialect::Std a, LanguageDialect::Std b);

std::string PSY_C_API to_string(LanguageDialect::Std std);

} // C
} // psy

#endif
