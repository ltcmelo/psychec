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

#include "binder/NameSpace.h"
#include "binder/Scope.h"

using namespace psy;
using namespace C;

struct Symbol::SymbolImpl
{
    SymbolImpl(const SyntaxTree* tree,
               const Symbol* containingSym,
               const Scope* enclosingScope,
               NameSpace ns,
               SymbolKind symK)
        : tree_(tree)
        , containingSym_(containingSym)
        , enclosingScope_(enclosingScope)
        , BF_all_(0)
    {
        BF_.ns_ = static_cast<std::uint32_t>(ns);
        BF_.symK_ = static_cast<std::uint32_t>(symK);
    }

    SymbolImpl(const SyntaxTree* tree,
               SymbolKind symK)
        : tree_(tree)
        , containingSym_(nullptr)
        , enclosingScope_(nullptr)
        , BF_all_(0)
    {
        BF_.symK_ = static_cast<std::uint32_t>(symK);
    }

    SymbolImpl(SymbolKind symK)
        : SymbolImpl(nullptr, symK)
    {
    }

    const SyntaxTree* tree_;
    const Symbol* containingSym_;
    const Scope* enclosingScope_;

    struct BitFields
    {
        // Symbol
        std::uint32_t symK_ : 3;
        std::uint32_t ns_ : 2;

        // Declaration
        std::uint32_t declSymK_: 2;
        std::uint32_t static_ : 1;

        // ObjectDeclaration
        std::uint32_t objDeclSymK_ : 3;

        // TypeDeclaration
        std::uint32_t tyDeclSymK_ : 3;

        // TagTypeDeclaration
        std::uint32_t tagTyDeclK_ : 2;
    };
    union
    {
        std::uint32_t BF_all_;
        BitFields BF_;
    };
};
