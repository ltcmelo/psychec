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

#ifndef PSYCHE_C_SYMBOL_H__
#define PSYCHE_C_SYMBOL_H__

#include "API.h"
#include "Fwds.h"

#include "SymbolCategory.h"
#include "SymbolKind.h"
#include "../common/infra/AccessSpecifiers.h"
#include "../common/infra/Pimpl.h"

#include <iostream>
#include <memory>

namespace psy {
namespace C {

/**
 * \brief The Symbol class.
 *
 * \note Resembles:
 * - \c Microsoft.CodeAnalysis.ISymbol from Roslyn.
 */
class PSY_C_API Symbol
{
public:
    virtual ~Symbol();

    /**
     * The SymbolCategory of \c this Symbol.
     */
    SymbolCategory category() const;

    /**
     * The SymbolKind of \c this Symbol.
     */
    SymbolKind kind() const;

    //!@{
    /**
     * Cast \c this Symbol.
     */
    virtual ProgramSymbol* asProgram() { return nullptr; }
    virtual const ProgramSymbol* asProgram() const { return nullptr; }
    virtual TranslationUnitSymbol* asTranslationUnit() { return nullptr; }
    virtual const TranslationUnitSymbol* asTranslationUnit() const { return nullptr; }
    virtual DeclarationSymbol* asDeclaration() { return nullptr; }
    virtual const DeclarationSymbol* asDeclaration() const { return nullptr; }
    virtual FunctionDeclarationSymbol* asFunctionDeclaration() { return nullptr; }
    virtual const FunctionDeclarationSymbol* asFunctionDeclaration() const { return nullptr; }
    virtual ObjectDeclarationSymbol* asObjectDeclaration() { return nullptr; }
    virtual const ObjectDeclarationSymbol* asObjectDeclaration() const { return nullptr; }
    virtual VariableDeclarationSymbol* asVariableDeclaration() { return nullptr; }
    virtual const VariableDeclarationSymbol* asVariableDeclaration() const { return nullptr; }
    virtual ParameterDeclarationSymbol* asParameterDeclaration() { return nullptr; }
    virtual const ParameterDeclarationSymbol* asParameterDeclaration() const { return nullptr; }
    virtual MemberDeclarationSymbol* asMemberDeclaration() { return nullptr; }
    virtual const MemberDeclarationSymbol* asMemberDeclaration() const { return nullptr; }
    virtual EnumeratorDeclarationSymbol* asEnumeratorDeclaration() { return nullptr; }
    virtual const EnumeratorDeclarationSymbol* asEnumeratorDeclaration() const { return nullptr; }
    virtual FieldDeclarationSymbol* asFieldDeclaration() { return nullptr; }
    virtual const FieldDeclarationSymbol* asFieldDeclaration() const { return nullptr; }
    virtual TypeDeclarationSymbol* asTypeDeclaration() { return nullptr; }
    virtual const TypeDeclarationSymbol* asTypeDeclaration() const { return nullptr; }
    virtual TagDeclarationSymbol* asTagTypeDeclaration() { return nullptr; }
    virtual const TagDeclarationSymbol* asTagTypeDeclaration() const { return nullptr; }
    virtual EnumDeclarationSymbol* asEnumDeclaration() { return nullptr; }
    virtual const EnumDeclarationSymbol* asEnumDeclaration() const { return nullptr; }
    virtual StructOrUnionDeclarationSymbol* asStructOrUnionDeclaration() { return nullptr; }
    virtual const StructOrUnionDeclarationSymbol* asStructOrUnionDeclaration() const { return nullptr; }
    virtual StructDeclarationSymbol* asStructDeclaration() { return nullptr; }
    virtual const StructDeclarationSymbol* asStructDeclaration() const { return nullptr; }
    virtual UnionDeclarationSymbol* asUnionDeclaration() { return nullptr; }
    virtual const UnionDeclarationSymbol* asUnionDeclaration() const { return nullptr; }
    virtual TypedefDeclarationSymbol* asTypedefDeclaration() { return nullptr; }
    virtual const TypedefDeclarationSymbol* asTypedefDeclaration() const { return nullptr; }
    //!@}

    /**
     * The Symbol, if any, that contains \c this Symbol.
     */
    const Symbol* containingSymbol() const;

protected:
    DECL_PIMPL(Symbol);
    Symbol(SymbolImpl* p);
    Symbol(const Symbol&) = delete;
    Symbol& operator=(const Symbol&) = delete;
};

PSY_C_API std::ostream& operator<<(std::ostream& os, const Symbol* sym);

} // C
} // psy

#endif
