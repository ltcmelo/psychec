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
#include <memory>

using namespace psyche;

namespace {

std::string executeCore(const char* cmd)
{
    std::shared_ptr<FILE> out(popen(cmd, "r"), pclose);
    if (!out)
        return "";

    std::string all;
    while (!feof(out.get())) {
        std::array<char, 512> buf;
        if (fgets(buf.data(), 512, out.get()))
            all += buf.data();
    }

    std::cout << "RESULT:" << all << std::endl;
    return all;
}

} // anonymous

std::string Process::execute(std::string&& s)
{
    return executeCore(s.c_str());
}

std::string Process::execute(const std::string &s)
{
    return executeCore(s.c_str());
}
