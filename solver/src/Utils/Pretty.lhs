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

Utilities for pretty printing

> module Utils.Pretty (module Utils.Pretty,
>                      module Text.PrettyPrint.HughesPJ) where

> import Text.PrettyPrint.HughesPJ

> import Data.Constraints
> import Data.Type

> class Pretty a where
>     pprint :: a -> Doc


> instance Pretty Name where
>     pprint = text . unName


> instance Pretty Field where
>     pprint (Field n (FunTy r p)) =
>         pprint r <+> text "(*" <+> pprint n <+> text ")"
>             <+> parens (hcat $ punctuate comma (map pprint p)) <+> semi
>     pprint (Field n t) = pprint t <+> pprint n <> semi <> char ' '


> instance Pretty Ty where
>     pprint (TyCon n) = pprint n
>     pprint (TyVar n) = pprint n
>     pprint (FunTy ret params)
>       = pprint ret <+> text "(*)" <+>
>         parens (hcat $ punctuate comma (map pprint (params)))
>     pprint (Struct fs n) = pprint n <+>
>                            braces (hcat $ (map pprint fs))
>     pprint (Pointer t) = pprint t <> char '*'
>     pprint (QualTy t) = pprint t <+> text "const"
>     pprint (EnumTy n) = pprint n <+> text "{ ____Placeholder_"
>         <> pprint (ensurePlainName n) <+> text "}" -- TODO: Implement enumerators.


> instance Pretty Constraint where
>     pprint (t :=: t') = pprint t <+> char '=' <+> pprint t'
>     pprint (t :>: t') = pprint t <+> char '>' <+> pprint t'
>     pprint (n :<-: t) = text "typeof" <> (parens $ pprint n) <+>
>                         char '=' <+> pprint t
>     pprint (Has t f)  = text "has" <> parens (pprint t        <> comma <>
>                                               pprint (name f) <+> colon <+>
>                                               pprint (ty f))
>     pprint (TypeDef n t) = text "typedef" <+> pprint n <+> text "as"
>                                           <+> pprint t
>     pprint (Def n t ctr) = text "def" <+> pprint n <+> char ':' <+>
>                                           pprint t <+> text "in" <+>
>                                           pprint ctr
>     pprint (Exists n ctr) = text "exists" <+> pprint n <+> char '.'
>                                           <+> pprint ctr
>     pprint (ctr :&: ctr') = pprint ctr <+> char ',' <+> pprint ctr'
>     pprint (ReadOnly n) = text "const" <+> pprint n
>     pprint Truth = text "Truth"

Name of a type

> nameOf :: Ty -> Name
> nameOf (TyCon n) = n
> nameOf (TyVar n) = n
> nameOf (Struct _ n) = n
> nameOf (Pointer t) = Name ((unName (nameOf t)) ++ "*")
> nameOf x@(FunTy t ts) = Name (show $ pprint x)
> nameOf (QualTy t) = nameOf t
> nameOf (EnumTy n) = n

> -- plainNameOf
> baseNameOf :: Ty -> Name
> baseNameOf (TyCon n) = n
> baseNameOf (TyVar n) = n
> baseNameOf (Struct _ n) = n
> baseNameOf (Pointer t) = baseNameOf t
> baseNameOf x@(FunTy t ts) = Name (show $ pprint x)
> baseNameOf (QualTy t) = baseNameOf t
> baseNameOf (EnumTy n) = n


> isElabStructName :: Pretty a => a -> Bool
> isElabStructName = (== "struct ") . take 7 . show . pprint

> isElabEnumName :: Pretty a => a -> Bool
> isElabEnumName = (== "enum ") . take 5 . show . pprint

> isElab k = isElabStructName k || isElabEnumName k


> ensurePlainName :: Name -> Name
> ensurePlainName n
>   | isElabStructName n = Name (drop 7 (show (pprint n)))
>   | isElabEnumName n = Name (drop 5 (show (pprint n)))
>   | otherwise = n

We need to make sure the typedefs we generate won't collide with existing names.
The problem is that a typedef introduce two names. For instance, the following
`typedef struct X { ...} Y;' introduces the name `struct X' and the name 'Y'.
The name `Y' certainly won't colide because it's exactly what we're inferring
and what we saw in the program. But there's no guarantees about the name
`struct X'. We add the underscores to reduce this risk - the really correct
approach would be to make scope-wide search and check for duplicates.

> ensureElabEnumName :: Name -> Name
> ensureElabEnumName n
>   | isElabEnumName n = n
>   | otherwise = Name ("enum " ++ "__" ++ (unName n) ++ "__")

> ensureElabStructName :: Name -> Name
> ensureElabStructName n
>   | isElabStructName n = n
>   | otherwise = Name ("struct " ++ "__" ++ (unName n) ++ "__")
