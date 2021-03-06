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

Driver::~Driver()
{}

int Driver::execute(int argc, char* argv[])
{
    std::string lang;
    cxxopts::Options opts(argv[0], "Cnippet");
    try {
        opts.positional_help("file.c")
            .show_positional_help();
        opts.add_options()

        /* General */
            ("h,help",
             "Print usage instructions.")
            ("d,debug",
             "Enable debugging output.",
             cxxopts::value<bool>(DEBUG::globalDebugEnabled))
            ("p,plugin",
             "Load the plugin with the given name.",
             cxxopts::value<std::string>())
            ("l,lang",
             "Specify the operating language.",
             cxxopts::value<std::string>()->default_value("C"))

        /* Host C compiler */
            ("cc",
             "Specify the host C compiler.",
             cxxopts::value<std::string>()->default_value("gcc"))
            ("cc-pp",
             "Run the host C compiler's preprocessor.",
             cxxopts::value<bool>()->default_value("false"))
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
             "Add a directory as an `#include' search path.",
             cxxopts::value<std::vector<std::string>>())

        /* Type inference */
            ("C-infer",
             "Infer the definition of missing types.")
            ("C-infer-only",
             "Don't allow types to be defined.")

         /* AST */
            ("C-dump-AST",
             "Dump the C AST to the console.")

        /* Output */
            ("o,output",
             "Specify output file",
             cxxopts::value<std::string>()->default_value("a.cstr"))

        /* Input */
            ("file",
             "Input file",
             cxxopts::value<std::vector<std::string>>());

        opts.parse_positional(std::vector<std::string>{"file"});

        auto cmd = opts.parse(argc, argv);

        if (cmd.count("help")) {
            std::cout << opts.help({"", "Group"}) << std::endl;
            return SUCCESS;
        }

        if (cmd.count("plugin")) {
            auto pluginName = cmd["plugin"].as<std::string>();
            Plugin::load(pluginName);
            if (!Plugin::isLoaded()) {
                std::cerr << kCnip << "cannot load plugin " << pluginName << std::endl;
                return ERROR_CannotLoadPluging;
            }
        }

        if (!cmd.count("file")) {
            std::cerr << kCnip << "input file unspecified" << std::endl;
            return ERROR_InputFileUnspecified;
        }

        lang = cmd["lang"].as<std::string>();
        applyOptions(cmd);
    }
    catch (...) {
        std::cerr << kCnip << "command-line error" << std::endl;
        return ERROR_UnrecognizedCommandLine;
    }

    auto [exit, source] = readFile(config_->input_.fullFileName());
    if (exit != 0)
        return ERROR_InputFileReadingFailure;

    if (lang == "C")
        return Executer_C(this).execute(source);

    std::cerr << kCnip << "unsupported language" << std::endl;
    return ERROR_UnsupportedLanguage;
}

void Driver::applyOptions(const cxxopts::ParseResult& cmd)
{
    std::string inputPath = cmd["file"].as<std::vector<std::string>>()[0];
    config_.reset(new Configuration(inputPath));

    config_->C_hostCC_ = cmd["cc"].as<std::string>();

    LanguageDialect::Std stdP;
    auto std = cmd["cc-std"].as<std::string>();
    std::for_each(std.begin(), std.end(),
                  [] (char& c) { c = ::tolower(c); });

    if (std == "c89" || std == "c90")
        stdP = LanguageDialect::Std::C89_90;
    else if (std == "c99")
        stdP = LanguageDialect::Std::C99;
    else if (std == "c17" || std == "c18")
        stdP = LanguageDialect::Std::C17_18;
    else
        stdP = LanguageDialect::Std::C11;
    config_->C_std = stdP;

    config_->C_pp_ = cmd["cc-pp"].as<bool>();
    if (cmd.count("cc-D"))
        config_->C_macroDefs_ = cmd["cc-D"].as<std::vector<std::string>>();
    if (cmd.count("cc-U"))
        config_->C_macroUndefs_ = cmd["cc-U"].as<std::vector<std::string>>();
    if (cmd.count("cc-I"))
        config_->C_searchPaths_ = cmd["cc-I"].as<std::vector<std::string>>();

    config_->C_dumpAST = cmd.count("C-dump-AST");
    config_->C_infer = cmd.count("C-infer");
    config_->C_inferOnly = cmd.count("C-infer-only");
}
