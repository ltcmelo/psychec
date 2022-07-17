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

#include "Configuration_C.h"

namespace {
const char* const kCStd = "c-std";
const char* const kHostCCompiler = "host-cc";
const char* const kExpandCPPIncludeDirectives = "cpp-includes";
const char* const kDefineCPPMacro = "cpp-D";
const char* const KUndefineCPPMacro = "cpp-U";
const char* const kAddDirToCPPSearchPath = "cpp-I";
}

using namespace cnip;

void ConfigurationForC::extend(cxxopts::Options& cmdLineOpts)
{
    cmdLineOpts.add_options()
            // https://gcc.gnu.org/onlinedocs/gcc/C-Dialect-Options.html#C-Dialect-Options
            (kCStd,
                "Specify the C standard.",
                cxxopts::value<std::string>()->default_value("c11"),
                "<c89|c90|c99|c11|c17>")

            (kHostCCompiler,
                "Specify a host C compiler.",
                cxxopts::value<std::string>()->default_value("gcc"),
                "<gcc|clang>")

            // https://gcc.gnu.org/onlinedocs/cpp/Include-Syntax.html
            (kExpandCPPIncludeDirectives,
                "Expand `#include' directives of the C preprocessor.",
                cxxopts::value<bool>()->default_value("false"))

            // https://gcc.gnu.org/onlinedocs/gcc/Directory-Options.html
            (kAddDirToCPPSearchPath,
                "Add a directory to the `#include' search path of the C preprocessor.",
                cxxopts::value<std::vector<std::string>>(),
                "path")

            // https://gcc.gnu.org/onlinedocs/gcc/Preprocessor-Options.html
            (kDefineCPPMacro,
                "Predefine a C preprocessor macro.",
                cxxopts::value<std::vector<std::string>>(),
                "<name|name=definition>")
            (KUndefineCPPMacro,
                "Undefine a C preprocessor macro.",
                cxxopts::value<std::vector<std::string>>(),
                "<name>")

        /* Ambiguity */
            ("C-ParseOptions-TreatmentOfAmbiguities",
                "Treatment of ambiguities.",
                cxxopts::value<std::string>()
                    ->default_value("DisambiguateAlgorithmicallyOrHeuristically"),
                "<None|Diagnose|DisambiguateAlgorithmically|DisambiguateAlgorithmicallyOrHeuristically|DisambiguateHeuristically>")

        /* Type inference */
            ("C-infer", "Infer the definition of missing types.")
            ("o,output", "Specify output file",
                cxxopts::value<std::string>()->default_value("a.cstr"))
        ;
}

ConfigurationForC::ConfigurationForC(const cxxopts::ParseResult& parsedCmdLine)
    : Configuration(parsedCmdLine)
{
    auto cc_std = parsedCmdLine[kCStd].as<std::string>();
    std::for_each(cc_std.begin(),
                  cc_std.end(),
                  [] (char& c) { c = ::tolower(c); });
    if (cc_std == "c89" || cc_std == "c90")
        langStd = LanguageDialect::Std::C89_90;
    else if (cc_std == "c99")
        langStd = LanguageDialect::Std::C99;
    else if (cc_std == "c17" || cc_std == "c18")
        langStd = LanguageDialect::Std::C17_18;
    else
        langStd = LanguageDialect::Std::C11;

    hostCompiler = parsedCmdLine[kHostCCompiler].as<std::string>();

    expandIncludes = parsedCmdLine[kExpandCPPIncludeDirectives].as<bool>();
    if (parsedCmdLine.count(kDefineCPPMacro))
        macrosToDefine = parsedCmdLine[kDefineCPPMacro].as<std::vector<std::string>>();
    if (parsedCmdLine.count(KUndefineCPPMacro))
        macrosToUndef = parsedCmdLine[KUndefineCPPMacro].as<std::vector<std::string>>();

    if (parsedCmdLine.count(kAddDirToCPPSearchPath))
        headerSearchPaths = parsedCmdLine[kAddDirToCPPSearchPath].as<std::vector<std::string>>();

    ParseOptions_TreatmentOfAmbiguities = parsedCmdLine["C-ParseOptions-TreatmentOfAmbiguities"].as<std::string>();

    inferMissingTypes = parsedCmdLine.count("infer");
}
