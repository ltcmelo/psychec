/******************************************************************************
 Copyright (c) 2017 Leandro T. C. Melo (ltcmelo@gmail.com)

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

#include "StdLibInspector.h"

#include "Control.h"
#include "Literals.h"
#include "Names.h"
#include "StdLibIndex.h"
#include "TranslationUnit.h"
#include <memory>
#include <iostream>
#include <cstring>

using namespace psyche;

namespace {

const char* const kInspectUnit = "<inspect_unit>";

} // anonymous

std::vector<std::string> StdLibInspector::identifyIncludes(const std::string& source)
{
    StringLiteral name(kInspectUnit, strlen(kInspectUnit));
    Control control;
    auto unit = std::make_unique<TranslationUnit>(&control, &name);

    // Keep aligned with PsycheC.
    Dialect dialect;
    dialect.c99 = 1;
    dialect.ext_KeywordsGNU = 1;
    dialect.ext_EnumeratorAttributes = 1;
    dialect.ext_AvailabilityAttribute = 1;
    dialect.nullptrOnNULL = 1;
    unit->setDialect(dialect);

    unit->setSource(source.c_str(), source.length());
    unit->tokenize();

    StdLibIndex index(StdLibIndex::Version::C99);
    return index.inspect(control);
}
