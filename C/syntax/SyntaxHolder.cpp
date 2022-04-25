// Copyright (c) 2020/21/22 Leandro T. C. Melo <ltcmelo@gmail.com>
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

#include "SyntaxHolder.h"

using namespace psy;
using namespace C;

SyntaxHolder::SyntaxHolder(LexedTokens::IndexType tkIdx)
    : syntax_(tkIdx)
    , variant_(Variant::Token)
{}

SyntaxHolder::SyntaxHolder(const SyntaxNode* node)
    : syntax_(node)
    , variant_(Variant::Node)
{}

SyntaxHolder::SyntaxHolder(const SyntaxNodeList* nodeList)
    : syntax_(nodeList)
    , variant_(Variant::NodeList)
{}

LexedTokens::IndexType SyntaxHolder::tokenIndex() const
{
    return std::get<LexedTokens::IndexType>(syntax_);
}

const SyntaxNode* SyntaxHolder::node() const
{
    return std::get<const SyntaxNode*>(syntax_);
}

const SyntaxNodeList* SyntaxHolder::nodeList() const
{
    return std::get<const SyntaxNodeList*>(syntax_);
}

bool SyntaxHolder::isToken() const
{
    return variant_ == Variant::Token;
}

bool SyntaxHolder::isNode() const
{
    return variant_ == Variant::Node;
}

bool SyntaxHolder::isNodeList() const
{
    return variant_ == Variant::NodeList;
}
