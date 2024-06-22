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

#include "Scope.h"

#include "symbols/Symbol_Declaration.h"

#include "../common/infra/Assertions.h"

#include <iostream>

using namespace psy;
using namespace C;

Scope::Scope(ScopeKind K)
    : K_(K)
    , outerScope_(nullptr)
{}

ScopeKind Scope::kind() const
{
    return K_;
}

const DeclarationSymbol* Scope::searchForDeclaration(
        const Identifier* ident,
        NameSpace ns) const
{
    auto key = std::make_pair(ident, ns);
    auto decl = decls_.find(key);
    if (decl != decls_.end())
        return decl->second;
    return outerScope_ != nullptr
            ? outerScope_->searchForDeclaration(ident, ns)
            : nullptr;
}

void Scope::encloseScope(std::unique_ptr<Scope> innerScope)
{
    innerScope->outerScope_ = this;
    innerScopes_.push_back(std::move(innerScope));
}

void Scope::morphFrom_FunctionPrototype_to_Block()
{
    PSY_ASSERT(K_ == ScopeKind::FunctionPrototype, return);

    K_ = ScopeKind::Block;
}

void Scope::addDeclaration(const DeclarationSymbol* decl)
{
    auto key = std::make_pair(decl->identifier(), decl->nameSpace());
    auto it = decls_.find(key);
    if (it != decls_.end()) {
        // TODO: Indicate (bool) and report.
        return;
    }
    decls_.insert(std::make_pair(key, decl));
}

Scope::~Scope()
{}
