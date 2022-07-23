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

#ifndef PSYCHE_C_SYMBOL_NAME_TAG_H__
#define PSYCHE_C_SYMBOL_NAME_TAG_H__

#include "SymbolName.h"
#include "TypeKind.h"

#include "../common/infra/InternalAccess.h"

#include <string>

namespace psy {
namespace C {

/**
 * \brief The TagSymbolName class.
 */
class PSY_C_API TagSymbolName final : public SymbolName
{
    friend std::string to_string(const TagSymbolName& name);
    friend bool operator==(const TagSymbolName& a, const TagSymbolName& b);

public:
    //!@{
    /**
     * Cast \c this SymbolNanem as a TagSymbolName.
     */
    virtual TagSymbolName* asTagSymbolName() override { return this; }
    virtual const TagSymbolName* asTagSymbolName() const override { return this; }
    //!@}

    /**
     * The choice of a tag.
     *
     * \remark \remark 6.7.2.3-2
     */
    enum class TagChoice : std::uint8_t
    {
        UNSPECIFIED = 0,

        Struct,
        Union,
        Enum
    };

    /**
     * The TagChoice of \c this TagSymbolName.
     */
    TagChoice tagChoice() const;

    /**
     * The text of \c this TagSymbolName.
     */
    virtual std::string text() const override;

PSY_INTERNAL_AND_RESTRICTED:
    PSY_GRANT_ACCESS(NamedTypeSymbol);

    TagSymbolName(TagChoice tagK, std::string tag);

private:
    TagChoice tagChoice_;
    std::string tag_;
};

bool operator!=(const TagSymbolName& a, const TagSymbolName& b);

} // C
} // psy

#endif
