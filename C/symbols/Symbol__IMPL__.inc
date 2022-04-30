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

#include "SyntaxTree.h"

#include "binder/Scope.h"
#include "binder/NameSpace.h"
#include "symbols/SymbolName_ALL.h"

using namespace psy;
using namespace C;

struct Symbol::SymbolImpl
{
    SymbolImpl(const SyntaxTree* tree,
               const Scope* scope,
               const Symbol* containingSym,
               SymbolKind kind)
        : tree_(tree)
        , scope_(scope)
        , ns_(nullptr) // TODO
        , containingSym_(containingSym)
        , kind_(kind)
        , BF_all_(0)
    {}

    const SyntaxTree* tree_;
    const Scope* scope_;
    const NameSpace* ns_;
    const Symbol* containingSym_;
    SymbolKind kind_;
    Accessibility access_;

    struct BitFields
    {
        // Symbol
        std::uint16_t static_ : 1;

        // TypeSymbol.
        std::uint16_t const_ : 1;
        std::uint16_t volatile_ : 1;
        std::uint16_t restrict_ : 1;
    };
    union
    {
        std::uint16_t BF_all_;
        BitFields BF_;
    };
};
