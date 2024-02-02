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

#ifndef PSYCHE_C_NAME_CATALOG_H__
#define PSYCHE_C_NAME_CATALOG_H__

#include "API.h"
#include "Fwds.h"

#include "../common/infra/InternalAccess.h"

#include <ostream>
#include <stack>
#include <string>
#include <tuple>
#include <unordered_map>
#include <utility>

namespace psy {
namespace C {

class PSY_C_NON_API NameCatalog
{
    friend std::ostream& operator<<(std::ostream& os, const NameCatalog& disambigCatalog);

public:
    ~NameCatalog();

PSY_INTERNAL_AND_RESTRICTED:
    PSY_GRANT_ACCESS(NameCataloger);
    PSY_GRANT_ACCESS(SyntaxCorrelationDisambiguator);

    void mapNodeAndMarkAsEncloser(const SyntaxNode* node);
    void markMappedNodeAsEncloser(const SyntaxNode* node);
    void dropEncloser();

    void catalogUseAsTypeName(const std::string& name);
    void catalogUseAsNonTypeName(const std::string& name);
    void catalogDefAsTypeName(const std::string& name);
    void catalogDefAsNonTypeName(const std::string& name);

    bool hasUseAsTypeName(const std::string& name) const;
    bool hasUseAsNonTypeName(const std::string& name) const;
    bool hasDefAsTypeName(const std::string& name) const;
    bool hasDefAsNonTypeName(const std::string& name) const;

private:
    using NameUseAndDef = std::unordered_map<std::string, bool>;
    using Enclosure = std::tuple<NameUseAndDef, NameUseAndDef>;
    using NamesByNode = std::unordered_map<const SyntaxNode*, Enclosure>;

    static constexpr int tyIdx_ = 0;
    static constexpr int nonTyIdx_ = 1;

    std::stack<const SyntaxNode*> enclosersStack_;
    mutable NamesByNode namesByNode_;

    Enclosure* currentEnclosure() const;
    bool isNodeMapped(const SyntaxNode*node) const;

    template <size_t, size_t> void catalogUse_CORE(const std::string& name);
    template <size_t> void catalogDef_CORE(const std::string& name);
    void catalogUseWithMutualExclusion(const std::string& name,
                                       NameUseAndDef& in,
                                       NameUseAndDef& out);
    template <size_t> bool hasUseAs_CORE(const std::string& name) const;
    template <size_t> bool hasDefAs_CORE(const std::string& name) const;
};

std::ostream& operator<<(std::ostream& os, const NameCatalog& catalog);

} // C
} // psy

#endif

