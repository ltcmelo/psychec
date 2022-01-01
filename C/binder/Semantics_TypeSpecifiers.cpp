// Copyright (c) 2022 Leandro T. C. Melo <ltcmelo@gmail.com>
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

#include "Semantics_TypeSpecifiers.h"

using namespace psy;
using namespace C;

BuiltinTypeKind Semantics_TypeSpecifiers::combine(BuiltinTypeKind builtTyKind, SyntaxKind tkK)
{
    switch (builtTyKind) {
        case BuiltinTypeKind::None:
            switch (tkK) {
                case Keyword_void:
                    return BuiltinTypeKind::Void;
                case Keyword_char:
                    return BuiltinTypeKind::Char;
                case Keyword_short:
                    return BuiltinTypeKind::Short;
                case Keyword_int:
                    return BuiltinTypeKind::Int;
                case Keyword_long:
                    return BuiltinTypeKind::Long;
                case Keyword_float:
                    return BuiltinTypeKind::Float;
                case Keyword_double:
                    return BuiltinTypeKind::Double;
                case Keyword__Bool:
                    return BuiltinTypeKind::Bool;
                case Keyword__Complex:
                    return BuiltinTypeKind::DoubleComplex;
                default:
                    // report
                    return builtTyKind;
            }

        case BuiltinTypeKind::Void:
            // report
            return builtTyKind;

        case BuiltinTypeKind::Char:
            switch (tkK) {
                case Keyword_signed:
                    return BuiltinTypeKind::Char_S;
                case Keyword_unsigned:
                    return BuiltinTypeKind::Char_U;
                default:
                    // report
                    return builtTyKind;
            }

        case BuiltinTypeKind::Char_S:
        case BuiltinTypeKind::Char_U:
            // report
            return builtTyKind;



        default:
            return BuiltinTypeKind::None;
    }
}
