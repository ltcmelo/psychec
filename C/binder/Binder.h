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

#ifndef PSYCHE_C_BINDER_H__
#define PSYCHE_C_BINDER_H__

#include "API.h"
#include "Fwds.h"

#include "binder/Scope.h"
#include "parser/LexedTokens.h"
#include "symbols/SymbolName.h"
#include "symbols/Symbol_ALL.h"
#include "symbols/TypeKind.h"
#include "syntax/SyntaxVisitor.h"

#include "../common/diagnostics/DiagnosticDescriptor.h"
#include "../common/infra/InternalAccess.h"

#include <memory>
#include <stack>
#include <string>
#include <utility>

namespace psy {
namespace C {

class SemanticModel;

/**
 * \brief The Binder class.
 */
class PSY_C_NON_API Binder final : protected SyntaxVisitor
{
    friend class BinderTester;

PSY_INTERNAL_AND_RESTRICTED:
    PSY_GRANT_ACCESS(SemanticModel);
    PSY_GRANT_ACCESS(ConstraintsInDeclarations);
    PSY_GRANT_ACCESS(ConstraintsInTypeSpecifiers);
    PSY_GRANT_ACCESS(ConstraintsInDeclarators);
    PSY_GRANT_ACCESS(SemanticsOfTypeQualifiers);

    Binder(SemanticModel* semaModel, const SyntaxTree* tree);
    ~Binder();

    void bind();

private:
    // Unavailable
    Binder(const Binder&) = delete;
    void operator=(const Binder&) = delete;

    SemanticModel* semaModel_;

    void openScope(ScopeKind scopeK);
    void closeScope();
    void reopenStashedScope();
    void closeScopeAndStashIt();
    std::stack<Scope*> scopes_;
    Scope* stashedScope_;

    template <class SymT> SymT* pushSym(const SyntaxNode* node, std::unique_ptr<SymT>);
    void popSym();
    using SymContT = std::stack<Symbol*>;
    SymContT syms_;

    template <class TySymT> TySymT* pushTySym(std::unique_ptr<TySymT>);
    void popTySym();
    using TySymContT = std::stack<TypeSymbol*>;
    TySymContT tySyms_;

    std::stack<FunctionTypeSymbol*> pendingFunTySyms_;

    template <class SymT, class... SymTArgs> std::unique_ptr<SymT> makeSymOrTySym(SymTArgs... args);
    template <class SymT, class... SymTArgs> void makeSymAndPushIt(const SyntaxNode* node, SymTArgs... arg);
    template <class SymT, class... SymTArgs> void makeTySymAndPushIt(SymTArgs... arg);

    struct DiagnosticsReporter
    {
        DiagnosticsReporter(Binder* binder)
            : binder_(binder)
        {}
        Binder* binder_;
        void diagnose(DiagnosticDescriptor&& desc, SyntaxToken tk);
    };

    DiagnosticsReporter diagReporter_;

    //--------------//
    // Declarations //
    //--------------//
    virtual Action visitTranslationUnit(const TranslationUnitSyntax*) override;
    virtual Action visitIncompleteDeclaration(const IncompleteDeclarationSyntax*) override;
    virtual Action visitStaticAssertDeclaration(const StaticAssertDeclarationSyntax*) override;

    template <class TyDeclT> Action visitTypeDeclaration_AtInternalDeclarations_COMMON(
            const TyDeclT* node,
            Action (Binder::*visit_DONE)(const TyDeclT*));

    virtual Action visitStructOrUnionDeclaration(const StructOrUnionDeclarationSyntax*) override;
    Action visitStructOrUnionDeclaration_AtSpecifier(const StructOrUnionDeclarationSyntax*);
    Action visitStructOrUnionDeclaration_DONE(const StructOrUnionDeclarationSyntax*);
    virtual Action visitEnumDeclaration(const EnumDeclarationSyntax*) override;
    Action visitEnumDeclaration_AtSpecifier(const EnumDeclarationSyntax*);
    Action visitEnumDeclaration_DONE(const EnumDeclarationSyntax*);

    template <class DeclT> Action visitDeclaration_AtSpecifiers_COMMON(
            const DeclT* node,
            Action (Binder::*visit_AtDeclarators)(const DeclT*));
    template <class DeclT> Action visitDeclaration_AtDeclarators_COMMON(
            const DeclT* node,
            Action (Binder::*visit_DONE)(const DeclT*));

    virtual Action visitVariableAndOrFunctionDeclaration(const VariableAndOrFunctionDeclarationSyntax*) override;
    Action visitVariableAndOrFunctionDeclaration_AtSpecifiers(const VariableAndOrFunctionDeclarationSyntax*);
    Action visitVariableAndOrFunctionDeclaration_AtDeclarators(const VariableAndOrFunctionDeclarationSyntax*);
    Action visitVariableAndOrFunctionDeclaration_DONE(const VariableAndOrFunctionDeclarationSyntax*);

    virtual Action visitFieldDeclaration(const FieldDeclarationSyntax*) override;
    Action visitFieldDeclaration_AtSpecifiers(const FieldDeclarationSyntax*);
    Action visitFieldDeclaration_AtDeclarators(const FieldDeclarationSyntax*);
    Action visitFieldDeclaration_DONE(const FieldDeclarationSyntax*);

    virtual Action visitEnumeratorDeclaration(const EnumeratorDeclarationSyntax*) override;
    Action visitEnumeratorDeclaration_AtImplicitSpecifier(const EnumeratorDeclarationSyntax*);
    Action visitEnumeratorDeclaration_AtDeclarator(const EnumeratorDeclarationSyntax*);
    Action visitEnumeratorDeclaration_DONE(const EnumeratorDeclarationSyntax*);

    virtual Action visitParameterDeclaration(const ParameterDeclarationSyntax*) override;
    Action visitParameterDeclaration_AtSpecifiers(const ParameterDeclarationSyntax*);
    Action visitParameterDeclaration_AtDeclarator(const ParameterDeclarationSyntax*);
    Action visitParameterDeclaration_DONE(const ParameterDeclarationSyntax*);

    virtual Action visitFunctionDefinition(const FunctionDefinitionSyntax*) override;
    Action visitFunctionDefinition_AtSpecifiers(const FunctionDefinitionSyntax*);
    Action visitFunctionDefinition_AtDeclarator(const FunctionDefinitionSyntax*);
    Action visitFunctionDefinition_DONE(const FunctionDefinitionSyntax*);

    /* Specifiers */
    virtual Action visitBuiltinTypeSpecifier(const BuiltinTypeSpecifierSyntax*) override;
    virtual Action visitTagTypeSpecifier(const TagTypeSpecifierSyntax*) override;
    virtual Action visitTypeDeclarationAsSpecifier(const TypeDeclarationAsSpecifierSyntax*) override;
    virtual Action visitTypedefName(const TypedefNameSyntax*) override;
    virtual Action visitTypeQualifier(const TypeQualifierSyntax*) override;
    Action visitIfNotTypeQualifier(const SpecifierSyntax*);
    Action visitIfTypeQualifier(const SpecifierSyntax*);

    /* Declarators */
    virtual Action visitArrayOrFunctionDeclarator(const ArrayOrFunctionDeclaratorSyntax*) override;
    virtual Action visitPointerDeclarator(const PointerDeclaratorSyntax*) override;
    virtual Action visitParenthesizedDeclarator(const ParenthesizedDeclaratorSyntax*) override;
    virtual Action visitSubscriptSuffix(const SubscriptSuffixSyntax*) override;
    virtual Action visitParameterSuffix(const ParameterSuffixSyntax*) override;
    virtual Action visitIdentifierDeclarator(const IdentifierDeclaratorSyntax*) override;
    virtual Action visitAbstractDeclarator(const AbstractDeclaratorSyntax*) override;
    Action nameSymAtTop(const char* s);
    Action typeSymAtTopAndPopIt();

    template <class DecltrT> Action determineContextAndMakeSym(const DecltrT* node);

    //------------//
    // Statements //
    //------------//
    virtual Action visitCompoundStatement(const CompoundStatementSyntax*) override;
    virtual Action visitDeclarationStatement(const DeclarationStatementSyntax*) override;
};

template <class SymT, class... SymTArgs>
std::unique_ptr<SymT> Binder::makeSymOrTySym(SymTArgs... args)
{
    std::unique_ptr<SymT> sym(new SymT(tree_,
                                       scopes_.top(),
                                       syms_.top(),
                                       std::forward<SymTArgs>(args)...));
    return sym;
}

template <class SymT, class... SymTArgs>
void Binder::makeSymAndPushIt(const SyntaxNode* node, SymTArgs... args)
{
    std::unique_ptr<SymT> sym = makeSymOrTySym<SymT>(std::forward<SymTArgs>(args)...);
    pushSym(node, std::move(sym));
}

template <class SymT, class... SymTArgs>
void Binder::makeTySymAndPushIt(SymTArgs... args)
{
    std::unique_ptr<SymT> sym = makeSymOrTySym<SymT>(std::forward<SymTArgs>(args)...);
    pushTySym(std::move(sym));
}

} // C
} // psy

#endif
