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

#ifndef PSYCHE_C_SYMBOL_FUNCTION_H__
#define PSYCHE_C_SYMBOL_FUNCTION_H__

#include "Symbol.h"
#include "TypeClass_NameableSymbol.h"
#include "TypeClass_TypeableSymbol.h"

#include "../common/infra/InternalAccess.h"

namespace psy {
namespace C {

/**
 * \brief The FunctionSymbol class.
 *
 * \note
 * This API is inspired by that of \c Microsoft.CodeAnalysis.IMethodSymbol
 * from Roslyn, the .NET Compiler Platform.
 */
class PSY_C_API FunctionSymbol final : public Symbol
                                     , public TypeClass_NameableSymbol
                                     , public TypeClass_TypeableSymbol
{
public:
    //!@{
    /**
     * Cast \c this Symbol as a FunctionSymbol.
     */
    virtual FunctionSymbol* asFunction() override { return this; }
    virtual const FunctionSymbol* asFunction() const override { return this; }
    //!@}

    /**
     * The SymbolName of \c this Symbol.
     */
    const SymbolName* name() const;

    /**
     * The type of \c this FunctionSymbol.
     *
     * \sa FunctionSymbol::returnType
     */
    const TypeSymbol* type() const;

    /**
     * The return type of \c this FunctionSymbol.
     *
     * \sa FunctionSymbol::type
     */
    const TypeSymbol* returnType() const;

PSY_INTERNAL_AND_RESTRICTED:
    PSY_GRANT_ACCESS(Binder);

    FunctionSymbol(const SyntaxTree* tree,
                   const Scope* scope,
                   const Symbol* containingSym);

    virtual void setName(std::unique_ptr<SymbolName> symName) override;
    virtual void setType(const TypeSymbol* tySym) override;

protected:
    DECL_PIMPL_SUB(FunctionSymbol);
};

std::string PSY_C_API to_string(const FunctionSymbol& sym);

} // C
} // psy

#endif
