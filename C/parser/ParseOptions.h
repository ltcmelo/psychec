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
     * The LanguageDialect used for parsing with \c this options.
     */
    const LanguageDialect& dialect() const;

    /**
     * The LanguageExtensions used for parsing with \c this options.
     */
    const LanguageExtensions& extensions() const;

    //!@{
    /**
     * Whether to classify identifiers that are keywords.
     */
    ParseOptions& classifyKeywordIdentifiers(bool yes);
    bool IsKeywordsIdentifiersClassified() const { return BF_.keywordIdentifiersClassified_; }
    //!@}

    /**
     * The CommentMode enumeration contains alternatives for
     * treating comments during parse.
     */
    enum class CommentMode : std::uint8_t
    {
        Discard,                  /**< Discard every comment. */
        KeepAll,                  /**< Keep all comments. */
        KeepDocumentationOnly     /**< Only keep documentation comments. */
    };

    //!@{
    /**
     * The comment mode.
     */
    ParseOptions& setCommentMode(CommentMode mode);
    CommentMode commentMode() const;
    //!@}

    /**
     * The DisambiguationStrategy enumeration contains alternatives for
     * disambiguating ambiguous syntax during parse.
     */
    enum class DisambiguationStrategy : std::uint8_t
    {
        AlgorithmicAndHeuristic,
        Algorithmic,
        None,
    };

    //!@{
    /**
     * The DisambiguationStrategy of \c this ParserOptions.
     */
    ParseOptions& setDisambiguationStrategy(DisambiguationStrategy strategy);
    DisambiguationStrategy disambiguationStrategy() const;
    //!@}

private:
    LanguageDialect dialect_;
    LanguageExtensions extensions_;

    struct BitFields
    {
        std::uint16_t commentMode_ : 2;
        std::uint16_t disambiguationStrategy_ : 2;
        std::uint16_t keywordIdentifiersClassified_ : 1;
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
