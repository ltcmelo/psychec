// Copyright (c) 2025 Leandro T. C. Melo <ltcmelo@gmail.com>
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

#include "CommandLineParser.h"

#include <cstring>
#include <iostream>
#include <iomanip>
#include <vector>
#include <sstream>
#include <string_view>

using namespace cnip;

namespace stdx {

bool starts_with(std::string_view s, std::string_view prefix) {
    return s.size() >= prefix.size() &&
           s.substr(0, prefix.size()) == prefix;
}

bool ends_with(std::string_view str, std::string_view suffix) {
    return str.size() >= suffix.size() &&
           str.substr(str.size() - suffix.size()) == suffix;
}

} // stdx

std::tuple<CommandOptions, std::vector<std::string>>
CommandLineParser::parseCommandLine(int argc, char* argv[])
{
    std::vector<std::string> cmdArgs;
    std::vector<std::string> subCmdArgs;
    auto subCmdDelimSeen = false;
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--") {
            subCmdDelimSeen = true;
            continue;
        }
        if (subCmdDelimSeen)
            subCmdArgs.push_back(arg);
        else
            cmdArgs.push_back(arg);
    }
    CommandOptions cmdOpts;
    detectCommandOptions(cmdOpts, cmdArgs, 0, false);
    if (subCmdArgs.size() > 1) {
        cmdOpts.compiler_ = subCmdArgs[0];
        detectCommandOptions(cmdOpts, subCmdArgs, 1, true);
    }

    return std::make_tuple(cmdOpts, subCmdArgs);
}

void CommandLineParser::detectCommandOptions(
        CommandOptions& cmdOpts,
        const std::vector<std::string>& cmdArgs,
        std::vector<std::string>::size_type cmdArgsIdx,
        bool areArgsOfSubCmd)
{
    bool acceptAsCFilePath = false;
    for (; cmdArgsIdx < cmdArgs.size(); ++cmdArgsIdx) {
        const auto& s = cmdArgs[cmdArgsIdx];
        if (s[0] != '-') {
            if (stdx::ends_with(s, ".c")
                    || stdx::ends_with(s, ".h")
                    || acceptAsCFilePath)
                cmdOpts.cFilePaths_.push_back(s);
            else if (stdx::ends_with(s, ".i"))
                cmdOpts.iFilePaths_.push_back(s);
            else
                throw std::invalid_argument("unhandled file path: " + s);
            continue;
        }
        auto opt = s.substr(1);
        if (opt.empty())
            throw std::invalid_argument("expected option: `-'");

        auto valueAtOrAfter = [&](const std::string& opt, const char* optName) {
            const auto len = strlen(optName);
            if (opt.size() == len) {
                if (cmdArgsIdx + 1 >= cmdArgs.size())
                    throw std::invalid_argument("expected option value: `-" + opt + "'");
                return cmdArgs[++cmdArgsIdx];
            }
            return opt.substr(len);
        };

        auto valueAt = [&](const std::string& opt, const char* optName) {
            const auto len = strlen(optName);
            if (opt.size() == len)
                throw std::invalid_argument("expected option value: `-" + opt + "'");
            return opt.substr(len);
        };

        auto valueAfter= [&](const std::string& opt, const char* optName) {
            if (cmdArgsIdx + 1 >= cmdArgs.size())
                throw std::invalid_argument("expected option value: `-" + opt + "'");
            return cmdArgs[++cmdArgsIdx];
        };

        // GNU directory search.
        if (opt[0] == 'I')
            cmdOpts.gnuDirSearchOpts_.I_.push_back(valueAtOrAfter(opt, "I"));
        else if (stdx::starts_with(opt, "iquote"))
            cmdOpts.gnuDirSearchOpts_.iquote_.push_back(valueAtOrAfter(opt, "iquote"));
        else if (stdx::starts_with(opt, "isystem"))
            cmdOpts.gnuDirSearchOpts_.isystem_.push_back(valueAtOrAfter(opt, "isystem"));
        else if (stdx::starts_with(opt, "idirafter"))
            cmdOpts.gnuDirSearchOpts_.idirafter_.push_back(valueAtOrAfter(opt, "idirafter"));
        else if (opt == "nostdinc")
            cmdOpts.gnuDirSearchOpts_.nostdinc_ = true;

        // GNU preprocessor options
        else if (opt[0] == 'D')
            cmdOpts.gnuPPCmdOpts_.D_.push_back(valueAtOrAfter(opt, "D"));
        else if (opt[0] == 'U')
            cmdOpts.gnuPPCmdOpts_.U_.push_back(valueAtOrAfter(opt, "U"));
        else if (stdx::starts_with(opt, "include"))
            cmdOpts.gnuPPCmdOpts_.include_.push_back(valueAtOrAfter(opt, "include"));
        else if (stdx::starts_with(opt, "imacros"))
            cmdOpts.gnuPPCmdOpts_.imacros_.push_back(valueAtOrAfter(opt, "imacros"));
        else if (opt == "undef")
            cmdOpts.gnuPPCmdOpts_.undef_ = true;
        else if (opt == "C")
            cmdOpts.gnuPPCmdOpts_.C_ = true;
        else if (opt == "CC")
            cmdOpts.gnuPPCmdOpts_.CC_ = true;

        // GNU C dialect
        else if (stdx::starts_with(opt, "std="))
            cmdOpts.std_ = valueAt(opt, "std=");
        else if (stdx::starts_with(opt, "-std="))
            cmdOpts.std_ = valueAtOrAfter(opt, "std=");
        else if (opt == "-std")
            cmdOpts.std_ = valueAfter(opt, "-std");
        else if (opt == "ansi" || opt == "-ansi")
            cmdOpts.ansi_ = true;

        // GNU Miscellanea
        else if (stdx::starts_with(opt, "x")) {
            auto v = valueAtOrAfter(opt, "x");
            acceptAsCFilePath = (v == "c" || v == "c-header")
                ? true
                : false;
        }

        // GNU Compilation stages
        else if (opt == "fsyntax-only" || opt == "-syntax-only")
            cmdOpts.syntax_only_ = true;

        // Psyche-C
        else if (!areArgsOfSubCmd) {
            if (opt == "help" || opt == "-help")
                cmdOpts.help_ = true;
            else if (opt == "dump-ast")
                cmdOpts.dumpAST_ = true;
            else if (opt == "include-stdlib-headers") {
                cmdOpts.includeStdLibHeaders_ = true;
                // TODO
                throw std::invalid_argument("wip");
            }
            else if (opt == "disambiguation")
                cmdOpts.disambigMode_ = valueAfter(opt, "disambiguation");
            else if (opt == "pp")
                cmdOpts.ppMode_ = valueAfter(opt, "pp");
            else if (opt == "cc")
                cmdOpts.compiler_ = valueAfter(opt, "cc");
            else if (opt == "comment")
                cmdOpts.commentMode_ = valueAfter(opt, "comment");
            else if (opt == "analysis")
                cmdOpts.analysisFilePaths_.push_back(valueAfter(opt, "analysis"));
            else
                throw std::invalid_argument("unrecognized option: `-" + opt + "'");
        }
    }
}

namespace {

struct HelpEntry
{
    std::string option_;
    std::string description_;
};

struct Help
{
    std::map<std::string, std::vector<HelpEntry>> entryByGroup_;
};

Help makeHelp()
{
    Help h;

    h.entryByGroup_["Directory search"] = {
        { "-I <dir>", "" },
        { "-iquote <dir>", "" },
        { "-isystem <dir>", "" },
        { "-idirafter <dir>", ""},
        { "-nostdinc", ""},
    };

    h.entryByGroup_["Preprocessor options"] = {
        { "-D <name>", "" },
        { "-D <name=definition>", "" },
        { "-U <name>", "" },
        { "-include <file>", "" },
        { "-imacros <file>", ""},
        { "-undef", ""},
        { "-C", ""},
        { "-CC", ""},
    };

    h.entryByGroup_["C dialect"] = {
        { "-std=<standard>", "`c89', `c90', `c99', `c11', `c17', or `c18'." },
        { "-ansi", "" },
    };

    h.entryByGroup_["Miscellanea"] = {
        { "-x <language>", "Only `c' and `c-header' are accepted." },
    };

    h.entryByGroup_["Compilation stages"] = {
        { "-fsyntax-only", "" },
    };

    h.entryByGroup_["Psyche-C"] = {
        { "-dump-ast", "Dump AST." },
        { "-include-stdlib-headers", "Include (missing) stdlib headers." },
        { "-disambiguation <mode>",
                "Disambiguation mode: `a' (algorithmically), "\
                "`h' (heuristically), `ah' (algorithmically and heuristically), "\
                "or `none' to report ambiguities. Default is `ah'." },
        { "-pp <mode>",
                "Preprocessing mode: `s' (strict), `r' (relaxed), "\
                "or `none' to skip preprocessing. Default is `s'." },
        { "-cc <name>",
                "C compiler name, when not in sub-command (used for preprocessing): "\
                "`gcc' or `clang'. Default is `gcc'."},
        { "-comment <mode>",
                "Comment mode: `d' (discard), `ka' (keep all), "\
                "or `kdo' (keep documentation only). Default is `d'." },
        { "-analysis <path>", "Path to analysis' shared library file." }
    };

    return h;
}

} // anonymous

std::vector<std::string> wrapText(const std::string& text, size_t width) {
    std::istringstream initialText(text);
    std::string curText;
    std::string curLine;
    std::vector<std::string> lines;

    while (initialText >> curText) {
        if (curLine.size() + curText.size() + 1 > width) {
            if (!curLine.empty())
                lines.push_back(curLine);
            curLine = curText;
        } else {
            if (!curLine.empty())
                curLine += " ";
            curLine += curText;
        }
    }
    if (!curLine.empty())
        lines.push_back(curLine);
    return lines;
}

void CommandLineParser::printHelp() const
{
    auto h = makeHelp();

    std::cout << "\ncnip [OPTIONS] file.c ... [-- <gcc/clang [OPTIONS] file.c ...>]\n\n";
    std::cout << "Many OPTIONS are equivalent to those of gcc and clang.\n\n";

    constexpr int optColWidth = 30;
    constexpr int descColWidth = 60;
    for (const auto& [group, entries] : h.entryByGroup_) {
        std::cout << group << ":\n";
        for (const auto& entry : entries) {
            auto opt = wrapText(entry.option_, optColWidth);
            auto description = wrapText(entry.description_, descColWidth);

            auto maxLineCnt = std::max(opt.size(), description.size());
            for (size_t i = 0; i < maxLineCnt; ++i) {
                std::cout << std::string(2, ' ');
                if (i < opt.size())
                    std::cout << std::left << std::setw(optColWidth) << opt[i];
                else
                    std::cout << std::string(optColWidth, ' ');
                std::cout << "  ";
                if (i < description.size())
                    std::cout << description[i];
                std::cout << "\n";
            }
        }
        std::cout << "\n";
    }
}
