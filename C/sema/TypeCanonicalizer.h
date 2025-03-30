// Copyright (c) 2024 Leandro T. C. Melo <ltcmelo@gmail.com>
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

#ifndef PSYCHE_C_TYPE_CANONICALIZER_H__
#define PSYCHE_C_TYPE_CANONICALIZER_H__

#include "API.h"
#include "Fwds.h"

#include "syntax/SyntaxVisitor.h"
#include "types/Type_Error.h"
#include "../common/diagnostics/DiagnosticDescriptor.h"
#include "../common/infra/AccessSpecifiers.h"

#include <stack>
#include <unordered_set>

namespace psy {
namespace C {

class PSY_C_INTERNAL_API TypeCanonicalizer final : protected SyntaxVisitor
{
PSY_INTERNAL:
    PSY_GRANT_INTERNAL_ACCESS(Compilation);
    PSY_GRANT_INTERNAL_ACCESS(TypeCanonicalizerAndResolverTester);

    TypeCanonicalizer(SemanticModel* semaModel, const SyntaxTree* tree);
    TypeCanonicalizer(const TypeCanonicalizer&) = delete;
    void operator=(const TypeCanonicalizer&) = delete;

    void canonicalizeTypes();

private:
    SemanticModel* semaModel_;
    const SpecifierSyntax* tySpecNode_;
    std::stack<const Symbol*> syms_;
    mutable std::unordered_set<const Type*> discardedTys_;

    struct DiagnosticsReporter
    {
        DiagnosticsReporter(TypeCanonicalizer* canonicalizer)
            : canonicalizer_(canonicalizer)
        {}
        TypeCanonicalizer* canonicalizer_;

        void diagnose(DiagnosticDescriptor&& desc, SyntaxToken tk);

        void TypeDeclarationNotFound(SyntaxToken tk);
        void ExpectedTypedefDeclaration(SyntaxToken tk);
        void TagTypeDoesNotMatchTagDeclaration(SyntaxToken tk);

        static const std::string ID_of_TypeDeclarationNotFound;
        static const std::string ID_of_ExpectedTypedefDeclaration;
        static const std::string ID_of_TagTypeDoesNotMatchTagDeclaration;
    };
    DiagnosticsReporter diagReporter_;

    const Type* canonicalize(const Type* ty, const Scope* scope);

    void canonicalizeAnonymousFields(FieldDeclarationSymbol* fldDecl);

    //--------------//
    // Declarations //
    //--------------//
    virtual Action visitTranslationUnit(const TranslationUnitSyntax*) override;
    virtual Action visitFunctionDefinition(const FunctionDefinitionSyntax*) override;
    virtual Action visitFieldDeclaration(const FieldDeclarationSyntax*) override;

    /* Specifiers */
    virtual Action visitTagTypeSpecifier(const TagTypeSpecifierSyntax*) override;
    virtual Action visitTypedefName(const TypedefNameSyntax*) override;

    /* Declarators */
    virtual Action visitPointerDeclarator(const PointerDeclaratorSyntax*) override;
    virtual Action visitParenthesizedDeclarator(const ParenthesizedDeclaratorSyntax*) override;
    virtual Action visitIdentifierDeclarator(const IdentifierDeclaratorSyntax*) override;
    Action visitDeclarator_COMMON(const DeclaratorSyntax*);
};

} // C
} // psy

#endif
