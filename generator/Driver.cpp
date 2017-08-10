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
using namespace std::placeholders;

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
    unit_->setDialect(specifiedDialect(flags));

    static DiagnosticCollector collector;
    control_.setDiagnosticCollector(&collector);

    return parse(source, true);
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

size_t Driver::parse(const std::string& source, bool allowReparse)
{
    control_.diagnosticCollector()->reset();

    unit_->setSource(source.c_str(), source.length());
    if (!unit_->parse())
        return ParsingFailed;

    if (control_.diagnosticCollector()->seenBlockingIssue())
        return SyntaxErrors;

    if (!unit_->ast() || !tuAst())
        return InvalidAST;

    if (flags_.flag_.dumpAst)
        ASTDumper(tu()).dump(tuAst(), ".ast.dot");

    std::function<std::vector<std::string> (Driver&)> f = &Driver::detectMissingHeaders;

    if (allowReparse
            && flags_.flag_.matchMode == static_cast<char>(StdLibMatchMode::Approx)) {
        return parse(augmentSource(source, std::bind(&Driver::guessMissingHeaders, this)), false);
    }

    return annotateAstWithSymbols(allowReparse);
}

size_t Driver::annotateAstWithSymbols(bool allowReparse)
{
    // During symbol binding we try to disambiguate ambiguities. Afterwards, it's
    // necessary to normalize the AST according to the disambiguation resolutions.
    Bind bind(tu());
    bind(tuAst(), global_);

    ASTNormalizer fixer(tu(), !flags_.flag_.noHeuristics);
    fixer.normalize(tuAst());
    if (isProgramAmbiguous(tu(), tuAst()))
        return ProgramAmbiguous;

    if (flags_.flag_.displayStats)
        std::cout << "Ambiguities stats" << std::endl << fixer.stats() << std::endl;

    if (flags_.flag_.dumpAst)
        ASTDumper(tu()).dump(tuAst(), ".ast.fixed.dot");

    if (allowReparse
            && flags_.flag_.matchMode == static_cast<char>(StdLibMatchMode::Strict)) {
        // TODO
    }

    if (flags_.flag_.disambOnly)
        return OK;

    return generateConstraints();
}

size_t Driver::generateConstraints()
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

    return OK;
}

std::string Driver::augmentSource(const std::string& baseSource,
                                  std::function<std::vector<std::string>()> getHeaders)
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

std::string Driver::preprocessHeaders(std::vector<std::string> &&headers)
{
    // Preprocess dependent headers.
    std::string in = "echo \"";
    for (const auto& h : headers)
        in += "#include <" + h + ">\n";
    in += "\" | gcc -E -P -x c -";

    return Process().execute(in);
}

TranslationUnitAST *Driver::tuAst() const
{
    return tu()->ast()->asTranslationUnit();
}
