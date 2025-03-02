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

#include "SyntaxUtilities.h"

#include "SyntaxNode.h"
#include "SyntaxNodes.h"
#include "SyntaxToken.h"
#include "Lexeme_ALL.h"

using namespace psy;
using namespace C;

const DeclaratorSyntax* SyntaxUtilities::innermostDeclaratorOf(const DeclaratorSyntax* decltor)
{
    while (decltor) {
        const DeclaratorSyntax* innerDecltor = innerDeclaratorOf(decltor);
        if (innerDecltor == decltor)
            break;
        decltor = innerDecltor;
    }
    return decltor;
}

const DeclaratorSyntax* SyntaxUtilities::innerDeclaratorOf(const DeclaratorSyntax* decltor)
{
    switch (decltor->kind()) {
        case SyntaxKind::PointerDeclarator:
            return decltor->asPointerDeclarator()->innerDeclarator();
        case SyntaxKind::ArrayDeclarator:
        case SyntaxKind::FunctionDeclarator:
            return decltor->asArrayOrFunctionDeclarator()->innerDeclarator();
        case SyntaxKind::BitfieldDeclarator:
            return decltor->asBitfieldDeclarator()->innerDeclarator();
        case SyntaxKind::ParenthesizedDeclarator:
            return decltor->asParenthesizedDeclarator()->innerDeclarator();
        default:
            return decltor;
    }
}

const DeclaratorSyntax* SyntaxUtilities::unparenthesizeDeclarator(
        const DeclaratorSyntax* decltor)
{
    while (decltor && decltor->asParenthesizedDeclarator())
        decltor = decltor->asParenthesizedDeclarator()->innerDeclarator();
    return decltor;
}

namespace psy {
namespace C {

const Identifier* identifierFrom(const IdentifierNameSyntax* node)
{
    return node->identifierToken().lexeme()->asIdentifier();
}

const StringLiteral* literalFrom(const StringLiteralExpressionSyntax* node)
{
    return node->literalToken().lexeme()->asStringLiteral();
}

} // C
} // psy
