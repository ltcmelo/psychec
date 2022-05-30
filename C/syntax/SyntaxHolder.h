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

#ifndef PSYCHE_C_SYNTAX_HOLDER_H__
#define PSYCHE_C_SYNTAX_HOLDER_H__

#include "API.h"
#include "Fwds.h"

#include "parser/LexedTokens.h"

#include <variant>

namespace psy {
namespace C {

/**
 * \brief The SyntaxHolder class.
 *
 * A wrapper for any syntax form (or a handle to it).
 */
class PSY_C_NON_API SyntaxHolder
{
public:
    enum class Variant : char
    {
        Token,
        Node,
        NodeList,
    };

    Variant variant() const { return variant_; }

    bool isToken() const;
    bool isNode() const;
    bool isNodeList() const;

    SyntaxHolder(LexedTokens::IndexType tkIdx);
    SyntaxHolder(const SyntaxNode* node);
    SyntaxHolder(const SyntaxNodeList* nodeList);

    LexedTokens::IndexType tokenIndex() const;
    const SyntaxNode* node() const;
    const SyntaxNodeList* nodeList() const;

private:
    std::variant<LexedTokens::IndexType,
                 const SyntaxNode*,
                 const SyntaxNodeList*> syntax_;
    Variant variant_;
};

} // C
} // psy

#endif
