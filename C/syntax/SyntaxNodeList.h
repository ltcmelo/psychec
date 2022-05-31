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

#ifndef PSYCHE_C_SYNTAX_NODE_LIST_H__
#define PSYCHE_C_SYNTAX_NODE_LIST_H__

#include "API.h"

#include "SyntaxToken.h"

#include "infra/List.h"
#include "parser/LexedTokens.h"

#include <iostream>

namespace psy {
namespace C {

class SyntaxTree;

class PSY_C_API SyntaxNodeList
{
public:

    /**
     * The first token of \c this SyntaxNodeList.
     */
    virtual SyntaxToken firstToken() const = 0;

    /**
     * The last token of \c this SyntaxNode.
     */
    virtual SyntaxToken lastToken() const = 0;

    static SyntaxToken token(LexedTokens::IndexType tkIdx, SyntaxTree* tree);

    virtual void acceptVisitor(SyntaxVisitor* visitor) = 0;
};


/**
 * \brief The CoreSyntaxNodeList class \b template.
 *
 * The base class of every syntax list.
 */
template <class SyntaxNodeT,
          class DerivedListT>
class PSY_C_API CoreSyntaxNodeList
        : public List<SyntaxNodeT, DerivedListT>
        , public SyntaxNodeList
{
public:
    CoreSyntaxNodeList(SyntaxTree* tree)
        : List<SyntaxNodeT, DerivedListT>()
        , tree_(tree)
    {}

    CoreSyntaxNodeList(SyntaxTree* tree, const SyntaxNodeT& node)
        : List<SyntaxNodeT, DerivedListT>(node)
        , tree_(tree)
    {}

    CoreSyntaxNodeList(const CoreSyntaxNodeList&) = delete;
    CoreSyntaxNodeList& operator=(const CoreSyntaxNodeList&) = delete;

    virtual SyntaxToken firstToken() const override
    {
        if (this->value)
            return this->value->firstToken();
        return SyntaxToken::invalid();
    }

    virtual SyntaxToken lastToken() const override
    {
        SyntaxNodeT node = this->lastValue();
        if (node)
            return node->lastToken();
        return SyntaxToken::invalid();
    }

    virtual void acceptVisitor(SyntaxVisitor* visitor) override
    {
        for (auto it = this; it; it = it->next) {
            SyntaxNodeT node = static_cast<SyntaxNodeT>(it->value);
            if (node)
                node->acceptVisitor(visitor);
        }
    }

    SyntaxTree* tree_;
};


/**
 * \brief The SyntaxNodePlainList class (template).
 */
template <class SyntaxNodeT>
class PSY_C_API SyntaxNodePlainList final
        : public CoreSyntaxNodeList<SyntaxNodeT,
                                    SyntaxNodePlainList<SyntaxNodeT>>
{
public:
    using CoreSyntaxNodeList<SyntaxNodeT, SyntaxNodePlainList<SyntaxNodeT>>::CoreSyntaxNodeList;
    using NodeType = SyntaxNodeT;
};


/**
 * \brief The SyntaxNodeSeparatedList class (template).
 */
template <class SyntaxNodeT>
class PSY_C_API SyntaxNodeSeparatedList final
        : public CoreSyntaxNodeList<SyntaxNodeT,
                                    SyntaxNodeSeparatedList<SyntaxNodeT>>
{
public:
    SyntaxToken delimiterToken() const;

    using Base = CoreSyntaxNodeList<SyntaxNodeT, SyntaxNodeSeparatedList<SyntaxNodeT>>;
    using NodeType = SyntaxNodeT;
    using CoreSyntaxNodeList<SyntaxNodeT,
                             SyntaxNodeSeparatedList<SyntaxNodeT>>::CoreSyntaxNodeList;

    unsigned delimTkIdx_ = 0;
};

} // C
} // psy

#endif
