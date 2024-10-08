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

#ifndef PSYCHE_C_TRANSLATION_UNIT_H__
#define PSYCHE_C_TRANSLATION_UNIT_H__

#include "Symbol.h"

#include "../common/infra/AccessSpecifiers.h"

namespace psy {
namespace C {

/**
 * \brief The TranslationUnit class.
 *
 * \note
 * This API is inspired by that of \c Microsoft.CodeAnalysis.IModuleSymbol
 * from Roslyn, the .NET Compiler Platform.
 */
class PSY_C_API TranslationUnit final : public Symbol
{
public:
    //!@{
    /**
     * Cast \c this Symbol as a TranslationUnit.
     */
    virtual TranslationUnit* asTranslationUnit() override { return this; }
    virtual const TranslationUnit* asTranslationUnit() const override { return this; }
    //!@}

    /**
     * Compute a displayable string for \c this TranslationUnit.
     */
    virtual std::string toDisplayString() const override;

    /**
     * The Scope that \c this TranslationUnit encloses.
     *
     * \remark 6.2.1-4
     */
    const Scope* enclosedScope() const;

PSY_INTERNAL:
    PSY_GRANT_INTERNAL_ACCESS(Binder);

    TranslationUnit(const Program* prog, const SyntaxTree* tree);

private:
    std::unique_ptr<Scope> enclosedScope_;
    const SyntaxTree* tree_;
};

std::string PSY_C_API to_string(const TranslationUnit& unit);

} // C
} // psy

#endif
