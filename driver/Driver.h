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

#ifndef PSYCHE_DRIVER_H__
#define PSYCHE_DRIVER_H__

#include "ASTFwds.h"
#include "Configuration.h"
#include "Control.h"
#include "FrontendConfig.h"
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
 * \brief The Driver class
 */
class Driver final
{
public:
    Driver(const Factory& factory);

    int process(int argc, char* argv[]);

    int process(const std::string& unitName,
                const std::string& source,
                const Configuration& config);

    const std::string& constraints() const { return constraints_; }

    //! Exit code of successfull run.
    static constexpr int Exit_OK = 0;

    //! Exit code of a generic error.
    static constexpr int Exit_Error = 1;

    //! Exit code with specific error diagnostics.
    static constexpr int Exit_UnspecifiedInputFileError = 2;
    static constexpr int Exit_UnknownCommandLineOptionError = 3;
    static constexpr int Exit_SyntaxError = 5;
    static constexpr int Exit_UnresolvedSyntaxAmbiguityError = 6;
    static constexpr int Exit_PluginLoadingError = 7;
    static constexpr int Exit_PreprocessingError = 8;

    //! Exit code of internal errors (negative values).
    static constexpr int Exit_ParsingError_Internal = -1;
    static constexpr int Exit_ASTError_Internal = -2;

private:
    TranslationUnit* unit() const;
    TranslationUnitAST* ast() const;

    static Dialect adjustedDialect(const Configuration& exec);

    void configure(const Configuration& config);
    void collectIncludes(const std::string& source);
    std::string augmentSource(const std::string&, const std::vector<std::string>&);
    int preprocess(const std::string& source);
    int parse(const std::string& source);
    int annotateAST();
    int instantiateGenerics();
    int generateConstraints();

    const Factory& factory_; // TODO: Get rid of it.

    Configuration config_;
    Control control_;
    Namespace* globalNs_;
    std::unique_ptr<TranslationUnit> unit_;
    std::string constraints_;
    std::string includes_;
    bool withGenerics_; // TODO: Integrate with config.

    friend class TestDisambiguator;
};

} // namespace psyche

#endif
