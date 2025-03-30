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

#ifndef PSYCHE_C_TAG_TYPE_DECLARATION_H__
#define PSYCHE_C_TAG_TYPE_DECLARATION_H__

#include "Declaration_Type.h"
#include "TagTypeDeclarationCategory.h"

#include <optional>
#include <vector>

namespace psy {
namespace C {

/**
 * \brief The TagTypeDeclarationSymbol class.
 */
class PSY_C_API TagTypeDeclarationSymbol : public TypeDeclarationSymbol
{
public:
    //!@{
    /**
     * Cast \c this Symbol as a TagTypeDeclarationSymbol.
     */
    virtual TagTypeDeclarationSymbol* asTagTypeDeclaration() override { return this; }
    virtual const TagTypeDeclarationSymbol* asTagTypeDeclaration() const override { return this; }
    //!@}

    /**
     * The TypeDeclarationCategory of \c this TagTypeDeclarationSymbol.
     */
    TagTypeDeclarationCategory category() const;

    /**
     * The \a new \a type introduced by \c this TagTypeDeclarationSymbol.
     */
    const TagType* introducedNewType() const;

    /**
     * The \a member of \c this TagTypeDeclarationSymbol with the given \c name.
     */
    const MemberDeclarationSymbol* member(const Identifier* name) const;

    using Members = std::vector<const MemberDeclarationSymbol*>;

    /**
     * The \a members of \c this TagTypeDeclarationSymbol.
     */
    Members members() const;

PSY_INTERNAL:
    PSY_GRANT_INTERNAL_ACCESS(DeclarationBinder);

protected:
    TagTypeDeclarationSymbol(
            SymbolKind symK,
            const Symbol* containingSym,
            const SyntaxTree* tree,
            const Scope* enclosingScope,
            TagType* tagTy);

    virtual const Identifier* denotingIdentifier() const override;

    Members membDecls_;
    void addMember(const MemberDeclarationSymbol* memb);
};

PSY_C_API std::ostream& operator<<(std::ostream& os, const TagTypeDeclarationSymbol* tagTyDecl);

} // C
} // psy

#endif
