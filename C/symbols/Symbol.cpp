// Copyright (c) 2021 Leandro T. C. Melo <ltcmelo@gmail.com>
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

#include "Symbol.h"

using namespace psy;
using namespace C;

struct Symbol::SymbolImpl
{
    SymbolImpl(const SyntaxTree* tree, SymbolKind kind)
        : tree_(tree)
        , kind_(kind)
    {}

    const SyntaxTree* tree_;
    SymbolKind kind_;
    SymbolName name_;
    Accessibility access_;
};

Symbol::Symbol(const SyntaxTree* tree,
               SymbolKind kind)
    : P(new SymbolImpl(tree, kind))
{
}

Symbol::~Symbol()
{}

Accessibility Symbol::declaredAccessibility() const
{
    return P->access_;
}

std::vector<SyntaxReference> Symbol::declaringSyntaxReferences() const
{
    return {};
}

SymbolKind Symbol::kind() const
{
    return P->kind_;
}

SymbolName Symbol::name() const
{
    return P->name_;
}

Location Symbol::location() const
{

}
