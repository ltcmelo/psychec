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

#include "Symbol_Value.h"
#include "Symbol__IMPL__.inc"

#include "symbols/Symbol_ALL.h"

#include <sstream>

using namespace psy;
using namespace C;

struct ValueSymbol::ValueSymbolImpl : SymbolImpl
{
    ValueSymbolImpl(const SyntaxTree* tree,
                    const Scope* scope,
                    const Symbol* containingSym,
                    ValueKind valKind)
        : SymbolImpl(tree, scope, containingSym, SymbolKind::Value)
        , valKind_(valKind)
        , name_(nullptr)
        , tySym_(nullptr)
    {}

    ValueKind valKind_;
    std::unique_ptr<SymbolName> name_;
    const TypeSymbol* tySym_;
};

ValueSymbol::ValueSymbol(const SyntaxTree* tree,
                          const Scope* scope,
                          const Symbol* containingSym,
                          ValueKind valKind)
    : Symbol(new ValueSymbolImpl(tree,
                                 scope,
                                 containingSym,
                                 valKind))
{}

ValueSymbol::~ValueSymbol()
{}

ValueKind ValueSymbol::valueKind() const
{
    return P_CAST->valKind_;
}

const SymbolName* ValueSymbol::name() const
{
    return P_CAST->name_.get();
}

const TypeSymbol* ValueSymbol::type() const
{
    return P_CAST->tySym_;
}

void ValueSymbol::setType(const TypeSymbol* tySym)
{
    P_CAST->tySym_ = tySym;
}

void ValueSymbol::setName(std::unique_ptr<SymbolName> symName)
{
    P_CAST->name_ = std::move(symName);
}

namespace psy {
namespace C {

std::string to_string(const ValueSymbol& sym)
{
    switch (sym.valueKind()) {
        case ValueKind::Enumerator:
            return to_string(*sym.asEnumerator());
        case ValueKind::Field:
            return to_string(*sym.asField());
        case ValueKind::Parameter:
            return to_string(*sym.asParameter());
        case ValueKind::Variable:
            return to_string(*sym.asVariable());
        default:
            PSY_ESCAPE_VIA_RETURN("");
            return "<INVALID or UNSPECIFIED value kind>";
    }
}

} // C
} // psy
