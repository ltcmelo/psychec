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

module Solver.Decaying where

import Data.Map (Map)
import qualified Data.Map as Map
import qualified Data.List as List
import Data.Type
import Data.BuiltIn
import Solver.SolverMonad


decay :: TyCtx -> VarCtx -> SolverM (TyCtx, VarCtx)
decay tctx vctx =
    return ((TyCtx $ Map.map (\(t, b) -> (matchPtrTy t, b))  (tyctx tctx)),
            (VarCtx $ Map.map (\(ValSym t d r st) -> ValSym (matchPtrTy t) d r st) (varctx vctx)))

matchPtrTy :: Ty -> Ty
matchPtrTy t@(PtrTy t') = matchFuncPtrTy t' 1
matchPtrTy (RecTy fs n) =
  RecTy (List.map (\(Field fn ft) -> Field fn (matchPtrTy ft)) fs) n
matchPtrTy (SumTy fs n) =
  SumTy (List.map (\(Field fn ft) -> Field fn (matchPtrTy ft)) fs) n
matchPtrTy (FunTy rt pt) = FunTy (matchPtrTy rt) (List.map matchPtrTy pt)
matchPtrTy t = t


matchFuncPtrTy :: (Num a, Eq a) => Ty -> a -> Ty
matchFuncPtrTy t@(PtrTy t') n = matchFuncPtrTy t' (n + 1)
matchFuncPtrTy t@(FunTy r p) _ = t
matchFuncPtrTy t n = rebuildPtrTy t n

rebuildPtrTy :: (Num a, Eq a) => Ty -> a -> Ty
rebuildPtrTy t 0 = t
rebuildPtrTy t n = rebuildPtrTy (PtrTy t) (n - 1)
