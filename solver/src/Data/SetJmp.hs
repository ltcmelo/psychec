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


-- | Module corresponding to header <setjmp.h>


module Data.SetJmp where

import Data.Type
import Data.BuiltIn


-- | Types
jmp_buf :: Ty
jmp_buf = NamedTy (Name "jmp_buf")


-- | Collection of types
setjmpTypes :: [Ty]
setjmpTypes = [jmp_buf]


-- | Collection of values.
setjmpValues :: [(Name, Ty)]
setjmpValues =
  [
    (Name "setjmp", FunTy void [jmp_buf])
  , (Name "longjmp", FunTy void [jmp_buf, int])
  ]
