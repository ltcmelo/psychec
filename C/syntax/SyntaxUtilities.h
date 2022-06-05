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

#ifndef PSYCHE_C_SYNTAX_UTILITIES_H__
#define PSYCHE_C_SYNTAX_UTILITIES_H__

#include "API.h"
#include "Fwds.h"

namespace psy {
namespace C {

/**
 * \brief The SyntaxUtilities class.
 */
class PSY_C_API SyntaxUtilities
{
public:
    /**
     * Return the \a innermost DeclaratorSyntax of the given \p decltor.
     *
     * \see innerDeclaratorOrSelf
     */
    static const DeclaratorSyntax* innermostDeclaratorOrSelf(const DeclaratorSyntax* decltor);

    /**
     * Return the \a inner DeclaratorSyntax of the given \p decltor.
     *
     * A DeclaratorSyntax has an \a inner DeclaratorSyntax if it's one the following:
     *   - a PointerDeclaratorSyntax;
     *   - an ArrayDeclaratorSyntax;
     *   - a FunctionDeclaratorSyntax;
     *   - or a BitfieldDeclaratorSyntax.
     */
    static const DeclaratorSyntax* innerDeclaratorOrSelf(const DeclaratorSyntax* decltor);

    /**
     * Return the \a stripped DeclaratorSyntax of the given \p decltor.
     *
     * A DeclaratorSyntax is \a stripped if it's not a ParenthesizedDeclaratorSyntax.
     */
    static const DeclaratorSyntax* strippedDeclaratorOrSelf(const DeclaratorSyntax* decltor);
};

} // C
} // psy

#endif
