// Copyright (c) 2016/17/18/19/20/21/22 Leandro T. C. Melo <ltcmelo@gmail.com>
// Copyright (c) 2008 Roberto Raggi <roberto.raggi@gmail.com>
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

#include "Lexeme_Constant.h"

using namespace psy;
using namespace C;


IntegerConstant::IntegerConstant(const char* chars, unsigned int size)
    : Lexeme(chars,
             size,
             LexemeKind::IntegerConstant)
{
}

IntegerConstant::RepresentationSuffix IntegerConstant::representationSuffix() const
{
    if (F_.llOrLL_) {
        return (F_.u_ || F_.U_)
                ? RepresentationSuffix::llOrLLAnduOrU
                : RepresentationSuffix::llOrLL;
    }
    if (F_.l_ || F_.L_) {
        return (F_.u_ || F_.U_)
                ? RepresentationSuffix::lOrLAnduOrU
                : RepresentationSuffix::lOrL;
    }
    return (F_.u_ || F_.U_)
            ? RepresentationSuffix::uOrU
            : RepresentationSuffix::None;
}

bool IntegerConstant::isOctalOrHexadecimal() const
{
    return F_.octal_ || F_.hex_;
}

FloatingConstant::FloatingConstant(const char* chars, unsigned int size)
    : Lexeme(chars,
             size,
             LexemeKind::FloatingConstant)
{}

FloatingConstant::RepresentationSuffix FloatingConstant::representationSuffix() const
{
    if (F_.fOrF_)
        return RepresentationSuffix::fOrF;
    if (F_.l_ or F_.L_)
        return RepresentationSuffix::lOrL;
    return RepresentationSuffix::None;
}

CharacterConstant::CharacterConstant(const char* chars, unsigned int size)
    : Lexeme(chars,
             size,
             LexemeKind::CharacterConstant)
{}

CharacterConstant::EncodingPrefix CharacterConstant::encodingPrefix() const
{
    if (F_.L_)
        return EncodingPrefix::L;
    if (F_.u_)
        return EncodingPrefix::u;
    if (F_.U_)
        return EncodingPrefix::U;
    return EncodingPrefix::None;
}

ImaginaryIntegerConstant::ImaginaryIntegerConstant(const char* chars, unsigned int size)
    : Lexeme(chars,
             size,
             LexemeKind::ImaginaryIntegerConstant)
{}

ImaginaryFloatingConstant::ImaginaryFloatingConstant(const char* chars, unsigned int size)
    : Lexeme(chars,
             size,
             LexemeKind::ImaginaryFloatingConstant)
{}
