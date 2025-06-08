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

#ifndef PSYCHE_GNU_DIRECTORY_SEARCH_OPTIONS_H__
#define PSYCHE_GNU_DIRECTORY_SEARCH_OPTIONS_H__

#include <string>
#include <vector>

namespace psy {
namespace gnu {

/**
 * https://gcc.gnu.org/onlinedocs/gcc/Directory-Options.html
 */
class DirectorySearchOptions final
{
public:
    DirectorySearchOptions();
    ~DirectorySearchOptions();

    std::vector<std::string> I_;
    std::vector<std::string> iquote_;
    std::vector<std::string> isystem_;
    std::vector<std::string> idirafter_;
    bool nostdinc_ = false;
};

} // gnu
} // psy

#endif
