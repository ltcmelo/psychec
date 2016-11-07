> {-# LANGUAGE FlexibleContexts #-}
> {-# LANGUAGE TupleSections #-}

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

> module Solver.ConstraintSolver where

> import Data.Map (Map)
> import qualified Data.Map as Map
> import qualified Data.List as List
> import Data.Maybe (isJust, fromJust)

> import Data.Constraints
> import Data.Type
> import Data.BuiltIn

> import Control.Monad  hiding (void)
> import Control.Monad.Trans
> import Control.Monad.State hiding (void)
> import Control.Monad.Except hiding (void)

> import Solver.SolverMonad
> import Solver.Unification
> import Solver.ConversionRules
> import Utils.Pretty


> solver :: Constraint -> IO (Either String (TyCtx, VarCtx))
> solver c = runSolverM (solve c) ((length $ fv c) + 1)

> solve :: Constraint -> SolverM (TyCtx, VarCtx)
> solve c = do
>             --expand type defs.
>             (tc0,c') <- stage1 (TyCtx $ Map.union builtinTyCtx stdTyCtx) c
>
>             --expand variable types and create missing variables
>             (vcx,c'') <- stage2 (VarCtx $ Map.union builtinVarCtx stdVarCtx) c'
>
>             --split constraints into equality and field acess. unify equalities
>             let (eqs, fds) = stage3 c''
>
>             s <- unifyList eqs
>             --liftIO (print $ subs s)
>             --liftIO (print $ pprint s)
>
>             --build record structures and remove built-ins
>             (tcx1, vcx1, ss) <- stage4 tc0 vcx fds s
>
>             let
>                 tcx2_ = undefTys (tyctx tcx1) Map.\\ builtinTyCtx Map.\\ stdTyCtx
>                 vcx2_ = undefVars (varctx vcx1) Map.\\ builtinVarCtx Map.\\ stdVarCtx
>
>             return (TyCtx tcx2_, VarCtx vcx2_)


> appendlName :: String -> Name -> Name
> appendlName str (Name name) = Name (str ++ name)

> stage1 :: TyCtx -> Constraint -> SolverM (TyCtx, Constraint)
> stage1 tctx (t :=: t')
>        = do
>            return (tctx, (replaceTy tctx t) :=:
>                          (replaceTy tctx t'))
> stage1 tctx (n :<-: t)
>        = return (tctx, n :<-: (replaceTy tctx t))
> stage1 tctx (Has n (Field n' t))
>        = return (tctx, Has n (Field n' (replaceTy tctx t)))
> stage1 tctx (TypeDef t t')
>        = defineTypeDef t t' tctx
> stage1 tctx (c :&: c')
>        = do
>            (tcx1, c1) <- stage1 tctx c
>            (tcx2, c1') <- stage1 tcx1 c'
>            return (tcx2, c1 :&: c1')
> stage1 tctx (Exists n c)
>        = do
>            v <- fresh
>            stage1 tctx (apply (n +-> v) c)
> stage1 tctx (Def n t c)
>        = do
>           (tcx, c')  <- stage1 tctx c
>           return (tcx, Def n (replaceTy tctx t) c')
> stage1 tctx c@(Const _)
>        = return (tctx, c)
> stage1 tctx Truth
>        = return (tctx, Truth)

> defineTypeDef :: Ty -> Ty -> TyCtx -> SolverM (TyCtx, Constraint)
> defineTypeDef (Pointer t) (Pointer t') tctx
>     = do
>         (tcx,c) <- defineTypeDef t t' tctx
>         return (tcx, (t :=: t') :&: c)
> defineTypeDef t@(Pointer l) t'@(TyVar v) tctx
>     = do
>          v' <- fresh
>          (tcx, c) <- defineTypeDef l v' tctx
>          return (tcx, (t' :=: (Pointer v')) :&: c)
> defineTypeDef (QualTy t) (QualTy t') tctx
>     = do
>         (tcx,c) <- defineTypeDef t t' tctx
>         return (tcx, (t :=: t') :&: c)
> defineTypeDef t@(QualTy l) t'@(TyVar v) tctx
>     = do
>          v' <- fresh
>          (tcx, c) <- defineTypeDef l v' tctx
>          return (tcx, (t' :=: (QualTy v')) :&: c)
> defineTypeDef t@(EnumTy n) t'@(TyVar v) tctx
>     = do
>          let
>            tctx' = TyCtx $ maybe (Map.insert n (EnumTy n, False) (tyctx tctx))
>                                  (const (tyctx tctx))
>                                  (Map.lookup n (tyctx tctx))
>          return (tctx', Truth)
> defineTypeDef t@(EnumTy n) t'@(EnumTy _) tctx
>     = do
>          error "todo: implement enum on enum"
> defineTypeDef t t' tctx
>     = do
>          let
>              actualTyDef tn td = case td of
>                Struct _ n -> ((TyCon $ ensureElabStructName n), True)
>                EnumTy n -> ((TyCon $ ensureElabEnumName n), True)
>                Pointer td' -> (Pointer td'', b)
>                  where (td'', b) = actualTyDef tn td'
>                QualTy td' -> (QualTy td'', b)
>                  where (td'', b) = actualTyDef tn td'
>                tc@(TyCon n) -> case Map.lookup n (tyctx tctx) of
>                                  Nothing -> (tc, True)
>                                  Just tinfo -> tinfo
>                _ -> (td, False)
>              tctx' = TyCtx $ maybe (Map.insert (nameOf t) (actualTyDef t t') (tyctx tctx))
>                                    (const (tyctx tctx))
>                                    (Map.lookup (nameOf t) (tyctx tctx))
>          return (tctx' , Truth)

> replaceTy :: TyCtx -> Ty -> Ty
> replaceTy tctx t@(TyCon n)
>     = maybe t fst (Map.lookup n (tyctx tctx))
> replaceTy tctx t@(TyVar _)
>     = t
> replaceTy tctx (FunTy t ts)
>     = FunTy (replaceTy tctx t)
>             (map (replaceTy tctx) ts)
> replaceTy tctx (QualTy t)
>     = QualTy (replaceTy tctx (dropQual t))
> replaceTy tctx (Pointer t)
>     = Pointer (replaceTy tctx t)
> replaceTy tctx (Struct fs n)
>     = Struct (map (\f -> f{ty = replaceTy tctx (ty f)}) fs)
>              n
> replaceTy tctx t@(EnumTy _)
>     = Data.BuiltIn.int

> stage2 :: VarCtx -> Constraint -> SolverM (VarCtx, Constraint)
> stage2 vtx (n :<-: t)
>     = case Map.lookup n (varctx vtx) of
>         Just (t',_) -> return (vtx, t :=: t')
>         Nothing ->
>             do
>               v <- fresh
>               return (VarCtx $ Map.insert n (v,False) (varctx vtx)
>                      , v :=: t)
> stage2 vtx (Def n t c)
>     = stage2 (VarCtx $ Map.insert n (t, True) (varctx vtx)) c
> stage2 vtx (c :&: c')
>     = do
>         (vtx1, c1) <- stage2 vtx c
>         (vtx2, c2) <- stage2 vtx1 c'
>         let
>           preferQualTy (lt@(QualTy _), b) _ = (lt, b)
>           preferQualTy _ (rt@(QualTy _), b) = (rt, b)
>           preferQualTy l r = l
>         return (VarCtx $ Map.unionWith preferQualTy (varctx vtx1) (varctx vtx2)
>                , c1 :&: c2)
> stage2 vtx c@(Has _ _)
>     = return (vtx, c)
> stage2 vtx c@(_ :=: _)
>     = return (vtx, c)
> stage2 vtx (Const n)
>     = case Map.lookup n (varctx vtx) of
>         Nothing -> error "const can only be applied on known values"
>         Just (t', b) ->
>           do
>             v <- fresh
>             return (VarCtx $ Map.insert n (QualTy t', b) (varctx vtx)
>                     , Truth)
> stage2 vtx Truth
>     = return (vtx, Truth)


> stage3 :: Constraint -> ([Constraint], [Constraint])
> stage3 (c :&: c') = (eq ++ eq', fs ++ fs')
>                     where
>                        (eq,fs) = stage3 c
>                        (eq',fs') = stage3 c'
> stage3 c@(Has _ _) = ([], [c])
> stage3 c@(_ :=: _) = ([c],[])
> stage3 Truth = ([],[])


> stage4 :: TyCtx -> VarCtx -> [Constraint] -> Subst -> SolverM (TyCtx, VarCtx, Subst)
> stage4 tcx vcx fs s
>     = do
>         -- Fields must be ordered by their owning struct and, within each
>         -- struct, ordered by name, because we unify on adjacent fields.
>         let
>           fsubs = apply s fs
>           tyAndFieldPred (Has t (Field fn _)) (Has t' (Field fn' _))
>               | tn == tn' = compare fn fn'
>               | otherwise = compare tn tn'
>               where tn = (nameOf t)
>                     tn' = (nameOf t')
>           sortedByTyAndFields = List.sortBy tyAndFieldPred fsubs
>         s1 <- unifyFields sortedByTyAndFields
>
>         let
>           -- Build a map of fields using as key a type name.
>           s2 = s1 @@ s
>           go (Has n t) ac = maybe (Map.insert (nameOf n) [apply s2 t] ac)
>                                   (\ts -> Map.insert (nameOf n) ((apply s2 t):ts) ac)
>                                   (Map.lookup (nameOf n) ac)
>           fieldMap = foldr go Map.empty (apply s2 fs)

>           -- We need one representation for each field, so equivalent ones are
>           -- merged. This is OK because we're processing correct programs.
>           orderByConTy [] = []
>           orderByConTy (x:xs)
>               | isVar (nameOf (ty x)) = (orderByConTy xs) ++ [x]
>               | otherwise = x:(orderByConTy xs)
>           fieldMapOrd = foldr (\(n, fs) acc -> (n, orderByConTy fs):acc) [] (Map.toList fieldMap)
>           mergeFields (n, fs) acc = (n, List.nubBy (\f1 f2 -> (name f1) == (name f2)) fs): acc
>           fieldMap' = foldr mergeFields [] fieldMapOrd
>           fieldMap'' = Map.fromList fieldMap'
>           s' = Subst $ Map.mapWithKey (\v fs -> Struct fs v) fieldMap''
>           s'' = s' @@ s2
>
>           -- Apply substitutions.
>           tcx_ = Map.map (\(t,b) -> (apply s'' t, b)) (tyctx tcx)
>           vcx_ = Map.map (\(t,b) -> (apply s'' t, b)) (varctx vcx)
>
>           -- Combine the fields into the structs. On the first time, we
>           -- operate on known structs (declared in the original program) and
>           -- keep track of them. On the second time, we re-combine the fields
>           -- for variables which we know are of some struct time, but along
>           -- the way we might discover new structs which were not declared.
>           -- Finally, we pass again over the type and variable environments
>           -- in order to combine the newly discovered structs which can
>           -- potentially be fields of any other struct.
>           (declaredStructs, tcx_') = Map.mapAccumWithKey combine Map.empty tcx_
>           (knownStructs', vcx_') = Map.mapAccum recombine (0, declaredStructs) $ undefVars vcx_
>           (knownStructs'', tcx_'') = Map.mapAccum recombine knownStructs' tcx_'
>           (_, vcx_'') = Map.mapAccum recombine knownStructs'' vcx_'
>
>           -- Deal with "orphan" types. As above, we make this in a two-phase
>           -- stage: first, to discover compound types orphans and, aferwards,
>           -- orphans that can be a plain uintptr_t.
>           wrapReplace acc k (t, b) = (acc', (t', b))
>             where (acc', t') = replaceOrphan acc k t id
>           (knownOrphans, tcx_''') = Map.mapAccumWithKey wrapReplace Map.empty tcx_''
>           (knownOrphans', vcx_''') = Map.mapAccumWithKey wrapReplace knownOrphans vcx_''
>           wrapReplace2 acc k (t, b) = (acc', (t', b))
>             where (acc', t') = replaceOrphan acc k t (const uintptr_t)
>           (knownOrphans'', tcx_'''') = Map.mapAccumWithKey wrapReplace2 knownOrphans' tcx_'''
>           (_, vcx_'''') = Map.mapAccumWithKey wrapReplace2 knownOrphans'' vcx_'''
>
>         return (TyCtx tcx_'''', VarCtx vcx_'''', s'')

> -- TODO: Merge combine/recombine.
> combine acc n ((Struct fs v), b) -- TODO: Fields.
>   | isVar v = case Map.lookup v acc of
>                 Nothing -> (Map.insert v (ensureElabStructName n) acc,
>                             (apply (v +-> (TyCon (ensureElabStructName n))) $ Struct fs (ensureElabStructName n), b))
>                 Just n' -> (acc, (TyCon n', b))
>   | otherwise = (acc, (Struct fs v, b))
> combine acc n (t@(FunTy rt ps), b) = (acc'', (FunTy rt' ps', b))
>   where (acc', (rt', _)) = combine acc (nameOf rt) (rt, b)
>         (acc'', ps') = combineParams acc' ps
>         combineParams pacc [] = (pacc, [])
>         combineParams pacc (x:xs) = (pacc'', x':xs')
>           where (pacc', (x', _)) = combine pacc (nameOf x) (x, False)
>                 (pacc'', xs') = combineParams pacc' xs
> combine acc n ((Pointer t), b) = (acc', (Pointer t', b))
>   where (acc', (t', _)) = combine acc n (t, b)
> combine acc n ((QualTy t), b) = (acc', (QualTy t', b))
>   where (acc', (t', _)) = combine acc n (t, b)
> combine acc n (t, b) = (acc, (t, b))

> recombine acc@(cnt, table) (t@(Struct fs v), b)
>   | isVar v = case Map.lookup v table of
>                   Nothing -> ((cnt + 1, Map.insert v newName table),
>                               (apply (v +-> (TyCon newName)) $ Struct fs' newName, b))
>                   Just n -> (acc, (TyCon n, b))
>   | otherwise = (acc, ((Struct fs' v), b))
>   where newName = ensureElabStructName (Name ("T" ++ (show cnt)))
>         (_, fs') = combineFields acc fs
>         combineFields facc [] = (facc, [])
>         combineFields facc (x@(Field fn ft):xs) = (facc'', (Field fn ft'):xs')
>            where (facc', (ft', _)) = recombine facc (ft, False)
>                  (facc'', xs') = combineFields facc' xs
> recombine acc (t@(FunTy rt ps), b) = (acc'', (FunTy rt' ps', b))
>   where (acc', (rt', _)) = recombine acc (rt, b)
>         (acc'', ps') = combineParams acc' ps
>         combineParams pacc [] = (pacc, [])
>         combineParams pacc (x:xs) = (pacc'', x':xs')
>           where (pacc', (x', _)) = recombine pacc (x, False)
>                 (pacc'', xs') = combineParams pacc' xs
> recombine acc (t@(Pointer t'), b) = (acc', (Pointer t'', b))
>   where (acc', (t'', _)) = recombine acc (t', b)
> recombine acc (t@(QualTy t'), b) = (acc', (QualTy t'', b))
>   where (acc', (t'', _)) = recombine acc (t', b)
> recombine acc@(_, table) (t, b)
>   | isVar t = case Map.lookup (nameOf t) table of
>                    Nothing -> (acc, (t, b))
>                    Just n -> (acc, (TyCon n, b))
>   | otherwise = (acc, (t, b))


> unifyList :: [Constraint] -> SolverM Subst
> unifyList [] = return nullSubst
> unifyList ((t :=: t') : ts)
>   = do
>       s <- unify t t'
>       --liftIO (print $ pprint (t :=: t') <+> text "|" <+> pprint s)
>       s' <- unifyList (apply s ts)
>       return (s' @@ s)

> unifyFields :: [Constraint] -> SolverM Subst
> unifyFields = unifyList . eqlist
>               where
>                 typeFrom (Field _ t) = t
>                 eqlist [] = []
>                 eqlist [ (Has _ t) ] = [ ]
>                 eqlist (h@(Has _ t@(Field n ty)) : h'@(Has _ t'@(Field n' ty')) : fs)
>                   = if n == n' then (((typeFrom t) :=: (typeFrom t')) : eqlist (h':fs)) else (eqlist (h':fs))


> -- TODO: Extract a common function to encapsulate the functionality  of
> -- replaceOrphan, replaceTy, recombine.
> replaceOrphan acc k (Struct fs n) f = (acc', Struct fs' n)
>   where (acc', fs') = combineFields acc fs
>         combineFields facc [] = (facc, [])
>         combineFields facc (x@(Field fn ft):xs) = (facc'', (Field fn ft'):xs')
>            where (facc', ft') = replaceOrphan facc (nameOf ft) ft f
>                  (facc'', xs') = combineFields facc' xs
> replaceOrphan acc k (FunTy rt ps) f = (acc'', FunTy rt' ps')
>   where (acc', rt') = replaceOrphan acc (nameOf rt) rt f
>         (acc'', ps') = combineParams acc' ps
>         combineParams pacc [] = (pacc, [])
>         combineParams pacc (x:xs) = (pacc'', x':xs')
>           where (pacc', x') = replaceOrphan pacc (nameOf x) x f
>                 (pacc'', xs') = combineParams pacc' xs
> replaceOrphan acc k (Pointer t) f = (acc', Pointer t')
>   where (acc', t') = replaceOrphan acc k t f
> replaceOrphan acc k (QualTy t) f = (acc', QualTy t')
>   where (acc', t') = replaceOrphan acc k (dropQual t) f
> replaceOrphan acc k v f
>   | isVar v = case Map.lookup v acc of
>                   Just t -> (acc, t)
>                   Nothing -> if isElabStructName k
>                              then (Map.insert v (TyCon k) acc, Struct [] k)
>                              else if isElabEnumName k
>                                   then (Map.insert v (TyCon k) acc, EnumTy k)
>                                   else (acc, f v)
>   | otherwise = (acc, v)


> isVar :: Pretty a => a -> Bool
> isVar = (== "#alpha") . take 6 . show . pprint
