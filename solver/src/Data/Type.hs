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

-- This modules defines C types and related stuff.

module Data.Type where


newtype Name = Name { unName :: String } deriving (Eq, Ord, Show)

emptyName :: Name
emptyName = Name ""

data Field = Field { name :: Name, ty   :: Ty } deriving (Eq, Ord, Show)

data Ty = TyCon Name
        | TyVar Name
        | FunTy Ty [Ty]
        | Struct [Field] Name
        | Pointer Ty
        | QualTy Ty
        | EnumTy Name -- TODO: Implement enumerators.
        deriving (Eq, Ord, Show)
