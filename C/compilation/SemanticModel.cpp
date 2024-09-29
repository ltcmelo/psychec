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
#include "binder/TypeResolver.h"
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
    {}

    bool bindingIsOK_;
    const SyntaxTree* tree_;
    Compilation* compilation_;
    std::unique_ptr<TranslationUnit> unit_;
    std::vector<std::unique_ptr<Declaration>> decls_;
    std::unordered_map<const Type*, std::unique_ptr<Type>> tys_;
    std::unordered_map<const SyntaxNode*, Declaration*> declsBySyntax_;
};

SemanticModel::SemanticModel(const SyntaxTree* tree, Compilation* compilation)
    : P(new SemanticModelImpl(tree, compilation))
{
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
    PSY_ASSERT_2(!P->unit_.get(), return nullptr);
    P->unit_ = std::move(unit);
    return P->unit_.get();
}

Declaration* SemanticModel::keepBinding(
        const SyntaxNode* node,
        std::unique_ptr<Declaration> decl)
{
    P->decls_.emplace_back(decl.release());
    Declaration* rawSym = P->decls_.back().get();
    PSY_ASSERT_2(P->declsBySyntax_.count(node) == 0, return nullptr);
    P->declsBySyntax_[node] = rawSym;
    return rawSym;
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

template <class CastT, class OrigT>
const CastT* SemanticModel::castDecl(const OrigT* origDecl,
                                     const CastT* (OrigT::*cast)() const) const
{
    auto decl = ((origDecl)->*(cast))();
    if (!decl) {
        PSY_ASSERT_1(!P->bindingIsOK_);
        return nullptr;
    }
    return decl;
}

Function* SemanticModel::declaredFunction(const FunctionDefinitionSyntax* node)
{
    auto decl = declaredDeclaration(node->declarator());
    if (!decl) {
        PSY_ASSERT_1(!P->bindingIsOK_);
        return nullptr;
    }
    PSY_ASSERT_2(decl->kind() == DeclarationKind::Function, return nullptr);
    auto funcDecl = decl->asFunction();
    return funcDecl;
}

const Function* SemanticModel::declaredFunction(const FunctionDefinitionSyntax* node) const
{
    return const_cast<SemanticModel*>(this)->declaredFunction(node);
}

Parameter* SemanticModel::declaredParameter(const ParameterDeclarationSyntax* node)
{
    auto decl = declaredDeclaration(node->declarator());
    if (!decl) {
        PSY_ASSERT_1(!P->bindingIsOK_);
        return nullptr;
    }
    PSY_ASSERT_2(decl->kind() == DeclarationKind::Object, return nullptr);
    auto objDecl = decl->asObjectDeclaration();
    PSY_ASSERT_2(objDecl->kind() == ObjectDeclarationKind::Parameter, return nullptr);
    auto parmDecl = objDecl->asParameter();
    return parmDecl;
}

const Parameter* SemanticModel::declaredParameter(const ParameterDeclarationSyntax* node) const
{
    return const_cast<SemanticModel*>(this)->declaredParameter(node);
}

const TypeDeclaration* SemanticModel::declaredTypeDeclaration(const TypeDeclarationSyntax* node) const
{
    auto it = P->declsBySyntax_.find(node);
    if (it == P->declsBySyntax_.end()) {
        PSY_ASSERT_1(!P->bindingIsOK_);
        return nullptr;
    }
    PSY_ASSERT_2(it->second->kind() == DeclarationKind::Type, return nullptr);
    auto tyDecl = it->second->asTypeDeclaration();
    return tyDecl;
}

const Struct* SemanticModel::declaredStruct(const StructOrUnionDeclarationSyntax* node) const
{
    auto tyDecl = declaredTypeDeclaration(node);
    PSY_ASSERT_2(tyDecl->kind() == TypeDeclarationKind::Tag, return nullptr);
    auto tagTyDecl = tyDecl->asTagTypeDeclaration();
    PSY_ASSERT_2(tagTyDecl->kind() == TagTypeDeclarationKind::Struct, return nullptr);
    return tagTyDecl->asStruct();
}

const Union* SemanticModel::declaredUnion(const StructOrUnionDeclarationSyntax* node) const
{
    auto tyDecl = declaredTypeDeclaration(node);
    PSY_ASSERT_2(tyDecl->kind() == TypeDeclarationKind::Tag, return nullptr);
    auto tagTyDecl = tyDecl->asTagTypeDeclaration();
    PSY_ASSERT_2(tagTyDecl->kind() == TagTypeDeclarationKind::Union, return nullptr);
    return tagTyDecl->asUnion();
}

const Enum* SemanticModel::declaredEnum(const EnumDeclarationSyntax* node) const
{
    auto tyDecl = declaredTypeDeclaration(node);
    PSY_ASSERT_2(tyDecl->kind() == TypeDeclarationKind::Tag, return nullptr);
    auto tagTyDecl = tyDecl->asTagTypeDeclaration();
    PSY_ASSERT_2(tagTyDecl->kind() == TagTypeDeclarationKind::Enum, return nullptr);
    return tagTyDecl->asEnum();
}

Enumerator* SemanticModel::declaredEnumerator(const EnumeratorDeclarationSyntax* node)
{
    auto it = P->declsBySyntax_.find(node);
    if (it == P->declsBySyntax_.end()) {
        PSY_ASSERT_1(!P->bindingIsOK_);
        return nullptr;
    }
    auto decl = it->second->asDeclaration();
    PSY_ASSERT_2(decl->kind() == DeclarationKind::Object, return nullptr);
    auto objDecl = decl->asObjectDeclaration();
    PSY_ASSERT_2(objDecl->kind() == ObjectDeclarationKind::Enumerator, return nullptr);
    auto enumeratorDecl = objDecl->asEnumerator();
    return enumeratorDecl;
}

const Enumerator* SemanticModel::declaredEnumerator(const EnumeratorDeclarationSyntax* node) const
{
    return const_cast<SemanticModel*>(this)->declaredEnumerator(node);
}

template <class VecT> VecT SemanticModel::declaredFields_CORE(
        const FieldDeclarationSyntax* node,
        VecT&& decls)
{
    for (auto decltorIt = node->declarators(); decltorIt; decltorIt = decltorIt->next) {
        auto decl = declaredDeclaration(decltorIt->value);
        if (!decl) {
            PSY_ASSERT_1(!P->bindingIsOK_);
            continue;
        }
        PSY_ASSERT_2(decl->kind() == DeclarationKind::Object, continue);
        auto objDecl = decl->asObjectDeclaration();
        PSY_ASSERT_2(objDecl->kind() == ObjectDeclarationKind::Field, continue);
        auto fldDecl = objDecl->asField();
        decls.push_back(fldDecl);
    }
    return std::move(decls);
}

std::vector<Field*> SemanticModel::declaredFields(const FieldDeclarationSyntax* node)
{
    std::vector<Field*> decls;
    return declaredFields_CORE(node, std::move(decls));
}

std::vector<const Field*> SemanticModel::declaredFields(const FieldDeclarationSyntax* node) const
{
    std::vector<const Field*> decls;
    return const_cast<SemanticModel*>(this)->declaredFields_CORE(node, std::move(decls));
}

template <class VecT> VecT SemanticModel::declaredDeclarations_CORE(
        const VariableAndOrFunctionDeclarationSyntax* node,
        VecT&& decls)
{
    for (auto decltorIt = node->declarators(); decltorIt; decltorIt = decltorIt->next) {
        auto decl = declaredDeclaration(decltorIt->value);
        if (!decl) {
            PSY_ASSERT_1(!P->bindingIsOK_);
            continue;
        }
        decls.push_back(decl);
    }
    return std::move(decls);
}

std::vector<Declaration*> SemanticModel::declaredDeclarations(
        const VariableAndOrFunctionDeclarationSyntax* node)
{
    std::vector<Declaration*> decls;
    return declaredDeclarations_CORE(node, std::move(decls));
}

std::vector<const Declaration*> SemanticModel::declaredDeclarations(
        const VariableAndOrFunctionDeclarationSyntax* node) const
{
    std::vector<const Declaration*> decls;
    return const_cast<SemanticModel*>(this)->declaredDeclarations_CORE(node, std::move(decls));
}

Declaration* SemanticModel::declaredDeclaration(const DeclaratorSyntax* node)
{
    auto node_P = SyntaxUtilities::strippedDeclaratorOrSelf(node);
    auto node_PP = SyntaxUtilities::innermostDeclaratorOrSelf(node_P);
    auto it = P->declsBySyntax_.find(node_PP);
    if (it == P->declsBySyntax_.end()) {
        PSY_ASSERT_1(!P->bindingIsOK_);
        return nullptr;
    }
    return it->second->asDeclaration();
}

const Declaration* SemanticModel::declaredDeclaration(const DeclaratorSyntax* node) const
{
    return const_cast<SemanticModel*>(this)->declaredDeclaration(node);
}

Declaration* SemanticModel::searchForDecl(
        std::function<bool (const std::unique_ptr<Declaration>&)> pred) const
{
    auto it = std::find_if(P->decls_.begin(), P->decls_.end(), pred);
    return it == P->decls_.end()
            ? nullptr
            : it->get();
}
