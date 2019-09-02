/******************************************************************************
 Copyright (c) 2017 Leandro T. C. Melo (ltcmelo@gmail.com)

 This library is free software; you can redistribute it and/or modify it under
 the terms of the GNU Lesser General Public License as published by the Free
 Software Foundation; either version 2.1 of the License, or (at your option)
 any later version.

 This library is distributed in the hope that it will be useful, but WITHOUT
 ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License
 for more details.

 You should have received a copy of the GNU Lesser General Public License along
 with this library; if not, write to the Free Software Foundation, Inc., 51
 Franklin Street, Fifth Floor, Boston, MA  02110-1301 USA
 *****************************************************************************/

#ifndef PSYCHE_STDLIB_INDEX_H__
#define PSYCHE_STDLIB_INDEX_H__

#include "FrontendFwds.h"
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

namespace psyche {

class StdLibIndex final
{
public:
    enum class Version
    {
        C89,
        C99,
        C11
    };

    StdLibIndex(Version std);

    std::vector<std::string> inspect(const Control&) const;

    bool recognizes(const char* ident) const;

private:
    enum class SymbolKind : char
    {
        Type,
        Value
    };
    using StdSymbol = std::pair<const char*, SymbolKind>;
    using Index = std::unordered_map<const char*, std::vector<StdSymbol>>;

    Version std_;
    static const Index c89idx_;
    static const Index c99idx_;
    static const Index c11idx_;
};

} // namespace psyche

#endif
