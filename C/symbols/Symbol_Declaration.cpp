// Copyright (c) 2024 Leandro T. C. Melo <ltcmelo@gmail.com>
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

#include "Declaration__IMPL__.inc"
#include "Symbol_Declaration.h"

#include "sema/Scope.h"
#include "sema/Compilation.h"
#include "symbols/Symbol_ALL.h"
#include "syntax/SyntaxNodes.h"
#include "syntax/SyntaxReference.h"

#include "../common/infra/Assertions.h"

#include <algorithm>
#include <sstream>

using namespace psy;
using namespace C;

DeclarationSymbol::DeclarationSymbol(DeclarationImpl* p)
    : Symbol(p)
{}

const Scope* DeclarationSymbol::enclosingScope() const
{
    return P_CAST->enclosingScope_;
}

const NameSpace DeclarationSymbol::nameSpace() const
{
    return NameSpace(P->F_.ns_);
}

DeclarationCategory DeclarationSymbol::category() const
{
    switch (kind()) {
        case SymbolKind::Program:
        case SymbolKind::TranslationUnit:
            break;
        case SymbolKind::FunctionDeclaration:
            return DeclarationCategory::Function;
        case SymbolKind::EnumeratorDeclaration:
        case SymbolKind::FieldDeclaration:
            return DeclarationCategory::Member;
        case SymbolKind::VariableDeclaration:
        case SymbolKind::ParameterDeclaration:
            return DeclarationCategory::Object;
        case SymbolKind::TypedefDeclaration:
        case SymbolKind::StructDeclaration:
        case SymbolKind::UnionDeclaration:
        case SymbolKind::EnumDeclaration:
            return DeclarationCategory::Type;
    }
    PSY_ASSERT_1(false);
    return DeclarationCategory(~0);
}

namespace psy {
namespace C {

PSY_C_API std::ostream& operator<<(std::ostream& os, const DeclarationSymbol* decl)
{
    if (!decl)
        return os << "<Declaration is null>";
    switch (decl->category()) {
        case DeclarationCategory::Function:
            return os << decl->asFunctionDeclaration();
        case DeclarationCategory::Object:
            return os << decl->asObjectDeclaration();
        case DeclarationCategory::Member:
            return os << decl->asMemberDeclaration();
        case DeclarationCategory::Type:
            return os << decl->asTypeDeclaration();
    }
    PSY_ASSERT_1(false);
    return os << "<invalid Declaration>";
}

} // C
} // psy
