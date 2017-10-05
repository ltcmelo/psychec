-- Copyright (c) 2016 Rodrigo Ribeiro (rodrigo@decsi.ufop.br)
--                    Leandro T. C. Melo (ltcmelo@gmail.com)
--                    Marcus Rodrigues (demaroar@gmail.com)
--
-- This library is free software; you can redistribute it and/or
-- modify it under the terms of the GNU Lesser General Public
-- License as published by the Free Software Foundation; either
-- version 2.1 of the License, or (at your option) any later version.
--
-- This library is distributed in the hope that it will be useful,
-- but WITHOUT ANY WARRANTY; without even the implied warranty of
-- MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
-- Lesser General Public License for more details.
--
-- You should have received a copy of the GNU Lesser General Public License
-- along with this library; if not, write to the Free Software Foundation,
-- Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301 USA


-- Specification of type conversion rules

module Solver.ConversionRules where

import Data.Type
import Data.BuiltIn

convertible :: Ty -> Ty -> Bool
convertible t t' =
  or [
      monoConvertible t t',
      or [t == PtrTy void, t' == PtrTy void],
      or [t == PtrTy (QualTy void Const), t' == PtrTy (QualTy void Const)]
     ]

monoConvertible :: Ty -> Ty -> Bool
monoConvertible t t' =
  or [
      t == t',
      and [isArithTy t, isArithTy t'],
      and [isArithTy t, isEnum t'],
      and [isEnum t, isArithTy t'],
      and [isArithTy t, t' == scalar_t__],
      and [t == scalar_t__, isArithTy t'],
      and [isPtrTy t, t' == scalar_t__],
      and [t == scalar_t__, isPtrTy t'],
      -- Due to function pointer decaying.
      and [isFunTy t, t' == scalar_t__],
      and [t == scalar_t__, isFunTy t']
     ]

isEnum :: Ty -> Bool
isEnum (EnumTy _) = True
isEnum _ = False
