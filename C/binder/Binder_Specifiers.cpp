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

template <class DeclT>
SyntaxVisitor::Action Binder::visitDeclaration_AtSpecifiers(
        const DeclT* node,
        Action (Binder::*visit_AtDeclarators)(const DeclT*))
{
    for (auto specIt = node->specifiers(); specIt; specIt = specIt->next)
        actOnTypeSpecifier(specIt->value);

    if (tySyms_.empty()) {
        Semantics_TypeSpecifiers::TypeSpecifierMissingDefaultsToInt(
                    node->lastToken(), &diagReporter_);

        makeTySymAndPushIt<NamedTypeSymbol>(BuiltinTypeKind::Int);
    }

    for (auto specIt = node->specifiers(); specIt; specIt = specIt->next)
        actOnTypeQualifier(specIt->value);

    return ((this)->*(visit_AtDeclarators))(node);
}

SyntaxVisitor::Action Binder::visitVariableAndOrFunctionDeclaration_AtSpecifiers(
        const VariableAndOrFunctionDeclarationSyntax* node)
{
    return visitDeclaration_AtSpecifiers(
                node,
                &Binder::visitVariableAndOrFunctionDeclaration_AtDeclarators);
}

SyntaxVisitor::Action Binder::visitFieldDeclaration_AtSpecifiers(const FieldDeclarationSyntax* node)
{
    return visitDeclaration_AtSpecifiers(
                node,
                &Binder::visitFieldDeclaration_AtDeclarators);
}

SyntaxVisitor::Action Binder::visitParameterDeclaration_AtSpecifiers(const ParameterDeclarationSyntax* node)
{
    return visitDeclaration_AtSpecifiers(
                node,
                &Binder::visitParameterDeclaration_AtDeclarators);
}

/* Specifiers */
SyntaxVisitor::Action Binder::actOnTypeSpecifier(const SpecifierSyntax* spec)
{
    if (spec->asTypeQualifier())
        return Action::Skip;

    visit(spec);

    return Action::Skip;
}

SyntaxVisitor::Action Binder::actOnTypeQualifier(const SpecifierSyntax* spec)
{
    if (!spec->asTypeQualifier())
        return Action::Skip;

    visit(spec);

    return Action::Skip;
}

SyntaxVisitor::Action Binder::visitBuiltinTypeSpecifier(const BuiltinTypeSpecifierSyntax* node)
{
    if (tySyms_.empty()) {
        BuiltinTypeKind builtTyK;
        switch (node->specifierToken().kind()) {
            case Keyword_void:
                builtTyK = BuiltinTypeKind::Void;
                break;
            case Keyword_char:
                builtTyK = BuiltinTypeKind::Char;
                break;
            case Keyword_short:
                builtTyK = BuiltinTypeKind::Short;
                break;
            case Keyword_int:
                builtTyK = BuiltinTypeKind::Int;
                break;
            case Keyword_long:
                builtTyK = BuiltinTypeKind::Long;
                break;
            case Keyword_float:
                builtTyK = BuiltinTypeKind::Float;
                break;
            case Keyword_double:
                builtTyK = BuiltinTypeKind::Double;
                break;
            case Keyword__Bool:
                builtTyK = BuiltinTypeKind::Bool;
                break;
            case Keyword__Complex:
                builtTyK = BuiltinTypeKind::DoubleComplex;
                break;
            case Keyword_signed:
                builtTyK = BuiltinTypeKind::Int_S;
                break;
            case Keyword_unsigned:
                builtTyK = BuiltinTypeKind::Int_U;
                break;
            default:
                PSYCHE_FAIL(return Action::Quit, "expected builtin type specifier");
                return Action::Quit;
        }

        makeTySymAndPushIt<NamedTypeSymbol>(builtTyK);
    }
    else {
        NamedTypeSymbol* namedTySym = tySyms_.top()->asNamedType();
        Semantics_TypeSpecifiers::specify(node->specifierToken(),
                                          namedTySym,
                                          &diagReporter_);
    }

    return Action::Skip;
}

SyntaxVisitor::Action Binder::visitTagTypeSpecifier(const TagTypeSpecifierSyntax* node)
{
    if (!node->declarations()) {
        TagSymbolName::NameSpace ns;
        switch (node->kind()) {
            case StructTypeSpecifier:
                ns = TagSymbolName::NameSpace::Structures;
                break;

            case UnionTypeSpecifier:
                ns = TagSymbolName::NameSpace::Unions;
                break;

            case EnumTypeSpecifier:
                ns = TagSymbolName::NameSpace::Enumerations;
                break;

            default:
                PSYCHE_FAIL_0(return Action::Quit);
                return Action::Quit;
        }

        makeTySymAndPushIt<NamedTypeSymbol>(ns, node->tagToken().valueText_c_str());
    }

    for (auto attrIt = node->attributes(); attrIt; attrIt = attrIt->next)
        visit(attrIt->value);

    for (auto declIt = node->declarations(); declIt; declIt = declIt->next) {
        TySymCont_T tySymUSES;
        std::swap(tySyms_, tySymUSES);

        visit(declIt->value);

        std::swap(tySyms_, tySymUSES);
    }

    for (auto attrIt = node->attributes_PostCloseBrace(); attrIt; attrIt = attrIt->next)
        visit(attrIt->value);

    return Action::Skip;
}

SyntaxVisitor::Action Binder::visitTypeDeclarationAsSpecifier(const TypeDeclarationAsSpecifierSyntax* node)
{
    visit(node->typeDeclaration());

    PSYCHE_ASSERT(node->typeDeclaration()->typeSpecifier()->asTagTypeSpecifier(),
                  return Action::Quit,
                  "expected tag type specifier");

    const TagTypeSpecifierSyntax* tySpec = node->typeDeclaration()->typeSpecifier()->asTagTypeSpecifier();
    TagSymbolName::NameSpace ns;
    switch (tySpec->kind()) {
        case StructTypeSpecifier:
            ns = TagSymbolName::NameSpace::Structures;
            break;

        case UnionTypeSpecifier:
            ns = TagSymbolName::NameSpace::Unions;
            break;

        case EnumTypeSpecifier:
            ns = TagSymbolName::NameSpace::Enumerations;
            break;

        default:
            PSYCHE_FAIL_0(return Action::Quit);
            return Action::Quit;
    }

    makeTySymAndPushIt<NamedTypeSymbol>(ns, tySpec->tagToken().valueText_c_str());

    return Action::Skip;
}

SyntaxVisitor::Action Binder::visitTypedefName(const TypedefNameSyntax* node)
{
    if (tySyms_.empty())
        makeTySymAndPushIt<NamedTypeSymbol>(node->identifierToken().valueText_c_str());

    return Action::Skip;
}

SyntaxVisitor::Action Binder::visitTypeQualifier(const TypeQualifierSyntax* node)
{
    PSYCHE_ASSERT_0(!tySyms_.empty(), return Action::Quit);

    Semantics_TypeQualifiers::qualify(node->qualifierKeyword(),
                                      tySyms_.top(),
                                      &diagReporter_);

    return Action::Skip;
}
