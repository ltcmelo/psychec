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

#ifndef PSYCHE_C_OBJECT_DECLARATION_H__
#define PSYCHE_C_OBJECT_DECLARATION_H__

#include "API.h"
#include "Fwds.h"

#include "MIXIN_NameableSymbol.h"
#include "MIXIN_TypeableSymbol.h"
#include "Symbol_Declaration.h"
#include "ObjectDeclarationKind.h"

#include "../common/infra/AccessSpecifiers.h"

#include <memory>

namespace psy {
namespace C {

/**
 * \brief The ObjectDeclaration class.
 */
class PSY_C_API ObjectDeclaration
        : public Declaration
        , public MIXIN_NameableSymbol
        , public MIXIN_TypeableSymbol
{
public:
    virtual ~ObjectDeclaration();

    //!@{
    /**
     * Cast \c this Declaration as a ObjectDeclaration.
     */
    virtual ObjectDeclaration* asObjectDeclaration() override { return this; }
    virtual const ObjectDeclaration* asObjectDeclaration() const override { return this; }

    /**
     * The ObjectDeclarationKind of \c this ObjectDeclaration.
     */
    ObjectDeclarationKind kind() const;

    //!@{
    /**
     * Cast \c this ObjectDeclaration.
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
     * The Identifier with which \c this ObjectDeclaration is declared.
     */
    virtual const Identifier* identifier() const override { return name(); }

    /**
     * The Identifier that names \c this ObjectDeclaration.
     */
    const Identifier* name() const;

    /**
     * The Type of \c this ObjectDeclaration.
     */
    const Type* type() const;

PSY_INTERNAL:
    PSY_GRANT_INTERNAL_ACCESS(Binder);
    PSY_GRANT_INTERNAL_ACCESS(TypeResolver);

    virtual void setName(const Identifier* name) override;
    virtual void setType(const Type* ty) override;
    virtual const Type* retypeableType() const override;

protected:
    DECL_PIMPL_SUB(ObjectDeclaration);
    ObjectDeclaration(const Symbol* containingSym,
                      const SyntaxTree* tree,
                      const Scope* enclosingScope,
                      ObjectDeclarationKind objDeclK);
};

std::string PSY_C_API to_string(const ObjectDeclaration& objDecl);

} // C
} // psy

#endif
