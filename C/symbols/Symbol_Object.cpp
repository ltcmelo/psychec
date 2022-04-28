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

#include "Symbol_Object.h"
#include "Symbol__IMPL__.inc"

#include "symbols/Symbols.h"

#include <sstream>

using namespace psy;
using namespace C;

struct ObjectSymbol::ObjectSymbolImpl : SymbolImpl
{
    ObjectSymbolImpl(const SyntaxTree* tree,
                    const Scope* enclosingScope,
                    const Symbol* containingSym,
                    ObjectKind objKind)
        : SymbolImpl(tree, enclosingScope, containingSym, SymbolKind::Object)
        , objKind_(objKind)
        , name_(nullptr)
        , tySym_(nullptr)
    {}

    ObjectKind objKind_;
    std::unique_ptr<SymbolName> name_;
    const TypeSymbol* tySym_;
};

ObjectSymbol::ObjectSymbol(const SyntaxTree* tree,
                         const Scope* enclosingScope,
                         const Symbol* containingSym,
                         ObjectKind objKind)
    : Symbol(new ObjectSymbolImpl(tree,
                                 enclosingScope,
                                 containingSym,
                                 objKind))
{}

ObjectSymbol::~ObjectSymbol()
{}

ObjectKind ObjectSymbol::objectKind() const
{
    return P_CAST->objKind_;
}

const SymbolName* ObjectSymbol::name() const
{
    return P_CAST->name_.get();
}

const TypeSymbol* ObjectSymbol::type() const
{
    return P_CAST->tySym_;
}

void ObjectSymbol::setType(const TypeSymbol* tySym)
{
    P_CAST->tySym_ = tySym;
}

void ObjectSymbol::setName(std::unique_ptr<SymbolName> symName)
{
    P_CAST->name_ = std::move(symName);
}

namespace psy {
namespace C {

std::string to_string(const ObjectSymbol& sym)
{
    switch (sym.objectKind()) {
        case ObjectKind::Field:
            return to_string(*sym.asField());
        case ObjectKind::Parameter:
            return to_string(*sym.asParameter());
        case ObjectKind::Variable:
            return to_string(*sym.asVariable());
        default:
            PSYCHE_FAIL_0(return "");
            return "<invalid object kind>";
    }
}

} // C
} // psy
