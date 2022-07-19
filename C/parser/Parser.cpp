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

#include "Parser__IMPL__.inc"

using namespace psy;
using namespace C;

int depth_ = 0;

/* Backtracker */

Parser::Backtracker::Backtracker(Parser* parser, LexedTokens::IndexType tkIdx)
    : parser_(parser)
    , refTkIdx_(tkIdx == 0 ? parser->curTkIdx_ : tkIdx)
    , done_(false)
{
    if (parser_->backtracker_)
        chained_.push(parser_->backtracker_);
    parser_->backtracker_ = this;
}

Parser::Backtracker::~Backtracker()
{
    if (!done_)
        discard();
}

void Parser::Backtracker::discard()
{
    if (done_)
        return;

    if (!chained_.empty()) {
        parser_->backtracker_ = chained_.top();
        chained_.pop();
    }
    else
        parser_->backtracker_ = nullptr;
    done_ = true;
}

void Parser::Backtracker::backtrack()
{
    if (parser_->curTkIdx_ == refTkIdx_) {
        discard();
        return;
    }

#ifdef DEBUG_RULE
    std::cerr << std::string(depth_ * 4, ' ')
              << "BACKTRACKING from  "
              << "`" << parser_->peek().valueText() << "'  "
              << parser_->curTkIdx_ << "  to  ";
#endif

    auto tkCnt = parser_->tree_->tokenCount();
    if (parser_->curTkIdx_ < tkCnt)
        parser_->curTkIdx_ = refTkIdx_;
    else
        parser_->curTkIdx_ = tkCnt - 1;

#ifdef DEBUG_RULE
    std::cerr << "`" << parser_->peek().valueText() << "'  "
              << parser_->curTkIdx_ << std::endl;
#endif

    discard();
}

/* DepthControl */

Parser::DepthControl::DepthControl(int& depth)
    : depth_(depth)
{
    if (depth_ > MAX_DEPTH_OF_STMTS)
        throw std::runtime_error("maximum depth of statements reached");
    ++depth_;
}

Parser::DepthControl::~DepthControl()
{
    --depth_;
}

/* Parser */

Parser::Parser(SyntaxTree* tree)
    : pool_(tree->unitPool())
    , tree_(tree)
    , backtracker_(nullptr)
    , diagReporter_(this)
    , curTkIdx_(1)
    , DEPTH_OF_EXPRS_(0)
    , DEPTH_OF_STMTS_(0)
{
    depth_ = 0;
}

Parser::~Parser()
{}

const SyntaxToken& Parser::peek(unsigned int LA) const
{
    return tree_->tokenAt(curTkIdx_ + LA - 1);
}

LexedTokens::IndexType Parser::consume()
{
    return curTkIdx_++;
}

/**
 * Match a token of the given kind.
 */
bool Parser::match(SyntaxKind expectedTkK, LexedTokens::IndexType* tkIdx)
{
    auto curTkK = peek().kind();
    if (curTkK == expectedTkK)
    {
        *tkIdx = consume();
        return true;
    }

    diagReporter_.ExpectedToken(SyntaxKind(expectedTkK));

    if (curTkK != EndOfFile)
        consume();
    *tkIdx = LexedTokens::invalidIndex();
    return false;
}

/**
 * Match a token of the given kind;; if the match was not successful,
 * skip until a token of the given kind is found.
 */
bool Parser::matchOrSkipTo(SyntaxKind expectedTkK, LexedTokens::IndexType* tkIdx)
{
    if (match(expectedTkK, tkIdx))
        return true;
    skipTo(expectedTkK);
    return false;
}

/**
 * Skip until a token of the given kind is found.
 */
void Parser::skipTo(SyntaxKind tkK)
{
    auto curTkK = peek().kind();
    while (curTkK != tkK) {
        if (curTkK == EndOfFile)
            return;
        consume();
        curTkK = peek().kind();
    }
}

/**
 * Whether the parser is in backtracking mode.
 */
bool Parser::mightBacktrack() const
{
    return static_cast<bool>(backtracker_);
}

/**
 * Parse the syntax associated to the SyntaxTree used to construct \c this Parser.
 *
 * \return a TranslationUnitSyntax.
 */
TranslationUnitSyntax* Parser::parse()
{
    auto unit = makeNode<TranslationUnitSyntax>();
    parseTranslationUnit(unit);
    return unit;
}

bool Parser::detectedAnyAmbiguity() const
{
    return !diagReporter_.retainedAmbiguityDiags_.empty();
}

std::vector<
    std::pair<DiagnosticDescriptor,
              LexedTokens::IndexType>> Parser::releaseRetainedAmbiguityDiags() const
{
    return std::move(diagReporter_.retainedAmbiguityDiags_);
}

bool Parser::ignoreDeclarator()
{
    while (true) {
        switch (peek().kind()) {
            // Just return.
            case EndOfFile:
            case CommaToken:
            case IdentifierToken:
            case AsteriskToken:
                return false;

            // Skip and return.
            case SemicolonToken:
            case CloseBraceToken:
                consume();
                return false;

            // Skip.
            default:
                consume();
        }
    }
}

bool Parser::ignoreDeclarationOrDefinition()
{
    while (true) {
        switch (peek().kind()) {
            // Just return.
            case EndOfFile:
            case Keyword_typedef:
            case Keyword_extern:
            case Keyword_static:
            case Keyword__Thread_local:
            case Keyword_ExtGNU___thread:
            case Keyword_auto:
            case Keyword_register:
            case Keyword_inline:
            case Keyword__Noreturn:
            case Keyword_struct:
            case Keyword_union:
            case Keyword_enum:
            case Keyword__Static_assert:
            case Keyword_ExtGNU___asm__:
                return false;

            // Skip and return.
            case SemicolonToken:
            case CloseBraceToken:
                consume();
                return false;

            // Skip.
            default:
                consume();
        }
    }
}

bool Parser::ignoreMemberDeclaration()
{
    while (true) {
        switch (peek().kind()) {
            // Just return.
            case EndOfFile:
            case Keyword_typedef:
            case Keyword_extern:
            case Keyword_static:
            case Keyword__Thread_local:
            case Keyword_ExtGNU___thread:
            case Keyword_auto:
            case Keyword_register:
            case Keyword_inline:
            case Keyword__Noreturn:
            case Keyword_struct:
            case Keyword_union:
            case Keyword_enum:
            case Keyword__Static_assert:
            case Keyword_ExtGNU___asm__:
                return false;

            // Skip and return.
            case CommaToken:
            case SemicolonToken:
                consume();
                return false;

            // Skip.
            default:
                consume();
        }
    }
}

bool Parser::ignoreStatement()
{
    while (true) {
        switch (peek().kind()) {
            // Just return.
            case EndOfFile:
            case Keyword_typedef:
            case Keyword_extern:
            case Keyword_static:
            case Keyword__Thread_local:
            case Keyword_ExtGNU___thread:
            case Keyword_auto:
            case Keyword_register:
            case Keyword_inline:
            case Keyword__Noreturn:
            case Keyword_struct:
            case Keyword_union:
            case Keyword_enum:
            case Keyword__Static_assert:
            case Keyword_ExtGNU___asm__:
                return false;

            // Skip and return.
            case CloseBraceToken:
            case SemicolonToken:
                consume();
                return false;

                // Skip.
            default:
                consume();
        }
    }
}
