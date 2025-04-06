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

#ifndef PSYCHE_C_LEGACY_OPTIONS_H__
#define PSYCHE_C_LEGACY_OPTIONS_H__

#include "API.h"

#include <cstdint>

namespace psy {
namespace C {

/**
 * \brief The LegacyOptions class.
 */
class PSY_C_API LegacyOptions
{
public:
    LegacyOptions();

    //!@{
    /**
     * Whether to enable ordered comparison between pointer and zero.
     *
     * \code
     * int * x;
     * 0 < x;
     * \endcode
     */
    LegacyOptions& enable_orderedComparisonBetweenPointerAndZero(bool enable);
    bool isEnabled_orderedComparisonBetweenPointerAndZero() const;
    //!@}

    //!@{
    /**
     * Whether to enable ordered comparison between pointer and integer.
     *
     * \code
     * int * x;
     * 1 < x;
     * \endcode
     */
    LegacyOptions& enable_orderedComparisonBetweenPointerAndInteger(bool enable);
    bool isEnabled_orderedComparisonBetweenPointerAndInteger() const;
    //!@}

private:
    struct BD
    {
        std::uint32_t orderedComparisonBetweenPointerAndZero_ : 1;
        std::uint32_t orderedComparisonBetweenPointerAndInteger_ : 1;
    };
    union
    {
        std::uint32_t BD_;
        BD F_;
    };
};

} // C
} // psy

#endif
