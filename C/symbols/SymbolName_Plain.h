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

#ifndef PSYCHE_C_SYMBOL_NAME_PLAIN_H__
#define PSYCHE_C_SYMBOL_NAME_PLAIN_H__

#include "SymbolName.h"

#include <string>

namespace psy {
namespace C {

/**
 * \brief The PlainSymbolName class.
 */
class PSY_C_API PlainSymbolName final : public SymbolName
{
    friend std::string to_string(const PlainSymbolName& name);
    friend bool operator==(const PlainSymbolName& a, const PlainSymbolName& b);

public:
    //!@{
    /**
     * Cast \c this SymbolName as a PlainSymbolName.
     */
    virtual PlainSymbolName* asPlainSymbolName() override { return this; }
    virtual const PlainSymbolName* asPlainSymbolName() const override { return this; }
    //!@}

    /**
     * The text of \c this SymbolName.
     */
    virtual std::string text() const override;

PSY_INTERNAL_AND_RESTRICTED:
    PSY_GRANT_ACCESS(Binder);
    PSY_GRANT_ACCESS(NamedTypeSymbol);

    PlainSymbolName(std::string s);

private:
    std::string s_;
};

bool operator!=(const PlainSymbolName& a, const PlainSymbolName& b);

} // C
} // psy

#endif
