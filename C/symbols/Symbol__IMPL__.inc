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

using namespace psy;
using namespace C;

struct Symbol::SymbolImpl
{
    SymbolImpl(const SyntaxTree* tree,
               const Scope* scope,
               const Symbol* containingSym,
               SymbolKind symKind)
        : tree_(tree)
        , scope_(scope)
        , ns_(nullptr) // TODO
        , containingSym_(containingSym)
        , BF_all_(0)
    {
        BF_.symK_ = static_cast<std::uint32_t>(symKind);
    }

    const SyntaxTree* tree_;
    const Scope* scope_;
    const NameSpace* ns_;
    const Symbol* containingSym_;

    struct BitFields
    {
        // Symbol
        std::uint32_t symK_ : 3;

        // DeclarationSymbol
        std::uint32_t declSymK_: 2;
        std::uint32_t static_ : 1;

        // ObjectDeclarationSymbol
        std::uint32_t valSymK_ : 3;

        // TypeDeclarationSymbol
        std::uint32_t tySymK_ : 3;
    };
    union
    {
        std::uint32_t BF_all_;
        BitFields BF_;
    };
};
