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

#include "Process.h"
#include <array>
#include <cstdio>  // Using POSIX's `popen'.
#include <iostream>

using namespace psyche;

namespace {

std::pair<int, std::string> executeCore(const char* cmd)
{
    FILE* pipe = popen(cmd, "r");
    if (!pipe)
        return std::make_pair(1, "");

    std::string all;
    while (!feof(pipe)) {
        std::array<char, 512> buf;
        if (fgets(buf.data(), 512, pipe))
            all += buf.data();
    }

    return std::make_pair(pclose(pipe), all);
}

} // anonymous

std::pair<int, std::string> Process::execute(std::string&& s)
{
    return executeCore(s.c_str());
}

std::pair<int, std::string> Process::execute(const std::string &s)
{
    return executeCore(s.c_str());
}
