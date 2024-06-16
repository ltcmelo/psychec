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

#ifndef PSYCHE_C_SEMANTIC_MODEL_H__
#define PSYCHE_C_SEMANTIC_MODEL_H__

#include "API.h"
#include "Fwds.h"

#include "../common/infra/AccessSpecifiers.h"
#include "../common/infra/Pimpl.h"

#include <vector>
#include <functional>

namespace psy {
namespace C {

class Binder;

/**
 * \brief The SemanticModel class.
 *
 * \note
 * This API is inspired by that of \c Microsoft.CodeAnalysis.SemanticModel
 * from Roslyn, the .NET Compiler Platform.
 */
class PSY_C_API SemanticModel
{
public:
    ~SemanticModel();

    /**
     * The SyntaxTree from which \c this SemanticModel was computed.
     */
    const SyntaxTree* syntaxTree() const;

    /**
     * The Compilation from which \c this SemanticModel was computed.
     */
    const Compilation* compilation() const;

    /**
     * The TranslationUnit of \c this SemanticModel.
     */
    const TranslationUnit* translationUnit() const;

    /**
     * The Function declared by the given FunctionDefinitionSyntax \c node.
     *
     * \note Similar to:
     * - \c Microsoft.CodeAnalysis.CSharp.CSharpExtensions.GetDeclaredSymbol of Roslyn.
     */
    const Function* declarationOf(const FunctionDefinitionSyntax* node) const;

    /**
     * The Parameter declared by the given ParameterDeclarationSyntax \c node.
     *
     * \note Similar to:
     * - \c Microsoft.CodeAnalysis.CSharp.CSharpExtensions.GetDeclaredSymbol of Roslyn.
     */
    const Parameter* declarationOf(const ParameterDeclarationSyntax* node) const;

    /**
     * The TypeDeclarationSymbol declared by the given TypeDeclarationSyntax \c node.
     *
     * \note Similar to:
     * - \c Microsoft.CodeAnalysis.CSharp.CSharpExtensions.GetDeclaredSymbol of Roslyn.
     */
    const TypeDeclarationSymbol* declarationOf(const TypeDeclarationSyntax* node) const;

    /**
     * The Enumerator declared by the given EnumeratorDeclarationSyntax \c node.
     *
     * \note Similar to:
     * - \c Microsoft.CodeAnalysis.CSharp.CSharpExtensions.GetDeclaredSymbol of Roslyn.
     */
    const Enumerator* declarationOf(const EnumeratorDeclarationSyntax* node) const;

    /**
     * The Field(s) declared by the given FieldDeclarationSyntax \c node.
     *
     * \note Similar to:
     * - \c Microsoft.CodeAnalysis.CSharp.CSharpExtensions.GetDeclaredSymbol of Roslyn.
     */
    std::vector<const Field*> declarationsOf(const FieldDeclarationSyntax* node) const;

    /**
     * The DeclarationSymbol(s) declared by the given VariableAndOrFunctionDeclarationSyntax \c node.
     *
     * \note Similar to:
     * - \c Microsoft.CodeAnalysis.CSharp.CSharpExtensions.GetDeclaredSymbol of Roslyn.
     */
    std::vector<const DeclarationSymbol*> declarationsOf(const VariableAndOrFunctionDeclarationSyntax* node) const;

    /**
     * The DeclarationSymbol declared by the given DeclaratorSyntax \c node.
     *
     * \note Similar to:
     * - \c Microsoft.CodeAnalysis.CSharp.CSharpExtensions.GetDeclaredSymbol of Roslyn.
     */
    const DeclarationSymbol* declarationOf(const DeclaratorSyntax* node) const;
    //!@}

    PSY_INTERNAL:
    PSY_GRANT_INTERNAL_ACCESS(Binder);
    PSY_GRANT_INTERNAL_ACCESS(Compilation);
    PSY_GRANT_INTERNAL_ACCESS(InternalsTestSuite);

    SemanticModel(const SyntaxTree* tree, Compilation* compilation);

    TranslationUnit* keepTranslationUnit(
            const TranslationUnitSyntax* node,
            std::unique_ptr<TranslationUnit> unitSym);
    DeclarationSymbol* keepAndBindDecl(
            const SyntaxNode* node,
            std::unique_ptr<DeclarationSymbol> sym);
    Type* keepType(std::unique_ptr<Type> ty);

    DeclarationSymbol* searchForDecl(
            std::function<bool (const std::unique_ptr<DeclarationSymbol>&)> pred) const;

    template <class CastT, class OrigT> const CastT* castDecl(
            const OrigT* origDecl,
            const CastT* (OrigT::*cast)() const) const;

private:
    SemanticModel(const SemanticModel&) = delete;
    SemanticModel& operator=(const SemanticModel&) = delete;

    DECL_PIMPL(SemanticModel)
};

} // C
} // psy

#endif
