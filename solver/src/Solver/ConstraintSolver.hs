-- Copyright (c) 2016 Rodrigo RiAbeiro (rodrigo@decsi.ufop.br)
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
import Data.BuiltIn
import Data.CLang
import Data.Type

import Control.Monad hiding (void)
import Control.Monad.Trans
import Control.Monad.State hiding (void)
import Control.Monad.Except hiding (void)

import Solver.Decaying
import Solver.ContextAssemble
import Solver.ConversionRules
import Solver.Retypeable
import Solver.SolverMonad
import Solver.Unification

import Utils.Pretty

import Debug.Trace


solver :: Constraint -> CLang -> Bool -> IO (Either String (TyCtx, VarCtx))
solver c cl ml = runSolverM (solve c cl ml) (((maxVarId 0 c) + 1), 1)


-- | Discover the maximum type variable id.
maxVarId :: Int -> Constraint -> Int
maxVarId k (Def _ _ c) = maxVarId k c
maxVarId k (Scope c) = maxVarId k c
maxVarId k (c :&: c') = max (maxVarId k c) (maxVarId k c')
maxVarId k (Exists (Name n) c) = maxVarId (max k (varId n)) c
maxVarId k _ = k


-- | Solve constraints.
solve :: Constraint -> CLang -> Bool -> SolverM (TyCtx, VarCtx)
solve c cl ml = do
  let
    -- Populate builtin and standard library types and values.
    tcx = TyCtx $ (builtinTypes cl) `Map.union` (if ml then (stdTypes cl) else Map.empty)
    vcx = VarCtx $ (builtinValues cl) `Map.union` (if ml then (stdValues cl) else Map.empty)

  -- Populate typing context and incorporate typedefs.
  (tcx0,c') <- stage1 tcx c

  -- Populate value context, create missing variables, generalize types.
  (vcx0,c'') <- stage2 vcx c'

  {--liftIO (print $ text "c\n" <+> pprint c)
  liftIO (print $ text "tcx0\n" <+> pprint (TyCtx $ cleanTypes C99 (tyctx tcx0)))
  liftIO (print $ text "c'\n" <+> pprint c')
  liftIO (print $ text "vcx0\n" <+> pprint (VarCtx $ cleanValues C99 (varctx vcx0)))--}

  -- Split constraints into equivalence, inequality, and field acess.
  let (eqs, iqs, fds) = stage3 c''

  -- Move function constraints to last positions, so we get more instantiated types to match
  -- variadic functions.
  let moveFun c@((FunTy _ _) :=: _) acc = acc ++ [c]
      moveFun c@(_ :=: (FunTy _ _)) acc = acc ++ [c]
      moveFun c acc = c:acc
      eqs' = List.foldr moveFun [] eqs

  -- Run plain unification on equalities and apply the substitutions on the inequalities,
  -- allowing us to sort them in a const-aware manner later.
  s0 <- punifyList eqs'

  (s, ineqs) <- makeEquivFromSymIneq s0 iqs

  -- Identity inconsistent pointer conversions, where the only solution is to bind a type
  -- variable to void*.
  let
    iqs'' = sortBySubTy ineqs
    sortBySubTy iq = List.sortBy subPred iq
    subPred (_ :>: t) (_ :>: t') = compare (nameOf t) (nameOf t')
  vs <- detectVoidPtr iqs''

  -- We want a binding order such that it weakens type qualifiers when possible, but
  -- enforces it when necessary. Therefore, the sorting prior to unifying inequalities.
  let
    vs' = vs @@ s
    iqs_'' = apply vs' iqs''
    (iq1, iq2, iq3, iq4, iq5) = dsort iqs_''
    iqs''' = iq1 ++ iq2 ++ iq3
  s' <- dunifyList iqs'''
  let s'' = s' @@ vs'

  -- Assemble records.
  (tcx1, vcx1, s''') <- stage4 tcx0 vcx0 fds s''

  let cc' = apply s''' c'
  vcx1' <- untypeVariadics cc' s''' vcx1

  -- Translate structural representation to a nominative one.
  (tcx2, vcx2) <- stage5 tcx1 vcx1' s'''

  -- Unify against void* and scalar types.
  let vqs = apply s''' (iq4 ++ iq5)
  ss <- dunifyList vqs
  let
    ss' = ss @@ s'''
    tcx3 = TyCtx $ Map.map (\(t,b) -> (apply ss' t, b)) (tyctx tcx2)
    vcx3 = VarCtx $ Map.map (\varInfo -> apply ss' varInfo) (varctx vcx2)

  -- Orphanize type variables that remain.
  let  (tcx4, vcx4) = stage6 tcx3 vcx3

  -- Decay function pointers.
  (tcx5, vcx5) <- decay tcx4 vcx4

  let
    -- Remove builtins and standard library components.
    tcx_ = undefTys (tyctx tcx5)
             Map.\\ (builtinTypes cl)
             Map.\\ (if ml then (stdTypes cl) else Map.empty)
    vcx_ = undefVars (varctx vcx5)
             Map.\\ (builtinValues cl)
             Map.\\ (if ml then (stdValues cl) else Map.empty)

    -- Remove anonymous types, their definitions are always in the program. Otherwise, we would
    -- have given them names.
    nonAnon _ ((RecTy _ n), b)
      | ensurePlain n == emptyName = False
      | otherwise = True
    nonAnon _ _ = True
    tcx_' = Map.filterWithKey nonAnon tcx_

  return (TyCtx tcx_', VarCtx vcx_)


instance Pretty ValSym where
  pprint =
   foo . bar
    where
   bar sym = (valty sym, static sym)
   foo (t, st) = pprint t <+> text (show st)
--}


-- | Populate the typing context, replacing "duplicate" types so that a single instance of
-- each one of them exists.
stage1 :: TyCtx -> Constraint -> SolverM (TyCtx, Constraint)
stage1 tctx (t :=: t') =
  return (tctx, (findCanonical tctx t) :=: (findCanonical tctx t'))
stage1 tctx (t :>: t') =
  return (tctx, (findCanonical tctx t) :>: (findCanonical tctx t'))
stage1 tctx (n :<-: t) =
  return (tctx, n :<-: (findCanonical tctx t))
stage1 tctx (Has n (Field n' t)) =
  return (tctx, Has n (Field n' (findCanonical tctx t)))
stage1 tctx (TypeDef t t') = createEquiv t t' tctx
stage1 tctx (c :&: c') = do
  (tcx1, c1) <- stage1 tctx c
  (tcx2, c1') <- stage1 tcx1 c'
  return (tcx2, c1 :&: c1')
stage1 tctx (Exists n c) = do
  v <- fresh
  stage1 tctx (apply (n +-> v) c)
stage1 tctx (Def n t c) = do
  (tcx, c') <- stage1 tctx c
  return (tcx, Def n (findCanonical tctx t) c')
stage1 tctx (Scope c) = do
  (tcx, c') <- stage1 tctx c
  return (tcx, Scope c')
stage1 tctx c@(ReadOnly _) = return (tctx, c)
stage1 tctx c@(Static _) = return (tctx, c)
stage1 tctx Truth = return (tctx, Truth)


-- | Find the canonical instance of a type.
findCanonical :: TyCtx -> Ty -> Ty
findCanonical _ t@(VarTy _) = t
findCanonical tctx t@(NamedTy n) = maybe t fst (Map.lookup n (tyctx tctx))
findCanonical tctx (FunTy t ts) = FunTy (findCanonical tctx t) (map (findCanonical tctx) ts)
findCanonical tctx (RecTy fs n) = RecTy (map (\f -> f{ty = findCanonical tctx (ty f)}) fs) n
findCanonical tctx (SumTy fs n) = SumTy (map (\f -> f{ty = findCanonical tctx (ty f)}) fs) n
findCanonical tctx (PtrTy t) = PtrTy (findCanonical tctx t)
findCanonical tctx (QualTy t q) = QualTy (findCanonical tctx t) q
findCanonical _ t@(AnyTy) = t
findCanonical _ t@(EnumTy _) = Data.BuiltIn.int -- We treat enumerations as plain integer.


-- | Create type equivalences.
createEquiv :: Ty -> Ty -> TyCtx -> SolverM (TyCtx, Constraint)
createEquiv (PtrTy t) (PtrTy t') tctx = do
  (tcx,c) <- createEquiv t t' tctx
  return (tcx, (t :=: t') :&: c)
createEquiv t@(PtrTy l) t'@(VarTy v) tctx = do
  v' <- fresh
  (tcx, c) <- createEquiv l v' tctx
  return (tcx, (t' :=: (PtrTy v')) :&: c)
createEquiv (QualTy t _) (QualTy t' _) tctx = do
  (tcx,c) <- createEquiv t t' tctx
  return (tcx, (t :=: t') :&: c)
createEquiv t@(QualTy l q) t'@(VarTy v) tctx = do
   v' <- fresh
   (tcx, c) <- createEquiv l v' tctx
   return (tcx, ((QualTy v' q) :=: t') :&: c)
createEquiv t@(EnumTy n) t'@(VarTy v) tctx = do
  let tctx' = TyCtx $ maybe (Map.insert n (EnumTy n, False) (tyctx tctx))
                            (const (tyctx tctx))
                            (Map.lookup n (tyctx tctx))
  return (tctx', Truth)
createEquiv t t'@(VarTy _) tcx = do
  let
    tctx' = TyCtx $ maybe (Map.insert (nameOf t) (flattenDecl tcx t t') (tyctx tcx))
                          (const (tyctx tcx))
                          (Map.lookup (nameOf t) (tyctx tcx))
  return (tctx' , Truth)
createEquiv t t' tcx = do -- Prefer a concrete type (when the definition only later appeared).
  let
    redefine _ = Map.insert (nameOf t) (t', True) (tyctx tcx)
    tctx' = TyCtx $ maybe (Map.insert (nameOf t) (flattenDecl tcx t t') (tyctx tcx))
                          (redefine)
                          (Map.lookup (nameOf t) (tyctx tcx))
  return (tctx' , Truth)


-- | Flatten a type declaration, discarding the definition part.
flattenDecl :: TyCtx -> Ty -> Ty -> (Ty, Bool)
  -- The declaration of a function is sufficient to make it "declared", even though if
  -- its definition might be missing. No matter what is its equivalent part.
flattenDecl _ (FunTy _ _) t = (t, True)
flattenDecl tctx _ t@(NamedTy n) = maybe (t, True) id (Map.lookup n (tyctx tctx))
flattenDecl _ _ t@(VarTy _) = (t, False)
flattenDecl tctx t' (QualTy t q) =
  (QualTy t'' q, b)
 where
  (t'', b) = flattenDecl tctx t' t
flattenDecl tctx t' (PtrTy t) =
  (PtrTy t'', b)
 where
  (t'', b) = flattenDecl tctx t' t
flattenDecl _ _ t@(FunTy _ _) = (t, False)
flattenDecl _ _ (RecTy _ n) = (NamedTy n, True)
flattenDecl _ _ (SumTy _ n) = (NamedTy n, True)
flattenDecl _ _ t = (t, False)


-- | Collect variable's type.
stage2 :: VarCtx -> Constraint -> SolverM (VarCtx, Constraint)
stage2 vtx (n :<-: t@(FunTy rt pts)) =
    case Map.lookup n (varctx vtx) of
        Just info ->
            case valty info of
                FunTy rt' pts' -> do
                    let pcs = zipWith (\t t' -> (t' :>: t)) pts pts'
                        pcs' = foldr (\c acc -> c :&: acc) Truth pcs
                        rtc = rt :>: rt'
                    return (vtx, pcs' :&: rtc)
                t' -> return (vtx, t' :=: t)
        Nothing -> do
            v <- fresh
            return ( VarCtx $ Map.insert n (ValSym v False False False) (varctx vtx)
                    , v :=: t )
stage2 vtx (n :<-: t) =
  case Map.lookup n (varctx vtx) of
    Just info -> return (vtx, valty info :=: t)
    Nothing -> do
      v <- fresh
      return (VarCtx $ Map.insert n (ValSym v False False False) (varctx vtx), v :=: t)
stage2 vtx (Def n t c) = do
  -- Functions might have their definition in the program but not their declaration. A function
  -- call prior to the definition will be inserted into the environment through an ascription
  -- constraint. Therefore, if when the definition is later on discovered, we need to preserve
  -- the symbol properties and "update" its type, possibly producing another constraint.
  let
    matchFunc t@(FunTy _ _) t' c = (t :=: t') :&: c
    matchFunc _ _ c = c
    replaceOrInsert t decl st  = VarCtx $ Map.insert n (ValSym t True decl st) (varctx vtx)
    (vtx', c') = case Map.lookup n (varctx vtx) of
             Just sym -> (replaceOrInsert t (readOnly sym) (static sym), matchFunc t (valty sym) c)
             Nothing -> (replaceOrInsert t False False, c)
  stage2 vtx' c'
stage2 vtx (Scope c) = do
  (vtx', c') <- stage2 vtx c
  let
    updateSym acc n ce = Map.adjust (\(ValSym t dc _ st) -> ValSym t dc ce st) n acc
    replaceOrNot n sym@(ValSym t d ce _) acc
      | not d = if Map.member n acc then updateSym acc n ce else Map.insert n sym acc
      | otherwise = acc
    vtx'' = VarCtx $ Map.foldrWithKey replaceOrNot (varctx vtx) (varctx vtx')
  return (vtx'', c')
stage2 vtx (c :&: c') = do
  (vtx1, c1) <- stage2 vtx c
  (vtx2, c2) <- stage2 vtx1 c'
  let
    -- Pick the symbol in which attributes are "enforced" by what's available in the program.
    choose (ValSym t@(QualTy _ _) dc ce st) (ValSym _ dc' ce' st') =
      ValSym t (dc && dc') (ce || ce') (st || st')
    choose (ValSym _ dc ce st) (ValSym t@(QualTy _ _) dc' ce' st') =
      ValSym t (dc && dc') (ce || ce') (st || st')
    -- A function can be defined, but not declared. We wanna pick the type of the one defined,
    -- available in the program. So we don't risk a mismatching signature due to modulo-
    -- conversion inferred type.
    choose (ValSym t dc ce st) (ValSym t' dc' ce' st')
      | dc = ValSym t (dc && dc') (ce || ce') (st || st')
      | otherwise = ValSym t' (dc && dc') (ce || ce') (st || st')
  return ( VarCtx $ Map.unionWith choose (varctx vtx1) (varctx vtx2)
           , c1 :&: c2 )
stage2 vtx c@(Has _ _) = return (vtx, c)
stage2 vtx c@(_ :=: _) = return (vtx, c)
stage2 vtx c@(_ :>: _) = return (vtx, c)
stage2 vtx (ReadOnly n) =
  case Map.lookup n (varctx vtx) of
    Nothing -> error "constexpr can only be applied on known values"
    Just info -> return
      (VarCtx $ Map.insert n
                (ValSym (QualTy (valty info) Const) (declared info) True (static info))
                (varctx vtx), Truth)
stage2 vtx c@(Static n) =
  case Map.lookup n (varctx vtx) of
    Nothing -> error "static can only be applied on known values"
    Just info -> return (VarCtx $ Map.insert n
                                  (ValSym (valty info) (declared info) (readOnly info) True)
                                  (varctx vtx), Truth)
stage2 vtx Truth = return (vtx, Truth)


-- | Split constraints into equivalences, inequalities, and field access.
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


-- | Convert symmetric inequalities such as alphaX :>: alphaY and alphaY > alphaX into an
--   equivalence alphaX = alphaY, reapplying substitutions in the end.
makeEquivFromSymIneq :: Subst -> [Constraint] -> SolverM (Subst, [Constraint])
makeEquivFromSymIneq s cstr = do
  let
    ineqs = apply s cstr
    lookupTable = List.foldr (\(t :>: t') acc -> Set.insert (t' :>: t) acc) Set.empty ineqs
    ineqs' = List.filter (\x -> if Set.member x lookupTable then False else True) ineqs
    equivs = ineqs List.\\ ineqs'
    equivs' = List.foldr (\(t :>: t') acc -> (t :=: t'):acc) [] equivs

  s' <-punifyList equivs'
  let ineqs'' = apply s ineqs'
      s'' = s' @@ s
  return (s'', ineqs'')


-- | Detect and instantiate void*.
detectVoidPtr :: [Constraint] -> SolverM Subst
detectVoidPtr [] = return nullSubst
detectVoidPtr [(_ :>: _)] = return nullSubst
detectVoidPtr ((t1 :>: t1'):c@(t2 :>: t2'):xs) = do
  let
    check t tn@(VarTy n) t' tn'@(VarTy n')
      | t /= tn
        && t' /= tn'
        && n == n'
        && (dropTopQual Const t) /= (dropTopQual Const t') = return (n +-> (PtrTy void))
      | t /= tn
        && t' /= tn'
        && n == n'
        && (dropTopQual Volatile t) /= (dropTopQual Volatile t') = return (n +-> (PtrTy void))
      | otherwise = return nullSubst
    check _ _ _ _  = return nullSubst
  s <- check t1 t1' t2 t2'
  s' <- detectVoidPtr ((apply s c):(apply s xs))
  return (s' @@ s)


-- | Sort constraint relations as according to our modeled subtyping relation.
dsort :: [Constraint] -> ([Constraint], [Constraint], [Constraint], [Constraint], [Constraint])
dsort [] = ([], [], [], [], [])
dsort (x:xs) =
  (eq1 ++ eq1', eq2 ++ eq2', eq3 ++ eq3', eq4 ++ eq4', eq5 ++ eq5')
 where
  (eq1, eq2, eq3, eq4, eq5) = dsort' x
  (eq1', eq2', eq3', eq4', eq5') = dsort xs

dsort' :: Constraint -> ([Constraint], [Constraint], [Constraint], [Constraint], [Constraint])
dsort' c@(_ :>: (PtrTy (QualTy t _)))
  | hasVarDep t = ([], [], [c], [], [])
  | t == Data.BuiltIn.void = ([], [], [], [c], [])
  | otherwise = ([c], [], [], [], [])
dsort' c@(_ :>: (PtrTy t))
  | t == Data.BuiltIn.void = ([], [], [], [c], [])
  | otherwise =  ([], [], [c], [], [])
dsort' c@((PtrTy t) :>: _)
  | hasVarDep t = ([], [], [c], [], [])
  | t == Data.BuiltIn.void = ([], [], [], [c], [])
  | t == Data.BuiltIn.scalar_t__ = ([], [], [], [], [c])
  | otherwise = case t of { QualTy _ _ -> ([], [], [c], [], []); _ -> ([], [c], [], [], []) }
dsort' c@(t :>: t')
  | t' == Data.BuiltIn.scalar_t__ = ([], [], [], [], [c])
  | otherwise = ([], [], [c], [], [])


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

    -- Fields are always assembled into a struct, which is the default behavior associated
    -- with field access constraints. It's impossible to tell whether such fields would
    -- have originated from a union, unless the actuall declaration of the type is inside
    -- the program or the variable has been declared through an elaborated type specifier
    -- using the the union keyword. Nevertheless, at this stage we're concerned about the
    -- structural representation of composite types. Later, we handle the cases in which
    -- such a composite type has actually been declared as a union.
    s' = Subst $ Map.mapWithKey (\v fs -> RecTy fs v) fieldMap''
    s'' = s' @@ s2

    -- Apply substitutions.
    tcx_ = Map.map (\(t,b) -> (apply s'' t, b)) (tyctx tcx)
    vcx_ = Map.map (\varInfo -> apply s'' varInfo) (varctx vcx)

  return (TyCtx tcx_, VarCtx vcx_, s'')


untypeVariadics :: Constraint -> Subst -> VarCtx -> SolverM (VarCtx)
untypeVariadics (n :<-: (FunTy _ pv)) s vcx =
  return vcx'
 where
  vcx' = case Map.lookup n (varctx vcx) of
    Nothing -> vcx -- When the name is typed as a function pointer.
    Just (ValSym (FunTy r pt) d ce st) ->
      let
        params (t1:xs1) (t2:xs2)
          | hasVarDep t2 = t1:(params xs1 xs2)
          | hasVarDep t1 = t2:(params xs1 xs2)
          | convertible (dropQual t1) t2 = t1:(params xs1 xs2)
          | convertible t1 (dropQual t2) = t2:(params xs1 xs2)
          | convertible (dropQual t1) (dropQual t2) = t1:(params xs1 xs2)
          | otherwise = [AnyTy]
        params (t:_) [] = [AnyTy]
        params [] (t:_) = [AnyTy]
        params [] [] = []
        f = ValSym (FunTy r (params pt pv)) d ce st
      in
       if d then vcx else (VarCtx $ Map.insert n f (varctx vcx))
    Just _ -> vcx

untypeVariadics (c1 :&: c2) s vcx = do
  vcx1 <- untypeVariadics c1 s vcx
  vcx2 <- untypeVariadics c2 s vcx1
  return vcx2
untypeVariadics (Def _ _ c) s vcx = untypeVariadics c s vcx
untypeVariadics (Scope c) s vcx = untypeVariadics c s vcx
untypeVariadics c s vcx = return vcx



-- | Bring structucally represented typing information into C world, by matching types with
-- their declared names, eventually creating fake names for the undeclared types.
stage5 :: TyCtx -> VarCtx -> Subst -> SolverM (TyCtx, VarCtx)
stage5 tcx vcx s = do
  let
    -- We filter types declared through elaborated specifiers and work on them independently,
    -- since it's illegal to typedef a type with an elaborated name. In addition, they are kept
    -- in an index so we are able to identify their definitions so they can be matched.
    tcxFlt_ = Map.filterWithKey (\k _ -> (not . isElab) k) (tyctx tcx)
    elabIdx = Map.foldrWithKey (\k (t, _) acc ->
      if isElab k then acc %% ((dropTopQualAny t) %-> k) else acc) nullIdx (tyctx tcx)
    tcxFlt_' = Map.map (\(t, b) -> (compact elabIdx t , b)) tcxFlt_
    vcxFlt_ = Map.map (\(ValSym t b ro st) -> ValSym (compact elabIdx t) b ro st) (varctx vcx)

    tcxElab_ = (tyctx tcx) Map.\\ tcxFlt_'
    tcxElab_' = Map.filter (not . snd) tcxElab_
    tcxElab_'' = Map.mapWithKey (\k (t, b) -> (unalpha2 k t, b)) tcxElab_'
    tcxElab_''' = Map.mapWithKey (\k (t, b) -> (keepElab k t, b)) tcxElab_''

    -- Collect composite types so we can compact them. Througout this process we also
    -- look into the substitutions because nested structs will only appear there.
    nonElabCompo = Map.foldr (\(t, _) acc -> acc ++ collect t) [] tcxFlt_'
    nonElabCompo' = Map.foldr (\(ValSym t _ _ _) acc -> acc ++ collect t) nonElabCompo vcxFlt_
    allCompo = Map.foldr (\(t, _) acc -> (collect t) ++ acc) nonElabCompo' tcxElab_'
    exclude = Set.fromList allCompo

    -- We only check against structs because fields are always assembled as such. Cases
    -- in which the composite type is actually union only happen if such elaborated type
    -- specifier is present in the program. Then, those types are filtered out above.
    consider [] cs h = (cs, h)
    consider (t@(RecTy _ n):tx) cs h
      | Set.member t h = consider tx cs h
      | isVar n = ([t] ++ rest, h'')
      | otherwise = consider tx cs h
     where
      h' = Set.insert t h
      (rest, h'') = consider tx cs h'
    consider _ _ _  = error "only struct types are collected"
    (nonElabCompo'', _) = Map.foldr
      (\t (xs, h) -> consider (collect t) xs h) (nonElabCompo', exclude) (subs s)

  -- Assign names and keep record of the type/name relation through an index. Those are
  -- inserted into the typing context as well.
  idx <- foldM (\acc t -> makeName acc t) nullIdx nonElabCompo''

  let
    tcx_ = Map.map (\(t, b) -> (compact idx t, b)) tcxFlt_'
    vcx_ = Map.map (\(ValSym t b ro st) -> ValSym (compact idx t) b ro st) vcxFlt_
    tcx_' = Map.foldrWithKey (\t n acc -> Map.insert n (t, False) acc) tcx_ (ty2n idx)

  -- "De-alphasize" top-level names so they match the ones we created.
  tcx_'' <- mapM (\c -> unalpha c) tcx_'

  let
    -- Create an index to be used for de-alphasizing fields from composite types.
    fullIdx = (ty2n idx) `Map.union` (ty2n elabIdx)
    gon2n (RecTy _ n) n' acc = Map.insert n n' acc
    gon2n (QualTy t _) n acc = gon2n t n acc
    gon2n (PtrTy t) n acc = gon2n t n acc
    gon2n _ _ acc = acc
    n2n = Map.foldrWithKey gon2n Map.empty fullIdx

    update t@(VarTy v) = maybe t (NamedTy) (Map.lookup v n2n)
    update t@(NamedTy _) = t
    update t@(EnumTy _) = t
    update (QualTy t q) = QualTy (update t) q
    update (PtrTy t) = PtrTy (update t)
    update (FunTy t tx) = FunTy (update t) (map (\t -> update t) tx)
    update (RecTy fs n) = RecTy (map (\(Field fn ft) -> Field fn (update ft)) fs) n
    tcx_''' = Map.map (\(t, b) -> (update t, b)) tcx_''
    tcxElab_'''' = Map.map (\(t, b) -> (update t, b)) tcxElab_'''

  return (TyCtx $ tcx_''' `Map.union` tcxElab_'''', VarCtx vcx_)


-- | Name a composite type by increasing IDs.
makeName :: TyIdx -> Ty -> SolverM TyIdx
makeName idx t@(RecTy _ _ ) = do
  n <- fakeName
  return $ idx %% (t %-> n)
makeName _ _ = error "cannot happen, only for composite"


-- | Enforce that a struct which is referenced by an elaborated name is accordingly named.
keepElab :: Name -> Ty -> Ty
keepElab k t@(RecTy fs n)
  | isVar n && isElabRec k = RecTy fs k
  | otherwise = t
keepElab _ t = t


-- | Orphanize unresolved type variables.
stage6 :: TyCtx -> VarCtx -> (TyCtx, VarCtx)
stage6 tcx vcx =
  let
    pick k t@(VarTy _) acc
      | isElab k = acc %% (t %-> k)
      | otherwise = acc
    pick k (QualTy t _) acc = pick k t acc
    pick k _ acc = acc
    elabOrph = Map.foldrWithKey (\k (t, _) acc -> pick k t acc) nullIdx (tyctx tcx)

    dummy n
      | isElabRec n = RecTy [Field (Name "dummy") Data.BuiltIn.int] n
      | isElabUnion n = SumTy [Field (Name "dummy") Data.BuiltIn.int] n
      | otherwise = EnumTy n
    go k t =
      case Map.lookup t (ty2n elabOrph) of
        Just k' -> if k == k' then dummy k else orphanize elabOrph t
        Nothing -> orphanize elabOrph t
    tcx_ = Map.mapWithKey (\k (t, b) -> (go k t, b)) (tyctx tcx)
    vcx_ = Map.map (\(ValSym t b ro st) -> ValSym (orphanize elabOrph t) b ro st) (varctx vcx)
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
    --liftIO (print $ pprint (t :>: t') <+> text "|" <+> pprint s)
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
  eqlist (h@(Has rt t@(Field n ty)) : h'@(Has rt' t'@(Field n' ty')) : fs)
    | (nameOf rt) == (nameOf rt') && n == n' = (((typeFrom t) :=: (typeFrom t')) : eqlist (h':fs))
    | otherwise = (eqlist (h':fs))
