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

#include "SyntaxTree.h"

#include "MemoryPool.h"

#include "compiler/Compilation.h"
#include "compiler/Binder.h"
#include "compiler/TypeChecker.h"
#include "parser/Lexer.h"
#include "parser/Parser.h"
#include "syntax/SyntaxLexemes.h"
#include "syntax/SyntaxNodes.h"

#include "../common/text/TextElementTable.h"

#include <algorithm>
#include <cstdarg>
#include <cstring>
#include <functional>
#include <iomanip>
#include <iostream>
#include <stack>
#include <vector>

// Uncomment to display the sequence of lexed tokens.
//#define DEBUG_LEXED_TOKENS

using namespace psy;
using namespace C;

struct SyntaxTree::SyntaxTreeImpl
{
    SyntaxTreeImpl(SourceText text,
                   ParseOptions parseOptions,
                   const std::string& path)
        : pool_(new MemoryPool())
        , text_(std::move(text))
        , parseOptions_(std::move(parseOptions))
        , path_(path)
        , rootNode_(nullptr)
    {
        if (path_.empty())
            path_ = "<buffer>";
    }

    std::unique_ptr<MemoryPool> pool_;

    SourceText text_;
    ParseOptions parseOptions_;
    std::string path_;

    TextElementTable<Identifier> identifiers_;
    TextElementTable<IntegerConstant> integers_;
    TextElementTable<FloatingConstant> floatings_;
    TextElementTable<CharacterConstant> characters_;
    TextElementTable<StringLiteral> strings_;

    SyntaxNode* rootNode_;

    LexedTokens tokens_;
    std::vector<LineDirective> lineDirectives_;
    std::vector<unsigned int> startOfLineOffsets_;
    SyntaxTree::ExpansionsTable expansions_;

    std::vector<Diagnostic> diagnostics_;
};

SyntaxTree::SyntaxTree(SourceText text,
                       ParseOptions options,
                       const std::string& path)
    : P(new SyntaxTreeImpl(text, options, path))
{}

SyntaxTree::~SyntaxTree()
{
}

MemoryPool* SyntaxTree::unitPool() const
{
    return P->pool_.get();
}

std::unique_ptr<SyntaxTree> SyntaxTree::parseText(SourceText text,
                                                  ParseOptions options,
                                                  const std::string& path,
                                                  SyntaxCategory syntaxCategory)
{
    std::unique_ptr<SyntaxTree> tree(new SyntaxTree(text, options, path));
    tree->buildTree(syntaxCategory);
    return tree;
}

std::string SyntaxTree::filePath() const
{
    return P->path_;
}

const SourceText& SyntaxTree::text() const
{
    return P->text_;
}

SyntaxNode* SyntaxTree::root() const
{
    return P->rootNode_;
}

bool SyntaxTree::hasTranslationUnitRoot() const
{
    return static_cast<bool>(P->rootNode_->asTranslationUnit());
}

TranslationUnitSyntax* SyntaxTree::translationUnitRoot() const
{
    if (hasTranslationUnitRoot())
        return P->rootNode_->asTranslationUnit();
    return nullptr;
}

std::vector<Diagnostic> SyntaxTree::diagnostics() const
{
    return P->diagnostics_;
}

/* Forward calls to the lexed-tokens container */
void SyntaxTree::addToken(SyntaxToken tk) { P->tokens_.add(tk); }
SyntaxToken& SyntaxTree::tokenAt(LexedTokens::IndexType tkIdx) { return P->tokens_.tokenAt(tkIdx); }
const SyntaxToken& SyntaxTree::tokenAt(LexedTokens::IndexType tkIdx) const { return P->tokens_.tokenAt(tkIdx); }
SyntaxTree::TokenSequenceType::size_type SyntaxTree::tokenCount() const { return P->tokens_.count(); }
LexedTokens::IndexType SyntaxTree::freeTokenSlot() const { return P->tokens_.freeSlot(); }

void SyntaxTree::buildTree(SyntaxCategory syntaxCat)
{
    Lexer lexer(this);
    lexer.lex();

#ifdef DEBUG_LEXED_TOKENS
    std::cout << "\n\n" << P->text_.rawText() << std::endl;

    std::cout.setf(std::ios::left, std::ios::adjustfield);
    for (auto i = 1U; i < P->lexedTokens_.size(); ++i) // Skip initial EOF marker.
    {
        SyntaxToken tk = P->lexedTokens_[i];

        std::cout << "    ";
        std::cout.width(16);
        std::cout << tk.valueText();

        auto offset = tk.charBegin();

        auto l = computeLineno(offset);
        auto c = computeColumn(offset, l);
        std::cout << "    ";
        std::cout.width(8);
        std::cout << (std::to_string(l) + ":" + std::to_string(c));

        std::cout << "    ";
        std::cout.width(8);
        std::cout << (std::string("(") + std::to_string(offset) + ")");
        std::cout << "    ";

        std::cout.width(6);
        std::cout << tk.rawKind_ << std::endl;

    }
    std::cout << "\n\n\n";
#endif

    Parser parser(this);
    switch (syntaxCat) {
        case SyntaxCategory::Declarations: {
            DeclarationSyntax* decl = nullptr;
            parser.parseExternalDeclaration(decl);
            P->rootNode_ = decl;
            break;
        }

        case SyntaxCategory::Expressions: {
            ExpressionSyntax* expr = nullptr;
            parser.parseExpression(expr);
            P->rootNode_ = expr;
            break;
        }

        case SyntaxCategory::Statements: {
            StatementSyntax* stmt = nullptr;
            parser.parseStatement(stmt, Parser::StatementContext::None);
            P->rootNode_ = stmt;
            break;
         }

        default:
            P->rootNode_ = parser.parse();
    }
}

const ParseOptions& SyntaxTree::parseOptions() const
{
    return P->parseOptions_;
}

const Identifier* SyntaxTree::identifier(const char* s, unsigned size)
{
    return P->identifiers_.findOrInsert(s, size);
}

const StringLiteral* SyntaxTree::stringLiteral(const char* s, unsigned size)
{
    return P->strings_.findOrInsert(s, size);
}

const IntegerConstant* SyntaxTree::integerConstant(const char* s, unsigned int size)
{
    return P->integers_.findOrInsert(s, size);
}

const FloatingConstant* SyntaxTree::floatingConstant(const char* s, unsigned int size)
{
    return P->floatings_.findOrInsert(s, size);
}

const CharacterConstant* SyntaxTree::characterConstant(const char* s, unsigned int size)
{
    return P->characters_.findOrInsert(s, size);
}


void SyntaxTree::relayLineStart(unsigned int offset)
{
    P->startOfLineOffsets_.push_back(offset);
}

void SyntaxTree::relayExpansion(unsigned int offset, std::pair<unsigned int, unsigned int> p)
{
    P->expansions_.insert(std::make_pair(offset, p));
}

void SyntaxTree::relayLineDirective(unsigned int offset,
                                    unsigned int lineno,
                                    const std::string& filePath)
{
    P->lineDirectives_.emplace_back(lineno, filePath, offset);
}

LinePosition SyntaxTree::computePosition(unsigned int offset) const
{
    unsigned int lineno = 0;
    unsigned int column = 0;

    auto it = P->expansions_.find(offset);
    if (it != P->expansions_.end()) {
        lineno = it->second.first;
        column = it->second.second + 1;
    }
    else {
        lineno = searchForLineno(offset);
        column = searchForColumn(offset, lineno);

        // Take line directives into consideration.
        auto lineDir = searchForLineDirective(offset);
        lineno -= searchForLineno(lineDir.offset()) + 1;
        lineno += lineDir.lineno();
    }

    return LinePosition(lineno, column);
}

unsigned int SyntaxTree::searchForLineno(unsigned int offset) const
{
    auto it = std::lower_bound(P->startOfLineOffsets_.begin(),
                               P->startOfLineOffsets_.end(),
                               offset);
    if (it == P->startOfLineOffsets_.end())
        return P->startOfLineOffsets_.size() - 1;

    if (it != P->startOfLineOffsets_.begin())
        --it;
    return std::distance(P->startOfLineOffsets_.begin(), it);
}

unsigned int SyntaxTree::searchForColumn(unsigned int offset, unsigned int lineno) const
{
    if (!offset)
        return 0;
    return offset - P->startOfLineOffsets_[lineno];
}

LineDirective SyntaxTree::searchForLineDirective(unsigned int offset) const
{
    auto it = std::lower_bound(P->lineDirectives_.begin(),
                               P->lineDirectives_.end(),
                               offset,
                               [] (auto lineDir, auto value) { return lineDir.offset() < value; });

    if (it != P->lineDirectives_.begin())
        --it;
    return *it;
}

void SyntaxTree::newDiagnostic(DiagnosticDescriptor descriptor,
                               LexedTokens::IndexType tkIdx) const
{
    newDiagnostic(descriptor, tokenAt(tkIdx));
}

void SyntaxTree::newDiagnostic(DiagnosticDescriptor descriptor,
                               SyntaxToken tk) const
{
    LinePosition start = computePosition(tk.charStart());
    LinePosition end = computePosition(tk.charEnd());
    FileLinePositionSpan line(P->path_, start, end);
    std::string snippet;

    auto it = std::lower_bound(P->startOfLineOffsets_.begin(), P->startOfLineOffsets_.end(), tk.charStart());
    if (it != P->startOfLineOffsets_.begin()) {
        --it;

        auto lineBegIt = P->text_.rawText().begin() + *it;
        auto lineCurIt = lineBegIt;
        while (lineCurIt != P->text_.rawText().end()) {
            if (*lineCurIt == '\n')
                break;
            ++lineCurIt;
        }

        snippet.assign(lineBegIt, lineCurIt);
        std::string marker(start.character(), ' ');
        marker += '^';
        snippet += "\n" + marker + "\n";
    }

    P->diagnostics_.emplace_back(descriptor, Location::create(line), snippet);
}
