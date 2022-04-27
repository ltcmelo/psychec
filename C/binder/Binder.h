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

#include "parser/LexedTokens.h"
#include "symbols/SymbolName.h"
#include "symbols/Symbols.h"
#include "symbols/TypeKind.h"
#include "syntax/SyntaxVisitor.h"

#include "../common/diagnostics/DiagnosticDescriptor.h"

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
class PSY_C_API Binder final : protected SyntaxVisitor
{
public:
    Binder(const Binder&) = delete;
    void operator=(const Binder&) = delete;
    ~Binder();

    void bind();

private:
    friend class BinderTest;
    friend class SemanticModel;
    friend class Semantics_TypeSpecifiers;
    friend class Semantics_TypeQualifiers;

    Binder(SemanticModel* semaModel, const SyntaxTree* tree);

    SemanticModel* semaModel_;

    template <class ScopeT> void openScope();
    void openNestedScope();
    void closeScope();
    std::stack<Scope*> scopes_;

    template <class SymT> SymT* pushSym(std::unique_ptr<SymT>);
    void popSym();
    using SymCont_T = std::stack<Symbol*>;
    SymCont_T syms_;

    template <class TySymT> TySymT* pushTySym(std::unique_ptr<TySymT>);
    void popTySym();
    using TySymCont_T = std::stack<TypeSymbol*>;
    TySymCont_T tySyms_;

    template <class SymT, class... Args> std::unique_ptr<SymT> makeSymOrTySym(Args... args);
    template <class SymT, class... Args> void makeSymAndPushIt(Args... arg);
    template <class SymT, class... Args> void makeTySymAndPushIt(Args... arg);

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
    virtual Action visitStructOrUnionDeclaration(const StructOrUnionDeclarationSyntax*) override;
    virtual Action visitEnumDeclaration(const EnumDeclarationSyntax*) override;
    virtual Action visitVariableAndOrFunctionDeclaration(const VariableAndOrFunctionDeclarationSyntax*) override;
    virtual Action visitFieldDeclaration(const FieldDeclarationSyntax*) override;
    virtual Action visitParameterDeclaration(const ParameterDeclarationSyntax*) override;
    virtual Action visitStaticAssertDeclaration(const StaticAssertDeclarationSyntax*) override;
    virtual Action visitFunctionDefinition(const FunctionDefinitionSyntax*) override;

    Action visitTypeDeclaration_COMMON(const TypeDeclarationSyntax*);
    template <class DeclT> Action visitDeclaration_AtSpecifiers(
            const DeclT* node,
            Action (Binder::*visit_AtDeclarators)(const DeclT*));
    template <class DeclT> Action visitDeclaration_AtDeclarators(
            const DeclT* node,
            Action (Binder::*visit_DONE)(const DeclT*));
    Action visitFunctionDefinition_AtSpecifiers(const FunctionDefinitionSyntax*);
    Action visitFunctionDefinition_AtDeclarator(const FunctionDefinitionSyntax*);
    Action visitFunctionDefinition_DONE(const FunctionDefinitionSyntax*);
    Action visitVariableAndOrFunctionDeclaration_AtSpecifiers(const VariableAndOrFunctionDeclarationSyntax*);
    Action visitVariableAndOrFunctionDeclaration_AtDeclarators(const VariableAndOrFunctionDeclarationSyntax*);
    Action visitVariableAndOrFunctionDeclaration_DONE(const VariableAndOrFunctionDeclarationSyntax*);
    Action visitFieldDeclaration_AtSpecifiers(const FieldDeclarationSyntax*);
    Action visitFieldDeclaration_AtDeclarators(const FieldDeclarationSyntax*);
    Action visitFieldDeclaration_DONE(const FieldDeclarationSyntax*);
    Action visitParameterDeclaration_AtSpecifiers(const ParameterDeclarationSyntax*);
    Action visitParameterDeclaration_AtDeclarator(const ParameterDeclarationSyntax*);
    Action visitParameterDeclaration_DONE(const ParameterDeclarationSyntax*);

    /* Specifiers */
    Action actOnTypeSpecifier(const SpecifierSyntax*);
    Action actOnTypeQualifier(const SpecifierSyntax*);
    virtual Action visitBuiltinTypeSpecifier(const BuiltinTypeSpecifierSyntax*) override;
    virtual Action visitTagTypeSpecifier(const TagTypeSpecifierSyntax*) override;
    virtual Action visitTypeDeclarationAsSpecifier(const TypeDeclarationAsSpecifierSyntax*) override;
    virtual Action visitTypedefName(const TypedefNameSyntax*) override;
    virtual Action visitTypeQualifier(const TypeQualifierSyntax*) override;

    /* Declarators */
    Action actOnDeclarator(const DeclaratorSyntax*);
    virtual Action visitArrayOrFunctionDeclarator(const ArrayOrFunctionDeclaratorSyntax*) override;
    virtual Action visitPointerDeclarator(const PointerDeclaratorSyntax*) override;
    virtual Action visitParenthesizedDeclarator(const ParenthesizedDeclaratorSyntax*) override;
    virtual Action visitIdentifierDeclarator(const IdentifierDeclaratorSyntax*) override;
    virtual Action visitAbstractDeclarator(const AbstractDeclaratorSyntax*) override;
    virtual Action visitSubscriptSuffix(const SubscriptSuffixSyntax*) override;
    virtual Action visitParameterSuffix(const ParameterSuffixSyntax*) override;

    //------------//
    // Statements //
    //------------//
    virtual Action visitCompoundStatement(const CompoundStatementSyntax*) override;
    virtual Action visitDeclarationStatement(const DeclarationStatementSyntax*) override;
};

template <class SymT, class... Args>
std::unique_ptr<SymT> Binder::makeSymOrTySym(Args... args)
{
    std::unique_ptr<SymT> sym(new SymT(tree_,
                                       scopes_.top(),
                                       syms_.top(),
                                       std::forward<Args>(args)...));
    return sym;
}

template <class SymT, class... Args>
void Binder::makeSymAndPushIt(Args... args)
{
    std::unique_ptr<SymT> sym = makeSymOrTySym<SymT>(std::forward<Args>(args)...);
    pushSym(std::move(sym));
}

template <class SymT, class... Args>
void Binder::makeTySymAndPushIt(Args... args)
{
    std::unique_ptr<SymT> sym = makeSymOrTySym<SymT>(std::forward<Args>(args)...);
    pushTySym(std::move(sym));
}

} // C
} // psy

#endif
