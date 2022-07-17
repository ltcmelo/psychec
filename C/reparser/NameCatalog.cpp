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

#include "NameCatalog.h"

#include "syntax/SyntaxNode.h"

#include "../common/infra/Assertions.h"
#include "../common/infra/Escape.h"

using namespace psy;
using namespace C;

#include <algorithm>

NameCatalog::~NameCatalog()
{}

void NameCatalog::createLevelAndEnter(const SyntaxNode* node)
{
    PSY_ASSERT(node, return);
    PSY_ASSERT(!levelExists(node), return);

    TypeNames types;
    Names nonTypes;
    if (!levelKeys_.empty()) {
        auto levelIt = levels_.find(levelKeys_.top());
        PSY_ASSERT(levelIt != levels_.end(), return);

        types = levelIt->second.first;
        nonTypes = levelIt->second.second;
    }

    levels_.insert(std::make_pair(node, std::make_pair(types, nonTypes)));

    enterLevel(node);
}

void NameCatalog::enterLevel(const SyntaxNode* node)
{
    PSY_ASSERT(node, return);
    PSY_ASSERT(levelExists(node), return);

    levelKeys_.push(node);
}

void NameCatalog::exitLevel()
{
    levelKeys_.pop();
}

void NameCatalog::catalogTypeName(std::string s)
{
    auto level = currentLevel();
    level->first.insert(std::move(s));
}

void NameCatalog::catalogName(std::string s)
{
    auto level = currentLevel();
    level->second.insert(std::move(s));
}

bool NameCatalog::containsTypeName(const std::string& s) const
{
    auto level = currentLevel();
    return level->first.find(s) != level->first.end();
}

bool NameCatalog::containsName(const std::string &s) const
{
    auto level = currentLevel();
    return level->second.find(s) != level->first.end();
}

bool NameCatalog::levelExists(const SyntaxNode* node) const
{
    return levels_.count(node) != 0;
}

NameCatalog::NameIndex* NameCatalog::currentLevel() const
{
    PSY_ASSERT(!levelKeys_.empty(), return nullptr);
    PSY_ASSERT(levelExists(levelKeys_.top()), return nullptr);

    return &levels_[levelKeys_.top()];
}

namespace psy {
namespace C {

std::ostream& operator<<(std::ostream& os, const NameCatalog& disambigCatalog)
{
    for (const auto& p : disambigCatalog.levels_) {
        os << to_string(p.first->kind()) << std::endl;
        os << "\tTypes: ";
        for (const auto& t : p.second.first)
            os << t << " ";
        std::cout << std::endl;
        os << "\tAny: ";
        for (const auto& v : p.second.second)
            os << v << " ";
        os << std::endl;
    }
    return os;
}

} // C
} // psy
