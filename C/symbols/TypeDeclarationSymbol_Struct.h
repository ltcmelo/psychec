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

#ifndef PSYCHE_C_STRUCT_TYPE_DECLARATION_SYMBOL_H__
#define PSYCHE_C_STRUCT_TYPE_DECLARATION_SYMBOL_H__

#include "DeclarationSymbol_Type.h"

namespace psy {
namespace C {

class PSY_C_API Struct final : public TypeDeclarationSymbol
{
public:
    //!@{
    /**
     * Cast \c this TypeDeclarationSymbol as a Struct.
     */
    virtual Struct* asStruct() override { return this; }
    virtual const Struct* asStruct() const override { return this; }
    //!@}

    /**
     * The Identifier with which \c this DeclarationSymbol is declared.
     */
    virtual const Identifier* identifier() const override;

    /**
     * Compute a displayable string for \c this Symbol.
     */
    virtual std::string toDisplayString() const override;

PSY_INTERNAL:
    PSY_GRANT_INTERNAL_ACCESS(Binder);

    Struct(const SyntaxTree* tree,
           const Symbol* containingSym,
           const Scope* scope,
           TagType* tagTy);
};

std::string PSY_C_API to_string(const Struct& strukt);

} // C
} // psy

#endif
