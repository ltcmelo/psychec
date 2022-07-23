// Copyright (c) 2016/17/18/19/20/21/22 Leandro T. C. Melo <ltcmelo@gmail.com>
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
#include "Fwds.h"

#include "parser/LexedTokens.h"
#include "parser/LineDirective.h"
#include "parser/ParseOptions.h"
#include "parser/TextCompleteness.h"
#include "parser/TextPreprocessingState.h"
#include "syntax/SyntaxToken.h"

#include "../common/diagnostics/Diagnostic.h"
#include "../common/infra/InternalAccess.h"
#include "../common/infra/Pimpl.h"
#include "../common/text/SourceText.h"

#include <cstdio>
#include <iostream>
#include <memory>
#include <ostream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

namespace psy {
namespace C {

/**
 * \brief The SyntaxTree class.
 *
 * \note
 * This API is inspired by that of \c Microsoft.CodeAnalysis.SyntaxTree
 * from Roslyn, the .NET Compiler Platform.
 */
class PSY_C_API SyntaxTree
{
public:
    ~SyntaxTree();

    /**
     * \brief The SyntaxCategory enumeration.
     *
     * The possible categories of syntax that a SyntaxTree
     * may represent. If unspecified, it is assumed that
     * a SyntaxTree represents a \a translation-unit.
     */
    enum class SyntaxCategory : uint8_t
    {
        UNSPECIFIED = 0,

        Declarations,
        Expressions,
        Statements,
    };

    /**
     * Parse the input \p text, as according to the \p syntaxCategory,
     * in order to build \c this SyntaxTree.
     */
    static std::unique_ptr<SyntaxTree> parseText(SourceText text,
                                                 TextPreprocessingState textPPState,
                                                 TextCompleteness textCompleteness,
                                                 ParseOptions parseOptions = ParseOptions(),
                                                 const std::string& filePath = "",
                                                 SyntaxCategory syntaxCategory = SyntaxCategory::UNSPECIFIED);

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

PSY_INTERNAL_AND_RESTRICTED:
    PSY_GRANT_ACCESS(SyntaxNode);
    PSY_GRANT_ACCESS(SyntaxNodeList);
    PSY_GRANT_ACCESS(Lexer);
    PSY_GRANT_ACCESS(Parser);
    PSY_GRANT_ACCESS(Binder);
    PSY_GRANT_ACCESS(Symbol);
    PSY_GRANT_ACCESS(Compilation);
    PSY_GRANT_ACCESS(InternalsTestSuite);
    PSY_GRANT_ACCESS(SyntaxWriterDOTFormat); // TODO: Remove this grant.

    MemoryPool* unitPool() const;

    using TokenSequenceType = std::vector<SyntaxToken>;
    using LineColum = std::pair<unsigned int, unsigned int>;
    using ExpansionsTable = std::unordered_map<unsigned int, LineColum>;

    /* Lexed-tokens access and manipulation */
    void addToken(SyntaxToken tk);
    SyntaxToken& tokenAt(LexedTokens::IndexType tkIdx);
    const SyntaxToken& tokenAt(LexedTokens::IndexType tkIdx) const;
    TokenSequenceType::size_type tokenCount() const;
    LexedTokens::IndexType freeTokenSlot() const;

    bool parseExitedEarly() const;

    const Identifier* identifier(const char* s, unsigned int size);
    const IntegerConstant* integerConstant(const char* s, unsigned int size);
    const FloatingConstant* floatingConstant(const char* s, unsigned int size);
    const ImaginaryIntegerConstant* imaginaryIntegerConstant(const char* s, unsigned int size);
    const ImaginaryFloatingConstant* imaginaryFloatingConstant(const char* s, unsigned int size);
    const CharacterConstant* characterConstant(const char* s, unsigned int size);
    const StringLiteral* stringLiteral(const char* s, unsigned size);

    void relayLineStart(unsigned int offset);
    void relayExpansion(unsigned int offset, std::pair<unsigned, unsigned> p);
    void relayLineDirective(unsigned int offset, unsigned int lineno, const std::string& filePath);

    const ParseOptions& parseOptions() const;

    LanguageDialect dialect() const { return dialect_; }
    void setDialect(LanguageDialect dialect) { dialect_ = dialect; }

    void newDiagnostic(DiagnosticDescriptor descriptor, LexedTokens::IndexType tkIdx) const;
    void newDiagnostic(DiagnosticDescriptor descriptor, SyntaxToken tk) const;

    void attachCompilation(const Compilation*) const;
    void detachCompilation(const Compilation*) const;
    std::unordered_set<const Compilation*> linkedCompilations() const;

private:
    SyntaxTree(SourceText text,
               TextPreprocessingState textPPState,
               TextCompleteness textCompleteness,
               ParseOptions parseOptions,
               const std::string& path);

    // Unavailable
    SyntaxTree(const SyntaxTree&) = delete;
    SyntaxTree& operator=(const SyntaxTree&) = delete;

    DECL_PIMPL(SyntaxTree)

    void buildFor(SyntaxCategory syntaxCategory);

    LinePosition computePosition(unsigned int offset) const;
    unsigned int searchForLineno(unsigned int offset) const;
    unsigned int searchForColumn(unsigned int offset, unsigned int lineno) const;
    LineDirective searchForLineDirective(unsigned int offset) const;

    // TODO: Move to implementaiton.
    LanguageDialect dialect_;
    std::vector<SyntaxToken> comments_;
};

} // C
} // psy


#endif
