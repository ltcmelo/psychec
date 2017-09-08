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

#ifndef PSYCHE_DRIVER_H__
#define PSYCHE_DRIVER_H__

#include "Config.h"
#include "ASTfwd.h"
#include "Control.h"
#include "CPlusPlusForwardDeclarations.h"
#include "Dialect.h"
#include "Factory.h"
#include "TranslationUnit.h"
#include <cstddef>
#include <functional>
#include <memory>
#include <string>
#include <tuple>
#include <vector>

namespace { const char* const kPsychePrefix = "psychec: "; }

namespace psyche {

class Factory;

/*!
 * \brief The ExecutionOptions struct
 */
struct PSYCHEC_API ExecutionOptions
{
    ExecutionOptions() : all_(0) {}

    struct Detail
    {
        uint32_t displayConstraints : 1;  //!< Display generated constraints.
        uint32_t displayStats : 1;  //!< Ambiguity- and constraints-related stats.
        uint32_t dumpAst : 1;  //!< Dump AST, both original and fixed one.
        uint32_t disambOnly : 1;  //!< Only disambiguate AST.
        uint32_t noHeuristics : 1;  //!< Disable heuristics on unresolved ambiguities.
        uint32_t stdlibMode : 2;  //!< Standard library usage detection mode.
        uint32_t noTypedef : 1;  //! < Forbid type declarations.

        //! Whether to handle GNU's error function as a printf variety.
        uint32_t handleGNUerrorFunc_ : 1;
    };
    union
    {
        Detail flag_;
        uint32_t all_;
    };

    std::string nativeCC_;
    std::vector<std::string> defs_;
    std::vector<std::string> undefs_;
};

/*!
 * \brief The StdLibMode enum
 */
enum class StdLibMode : char
{
    Ignore,
    Approx,
    Strict
};

/*!
 * \brief The Driver class
 */
class PSYCHEC_API Driver final
{
public:
    Driver(const Factory& factory);

    int process(int argc, char* argv[]);

    int process(const std::string& unitName,
                const std::string& source,
                const ExecutionOptions& flags);

    const std::string& constraints() const { return constraints_; }

    //! Successfull run.
    static constexpr int OK = 0;

    //! Diagnostics in general (positive values).
    static constexpr int UnspecifiedInputFile = 2;
    static constexpr int UnknownCommandLineOption = 3;
    static constexpr int InvalidCommandLineValue = 4;
    static constexpr int InvalidSyntax = 5;
    static constexpr int UnresolvedAmbiguity = 6;

    //! Internal errors (negative values).
    static constexpr int ParsingError = -1;
    static constexpr int UnavailableAstError = -2;

private:
    friend class TestDisambiguator;

    static CPlusPlus::Dialect specifiedDialect(const ExecutionOptions& exec);

    void configure(const ExecutionOptions& flags);

    int parse(const std::string& source, bool firstPass);
    int annotateAstWithSymbols(bool firstPass);
    int generateConstraints();

    std::vector<std::string> guessMissingHeaders();
    std::vector<std::string> detectMissingHeaders();

    std::string augmentSource(const std::string& baseSource,
                              std::function<std::vector<std::string>()>);

    std::string preprocessIncludes() const;

    void honorFlag(bool flag, std::function<void ()>) const;

    CPlusPlus::TranslationUnitAST* tuAst() const;
    CPlusPlus::TranslationUnit* tu() const;

    const Factory& factory_;
    CPlusPlus::Control control_;
    ExecutionOptions opts_;
    CPlusPlus::Namespace* global_;
    std::unique_ptr<CPlusPlus::TranslationUnit> unit_;
    std::string constraints_;
    std::string includes_;
};

} // namespace psyche

#endif
