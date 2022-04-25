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

#include "TextElement.h"

#include <cstring>

using namespace psy;

TextElement::TextElement(const char* chars, unsigned int size)
    : size_(size)
    , chars_(new char[size + 1])
    , next_(nullptr)
{
    std::strncpy(chars_, chars, size);
    chars_[size] = 0;
    hashCode_ = hashCode(chars_, size_);
}

TextElement::~TextElement()
{
    delete[] chars_;
}

unsigned int TextElement::hashCode(const char* chars, unsigned int size)
{
    // Hash taken from QtCore's qHash for strings, which in turn has the note:

    /*
    These functions are based on Peter J. Weinberger's hash function
    (from the Dragon Book). The constant 24 in the original function
    was replaced with 23 to produce fewer collisions on input such as
    "a", "aa", "aaa", "aaaa", ...
    */

    unsigned int h = 0;
    while (size--) {
        h = (h << 4) + *chars++;
        h ^= (h & 0xf0000000) >> 23;
        h &= 0x0fffffff;
    }
    return h;
}

namespace psy {

bool operator==(const TextElement& a, const TextElement& b)
{
    if (&a == &b)
        return true;

    if (a.size() != b.size())
        return false;

    if (a.hashCode() != b.hashCode())
        return false;

    return !std::strcmp(a.c_str(), b.c_str());
}

} // psy
