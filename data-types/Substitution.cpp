// Copyright (c) 2019/20/21/22 Leandro T. C. Melo <ltcmelo@gmail.com>
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

#include "Substitution.h"

#include <iostream>
#include <string>
#include <vector>

namespace psy {

template <class T>
Substitution<T> Substitution<T>::Trivial;

template <>
std::string applyAll(const Substitution<std::string>& sub, const std::string& input)
{
    if (sub == Substitution<std::string>::Trivial)
        return input;

    std::string substituted = input;
    std::string::size_type pos = 0;
    while ((pos = substituted.find(sub.from(), pos)) != std::string::npos) {
        substituted.replace(pos, sub.from().size(), sub.to());
        pos += sub.to().size();
    }
    return substituted;
}

template <>
std::string applyOnce(const std::vector<Substitution<std::string>>& seq, const std::string& input)
{
    std::string substituted = input;
    std::string::size_type pos = 0;
    for (const auto& sub : seq) {
        if (sub == Substitution<std::string>::Trivial)
            continue;

        if ((pos = substituted.find(sub.from(), pos)) != std::string::npos) {
            substituted.replace(pos, sub.from().size(), sub.to());
            pos += sub.to().size();
        }
    }
    return substituted;
}

} // psy
