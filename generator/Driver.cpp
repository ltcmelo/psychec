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
#include "ASTNormalizer.h"
#include "Bind.h"
#include "Control.h"
#include "ConstraintGenerator.h"
#include "ConstraintWriter.h"
#include "Debug.h"
#include "DiagnosticCollector.h"
#include "Dumper.h"
#include "Factory.h"
#include "Literals.h"
#include "Observer.h"
#include "Symbols.h"
#include "TranslationUnit.h"
#include "Utils.h"
#include <iostream>
#include <sstream>

using namespace CPlusPlus;

namespace psyche {

extern bool debugEnabled;

std::tuple<size_t,
           std::unique_ptr<CPlusPlus::TranslationUnit>,
           std::string>
process(const std::string& unitName,
        const std::string& source,
        CPlusPlus::Control& control,
        ExecutionFlags& flags,
        Factory* factory)
{
    DiagnosticCollector collector;
    control.setDiagnosticClient(&collector);
    StringLiteral name(unitName.c_str(), unitName.length());
    std::unique_ptr<TranslationUnit> unit(new TranslationUnit(&control, &name));
    unit->setSource(source.c_str(), source.length());

    // Set language specifics.
    LanguageOptions features;
    features.c99 = 1;
    features.nullptrOnNULL = 1;
    unit->setLanguageFeatures(features);

    // Check whether the parser finished successfully.
    if (!unit->parse())
        return std::make_tuple(kParsingFailed, nullptr, "");

    // We only proceed if the source is free from syntax errors.
    if (!collector.isEmpty())
        return std::make_tuple(kSyntaxErrors, nullptr, "");

    // If we have no AST, there's nothing to do.
    if (!unit->ast() || !unit->ast()->asTranslationUnit())
        return std::make_tuple(kInvalidAST, nullptr, "");

    TranslationUnitAST* ast = unit->ast()->asTranslationUnit();
    if (flags.flag_.dumpAst)
        Dumper(unit.get()).dump(ast, ".ast.dot");

    // Bind and try to disambiguate ambiguities, if any.
    Namespace* global = control.newNamespace(0, nullptr);
    Bind bind(unit.get());
    bind(ast, global);

    // Normalize the AST based disambiguation resolutions.
    ASTNormalizer fixer(unit.get(), !flags.flag_.nonHeuristic);
    fixer.normalize(ast);
    if (flags.flag_.dumpAst)
        Dumper(unit.get()).dump(ast, ".ast.fixed.dot");

    // If the program is inherently ambiguous, it's too bad.
    if (isProgramAmbiguous(unit.get(), ast))
        return std::make_tuple(kProgramAmbiguous, nullptr, "");

    if (flags.flag_.disambOnly)
        return std::make_tuple(kOK, std::move(unit), "");

    if (flags.flag_.displayStats)
        std::cout << "Ambiguities stats" << std::endl << fixer.stats() << std::endl;

    std::ostringstream oss;
    auto writer = factory->makeWriter(oss);
    ConstraintGenerator generator(unit.get(), writer.get());
    auto observer = factory->makeObserver();
    generator.installObserver(observer.get());

    if (flags.flag_.handleGNUerrorFunc_)
        generator.addVariadic("error", 2);

    generator.generate(ast->asTranslationUnit(), global);

    if (flags.flag_.displayStats)
        std::cout << "Stats: " << writer->totalConstraints() << std::endl;
    if (flags.flag_.displayCstr)
        std::cout << "Constraints:\n" << oss.str() << std::endl;

    return std::make_tuple(kOK, std::move(unit), oss.str());
}

} // namespace psyche
