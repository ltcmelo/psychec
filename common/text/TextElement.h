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

#ifndef PSYCHE_TEXT_ELEMENT_H__
#define PSYCHE_TEXT_ELEMENT_H__

#include "../API.h"

namespace psy {

/**
 * \brief The TextElement class.
 *
 * A read-only element of text, stored in dedicated memory, and
 * chained together with other elements within a hash table.
 *
 * \see TextElementTable
 */
class PSY_API TextElement
{
public:
    TextElement(const char* c_str, unsigned int size);
    TextElement(const TextElement& other) = delete;
    virtual ~TextElement();
    void operator=(const TextElement& other) = delete;

    typedef const char* iterator;
    typedef iterator const_iterator;

    iterator begin() const { return chars_; }
    iterator end() const { return chars_ + size_; }

    //!@{
    /**
     * The number of characters in the text.
     */
    unsigned int size() const { return size_; }
    unsigned int length() const { return size_; }
    //!@}

    /**
     * The character at the given index \c idx.
     */
    char at(unsigned int idx) const { return chars_[idx]; }

    /**
     * A null-terminated C-style string of the text.
     */
    const char* c_str() const { return chars_; }

private:
    template <class> friend class TextElementTable;
    friend bool operator==(const TextElement& a, const TextElement& b);

    unsigned int size_;
    char* chars_;
    unsigned int hashCode_;
    TextElement* next_;

    unsigned int hashCode() const { return hashCode_; }
    static unsigned int hashCode(const char* c_str, unsigned int size);
};

} // psy

#endif
