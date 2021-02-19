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

#include "DeclarationName.h"

#include "DeclarationNames.h"

#include "syntax/SyntaxLexemes.h"

#include <cstring>

using namespace psy;
using namespace C;

DeclarationName::DeclarationName(Kind kind)
    : kind_(kind)
{}

DeclarationName::~DeclarationName()
{}

DeclarationName::Kind DeclarationName::kind() const
{
    return kind_;
}

namespace psy {
namespace C {

std::string PSY_C_API to_string(DeclarationName::Kind kind)
{
    switch (kind) {
        case DeclarationName::Kind::Simple:
            return "IdentifierName";
        case DeclarationName::Kind::Tag:
            return "TagName";
        case DeclarationName::Kind::Anonymous:
            return "AnonymousName";
        default:
            return "<error-name>";
    }
}

} // C
} // psy
