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

#include "GnuCompilerFacade.h"

#include "Process.h"

#include <iostream>
#include <sstream>

namespace
{
const char * const kInclude = "#include";
}

using namespace psy;

GnuCompilerFacade::GnuCompilerFacade(const std::string& compilerName,
                               const std::string& std,
                               const std::vector<std::string>& D,
                               const std::vector<std::string>& U)
    : compilerName_(compilerName)
    , std_(std)
    , D_(D)
    , U_(U)
{}

std::pair<int, std::string> GnuCompilerFacade::preprocess(const std::string& srcText)
{
    std::string in = "cat << 'EOF' | ";
    in += compilerName_;
    in += assembleMacroCmd();
    in += " ";
    in += "-std=" + std_ + " ";
    in += "-E -x c -CC -";
    in += "\n" + srcText + "\nEOF";

    return Process().execute(in);
}

std::pair<int, std::string> GnuCompilerFacade::preprocess_IgnoreIncludes(const std::string& srcText)
{
    std::string srcText_P;
    srcText_P.reserve(srcText.length());

    std::istringstream iss(srcText);
    std::string line;
    while (std::getline(iss, line)) {
        line.erase(0, line.find_first_not_of(' '));

        if (line.find(kInclude) == 0)
            continue;

        srcText_P += (line + "\n");
    }

    return preprocess(srcText_P);
}

std::string GnuCompilerFacade::assembleMacroCmd() const
{
    std::string s;
    for (const auto& d : D_)
        s += " -D " + d;
    for (const auto& u : U_)
        s += " -U " + u;
    return s;
}
