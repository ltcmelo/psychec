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

    void createLevelAndEnter(const SyntaxNode*);
    void enterLevel(const SyntaxNode*);
    void exitLevel();

    void catalogTypeName(std::string s);
    void catalogName(std::string s);

    bool containsTypeName(const std::string& s) const;
    bool containsName(const std::string& s) const;

private:
    using TypeNames = std::unordered_set<std::string>;
    using Names = std::unordered_set<std::string>;
    using NameIndex = std::pair<TypeNames, Names>;

    using Levels = std::unordered_map<const SyntaxNode*, NameIndex>;

    mutable Levels levels_;
    std::stack<const SyntaxNode*> levelKeys_;

    bool levelExists(const SyntaxNode*) const;
    NameIndex* currentLevel() const;
};

std::ostream& operator<<(std::ostream& os, const NameCatalog& disambigCatalog);

} // C
} // psy

#endif

