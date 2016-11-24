Copyright (c) 2016 Rodrigo Ribeiro (rodrigo@decsi.ufop.br)
                   Leandro T. C. Melo (ltcmelo@gmail.com)
                   Marcus Rodrigues (demaroar@gmail.com)

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License
along with this library; if not, write to the Free Software Foundation,
Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301 USA


Built-in types, operators and contexts

> module Data.BuiltIn where

> import Data.Type
> import Data.Map (Map)
> import qualified Data.Map as Map
> import Data.STDLib
> import Utils.Pretty (nameOf)


> data VarInfo = VarInfo { varty :: Ty
>                        , declared :: Bool
>                        , readOnly :: Bool }
>                deriving (Eq, Ord, Show)


Helpers to operate on contexts

> typesToCtx :: [Ty] -> Map Name (Ty, Bool)
> typesToCtx = Map.fromList . map (\t -> (nameOf t, (t, True)))
> valuesToCtx :: [(Name, Ty)] -> Map Name VarInfo
> valuesToCtx = Map.fromList . map (\(n,t) -> (n, VarInfo t True False))

Contexts of C's builtin types and values

> builtinTyCtx :: Map Name (Ty, Bool)
> builtinTyCtx
>     = typesToCtx types
>       where
>         types = [ void
>                 , char, signedChar, unsignedChar
>                 , short, unsignedShort
>                 , int, unsignedInt
>                 , long, longLong, unsignedLong, unsignedLongLong
>                 , float, double, longDouble
>                 ]

> builtinVarCtx :: Map Name VarInfo
> builtinVarCtx
>     = Map.fromList types
>       where
>          types = [
>                  -- Operators in which operands are allowed to be either
>                  -- integrals or pointers may NOT be listed here, since they
>                  -- lead to overunification. Those operadors are handled in
>                  -- the generator through the pointer x integers lattice.
>                  -- Therefore only non-polymorphic operators are builtin.
>                    (Name "*", VarInfo (FunTy int [int, int]) True False)
>                  , (Name "/", VarInfo (FunTy double [double, double]) True False)
>                  , (Name "%", VarInfo (FunTy int [int, int]) True False)
>                  , (Name "<<", VarInfo (FunTy int [int, int]) True False)
>                  , (Name ">>", VarInfo (FunTy int [int, int]) True False)
>                  , (Name "&", VarInfo (FunTy int [int, int]) True False)
>                  , (Name "|", VarInfo (FunTy int [int, int]) True False)
>                  , (Name "^", VarInfo (FunTy int [int, int]) True False)
>                  ]


> stdTyCtx :: Map Name (Ty, Bool)
> stdTyCtx
>   = typesToCtx stdintTypes `Map.union` typesToCtx stddefTypes
>     `Map.union` typesToCtx sysTypesTypes `Map.union` typesToCtx sysStatTypes

> stdVarCtx :: Map Name VarInfo
> stdVarCtx
>   = valuesToCtx stdintValues `Map.union` valuesToCtx stdlibValues
>     `Map.union` valuesToCtx stringValues `Map.union` valuesToCtx sysStatValues
>     `Map.union` valuesToCtx sysTypesValues `Map.union` valuesToCtx mathValues


Builtin types

> uintptr_t :: Ty
> uintptr_t = TyCon (Name "uintptr_t")

void

> void :: Ty
> void = TyCon (Name "void")

character types

> char :: Ty
> char = TyCon (Name "char")
> signedChar :: Ty
> signedChar = TyCon (Name "signed char")
> unsignedChar :: Ty
> unsignedChar = TyCon (Name "unsigned char")

short types

> short :: Ty
> short = TyCon (Name "short")
> unsignedShort :: Ty
> unsignedShort = TyCon (Name "unsigned short")

int types

> int :: Ty
> int = TyCon (Name "int")
> unsignedInt :: Ty
> unsignedInt = TyCon (Name "unsigned int")

long types

> long :: Ty
> long = TyCon (Name "long")
> longLong :: Ty
> longLong = TyCon (Name "long long")
> unsignedLong :: Ty
> unsignedLong = TyCon (Name "unsigned long")
> unsignedLongLong :: Ty
> unsignedLongLong = TyCon (Name "unsigned long long")

floating-point types

> float :: Ty
> float = TyCon (Name "float")
> double :: Ty
> double = TyCon (Name "double")
> longDouble :: Ty
> longDouble = TyCon (Name "long double")

type inspection helpers

> isPointer :: Ty -> Bool
> isPointer (Pointer _) = True
> isPointer _ = False


Whether a given type can be classified as arithmetic type.

> isArithTy :: Ty -> Bool
> isArithTy (TyCon (Name n)) = isArithName n
>   where

>    -- Variations of char
>    isArithName "char" = True
>    isArithName "signed char" = True
>    isArithName "unsigned char" = True

>    -- Variations of short
>    isArithName "short" = True
>    isArithName "short int" = True
>    isArithName "signed short" = True
>    isArithName "signed short int" = True
>    isArithName "unsigned short" = True
>    isArithName "unsigned short int" = True

>    -- Variations of int
>    isArithName "int" = True
>    isArithName "unsigned int" = True

>    -- Variations of long
>    isArithName "long" = True
>    isArithName "long long" = True
>    isArithName "unsigned long" = True
>    isArithName "unsigned long long" = True

>    -- Variations of floating-point
>    isArithName "float" = True
>    isArithName "double" = True
>    isArithName "long double" = True

>    -- Types from <stdint.h>
>    isArithName "int8_t" = True
>    isArithName "int16_t" = True
>    isArithName "int32_t" = True
>    isArithName "int64_t" = True
>    isArithName "int_fast8_t" = True
>    isArithName "int_fast16_t" = True
>    isArithName "int_fast32_t" = True
>    isArithName "int_fast64_t" = True
>    isArithName "int_least8_t" = True
>    isArithName "int_least16_t" = True
>    isArithName "int_least32_t" = True
>    isArithName "int_least64_t" = True
>    isArithName "uint8_t" = True
>    isArithName "uint16_t" = True
>    isArithName "uint32_t" = True
>    isArithName "uint64_t" = True
>    isArithName "uint_fast8_t" = True
>    isArithName "uint_fast16_t" = True
>    isArithName "uint_fast32_t" = True
>    isArithName "uint_fast64_t" = True
>    isArithName "uint_least8_t" = True
>    isArithName "uint_least16_t" = True
>    isArithName "uint_least32_t" = True
>    isArithName "uint_least64_t" = True
>    isArithName "intmax_t" = True
>    isArithName "uintmax_t" = True
>    isArithName "intptr_t" = True
>    isArithName "uintptr_t" = True

>    -- Types from <stddef.h>
>    isArithName "size_t" = True

>    -- Types from <sys/types.h>
>    isArithName "uid_t" = True
>    isArithName "gid_t" = True
>    isArithName "id_t" = True
>    isArithName "mode_t" = True
>    isArithName "nlink_t" = True
>    isArithName "blkcnt_t" = True
>    isArithName "fsblkcnt_t" = True
>    isArithName "fsfilcnt_t" = True
>    isArithName "ino_t" = True

>    -- Non-arithmetic types
>    isArithName _ = False
> isArithTy _ = False



stddef.h - A few basic types and macro definitions.

> size_t :: Ty
> size_t = TyCon (Name "size_t")
> ptrdiff_t :: Ty
> ptrdiff_t = TyCon (Name "ptrdiff_t")
> max_align_t :: Ty -- C11
> max_align_t = TyCon (Name "max_align_t")
> stddefTypes :: [Ty]
> stddefTypes = [size_t, ptrdiff_t, max_align_t]


stdint.h - Particularly important to preserve platform-specific arithmetic types.

> int8_t :: Ty
> int8_t = TyCon (Name "int8_t")
> int16_t :: Ty
> int16_t = TyCon (Name "int16_t")
> int32_t :: Ty
> int32_t = TyCon (Name "int32_t")
> int64_t :: Ty
> int64_t = TyCon (Name "int64_t")
> int_fast8_t :: Ty
> int_fast8_t = TyCon (Name "int_fast8_t")
> int_fast16_t :: Ty
> int_fast16_t = TyCon (Name "int_fast16_t")
> int_fast32_t :: Ty
> int_fast32_t = TyCon (Name "int_fast32_t")
> int_fast64_t :: Ty
> int_fast64_t = TyCon (Name "int_fast64_t")
> int_least8_t :: Ty
> int_least8_t = TyCon (Name "int_least8_t")
> int_least16_t :: Ty
> int_least16_t = TyCon (Name "int_least16_t")
> int_least32_t :: Ty
> int_least32_t = TyCon (Name "int_least32_t")
> int_least64_t :: Ty
> int_least64_t = TyCon (Name "int_least64_t")
> uint8_t :: Ty
> uint8_t = TyCon (Name "uint8_t")
> uint16_t :: Ty
> uint16_t = TyCon (Name "uint16_t")
> uint32_t :: Ty
> uint32_t = TyCon (Name "uint32_t")
> uint64_t :: Ty
> uint64_t = TyCon (Name "uint64_t")
> uint_fast8_t :: Ty
> uint_fast8_t = TyCon (Name "uint_fast8_t")
> uint_fast16_t :: Ty
> uint_fast16_t = TyCon (Name "uint_fast16_t")
> uint_fast32_t :: Ty
> uint_fast32_t = TyCon (Name "uint_fast32_t")
> uint_fast64_t :: Ty
> uint_fast64_t = TyCon (Name "uint_fast64_t")
> uint_least8_t :: Ty
> uint_least8_t = TyCon (Name "uint_least8_t")
> uint_least16_t :: Ty
> uint_least16_t = TyCon (Name "uint_least16_t")
> uint_least32_t :: Ty
> uint_least32_t = TyCon (Name "uint_least32_t")
> uint_least64_t :: Ty
> uint_least64_t = TyCon (Name "uint_least64_t")
> intmax_t :: Ty
> intmax_t = TyCon (Name "intmax_t")
> uintmax_t :: Ty
> uintmax_t = TyCon (Name "uintmax_t")
> intptr_t :: Ty
> intptr_t = TyCon (Name "intptr_t")
> stdintTypes :: [Ty]
> stdintTypes =
>         [ int8_t, int16_t, int32_t, int64_t
>         , int_fast8_t, int_fast16_t, int_fast32_t, int_fast64_t
>         , int_least8_t, int_least16_t, int_least32_t, int_least64_t
>         , uint8_t, uint16_t, uint32_t, uint64_t
>         , uint_fast8_t, uint_fast16_t, uint_fast32_t, uint_fast64_t
>         , uint_least8_t, uint_least16_t, uint_least32_t, uint_least64_t
>         , intmax_t, uintmax_t
>         , intptr_t, uintptr_t
>         ]
> stdintValues :: [(Name, Ty)]
> stdintValues =
>               [ (Name "INT8_MIN", int8_t)
>               , (Name "INT16_MIN", int16_t)
>               , (Name "INT32_MIN", int32_t)
>               , (Name "INT64_MIN", int64_t)
>               , (Name "INT_FAST8_MIN", int_fast8_t)
>               , (Name "INT_FAST16_MIN", int_fast16_t)
>               , (Name "INT_FAST32_MIN", int_fast32_t)
>               , (Name "INT_FAST64_MIN", int_fast64_t)
>               , (Name "INT_LEAST8_MIN", int_least8_t)
>               , (Name "INT_LEAST16_MIN", int_least16_t)
>               , (Name "INT_LEAST32_MIN", int_least32_t)
>               , (Name "INT_LEAST64_MIN", int_least64_t)
>               , (Name "INTPTR_MIN", intptr_t)
>               , (Name "INTMAX_MIN", intmax_t)
>               , (Name "INT8_MAX", int8_t)
>               , (Name "INT16_MAX", int16_t)
>               , (Name "INT32_MAX", int32_t)
>               , (Name "INT64_MAX", int64_t)
>               , (Name "INT_FAST8_MAX", int_fast8_t)
>               , (Name "INT_FAST16_MAX", int_fast16_t)
>               , (Name "INT_FAST32_MAX", int_fast32_t)
>               , (Name "INT_FAST64_MAX", int_fast64_t)
>               , (Name "INT_LEAST8_MAX", int_least8_t)
>               , (Name "INT_LEAST16_MAX", int_least16_t)
>               , (Name "INT_LEAST32_MAX", int_least32_t)
>               , (Name "INT_LEAST64_MAX", int_least64_t)
>               , (Name "INTPTR_MAX", intptr_t)
>               , (Name "INTMAX_MAX", intmax_t)
>               , (Name "UINT8_MAX", uint8_t)
>               , (Name "UINT16_MAX", uint16_t)
>               , (Name "UINT32_MAX", uint32_t)
>               , (Name "UINT64_MAX", uint64_t)
>               , (Name "UINT_FAST8_MAX", uint_fast8_t)
>               , (Name "UINT_FAST16_MAX", uint_fast16_t)
>               , (Name "UINT_FAST32_MAX", uint_fast32_t)
>               , (Name "UINT_FAST64_MAX", uint_fast64_t)
>               , (Name "UINT_LEAST8_MAX", uint_least8_t)
>               , (Name "UINT_LEAST16_MAX", uint_least16_t)
>               , (Name "UINT_LEAST32_MAX", uint_least32_t)
>               , (Name "UINT_LEAST64_MAX", uint_least64_t)
>               , (Name "UINTPTR_MAX", uintptr_t)
>               , (Name "UINTMAX_MAX", uintmax_t)
>               , (Name "SIZE_MAX", size_t)
>               ]


stdlib.h - General utilities.

> stdlibValues :: [(Name, Ty)]
> stdlibValues
>   = [ (Name "malloc", FunTy (Pointer void) [int])
>     , (Name "calloc", FunTy (Pointer void) [int, int])
>     , (Name "realloc", FunTy (Pointer void) [(Pointer void), int])
>     , (Name "free", FunTy void [(Pointer void)])
>     , (Name "abort", FunTy void [])
>     , (Name "exit", FunTy void [int])
>     , (Name "atexit", FunTy void [(FunTy void [])])
>     , (Name "atoi", FunTy int [Pointer (QualTy char)])
>     , (Name "atof", FunTy double [Pointer (QualTy char)])
>     , (Name "atol", FunTy long [Pointer (QualTy char)])
>     , (Name "atoll", FunTy longLong [Pointer (QualTy char)])
>     , (Name "rand", FunTy int [])
>     , (Name "RAND_MAX", int)
>     , (Name "EXIT_SUCCESS", int)
>     , (Name "EXIT_FAILURE", int)
>     ]


string.h - String utility functions.

> stringValues
>   = [ (Name "memmove", FunTy (Pointer void) [Pointer void, Pointer (QualTy void), size_t])
>     , (Name "memcpy", FunTy (Pointer void) [Pointer void, Pointer (QualTy void), size_t])
>     , (Name "memcmp", FunTy int [Pointer (QualTy void), Pointer (QualTy void), size_t])
>     , (Name "strlen", FunTy size_t [Pointer (QualTy char)])
>     , (Name "stpcpy", FunTy (Pointer char) [Pointer char, Pointer (QualTy char)])
>     , (Name "strcpy", FunTy (Pointer char) [Pointer char, Pointer (QualTy char)])
>     , (Name "strncpy", FunTy (Pointer char) [Pointer char, Pointer (QualTy char), size_t])
>     , (Name "strcat", FunTy (Pointer char) [Pointer char, Pointer (QualTy char)])
>     , (Name "strncat", FunTy (Pointer char) [Pointer char, Pointer (QualTy char), size_t])
>     , (Name "strcmp", FunTy int [Pointer (QualTy char), Pointer (QualTy char)])
>     , (Name "strtok", FunTy (Pointer char) [Pointer char, Pointer (QualTy char)])
>     , (Name "strtof", FunTy float [Pointer (QualTy char), Pointer (Pointer char)])
>     , (Name "strtod", FunTy double [Pointer (QualTy char), Pointer (Pointer char)])
>     , (Name "strtold", FunTy longDouble [Pointer (QualTy char), Pointer (Pointer char)])
>     ]


stdio.h - Input/output

> file :: Ty
> file = TyCon (Name "FILE")
> stdioTypes :: [Ty]
> stdioTypes = [file]
> stdioValues :: [(Name, Ty)]
> stdioValues =
>     [ (Name "fclose", FunTy int [Pointer file])
>     , (Name "feof", FunTy int [Pointer file])
>     , (Name "ferror", FunTy int [Pointer file])
>     , (Name "fputc", FunTy int [int, Pointer file])
>     , (Name "fputs", FunTy int [Pointer (QualTy char), Pointer file])
>     , (Name "putchar", FunTy int [int])
>     , (Name "fopen", FunTy (Pointer file) [Pointer (QualTy char), Pointer (QualTy char)])
>     , (Name "fread", FunTy size_t [Pointer void, size_t, size_t, Pointer file])
>     , (Name "fwrite", FunTy size_t [Pointer (QualTy void), size_t, size_t, Pointer file])
>     , (Name "setvbuf", FunTy int [Pointer file, Pointer char, int, size_t])
>     , (Name "stdout", Pointer file)
>     , (Name "EOF", int)
>     , (Name "SEEK_CUR", int)
>     , (Name "_IOFBF", int)
>     , (Name "_IOLBF", int)
>     , (Name "_IONBF", int)
>     ]


math.h - Mathematical stuff.

> mathValues :: [(Name, Ty)]
> mathValues
>   = [ (Name "sqrtf", FunTy float [float])
>     , (Name "sqrt", FunTy double [double])
>     , (Name "sqrtl", FunTy longDouble [longDouble])
>     , (Name "cos", FunTy double [double])
>     , (Name "cosf", FunTy float [float])
>     , (Name "cosl", FunTy longDouble [longDouble])
>     , (Name "sin", FunTy double [double])
>     , (Name "sinf", FunTy float [float])
>     , (Name "cosl", FunTy longDouble [longDouble])
>     ]


Hacks: Stuff from C non-standard headers that are injected into scope on certain
certain UNIX-like platforms. If we don't take those into account we could end-up
re-typedefining types in an incompatible manner. So far this is mainly POSIX.

sys/stat.h - Things related to stat.

> struct_stat :: Ty
> struct_stat = TyCon (Name "struct stat")
> sysStatTypes :: [Ty]
> sysStatTypes
>   = [ struct_stat ]
> sysStatValues
>   = [ (Name "stat", FunTy int [Pointer (QualTy char), Pointer struct_stat])
>     , (Name "fstat", FunTy int [int, Pointer struct_stat])
>     , (Name "lstat", FunTy int [Pointer (QualTy char), Pointer struct_stat])
>     , (Name "fstatat", FunTy int [int, Pointer (QualTy char), Pointer struct_stat, int])
>     ]


sys/types.h - Bunch of types.

> mode_t :: Ty
> mode_t = TyCon (Name "mode_t")
> nlink_t :: Ty
> nlink_t = TyCon (Name "nlink_t")
> uid_t :: Ty
> uid_t = TyCon (Name "uid_t")
> gid_t :: Ty
> gid_t = TyCon (Name "gid_t")
> id_t :: Ty
> id_t = TyCon (Name "id_t")
> blkcnt_t :: Ty
> blkcnt_t = TyCon (Name "blkcnt_t")
> fsblkcnt_t :: Ty
> fsblkcnt_t = TyCon (Name "fsblkcnt_t")
> fsfilcnt_t :: Ty
> fsfilcnt_t = TyCon (Name "fsfilcnt_t")
> ino_t :: Ty
> ino_t = TyCon (Name "ino_t")
> dev_t :: Ty
> dev_t = TyCon (Name "dev_t")
> sysTypesTypes :: [Ty]
> sysTypesTypes
>   = [ mode_t, nlink_t, uid_t, gid_t, id_t, blkcnt_t, fsblkcnt_t
>     ,  fsfilcnt_t, ino_t, dev_t
>     ]
> sysTypesValues
>   = [ (Name "strmode", FunTy void [mode_t, (Pointer char)])
>     , (Name "umask", FunTy mode_t [mode_t])
>     , (Name "S_ISBLK", FunTy int [int])
>     , (Name "S_ISCHR", FunTy int [int])
>     , (Name "S_ISDIR", FunTy int [int])
>     , (Name "S_ISFIFO", FunTy int [int])
>     , (Name "S_ISREG", FunTy int [int])
>     , (Name "S_ISBLK", FunTy int [int])
>     , (Name "S_ISLNK", FunTy int [int])
>     , (Name "S_IRWXU", int)
>     , (Name "S_IRUSR", int)
>     , (Name "S_IWUSR", int)
>     , (Name "S_IXUSR", int)
>     , (Name "S_ISUID", int)
>     , (Name "S_ISGID", int)
>     , (Name "S_ISVTX", int)
>     ]
