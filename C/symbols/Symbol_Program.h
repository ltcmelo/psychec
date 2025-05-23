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
class PSY_C_API ProgramSymbol final : public Symbol
{
public:
    //!@{
    /**
     * Cast \c this Symbol as a ProgramSymbol.
     */
    virtual ProgramSymbol* asProgram() override { return this; }
    virtual const ProgramSymbol* asProgram() const override { return this; }
    //!@}

PSY_INTERNAL:
    PSY_GRANT_INTERNAL_ACCESS(Compilation);
    PSY_GRANT_INTERNAL_ACCESS(SemanticModel);

    DECL_PIMPL_SUB(Program);
    ProgramSymbol();
};

PSY_C_API std::ostream& operator<<(std::ostream& os, const ProgramSymbol* prog);

} // C
} // psy

#endif
