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
import Solver.Retypeable
import Utils.Pretty

import Debug.Trace


solver :: Constraint -> IO (Either String (TyCtx, VarCtx))
solver c = runSolverM (solve c) (((length $ fv c) + 1), 1)

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
    (tcx1, vcx1, s''') <- stage4 tc0 vcx fds s''

    -- Translate structural representation to a nominative one.
    (tcx_n, vcx_n) <- stage5 tcx1 vcx1 s'''

    -- Unify against the top type: void*.
    let vqs = apply s''' iq4
    ss <- dunifyList vqs
    let ss' = ss @@ s'''
        tcx11 = TyCtx $ Map.map (\(t,b) -> (apply ss' t, b)) (tyctx tcx_n)
        vcx11 = VarCtx $ Map.map (\varInfo -> apply ss' varInfo) (varctx vcx_n)

        -- Orphanification.
        (tcx1', vcx1') = stage6 tcx11 vcx11

    -- Decay function pointers.
    (tcx2, vcx2) <- decay tcx1' vcx1'

    -- Remove builtins and standard library components.
    let
        tcx2_ = undefTys (tyctx tcx2) Map.\\ builtinTyCtx Map.\\ stdTyCtx
        vcx2_ = undefVars (varctx vcx2) Map.\\ builtinVarCtx Map.\\ stdVarCtx

    return (TyCtx tcx2_, VarCtx vcx2_)


cleanv c = VarCtx $  (varctx c) Map.\\ builtinVarCtx Map.\\ stdVarCtx
cleant c = TyCtx $  (tyctx c) Map.\\ builtinTyCtx Map.\\ stdTyCtx

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
    | hasVarDep t = ([], [], [c], [])
    | t == Data.BuiltIn.void = ([], [], [], [c])
    | otherwise = ([c], [], [], [])
dsort' c@(_ :>: (Pointer t))
    | t == Data.BuiltIn.void = ([], [], [], [c])
    | otherwise =  ([], [], [c], [])
dsort' c@((Pointer t) :>: _)
    | hasVarDep t = ([], [], [c], [])
    | t == Data.BuiltIn.void = ([], [], [], [c])
    | otherwise = case t of
        QualTy _ -> ([], [], [c], [])
        _ -> ([], [c], [], [])
dsort' c = ([], [], [c], [])


-- | Assemble records by unified their fields. After this stage, all typing information
-- (including composite types) is complete. However, as in a structural type system.
stage4 :: TyCtx -> VarCtx -> [Constraint] -> Subst -> SolverM (TyCtx, VarCtx, Subst)
stage4 tcx vcx fs s = do
  -- To unify fields we need to sort them first (fields beloging to the same record will be
  -- adjacent one to the other). Each unification might discover additional relations:
  -- nested structs. So we need do this recursively, until no substitutions are generated.
  let
    discover fs s = do
      let sorted = sortFields fs s
      s' <- punifyFields sorted
      if s' /= nullSubst then discover sorted (s' @@ s) else return s
  s1 <- discover fs s

  let
    -- Build a map of fields using as key a type name.
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

  return (TyCtx tcx_, VarCtx vcx_, s'')


-- | Bring structucally represented typing information into C world, by matching types with
-- their declared names, eventually creating fake names for the undeclared types.
stage5 :: TyCtx -> VarCtx -> Subst -> SolverM (TyCtx, VarCtx)
stage5 tcx vcx s = do
  let
    -- We filter types declared through elaborated names and work on them independently,
    -- since we cannot typedef them.
    elabs = Map.foldrWithKey (\k (t, _) acc ->
      if isElabStructName k then acc %% (t %-> k) else acc) nullIdx (tyctx tcx)
    tcxFlt_ = Map.filterWithKey (\k _ -> (not . isElabStructName) k) (tyctx tcx)
    tcxFlt_' = Map.map (\(t, b) -> (canonicalize elabs t , b)) tcxFlt_
    vcxFlt_ = Map.map (\(VarInfo t b ro) -> VarInfo (canonicalize elabs t) b ro) (varctx vcx)

    tcxElab_ = (tyctx tcx) Map.\\ tcxFlt_'
    tcxElab_' = Map.mapWithKey (\k (t, b) -> (unalpha2 k t, b)) tcxElab_
    tcxElab_'' = Map.mapWithKey (\k (t, b) -> (keepElab k t, b)) tcxElab_'

    -- Collect composite types so we can canonicalize them. Througout this process we also
    -- look into the substitutions because nested structs will only appear there.
    composite = Map.foldr (\(t, _) acc -> acc ++ collect t) [] tcxFlt_'
    composite' = Map.foldr (\(VarInfo t _ _) acc -> acc ++ collect t) composite vcxFlt_

    m = Set.fromList composite'
    consider [] cs q = (cs, q)
    consider (t@(Struct _ n):tx) cs q
      | Set.member t m = consider tx cs q
      | isVar n = ([t] ++ rest, q'')
      | otherwise = consider tx cs q
     where
      q' = Set.insert t q
      (rest, q'') = consider tx cs q'
    consider (t:tx) cs q = error "only struct types are collected"
    (composite'', _) = Map.foldr (\t (xs, q) -> consider (collect t) xs q) (composite', m) (subs s)

  -- Assign names and keep record of the type/name relation through an index. Those are
  -- inserted into the typing context as well.
  idx <- foldM (\acc t -> makeName acc t) nullIdx composite''

  let
    tcx_ = Map.map (\(t, b) -> (canonicalize idx t, b)) tcxFlt_'
    vcx_ = Map.map (\(VarInfo t b ro) -> VarInfo (canonicalize idx t) b ro) vcxFlt_
    tcx_' = Map.foldrWithKey (\t n acc -> Map.insert n (t, False) acc) tcx_ (ty2n idx)

  -- "De-alphasize" top-level names so they match the ones we created.
  tcx_'' <- mapM (\c -> unalpha c) tcx_'

  let
    -- "De-alphasize" fields from composite types.
    n2n = Map.foldrWithKey (\(Struct _ n) n' acc -> Map.insert n n' acc) Map.empty (ty2n idx)
    update t@(TyVar v) = maybe t (TyCon) (Map.lookup v n2n)
    update t@(TyCon _) = t
    update t@(EnumTy _) = t
    update (QualTy t) = QualTy (update t)
    update (Pointer t) = Pointer (update t)
    update (FunTy t tx) = FunTy (update t) (map (\t -> update t) tx)
    update (Struct fs n) = Struct (map (\(Field fn ft) -> Field fn (update ft)) fs) n
    tcx_''' = Map.map (\(t, b) -> (update t, b)) tcx_''
    tcxElab_''' = Map.map (\(t, b) -> (update t, b)) tcxElab_''

  return (TyCtx $ tcx_''' `Map.union` tcxElab_''', VarCtx vcx_)


-- | Name a composite type by increasing IDs.
makeName :: TyIdx -> Ty -> SolverM TyIdx
makeName idx t@(Struct _ _ ) = do
  n <- fakeName
  return $ idx %% (t %-> n)
makeName _ _ = error "cannot happen, only for composite"


-- | Enforce that a struct which is referenced by an elaborated name is accordingly named.
keepElab :: Name -> Ty -> Ty
keepElab k t@(Struct fs n)
  | isVar n && isElabStructName k = Struct fs k
  | otherwise = t
keepElab _ t = t


-- | Orphanize unresolved type variables.
stage6 :: TyCtx -> VarCtx -> (TyCtx, VarCtx)
stage6 tcx vcx =
  let
    pick k t@(TyVar _) acc
      | isElab k = acc %% (t %-> k)
      | otherwise = acc
    pick _ _ acc = acc
    elabOrph = Map.foldrWithKey (\k (t, _) acc -> pick k t acc) nullIdx (tyctx tcx)

    dummy n
      | isElabStructName n = Struct [Field (Name "dummy") Data.BuiltIn.int] n
      | otherwise = EnumTy n
    go k t =
      case Map.lookup t (ty2n elabOrph) of
        Just k' -> if k == k' then dummy k else orphanize elabOrph t
        Nothing -> orphanize elabOrph t
    tcx_ = Map.mapWithKey (\k (t, b) -> (go k t, b)) (tyctx tcx)
    vcx_ = Map.map (\(VarInfo t b ro) -> VarInfo (orphanize elabOrph t) b ro) (varctx vcx)
  in
    (TyCtx tcx_, VarCtx vcx_)


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


sortFields :: [Constraint] -> Subst -> [Constraint]
sortFields fs s =
  let
    fsubs = apply s fs
    tyAndFieldPred (Has t (Field fn _)) (Has t' (Field fn' _))
      | tn == tn' = compare fn fn'
      | otherwise = compare tn tn'
     where
      tn = (nameOf t)
      tn' = (nameOf t') in
  List.sortBy tyAndFieldPred fsubs


punifyFields :: [Constraint] -> SolverM Subst
punifyFields =
  punifyList . eqlist
 where
  typeFrom (Field _ t) = t
  eqlist [] = []
  eqlist [ (Has _ t) ] = [ ]
  eqlist (h@(Has _ t@(Field n ty)) : h'@(Has _ t'@(Field n' ty')) : fs)
    | n == n' = (((typeFrom t) :=: (typeFrom t')) : eqlist (h':fs))
    | otherwise = (eqlist (h':fs))
