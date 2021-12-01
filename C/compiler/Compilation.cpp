// Copyright (c) 2016/17/18/19/20/21 Leandro T. C. Melo <ltcmelo@gmail.com>
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

using namespace psy;
using namespace C;

struct Compilation::CompilationImpl
{
    CompilationImpl(Compilation* q)
        : Q_(q)
    {}

    Compilation* Q_;
    std::string id_;
    std::unique_ptr<SyntaxTree> tree_;
    std::unique_ptr<SemanticModel> semaModel_;
};

Compilation::Compilation()
    : P(new CompilationImpl(this))
{}

Compilation::~Compilation()
{}

std::unique_ptr<Compilation> Compilation::create(const std::string& id,
                                                 std::unique_ptr<SyntaxTree> tree)
{
    std::unique_ptr<Compilation> compilation(new Compilation);
    compilation->P->id_ = id;
    compilation->P->tree_ = std::move(tree);
    return compilation;
}

const SyntaxTree* Compilation::syntaxTree() const
{
    return P->tree_.get();
}

const SemanticModel* Compilation::semanticModel() const
{
    std::unique_ptr<SemanticModel> semaModel(new SemanticModel(P->tree_.get()));
    P->semaModel_ = std::move(semaModel);
    return P->semaModel_.get();
}
