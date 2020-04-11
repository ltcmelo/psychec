// Copyright (c) 2019 Leandro T. C. Melo <ltcmelo@gmail.com>
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

#include "Unparser.h"

#include "AST.h"
#include "TranslationUnit.h"
#include <iostream>

using namespace psyche;

void Unparser::unparse(AST* ast, std::ostream& os)
{
    os_ = &os;
    accept(ast);
}

void Unparser::terminal(unsigned tk, AST* ast)
{
    auto tokk = translationUnit()->tokenKind(tk);
    if (tokk == T_EOF_SYMBOL)
        return;

    *os_ << spell(tk);
    if (tokk == T_RBRACE
            || tokk == T_LBRACE
            || tokk == T_SEMICOLON)
        *os_ << "\n";
    else
        *os_ << " ";
}
