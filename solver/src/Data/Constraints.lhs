> {-# LANGUAGE DeriveDataTypeable #-}

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

Definition of constraints
======================

> module Data.Constraints where

> import Data.Type
> import Data.Generics

> data Constraint = Ty :=: Ty                 -- equality
>                 | Name :<-: Ty              -- type ascription
>                 | Has Ty Field              -- field constraint
>                 | TypeDef Ty Ty             -- type definition
>                 | Def Name Ty Constraint    -- symbol definition
>                 | Constraint :&: Constraint -- conjunction
>                 | Exists Name Constraint    -- fresh variable introduction
>                 | ReadOnly Name             -- static values
>                 | Truth
>                 deriving (Eq, Ord, Show, Data, Typeable)
