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

module Solver.ConstraintSolver where

import Data.Map (Map)
import qualified Data.Map as Map
import Data.Set (Set)
import qualified Data.Set as Set
import qualified Data.List as List
import Data.Maybe (isJust, fromJust)

import Data.Constraints
import Data.Type
import Data.BuiltIn

import Control.Monad hiding (void)
import Control.Monad.Trans
import Control.Monad.State hiding (void)
import Control.Monad.Except hiding (void)

import Solver.SolverMonad
import Solver.Unification
import Solver.ConversionRules
import Solver.Decaying
import Utils.Pretty

import Debug.Trace


solver :: Constraint -> IO (Either String (TyCtx, VarCtx))
solver c = runSolverM (solve c) ((length $ fv c) + 1)

solve :: Constraint -> SolverM (TyCtx, VarCtx)
solve c = do
    -- Expand typedefs.
    (tc0,c') <- stage1 (TyCtx $ Map.union builtinTyCtx stdTyCtx) c

    -- Expand variable types and create missing variables.
    (vcx,c'') <- stage2 (VarCtx $ Map.union builtinVarCtx stdVarCtx) c'

    -- Split constraints into equality, inequality, and field acess.
    let (eqs, iqs, fds) = stage3 c''

    -- Run plain unification on equalities and apply substitutions on the inequalities. This will
    -- instantiate types and make it possible to sort the inequalities according to a desired
    -- directionality criteria. Therefore, once directional unification is run on the inequalities
    -- type variables are bound in a proper order, that corresponds to a subtyping order.
    s <- punifyList eqs
    let iqs' = apply s iqs
        (iq1, iq2, iq3, iq4) = dsort iqs'
        iqs'' = iq1 ++ iq2 ++ iq3
    s' <- dunifyList iqs''
    let s'' = s' @@ s

    -- Assemble records.
    (tcx1, vcx1, s''', cnt) <- stage4 tc0 vcx fds s''

    -- Unify against the top type: void*.
    let vqs = apply s''' iq4
    ss <- dunifyList vqs
    let ss' = ss @@ s'''
        tcx11 = TyCtx $ Map.map (\(t,b) -> (apply ss' t, b)) (tyctx tcx1)
        vcx11 = VarCtx $ Map.map (\varInfo -> apply ss' varInfo) (varctx vcx1)

     -- Orphanification.
    (tcx1', vcx1') <- stage5 tcx11 vcx11 cnt

    -- Decay function pointers.
    (tcx2, vcx2) <- decay tcx1' vcx1'

    -- Remove builtins and standard library components.
    let
        tcx2_ = undefTys (tyctx tcx2) Map.\\ builtinTyCtx Map.\\ stdTyCtx
        vcx2_ = undefVars (varctx vcx2) Map.\\ builtinVarCtx Map.\\ stdVarCtx

    return (TyCtx tcx2_, VarCtx vcx2_)


--cleanv c = VarCtx $  (varctx c) Map.\\ builtinVarCtx Map.\\ stdVarCtx
--cleant c = TyCtx $  (tyctx c) Map.\\ builtinTyCtx Map.\\ stdTyCtx

stage1 :: TyCtx -> Constraint -> SolverM (TyCtx, Constraint)
stage1 tctx (t :=: t') =
    return (tctx, (replaceTy tctx t) :=: (replaceTy tctx t'))
stage1 tctx (t :>: t') =
    return (tctx, (replaceTy tctx t) :>: (replaceTy tctx t'))
stage1 tctx (n :<-: t) =
    return (tctx, n :<-: (replaceTy tctx t))
stage1 tctx (Has n (Field n' t)) =
    return (tctx, Has n (Field n' (replaceTy tctx t)))
stage1 tctx (TypeDef t t') = defineTypeDef t t' tctx
stage1 tctx (c :&: c') = do
    (tcx1, c1) <- stage1 tctx c
    (tcx2, c1') <- stage1 tcx1 c'
    return (tcx2, c1 :&: c1')
stage1 tctx (Exists n c) = do
    v <- fresh
    stage1 tctx (apply (n +-> v) c)
stage1 tctx (Def n t c) = do
    (tcx, c')  <- stage1 tctx c
    return (tcx, Def n (replaceTy tctx t) c')
stage1 tctx c@(ReadOnly _) = return (tctx, c)
stage1 tctx Truth = return (tctx, Truth)

defineTypeDef :: Ty -> Ty -> TyCtx -> SolverM (TyCtx, Constraint)
defineTypeDef (Pointer t) (Pointer t') tctx = do
    (tcx,c) <- defineTypeDef t t' tctx
    return (tcx, (t :=: t') :&: c)
defineTypeDef t@(Pointer l) t'@(TyVar v) tctx = do
    v' <- fresh
    (tcx, c) <- defineTypeDef l v' tctx
    return (tcx, (t' :=: (Pointer v')) :&: c)
defineTypeDef (QualTy t) (QualTy t') tctx = do
    (tcx,c) <- defineTypeDef t t' tctx
    return (tcx, (t :=: t') :&: c)
defineTypeDef t@(QualTy l) t'@(TyVar v) tctx = do
     v' <- fresh
     (tcx, c) <- defineTypeDef l v' tctx
     return (tcx, (t' :=: (QualTy v')) :&: c)
defineTypeDef t@(EnumTy n) t'@(TyVar v) tctx = do
    let
      tctx' = TyCtx $ maybe (Map.insert n (EnumTy n, False) (tyctx tctx))
                            (const (tyctx tctx))
                            (Map.lookup n (tyctx tctx))
    return (tctx', Truth)
defineTypeDef t@(EnumTy n) t'@(EnumTy _) tctx = error "TODO: Implement me."
defineTypeDef t t' tctx = do
    let
      actualTyDef tn td =
        case td of
             Struct _ n -> ((TyCon $ ensureElabStructName n), True)
             EnumTy n -> ((TyCon $ ensureElabEnumName n), True)
             Pointer td' -> (Pointer td'', b)
               where (td'', b) = actualTyDef tn td'
             QualTy td' -> (QualTy td'', b)
               where (td'', b) = actualTyDef tn td'
             tc@(TyCon n) ->
               case Map.lookup n (tyctx tctx) of
                    Nothing -> (tc, True)
                    Just tinfo -> tinfo
             -- We don't want to have declared (but undefined) functions in the context
             -- marked as undeclared.
             _ -> (td, case tn of { FunTy _ _ ->  True; _ -> False; })
      tctx' = TyCtx $ maybe (Map.insert (nameOf t) (actualTyDef t t') (tyctx tctx))
                            (const (tyctx tctx))
                            (Map.lookup (nameOf t) (tyctx tctx))
    return (tctx' , Truth)

replaceTy :: TyCtx -> Ty -> Ty
replaceTy tctx t@(TyCon n) = maybe t fst (Map.lookup n (tyctx tctx))
replaceTy tctx t@(TyVar _) = t
replaceTy tctx (FunTy t ts) = FunTy (replaceTy tctx t) (map (replaceTy tctx) ts)
replaceTy tctx (QualTy t) = QualTy (replaceTy tctx t)
replaceTy tctx (Pointer t) = Pointer (replaceTy tctx t)
replaceTy tctx (Struct fs n) =
    Struct (map (\f -> f{ty = replaceTy tctx (ty f)}) fs) n
replaceTy tctx t@(EnumTy _) = Data.BuiltIn.int


stage2 :: VarCtx -> Constraint -> SolverM (VarCtx, Constraint)
stage2 vtx (n :<-: t@(FunTy rt pts)) =
    case Map.lookup n (varctx vtx) of
        Just info ->
            case varty info of
                FunTy rt' pts' -> do
                    let pcs = zipWith (\t t' -> (t' :>: t)) pts pts'
                        pcs' = foldr (\c acc -> c :&: acc) Truth pcs
                        rtc = rt' :>: rt
                    return (vtx, pcs' :&: rtc)
                -- FIXME: Verify scoping.
                t' -> return (vtx, t' :=: t) -- error (show $ pprint t)
        Nothing -> do
            v <- fresh
            return ( VarCtx $ Map.insert n (VarInfo v False False) (varctx vtx)
                    , v :=: t )
stage2 vtx (n :<-: t) =
    case Map.lookup n (varctx vtx) of
        Just info -> return (vtx, varty info :=: t)
        Nothing -> do
            v <- fresh
            return ( VarCtx $ Map.insert n (VarInfo v False False) (varctx vtx)
                   , v :=: t )
stage2 vtx (Def n t c) =
    stage2 (VarCtx $ Map.insert n (VarInfo t True False) (varctx vtx)) c
stage2 vtx (c :&: c') = do
    (vtx1, c1) <- stage2 vtx c
    (vtx2, c2) <- stage2 vtx1 c'
    let
        preferQual (VarInfo lt@(QualTy _) b ro) _ = VarInfo lt b ro
        preferQual _ (VarInfo rt@(QualTy _) b ro) = VarInfo rt b ro
        preferQual l r = l
    return ( VarCtx $ Map.unionWith preferQual (varctx vtx1) (varctx vtx2)
           , c1 :&: c2 )
stage2 vtx c@(Has _ _) = return (vtx, c)
stage2 vtx c@(_ :=: _) = return (vtx, c)
stage2 vtx c@(_ :>: _) = return (vtx, c)
stage2 vtx (ReadOnly n) =
    case Map.lookup n (varctx vtx) of
        Nothing -> error "const can only be applied on known values"
        Just info -> do
             v <- fresh
             return ( VarCtx $ Map.insert n
                               (VarInfo (QualTy (varty info)) (declared info) True)
                               (varctx vtx)
                    , Truth)
stage2 vtx Truth = return (vtx, Truth)


stage3 :: Constraint -> ([Constraint], [Constraint], [Constraint])
stage3 (c :&: c') =
    (eq ++ eq', iq ++ iq', fs ++ fs')
  where
    (eq, iq, fs) = stage3 c
    (eq',iq', fs') = stage3 c'
stage3 c@(Has _ _) = ([], [], [c])
stage3 c@(_ :=: _) = ([c], [], [])
stage3 c@(_ :>: _) = ([], [c], [])
stage3 Truth = ([], [], [])


dsort :: [Constraint] -> ([Constraint], [Constraint], [Constraint], [Constraint])
dsort [] = ([], [], [], [])
dsort (x:xs) =
    (eq1 ++ eq1', eq2 ++ eq2', eq3 ++ eq3', eq4 ++ eq4')
  where
    (eq1, eq2, eq3, eq4) = dsort' x
    (eq1', eq2', eq3', eq4') = dsort xs

dsort' :: Constraint -> ([Constraint], [Constraint], [Constraint], [Constraint])
dsort' c@(_ :>: (Pointer (QualTy t)))
    | isTyVarDep t = ([], [], [c], [])
    | t == Data.BuiltIn.void = ([], [], [], [c])
    | otherwise = ([c], [], [], [])
dsort' c@(_ :>: (Pointer t))
    | t == Data.BuiltIn.void = ([], [], [], [c])
    | otherwise =  ([], [], [c], [])
dsort' c@((Pointer t) :>: _)
    | isTyVarDep t = ([], [], [c], [])
    | t == Data.BuiltIn.void = ([], [], [], [c])
    | otherwise = case t of
        QualTy _ -> ([], [], [c], [])
        _ -> ([], [c], [], [])
dsort' c = ([], [], [c], [])


stage4 :: TyCtx -> VarCtx -> [Constraint] -> Subst -> SolverM (TyCtx, VarCtx, Subst, Int)
stage4 tcx vcx fs s = do
    -- We must sort fields by struct and by name.
    let
        fsubs = apply s fs
        tyAndFieldPred (Has t (Field fn _)) (Has t' (Field fn' _))
            | tn == tn' = compare fn fn'
            | otherwise = compare tn tn'
          where
            tn = (nameOf t)
            tn' = (nameOf t')
        sortedByTyAndFields = List.sortBy tyAndFieldPred fsubs
    s1 <- punifyFields sortedByTyAndFields

    -- Build a map of fields using as key a type name.
    let
        s2 = s1 @@ s
        go (Has n t) ac = maybe (Map.insert (nameOf n) [apply s2 t] ac)
                                (\ts -> Map.insert (nameOf n) ((apply s2 t):ts) ac)
                                (Map.lookup (nameOf n) ac)
        fieldMap = foldr go Map.empty (apply s2 fs)

        -- Fields might be used several times, but we only need one of each.
        orderByConTy [] = []
        orderByConTy (x:xs)
             | isVar (nameOf (ty x)) = (orderByConTy xs) ++ [x]
             | otherwise = x:(orderByConTy xs)
        fieldMapOrd = foldr (\(n, fs) acc -> (n, orderByConTy fs):acc) [] (Map.toList fieldMap)
        mergeFields (n, fs) acc = (n, List.nubBy (\f1 f2 -> (name f1) == (name f2)) fs): acc
        fieldMap' = foldr mergeFields [] fieldMapOrd
        fieldMap'' = Map.fromList fieldMap'
        s' = Subst $ Map.mapWithKey (\v fs -> Struct fs v) fieldMap''
        s'' = s' @@ s2

        -- Apply substitutions.
        tcx_ = Map.map (\(t,b) -> (apply s'' t, b)) (tyctx tcx)
        vcx_ = Map.map (\varInfo -> apply s'' varInfo) (varctx vcx)

        -- We now need to combine type variables which are of struct types to
        -- their corresponding structs. If the given structs are declared in the
        -- program, their names are supplied to the type constructor. Otherwise,
        -- we create fake names for them. TODO: Refactor this little mess...
        acc = (0, Map.empty, Map.empty)
        keepType acc k t = (acc, t)
        makeElabStruct acc@(cnt, nonOrphan, orphan) n v fs =
            ( (cnt, Map.insert v t' nonOrphan, orphan)
            , apply (v +-> t') $ Struct fs n')
          where
            n' = ensureElabStructName n
            t' = TyCon n'
        (pending, tcx_') = Map.mapAccumWithKey (retype keepType makeElabStruct) acc tcx_

        -- Fake structs
        makeFakeStruct acc@(cnt, nonOrphan, orphan) _ v fs' =
            ( (cnt + 1, Map.insert v t' nonOrphan, orphan)
            , apply (v +-> t') $ Struct fs' fakeName)
          where
            t' = TyCon fakeName
            fakeName = ensureElabStructName (Name ("T" ++ (show cnt)))

        -- TODO: Store nested structs names in non-orphans map.
        -- This has gotten messy... Need to refactor and split this stage.
        structonly t@(Struct fs n) = Struct (map (applyCore2 s'' Set.empty) fs) n
        structonly t@(Pointer t') = Pointer (structonly t')
        structonly t = t
        structonly' (t, b) = (structonly t, b)
        tcx_2 = Map.map structonly' tcx_'
        structonly'' (VarInfo t b ro) = (VarInfo (structonly t) b ro)
        vcx_2 = Map.map structonly'' vcx_'

        typefy acc@(_, nonOrphan, orphan) k t =
            case Map.lookup (nameOf t) nonOrphan of
                Just t' -> (acc, t')
                Nothing -> (acc, t)
        (pending1, vcx_') = Map.mapAccumWithKey
            (retypeVar typefy makeFakeStruct) pending $ undefVars vcx_
        (pending2, tcx_'') = Map.mapAccumWithKey
            (retype typefy makeFakeStruct) pending1 tcx_2
        (pending3, vcx_'') = Map.mapAccumWithKey
            (retypeVar typefy makeFakeStruct) pending2 vcx_2

        s''' = Subst $ ((\(_, x, _) -> x) pending3)
        cnt = (\(x, _, _) -> x) pending3

    return (TyCtx tcx_'', VarCtx vcx_'', s''', cnt)


stage5 :: TyCtx -> VarCtx -> Int -> SolverM (TyCtx, VarCtx)
stage5 tcx vcx cnt = do
    let
        -- Deal with orphans
        orphanize f acc@(cnt, nonOrphan, orphan) k v =
            case Map.lookup v orphan of
                Just t -> (acc, t)
                Nothing -> if isElabStructName k
                    then ( (cnt, nonOrphan, Map.insert v (TyCon k) orphan)
                         , Struct [ Field (Name "dummy") Data.BuiltIn.int ] k)
                    else if isElabEnumName k
                        then ( (cnt, nonOrphan, Map.insert v (TyCon k) orphan)
                             , EnumTy k)
                        else (acc, f v)

        pending3 = (cnt, Map.empty, Map.empty)
        (pending4, tcx_''') = Map.mapAccumWithKey
            (retype (orphanize id) (error "error")) pending3 (tyctx tcx)
        (pending5, vcx_''') = Map.mapAccumWithKey
            (retypeVar (orphanize id) (error "error")) pending4 (varctx vcx)
        (pending6, tcx_'''') = Map.mapAccumWithKey
            (retype (orphanize (const orphan)) (error "error")) pending5 tcx_'''
        (_, vcx_'''') = Map.mapAccumWithKey
            (retypeVar (orphanize (const orphan)) (error "error")) pending6 vcx_'''

    return (TyCtx tcx_'''', VarCtx vcx_'''')


retype f g acc k (t, b) = (acc', (t', b))
  where (acc', t') = combine acc k t f g
retypeVar f g acc k (VarInfo t b ro) = (acc', VarInfo t' b ro)
  where (acc', t') = combine acc k t f g

combine acc@(_, nonOrphan, _) k t@(Struct fs v) f g
    | isVar v = case Map.lookup v nonOrphan of
                    Just t' -> (acc, t')
                    Nothing -> g acc k v fs'
    | otherwise = (acc', Struct fs' v)
  where
    (acc', fs') = combine' acc fs
    combine' facc [] = (facc, [])
    combine' facc (x@(Field fn ft):xs) =
        (facc'', (Field fn ft'):xs')
      where
        (facc', ft') = combine facc fn ft f g
        (facc'', xs') = combine' facc' xs
combine acc k t@(FunTy rt ps) f g =
    (acc'', FunTy rt' ps')
  where
    (acc', rt') = combine acc k rt f g
    (acc'', ps') = combine' acc' ps
    combine' pacc [] = (pacc, [])
    combine' pacc (x:xs) =
        (pacc'', x':xs')
      where
        (pacc', x') = combine pacc (nameOf x) x f g
        (pacc'', xs') = combine' pacc' xs
combine acc k t@(Pointer t') f g = (acc', Pointer t'')
  where (acc', t'') = combine acc k t' f g
combine acc k t@(QualTy t') f g = (acc', QualTy t'')
  where (acc', t'') = combine acc k t' f g
combine acc k t f g
    | isVar t = f acc k t
    | otherwise = (acc, t)


punifyList :: [Constraint] -> SolverM Subst
punifyList [] = return nullSubst
punifyList ((t :=: t') : ts) = do
    s <- punify t t'
    --liftIO (print $ pprint (t :=: t') <+> text "|" <+> pprint s)
    s' <- punifyList (apply s ts)
    return (s' @@ s)


dunifyList :: [Constraint] -> SolverM Subst
dunifyList [] = return nullSubst
dunifyList ((t :>: t') : ts) = do
    s <- dunify t t' Relax
    s' <- dunifyList (apply s ts)
    return (s' @@ s)


punifyFields :: [Constraint] -> SolverM Subst
punifyFields = punifyList . eqlist
  where
    typeFrom (Field _ t) = t
    eqlist [] = []
    eqlist [ (Has _ t) ] = [ ]
    eqlist (h@(Has _ t@(Field n ty)) : h'@(Has _ t'@(Field n' ty')) : fs) =
        if n == n'
            then (((typeFrom t) :=: (typeFrom t')) : eqlist (h':fs))
            else (eqlist (h':fs))

isVar :: Pretty a => a -> Bool
isVar = (== "#alpha") . take 6 . show . pprint
