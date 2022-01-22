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

#include "CompilerFrontEnd_C.h"

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

constexpr int CCompilerFrontEnd::ERROR_PreprocessorInvocationFailure;
constexpr int CCompilerFrontEnd::ERROR_PreprocessedFileWritingFailure;
constexpr int CCompilerFrontEnd::ERROR_UnsuccessfulParsing;
constexpr int CCompilerFrontEnd::ERROR_InvalidSyntaxTree;

CCompilerFrontEnd::CCompilerFrontEnd(const cxxopts::ParseResult& parsedCmdLine)
    : CompilerFrontEnd()
    , config_(new CCompilerConfiguration(parsedCmdLine))
{}

CCompilerFrontEnd::~CCompilerFrontEnd()
{}

int CCompilerFrontEnd::run(const std::string& srcText, const FileInfo& fi)
{
    if (srcText.empty())
         return 0;

    try {
        return run_CORE(srcText, fi);
    }
    catch (...) {
        Plugin::unload();
        return Driver::ERROR;
    }
}

int CCompilerFrontEnd::run_CORE(const std::string& srcText, const FileInfo& fi)
{
//    if (config_->C_infer
//            || config_->C_inferOnly) {
//        inferMode_ = true;
//    }

    auto [includes, srcText_P] = extendSource(srcText);

    if (config_->pp) {
        auto [exit, srcText_PP] = invokePreprocessor(srcText_P, fi);
        if (exit != 0)
            return exit;
        srcText_P = srcText_PP;
    }

    auto [exit, tree] = invokeParser(srcText_P, fi);
    if (exit != 0)
        return exit;

    return invokeBinder(std::move(tree));
}

std::pair<std::string, std::string> CCompilerFrontEnd::extendSource(
        const std::string &srcText)
{
    std::string includes;
    std::istringstream iss(srcText);
    std::string line;
    while (std::getline(iss, line)) {
        line.erase(0, line.find_first_not_of(' '));
        if (line.find(kInclude) == 0)
            includes += line + '\n';
    }

    if (!Plugin::isLoaded()) {
//        if (inferMode_)
//            std::cout << kCnip << "stdlib names will be treated as ordinary identifiers" << std::endl;
        return std::make_pair(includes, srcText);
    }

    SourceInspector* inspector = Plugin::createInspector();
    auto headerNames = inspector->detectRequiredHeaders(srcText);
    if (headerNames.empty())
        return std::make_pair(includes, srcText);

    std::string extSource;
    extSource += "\n/* CNIPPET: Start of #include section */\n";
    for (const auto& name : headerNames) {
        auto line = std::string(kInclude) + " <" + name + ">\n";
        extSource += line;
        includes += line;
    }
    extSource += "\n/* End of #include section */\n\n";
    extSource += srcText;

    return std::make_pair(includes, extSource);
}

std::pair<int, std::string> CCompilerFrontEnd::invokePreprocessor(const std::string& srcText, const FileInfo& fi)
{
    CompilerFacade cc(config_->hostCompiler,
                      to_string(config_->langStd_),
                      config_->ppD,
                      config_->ppU);

    auto [exit, ppSource] = cc.preprocess(srcText);
    if (exit != 0) {
        std::cerr << kCnip << "preprocessor invocation failed" << std::endl;
        return std::make_pair(ERROR_PreprocessorInvocationFailure, "");
    }

    exit = writeFile(fi.fullFileBaseName() + ".i", ppSource);
    if (exit != 0) {
        std::cerr << kCnip << "preprocessed file write failure" << std::endl;
        return std::make_pair(ERROR_PreprocessedFileWritingFailure, "");
    }

    return std::make_pair(0, ppSource);
}

std::pair<int, std::unique_ptr<SyntaxTree>> CCompilerFrontEnd::invokeParser(
            const std::string& srcText,
            const FileInfo& fi)
{
    auto tree = SyntaxTree::parseText(srcText,
                                      ParseOptions(),
                                      fi.fileName());

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

    if (config_->dumpAst) {
        std::ostringstream ossTree;
        SyntaxNamePrinter printer(tree.get());
        printer.print(TU,
                      SyntaxNamePrinter::Style::Decorated,
                      ossTree);
        std::cout << ossTree.str() << std::endl;
    }

    return std::make_pair(0, std::move(tree));
}

int CCompilerFrontEnd::invokeBinder(std::unique_ptr<SyntaxTree> tree)
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
