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

#ifndef PSYCHE_C_SYMBOL_NAME_H__
#define PSYCHE_C_SYMBOL_NAME_H__

#include "API.h"
#include "Fwds.h"

#include "SymbolNameKind.h"

#include "../common/infra/InternalAccess.h"

#include <cstdint>
#include <string>

namespace psy {
namespace C {

/**
 * \brief The SymbolName class.
 */
class PSY_C_API SymbolName
{
public:
    virtual ~SymbolName();

    //!@{
    /**
     * Cast \c this SymbolName.
     */
    virtual PlainSymbolName* asPlainSymbolName() { return nullptr; }
    virtual const PlainSymbolName* asPlainSymbolName() const { return nullptr; }
    virtual TagSymbolName* asTagSymbolName() { return nullptr; }
    virtual const TagSymbolName* asTagSymbolName() const { return nullptr; }
    virtual EmptySymbolName* asEmptySymbolName() { return nullptr; }
    virtual const EmptySymbolName* asEmptySymbolName() const { return nullptr; }

    /**
     * The SymbolName of \c this SymbolName.
     */
    SymbolNameKind kind() const;

    /**
     * The text of \c this SymbolName.
     */
    virtual std::string text() const = 0;

protected:
    SymbolName();
};

std::string to_string(const SymbolName& name);

} // C
} // psy

#endif
