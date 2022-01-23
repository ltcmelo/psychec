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

#include "CompilerFrontEnd.h"
#include "CompilerFrontEnd_C.h"
#include "Configuration.h"
#include "Configuration_C.h"
#include "FileInfo.h"
#include "IO.h"
#include "Plugin.h"

#include <algorithm>
#include <iostream>
#include <iterator>
#include <sstream>
#include <cstring>

using namespace psy;
using namespace cnip;

namespace DEBUG { extern bool globalDebugEnabled; }

constexpr int Driver::SUCCESS;
constexpr int Driver::ERROR_NoInputFile;
constexpr int Driver::ERROR_UnrecognizedCmdLineOption;
constexpr int Driver::ERROR_CannotLoadPluging;
constexpr int Driver::ERROR_LanguageNotRecognized;

Driver::Driver()
{}

Driver::~Driver()
{}

int Driver::execute(int argc, char* argv[])
{
    cxxopts::Options cmdLineOpts(argv[0], "cnippet");
    cmdLineOpts
        .positional_help("file")
        .add_options()
            ("file",
                "The input file(s) path(s).",
                cxxopts::value<std::vector<std::string>>())
            ("l,lang",
                "Specify the programming language.",
                cxxopts::value<std::string>()->default_value("C"),
                "<C>")
            ("z,dump-AST",
                "Dump the program's AST to the console.")
            ("d,debug",
                "Enable debugging.",
                cxxopts::value<bool>(DEBUG::globalDebugEnabled))
            ("p,plugin",
                "Load plugin with the given name.",
                cxxopts::value<std::string>())
            ("h,help",
                "Print instructions.")
    ;

    ConfigurationForC::extend(cmdLineOpts);

    std::unique_ptr<CompilerFrontEnd> FE;
    std::vector<std::string> filesPaths;
    try {
        cmdLineOpts.parse_positional(std::vector<std::string>{"file"});
        auto parsedCmdLine = cmdLineOpts.parse(argc, argv);

        if (parsedCmdLine.count("help")) {
            std::cout << cmdLineOpts.help({"", "Group"}) << std::endl;
            return SUCCESS;
        }

        if (parsedCmdLine.count("plugin")) {
            auto pluginName = parsedCmdLine["plugin"].as<std::string>();
            Plugin::load(pluginName);
            if (!Plugin::isLoaded()) {
                std::cerr << kCnip << "cannot load plugin " << pluginName << std::endl;
                return ERROR_CannotLoadPluging;
            }
        }

        if (parsedCmdLine.count("file"))
            filesPaths = parsedCmdLine["file"].as<std::vector<std::string>>();
        else {
            std::cerr << kCnip << "no input file(s)" << std::endl;
            return ERROR_NoInputFile;
        }

        auto lang = parsedCmdLine["lang"].as<std::string>();
        if (lang != "C") {
            std::cerr << kCnip << "language " << lang << " not recognized" << std::endl;
            return ERROR_LanguageNotRecognized;
        }

        FE.reset(new CCompilerFrontEnd(parsedCmdLine));
    }
    catch (...) {
        std::cerr << kCnip << "unrecognized command-line option" << std::endl;
        return ERROR_UnrecognizedCmdLineOption;
    }

    for (auto filePath : filesPaths) {
        auto [exit, srcText] = readFile(filePath);
        if (exit != 0)
            return ERROR_FileNotFound;

        FileInfo fi(filePath);

        try {
            exit = FE->run(srcText, fi);
        }
        catch (...) {
            Plugin::unload();
            return Driver::ERROR;
        }

        if (exit != 0)
            return exit;
    }

    return SUCCESS;
}
