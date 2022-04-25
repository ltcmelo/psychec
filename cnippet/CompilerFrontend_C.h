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

#ifndef CNIPPET_C_COMPILER_FRONTEND_C_H__
#define CNIPPET_C_COMPILER_FRONTEND_C_H__

#include "Driver.h"

#include "CompilerFrontend.h"
#include "Configuration_C.h"

#include "C/SyntaxTree.h"

#include <utility>
#include <string>

namespace cnip {

/*!
 * \brief The CCompilerFrontend class.
 */
class CCompilerFrontend : public CompilerFrontend
{
public:
    CCompilerFrontend(const cxxopts::ParseResult& parsedCmdLine);
    virtual ~CCompilerFrontend();

    int run(const std::string& srcText, const psy::FileInfo& fi) override;

private:

    int extendWithStdLibHeaders(const std::string& srcText, const psy::FileInfo& fi);
    int preprocess(const std::string& srcText, const psy::FileInfo& fi);
    int constructSyntaxTree(const std::string& srcText, const psy::FileInfo& fi);
    int computeSemanticModel(std::unique_ptr<psy::C::SyntaxTree> tree);

    static constexpr int ERROR_PreprocessorInvocationFailure = 100;
    static constexpr int ERROR_PreprocessedFileWritingFailure = 101;
    static constexpr int ERROR_UnsuccessfulParsing = 102;
    static constexpr int ERROR_InvalidSyntaxTree = 103;

    std::unique_ptr<ConfigurationForC> config_;
};

} // cnip

#endif
