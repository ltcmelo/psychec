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

#include "Driver.h"

#include "cnippet/CommandLineParser.h"
#include "cnippet/Plugin.h"
#include "compiler_support/gnu/GNUCompilerFacade.h"
#include "syntax/SyntaxNamePrinter.h"
#include "utility/IO.h"

#include <algorithm>
#include <iostream>
#include <iterator>
#include <sstream>
#include <cstring>
#include <unistd.h>

using namespace psy;
using namespace cnip;

Driver::Driver()
{}

Driver::~Driver()
{}

int Driver::go(int argc, char* argv[])
{
    CommandLineParser cmdLineParser;
    CommandOptions cmdOpts;
    std::vector<std::string> subCmdArgs;
    try {
        std::tie(cmdOpts, subCmdArgs) = cmdLineParser.parseCommandLine(argc, argv);
    }
    catch (const std::exception& ex) {
        std::cerr << kCnip << ex.what() << std::endl;
        return 1;
    }

    if (cmdOpts.help_) {
        cmdLineParser.printHelp();
        return 0;
    }

    if (cmdOpts.cFilePaths_.empty() && cmdOpts.iFilePaths_.empty()) {
        std::cerr << kCnip << "no input files" << std::endl;
        return 1;
    }

    Files files;
    auto validatePaths =
            [&] (const std::vector<std::string>& filePaths) {
        for (const auto& filePath : filePaths) {
            auto [exit, text] = readFile(filePath);
            if (exit != 0) {
                std::cerr << kCnip << "no such file: " << filePath << std::endl;
                return false;
            }
            FileInfo fi(filePath);
            FileText ft;
            if (fi.fileSuffix() == "i")
                ft.i_ = text;
            else
                ft.c_ = text;
            files.push_back(std::make_pair(fi, ft));
        }
        return true;
    };

    if (validatePaths(cmdOpts.cFilePaths_)
            && validatePaths(cmdOpts.iFilePaths_)
            && runPreCPP(cmdOpts, files)
            && runCPP(cmdOpts, files)
            && runCFrontEnd(cmdOpts, files)
            && !compilation_->isEmpty()
            && analyze(cmdOpts)) {
        return subCmdArgs.empty()
                ? 0
                : executeSubCommand(subCmdArgs);
    }

    return 1;
}

bool Driver::runPreCPP(const CommandOptions& cmdOpts, Files& files) const
{
    // TODO: Move to stdlib-support.
    if (cmdOpts.includeStdLibHeaders_) {
        for (auto& f : files) {
            const auto& text = f.second.c_;
            std::istringstream iss(text);
            std::string line;
            std::string existingHeaders;
            while (std::getline(iss, line)) {
                line.erase(0, line.find_first_not_of(' '));
                if (line.find("#include") == 0)
                    existingHeaders += line + '\n';
            }
#if 0
            SourceInspector* inspector = Plugin::createInspector();
            auto stdLibHeaders = inspector->detectRequiredHeaders(srcText);
            if (stdLibHeaders.empty())
                return preprocess(srcText, fi);

            std::string srcText_P;
            srcText_P += "\n/* psychec: Start of #include section */\n";
            for (const auto& name : stdLibHeaders) {
                auto line = std::string(kInclude) + " <" + name + ">\n";
                srcText_P += line;
                existingHeaders += line;
            }
            srcText_P += "\n/* End of #include section */\n\n";
            srcText_P += srcText;
#endif
        }
    }

    return true;
}

bool Driver::runCPP(const CommandOptions& cmdOpts, Files& files) const
{
    if (cmdOpts.ppMode_ == "none") {
        for (auto& file : files) {
            const auto& fi = file.first;
            if (fi.fileSuffix() != "i")
                file.second.i_ = file.second.c_;
        }
        return true;
    }

    bool strictPP;
    if (cmdOpts.ppMode_ == "s")
        strictPP = true;
    else if (cmdOpts.ppMode_ == "r")
        strictPP = false;
    else {
        std::cerr << kCnip << "unrecognized preprocessing mode: "
                  << cmdOpts.ppMode_ << std::endl;
        return false;
    }

    gnu::CompilerFacade compiler(cmdOpts.compiler_,
                                 cmdOpts.std_,
                                 cmdOpts.gnuDirSearchOpts_,
                                 cmdOpts.gnuPPCmdOpts_);

    for (auto& f : files) {
        const auto& fi = f.first;
        if (fi.fileSuffix() == "i")
            continue;
        std::string text;
        int exit;
        if (strictPP)
            std::tie(exit, text) = compiler.preprocessFile(fi.fullFileName());
        else {
            // TODO: Make it per include tolerant.
            std::tie(exit, text) = compiler.preprocess_IgnoreIncludes(text);
        }
        if (exit != 0) {
            std::cerr << kCnip << "preprocessing failed" << std::endl;
            return false;
        }
        f.second.i_ = text;
    }

    return true;
}

bool Driver::runCFrontEnd(const CommandOptions& cmdOpts, Files& files)
{
    LanguageDialect::Std std;
    if (cmdOpts.std_ == "c89" || cmdOpts.std_ == "c90")
        std = LanguageDialect::Std::C89_90;
    else if (cmdOpts.std_ == "c99")
        std = LanguageDialect::Std::C99;
    else if (cmdOpts.std_ == "c17" || cmdOpts.std_ == "c18")
        std = LanguageDialect::Std::C17_18;
    else if (cmdOpts.std_ == "c11")
        std = LanguageDialect::Std::C11;
    else {
        std::cerr << kCnip << "unsupported C Standard: "
                  << cmdOpts.std_ << std::endl;
        return false;
    }

    // TODO
    ParseOptions parseOpts{ LanguageDialect(std) };

    if (cmdOpts.disambigMode_ == "a")
        parseOpts.setDisambiguationMode(
                ParseOptions::DisambiguationMode::Algorithmic);
    else if (cmdOpts.disambigMode_ == "h")
        parseOpts.setDisambiguationMode(
                ParseOptions::DisambiguationMode::Heuristic);
    else if (cmdOpts.disambigMode_ == "ah")
        parseOpts.setDisambiguationMode(
                ParseOptions::DisambiguationMode::AlgorithmicAndHeuristic);
    else if (cmdOpts.disambigMode_ == "none")
        parseOpts.setDisambiguationMode(
                ParseOptions::DisambiguationMode::None);
    else {
        std::cerr << kCnip << "unrecognized disambiguation mode: "
                  << cmdOpts.disambigMode_ << std::endl;
        return false;
    }

    if (cmdOpts.commentMode_ == "ka")
        parseOpts.setCommentMode(ParseOptions::CommentMode::KeepAll);
    else if (cmdOpts.commentMode_ == "kdo")
        parseOpts.setCommentMode(ParseOptions::CommentMode::KeepDocumentationOnly);
    if (cmdOpts.commentMode_ == "d")
        parseOpts.setCommentMode(ParseOptions::CommentMode::Discard);
    else {
        std::cerr << kCnip << "unrecognized comment mode: "
                  << cmdOpts.disambigMode_ << std::endl;
        return false;
    }

    auto diagnose = [](const SyntaxTree* tree, auto... categories) {
        if (tree->diagnostics().empty())
            return std::make_pair(false, false);
        bool error = false, warn = false;
        auto diags = tree->diagnostics();
        std::copy_if(diags.begin(),
                     diags.end(),
                     std::ostream_iterator<Diagnostic>(std::cerr),
                     [&error, &warn, categories...] (const Diagnostic& diag) {
                         if (diag.severity() == DiagnosticSeverity::Error)
                             error = true;
                         else if (diag.severity() == DiagnosticSeverity::Warning)
                             warn = true;
                         else
                             return false;
                         auto category = diag.descriptor().category();
                         return ((category == categories) || ...);
                     });
        std::cerr << std::endl;
        return std::make_pair(error, warn);
    };

    compilation_ = Compilation::create("<cnippet-compilation>");

    for (const auto& f : files) {
        const auto& fi = f.first;
        const auto& text = f.second.i_;
        auto tree = SyntaxTree::parseText(
                    text,
                    TextPreprocessingState::Preprocessed,
                    TextCompleteness::Fragment,
                    parseOpts,
                    fi.fileName());
        if (!tree) {
            std::cerr << "cannot create syntax tree" << std::endl;
            return false;
        }
        auto TU = tree->translationUnit();
        if (!TU) {
            std::cerr << "syntax tree root is invalid" << std::endl;
            return false;
        }

        auto [error, _] = diagnose(tree.get(), DiagnosticCategory::Syntax);
        if (error)
            return false;

        if (cmdOpts.dumpAST_) {
            std::ostringstream oss;
            SyntaxNamePrinter printer(tree.get());
            printer.print(TU, SyntaxNamePrinter::Style::Decorated, oss);
            std::cout << oss.str() << std::endl;
        }

        if (cmdOpts.syntax_only_)
            continue;

        auto tree_RAW = tree.get();
        compilation_->addSyntaxTree(std::move(tree));
        compilation_->computeSemanticModel(tree_RAW);

        std::tie(error, _) = diagnose(
                    tree_RAW,
                    DiagnosticCategory::Binding,
                    DiagnosticCategory::TypeResolution,
                    DiagnosticCategory::TypeChecking);
        if (error)
            return false;
    }

    return true;
}

bool Driver::analyze(const CommandOptions& cmdOpts) const
{
    for (const auto& path : cmdOpts.analysisFilePaths_) {
        auto handle = Plugin::loadAnalysis(path);
        if (!handle) {
            std::cerr << kCnip << "cannot load analysis" << std::endl;
            return false;
        }
        auto r = Plugin::analyze(handle, compilation_.get());
        if (r < 0) {
            std::cerr << kCnip << "analysis failed" << std::endl;
            return false;
        }
    }
    return true;
}

int Driver::executeSubCommand(const std::vector<std::string>& subCmdArgs) const
{
    std::vector<char*> args;
    for (const auto& s : subCmdArgs)
        args.push_back(strdup(s.c_str()));
    args.push_back(nullptr);
    return execvp(args[0], args.data());
}
