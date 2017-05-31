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

#include "IO.h"
#include <fstream>
#include <iostream>
#include <sstream>

namespace psyche {

std::string readFile(const std::string& fileName)
{
    std::ifstream ifs(fileName);
    if (!ifs.is_open()) {
        std::cerr << "File not found: " << fileName << std::endl;
        return std::string();
    }

    std::stringstream ss;
    ss << ifs.rdbuf();
    ifs.close();
    return ss.str();
}

void writeFile(const std::string& content, const std::string& fileName)
{
    std::ofstream ofs(fileName);
    ofs << content;
    ofs.close();
}

} // namespace psyche
