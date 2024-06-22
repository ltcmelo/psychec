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
#include "binder/DeclarationResolver.h"
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
    std::unique_ptr<TranslationUnit> unit_;
    std::vector<std::unique_ptr<DeclarationSymbol>> decls_;
    std::vector<std::unique_ptr<Type>> tys_;
    std::unordered_map<const SyntaxNode*, DeclarationSymbol*> declsBySyntax_;
};

SemanticModel::SemanticModel(const SyntaxTree* tree, Compilation* compilation)
    : P(new SemanticModelImpl(tree, compilation))
{
    Binder binder(this, tree);
    binder.bind();
    DeclarationResolver declResolver(this, tree);
    declResolver.resolveDeclarations();
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

const TranslationUnit *SemanticModel::translationUnit() const
{
    return P->unit_.get();
}

TranslationUnit* SemanticModel::keepTranslationUnit(
        const TranslationUnitSyntax* node,
        std::unique_ptr<TranslationUnit> unit)
{
    PSY_ASSERT(!P->unit_.get(), return nullptr);
    P->unit_ = std::move(unit);
    return P->unit_.get();
}

DeclarationSymbol* SemanticModel::keepAndBindDecl(
        const SyntaxNode* node,
        std::unique_ptr<DeclarationSymbol> decl)
{
    P->decls_.emplace_back(decl.release());
    DeclarationSymbol* rawSym = P->decls_.back().get();
    PSY_ASSERT(P->declsBySyntax_.count(node) == 0, return nullptr);
    P->declsBySyntax_[node] = rawSym;
    return rawSym;
}

Type* SemanticModel::keepType(std::unique_ptr<Type> ty)
{
    P->tys_.emplace_back(ty.release());
    return P->tys_.back().get();
}

template <class CastT, class OrigT>
const CastT* SemanticModel::castDecl(const OrigT* origDecl,
                                     const CastT* (OrigT::*cast)() const) const
{
    auto decl = ((origDecl)->*(cast))();
    if (!decl) {
        PSY_ASSERT_NO_STMT(!P->expectValidSyms_);
        return nullptr;
    }
    return decl;
}

const Function* SemanticModel::declarationOf(const FunctionDefinitionSyntax* node) const
{
    auto decl = declarationOf(node->declarator());
    if (!decl)
        return nullptr;
    auto funcSym = castDecl(decl, &DeclarationSymbol::asFunction);
    return funcSym;
}

const Parameter* SemanticModel::declarationOf(const ParameterDeclarationSyntax* node) const
{
    auto decl = declarationOf(node->declarator());
    if (!decl)
        return nullptr;
    auto objDecl = castDecl(decl, &DeclarationSymbol::asObjectDeclarationSymbol);
    if (!objDecl)
        return nullptr;
    auto parmDecl = castDecl(objDecl, &ObjectDeclarationSymbol::asParameter);
    return parmDecl;
}

const TypeDeclarationSymbol* SemanticModel::declarationOf(const TypeDeclarationSyntax* node) const
{
    auto it = P->declsBySyntax_.find(node);
    if (it == P->declsBySyntax_.end()) {
        PSY_ASSERT_NO_STMT(!P->expectValidSyms_);
        return nullptr;
    }
    auto tyDecl = castDecl(
                it->second->asDeclarationSymbol(),
                &DeclarationSymbol::asTypeDeclarationSymbol);
    return tyDecl;
}

const Enumerator* SemanticModel::declarationOf(const EnumeratorDeclarationSyntax* node) const
{
    auto it = P->declsBySyntax_.find(node);
    if (it == P->declsBySyntax_.end()) {
        PSY_ASSERT_NO_STMT(!P->expectValidSyms_);
        return nullptr;
    }
    auto objDecl = castDecl(it->second->asDeclarationSymbol(), &DeclarationSymbol::asObjectDeclarationSymbol);
    if (!objDecl)
        return nullptr;
    auto enumeratorDecl = castDecl(objDecl, &ObjectDeclarationSymbol::asEnumerator);
    if (!enumeratorDecl)
        return nullptr;
    return enumeratorDecl;
}

std::vector<const Field*> SemanticModel::declarationsOf(const FieldDeclarationSyntax* node) const
{
    std::vector<const Field*> fldDecls;
    for (auto decltorIt = node->declarators(); decltorIt; decltorIt = decltorIt->next) {
        auto decl = declarationOf(decltorIt->value);
        if (!decl)
            continue;
        auto objDecl = castDecl(decl, &DeclarationSymbol::asObjectDeclarationSymbol);
        if (!objDecl)
            continue;
        auto fldDecl = castDecl(objDecl, &ObjectDeclarationSymbol::asField);
        if (!fldDecl)
            continue;
        fldDecls.push_back(fldDecl);
    }
    return fldDecls;
}

std::vector<const DeclarationSymbol*> SemanticModel::declarationsOf(
        const VariableAndOrFunctionDeclarationSyntax* node) const
{
    std::vector<const DeclarationSymbol*> decls;
    for (auto decltorIt = node->declarators(); decltorIt; decltorIt = decltorIt->next) {
        auto decl = declarationOf(decltorIt->value);
        if (!decl) {
            PSY_ASSERT_NO_STMT(!P->expectValidSyms_);
            continue;
        }
        decls.push_back(decl);
    }
    return decls;
}

const DeclarationSymbol* SemanticModel::declarationOf(const DeclaratorSyntax* node) const
{
    auto node_P = SyntaxUtilities::strippedDeclaratorOrSelf(node);
    auto node_PP = SyntaxUtilities::innermostDeclaratorOrSelf(node_P);
    auto it = P->declsBySyntax_.find(node_PP);
    if (it == P->declsBySyntax_.end()) {
        PSY_ASSERT_NO_STMT(!P->expectValidSyms_);
        return nullptr;
    }
    return it->second->asDeclarationSymbol();
}

DeclarationSymbol* SemanticModel::searchForDecl(
        std::function<bool (const std::unique_ptr<DeclarationSymbol>&)> pred) const
{
    auto it = std::find_if(P->decls_.begin(), P->decls_.end(), pred);
    return it == P->decls_.end()
            ? nullptr
            : it->get();
}
