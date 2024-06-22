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

#ifndef PSYCHE_C_OBJECT_DECLARATION_SYMBOL_H__
#define PSYCHE_C_OBJECT_DECLARATION_SYMBOL_H__

#include "API.h"
#include "Fwds.h"

#include "MIXIN_NameableSymbol.h"
#include "MIXIN_TypeableSymbol.h"
#include "Symbol_Declaration.h"
#include "ObjectDeclarationSymbolKind.h"

#include "../common/infra/AccessSpecifiers.h"

#include <memory>

namespace psy {
namespace C {

/**
 * \brief The ObjectDeclarationSymbol class.
 */
class PSY_C_API ObjectDeclarationSymbol
        : public DeclarationSymbol
        , public MIXIN_NameableSymbol
        , public MIXIN_TypeableSymbol
{
public:
    virtual ~ObjectDeclarationSymbol();

    //!@{
    /**
     * Cast \c this Symbol as a ObjectDeclarationSymbol.
     */
    virtual ObjectDeclarationSymbol* asObjectDeclarationSymbol() override { return this; }
    virtual const ObjectDeclarationSymbol* asObjectDeclarationSymbol() const override { return this; }

    /**
     * The ObjectDeclarationSymbolKind of \c this ObjectDeclarationSymbol.
     */
    ObjectDeclarationSymbolKind kind() const;

    //!@{
    /**
     * Cast \c this ObjectDeclarationSymbol.
     */
    virtual Enumerator* asEnumerator() { return nullptr; }
    virtual const Enumerator* asEnumerator() const { return nullptr; }
    virtual Field* asField() { return nullptr; }
    virtual const Field* asField() const { return nullptr; }
    virtual Variable* asVariable() { return nullptr; }
    virtual const Variable* asVariable() const { return nullptr; }
    virtual Parameter* asParameter() { return nullptr; }
    virtual const Parameter* asParameter() const { return nullptr; }
    //!@}

    /**
     * The Identifier with which \c this DeclarationSymbol is declared.
     */
    virtual const Identifier* identifier() const override { return name(); }

    /**
     * The Identifier that names \c this ObjectDeclarationSymbol.
     */
    const Identifier* name() const;

    /**
     * The Type of \c this value.
     */
    const Type* type() const;

PSY_INTERNAL:
    PSY_GRANT_INTERNAL_ACCESS(Binder);

    virtual void setName(const Identifier* name) override;
    virtual void setType(const Type* ty) override;

protected:
    DECL_PIMPL_SUB(ObjectDeclarationSymbol);
    ObjectDeclarationSymbol(const SyntaxTree* tree,
                            const Symbol* containingSym,
                            const Scope* scope,
                            ObjectDeclarationSymbolKind valSymK);
};

std::string PSY_C_API to_string(const ObjectDeclarationSymbol& sym);

} // C
} // psy

#endif
