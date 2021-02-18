// Copyright (c) 2016/17/18/19/20/21 Leandro T. C. Melo <ltcmelo@gmail.com>
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

#include "SyntaxLexemes.h"

using namespace psy;
using namespace C;

//---------------------//
// Identifier/keywords //
//---------------------//

Identifier::Identifier(const char* chars, unsigned int size)
    : SyntaxLexeme(chars,
                   size,
                   Kind::Identifier)
{}

//-----------//
// Constants //
//-----------//

Constant::Constant(const char* chars, unsigned int size, Kind kind)
    : SyntaxLexeme(chars, size, kind)
{
    checkHexPrefix();
    checkVariousPrefixesAndSuffixes();
}

IntegerConstant::IntegerConstant(const char* chars, unsigned int size)
    : Constant(chars,
               size,
               Kind::IntegerConstant)
{}

IntegerConstant::Signedness IntegerConstant::signedness() const
{
    return (BF_.u_ || BF_.U_) ? Signedness::Unsigned : Signedness::Signed;
}

IntegerConstant::Variant IntegerConstant::variant() const
{
    if (BF_.llOrLL_)
        return Variant::LongLong;

    if (BF_.l_ or BF_.L_)
        return Variant::Long;

    return Variant::Int;
}

FloatingConstant::FloatingConstant(const char* chars, unsigned int size)
    : Constant(chars,
               size,
               Kind::FloatingConstant)
{}

FloatingConstant::Variant FloatingConstant::variant() const
{
    if (BF_.fOrF_)
        return Variant::Float;

    if (BF_.l_ or BF_.L_)
        return Variant::LongDouble;

    return Variant::Double;
}

CharacterConstant::CharacterConstant(const char* chars, unsigned int size)
    : Constant(chars,
               size,
               Kind::CharacterConstant)
{}

CharacterConstant::Variant CharacterConstant::variant() const
{
    if (BF_.L_)
        return Variant::L_wchar_t;

    if (BF_.u_)
        return Variant::u_char16_t;

    if (BF_.U_)
        return Variant::U_char32_t;

    return Variant::Plain_u_char;
}

//----------------//
// String literal //
//----------------//

StringLiteral::StringLiteral(const char *chars, unsigned int size)
    : SyntaxLexeme(chars,
                   size,
                   Kind::StringLiteral)
{
    checkVariousPrefixesAndSuffixes();
}

StringLiteral::Variant StringLiteral::variant() const
{
    if (BF_.L_)
        return Variant::L_wchar_t;

    if (BF_.u8_)
        return Variant::u8_char;

    if (BF_.u_)
        return Variant::u_char16_t;

    if (BF_.U_)
        return Variant::U_char32_t;

    return Variant::Plain_char;
}
