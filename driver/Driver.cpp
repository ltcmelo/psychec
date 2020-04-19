/******************************************************************************
 Copyright (c) 2016-20 Leandro T. C. Melo (ltcmelo@gmail.com)

 This library is free software; you can redistribute it and/or modify it under
 the terms of the GNU Lesser General Public License as published by the Free
 Software Foundation; either version 2.1 of the License, or (at your option)
 any later version.

 This library is distributed in the hope that it will be useful, but WITHOUT
 ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License
 for more details.

 You should have received a copy of the GNU Lesser General Public License along
 with this library; if not, write to the Free Software Foundation, Inc., 51
 Franklin Street, Fifth Floor, Boston, MA  02110-1301 USA
 *****************************************************************************/

#include "Driver.h"

#include "AST.h"
#include "ASTDotWriter.h"
#include "ASTNormalizer.h"
#include "BaseTester.h"
#include "Binder.h"
#include "CompilerFacade.h"
#include "ConstraintGenerator.h"
#include "ConstraintWriter.h"
#include "DeclarationInterceptor.h"
#include "Debug.h"
#include "DiagnosticCollector.h"
#include "DomainLattice.h"
#include "FileInfo.h"
#include "GenericsInstantiatior.h"
#include "IO.h"
#include "Literals.h"
#include "Plugin.h"
#include "ProgramValidator.h"
#include "SourceInspector.h"
#include "Symbols.h"
#include "cxxopts.hpp"

#include <iostream>
#include <sstream>
#include <cstring>

using namespace psyche;
using namespace psyche;
using namespace std::placeholders;

namespace psyche {
    extern bool debugEnabled;
}

constexpr int Driver::Exit_OK;
constexpr int Driver::Exit_Error;
constexpr int Driver::Exit_ParsingError_Internal;
constexpr int Driver::Exit_SyntaxError;
constexpr int Driver::Exit_ASTError_Internal;
constexpr int Driver::Exit_UnresolvedSyntaxAmbiguityError;
constexpr int Driver::Exit_UnspecifiedInputFileError;
constexpr int Driver::Exit_UnknownCommandLineOptionError;
constexpr int Driver::Exit_PluginLoadingError;
constexpr int Driver::Exit_PreprocessingError;


int main(int argc, char* argv[])
{
    Factory factory; // TODO: Will go away.
    Driver driver(factory);
    return driver.process(argc, argv);
}

namespace {

void honorFlag(bool flag, std::function<void ()> f)
{
    if (flag)
        f();
}

} // anonymous

Driver::Driver(const Factory& factory)
    : factory_(factory)
    , globalNs_(nullptr)
    , withGenerics_(true)
{}

TranslationUnit *Driver::unit() const
{
    return unit_.get();
}

TranslationUnitAST* Driver::ast() const
{
    return unit()->ast()->asTranslationUnit();
}

Dialect Driver::adjustedDialect(const Configuration&)
{
    Dialect dialect;
    dialect.c99 = 1; // TODO: Requires further work for actual customization.
    dialect.ext_KeywordsGNU = 1;
    dialect.ext_EnumeratorAttributes = 1;
    dialect.ext_AvailabilityAttribute = 1;
    dialect.nullptrOnNULL = 1;
    dialect.generics = 1;

    return dialect;
}

void Driver::configure(const Configuration& config)
{
    control_.reset();
    globalNs_ = control_.newNamespace(0, nullptr);
    config_ = config;
}

std::string Driver::augmentSource(const std::string& source, const std::vector<std::string>& headers)
{
    if (headers.empty())
        return source;

    for (const auto& h : headers)
        includes_ += "#include <" + h + ">\n";
    includes_ += "\n";

    std::string augmented = "\n/* psychec: automatic inclusion */\n";
    augmented += includes_;
    augmented += "/* psychec end: automatic inclusion */\n\n";
    augmented += source;

    return augmented;
}

int Driver::process(int argc, char *argv[])
{
    Configuration config;
    std::string in;
    cxxopts::Options options(argv[0], "PsycheC constraint generator");

    try {
        options.positional_help("file");

        options.add_options()
            ("h,help", "Print help")
            ("o,output", "Specify output file",
                cxxopts::value<std::string>()->default_value("a.cstr"))
            ("d,debug", "Enable debugging messages",
                cxxopts::value<bool>(debugEnabled))
            ("c,constraints", "Display generated constraints")
            ("a,ast", "Dump AST in .dot format")
            ("s,stats", "Display AST disambiguation and constraint-generation stats")
            ("t,test", "Run tests (except for inference)")
            ("p,plugin", "Load named plugin",
                cxxopts::value<std::string>())
            ("no-heuristic", "Disable heuristics on unresolved syntax ambiguities")
            ("no-typedef", "Forbid typedef and struct/union declarations")
            ("cc", "Specify host C compiler",
                cxxopts::value<std::string>()->default_value("gcc"))
            ("cc-std", "Specify C dialect",
                cxxopts::value<std::string>()->default_value("c99"))
            ("cc-D", "Predefine a macro",
                cxxopts::value<std::vector<std::string>>())
            ("cc-U", "Undefine a macro",
                cxxopts::value<std::vector<std::string>>())
            ("cc-I", "Specify #include path",
                cxxopts::value<std::vector<std::string>>())
            ("positional", "Positional arguments",
                cxxopts::value<std::vector<std::string>>());

        options.parse_positional(std::vector<std::string>{"file", "positional"});
        options.parse(argc, argv);
    }
    catch (const cxxopts::OptionException& e) {
        std::cerr << kPsychePrefix << e.what() << std::endl;
        return Exit_UnknownCommandLineOptionError;
    }

    if (options.count("help")) {
        std::cout << options.help({"", "Group"}) << std::endl;
        return Exit_OK;
    }

    if (options.count("test")) {
        try {
            BaseTester::runSuite();
        } catch (...) {
            std::cout << "\nTests failed." << std::endl;
            return Exit_Error;
        }
        return Exit_OK;
    }

    if (!options.count("positional")) {
        std::cerr << kPsychePrefix << "unspecified input file" << std::endl;
        return Exit_UnspecifiedInputFileError;
    }

    // The input file name is the single positional argument.
    if (options.count("positional")) {
        auto& v = options["positional"].as<std::vector<std::string>>();
        in = v[0];
    }

    config.value_.dumpAst = options.count("ast");
    config.value_.displayConstraints = options.count("constraints");
    config.value_.displayStats = options.count("stats");
    config.value_.noHeuristics = options.count("no-heuristic");
    config.value_.noTypedef = options.count("no-typedef");
    config.value_.handleGNUerrorFunc_ = true; // TODO: POSIX stuff?
    config.nativeCC_ = options["cc"].as<std::string>();
    config.dialectName_ = options["cc-std"].as<std::string>();
    config.macroDefs_ = options["cc-D"].as<std::vector<std::string>>();
    config.macroUndefs_ = options["cc-U"].as<std::vector<std::string>>();
    config.includePaths_ = options["cc-I"].as<std::vector<std::string>>();

    if (options.count("plugin")) {
        auto pluginName = options["plugin"].as<std::string>();
        Plugin::load(pluginName);
        if (!Plugin::isLoaded()) {
            std::cerr << kPsychePrefix << "cannot load plugin " << pluginName << std::endl;
            return Exit_PluginLoadingError;
        }
    }

    const std::string& source = readFile(in);
    int code = 0;
    try {
        code = process(in, source, config);
    } catch (...) {
        Plugin::unload();
    }

    switch (code) {
    case Exit_OK:
        if (!constraints_.empty()) {
            writeFile(options["output"].as<std::string>(), constraints_);
            if (!includes_.empty()) {
                FileInfo fi(options["output"].as<std::string>());
                writeFile(fi.fullFileBaseName() + ".inc", includes_);
            }
        }
        break;

    case Exit_ParsingError_Internal:
        std::cerr << kPsychePrefix << "parsing (internal) error" << std::endl;
        break;

    case Exit_ASTError_Internal:
        std::cerr << kPsychePrefix << "unavailable AST (internal) error" << in << std::endl;
        break;

    default:
        break;
    }

    return code;
}

int Driver::process(const std::string& unitName,
                    const std::string& source,
                    const Configuration& config)
{
    configure(config);

    StringLiteral name(unitName.c_str(), unitName.length());
    unit_.reset(new TranslationUnit(&control_, &name));
    unit_->setDialect(adjustedDialect(config));

    static DiagnosticCollector collector;
    control_.setDiagnosticCollector(&collector);

    collectIncludes(source);

    if (Plugin::isLoaded()) {
        SourceInspector* inspector = Plugin::createInspector();
        auto includes = inspector->identifyIncludes(source);
        return preprocess(augmentSource(source, includes));
    }

    return parse(source);
}

void Driver::collectIncludes(const std::string& source)
{
    std::istringstream iss(source);
    for (std::string line; std::getline(iss, line);) {
        if (line.find("#include ") == 0)
            includes_ += line + "\n";
    }
}

int Driver::preprocess(const std::string& source)
{
    CompilerFacade cc(config_.nativeCC_, config_.macroDefs_, config_.macroUndefs_);
    auto r = cc.preprocessSource(source);
    if (!r.first) {
        writeFile(FileInfo(unit()->fileName()).fullFileBaseName() + ".i", r.second);
        return parse(r.second);
    }

    return Exit_PreprocessingError;
}

int Driver::parse(const std::string& source)
{
    control_.diagnosticCollector()->reset();

    unit_->setSource(source.c_str(), source.length());

    if (!unit_->parse())
        return Exit_ParsingError_Internal;

    if (!unit_->ast() || !ast())
        return Exit_ASTError_Internal;

    honorFlag(config_.value_.dumpAst,
              [this] () { ASTDotWriter(unit()).write(ast(), ".ast.dot"); });

    ProgramValidator validator(unit(), config_.value_.noTypedef);
    validator.validate(ast());

    if (control_.diagnosticCollector()->seenBlockingIssue())
        return Exit_SyntaxError;

    return annotateAST();
}

int Driver::annotateAST()
{
    // Create symbols.
    Binder bind(unit());
    bind(ast(), globalNs_);

    // Try to disambiguate syntax ambiguities and normalize the AST according to the resolutions.
    ASTNormalizer fixer(unit(), !config_.value_.noHeuristics);
    if (!fixer.normalize(ast()))
        return Exit_UnresolvedSyntaxAmbiguityError;

    honorFlag(config_.value_.displayStats,
              [this, &fixer] () {
                 std::cout << "Ambiguities stats" << std::endl << fixer.stats() << std::endl;
              });

    honorFlag(config_.value_.dumpAst,
              [this] () { ASTDotWriter(unit()).write(ast(), ".ast.fixed.dot"); });

    if (config_.value_.disambOnly)
        return Exit_OK;

    return withGenerics_
                ? instantiateGenerics()
                : generateConstraints();
}

int Driver::instantiateGenerics()
{
    GenericsInstantiatior instantiator(unit());
    bool r = instantiator.quantify(ast(), globalNs_);
    if (!r)
        return generateConstraints();

    auto origSource = readFile(unit()->fileName());
    auto newSource = instantiator.instantiate(origSource);

    writeFile(FileInfo(unit()->fileName()).fullFileBaseName() + ".poly", newSource);

    // Ignore generics in next pass.
    withGenerics_ = false;
    config_.macroDefs_.push_back("_Template=");
    config_.macroDefs_.push_back("'_Forall(v)=v'");
    config_.macroDefs_.push_back("'_Exists(v)=v'");

    return preprocess(newSource);
}

int Driver::generateConstraints()
{
    // Build domain lattice.
    DomainLattice lattice(unit());
    lattice.categorize(ast(), globalNs_);

    std::ostringstream oss;
    auto writer = factory_.makeConstraintWriter(oss);

    ConstraintGenerator generator(unit(), writer.get());
    generator.employDomainLattice(&lattice);

    if (Plugin::isLoaded()) {
        generator.installInterceptor(Plugin::createInterceptor());
        generator.installObserver(Plugin::createObserver());
    }

    if (config_.value_.handleGNUerrorFunc_)
        generator.addPrintfLike("error", 2);
    generator.generate(ast(), globalNs_);

    constraints_ = oss.str();

    honorFlag(config_.value_.displayConstraints,
              [this] () { std::cout << constraints_ << std::endl; });

    return Exit_OK;
}
