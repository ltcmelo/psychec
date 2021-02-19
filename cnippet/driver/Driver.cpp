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

#include "Driver.h"

#include "Executer_C.h"
#include "FileInfo.h"
#include "IO.h"
#include "Plugin.h"

#include <algorithm>
#include <iostream>
#include <iterator>
#include <sstream>
#include <cstring>

using namespace cnip;

namespace DEBUG { extern bool globalDebugEnabled; }

constexpr int Driver::SUCCESS;
constexpr int Driver::ERROR_InputFileUnspecified;
constexpr int Driver::ERROR_UnrecognizedCommandLine;
constexpr int Driver::ERROR_CannotLoadPluging;
constexpr int Driver::ERROR_UnsupportedLanguage;

Driver::Driver()
{}

int Driver::execute(int argc, char* argv[])
{

    cxxopts::Options opts(argv[0], "Cnippet");
    try {
        opts.positional_help("file");
        opts.add_options()

        /* General */
            ("h,help",
             "Print usage instructions.")
            ("d,debug",
             "Enable debugging output.",
             cxxopts::value<bool>(DEBUG::globalDebugEnabled))
            ("p,plugin",
             "Load the plugin of given name.",
             cxxopts::value<std::string>())
            ("l,lang",
             "Specify the operating language.",
             cxxopts::value<std::string>()->default_value("C"))

        /* Host C compiler */
            ("cc",
             "Specify the host C compiler.",
             cxxopts::value<std::string>()->default_value("gcc"))
            ("cc-std",
             "Specify the C dialect.",
             cxxopts::value<std::string>()->default_value("c11"))
            ("cc-D",
             "Predefine a macro.",
             cxxopts::value<std::vector<std::string>>())
            ("cc-U",
             "Undefine a macro.",
             cxxopts::value<std::vector<std::string>>())
            ("cc-I",
             "Add a directory to the list of `#include' search paths.",
             cxxopts::value<std::vector<std::string>>())

        /* Type inference */
            ("C-infer",
             "Infer the definition of missing type declarations.")
            ("C-infer-only",
             "Don't allow types to be defined, rely only on type inference.")

         /* AST */
            ("C-dump-AST",
             "Dump the C AST to the console.")
            ("C-AST-no-heuristics",
             "Disable heuristics on unresolved AST ambiguities.")
            ("C-AST-stats",
             "Display AST disambiguation statistics.")

        /* Output */
            ("o,output",
             "Specify output file",
             cxxopts::value<std::string>()->default_value("a.cstr"))
            ("c,constraints",
             "Display generated constraints.")

        /* Input */
            ("positional",
             "Positional arguments",
             cxxopts::value<std::vector<std::string>>());

        opts.parse_positional(std::vector<std::string>{"file", "positional"});
        opts.parse(argc, argv);
    }
    catch (const cxxopts::OptionException& ex) {
        std::cerr << kCnip << ex.what() << std::endl;
        return ERROR_UnrecognizedCommandLine;
    }

    if (opts.count("help")) {
        std::cout << opts.help({"", "Group"}) << std::endl;
        return SUCCESS;
    }

    if (opts.count("plugin")) {
        auto pluginName = opts["plugin"].as<std::string>();
        Plugin::load(pluginName);
        if (!Plugin::isLoaded()) {
            std::cerr << kCnip << "cannot load plugin " << pluginName << std::endl;
            return ERROR_CannotLoadPluging;
        }
    }

    if (!opts.count("positional")) {
        std::cerr << kCnip << "input file unspecified" << std::endl;
        return ERROR_InputFileUnspecified;
    }

    applyOptions(opts);

    auto [exit, source] = readFile(config_->input_.fullFileName());
    if (exit != 0)
        return ERROR_InputFileReadingFailure;

    auto lang = opts["lang"].as<std::string>();
    if (lang == "C")
        return Executer_C(this).execute(source);

    std::cerr << kCnip << "unsupported language" << std::endl;
    return ERROR_UnsupportedLanguage;
}

void Driver::applyOptions(cxxopts::Options opts)
{
    std::string inputPath = opts["positional"].as<std::vector<std::string>>()[0];
    config_.reset(new Configuration(inputPath));

    config_->C_hostCC_ = opts["cc"].as<std::string>();

    auto std = opts["cc-std"].as<std::string>();
    std::for_each(std.begin(), std.end(),
                  [] (char& c) { c = ::tolower(c); });
    LanguageDialect::Std stdP = LanguageDialect::Std::C11;
    if (std == "c89" || std == "c90")
        stdP = LanguageDialect::Std::C89_90;
    else if (std == "c99")
        stdP = LanguageDialect::Std::C99;
    else if (std == "c17" || std == "c18")
        stdP = LanguageDialect::Std::C17_18;
    config_->C_std = stdP;

    config_->C_macroDefs_ = opts["cc-D"].as<std::vector<std::string>>();
    config_->C_macroUndefs_ = opts["cc-U"].as<std::vector<std::string>>();
    config_->C_searchPaths_ = opts["cc-I"].as<std::vector<std::string>>();
    config_->C_dumpAST = opts.count("C-dump-AST");
    config_->C_infer = opts.count("C-infer");
    config_->C_inferOnly = opts.count("C-infer-only");
}

