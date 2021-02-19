-- Copyright (c) 2018 Leandro T. C. Melo (ltcmelo@gmail.com)
-- License: GPLv3
-- This implementation focus readability and formalism.

{-# LANGUAGE NamedFieldPuns #-}

import Control.Monad
import Control.Monad.Except
import Control.Monad.State
import Data.List
import Data.Map (Map)
import Data.Set (Set)
import qualified Data.List as List
import qualified Data.Map as Map
import qualified Data.Set as Set
import Debug.Trace
import System.Environment
import System.Exit
import System.IO
import Text.ParserCombinators.Parsec
import Text.ParserCombinators.Parsec.Expr
import Text.ParserCombinators.Parsec.Language
import qualified Text.ParserCombinators.Parsec.Token as Token
import qualified Text.PrettyPrint.HughesPJ as PP


----------------
-- The driver --
----------------

main :: IO ()
main = do
  putStrLn "compile muC program"
  args <- getArgs
  case args of
    [file] -> do
      src <- readFile file
      src' <- compile src
      putStrLn $ "\n\n" ++ src'
      let (name, _) = break (== '.') file
      writeFile ("new_" ++ name ++ ".c") src'
    _ -> error "invalid argument"

compile :: String -> IO (String)
compile src =
  case parseSource src  of
    Left err -> return err
    Right p -> do
      debug "AST" (show (fmt 0 p))

      let m = buildLattice p (M $ Map.empty)
      debug "lattice of shapes" (show $ ppM m)

      k <- generateConstraints p m
      debug "K" (show $ ppK k)

      let
        phi_i = Map.empty
        psi_i = Map.empty
        theta_i = (Map.fromList
                    [ (hat IntTy, IntTy),
                      (hat DoubleTy, DoubleTy),
                      (hat VoidTy, VoidTy) ])
        cfg = Config phi_i psi_i theta_i k [] [] [] []

      cfg'@(Config { phi, psi , theta }) <- solveConstraints k cfg
      debug "final config" (showConfig cfg')

      let ok = satisfyK (phi, psi, theta) k
      debug "semantics" (if ok then "OK" else error "does NOT hold\n")

      let ts = verifyTyping cfg' Map.empty p
      debug "typing" ("OK " ++ show ts)

      let preamble = rewriteInC (theta Map.\\ theta_i)
          src' = preamble ++ src
      return src'


---------------------------
-- The definition of muC --
---------------------------

data Stamp = Stamp Int deriving (Eq, Ord, Show)

newtype Ident = Ident { _x :: String } deriving (Eq, Ord, Show)

data Type = IntTy
          | DoubleTy
          | VoidTy
          | PtrTy Type
          | ConstTy Type
          | ArrowTy Type [Type]
          | RecTy [Decl] Ident
          | NamedTy Ident
          | TyVar Stamp
          deriving (Eq, Ord, Show)

data BinOptr = Add
             | Divide
             | Multiply
             | And
             | Or
             | Assign
             deriving (Eq, Ord, Show)

data Lit = IntLit Int
         | DoubleLit Double
         deriving (Eq, Ord, Show)

data Expr = NumLit Lit
          | Var Ident
          | FldAcc Expr Ident
          | Deref Expr
          | AddrOf Expr
          | BinExpr BinOptr Expr Expr
          deriving (Eq, Ord, Show)

data Stmt = ExprStmt Expr
          | DeclStmt Decl
          | RetStmt Expr
          deriving (Eq, Ord, Show)

data Decl = Decl { _ft :: Type, _fx :: Ident } deriving (Eq, Ord, Show)

data FunDef = FunDef Type Ident [Decl] [Stmt] deriving (Eq, Ord, Show)

data TypeDef = TypeDef Type Type deriving (Eq, Ord, Show)

data Prog = Prog [TypeDef] [FunDef] deriving (Eq, Ord, Show)


----------------------------
-- The constraints syntax --
----------------------------

data K = T
       | B
       | K :&: K
       | Exists [Type] K
       | Def Ident Type K
       | Fun Ident Type K
       | TypeOf Ident Type
       | Syn Type Type
       | Has Type Decl
       | Type :=: Type
       | Type :<=: Type
       deriving (Eq, Ord, Show)


-------------------
-- Substitutions --
-------------------

data Subst = Stamp :-> Type
           | Trivial
           deriving (Eq, Ord, Show)

class Substitutable a where
  -- | Apply a single substitution.
  apply :: Subst -> a -> a

  -- | Apply multiple substitutions at once.
  applyMany :: [Subst] -> a -> a
  applyMany sl a = foldr (\s acc -> apply s acc) a sl

  -- | Obtain the free type variables.
  ftv :: a -> [Stamp]

instance Substitutable a => Substitutable [a] where
  apply s = map (apply s)
  ftv = foldr (union . ftv) []

instance Substitutable Type where
  apply Trivial t = t
  apply s t@(IntTy) = t
  apply s t@(DoubleTy) = t
  apply s t@(VoidTy) = t
  apply s (PtrTy t) = PtrTy (apply s t)
  apply s (ConstTy t) = ConstTy (apply s t)
  apply s (ArrowTy rt pt) = ArrowTy (apply s rt) (apply s pt)
  apply s (RecTy fs x) = RecTy (apply s fs) x
  apply s t@(NamedTy _) = t
  apply (st :-> t) t'@(TyVar st') = if st == st' then t else t'
  ftv IntTy = []
  ftv DoubleTy = []
  ftv VoidTy = []
  ftv (PtrTy t) = ftv t
  ftv (ConstTy t) = ftv t
  ftv (ArrowTy rt pt) = ftv rt `union` ftv pt
  ftv (RecTy fs _) = ftv fs
  ftv (NamedTy _) = []
  ftv (TyVar st) = [st]

instance Substitutable K where
  apply Trivial k = k
  apply _ T = T
  apply _ B = B
  apply s (k1 :&: k2) = (apply s k1) :&: (apply s k2)
  apply s (Exists t k) = Exists (apply s t) (apply s k)
  apply s (Def x t@(TyVar _) k) = Def x (apply s t) (apply s k)
  apply s (Fun f t@(ArrowTy rt pl) k) = Fun f (apply s t) (apply s k)
  apply s (TypeOf x t) = TypeOf x (apply s t)
  apply s (Syn t1 t2)  = Syn (apply s t1) (apply s t2)
  apply s (Has t fld) = Has (apply s t) (apply s fld)
  apply s (t1 :=: t2) = (apply s t1) :=: (apply s t2)
  apply s (t1 :<=: t2) = (apply s t1) :<=: (apply s t2)
  ftv _ = []

instance Substitutable Decl where
  apply s (Decl t x) = Decl (apply s t) x
  ftv (Decl t _) = ftv t

instance Substitutable v => Substitutable (Map k v) where
  apply s = Map.map (apply s)
  ftv = Map.foldr (union . ftv) []

-- This function exists for presentation purposes.
foreachValue :: Substitutable a => [Subst] -> Map k a -> Map k a
foreachValue s = Map.map (applyMany s)


-------------------------
-- Type identification --
-------------------------

newtype TypeId = TypeId { _id :: String } deriving (Eq, Ord, Show)

-- | Compute the typeid of a type.
hat :: Type -> TypeId
hat IntTy = TypeId "int"
hat DoubleTy = TypeId "double"
hat VoidTy = TypeId "void"
hat (PtrTy t) = TypeId $ (_id (hat t) ++ "*")
hat (ConstTy t) = TypeId $ "const[" ++ (_id (hat t)) ++ "]"
hat (ArrowTy rt pt) = TypeId $
  "[" ++ (_id (hat rt))  ++ "(*)(" ++
  (foldr (\t acc -> (_id (hat t)) ++ acc) ")" pt) ++ "]"
hat (RecTy _ x) = TypeId (_x x)
hat (NamedTy x) = TypeId (_x x)
hat (TyVar (Stamp n)) = TypeId $ "α" ++ (show n)


------------------------------
-- The mappings ψ, Φ, and θ --
------------------------------

type Phi = Map Stamp Type
type Psi = Map Ident Type
type Theta = Map TypeId Type

-- | Find, in Psi, the type mapped to an identifier.
findInPsi :: Ident -> Psi -> Type
findInPsi x psi =
  case Map.lookup x psi of
    Just t -> t
    _ -> error $ "no τ for identifier " ++ show (ppK x) ++ " in ψ\n"

-- | Find, in Phi, the type mapped to a stamp.
findInPhi :: Stamp -> Phi -> Type
findInPhi st phi =
  case Map.lookup st phi of
    Just t -> t
    _ -> error $
         "no τ for stamp " ++ show (ppK st) ++ " in Φ\n"

-- | Find, in Theta, the type mapped to a typeid.
findInTheta :: TypeId -> Theta -> Type
findInTheta hat theta =
  case Map.lookup hat theta of
    Just t -> t
    _ -> error $
         "no τ for typeid " ++ show (ppK hat) ++ " in θ\n"

-- | Add, to Psi, an identifier to type mapping.
addToPsi :: Ident -> Type -> Psi -> Psi
addToPsi x t psi =
  case Map.lookup x psi of
    Just t -> error $
      show (ppK t) ++ ", indexed by " ++
      show (ppK x) ++ ", is already in ψ\n"
    _ -> Map.insert x t psi

-- | Add, to Phi, a stamp to type mapping.
addToPhi :: Stamp -> Type -> Phi -> Phi
addToPhi st t phi =
  case Map.lookup st phi of
    Just t -> error $
      show (ppK t) ++ ", indexed by " ++
      show (ppK st) ++ ", is already in Φ\n"
    _ -> Map.insert st t phi

-- | Add, to Theta, a type id to type mapping.
addToTheta :: TypeId -> Type -> Theta -> Theta
addToTheta tid t theta =
  case Map.lookup tid theta of
    Just t -> error $
      show (ppK t) ++ ", indexed by " ++
      show (ppK tid) ++ ", is already in θ\n"
    _ -> Map.insert tid t theta


----------------------------------
-- The semantics of constraints --
----------------------------------

trace_Sema = False

satisfyK :: (Phi, Psi, Theta) -> K -> Bool

-- | KT
satisfyK (_, _, _) T = True

-- | KAnd
satisfyK d@(phi, psi, theta) k@(k1 :&: k2) =
  let check1 = satisfyK (phi, psi, theta) k1
      check2 = satisfyK (phi, psi, theta) k2
  in if trace_Sema
     then trace ("\nsatisfy " ++ show (ppK k) ++ "\n" ++ show (formatPhiPsiTheta d) ++
                 "k1 " ++ show (ppK k1) ++ "\nk2 " ++ show (ppK k2)) (check1 && check2)
     else check1 && check2

-- | KEx
satisfyK (phi, psi, theta) kk@(Exists tl k) =
  let  go st t = isSubTy phi (findInPhi st phi) t
  in foldr (\(TyVar st) acc -> (Map.foldr (\t acc_ -> go st t || acc_) False phi) && acc) True tl
     && satisfyK (phi, psi, theta) k

-- | KDef
satisfyK (phi, psi, theta) (Def x (TyVar st) k) =
  findInPsi x psi == findInPhi st phi
  && satisfyK (phi, psi, theta) k

-- | KFun
satisfyK (phi, psi, theta) (Fun f (ArrowTy rt@(TyVar st) p) k) =
  findInPsi f psi == ArrowTy (findInPhi st phi) p
  && satisfyK (phi, psi, theta) k

-- | KInst
satisfyK (phi, psi, theta) k@(TypeOf x t@(TyVar _)) =
  satisfyK (phi, psi, theta) ((findInPsi x psi) :=: t)

-- | KSyn
satisfyK (phi, psi, theta) (Syn t a@(TyVar _)) =
  let t' = findInTheta (hat t) theta
  in satisfyK (phi, psi, theta) (t' :=: a)

-- | KHas
satisfyK (phi, psi, theta) (Has (TyVar st) (Decl t x)) =
  let t' = case findInPhi st phi of
             (TyVar _) -> t
             gt -> field x (findInTheta (hat gt) theta)
  in satisfyK (phi, psi, theta) (t' :=: t)

-- | KEq
satisfyK (phi, _, _) k@(t1 :=: t2) = isSubTy phi t1 t2 && isSubTy phi t2 t1

-- | KIq
satisfyK (phi, _, _) k@(t1 :<=: t2) = isSubTy phi t1 t2


satisfyC_ :: (Config, Config) -> K -> Config
satisfyC_ (cfg, cfg') k =
  if satisfyK ((phi cfg), (psi cfg), (theta cfg)) k
  then cfg'
  else error $ "entailment failed"

satisfyC :: Config -> Config
satisfyC cfg =
  if satisfyK ((phi cfg), (psi cfg), (theta cfg)) (groupK cfg)
  then cfg
  else error $ "entailment failed"

-- | Return the type of the field in a record.
field :: Ident -> Type -> Type
field x (RecTy ds _) =
  let ds' = filter (\(Decl _ x') -> x == x') ds
  in case length ds' of
    1 -> _ft (ds' !! 0)
    _ -> error $ "record has no field " ++ show (ppK x) ++ "\n"
field _ t = error $ "type " ++ show (ppK t) ++ " is not a record\n"

-- | Return whether the type is a ground type.
isGround :: Type -> Bool
isGround t =
  if ftv t == []
  then True
  else False


------------------------
-- The type predicate --
------------------------

trace_Pred = False

isSubTy :: Phi -> Type -> Type -> Bool
isSubTy phi t1@(TyVar st1) t2 =
  if (isIdentity phi t1)
  then True
  else (isGround (findInPhi st1 phi))
       && isSubTy phi (findInPhi st1 phi) t2
isSubTy phi t1 t2@(TyVar st2) =
  if (isIdentity phi t2)
  then True
  else (isGround (findInPhi st2 phi))
       && isSubTy phi t1 (findInPhi st2 phi)
isSubTy phi (ConstTy t1) (ConstTy t2) =
  isSubTy phi t1 t2
isSubTy phi (ConstTy t1) t2 =
  isSubTy phi t1 t2
isSubTy phi t@(PtrTy t1) t'@(PtrTy t2) =
  isSubTyPtr phi t1 t2
isSubTy _ IntTy IntTy = True
isSubTy _ IntTy DoubleTy = True
isSubTy _ DoubleTy DoubleTy = True
isSubTy _ VoidTy VoidTy = True
isSubTy _ (NamedTy x1) (NamedTy x2) = x1 == x2
isSubTy _ t1@(RecTy _ _) t2@(RecTy _ _) = t1 == t2
isSubTy phi t1 t2 =
  if trace_Pred
  then trace ("unknown (value) subtyping " ++ show (ppK t1) ++ "<:" ++ show (ppK t2)) False
  else False

isSubTyPtr :: Phi -> Type -> Type -> Bool
isSubTyPtr phi t1@(TyVar st1) t2 =
  if (isIdentity phi t1)
  then True
  else (isGround (findInPhi st1 phi))
       && isSubTyPtr phi (findInPhi st1 phi) t2
isSubTyPtr phi t1 t2@(TyVar st2) =
  if (isIdentity phi t2)
  then True
  else (isGround (findInPhi st2 phi))
       && isSubTyPtr phi t1 (findInPhi st2 phi)
isSubTyPtr phi (ConstTy t1) (ConstTy t2) =
  isSubTyPtr phi t1 t2
isSubTyPtr phi t1 (ConstTy t2) =
  isSubTyPtr phi t1 t2
isSubTyPtr _ _ VoidTy = True
isSubTyPtr _ IntTy IntTy = True
isSubTyPtr _ DoubleTy DoubleTy = True
isSubTyPtr _ (NamedTy x1) (NamedTy x2) = x1 == x2
isSubTyPtr _ t1@(RecTy _ _) t2@(RecTy _ _) = t1 == t2
isSubTyPtr phi t1 t2 =
  if trace_Pred
  then trace ("unknown (pointer) subtyping " ++ show (ppK t1) ++ "<:" ++ show (ppK t2)) False
  else False


-- | Subtyping predicate for ground types.

isSubTy' :: Type -> Type -> Bool
isSubTy' (TyVar _) _ = error $ "expected ground type "
isSubTy' _ (TyVar _) = error $ "expected ground type "
isSubTy' (ConstTy t1) (ConstTy t2) =
  isSubTy' t1 t2
isSubTy' (ConstTy t1) t2 =
  isSubTy' t1 t2
isSubTy' (PtrTy t1) (PtrTy t2) =
  isSubTyPtr' t1 t2
isSubTy' IntTy IntTy = True
isSubTy' DoubleTy DoubleTy = True
isSubTy' VoidTy VoidTy = True
isSubTy' IntTy DoubleTy = True
isSubTy' (NamedTy x1) (NamedTy x2) = x1 == x2
isSubTy' t1@(RecTy _ _) t2@(RecTy _ _) = t1 == t2
isSubTy' t1 t2 =
  if trace_Pred
  then trace ("unknown (value/ground) subtyping " ++ show (ppK t1) ++ "<:" ++ show (ppK t2)) False
  else False

isSubTyPtr' :: Type -> Type -> Bool
isSubTyPtr' (TyVar _) _ = error $ "expected ground type "
isSubTyPtr' _ (TyVar _) = error $ "expected ground type "
isSubTyPtr' (ConstTy t1) (ConstTy t2) =
  isSubTyPtr' t1 t2
isSubTyPtr' t1 (ConstTy t2) =
  isSubTyPtr' t1 t2
isSubTyPtr' (PtrTy t1) (PtrTy t2) =
  isSubTyPtr' t2 t2
isSubTyPtr' _ VoidTy = True
isSubTyPtr' IntTy IntTy = True
isSubTyPtr' DoubleTy DoubleTy = True
isSubTyPtr' (NamedTy x1) (NamedTy x2) = x1 == x2
isSubTyPtr' t1@(RecTy _ _) t2@(RecTy _ _) = t1 == t2
isSubTyPtr' t1 t2 =
  if trace_Pred
  then trace ("unknown (pointer/ground) subtyping " ++ show (ppK t1) ++ "<:" ++ show (ppK t2)) False
  else False

-- | Whether we have an identity relation.
isIdentity phi t@(TyVar st) = t == findInPhi st phi


---------------------------
-- Constraint generation --
---------------------------

generateConstraints :: Prog -> M -> IO K
generateConstraints p m = do
  (c, _) <- runStateT (genProg p m) 0
  return c

-- | The generator is typed as a monad to allow isolation of
-- the fresh variable supply.
type GenMonad a = StateT Int IO a
fresh :: GenMonad Type
fresh = do
  n <- get
  put (n + 1)
  return $ TyVar (Stamp n)

-- | Constraint generation for a program.
genProg :: Prog -> M -> GenMonad K
genProg (Prog _ []) _ = return T
genProg (Prog [] ((FunDef rt f d s):fs)) m = do
  a <- fresh
  syn <- buildSyn rt a
  let pt = foldl (\acc (Decl t _) -> t:acc) [] d
  k <- genFun d s a m
  k' <- genProg (Prog [] fs) m
  return $
    Exists [a] $
    syn :&:
    Fun f (ArrowTy a pt) k :&:
    k'
genProg (Prog ((TypeDef t1 t2):tds) fs) m = do
  a <- fresh
  k <- genProg (Prog tds fs) m
  return $
    Exists [a] $
    (Syn t2 a) :&:
    (a :=: t1) :&:
    k

-- | Constraint generation for functions.
genFun :: [Decl] -> [Stmt] -> Type -> M -> GenMonad K
genFun [] s rt m = genStmt s rt m
genFun ((Decl t x):dx) s rt m = do
  a <- fresh
  syn <- buildSyn t a
  k <- genFun dx s rt m
  return $
    Exists [a] $
    syn :&:
    Def x a k

-- | Constraint generation for statements.
genStmt :: [Stmt] -> Type -> M -> GenMonad K
genStmt ((DeclStmt (Decl t x)):sl) rt m = do
  a <- fresh
  syn <- buildSyn t a
  k <- genStmt sl rt m
  return $
    Exists [a] $
    syn :&:
    Def x a k
genStmt ((ExprStmt e):sl) rt m = do
  a <-  fresh
  k1 <- genExpr e a m
  k2 <- genStmt sl rt m
  return $
    Exists [a] k1 :&:
    k2
genStmt ((RetStmt e):[]) rt m = do
  a <- fresh
  k <- genExpr e a m
  return $
    Exists [a] $
    keepOrDrop (shapeOf FunRole m) rt (shapeOf (ValRole e) m) a Assign :&:
    k

-- | Constraint generation for expressions.
genExpr :: Expr -> Type -> M -> GenMonad K
genExpr (NumLit l) t _ = return (rho l :=: t)
genExpr (Var x) t _ = return (TypeOf x t)
genExpr (FldAcc e x) t m = do
  a1 <- fresh
  a2 <- fresh
  a3 <- fresh
  k <- genExpr e a1 m
  return $
    Exists [a1, a2, a3] $
    (Has a2 (Decl a3 x)) :&:
    (a1 :=: (PtrTy a2)) :&:
    (a3 :=: t) :&:
    k
genExpr (Deref e) t m = do
  a <- fresh
  k <- genExpr e a m
  return $
    Exists [a] $
    (a :=: PtrTy t) :&:
    k
genExpr (AddrOf e) t m = do
  a1 <- fresh
  a2 <- fresh
  k <- genExpr e a2 m
  return $
    Exists [a1, a2] $
    (a1 :=: PtrTy a2) :&:
    (a1 :=: t) :&:
    k
genExpr e@(BinExpr op e1 e2) t m = do
  a1 <- fresh
  a2 <- fresh
  k1 <- genExpr e1 a1 m
  k2 <- genExpr e2 a2 m
  return $
    Exists [a1, a2] $
    k1 :&:
    k2 :&:
    keepOrDrop (shapeOf (ValRole e1) m) a1 (shapeOf (ValRole e2) m) a2 op :&:
    select (shapeOf (ValRole e1) m) a1 (shapeOf (ValRole e2) m) a2 t op

-- | The type of a literal.
rho :: Lit -> Type
rho (IntLit _) = IntTy
rho (DoubleLit _) = DoubleTy


---------------------
-- Synonym builder --
---------------------

-- | Recursively build type synonyms.
buildSyn :: Type -> Type -> GenMonad K
buildSyn t@(PtrTy tt) a = do
  b <- fresh
  syn <- buildSyn tt b
  return $
    Exists [b] $
    Syn t a :&:
    Syn tt b :&:
    ((PtrTy b) :=: a) :&:
    syn
buildSyn t@(ConstTy tt) a = do
  b <- fresh
  syn <- buildSyn tt b
  return $
    Exists [b] $
    Syn t a :&:
    Syn tt b :&:
    ((ConstTy b) :=: a) :&:
    syn
buildSyn t a =
  return $ Syn t a


--------------------------
-- Auxiliary generators --
--------------------------

-- | Keep or drop a constraint.
keepOrDrop :: Shape -> Type -> Shape -> Type -> BinOptr -> K
keepOrDrop sp1 a1 sp2 a2 op =
  if (fst sp1 /= fst sp2
      && (fst sp1 == P || fst sp2 == P)
      && fst sp1 /= U
      && fst sp2 /= U)
  then T
  else if (op == Assign)
       then (a2 :<=: a1)
       else if (fst sp1 == I && fst sp2 == FP)
            then (a1 :<=: a2)
            else if (fst sp1 == FP && fst sp2 == I)
                 then (a2 :<=: a1)
                 else if (snd sp1 == snd sp2)
                      then (a1 :=: a2)
                      -- The shape carries an annotation, use it accordinly.
                      else if (snd sp1 == (ConstTy (snd sp2)))
                           then (a1 :<=: a2)
                           else (a2 :<=: a1)

-- | Select operands and result types.
select :: Shape -> Type -> Shape -> Type -> Type -> BinOptr -> K
select sp1 a1 sp2 a2 t op  =
  case op of
    Add -> select_Add sp1 a1 sp2 a2 t
    Assign -> (t :=: a1)
    And -> select_AndOr sp1 a1 sp2 a2 t
    Or -> select_AndOr sp1 a1 sp2 a2 t
    Divide -> select_DivideMultiply sp1 a1 sp2 a2 t
    Multiply -> select_DivideMultiply sp1 a1 sp2 a2 t

select_Add :: Shape -> Type -> Shape -> Type -> Type -> K
select_Add sp1 a1 sp2 a2 t =
  if (fst sp1 == P)
  then (a1 :=: t) :&: ((ConstTy IntTy) :<=: a2)
  else if (fst sp2 == P)
       then (a2 :=: t) :&: ((ConstTy IntTy) :<=: a1)
       else (t :<=: DoubleTy)

select_AndOr :: Shape -> Type -> Shape -> Type -> Type -> K
select_AndOr sp1 a1 sp2 a2 t = t :=: IntTy

select_DivideMultiply :: Shape -> Type -> Shape -> Type -> Type -> K
select_DivideMultiply sp1 a1 sp2 a2 t =
  if (fst sp1 == I && fst sp2 == I)
  then (t :=: IntTy)
       :&: ((ConstTy IntTy) :<=: a1)
       :&: ((ConstTy IntTy) :<=: a2)
  else if (fst sp1 == I)
       then (t :<=: DoubleTy)
            :&: ((ConstTy IntTy) :<=: a1)
            :&: (a2 :<=: DoubleTy)
       else if (fst sp2 == I)
            then (t :<=: DoubleTy)
                 :&: (a1 :<=: DoubleTy)
                 :&: ((ConstTy IntTy) :<=: a2)
            else (t :<=: DoubleTy)
                 :&: (a1 :<=: DoubleTy)
                 :&: (a2 :<=: DoubleTy)

---------------------------
-- The lattice of shapes --
---------------------------

data ShapeKey = U
              | S
              | P
              | N
              | I
              | FP
              deriving (Eq, Ord, Show)

type Shape = (ShapeKey, Type)


-- | Create a shapeFromUse based on use.
shapeFromUse :: ShapeKey -> Shape
shapeFromUse sk = (sk, NamedTy $ Ident "<empty type>")

-- | Create a shapeFromUse out of a type.
shapeFromTy :: Type -> Shape
shapeFromTy t@IntTy = (I, t)
shapeFromTy t@DoubleTy = (FP, t)
shapeFromTy t@(ConstTy t') = (fst (shapeFromTy t'), t)
shapeFromTy t@(PtrTy _) = (P, t)
shapeFromTy _ = shapeFromUse U


data SyntaxRole = ValRole Expr
                | FunRole
                deriving (Eq, Ord, Show)

-- | The table M.
newtype M = M { _shapes :: Map SyntaxRole Shape } deriving (Eq, Ord, Show)

insertOrUpdate :: SyntaxRole -> Shape -> M -> (Shape, M)
insertOrUpdate ro sp m =
  (sp', M $ Map.insert ro sp' (_shapes m))
  where
    sp' = case Map.lookup ro (_shapes m) of
      Just sp'' ->
        case fst sp'' of
          P  -> sp''
          I  -> sp''
          FP -> sp''
          N  -> if (fst sp == I || fst sp == FP)
                then sp
                else sp''
          S  -> if (fst sp == I
                    || fst sp == FP
                    || fst sp == P)
                then sp
                else sp''
          U  -> sp
      Nothing -> sp

shapeOf :: SyntaxRole -> M -> Shape
shapeOf ro m =
  case Map.lookup ro (_shapes m) of
    Just sp -> sp
    Nothing -> shapeFromUse U


classifyE :: Expr -> Shape -> M -> (Shape, M)
classifyE e@(NumLit v) _ m =
  insertOrUpdate (ValRole e) sp m
  where
    sp = case v of
      (IntLit 0) -> shapeFromUse S
      (IntLit _) -> shapeFromUse I
      _ -> shapeFromUse FP
classifyE e@(Var _) sp m =
  insertOrUpdate (ValRole e) sp m
classifyE e@(FldAcc e' x) sp m =
  insertOrUpdate (ValRole e) sp m'
  where
    (_, m') = classifyE e' (shapeFromUse P) m
classifyE e@(Deref e') sp m =
  insertOrUpdate (ValRole e) sp m'
  where
    (_, m') = classifyE e' (shapeFromUse P) m
classifyE e@(AddrOf e') sp m =
  insertOrUpdate (ValRole e) (shapeFromUse P) m'
  where
    (_, m') = classifyE e' sp m
classifyE e@(BinExpr Add e1 e2) sp m =
  insertOrUpdate (ValRole e) sp'''' m''
  where
    sp' = if (sp == shapeFromUse I
              || sp == shapeFromUse FP
              || sp == shapeFromUse N)
          then sp
          else shapeFromUse S
    (sp1, m') = classifyE e1 sp' m
    sp'' = if (fst sp1 == P)
           then shapeFromUse I
           else if (sp == shapeFromUse I
                   || sp == shapeFromUse FP
                   || sp == shapeFromUse N)
                then sp
                else shapeFromUse S
    (sp2, m'') = classifyE e2 sp'' m'
    sp''' = if (fst sp2 == P)
            then shapeFromUse I
            else sp''
    (sp3, m''') = classifyE e1 sp''' m''
    sp'''' = if (fst sp3 == P || fst sp2 == P)
             then shapeFromUse P
             else shapeFromUse N
classifyE e@(BinExpr Divide e1 e2) sp m =
  insertOrUpdate (ValRole e) (shapeFromUse N) m''
  where
    (_, m') = classifyE e1 (shapeFromUse N) m
    (_, m'') = classifyE e2 (shapeFromUse N) m'
classifyE e@(BinExpr Multiply e1 e2) sp m =
  insertOrUpdate (ValRole e) (shapeFromUse N) m''
  where
    (_, m') = classifyE e1 (shapeFromUse N) m
    (_, m'') = classifyE e2 (shapeFromUse N) m'
classifyE e@(BinExpr And e1 e2) sp m =
  insertOrUpdate (ValRole e) (shapeFromUse I) m''
  where
    (_, m') = classifyE e1 (shapeFromUse S) m
    (_, m'') = classifyE e2 (shapeFromUse S) m'
classifyE e@(BinExpr Or e1 e2) sp m =
  insertOrUpdate (ValRole e) (shapeFromUse I) m''
  where
    (_, m') = classifyE e1 (shapeFromUse S) m
    (_, m'') = classifyE e2 (shapeFromUse S) m'
classifyE e@(BinExpr Assign e1 e2) sp m =
  insertOrUpdate (ValRole e) sp1 m''
  where
    (sp2, m') = classifyE e2 sp m
    (sp1, m'') = classifyE e1 sp2 m'

classifyD :: Decl -> M -> (Shape, M)
classifyD (Decl { _ft = t, _fx = x }) m =
   insertOrUpdate (ValRole (Var x)) (shapeFromTy t) m


-- | Build lattice of shapes until stabilization.
buildLattice :: Prog -> M -> M
buildLattice p@(Prog _ fs) m =
  let
    go ((DeclStmt d):xs) acc =
      let (sp, m) = classifyD d acc
      in go xs m
    go ((ExprStmt e):xs) acc =
      let (sp, m) = classifyE e (shapeOf (ValRole e) acc) acc
      in go xs m
    go ((RetStmt e):[]) acc = snd $ classifyE e (shapeOf (ValRole e) acc) acc

    handleParam ds = map (\d -> DeclStmt d) ds
    handleRet rt m = M $ Map.insert FunRole (shapeFromTy rt) (_shapes m)

    m' = foldr (\(FunDef rt _ ds ss) acc -> go
                 ((handleParam ds) ++ ss) (handleRet rt acc)) m fs
  in if (m' == m)
     then m'
     else buildLattice p m'


-----------------
-- Unification --
-----------------

data StratMode = Relax
               | Enforce
               deriving (Eq, Ord, Show)

class Substitutable a => UnifiableC a where
  uC :: a -> a -> [Subst]
  uS :: a -> a -> StratMode -> [Subst]

instance UnifiableC Type where
  uC (TyVar st) t2 =
    let s = st :-> t2
    in if (trace_UC)
       then trace("uC " ++ show (ppK s)) [s]
       else [s]
  uC t1 t2@(TyVar _) = uC t2 t1
  uC IntTy IntTy = [Trivial]
  uC DoubleTy DoubleTy = [Trivial]
  uC VoidTy VoidTy = [Trivial]
  uC t1@(NamedTy x1) t2@(NamedTy x2)
    | x1 == x2 = [Trivial]
    | otherwise = error $ "can't (classic) unify named types " ++
                  (show $ ppK t1) ++ "::" ++ (show $ ppK t2)
  uC (ConstTy t1) (ConstTy t2) = uC t1 t2
  uC (PtrTy t1) (PtrTy t2) = uC t1 t2
  uC t1@(RecTy fs1 x1) t2@(RecTy fs2 x2) = undefined
  uC (ArrowTy rt1 [pt1]) (ArrowTy rt2 [pt2]) = undefined
  uC t1 t2 = error $ "unknown (classic) unification from " ++
             (show $ ppK t1) ++ " to " ++ (show $ ppK t2)


  uS t1@(PtrTy _) (TyVar st) _ = [st :-> t1]
  uS t1 t2@(TyVar st) sm
    | sm == Enforce = [st :-> t1]
    | otherwise = [st :-> (relax t1)]
  uS (TyVar st) t2 _ = [st :-> (relax t2)]
  uS IntTy IntTy _ = [Trivial]
  uS IntTy DoubleTy Relax = [Trivial]
  uS DoubleTy DoubleTy _ = [Trivial]
  uS VoidTy VoidTy _ = [Trivial]
  uS t1@(NamedTy x1) t2@(NamedTy x2) _
    | x1 == x2 = [Trivial]
    | otherwise = error $ "can't (directional) unify named types " ++
                  (show $ ppK t1) ++ "::" ++ (show $ ppK t2)
  uS (ConstTy t1) (ConstTy t2) sm = uS t1 t2 sm
  uS (ConstTy t1) t2 Relax = uS t1 t2 Relax
  uS t1 (ConstTy t2) Enforce = uS t1 t2 Enforce
  uS (PtrTy (TyVar st)) (PtrTy t2@VoidTy) _ = [st :-> t2]
  uS (PtrTy _) (PtrTy VoidTy) _ = [Trivial]
  uS (PtrTy t1) (PtrTy t2) _ = uS t1 t2 Enforce
  uS t1@(RecTy fs1 x1) t2@(RecTy fs2 x2) _ = undefined
  uS (ArrowTy rt1 [pt1]) (ArrowTy rt2 [pt2]) _ = undefined
  uS t1 t2 _ = error $ "unknown (directional) unification from " ++
               (show $ ppK t1) ++ " to " ++ (show $ ppK t2)

instance UnifiableC Decl where
  uC (Decl t1 x1) (Decl t2 x2)
    | x1 == x2 = uC t1 t2
    | otherwise = error $ "can't unify decl " ++
                  (show x1) ++ "::" ++ (show x2)
  uS (Decl t1 x1) (Decl t2 x2) m
    | x1 == x2 = uS t1 t2 m
    | otherwise = error $ "can't unify decl " ++
                  (show x1) ++ "::" ++ (show x2)

instance UnifiableC a => UnifiableC [a] where
  uC [] [] = [Trivial]
  uC _  [] = error "can't unify lists, different lengths"
  uC [] x  = uC x []
  uC (a1:as1) (a2:as2) =
    let s = uC a1 a2
        s' = uC (applyMany s as1) (applyMany s as2)
    in s ++ s'

  uS [] [] _ = [Trivial]
  uS _  [] _ = error "can't unify lists, different lengths"
  uS [] x m = uS x [] m
  uS (a1:as1) (a2:as2) m =
    let s = uS a1 a2 m
        s' = uS (applyMany s as1) (applyMany s as2) m
    in s ++ s'

trace_UC = False


----------------------------------
-- The constness-relax function --
----------------------------------

-- | Relax constness.
relax :: Type -> Type
relax (ConstTy t) = t
relax (PtrTy t) = PtrTy (relax t)
relax t = t


------------------------------
-- The solver configuration --
------------------------------

data Config = Config
  { phi :: Phi,
    psi :: Psi,
    theta :: Theta,
    k :: K,
    kE :: [K],
    kI :: [K],
    kW :: [K],
    kF :: [K]
  }
  deriving (Eq, Ord, Show)

groupK :: Config -> K
groupK cfg@(Config { k, kE, kI, kW, kF }) =
  let go el acc = acc :&: el
  in foldr go k (kE ++ kI ++ kW ++ kF)


---------------------------
-- Solver: preprocessing --
---------------------------

preprocess :: Config -> Config

-- | PP-and
preprocess cfg@(Config { k = k1 :&: k2 }) =
  let cfg' = preprocess (cfg { k = k1 })
      cfg'' = satisfyC cfg'
  in  preprocess $ (cfg'' { k = k2 })

-- | PP-ex
preprocess cfg@(Config { k = Exists ts k' }) =
  let self t@(TyVar st) acc = addToPhi st t acc
      phi' = foldr self (phi cfg) ts
  in preprocess $ cfg { phi = phi', k = k' }

-- | PP-syn
preprocess cfg@(Config { k = Syn t a, theta } ) =
  let theta' = if Map.member (hat t) theta
               then theta
               else addToTheta (hat t) a theta
      cfg' = cfg { theta = theta', k = ((findInTheta (hat t) theta') :=: a) }
  in preprocess $ cfg'

-- | PP-def
preprocess cfg@(Config { k = Def x (TyVar st) k' }) =
  let psi' = addToPsi x (findInPhi st (phi cfg)) (psi cfg)
  in preprocess $ cfg { psi = psi', k = k' }

-- | PP-fun
preprocess cfg@(Config { k = Fun f (ArrowTy (TyVar st) t) k' }) =
  let psi' = addToPsi f (ArrowTy (findInPhi st (phi cfg)) t) (psi cfg)
  in preprocess $ cfg { psi = psi', k = k' }

-- | PP-inst
preprocess cfg@(Config { k = TypeOf x t }) =
  let k' = findInPsi x (psi cfg) :=: t
  in preprocess $ cfg { k = k' }

-- | PP-eq
preprocess cfg@(Config { k = k'@(t1 :=: t2), kE }) =
  preprocess $ cfg { k = T, kE = k':kE }

-- | PP-has
preprocess cfg@(Config { k = k'@(Has _ _), kF }) =
  preprocess $ cfg { k = T, kF = k':kF }

-- | PP-iq
preprocess cfg@(Config { k = k'@(t1 :<=: t2), kI } ) =
  preprocess $ cfg { k = T, kI = k':kI }

-- | PP-end
preprocess cfg@(Config { k = T }) =
  if (not trace_PP)
  then cfg
  else trace (showConfig cfg ++ "\n") cfg

trace_PP = False


-----------------------------------
-- Solver: 1st unification round --
-----------------------------------

trace_U = False

unifyEq :: Config -> Config

-- | UE-base
unifyEq cfg@(Config { kE = k@(t1 :=: t2):kE_ }) =
  let s = uC t1 t2
      phi' = foreachValue s (phi cfg)
      psi' = foreachValue s (psi cfg)
      theta' = foreachValue s (theta cfg)
      kE' = applyMany s kE_
      kF' = applyMany s (kF cfg)
      kI' = applyMany s (kI cfg)
      cfg' = cfg { phi = phi',
                   psi = psi',
                   theta = theta',
                   kE = kE',
                   kF = kF',
                   kI = kI' }
      rw = unifyEq (satisfyC_ (cfg, cfg') k)
  in if (not trace_U)
     then rw
     else trace("uC: " ++ show (ppK s) ++
                "\n≡'  " ++ show (ppK kE') ++
                "\n≤'  " ++ show (ppK kI') ++ "\n") rw

-- | UE-end
unifyEq cfg@(Config { kE = [] }) = cfg


-----------------------------------
-- Solver: 2nd unification round --
-----------------------------------

splitOrderLift :: Config -> Config

-- | SOL
splitOrderLift cfg =
  let (kI', kW') = splitWob ((kI cfg) ++ [B]) []
      kI'' = orderSub (kI' ++ [B]) []
      kI''' = liftSub(kI'' ++ [B]) []
  in cfg { kI = kI''', kW = kW'}


unifyIq :: Config -> Config

-- | UI-base
unifyIq cfg@(Config {
                kI = k@(t1 :<=: t2):kI_ }) =
  let s = uS t1 t2 Relax
      phi' = foreachValue s (phi cfg)
      psi' = foreachValue s (psi cfg)
      theta' = foreachValue s (theta cfg)
      kI' = applyMany s kI_
      kF' = applyMany s (kF cfg)
      kW' = applyMany s (kW cfg)
      (kI'', kW'') = splitWob (kI' ++ kW' ++ [B]) []
      kI''' = orderSub (kI'' ++ [B]) []
      kI'''' = liftSub (kI''' ++ [B]) []
      cfg' = cfg { phi = phi',
                   psi = psi',
                   theta = theta',
                   kI = kI'''',
                   kF = kF',
                   kW = kW'' }
      rw = unifyIq (satisfyC_ (cfg, cfg') k)
  in if (not trace_U)
     then rw
     else trace("uS:   " ++ show (ppK s) ++
                "\n≤'    " ++ show (ppK kI') ++
                "\n≤''   " ++ show (ppK kI'') ++
                "\n≤'''  " ++ show (ppK kI''') ++
                "\n≤'''' " ++ show (ppK kI'''') ++ "\n") rw

-- | UI-end
unifyIq cfg = cfg


unifyWb ::  Config -> Config

-- | UW-base
unifyWb cfg@(Config { kI = k@(t1 :<=: t2):kI_ }) =
  let s = uS t1 t2 Relax
      phi' = foreachValue s (phi cfg)
      psi' = foreachValue s (psi cfg)
      theta' = foreachValue s (theta cfg)
      kI_' = applyMany s kI_
      kF' = applyMany s (kF cfg)
      cfg' = cfg { phi = phi',
                   psi = psi',
                   theta = theta',
                   kI = kI_',
                   kF = kF' }
      rw = unifyWb (satisfyC_ (cfg, cfg') k)
  in if (not trace_U)
     then rw
     else trace("uS (wobbly): " ++ show (ppK s) ++ "\n... " ++ show (ppK kI_') ++ "\n") rw

-- | UW-end
unifyWb cfg = cfg


----------------------------------
-- Splitting, ordering, lifting --
----------------------------------

-- | Split wobbly relations.
splitWob :: [K] -> [K] -> ([K], [K])
splitWob (w@((TyVar _) :<=: (TyVar _)):k) kW =
  splitWob k (w:kW)
splitWob (nw@(t1 :<=: t2):k) kW =
  splitWob (k ++ [nw]) kW
splitWob (B:k) kW =
  (k, kW)

-- | Order inequality constraints.
orderSub :: [K] -> [K] -> [K]
orderSub ((t1@(PtrTy (ConstTy _)) :<=: t2):kW) kS =
  orderSub kW ((t1 :<=: t2):kS)
orderSub ((t1 :<=: t2@(PtrTy _)):kW) kS =
  orderSub kW ((t1 :<=: t2):kS)
orderSub ((t1@DoubleTy :<=: t2):kW) kS =
  orderSub kW ((t1 :<=: t2):kS)
orderSub ((t1 :<=: t2@IntTy):kW) kS =
  orderSub kW ((t1 :<=: t2):kS)
orderSub ((t1 :<=: t2):kW) kS =
  orderSub (kW ++ [t1 :<=: t2]) kS
orderSub (B:kW) kS =
  kS ++ kW

-- | Detect presence of top type.
liftSub :: [K] -> [K] -> [K]
liftSub (k1@((PtrTy t1) :<=: t2@(PtrTy (TyVar (Stamp n)))):
         k2@((PtrTy t1') :<=: t2'@(PtrTy (TyVar (Stamp n')))):k) kn
  | n == n'
    && ((unqualTy t1) /= (unqualTy t1'))
    && (isGround t1)
    && (isGround t1') =
      -- Check t1 only, since `const' pointers (when existing) appear first.
      let t = case t1 of
             ConstTy _ -> (PtrTy (ConstTy VoidTy))
             _ ->  PtrTy VoidTy
      in liftSub k (kn ++ ((t :<=: t2):k1:[k2]))
  | otherwise = liftSub (k2:k) (kn ++ [k1])
liftSub (k1@( _ :<=: _):k) kn =
  liftSub k (kn ++ [k1])
liftSub (B:k) kn =
  kn ++ k

-- | Ensure unqualified type.
unqualTy :: Type -> Type
unqualTy (ConstTy t) = t
unqualTy t = t


------------------------------
-- Membership normalization --
------------------------------

-- | SH
sortHas :: Config -> Config
sortHas cfg =
  let criteria k1@(Has t1 (Decl _ x1)) k2@(Has t2 (Decl _ x2))
        | t1 == t2 = compare x1 x2
        | otherwise = compare t1 t2
  in cfg { kF = sortBy criteria (kF cfg) }

-- | MN-join
normFlds :: Config -> Config
normFlds cfg@(Config {
                 kE,
                 kF = h@(Has t1 (Decl ft1 x1)):kF_@(Has t2 (Decl ft2 x2):_) }) =
  let kE' = if (t1 == t2) && (x1 == x2)
            then (ft1 :=: ft2):kE
            else kE
  in normFlds cfg { kE = kE', kF = kF_ ++ [h] }

-- | MN-skip
normFlds cfg@(Config { kF = h@(Has _ _):B:kF_ }) =
  cfg { kF = kF_ ++ [h] }

-- | MN-nfld
normFlds cfg@(Config { kE = [], kF = [B] }) =
  cfg { kF = [] }


-------------------------------------
-- Convergence of field membership --
-------------------------------------

-- | Convergence of has constraints
converge :: Config -> IO Config
converge cfg = do
  let cfg' = sortHas cfg
  debug "sort-membership" (showConfig cfg')

  let cfg'' = normFlds cfg' { kF = (kF cfg') ++ [B] }
  debug "equalize-fields" (showConfig cfg'')

  let cfg''' = unifyEq cfg''
  debug "unify-fields" (showConfig cfg''')

  if (kF cfg'') == (kF cfg''')
    then return $ cfg'''
    else converge cfg'''


--------------------------------
-- Solver: record composition --
--------------------------------

composeRecs :: Config -> Config

-- | RC-inst
composeRecs cfg@(Config {
                    phi,
                    psi,
                    theta,
                    kF = k@(Has t@(TyVar st@(Stamp n)) d):kF_ }) =
  let x = Ident $ "TYPE_" ++ (show n)
      t = NamedTy x
      s = st :-> t
      theta' = (addToTheta (hat t) (RecTy [d] x) theta)
  in composeRecs cfg { phi = apply s phi,
                       psi = apply s psi,
                       theta = apply s theta',
                       kF = apply s kF_ }

-- | RC-upd
composeRecs cfg@(Config {
                    theta,
                    kF = k@(Has t@(NamedTy _) d):kF_ }) =
  case findInTheta (hat t) theta of
    r@(RecTy dl x) ->
      let r' = if (elem d dl) then r else (RecTy (d:dl) x)
      in composeRecs cfg { kF = kF_,
                           theta = getsUpdate (hat t) r' theta }
    _ -> error $ "can't recognized record " ++ show (ppK k)

-- | RC-end
composeRecs cfg = cfg


-- | Update the type id to type mapping of Theta.
getsUpdate :: TypeId -> Type -> Theta -> Theta
getsUpdate tid t theta =
  case Map.lookup tid theta of
    Just _ -> Map.alter (\_ -> Just t) tid theta
    _ -> error $ show (ppK tid) ++ " can't be found in Φ for update\n"


------------------------------
-- Solver: de-orphanization --
------------------------------

deorph :: Config -> Config

-- | DO
deorph cfg@Config { phi, psi, theta } =
  cfg { phi = phi', psi = psi', theta = theta' }
  where
    bind ((tid, TyVar st@(Stamp n)):l) =
      if "struct " `isPrefixOf` (_id tid)
      then (st :-> (RecTy [Decl IntTy (Ident "dummy")] (Ident (_id tid)))):(bind l)
      else (st :-> NamedTy (Ident "int/*orphan*/ ")):(bind l)
    bind (_:l) = bind l
    bind [] = []

    s = bind (Map.toList theta)
    phi' = foreachValue s phi
    psi' = foreachValue s psi
    theta' = foreachValue s theta


---------------------
-- Complete solver --
---------------------

solveConstraints :: K -> Config -> IO Config
solveConstraints k cfg = do
  let cfgPP = preprocess cfg
  debug "preprocessing" (showConfig cfgPP)

  let cfgUE = unifyEq cfgPP
  debug "unify-equivalences" (showConfig cfgUE)

  let cfgSOL = splitOrderLift cfgUE
  debug "split-order-lift" (showConfig cfgSOL)

  let cfgUI = unifyIq cfgSOL
  debug "unify-inequalities" (showConfig cfgUI)

  let cfgUP = unifyWb cfgUI { kI = (kW cfgUI), kW = [] }
  debug "unify-wobbly" (showConfig cfgUP)

  cfgUF <- converge cfgUP

  let cfgCR = composeRecs cfgUF
  debug "compose-records" (showConfig cfgCR)

  let cfgOR = deorph cfgCR
  debug "deorph" (showConfig cfgOR)

  return cfgOR

debug msg content = do
  putStrLn $ "\n<<< " ++ msg ++ " >>>\n" ++ content
  writeFile (msg ++ ".log") content


------------------
-- Typing rules --
------------------

type Gamma = Map Ident Type

verifyTyping :: Config -> Gamma -> Prog -> [Type]
verifyTyping c g p = typeProg c g p

-- | TCPrg
typeProg :: Config -> Gamma -> Prog -> [Type]
typeProg c gam (Prog _ fl) =
  foldr (\f acc -> (typeFunDef c gam f):acc) [] fl

-- | TCFun
typeFunDef :: Config -> Gamma -> FunDef -> Type
typeFunDef c gam (FunDef rt f dl s) =
  typeParam c gam dl s rt

-- | TCPar
typeParam :: Config -> Gamma -> [Decl] -> [Stmt] -> Type -> Type
typeParam c gam [] s rt = typeStmt c gam s rt
typeParam c gam ((Decl t x):dl) s rt =
  let t' = findInPsi x (psi c)
      gam' = addToGamma x t' gam
  in typeParam c gam' dl s rt

-- Type checking signature for statements.
typeStmt :: Config -> Gamma -> [Stmt] -> Type -> Type

-- | TCDcl
typeStmt c gam ((DeclStmt (Decl t x)):sl) rt =
  let t' = findInPsi x (psi c)
      gam' = addToGamma x t' gam
  in typeStmt c gam' sl rt

-- | TCExp
typeStmt c gam ((ExprStmt e):sl) rt =
  let t = typeExpr c gam e
  in if (isGround t)
     then typeStmt c gam sl rt
     else error $ "expected ground type " ++ show (ppC t)
          ++ " for expression " ++ show e

-- | TCRetZr
typeStmt c gam ((RetStmt (NumLit (IntLit 0))):[]) rt =
  let rt' = (findInTheta (hat rt) (theta c))
  in if isScaTy rt'
     then rt'
     else error $ "0 doesn't type with " ++ show (ppC rt') ++ " as return"

-- | TCRet
typeStmt c gam ((RetStmt e):[]) rt =
  let t = typeExpr c gam e
      rt' = (findInTheta (hat rt) (theta c))
  in if isSubTy' t rt'
     then rt'
     else error $ "return doesn't type "
          ++ show (ppC rt) ++ "::" ++ show (ppC rt')

-- Type checking signature for expressions.
typeExpr :: Config -> Gamma -> Expr -> Type

-- | TCLit
typeExpr _ _ (NumLit l) = rho l

-- | TCVar
typeExpr c gam e@(Var x) =
  let t = findInPsi x (psi c)
  in if (t == findInGamma x gam && isGround t)
     then t
     else error $ "Γ and C type mismatch " ++ (show e)

-- | TCFld
typeExpr c gam (FldAcc e x) =
  let pt = typeExpr c gam e
  in case pt of
       PtrTy rt ->
         case findInTheta (hat rt) (theta c) of
           t@(RecTy dl _) -> field x t
           _ -> error $ "expected record in Γ " ++ show (ppC rt)
       _ -> error $ "expected " ++ show (ppC pt) ++ " typed as pointer"

-- | TCDrf
typeExpr c gam (Deref e) =
  let t = typeExpr c gam e
  in case t of
    PtrTy t' -> t'
    _ -> error $ "dereference doesn't type check"

-- | TCAdr
typeExpr c gam (AddrOf e) =
  PtrTy (typeExpr c gam e)

-- | TCAsgZr
typeExpr c gam (BinExpr Assign e1 (NumLit (IntLit 0))) =
  let lht = typeExpr c gam e1
  in if isScaTy lht
     then lht
     else error $ "assignment to 0 doesn't type check"

-- | TCAsg
typeExpr c gam (BinExpr Assign e1 e2) =
  let lht = typeExpr c gam e1
      rht = typeExpr c gam e2
  in if isSubTy' rht lht
     then rht
     else error $ "assignment doesn't type check"

-- | TCAdd
typeExpr c gam (BinExpr Add e1 e2) =
  let lht = typeExpr c gam e1
      lht' = case lht of {(ConstTy t) -> t; _ -> lht}
      rht = typeExpr c gam e2
      rht' = case rht of {(ConstTy t) -> t; _ -> rht}
  in case lht' of
    PtrTy _ -> if rht' == IntTy
               then lht
               else error $ "expected int as RHS of +"
    _ -> case rht' of
      PtrTy _ -> if lht' == IntTy
                 then rht
                 else error $ "expected int as LHS of +"
      _ -> if isAriTy lht && isAriTy rht
           then highRank lht rht
           else error $ "incompatible types in + (Add)"

-- | TCAnd
typeExpr c gam (BinExpr And e1 e2) =
  let lht = typeExpr c gam e1
      rht = typeExpr c gam e2
  in if isScaTy lht && isScaTy rht
     then IntTy
     else error $ "incompatible types in && (logical AND)"

-- | TCOr
typeExpr c gam (BinExpr Or e1 e2) =
  let lht = typeExpr c gam e1
      rht = typeExpr c gam e2
  in if isScaTy lht && isScaTy rht
     then IntTy
     else error $ "incompatible types in || (logical OR)"

-- | TCDiv
typeExpr c gam (BinExpr Divide e1 e2) =
  let lht = typeExpr c gam e1
      rht = typeExpr c gam e2
  in if isAriTy lht && isAriTy rht
     then highRank lht rht
     else error $ "incompatible types in / (division)"

-- | TCMul
typeExpr c gam (BinExpr Multiply e1 e2) =
  let lht = typeExpr c gam e1
      rht = typeExpr c gam e2
  in if isAriTy lht && isAriTy rht
     then highRank lht rht
     else error $ "incompatible types in * (multiplication)"


-- | Find, in Gamma, the type mapped to an identifier.
findInGamma :: Ident -> Gamma -> Type
findInGamma x gam =
  case Map.lookup x gam of
    Just t -> t
    _ -> error $
         "no τ for identier " ++ show (ppK x) ++ " in Γ\n"

-- | Add, to Gamma, an identifier and its mapped type.
addToGamma :: Ident -> Type -> Gamma -> Gamma
addToGamma x t gam =
  case Map.lookup x gam of
    Just t -> error $
      show (ppK t) ++ ", indexed by " ++
      show (ppK x) ++ ", is already in Γ\n"
    _ -> Map.insert x t gam


--------------------
-- Typing support --
--------------------

-- | Return whether the type is an arithmetic type.
isAriTy :: Type -> Bool
isAriTy (ConstTy t) = isAriTy t
isAriTy IntTy = True
isAriTy DoubleTy = True
isAriTy _ = error $ "expected arithmetic type"

-- | Return whether the type is scalar.
isScaTy :: Type -> Bool
isScaTy (ConstTy t) = isScaTy t
isScaTy (PtrTy _) = True
isScaTy IntTy = True
isScaTy DoubleTy = True
isScaTy _ = error $ "expected scalar type"

-- | Return whether the type is a pointer.
isPtrTy :: Type -> Bool
isPtrTy (ConstTy t) = isPtrTy t
isPtrTy (PtrTy _) = True
isPtrTy _ = error $ "expected pointer type"

-- | Return the highest ranked of 2 arithmetic types.
highRank :: Type -> Type -> Type
highRank t1 t2 =
  case t1 of
    (ConstTy t1') -> highRank t1' t2
    IntTy -> t2
    _ -> t1


-------------------------------------
-- Pretty printing for constraints --
-------------------------------------

class PrettyK a where
  ppK :: a -> PP.Doc

instance PrettyK a => PrettyK [a] where
  ppK v = foldr (\x acc -> ppK x PP.<+> PP.text " " PP.<+> acc )
          PP.empty v

instance PrettyK Ident where
  ppK = PP.text . _x

instance PrettyK TypeId where
  ppK = PP.text . _id

instance PrettyK Type where
  ppK IntTy = PP.text "int"
  ppK DoubleTy = PP.text "double"
  ppK VoidTy = PP.text "void"
  ppK (PtrTy t) = ppK t PP.<> PP.text "*"
  ppK (ConstTy t) = PP.text "const " PP.<> ppK t
  ppK (ArrowTy rt ps) =
    PP.text " (" PP.<>
    (PP.hcat $ PP.punctuate (PP.text ", ") (map ppK ps)) PP.<>
    PP.text " )" PP.<>
    PP.text "⟶  " PP.<> ppK rt
  ppK (RecTy flds x) = PP.char '@' PP.<> ppK x PP.<> PP.char '@'
  ppK (NamedTy x) = ppK x
  ppK (TyVar (Stamp n)) = PP.text "α" PP.<> PP.text (show n)

instance PrettyK Subst where
  ppK Trivial = PP.text "[]"
  ppK (st :-> t) = ppK st PP.<> PP.text "->" PP.<> ppK t

instance PrettyK K where
  ppK T = PP.text "⊤"
  ppK B = PP.text "⊥"
  ppK (k1 :&: k2) = ppK k1 PP.<+> PP.text " ^ " PP.<+> ppK k2
  ppK (Exists t k) =
    PP.text "∃" PP.<>
    (foldl (\acc t@(TyVar _) -> acc PP.<> ppK t) PP.empty t) PP.<>
    PP.text ". " PP.<> ppK k
  ppK (Def x t k) =
    PP.text "def" PP.<+> ppK x PP.<> PP.colon PP.<> ppK t PP.<+>
    PP.text "in" PP.<+> ppK k
  ppK (Fun f t@(ArrowTy _ _) k) =
    PP.text "fun" PP.<+> ppK f PP.<> PP.colon PP.<> ppK t PP.<+>
    PP.text "in " PP.<+> ppK k
  ppK (TypeOf x t) =
    PP.text "typeof(" PP.<> ppK x PP.<> PP.text ","
    PP.<> ppK t PP.<> PP.char ')'
  ppK (Syn t1 t2) =
    PP.text "syn " PP.<> ppK t1 PP.<> PP.text " as " PP.<> ppK t2
  ppK (Has t fld) =
    PP.text "has" PP.<>
    PP.parens (ppK t PP.<> PP.comma PP.<+>
               ppK (_fx fld) PP.<> PP.colon PP.<>
               ppK (_ft fld))
  ppK (t1 :=: t2) = ppK t1 PP.<> PP.text "≡" PP.<> ppK t2
  ppK (t1 :<=: t2) = ppK t1 PP.<> PP.text "≤" PP.<> ppK t2

instance PrettyK Stamp where
  ppK (Stamp n) = PP.text "α" PP.<> PP.text (show n)


--------------------------------
-- Pretty printing of lattice --
--------------------------------

class PrettyM a where
  ppM :: a -> PP.Doc

instance PrettyM ShapeKey where
  ppM U = PP.text "<Undefined>"
  ppM S = PP.text "<Scalar>"
  ppM P = PP.text "<Pointer>"
  ppM I = PP.text "<Integral>"
  ppM FP = PP.text "<FloatingPoint>"
  ppM N = PP.text "<Numeric>"

instance PrettyM SyntaxRole where
  ppM (ValRole e) = ppM e
  ppM FunRole = PP.text "... ⟶  "

instance PrettyM Expr where
  ppM (NumLit v) = PP.text $ show v
  ppM (Var x) = PP.text (_x x)
  ppM (FldAcc e x) = ppM e PP.<> PP.text "->" PP.<> PP.text (_x x)
  ppM (Deref e) = PP.char '*' PP.<> ppM e
  ppM (AddrOf e) = PP.char '&' PP.<> ppM e
  ppM (BinExpr Add e1 e2) = ppM e1 PP.<> PP.char '+' PP.<> ppM e2
  ppM (BinExpr Divide e1 e2) = ppM e1 PP.<> PP.char '/' PP.<> ppM e2
  ppM (BinExpr Multiply e1 e2) = ppM e1 PP.<> PP.char '*' PP.<> ppM e2
  ppM (BinExpr And e1 e2) = ppM e1 PP.<> PP.text "&&" PP.<> ppM e2
  ppM (BinExpr Or e1 e2) = ppM e1 PP.<> PP.text "||" PP.<> ppM e2
  ppM (BinExpr Assign e1 e2) = ppM e1 PP.<> PP.char '=' PP.<> ppM e2

instance PrettyM M where
  ppM m = Map.foldrWithKey (\k v acc -> ppM k PP.<+> ppM (fst v) PP.$$ acc)
                        PP.empty
                        (_shapes m)


----------------------------
-- Pretty printing of AST --
----------------------------

class PrettyAST a where
  fmt :: Int -> a -> PP.Doc

instance PrettyAST a => PrettyAST [a] where
  fmt n (s:sl) =
    fmt n s PP.<> (foldr (\s d -> fmt n s PP.<> d) PP.empty sl)

instance PrettyAST Lit where
  fmt _ (IntLit l) =
    PP.char '`' PP.<>
    PP.text (show l) PP.<>
    PP.char '\''
  fmt _ (DoubleLit l) =
    PP.char '`' PP.<>
    PP.text (show l) PP.<>
    PP.char '\''

instance PrettyAST Expr where
  fmt n e@(NumLit l) = indent n PP.<> PP.text "NumLit" PP.<+> fmt 0 l
  fmt n e@(Var x) =
    indent n PP.<>
    PP.text "Var" PP.<+>
    PP.char '`' PP.<>
    PP.text (_x x) PP.<>
    PP.char '\''
  fmt n e@(FldAcc x t) = indent n PP.<> PP.text "FieldAccess"
  fmt n e@(Deref e1) = indent n PP.<> PP.text "Deref" PP.<> fmt (n + 1) e1
  fmt n e@(AddrOf e1) = indent n PP.<> PP.text "AddrOf" PP.<> fmt (n + 1) e1
  fmt n e@(BinExpr op e1 e2) =
    indent n PP.<>
    PP.text "BinExpr" PP.<+>
    PP.char '`' PP.<> PP.text (show op) PP.<> PP.char '\'' PP.<+>
    fmt (n + 1) e1 PP.<>
    fmt (n + 1) e2

instance PrettyAST Stmt where
  fmt n (DeclStmt d) = indent n PP.<> PP.text "DeclStmt" PP.<> fmt (n + 1) d
  fmt n (ExprStmt e) = indent n PP.<> PP.text "ExprStmt" PP.<> fmt (n + 1) e
  fmt n (RetStmt e) = indent n PP.<> PP.text "RetStmt" PP.<> fmt (n + 1) e

instance PrettyAST Decl where
  fmt n (Decl _ x) =
    indent n PP.<>
    PP.text "Decl" PP.<+>
    PP.char '`' PP.<>
    PP.text (_x x) PP.<>
    PP.char '\''

instance PrettyAST FunDef where
  fmt n f@(FunDef _ _ ps sl) =
    PP.text "Function" PP.<>
    (foldr (\p d -> fmt (n + 1) p PP.<> d) PP.empty ps) PP.<>
    fmt (n + 1) sl

instance PrettyAST Prog where
  fmt n p@(Prog _ fs) =
    (foldr (\f d -> fmt (n + 1) f PP.<> d) PP.empty fs)

indent :: Int -> PP.Doc
indent n = PP.char '\n' PP.<> PP.text (replicate n ' ')


---------------------------------
-- Pretty printing for C types --
---------------------------------

class PrettyC a where
  ppC :: a -> PP.Doc

instance PrettyC TypeId where
  ppC tn = PP.text $ _id tn

instance PrettyC Ident where
  ppC x = PP.text $ _x x

instance PrettyC Decl where
  ppC (Decl t x) = PP.space PP.<>  ppC t PP.<+> ppC x PP.<> PP.semi

instance PrettyC Type where
  ppC IntTy = PP.text "int"
  ppC DoubleTy = PP.text "double"
  ppC VoidTy = PP.text "void"
  ppC (PtrTy t) = ppC t PP.<> PP.char '*'
  ppC (ConstTy t) = PP.text "const" PP.<+> ppC t
  ppC (ArrowTy rt pt) =
    ppC rt PP.<> PP.text "(*)" PP.<>
    PP.parens (PP.hcat $ PP.punctuate (PP.text ", ") (map ppC pt))
  ppC (RecTy fld x) =
    let prefix = if "struct " `isPrefixOf` (_x x)
                 then ""
                 else "struct "
    in PP.text prefix PP.<> PP.text (_x x) PP.<+>
    PP.braces (PP.hcat $ (map ppC fld))
  ppC (NamedTy x) = PP.text $ _x x
  ppC (TyVar (Stamp n)) = PP.text "α" PP.<> PP.text (show n)


formatMap m = show $
  Map.foldrWithKey
  (\k v acc -> PP.lbrace PP.<> PP.space PP.<> ppK k PP.<>
               PP.comma PP.<> PP.space PP.<> ppC v PP.<>
               PP.rbrace PP.<> PP.comma PP.<> PP.space PP.<> acc)
  PP.empty  m

formatPhiPsiTheta (phi, psi, theta) =
  PP.text (" Φ = { " ++ formatMap phi) PP.<+> PP.text "}\n" PP.<>
  PP.text (" ψ = { " ++ formatMap psi) PP.<+> PP.text "}\n" PP.<>
  PP.text (" Θ = { " ++ formatMap theta) PP.<+> PP.text "}\n"

showConfig cfg@(Config { phi, psi, theta, k, kE, kF, kI, kW }) =
  show $
  formatPhiPsiTheta (phi, psi, theta) PP.<>
  PP.text " [Ke] = " PP.<> ppK kE PP.<+> PP.text "\n" PP.<>
  PP.text " [Kf] = " PP.<> ppK kF PP.<+> PP.text "\n" PP.<>
  PP.text " [Ki] = " PP.<> ppK kI PP.<+> PP.text "\n" PP.<>
  PP.text " [Kw] = " PP.<> ppK kW


--------------------
-- Parser for muC --
--------------------

langDef = emptyDef {
  Token.identStart = letter,
  Token.identLetter = alphaNum <|> char '_',
  Token.reservedNames =
      [ "int", "double", "void", "const", "return", "struct", "typedef" ],
  Token.reservedOpNames =
      [ "*", "/", "+", "||", "=", "&", "->" ]
  }

lexer = Token.makeTokenParser langDef
identifier = Token.identifier lexer
reserved = Token.reserved lexer
reservedOp = Token.reservedOp lexer
parens = Token.parens lexer
braces = Token.braces lexer
integer = Token.integer lexer
float = Token.float lexer
semi = Token.semi lexer
whiteSpace = Token.whiteSpace lexer
comma = Token.comma lexer
symbol = Token.symbol lexer

parseSource :: String -> Either String Prog
parseSource = either (Left . show) Right . parse progParser ""

progParser :: Parser Prog
progParser = Prog <$> many tydefParser <*> many funParser

tydefParser :: Parser TypeDef
tydefParser = TypeDef <$> (reserved "typedef" *>) qualPtrTyParser <*> tyParser <* semi

funParser :: Parser FunDef
funParser = FunDef
            <$> qualPtrTyParser
            <*> identParser
            <*> parens (declParser `sepBy` comma)
            <*> stmtListParser

qualPtrTyParser :: Parser Type
qualPtrTyParser = f <$> tyParser <*> (optionMaybe (reserved "const"))
  where
    f t Nothing = t
    f t _ = ConstTy t

tyParser :: Parser Type
tyParser = f <$> nonPtrTyParser <*> (many starParser)
  where
    f t ts = foldr (\_ ac -> PtrTy ac) t ts

nonPtrTyParser :: Parser Type
nonPtrTyParser = try (qualTyParser intTyParser)
                 <|> try (qualTyParser fpTyParser)
                 <|> try (qualTyParser namedTyParser)
                 <|> qualTyParser recTyParser

starParser :: Parser ()
starParser = () <$ symbol "*"

intTyParser :: Parser Type
intTyParser = IntTy <$ reserved "int"

fpTyParser :: Parser Type
fpTyParser = DoubleTy <$ reserved "double"

voidTyParser :: Parser Type
voidTyParser = VoidTy <$ reserved "void"

namedTyParser :: Parser Type
namedTyParser = f <$> (optionMaybe (reserved "struct")) <*> identParser
  where
    f Nothing n = NamedTy n
    f _ n = NamedTy (Ident ("struct " ++ (_x n)))

qualTyParser :: Parser Type -> Parser Type
qualTyParser p = f <$> (optionMaybe (reserved "const")) <*> p
  where
    f Nothing t = t
    f _ t = ConstTy t

recTyParser :: Parser Type
recTyParser = RecTy
              <$> braces (declParser `sepBy` semi)
              <*> identParser

declParser :: Parser Decl
declParser = Decl <$> qualPtrTyParser <*> identParser

exprParser :: Parser Expr
exprParser = buildExpressionParser table baseExprParser
  where
    table =
      [ [ Prefix (reservedOp "*" >> return Deref) ],
        [ Prefix (reservedOp "&" >> return AddrOf) ],
        [ Infix (reservedOp "/" >> return (BinExpr Divide)) AssocLeft ],
        [ Infix (reservedOp "*" >> return (BinExpr Divide)) AssocLeft ],
        [ Infix (reservedOp "+" >> return (BinExpr Add)) AssocLeft ],
        [ Infix (reservedOp "&&" >> return (BinExpr And)) AssocLeft ],
        [ Infix (reservedOp "||" >> return (BinExpr Or)) AssocLeft ],
        [ Infix (reservedOp "=" >> return (BinExpr Assign)) AssocLeft ] ]

baseExprParser :: Parser Expr
baseExprParser = f <$> fldAccParser <*> (many (reservedOp "->" *> identParser))
  where
    f fld xs = foldr (\ x acc -> FldAcc acc x) fld xs

fldAccParser :: Parser Expr
fldAccParser = f <$> primExprParser <*> option id fldAcc
  where
    f x expr = expr x
    fldAcc = flip FldAcc <$> (reservedOp "->" *> identParser)

intParser :: Parser Lit
intParser = IntLit <$> (fromInteger <$> integer)

fpParser :: Parser Lit
fpParser = DoubleLit <$> float

primExprParser :: Parser Expr
primExprParser =  NumLit <$> (try fpParser <|> intParser)
                  <|> Var <$> identParser

stmtParser :: Parser Stmt
stmtParser = RetStmt <$> (reserved "return" *> exprParser)
             <|> try (DeclStmt <$> declParser)
             <|> ExprStmt <$> exprParser

stmtListParser :: Parser [Stmt]
stmtListParser = braces (stmtParser `endBy` semi)

identParser :: Parser Ident
identParser = Ident <$> identifier


------------------
-- Rewrite to C --
------------------

-- | Rewrite inferred types to their C form. This function is simplified.
--   Check PyscheC for a complete implementation.
rewriteInC :: Map TypeId Type -> String
rewriteInC theta =
  let
    p (tid@(TypeId id), RecTy _ _ )
      | "struct " `isPrefixOf` id = False
      | otherwise = True
    p (tid@(TypeId id), t)
      | "const " `isPrefixOf` id = False
      | "*" `isSuffixOf` id = False
      | tid == (hat t) = False -- Duplicate: self-definition.
      | otherwise = True
    filtered = filter p (Map.toList theta)

    print (t1, t2) acc = "typedef " ++
                PP.render (ppC t2) ++ " " ++
                PP.render (ppC t1) ++ ";\n" ++ acc

    tydefs = foldr print "" filtered
  in
    (rewriteInC' theta) ++ tydefs

rewriteInC' :: Map TypeId Type -> String
rewriteInC' theta =
  let print' (RecTy _ (Ident x)) = "typedef struct " ++ x ++ " " ++ x ++ ";\n"
      print' (PtrTy t) = print' t
      print' (ConstTy t) = print' t
      print' _ = ""
      print (tid@(TypeId id), t) acc
        | "struct " `isPrefixOf` id = PP.render (ppC t) ++ ";\n"
        | otherwise = print' t ++ acc
  in foldr print "" (Map.toList theta)
