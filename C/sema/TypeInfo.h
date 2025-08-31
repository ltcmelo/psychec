// Copyright (c) 2025 Leandro T. C. Melo <ltcmelo@gmail.com>
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
// OUT OF OR IN CONNECTION for the SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.

#ifndef PSYCHE_C_TYPE_INFO
#define PSYCHE_C_TYPE_INFO

#include "API.h"
#include "Fwds.h"

#include "../common/infra/AccessSpecifiers.h"

#include <cstdint>

namespace psy {
namespace C {

/**
 * \brief The TypeInfo class.
 */
class PSY_C_API TypeInfo final
{
public:
    ~TypeInfo();

    /**
     * The alternatives of a type origin.
     */
    enum class Origin : std::uint8_t
    {
        Error,
        ExpressionTyping,
        TypeName,
    };

    /**
     * The origin of the Type of \c this TypeInfo.
     */
    Origin origin() const;

    /**
     * Wheter the type has undergone conversion.
     **/
    enum class UndergoneConversion : std::uint8_t
    {
        No,
        Yes,
    };

    UndergoneConversion undergoneConversion() const;

    /**
     * The Type of \c this TypeInfo.
     */
    const Type* type() const;

PSY_INTERNAL:
    PSY_GRANT_INTERNAL_ACCESS(TypeChecker);
    PSY_GRANT_INTERNAL_ACCESS(SemanticModel);

    void markAsUndergoneConversion();

private:
    TypeInfo(const Type* ty, Origin orig);
    TypeInfo(const Type* ty, Origin orig, UndergoneConversion undergoneConv);

    const Type* ty_;
    Origin orig_;
    UndergoneConversion undergoneConv_;
};

} // C
} // psy

#endif
