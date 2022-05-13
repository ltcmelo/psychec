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

#include "Symbol_Function.h"
#include "Symbol__IMPL__.inc"

#include "symbols/Symbol_ALL.h"

#include "../common/infra/Assertions.h"

#include <sstream>

using namespace psy;
using namespace C;

struct FunctionSymbol::FunctionSymbolImpl : SymbolImpl
{
    FunctionSymbolImpl(const SyntaxTree* tree,
                       const Scope* scope,
                       const Symbol* containingSym)
        : SymbolImpl(tree, scope, containingSym, SymbolKind::Function)
        , name_(nullptr)
        , tySym_(nullptr)
    {}

    std::unique_ptr<SymbolName> name_;
    const TypeSymbol* tySym_;
};

FunctionSymbol::FunctionSymbol(const SyntaxTree* tree,
                               const Scope* scope,
                               const Symbol* containingSym)
    : Symbol(new FunctionSymbolImpl(tree,
                                    scope,
                                    containingSym))
{}

const SymbolName* FunctionSymbol::name() const
{
    return P_CAST->name_.get();
}

void FunctionSymbol::setName(std::unique_ptr<SymbolName> symName)
{
    P_CAST->name_ = std::move(symName);
}

const TypeSymbol* FunctionSymbol::type() const
{
    return P_CAST->tySym_;
}

void FunctionSymbol::setType(const TypeSymbol* tySym)
{
    P_CAST->tySym_ = tySym;
}

namespace psy {
namespace C {

std::string to_string(const FunctionSymbol& sym)
{
    std::ostringstream oss;
    oss << "[@function |";
    oss << " " << (sym.name() ? to_string(*sym.name()) : "name:NULL");
    oss << " " << (sym.type() ? to_string(*sym.type()) : "type:NULL");
    oss << " " << (sym.scope() ? to_string(sym.scope()->kind()) : "scope:NULL");
    oss << " @]";

    return oss.str();
}

} // C
} // psy
