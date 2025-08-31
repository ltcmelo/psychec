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

#include "Type.h"

using namespace psy;
using namespace C;

struct Type::TypeImpl
{
    TypeImpl(TypeKind tyK)
        : BD_(0)
    {
        F_.tyK_ = static_cast<std::uint8_t>(tyK);
    }

    struct BD
    {
        // Type
        std::uint32_t tyK_ : 4;

        // BasicType
        std::uint32_t basicTyK_: 5;

        // PointerType
        std::uint32_t ariseFromArrayDecay_ : 1;
        std::uint32_t ariseFromFuncDecay_ : 1;

        // TagType
        std::uint32_t tagTyK_ : 2;

        // FunctionType
        std::uint32_t isVariadic_ : 1;
    };
    union
    {
        std::uint32_t BD_;
        BD F_;
    };
};
