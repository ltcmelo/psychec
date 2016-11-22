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

> module Utils.Writer where

> import Data.Type
> import Data.BuiltIn
> import Utils.Pretty
> import Solver.ConstraintSolver
> import Solver.SolverMonad (TyCtx (..), VarCtx(..))
> import Text.PrettyPrint.HughesPJ
> import Utils.Pretty (pprint, (<+>), text)
> import qualified Data.Map as Map
> import qualified Data.List as List
> import qualified Data.Graph as Graph

> writeCFile :: (TyCtx, VarCtx) -> String
> writeCFile ctxs = writeIncs ++ writeBoolDef ++ writeDecls ctxs


> writeIncs :: String
> writeIncs = "#include <stdint.h>\n" ++
>             "#include <stddef.h>\n" ++
>             "#include <stdlib.h>\n" ++
>             "#include <string.h>\n" ++
>             "#include <sys/types.h>\n" ++
>             "#include <sys/stat.h>\n" ++
>             "\n"


Our generator parses a `bool' type and interpret it as an `int'. There's a lot
of C code using such "tricks". If we don't do so, our solver would still
generate definitions as those below, but `true' and `false' would not be
initialized and potentially leading to incorrect semantics of the original
code. We claim this is a fair adaptation based on de facto knowledge.

> writeBoolDef :: String
> writeBoolDef = "typedef int bool;\n" ++
>                "const bool false = 0;\n" ++
>                "const bool true = 1;\n" ++
>                "\n"


> writeDecls :: (TyCtx, VarCtx) -> String
> writeDecls (tcx,vcx) = t ++ v
>     where
>         t = foldr gentydef [] (sortTypeDefinitions tcx)
>         gentydef (n, t) ac = (show $ text "typedef " <+>
>                               writeTyDecl t n <+>
>                               text ";\n") ++ ac
>         -- Constant counter begins at 128 to avoid colisions with
>         -- ASCII characters. This will change in the future,
>         -- with the generator sending the values, so there's no
>         -- duplication risk in case statements for example.
>         (v, _) = Map.foldrWithKey genvardef ([], 128) (varctx vcx)
>         genvardef n (VarInfo t _ ro) (l, cnt)
>             = if ro then
>                   ((show $ writeDefine n cnt <+> text "\n") ++ l, cnt + 1)
>               else
>                   ((show $ writeVarDecl t n <+> text "\n") ++ l, cnt)


Write declaration of missing values.

> writeVarDecl :: Ty -> Name -> Doc
> writeVarDecl (FunTy ret params) n
>   =  pprint ret <+> pprint n <+> parens (hcat $ punctuate comma (map pprint params))
>      <+> text ";"
> writeVarDecl t n = pprint t <+> pprint n <+> text ";"

> writeDefine :: Show a => Name -> a -> Doc
> writeDefine n cnt = text "#define " <+> pprint n <+> text (show cnt)


Write declaration of missing types.

> writeTyDecl :: Ty -> Name -> Doc
> writeTyDecl (FunTy ret params) n
>       = pprint ret <+> text "(*" <+> pprint n <+> text ")"
>             <+> parens (hcat $ punctuate comma (map pprint params))
> writeTyDecl t n = pprint t <+> pprint (ensurePlainName n)


Sort declarations that depend one on another

> sortTypeDefinitions :: TyCtx -> [(Name, Ty)]
> sortTypeDefinitions tcx = typeDeclarationList
>   where
>     allTypeNames = map (\(x, y) -> (TyCon x, fst y)) $ Map.toList (tyctx tcx) -- get all the type names
>     typeNames = List.nub . foldl (\acc (x, y) -> x:y:acc) [] $ allTypeNames -- remove repeated types
>     numberOfDifferentTypes = length typeNames -- get the number of diferent type names
>     typeIdMap = Map.fromList $ zip typeNames [1..numberOfDifferentTypes] -- create an map (typename, id)
>     idTypeMap = Map.fromList $ zip [1..numberOfDifferentTypes] typeNames -- create an map (id, typename)
>     typeToId acc (type1, type2) = (typeId1, typeId2) : acc
>       where
>         typeId1 = maybe (-1) id $ Map.lookup type1 typeIdMap
>         typeId2 = maybe (-1) id $ Map.lookup type2 typeIdMap
>     typeDependenceEdges = foldl typeToId [] allTypeNames
>     -- TODO: increase the number of edges in the typeDependenceGraph (add constraints to the types inside of structs)
>     typeDependenceGraph = Graph.buildG (1, numberOfDifferentTypes) typeDependenceEdges
>     typeDeclarationOrder = reverse . Graph.topSort $ typeDependenceGraph
>     shouldIDefineThisType typeName = and $ restrictions <*> [typeName]
>       where
>         restrictions = [(\ty -> not . elem (nameOf ty) $ Map.keys builtinTyCtx)
>                        ,(\ty -> not . typeIsStruct $ ty)
>                        ,(\ty -> not . typeIsEnum $ ty)
>                        ,(\ty -> not . typeIsFunction $ ty)]
>     insertTypeDeclaration typeId acc =
>       case Map.lookup typeId idTypeMap of
>         Nothing -> acc
>         Just t' -> case Map.lookup (nameOf t') (tyctx tcx) of
>           Nothing -> acc
>           Just (t'', _) -> if (shouldIDefineThisType t')
>             then (nameOf t', t''):acc
>             else acc
>     typeDeclarationList = foldr insertTypeDeclaration [] typeDeclarationOrder
