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
#include "syntax/SyntaxTree.h"

#include "sema/DeclarationBinder.h"
#include "sema/TypeCanonicalizer.h"
#include "sema/TypedefNameTypeResolver.h"
#include "symbols/Symbol_ALL.h"
#include "types/Type_ALL.h"
#include "sema/TypeChecker.h"

#include <algorithm>
#include <unordered_map>

using namespace psy;
using namespace C;

struct Compilation::CompilationImpl
{
    CompilationImpl(Compilation* q)
        : Q_(q)
        , tyVoid_(new VoidType)
        , tyChar_(new BasicType(BasicTypeKind::Char))
        , tyCharS_(new BasicType(BasicTypeKind::Char_S))
        , tyCharU_(new BasicType(BasicTypeKind::Char_U))
        , tyShortS_(new BasicType(BasicTypeKind::Short_S))
        , tyShortU_(new BasicType(BasicTypeKind::Short_U))
        , tyIntS_(new BasicType(BasicTypeKind::Int_S))
        , tyIntU_(new BasicType(BasicTypeKind::Int_U))
        , tyLongS_(new BasicType(BasicTypeKind::Long_S))
        , tyLongU_(new BasicType(BasicTypeKind::Long_U))
        , tyLongLongS_(new BasicType(BasicTypeKind::LongLong_S))
        , tyLongLongU_(new BasicType(BasicTypeKind::LongLong_U))
        , tyFloat_(new BasicType(BasicTypeKind::Float))
        , tyDouble_(new BasicType(BasicTypeKind::Double))
        , tyLongDouble_(new BasicType(BasicTypeKind::LongDouble))
        , tyFloatComplex_(new BasicType(BasicTypeKind::FloatComplex))
        , tyDoubleComplex_(new BasicType(BasicTypeKind::DoubleComplex))
        , tyLongDoubleComplex_(new BasicType(BasicTypeKind::LongDoubleComplex))
        , tyBool_(new BasicType(BasicTypeKind::Bool))
        , tyErr_(new ErrorType())
        , prog_(new ProgramSymbol)
    {}

    Compilation* Q_;
    std::string id_;
    PlatformOptions platformOpts_;
    InferenceOptions inferOpts_;
    std::unique_ptr<VoidType> tyVoid_;
    std::unique_ptr<BasicType> tyChar_;
    std::unique_ptr<BasicType> tyCharS_;
    std::unique_ptr<BasicType> tyCharU_;
    std::unique_ptr<BasicType> tyShortS_;
    std::unique_ptr<BasicType> tyShortU_;
    std::unique_ptr<BasicType> tyIntS_;
    std::unique_ptr<BasicType> tyIntU_;
    std::unique_ptr<BasicType> tyLongS_;
    std::unique_ptr<BasicType> tyLongU_;
    std::unique_ptr<BasicType> tyLongLongS_;
    std::unique_ptr<BasicType> tyLongLongU_;
    std::unique_ptr<BasicType> tyFloat_;
    std::unique_ptr<BasicType> tyDouble_;
    std::unique_ptr<BasicType> tyLongDouble_;
    std::unique_ptr<BasicType> tyFloatComplex_;
    std::unique_ptr<BasicType> tyDoubleComplex_;
    std::unique_ptr<BasicType> tyLongDoubleComplex_;
    std::unique_ptr<BasicType> tyBool_;
    std::unique_ptr<ErrorType> tyErr_;
    std::unique_ptr<ProgramSymbol> prog_;
    std::unordered_map<const SyntaxTree*, bool> isDirty_;
    std::unordered_map<const SyntaxTree*, std::unique_ptr<SemanticModel>> semaModels_;
    std::vector<std::unique_ptr<const SyntaxTree>> trees_;
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

std::unique_ptr<Compilation> Compilation::create(
        const std::string& id,
        PlatformOptions platformOpts,
        InferenceOptions inferOpts)
{
    std::unique_ptr<Compilation> compilation(new Compilation);
    compilation->P->id_ = id;
    compilation->P->platformOpts_ = platformOpts;
    compilation->P->inferOpts_ = inferOpts;
    return compilation;
}

const std::string& Compilation::Id() const
{
    return P->id_;
}

const InferenceOptions& Compilation::inferenceOptions() const
{
    return P->inferOpts_;
}

const PlatformOptions& Compilation::platformOptions() const
{
    return P->platformOpts_;
}

bool Compilation::isEmpty() const
{
    return P->trees_.empty();
}

const ProgramSymbol* Compilation::program() const
{
    return P->prog_.get();
}

ProgramSymbol* Compilation::program()
{
    return P->prog_.get();
}

void Compilation::addSyntaxTree(std::unique_ptr<const SyntaxTree> tree)
{
    auto it = P->semaModels_.find(tree.get());
    if (it != P->semaModels_.end())
        return;

    auto tree_RAW = tree.get();
    P->trees_.emplace_back(std::move(tree));

    P->semaModels_.insert(
        it,
        std::make_pair(
            tree_RAW,
            new SemanticModel(tree_RAW, const_cast<Compilation*>(this))));
    P->isDirty_[tree_RAW] = true;
    tree_RAW->attachCompilation(this);
}

std::vector<const SyntaxTree*> Compilation::syntaxTrees() const
{
    std::vector<const SyntaxTree*> trees;
    trees.reserve(P->trees_.size());
    for (const auto& tree : P->trees_)
        trees.push_back(tree.get());
    return trees;
}

void Compilation::computeSemanticModel(const SyntaxTree* tree)
{
    PSY_ASSERT_2(P->isDirty_.count(tree), return);
    if (P->isDirty_[tree]) {
        bindDeclarations(tree);
        canonicalizeTypes(tree);
        resolveTypedefNameTypes(tree);
        checkTypes(tree);
        P->isDirty_[tree] = false;
    }
}

const VoidType* Compilation::canonicalVoidType() const
{
    return P->tyVoid_.get();
}

const ErrorType* Compilation::canonicalErrorType() const
{
    return P->tyErr_.get();
}

const BasicType* Compilation::canonicalBasicType(BasicTypeKind basicTyK) const
{
    switch (basicTyK) {
        case BasicTypeKind::Char:
            return P->tyChar_.get();
        case BasicTypeKind::Char_S:
            return P->tyCharS_.get();
        case BasicTypeKind::Char_U:
            return P->tyCharU_.get();
        case BasicTypeKind::Short_S:
            return P->tyShortS_.get();
        case BasicTypeKind::Short_U:
            return P->tyShortU_.get();
        case BasicTypeKind::Int_S:
            return P->tyIntS_.get();
        case BasicTypeKind::Int_U:
            return P->tyIntU_.get();
        case BasicTypeKind::Long_S:
            return P->tyLongS_.get();
        case BasicTypeKind::Long_U:
            return P->tyLongU_.get();
        case BasicTypeKind::LongLong_S:
            return P->tyLongLongS_.get();
        case BasicTypeKind::LongLong_U:
            return P->tyLongLongU_.get();
        case BasicTypeKind::Float:
            return P->tyFloat_.get();
        case BasicTypeKind::Double:
            return P->tyDouble_.get();
        case BasicTypeKind::LongDouble:
            return P->tyLongDouble_.get();
        case BasicTypeKind::FloatComplex:
            return P->tyFloatComplex_.get();
        case BasicTypeKind::DoubleComplex:
            return P->tyDoubleComplex_.get();
        case BasicTypeKind::LongDoubleComplex:
            return P->tyLongDoubleComplex_.get();
        case BasicTypeKind::Bool:
            return P->tyBool_.get();
    }
    PSY_ASSERT_1(false);
    return P->tyIntU_.get();
}

void Compilation::bindDeclarations(const SyntaxTree* tree) const
{
    PSY_ASSERT_2(P->semaModels_.count(tree), return);
    auto model = P->semaModels_[tree].get();
    DeclarationBinder binder(model, tree);
    binder.bindDeclarations();
}

void Compilation::bindDeclarations() const
{
    for (const auto& p : P->semaModels_) {
        DeclarationBinder binder(p.second.get(), p.first);
        binder.bindDeclarations();
    }
}

void Compilation::canonicalizeTypes(const SyntaxTree* tree) const
{
    PSY_ASSERT_2(P->semaModels_.count(tree), return);
    auto model = P->semaModels_[tree].get();
    TypeCanonicalizer canonicalizer(model, tree);
    canonicalizer.canonicalizeTypes();
}

void Compilation::canonicalizeTypes() const
{
    for (const auto& p : P->semaModels_) {
        TypeCanonicalizer canonicalizer(p.second.get(), p.first);
        canonicalizer.canonicalizeTypes();
    }
}


void Compilation::resolveTypedefNameTypes(const SyntaxTree* tree) const
{
    PSY_ASSERT_2(P->semaModels_.count(tree), return);
    auto model = P->semaModels_[tree].get();
    TypedefNameTypeResolver resolver(model, tree);
    resolver.resolveTypedefNameTypes();
}

void Compilation::resolveTypedefNameTypes() const
{
    for (const auto& p : P->semaModels_) {
        TypedefNameTypeResolver resolver(p.second.get(), p.first);
        resolver.resolveTypedefNameTypes();
    }
}

void Compilation::checkTypes(const SyntaxTree* tree) const
{
    PSY_ASSERT_2(P->semaModels_.count(tree), return);
    auto model = P->semaModels_[tree].get();
    TypeChecker checker(model, tree);
    checker.checkTypes();
}

void Compilation::checkTypes() const
{
    for (const auto& p : P->semaModels_) {
        TypeChecker checker(p.second.get(), p.first);
        checker.checkTypes();
    }
}

const SemanticModel* Compilation::semanticModel(const SyntaxTree* tree) const
{
    PSY_ASSERT_2(P->semaModels_.count(tree), return nullptr);
    return P->semaModels_[tree].get();
}
