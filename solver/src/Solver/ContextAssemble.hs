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


-- | Context assembling functions.


module Solver.ContextAssemble where

import Data.BuiltIn
import Data.CLang
import Data.Map (Map)
import qualified Data.Map as Map
import Data.Type
import Data.IntTypes
import Data.SetJmp
import Data.Signal
import Data.StdDef
import Data.StdInt
import Data.StdLib
import Data.StdIO
import Data.String
import Data.Math
import Data.WChar
import Data.WCType

import Utils.Pretty (nameOf)


-- | C's builtin types by language version.
builtinTypes :: CLang -> Map Name (Ty, Bool)
builtinTypes l =
  fillTypes types
 where
  types = [void,
           char, signedChar, unsignedChar,
           short, unsignedShort,
           int, unsignedInt,
           long, unsignedLong,
           longLong, unsignedLongLong,
           _Bool,
           __int128, __uint128,
           float, -- floatComplex, floatImaginary,
           double, -- doubleComplex, doubleImaginary,
           longDouble, -- longDoubleComplex, longDoubleImaginary
           size_t, intptr_t, uintptr_t,
           scalar_t__]


-- | C's builtin values by language version.
builtinValues :: CLang -> Map Name ValSym
builtinValues l =
  Map.fromList values
 where
  -- Only non-polymorphic operators can be listed here (see ConversionRules.hs).
  values = [(Name "<<", ValSym (FunTy int [int, int]) True False False),  -- Trigger int promotion.
            (Name ">>", ValSym (FunTy int [int, int]) True False False)]


-- | C standard library's types by language version.
stdTypes :: CLang -> Map Name (Ty, Bool)
stdTypes l =
  fillTypes stdintTypes `Map.union` fillTypes stddefTypes
                        `Map.union` fillTypes stdioTypes
                        `Map.union` fillTypes stdlibTypes
                        `Map.union` fillTypes setjmpTypes
                        `Map.union` fillTypes signalTypes
                        `Map.union` fillTypes wctypeTypes
                        `Map.union` fillTypes wcharTypes
                        `Map.union` fillTypes sysTypesTypes
                        `Map.union` fillTypes sysStatTypes


-- | C standard library's values by language version.
stdValues :: CLang -> Map Name ValSym
stdValues l =
  fillValues stdintValues `Map.union` fillValues stdioValues
                          `Map.union` fillValues stdlibValues
                          `Map.union` fillValues stringValues
                          `Map.union` fillValues setjmpValues
                          `Map.union` fillValues inttypesValues
                          `Map.union` fillValues signalValues
                          `Map.union` fillValues wctypeValues
                          `Map.union` fillValues wcharValues
                          `Map.union` fillValues sysStatValues
                          `Map.union` fillValues sysTypesValues
                          `Map.union` fillValues mathValues


-- | Clean the map from builtint/stdlib values.
cleanValues :: CLang -> Map Name ValSym -> Map Name ValSym
cleanValues l = (Map.\\ (builtinValues l)) . (Map.\\ (stdValues l))


-- | Clean the map from builtin/stdlib types.
cleanTypes :: CLang -> Map Name (Ty, Bool) -> Map Name (Ty, Bool)
cleanTypes l = (Map.\\ (builtinTypes l)) . (Map.\\ (stdTypes l))


-- Helpers to operate on contexts
fillTypes :: [Ty] -> Map Name (Ty, Bool)
fillTypes = Map.fromList . map (\t -> (nameOf t, (t, True)))
fillValues :: [(Name, Ty)] -> Map Name ValSym
fillValues = Map.fromList . map (\(n,t) -> (n, ValSym t True False False))
