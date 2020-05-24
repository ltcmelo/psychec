-- Copyright (c) 2018 Leandro T. C. Melo (ltcmelo@gmail.com)
-- License: GPLv3
-- This implementation focus readability and formalism.

import Data.List


data E = E (Char, Char) deriving (Eq, Ord, Show)

data Td = Td Char  deriving (Eq, Ord, Show)

data Vd = Vd (Char, Char) deriving (Eq, Ord, Show)

data S = S_e E
       | S_td Td
       | S_vd Vd
       | E_or_Vd E Vd
       deriving (Eq, Ord, Show)

data P = P [S] deriving (Eq, Ord, Show)

sOfP :: P -> [S]
sOfP (P xs) = xs


parseCore :: String -> (P, [Char], [Char])
parseCore _ =
  (P [ (S_td (Td 'A')),
       (S_vd (Vd ('A', 'x'))),
       (E_or_Vd (E ('A', 'y')) (Vd ('A', 'y'))),
       (S_e (E ('x', 'y')))
     ],
   [],
   [])

parse :: String -> P
parse s =
  let
    (p_abg, tyset, valset) = parseCore s
    p = foldl go [] (sOfP p_abg)

    go p' (E_or_Vd e@(E (x, y)) vd@(Vd (t, _))) =
      if x `elem` valset && y `elem` valset
      then p' ++ [S_e e]
      else if t `elem` tyset
           then p' ++ [S_vd vd]
           else p' ++ (decideAdhoc e vd)
    go p' s = p' ++ [s]
  in P p

decideAdhoc :: E -> Vd -> [S]
decideAdhoc _ _ = []

main :: IO ()
main = do
  let ast = parse "..."
  putStrLn "final AST"
  putStrLn (show ast)
