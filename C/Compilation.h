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

#ifndef PSYCHE_C_COMPILATION_H__
#define PSYCHE_C_COMPILATION_H__

#include "API.h"
#include "APIFwds.h"

#include "../common/infra/Pimpl.h"

#include <memory>
#include <string>

namespace psy {
namespace C {

class SemanticModel;

/**
 * \brief The Compilation class.
 *
 * \note
 * This API inspired by that of \c Microsoft.CodeAnalysis.Compilation
 * from Roslyn, the .NET Compiler Platform.
 */
class PSY_C_API Compilation
{
public:
    Compilation(const Compilation&) = delete;
    Compilation& operator=(const Compilation&) = delete;
    ~Compilation();

    /**
     * Create a new compilation.
     */
    std::unique_ptr<Compilation> create(const std::string& id);

    /**
     * The semantic model associated to the given \p tree, which
     * must be part of \c this compilation.
     */
    std::unique_ptr<SemanticModel> semanticModel(SyntaxTree* tree) const;

private:
    Compilation(const std::string& id);

    DECL_PIMPL(Compilation);
};

} // C
} // psy

#endif
