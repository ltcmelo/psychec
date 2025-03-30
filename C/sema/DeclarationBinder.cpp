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

#include "DeclarationBinder.h"
#include "DeclarationBinder__MACROS__.inc"

#include "syntax/SyntaxTree.h"
#include "sema/Scope.h"
#include "sema/Compilation.h"
#include "symbols/Symbol_ALL.h"
#include "syntax/SyntaxFacts.h"
#include "syntax/Lexeme_ALL.h"
#include "syntax/SyntaxNodes.h"
#include "syntax/SyntaxUtilities.h"
#include "syntax/SyntaxVisitor__MACROS__.inc"
#include "types/Type_ALL.h"
#include "../common/infra/Assertions.h"

#include <iostream>

using namespace psy;
using namespace C;

DeclarationBinder::DeclarationBinder(SemanticModel* semaModel, const SyntaxTree* tree)
    : SyntaxVisitor(tree)
    , semaModel_(semaModel)
    , stashedScope_(nullptr)
    , BD_(0)
    , ptrdiff_t_(tree->findIdentifier("ptrdiff_t", 9))
    , size_t_(tree->findIdentifier("size_t", 6))
    , max_align_t_(tree->findIdentifier("max_align_t", 11))
    , wchar_t_(tree->findIdentifier("wchar_t", 7))
    , char16_t_(tree->findIdentifier("char16_t", 8))
    , char32_t_(tree->findIdentifier("char32_t", 8))
    , diagReporter_(this)
{}

DeclarationBinder::~DeclarationBinder()
{
}

void DeclarationBinder::bindDeclarations()
{
    visit(tree_->root());
}

void DeclarationBinder::pushNewScope(
        const SyntaxNode* node,
        ScopeKind scopeK,
        bool encloseInOuterScope)
{
    PSY_ASSERT_2(scopeK == ScopeKind::File
                   || scopeK == ScopeKind::Block
                   || scopeK == ScopeKind::Function
                   || scopeK == ScopeKind::FunctionPrototype,
               return);

    std::unique_ptr<Scope> newScope(new Scope(scopeK));
    auto scope = semaModel_->keepScope(std::move(newScope));
    if (!encloseInOuterScope)
        scopes_.push(scope);
    else {
        PSY_ASSERT_2(!scopes_.empty(), return);
        auto outerScope = scopes_.top();
        scopes_.push(scope);
        outerScope->encloseScope(scope);
    }
}

void DeclarationBinder::pushStashedScope()
{
    PSY_ASSERT_2(stashedScope_, return);
    scopes_.push(stashedScope_);
}

void DeclarationBinder::popScope()
{
    scopes_.pop();
}

void DeclarationBinder::popAndStashScope()
{
    stashedScope_ = scopes_.top();
    scopes_.pop();
}

Symbol* DeclarationBinder::containingUnitOrDeclaration()
{
    if (!decls_.empty())
        return decls_.top();
    return unit_;
}

void DeclarationBinder::pushDeclaration(DeclarationSymbol* sym)
{
    DBG_DECL_STACK(sym);
    decls_.push(sym);
}

DeclarationSymbol* DeclarationBinder::popDeclaration()
{
    PSY_ASSERT_2(!decls_.empty(), return nullptr);
    DBG_DECL_STACK(syms_.top());
    auto decl = decls_.top();
    decls_.pop();
    return decl;
}


void DeclarationBinder::finishDeclaration()
{
    auto decl = popDeclaration();
    PSY_ASSERT_2(decl, return);
    SCOPE_AT_TOP(auto scope, );
    scope->addDeclaration(decl->asDeclaration());
}

void DeclarationBinder::pushType(Type* ty)
{
    DBG_TY_STACK(ty);
    tys_.push(ty);
}

Type* DeclarationBinder::popType()
{
    PSY_ASSERT_2(!tys_.empty(), return nullptr);
    DBG_TY_STACK(tys_.top());
    auto ty = tys_.top();
    tys_.pop();
    return ty;
}

const Identifier* DeclarationBinder::obtainName(const SyntaxToken& tk) const
{
    return tk.lexeme()
            ? tk.lexeme()->asIdentifier()
            : tree_->findIdentifier("", 0);
}

const Identifier* DeclarationBinder::obtainTag(const SyntaxToken& tk)
{
    if (tk.lexeme())
        return tk.lexeme()->asIdentifier();
    auto tag = semaModel_->freshSyntheticTag();
    syntheticTags_.push(tag);
    return tag;
}

const Identifier* DeclarationBinder::obtainKnownTag(const SyntaxToken& tk)
{
    if (tk.lexeme())
        return tk.lexeme()->asIdentifier();
    PSY_ASSERT_2(!syntheticTags_.empty()
                    && syntheticTags_.top(),
                 return nullptr);
    auto actualTag = syntheticTags_.top();
    syntheticTags_.pop();
    return actualTag;
}

//--------------//
// Declarations //
//--------------//

SyntaxVisitor::Action DeclarationBinder::visitTranslationUnit(const TranslationUnitSyntax* node)
{
    std::unique_ptr<TranslationUnitSymbol> unit(
                new TranslationUnitSymbol(semaModel_->compilation()->program(),
                                          tree_));
    unit_ = semaModel_->setTranslationUnit(std::move(unit));

    pushNewScope(node, ScopeKind::File, false);
    VISIT(node->declarations());
    PSY_ASSERT_2(scopes_.size() == 1, return Action::Quit);
    PSY_ASSERT_2(scopes_.top()->kind() == ScopeKind::File, return Action::Quit);
    popScope();

    return Action::Skip;
}

SyntaxVisitor::Action DeclarationBinder::visitIncompleteDeclaration(const IncompleteDeclarationSyntax* node)
{
    diagReporter_.UselessDeclaration(node->lastToken());

    return Action::Skip;
}

SyntaxVisitor::Action DeclarationBinder::visitStructOrUnionDeclaration(const StructOrUnionDeclarationSyntax* node)
{
    return visitStructOrUnionDeclaration_AtSpecifier(node);
}

SyntaxVisitor::Action DeclarationBinder::visitEnumDeclaration(const EnumDeclarationSyntax* node)
{
    return visitEnumDeclaration_AtSpecifier(node);
}

SyntaxVisitor::Action DeclarationBinder::visitTypedefDeclaration(const TypedefDeclarationSyntax* node)
{
    return visitTypedefDeclaration_AtSpecifier(node);
}

SyntaxVisitor::Action DeclarationBinder::visitVariableAndOrFunctionDeclaration(
        const VariableAndOrFunctionDeclarationSyntax* node)
{
    return visitVariableAndOrFunctionDeclaration_AtSpecifiers(node);
}

SyntaxVisitor::Action DeclarationBinder::visitFieldDeclaration(const FieldDeclarationSyntax* node)
{
    return visitFieldDeclaration_AtSpecifiers(node);
}

SyntaxVisitor::Action DeclarationBinder::visitEnumeratorDeclaration(const EnumeratorDeclarationSyntax* node)
{
    return visitEnumeratorDeclaration_AtImplicitSpecifier(node);
}

SyntaxVisitor::Action DeclarationBinder::visitParameterDeclaration(const ParameterDeclarationSyntax* node)
{
    return visitParameterDeclaration_AtSpecifiers(node);
}

SyntaxVisitor::Action DeclarationBinder::visitStaticAssertDeclaration(const StaticAssertDeclarationSyntax*)
{
    return Action::Skip;
}

SyntaxVisitor::Action DeclarationBinder::visitFunctionDefinition(const FunctionDefinitionSyntax* node)
{
    return visitFunctionDefinition_AtSpecifiers(node);
}

//------------//
// Statements //
//------------//

SyntaxVisitor::Action DeclarationBinder::visitCompoundStatement(const CompoundStatementSyntax* node)
{
    pushNewScope(node, ScopeKind::Block, true);
    VISIT(node->statements());
    popScope();

    return Action::Skip;
}

//-------------//
// Expressions //
//-------------//

SyntaxVisitor::Action DeclarationBinder::visitIdentifierName(const IdentifierNameSyntax* node)
{
    SCOPE_AT_TOP(auto scope, Action::Quit);
    semaModel_->setScopeOf(node, scope);

    return Action::Skip;
}

//--------//
// Common //
//--------//

SyntaxVisitor::Action DeclarationBinder::visitTypeName(const TypeNameSyntax* node)
{
    TypeStack tys;
    std::swap(tys_, tys);
    auto action = visitTypeName_AtSpecifier(node);
    std::swap(tys_, tys);
    return action;
}
