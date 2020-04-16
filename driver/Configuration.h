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

#ifndef PSYCHE_CONFIGURATION_H__
#define PSYCHE_CONFIGURATION_H__

#include <cinttypes>
#include <string>
#include <vector>

namespace psyche {

/*!
 * \brief The Configuration class
 */
class Configuration
{
public:
    Configuration() : bits_(0) {}

    struct Bits
    {
        //!< Display the generated constraints.
        uint32_t displayConstraints : 1;

        //!< Display ambiguity- and constraints-related stats.
        uint32_t displayStats : 1;

          //!< Dump AST, both the original and disambiguated one.
        uint32_t dumpAst : 1;

          //!< Run only AST disambiguation phase.
        uint32_t disambOnly : 1;

          //!< Disable heuristics to resolve AST ambiguities.
        uint32_t noHeuristics : 1;

          //! < Forbid type declarations (e.g., `struct' and `typedef').
        uint32_t noTypedef : 1;

        //! Whether to handle GNU's error function as a printf variety.
        uint32_t handleGNUerrorFunc_  : 1;
    };
    union
    {
        Bits value_;
        uint32_t bits_;
    };

    std::string nativeCC_;
    std::string dialectName_;
    std::vector<std::string> macroDefs_;
    std::vector<std::string> macroUndefs_;
    std::vector<std::string> includePaths_;
};

} // namespace psyche

#endif
