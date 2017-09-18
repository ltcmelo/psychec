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
fpos_t :: Ty
fpos_t = NamedTy (Name "fpos_t")


-- | Collection of types.
stdioTypes :: [Ty]
stdioTypes = [file]


-- | Collection of values.
stdioValues :: [(Name, Ty)]
stdioValues =
    [ (Name "fopen", FunTy (PtrTy file) [charConstPtr, charConstPtr])
    , (Name "freopen", FunTy (PtrTy file) [charConstPtr, charConstPtr, PtrTy file])
    , (Name "fclose", FunTy int [PtrTy file])
    , (Name "fflush", FunTy int [PtrTy file])
    , (Name "setbuf", FunTy (PtrTy file) [charPtr])
    , (Name "setvbuf", FunTy int [PtrTy file, PtrTy char, int, size_t])
    , (Name "fwide", FunTy int [filePtr, int])
    , (Name "fread", FunTy size_t [PtrTy void, size_t, size_t, PtrTy file])
    , (Name "fwrite", FunTy size_t [PtrTy (constTy void), size_t, size_t, PtrTy file])
    , (Name "fgetc", FunTy int [PtrTy file])
    , (Name "getc", FunTy int [PtrTy file])
    , (Name "fgets", FunTy charPtr [charPtr, int, PtrTy file])
    , (Name "fputc", FunTy int [int, PtrTy file])
    , (Name "putc", FunTy int [int, PtrTy file])
    , (Name "fputs", FunTy int [charConstPtr, PtrTy file])
    , (Name "getchar", FunTy int [void])
    , (Name "gets", FunTy charPtr [charPtr])
    , (Name "putchar", FunTy int [int])
    , (Name "puts", FunTy int [charConstPtr])
    , (Name "ungetc", FunTy int [int, PtrTy file])
    , (Name "scanf", FunTy int [charConstPtr])
    , (Name "fscanf", FunTy int [PtrTy file, charConstPtr])
    , (Name "sscanf", FunTy int [charConstPtr, charConstPtr])
    , (Name "vscanf", FunTy int [charConstPtr, va_list])
    , (Name "vfscanf", FunTy int [PtrTy file, charConstPtr, va_list])
    , (Name "vsscanf", FunTy int [charConstPtr, charConstPtr, va_list])
    , (Name "printf", FunTy int [charConstPtr, AnyTy])
    , (Name "fprintf", FunTy int [PtrTy file, charConstPtr, AnyTy])
    , (Name "sprintf", FunTy int [charPtr, charConstPtr])
    , (Name "ftell", FunTy long [PtrTy file])
    , (Name "fgetpos", FunTy int [PtrTy file, fpos_t])
    , (Name "fseek", FunTy int [PtrTy file, long, int])
    , (Name "fsetpos", FunTy int [filePtr, PtrTy (constTy fpos_t)])
    , (Name "rewind", FunTy void [filePtr])
    , (Name "clearerr", FunTy void [filePtr])
    , (Name "feof", FunTy int [filePtr])
    , (Name "ferror", FunTy int [filePtr])
    , (Name "perror", FunTy void [charConstPtr])
    , (Name "remove", FunTy int [charConstPtr])
    , (Name "rename", FunTy int [charConstPtr, charConstPtr])
    -- Macros
    , (Name "stdin", filePtr)
    , (Name "stdout", filePtr)
    , (Name "stderr", filePtr)
    , (Name "EOF", int)
    , (Name "FOPEN_MAX", int)
    , (Name "FILENAME_MAX", int)
    , (Name "BUFSIZ", int)
    , (Name "SEEK_SET", int)
    , (Name "SEEK_CUR", int)
    , (Name "SEEK_END", int)
    ]


-- Convenience
filePtr = PtrTy file
