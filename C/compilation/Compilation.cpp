// Copyright (c) 2016/17/18/19/20/21/22 Leandro T. C. Melo <ltcmelo@gmail.com>
// Copyright (c) 2008 Roberto Raggi <roberto.raggi@gmail.com>
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

#include "Compilation.h"

#include "Assembly.h"
#include "SemanticModel.h"
#include "SyntaxTree.h"

#include <algorithm>
#include <unordered_map>

using namespace psy;
using namespace C;

struct Compilation::CompilationImpl
{
    CompilationImpl(Compilation* q)
        : Q_(q)
        , assembly_(new Assembly)
    {}

    Compilation* Q_;
    std::string id_;
    std::unique_ptr<Assembly> assembly_;
    std::unordered_map<const SyntaxTree*, bool> isDirty_;
    std::unordered_map<const SyntaxTree*, std::unique_ptr<SemanticModel>> semaModels_;
};

Compilation::Compilation()
    : P(new CompilationImpl(this))
{}

Compilation::~Compilation()
{
    for (const auto& kv : P->semaModels_) {
        auto tree = kv.first;

        auto it = P->semaModels_.find(tree);
        if (it == P->semaModels_.end())
            continue;
        tree->detachCompilation(this);
    }
}

std::unique_ptr<Compilation> Compilation::create(const std::string& id)
{
    std::unique_ptr<Compilation> compilation(new Compilation);
    compilation->P->id_ = id;
    return compilation;
}

const Assembly* Compilation::assembly() const
{
    return P->assembly_.get();
}

Assembly* Compilation::assembly()
{
    return P->assembly_.get();
}

void Compilation::addSyntaxTree(const SyntaxTree* tree)
{
    auto it = P->semaModels_.find(tree);
    if (it != P->semaModels_.end())
        return;

    P->semaModels_.insert(it, std::make_pair(tree, nullptr));
    P->isDirty_[tree] = true;
    tree->attachCompilation(this);
}

void Compilation::addSyntaxTrees(std::vector<const SyntaxTree*> trees)
{
    for (auto tree : trees)
        addSyntaxTree(tree);
}

std::vector<const SyntaxTree*> Compilation::syntaxTrees() const
{
    std::vector<const SyntaxTree*> trees(P->semaModels_.size());
    std::transform(P->semaModels_.begin(),
                   P->semaModels_.end(),
                   std::back_inserter(trees),
                   [] (const auto& kv) { return kv.first; });
    return  trees;
}

const SemanticModel* Compilation::semanticModel(const SyntaxTree* tree) const
{
    if (P->isDirty_[tree]) {
        // TODO: Remove from the assembly the symbols associated
        // with the given syntax tree.
        P->semaModels_[tree].reset(new SemanticModel(tree, const_cast<Compilation*>(this)));
        P->isDirty_[tree] = false;
    }

    return P->semaModels_[tree].get();
}
