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

#ifndef PSYCHE_C_FUNCTION_DECLARATION_SYMBOL_H__
#define PSYCHE_C_FUNCTION_DECLARATION_SYMBOL_H__

#include "Symbol_Declaration.h"
#include "MIXIN_NameableSymbol.h"
#include "MIXIN_TypeableSymbol.h"

#include "../common/infra/AccessSpecifiers.h"

namespace psy {
namespace C {

/**
 * \brief The FunctionSymbol class.
 *
 * \note
 * This API is inspired by that of \c Microsoft.CodeAnalysis.IMethodSymbol
 * from Roslyn, the .NET Compiler Platform.
 */
class PSY_C_API Function final : public DeclarationSymbol
                               , public MIXIN_NameableSymbol
                               , public MIXIN_TypeableSymbol
{
public:
    //!@{
    /**
     * Cast \c this Symbol as a FunctionSymbol.
     */
    virtual Function* asFunction() override { return this; }
    virtual const Function* asFunction() const override { return this; }
    //!@}

    /**
     * The Identifier that names \c this FunctionSymbol.
     */
    const Identifier* name() const;

    /**
     * The Type of \c this FunctionSymbol.
     *
     * \sa FunctionSymbol::returnType
     */
    const Type* type() const;

    /**
     * The Type of the return of \c this FunctionSymbol.
     *
     * \sa FunctionSymbol::type
     */
    const Type* returnType() const;

    /**
     * Compute a displayable string for \c this Symbol.
     */
    virtual std::string toDisplayString() const override;

PSY_INTERNAL:
    PSY_GRANT_INTERNAL_ACCESS(Binder);

    Function(const SyntaxTree* tree,
             const Scope* scope,
             const Symbol* containingSym);

    virtual void setName(const Identifier* name) override;
    virtual void setType(const Type* ty) override;

protected:
    DECL_PIMPL_SUB(FunctionSymbol);
};

std::string PSY_C_API to_string(const Function& sym);

} // C
} // psy

#endif
