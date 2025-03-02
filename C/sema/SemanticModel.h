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
// OUT OF OR IN CONNECTION for the SOFTWARE OR THE USE OR OTHER DEALINGS IN
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

class DeclarationBinder;

/**
 * \brief The SemanticModel class.
 *
 * \note Resembles:
 * \c Microsoft.CodeAnalysis.SemanticModel from Roslyn.
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
     * The TranslationUnitSymbol of \c this SemanticModel.
     */
    const TranslationUnitSymbol* translationUnit() const;

    /**
     * The FunctionDeclarationSymbol declared for the given FunctionDefinitionSyntax \c node.
     *
     * \note Similar to:
     * - \c Microsoft.CodeAnalysis.CSharp.CSharpExtensions.GetDeclaredSymbol of Roslyn.
     */
    const FunctionDeclarationSymbol* functionFor(const FunctionDefinitionSyntax* node) const;

    /**
     * The ParameterDeclarationSymbol declared for the given ParameterDeclarationSyntax \c node.
     *
     * \note Similar to:
     * - \c Microsoft.CodeAnalysis.CSharp.CSharpExtensions.GetDeclaredSymbol of Roslyn.
     */
    const ParameterDeclarationSymbol* parameterFor(const ParameterDeclarationSyntax* node) const;

    /**
     * The StructDeclarationSymbol declared for the given StructOrUnionDeclarationSyntax \c node.
     *
     * \note Similar to:
     * - \c Microsoft.CodeAnalysis.CSharp.CSharpExtensions.GetDeclaredSymbol of Roslyn.
     */
    const StructDeclarationSymbol* structFor(const StructOrUnionDeclarationSyntax* node) const;

    /**
     * The UnionDeclarationSymbol declared for the given StructOrUnionDeclarationSyntax \c node.
     *
     * \note Similar to:
     * - \c Microsoft.CodeAnalysis.CSharp.CSharpExtensions.GetDeclaredSymbol of Roslyn.
     */
    const UnionDeclarationSymbol* unionFor(const StructOrUnionDeclarationSyntax* node) const;

    /**
     * The EnumDeclarationSymbol declared for the given EnumDeclarationSyntax \c node.
     *
     * \note Similar to:
     * - \c Microsoft.CodeAnalysis.CSharp.CSharpExtensions.GetDeclaredSymbol of Roslyn.
     */
    const EnumDeclarationSymbol* enumFor(const EnumDeclarationSyntax* node) const;

    /**
     * The TypeDeclarationSymbol declared for the given TypeDeclarationSyntax \c node.
     *
     * \note Similar to:
     * - \c Microsoft.CodeAnalysis.CSharp.CSharpExtensions.GetDeclaredSymbol of Roslyn.
     */
    const TypeDeclarationSymbol* typeDeclarationFor(const TypeDeclarationSyntax* node) const;

    /**
     * The EnumeratorDeclarationSymbol declared for the given EnumeratorDeclarationSyntax \c node.
     *
     * \note Similar to:
     * - \c Microsoft.CodeAnalysis.CSharp.CSharpExtensions.GetDeclaredSymbol of Roslyn.
     */
    const EnumeratorDeclarationSymbol* enumeratorFor(const EnumeratorDeclarationSyntax* node) const;

    /**
     * The FieldDeclarationSymbol(s) declared for the given FieldDeclarationSyntax \c node.
     *
     * \note Similar to:
     * - \c Microsoft.CodeAnalysis.CSharp.CSharpExtensions.GetDeclaredSymbol of Roslyn.
     */
    std::vector<const FieldDeclarationSymbol*> fieldsFor(const FieldDeclarationSyntax* node) const;

    /**
     * The DeclarationSymbol(s) declared for the given VariableAndOrFunctionDeclarationSyntax \c node.
     *
     * \note Similar to:
     * - \c Microsoft.CodeAnalysis.CSharp.CSharpExtensions.GetDeclaredSymbol of Roslyn.
     */
    std::vector<const DeclarationSymbol*>
         variablesAndOrFunctionsFor(const VariableAndOrFunctionDeclarationSyntax* node) const;

    /**
     * The DeclarationSymbol declared by the given DeclaratorSyntax \c node.
     */
    const DeclarationSymbol* declarationBy(const DeclaratorSyntax* node) const;
    //!@}

    /**
     * Search \c this SemanticModel for a DeclarationSymbol with the given predicate.
     */
    const DeclarationSymbol*
        searchForDeclaration(std::function<bool (const DeclarationSymbol*)> pred) const;

    /**
     * The Scope of the given \c node.
     */
    const Scope* scopeOf(const IdentifierNameSyntax* node) const;

    /**
     * The \a declaration of \c ptrdiff_t, if one exists.
     */
    const TypedefDeclarationSymbol* ptrdiff_t_typedef() const;

    /**
     * The \a declaration of \c size_t, if one exists.
     */
    const TypedefDeclarationSymbol* size_t_typedef() const;

    /**
     * The \a declaration of \c max_align_t, if one exists.
     */
    const TypedefDeclarationSymbol* max_align_t_typedef() const;

    /**
     * The \a declaration of \c wchar_t, if one exists.
     */
    const TypedefDeclarationSymbol* wchar_t_typedef() const;

    /**
     * The \a declaration of \c char16_t, if one exists.
     */
    const TypedefDeclarationSymbol* char16_t_typedef() const;

    /**
     * The \a declaration of \c char32_t, if one exists.
     */
    const TypedefDeclarationSymbol* char32_t_typedef() const;

PSY_INTERNAL:
    PSY_GRANT_INTERNAL_ACCESS(Compilation);
    PSY_GRANT_INTERNAL_ACCESS(DeclarationBinder);
    PSY_GRANT_INTERNAL_ACCESS(TypeResolver);
    PSY_GRANT_INTERNAL_ACCESS(TypeChecker);
    PSY_GRANT_INTERNAL_ACCESS(InternalsTestSuite);

    SemanticModel(const SyntaxTree* tree, Compilation* compilation);

    TranslationUnitSymbol* setTranslationUnit(std::unique_ptr<TranslationUnitSymbol> unit);

    DeclarationSymbol* addDeclaration(const SyntaxNode* node, std::unique_ptr<DeclarationSymbol> decl);

    DeclarationSymbol* declarationBy(const DeclaratorSyntax* node);
    FunctionDeclarationSymbol* functionFor(const FunctionDefinitionSyntax* node);
    ParameterDeclarationSymbol* parameterFor(const ParameterDeclarationSyntax* node);
    TypeDeclarationSymbol* typeDeclarationFor(const TypeDeclarationSyntax* node);
    EnumeratorDeclarationSymbol* enumeratorFor(const EnumeratorDeclarationSyntax* node);
    std::vector<FieldDeclarationSymbol*>
        fieldsFor(const FieldDeclarationSyntax* node);
    template <class VecT> VecT
        fieldsFor_CORE(const FieldDeclarationSyntax* node, VecT&& decls);
    std::vector<DeclarationSymbol*>
        variableAndOrFunctionsFor(const VariableAndOrFunctionDeclarationSyntax* node);
    template <class VecT> VecT
        variablesAndOrFunctionsFor_CORE(
            const VariableAndOrFunctionDeclarationSyntax* node,
            VecT&& decls);

    Scope* keepScope(std::unique_ptr<Scope> scope);
    void setScopeOf(const IdentifierNameSyntax* node, const Scope* scope);

    Type* keepType(std::unique_ptr<Type> ty);
    void dropType(const Type* ty);

    void set_ptrdiff_t_typedef(const TypedefDeclarationSymbol*);
    void set_size_t_typedef(const TypedefDeclarationSymbol*);
    void set_max_align_t_typedef(const TypedefDeclarationSymbol*);
    void set_wchar_t_typedef(const TypedefDeclarationSymbol*);
    void set_char16_t_typedef(const TypedefDeclarationSymbol*);
    void set_char32_t_typedef(const TypedefDeclarationSymbol*);

private:
    SemanticModel(const SemanticModel&) = delete;
    SemanticModel& operator=(const SemanticModel&) = delete;
    DECL_PIMPL(SemanticModel)
};

} // C
} // psy

#endif
