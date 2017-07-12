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

-- | Collection of values.
stdlibValues :: [(Name, Ty)]
stdlibValues
  = [ (Name "malloc", FunTy (PtrTy void) [int])
    , (Name "calloc", FunTy (PtrTy void) [int, int])
    , (Name "realloc", FunTy (PtrTy void) [(PtrTy void), int])
    , (Name "free", FunTy void [(PtrTy void)])
    , (Name "abort", FunTy void [])
    , (Name "exit", FunTy void [int])
    , (Name "atexit", FunTy void [(FunTy void [])])
    , (Name "atoi", FunTy int [PtrTy (constTy char)])
    , (Name "atof", FunTy double [PtrTy (constTy char)])
    , (Name "atol", FunTy long [PtrTy (constTy char)])
    , (Name "atoll", FunTy longLong [PtrTy (constTy char)])
    , (Name "rand", FunTy int [])
    , (Name "RAND_MAX", int)
    , (Name "EXIT_SUCCESS", int)
    , (Name "EXIT_FAILURE", int)
    ]
