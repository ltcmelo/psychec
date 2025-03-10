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

#ifndef PSYCHE_C_TYPE_DECLARATION_H__
#define PSYCHE_C_TYPE_DECLARATION_H__

#include "API.h"
#include "Fwds.h"

#include "Symbol_Declaration.h"
#include "TypeDeclarationCategory.h"

#include "../common/infra/AccessSpecifiers.h"

#include <memory>

namespace psy {
namespace C {

/**
 * \brief The TypeDeclarationSymbol class.
 *
 * \note Resembles:
 * \c Microsoft.CodeAnalysis.ITypeDeclaration from Roslyn.
 *
 * \see Type
 */
class PSY_C_API TypeDeclarationSymbol : public DeclarationSymbol
{
public:
    virtual ~TypeDeclarationSymbol();

    /**
     * The TypeDeclarationCategory of \c this TypeDeclarationSymbol.
     */
    TypeDeclarationCategory category() const;

    //!@{
    /**
     * Cast \c this Symbol as a TypeDeclarationSymbol.
     */
    virtual TypeDeclarationSymbol* asTypeDeclaration() override { return this; }
    virtual const TypeDeclarationSymbol* asTypeDeclaration() const override { return this; }
    //!@}

    /**
     * The \a type introduced by \c this TypeDeclarationSymbol.
     *
     * \sa Typedef::synonymType
     * \sa TagTypeDeclaration::introducedNewType
     */
    const Type* introducedType() const;

protected:
    DECL_PIMPL_SUB(TypeDeclaration);
    TypeDeclarationSymbol(TypeDeclarationImpl* p);
    TypeDeclarationSymbol(const TypeDeclarationSymbol&) = delete;
    TypeDeclarationSymbol& operator=(const DeclarationSymbol&) = delete;

PSY_INTERNAL:
    PSY_GRANT_INTERNAL_ACCESS(DeclarationBinder);
};

PSY_C_API std::ostream& operator<<(std::ostream& os, const TypeDeclarationSymbol* tyDecl);

} // C
} // psy

#endif
