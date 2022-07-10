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

#include "DisambiguationCatalogue.h"

#include "syntax/SyntaxNode.h"

#include "../common/infra/Assertions.h"
#include "../common/infra/Escape.h"

using namespace psy;
using namespace C;

#include <algorithm>

DisambiguationCatalogue::~DisambiguationCatalogue()
{}

void DisambiguationCatalogue::createLevelAndEnter(const SyntaxNode* node)
{
    PSY_ASSERT(node, return);
    PSY_ASSERT(!levelExists(node), return);

    T t;
    V v;
    if (!levelKeys_.empty()) {
        auto levelIt = levels_.find(levelKeys_.top());
        PSY_ASSERT(levelIt != levels_.end(), return);

        t = levelIt->second.first;
        v = levelIt->second.second;
    }

    levels_.insert(std::make_pair(node, std::make_pair(t, v)));

    enterLevel(node);
}

void DisambiguationCatalogue::enterLevel(const SyntaxNode* node)
{
    PSY_ASSERT(node, return);
    PSY_ASSERT(levelExists(node), return);

    levelKeys_.push(node);
}

void DisambiguationCatalogue::exitLevel()
{
    levelKeys_.pop();
}

void DisambiguationCatalogue::addT(std::string s)
{
    auto level = currentLevel();
    level->first.insert(s);
}

void DisambiguationCatalogue::addV(std::string s)
{
    auto level = currentLevel();
    level->second.insert(s);
}

bool DisambiguationCatalogue::levelExists(const SyntaxNode* node) const
{
    return levels_.count(node) != 0;
}

DisambiguationCatalogue::TV* DisambiguationCatalogue::currentLevel()
{
    PSY_ASSERT(!levelKeys_.empty(), return nullptr);
    PSY_ASSERT(levelExists(levelKeys_.top()), return nullptr);

    return &levels_[levelKeys_.top()];
}

namespace psy {
namespace C {

std::ostream& operator<<(const DisambiguationCatalogue dc, std::ostream& os)
{
    for (const auto& p : dc.levels_) {
        os << to_string(p.first->kind()) << std::endl;
        os << "\tT" << std::endl;
        for (const auto& t : p.second.first)
            os << "\t\t" << t << std::endl;
        os << "\tV" << std::endl;
        for (const auto& v : p.second.second)
            os << "\t\t" << v << std::endl;
        os << std::endl;
    }
    return os;
}

} // C
} // psy
