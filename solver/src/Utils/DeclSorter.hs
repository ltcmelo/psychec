-- Copyright (c) 2016 Leandro T. C. Melo (ltcmelo@gmail.com)
--
-- This library is free software; you can redistribute it and/or modify it under
-- the terms of the GNU Lesser General Public License as published by the Free
-- Software Foundation; either version 2.1 of the License, or (at your option)
-- any later version.
--
-- This library is distributed in the hope that it will be useful, but WITHOUT
-- ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
-- FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License
-- for more details.
--
-- You should have received a copy of the GNU Lesser General Public License
-- along with this library; if not, write to the Free Software Foundation, Inc.,
-- 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301 USA


-- This modules sort declarations according to their type dependences. This is  done by creating a
-- graph in which types are nodes and then topologically sorting it.

module Utils.DeclSorter where

import Data.Type
import Data.Graph
import qualified Data.Map as Map
import Debug.Trace
import Utils.Pretty
import Solver.SolverMonad (TyCtx (..))


-- | Populate the graph by mapping each vertice to the type it represents.
populate :: TyCtx -> Map.Map Name Int
populate tctx =
    Map.foldr (\(t, _) acc -> Map.insert (nameOf t) (length acc) acc) Map.empty (tyctx tctx)

-- | Created edges between types that depend one another.
buildDeps :: Ty -> Map.Map Name Int -> [Int]
buildDeps (Struct fs n) m =
    foldr (\(Field _ ft) acc -> (buildDeps ft m) ++ acc) [] fs
buildDeps (Pointer t) m = buildDeps t m
buildDeps (QualTy t) m = buildDeps t m
buildDeps t m =
    case Map.lookup (nameOf t) m of
        Nothing -> []
        Just id -> [id]
        --Just id -> trace ("    depends on " ++ show id) ([id])

-- | Topological sort the graph (we want the reverse order).
sortDecls :: TyCtx -> [(Name, Ty)]
sortDecls tctx =
    foldr (\v acc -> (flt (vf v)):acc) [] (reverse $ topSort g)
    --trace (print g vf) (foldr (\v acc -> (flt (vf v)):acc) [] (reverse $ topSort g))
  where
    (el, _) =
      Map.foldrWithKey
        (\n (t, _) (l, m) ->
          (((n, t), length l, buildDeps t m):l, m))
          --(((n, t), length l, trace(show (pprint t)) (buildDeps t tm)):l, tm))
        ([], m)
        (tyctx tctx)
    m = populate tctx
    (g, vf, _) = graphFromEdges el
    flt (node, _, _) = node

-- | Helper to print the graph.
print g vf =
    (show $ edges g)
      ++ "\n"
      ++ foldr (\v acc -> (sortDecls (vf v)) ++ "\n" ++ acc) "" (vertices g)
  where
    sortDecls ((n, t), k, _) = show k ++ ": " ++ show (pprint n <+> text "as" <+> pprint t)
