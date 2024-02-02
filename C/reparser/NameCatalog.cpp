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

constexpr int NameCatalog::tyIdx_;
constexpr int NameCatalog::nonTyIdx_;

NameCatalog::~NameCatalog()
{}

void NameCatalog::mapNodeAndMarkAsEncloser(const SyntaxNode* node)
{
    PSY_ASSERT(node, return);
    PSY_ASSERT(!isNodeMapped(node), return);

    NameUseAndDef tyUseAndDef;
    NameUseAndDef nonTyUseAndDef;
    if (!enclosersStack_.empty()) {
        auto iter = namesByNode_.find(enclosersStack_.top());
        PSY_ASSERT(iter != namesByNode_.end(), return);
        tyUseAndDef = std::get<tyIdx_>(iter->second);
        nonTyUseAndDef = std::get<nonTyIdx_>(iter->second);
    }

    namesByNode_.insert(
            std::make_pair(
                    node,
                    std::make_tuple(tyUseAndDef, nonTyUseAndDef)));

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

void NameCatalog::catalogUseAsTypeName(const std::string& name)
{
    catalogUse_CORE<tyIdx_, nonTyIdx_>(name);
}

void NameCatalog::catalogUseAsNonTypeName(const std::string& name)
{
    catalogUse_CORE<nonTyIdx_, tyIdx_>(name);
}

template <size_t inIdx, size_t outIdx>
void NameCatalog::catalogUse_CORE(const std::string& name)
{
    auto enclosure = currentEnclosure();
    catalogUseWithMutualExclusion(
                name,
                std::get<inIdx>(*enclosure),
                std::get<outIdx>(*enclosure));
}

void NameCatalog::catalogUseWithMutualExclusion(const std::string& name,
                                                NameCatalog::NameUseAndDef& in,
                                                NameCatalog::NameUseAndDef& out)
{
    in.insert(std::make_pair(name, false));

    auto iter = out.find(name);
    if (iter != out.end())
        out.erase(iter);
}

void NameCatalog::catalogDefAsTypeName(const std::string& name)
{
    PSY_ASSERT(hasUseAsTypeName(name), return);
    catalogDef_CORE<tyIdx_>(name);
}

void NameCatalog::catalogDefAsNonTypeName(const std::string& name)
{
    PSY_ASSERT(hasUseAsNonTypeName(name), return);
    catalogDef_CORE<nonTyIdx_>(name);
}

template <size_t idx>
void NameCatalog::catalogDef_CORE(const std::string& name)
{
    auto enclosure = currentEnclosure();
    std::get<idx>(*enclosure)[name] = true;
}

bool NameCatalog::hasUseAsTypeName(const std::string& name) const
{
    return hasUseAs_CORE<tyIdx_>(name);
}

bool NameCatalog::hasUseAsNonTypeName(const std::string &name) const
{
    return hasUseAs_CORE<nonTyIdx_>(name);
}

template <size_t idx>
bool NameCatalog::hasUseAs_CORE(const std::string& name) const
{
    auto enclosure = currentEnclosure();
    const auto& useAndDef = std::get<idx>(*enclosure);
    return useAndDef.find(name) != useAndDef.end();
}

bool NameCatalog::hasDefAsNonTypeName(const std::string& name) const
{
    if (!hasUseAsNonTypeName(name))
        return false;
    return hasDefAs_CORE<nonTyIdx_>(name);
}

bool NameCatalog::hasDefAsTypeName(const std::string& name) const
{
    if (!hasUseAsTypeName(name))
        return false;
    return hasDefAs_CORE<tyIdx_>(name);
}

template <size_t idx>
bool NameCatalog::hasDefAs_CORE(const std::string& name) const
{
    auto enclosure = currentEnclosure();
    const auto& useAndDef = std::get<idx>(*enclosure);
    auto iter = useAndDef.find(name);
    if (iter == useAndDef.end())
        return false;
    return iter->second;
}

bool NameCatalog::isNodeMapped(const SyntaxNode* node) const
{
    return namesByNode_.count(node) != 0;
}

NameCatalog::Enclosure* NameCatalog::currentEnclosure() const
{
    PSY_ASSERT(!enclosersStack_.empty(), return nullptr);
    PSY_ASSERT(isNodeMapped(enclosersStack_.top()), return nullptr);

    return &namesByNode_[enclosersStack_.top()];
}

namespace psy {
namespace C {

std::ostream& operator<<(std::ostream& os, const NameCatalog& catalog)
{
    os << "\n----------------------------------"
       << "\n---------- Name Catalog ----------"
       << "\n----------------------------------";
    for (const auto& p : catalog.namesByNode_) {
        os << "\n-" << to_string(p.first->kind()) << std::endl;
        os << "\tType names: ";
        for (const auto& t : std::get<NameCatalog::tyIdx_>(p.second))
            os << t.first << " ";
        std::cout << std::endl;
        os << "\tNon-type names: ";
        for (const auto& v : std::get<NameCatalog::nonTyIdx_>(p.second))
            os << v.first << " ";
    }
    os << "\n----------------------------------";

    return os;
}

} // C
} // psy
