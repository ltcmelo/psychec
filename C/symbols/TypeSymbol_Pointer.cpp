// Copyright (c) 2021/22 Leandro T. C. Melo <ltcmelo@gmail.com>
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

#include "TypeSymbol_Pointer.h"
#include "TypeSymbol__IMPL__.inc"

#include <sstream>

using namespace psy;
using namespace C;

struct PointerTypeSymbol::PointerTypeSymbolImpl : TypeSymbolImpl
{
    PointerTypeSymbolImpl(const SyntaxTree* tree,
                          const Scope* scope,
                          const Symbol* containingSym,
                          const TypeSymbol* refedTySym)
        : TypeSymbolImpl(tree,
                         scope,
                         containingSym,
                         TypeKind::Pointer)
        , refedTySym_(refedTySym)
        , arrayDecay_(false)
        , functionDecay_(false)
    {}

    const TypeSymbol* refedTySym_;
    bool arrayDecay_;
    bool functionDecay_;
};

PointerTypeSymbol::PointerTypeSymbol(const SyntaxTree* tree,
                                     const Scope* scope,
                                     const Symbol* containingSym,
                                     const TypeSymbol* refedTySym)
    : TypeSymbol(new PointerTypeSymbolImpl(tree,
                                           scope,
                                           containingSym,
                                           refedTySym))
{}

const TypeSymbol* PointerTypeSymbol::referencedType() const
{
    return P_CAST->refedTySym_;
}

bool PointerTypeSymbol::arisesFromArrayDecay() const
{
    return P_CAST->arrayDecay_;
}

bool PointerTypeSymbol::arisesFromFunctionDecay() const
{
    return P_CAST->functionDecay_;
}

void PointerTypeSymbol::markAsArisingFromArrayDecay()
{
    P_CAST->arrayDecay_ = true;
}

void PointerTypeSymbol::markAsArisingFromFunctionDecay()
{
    P_CAST->functionDecay_ = true;
}

namespace psy {
namespace C {

std::string to_string(const PointerTypeSymbol& tySym)
{
    std::ostringstream oss;
    oss << "<*pointer type |";
    oss << " " << to_string(*tySym.referencedType());
    oss << " *>";

    return oss.str();
}

} // C
} // psy
