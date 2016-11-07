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

#ifndef PSYCHE_RUNNER_H__
#define PSYCHE_RUNNER_H__

#include "CPlusPlusForwardDeclarations.h"
#include <string>
#include <memory>

namespace psyche {

struct AnalysisOptions
{
    AnalysisOptions() : flags_(~0) {}

    struct Flags
    {
        //! Whether to write the generate constraints back.
        uint32_t writeConstraints_   : 1;
        //! Whether to handle GNU's error function as a printf variety.
        uint32_t handleGNUerrorFunc_ : 1;
        //! For future use.
        uint32_t unused              : 30;
    };
    union
    {
        Flags flag_;
        uint32_t flags_;
    };

    std::string constraints_;
};

std::unique_ptr<CPlusPlus::TranslationUnit>
analyseProgram(const std::string& source,
               CPlusPlus::Control &control,
               CPlusPlus::StringLiteral &name,
               AnalysisOptions &options);

} // namespace psyche

#endif
