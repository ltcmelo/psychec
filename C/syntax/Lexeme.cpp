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

#include "Lexeme.h"

using namespace psy;
using namespace C;

Lexeme::Lexeme(const char* chars, unsigned int size, LexemeKind kind)
    : TextElement(chars, size)
    , BD_(0)
{
    F_.kind_ = static_cast<std::uint16_t>(kind);

    checkHexAndOctalPrefix();
    checkVariousPrefixesAndSuffixes();
}

Lexeme::~Lexeme()
{}

Lexeme::LexemeKind Lexeme::kind() const
{
    return LexemeKind(F_.kind_);
}

void Lexeme::checkHexAndOctalPrefix()
{
    const char* chars = begin();
    if (size() > 0 && chars[0] == '0') {
        if (size() > 1) {
            if (chars[1] == 'x' || chars[1] == 'X')
                F_.hex_ = 1;
            else if (chars[1] >= '0' && chars[1] <= '7')
                F_.octal_ = 1;
            else
                ; // TODO: Flag as invalid.
        } else {
            F_.octal_ = 1;
        }
    }
}

void Lexeme::checkVariousPrefixesAndSuffixes()
{
    const char* kEnd = end();
    for (const char* cur = begin(); cur != kEnd; ++cur) {
        switch (*cur) {
            case 'l':
                if (cur + 1 != kEnd && *(cur + 1 ) == 'l') {
                    F_.llOrLL_ = 1;
                    ++cur;
                }
                else
                    F_.l_ = 1;
                break;

            case 'L':
                if (cur + 1 != kEnd && *(cur + 1) == 'L') {
                    F_.llOrLL_ = 1;
                    ++cur;
                }
                else
                    F_.L_ = 1;
                break;

            case 'u':
                if (cur + 1 != kEnd && *(cur + 1 ) == '8') {
                    F_.u8_ = 1;
                    ++cur;
                }
                else
                    F_.u_ = 1;
                break;

            case 'U':
                F_.U_ = 1;
                break;

            case 'f':
            case 'F':
                F_.fOrF_ = 1;
                break;
        }
    }
}

std::string Lexeme::valueText() const
{
    return c_str();
}

template <>
int Lexeme::value<int>() const
{
    return std::stoi(valueText());
}

template <>
long Lexeme::value<long>() const
{
    return std::stol(valueText());
}

template <>
long long Lexeme::value<long long>() const
{
    return std::stoll(valueText());
}

template <>
unsigned long Lexeme::value<unsigned long>() const
{
    return std::stoul(valueText());
}

template <>
unsigned long long Lexeme::value<unsigned long long>() const
{
    return std::stoull(valueText());
}

template <>
float Lexeme::value<float>() const
{
    return std::stof(valueText());
}

template <>
double Lexeme::value<double>() const
{
    return std::stod(valueText());
}

template <>
long double Lexeme::value<long double>() const
{
    return std::stold(valueText());
}

template <>
unsigned char Lexeme::value<unsigned char>() const
{
    // Lexeme is: 'c'
    return c_str()[1];
}

template <>
wchar_t Lexeme::value<wchar_t>() const
{
    // Lexeme is: L'c'
    return c_str()[2];
}

template <>
char16_t Lexeme::value<char16_t>() const
{
    // Lexeme is: u'c'
    return c_str()[2];
}

template <>
char32_t Lexeme::value<char32_t>() const
{
    // Lexeme is: U'c'
    return c_str()[2];
}

template <>
std::string Lexeme::value<std::string>() const
{
    auto p = valueText();
    return std::string(p.c_str() + 1, p.size() - 2);
}
