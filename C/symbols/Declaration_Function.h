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

#ifndef PSYCHE_C_FUNCTION_H__
#define PSYCHE_C_FUNCTION_H__

#include "Symbol_Declaration.h"
#include "MIXIN_NameableDeclarationSymbol.h"
#include "MIXIN_TypeableDeclarationSymbol.h"

namespace psy {
namespace C {

/**
 * \brief The FunctionDeclarationSymbol class.
 *
 * \note Resembles:
 * - \c Microsoft.CodeAnalysis.IMethodSymbol from Roslyn.
 */
class PSY_C_API FunctionDeclarationSymbol final
        : public DeclarationSymbol
        , public MIXIN_NameableDeclarationSymbol
        , public MIXIN_TypeableDeclarationSymbol
{
public:
    //!@{
    /**
     * Cast \c this Symbol as a FunctionDeclarationSymbol.
     */
    virtual FunctionDeclarationSymbol* asFunctionDeclaration() override { return this; }
    virtual const FunctionDeclarationSymbol* asFunctionDeclaration() const override { return this; }
    //!@}

    /**
     * The \a name of the function declared by \c this FunctionDeclarationSymbol.
     */
    const Identifier* name() const override;

    /**
     * The \a type of \c this FunctionDeclarationSymbol.
     *
     * \sa FunctionDeclarationSymbol::returnType
     */
    const Type* type() const override;

    /**
     * Whether \c this FunctionDeclarationSymbol is of a definition.
     */
    bool isDefinition() const;

PSY_INTERNAL:
    PSY_GRANT_INTERNAL_ACCESS(DeclarationBinder);
    PSY_GRANT_INTERNAL_ACCESS(TypeChecker);

    DECL_PIMPL_SUB(Function);
    FunctionDeclarationSymbol(const Symbol* containingSym,
                              const SyntaxTree* tree,
                              const Scope* enclosingScope);

    virtual const Identifier* denotingIdentifier() const override { return name(); }
    virtual void setName(const Identifier* name) override;
    virtual void setType(const Type* ty) override;
    Type* type();
    void setIsDefinition(bool isDef);
};

PSY_C_API std::ostream& operator<<(std::ostream& os, const FunctionDeclarationSymbol* func);

} // C
} // psy

#endif
