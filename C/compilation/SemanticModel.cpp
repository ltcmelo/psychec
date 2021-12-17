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

#include "SemanticModel.h"

#include "Assembly.h"
#include "Compilation.h"

#include "binder/Binder.h"
#include "symbols/Symbols.h"

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
};

SemanticModel::SemanticModel(Compilation* compilation, const SyntaxTree* tree)
    : P(new SemanticModelImpl(compilation))
{
    Binder binder(this, tree);
    binder.bind();
}

SemanticModel::~SemanticModel()
{}

Symbol* SemanticModel::storeSymbol(std::unique_ptr<Symbol> sym)
{
    auto& syms = P->compilation_->assembly()->syms_;
    auto [it, _] = syms.insert(std::move(sym));
    return it->get();
}
