// Copyright (c) 2020/21/22 Leandro T. C. Melo <ltcmelo@gmail.com>
// Copyright (c) 2008 Roberto Raggi <roberto.raggi@gmail.com>
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.

#ifndef PSYCHE_C_PARSE_OPTIONS_H__
#define PSYCHE_C_PARSE_OPTIONS_H__

#include "API.h"

#include "LanguageDialect.h"
#include "LanguageExtensions.h"

#include "../common/infra/InternalAccess.h"

#include <cstdint>

namespace psy {
namespace C {

/**
 * \brief The ParseOptions class.
 *
 * Options to the lexer are also specified through this class.
 */
class PSY_C_API ParseOptions
{
public:
    //!@{
    /**
     * Create ParseOptions.
     */
    ParseOptions();
    ParseOptions(LanguageDialect dialect,
                 LanguageExtensions extensions);
    //!@}

    /**
     * The LanguageDialect of \c this ParseOptions.
     */
    const LanguageDialect& dialect() const;

    /**
     * The LanguageExtensions of \c this ParseOptions.
     */
    const LanguageExtensions& extensions() const;

    //!@{
    /**
     * \brief The alternatives for TreatmentOfIdentifiers during parse.
     */
    enum class TreatmentOfIdentifiers : std::uint8_t
    {
        None,    /**< No special treatment. */
        Classify /**< Classify into keywords and non-keywords ("plain identifiers"). */
    };
    /**
     * The TreatmentOfIdentifiers of \c this ParserOptions.
     */
    ParseOptions& setTreatmentOfIdentifiers(TreatmentOfIdentifiers treatOfIdent);
    TreatmentOfIdentifiers treatmentOfIdentifiers() const;
    //!@}

    //!@{
    /**
     * \brief The alternatives for TreatmentOfComments during parse.
     */
    enum class TreatmentOfComments : std::uint8_t
    {
        None,                 /**< No special treatment. */
        Keep,                 /**< Keep comments. */
        KeepDocumentationOnly /**< Keep documentation comments only. */
    };
    /**
     * The TreatmentOfComments of \c this ParserOptions.
     */
    ParseOptions& setTreatmentOfComments(TreatmentOfComments treatOfComments);
    TreatmentOfComments treatmentOfComments() const;
    //!@}

    //!@{
    /**
     * * \brief The alternatives for TreatmentOfAmbiguities during parse.
     */
    enum class TreatmentOfAmbiguities : std::uint8_t
    {
        None,                                       /**< No special treatment (ambiguities are preserved). */
        Diagnose,                                   /**< Diagnose ambiguities. */
        DisambiguateAlgorithmically,                /**< Disambiguate ambiguities algorithmically. */
        DisambiguateAlgorithmicallyOrHeuristically, /**< Disambiguate ambiguities algorithmically/heristically. */
        DisambiguateHeuristically,                  /**< Disambiguate ambiguities heuristically. */
    };
    /**
     * The TreatmentOfAmbiguities of \c this ParserOptions.
     */
    ParseOptions& setTreatmentOfAmbiguities(TreatmentOfAmbiguities treatOfAmbigs);
    TreatmentOfAmbiguities treatmentOfAmbiguities() const;
    //!@}

private:
    LanguageDialect dialect_;
    LanguageExtensions extensions_;

    struct BitFields
    {
        std::uint16_t treatmentOfIdentifiers_ : 2;
        std::uint16_t treatmentOfComments_ : 2;
        std::uint16_t treatmentOfAmbiguities_ : 2;
    };
    union
    {
        BitFields BF_;
        std::uint16_t bits_;
    };
};

} // C
} // psy

#endif
