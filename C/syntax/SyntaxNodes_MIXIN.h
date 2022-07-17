// Copyright (c) 2016/17/18/19/20/21/22 Leandro T. C. Melo <ltcmelo@gmail.com>
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

#ifndef PSYCHE_C_SYNTAX_NODES_MIXIN_H__
#define PSYCHE_C_SYNTAX_NODES_MIXIN_H__

#include "SyntaxNode.h"
#include "SyntaxToken.h"
#include "SyntaxTree.h"

namespace psy {
namespace C {

class MIXIN_LeftExpressionInfixOperatorRightExpression
{
protected:
    friend class Parser;
    friend class Disambiguator;

    virtual const ExpressionSyntax* left() const = 0;
    virtual SyntaxToken operatorToken() const = 0;
    virtual const ExpressionSyntax* right() const = 0;

    ExpressionSyntax* leftExpr_ = nullptr;
    LexedTokens::IndexType oprtrTkIdx_ = LexedTokens::invalidIndex();
    ExpressionSyntax* rightExpr_ = nullptr;
};

class MIXIN_GNUExtensionFlag
{
protected:
    friend class Parser;
    friend class Disambiguator;

    virtual SyntaxToken extensionKeyword() const = 0;

    LexedTokens::IndexType extKwTkIdx_ = LexedTokens::invalidIndex();
};

} // C
} // psy

#endif
