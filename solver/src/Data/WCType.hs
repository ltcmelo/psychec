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


-- | Module corresponding to header <wctype.h>


module Data.WCType where

import Data.Type
import Data.BuiltIn


-- | Types
wint_t :: Ty
wint_t = NamedTy (Name "wint_t")
wctrans_t :: Ty
wctrans_t = NamedTy (Name "wctrans_t")
wctype_t :: Ty
wctype_t = NamedTy (Name "wctype_t")


-- | Collection of types
wctypeTypes :: [Ty]
wctypeTypes = [wint_t, wctrans_t, wctype_t]


-- | Collection of values.
wctypeValues :: [(Name, Ty)]
wctypeValues =
  [
    (Name "iswalnum", FunTy int [wint_t])
  , (Name "iswalpha", FunTy int [wint_t])
  , (Name "iswlower", FunTy int [wint_t])
  , (Name "iswupper", FunTy int [wint_t])
  , (Name "iswdigit", FunTy int [wint_t])
  , (Name "iswxdigit", FunTy int [wint_t])
  , (Name "iswcntrl", FunTy int [wint_t])
  , (Name "iswgraph", FunTy int [wint_t])
  , (Name "iswspace", FunTy int [wint_t])
  , (Name "iswblank", FunTy int [wint_t])
  , (Name "iswprint", FunTy int [wint_t])
  , (Name "iswpunct", FunTy int [wint_t])
  , (Name "iswctype", FunTy int [wint_t])
  , (Name "towlower", FunTy int [wint_t])
  , (Name "towupper", FunTy int [wint_t])
  , (Name "towctrans", FunTy int [wint_t])
  , (Name "wctrans", FunTy wctrans_t [charConstPtr])
  ]
