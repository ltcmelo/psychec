/******************************************************************************
 Copyright (c) 2016-17 Leandro T. C. Melo (ltcmelo@gmail.com)

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

#include "CompilerFacade.h"
#include "Process.h"
#include <iostream>

using namespace psyche;

CompilerFacade::CompilerFacade(const std::string& nativeCC,
                               const std::vector<std::string>& macros)
    : nativeCC_(nativeCC)  // TODO: Native compiler support validation.
    , macros_(macros)
{}

std::string CompilerFacade::preprocessSource(const std::string& source)
{
    std::string in = "echo \"" + source + "\" | ";
    in += nativeCC_;
    in += predefinedMacros();
    in += " -E -x c -";

    return Process().execute(in);
}

std::string CompilerFacade::predefinedMacros() const
{
    std::string s;
    for (const auto& m : macros_)
        s += " -D " + m;
    return s;
}
