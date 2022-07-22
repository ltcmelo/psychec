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

#ifndef PSYCHE_C_SYNTAX_CORRELATION_DISAMBIGUATOR_H__
#define PSYCHE_C_SYNTAX_CORRELATION_DISAMBIGUATOR_H__

#include "API.h"

#include "reparser/Disambiguator.h"
#include "reparser/NameCatalog.h"

#include "../common/infra/InternalAccess.h"

#include <memory>

namespace psy {
namespace C {

class PSY_C_NON_API SyntaxCorrelationDisambiguator final : public Disambiguator
{
PSY_INTERNAL_AND_RESTRICTED:
    PSY_GRANT_ACCESS(Reparser);

    SyntaxCorrelationDisambiguator(SyntaxTree* tree);

    void acquireCatalog(std::unique_ptr<NameCatalog> catalog);

private:
    std::unique_ptr<NameCatalog> catalog_;

    virtual Disambiguation disambiguateExpression(const AmbiguousCastOrBinaryExpressionSyntax*) const override;
    virtual Disambiguation disambiguateStatement(const AmbiguousExpressionOrDeclarationStatementSyntax*) const override;
    virtual Disambiguation disambiguateTypeReference(const AmbiguousTypeNameOrExpressionAsTypeReferenceSyntax*) const override;

    bool recognizesTypeName(const std::string& name) const;
    bool recognizesName(const std::string& name) const;

    //--------------//
    // Declarations //
    //--------------//
    Action visitTranslationUnit(const TranslationUnitSyntax*) override;
};

} // C
} // psy

#endif
