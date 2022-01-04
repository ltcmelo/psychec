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

#include "Symbol_Value.h"
#include "Symbol__IMPL__.inc"

#include "Symbol_Type.h"

using namespace psy;
using namespace C;

struct ValueSymbol::ValueSymbolImpl : SymbolImpl
{
    ValueSymbolImpl(const SyntaxTree* tree,
                    const Scope* outerScope,
                    const Symbol* containingSym,
                    ValueKind valKind)
        : SymbolImpl(tree, outerScope, containingSym, SymbolKind::Value)
        , valKind_(valKind)
    {}

    ValueKind valKind_;
    std::unique_ptr<TypeSymbol> tySym_;
};

ValueSymbol::ValueSymbol(const SyntaxTree* tree,
                         const Scope* outerScope,
                         const Symbol* containingSym,
                         ValueKind valKind)
    : Symbol(new ValueSymbolImpl(tree,
                                 outerScope,
                                 containingSym,
                                 valKind))
{}

ValueSymbol::~ValueSymbol()
{}

ValueKind ValueSymbol::valueKind() const
{
    return P_CAST->valKind_;
}

const TypeSymbol* ValueSymbol::type() const
{
    return P_CAST->tySym_.get();
}

TypeSymbol* ValueSymbol::type()
{
    return P_CAST->tySym_.get();
}

TypeSymbol *ValueSymbol::releaseType()
{
    return P_CAST->tySym_.release();
}

TypeSymbol* ValueSymbol::giveType(std::unique_ptr<TypeSymbol> tySym)
{
    P_CAST->tySym_ = std::move(tySym);
    return P_CAST->tySym_.get();
}
