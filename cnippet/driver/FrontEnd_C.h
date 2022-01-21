// Copyright (c) 2016/17/18/19/20/21 Leandro T. C. Melo <ltcmelo@gmail.com>
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

#ifndef CNIPPET_FRONTEND_C_H__
#define CNIPPET_FRONTEND_C_H__

#include "Driver.h"

#include "FrontEnd.h"
#include "SyntaxTree.h"

#include <utility>
#include <string>

namespace cnip {

class CFrontEnd : public FrontEnd
{
public:
    CFrontEnd(const cxxopts::ParseResult& parsedCmdLine);

    int run(const std::string& srcText, const FileInfo& fi) override;

private:
    int run_CORE(const std::string& srcText, const FileInfo& fi);

    std::pair<std::string, std::string> extendSource(const std::string& srcText);
    std::pair<int, std::string> invokePreprocessor(const std::string& srcText, const FileInfo& fi);
    std::pair<int, std::unique_ptr<SyntaxTree>> invokeParser(
            const std::string& srcText, const FileInfo& fi);
    int invokeBinder(std::unique_ptr<SyntaxTree> tree);

    static constexpr int ERROR_PreprocessorInvocationFailure = 100;
    static constexpr int ERROR_PreprocessedFileWritingFailure = 101;
    static constexpr int ERROR_UnsuccessfulParsing = 102;
    static constexpr int ERROR_InvalidSyntaxTree = 103;

    std::unique_ptr<Configuration> config_;
};

} // cnip

#endif
