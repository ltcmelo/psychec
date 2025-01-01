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

#ifndef PSYCHE_C_SCOPE_H__
#define PSYCHE_C_SCOPE_H__

#include "API.h"
#include "Fwds.h"

#include "ScopeKind.h"
#include "NameSpace.h"

#include "../common/infra/AccessSpecifiers.h"

#include <cstdint>
#include <memory>
#include <unordered_map>
#include <utility>
#include <vector>

using namespace psy;
using namespace C;

template <>
struct std::hash<std::pair<const Identifier*, NameSpace>>
{
     std::size_t operator()(const std::pair<const Identifier*, NameSpace>& k) const
     {
        auto h1 = std::hash<const Identifier*>{}(k.first);
        auto h2 = std::hash<NameSpace>{}(k.second);
        return h1 ^ (h2 << 1);
     }
};

namespace psy {
namespace C {

/**
 * \brief The Scope class.
 *
 * \note Similar to:
 * - \c clang::Scope of LLVM/Clang.
 *
 * \remark 6.2.1
 */
class PSY_C_API Scope
{
public:
    virtual ~Scope();

    /**
     * The ScopeKind of \c this Scope.
     */
    ScopeKind kind() const;

    /**
     * Search \c this Scope and \a outer \a scopes of \c this Scope for a Declaration with
     * Identifier \p ident in NameSpace \p ns.
     */
    const Declaration* searchForDeclaration(const Identifier* ident, NameSpace ns) const;

    /**
     * The Declaration(s) in \c this Scope.
     */
    std::vector<const Declaration*> declarations() const;

    /**
     * The \a outer \a scope of \c this Scope.
     */
    const Scope* outerScope() const;

PSY_INTERNAL:
    PSY_GRANT_INTERNAL_ACCESS(Binder);
    PSY_GRANT_INTERNAL_ACCESS(TranslationUnit);

    Scope(ScopeKind scopeK);

    void encloseScope(Scope* innerScope);
    void morphFrom_FunctionPrototype_to_Block();
    void addDeclaration(const Declaration*);

private:
    ScopeKind scopeK_;
    Scope* outerScope_;
    std::unordered_map<
        std::pair<const Identifier*, NameSpace>,
        const Declaration*> decls_;
};

} // C
} // psy

#endif
