// Copyright (c) 2016/17/18/19/20/21/22 Leandro T. C. Melo <ltcmelo@gmail.com>
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

#ifndef PSYCHE_STDLIB_INDEX_H__
#define PSYCHE_STDLIB_INDEX_H__

#include "Fwds.h"
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

namespace psy {
namespace C {

class StdLibIndex final
{
public:
    enum class Version
    {
        C89,
        C99,
        C11
    };

    StdLibIndex(Version std);

    std::vector<std::string> inspect(const Compilation&) const;

    bool recognizes(const char* ident) const;

private:
    enum class SymbolKind : char
    {
        Type,
        Value
    };
    using StdSymbol = std::pair<const char*, SymbolKind>;
    using Index = std::unordered_map<const char*, std::vector<StdSymbol>>;

    Version std_;
    static const Index c89idx_;
    static const Index c99idx_;
    static const Index c11idx_;
};

} // C
} // psy

#endif
