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

using namespace psy;
using namespace C;

#include <algorithm>
#include <iostream>

constexpr int NameCatalog::Types;
constexpr int NameCatalog::NonTypes;

NameCatalog::~NameCatalog()
{}

void NameCatalog::indexNodeAndMarkAsEncloser(const SyntaxNode* node)
{
    PSY_ASSERT_2(node, return);
    PSY_ASSERT_2(!isIndexed(node), return);

    NameUseAndDef tyUseAndDef;
    NameUseAndDef nonTyUseAndDef;
    if (!enclosureStack_.empty()) {
        auto iter = enclosureIdx_.find(enclosureStack_.top());
        PSY_ASSERT_2(iter != enclosureIdx_.end(), return);
        tyUseAndDef = std::get<Types>(iter->second);
        nonTyUseAndDef = std::get<NonTypes>(iter->second);
    }

    enclosureIdx_.insert(
            std::make_pair(
                    node,
                    std::make_tuple(tyUseAndDef, nonTyUseAndDef)));

    markIndexedNodeAsEncloser(node);
}

void NameCatalog::markIndexedNodeAsEncloser(const SyntaxNode* node)
{
    PSY_ASSERT_2(node, return);
    PSY_ASSERT_2(isIndexed(node), return);

    enclosureStack_.push(node);
}

void NameCatalog::dropEncloser()
{
    enclosureStack_.pop();
}

void NameCatalog::catalogUseAsTypeName(const std::string& name)
{
    catalogUse_CORE<Types, NonTypes>(name);
}

void NameCatalog::catalogUseAsNonTypeName(const std::string& name)
{
    catalogUse_CORE<NonTypes, Types>(name);
}

template <size_t UseAndDef, size_t OtherUseAndDef>
void NameCatalog::catalogUse_CORE(const std::string& name)
{
    auto enclosure = currentEnclosure();
    auto curDepth = enclosureStack_.size();
    auto& useAndDef = std::get<UseAndDef>(*enclosure);
    useAndDef.insert(std::make_pair(name, std::make_pair(false, curDepth)));
    auto& otherUseAndDef = std::get<OtherUseAndDef>(*enclosure);
    auto iter = otherUseAndDef.find(name);
    if (iter != otherUseAndDef.end() && curDepth > iter->second.second)
        otherUseAndDef.erase(iter);
}

void NameCatalog::catalogDefAsTypeName(const std::string& name)
{
    PSY_ASSERT_2(hasUseAsTypeName(name), return);
    catalogDef_CORE<Types>(name);
}

void NameCatalog::catalogDefAsNonTypeName(const std::string& name)
{
    PSY_ASSERT_2(hasUseAsNonTypeName(name), return);
    catalogDef_CORE<NonTypes>(name);
}

template <size_t idx>
void NameCatalog::catalogDef_CORE(const std::string& name)
{
    auto enclosure = currentEnclosure();
    std::get<idx>(*enclosure)[name].first = true;
}

bool NameCatalog::hasUseAsTypeName(const std::string& name) const
{
    return hasUseAs_CORE<Types>(name);
}

bool NameCatalog::hasUseAsNonTypeName(const std::string &name) const
{
    return hasUseAs_CORE<NonTypes>(name);
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
    return hasDefAs_CORE<NonTypes>(name);
}

bool NameCatalog::hasDefAsTypeName(const std::string& name) const
{
    if (!hasUseAsTypeName(name))
        return false;
    return hasDefAs_CORE<Types>(name);
}

template <size_t idx>
bool NameCatalog::hasDefAs_CORE(const std::string& name) const
{
    auto enclosure = currentEnclosure();
    const auto& useAndDef = std::get<idx>(*enclosure);
    auto iter = useAndDef.find(name);
    if (iter == useAndDef.end())
        return false;
    return iter->second.first;
}

bool NameCatalog::isIndexed(const SyntaxNode* node) const
{
    return enclosureIdx_.count(node) != 0;
}

NameCatalog::Enclosure* NameCatalog::currentEnclosure() const
{
    PSY_ASSERT_2(!enclosureStack_.empty(), return nullptr);
    PSY_ASSERT_2(isIndexed(enclosureStack_.top()), return nullptr);

    return &enclosureIdx_[enclosureStack_.top()];
}

namespace psy {
namespace C {

std::ostream& operator<<(std::ostream& os, const NameCatalog& catalog)
{
    os << "\n----------------------------------"
       << "\n---------- Name Catalog ----------"
       << "\n----------------------------------";
    for (const auto& p : catalog.enclosureIdx_) {
        os << "\n-" << to_string(p.first->kind()) << std::endl;
        os << "\tType names: ";
        for (const auto& t : std::get<NameCatalog::Types>(p.second))
            os << t.first << " ";
        std::cout << std::endl;
        os << "\tNon-type names: ";
        for (const auto& v : std::get<NameCatalog::NonTypes>(p.second))
            os << v.first << " ";
    }
    os << "\n----------------------------------";

    return os;
}

} // C
} // psy
