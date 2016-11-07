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

#include "FreshVarSupply.h"

using namespace psyche;

const std::string FreshVarSupply::typeVarPrefix_ = "#alpha";

FreshVarSupply::FreshVarSupply()
    : typeVarCount_(0)
{}

std::string FreshVarSupply::createTypeVar1()
{
    int count = ++typeVarCount_;
    return typeVarPrefix_ + std::to_string(count);
}

std::tuple<std::string, std::string> FreshVarSupply::createTypeVar2()
{
    // Create the first one to make sure sequential ordering (remember
    // argument evaluation order in C++ is implementation-defined).
    const std::string& alpha = createTypeVar1();
    return std::make_tuple(alpha, createTypeVar1());
}

std::tuple<std::string, std::string, std::string> FreshVarSupply::createTypeVar3()
{
    const std::string& alpha = createTypeVar1();
    return std::tuple_cat(std::make_tuple(alpha), createTypeVar2());
}

void FreshVarSupply::resetCounter()
{
    typeVarCount_ = 0;
}
