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

#include "Symbol__IMPL__.inc"
#include "symbols/Symbol_ALL.h"

#include "../common/infra/Assertions.h"

Symbol::Symbol(SymbolImpl* p)
    : P(p)
{}

Symbol::~Symbol()
{}

SymbolCategory Symbol::category() const
{
    switch (kind()) {
        case SymbolKind::Program:
            return SymbolCategory::Program;
        case SymbolKind::TranslationUnit:
            return SymbolCategory::TranslationUnit;
        case SymbolKind::FunctionDeclaration:
        case SymbolKind::EnumeratorDeclaration:
        case SymbolKind::FieldDeclaration:
        case SymbolKind::VariableDeclaration:
        case SymbolKind::ParameterDeclaration:
        case SymbolKind::TypedefDeclaration:
        case SymbolKind::StructDeclaration:
        case SymbolKind::UnionDeclaration:
        case SymbolKind::EnumDeclaration:
            return SymbolCategory::Declaration;
    }
    PSY_ASSERT_1(false);
    return SymbolCategory(-1);
}

SymbolKind Symbol::kind() const
{
    return SymbolKind(P->F_.symK_);
}

const Symbol* Symbol::containingSymbol() const
{
    return P->containingSym_;
}

namespace psy {
namespace C {

std::ostream& operator<<(std::ostream& os, const Symbol* sym)
{
    if (!sym)
        return os << "<Symbol is null>";
    switch (sym->category()) {
        case SymbolCategory::Program:
            return os << sym->asProgram();
        case SymbolCategory::TranslationUnit:
            return os << sym->asTranslationUnit();
        case SymbolCategory::Declaration:
            return os << sym->asDeclaration();
    }
    PSY_ASSERT_1(false);
    return os << "<invalid Symbol>";
}

} // C
} // psy
