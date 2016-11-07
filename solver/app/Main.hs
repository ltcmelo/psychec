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

module Main where

import Options.Applicative hiding ((<+>))
import System.FilePath
import Solver.ConstraintSolver
import Parser.ConstraintParser hiding (optional)
import Utils.Pretty (pprint, (<+>), text)
import Utils.Writer
import Solver.SolverMonad (TyCtx (..), VarCtx(..))

data Config = Config {
                inputFile  :: FilePath
              , outputFile :: Maybe FilePath
              } deriving Show

config = Config <$> strOption
                      (  long "input-file"
                      <> short 'i'
                      <> metavar "INPUT"
                      <> help "Constraint input file")
                <*> optional
                      (strOption
                         (  long "output-file"
                         <> short 'o'
                         <> metavar "OUTPUT"
                         <> help "Output file"))

opts :: ParserInfo Config
opts = info (config <**> helper)
            ( fullDesc
            <> progDesc "Infer missing typedef's for a constraint in INPUT file"
            <> header "Constraint solver for typedef inference" )


outfile :: Maybe FilePath -> FilePath
outfile Nothing = "./result.txt"
outfile (Just f) = f

execSolver :: Maybe FilePath -> String -> IO ()
execSolver fp s = case parser s of
                    Left err -> putStrLn err
                    Right c  ->
                        do
                          inf <- solver c
                          case inf of
                            Left err'  -> putStrLn err'
                            Right inf' -> do
                                            writeFile (outfile fp)
                                                      (writeCFile inf')

main :: IO ()
main = do
        cfg <- execParser opts
        cont <- readFile (inputFile cfg)
        execSolver (outputFile cfg) cont


