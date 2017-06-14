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


-- | Module corresponding to header <stdio.h>


module Data.StdIO where

import Data.Type
import Data.BuiltIn
import Data.StdDef


-- | Types
file :: Ty
file = NamedTy (Name "FILE")


-- | Collection of types.
stdioTypes :: [Ty]
stdioTypes = [file]


-- | Collection of values.
stdioValues :: [(Name, Ty)]
stdioValues =
    [ (Name "fclose", FunTy int [PtrTy file])
    , (Name "feof", FunTy int [PtrTy file])
    , (Name "ferror", FunTy int [PtrTy file])
    , (Name "fputc", FunTy int [int, PtrTy file])
    , (Name "fputs", FunTy int [PtrTy (QualTy char), PtrTy file])
    , (Name "putchar", FunTy int [int])
    , (Name "fopen", FunTy (PtrTy file) [PtrTy (QualTy char), PtrTy (QualTy char)])
    , (Name "fread", FunTy size_t [PtrTy void, size_t, size_t, PtrTy file])
    , (Name "fwrite", FunTy size_t [PtrTy (QualTy void), size_t, size_t, PtrTy file])
    , (Name "setvbuf", FunTy int [PtrTy file, PtrTy char, int, size_t])
    , (Name "stdout", PtrTy file)
    , (Name "EOF", int)
    , (Name "SEEK_CUR", int)
    , (Name "_IOFBF", int)
    , (Name "_IOLBF", int)
    , (Name "_IONBF", int)
    ]
