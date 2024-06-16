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

#include "../common/infra/AccessSpecifiers.h"

#include <cstdint>

namespace psy {
namespace C {

/**
 * \brief The ParseOptions class.
 *
 * Options of the lexer are also specified through this class.
 */
class PSY_C_API ParseOptions
{
public:
    //!@{
    /**
     * Create ParseOptions.
     */
    ParseOptions();
    ParseOptions(LanguageDialect langDialect);
    ParseOptions(LanguageDialect langDialect,
                 LanguageExtensions langExts);
    //!@}

    /**
     * The LanguageDialect of \c this ParseOptions.
     */
    const LanguageDialect& languageDialect() const;

    /**
     * The LanguageExtensions of \c this ParseOptions.
     */
    const LanguageExtensions& languageExtensions() const;

    //!@{
    /**
     * Whether to enable the recognition of keywords.
     */
    ParseOptions& enable_keywordRecognition(bool enable);
    bool isEnabled_keywordRecognition() const;
    //!@}

    //!@{
    /**
     * \brief The CommentMode modes.
     */
    enum class CommentMode : std::uint8_t
    {
        Discard,              /**< Discard comments. */
        KeepAll,              /**< Keep all comments. */
        KeepOnlyDocumentation /**< keep only documentation comments. */
    };
    /**
     * The CommentMode of \c this ParserOptions.
     */
    ParseOptions& setCommentMode(CommentMode commentMode);
    CommentMode commentMode() const;
    //!@}

    //!@{
    /**
     * * \brief The AmbiguityMode modes.
     */
    enum class AmbiguityMode : std::uint8_t
    {
        Diagnose,                                    /**< Diagnose ambiguities. */
        DisambiguateAlgorithmically,                 /**< Disambiguate ambiguities algorithmically. */
        DisambiguateAlgorithmicallyAndHeuristically, /**< Disambiguate ambiguities algorithmically and heristically. */
        DisambiguateHeuristically,                   /**< Disambiguate ambiguities heuristically. */
    };
    /**
     * The AmbiguityMode of \c this ParserOptions.
     */
    ParseOptions& setAmbiguityMode(AmbiguityMode ambiguityMode);
    AmbiguityMode ambiguityMode() const;
    //!@}

private:
    LanguageDialect langDialect_;
    LanguageExtensions langExts_;

    struct BitFields
    {
        std::uint8_t keywordRecognition_ : 1;
        std::uint16_t commentMode_ : 2;
        std::uint16_t ambigMode_ : 2;
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
