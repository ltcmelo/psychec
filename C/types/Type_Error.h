// Copyright (c) 2021/22/23/24 Leandro T. C. Melo <ltcmelo@gmail.com>
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

#ifndef PSYCHE_C_TYPE_ERROR_H__
#define PSYCHE_C_TYPE_ERROR_H__

#include "API.h"
#include "Fwds.h"

#include "Type.h"

namespace psy {
namespace C {

/**
 * \brief The ErrorType class.
 */
class PSY_C_API ErrorType final : public Type
{
public:
    //!@{
    /**
     * Cast \c this Type as an ErrorType.
     */
    virtual ErrorType* asErrorType() override { return this; }
    virtual const ErrorType* asErrorType() const override { return this; }
    //!@}

    /**
     * The DetectedUse alternatives of a ErrorType.
     */;
    enum class DetectedUse : std::uint8_t
    {
        AsTypedefNameType,
        AsStructType,
        AsUnionType,
        AsEnumType
    };

    /**
     * The DetectedUse of \c this ErrorType.
     */
    DetectedUse detectedUse() const;

PSY_INTERNAL:
    PSY_GRANT_INTERNAL_ACCESS(Compilation);
    PSY_GRANT_INTERNAL_ACCESS(DeclarationBinder);
    PSY_GRANT_INTERNAL_ACCESS(TypeResolver);
    PSY_GRANT_INTERNAL_ACCESS(TypeChecker);

    ErrorType();
    ErrorType(DetectedUse detectedUse);

    DetectedUse detectedUse_;
};

PSY_C_API std::ostream& operator<<(std::ostream& os, const ErrorType* unknownTy);

} // C
} // psy

#endif
