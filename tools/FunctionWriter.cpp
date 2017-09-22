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

#include "FunctionWriter.h"

using namespace psyche;
using namespace psyche;

std::string FunctionWriter::writeFunction(psyche::Function* f, const std::string name)
{
    this->spelling_.clear();
    this->funcRet(f);
    this->spelling_.append(" " + name);
    this->funcParam(f);
    return this->spelling_;
}

void FunctionWriter::funcParam(Function* f)
{
    int cnt = 0;
    spelling_.append(CSyntax::paramLDelim());
    if (f->argumentCount()) {
        visitType(f->argumentAt(0)->type());
        spelling_.append(" p" + std::to_string(cnt++));
        for (auto i = 1u; i < f->argumentCount(); ++i) {
            spelling_.append(", ");
            visitType(f->argumentAt(i)->type());
            spelling_.append(" p" + std::to_string(cnt++));
        }
    }
    spelling_.append(CSyntax::paramRDelim());
}
