// Copyright (c) 2016/17/18/19/20/21 Leandro T. C. Melo <ltcmelo@gmail.com>
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

#ifndef PSYCHE_C_SYNTAX_TREE_H__
#define PSYCHE_C_SYNTAX_TREE_H__

#include "API.h"
#include "APIFwds.h"

#include "parser/LexedTokens.h"
#include "parser/LineDirective.h"
#include "parser/ParseOptions.h"
#include "parser/PreprocessorOptions.h"
#include "syntax/SyntaxToken.h"

#include "../common/diagnostics/DiagnosticCollector.h"
#include "../common/infra/Pimpl.h"
#include "../common/text/SourceText.h"

#include <cstdio>
#include <iostream>
#include <memory>
#include <ostream>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

namespace psy {
namespace C {

/**
 * \brief The SyntaxTree class.
 *
 * \note
 * API inspired by \c Microsoft.CodeAnalysis.SyntaxTree from Roslyn, the .NET Compiler Platform.
 */
class PSY_C_API SyntaxTree
{
public:
    SyntaxTree(const SyntaxTree&) = delete;
    ~SyntaxTree();
    void operator=(const SyntaxTree&) = delete;

    /**
     * \brief The SyntaxCategory enumeration.
     *
     * The possible categories of syntax that a SyntaxTree
     * may represent. If unspecified, it is assumed that
     * a SyntaxTree represents a \a translation-unit.
     */
    enum class SyntaxCategory : uint8_t
    {
        Unspecified,
        Declarations,
        Expressions,
        Statements,
    };

    /**
     * Parse the input \p text, as according to the \p syntaxCategory,
     * in order to build \c this SyntaxTree.
     */
    static std::unique_ptr<SyntaxTree> parseText(SourceText text,
                                                 ParseOptions options = ParseOptions(),
                                                 const std::string& path = "",
                                                 SyntaxCategory syntaxCategory = SyntaxCategory::Unspecified);

    /**
     * The path of the file associated to \c this SyntaxTree.
     */
    std::string filePath() const;

    /**
     * The SourceText associated to \c this SyntaxTree.
     */
    const SourceText& text() const;

    /**
     * The root node of this \c this SyntaxTree.
     */
    SyntaxNode* root() const;

    /**
     * Whether \c this SyntaxTree has a \c TranslationUnit as the root node.
     */
    bool hasTranslationUnitRoot() const;

    /**
     * The \c TranslationUnit root of \c this SyntaxTree (in one exists).
     */
    TranslationUnitSyntax* translationUnitRoot() const;

    /**
     * The diagnostics in \c this SyntaxTree.
     */
    std::vector<Diagnostic> diagnostics() const;

private:
    SyntaxTree(SourceText text,
               ParseOptions options,
               const std::string& path);

    DECL_PIMPL(SyntaxTree)

    MemoryPool* unitPool() const;

    using TokenSequenceType = std::vector<SyntaxToken>;
    using LineColum = std::pair<unsigned int, unsigned int>;
    using ExpansionsTable = std::unordered_map<unsigned int, LineColum>;

    friend class SyntaxNode;
    friend class Lexer;
    friend class Parser;
    friend class Binder;

    // TODO: To be removed.
    friend class Unparser;
    friend class SyntaxWriterDOTFormat;

    /* Lexed-tokens access and manipulation */
    void addToken(SyntaxToken tk);
    SyntaxToken& tokenAt(LexedTokens::IndexType tkIdx);
    const SyntaxToken& tokenAt(LexedTokens::IndexType tkIdx) const;
    TokenSequenceType::size_type tokenCount();
    LexedTokens::IndexType freeTokenSlot() const;

    void buildTree(SyntaxCategory syntaxCat);
    const ParseOptions& options() const;

    const Identifier* identifier(const char* s, unsigned int size);
    const IntegerConstant* integerConstant(const char* s, unsigned int size);
    const FloatingConstant* floatingConstant(const char* s, unsigned int size);
    const CharacterConstant* characterConstant(const char* s, unsigned int size);
    const StringLiteral* stringLiteral(const char* s, unsigned size);

    void relayLineStart(unsigned int offset);
    void relayExpansion(unsigned int offset, std::pair<unsigned, unsigned> p);
    void relayLineDirective(unsigned int offset, unsigned int lineno, const std::string& filePath);

    LinePosition computePosition(unsigned int offset) const;
    unsigned int searchForLineno(unsigned int offset) const;
    unsigned int searchForColumn(unsigned int offset, unsigned int lineno) const;
    LineDirective searchForLineDirective(unsigned int offset) const;

    void newDiagnostic(DiagnosticDescriptor descriptor, LexedTokens::IndexType tkIdx);

    void bind() {}
    void typeCheck() {}

    LanguageDialect dialect() const { return dialect_; }
    void setDialect(LanguageDialect dialect) { dialect_ = dialect; }

    // TODO: Move to implementaiton.
    LanguageDialect dialect_;
    std::vector<SyntaxToken> _comments;
};

} // C
} // psy


#endif
