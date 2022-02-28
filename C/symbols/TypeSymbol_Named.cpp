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

#include "TypeSymbol_Named.h"
#include "TypeSymbol__IMPL__.inc"

#include <iostream>

using namespace psy;
using namespace C;

struct NamedTypeSymbol::NamedTypeSymbolImpl : TypeSymbolImpl
{
    NamedTypeSymbolImpl(const SyntaxTree* tree,
                        const Scope* outerScope,
                        const Symbol* containingSym,
                        TypeNameKind typeNameKind)
        : TypeSymbolImpl(tree,
                         outerScope,
                         containingSym,
                         TypeKind::Named)
        , name_(nullptr)
        , typeNameKind_(typeNameKind)
        , builtinKind_(BuiltinTypeKind::None)
    {}

    std::unique_ptr<SymbolName> name_;
    TypeNameKind typeNameKind_;
    BuiltinTypeKind builtinKind_;
};

NamedTypeSymbol::NamedTypeSymbol(const SyntaxTree* tree,
                                 const Scope* outerScope,
                                 const Symbol* containingSym,
                                 BuiltinTypeKind builtinKind)
    : TypeSymbol(new NamedTypeSymbolImpl(tree,
                                         outerScope,
                                         containingSym,
                                         TypeNameKind::Builtin))
{
    P_CAST->name_.reset(new PlainSymbolName("int"));
}

NamedTypeSymbol::NamedTypeSymbol(const SyntaxTree* tree,
                                 const Scope* outerScope,
                                 const Symbol* containingSym,
                                 const std::string& name)
    : TypeSymbol(new NamedTypeSymbolImpl(tree,
                                         outerScope,
                                         containingSym,
                                         TypeNameKind::Synonym))
{
    P_CAST->name_.reset(new PlainSymbolName(name));
}

NamedTypeSymbol::NamedTypeSymbol(const SyntaxTree* tree,
                                 const Scope* outerScope,
                                 const Symbol* containingSym,
                                 TagSymbolName::NameSpace ns,
                                 const std::string& tag)
    : TypeSymbol(new NamedTypeSymbolImpl(tree,
                                         outerScope,
                                         containingSym,
                                         TypeNameKind::Tag))
{
    P_CAST->name_.reset(new TagSymbolName(ns, tag));
}

const SymbolName* NamedTypeSymbol::name() const
{
    return P_CAST->name_.get();
}

TypeNameKind NamedTypeSymbol::typeNameKind() const
{
    return P_CAST->typeNameKind_;
}

BuiltinTypeKind NamedTypeSymbol::builtinKind() const
{
    return P_CAST->builtinKind_;
}

void NamedTypeSymbol::patchBuiltinTypeKind(BuiltinTypeKind builtTyKind)
{
    P_CAST->builtinKind_ = builtTyKind;
}
