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

#include "SemanticModel.h"

#include "Assembly.h"
#include "Compilation.h"

#include "binder/Binder.h"
#include "symbols/Symbol_ALL.h"

#include "../common/infra/Assertions.h"

#include <unordered_map>
#include <unordered_set>
#include <vector>

using namespace psy;
using namespace C;

struct SemanticModel::SemanticModelImpl
{
    SemanticModelImpl(Compilation* compilation)
        : compilation_(compilation)
    {}

    Compilation* compilation_;
    std::unordered_map<const SyntaxNode*, Symbol*> declSyms_;
};

SemanticModel::SemanticModel(Compilation* compilation, const SyntaxTree* tree)
    : P(new SemanticModelImpl(compilation))
{
    Binder binder(this, tree);
    binder.bind();
}

SemanticModel::~SemanticModel()
{}

const Compilation* SemanticModel::compilation() const
{
    return P->compilation_;
}

const Symbol *SemanticModel::declaredSymbol(const DeclaratorSyntax* node) const
{
    return nullptr;
}

Symbol* SemanticModel::storeDeclaredSym(const SyntaxNode* node, std::unique_ptr<Symbol> sym)
{
    auto& allSyms = P->compilation_->assembly()->symDEFs_;

    auto [it, _] = allSyms.insert(std::move(sym));
    Symbol* rawSym = it->get();

    PSY_ASSERT(P->declSyms_.count(node) == 0, return rawSym);
    P->declSyms_[node] = rawSym;

    return rawSym;
}

Symbol* SemanticModel::storeUsedSym(std::unique_ptr<Symbol> sym)
{
    auto& syms = P->compilation_->assembly()->symUSEs_;
    syms.emplace_back(sym.release());
    return syms.back().get();
}
