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

#include "Parser.h"

#include "Lexer.h"

#include "compilation/Compilation.h"
#include "syntax/SyntaxFacts.h"
#include "syntax/SyntaxLexeme_ALL.h"
#include "syntax/SyntaxNodes.h"
#include "syntax/SyntaxToken.h"
#include "syntax/SyntaxUtilities.h"

#include "../common/infra/Assertions.h"

#include <cstring>
#include <iostream>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <utility>

using namespace  psy;
using namespace C;

        /******************************************/
        /* Do NOT include this file from headers. */
        /******************************************/

#define MAX_DEPTH_OF_EXPRS 1000
#define MAX_DEPTH_OF_STMTS 100

namespace psy {
namespace C {

static int depth_;

class DebugRule
{
public:
    const char* ruleID_;
    Parser* parser_;

    DebugRule(const char* name, Parser* parser)
        : ruleID_(name)
        , parser_(parser)
    {
        std::cerr << std::string(depth_++ * 4, ' ')
                  << "`" << parser->peek().valueText() << "'"
                  << "  " << parser->curTkIdx_
                  << "  " << name
                  << "  " << (parser->backtracker_ ? "BT" : "")
                  << std::endl;
    }

    ~DebugRule()
    {
        std::cerr << std::string(--depth_ * 4, ' ')
                  << "<<< " << ruleID_ << "  "
                  << "`" << parser_->peek().valueText() << "'  "
                  << (parser_->backtracker_ ? "BT" : "")
                  << std::endl;
    }
};

} // C
} // psy

//#define DEBUG_RULE
#ifdef DEBUG_RULE
#    define DEBUG_THIS_RULE() DebugRule D__(__func__, this)
#else
#    define DEBUG_THIS_RULE() do {} while (0)
#endif


template <class NodeT,
          class... Args>
NodeT* Parser::makeNode(Args&&... args) const
{
    return new (pool_) NodeT(tree_, std::forward<Args>(args)...);
}

/**
 * Parse a comma-separated sequence of items. Whether to accept or not
 * a trailing comma is defined by the caller (within the function passed
 * as the item parser).
 */
template <class NodeT, class NodeListT>
bool Parser::parseCommaSeparatedItems(NodeListT*& nodeList,
                                      bool (Parser::*parseItem)(NodeT*& node, NodeListT*& nodeList))
{
    DEBUG_THIS_RULE();

    NodeT* node = nullptr;
    if (!(((this)->*parseItem)(node, nodeList)))
        return false;

    NodeListT** nodeList_cur = &nodeList;
    *nodeList_cur = makeNode<NodeListT>(node);

    while (peek().kind() == CommaToken) {
        (*nodeList_cur)->delimTkIdx_ = consume();
        nodeList_cur = &(*nodeList_cur)->next;

        *nodeList_cur = makeNode<NodeListT>();
        if (!(((this)->*parseItem)((*nodeList_cur)->value, nodeList)))
            return false;
    }
    return true;
}
