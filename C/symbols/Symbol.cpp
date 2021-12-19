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

#include "Symbol__IMPL__.inc"

#include "compilation/Assembly.h"
#include "compilation/Compilation.h"
#include "syntax/SyntaxNodes.h"
#include "syntax/SyntaxReference.h"

#include "../common/infra/PsycheAssert.h"

#include <algorithm>
#include <sstream>

Symbol::Symbol(const SyntaxTree* tree,
               const Scope* scope,
               const Symbol* containingSym,
               SymbolKind kind)
    : P(new SymbolImpl(tree,
                       scope,
                       containingSym,
                       kind))
{
}

Symbol::~Symbol()
{}

const Assembly* Symbol::assembly() const
{
    for (auto compilation : P->tree_->linkedCompilations()) {
      const auto&& syms = compilation->assembly()->symbols();
      auto it = std::find(syms.begin(), syms.end(), this);
      if (it != syms.end())
          return compilation->assembly();
    }

    PSYCHE_FAIL(return nullptr, "expected assembly");
    return nullptr;
}

Accessibility Symbol::declaredAccessibility() const
{
    return P->access_;
}

std::vector<SyntaxReference> Symbol::declaringSyntaxReferences() const
{
    return {};
}

SymbolKind Symbol::kind() const
{
    return P->kind_;
}

SymbolName Symbol::name() const
{
    return P->name_;
}

Location Symbol::location() const
{
    const auto& synRefs = declaringSyntaxReferences();

    std::vector<Location> locs;
    std::transform(synRefs.begin(),
                   synRefs.end(),
                   std::back_inserter(locs),
                   [] (auto& synRef) {
                        return synRef.syntax()->firstToken().location();
                   });

    // TODO
    return locs.front();
}

template <class ScopeT>
ScopeT* Symbol::newScope()
{
    std::unique_ptr<ScopeT> scope(new ScopeT());
    P->innerScope_ = std::move(scope);
    return static_cast<ScopeT*>(P->innerScope_.get());
}

template BlockScope* Symbol::newScope<BlockScope>();
template FileScope* Symbol::newScope<FileScope>();
template FunctionScope* Symbol::newScope<FunctionScope>();

void Symbol::givePlainName(std::string s)
{
    P->name_ = PlainSymbolName(s);
}

namespace psy {
namespace C {

std::string to_string(const Symbol& sym)
{
    std::ostringstream oss;
    oss << "Symbol ";
    oss << to_string(sym.name()) << ' ';
    oss << to_string(sym.kind()) << ' ';

    return oss.str();
}

} // C
} // psy
