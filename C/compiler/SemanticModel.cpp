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

#include "SemanticModel.h"

#include "Binder.h"
#include "Compilation.h"

#include <unordered_set>
#include <vector>

using namespace psy;
using namespace C;

struct SemanticModel::SemanticModelImpl
{
    SemanticModelImpl()
    {}

    std::vector<std::unique_ptr<DeclarationName>> names_;
};

SemanticModel::SemanticModel(const SyntaxTree* tree)
    : P(new SemanticModelImpl())
{
    Binder binder(tree);
    binder.bind();
}

SemanticModel::~SemanticModel()
{}

template <class NameT,
          class... ArgsT>
NameT* SemanticModel::make(ArgsT&&... args)
{
    P->names_.emplace_back(new NameT(std::forward<ArgsT>(args)...));
    return static_cast<NameT*>(P->names_.back().get());
}

IdentifierName* SemanticModel::makeName(const Identifier* identifier)
{
    return make<IdentifierName>(identifier);
}

TagName* SemanticModel::makeName(TagName::TagKind typeSpecifierKind,
                                 const Identifier* identifier)
{
    return make<TagName>(typeSpecifierKind, identifier);
}

AnonymousName* SemanticModel::makeName()
{
    return make<AnonymousName>();
}
