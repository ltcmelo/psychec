/******************************************************************************
 Copyright (c) 2016,17 Leandro T. C. Melo (ltcmelo@gmail.com)

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
#include "ASTDumper.h"
#include "ASTNormalizer.h"
#include "BaseTester.h"
#include "Bind.h"
#include "CompilerFacade.h"
#include "ConstraintGenerator.h"
#include "ConstraintWriter.h"
#include "DeclarationInterceptor.h"
#include "Debug.h"
#include "DiagnosticCollector.h"
#include "DomainLattice.h"
#include "FileInfo.h"
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

constexpr int Driver::OK;
constexpr int Driver::ParsingError;
constexpr int Driver::InvalidSyntax;
constexpr int Driver::UnavailableAstError;
constexpr int Driver::UnresolvedAmbiguity;
constexpr int Driver::UnspecifiedInputFile;
constexpr int Driver::UnknownCommandLineOption;
constexpr int Driver::InvalidCommandLineValue;
constexpr int Driver::FailureLoadingPlugin;
constexpr int Driver::PreprocessingError;

namespace {

void honorFlag(bool flag, std::function<void ()> f)
{
    if (flag)
        f();
}

} // anonymous

Driver::Driver(const Factory& factory)
    : factory_(factory)
    , global_(nullptr)
{}

TranslationUnit *Driver::tu() const
{
    return unit_.get();
}

TranslationUnitAST *Driver::ast() const
{
    return tu()->ast()->asTranslationUnit();
}

Dialect Driver::adjustedDialect(const ExecutionOptions&)
{
    Dialect dialect;
    dialect.c99 = 1; // TODO: Requires further work for actual customization.
    dialect.ext_KeywordsGNU = 1;
    dialect.ext_EnumeratorAttributes = 1;
    dialect.ext_AvailabilityAttribute = 1;
    dialect.nullptrOnNULL = 1;

    return dialect;
}

void Driver::configure(const ExecutionOptions& flags)
{
    control_.reset();
    global_ = control_.newNamespace(0, nullptr);
    opts_ = flags;
}

std::string Driver::augmentSource(const std::string& source, const std::vector<std::string>& headers)
{
    if (headers.empty())
        return source;

    for (const auto& h : headers)
        includes_ += "#include <" + h + ">\n";
    includes_ += "\n";

    std::string fullSource = includes_;
    fullSource += source;
    return fullSource;
}

int Driver::process(int argc, char *argv[])
{
    ExecutionOptions exeOpts;
    std::string in;
    cxxopts::Options cmdOpts(argv[0], "PsycheC constraint generator");

    try {
        cmdOpts.positional_help("file");

        cmdOpts.add_options()
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
            ("positional", "Positional arguments",
                cxxopts::value<std::vector<std::string>>());

        cmdOpts.parse_positional(std::vector<std::string>{"file", "positional"});
        cmdOpts.parse(argc, argv);
    } catch (const cxxopts::OptionException& e) {
        std::cerr << kPsychePrefix << e.what() << std::endl;
        return UnknownCommandLineOption;
    }

    if (cmdOpts.count("help")) {
        std::cout << cmdOpts.help({"", "Group"}) << std::endl;
        return OK;
    }

    if (cmdOpts.count("test")) {
        try {
            BaseTester::runSuite();
        } catch (...) {
            std::cout << "\nYou BROKE stuff! Take a look at it!" << std::endl;
        }
        return OK;
    }

    if (!cmdOpts.count("positional")) {
        std::cerr << kPsychePrefix << "unspecified input file" << std::endl;
        return UnspecifiedInputFile;
    }

    // The input file name is the single positional argument.
    if (cmdOpts.count("positional")) {
        auto& v = cmdOpts["positional"].as<std::vector<std::string>>();
        in = v[0];
    }

    exeOpts.flag_.dumpAst = cmdOpts.count("ast");
    exeOpts.flag_.displayConstraints = cmdOpts.count("constraints");
    exeOpts.flag_.displayStats = cmdOpts.count("stats");
    exeOpts.flag_.noHeuristics = cmdOpts.count("no-heuristic");
    exeOpts.flag_.noTypedef = cmdOpts.count("no-typedef");
    exeOpts.flag_.handleGNUerrorFunc_ = true; // TODO: POSIX stuff?
    exeOpts.nativeCC_ = cmdOpts["cc"].as<std::string>();
    exeOpts.dialect_ = cmdOpts["cc-std"].as<std::string>();
    exeOpts.defs_ = cmdOpts["cc-D"].as<std::vector<std::string>>();
    exeOpts.undefs_ = cmdOpts["cc-U"].as<std::vector<std::string>>();

    if (cmdOpts.count("plugin")) {
        Plugin::load(cmdOpts["plugin"].as<std::string>());
        if (!Plugin::isLoaded()) {
            std::cerr << kPsychePrefix << "cannot load plugin" << std::endl;
            return FailureLoadingPlugin;
        }
    }

    const std::string& source = readFile(in);
    int code = 0;
    try {
        code = process(in, source, exeOpts);
    } catch (...) {
        Plugin::unload();
    }

    switch (code) {
    case OK:
        if (!constraints_.empty()) {
            writeFile(constraints_, cmdOpts["output"].as<std::string>());
            if (!includes_.empty()) {
                FileInfo info(cmdOpts["output"].as<std::string>());
                writeFile(includes_, info.fullDir() + "/" + info.fileBaseName() + ".inc");
            }
        }
        break;

    case ParsingError:
        std::cerr << kPsychePrefix << "parsing (internal) error" << std::endl;
        break;

    case UnavailableAstError:
        std::cerr << kPsychePrefix << "unavailable AST (internal) error" << in << std::endl;
        break;

    default:
        break;
    }

    return code;
}

int Driver::process(const std::string& unitName,
                    const std::string& source,
                    const ExecutionOptions& flags)
{
    configure(flags);

    StringLiteral name(unitName.c_str(), unitName.length());
    unit_.reset(new TranslationUnit(&control_, &name));
    unit_->setDialect(adjustedDialect(flags));

    static DiagnosticCollector collector;
    control_.setDiagnosticCollector(&collector);

    if (Plugin::isLoaded()) {
        SourceInspector* inspector = Plugin::createInspector();
        auto includes = inspector->identifyIncludes(source);
        return preprocess(augmentSource(source, includes));
    }

    return parse(source);
}

int Driver::preprocess(const std::string& source)
{
    CompilerFacade cc(opts_.nativeCC_, opts_.defs_, opts_.undefs_);
    auto res = cc.preprocessSource(source);
    if (!res.first)
        return parse(res.second);

    return PreprocessingError;
}

int Driver::parse(const std::string& source)
{
    control_.diagnosticCollector()->reset();

    unit_->setSource(source.c_str(), source.length());
    if (!unit_->parse())
        return ParsingError;

    if (!unit_->ast() || !ast())
        return UnavailableAstError;

    honorFlag(opts_.flag_.dumpAst,
              [this] () { ASTDumper(tu()).dump(ast(), ".ast.dot"); });

    ProgramValidator validator(tu(), opts_.flag_.noTypedef);
    validator.validate(ast());

    if (control_.diagnosticCollector()->seenBlockingIssue())
        return InvalidSyntax;

    return annotateAst();
}

int Driver::annotateAst()
{
    // Create symbols.
    Bind bind(tu());
    bind(ast(), global_);

    // Try to disambiguate syntax ambiguities and normalize the AST according to the resolutions.
    ASTNormalizer fixer(tu(), !opts_.flag_.noHeuristics);
    if (!fixer.normalize(ast()))
        return UnresolvedAmbiguity;

    honorFlag(opts_.flag_.displayStats,
              [this, &fixer] () {
                 std::cout << "Ambiguities stats" << std::endl << fixer.stats() << std::endl;
              });

    honorFlag(opts_.flag_.dumpAst,
              [this] () { ASTDumper(tu()).dump(ast(), ".ast.fixed.dot"); });

    if (opts_.flag_.disambOnly)
        return OK;

    return generateConstraints();
}

int Driver::generateConstraints()
{
    // Build domain lattice.
    DomainLattice lattice(tu());
    lattice.categorize(ast(), global_);

    std::ostringstream oss;
    auto writer = factory_.makeConstraintWriter(oss);

    ConstraintGenerator generator(tu(), writer.get());
    generator.employDomainLattice(&lattice);

    if (Plugin::isLoaded()) {
        generator.installInterceptor(Plugin::createInterceptor());
        generator.installObserver(Plugin::createObserver());
    }

    if (opts_.flag_.handleGNUerrorFunc_)
        generator.addPrintfLike("error", 2);
    generator.generate(ast(), global_);

    constraints_ = oss.str();

    honorFlag(opts_.flag_.displayConstraints,
              [this] () { std::cout << constraints_ << std::endl; });

    return OK;
}
