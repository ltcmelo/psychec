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

#include "ObjectDeclarationSymbol_Variable.h"

#include "binder/Scope.h"
#include "syntax/Lexeme_Identifier.h"
#include "types/Type_ALL.h"

#include <sstream>

using namespace psy;
using namespace C;

Variable::Variable(const SyntaxTree* tree,
                   const Symbol* containingSym,
                   const Scope* scope)
    : ObjectDeclarationSymbol(tree,
                              containingSym,
                              scope,
                              ObjectDeclarationSymbolKind::Variable)
{}

std::string Variable::toDisplayString() const
{
    return "";
}

namespace psy {
namespace C {

std::string to_string(const Variable& var)
{
    std::ostringstream oss;
    oss << "<Variable |";
    oss << " name:" << var.name()->valueText();
    oss << " type:" << to_string(*var.type());
    oss << ">";
    return oss.str();
}

} // C
} // psy
