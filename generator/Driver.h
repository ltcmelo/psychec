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

#ifndef PSYCHE_DRIVER_H__
#define PSYCHE_DRIVER_H__

#include "Config.h"
#include "ASTfwd.h"
#include "Control.h"
#include "CPlusPlusForwardDeclarations.h"
#include "Dialect.h"
#include "Factory.h"
#include <cstddef>
#include <functional>
#include <memory>
#include <string>
#include <tuple>

namespace psyche {

class Factory;

/*!
 * \brief The ExecFlags struct
 */
struct PSYCHEC_API ExecutionFlags
{
    ExecutionFlags() : all_(0) {}

    struct Detail
    {
        uint32_t displayCstr : 1;  //!< Display generated constraints.
        uint32_t displayStats : 1;  //!< Ambiguity- and constraints-related stats.
        uint32_t dumpAst : 1;  //!< Both original and fixed one.
        uint32_t testOnly : 1;  //!< Test run.
        uint32_t disambOnly : 1;  //!< Only disambiguate AST.
        uint32_t noHeuristics : 1;  //!< Disable heuristics on unresolved ambiguities.
        uint32_t matchMode : 2;  //!< Standard library usage detection mode.

        //! Whether to handle GNU's error function as a printf variety.
        uint32_t handleGNUerrorFunc_ : 1;
    };
    union
    {
        Detail flag_;
        uint32_t all_;
    };
};

/*!
 * \brief The StdLibMatchMode enum
 */
enum class StdLibMatchMode : char
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

    size_t process(const std::string& unitName,
                   const std::string& source,
                   const ExecutionFlags& flags);

    CPlusPlus::TranslationUnit* tu() const;
    CPlusPlus::TranslationUnitAST* tuAst() const;
    const std::string& constraints() const { return constraints_; }

    static constexpr size_t OK = 0;
    static constexpr size_t ParsingFailed = 10;
    static constexpr size_t SyntaxErrors = 11;
    static constexpr size_t InvalidAST = 12;
    static constexpr size_t ProgramAmbiguous = 13;

private:
    static CPlusPlus::Dialect specifiedDialect(const ExecutionFlags& exec);

    void configure(const ExecutionFlags& flags);

    size_t parse(const std::string& source, bool allowReparse);
    size_t annotateAstWithSymbols(bool allowReparse);
    size_t generateConstraints();

    static std::string augmentSource(const std::string& baseSource,
                                     std::function<std::vector<std::string>()>);

    std::vector<std::string> guessMissingHeaders();
    std::vector<std::string> detectMissingHeaders();

    static std::string preprocessHeaders(std::vector<std::string>&& headers);

    const Factory factory_;
    CPlusPlus::Control control_;
    ExecutionFlags flags_;
    CPlusPlus::Namespace* global_;
    std::unique_ptr<CPlusPlus::TranslationUnit> unit_;
    std::string constraints_;
};

} // namespace psyche

#endif
