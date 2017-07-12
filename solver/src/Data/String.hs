-- Copyright (c) 2016 Leandro T. C. Melo (ltcmelo@gmail.com)
--
-- This library is free software; you can redistribute it and/or modify it under
-- the terms of the GNU Lesser General Public License as published by the Free
-- Software Foundation; either version 2.1 of the License, or (at your option)
-- any later version.
--
-- This library is distributed in the hope that it will be useful, but WITHOUT
-- ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
-- FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License
-- for more details.
--
-- You should have received a copy of the GNU Lesser General Public License
-- along with this library; if not, write to the Free Software Foundation, Inc.,
-- 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301 USA


-- | Module corresponding to header <string.h>


module Data.String where

import Data.Type
import Data.BuiltIn
import Data.StdDef


-- | Collection of values.
stringValues =
  [ (Name "memmove", FunTy (PtrTy void) [PtrTy void, PtrTy (constTy void), size_t])
    , (Name "memcpy", FunTy (PtrTy void) [PtrTy void, PtrTy (constTy void), size_t])
    , (Name "memcmp", FunTy int [PtrTy (constTy void), PtrTy (constTy void), size_t])
    , (Name "strlen", FunTy size_t [PtrTy (constTy char)])
    , (Name "stpcpy", FunTy (PtrTy char) [PtrTy char, PtrTy (constTy char)])
    , (Name "strcpy", FunTy (PtrTy char) [PtrTy char, PtrTy (constTy char)])
    , (Name "strncpy", FunTy (PtrTy char) [PtrTy char, PtrTy (constTy char), size_t])
    , (Name "strcat", FunTy (PtrTy char) [PtrTy char, PtrTy (constTy char)])
    , (Name "strncat", FunTy (PtrTy char) [PtrTy char, PtrTy (constTy char), size_t])
    , (Name "strcmp", FunTy int [PtrTy (constTy char), PtrTy (constTy char)])
    , (Name "strtok", FunTy (PtrTy char) [PtrTy char, PtrTy (constTy char)])
    , (Name "strtof", FunTy float [PtrTy (constTy char), PtrTy (PtrTy char)])
    , (Name "strtod", FunTy double [PtrTy (constTy char), PtrTy (PtrTy char)])
    , (Name "strtold", FunTy longDouble [PtrTy (constTy char), PtrTy (PtrTy char)])
    ]
