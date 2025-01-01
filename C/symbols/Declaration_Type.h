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
#include "TypeDeclarationKind.h"

#include "../common/infra/AccessSpecifiers.h"

#include <memory>

namespace psy {
namespace C {

/**
 * \brief The TypeDeclaration class.
 *
 * \note
 * This API is inspired by that of \c Microsoft.CodeAnalysis.ITypeDeclaration
 * from Roslyn, the .NET Compiler Platform. Yet, Psyche-C distinguishes
 * between type symbols and types.
 *
 * \see Type
 */
class PSY_C_API TypeDeclaration : public Declaration
{
public:
    virtual ~TypeDeclaration();

    //!@{
    /**
     * Cast \c this Symbol as a TypeDeclaration.
     */
    virtual TypeDeclaration* asTypeDeclaration() override { return this; }
    virtual const TypeDeclaration* asTypeDeclaration() const override { return this; }
    //!@}

    /**
     * The TypeDeclarationKind of \c this TypeDeclaration.
     */
    TypeDeclarationKind kind() const;

    //!@{
    /**
     * Cast \c this TypeDeclaration.
     */
    virtual TagTypeDeclaration* asTagTypeDeclaration() { return nullptr; }
    virtual const TagTypeDeclaration* asTagTypeDeclaration() const { return nullptr; }
    virtual Typedef* asTypedef() { return nullptr; }
    virtual const Typedef* asTypedef() const { return nullptr; }
    //!@}

PSY_INTERNAL:
    PSY_GRANT_INTERNAL_ACCESS(Binder);

protected:
    DECL_PIMPL_SUB(TypeDeclaration);
    TypeDeclaration(TypeDeclarationImpl* p);
};

std::string PSY_C_API to_string(const TypeDeclaration* tyDecl);

} // C
} // psy

#endif
