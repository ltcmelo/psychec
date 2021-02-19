// Copyright (c) 2016/17/18/19/20/21 Leandro T. C. Melo <ltcmelo@gmail.com>
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

namespace psy {

/**
 * \brief The TextSpan class.
 *
 * A read-only sequence of characters, such as that from a lexed string.
 *
 * \note
 * This API is influenced by that of \c Microsoft.CodeAnalysis.Text.TextSpan, from Roslyn
 * (the .NET Compiler Platform).
 */
class PSY_API TextSpan
{
public:
    TextSpan(const char* c_str, unsigned size);
    TextSpan(const TextSpan& other) = delete;
    virtual ~TextSpan();
    void operator=(const TextSpan& other) = delete;

    typedef const char* iterator;
    typedef iterator const_iterator;

    iterator begin() const { return chars_; }
    iterator end() const { return chars_ + size_; }

    //!@{
    /**
     * The number of characters in the text.
     */
    unsigned size() const { return size_; }
    unsigned length() const { return size_; }
    //!@}

    /**
     * The character at the given index \c idx.
     */
    char at(unsigned idx) const { return chars_[idx]; }

    /**
     * A null-terminated C-style string of the text.
     */
    const char* c_str() const { return chars_; }

    unsigned hashCode() const { return hashCode_; }
    static unsigned hashCode(const char* c_str, unsigned size);

private:
    unsigned size_;
    char* chars_;
    unsigned hashCode_;

    template <class> friend class TextSpanContainer;
    TextSpan* next_;
};

bool operator==(const TextSpan& a, const TextSpan& b);

} // psy

#endif
