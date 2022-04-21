// Copyright (c) 2021 Leandro T. C. Melo <ltcmelo@gmail.com>
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

#include <cstdint>
#include <string>

namespace psy {
namespace C {

/**
 * \brief The SymbolName class.
 */
class PSY_C_API SymbolName
{
protected:
    SymbolName();

public:
    virtual ~SymbolName();

    virtual PlainSymbolName* asPlainSymbolName() { return nullptr; }
    virtual const PlainSymbolName* asPlainSymbolName() const { return nullptr; }
    virtual TagSymbolName* asTagSymbolName() { return nullptr; }
    virtual const TagSymbolName* asTagSymbolName() const { return nullptr; }
    virtual EmptySymbolName* asEmptySymbolName() { return nullptr; }
    virtual const EmptySymbolName* asEmptySymbolName() const { return nullptr; }

    /**
     * \brief The SymbolName::Kind enum.
     */
    enum class Kind : std::uint8_t
    {
        Empty,
        Plain,
        Tag
    };

    /**
     * The Kind of \c this SymbolName.
     */
    Kind kind() const;

    /**
     * The text of \c this SymbolName.
     */
    virtual std::string text() const = 0;

private:
    friend class Symbol;
};

std::string to_string(const SymbolName& name);

bool operator==(const SymbolName& a, const SymbolName& b);
bool operator!=(const SymbolName& a, const SymbolName& b);

} // C
} // psy

#endif
