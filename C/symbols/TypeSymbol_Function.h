// Copyright (c) 2022 Leandro T. C. Melo <ltcmelo@gmail.com>
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

#ifndef PSYCHE_C_SYMBOL_FUNCTION_TYPE_H__
#define PSYCHE_C_SYMBOL_FUNCTION_TYPE_H__

#include "Symbol_Type.h"

#include <vector>

namespace psy {
namespace C {

/**
 * \brief The FunctionTypeSymbol class.
 *
 * \remark 6.2.5-20
 */
class PSY_C_API FunctionTypeSymbol final : public TypeSymbol
{
public:
    //!@{
    /**
     * Cast \c this TypeSymbol as a FunctionTypeSymbol.
     */
    virtual FunctionTypeSymbol* asFunctionType() { return this; }
    virtual const FunctionTypeSymbol* asFunctionType() const { return this; }
    //!@}

    /**
     * The return type of \c this FunctionTypeSymbol.
     */
    const TypeSymbol* returnType() const;

    /**
     * The parameter types of \c this FunctionTypeSymbol.
     */
    std::vector<const TypeSymbol*> parameterTypes() const;

PSY_INTERNAL_AND_RESTRICTED:
    PSY_GRANT_ACCESS(Binder);

    FunctionTypeSymbol(const SyntaxTree* tree,
                       const Scope* scope,
                       const Symbol* containingSym,
                       const TypeSymbol* retTySym);

    void addParameter(const TypeSymbol* parmTySym);

private:
    DECL_PIMPL_SUB(FunctionTypeSymbol)
};

std::string PSY_C_API to_string(const FunctionTypeSymbol& tySym);

} // C
} // psy

#endif
