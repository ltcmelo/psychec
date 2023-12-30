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
#include <unordered_map>
#include <unordered_set>
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

    void mapNodeAndMarkAsEncloser(const SyntaxNode*);
    void markMappedNodeAsEncloser(const SyntaxNode*);
    void dropEncloser();

    void catalogTypeName(const std::string& s);
    void catalogNonTypeName(const std::string& s);

    bool isNameEnclosedAsTypeName(const std::string& s) const;
    bool isNameEnclosedAsNonTypeName(const std::string& s) const;

private:
    std::stack<const SyntaxNode*> enclosersStack_;

    using NameContainer = std::unordered_set<std::string>;
    using TypeNamesAndNames = std::pair<NameContainer, NameContainer>;

    TypeNamesAndNames* enclosedNames() const;
    void catalogName(const std::string& s, NameContainer& in, NameContainer& out);
    bool isNameEnclosed(const std::string& s, const NameContainer& container) const;

    using NamesByNode = std::unordered_map<const SyntaxNode*, TypeNamesAndNames>;

    mutable NamesByNode namesByNode_;
    bool isNodeMapped(const SyntaxNode*) const;
};

std::ostream& operator<<(std::ostream& os, const NameCatalog& disambigCatalog);

} // C
} // psy

#endif

