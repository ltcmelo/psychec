// Copyright (c) 2022 Leandro T. C. Melo <ltcmelo@gmail.com>
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

#include "MIXIN_TypeableDeclarationSymbol.h"

#include "Symbol_Declaration.h"

using namespace psy;
using namespace C;

MIXIN_TypeableDeclarationSymbol* MIXIN_TypeableDeclarationSymbol::
    from(DeclarationSymbol* decl)
{
    return dynamic_cast<MIXIN_TypeableDeclarationSymbol*>(decl);
}

const MIXIN_TypeableDeclarationSymbol* MIXIN_TypeableDeclarationSymbol::
    from(const DeclarationSymbol* decl)
{
    return dynamic_cast<const MIXIN_TypeableDeclarationSymbol*>(decl);
}
