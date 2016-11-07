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

Unification algorithm

> module Solver.Unification where

> import Control.Monad.Except
> import Control.Monad.State

> import Data.List (union, sort)
> import Data.Map (Map)
> import qualified Data.Map as Map

> import Data.Type
> import Data.Constraints
> import qualified Data.BuiltIn as BuiltIn
> import Solver.SolverMonad
> import Solver.ConversionRules
> import Utils.Pretty

A type class for the unification algorithm

> class Apply a where
>     apply :: Subst -> a -> a

> instance Apply a => Apply [a] where
>     apply s = map (apply s)

> (@@) :: Subst -> Subst -> Subst
> s1 @@ s2 = Subst $ (Map.map (apply s1) (subs s2))
>                    `Map.union`
>                    (subs s1)

> class Apply a => Unifiable a where
>     fv    :: a -> [Name]
>     unify :: a -> a -> SolverM Subst

> instance Unifiable a => Unifiable [a] where
>     fv      = foldr (union . fv) []
>     unify [] [] = return nullSubst
>     unify _ [] = wrongArgumentNumberError
>     unify [] _ = wrongArgumentNumberError
>     unify (t:ts) (t':ts')
>         = do
>             s <- unify t t'
>             s' <- unify (apply s ts)
>                         (apply s ts')
>             return (s' @@ s)


> varBind :: Name -> Ty -> SolverM Subst
> varBind n t = return (n +-> t)
>        -- | n `elem` fv t = occursCheckError n t
>       -- | otherwise = return $ Subst (Map.singleton n t)

> dropQual :: Ty -> Ty
> dropQual (QualTy t) = dropQual t
> dropQual t = t

> instance Apply Ty where
>     apply s t@(TyVar v) = maybe t id (Map.lookup v (subs s))
>     apply s t@(TyCon c) = t
>     apply s (Pointer t) = Pointer (apply s t)
>     apply s (FunTy t ts) = FunTy (apply s t) (apply s ts)
>     apply s (Struct fs n) = Struct (apply s fs) n
>     apply s (QualTy t) = QualTy (apply s (dropQual t))
>     apply s t@(EnumTy n) = t

> isTyVar :: Pretty a => a -> Bool
> isTyVar = (== "#alpha") . take 6 . show . pprint

> instance Unifiable Ty where
>     fv (TyVar v) = [v]
>     fv (TyCon _) = []
>     fv (Pointer t) = fv t
>     fv (FunTy t ts) = fv t `union` fv ts
>     fv (Struct fs _) = fv fs
>     fv (QualTy t) = fv t
>     fv (EnumTy _) = []
>
>     -- When unifying qualified types we simply discard the qualifier. This
>     -- works because programs we analyse are by premisse correct, so we won't
>     -- break things like const-corretness.
>
>     unify (TyVar v) (QualTy t') = varBind v (QualTy $ dropQual t')
>     unify (TyVar v) t
>           | t == (Pointer BuiltIn.void) = return nullSubst
>           | otherwise = varBind v t
>     unify (QualTy t') (TyVar v) = varBind v (QualTy $ dropQual t')
>     unify t (TyVar v)
>           | t == (Pointer BuiltIn.void) = return nullSubst
>           | otherwise = varBind v t
>     unify (TyCon c) (TyCon c')
>           | convertible (TyCon c) (TyCon c') = return nullSubst
>           | otherwise = differentTypeConstructorsError c c'
>     unify (Pointer t) (Pointer t')
>           | convertible (Pointer t) (Pointer t') = return nullSubst
>           | otherwise = unify t t'
>     unify f@(FunTy t ts) (Pointer f'@(FunTy t' ts')) = unify f f'
>     unify (Pointer f@(FunTy t ts)) f'@(FunTy t' ts') = unify f' f
>     unify (FunTy t ts) (FunTy t' ts')
>         = do
>            s <- unify t t'
>            s' <- unify (apply s ts) (apply s ts')
>            return (s' @@ s)
>     unify (Struct fs n) (Struct fs' n')
>         | n == n' = unify (sort fs) (sort fs')
>         | otherwise = differentTypeConstructorsError n n'
>     unify (QualTy t) (QualTy t') = unify t t'
>     unify (QualTy t) t' = unify t t'
>     unify t (QualTy t') = unify t t'
>     unify t@(EnumTy _) t'
>         | convertible t t' = return nullSubst
>         | otherwise = differentTypeConstructorsError (Name $ show $ pprint t)
>                                                      (Name $ show $ pprint t')
>     unify t t'@(EnumTy _)
>         | convertible t t' = return nullSubst
>         | otherwise = differentTypeConstructorsError (Name $ show $ pprint t)
>                                                      (Name $ show $ pprint t')
>     unify t t'
>         | convertible t t' = return nullSubst
>         | otherwise = differentTypeConstructorsError (Name $ show $ pprint t)
>                                                      (Name $ show $ pprint t')

> instance Apply Field where
>     apply s (Field n t) = Field n (apply s t)

> instance Unifiable Field where
>     fv = fv . ty
>     unify (Field n t) (Field n' t')
>           | n == n' = unify t t'
>           | otherwise = unifyDifferentFields n n'


> instance Apply Constraint where
>     apply s (t :=: t') = (apply s t) :=: (apply s t')
>     apply s (n :<-: t) = n :<-: (apply s t)
>     apply s (Has t f) = Has (apply s t) (apply s f)
>     apply s (Def n t c) = Def n (apply s t) (apply s c)
>     apply s (c :&: c') = (apply s c) :&: (apply s c')
>     apply s (Exists n c) = Exists n (apply s c)
>     apply s (TypeDef t t') = TypeDef (apply s t) (apply s t')
>     apply s c@(Const _) = c
>     apply s Truth = Truth

> instance Unifiable Constraint where
>     unify _ _ = error "Impossible! Unify Constraint"
>     fv (t :=: t') = fv t `union` fv t'
>     fv ( _ :<-: t) = fv t
>     fv (Has t (Field _ t')) = fv t `union` fv t'
>     fv (Def _ t c) = fv t `union` fv c
>     fv (c :&: c') = fv c `union` fv c'
>     fv (Exists n c) = n : fv c
>     fv (TypeDef t t') = fv t `union` fv t'
>     fv (Const _) = []
>     fv t = []

> wrongArgumentNumberError :: SolverM a
> wrongArgumentNumberError = throwError "Error! Wrong argument number."

> occursCheckError :: Name -> Ty -> SolverM a
> occursCheckError n t = throwError $ show $
>                           text "Cannot unify:\n" <>
>                           pprint n <+> text "with\n" <>
>                           pprint t  <> text "\noccurs check"

> differentTypeConstructorsError :: Name -> Name -> SolverM a
> differentTypeConstructorsError v v'
>     = throwError $ show $
>          text "Cannot unify types with different constructors:\n" <>
>          pprint v <+> text "and" <+> pprint v'

> unifyDifferentFields :: Name -> Name -> SolverM a
> unifyDifferentFields n n' = throwError $ show $
>                                 text "Cannot unify different fields:\n" <+>
>                                 pprint n <+> text "\nwith\n" <+> pprint n'
