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
#include "syntax/SyntaxTree.h"

#include "types/TypeKind_Basic.h"
#include "sema/InferenceOptions.h"
#include "sema/PlatformOptions.h"

#include "../common/infra/AccessSpecifiers.h"
#include "../common/infra/Pimpl.h"

#include <string>
#include <vector>

namespace psy {
namespace C {

class SemanticModel;

/**
 * \brief The Compilation class.
 *
 * \note Resembles:
 * \c Microsoft.CodeAnalysis.Compilation from Roslyn.
 */
class PSY_C_API Compilation
{
    friend class InternalsTestSuite;

public:
    ~Compilation();

    /**
     * Create an empty Compilation identified by \p id.
     */
    static std::unique_ptr<Compilation> create(
            const std::string& id,
            PlatformOptions platformOpts = PlatformOptions(),
            InferenceOptions inferOpts = InferenceOptions());

    /**
     * The id of \c this Compilation.
     */
    const std::string& Id() const;

    /**
     * The inference options for \c this Compilation.
     */
    const InferenceOptions& inferenceOptions() const;

    /**
     * The platform options for \c this Compilation.
     */
    const PlatformOptions& platformOptions() const;

    /**
     * Whether \c this Compilation contains any SyntaxTrees.
     */
    bool isEmpty() const;

    /**
     * Add SyntaxTree \p tree to \c this Compilation.
     */
    void addSyntaxTree(std::unique_ptr<const SyntaxTree> tree);

    /**
     * The SyntaxTrees in \c this Compilation.
     */
    std::vector<const SyntaxTree*> syntaxTrees() const;

    /**
     * Compute the SemanticModel of the given \p tree.
     */
    void computeSemanticModel(const SyntaxTree* tree);

    /**
     * The SemanticModel of the given \p tree SyntaxTree.
     *
     * \note Similar to:
     * - \c Microsoft.CodeAnalysis.Compilation.GetSemanticModel of Roslyn.
     */
    const SemanticModel* semanticModel(const SyntaxTree* tree) const;

    /**
     * The Program in \c this Compilation.
     */
    const ProgramSymbol* program() const;

    /**
     * The canonical BasicType of \c basicTyK BasicTypeKind.
     */
    const BasicType* canonicalBasicType(BasicTypeKind basicTyK) const;

    /**
     * The canonical VoidType.
     */
    const VoidType* canonicalVoidType() const;

    /**
     * The canonical ErrorType.
     */
    const ErrorType* canonicalErrorType() const;

PSY_INTERNAL:
    PSY_GRANT_INTERNAL_ACCESS(SemanticModel);

    ProgramSymbol* program();

    void bindDeclarations(const SyntaxTree* tree) const;
    void bindDeclarations() const;
    void canonicalizeTypes(const SyntaxTree* tree) const;
    void canonicalizeTypes() const;
    void resolveTypedefNameTypes(const SyntaxTree* tree) const;
    void resolveTypedefNameTypes() const;
    void checkTypes(const SyntaxTree* tree) const;
    void checkTypes() const;

private:
    DECL_PIMPL(Compilation);
    Compilation();
    Compilation(const Compilation&) = delete;
    Compilation& operator=(const Compilation&) = delete;
};

} // C
} // psy

#endif
