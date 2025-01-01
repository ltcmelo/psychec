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
#include "TagTypeDeclarationKind.h"

namespace psy {
namespace C {

class PSY_C_API TagTypeDeclaration : public TypeDeclaration
{
public:
    //!@{
    /**
     * Cast \c this TypeDeclaration as a TagTypeDeclaration.
     */
    virtual TagTypeDeclaration* asTagTypeDeclaration() override { return this; }
    virtual const TagTypeDeclaration* asTagTypeDeclaration() const override { return this; }
    //!@}

    /**
     * The TypeDeclarationKind of \c this TagTypeDeclaration.
     */
    TagTypeDeclarationKind kind() const;

    //!@{
    /**
     * Cast \c this TagTypeDeclaration.
     */
    virtual Struct* asStruct() { return nullptr; }
    virtual const Struct* asStruct() const { return nullptr; }
    virtual Union* asUnion() { return nullptr; }
    virtual const Union* asUnion() const { return nullptr; }
    virtual Enum* asEnum() { return nullptr; }
    virtual const Enum* asEnum() const { return nullptr; }
    //!@}

    /**
     * The Identifier with which \c this TagTypeDeclaration is declared.
     */
    virtual const Identifier* identifier() const override;

    /**
     * The TagType specified by \c this TagTypeDeclaration.
     */
    const TagType* specifiedType() const;

PSY_INTERNAL:
    PSY_GRANT_INTERNAL_ACCESS(Binder);

protected:
    TagTypeDeclaration(const Symbol* containingSym,
                       const SyntaxTree* tree,
                       const Scope* enclosingScope,
                       TagType* tagTy,
                       TagTypeDeclarationKind tagTyDeclK);
};

std::string PSY_C_API to_string(const TagTypeDeclaration* tagTyDecl);

} // C
} // psy

#endif
