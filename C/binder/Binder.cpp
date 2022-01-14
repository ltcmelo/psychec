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

Binder::Binder(SemanticModel* semaModel, const SyntaxTree* tree)
    : SyntaxVisitor(tree)
    , semaModel_(semaModel)
    , diagReporter_(this)
{}

Binder::~Binder()
{
}

void Binder::bind()
{
    visit(tree_->root());
}

template <class SymT>
SymT* Binder::makeAndPushSymDEF()
{
    std::unique_ptr<SymT> sym(new SymT(tree_,
                                       scopes_.top(),
                                       symDEFs_.top()));
    return pushSymDEF(std::move(sym));
}

template FieldSymbol* Binder::makeAndPushSymDEF<FieldSymbol>();
template FunctionSymbol* Binder::makeAndPushSymDEF<FunctionSymbol>();
template ParameterSymbol* Binder::makeAndPushSymDEF<ParameterSymbol>();
template VariableSymbol* Binder::makeAndPushSymDEF<VariableSymbol>();
template <>
LinkUnitSymbol* Binder::makeAndPushSymDEF<LinkUnitSymbol>()
{
    std::unique_ptr<LinkUnitSymbol> sym(new LinkUnitSymbol(tree_, nullptr, nullptr));
    return pushSymDEF(std::move(sym));
}

NamedTypeSymbol* Binder::makeAndPushSymDEF(TypeKind tyKind)
{
    std::unique_ptr<NamedTypeSymbol> sym(
                new NamedTypeSymbol(tree_,
                                    scopes_.top(),
                                    symDEFs_.top(),
                                    tyKind));
    return pushSymDEF(std::move(sym));
}

template <class SymT>
SymT* Binder::pushSymDEF(std::unique_ptr<SymT> sym)
{
    symDEFs_.push(sym.get());
    return static_cast<SymT*>(semaModel_->storeSymDEF(std::move(sym)));
}

void Binder::popSymDEF()
{
    symDEFs_.pop();
}

template <class ScopeT>
void Binder::openScope()
{
    auto scope = symDEFs_.top()->makeScope<ScopeT>();
    scopes_.push(scope);
}

void Binder::openNestedScope()
{
    auto scope = scopes_.top()->makeNestedScope();
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
    makeAndPushSymDEF<LinkUnitSymbol>();
    openScope<FileScope>();

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
    declData_ = std::make_unique<DeclData>();

    for (auto specIt = node->specifiers(); specIt; specIt = specIt->next)
        visit(specIt->value);

    for (auto decltorIt = node->declarators(); decltorIt; decltorIt = decltorIt->next) {
        auto decltor = decltorIt->value;

        switch (decltor->kind()) {
            case FunctionDeclarator:
                makeAndPushSymDEF<FunctionSymbol>();
                break;

            case ArrayDeclarator:
            case PointerDeclarator:
            case IdentifierDeclarator:
                makeAndPushSymDEF<VariableSymbol>();
                break;

            default:
                PSYCHE_FAIL_0(return Action::Quit);
        }

        visit(decltor);

        auto sym = symDEFs_.top();
        switch (sym->kind()) {
            case SymbolKind::Function:
                PSYCHE_ASSERT_0(decltor->kind() == FunctionDeclarator, return Action::Quit);
                break;

            case SymbolKind::Value: {
                auto valSym = sym->asValue();
                switch (decltor->kind())
                {
                    case ArrayDeclarator:
                        break;

                    case PointerDeclarator:
                        valSym->setType(declData_->ptrTySyms.top());
                        std::stack<TypeSymbol*>().swap(declData_->ptrTySyms);
                        break;

                    case IdentifierDeclarator:
                         valSym->setType(declData_->tySym);
                         break;

                    default:
                        PSYCHE_FAIL_0(return Action::Quit);
                }

                break;
            }

            default:
                PSYCHE_FAIL_0(return Action::Quit);
        }

        popSymDEF();
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
                switch (symDEFs_.top()->kind()) {
                    case SymbolKind::Type:
                        makeAndPushSymDEF<FieldSymbol>();
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

void Binder::f(std::unique_ptr<TypeSymbol> tySym)
{
    declData_->tySym = tySym.get();
    semaModel_->storeSymUSE(std::move(tySym));
}

/* Specifiers */
SyntaxVisitor::Action Binder::visitBuiltinTypeSpecifier(const BuiltinTypeSpecifierSyntax* node)
{
    PSYCHE_ASSERT_0(declData_, return Action::Quit);

    if (!declData_->tySym) {
        std::unique_ptr<TypeSymbol> namedTySym(
                new NamedTypeSymbol(tree_,
                                    scopes_.top(),
                                    symDEFs_.top(),
                                    TypeKind::Builtin));
        f(std::move(namedTySym));
    }

    NamedTypeSymbol* namedTySym = declData_->tySym->asNamedType();
    if (!namedTySym) {
        //error
        return Action::Skip;
    }

    auto builtTyKind = namedTySym->builtinTypeKind();
    builtTyKind = Semantics_TypeSpecifiers::combine(node->specifierToken(),
                                                    builtTyKind,
                                                    &diagReporter_);
    namedTySym->patchBuiltinTypeKind(builtTyKind);

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

    makeAndPushSymDEF(tyKind);
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
    PSYCHE_ASSERT_0(declData_, return Action::Quit);

    if (!declData_->tySym) {
        std::unique_ptr<TypeSymbol> namedTySym(
                new NamedTypeSymbol(tree_,
                                    scopes_.top(),
                                    symDEFs_.top(),
                                    TypeKind::Synonym));
        f(std::move(namedTySym));
    }

    NamedTypeSymbol* namedTySym = declData_->tySym->asNamedType();
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
    auto sym = symDEFs_.top();
    switch (sym->kind()) {
        case SymbolKind::Function:
            break;

        case SymbolKind::Value: {
            break;
        }

        default:
//            PSYCHE_FAIL(return Action::Quit, "unexpected symbol");
            return Action::Quit;
    }

    return Action::Skip;
}

/* Declarators */
SyntaxVisitor::Action Binder::visitArrayOrFunctionDeclarator(const ArrayOrFunctionDeclaratorSyntax* node)
{
    return Action::Skip;
}

SyntaxVisitor::Action Binder::visitPointerDeclarator(const PointerDeclaratorSyntax* node)
{
    TypeSymbol* refedTySym;
    if (declData_->ptrTySyms.empty())
        refedTySym = declData_->tySym;
    else
        refedTySym = declData_->ptrTySyms.top();

    auto ptrTySym =  new PointerTypeSymbol(tree_,
                                           scopes_.top(),
                                                      symDEFs_.top(),
                                                      refedTySym);

    semaModel_->storeSymUSE(std::unique_ptr<TypeSymbol>(ptrTySym));

    declData_->ptrTySyms.push(ptrTySym);


    visit(node->innerDeclarator());

    return Action::Skip;
}

SyntaxVisitor::Action Binder::visitIdentifierDeclarator(const IdentifierDeclaratorSyntax* node)
{
    std::cout << "setting name " << node->identifierToken().valueText() << std::endl;
    std::unique_ptr<SymbolName> name(
                new PlainSymbolName(node->identifierToken().valueText_c_str()));
    symDEFs_.top()->setName(std::move(name));

    return Action::Skip;
}

SyntaxVisitor::Action Binder::visitAbstractDeclarator(const AbstractDeclaratorSyntax*)
{
    std::unique_ptr<SymbolName> name(new EmptySymbolName);
    symDEFs_.top()->setName(std::move(name));

    return Action::Skip;
}

//------------//
// Statements //
//------------//
SyntaxVisitor::Action Binder::visitCompoundStatement(const CompoundStatementSyntax* node)
{
    openNestedScope();

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
