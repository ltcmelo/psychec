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

#include "Binder.h"

#include "SyntaxTree.h"

#include "binder/Scope.h"
#include "compilation/SemanticModel.h"
#include "symbols/Symbol_ALL.h"
#include "symbols/SymbolName_ALL.h"
#include "syntax/SyntaxFacts.h"
#include "syntax/SyntaxNodes.h"
#include "syntax/SyntaxUtilities.h"

#include "../common/infra/Assertions.h"

#include <iostream>

using namespace psy;
using namespace C;

template <class TyDeclT>
SyntaxVisitor::Action Binder::visitTypeDeclaration_AtInternalDeclarations_COMMON(
        const TyDeclT* node,
        Action (Binder::*visit_DONE)(const TyDeclT*))
{
    for (auto declIt = node->typeSpecifier()->declarations(); declIt; declIt = declIt->next)
        visit(declIt->value);

    popSym();

    return ((this)->*(visit_DONE))(node);
}

SyntaxVisitor::Action Binder::visitStructOrUnionDeclaration_AtSpecifier(
        const StructOrUnionDeclarationSyntax* node)
{
    const TagTypeSpecifierSyntax* tySpec = node->typeSpecifier();
    TagSymbolName::TagChoice tagChoice;
    switch (tySpec->kind()) {
        case StructTypeSpecifier:
            tagChoice = TagSymbolName::TagChoice::Struct;
            break;

        case UnionTypeSpecifier:
            tagChoice = TagSymbolName::TagChoice::Union;
            break;

        default:
            PSY_ESCAPE_VIA_RETURN(Action::Quit);
    }

    makeSymAndPushIt<NamedTypeSymbol>(node,
                                      tagChoice,
                                      tySpec->tagToken().valueText_c_str());

    return visitTypeDeclaration_AtInternalDeclarations_COMMON(
                node,
                &Binder::visitStructOrUnionDeclaration_DONE);
}

SyntaxVisitor::Action Binder::visitEnumDeclaration_AtSpecifier(const EnumDeclarationSyntax* node)
{
    makeSymAndPushIt<NamedTypeSymbol>(node,
                                      TagSymbolName::TagChoice::Enum,
                                      node->typeSpecifier()->tagToken().valueText_c_str());

    return visitTypeDeclaration_AtInternalDeclarations_COMMON(
                node,
                &Binder::visitEnumDeclaration_DONE);
}

SyntaxVisitor::Action Binder::visitTypedefDeclaration_AtSpecifier(const TypedefDeclarationSyntax* node)
{
    return visitDeclaration_AtSpecifiers_COMMON(
                node,
                &Binder::visitTypedefDeclaration_AtDeclarators);
}

template <class DeclT>
SyntaxVisitor::Action Binder::visitDeclaration_AtSpecifiers_COMMON(
        const DeclT* node,
        Action (Binder::*visit_AtDeclarators)(const DeclT*))
{
    for (auto specIt = node->specifiers(); specIt; specIt = specIt->next)
        visitIfNotTypeQualifier(specIt->value);

    if (tySyms_.empty()) {
        diagReporter_.TypeSpecifierMissingDefaultsToInt(node->lastToken());
        makeTySymAndPushIt<NamedTypeSymbol>(BuiltinTypeKind::Int);
    }

    for (auto specIt = node->specifiers(); specIt; specIt = specIt->next)
        visitIfTypeQualifier(specIt->value);

    return ((this)->*(visit_AtDeclarators))(node);
}

SyntaxVisitor::Action Binder::visitVariableAndOrFunctionDeclaration_AtSpecifiers(
        const VariableAndOrFunctionDeclarationSyntax* node)
{
    return visitDeclaration_AtSpecifiers_COMMON(
                node,
                &Binder::visitVariableAndOrFunctionDeclaration_AtDeclarators);
}

SyntaxVisitor::Action Binder::visitFunctionDefinition_AtSpecifiers(const FunctionDefinitionSyntax* node)
{
    return visitDeclaration_AtSpecifiers_COMMON(
                node,
                &Binder::visitFunctionDefinition_AtDeclarator);
}

SyntaxVisitor::Action Binder::visitFieldDeclaration_AtSpecifiers(const FieldDeclarationSyntax* node)
{
    return visitDeclaration_AtSpecifiers_COMMON(
                node,
                &Binder::visitFieldDeclaration_AtDeclarators);
}

SyntaxVisitor::Action Binder::visitEnumeratorDeclaration_AtImplicitSpecifier(const EnumeratorDeclarationSyntax* node)
{
    makeTySymAndPushIt<NamedTypeSymbol>(BuiltinTypeKind::Int);

    return visitEnumeratorDeclaration_AtDeclarator(node);
}

SyntaxVisitor::Action Binder::visitParameterDeclaration_AtSpecifiers(const ParameterDeclarationSyntax* node)
{
    return visitDeclaration_AtSpecifiers_COMMON(
                node,
                &Binder::visitParameterDeclaration_AtDeclarator);
}

SyntaxVisitor::Action Binder::visitIfNotTypeQualifier(const SpecifierSyntax* spec)
{
    if (spec->asTypeQualifier())
        return Action::Skip;

    visit(spec);

    return Action::Skip;
}

SyntaxVisitor::Action Binder::visitIfTypeQualifier(const SpecifierSyntax* spec)
{
    if (!spec->asTypeQualifier())
        return Action::Skip;

    visit(spec);

    return Action::Skip;
}

SyntaxVisitor::Action Binder::visitBuiltinTypeSpecifier(const BuiltinTypeSpecifierSyntax* node)
{
    auto tySpecTk = node->specifierToken();

    if (tySyms_.empty()) {
        BuiltinTypeKind builtTyK;
        switch (tySpecTk.kind()) {
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
                PSY_ESCAPE_VIA_RETURN(Action::Quit);
        }

        makeTySymAndPushIt<NamedTypeSymbol>(builtTyK);
        return Action::Skip;
    }

    NamedTypeSymbol* namedTySym = tySyms_.top()->asNamedType();
    auto curBuiltTyK = namedTySym->builtinTypeKind();
    BuiltinTypeKind extraBuiltTyK;
    switch (curBuiltTyK) {
        case BuiltinTypeKind::UNSPECIFIED:
            switch (tySpecTk.kind()) {
                case Keyword_void:
                    extraBuiltTyK = BuiltinTypeKind::Void;
                    break;
                case Keyword_char:
                    extraBuiltTyK = BuiltinTypeKind::Char;
                    break;
                case Keyword_short:
                    extraBuiltTyK = BuiltinTypeKind::Short;
                    break;
                case Keyword_int:
                    extraBuiltTyK = BuiltinTypeKind::Int;
                    break;
                case Keyword_long:
                    extraBuiltTyK = BuiltinTypeKind::Long;
                    break;
                case Keyword_float:
                    extraBuiltTyK = BuiltinTypeKind::Float;
                    break;
                case Keyword_double:
                    extraBuiltTyK = BuiltinTypeKind::Double;
                    break;
                case Keyword__Bool:
                    extraBuiltTyK = BuiltinTypeKind::Bool;
                    break;
                case Keyword__Complex:
                    extraBuiltTyK = BuiltinTypeKind::DoubleComplex;
                    break;
                case Keyword_signed:
                    extraBuiltTyK = BuiltinTypeKind::Int_S;
                    break;
                case Keyword_unsigned:
                    extraBuiltTyK = BuiltinTypeKind::Int_U;
                    break;
                default:
                    PSY_ESCAPE_VIA_RETURN(Action::Skip);
            }
            break;

        case BuiltinTypeKind::Void:
            // report
            return Action::Skip;

        case BuiltinTypeKind::Char:
            switch (tySpecTk.kind()) {
                case Keyword_signed:
                    extraBuiltTyK = BuiltinTypeKind::Char_S;
                    break;
                case Keyword_unsigned:
                    extraBuiltTyK = BuiltinTypeKind::Char_U;
                    break;
                default:
                    // report
                    return Action::Skip;
            }
            break;

        case BuiltinTypeKind::Char_S:
        case BuiltinTypeKind::Char_U:
            // report
            return Action::Skip;

        case BuiltinTypeKind::Short:
            switch (tySpecTk.kind()) {
                case Keyword_signed:
                    extraBuiltTyK = BuiltinTypeKind::Short_S;
                    break;
                case Keyword_unsigned:
                    extraBuiltTyK = BuiltinTypeKind::Short_U;
                    break;
                default:
                    // report
                    return Action::Skip;
            }
            break;

        case BuiltinTypeKind::Short_S:
        case BuiltinTypeKind::Short_U:
            // report
            return Action::Skip;

        case BuiltinTypeKind::Int:
            switch (tySpecTk.kind()) {
                case Keyword_long:
                    extraBuiltTyK = BuiltinTypeKind::Long;
                    break;
                case Keyword_signed:
                    extraBuiltTyK = BuiltinTypeKind::Int_S;
                    break;
                case Keyword_unsigned:
                    extraBuiltTyK = BuiltinTypeKind::Int_U;
                    break;
                default:
                    diagReporter_.TwoOrMoreDataTypesInDeclarationSpecifiers(tySpecTk);
                    return Action::Skip;
            }
            break;

        case BuiltinTypeKind::Int_S:
        case BuiltinTypeKind::Int_U:
            // report
            return Action::Skip;

        case BuiltinTypeKind::Long:
            switch (tySpecTk.kind()) {
                case Keyword_int:
                    return Action::Skip;
                case Keyword_signed:
                    extraBuiltTyK = BuiltinTypeKind::Long_S;
                    break;
                case Keyword_unsigned:
                    extraBuiltTyK = BuiltinTypeKind::Long_U;
                    break;
                default:
                    diagReporter_.TwoOrMoreDataTypesInDeclarationSpecifiers(tySpecTk);
                    return Action::Skip;
            }
            break;

        case BuiltinTypeKind::Long_S:
        case BuiltinTypeKind::Long_U:
            // report
            return Action::Skip;

        default:
            PSY_ESCAPE_VIA_RETURN(Action::Skip);
    }

    if (extraBuiltTyK != curBuiltTyK)
        namedTySym->patchBuiltinTypeKind(extraBuiltTyK);

    return Action::Skip;
}

SyntaxVisitor::Action Binder::visitTagTypeSpecifier(const TagTypeSpecifierSyntax* node)
{
    if (!node->declarations()) {
        TagSymbolName::TagChoice tagChoice;
        switch (node->kind()) {
            case StructTypeSpecifier:
                tagChoice = TagSymbolName::TagChoice::Struct;
                break;

            case UnionTypeSpecifier:
                tagChoice = TagSymbolName::TagChoice::Union;
                break;

            case EnumTypeSpecifier:
                tagChoice = TagSymbolName::TagChoice::Enum;
                break;

            default:
                PSY_ESCAPE_VIA_RETURN(Action::Quit);
                return Action::Quit;
        }

        makeTySymAndPushIt<NamedTypeSymbol>(tagChoice, node->tagToken().valueText_c_str());
    }

    for (auto attrIt = node->attributes(); attrIt; attrIt = attrIt->next)
        visit(attrIt->value);

    for (auto declIt = node->declarations(); declIt; declIt = declIt->next) {
        TySymContT tySyms;
        std::swap(tySyms_, tySyms);
        visit(declIt->value);
        std::swap(tySyms_, tySyms);
    }

    for (auto attrIt = node->attributes_PostCloseBrace(); attrIt; attrIt = attrIt->next)
        visit(attrIt->value);

    return Action::Skip;
}

SyntaxVisitor::Action Binder::visitTagDeclarationAsSpecifier(
        const TagDeclarationAsSpecifierSyntax* node)
{
    visit(node->tagDeclaration());

    const TagTypeSpecifierSyntax* tySpec = node->tagDeclaration()->typeSpecifier();
    TagSymbolName::TagChoice tagChoice;
    switch (tySpec->kind()) {
        case StructTypeSpecifier:
            tagChoice = TagSymbolName::TagChoice::Struct;
            break;

        case UnionTypeSpecifier:
            tagChoice = TagSymbolName::TagChoice::Union;
            break;

        case EnumTypeSpecifier:
            tagChoice = TagSymbolName::TagChoice::Enum;
            break;

        default:
            PSY_ESCAPE_VIA_RETURN(Action::Quit);
    }

    makeTySymAndPushIt<NamedTypeSymbol>(tagChoice, tySpec->tagToken().valueText_c_str());

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
    PSY_ASSERT(!tySyms_.empty(), return Action::Quit);
    TypeSymbol* tySym = tySyms_.top();

    const auto tyQualTk = node->qualifierKeyword();
    switch (tyQualTk.kind()) {
        case Keyword_const:
            tySym->qualifyWithConst();
            break;

        case Keyword_volatile:
            tySym->qualifyWithVolatile();
            break;

        case Keyword_restrict:
            if (tySym->typeKind() == TypeKind::Pointer)
                tySym->qualifyWithRestrict();
            else
                diagReporter_.InvalidUseOfRestrict(tyQualTk);
            break;

        case Keyword__Atomic:
            tySym->qualifyWithAtomic();
            break;

        default:
            PSY_ESCAPE_VIA_BREAK;
    }

    return Action::Skip;
}
