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


-- | Module corresponding to header <stdlib.h>


module Data.StdLib where

import Data.Type
import Data.BuiltIn
import Data.WChar


-- | Types
sig_atomic_t :: Ty
sig_atomic_t = NamedTy (Name "sig_atomic_t")


-- | Collection of types
stdlibTypes :: [Ty]
stdlibTypes = [sig_atomic_t]


-- | Collection of values.
stdlibValues :: [(Name, Ty)]
stdlibValues =
  [ (Name "abort", FunTy void [])
  , (Name "exit", FunTy void [int])
  , (Name "quick_exit", FunTy void [int])
  , (Name "_Exit", FunTy void [int])
  , (Name "atexit", FunTy void [PtrTy (FunTy void [])])
  , (Name "at_quick_exit", FunTy void [PtrTy (FunTy void [])])
  , (Name "EXIT_SUCCESS", int)
  , (Name "EXIT_FAILURE", int)
  , (Name "system", FunTy int [charConstPtr])
  , (Name "getenv", FunTy charPtr [charConstPtr])
  , (Name "signal", FunTy (PtrTy (FunTy void [int])) [int, (PtrTy (FunTy void [int]))])
  , (Name "malloc", FunTy (PtrTy void) [size_t])
  , (Name "calloc", FunTy (PtrTy void) [size_t, size_t])
  , (Name "realloc", FunTy (PtrTy void) [(PtrTy void), size_t])
  , (Name "free", FunTy void [(PtrTy void)])
  , (Name "atof", FunTy double [PtrTy (constTy char)])
  , (Name "atoi", FunTy int [PtrTy (constTy char)])
  , (Name "atol", FunTy long [PtrTy (constTy char)])
  , (Name "atoll", FunTy longLong [PtrTy (constTy char)])
  , (Name "strtol", FunTy long [charConstPtr, PtrTy charPtr, int])
  , (Name "strtoll", FunTy long [charConstPtr, PtrTy charPtr, int])
  , (Name "strtoul", FunTy unsignedLong [charConstPtr, PtrTy charPtr, int])
  , (Name "strtoull", FunTy unsignedLongLong [charConstPtr, PtrTy charPtr, int])
  , (Name "strtof", FunTy float [charConstPtr, PtrTy charPtr])
  , (Name "strtod", FunTy double [charConstPtr, PtrTy charPtr])
  , (Name "strtold", FunTy longDouble [charConstPtr, PtrTy charPtr])
  , (Name "mblen", FunTy int [charConstPtr, size_t])
  , (Name "mbtowc", FunTy wchar_t [charConstPtr])
  , (Name "wctomb_s", FunTy int [charPtr, wchar_t])
  , (Name "mbstowcs", FunTy size_t [wchar_t, charConstPtr, size_t])
  , (Name "wcstombs", FunTy size_t [charPtr, PtrTy (constTy wchar_t), size_t])
  , (Name "rand", FunTy int [])
  , (Name "srand", FunTy void [unsignedInt])
  , (Name "RAND_MAX", int)
  ]
