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

#ifndef PSYCHE_C_DECLARATION_SYMBOL_H__
#define PSYCHE_C_DECLARATION_SYMBOL_H__

#include "API.h"
#include "Fwds.h"

#include "Symbol.h"
#include "DeclarationSymbolKind.h"

#include "binder/NameSpace.h"

namespace psy {
namespace C {

class PSY_C_API DeclarationSymbol : public Symbol
{
public:
    //!@{
    /**
     * Cast \c this Symbol as a TypeDeclarationSymbol.
     */
    virtual DeclarationSymbol* asDeclarationSymbol() override { return this; }
    virtual const DeclarationSymbol* asDeclarationSymbol() const override { return this; }
    //!@}

    /**
     * The DeclarationSymbolKind of \c this Symbol.
     */
    DeclarationSymbolKind kind() const;

    //!@{
    /**
     * Cast \c this DeclarationSymbol.
     */
    virtual Function* asFunction() { return nullptr; }
    virtual const Function* asFunction() const { return nullptr; }
    virtual ObjectDeclarationSymbol* asObjectDeclarationSymbol() { return nullptr; }
    virtual const ObjectDeclarationSymbol* asObjectDeclarationSymbol() const { return nullptr; }
    virtual TypeDeclarationSymbol* asTypeDeclarationSymbol() { return nullptr; }
    virtual const TypeDeclarationSymbol* asTypeDeclarationSymbol() const { return nullptr; }
    //!@}

    /**
     * The Identifier with which \c this DeclarationSymbol is declared.
     */
    virtual const Identifier* identifier() const = 0;

    /**
     * The Scope that encloses \c this DeclarationSymbol.
     *
     * \remark 6.2.1-4
     */
    const Scope* enclosingScope() const;

    /**
     * The NameSpace of \c this DeclarationSymbol.
     *
     * \remark 6.2.3
     */
    const NameSpace nameSpace() const;

    /**
     * References to the SyntaxNodes that \a declare \c this Symbol.
     */
    std::vector<SyntaxReference> declaringSyntaxReferences() const;

    /**
     * The Location of \c this Symbol.
     */
    Location location() const;

protected:
    DeclarationSymbol(SymbolImpl* p,
                      DeclarationSymbolKind declSymK);
    DeclarationSymbol(const DeclarationSymbol&) = delete;
    DeclarationSymbol& operator=(const DeclarationSymbol&) = delete;
};

std::string PSY_C_API to_string(const DeclarationSymbol& decl);

} // C
} // psy

#endif
