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

#include "Disambiguator_GuidelineImposition.h"

#include "syntax/SyntaxNodes.h"

#include "../common/infra/Assertions.h"

using namespace psy;
using namespace C;

GuidelineImpositionDisambiguator::GuidelineImpositionDisambiguator(SyntaxTree* tree)
    : Disambiguator(tree)
{}

Disambiguator::Disambiguation GuidelineImpositionDisambiguator::disambiguateExpression(
        const AmbiguousCastOrBinaryExpressionSyntax* node) const
{
    return Disambiguation::KeepCastExpression;
}

Disambiguator::Disambiguation GuidelineImpositionDisambiguator::disambiguateStatement(
        const AmbiguousExpressionOrDeclarationStatementSyntax* node) const
{
    switch (node->kind()) {
        case AmbiguousMultiplicationOrPointerDeclaration:
            return Disambiguation::KeepDeclarationStatement;

        case AmbiguousCallOrVariableDeclaration:
            return Disambiguation::KeepExpressionStatement;

        default:
            PSY_ASSERT(false, return Disambiguation::Inconclusive);
    }
}

Disambiguator::Disambiguation GuidelineImpositionDisambiguator::disambiguateTypeReference(
        const AmbiguousTypeNameOrExpressionAsTypeReferenceSyntax* node) const
{
    return Disambiguation::KeepTypeName;
}
