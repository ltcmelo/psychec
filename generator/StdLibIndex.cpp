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

#include "StdLibIndex.h"
#include "Control.h"
#include <algorithm>
#include <cstring>
#include <iterator>
#include <unordered_set>

using namespace psyche;
using namespace CPlusPlus;

// C89/90
const StdLibIndex::Index StdLibIndex::c89idx_
    {
        { "assert.h",
            {
                { "assert", SymbolKind::Value }
            }
        },
        { "errno.h",
            {
                { "errno", SymbolKind::Value }
            }
        },
        { "ctype.h",
            {
                { "isalnum", SymbolKind::Value },
                { "isalpha", SymbolKind::Value },
                { "islower", SymbolKind::Value },
                { "isupper", SymbolKind::Value },
                { "isdigit", SymbolKind::Value },
                { "isxdigit", SymbolKind::Value },
                { "iscntrl", SymbolKind::Value },
                { "isgraph", SymbolKind::Value },
                { "isspace", SymbolKind::Value },
                { "isprint", SymbolKind::Value },
                { "ispunch", SymbolKind::Value },
                { "tolower", SymbolKind::Value },
                { "toupper", SymbolKind::Value }
            }
        },
        { "stdlib.h",
            {
                { "atof", SymbolKind::Value },
                { "atoi", SymbolKind::Value },
                { "atol", SymbolKind::Value },
                { "strtol", SymbolKind::Value },
                { "strtoul", SymbolKind::Value },
                { "strtod", SymbolKind::Value }
            }
        },
        { "string.h",
            {
                { "strcpy", SymbolKind::Value },
                { "strncpy", SymbolKind::Value },
                { "strcat", SymbolKind::Value },
                { "strncat", SymbolKind::Value },
                { "strxfrm", SymbolKind::Value },
                { "strlen", SymbolKind::Value },
                { "strcmp", SymbolKind::Value },
                { "strncmp", SymbolKind::Value },
                { "strcoll", SymbolKind::Value },
                { "strchr", SymbolKind::Value },
                { "strspn", SymbolKind::Value },
                { "strcspn", SymbolKind::Value }
            }
        }
    };

// C99
const StdLibIndex::Index StdLibIndex::c99idx_
    {
        { "ctype.h",
            {
                { "isblank", SymbolKind::Value }
            }
        },
        { "stdlib.h",
            {
                { "atoll", SymbolKind::Value },
                { "strtoll", SymbolKind::Value },
                { "strtoull", SymbolKind::Value },
                { "strtof", SymbolKind::Value },
                { "strtold", SymbolKind::Value }
            }
        }
    };

// C11
const StdLibIndex::Index StdLibIndex::c11idx_
    {
        { "assert.h",
            {
                { "static_assert", SymbolKind::Value }
            }
        },
        { "errno.h",
            {
                { "errno_t", SymbolKind::Type }
            }
        },
        { "string.h",
            {
                { "strcpy_s", SymbolKind::Value },
                { "strncpy_s", SymbolKind::Value },
                { "strcat_s", SymbolKind::Value },
                { "strncat_s", SymbolKind::Value },
                { "strnlen_s", SymbolKind::Value }
            }
        }
    };


StdLibIndex::StdLibIndex(Version std)
    : std_(std)
{}

std::vector<std::string> StdLibIndex::inspect(const Control& control) const
{
    std::unordered_set<std::string> deps;

    auto f = [&deps, &control] (const Index& idx) {
        for (const auto& v : idx) {
            if (deps.count(v.first)) // Maybe the header has already been inserted.
                continue;
            for (const auto& s : v.second) {
                if (control.findIdentifier(s.first, strlen(s.first))) {
                    deps.insert(v.first);
                    break;
                }
            }
        }
    };

    f(c89idx_);
    if (std_ == Version::C99)
        f(c99idx_);
    if (std_ == Version::C11)
        f(c11idx_);

    std::vector<std::string> v;
    std::move(deps.begin(), deps.end(), std::back_inserter(v));
    return v;
}
