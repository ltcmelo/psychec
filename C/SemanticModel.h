// Copyright (c) 2016/17/18/19/20/21 Leandro T. C. Melo <ltcmelo@gmail.com>
// Copyright (c) 2008 Roberto Raggi <roberto.raggi@gmail.com>
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
#include "APIFwds.h"

#include "names/DeclarationNames.h"

#include "../common/infra/Pimpl.h"

namespace psy {
namespace C {

class Binder;

/**
 * \brief The SemanticModel class.
 *
 * \note
 * This API inspired by that of \c Microsoft.CodeAnalysis.SemanticModel
 * from Roslyn, the .NET Compiler Platform.
 */
class PSY_C_API SemanticModel
{
public:
    ~SemanticModel();
    SemanticModel(const SemanticModel&) = delete;
    SemanticModel& operator=(const SemanticModel&) = delete;

    /**
     * The Compilation \c this SemanticModel was obtained from.
     */
    const Compilation* compilation() const;

    /**
     * The SyntaxTree \c this SemanticModel was obtained from.
     */
    const SyntaxTree* syntaxTree() const;

private:
    DECL_PIMPL(SemanticModel)

    friend class Compilation;
    friend class Binder;

    SemanticModel(Compilation* compilation, SyntaxTree* tree);

    void bind();

    /* Names */
    template <class NameT, class... ArgsT> NameT* make(ArgsT&&... args);
    IdentifierName* makeName(const Identifier* identifier);
    TagName* makeName(TagName::TagKind typeSpecifierKind, const Identifier* identifier);
    AnonymousName* makeName();

    Compilation* compilation();
    SyntaxTree* syntaxTree();
};

} // C
} // psy

#endif
