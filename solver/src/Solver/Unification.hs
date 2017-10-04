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

module Solver.Unification where

import Control.Monad.Except
import Control.Monad.State

import Data.List (union, sort)
import Data.Map (Map)
import qualified Data.Map as Map
import Data.Set (Set)
import qualified Data.Set as Set

import Data.Type
import Data.Constraints
import Data.BuiltIn
import Solver.SolverMonad
import Solver.ConversionRules
import Utils.Pretty

import Debug.Trace


(@@) :: Subst -> Subst -> Subst
s1 @@ s2 = Subst $ (Map.map (apply s1) (subs s2)) `Map.union` (subs s1)

class Apply a where
    apply :: Subst -> a -> a

instance Apply a => Apply [a] where
    apply s = map (apply s)

instance Apply Constraint where
    apply s (t :=: t') = (apply s t) :=: (apply s t')
    apply s (t :>: t') = (apply s t) :>: (apply s t')
    apply s (n :<-: t) = n :<-: (apply s t)
    apply s (Has t f) = Has (apply s t) (apply s f)
    apply s (Def n t c) = Def n (apply s t) (apply s c)
    apply s (Scope c) = Scope (apply s c)
    apply s (c :&: c') = (apply s c) :&: (apply s c')
    apply s (Exists n c) = Exists n (apply s c)
    apply s (TypeDef t t') = TypeDef (apply s t) (apply s t')
    apply s c@(ReadOnly _) = c
    apply s c@(Static _) = c
    apply s Truth = Truth

instance Apply Ty where
    apply s t@(VarTy v) = maybe t id (Map.lookup v (subs s))
    apply s t@(NamedTy c) = t
    apply s (PtrTy t) = PtrTy (apply s t)
    apply s (FunTy t ts) = FunTy (apply s t) (apply s ts)
    apply s (RecTy fs n) = RecTy (apply s fs) n
    -- Duplicate type qualifiers don't make sense, so drop substituted ones.
    apply s (QualTy t q) = QualTy (dropTopQual q (apply s t)) q
    apply s t@(EnumTy n) = t
    apply s t@(AnyTy) = t

instance Apply Field where
    apply s (Field n t) = Field n (apply s t)

instance Apply ValSym where
    apply s (ValSym t b ro st) = ValSym (apply s t) b ro st


data Constness = Relax | Enforce  deriving (Eq, Ord, Show)


class Apply a => Unifiable a where
    -- | Free variables
    fv :: a -> [Name]
    -- | Plain (standard) unification
    punify :: a -> a -> SolverM Subst
    -- | Directional unification
    dunify :: a -> a -> Constness -> SolverM Subst


instance Unifiable a => Unifiable [a] where
    fv = foldr (union . fv) []
    punify [] [] = return nullSubst
    punify _  [] = wrongArgumentNumberError
    punify []  _ = wrongArgumentNumberError
    punify (t:ts) (t':ts') = do
        s <- punify t t'
        s' <- punify (apply s ts) (apply s ts')
        return (s' @@ s)
    dunify (t:ts) (t':ts') _ = do
        s <- dunify t t' Relax
        s' <- dunify (apply s ts) (apply s ts') Relax
        return (s' @@ s)
    dunify ts ts' _ = punify ts ts'

instance Unifiable Constraint where
    punify _ _ = error "Impossible (plain) unify constraint"
    dunify _ _ _ = error "Impossible (directionaly) unify constraint"
    fv (t :=: t') = fv t `union` fv t'
    fv ( _ :<-: t) = fv t
    fv (Has t (Field _ t')) = fv t `union` fv t'
    fv (Def _ t c) = fv t `union` fv c
    fv (Scope c) = fv c
    fv (c :&: c') = fv c `union` fv c'
    fv (Exists n c) = n : fv c
    fv (TypeDef t t') = fv t `union` fv t'
    fv (ReadOnly _) = []
    fv (Static _) = []
    fv t = []

instance Unifiable Ty where
    -- Free variables
    fv (VarTy v) = [v]
    fv (NamedTy _) = []
    fv (PtrTy t) = fv t
    fv (FunTy t ts) = fv t `union` fv ts
    fv (RecTy fs _) = fv fs
    fv (QualTy t _) = fv t
    fv (EnumTy _) = []

    -- Plain unification
    punify AnyTy t = return nullSubst

    punify t'@(VarTy v) t
        | monoConvertible t t' = return nullSubst
        | otherwise = varBind v t

    punify t AnyTy = return nullSubst

    punify t t'@(VarTy v)
        | monoConvertible t t' = return nullSubst
        | otherwise = varBind v t

    punify (NamedTy c) (NamedTy c')
        | convertible (NamedTy c) (NamedTy c') = return nullSubst
        | otherwise = differentTypeConstructorsError c c'

    punify p@(PtrTy t@(VarTy n)) p'@(PtrTy t')
        | p == t' || monoConvertible p p' = return nullSubst
        | otherwise = punify t t'
    punify p@(PtrTy t) p'@(PtrTy t')
        | monoConvertible p p' = return nullSubst
        | otherwise = punify t t'

    punify f@(FunTy t ts) (PtrTy p) = punify f p
    punify (PtrTy p) f'@(FunTy t' ts') = punify f' p

    -- During unification of function types, we catch errors arising from inconsistent number
    -- of arguments or due to incompatible argument types. Those are latter identified as
    -- variadic functions or generic selections, if the mismatch occurs at the first index.
    -- TODO: Implementation of generic selections is not yet complete.
    punify (FunTy t ts) (FunTy t' ts') = do
        s <- punify t t'
        s' <- punify (apply s ts) (apply s ts') `catchError` (\_ -> return s)
        return (s' @@ s)

    punify (RecTy fs n) (RecTy fs' n')
        | n == n' = punify (sort fs) (sort fs')
        | otherwise = differentTypeConstructorsError n n'

    punify (QualTy t q) (QualTy t' q')
      | q == q' = punify t t'
      | otherwise = incompatibleQualifiers
    punify (QualTy t _) t' = punify t t'
    punify t'(QualTy t _) = punify t t'

    punify t@(EnumTy _) t'
        | convertible t t' = return nullSubst
        | otherwise = differentTypeConstructorsError (Name $ show $ pprint t)
                                                     (Name $ show $ pprint t')
    punify t t'@(EnumTy _)
        | convertible t t' = return nullSubst
        | otherwise = differentTypeConstructorsError (Name $ show $ pprint t)
                                                     (Name $ show $ pprint t')
    punify t t'
        | convertible t t' = return nullSubst
        | otherwise = differentTypeConstructorsError (Name $ show $ pprint t)
                                                     (Name $ show $ pprint t')

    -- Directional unification
    dunify t@(VarTy v) t'@(PtrTy _) _ = varBind v t'

    dunify AnyTy t _ = return nullSubst

    dunify t'@(VarTy v) t m
        | m == Relax = varBind v (dropQual t)
        | otherwise = varBind v t

    dunify t AnyTy _ = return nullSubst

    dunify t t'@(VarTy v) m = varBind v (dropQual t)

    dunify (NamedTy c) (NamedTy c') _
        | convertible (NamedTy c) (NamedTy c') = return nullSubst
        | otherwise = differentTypeConstructorsError c c'

    dunify p@(PtrTy t) p'@(PtrTy t') m
        | convertible p p' = return nullSubst
        | otherwise = dunify t t' Enforce

    dunify f@(FunTy t ts) (PtrTy p) m = dunify f p m
    dunify (PtrTy p) f'@(FunTy t' ts') m = dunify f' p m

    dunify (FunTy t ts) (FunTy t' ts') _ = do
        s <- dunify t t' Relax
        s' <- dunify (apply s ts) (apply s ts') Relax `catchError` (\_ -> return s)
        return (s' @@ s)

    dunify (RecTy fs n) (RecTy fs' n') _
        | n == n' = dunify (sort fs) (sort fs') Relax
        | otherwise = differentTypeConstructorsError n n'

    dunify (QualTy t q) (QualTy t' q') m
      | q == q' = dunify t t' m
      | otherwise = incompatibleQualifiers
    dunify (QualTy t _) t' _ = dunify t t' Relax
    dunify t (QualTy t' _) m = dunify t t' m

    dunify t@(EnumTy _) t' _
        | convertible t t' = return nullSubst
        | otherwise = differentTypeConstructorsError (Name $ show $ pprint t)
                                                     (Name $ show $ pprint t')
    dunify t t'@(EnumTy _) _
        | convertible t t' = return nullSubst
        | otherwise = differentTypeConstructorsError (Name $ show $ pprint t)
                                                     (Name $ show $ pprint t')
    dunify t t' _
        | convertible t t' = return nullSubst
        | otherwise = differentTypeConstructorsError (Name $ show $ pprint t)
                                                     (Name $ show $ pprint t')


instance Unifiable Field where
    fv = fv . ty
    punify (Field n t) (Field n' t')
          | n == n' = punify t t'
          | otherwise = punifyDifferentFields n n'
    dunify (Field n t) (Field n' t') m
          | n == n' = dunify t t' m
          | otherwise = punifyDifferentFields n n'


varBind :: Name -> Ty -> SolverM Subst
varBind n t = return (n +-> t)
    -- | n `elem` fv t = occursCheckError n t
    -- | otherwise = return $ Subst (Map.singleton n t)


dropQual :: Ty -> Ty
dropQual (QualTy t _) = dropQual t
dropQual (PtrTy t) = PtrTy (dropQual t)
dropQual t = t

dropTopQual q t'@(QualTy t q')
  | q == q' = t
  | otherwise = t'
dropTopQual _ t = t

dropTopQualAny (QualTy t _) = t
dropTopQualAny t = t

hasVarDep (PtrTy t) = hasVarDep t
hasVarDep (QualTy t _) = hasVarDep t
hasVarDep (VarTy _) = True
hasVarDep _ = False

wrongArgumentNumberError :: SolverM a
wrongArgumentNumberError = throwError "Error! Wrong argument number."

occursCheckError :: Name -> Ty -> SolverM a
occursCheckError n t = throwError $ show $
                          text "Cannot unify:\n" <>
                          pprint n <+> text "with\n" <>
                          pprint t  <> text "\noccurs check"

differentTypeConstructorsError :: Name -> Name -> SolverM a
differentTypeConstructorsError v v'
    = throwError $ show $
         text "Cannot unify types with different constructors:\n" <>
         pprint v <+> text "and" <+> pprint v'

punifyDifferentFields :: Name -> Name -> SolverM a
punifyDifferentFields n n' = throwError $ show $
                                text "Cannot unify different fields:\n" <+>
                                pprint n <+> text "\nwith\n" <+> pprint n'

incompatibleQualifiers :: SolverM a
incompatibleQualifiers = throwError $ show $ text "incompatible qualifiers\n"
