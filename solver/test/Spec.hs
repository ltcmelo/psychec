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

import Test.Tasty
import Test.Tasty.HUnit

import Control.Monad
import Data.Constraints
import Data.Either
import Data.BuiltIn
import qualified Data.Map as Map
import Data.Type
import Parser.ConstraintParser hiding (typeDefParser)
import Solver.ConstraintSolver
import Solver.SolverMonad
import Utils.Pretty hiding (semi)
import Utils.Writer
import System.Process

main :: IO ()
main = defaultMain tests

cases :: String
cases = "./test/cases/"

tests :: TestTree
tests = testGroup "Unit tests"
                  [
                    doTest "T1.c"
                  , doTest "T2.c"
                  , doTest "T3.c"
                  , doTest "T4.c"
                  , doTest "T5.c"
                  , doTest "T6.c"
                  , doTest "T7.c"
                  , doTest "T8.c"
                  , doTest "T9.c"
                  , doTest "T10.c"
                  , doTest "T11.c"
                  , doTest "T12.c"
                  , doTest "T13.c"
                  , doTest "T14.c"
                  , doTest "T15.c"
                  , doTest "T16.c"
                  , doTest "T17.c"
                  , doTest "T18.c"
                  , doTest "T19.c"
                  , doTest "T20.c"
                  , doTest "T21.c"
                  , doTest "T22.c"
                  , doTest "T23.c"
                  , doTest "T24.c"
                  , doTest "T25.c"
                  , doTest "T26.c"
                  , doTest "T27.c"
                  , doTest "T28.c"
                  , doTest "T29.c"
                  , doTest "T30.c"
                  , doTest "T31.c"
                  , doTest "T32.c"
                  , doTest "T33.c"
                  , doTest "T34.c"
                  , doTest "T35.c"
                  , doTest "T36.c"
                  , doTest "T37.c"
                  , doTest "T38.c"
                  , doTest "T39.c"
                  , doTest "T40.c"
                  , doTest "T41.c"
                  , doTest "T42.c"
                  , doTest "T43_Sedgewick.c" -- Stack (4.5)
                  , doTest "T44_Sedgewick.c" -- queue (4.10)
                  , doTest "T45_Sedgewick.c" -- list1 (3.14)
                  , doTest "T45_Sedgewick_nofunctions.c"
                  , doTest "T46_Sedgewick.c" -- list1 // another example (3.19)
                  , doTest "T46_Sedgewick_nofunctions.c"
                  , doTest "T47_Sedgewick.c" -- Complex // another example (4.15)
                  , doTest "T48_Sedgewick.c" -- Complex (4.17)
                  , doTest "T49_Sedgewick.c" -- poly (4.23)
                  , doTest "T50_Sedgewick.c" -- tree (5.19)
                  , doTest "T51_Sedgewick.c" -- tree // another example (5.20)
                  , doTest "T52_Sedgewick.c" -- tree // another example balanced tree (13.1)
                  , doTest "T52_Sedgewick_nofunctions.c"
                  , doTest "T53_Sedgewick.c" -- graph matrix (17.5)
                  , doTest "T54_Sedgewick.c" -- tree3 (17.10)
                  , doTest "T55_Sedgewick.c" -- adjlist (partially trimmed)
                  , doTest "T55_Sedgewick_nofunctions.c"
                  , doTest "T56_Sedgewick.c" -- (partially trimmed)
                  , doTest "T57.c"
                  , doTest "T58.c"
                  , doTest "T59_Sedgewick.c" -- (partially trimmed)
                  , doTest "T60.c"
                  , doTest "T61.c"
                  , doTest "T62_Sedgewick.c" -- (3.2)
                  , doTest "T63_Sedgewick.c" -- (3.4)
                  , doTest "T64_Sedgewick.c" -- (3.8)
                  , doTest "T65_Sedgewick.c" -- (3.9)
                  , doTest "T66_Sedgewick.c" -- (3.10)
                  , doTest "T67_Sedgewick.c" -- (3.14)
                  , doTest "T68_Sedgewick.c" -- (3.19)
                  , doTest "T69_Sedgewick.c" -- (3.21)
                  , doTest "T70_Sedgewick.c" -- (4.4)
                  , doTest "T71_Sedgewick.c" -- (4.10)
                  , doTest "T72_Sedgewick.c" -- (4.11)
                  , doTest "T74_Sedgewick.c" -- (4.13)
                  , doTest "T75_Sedgewick.c" -- (4.19)
                  , doTest "T76_Sedgewick.c" -- (5.13)
                  , doTest "T77_Sedgewick.c" -- (5.17)
                  , doTest "T78_Sedgewick.c" -- (5.18)
                  , doTest "T79_Sedgewick.c" -- (13.4)
                  , doTest "T80_Sedgewick.c" -- (13.8)
                  , doTest "T81_Sedgewick.c" -- (17.2)
                  , doTest "T82_Sedgewick.c" -- (17.3)
                  , doTest "T83_Sedgewick.c" -- (17.5)
                  , doTest "T84_Sedgewick.c" -- (17.7)
                  , doTest "T85_Sedgewick.c" -- (17.8)
                  , doTest "T86_Sedgewick.c" -- (17.10)
                  , doTest "T87_Sedgewick.c" -- (17.11)
                  , doTest "T88_Sedgewick.c" -- (17.12)
                  , doTest "T89_Sedgewick.c" -- (17.13)
                  , doTest "T90_Sedgewick.c" -- (17.15)
                  , doTest "T91.c"
                  , doTest "T92.c"
                  , doTest "T93.c"
                  , doTest "T94.c"
                  , doTest "T95.c"
                  , doTest "T96.c"
                  , doTest "T97.c"
                  , doTest "T98.c"
                  , doTest "T99.c"
                  , doTest "T100.c"
                  , doTest "T101.c"
                  , doTest "T102.c"
                  , doTest "T103.c"
                  , doTest "T104.c"
                  , doTest "T105.c"
                  , doTest "T106.c"
                  , doTest "T107.c"
                  , doTest "T108.c"
                  , doTest "T109.c"
                  , doTest "T110.c"
                  , doTest "T111.c"
                  , doTest "T112.c"
                  , doTest "T113.c"
                  , doTest "T114.c"
                  , doTest "T115.c"
                  , doTest "T116.c"
                  , doTest "T117.c"
                  , doTest "T118.c"
                  , doTest "T119.c"
                  , doTest "T120.c"
                  , doTest "T121.c"
                  , doTest "T122.c"
                  , doTest "T123.c"
                  , doTest "T124.c"
                  , doTest "T125.c"
                  , doTest "T126.c"
                  , doTest "T127.c"
                  , doTest "T128.c"
                  , doTest "T129.c"
                  , doTest "T130.c"
                  , doTest "T131.c"
                  , doTest "T132.c"
                  , doTest "T133.c"
                  , doTest "T134.c"
                  , doTest "T135.c"
                  , doTest "T136.c"
                  , doTest "T137.c"
                  , doTest "T138.c"
                  , doTest "T139.c"
                  , doTest "T140.c"
                  , doTest "T141.c"
                  , doTest "T142.c"
                  , doTest "T143.c" -- Non-trivial function calls.
                  , doTest "T144.c" -- const qualification on function args.
                  , doTest "T145.c" -- Nested anonymous struct with declaration.
                  , doTest "T146.c" -- Nested anonymous struct without declaration.
                  , doTest "T147.c" -- Duplicate definition.
                  , doTest "T148.c" -- Ordering or declarations.
                  ]

doTest s
    = testCase ("Testing " ++ s) $
      do
        callProcess "../Gen" [cases ++ s, "clean"]
        ctr <- readFile (cases ++ s ++ ".ctr")
        case parser ctr of
          Left err -> error $ "Error parsing constraints:\n" ++ err
          Right c  -> do
            r <- solver c
            case r of
                Left err' -> error $ "Error solving constraints:\n" ++ show err'
                Right ctxs -> do
                    expected <- readFile (cases ++ s ++ ".expected")
                    let
                      removeSpaces xs = [ x | x <- xs, not (x `elem` " \n")]
                      actual = writeDecls ctxs
                      actualNoWS = removeSpaces actual
                      expectedNoWS = removeSpaces expected

                    unless (actualNoWS == expectedNoWS)
                           (error $ printResults expected actual)

printResults expect actual
        = show $ text "\n[Expected]\n" <+> text expect <+>
                 text "\n[Inferred]\n" <+> text actual
