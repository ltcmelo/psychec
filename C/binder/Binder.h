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

#ifndef PSYCHE_C_BINDER_H__
#define PSYCHE_C_BINDER_H__

#include "API.h"
#include "APIFwds.h"

#include "parser/LexedTokens.h"
#include "symbols/SymbolName.h"
#include "symbols/Symbols.h"
#include "symbols/TypeKind.h"
#include "syntax/SyntaxVisitor.h"

#include "../common/diagnostics/DiagnosticDescriptor.h"

#include <memory>
#include <stack>
#include <string>

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
    friend class SemanticModel;
    friend class Semantics_TypeSpecifiers;

    Binder(SemanticModel* semaModel, const SyntaxTree* tree);

    SemanticModel* semaModel_;

    template <class ScopeT> void openScope();
    void openNestedScope();
    void closeScope();
    std::stack<Scope*> scopes_;

    template <class SymT> void makeAndPushSymDEF();
    template <class SymT> void pushSymDEF(std::unique_ptr<SymT>);
    void popSymDEF();
    std::stack<Symbol*> symDEFs_;

    void makeAndPushTySymDEF(TypeKind);
    template <class TySymT> void pushTySymUSE(std::unique_ptr<TySymT>);
    void popTySymUSE();
    std::stack<TypeSymbol*> tySymUSEs_;

    struct DiagnosticsReporter
    {
        DiagnosticsReporter(Binder* binder)
            : binder_(binder)
        {}

        Binder* binder_;

        void diagnose(DiagnosticDescriptor&& desc, SyntaxToken tk);

        static const std::string ID_of_UselessDeclaration;

        void UselessDeclaration(SyntaxToken tk);
    };

    DiagnosticsReporter diagReporter_;

    //--------------//
    // Declarations //
    //--------------//
    virtual Action visitTranslationUnit(const TranslationUnitSyntax*) override;
    virtual Action visitIncompleteDeclaration(const IncompleteDeclarationSyntax*) override;
    Action visitTypeDeclaration_COMMON(const TypeDeclarationSyntax*);
    virtual Action visitStructOrUnionDeclaration(const StructOrUnionDeclarationSyntax*) override;
    virtual Action visitEnumDeclaration(const EnumDeclarationSyntax*) override;
    virtual Action visitVariableAndOrFunctionDeclaration(const VariableAndOrFunctionDeclarationSyntax*) override;
    virtual Action visitFieldDeclaration(const FieldDeclarationSyntax*) override;
    virtual Action visitParameterDeclaration(const ParameterDeclarationSyntax*) override;
    virtual Action visitStaticAssertDeclaration(const StaticAssertDeclarationSyntax*) override;
    virtual Action visitFunctionDefinition(const FunctionDefinitionSyntax*) override;

    /* Specifiers */
    virtual Action visitBuiltinTypeSpecifier(const BuiltinTypeSpecifierSyntax*) override;
    virtual Action visitTagTypeSpecifier(const TagTypeSpecifierSyntax*) override;
    virtual Action visitTypeDeclarationAsSpecifier(const TypeDeclarationAsSpecifierSyntax*) override;
    virtual Action visitTypedefName(const TypedefNameSyntax*) override;
    virtual Action visitTypeQualifier(const TypeQualifierSyntax*) override;

    /* Declarators */
    virtual Action visitArrayOrFunctionDeclarator(const ArrayOrFunctionDeclaratorSyntax*) override;
    virtual Action visitPointerDeclarator(const PointerDeclaratorSyntax*) override;
    //virtual Action visitParenthesizedDeclarator(const ParenthesizedDeclaratorSyntax*) override;
    virtual Action visitIdentifierDeclarator(const IdentifierDeclaratorSyntax*) override;
    virtual Action visitAbstractDeclarator(const AbstractDeclaratorSyntax*) override;

    //------------//
    // Statements //
    //------------//
    virtual Action visitCompoundStatement(const CompoundStatementSyntax*) override;
    virtual Action visitDeclarationStatement(const DeclarationStatementSyntax*) override;
};

} // C
} // psy

#endif
