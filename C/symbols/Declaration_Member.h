// Copyright (c) 2025 Leandro T. C. Melo <ltcmelo@gmail.com>
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

#ifndef PSYCHE_C_MEMBER_DECLARATION_H__
#define PSYCHE_C_MEMBER_DECLARATION_H__

#include "Symbol_Declaration.h"
#include "MemberDeclarationCategory.h"
#include "MIXIN_NameableDeclarationSymbol.h"
#include "MIXIN_TypeableDeclarationSymbol.h"

#include "../common/infra/AccessSpecifiers.h"

#include <memory>

namespace psy {
namespace C {

/**
 * \brief The MemberDeclarationSymbol class.
 */
class PSY_C_API MemberDeclarationSymbol
        : public DeclarationSymbol
        , public MIXIN_NameableDeclarationSymbol
        , public MIXIN_TypeableDeclarationSymbol
{
public:
    virtual ~MemberDeclarationSymbol();

    //!@{
    /**
     * Cast \c this Symbol as a MemberDeclarationSymbol.
     */
    virtual MemberDeclarationSymbol* asMemberDeclaration() override { return this; }
    virtual const MemberDeclarationSymbol* asMemberDeclaration() const override { return this; }

    /**
     * The MemberDeclarationCategory of \c this MemberDeclarationSymbol.
     */
    MemberDeclarationCategory category() const;

    /**
     * The \a name of the \a Member declared by \c this MemberDeclarationSymbol.
     */
    const Identifier* name() const override;

    /**
     * The \a type of the \a Member declared by \c this MemberDeclarationSymbol.
     */
    const Type* type() const override;

protected:
    DECL_PIMPL_SUB(MemberDeclaration);
    MemberDeclarationSymbol(SymbolKind symK,
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

PSY_C_API std::ostream& operator<<(std::ostream& os, const MemberDeclarationSymbol* membDecl);

} // C
} // psy

#endif
