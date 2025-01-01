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

#ifndef PSYCHE_C_DECLARATION_H__
#define PSYCHE_C_DECLARATION_H__

#include "API.h"
#include "Fwds.h"

#include "Symbol.h"
#include "DeclarationKind.h"

#include "binder/NameSpace.h"

namespace psy {
namespace C {

class PSY_C_API Declaration : public Symbol
{
public:
    //!@{
    /**
     * Cast \c this Symbol as a Declaration.
     */
    virtual Declaration* asDeclaration() override { return this; }
    virtual const Declaration* asDeclaration() const override { return this; }
    //!@}

    /**
     * The DeclarationKind of \c this Symbol.
     */
    DeclarationKind kind() const;

    //!@{
    /**
     * Cast \c this Declaration.
     */
    virtual Function* asFunction() { return nullptr; }
    virtual const Function* asFunction() const { return nullptr; }
    virtual ObjectDeclaration* asObjectDeclaration() { return nullptr; }
    virtual const ObjectDeclaration* asObjectDeclaration() const { return nullptr; }
    virtual TypeDeclaration* asTypeDeclaration() { return nullptr; }
    virtual const TypeDeclaration* asTypeDeclaration() const { return nullptr; }
    //!@}

    /**
     * The Identifier with which \c this Declaration is declared.
     */
    virtual const Identifier* identifier() const = 0;

    /**
     * The Scope that encloses \c this Declaration.
     *
     * \remark 6.2.1-4
     */
    const Scope* enclosingScope() const;

    /**
     * The NameSpace of \c this Declaration.
     *
     * \remark 6.2.3
     */
    const NameSpace nameSpace() const;

    /**
     * References to the SyntaxNodes that \a declare \c this Declaration.
     */
    std::vector<SyntaxReference> declaringSyntaxReferences() const;

    /**
     * The Location of \c this Declaration.
     */
    Location location() const;

protected:
    DECL_PIMPL_SUB(Declaration);
    Declaration(DeclarationImpl* p);
    Declaration(const Declaration&) = delete;
    Declaration& operator=(const Declaration&) = delete;
};

PSY_C_API std::string to_string(const Declaration* decl);

} // C
} // psy

#endif
