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
#include <iostream>

NameCatalog::~NameCatalog()
{}

void NameCatalog::mapNodeAndMarkAsEncloser(const SyntaxNode* node)
{
    PSY_ASSERT(node, return);
    PSY_ASSERT(!isNodeMapped(node), return);

    NameSet tyNames;
    NameSet nonTyNames;
    if (!enclosersStack_.empty()) {
        auto iter = namesByNode_.find(enclosersStack_.top());
        PSY_ASSERT(iter != namesByNode_.end(), return);

        tyNames = std::get<0>(iter->second);
        nonTyNames = std::get<1>(iter->second);
    }

    namesByNode_.insert(
            std::make_pair(
                    node,
                    std::make_tuple(tyNames, nonTyNames, false)));

    markMappedNodeAsEncloser(node);
}

void NameCatalog::markMappedNodeAsEncloser(const SyntaxNode* node)
{
    PSY_ASSERT(node, return);
    PSY_ASSERT(isNodeMapped(node), return);

    enclosersStack_.push(node);
}

void NameCatalog::dropEncloser()
{
    enclosersStack_.pop();
}

void NameCatalog::catalogTypeName(const std::string& s)
{
    auto enclosure = currentNameEnclosure();
    catalogName(s, std::get<0>(*enclosure), std::get<1>(*enclosure));
}

void NameCatalog::catalogNonTypeName(const std::string& s)
{
    auto enclosure = currentNameEnclosure();
    catalogName(s, std::get<1>(*enclosure), std::get<0>(*enclosure));
}

void NameCatalog::flagAsVariableName(const std::string& s)
{
    PSY_ASSERT(isNameEnclosedAsNonTypeName(s), return);

    auto enclosure = currentNameEnclosure();
    std::get<2>(*enclosure) = true;
}

void NameCatalog::catalogName(const std::string& s,
                              NameCatalog::NameSet& toInclude,
                              NameCatalog::NameSet& toExclude)
{
    toInclude.insert(s);

    auto excludeIt = toExclude.find(s);
    if (excludeIt != toExclude.end())
        toExclude.erase(excludeIt);
}

bool NameCatalog::isNameEnclosedAsTypeName(const std::string& s) const
{
    auto enclosure = currentNameEnclosure();
    return isNameEnclosed(s, std::get<0>(*enclosure));
}

bool NameCatalog::isNameEnclosedAsNonTypeName(const std::string &s) const
{
    auto enclosure = currentNameEnclosure();
    return isNameEnclosed(s, std::get<1>(*enclosure));
}

bool NameCatalog::isNameEnclosed(const std::string& s,
                                 const NameCatalog::NameSet& names) const
{
    return names.find(s) != names.end();
}

bool NameCatalog::isVariableName(const std::string& s) const
{
    if (!isNameEnclosedAsNonTypeName(s))
        return false;

    auto enclosure = currentNameEnclosure();
    return std::get<2>(*enclosure);
}

bool NameCatalog::isNodeMapped(const SyntaxNode* node) const
{
    return namesByNode_.count(node) != 0;
}

NameCatalog::NameEnclosure* NameCatalog::currentNameEnclosure() const
{
    PSY_ASSERT(!enclosersStack_.empty(), return nullptr);
    PSY_ASSERT(isNodeMapped(enclosersStack_.top()), return nullptr);

    return &namesByNode_[enclosersStack_.top()];
}

namespace psy {
namespace C {

std::ostream& operator<<(std::ostream& os, const NameCatalog& disambigCatalog)
{
    os << "\n----------------------------------"
       << "\n---------- Name Catalog ----------"
       << "\n----------------------------------";
    for (const auto& p : disambigCatalog.namesByNode_) {
        os << "\n-" << to_string(p.first->kind()) << std::endl;
        os << "\tType names: ";
        for (const auto& t : std::get<0>(p.second))
            os << t << " ";
        std::cout << std::endl;
        os << "\tNames: ";
        for (const auto& v : std::get<1>(p.second))
            os << v << " ";
    }
    os << "\n----------------------------------";

    return os;
}

} // C
} // psy
