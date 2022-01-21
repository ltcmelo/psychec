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

#include "Frontend_C.h"

#include "CompilerFacade.h"
#include "FileInfo.h"
#include "IO.h"
#include "Plugin.h"

#include "compilation/Compilation.h"
#include "plugin-api/SourceInspector.h"
#include "syntax/SyntaxNamePrinter.h"

using namespace cnip;
using namespace psy;
using namespace C;

namespace DEBUG
{
bool globalDebugEnabled = false;
}

namespace
{
const char * const kInclude = "#include";
} // anonymous

constexpr int FrontEnd_C::ERROR_PreprocessorInvocationFailure;
constexpr int FrontEnd_C::ERROR_PreprocessedFileWritingFailure;
constexpr int FrontEnd_C::ERROR_UnsuccessfulParsing;
constexpr int FrontEnd_C::ERROR_InvalidSyntaxTree;

FrontEnd_C::FrontEnd_C(const cxxopts::ParseResult& parsedCmdLine)
    : FrontEnd(parsedCmdLine)
    , config_(new Configuration(parsedCmdLine["file"].as<std::vector<std::string>>()[0]))
{
    config_->C_dumpAST = parsedCmdLine.count("dump-AST");

    config_->C_hostCC_ = parsedCmdLine["cc"].as<std::string>();

    auto cc_std = parsedCmdLine["cc-std"].as<std::string>();
    std::for_each(cc_std.begin(),
                  cc_std.end(),
                  [] (char& c) { c = ::tolower(c); });
    if (cc_std == "c89" || cc_std == "c90")
        config_->STD_ = LanguageDialect::Std::C89_90;
    else if (cc_std == "c99")
        config_->STD_ = LanguageDialect::Std::C99;
    else if (cc_std == "c17" || cc_std == "c18")
        config_->STD_ = LanguageDialect::Std::C17_18;
    else
        config_->STD_ = LanguageDialect::Std::C11;

    config_->C_pp_ = parsedCmdLine["cc-pp"].as<bool>();
    if (parsedCmdLine.count("cc-D"))
        config_->C_macroDefs_ = parsedCmdLine["cc-D"].as<std::vector<std::string>>();
    if (parsedCmdLine.count("cc-U"))
        config_->C_macroUndefs_ = parsedCmdLine["cc-U"].as<std::vector<std::string>>();
    if (parsedCmdLine.count("cc-I"))
        config_->C_searchPaths_ = parsedCmdLine["cc-I"].as<std::vector<std::string>>();

    config_->C_infer = parsedCmdLine.count("C-infer");
    config_->C_inferOnly = parsedCmdLine.count("C-infer-only");
}

int FrontEnd_C::run(const std::string& source)
{
    if (source.empty())
         return 0;

    try {
        return run_CORE(source);
    }
    catch (...) {
        Plugin::unload();
        return Driver::ERROR;
    }
}

int FrontEnd_C::run_CORE(std::string source)
{
//    if (config_->C_infer
//            || config_->C_inferOnly) {
//        inferMode_ = true;
//    }

    auto [includes, source_P] = extendSource(source);

    if (config_->C_pp_) {
        auto [exit, source_PP] = invokePreprocessor(source_P);
        if (exit != 0)
            return exit;
        source_P = source_PP;
    }

    auto [exit, tree] = invokeParser(source_P);
    if (exit != 0)
        return exit;

    return invokeBinder(std::move(tree));
}

std::pair<std::string, std::string> FrontEnd_C::extendSource(
        const std::string &source)
{
    std::string includes;
    std::istringstream iss(source);
    std::string line;
    while (std::getline(iss, line)) {
        line.erase(0, line.find_first_not_of(' '));
        if (line.find(kInclude) == 0)
            includes += line + '\n';
    }

    if (!Plugin::isLoaded()) {
//        if (inferMode_)
//            std::cout << kCnip << "stdlib names will be treated as ordinary identifiers" << std::endl;
        return std::make_pair(includes, source);
    }

    SourceInspector* inspector = Plugin::createInspector();
    auto headerNames = inspector->detectRequiredHeaders(source);
    if (headerNames.empty())
        return std::make_pair(includes, source);

    std::string extSource;
    extSource += "\n/* CNIPPET: Start of #include section */\n";
    for (const auto& name : headerNames) {
        auto line = std::string(kInclude) + " <" + name + ">\n";
        extSource += line;
        includes += line;
    }
    extSource += "\n/* End of #include section */\n\n";
    extSource += source;

    return std::make_pair(includes, extSource);
}

std::pair<int, std::string> FrontEnd_C::invokePreprocessor(std::string source)
{
    CompilerFacade cc(config_->C_hostCC_,
                      to_string(config_->STD_),
                      config_->C_macroDefs_,
                      config_->C_macroUndefs_);

    auto [exit, ppSource] = cc.preprocess(source);
    if (exit != 0) {
        std::cerr << kCnip << "preprocessor invocation failed" << std::endl;
        return std::make_pair(ERROR_PreprocessorInvocationFailure, "");
    }

    exit = writeFile(config_->FI_.fullFileBaseName() + ".i", ppSource);
    if (exit != 0) {
        std::cerr << kCnip << "preprocessed file write failure" << std::endl;
        return std::make_pair(ERROR_PreprocessedFileWritingFailure, "");
    }

    return std::make_pair(0, ppSource);
}

std::pair<int, std::unique_ptr<SyntaxTree>> FrontEnd_C::invokeParser(const std::string& source)
{
    auto tree = SyntaxTree::parseText(source,
                                      ParseOptions(),
                                      config_->FI_.fileName());

    if (!tree) {
        std::cerr << "unsuccessful parsing" << std::endl;
        return std::make_pair(ERROR_UnsuccessfulParsing, nullptr);
    }

    TranslationUnitSyntax* TU = tree->translationUnitRoot();
    if (!TU) {
        std::cerr << "invalid syntax tree" << std::endl;
        return std::make_pair(ERROR_InvalidSyntaxTree, nullptr);
    }

    if (!tree->diagnostics().empty()) {
        auto c = tree->diagnostics();
        std::copy(c.begin(), c.end(),
                  std::ostream_iterator<Diagnostic>(std::cerr));
        std::cerr << std::endl;
    }

    if (config_->C_dumpAST) {
        std::ostringstream ossTree;
        SyntaxNamePrinter printer(tree.get());
        printer.print(TU,
                      SyntaxNamePrinter::Style::Decorated,
                      ossTree);
        std::cout << ossTree.str() << std::endl;
    }

    return std::make_pair(0, std::move(tree));
}

int FrontEnd_C::invokeBinder(std::unique_ptr<SyntaxTree> tree)
{
    auto compilation = Compilation::create(tree->filePath());
    compilation->addSyntaxTrees({ tree.get() });
    /*auto semaModel = */compilation->semanticModel(tree.get());

    // show only not yet shown
    if (!tree->diagnostics().empty()) {
        auto c = tree->diagnostics();
        std::copy(c.begin(), c.end(),
                  std::ostream_iterator<Diagnostic>(std::cerr));
        std::cerr << std::endl;
    }

    return 0;
}
