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

#ifndef PSYCHE_C_PROGRAM_H__
#define PSYCHE_C_PROGRAM_H__

#include "API.h"
#include "Fwds.h"

#include "types/TypeKind_Basic.h"
#include "symbols/Symbol.h"

#include "../common/infra/AccessSpecifiers.h"
#include "../common/infra/Pimpl.h"

namespace psy {
namespace C {

/**
 * \brief The Program class.
 *
 * \remark 5.1.1.1
 */
class PSY_C_API Program final : public Symbol
{
public:
    //!@{
    /**
     * Cast \c this Symbol as a Program.
     */
    virtual Program* asProgram() override { return this; }
    virtual const Program* asProgram() const override { return this; }
    //!@}

    /**
     * Compute a displayable string for \c this Symbol.
     */
    virtual std::string toDisplayString() const override;

    /**
     * The canonical BasicType of BasicTypeKind \c basicTyK.
     */
    const BasicType* canonicalBasicType(BasicTypeKind basicTyK) const;

    /**
     * The canonical VoidType.
     */
    const VoidType* canonicalVoidType() const;

PSY_INTERNAL:
    PSY_GRANT_INTERNAL_ACCESS(Compilation);
    PSY_GRANT_INTERNAL_ACCESS(SemanticModel);

    Program();

    DECL_PIMPL_SUB(Program);
};

std::string PSY_C_API to_string(const Symbol& prog);

} // C
} // psy

#endif
