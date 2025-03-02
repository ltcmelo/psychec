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
#include "sema/DeclarationBinder.h"
#include "sema/TypeResolver.h"
#include "syntax/SyntaxNodes.h"
#include "syntax/SyntaxUtilities.h"
#include "symbols/Symbol_ALL.h"
#include "types/Type_ALL.h"
#include "../common/infra/Assertions.h"

#include <iostream>
#include <unordered_map>
#include <unordered_set>

using namespace psy;
using namespace C;

struct SemanticModel::SemanticModelImpl
{
    SemanticModelImpl(const SyntaxTree* tree, Compilation* compilation)
        : bindingIsOK_(false) // TODO
        , tree_(tree)
        , compilation_(compilation)
        , ptrdiff_t_Tydef_(nullptr)
        , size_t_Tydef_(nullptr)
        , max_align_t_Tydef_(nullptr)
        , wchar_t_Tydef_(nullptr)
        , char16_t_Tydef_(nullptr)
        , char32_t_Tydef_(nullptr)
    {}

    bool bindingIsOK_;
    const SyntaxTree* tree_;
    Compilation* compilation_;
    std::unique_ptr<TranslationUnitSymbol> unit_;
    std::vector<std::unique_ptr<DeclarationSymbol>> decls_;
    std::unordered_map<const Type*, std::unique_ptr<Type>> tys_;
    std::unordered_map<const SyntaxNode*, DeclarationSymbol*> declsBySyntax_; // bindings
    std::unordered_set<std::unique_ptr<Scope>> scopes_;
    std::unordered_map<const SyntaxNode*, const Scope*> nodeScope_;
    const TypedefDeclarationSymbol* ptrdiff_t_Tydef_;
    const TypedefDeclarationSymbol* size_t_Tydef_;
    const TypedefDeclarationSymbol* max_align_t_Tydef_;
    const TypedefDeclarationSymbol* wchar_t_Tydef_;
    const TypedefDeclarationSymbol* char16_t_Tydef_;
    const TypedefDeclarationSymbol* char32_t_Tydef_;
};

SemanticModel::SemanticModel(const SyntaxTree* tree, Compilation* compilation)
    : P(new SemanticModelImpl(tree, compilation))
{}

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

const TranslationUnitSymbol* SemanticModel::translationUnit() const
{
    return P->unit_.get();
}

TranslationUnitSymbol* SemanticModel::setTranslationUnit(std::unique_ptr<TranslationUnitSymbol> unit)
{
    PSY_ASSERT_2(!P->unit_.get(), return nullptr);
    P->unit_ = std::move(unit);
    return P->unit_.get();
}

DeclarationSymbol* SemanticModel::addDeclaration(
        const SyntaxNode* node,
        std::unique_ptr<DeclarationSymbol> decl)
{
    P->decls_.emplace_back(decl.release());
    DeclarationSymbol* addedDecl = P->decls_.back().get();
    PSY_ASSERT_2(P->declsBySyntax_.count(node) == 0, return nullptr);
    P->declsBySyntax_[node] = addedDecl;
    return addedDecl;
}

Type* SemanticModel::keepType(std::unique_ptr<Type> ty)
{
    auto p = P->tys_.insert(std::make_pair(ty.get(), std::move(ty)));
    PSY_ASSERT_2(p.second, return nullptr);
    return p.first->second.get();
}

void SemanticModel::dropType(const Type* ty)
{
    P->tys_.erase(ty);
}

Scope* SemanticModel::keepScope(std::unique_ptr<Scope> scope)
{
    auto r = P->scopes_.insert(std::move(scope));
    PSY_ASSERT_1(r.second);
    return r.first->get();
}

void SemanticModel::setScopeOf(const IdentifierNameSyntax* node, const Scope* scope)
{
    P->nodeScope_[node] = scope;
}

const Scope* SemanticModel::scopeOf(const IdentifierNameSyntax* node) const
{
    return P->nodeScope_[node];
}

FunctionDeclarationSymbol* SemanticModel::functionFor(const FunctionDefinitionSyntax* node)
{
    auto decl = declarationBy(node->declarator());
    if (!decl) {
        PSY_ASSERT_1(!P->bindingIsOK_);
        return nullptr;
    }
    PSY_ASSERT_2(decl->kind() == SymbolKind::FunctionDeclaration, return nullptr);
    auto funcDecl = decl->asFunctionDeclaration();
    return funcDecl;
}

const FunctionDeclarationSymbol* SemanticModel::functionFor(const FunctionDefinitionSyntax* node) const
{
    return const_cast<SemanticModel*>(this)->functionFor(node);
}

ParameterDeclarationSymbol* SemanticModel::parameterFor(const ParameterDeclarationSyntax* node)
{
    auto decl = declarationBy(node->declarator());
    if (!decl) {
        PSY_ASSERT_1(!P->bindingIsOK_);
        return nullptr;
    }
    PSY_ASSERT_2(decl->kind() == SymbolKind::ParameterDeclaration, return nullptr);
    auto parmDecl = decl->asParameterDeclaration();
    return parmDecl;
}

const ParameterDeclarationSymbol* SemanticModel::parameterFor(const ParameterDeclarationSyntax* node) const
{
    return const_cast<SemanticModel*>(this)->parameterFor(node);
}

const TypeDeclarationSymbol* SemanticModel::typeDeclarationFor(const TypeDeclarationSyntax* node) const
{
    auto it = P->declsBySyntax_.find(node);
    if (it == P->declsBySyntax_.end()) {
        PSY_ASSERT_1(!P->bindingIsOK_);
        return nullptr;
    }
    PSY_ASSERT_2(it->second->category() == DeclarationCategory::Type, return nullptr);
    auto tyDecl = it->second->asTypeDeclaration();
    return tyDecl;
}

const StructDeclarationSymbol* SemanticModel::structFor(const StructOrUnionDeclarationSyntax* node) const
{
    auto tyDecl = typeDeclarationFor(node);
    PSY_ASSERT_2(tyDecl->category() == TypeDeclarationCategory::Tag, return nullptr);
    auto tagTyDecl = tyDecl->asTagTypeDeclaration();
    PSY_ASSERT_2(tagTyDecl->category() == TagTypeDeclarationCategory::Struct, return nullptr);
    return tagTyDecl->asStructDeclaration();
}

const UnionDeclarationSymbol* SemanticModel::unionFor(const StructOrUnionDeclarationSyntax* node) const
{
    auto tyDecl = typeDeclarationFor(node);
    PSY_ASSERT_2(tyDecl->category() == TypeDeclarationCategory::Tag, return nullptr);
    auto tagTyDecl = tyDecl->asTagTypeDeclaration();
    PSY_ASSERT_2(tagTyDecl->category() == TagTypeDeclarationCategory::Union, return nullptr);
    return tagTyDecl->asUnionDeclaration();
}

const EnumDeclarationSymbol* SemanticModel::enumFor(const EnumDeclarationSyntax* node) const
{
    auto tyDecl = typeDeclarationFor(node);
    PSY_ASSERT_2(tyDecl->category() == TypeDeclarationCategory::Tag, return nullptr);
    auto tagTyDecl = tyDecl->asTagTypeDeclaration();
    PSY_ASSERT_2(tagTyDecl->category() == TagTypeDeclarationCategory::Enum, return nullptr);
    return tagTyDecl->asEnumDeclaration();
}

EnumeratorDeclarationSymbol* SemanticModel::enumeratorFor(const EnumeratorDeclarationSyntax* node)
{
    auto it = P->declsBySyntax_.find(node);
    if (it == P->declsBySyntax_.end()) {
        PSY_ASSERT_1(!P->bindingIsOK_);
        return nullptr;
    }
    auto decl = it->second->asDeclaration();
    PSY_ASSERT_2(decl->kind() == SymbolKind::EnumeratorDeclaration, return nullptr);
    return decl->asEnumeratorDeclaration();
}

const EnumeratorDeclarationSymbol* SemanticModel::enumeratorFor(const EnumeratorDeclarationSyntax* node) const
{
    return const_cast<SemanticModel*>(this)->enumeratorFor(node);
}

template <class VecT> VecT SemanticModel::fieldsFor_CORE(
        const FieldDeclarationSyntax* node,
        VecT&& decls)
{
    for (auto decltorIt = node->declarators(); decltorIt; decltorIt = decltorIt->next) {
        auto decl = declarationBy(decltorIt->value);
        if (!decl) {
            PSY_ASSERT_1(!P->bindingIsOK_);
            continue;
        }
        PSY_ASSERT_2(decl->kind() == SymbolKind::FieldDeclaration, continue);
        auto fldDecl = decl->asFieldDeclaration();
        decls.push_back(fldDecl);
    }
    return std::move(decls);
}

std::vector<FieldDeclarationSymbol*> SemanticModel::fieldsFor(const FieldDeclarationSyntax* node)
{
    std::vector<FieldDeclarationSymbol*> decls;
    return fieldsFor_CORE(node, std::move(decls));
}

std::vector<const FieldDeclarationSymbol*> SemanticModel::fieldsFor(const FieldDeclarationSyntax* node) const
{
    std::vector<const FieldDeclarationSymbol*> decls;
    return const_cast<SemanticModel*>(this)->fieldsFor_CORE(node, std::move(decls));
}

template <class VecT> VecT SemanticModel::variablesAndOrFunctionsFor_CORE(
        const VariableAndOrFunctionDeclarationSyntax* node,
        VecT&& decls)
{
    for (auto decltorIt = node->declarators(); decltorIt; decltorIt = decltorIt->next) {
        auto decl = declarationBy(decltorIt->value);
        if (!decl) {
            PSY_ASSERT_1(!P->bindingIsOK_);
            continue;
        }
        decls.push_back(decl);
    }
    return std::move(decls);
}

std::vector<DeclarationSymbol*> SemanticModel::variableAndOrFunctionsFor(
        const VariableAndOrFunctionDeclarationSyntax* node)
{
    std::vector<DeclarationSymbol*> decls;
    return variablesAndOrFunctionsFor_CORE(node, std::move(decls));
}

std::vector<const DeclarationSymbol*> SemanticModel::variablesAndOrFunctionsFor(
        const VariableAndOrFunctionDeclarationSyntax* node) const
{
    std::vector<const DeclarationSymbol*> decls;
    return const_cast<SemanticModel*>(this)
            ->variablesAndOrFunctionsFor_CORE(node, std::move(decls));
}

DeclarationSymbol* SemanticModel::declarationBy(const DeclaratorSyntax* node)
{
    node = SyntaxUtilities::innermostDeclaratorOf(node);
    auto it = P->declsBySyntax_.find(node);
    if (it == P->declsBySyntax_.end()) {
        PSY_ASSERT_1(!P->bindingIsOK_);
        return nullptr;
    }
    return it->second->asDeclaration();
}

const DeclarationSymbol* SemanticModel::declarationBy(const DeclaratorSyntax* node) const
{
    return const_cast<SemanticModel*>(this)->declarationBy(node);
}

const DeclarationSymbol* SemanticModel::searchForDeclaration(
        std::function<bool (const DeclarationSymbol*)> pred) const
{
    auto it = std::find_if(
                P->decls_.begin(),
                P->decls_.end(),
                [pred](const std::unique_ptr<DeclarationSymbol>& d) { return pred(d.get()); });
    return it == P->decls_.end()
            ? nullptr
            : it->get();
}

const TypedefDeclarationSymbol* SemanticModel::ptrdiff_t_typedef() const
{
    return P->ptrdiff_t_Tydef_;
}

const TypedefDeclarationSymbol* SemanticModel::size_t_typedef() const
{
    return P->size_t_Tydef_;
}

const TypedefDeclarationSymbol* SemanticModel::max_align_t_typedef() const
{
    return P->max_align_t_Tydef_;
}

const TypedefDeclarationSymbol* SemanticModel::wchar_t_typedef() const
{
    return P->wchar_t_Tydef_;
}

const TypedefDeclarationSymbol* SemanticModel::char16_t_typedef() const
{
    return P->char16_t_Tydef_;
}

const TypedefDeclarationSymbol* SemanticModel::char32_t_typedef() const
{
    return P->char32_t_Tydef_;
}

void SemanticModel::set_ptrdiff_t_typedef(const TypedefDeclarationSymbol* decl)
{
    P->ptrdiff_t_Tydef_ = decl;
}

void SemanticModel::set_size_t_typedef(const TypedefDeclarationSymbol* decl)
{
    P->size_t_Tydef_ = decl;
}

void SemanticModel::set_max_align_t_typedef(const TypedefDeclarationSymbol* decl)
{
    P->max_align_t_Tydef_ = decl;
}

void SemanticModel::set_wchar_t_typedef(const TypedefDeclarationSymbol* decl)
{
    P->wchar_t_Tydef_ = decl;
}

void SemanticModel::set_char16_t_typedef(const TypedefDeclarationSymbol* decl)
{
    P->char16_t_Tydef_ = decl;
}

void SemanticModel::set_char32_t_typedef(const TypedefDeclarationSymbol* decl)
{
    P->char32_t_Tydef_ = decl;
}
