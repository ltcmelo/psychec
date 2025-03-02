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
#include "syntax/Lexeme_Identifier.h"

#include "../common/infra/Assertions.h"

#include <algorithm>
#include <iterator>
#include <iostream>

using namespace psy;
using namespace C;

Scope::Scope(ScopeKind scopeK)
    : scopeK_(scopeK)
    , outerScope_(nullptr)
{}

ScopeKind Scope::kind() const
{
    return scopeK_;
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

std::vector<const DeclarationSymbol*> Scope::declarations() const
{
    std::vector<const DeclarationSymbol*> decls;
    decls.reserve(decls_.size());
    std::transform(decls_.begin(),
                   decls_.end(),
                   std::back_inserter(decls),
                   [] (auto p) { return p.second; });
    return decls;
}

const Scope* Scope::outerScope() const
{
    return outerScope_;
}

void Scope::encloseScope(Scope* innerScope)
{
    innerScope->outerScope_ = this;
}

void Scope::morphFrom_FunctionPrototype_to_Block()
{
    PSY_ASSERT_2(scopeK_ == ScopeKind::FunctionPrototype, return);
    scopeK_ = ScopeKind::Block;
}

void Scope::addDeclaration(const DeclarationSymbol* decl)
{
    auto key = std::make_pair(decl->denotingIdentifier(), decl->nameSpace());
    auto it = decls_.find(key);
    if (it != decls_.end()) {
        // TODO: if not nullptr identifier, indicate (bool) and report.
        return;
    }
    decls_.insert(std::make_pair(key, decl));
}

Scope::~Scope()
{}
