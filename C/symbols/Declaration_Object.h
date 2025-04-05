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

#include "Symbol_Declaration.h"
#include "MIXIN_NameableDeclarationSymbol.h"
#include "MIXIN_TypeableDeclarationSymbol.h"

#include "../common/infra/AccessSpecifiers.h"

#include <memory>

namespace psy {
namespace C {

/**
 * \brief The ObjectDeclarationSymbol class.
 */
class PSY_C_API ObjectDeclarationSymbol
        : public DeclarationSymbol
        , public MIXIN_NameableDeclarationSymbol
        , public MIXIN_TypeableDeclarationSymbol
{
public:
    virtual ~ObjectDeclarationSymbol();

    //!@{
    /**
     * Cast \c this Symbol as a ObjectDeclarationSymbol.
     */
    virtual ObjectDeclarationSymbol* asObjectDeclaration() override { return this; }
    virtual const ObjectDeclarationSymbol* asObjectDeclaration() const override { return this; }

    /**
     * The \a name of the \a object declared by \c this ObjectDeclarationSymbol.
     */
    const Identifier* name() const override;

    /**
     * The \a type of the \a object declared by \c this ObjectDeclarationSymbol.
     */
    const Type* type() const override;

protected:
    DECL_PIMPL_SUB(ObjectDeclaration);
    ObjectDeclarationSymbol(SymbolKind symK,
                      const Symbol* containingSym,
                      const SyntaxTree* tree,
                      const Scope* enclosingScope);

PSY_INTERNAL:
    PSY_GRANT_INTERNAL_ACCESS(DeclarationBinder);
    PSY_GRANT_INTERNAL_ACCESS(TypeCanonicalizer);

    virtual const Identifier* denotingIdentifier() const override { return name(); }
    virtual void setName(const Identifier* name) override;
    virtual void setType(const Type* ty) override;
};

PSY_C_API std::ostream& operator<<(std::ostream& os, const ObjectDeclarationSymbol* objDecl);

} // C
} // psy

#endif
