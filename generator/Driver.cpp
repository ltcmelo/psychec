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

#include "Driver.h"
#include "AST.h"
#include "ASTDumper.h"
#include "ASTNormalizer.h"
#include "Bind.h"
#include "Control.h"
#include "ConstraintGenerator.h"
#include "ConstraintWriter.h"
#include "Debug.h"
#include "DiagnosticCollector.h"
#include "DomainLattice.h"
#include "Literals.h"
#include "Observer.h"
#include "Process.h"
#include "Symbols.h"
#include "StdLibIndex.h"
#include "TranslationUnit.h"
#include "Utils.h"
#include <iostream>
#include <sstream>
#include <cstring>

using namespace psyche;
using namespace CPlusPlus;

namespace psyche {
    extern bool debugEnabled;
}

constexpr size_t Driver::OK;
constexpr size_t Driver::ParsingFailed;
constexpr size_t Driver::SyntaxErrors;
constexpr size_t Driver::InvalidAST;
constexpr size_t Driver::ProgramAmbiguous;

Driver::Driver(const Factory& factory)
    : factory_(factory)
    , global_(nullptr)
{}

size_t Driver::process(const std::string& unitName,
                       const std::string& source,
                       const ExecutionFlags& flags)
{
    configure(flags);

    StringLiteral name(unitName.c_str(), unitName.length());
    unit_.reset(new TranslationUnit(&control_, &name));

    auto error = buildAst(source);
    if (error)
        return error;

    if (flags_.flag_.libDetect == static_cast<char>(LibDetectMode::Approx))
        reparseWithGuessedLibs();

    if (!annotateSymbols())
        return ProgramAmbiguous;

    if (flags_.flag_.libDetect == static_cast<char>(LibDetectMode::Strict))
        reprocessWithLibs();

    if (flags_.flag_.disambOnly)
        return OK;

    generateConstraints();

    return OK;
}

TranslationUnit *Driver::tu() const
{
    return unit_.get();
}

Dialect Driver::specifiedDialect(const ExecutionFlags&)
{
    Dialect dialect;
    dialect.c99 = 1;
    dialect.nullptrOnNULL = 1;
    return dialect;
}

void Driver::configure(const ExecutionFlags& flags)
{
    control_.reset();
    global_ = control_.newNamespace(0, nullptr);
    flags_ = flags;
}

size_t Driver::buildAst(const std::string& source)
{
    unit_->setDialect(specifiedDialect(flags_));

    static DiagnosticCollector collector;
    collector.clear();
    control_.setDiagnosticClient(&collector);

    unit_->setSource(source.c_str(), source.length());
    if (!unit_->parse())
        return ParsingFailed;

    if (!collector.isEmpty())
        return SyntaxErrors;

    if (!unit_->ast() || !tuAst())
        return InvalidAST;

    if (flags_.flag_.dumpAst)
        ASTDumper(tu()).dump(tuAst(), ".ast.dot");

    return OK;
}

bool Driver::annotateSymbols()
{
    // During symbol binding we try to disambiguate ambiguities. Afterwards, it's
    // necessary to normalize the AST according to the disambiguation resolutions.
    Bind bind(tu());
    bind(tuAst(), global_);

    ASTNormalizer fixer(tu(), !flags_.flag_.noHeuristics);
    fixer.normalize(tuAst());
    if (isProgramAmbiguous(tu(), tuAst()))
        return false;

    if (flags_.flag_.displayStats)
        std::cout << "Ambiguities stats" << std::endl << fixer.stats() << std::endl;

    if (flags_.flag_.dumpAst)
        ASTDumper(tu()).dump(tuAst(), ".ast.fixed.dot");

    return true;
}

void Driver::generateConstraints()
{
    // Build domain lattice.
    DomainLattice lattice(tu());
    lattice.categorize(tuAst(), global_);

    std::ostringstream oss;
    auto writer = factory_.makeWriter(oss);
    auto observer = factory_.makeObserver();

    ConstraintGenerator generator(tu(), writer.get());
    generator.employDomainLattice(&lattice);
    generator.installObserver(observer.get());
    if (flags_.flag_.handleGNUerrorFunc_)
        generator.addVariadic("error", 2);
    generator.generate(tuAst(), global_);
    constraints_ = oss.str();

    if (flags_.flag_.displayStats)
        std::cout << "Stats: " << writer->totalConstraints() << std::endl;

    if (flags_.flag_.displayCstr)
        std::cout << "Constraints:\n" << constraints_ << std::endl;
}

void Driver::reparseWithGuessedLibs()
{
    std::cout << "guess...\n";
    StdLibIndex index(StdLibIndex::Version::C99);
    std::vector<std::string> headers = index.inspect(control_);
    if (headers.empty())
        return;

    std::string in = R"(echo ")";
    for (const auto& h : headers)
        in += "#include <" + h + ">\n";
    in += R"(" | gcc -E -x c -)";


    std::cout << in << std::endl;
    Process process;
    process.execute(in);
}

void Driver::reprocessWithLibs()
{

}

TranslationUnitAST *Driver::tuAst() const
{
    return tu()->ast()->asTranslationUnit();
}
