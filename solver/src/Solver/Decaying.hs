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
import Data.Type
import Data.BuiltIn
import Solver.SolverMonad


decay :: TyCtx -> VarCtx -> SolverM (TyCtx, VarCtx)
decay tctx vctx =
    return ((TyCtx $ Map.map (\(t, b) -> (matchPointer t, b))  (tyctx tctx)),
            (VarCtx $ Map.map (\(VarInfo t d r) -> VarInfo (matchPointer t) d r) (varctx vctx)))

matchPointer :: Ty -> Ty
matchPointer t@(Pointer t') = matchFuncPointer t' 1
matchPointer t = t

matchFuncPointer :: (Num a, Eq a) => Ty -> a -> Ty
matchFuncPointer t@(Pointer t') n = matchFuncPointer t' (n + 1)
matchFuncPointer t@(FunTy r p) _ = t
matchFuncPointer t n = rebuildPointer t n

rebuildPointer :: (Num a, Eq a) => Ty -> a -> Ty
rebuildPointer t 0 = t
rebuildPointer t n = rebuildPointer (Pointer t) (n - 1)
