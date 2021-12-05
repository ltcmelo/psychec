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

#include "Executer_C.h"

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

constexpr int Executer_C::ERROR_PreprocessorInvocationFailure;
constexpr int Executer_C::ERROR_PreprocessedFileWritingFailure;
constexpr int Executer_C::ERROR_UnsuccessfulParsing;
constexpr int Executer_C::ERROR_InvalidSyntaxTree;

int Executer_C::execute(const std::string& source)
{
    if (source.empty())
         return 0;

    try {
        return executeCore(source);
    }
    catch (...) {
        Plugin::unload();
        return Driver::ERROR;
    }
}

int Executer_C::executeCore(std::string source)
{
    if (driver_->config_->C_infer
            || driver_->config_->C_inferOnly) {
        inferMode_ = true;
    }

    auto [includes, source_P] = extendSource(source);

    if (driver_->config_->C_pp_) {
        auto [exit, source_PP] = invokePreprocessor(source_P);
        if (exit != 0)
            return exit;
        source_P = source_PP;
    }

    auto [exit, tree] = invokeParser(source_P);
    if (exit != 0)
        return exit;

    return invokeCompiler(std::move(tree));
}

std::pair<std::string, std::string> Executer_C::extendSource(
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
        if (inferMode_)
            std::cout << kCnip << "stdlib names will be treated as ordinary identifiers" << std::endl;
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

std::pair<int, std::string> Executer_C::invokePreprocessor(std::string source)
{
    CompilerFacade cc(driver_->config_->C_hostCC_,
                      to_string(driver_->config_->C_std),
                      driver_->config_->C_macroDefs_,
                      driver_->config_->C_macroUndefs_);

    auto [exit, ppSource] = cc.preprocess(source);
    if (exit != 0) {
        std::cerr << kCnip << "preprocessor invocation failed" << std::endl;
        return std::make_pair(ERROR_PreprocessorInvocationFailure, "");
    }

    exit = writeFile(driver_->config_->input_.fullFileBaseName() + ".i", ppSource);
    if (exit != 0) {
        std::cerr << kCnip << "preprocessed file write failure" << std::endl;
        return std::make_pair(ERROR_PreprocessedFileWritingFailure, "");
    }

    return std::make_pair(0, ppSource);
}

std::pair<int, std::unique_ptr<SyntaxTree>> Executer_C::invokeParser(const std::string& source)
{
    auto tree = SyntaxTree::parseText(source,
                                      ParseOptions(),
                                      driver_->config_->input_.fileName());

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

    if (driver_->config_->C_dumpAST) {
        std::ostringstream ossTree;
        SyntaxNamePrinter printer(tree.get());
        printer.print(TU,
                      SyntaxNamePrinter::Style::Decorated,
                      ossTree);
        std::cout << ossTree.str() << std::endl;
    }

    return std::make_pair(0, std::move(tree));
}

int Executer_C::invokeCompiler(std::unique_ptr<SyntaxTree> tree)
{
    auto compilation = Compilation::create(tree->filePath(), std::move(tree));
    /*auto semaModel = */compilation->semanticModel();

    // show only not yet shown
    if (!compilation->syntaxTree()->diagnostics().empty()) {
        auto c = compilation->syntaxTree()->diagnostics();
        std::copy(c.begin(), c.end(),
                  std::ostream_iterator<Diagnostic>(std::cerr));
        std::cerr << std::endl;
    }

    return 0;
}
