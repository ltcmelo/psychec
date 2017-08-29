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


-- Retyping capabilities.

module Solver.Retypeable where

import Data.BuiltIn
import Data.Maybe
import Data.Map (Map)
import qualified Data.Map as Map
import Data.Set (Set)
import qualified Data.Set as Set
import Data.Type
import Solver.SolverMonad
import Utils.Pretty
import Debug.Trace


class Retypeable a where
  collect :: a -> [Ty]
  compact :: TyIdx -> a -> a
  orphanize :: TyIdx -> a -> a

instance Retypeable a => Retypeable [a] where
  collect = concat . map (collect)
  compact idx = map (compact idx)
  orphanize idx = map (orphanize idx)

instance Retypeable Ty where
  collect (VarTy _) = []
  collect (NamedTy _) = []
  collect (EnumTy t) = []
  collect (QualTy t _) = collect t
  collect (PtrTy t) = collect t
  collect (FunTy t tx) = collect t ++ collect tx
  collect t@(RecTy fs _) = [t] ++ collect fs
  collect AnyTy = []

  compact _ t@(VarTy _) = t
  compact _ t@(NamedTy _) = t
  compact _ t@(EnumTy _) = t
  compact idx (QualTy t q) = QualTy (compact idx t) q
  compact idx (PtrTy t) = PtrTy (compact idx t)
  compact idx (FunTy t tx) = FunTy (compact idx t) (compact idx tx)
  compact idx t@(RecTy fs _) = maybe t (\k -> NamedTy k) (Map.lookup t (ty2n idx))
  compact _ t@(AnyTy) = t

  orphanize idx t@(VarTy v) = maybe orphan NamedTy (Map.lookup t (ty2n idx))
  orphanize _ t@(NamedTy _) = t
  orphanize _ t@(EnumTy _) = t
  orphanize idx (QualTy t q) = QualTy (orphanize idx t) q
  orphanize idx (PtrTy t) = PtrTy (orphanize idx t)
  orphanize idx (FunTy t tx) = FunTy (orphanize idx t) (orphanize idx tx)
  orphanize idx (RecTy fs n) = RecTy (orphanize idx fs) n
  orphanize _ t@(AnyTy) = t

instance Retypeable Field where
  collect (Field _ t) = collect t
  compact idx (Field n t) = Field n (compact idx t)
  orphanize idx (Field n t) = Field n (orphanize idx t)


newtype TyIdx = TyIdx { ty2n :: Map Ty Name }

nullIdx :: TyIdx
nullIdx = TyIdx Map.empty

(%->) :: Ty -> Name -> TyIdx
t %-> n = TyIdx (Map.singleton t n)

(%%) :: TyIdx -> TyIdx -> TyIdx
nom1 %% nom2 = TyIdx $ (ty2n nom1) `Map.union` (ty2n nom2)

instance Pretty TyIdx where
  pprint = printer2 "%->" . ty2n


-- | Elminate top-level (and eventual recursive) alphas.
unalpha :: (Ty, Bool) -> SolverM (Ty, Bool)
unalpha p@(t@(RecTy fs n), b)
  | isVar n = do
    n <- fakeName
    let n' = Name ("struct " ++ (unName n))
    return $ (RecTy (check fs n') n', b)
  | otherwise = return p
 where
  check fs k = map (\(Field fn ft) -> Field fn (unalphaHelper k n ft)) fs
unalpha p = return p


-- | Elminate top-level (and eventual recursive) alphas preserving elaborated names.
unalpha2 :: Name -> Ty -> Ty
unalpha2 kn (RecTy fs n) =
  RecTy (check fs kn) kn
 where
  check fs k = map (\(Field fn ft) -> Field fn (unalphaHelper k n ft)) fs
unalpha2 kn (QualTy t q) =
  QualTy t' q
 where
  t' = unalpha2 kn t
unalpha2 _ t = t

-- Helper function for the unalpha versions above.
unalphaHelper k n t@(VarTy v)
  | n == v = NamedTy k
  | otherwise = t
unalphaHelper _ _ t@(NamedTy _) = t
unalphaHelper _ _ t@(EnumTy _) = t
unalphaHelper k n (QualTy t q) = QualTy (unalphaHelper k n t) q
unalphaHelper k n (PtrTy t) = PtrTy (unalphaHelper k n t)
unalphaHelper _ _ t@(FunTy _ _) = t
unalphaHelper _ _ (RecTy _ _) = error "shouldn't have structs at this point"
