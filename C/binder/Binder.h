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
#include "compilation/SemanticModel.h"
#include "parser/LexedTokens.h"
#include "symbols/Symbol_ALL.h"
#include "syntax/SyntaxVisitor.h"
#include "syntax/SyntaxNodes.h"

#include "../common/diagnostics/DiagnosticDescriptor.h"
#include "../common/infra/AccessSpecifiers.h"

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
class PSY_C_INTERNAL_API Binder final : protected SyntaxVisitor
{
public:
    ~Binder();

PSY_INTERNAL:
    PSY_GRANT_INTERNAL_ACCESS(BinderTester);
    PSY_GRANT_INTERNAL_ACCESS(Compilation);

    Binder(SemanticModel* semaModel, const SyntaxTree* tree);
    Binder(const Binder&) = delete;
    void operator=(const Binder&) = delete;

    void bind();

private:
    SemanticModel* semaModel_;

    std::stack<Scope*> scopes_;
    Scope* stashedScope_;
    void pushNewScope(const SyntaxNode* node, ScopeKind scopeK, bool encloseInOuterScope);
    void pushStashedScope();
    void popScope();
    void popAndStashScope();

    using SymContT = std::stack<Symbol*>;
    SymContT syms_;
    void pushSymbol(Symbol*);
    Symbol* popSymbol();
    Declaration* popSymbolAsDeclaration();
    template <class SymT, class... SymTArgs> SymT* bindAndPushSymbol(
            const SyntaxNode* node,
            SymTArgs... arg);
    void bindObjectOrFunctionAndPushSymbol(const SyntaxNode* node);
    void nameDeclarationAtTop(const Identifier* name);
    void typeDeclarationAtTopWithTypeAtTop();

    using TyContT = std::stack<Type*>;
    TyContT tys_;
    std::stack<FunctionType*> pendingFunTys_;
    void pushType(Type*);
    Type* popType();
    void popTypesUntilNonDerivedDeclaratorType();
    template <class TyT, class... TyTArgs> TyT* makeType(TyTArgs... args);

    struct BD
    {
        std::uint32_t inTydefDecltor_: 1;
        std::uint32_t inImplicitIntTySpec_: 1;
        std::uint32_t inImplicitDoubleTySpec_: 1;
        std::uint32_t inExplicitSignedOrUnsignedTySpec_: 1;
    };
    union {
        std::uint32_t BD_;
        BD F_;
    };

    // Common definitions <stddef.h>
    const Identifier* ptrdiff_t_;
    const Identifier* size_t_;
    const Identifier* max_align_t_;
    const Identifier* wchar_t_;
    // Unicode utilities <uchar.h>
    const Identifier* char16_t_;
    const Identifier* char32_t_;

    struct DiagnosticsReporter
    {
        DiagnosticsReporter(Binder* binder)
            : binder_(binder)
        {}
        Binder* binder_;

        void diagnose(DiagnosticDescriptor&& desc, SyntaxToken tk);

        /* Declarations */
        void UselessDeclaration(SyntaxToken declTk);
        static const std::string ID_of_UselessDeclaration;

        /* Declarators */
        void FunctionReturningFunction(SyntaxToken decltorTk);
        void FunctionReturningArray(SyntaxToken decltorTk);
        static const std::string ID_FunctionReturningFunction;
        static const std::string ID_FunctionReturningArray;

        /* Type specifiers */
        void TypeSpecifierMissingDefaultsToInt(SyntaxToken declTk);
        void InvalidType(SyntaxToken tySpecTk);
        static const std::string ID_TypeSpecifierMissingDefaultsToInt;
        static const std::string ID_InvalidType;

        /* Type qualifiers */
        void InvalidUseOfRestrict(SyntaxToken tyQualTk);
        static const std::string ID_InvalidUseOfRestrict;
    };
    DiagnosticsReporter diagReporter_;

    const Identifier* identifier(const SyntaxToken& tk) const;

    //--------------//
    // Declarations //
    //--------------//
    virtual Action visitTranslationUnit(const TranslationUnitSyntax*) override;
    virtual Action visitIncompleteDeclaration(const IncompleteDeclarationSyntax*) override;
    virtual Action visitStaticAssertDeclaration(const StaticAssertDeclarationSyntax*) override;

    template <class TyDeclT> Action visitTagDeclaration_AtInternalDeclarations_COMMON(
            const TyDeclT* node,
            Action (Binder::*visit_AtEnd)(const TyDeclT*));

    virtual Action visitStructOrUnionDeclaration(const StructOrUnionDeclarationSyntax*) override;
    Action visitStructOrUnionDeclaration_AtSpecifier(const StructOrUnionDeclarationSyntax*);
    Action visitStructOrUnionDeclaration_AtEnd(const StructOrUnionDeclarationSyntax*);

    virtual Action visitEnumDeclaration(const EnumDeclarationSyntax*) override;
    Action visitEnumDeclaration_AtSpecifier(const EnumDeclarationSyntax*);
    Action visitEnumDeclaration_AtEnd(const EnumDeclarationSyntax*);

    virtual Action visitTypedefDeclaration(const TypedefDeclarationSyntax*) override;
    Action visitTypedefDeclaration_AtSpecifier(const TypedefDeclarationSyntax*);
    Action visitTypedefDeclaration_AtDeclarators(const TypedefDeclarationSyntax*);
    Action visitTypedefDeclaration_AtEnd(const TypedefDeclarationSyntax*);

    virtual Action visitVariableAndOrFunctionDeclaration(const VariableAndOrFunctionDeclarationSyntax*) override;
    Action visitVariableAndOrFunctionDeclaration_AtSpecifiers(const VariableAndOrFunctionDeclarationSyntax*);
    Action visitVariableAndOrFunctionDeclaration_AtDeclarators(const VariableAndOrFunctionDeclarationSyntax*);
    Action visitVariableAndOrFunctionDeclaration_AtEnd(const VariableAndOrFunctionDeclarationSyntax*);

    virtual Action visitFieldDeclaration(const FieldDeclarationSyntax*) override;
    Action visitFieldDeclaration_AtSpecifiers(const FieldDeclarationSyntax*);
    Action visitFieldDeclaration_AtDeclarators(const FieldDeclarationSyntax*);
    Action visitFieldDeclaration_AtEnd(const FieldDeclarationSyntax*);

    virtual Action visitEnumeratorDeclaration(const EnumeratorDeclarationSyntax*) override;
    Action visitEnumeratorDeclaration_AtImplicitSpecifier(const EnumeratorDeclarationSyntax*);
    Action visitEnumeratorDeclaration_AtDeclarator(const EnumeratorDeclarationSyntax*);
    Action visitEnumeratorDeclaration_AtEnd(const EnumeratorDeclarationSyntax*);

    virtual Action visitParameterDeclaration(const ParameterDeclarationSyntax*) override;
    Action visitParameterDeclaration_AtSpecifiers(const ParameterDeclarationSyntax*);
    Action visitParameterDeclaration_AtDeclarator(const ParameterDeclarationSyntax*);
    Action visitParameterDeclaration_AtEnd(const ParameterDeclarationSyntax*);

    virtual Action visitFunctionDefinition(const FunctionDefinitionSyntax*) override;
    Action visitFunctionDefinition_AtSpecifiers(const FunctionDefinitionSyntax*);
    Action visitFunctionDefinition_AtDeclarator(const FunctionDefinitionSyntax*);
    Action visitFunctionDefinition_AtEnd(const FunctionDefinitionSyntax*);

    /* Specifiers */
    virtual Action visitBasicTypeSpecifier(const BasicTypeSpecifierSyntax*) override;
    virtual Action visitVoidTypeSpecifier(const VoidTypeSpecifierSyntax*) override;
    virtual Action visitTagTypeSpecifier(const TagTypeSpecifierSyntax*) override;
    virtual Action visitTagDeclarationAsSpecifier(const TagDeclarationAsSpecifierSyntax*) override;
    virtual Action visitTypedefName(const TypedefNameSyntax*) override;
    virtual Action visitTypeQualifier(const TypeQualifierSyntax*) override;
    virtual Action visitExtGNU_Attribute(const ExtGNU_AttributeSyntax*) override;
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
    virtual Action visitBitfieldDeclarator(const BitfieldDeclaratorSyntax*) override;

    //------------//
    // Statements //
    //------------//
    virtual Action visitCompoundStatement(const CompoundStatementSyntax*) override;

    //--------//
    // Common //
    //--------//
    virtual Action visitTypeName(const TypeNameSyntax*) override;
    Action visitTypeName_AtSpecifier(const TypeNameSyntax*);
    Action visitTypeName_AtDeclarator(const TypeNameSyntax*);
    Action visitTypeName_AtEnd(const TypeNameSyntax*);

    template <class NodeT>
        Action visit_AtSpecifiers_COMMON(
            const NodeT* node,
            Action (Binder::*visit_AtDeclarators)(const NodeT*));
    template <class NodeT>
        Action visit_AtMultipleDeclarators_COMMON(
            const NodeT* node,
            Action (Binder::*visit_AtEnd)(const NodeT*));
    template <class NodeT>
        Action visit_AtSingleDeclarator_COMMON(
            const NodeT* node,
            Action (Binder::*visit_AtEnd)(const NodeT*));
    template <class NodeT>
        Action visit_AtEnd_COMMON(const NodeT*);
};

template <class SymT, class... SymTArgs>
SymT* Binder::bindAndPushSymbol(const SyntaxNode* node, SymTArgs... args)
{
    std::unique_ptr<SymT> sym(new SymT(syms_.top(),
                                       tree_,
                                       scopes_.top(),
                                       std::forward<SymTArgs>(args)...));
    auto rawSym = static_cast<SymT*>(semaModel_->mapAndKeepDeclaration(node, std::move(sym)));
    pushSymbol(rawSym);
    return rawSym;
}

template <class TyT, class... TyTArgs>
TyT* Binder::makeType(TyTArgs... args)
{
    std::unique_ptr<TyT> ty(new TyT(std::forward<TyTArgs>(args)...));
    return static_cast<TyT*>(semaModel_->keepType(std::move(ty)));
}

} // C
} // psy

#endif
