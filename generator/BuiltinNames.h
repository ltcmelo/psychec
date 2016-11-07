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

#ifndef PSYCHE_BUILTINNAMES_H__
#define PSYCHE_BUILTINNAMES_H__

namespace psyche {

const char* const kVoidTy = "void";

const char* const kCharTy = "char";
const char* const kShortTy = "short";
const char* const kIntTy = "int";
const char* const kLongTy = "long";
const char* const kLongLongTy = "long long";

const char* const kFloatTy = "float";
const char* const kDoubleTy = "double";
const char* const kLongDouble = "long double";

const char* const kDefaultFloatPointTy = kDoubleTy;
const char* const kDefaultIntTy = kIntTy;
const char* const kDefaultArithTy = kDefaultIntTy;
const char* const kDefaultStrTy = "char*";
const char* const kDefaultConstStrTy = "char const*";

} // namepspace psyche

#endif
