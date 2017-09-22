// Copyright (c) 2008 Roberto Raggi <roberto.raggi@gmail.com>
// Modifications: Copyright (c) 2016 Leandro T. C. Melo (ltcmelo@gmail.com)
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

#ifndef PSYCHE_DIALECT_H__
#define PSYCHE_DIALECT_H__

namespace psyche {

struct Dialect
{
    Dialect() : flags(0) {}

    bool isCpp() const { return !isC() && !isObjC(); }
    bool isC() const { return c89 || c99 || c11; }
    bool isObjC() const { return objC; }

    union {
        unsigned int flags;
        struct
        {
            // C++
            unsigned int cpp11 : 1;
            unsigned int cpp14 : 1;

            // C
            unsigned int c89 : 1;
            unsigned int c99 : 1;
            unsigned int c11 : 1;

            // Objective-C
            unsigned int objC : 1;

            // Qt
            unsigned int qt : 1;
            unsigned int qtMocRun : 1;
            unsigned int qtKeywords : 1; // If Qt is used but QT_NO_KEYWORDS defined.

            // Extensions
            unsigned int ext_KeywordsGNU : 1; // https://www.gnu.org/software/gnu-c-manual/gnu-c-manual.html#Keywords
            unsigned int ext_EnumeratorAttributes : 1; // http://clang.llvm.org/docs/LanguageExtensions.html#attributes-on-enumerators
            unsigned int ext_AvailabilityAttribute : 1; // https://clang.llvm.org/docs/AttributeReference.html#availability

            // Miscellanea
            unsigned int nullptrOnNULL : 1;
        };
    };
};

} // namespace psyche

#endif
