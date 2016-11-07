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

Definition of C type syntax
===========================

> {-# LANGUAGE DeriveDataTypeable #-}

> module Data.Type where

> import Data.List (union)
> import Data.Generics

Definition of names

> newtype Name = Name { unName :: String }
>                deriving (Eq, Ord, Show, Data, Typeable)

Definition of fields

> data Field = Field { name :: Name
>                    , ty   :: Ty   }
>              deriving (Eq, Ord, Show, Data, Typeable)

Definition of types

> data Ty = TyCon Name
>         | TyVar Name
>         | FunTy Ty [Ty]
>         | Struct [Field] Name
>         | Pointer Ty
>         | QualTy Ty
>         | EnumTy Name -- TODO: Implement enumerators.
>         deriving (Eq, Ord, Show, Data, Typeable)

Utility

> typeIsStruct :: Ty -> Bool
> typeIsStruct (Struct _ _) = True
> typeIsStruct (Pointer ty) = typeIsStruct ty
> typeIsStruct (QualTy ty) = typeIsStruct ty
> typeIsStruct _ = False

> typeIsEnum :: Ty -> Bool
> typeIsEnum (EnumTy _) = True
> typeIsEnum (Pointer ty) = typeIsEnum ty
> typeIsEnum _ = False

> typeIsFunction :: Ty -> Bool
> typeIsFunction (FunTy _ _) = True
> typeIsFunction _ = False
