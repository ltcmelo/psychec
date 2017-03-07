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
    apply s (c :&: c') = (apply s c) :&: (apply s c')
    apply s (Exists n c) = Exists n (apply s c)
    apply s (TypeDef t t') = TypeDef (apply s t) (apply s t')
    apply s c@(ReadOnly _) = c
    apply s Truth = Truth

instance Apply Ty where
    apply s t@(TyVar v) = maybe t id (Map.lookup v (subs s))
    apply s t@(TyCon c) = t
    apply s (Pointer t) = Pointer (apply s t)
    apply s (FunTy t ts) = FunTy (apply s t) (apply s ts)
    apply s (Struct fs n) = Struct (apply s fs) n
    apply s (QualTy t) = QualTy (apply s (dropTopQual t))
    apply s t@(EnumTy n) = t

instance Apply Field where
    apply s (Field n t) = Field n (apply s t)

instance Apply VarInfo where
    apply s (VarInfo t b ro) = VarInfo (apply s t) b ro


data Constness = Relax | Enforce
                 deriving (Eq, Ord, Show)

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
    fv (c :&: c') = fv c `union` fv c'
    fv (Exists n c) = n : fv c
    fv (TypeDef t t') = fv t `union` fv t'
    fv (ReadOnly _) = []
    fv t = []

instance Unifiable Ty where
    -- Free variables

    fv (TyVar v) = [v]
    fv (TyCon _) = []
    fv (Pointer t) = fv t
    fv (FunTy t ts) = fv t `union` fv ts
    fv (Struct fs _) = fv fs
    fv (QualTy t) = fv t
    fv (EnumTy _) = []

    -- Plain unification

    punify t'@(TyVar v) t
        | convertible t t' = return nullSubst
        | otherwise = varBind v t
    punify t t'@(TyVar v)
        | convertible t t' = return nullSubst
        | otherwise = varBind v t

    punify (TyCon c) (TyCon c')
        | convertible (TyCon c) (TyCon c') = return nullSubst
        | otherwise = differentTypeConstructorsError c c'

    punify p@(Pointer t) p'@(Pointer t')
        | convertible p p' = return nullSubst
        | otherwise = punify t t'
        
    punify f@(FunTy t ts) (Pointer p) = punify f p
    punify (Pointer p) f'@(FunTy t' ts') = punify f' p

    punify (FunTy t ts) (FunTy t' ts') = do
        s <- punify t t'
        s' <- punify (apply s ts) (apply s ts')
        return (s' @@ s)

    punify (Struct fs n) (Struct fs' n')
        | n == n' = punify (sort fs) (sort fs')
        | otherwise = differentTypeConstructorsError n n'

    punify (QualTy t) (QualTy t') = punify t t'
    punify (QualTy t) t' = punify t t'

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
    
    dunify t@(TyVar v) t'@(Pointer _) _ = varBind v t'

    dunify t'@(TyVar v) t m
        | convertible t t' = return nullSubst
        | m == Relax = varBind v (dropQual t)
        | otherwise = varBind v t
    dunify t t'@(TyVar v) m
        | convertible t t' = return nullSubst
        | otherwise = varBind v (dropQual t)

    dunify (TyCon c) (TyCon c') _
        | convertible (TyCon c) (TyCon c') = return nullSubst
        | otherwise = differentTypeConstructorsError c c'

    dunify p@(Pointer t) p'@(Pointer t') m
        | convertible p p' = return nullSubst
        | otherwise = dunify t t' Enforce

    dunify f@(FunTy t ts) (Pointer p) m = dunify f p m
    dunify (Pointer p) f'@(FunTy t' ts') m = dunify f' p m

    dunify (FunTy t ts) (FunTy t' ts') _ = do
        s <- dunify t t' Relax
        s' <- dunify (apply s ts) (apply s ts') Relax
        return (s' @@ s)

    dunify (Struct fs n) (Struct fs' n') _
        | n == n' = dunify (sort fs) (sort fs') Relax
        | otherwise = differentTypeConstructorsError n n'

    dunify (QualTy t) (QualTy t') m = dunify t t' m
    dunify (QualTy t) t' _ = dunify t t' Relax
    dunify t (QualTy t') m
        | m == Relax = dunify t t' Relax
        | otherwise = differentTypeConstructorsError (Name $ show $ pprint t)
                                                     (Name $ show $ pprint (QualTy t'))
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
dropQual (QualTy t) = dropQual t
dropQual (Pointer t) = Pointer (dropQual t)
dropQual t = t

dropTopQual (QualTy t) = t
dropTopQual t = t


isTyVar :: Pretty a => a -> Bool
isTyVar = (== "#alpha") . take 6 . show . pprint

isTyVarDep (Pointer t) = isTyVarDep t
isTyVarDep (QualTy t) = isTyVarDep t
isTyVarDep (TyVar _) = True
isTyVarDep _ = False


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
