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


-- Utilities for pretty printing

module Utils.Pretty (module Utils.Pretty,
                     module Text.PrettyPrint.HughesPJ) where

import Data.Constraints
import Data.Type
import Text.PrettyPrint.HughesPJ


class Pretty a where
  pprint :: a -> Doc

instance Pretty a => Pretty [a] where
  pprint v = foldr (\x acc -> pprint x <+> text "  " <+> acc ) (text "") v

instance Pretty Name where
  pprint = text . unName

instance Pretty Field where
  pprint (Field n (FunTy r p)) =
    pprint r <+> text "(*" <+> pprint n <+> text ")"
             <+> parens (hcat $ punctuate comma (map pprint p)) <+> semi
  pprint (Field n t) = pprint t <+> pprint n <> semi <> char ' '

instance Pretty Ty where
  pprint (NamedTy n) = pprint n
  pprint (VarTy n) = pprint n
  pprint (FunTy ret params) = pprint ret <+> text "(*)" <+>
    parens (hcat $ punctuate comma (map pprint (params)))
  pprint (RecTy fs n) = pprint n <+> braces (hcat $ (map pprint fs))
  pprint (SumTy fs n) = pprint n <+> braces (hcat $ (map pprint fs))
  pprint (PtrTy t) = pprint t <> char '*'
  pprint (QualTy t Const) = pprint t <+> text "const"
  pprint (QualTy t Volatile) = pprint t <+> text "volatile"
  pprint (EnumTy n) =
    pprint n <+> text "{ ____Placeholder_" <> pprint (ensurePlain n) <+> text "}"
  pprint AnyTy = text "..."

instance Pretty Constraint where
  pprint (t :=: t') = pprint t <+> char '=' <+> pprint t'
  pprint (t :>: t') = pprint t <+> char '>' <+> pprint t'
  pprint (n :<-: t) = text "typeof" <> (parens $ pprint n) <+> char '=' <+> pprint t
  pprint (Has t f)  = text "has" <> parens (pprint t        <> comma <>
                                            pprint (name f) <+> colon <+>
                                            pprint (ty f))
  pprint (TypeDef n t) = text "typedef" <+> pprint n <+> text "as" <+> pprint t
  pprint (Def n t ctr) = text "def" <+> pprint n <+> char ':' <+>
                                        pprint t <+> text "in" <+>
                                        pprint ctr
  pprint (Scope c) = text "{" <+> pprint c <+> text "}"
  pprint (Exists n ctr) = text "exists" <+> pprint n <+> char '.' <+> pprint ctr
  pprint (ctr :&: ctr') = pprint ctr <+> text " ^ " <+> pprint ctr'
  pprint (ReadOnly n) = text "constexpr" <+> pprint n
  pprint (Static n) = text "static" <+> pprint n
  pprint Truth = text "Truth"


-- | The names of types.
nameOf :: Ty -> Name
nameOf (NamedTy n) = n
nameOf (VarTy n) = n
nameOf (RecTy _ n) = n
nameOf (SumTy _ n) = n
nameOf (PtrTy t) = Name ((unName (nameOf t)) ++ "*")
nameOf x@(FunTy t ts) = Name (show $ pprint x)
nameOf (QualTy t _) = nameOf t
nameOf (EnumTy n) = n
nameOf AnyTy = Name "..."

-- | Whehther or not we have a type variable.
isVar :: Pretty a => a -> Bool
isVar = (== "#alpha") . take 6 . show . pprint


-- | Elaborated names stuff.
isElabRec :: Pretty a => a -> Bool
isElabRec = (== "struct ") . take 7 . show . pprint

isElabUnion :: Pretty a => a -> Bool
isElabUnion = (== "union ") . take 6 . show . pprint

isElabEnum :: Pretty a => a -> Bool
isElabEnum = (== "enum ") . take 5 . show . pprint

isElab k = isElabRec k || isElabEnum k || isElabUnion k

ensurePlain :: Name -> Name
ensurePlain n
  | isElabRec n = Name (drop 7 (show (pprint n)))
  | isElabEnum n = Name (drop 5 (show (pprint n)))
  | isElabUnion n = Name (drop 6 (show (pprint n)))
  | otherwise = n

ensurePlainEnum :: Name -> Name
ensurePlainEnum n
  | isElabEnum n = n
  | otherwise = Name ("enum " ++ "__" ++ (unName n) ++ "__")

ensurePlainRec :: Name -> Name
ensurePlainRec n
  | isElabRec n = n
  | otherwise = Name ("struct " ++ "__" ++ (unName n) ++ "__")

