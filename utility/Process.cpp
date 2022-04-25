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

#include "Process.h"

#include <array>
#include <cstdio> // Using POSIX's `popen'.
#include <iostream>

using namespace psy;

namespace {

std::pair<int, std::string> executeCore(const char* cmd)
{
    FILE* pipe = popen(cmd, "r");
    if (!pipe)
        return std::make_pair(1, "");

    std::string all;
    while (!feof(pipe)) {
        std::array<char, 512> buf;
        if (fgets(buf.data(), 512, pipe))
            all += buf.data();
    }

    return std::make_pair(pclose(pipe), all);
}

} // anonymous

std::pair<int, std::string> Process::execute(std::string&& s)
{
    return executeCore(s.c_str());
}

std::pair<int, std::string> Process::execute(const std::string &s)
{
    return executeCore(s.c_str());
}
