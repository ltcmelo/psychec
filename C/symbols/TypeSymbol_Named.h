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

#ifndef PSYCHE_C_SYMBOL_NAMED_TYPE_H__
#define PSYCHE_C_SYMBOL_NAMED_TYPE_H__

#include "Symbol_Type.h"
#include "SymbolName_Tag.h"
#include "TypeKind_Builtin.h"
#include "TypeKind_Named.h"

#include <memory>
#include <string>

namespace psy {
namespace C {

/**
 * \brief The NamedTypeSymbol class.
 *
 * \note
 * This API is inspired by that of \c Microsoft.CodeAnalysis.INamedTypeSymbol
 * from Roslyn, the .NET Compiler Platform.
 */
class PSY_C_API NamedTypeSymbol final : public TypeSymbol
{
public:
    //!@{
    /**
     * Cast \c this TypeSymbol as a NamedTypeSymbol.
     */
    virtual NamedTypeSymbol* asNamedType() override { return this; }
    virtual const NamedTypeSymbol* asNamedType() const override { return this; }
    //!@}

    /**
     * The type name kind of \c this NamedTypeSymbol.
     */
    NamedTypeKind namedTypeKind() const;

    /**
     * The BuiltinTypeKind of \c this type.
     */
    BuiltinTypeKind builtinTypeKind() const;

    /**
     * The SymbolName of \c this Symbol.
     */
    const SymbolName* name() const;

PSY_INTERNAL_AND_RESTRICTED:
    PSY_GRANT_ACCESS(Binder);
    PSY_GRANT_ACCESS(ConstraintsInTypeSpecifiers);

    NamedTypeSymbol(const SyntaxTree* tree,
                    const Scope* scope,
                    const Symbol* containingSym,
                    BuiltinTypeKind builtTyK);

    NamedTypeSymbol(const SyntaxTree* tree,
                    const Scope* scope,
                    const Symbol* containingSym,
                    const std::string& name);

    NamedTypeSymbol(const SyntaxTree* tree,
                    const Scope* scope,
                    const Symbol* containingSym,
                    TagSymbolName::TagChoice tagChoice,
                    const std::string& tag);

    void patchBuiltinTypeKind(BuiltinTypeKind);

private:
    DECL_PIMPL_SUB(NamedTypeSymbol)
};

std::string PSY_C_API to_string(const NamedTypeSymbol& tySym);

} // C
} // psy

#endif
