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

#include "common/diagnostics/DiagnosticDescriptor.h"
#include "parser/LexedTokens.h"
#include "syntax/SyntaxVisitor.h"

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

    Binder(SemanticModel* semaModel, const SyntaxTree* tree);

    SemanticModel* semaModel_;
    std::stack<Scope*> scopes_;
    std::stack<Symbol*> syms_;

    template <class SymbolT> SymbolT* newSymbol_COMMON(std::unique_ptr<SymbolT>);
    template <class SymbolT> SymbolT* newSymbol();

    template <class ScopeT> void openScopeInSymbol();
    void openScopeInScope();
    void closeScope();

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
    virtual Action visitTranslationUnit(const TranslationUnitSyntax* node) override;
    virtual Action visitIncompleteDeclaration(const IncompleteDeclarationSyntax* node) override;
    SyntaxVisitor::Action common_visitTypeDeclaration(const TypeDeclarationSyntax* node);
    virtual Action visitStructOrUnionDeclaration(const StructOrUnionDeclarationSyntax* node) override;
    virtual Action visitEnumDeclaration(const EnumDeclarationSyntax* node) override;
    virtual Action visitVariableAndOrFunctionDeclaration(const VariableAndOrFunctionDeclarationSyntax* node) override;

    /* Declarators */
    virtual Action visitIdentifierDeclarator(const IdentifierDeclaratorSyntax*) override;

    //------------//
    // Statements //
    //------------//
    virtual Action visitCompoundStatement(const CompoundStatementSyntax* node) override;
};

} // C
} // psy

#endif
