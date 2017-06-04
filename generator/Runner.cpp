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

#include "Runner.h"
#include "AST.h"
#include "AstFixer.h"
#include "Bind.h"
#include "Control.h"
#include "ConstraintGenerator.h"
#include "ConstraintWriter.h"
#include "Debug.h"
#include "DiagnosticCollector.h"
#include "Dumper.h"
#include "Factory.h"
#include "IO.h"
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

/*!
 * \brief process
 * \param source
 * \param control
 * \param name
 *
 * Core function that triggers all the work.
 */
std::unique_ptr<TranslationUnit> process(const std::string& source,
                                         StringLiteral &unitName,
                                         Control &control,
                                         ProgramCommand &cmd,
                                         Factory* factory)
{
    std::unique_ptr<TranslationUnit> unit(new TranslationUnit(&control, &unitName));
    unit->setSource(source.c_str(), source.length());

    // Set language options.
    LanguageOptions features;
    features.c99 = 1;
    features.nullptrOnNULL = 1;
    unit->setLanguageFeatures(features);

    DiagnosticCollector collector;
    control.setDiagnosticClient(&collector); // Collector is alive only within this scope.

    // Check whether the parser finished successfully.
    if (!unit->parse()) {
        std::cout << "Parsing failed" << std::endl;
        return nullptr;
    }

    // We only proceed if the source is free from syntax errors.
    if (!collector.isEmpty()) {
        std::cout << "Source has syntax errors" << std::endl;
        return nullptr;
    }

    // If we have no AST, there's nothing to do.
    if (!unit->ast() || !unit->ast()->asTranslationUnit()) {
        std::cout << "No AST" << std::endl;
        return nullptr;
    }

    TranslationUnitAST* ast = unit->ast()->asTranslationUnit();
    if (cmd.flag_.dumpAst)
        Dumper(unit.get()).dump(ast, ".ast.dot");

    // Binding phase, this is when we create symbols.
    Namespace* globalNs = control.newNamespace(0, nullptr);
    Bind bind(unit.get());
    bind(ast, globalNs);

    // Disambiguate eventual ambiguities.
    AstFixer astFixer(unit.get());
    astFixer.fix(ast);
    if (cmd.flag_.dumpAst)
        Dumper(unit.get()).dump(ast, ".ast.fixed.dot");

    if (isProgramAmbiguous(unit.get(), ast)) {
        std::cout << "Code has unresolved ambiguities" << std::endl;
        return nullptr;
    }

    if (cmd.flag_.disambOnly)
        return unit;

    if (cmd.flag_.displayStats)
        std::cout << "Ambiguities stats" << std::endl << astFixer.stats() << std::endl;

    std::ostringstream oss;
    auto writer = factory->makeWriter(oss);
    auto observer = factory->makeObserver();
    ConstraintGenerator generator(unit.get(), writer.get(), observer.get());
    if (cmd.flag_.handleGNUerrorFunc_)
        generator.addVariadic("error", 2);
    generator.generate(ast->asTranslationUnit(), globalNs);
    writeFile(oss.str(), cmd.output_);

    if (cmd.flag_.displayStats)
        std::cout << "Stats: " << writer->totalConstraints() << std::endl;
    if (cmd.flag_.displayCstr)
        std::cout << "Constraints:\n" << oss.str() << std::endl;

    return unit;
}

} // namespace psyche
