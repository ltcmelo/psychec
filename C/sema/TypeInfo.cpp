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

#include "TypeInfo.h"

using namespace psy;
using namespace C;

TypeInfo::TypeInfo(const Type* ty, Origin orig)
    : ty_(ty)
    , orig_(orig)
    , undergoneConv_(UndergoneConversion::No)
{}

TypeInfo::TypeInfo(const Type* ty,
                   Origin tyOrig,
                   UndergoneConversion undergoneConv)
    : ty_(ty)
    , orig_(tyOrig)
    , undergoneConv_(undergoneConv)
{}

TypeInfo::~TypeInfo()
{}

TypeInfo::Origin TypeInfo::origin() const
{
    return orig_;
}

void TypeInfo::markAsUndergoneConversion()
{
    undergoneConv_ = UndergoneConversion::Yes;
}

TypeInfo::UndergoneConversion TypeInfo::undergoneConversion() const
{
    return undergoneConv_;
}

const Type* TypeInfo::type() const
{
    return ty_;
}
