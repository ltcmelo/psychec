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

#include "SyntaxLexeme.h"

using namespace psy;
using namespace C;

SyntaxLexeme::SyntaxLexeme(const char* chars, unsigned int size, Kind kind)
    : TextElement(chars, size)
    , BF_all_(0)
{
    BF_.kind_ = static_cast<std::uint16_t>(kind);
}

SyntaxLexeme::~SyntaxLexeme()
{}

SyntaxLexeme::Kind SyntaxLexeme::kind() const
{
    return Kind(BF_.kind_);
}

void SyntaxLexeme::checkHexPrefix()
{
    const char* chars = begin();
    if (size() > 1
            && chars[0] == '0'
            && (chars[1] == 'x' || chars[1] == 'X')) {
        BF_.hex_ = 1;
    }
}

void SyntaxLexeme::checkVariousPrefixesAndSuffixes()
{
    const char* kEnd = end();
    for (const char* cur = begin(); cur != kEnd; ++cur) {
        switch (*cur) {
            case 'l':
                if (cur + 1 != kEnd && *(cur + 1 ) == 'l') {
                    BF_.llOrLL_ = 1;
                    ++cur;
                }
                else
                    BF_.l_ = 1;
                break;

            case 'L':
                if (cur + 1 != kEnd && *(cur + 1) == 'L') {
                    BF_.llOrLL_ = 1;
                    ++cur;
                }
                else
                    BF_.L_ = 1;
                break;

            case 'u':
                if (cur + 1 != kEnd && *(cur + 1 ) == '8') {
                    BF_.u8_ = 1;
                    ++cur;
                }
                else
                    BF_.u_ = 1;
                break;

            case 'U':
                BF_.U_ = 1;
                break;

            case 'f':
            case 'F':
                BF_.fOrF_ = 1;
                break;
        }
    }
}

std::string SyntaxLexeme::valueText() const
{
    return c_str();
}

template <>
int SyntaxLexeme::value<int>() const
{
    return std::stoi(valueText());
}

template <>
long SyntaxLexeme::value<long>() const
{
    return std::stol(valueText());
}

template <>
long long SyntaxLexeme::value<long long>() const
{
    return std::stoll(valueText());
}

template <>
unsigned long SyntaxLexeme::value<unsigned long>() const
{
    return std::stoul(valueText());
}

template <>
unsigned long long SyntaxLexeme::value<unsigned long long>() const
{
    return std::stoull(valueText());
}

template <>
float SyntaxLexeme::value<float>() const
{
    return std::stof(valueText());
}

template <>
double SyntaxLexeme::value<double>() const
{
    return std::stod(valueText());
}

template <>
long double SyntaxLexeme::value<long double>() const
{
    return std::stold(valueText());
}

template <>
unsigned char SyntaxLexeme::value<unsigned char>() const
{
    // Lexeme is: 'c'
    return c_str()[1];
}

template <>
wchar_t SyntaxLexeme::value<wchar_t>() const
{
    // Lexeme is: L'c'
    return c_str()[2];
}

template <>
char16_t SyntaxLexeme::value<char16_t>() const
{
    // Lexeme is: u'c'
    return c_str()[2];
}

template <>
char32_t SyntaxLexeme::value<char32_t>() const
{
    // Lexeme is: U'c'
    return c_str()[2];
}

template <>
std::string SyntaxLexeme::value<std::string>() const
{
    auto p = valueText();
    return std::string(p.c_str() + 1, p.size() - 2);
}
