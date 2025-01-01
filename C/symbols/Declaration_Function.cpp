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

#include "Declaration__IMPL__.inc"
#include "Declaration_Function.h"

#include "symbols/Symbol_ALL.h"
#include "syntax/Lexeme_Identifier.h"
#include "types/Type_ALL.h"

#include <sstream>

using namespace psy;
using namespace C;

struct Function::FunctionImpl : DeclarationImpl
{
    FunctionImpl(const Symbol* containingSym,
                 const SyntaxTree* tree,
                 const Scope* enclosingScope)
        : DeclarationImpl(containingSym,
                          DeclarationKind::Function,
                          tree,
                          enclosingScope,
                          NameSpace::OrdinaryIdentifiers)
        , name_(nullptr)
        , ty_(nullptr)
    {}

    const Identifier* name_;
    const Type* ty_;
};

Function::Function(const Symbol* containingSym,
                   const SyntaxTree* tree,
                   const Scope* enclosingScope)
    : Declaration(
          new FunctionImpl(containingSym,
                           tree,
                           enclosingScope))
{}

const Identifier* Function::name() const
{
    return P_CAST->name_;
}

void Function::setName(const Identifier* name)
{
    P_CAST->name_ = name;
}

const Type* Function::type() const
{
    return P_CAST->ty_;
}

const Type* Function::retypeableType() const
{
    return type();
}

void Function::setType(const Type* ty)
{
    P_CAST->ty_ = ty;
}

std::string Function::toDisplayString() const
{
    return "";
}

namespace psy {
namespace C {

std::string to_string(const Function* func)
{
    if (!func)
        return "<Function is null>";
    std::ostringstream oss;
    oss << "<Function |";
    oss << " name:" << func->name()->valueText();
    oss << " type:" << to_string(func->type());
    oss << ">";
    return oss.str();
}

} // C
} // psy
