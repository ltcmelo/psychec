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

#include "Binder.h"

#include "SyntaxTree.h"

#include "binder/Scopes.h"
#include "binder/Semantics_TypeQualifiers.h"
#include "binder/Semantics_TypeSpecifiers.h"
#include "compilation/SemanticModel.h"
#include "symbols/Symbols.h"
#include "symbols/SymbolNames.h"
#include "syntax/SyntaxFacts.h"
#include "syntax/SyntaxNodes.h"
#include "syntax/SyntaxUtilities.h"

#include "../common/infra/PsycheAssert.h"

#include <iostream>

using namespace psy;
using namespace C;

SyntaxVisitor::Action Binder::visitVariableAndOrFunctionDeclaration_AtSpecifiers(
        const VariableAndOrFunctionDeclarationSyntax* node)
{
    std::vector<SpecifierSyntax*> tyQualSpecs;
    for (auto specIt = node->specifiers(); specIt; specIt = specIt->next) {
        auto spec = specIt->value;

        if (spec->asTypeQualifier()) {
            tyQualSpecs.push_back(spec);
            continue;
        }

        visit(spec);
    }

    if (tySymUSEs_.empty()) {
        Semantics_TypeSpecifiers::TypeSpecifierMissingDefaultsToInt(
                    node->lastToken(), &diagReporter_);

        std::unique_ptr<NamedTypeSymbol> namedTySym(
                new NamedTypeSymbol(tree_,
                                    scopes_.top(),
                                    symDEFs_.top(),
                                    TypeKind::Builtin));
        pushTySymUSE(std::move(namedTySym));
    }

    for (auto spec : tyQualSpecs)
        visit(spec);

    return visitVariableAndOrFunctionDeclaration_AtDeclarators(node);
}

/* Specifiers */
SyntaxVisitor::Action Binder::visitBuiltinTypeSpecifier(const BuiltinTypeSpecifierSyntax* node)
{
    if (tySymUSEs_.empty()) {
        std::unique_ptr<NamedTypeSymbol> namedTySym(
                new NamedTypeSymbol(tree_,
                                    scopes_.top(),
                                    symDEFs_.top(),
                                    TypeKind::Builtin));
        pushTySymUSE(std::move(namedTySym));
    }

    NamedTypeSymbol* namedTySym = tySymUSEs_.top()->asNamedType();
    if (!namedTySym) {
        //error
        return Action::Skip;
    }

    Semantics_TypeSpecifiers::specify(node->specifierToken(),
                                      namedTySym,
                                      &diagReporter_);

    std::unique_ptr<SymbolName> name(
                new PlainSymbolName(node->specifierToken().valueText_c_str()));
    namedTySym->setName(std::move(name));

    return Action::Skip;
}

SyntaxVisitor::Action Binder::visitTagTypeSpecifier(const TagTypeSpecifierSyntax* node)
{
    TypeKind tyKind;
    switch (node->keyword().kind()) {
        case Keyword_struct:
            tyKind = TypeKind::Struct;
            break;

        case Keyword_union:
            tyKind = TypeKind::Union;
            break;

        case Keyword_enum:
            tyKind = TypeKind::Enum;
            break;

        default:
            PSYCHE_FAIL(return Action::Quit, "unknown keyword");
            return Action::Quit;
    }

    makeAndPushTySymDEF(tyKind);
    std::unique_ptr<SymbolName> name(
                new TagSymbolName(tyKind,
                                  node->tagToken().valueText_c_str()));
    symDEFs_.top()->setName(std::move(name));

    for (auto attrIt = node->attributes(); attrIt; attrIt = attrIt->next)
        visit(attrIt->value);

    for (auto declIt = node->declarations(); declIt; declIt = declIt->next)
        visit(declIt->value);

    for (auto attrIt = node->attributes_PostCloseBrace(); attrIt; attrIt = attrIt->next)
        visit(attrIt->value);

    return Action::Skip;
}

SyntaxVisitor::Action Binder::visitTypeDeclarationAsSpecifier(const TypeDeclarationAsSpecifierSyntax* node)
{
    return Action::Skip;
}

SyntaxVisitor::Action Binder::visitTypedefName(const TypedefNameSyntax* node)
{
    if (tySymUSEs_.empty()) {
        std::unique_ptr<NamedTypeSymbol> namedTySym(
                new NamedTypeSymbol(tree_,
                                    scopes_.top(),
                                    symDEFs_.top(),
                                    TypeKind::Synonym));
        pushTySymUSE(std::move(namedTySym));
    }

    NamedTypeSymbol* namedTySym = tySymUSEs_.top()->asNamedType();
    if (!namedTySym) {
        //error
        return Action::Skip;
    }

    std::unique_ptr<SymbolName> name(
                new PlainSymbolName(node->identifierToken().valueText_c_str()));
    namedTySym->setName(std::move(name));

    return Action::Skip;
}

SyntaxVisitor::Action Binder::visitTypeQualifier(const TypeQualifierSyntax* node)
{
    PSYCHE_ASSERT_0(!tySymUSEs_.empty(), return Action::Quit);

    Semantics_TypeQualifiers::qualify(node->qualifierKeyword(),
                                      tySymUSEs_.top(),
                                      &diagReporter_);

    return Action::Skip;
}
