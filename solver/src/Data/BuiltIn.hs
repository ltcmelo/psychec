-- Copyright (c) 2016 Rodrigo Ribeiro (rodrigo@decsi.ufop.br)
-- Copyright (c) 2016 Leandro T. C. Melo (ltcmelo@gmail.com)
-- Copyright (c) 2016 Marcus Rodrigues (demaroar@gmail.com)
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


module Data.BuiltIn where

import Data.Type
import Data.Map (Map)
import qualified Data.Map as Map
import Utils.Pretty (nameOf)

-- Use separate container for symbol attributes?
data ValSym = ValSym { valty :: Ty,
                       declared :: Bool,
                       readOnly :: Bool,
                       static :: Bool } deriving (Eq, Ord, Show)


void :: Ty
void = NamedTy (Name "void")

char :: Ty
char = NamedTy (Name "char")
signedChar :: Ty
signedChar = NamedTy (Name "signed char")
unsignedChar :: Ty
unsignedChar = NamedTy (Name "unsigned char")

short :: Ty
short = NamedTy (Name "short")
unsignedShort :: Ty
unsignedShort = NamedTy (Name "unsigned short")

int :: Ty
int = NamedTy (Name "int")
unsignedInt :: Ty
unsignedInt = NamedTy (Name "unsigned int")

long :: Ty
long = NamedTy (Name "long")
longLong :: Ty
longLong = NamedTy (Name "long long")
unsignedLong :: Ty
unsignedLong = NamedTy (Name "unsigned long")
unsignedLongLong :: Ty
unsignedLongLong = NamedTy (Name "unsigned long long")

float :: Ty
float = NamedTy (Name "float")
double :: Ty
double = NamedTy (Name "double")
longDouble :: Ty
longDouble = NamedTy (Name "long double")

size_t :: Ty
size_t = NamedTy (Name "size_t")

_Bool :: Ty
_Bool = NamedTy (Name "_Bool")

__int128 :: Ty
__int128 = NamedTy (Name "__int128")
__uint128 :: Ty
__uint128 = NamedTy (Name "__uint128")

intptr_t :: Ty
intptr_t = NamedTy (Name "intptr_t")

uintptr_t :: Ty
uintptr_t = NamedTy (Name "uintptr_t")


-- | Scalar, either an integer or pointer.
scalar_t__ :: Ty
scalar_t__ = NamedTy (Name "scalar_t__")


-- | A type for orphans and scalars (we could eventually separate those).
orphan :: Ty
orphan = NamedTy (Name "int /*<<< orphan*/ ")


-- | Whether a given type can be classified as arithmetic type.
isArithTy :: Ty -> Bool
isArithTy (NamedTy (Name n)) =
  isArithName n
 where
  -- Variations of char
  isArithName "char" = True
  isArithName "signed char" = True
  isArithName "unsigned char" = True

  -- Variations of short
  isArithName "short" = True
  isArithName "short int" = True
  isArithName "signed short" = True
  isArithName "signed short int" = True
  isArithName "unsigned short" = True
  isArithName "unsigned short int" = True

  -- Variations of int
  isArithName "int" = True
  isArithName "unsigned int" = True

  -- Variations of long
  isArithName "long" = True
  isArithName "long long" = True
  isArithName "unsigned long" = True
  isArithName "unsigned long long" = True

  -- Variations of floating-point
  isArithName "float" = True
  isArithName "double" = True
  isArithName "long double" = True

  -- Types from <stdint.h>
  isArithName "int8_t" = True
  isArithName "int16_t" = True
  isArithName "int32_t" = True
  isArithName "int64_t" = True
  isArithName "int_fast8_t" = True
  isArithName "int_fast16_t" = True
  isArithName "int_fast32_t" = True
  isArithName "int_fast64_t" = True
  isArithName "int_least8_t" = True
  isArithName "int_least16_t" = True
  isArithName "int_least32_t" = True
  isArithName "int_least64_t" = True
  isArithName "uint8_t" = True
  isArithName "uint16_t" = True
  isArithName "uint32_t" = True
  isArithName "uint64_t" = True
  isArithName "uint_fast8_t" = True
  isArithName "uint_fast16_t" = True
  isArithName "uint_fast32_t" = True
  isArithName "uint_fast64_t" = True
  isArithName "uint_least8_t" = True
  isArithName "uint_least16_t" = True
  isArithName "uint_least32_t" = True
  isArithName "uint_least64_t" = True
  isArithName "intmax_t" = True
  isArithName "uintmax_t" = True
  isArithName "intptr_t" = True
  isArithName "uintptr_t" = True

  -- Types from <stddef.h>
  isArithName "size_t" = True

  -- Types from <sys/types.h>
  isArithName "uid_t" = True
  isArithName "gid_t" = True
  isArithName "id_t" = True
  isArithName "mode_t" = True
  isArithName "nlink_t" = True
  isArithName "blkcnt_t" = True
  isArithName "fsblkcnt_t" = True
  isArithName "fsfilcnt_t" = True
  isArithName "ino_t" = True

  -- Non-arithmetic types
  isArithName _ = False
isArithTy _ = False


-- | Convenience for common stuff.
constTy :: Ty -> Ty
constTy t = QualTy t Const

charPtr :: Ty
charPtr = PtrTy char
charConstPtr :: Ty
charConstPtr = PtrTy (constTy char)

voidPtr :: Ty
voidPtr = PtrTy void
voidConstPtr :: Ty
voidConstPtr = PtrTy (constTy void)


-- | Compiler internals.
va_list :: Ty
va_list = NamedTy (Name "va_list")


-- helper
isPtrTy :: Ty -> Bool
isPtrTy (PtrTy _) = True
isPtrTy _ = False




-- Hacks: Stuff from C non-standard headers that are injected into scope on certain
-- certain UNIX-like platforms. If we don't take those into account we could end-up
-- re-typedefining types in an incompatible manner. So far this is mainly POSIX.

-- | Header sys/stat.h
struct_stat :: Ty
struct_stat = NamedTy (Name "struct stat")
sysStatTypes :: [Ty]
sysStatTypes
  = [ struct_stat ]
sysStatValues
  = [ (Name "stat", FunTy int [PtrTy (constTy char), PtrTy struct_stat])
    , (Name "fstat", FunTy int [int, PtrTy struct_stat])
    , (Name "lstat", FunTy int [PtrTy (constTy char), PtrTy struct_stat])
    , (Name "fstatat", FunTy int [int, PtrTy (constTy char), PtrTy struct_stat, int])
    ]


-- | Header sys/types.h
mode_t :: Ty
mode_t = NamedTy (Name "mode_t")
nlink_t :: Ty
nlink_t = NamedTy (Name "nlink_t")
uid_t :: Ty
uid_t = NamedTy (Name "uid_t")
gid_t :: Ty
gid_t = NamedTy (Name "gid_t")
id_t :: Ty
id_t = NamedTy (Name "id_t")
blkcnt_t :: Ty
blkcnt_t = NamedTy (Name "blkcnt_t")
fsblkcnt_t :: Ty
fsblkcnt_t = NamedTy (Name "fsblkcnt_t")
fsfilcnt_t :: Ty
fsfilcnt_t = NamedTy (Name "fsfilcnt_t")
ino_t :: Ty
ino_t = NamedTy (Name "ino_t")
dev_t :: Ty
dev_t = NamedTy (Name "dev_t")
sysTypesTypes :: [Ty]
sysTypesTypes
  = [ mode_t, nlink_t, uid_t, gid_t, id_t, blkcnt_t, fsblkcnt_t
    ,  fsfilcnt_t, ino_t, dev_t
    ]
sysTypesValues
  = [ (Name "strmode", FunTy void [mode_t, (PtrTy char)])
    , (Name "umask", FunTy mode_t [mode_t])
    , (Name "S_ISBLK", FunTy int [int])
    , (Name "S_ISCHR", FunTy int [int])
    , (Name "S_ISDIR", FunTy int [int])
    , (Name "S_ISFIFO", FunTy int [int])
    , (Name "S_ISREG", FunTy int [int])
    , (Name "S_ISBLK", FunTy int [int])
    , (Name "S_ISLNK", FunTy int [int])
    , (Name "S_IRWXU", int)
    , (Name "S_IRUSR", int)
    , (Name "S_IWUSR", int)
    , (Name "S_IXUSR", int)
    , (Name "S_ISUID", int)
    , (Name "S_ISGID", int)
    , (Name "S_ISVTX", int)
    ]
