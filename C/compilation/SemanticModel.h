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

#ifndef PSYCHE_C_SEMANTIC_MODEL_H__
#define PSYCHE_C_SEMANTIC_MODEL_H__

#include "API.h"
#include "Fwds.h"

#include "../common/infra/InternalAccess.h"
#include "../common/infra/Pimpl.h"

namespace psy {
namespace C {

class Binder;

/**
 * \brief The SemanticModel class.
 *
 * \note
 * This API is inspired by that of \c Microsoft.CodeAnalysis.SemanticModel
 * from Roslyn, the .NET Compiler Platform.
 */
class PSY_C_API SemanticModel
{
public:
    ~SemanticModel();

    /**
     * The Compilation which \c this SemanticModel was computed from.
     */
    const Compilation* compilation() const;

    //!@{
    /**
     * The Symbol declared by the given by SyntaxNode \p node.
     */
    const Symbol* declaredSymbol(const DeclaratorSyntax* node) const;
    //!@}

PSY_INTERNAL:
    PSY_GRANT_ACCESS(Binder);
    PSY_GRANT_ACCESS(Compilation);

    SemanticModel(Compilation* compilation, const SyntaxTree* tree);

    Symbol* storeDeclaredSym(const SyntaxNode* node, std::unique_ptr<Symbol> sym);
    Symbol* storeUsedSym(std::unique_ptr<Symbol> sym);

private:
    // Unavailable
    SemanticModel(const SemanticModel&) = delete;
    SemanticModel& operator=(const SemanticModel&) = delete;

    DECL_PIMPL(SemanticModel)
};

} // C
} // psy

#endif
