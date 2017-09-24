/******************************************************************************
 * Copyright (c) 2016 Leandro T. C. Melo (ltcmelo@gmail.com)
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301
 * USA
 *****************************************************************************/

#ifndef PSYCHE_SYMBOLPP__
#define PSYCHE_SYMBOLPP__

#include "FrontendConfig.h"
#include "SymbolVisitor.h"
#include <string>
#include <sstream>

namespace psyche {

class CFE_API SymbolPP final : public psyche::SymbolVisitor
{
public:
    std::string print(const psyche::Symbol * symbol);

private:
     bool visit(psyche::Declaration *) override;
     bool visit(psyche::Argument *) override;
     bool visit(psyche::Function *) override;
     bool visit(psyche::Class *) override;
     bool visit(psyche::Block *) override;

     void appendName(const psyche::Name* name);

     std::ostringstream name_;
};

} // namespace psyche

#endif
