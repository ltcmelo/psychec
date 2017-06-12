/******************************************************************************
 * Copyright (c) 2016 Leandro T. C. Melo (ltcmelo@gmail.com)
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301
 * USA
 *****************************************************************************/

#include "Control.h"
#include "Debug.h"
#include "Driver.h"
#include "Factory.h"
#include "IO.h"
#include "Literals.h"
#include "Tester.h"
#include "TranslationUnit.h"
#include "cxxopts.hpp"
#include <iostream>
#include <string.h>

using namespace CPlusPlus;
using namespace psyche;

/*!
 * \brief main
 * \return
 */
int main(int argc, char* argv[])
{
    ExecutionFlags flags;
    std::string in;
    std::string out;
    std::string mode;

    try {
        cxxopts::Options cmdLine(argv[0], "PsycheC constraint generator");
        cmdLine.positional_help("file");

        cmdLine.add_options()
            ("o,output", "Output file",
                cxxopts::value<std::string>()->default_value("a.cstr"))
            ("m,mode", "Run mode: dev, pro",
                cxxopts::value<std::string>()->default_value("pro"))
            ("c,cstr", "Display constraints")
            ("s,stats", "Display stats")
            ("d,debug", "Enable debugging statements",
                cxxopts::value<bool>(debugEnabled))
            ("r,strict", "Disable heuristics on unresolved AST ambiguities")
            ("t,test", "Test AST disambiguation")
            ("p,dump-ast", "Dump AST in .dot format")
            ("h,help", "Print help")
            ("positional", "Positional arguments",
                cxxopts::value<std::vector<std::string>>());

        cmdLine.parse_positional(std::vector<std::string>{"file", "positional"});
        cmdLine.parse(argc, argv);

        if (cmdLine.count("test"))
            flags.flag_.testOnly = 1;
        if (cmdLine.count("dump-ast"))
            flags.flag_.dumpAst = 1;
        if (cmdLine.count("cstr"))
            flags.flag_.displayCstr = 1;
        if (cmdLine.count("stats"))
            flags.flag_.displayStats = 1;
        if (cmdLine.count("strict"))
            flags.flag_.nonHeuristic = 1;

        if (cmdLine.count("help")
                || (!cmdLine.count("positional") && (!flags.flag_.testOnly))) {
            std::cout << cmdLine.help({"", "Group"}) << std::endl;
            return 0;
        }

        // The input file name is the single positional argument.
        if (cmdLine.count("positional")) {
            auto& v = cmdLine["positional"].as<std::vector<std::string>>();
            in = v[0];
        }

        // Handle GNU's error variadic function by default. TODO: POSIX stuff too.
        flags.flag_.handleGNUerrorFunc_ = true;

        mode = cmdLine["mode"].as<std::string>();
        out = cmdLine["output"].as<std::string>();
    } catch (const cxxopts::OptionException& e) {
        std::cout << "Error parsing options: " << e.what() << std::endl;
        return 1;
    }

    if (flags.flag_.testOnly || mode == "dev") {
        try {
            Tester tester;
            tester.testAll();
            std::cout << "AST disambiguation tests passed successfully!\n"
                      << "For type-inference tests: $cd solver && stack test" << std::endl;
            if (flags.flag_.testOnly)
                return 0;
        } catch (...) {
            std::cout << "\nYou BROKE stuff! Take a look at it!" << std::endl;
            return 0;
        }
    }

    const std::string& source = readFile(in);
    Control control;
    Factory factory;
    auto result = process(in, source, control, flags, &factory);
    const auto code = std::get<0>(result);
    if (code == kParsingFailed)
        std::cout << "Parsing failed" << std::endl;
    else if (code == kSyntaxErrors)
        std::cout << "Source has syntax errors" << std::endl;
    else if (code == kInvalidAST)
        std::cout << "No AST" << std::endl;
    else if (code == kProgramAmbiguous)
        std::cout << "Code has unresolved ambiguities" << std::endl;
    else
        writeFile(std::get<2>(result), out);

    return code;
}
