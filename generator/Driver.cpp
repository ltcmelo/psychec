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
#include "Debug.h"
#include "DiagnosticCollector.h"
#include "DomainLattice.h"
#include "IO.h"
#include "Literals.h"
#include "Observer.h"
#include "ProgramValidator.h"
#include "Symbols.h"
#include "StdLibIndex.h"

#include "cxxopts.hpp"

#include <iostream>
#include <sstream>
#include <cstring>

using namespace psyche;
using namespace CPlusPlus;
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

Driver::Driver(const Factory& factory)
    : factory_(factory)
    , global_(nullptr)
{}

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
            ("dump-ast", "Dump AST in .dot format")
            ("s,stats", "Display AST disambiguation and constraint-generation stats")
            ("t,test-disambiguation", "Run AST disambiguation tests")
            ("no-heuristic", "Disable heuristics on unresolved AST ambiguities")
            ("no-typedef", "Forbid typedef and struct/union declarations")
            ("match-stdlib", "Match stdlib names: ignore, approx, strict",
                cxxopts::value<std::string>()->default_value("ignore"))
            ("CC", "Specify host C compiler",
                cxxopts::value<std::string>()->default_value("gcc"))
            ("cc-D", "Predefine a macro",
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

    if (cmdOpts.count("test-disambiguation")) {
        try {
            BaseTester::runSuite();
        } catch (...) {
            std::cout << "\nYou BROKE stuff! Take a look at it!" << std::endl;
            return OK;
        }
        std::cout << "Tests passed successfully!\n"
                  << "For type-inference tests: $cd solver && stack test" << std::endl;
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

    const auto& matchLib = cmdOpts["match-stdlib"].as<std::string>();
    if (matchLib == "ignore")
        exeOpts.flag_.stdlibMode = static_cast<uint32_t>(StdLibMode::Ignore);
    else if (matchLib == "approx")
        exeOpts.flag_.stdlibMode = static_cast<uint32_t>(StdLibMode::Approx);
    else if (matchLib == "strict")
        exeOpts.flag_.stdlibMode = static_cast<uint32_t>(StdLibMode::Strict);
    else {
        std::cerr << kPsychePrefix << "invalid command line value" << std::endl;
        return InvalidCommandLineValue;
    }

    exeOpts.flag_.dumpAst = cmdOpts.count("dump-ast");
    exeOpts.flag_.displayConstraints = cmdOpts.count("constraints");
    exeOpts.flag_.displayStats = cmdOpts.count("stats");
    exeOpts.flag_.noHeuristics = cmdOpts.count("no-heuristic");
    exeOpts.flag_.noTypedef = cmdOpts.count("no-typedef");
    exeOpts.flag_.handleGNUerrorFunc_ = true; // TODO: POSIX stuff?
    exeOpts.nativeCC_ = cmdOpts["CC"].as<std::string>();
    exeOpts.macros_ = cmdOpts["cc-D"].as<std::vector<std::string>>();

    const std::string& source = readFile(in);

    const auto code = process(in, source, exeOpts);
    switch (code) {
    case OK:
        if (!constraints_.empty())
            writeFile(constraints_, cmdOpts["output"].as<std::string>());
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
    unit_->setDialect(specifiedDialect(flags));

    static DiagnosticCollector collector;
    control_.setDiagnosticCollector(&collector);

    return parse(source, true);
}

TranslationUnit *Driver::tu() const
{
    return unit_.get();
}

Dialect Driver::specifiedDialect(const ExecutionOptions&)
{
    Dialect dialect;
    dialect.c99 = 1;
    dialect.nullptrOnNULL = 1;

    return dialect;
}

void Driver::configure(const ExecutionOptions& flags)
{
    control_.reset();
    global_ = control_.newNamespace(0, nullptr);
    opts_ = flags;
}

int Driver::parse(const std::string& source, bool firstPass)
{
    control_.diagnosticCollector()->reset();

    unit_->setSource(source.c_str(), source.length());
    if (!unit_->parse())
        return ParsingError;

    if (!unit_->ast() || !tuAst())
        return UnavailableAstError;

    honorFlag(opts_.flag_.dumpAst,
              [this] () { ASTDumper(tu()).dump(tuAst(), ".ast.dot"); });

    ProgramValidator validator(tu(), firstPass && opts_.flag_.noTypedef);
    validator.validate(tuAst());

    if (control_.diagnosticCollector()->seenBlockingIssue())
        return InvalidSyntax;

    if (firstPass
            && opts_.flag_.stdlibMode == static_cast<char>(StdLibMode::Approx)) {
        return parse(augmentSource(source, std::bind(&Driver::guessMissingHeaders, this)), false);
    }

    return annotateAstWithSymbols(firstPass);
}

int Driver::annotateAstWithSymbols(bool firstPass)
{
    // During symbol binding we try to disambiguate ambiguities. Afterwards, it's
    // necessary to normalize the AST according to the disambiguation resolutions.
    Bind bind(tu());
    bind(tuAst(), global_);

    ASTNormalizer fixer(tu(), !opts_.flag_.noHeuristics);
    if (!fixer.normalize(tuAst()))
        return UnresolvedAmbiguity;

    honorFlag(opts_.flag_.displayStats,
              [this, &fixer] () {
                 std::cout << "Ambiguities stats" << std::endl << fixer.stats() << std::endl;
              });

    honorFlag(opts_.flag_.dumpAst,
              [this] () { ASTDumper(tu()).dump(tuAst(), ".ast.fixed.dot"); });

    if (firstPass
            && opts_.flag_.stdlibMode == static_cast<char>(StdLibMode::Strict)) {
        // TODO
    }

    if (opts_.flag_.disambOnly)
        return OK;

    return generateConstraints();
}

int Driver::generateConstraints()
{
    // Build domain lattice.
    DomainLattice lattice(tu());
    lattice.categorize(tuAst(), global_);

    std::ostringstream oss;
    auto writer = factory_.makeConstraintWriter(oss);
    auto observer = factory_.makeObserver();

    ConstraintGenerator generator(tu(), writer.get());
    generator.employDomainLattice(&lattice);
    generator.installObserver(observer.get());
    if (opts_.flag_.handleGNUerrorFunc_)
        generator.addPrintfLike("error", 2);
    generator.generate(tuAst(), global_);

    constraints_ = oss.str();

    honorFlag(opts_.flag_.displayConstraints,
              [this] () { std::cout << constraints_ << std::endl; });

    return OK;
}

std::string Driver::augmentSource(const std::string& baseSource,
                                  std::function<std::vector<std::string>()> getHeaders) const
{
    std::string fullSource = preprocessHeaders(getHeaders());
    fullSource.reserve(fullSource.length() + baseSource.length());
    fullSource += baseSource;

    return fullSource;
}

std::vector<std::string> Driver::guessMissingHeaders()
{
    StdLibIndex index(StdLibIndex::Version::C99);
    return index.inspect(control_);
}

std::vector<std::string> Driver::detectMissingHeaders()
{
    return std::vector<std::string>();
}

std::string Driver::preprocessHeaders(std::vector<std::string> &&headers) const
{
    if (headers.empty())
        return "";

    std::string in;
    for (const auto& h : headers)
        in += "#include <" + h + ">\n";

    CompilerFacade cc(opts_.nativeCC_, opts_.macros_);
    return cc.preprocessSource(in);
}

void Driver::honorFlag(bool flag, std::function<void ()> f) const
{
    if (flag)
        f();
}

TranslationUnitAST *Driver::tuAst() const
{
    return tu()->ast()->asTranslationUnit();
}
