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

#include "GNUCompilerFacade.h"

#include "utility/Process.h"

#include <iostream>
#include <sstream>

namespace
{
const char * const kInclude = "#include";
}

using namespace psy;
using namespace gnu;

CompilerFacade::CompilerFacade(
        const std::string& compiler,
        const std::string& std,
        const DirectorySearchOptions& dirSearchOpts,
        const PreprocessorCommandOptions& ppCmdOpts)
    : compiler_(compiler)
    , std_(std)
    , dirSearchOpts_(dirSearchOpts)
    , ppCmdOpts_(ppCmdOpts)
{}

std::pair<int, std::string> CompilerFacade::preprocessFile(const std::string& filePath)
{
    std::string cmd = compiler_ + commandOptions();
    cmd += " -E -x c -CC ";
    cmd += filePath;
    return Process().execute(cmd);
}

std::pair<int, std::string> CompilerFacade::preprocessText(const std::string& fileText)
{
    std::string in = "cat << 'EOF' | ";
    in += compiler_ + commandOptions();
    in += " -E -x c -CC -\n";
    in += fileText;
    in += "\nEOF";
    return Process().execute(in);
}

std::pair<int, std::string> CompilerFacade::preprocess_IgnoreIncludes(const std::string& fileText)
{
    std::string srcText_P;
    srcText_P.reserve(fileText.length());

    std::istringstream iss(fileText);
    std::string line;
    while (std::getline(iss, line)) {
        line.erase(0, line.find_first_not_of(' '));
        if (line.find(kInclude) == 0)
            continue;
        srcText_P += (line + "\n");
    }
    return preprocessText(srcText_P);
}

std::string CompilerFacade::commandOptions() const
{
    std::string opts;
    if (!std_.empty())
        opts += " -std=" + std_;
    for (const auto& s : dirSearchOpts_.I_)
        opts += " -I " + s;;
    for (const auto& s : dirSearchOpts_.iquote_)
        opts += " -iquote " + s;;
    for (const auto& s : dirSearchOpts_.isystem_)
        opts += " -isystem " + s;;
    if (dirSearchOpts_.nostdinc_)
        opts += " -nostdinc";
    for (const auto& s : ppCmdOpts_.D_)
        opts += " -D " + s;;
    for (const auto& s : ppCmdOpts_.U_)
        opts += " -U " + s;;
    for (const auto& s : ppCmdOpts_.include_)
        opts += " -include " + s;;
    for (const auto& s : ppCmdOpts_.imacros_)
        opts += " -imacros " + s;;
    if (ppCmdOpts_.undef_)
        opts += " -undef";
    if (ppCmdOpts_.C_)
        opts += " -C";
    if (ppCmdOpts_.CC_)
        opts += " -CC";
    return opts;
}
