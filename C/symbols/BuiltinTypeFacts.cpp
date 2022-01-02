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

#include "BuiltinTypeFacts.h"

using namespace psy;
using namespace C;

bool BuiltinTypeFacts::areEquivalent(BuiltinTypeKind builtTyKind_a, BuiltinTypeKind builtTyKind_b)
{
    if (builtTyKind_a == builtTyKind_b)
        return true;

    switch (builtTyKind_a) {
        case BuiltinTypeKind::Short:
            return builtTyKind_b == BuiltinTypeKind::Short_S;
        case BuiltinTypeKind::Short_S:
            return builtTyKind_b == BuiltinTypeKind::Short;
        case BuiltinTypeKind::Int:
            return builtTyKind_b == BuiltinTypeKind::Int_S;
        case BuiltinTypeKind::Int_S:
            return builtTyKind_b == BuiltinTypeKind::Int;
        case BuiltinTypeKind::Long:
            return builtTyKind_b == BuiltinTypeKind::Long_S;
        case BuiltinTypeKind::Long_S:
            return builtTyKind_b == BuiltinTypeKind::Long;
        case BuiltinTypeKind::LongLong:
            return builtTyKind_b == BuiltinTypeKind::LongLong_S;
        case BuiltinTypeKind::LongLong_S:
            return builtTyKind_b == BuiltinTypeKind::LongLong;
        default:
            return false;
    }
}
