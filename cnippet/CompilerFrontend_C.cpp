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

#include "CompilerFrontend_C.h"

#include "FileInfo.h"
#include "GnuCompilerFacade.h"
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
}

constexpr int CCompilerFrontend::ERROR_PreprocessorInvocationFailure;
constexpr int CCompilerFrontend::ERROR_PreprocessedFileWritingFailure;
constexpr int CCompilerFrontend::ERROR_UnsuccessfulParsing;
constexpr int CCompilerFrontend::ERROR_InvalidSyntaxTree;

CCompilerFrontend::CCompilerFrontend(const cxxopts::ParseResult& parsedCmdLine)
    : CompilerFrontend()
    , config_(new ConfigurationForC(parsedCmdLine))
{}

CCompilerFrontend::~CCompilerFrontend()
{}

int CCompilerFrontend::run(const std::string& srcText, const FileInfo& fi)
{
    if (srcText.empty())
         return 0;

    return config_->inferMissingTypes ? extendWithStdLibHeaders(srcText, fi)
                                          : preprocess(srcText, fi);
}

int CCompilerFrontend::extendWithStdLibHeaders(const std::string& srcText,
                                               const psy::FileInfo& fi)
{
    if (!Plugin::isLoaded())
        return 1;

    std::istringstream iss(srcText);
    std::string line;
    std::string existingHeaders;
    while (std::getline(iss, line)) {
        line.erase(0, line.find_first_not_of(' '));
        if (line.find(kInclude) == 0)
            existingHeaders += line + '\n';
    }

    SourceInspector* inspector = Plugin::createInspector();
    auto stdLibHeaders = inspector->detectRequiredHeaders(srcText);
    if (stdLibHeaders.empty())
        return preprocess(srcText, fi);

    std::string srcText_P;
    srcText_P += "\n/* CNIPPET: Start of #include section */\n";
    for (const auto& name : stdLibHeaders) {
        auto line = std::string(kInclude) + " <" + name + ">\n";
        srcText_P += line;
        existingHeaders += line;
    }
    srcText_P += "\n/* End of #include section */\n\n";
    srcText_P += srcText;

    return preprocess(srcText_P, fi);
}

int CCompilerFrontend::preprocess(const std::string& srcText,
                                  const psy::FileInfo& fi)
{
    GnuCompilerFacade cc(config_->hostCompiler,
                      to_string(config_->langStd),
                      config_->macrosToDefine,
                      config_->macrosToUndef);

    std::string srcText_P;
    int exit;
    if (config_->expandIncludes) {
        std::tie(exit, srcText_P) = cc.preprocess(srcText);
        if (exit != 0) {
            std::cerr << kCnip << "preprocessor invocation failed" << std::endl;
            return ERROR_PreprocessorInvocationFailure;
        }

        exit = writeFile(fi.fullFileBaseName() + ".i", srcText_P);
        if (exit != 0) {
            std::cerr << kCnip << "preprocessed file write failure" << std::endl;
            return ERROR_PreprocessedFileWritingFailure;
        }
    }
    else {
        std::tie(exit, srcText_P) = cc.preprocess_IgnoreIncludes(srcText);
    }

    return constructSyntaxTree(srcText_P, fi);
}

int CCompilerFrontend::constructSyntaxTree(const std::string& srcText,
                                           const psy::FileInfo& fi)
{
    ParseOptions parseOpts;

    // TODO: Move to driver/config.
    if (!config_->ParseOptions_TreatmentOfAmbiguities.empty()) {
        if (config_->ParseOptions_TreatmentOfAmbiguities == "None")
            parseOpts.setTreatmentOfAmbiguities(ParseOptions::TreatmentOfAmbiguities::None);
        else if (config_->ParseOptions_TreatmentOfAmbiguities == "Diagnose")
            parseOpts.setTreatmentOfAmbiguities(ParseOptions::TreatmentOfAmbiguities::Diagnose);
        else if (config_->ParseOptions_TreatmentOfAmbiguities == "DisambiguateAlgorithmically")
            parseOpts.setTreatmentOfAmbiguities(ParseOptions::TreatmentOfAmbiguities::DisambiguateAlgorithmically);
        else if (config_->ParseOptions_TreatmentOfAmbiguities == "DisambiguateAlgorithmicallyOrHeuristically")
            parseOpts.setTreatmentOfAmbiguities(ParseOptions::TreatmentOfAmbiguities::DisambiguateAlgorithmicallyOrHeuristically);
        else if (config_->ParseOptions_TreatmentOfAmbiguities == "DisambiguateHeuristically")
            parseOpts.setTreatmentOfAmbiguities(ParseOptions::TreatmentOfAmbiguities::DisambiguateHeuristically);
        else {
            std::cerr << "unrecognized --C-ParseOptions-TreatmentOfAmbiguities" << std::endl;
            return 1;
        }
    }

    auto tree = SyntaxTree::parseText(srcText,
                                      TextPreprocessingState::Preprocessed,
                                      TextCompleteness::Fragment,
                                      parseOpts,
                                      fi.fileName());

    if (!tree) {
        std::cerr << "unsuccessful parsing" << std::endl;
        return ERROR_UnsuccessfulParsing;
    }

    TranslationUnitSyntax* TU = tree->translationUnitRoot();
    if (!TU) {
        std::cerr << "invalid syntax tree" << std::endl;
        return ERROR_InvalidSyntaxTree;
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

    return config_->WIP_ ? computeSemanticModel(std::move(tree))
                         : 0;
}

int CCompilerFrontend::computeSemanticModel(std::unique_ptr<SyntaxTree> tree)
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
