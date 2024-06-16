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

#include "Compilation.h"

#include "binder/Binder.h"
#include "syntax/SyntaxNodes.h"
#include "syntax/SyntaxUtilities.h"
#include "symbols/Symbol_ALL.h"
#include "types/Type_ALL.h"

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
    std::unique_ptr<TranslationUnit> unitSym;
    std::vector<std::unique_ptr<DeclarationSymbol>> declSyms_;
    std::vector<std::unique_ptr<Type>> tys_;
    std::unordered_map<const SyntaxNode*, DeclarationSymbol*> declSymByNode_;
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

const TranslationUnit *SemanticModel::translationUnitSymbol() const
{
    return P->unitSym.get();
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

TranslationUnit* SemanticModel::keepUnitSym(
        const TranslationUnitSyntax* node,
        std::unique_ptr<TranslationUnit> sym)
{
    PSY_ASSERT(!P->unitSym.get(), return nullptr);
    P->unitSym = std::move(sym);
    return P->unitSym.get();
}

DeclarationSymbol* SemanticModel::keepAndBindDeclSym(
        const SyntaxNode* node,
        std::unique_ptr<DeclarationSymbol> sym)
{
    P->declSyms_.emplace_back(sym.release());
    DeclarationSymbol* rawSym = P->declSyms_.back().get();
    PSY_ASSERT(P->declSymByNode_.count(node) == 0, return nullptr);
    P->declSymByNode_[node] = rawSym;
    return rawSym;
}

Type* SemanticModel::keepType(std::unique_ptr<Type> ty)
{
    P->tys_.emplace_back(ty.release());
    return P->tys_.back().get();
}

const Function* SemanticModel::declaredSymbol(const FunctionDefinitionSyntax* node) const
{
    auto declSym = declaredSymbol(node->declarator());
    if (!declSym)
        return nullptr;

    auto funcSym = castSym(declSym, &DeclarationSymbol::asFunction);
    if (!funcSym)
        return nullptr;

    return funcSym;
}

const Parameter* SemanticModel::declaredSymbol(const ParameterDeclarationSyntax* node) const
{
    auto declSym = declaredSymbol(node->declarator());
    if (!declSym)
        return nullptr;

    auto valSym = castSym(declSym, &DeclarationSymbol::asObjectDeclarationSymbol);
    if (!valSym)
        return nullptr;

    auto parmSym = castSym(valSym, &ObjectDeclarationSymbol::asParameter);
    if (!parmSym)
        return nullptr;

    return parmSym;
}

const TypeDeclarationSymbol* SemanticModel::declaredSymbol(const TypeDeclarationSyntax* node) const
{
    auto it = P->declSymByNode_.find(node);
    if (it == P->declSymByNode_.end()) {
        PSY_ASSERT_NO_STMT(!P->expectValidSyms_);
        return nullptr;
    }

    auto tySym = castSym(it->second->asDeclarationSymbol(), &DeclarationSymbol::asTypeDeclarationSymbol);
    if (!tySym)
        return nullptr;

    return tySym;
}

const Enumerator* SemanticModel::declaredSymbol(const EnumeratorDeclarationSyntax* node) const
{
    auto it = P->declSymByNode_.find(node);
    if (it == P->declSymByNode_.end()) {
        PSY_ASSERT_NO_STMT(!P->expectValidSyms_);
        return nullptr;
    }

    auto valSym = castSym(it->second->asDeclarationSymbol(), &DeclarationSymbol::asObjectDeclarationSymbol);
    if (!valSym)
        return nullptr;

    auto enumtrSym = castSym(valSym, &ObjectDeclarationSymbol::asEnumerator);
    if (!enumtrSym)
        return nullptr;

    return enumtrSym;
}

std::vector<const Field*> SemanticModel::declaredSymbols(const FieldDeclarationSyntax* node) const
{
    std::vector<const Field*> fldSyms;
    for (auto decltorIt = node->declarators(); decltorIt; decltorIt = decltorIt->next) {
        auto declSym = declaredSymbol(decltorIt->value);
        if (!declSym)
            continue;

        auto valSym = castSym(declSym, &DeclarationSymbol::asObjectDeclarationSymbol);
        if (!valSym)
            continue;

        auto fldSym = castSym(valSym, &ObjectDeclarationSymbol::asField);
        if (!fldSym)
            continue;

        fldSyms.push_back(fldSym);
    }
    return fldSyms;
}

std::vector<const DeclarationSymbol*> SemanticModel::declaredSymbols(
        const VariableAndOrFunctionDeclarationSyntax* node) const
{
    std::vector<const DeclarationSymbol*> syms;
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

const DeclarationSymbol* SemanticModel::declaredSymbol(const DeclaratorSyntax* node) const
{
    auto node_P = SyntaxUtilities::strippedDeclaratorOrSelf(node);
    auto node_PP = SyntaxUtilities::innermostDeclaratorOrSelf(node_P);
    auto it = P->declSymByNode_.find(node_PP);
    if (it == P->declSymByNode_.end()) {
        PSY_ASSERT_NO_STMT(!P->expectValidSyms_);
        return nullptr;
    }
    return it->second->asDeclarationSymbol();
}

DeclarationSymbol* SemanticModel::searchForDeclSym(
        std::function<bool (const std::unique_ptr<DeclarationSymbol>&)> pred) const
{
    auto it = std::find_if(P->declSyms_.begin(), P->declSyms_.end(), pred);
    return it == P->declSyms_.end()
            ? nullptr
            : it->get();
}
