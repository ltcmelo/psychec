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
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.

#include "TypeDeclaration__IMPL__.inc"
#include "TagTypeDeclaration_Union.h"

#include "syntax/Lexeme_Identifier.h"
#include "types/Type_Tag.h"

#include <iostream>
#include <sstream>

using namespace psy;
using namespace C;

Union::Union(const Symbol* containingSym,
             const SyntaxTree* tree,
             const Scope* enclosingScope,
             TagType* tagTy)
    : TagTypeDeclaration(containingSym,
                         tree,
                         enclosingScope,
                         tagTy,
                         TagTypeDeclarationKind::Union)
{
}

std::string Union::toDisplayString() const
{
    std::ostringstream oss;
    oss << "union ";
    oss << P_CAST->ty_->asTagType()->tag()->valueText();
    return oss.str();
}

namespace psy {
namespace C {

std::string to_string(const Union* uniom)
{
    if (!uniom)
        return "<Union is null>";
    std::ostringstream oss;
    oss << "<Union |";
    oss << " type:" << to_string(uniom->specifiedType());
    oss << ">";
    return oss.str();
}

} // C
} // psy
