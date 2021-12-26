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
    , semaModel_(semaModel)
    , diagReporter_(this)
{}

Binder::~Binder()
{}

void Binder::bind()
{
    visit(tree_->root());
}

template <class SymT>
SymT* Binder::newSym_COMMON(std::unique_ptr<SymT> sym)
{
    syms_.push(sym.get());
    return static_cast<SymT*>(semaModel_->storeSymbol(std::move(sym)));
}

template <class SymT>
SymT* Binder::newDeclSym()
{
    std::unique_ptr<SymT> sym(new SymT(tree_,
                                             scopes_.top(),
                                             syms_.top()));
    return newSym_COMMON(std::move(sym));
}

template FieldSymbol* Binder::newDeclSym<FieldSymbol>();
template FunctionSymbol* Binder::newDeclSym<FunctionSymbol>();
template ParameterSymbol* Binder::newDeclSym<ParameterSymbol>();
template VariableSymbol* Binder::newDeclSym<VariableSymbol>();

NamedTypeSymbol* Binder::newTyDeclSym(std::unique_ptr<SymbolName> symName,
                                   TypeKind tyKind)
{
    std::unique_ptr<NamedTypeSymbol> sym(
                new NamedTypeSymbol(tree_,
                                    scopes_.top(),
                                    syms_.top(),
                                    std::move(symName),
                                    tyKind));
    return newSym_COMMON(std::move(sym));
}

template <>
LinkUnitSymbol* Binder::newDeclSym<LinkUnitSymbol>()
{
    std::unique_ptr<LinkUnitSymbol> sym(new LinkUnitSymbol(tree_, nullptr, nullptr));
    return newSym_COMMON(std::move(sym));
}

template <class ScopeT>
void Binder::openScopeInSymbol()
{
    auto scope = syms_.top()->newScope<ScopeT>();
    scopes_.push(scope);
}

void Binder::openScopeInScope()
{
    auto scope = scopes_.top()->newBlock();
    scopes_.push(scope);
}

void Binder::closeScope()
{
    scopes_.pop();
}

//--------------//
// Declarations //
//--------------//
SyntaxVisitor::Action Binder::visitTranslationUnit(const TranslationUnitSyntax* node)
{
    newDeclSym<LinkUnitSymbol>();
    openScopeInSymbol<FileScope>();

    for (auto declIt = node->declarations(); declIt; declIt = declIt->next)
        visit(declIt->value);

    closeScope();

    return Action::Skip;
}

SyntaxVisitor::Action Binder::visitIncompleteDeclaration(const IncompleteDeclarationSyntax* node)
{
    diagReporter_.UselessDeclaration(node->lastToken());

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
    for (auto specIt = node->specifiers(); specIt; specIt = specIt->next)
        visit(specIt->value);

    for (auto decltorIt = node->declarators(); decltorIt; decltorIt = decltorIt->next) {
        auto decltor = SyntaxUtilities::strippedDeclarator(decltorIt->value);
        switch (decltor->kind()) {
            case FunctionDeclarator:
                newDeclSym<FunctionSymbol>();
                break;

            case ArrayDeclarator:
            case IdentifierDeclarator:
                switch (syms_.top()->kind()) {
                    case SymbolKind::LinkUnit:
                    case SymbolKind::Function:
                        newDeclSym<VariableSymbol>();
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

        visit(decltorIt->value);
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
                switch (syms_.top()->kind()) {
                    case SymbolKind::NamedType:
                        newDeclSym<FieldSymbol>();
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
    newTyDeclSym(std::move(symName), tyKind);

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
    std::unique_ptr<SymbolName> symName(
                new PlainSymbolName(node->identifierToken().valueText_c_str()));
    newTyDeclSym(std::move(symName), TypeKind::Typedef);

    return Action::Skip;
}

/* Declarators */
SyntaxVisitor::Action Binder::visitIdentifierDeclarator(const IdentifierDeclaratorSyntax* node)
{
    std::unique_ptr<SymbolName> symName(
                new PlainSymbolName(node->identifierToken().valueText_c_str()));
    syms_.top()->giveName(std::move(symName));

    return Action::Skip;
}

SyntaxVisitor::Action Binder::visitAbstractDeclarator(const AbstractDeclaratorSyntax*)
{
    std::unique_ptr<SymbolName> symName(new EmptySymbolName);
    syms_.top()->giveName(std::move(symName));

    return Action::Skip;
}

//------------//
// Statements //
//------------//
SyntaxVisitor::Action Binder::visitCompoundStatement(const CompoundStatementSyntax* node)
{
    openScopeInScope();

    for (auto stmtIt = node->statements(); stmtIt; stmtIt = stmtIt->next)
        visit(stmtIt->value);

    closeScope();

    return Action::Skip;
}

SyntaxVisitor::Action Binder::visitDeclarationStatement(const DeclarationStatementSyntax* node)
{
    visit(node->declaration());

    return Action::Skip;
}
