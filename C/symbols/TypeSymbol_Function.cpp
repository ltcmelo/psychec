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

#include "TypeSymbol_Function.h"
#include "TypeSymbol__IMPL__.inc"

#include <sstream>

using namespace psy;
using namespace C;

struct FunctionTypeSymbol::FunctionTypeSymbolImpl : TypeSymbolImpl
{
    FunctionTypeSymbolImpl(const SyntaxTree* tree,
                           const Scope* scope,
                           const Symbol* containingSym,
                           const TypeSymbol* retTySym)
        : TypeSymbolImpl(tree,
                         scope,
                         containingSym,
                         TypeKind::Function)
        , retTySym_(retTySym)
    {}

    const TypeSymbol* retTySym_;
    std::vector<const TypeSymbol*> parmTySyms_;
};

FunctionTypeSymbol::FunctionTypeSymbol(const SyntaxTree* tree,
                                       const Scope* scope,
                                       const Symbol* containingSym,
                                       const TypeSymbol* retTySym)
    : TypeSymbol(new FunctionTypeSymbolImpl(tree,
                                            scope,
                                            containingSym,
                                            retTySym))
{}

const TypeSymbol* FunctionTypeSymbol::returnType() const
{
    return P_CAST->retTySym_;
}

std::vector<const TypeSymbol*> FunctionTypeSymbol::parameterTypes() const
{
    return P_CAST->parmTySyms_;
}

void FunctionTypeSymbol::addParameter(const TypeSymbol* parmTySym)
{
    P_CAST->parmTySyms_.push_back(parmTySym);
}

namespace psy {
namespace C {

std::string to_string(const FunctionTypeSymbol& tySym)
{
    std::ostringstream oss;
    oss << "<@function type |";
    oss << " return:" << to_string(*tySym.returnType());
    oss << " @>";

    return oss.str();
}

} // C
} // psy
