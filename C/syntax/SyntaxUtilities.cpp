// Copyright (c) 2021 Leandro T. C. Melo <ltcmelo@gmail.com>
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

using namespace psy;
using namespace C;

const DeclaratorSyntax* SyntaxUtilities::innermostDeclarator(const DeclaratorSyntax* decltor)
{
    while (decltor) {
        const DeclaratorSyntax* innerDecltor = nullptr;
        switch (decltor->kind()) {
            case ParenthesizedDeclarator:
                innerDecltor = decltor->asParenthesizedDeclarator()->innerDeclarator();
                break;

            case PointerDeclarator:
                innerDecltor = decltor->asPointerDeclarator()->innerDeclarator();
                break;

            case ArrayDeclarator:
            case FunctionDeclarator:
                innerDecltor = decltor->asArrayOrFunctionDeclarator()->innerDeclarator();
                break;

            case BitfieldDeclarator:
                innerDecltor = decltor->asBitfieldDeclarator()->innerDeclarator();
                break;

            default:
                return decltor;
        }
        if (!innerDecltor)
            break;

        decltor = innerDecltor;
    }
    return decltor;
}

const DeclaratorSyntax*
SyntaxUtilities::declaratorEnclosedIn(const ParenthesizedDeclaratorSyntax* parenDecltor)
{
    const DeclaratorSyntax* decltor = parenDecltor;
    while (decltor && decltor->asParenthesizedDeclarator())
        decltor = decltor->asParenthesizedDeclarator()->innerDeclarator();
    return decltor;
}

const DeclaratorSyntax* SyntaxUtilities::strippedDeclarator(const DeclaratorSyntax* decltor)
{
    if (decltor->asParenthesizedDeclarator())
        return declaratorEnclosedIn(decltor->asParenthesizedDeclarator());
    return decltor;
}
