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

#ifndef PSYCHE_C_LINE_DIRECTIVE_LINE_H__
#define PSYCHE_C_LINE_DIRECTIVE_LINE_H__

#include "API.h"

#include "../common/location/LinePosition.h"

#include <string>

namespace psy {
namespace C {

/**
 * \brief The LineDirective class.
 */
class PSY_C_NON_API LineDirective
{
public:
    LineDirective(unsigned int lineno,
                  const std::string& fileName,
                  unsigned int offset);

    /**
     * The file name.
     */
    std::string fileName() const;

    /**
     * The line number.
     */
    unsigned int lineno() const;

    /**
     * The offset in the file.
     */
    unsigned int offset() const;

private:
    unsigned int lineno_;
    std::string fileName_;
    unsigned int offset_;
};

} // C
} // psy

#endif
