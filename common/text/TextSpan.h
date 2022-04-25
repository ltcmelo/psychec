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

#ifndef PSYCHE_TEXT_SPAN_H__
#define PSYCHE_TEXT_SPAN_H__

#include "../API.h"

#include <ostream>

namespace psy {

/**
 * \brief The TextSpan class.
 *
 * An abstraction representation of an immutable span of text.
 *
 * \attention
 * The memory under which a piece of text is stored is unknown to this class;
 * it is reponsibility of the user to employ this abstraction in consistent manner.
 *
 * \note
 * This API is inspired by that of \c Microsoft.CodeAnalysis.Text.TextSpan
 * from Roslyn, the .NET Compiler Platform.
 */
class PSY_API TextSpan
{
public:
    TextSpan(unsigned int start, unsigned int  end)
        : start_(start)
        , end_(end)
    {}

    /**
     * The start of \c this span.
     */
    unsigned int start() const { return start_; }

    /**
     * The end of \c this span.
     */
    unsigned int end() const { return end_; }

private:
    unsigned int start_;
    unsigned int end_;
};

bool operator==(const TextSpan& a, const TextSpan& b);

std::ostream& operator<<(std::ostream& os, const TextSpan& span);

} // psy

#endif
