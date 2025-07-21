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

#ifndef CNIPPET_COMMAND_OPTIONS_H__
#define CNIPPET_COMMAND_OPTIONS_H__

#include "compiler_support/gnu/GNUPreprocessorCommandOptions.h"
#include "compiler_support/gnu/GNUDirectorySearchOptions.h"

#include <string>
#include <vector>

namespace cnip {

class CommandOptions final
{
public:
    CommandOptions();

    std::vector<std::string> cFilePaths_;
    std::vector<std::string> iFilePaths_;

    // GNU directory search.
    psy::gnu::DirectorySearchOptions gnuDirSearchOpts_;

    // GNU preprocessor options
    psy::gnu::PreprocessorCommandOptions gnuPPCmdOpts_;

    // GNU C dialect
    std::string std_ = "c17";
    bool ansi_ = false;

    // GNU Compilation stages
    bool syntax_only_ = false;

    // Psyche-C
    bool help_ = false;
    bool dumpAST_ = false;
    bool includeStdLibHeaders_ = false;
    std::string disambigMode_ = "ah";
    std::string ppMode_ = "s";
    std::string compiler_ = "gcc";
    std::string commentMode_ = "d";
    std::vector<std::string> analysisFilePaths_;
};

} // cnip

#endif
