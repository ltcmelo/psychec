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

#ifndef PSYCHE_C_UNION_H__
#define PSYCHE_C_UNION_H__

#include "TypeDeclaration_Tag.h"

namespace psy {
namespace C {

class PSY_C_API Union final : public TagTypeDeclaration
{
public:
    //!@{
    /**
     * Cast \c this TagTypeDeclaration as a Union.
     */
    virtual Union* asUnion() override { return this; }
    virtual const Union* asUnion() const override { return this; }
    //!@}

    /**
     * Compute a displayable string for \c this Union.
     */
    virtual std::string toDisplayString() const override;

PSY_INTERNAL:
    PSY_GRANT_INTERNAL_ACCESS(Binder);

    Union(const Symbol* containingSym,
          const SyntaxTree* tree,
          const Scope* enclosingScope,
          TagType* tagTy);
};

PSY_C_API std::string to_string(const Union* uniom);

} // C
} // psy

#endif
