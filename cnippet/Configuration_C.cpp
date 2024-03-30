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
const char* const kCompiler = "C-compiler";
const char* const kStd = "C-std";
const char* const kPPInclude = "C-pp-includes";
const char* const kSearchPaths = "C-I";
const char* const kDefinedMacros = "C-D";
const char* const KUndefedMacros = "C-U";
const char* const kCommentMode = "C-comment-mode";
const char* const kAmbigMode = "C-ambiguity-mode";
}

using namespace cnip;

ConfigurationForC::ConfigurationForC(const cxxopts::ParseResult& parsedCmdLine)
    : Configuration(parsedCmdLine)
{
    compiler_ = parsedCmdLine[kCompiler].as<std::string>();

    std_ = parsedCmdLine[kStd].as<std::string>();
    std::for_each(std_.begin(),
                  std_.end(),
                  [] (char& c) { c = ::tolower(c); });

    ppIncludes_ = parsedCmdLine[kPPInclude].as<bool>();

    if (parsedCmdLine.count(kSearchPaths))
        searchPaths_ = parsedCmdLine[kSearchPaths].as<std::vector<std::string>>();

    if (parsedCmdLine.count(kDefinedMacros))
        definedMacros_ = parsedCmdLine[kDefinedMacros].as<std::vector<std::string>>();
    if (parsedCmdLine.count(KUndefedMacros))
        undefedMacros_ = parsedCmdLine[KUndefedMacros].as<std::vector<std::string>>();

    commentMode_ = parsedCmdLine[kCommentMode].as<std::string>();

    ambigMode_ = parsedCmdLine[kAmbigMode].as<std::string>();

    inferTypes_ = parsedCmdLine.count("C-infer-types");
}

bool ConfigurationForC::isValid() const
{
    if (!(std_ == "c89"
          || std_ == "c90"
          || std_ == "c99"
          || std_ == "c17"
          || std_ == "c18"
          || std_ == "c11"))
        return false;

    if (!(ambigMode_ == "D"
          || ambigMode_ == "DA"
          || ambigMode_ == "DAH"
          || ambigMode_ == "DH"))
        return false;

    return true;
}

void ConfigurationForC::extend(cxxopts::Options& cmdLineOpts)
{
    cmdLineOpts.add_options()

        /* Compiler */
            (kCompiler,
                "Select the compiler.",
                cxxopts::value<std::string>()->default_value("gcc"),
                "{gcc,clang}")

        /* Language */
            // https://gcc.gnu.org/onlinedocs/gcc/C-Dialect-Options.html#C-Dialect-Options
            (kStd,
                "Select the standard.",
                cxxopts::value<std::string>()->default_value("c11"),
                "{c89,c90,c99,c11,c17}")

        /* Preprocessor */
            (kPPInclude,
                "Preprocess `#include' directives.",
                cxxopts::value<bool>()->default_value("false"))

            // https://gcc.gnu.org/onlinedocs/gcc/Directory-Options.html
            (kSearchPaths,
                "Add directory <dir> to the search path of `#include' directives.",
                cxxopts::value<std::vector<std::string>>(),
                "<dir>")

            // https://gcc.gnu.org/onlinedocs/gcc/Preprocessor-Options.html
            (kDefinedMacros,
                "Define macro <name> with <value> (or 1 if <value> omitted).",
                cxxopts::value<std::vector<std::string>>(),
                "<name>=<value>")
            (KUndefedMacros,
                "Undefine macro <name>.",
                cxxopts::value<std::vector<std::string>>(),
                "<name>")

        /* Parser */
            (kCommentMode,
                "Select the comment mode: "
                "D=discard, "
                "KA=keep all, "
                "KOD=keep only documentation",
                cxxopts::value<std::string>()
                    ->default_value("D"),
                "{D,KA,KOD}")

            (kAmbigMode,
                "Select the ambiguity mode: "
                "D=diagnose, "
                "DA=disambiguate algorithmically, "
                "DAH=disambiguate algorithmically and heuristically, "
                "DH=disambiguate heuristically.",
                cxxopts::value<std::string>()
                    ->default_value("DAH"),
                "{D,DA,DAH,DH}")

        /* Type inference */
            ("C-infer-types", "Infer missing types.")

        ;
//            ("o,output", "Specify output file",
//                cxxopts::value<std::string>()->default_value("a.cstr"))
}
