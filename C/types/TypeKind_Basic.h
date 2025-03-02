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
// AUTHORS OR COPYRIGHT HERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.

#ifndef PSYCHE_C_TYPE_KIND_BASIC_H__
#define PSYCHE_C_TYPE_KIND_BASIC_H__

#include "API.h"
#include "Fwds.h"

#include "../common/infra/Assertions.h"

#include <cstdint>
#include <string>

namespace psy {
namespace C {

/**
 * \brief The BasicTypeKind enum.
 *
 * The unsigned integer types are suffixed with \c _U;
 * the signed integer types are suffixed with \c _S.
 * The signed integer types are equivalent to their "plain" versions, except for \c char:
 * \c char is a separate type from <tt>signed char</tt> and <tt>unsigned char</tt>;
 * its enumerator is \c Char only (without any suffix).
 *
 * \remark 6.2.5-3
 * \remark 6.2.5-17
 * \remark 6.2.5-15 (with footnote 45)
 * \remark 6.7.2-2
 */
enum class PSY_C_API BasicTypeKind : std::uint8_t
{
    Char,       // char
    Char_S,     // signed char
    Char_U,     // unsigned char
    Short_S,    // short, signed short, short int, or signed short int
    Short_U,    // unsigned short, or unsigned short int
    Int_S,      // int, signed, or signed int
    Int_U,      // unsigned, or unsigned int
    Long_S,     // long, signed long, long int, or signed long int
    Long_U,     // unsigned long, or unsigned long int
    LongLong_S, // long long, signed long long, long long int, or signed long long int
    LongLong_U, // unsigned long long, or unsigned long long int
    Bool,
    Float,
    Double,
    LongDouble,
    FloatComplex,
    DoubleComplex,
    LongDoubleComplex
};

PSY_C_API inline std::ostream& operator<<(std::ostream& os, BasicTypeKind basicTyK)
{
    switch (basicTyK) {
        case BasicTypeKind::Char:
                return os <<  "char";
        case BasicTypeKind::Char_S:
                return os <<  "signed char";
        case BasicTypeKind::Char_U:
                return os <<  "unsigned char";
        case BasicTypeKind::Short_S:
                return os <<  "signed short";
        case BasicTypeKind::Short_U:
                return os <<  "unsigned short";
        case BasicTypeKind::Int_S:
                return os <<  "signed int";
        case BasicTypeKind::Int_U:
                return os <<  "unsigned int";
        case BasicTypeKind::Long_S:
                return os <<  "signed long";
        case BasicTypeKind::Long_U:
                return os <<  "unsigned long";
        case BasicTypeKind::LongLong_S:
                return os <<  "signed long long";
        case BasicTypeKind::LongLong_U:
                return os <<  "unsigned long long";
        case BasicTypeKind::Bool:
                return os <<  "_Bool";
        case BasicTypeKind::Float:
                return os <<  "float";
        case BasicTypeKind::Double:
                return os <<  "double";
        case BasicTypeKind::LongDouble:
                return os <<  "long double";
        case BasicTypeKind::FloatComplex:
                return os <<  "float _Complex";
        case BasicTypeKind::DoubleComplex:
                return os <<  "double _Complex";
        case BasicTypeKind::LongDoubleComplex:
                return os <<  "long double _Complex";
    }
    PSY_ASSERT_1(false);
    return os <<  "<invalid BasicTypeKind>";
}

/**
 * \remark 6.2.5-4
 */
inline bool PSY_C_API isSignedIntegerTypeKind(const BasicTypeKind& basicTyK)
{
    switch (basicTyK) {
        case BasicTypeKind::Char_S:
        case BasicTypeKind::Short_S:
        case BasicTypeKind::Int_S:
        case BasicTypeKind::Long_S:
        case BasicTypeKind::LongLong_S:
            return true;
        default:
            return false;
    }
}

/**
 * \remark 6.2.5-6
 */
inline bool PSY_C_API isUnsignedIntegerTypeKind(const BasicTypeKind& basicTyK)
{
    switch (basicTyK) {
        case BasicTypeKind::Char_U:
        case BasicTypeKind::Short_U:
        case BasicTypeKind::Int_U:
        case BasicTypeKind::Long_U:
        case BasicTypeKind::LongLong_U:
        case BasicTypeKind::Bool:
            return true;
        default:
            return false;
    }
}

/**
 * \remark 6.2.5-17
 */
inline bool PSY_C_API isIntegerTypeKind(const BasicTypeKind& basicTyK)
{
    switch (basicTyK) {
        case BasicTypeKind::Char:
        case BasicTypeKind::Char_U:
        case BasicTypeKind::Char_S:
        case BasicTypeKind::Short_S:
        case BasicTypeKind::Short_U:
        case BasicTypeKind::Int_U:
        case BasicTypeKind::Int_S:
        case BasicTypeKind::Long_U:
        case BasicTypeKind::Long_S:
        case BasicTypeKind::LongLong_U:
        case BasicTypeKind::LongLong_S:
        case BasicTypeKind::Bool:
            return true;
        default:
            return false;
    }
}

inline bool PSY_C_API isRealTypeKind(const BasicTypeKind& basicTyK)
{
    switch (basicTyK) {
        case BasicTypeKind::Char_U:
        case BasicTypeKind::Char_S:
        case BasicTypeKind::Short_U:
        case BasicTypeKind::Short_S:
        case BasicTypeKind::Int_U:
        case BasicTypeKind::Int_S:
        case BasicTypeKind::Long_U:
        case BasicTypeKind::Long_S:
        case BasicTypeKind::LongLong_U:
        case BasicTypeKind::LongLong_S:
        case BasicTypeKind::Bool:
        case BasicTypeKind::Float:
        case BasicTypeKind::Double:
        case BasicTypeKind::LongDouble:
            return true;
        default:
            return false;
    }
}

} // C
} // psy

#endif
