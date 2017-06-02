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
#include "IO.h"
#include "Literals.h"
#include "Runner.h"
#include "Tester.h"
#include "TranslationUnit.h"
#include "cxxopts.hpp"
#include <iostream>
#include <memory>
#include <string.h>

using namespace CPlusPlus;
using namespace psyche;

/*!
 * \brief main
 * \return
 */
int main(int argc, char* argv[])
{
    ProgramCommand cmd;
    std::string file;
    std::string mode;

    try {
        cxxopts::Options options(argv[0], "PsycheC constraint generator");
        options.positional_help("file");

        options.add_options()
            ("o,output", "Output file",
                cxxopts::value<std::string>()->default_value("a.cstr"))
            ("m,mode", "Run mode: dev, prod",
                cxxopts::value<std::string>()->default_value("prod"))
            ("c,cstr", "Display constraints")
            ("s,stats", "Display stats")
            ("d,debug", "Enable debugging statements",
                cxxopts::value<bool>(debugEnabled))
            ("t,test", "Check tests only")
            ("p,dump-ast", "Dump AST in .dot format")
            ("h,help", "Print help")
            ("positional", "Positional arguments",
                cxxopts::value<std::vector<std::string>>());

        options.parse_positional(std::vector<std::string>{"file", "positional"});
        options.parse(argc, argv);

        if (options.count("test"))
            cmd.flag_.testOnly = 1;
        if (options.count("dump-ast"))
            cmd.flag_.dumpAst = 1;
        if (options.count("cstr"))
            cmd.flag_.displayCstr = 1;
        if (options.count("stats"))
            cmd.flag_.displayStats = 1;

        if (options.count("help")
                || (!options.count("positional") && (!cmd.flag_.testOnly))) {
            std::cout << options.help({"", "Group"}) << std::endl;
            return 0;
        }

        // The input file name is the single positional argument.
        if (options.count("positional")) {
            auto& v = options["positional"].as<std::vector<std::string>>();
            file = v[0];
        }

        mode = options["mode"].as<std::string>();
        cmd.output_ = options["output"].as<std::string>();
    } catch (const cxxopts::OptionException& e) {
        std::cout << "Error parsing options: " << e.what() << std::endl;
        return 1;
    }

    if (cmd.flag_.testOnly || mode == "dev") {
        try {
            Tester tester;
            tester.testAll();
            std::cout << "Tests passed successfully!" << std::endl;
            if (cmd.flag_.testOnly)
                return 0;
        } catch (...) {
            std::cout << "\nYou BROKE stuff! Take a look at it!" << std::endl;
            return 0;
        }
    }

    StringLiteral name(file.c_str(), file.length());
    const std::string& source = readFile(file);
    Control control;
    process(source, control, name, cmd);

    return 0;
}
