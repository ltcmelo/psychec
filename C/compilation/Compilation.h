// Copyright (c) 2016/17/18/19/20/21/22 Leandro T. C. Melo <ltcmelo@gmail.com>
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
#include "Fwds.h"
#include "SyntaxTree.h"

#include "../common/infra/InternalAccess.h"
#include "../common/infra/Pimpl.h"

#include <string>
#include <vector>

namespace psy {
namespace C {

class SemanticModel;

/**
 * \brief The Compilation class.
 *
 * \note
 * This API is inspired by that of \c Microsoft.CodeAnalysis.Compilation
 * from Roslyn, the .NET Compiler Platform.
 */
class PSY_C_API Compilation
{
    friend class InternalsTestSuite;

public:
    ~Compilation();

    /**
     * Create an empty compilation identified by \p id with SyntaxTree \p tree.
     */
    static std::unique_ptr<Compilation> create(const std::string& id);

    /**
     * The Assembly produced by \c this Compilation.
     */
    const Assembly* assembly() const;

    /**
     * Add a SyntaxTree to \c this Compilation.
     */
    void addSyntaxTree(const SyntaxTree* tree);

    /**
     * Add SyntaxTrees to \c this Compilation.
     */
    void addSyntaxTrees(std::vector<const SyntaxTree*> trees);

    /**
     * The SyntaxTrees in \c this Compilation.
     */
    std::vector<const SyntaxTree*> syntaxTrees() const;

    /**
     * The SemanticModel for the SyntaxTree \p tree in \c this Compilation.
     */
    const SemanticModel* semanticModel(const SyntaxTree* tree) const;

PSY_INTERNAL_AND_RESTRICTED:
    PSY_GRANT_ACCESS(SemanticModel);

    Assembly* assembly();

private:
    Compilation();

    // Unavailable
    Compilation(const Compilation&) = delete;
    Compilation& operator=(const Compilation&) = delete;

    DECL_PIMPL(Compilation);
};

} // C
} // psy

#endif
