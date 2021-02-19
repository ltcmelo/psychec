// Copyright (c) 2016/17/18/19/20/21 Leandro T. C. Melo <ltcmelo@gmail.com>
// Copyright (c) 2008 Roberto Raggi <roberto.raggi@gmail.com>
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

#ifndef PSYCHE_C_DECLARATION_NAMES_H__
#define PSYCHE_C_DECLARATION_NAMES_H__

#include "API.h"
#include "DeclarationName.h"

namespace psy {
namespace C {

/**
 * \brief The IdentifierName class.
 *
 * A name that consists of an \a identifier.
 */
class PSY_C_API IdentifierName final : public DeclarationName
{
    friend class SemanticModel;

public:
    /**
     * The \a identifier that composes \c this IdentifierName.
     */
    const Identifier* identifier() const;

    virtual const IdentifierName* asIdentifierName() const override { return this; }

private:
    IdentifierName(const Identifier* identifier);
    const Identifier* identifier_;
};

/**
 * \brief The TagName class.
 *
 * \remark 6.7.2.3, 6.7.2.3-7, and 6.7.2.3-9.
 */
class PSY_C_API TagName final : public DeclarationName
{
    friend class SemanticModel;

public:
    enum class TagKind : unsigned char
    {
        Struct,
        Union,
        Enum
    };

    /**
     * The TagKind of the DeclarationName tagged through \c this TagName.
     */
    TagKind taggedTypeKind() const;

    /**
     * The \a identifier that composes \c this TagName.
     */
    const Identifier* identifier() const;

    virtual const TagName* asTagName() const override  { return this; }

private:
    TagName(TagKind tagKind, const Identifier* identifier);
    TagKind tagKind_;
    const Identifier* identifier_;
};

/**
 * \brief The AnonymousName class.
 *
 * An absent name.
 *
 * \remark 6.7.2.1-13
 */
class PSY_C_API AnonymousName final : public DeclarationName
{
    friend class SemanticModel;

public:
    virtual const AnonymousName* asAnonymousName() const override { return this; }

private:
    AnonymousName();
};

} // C
} // psy

#endif
