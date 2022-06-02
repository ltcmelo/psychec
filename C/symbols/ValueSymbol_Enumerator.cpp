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

#include "ValueSymbol_Enumerator.h"

#include "symbols/Symbol_ALL.h"
#include "binder/Scope.h"

#include <sstream>

using namespace psy;
using namespace C;

EnumeratorSymbol::EnumeratorSymbol(const SyntaxTree* tree,
                                   const Scope* scope,
                                   const Symbol* containingSym)
    : ValueSymbol(tree,
                  scope,
                  containingSym,
                  ValueKind::Enumerator)
{}

namespace psy {
namespace C {

std::string to_string(const EnumeratorSymbol& sym)
{
    std::ostringstream oss;
    oss << "{^enumerator |";
    oss << " " << (sym.name() ? to_string(*sym.name()) : "name:NULL");
    oss << " " << (sym.type() ? to_string(*sym.type()) : "type:NULL");
    oss << " " << (sym.scope() ? to_string(sym.scope()->kind()) : "scope:NULL");
    oss << " ^}";

    return oss.str();
}

} // C
} // psi
