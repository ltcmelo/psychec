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

#include "compilation/SemanticModel.h"
#include "binder/Scopes.h"
#include "symbols/Symbols.h"
#include "symbols/SymbolNames.h"
#include "syntax/SyntaxNodes.h"
#include "syntax/SyntaxUtilities.h"

#include "../common/infra/PsycheAssert.h"

#include <iostream>

using namespace psy;
using namespace C;

Binder::Binder(SemanticModel* semaModel, const SyntaxTree* tree)
    : SyntaxVisitor(tree)
    , semaModel__(semaModel)
    , diagReporter__(this)
{}

Binder::~Binder()
{
}

void Binder::bind()
{
    visit(tree_->root());
}

template <class SymT>
SymT* Binder::makeAndPushDeclSym__()
{
    std::unique_ptr<SymT> sym(new SymT(tree_,
                                       scopes__.top(),
                                       syms__.top()));
    return pushSym__(std::move(sym));
}

template FieldSymbol* Binder::makeAndPushDeclSym__<FieldSymbol>();
template FunctionSymbol* Binder::makeAndPushDeclSym__<FunctionSymbol>();
template ParameterSymbol* Binder::makeAndPushDeclSym__<ParameterSymbol>();
template VariableSymbol* Binder::makeAndPushDeclSym__<VariableSymbol>();

NamedTypeSymbol* Binder::makeAndPushDeclSym__(std::unique_ptr<SymbolName> symName,
                                              TypeKind tyKind)
{
    std::unique_ptr<NamedTypeSymbol> sym(
                new NamedTypeSymbol(tree_,
                                    scopes__.top(),
                                    syms__.top(),
                                    std::move(symName),
                                    tyKind));
    return pushSym__(std::move(sym));
}

template <>
LinkUnitSymbol* Binder::makeAndPushDeclSym__<LinkUnitSymbol>()
{
    std::unique_ptr<LinkUnitSymbol> sym(new LinkUnitSymbol(tree_, nullptr, nullptr));
    return pushSym__(std::move(sym));
}

template <class SymT>
SymT* Binder::pushSym__(std::unique_ptr<SymT> sym)
{
    syms__.push(sym.get());
    return static_cast<SymT*>(semaModel__->storeSymbol(std::move(sym)));
}

void Binder::popSym__()
{
    syms__.pop();
}

template <class ScopeT>
void Binder::openScope__()
{
    auto scope = syms__.top()->makeScope__<ScopeT>();
    scopes__.push(scope);
}

void Binder::openNestedScope__()
{
    auto scope = scopes__.top()->makeNestedScope__();
    scopes__.push(scope);
}

void Binder::closeScope__()
{
    scopes__.pop();
}

//--------------//
// Declarations //
//--------------//
SyntaxVisitor::Action Binder::visitTranslationUnit(const TranslationUnitSyntax* node)
{
    makeAndPushDeclSym__<LinkUnitSymbol>();
    openScope__<FileScope>();

    for (auto declIt = node->declarations(); declIt; declIt = declIt->next)
        visit(declIt->value);

    closeScope__();

    return Action::Skip;
}

SyntaxVisitor::Action Binder::visitIncompleteDeclaration(const IncompleteDeclarationSyntax* node)
{
    diagReporter__.UselessDeclaration(node->lastToken());

    for (auto specIt = node->specifiers(); specIt; specIt = specIt->next)
        ;

    return Action::Skip;
}

SyntaxVisitor::Action Binder::visitTypeDeclaration_COMMON(const TypeDeclarationSyntax* node)
{
    visit(node->typeSpecifier());

    return Action::Skip;
}

SyntaxVisitor::Action Binder::visitStructOrUnionDeclaration(const StructOrUnionDeclarationSyntax* node)
{
    return visitTypeDeclaration_COMMON(node);
}

SyntaxVisitor::Action Binder::visitEnumDeclaration(const EnumDeclarationSyntax* node)
{
    return visitTypeDeclaration_COMMON(node);
}

SyntaxVisitor::Action Binder::visitVariableAndOrFunctionDeclaration(const VariableAndOrFunctionDeclarationSyntax* node)
{
    for (auto decltorIt = node->declarators(); decltorIt; decltorIt = decltorIt->next) {
        auto decltor = SyntaxUtilities::strippedDeclarator(decltorIt->value);

        switch (decltor->kind()) {
            case FunctionDeclarator:
                makeAndPushDeclSym__<FunctionSymbol>();
                break;

            case ArrayDeclarator:
            case IdentifierDeclarator:
                switch (syms__.top()->kind()) {
                    case SymbolKind::LinkUnit:
                    case SymbolKind::Function:
                        makeAndPushDeclSym__<VariableSymbol>();
                        break;

                    default:
                        PSYCHE_FAIL(return Action::Quit, "unexpected symbol");
                        return Action::Quit;
                }
                break;

            default:
                PSYCHE_FAIL(return Action::Quit, "unexpected declarator");
                break;
        }

        for (auto specIt = node->specifiers(); specIt; specIt = specIt->next)
            visit(specIt->value);

        visit(decltorIt->value);

        popSym__();
    }

    return Action::Skip;
}

SyntaxVisitor::Action Binder::visitFieldDeclaration(const FieldDeclarationSyntax* node)
{
    for (auto specIt = node->specifiers(); specIt; specIt = specIt->next)
        visit(specIt->value);

    for (auto decltorIt = node->declarators(); decltorIt; decltorIt = decltorIt->next) {
        auto decltor = SyntaxUtilities::strippedDeclarator(decltorIt->value);
        switch (decltor->kind()) {
            case ArrayDeclarator:
            case IdentifierDeclarator:
                switch (syms__.top()->kind()) {
                    case SymbolKind::Type:
                        makeAndPushDeclSym__<FieldSymbol>();
                        break;

                    default:
                        PSYCHE_FAIL(return Action::Quit, "unexpected symbol");
                        return Action::Quit;
                }
                break;

            default:
                PSYCHE_FAIL(return Action::Quit, "unexpected declarator");
                break;
        }
    }

    return Action::Skip;
}

SyntaxVisitor::Action Binder::visitParameterDeclaration(const ParameterDeclarationSyntax* node)
{
    for (auto specIt = node->specifiers(); specIt; specIt = specIt->next)
        visit(specIt->value);

    visit(node->declarator());

    return Action::Skip;
}

SyntaxVisitor::Action Binder::visitStaticAssertDeclaration(const StaticAssertDeclarationSyntax*)
{
    return Action::Skip;
}

SyntaxVisitor::Action Binder::visitFunctionDefinition(const FunctionDefinitionSyntax* node)
{
    for (auto specIt = node->specifiers(); specIt; specIt = specIt->next)
        visit(specIt->value);

    visit(node->declarator());

    visit(node->body());

    return Action::Skip;
}

/* Specifiers */
SyntaxVisitor::Action Binder::visitBuiltinTypeSpecifier(const BuiltinTypeSpecifierSyntax* node)
{
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

    std::unique_ptr<SymbolName> symName(
                new TagSymbolName(tyKind,
                                  node->tagToken().valueText_c_str()));
    makeAndPushDeclSym__(std::move(symName), tyKind);

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
    return Action::Skip;
}

/* Declarators */
SyntaxVisitor::Action Binder::visitIdentifierDeclarator(const IdentifierDeclaratorSyntax* node)
{
    std::unique_ptr<SymbolName> symName(
                new PlainSymbolName(node->identifierToken().valueText_c_str()));
    syms__.top()->giveName_(std::move(symName));

    return Action::Skip;
}

SyntaxVisitor::Action Binder::visitAbstractDeclarator(const AbstractDeclaratorSyntax*)
{
    std::unique_ptr<SymbolName> symName(new EmptySymbolName);
    syms__.top()->giveName_(std::move(symName));

    return Action::Skip;
}

//------------//
// Statements //
//------------//
SyntaxVisitor::Action Binder::visitCompoundStatement(const CompoundStatementSyntax* node)
{
    openNestedScope__();

    for (auto stmtIt = node->statements(); stmtIt; stmtIt = stmtIt->next)
        visit(stmtIt->value);

    closeScope__();

    return Action::Skip;
}

SyntaxVisitor::Action Binder::visitDeclarationStatement(const DeclarationStatementSyntax* node)
{
    visit(node->declaration());

    return Action::Skip;
}
