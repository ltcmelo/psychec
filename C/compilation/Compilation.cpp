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

#include "SemanticModel.h"
#include "SyntaxTree.h"

#include "binder/Binder.h"
#include "binder/TypeResolver.h"
#include "symbols/Symbol_ALL.h"

#include <algorithm>
#include <unordered_map>

using namespace psy;
using namespace C;

struct Compilation::CompilationImpl
{
    CompilationImpl(Compilation* q)
        : Q_(q)
        , program_(new Program)
    {}

    Compilation* Q_;
    std::string id_;
    std::unique_ptr<Program> program_;
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

const Program* Compilation::program() const
{
    return P->program_.get();
}

Program* Compilation::program()
{
    return P->program_.get();
}

void Compilation::addSyntaxTree(const SyntaxTree* tree)
{
    auto it = P->semaModels_.find(tree);
    if (it != P->semaModels_.end())
        return;

    P->semaModels_.insert(
        it,
        std::make_pair(
            tree,
            new SemanticModel(tree, const_cast<Compilation*>(this))));
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

const SemanticModel* Compilation::analyze(const SyntaxTree* tree) const
{
    PSY_ASSERT_2(P->isDirty_.count(tree), return nullptr);
    if (P->isDirty_[tree]) {
        bind();
        resolveTypes();
        P->isDirty_[tree] = false;
    }
    return semanticModel(tree);
}

void Compilation::bind() const
{
    for (const auto& p : P->semaModels_) {
        Binder binder(p.second.get(), p.first);
        binder.bind();
    }
}

void Compilation::resolveTypes() const
{
    for (const auto& p : P->semaModels_) {
        TypeResolver tyResolver(p.second.get(), p.first);
        tyResolver.resolveTypes();
    }
}

const SemanticModel* Compilation::semanticModel(const SyntaxTree* tree) const
{
    PSY_ASSERT_2(P->semaModels_.count(tree), return nullptr);
    return P->semaModels_[tree].get();
}
