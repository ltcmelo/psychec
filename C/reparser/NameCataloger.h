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

#ifndef PSYCHE_C_NAME_CATALOGER_H__
#define PSYCHE_C_NAME_CATALOGER_H__

#include "API.h"

#include "NameCatalog.h"

#include "syntax/SyntaxVisitor.h"

#include <memory>
#include <ostream>

namespace psy {
namespace C {

class PSY_C_NON_API NameCataloger : public SyntaxVisitor
{
public:
    NameCataloger(SyntaxTree* tree);

    std::unique_ptr<NameCatalog> catalogFor(const SyntaxNode*);

private:
    std::unique_ptr<NameCatalog> catalog_;

    using SyntaxVisitor::visit;
    using Base = SyntaxVisitor;

    //--------------//
    // Declarations //
    //--------------//
    Action visitTranslationUnit(const TranslationUnitSyntax*) override;

    /* Specifiers */
    Action visitTypedefName(const TypedefNameSyntax*) override;

    /* Declarators */
    Action visitIdentifierDeclarator(const IdentifierDeclaratorSyntax*) override;

    //-------------//
    // Expressions //
    //-------------//
    Action visitIdentifierName(const IdentifierNameSyntax*) override;

    //-------------//
    // Ambiguities //
    //-------------//
    Action visitAmbiguousTypeNameOrExpressionAsTypeReference(const AmbiguousTypeNameOrExpressionAsTypeReferenceSyntax*) override;
    Action visitAmbiguousCastOrBinaryExpression(const AmbiguousCastOrBinaryExpressionSyntax*) override;
    Action visitAmbiguousExpressionOrDeclarationStatement(const AmbiguousExpressionOrDeclarationStatementSyntax*) override;
};

} // C
} // psy

#endif
