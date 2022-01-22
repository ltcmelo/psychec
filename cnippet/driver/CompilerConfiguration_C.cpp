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

#include "CompilerConfiguration_C.h"

using namespace cnip;

void CCompilerConfiguration::extend(cxxopts::Options& cmdLineOpts)
{
    cmdLineOpts.add_options()
        /* Host C compiler */
            ("cc", "Specify the host C compiler.",
                cxxopts::value<std::string>()->default_value("gcc"))
            ("cc-pp", "Run the host C compiler's preprocessor.",
                cxxopts::value<bool>()->default_value("false"))
            ("cc-std", "Specify the C dialect.",
                cxxopts::value<std::string>()->default_value("c11"))
            ("cc-D", "Predefine a macro.",
                cxxopts::value<std::vector<std::string>>())
            ("cc-U", "Undefine a macro.",
                cxxopts::value<std::vector<std::string>>())
            ("cc-I", "Add a directory to `#include' search path.",
                cxxopts::value<std::vector<std::string>>())

        /* Type inference */
            ("C-infer", "Infer the definition of missing types.")
            ("C-infer-only", "Don't allow types to be defined.")
            ("o,output", "Specify output file",
                cxxopts::value<std::string>()->default_value("a.cstr"))
        ;
}

CCompilerConfiguration::CCompilerConfiguration(const cxxopts::ParseResult& parsedCmdLine)
    : CompilerConfiguration(parsedCmdLine)
{
    cmdLineOpt_cc = parsedCmdLine["cc"].as<std::string>();

    auto cc_std = parsedCmdLine["cc-std"].as<std::string>();
    std::for_each(cc_std.begin(),
                  cc_std.end(),
                  [] (char& c) { c = ::tolower(c); });
    if (cc_std == "c89" || cc_std == "c90")
        cmdLineOpt_cc_std = LanguageDialect::Std::C89_90;
    else if (cc_std == "c99")
        cmdLineOpt_cc_std = LanguageDialect::Std::C99;
    else if (cc_std == "c17" || cc_std == "c18")
        cmdLineOpt_cc_std = LanguageDialect::Std::C17_18;
    else
        cmdLineOpt_cc_std = LanguageDialect::Std::C11;

    cmdLineOpt_cc_pp = parsedCmdLine["cc-pp"].as<bool>();
    if (parsedCmdLine.count("cc-D"))
        cmdLineOpt_cc_D = parsedCmdLine["cc-D"].as<std::vector<std::string>>();
    if (parsedCmdLine.count("cc-U"))
        cmdLineOpt_cc_U = parsedCmdLine["cc-U"].as<std::vector<std::string>>();
    if (parsedCmdLine.count("cc-I"))
        cmdLineOpt_cc_I = parsedCmdLine["cc-I"].as<std::vector<std::string>>();

    C_infer = parsedCmdLine.count("C-infer");
    C_inferOnly = parsedCmdLine.count("C-infer-only");
}
