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
#include "syntax/SyntaxNodes.h"
#include "syntax/SyntaxUtilities.h"
#include "symbols/Symbol_ALL.h"

#include "../common/infra/Assertions.h"
#include "../common/infra/Escape.h"

#include <iostream>
#include <unordered_map>
#include <unordered_set>

using namespace psy;
using namespace C;

struct SemanticModel::SemanticModelImpl
{
    SemanticModelImpl(const SyntaxTree* tree, Compilation* compilation)
        : expectValidSyms_(true)
        , tree_(tree)
        , compilation_(compilation)
    {}

    bool expectValidSyms_;
    const SyntaxTree* tree_;
    Compilation* compilation_;
    std::unordered_map<const SyntaxNode*, Symbol*> declSyms_;
};

SemanticModel::SemanticModel(const SyntaxTree* tree, Compilation* compilation)
    : P(new SemanticModelImpl(tree, compilation))
{
    Binder binder(this, tree);
    binder.bind();
}

SemanticModel::~SemanticModel()
{}

const SyntaxTree* SemanticModel::syntaxTree() const
{
    return P->tree_;
}

const Compilation* SemanticModel::compilation() const
{
    return P->compilation_;
}

template <class SymCastT, class SymOriT>
const SymCastT* SemanticModel::castSym(const SymOriT* symOri,
                                       const SymCastT* (SymOriT::*cast)() const) const
{
    auto symCast = ((symOri)->*(cast))();
    if (!symCast) {
        PSY_ASSERT_NO_STMT(!P->expectValidSyms_);
        return nullptr;
    }

    return symCast;
}

const LibrarySymbol* SemanticModel::declaredSymbol(const TranslationUnitSyntax* node) const
{
    auto it = P->declSyms_.find(node);
    if (it == P->declSyms_.end()) {
        PSY_ASSERT_NO_STMT(!P->expectValidSyms_);
        return nullptr;
    }

    auto libSym = castSym(it->second, &Symbol::asLibrary);
    if (!libSym)
        return nullptr;

    return libSym;
}

const FunctionSymbol* SemanticModel::declaredSymbol(const FunctionDefinitionSyntax* node) const
{
    auto sym = declaredSymbol(node->declarator());
    if (!sym)
        return nullptr;

    auto funcSym = castSym(sym, &Symbol::asFunction);
    if (!funcSym)
        return nullptr;

    return funcSym;
}

const ParameterSymbol* SemanticModel::declaredSymbol(const ParameterDeclarationSyntax* node) const
{
    auto sym = declaredSymbol(node->declarator());
    if (!sym)
        return nullptr;

    auto valSym = castSym(sym, &Symbol::asValue);
    if (!valSym)
        return nullptr;

    auto parmSym = castSym(valSym, &ValueSymbol::asParameter);
    if (!parmSym)
        return nullptr;

    return parmSym;
}

const NamedTypeSymbol* SemanticModel::declaredSymbol(const TypeDeclarationSyntax* node) const
{
    auto it = P->declSyms_.find(node);
    if (it == P->declSyms_.end()) {
        PSY_ASSERT_NO_STMT(!P->expectValidSyms_);
        return nullptr;
    }

    auto tySym = castSym(it->second, &Symbol::asType);
    if (!tySym)
        return nullptr;

    auto namedTySym = castSym(tySym, &TypeSymbol::asNamedType);
    if (!namedTySym)
        return nullptr;

    return namedTySym;
}

const EnumeratorSymbol* SemanticModel::declaredSymbol(const EnumeratorDeclarationSyntax* node) const
{
    auto it = P->declSyms_.find(node);
    if (it == P->declSyms_.end()) {
        PSY_ASSERT_NO_STMT(!P->expectValidSyms_);
        return nullptr;
    }

    auto valSym = castSym(it->second, &Symbol::asValue);
    if (!valSym)
        return nullptr;

    auto enumtrSym = castSym(valSym, &ValueSymbol::asEnumerator);
    if (!enumtrSym)
        return nullptr;

    return enumtrSym;
}

std::vector<const FieldSymbol*> SemanticModel::declaredSymbols(const FieldDeclarationSyntax* node) const
{
    std::vector<const FieldSymbol*> fldSyms;
    for (auto decltorIt = node->declarators(); decltorIt; decltorIt = decltorIt->next) {
        auto sym = declaredSymbol(decltorIt->value);
        if (!sym)
            continue;

        auto valSym = castSym(sym, &Symbol::asValue);
        if (!valSym)
            continue;

        auto fldSym = castSym(valSym, &ValueSymbol::asField);
        if (!fldSym)
            continue;

        fldSyms.push_back(fldSym);
    }
    return fldSyms;
}

std::vector<const Symbol*> SemanticModel::declaredSymbols(
        const VariableAndOrFunctionDeclarationSyntax* node) const
{
    std::vector<const Symbol*> syms;
    for (auto decltorIt = node->declarators(); decltorIt; decltorIt = decltorIt->next) {
        auto sym = declaredSymbol(decltorIt->value);
        if (!sym) {
            PSY_ASSERT_NO_STMT(!P->expectValidSyms_);
            continue;
        }

        syms.push_back(sym);
    }
    return syms;
}

const Symbol* SemanticModel::declaredSymbol(const DeclaratorSyntax* node) const
{
    auto node_P = SyntaxUtilities::strippedDeclaratorOrSelf(node);
    auto node_PP = SyntaxUtilities::innermostDeclaratorOrSelf(node_P);

    auto it = P->declSyms_.find(node_PP);
    if (it == P->declSyms_.end()) {
        PSY_ASSERT_NO_STMT(!P->expectValidSyms_);
        return nullptr;
    }

    return it->second;
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
