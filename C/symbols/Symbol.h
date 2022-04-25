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

#include "Accessibility.h"
#include "SymbolKind.h"
#include "SymbolName.h"

#include "syntax/SyntaxReference.h"

#include "../common/location/Location.h"
#include "../common/infra/Pimpl.h"

#include <memory>
#include <vector>

namespace psy {
namespace C {

/**
 * \brief The Symbol class.
 *
 * \note
 * This API is inspired by that of \c Microsoft.CodeAnalysis.ISymbol
 * from Roslyn, the .NET Compiler Platform.
 */
class PSY_C_API Symbol
{
public:
    Symbol(const Symbol&) = delete;
    Symbol& operator=(const Symbol&) = delete;
    virtual ~Symbol();

    /**
     * The SymbolKind of \c this Symbol.
     */
    SymbolKind kind() const;

    virtual LibrarySymbol* asLibrary() { return nullptr; }
    virtual const LibrarySymbol* asLibrary() const { return nullptr; }
    virtual FunctionSymbol* asFunction() { return nullptr; }
    virtual const FunctionSymbol* asFunction() const { return nullptr; }
    virtual ObjectSymbol* asObject() { return nullptr; }
    virtual const ObjectSymbol* asObject() const { return nullptr; }
    virtual TypeSymbol* asType() { return nullptr; }
    virtual const TypeSymbol* asType() const { return nullptr; }

    /**
     * The Assembly where \c this Symbol is \a defined.
     */
    const Assembly* assembly() const;

    /**
     * The outer Scope of \c this Symbol.
     *
     * \remark 6.2.1-4
     */
    const Scope* outerScope() const;

    /**
     * The inner Scope of \c this Symbol.
     *
     * \remark 6.2.1-4
     */
    const Scope* innerScope() const;

    /**
     * The Symbol immediately containing \c this Symbol.
     */
    const Symbol* containingSymbol() const;

    /**
     * The Accessibility declared for \c this Symbol.
     */
    Accessibility declaredAccessibility() const;

    /**
     * References to the SyntaxNodes that \a declare \c this Symbol.
     */
    std::vector<SyntaxReference> declaringSyntaxReferences() const;

    /**
     * The Location where \c this Symbol is \a defined.
     */
    Location location() const;

protected:
    DECL_PIMPL(Symbol);

    Symbol(SymbolImpl* p);

private:
    friend class Binder;

    template <class ScopeT> ScopeT* makeScope();
};

std::string PSY_C_API to_string(const Symbol& sym);

} // C
} // psy

#endif
