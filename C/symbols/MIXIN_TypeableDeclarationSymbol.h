// Copyright (c) 2022 Leandro T. C. Melo <ltcmelo@gmail.com>
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

#ifndef PSYCHE_C_TYPEABLE_DECLARATION_SYMBOL_H__
#define PSYCHE_C_TYPEABLE_DECLARATION_SYMBOL_H__

#include "API.h"
#include "Fwds.h"

#include "../common/infra/AccessSpecifiers.h"

#include <memory>

namespace psy {
namespace C {

class PSY_C_INTERNAL_API MIXIN_TypeableDeclarationSymbol
{
public:
    virtual ~MIXIN_TypeableDeclarationSymbol() {}

    static MIXIN_TypeableDeclarationSymbol* from(DeclarationSymbol*);
    static const MIXIN_TypeableDeclarationSymbol* from(const DeclarationSymbol*);

    virtual const Type* type() const = 0;

PSY_INTERNAL:
    PSY_GRANT_INTERNAL_ACCESS(DeclarationBinder);
    PSY_GRANT_INTERNAL_ACCESS(TypeResolver);

    virtual void setType(const Type* ty) = 0;
};

} // C
} // psy

#endif
