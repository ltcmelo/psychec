// Copyright (c) 2020/21 Leandro T. C. Melo <ltcmelo@gmail.com>
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

#ifndef PSYCHE_C_PREPROCESSOR_OPTIONS_H__
#define PSYCHE_C_PREPROCESSOR_OPTIONS_H__

#include "API.h"

#include <vector>
#include <string>

namespace psy {
namespace C {

/**
 * The PreprocessorOptions class.
 *
 * \see https://gcc.gnu.org/onlinedocs/gcc/Preprocessor-Options.html
 */
class PSY_C_API PreprocessorOptions
{
public:
    PreprocessorOptions();

    /**
     * Add a predefined macro, equivalent to \a -D \c name.
     */
    PreprocessorOptions& D(const std::string& name);

    /**
     * Add a predefined macro, equivalent to \a -D \c name=def.
     */
    PreprocessorOptions& D(const std::string& name, const std::string& def);

    /**
     * The predefined macros list.
     */
    const std::vector<std::string>& Ds() const;

    /**
     * Cancel any previous definition of \c name, equivalent to \a -U \c name.
     */
    PreprocessorOptions& U(const std::string& name);

    /**
     * The undefined macros list.
     */
    const std::vector<std::string>& Us() const;

private:
    std::vector<std::string> D_;
    std::vector<std::string> U_;
};

} // C
} // psy

#endif
