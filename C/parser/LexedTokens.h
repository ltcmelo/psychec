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

#ifndef PSYCHE_C_LEXED_TOKENS_H__
#define PSYCHE_C_LEXED_TOKENS_H__

#include "API.h"

#include "syntax/SyntaxToken.h"

#include <vector>

namespace psy {
namespace C {

/**
 * \brief The LexedTokens class.
 *
 * The container of all tokens lexed by the Lexer.
 */
class PSY_C_API LexedTokens
{
public:
    using SizeType = std::vector<SyntaxToken>::size_type;
    using IndexType = SizeType;

    void add(SyntaxToken tk);
    SyntaxToken& tokenAt(IndexType tkIdx);
    IndexType freeSlot() const;
    SizeType count() const;
    void clear();

    static IndexType invalidIndex();

private:
    std::vector<SyntaxToken> v_;
};

} // C
} // psy

#endif
