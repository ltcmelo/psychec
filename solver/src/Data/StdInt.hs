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


-- | Module corresponding to header <stdint.h>


module Data.StdInt where

import Data.BuiltIn
import Data.Type
import Data.StdDef


-- | Types
int8_t :: Ty
int8_t = NamedTy (Name "int8_t")
int16_t :: Ty
int16_t = NamedTy (Name "int16_t")
int32_t :: Ty
int32_t = NamedTy (Name "int32_t")
int64_t :: Ty
int64_t = NamedTy (Name "int64_t")
int_fast8_t :: Ty
int_fast8_t = NamedTy (Name "int_fast8_t")
int_fast16_t :: Ty
int_fast16_t = NamedTy (Name "int_fast16_t")
int_fast32_t :: Ty
int_fast32_t = NamedTy (Name "int_fast32_t")
int_fast64_t :: Ty
int_fast64_t = NamedTy (Name "int_fast64_t")
int_least8_t :: Ty
int_least8_t = NamedTy (Name "int_least8_t")
int_least16_t :: Ty
int_least16_t = NamedTy (Name "int_least16_t")
int_least32_t :: Ty
int_least32_t = NamedTy (Name "int_least32_t")
int_least64_t :: Ty
int_least64_t = NamedTy (Name "int_least64_t")
uint8_t :: Ty
uint8_t = NamedTy (Name "uint8_t")
uint16_t :: Ty
uint16_t = NamedTy (Name "uint16_t")
uint32_t :: Ty
uint32_t = NamedTy (Name "uint32_t")
uint64_t :: Ty
uint64_t = NamedTy (Name "uint64_t")
uint_fast8_t :: Ty
uint_fast8_t = NamedTy (Name "uint_fast8_t")
uint_fast16_t :: Ty
uint_fast16_t = NamedTy (Name "uint_fast16_t")
uint_fast32_t :: Ty
uint_fast32_t = NamedTy (Name "uint_fast32_t")
uint_fast64_t :: Ty
uint_fast64_t = NamedTy (Name "uint_fast64_t")
uint_least8_t :: Ty
uint_least8_t = NamedTy (Name "uint_least8_t")
uint_least16_t :: Ty
uint_least16_t = NamedTy (Name "uint_least16_t")
uint_least32_t :: Ty
uint_least32_t = NamedTy (Name "uint_least32_t")
uint_least64_t :: Ty
uint_least64_t = NamedTy (Name "uint_least64_t")
intmax_t :: Ty
intmax_t = NamedTy (Name "intmax_t")
uintmax_t :: Ty
uintmax_t = NamedTy (Name "uintmax_t")


-- | Collection of types
stdintTypes :: [Ty]
stdintTypes =
        [ int8_t, int16_t, int32_t, int64_t
        , int_fast8_t, int_fast16_t, int_fast32_t, int_fast64_t
        , int_least8_t, int_least16_t, int_least32_t, int_least64_t
        , uint8_t, uint16_t, uint32_t, uint64_t
        , uint_fast8_t, uint_fast16_t, uint_fast32_t, uint_fast64_t
        , uint_least8_t, uint_least16_t, uint_least32_t, uint_least64_t
        , intmax_t, uintmax_t
        ]


-- | Collection of values
stdintValues :: [(Name, Ty)]
stdintValues =
              [ (Name "INT8_MIN", int8_t)
              , (Name "INT16_MIN", int16_t)
              , (Name "INT32_MIN", int32_t)
              , (Name "INT64_MIN", int64_t)
              , (Name "INT_FAST8_MIN", int_fast8_t)
              , (Name "INT_FAST16_MIN", int_fast16_t)
              , (Name "INT_FAST32_MIN", int_fast32_t)
              , (Name "INT_FAST64_MIN", int_fast64_t)
              , (Name "INT_LEAST8_MIN", int_least8_t)
              , (Name "INT_LEAST16_MIN", int_least16_t)
              , (Name "INT_LEAST32_MIN", int_least32_t)
              , (Name "INT_LEAST64_MIN", int_least64_t)
              , (Name "INTPTR_MIN", intptr_t)
              , (Name "INTMAX_MIN", intmax_t)
              , (Name "INT8_MAX", int8_t)
              , (Name "INT16_MAX", int16_t)
              , (Name "INT32_MAX", int32_t)
              , (Name "INT64_MAX", int64_t)
              , (Name "INT_FAST8_MAX", int_fast8_t)
              , (Name "INT_FAST16_MAX", int_fast16_t)
              , (Name "INT_FAST32_MAX", int_fast32_t)
              , (Name "INT_FAST64_MAX", int_fast64_t)
              , (Name "INT_LEAST8_MAX", int_least8_t)
              , (Name "INT_LEAST16_MAX", int_least16_t)
              , (Name "INT_LEAST32_MAX", int_least32_t)
              , (Name "INT_LEAST64_MAX", int_least64_t)
              , (Name "INTPTR_MAX", intptr_t)
              , (Name "INTMAX_MAX", intmax_t)
              , (Name "UINT8_MAX", uint8_t)
              , (Name "UINT16_MAX", uint16_t)
              , (Name "UINT32_MAX", uint32_t)
              , (Name "UINT64_MAX", uint64_t)
              , (Name "UINT_FAST8_MAX", uint_fast8_t)
              , (Name "UINT_FAST16_MAX", uint_fast16_t)
              , (Name "UINT_FAST32_MAX", uint_fast32_t)
              , (Name "UINT_FAST64_MAX", uint_fast64_t)
              , (Name "UINT_LEAST8_MAX", uint_least8_t)
              , (Name "UINT_LEAST16_MAX", uint_least16_t)
              , (Name "UINT_LEAST32_MAX", uint_least32_t)
              , (Name "UINT_LEAST64_MAX", uint_least64_t)
              , (Name "UINTPTR_MAX", uintptr_t)
              , (Name "UINTMAX_MAX", uintmax_t)
              , (Name "SIZE_MAX", size_t)
              ]
