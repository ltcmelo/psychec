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

Constraints' Parser

> module Parser.ConstraintParser (module Parser.ConstraintParser, module Text.Parsec) where

> import Data.Functor
> import Data.Functor.Identity

> import Text.Parsec
> import Text.Parsec.Char
> import Text.Parsec.Language
> import Text.Parsec.Token (TokenParser)
> import qualified Text.Parsec.Token as Tk
> import qualified Text.Parsec.Expr as Ex

> import Data.Type
> import Data.Constraints

> import Debug.Trace

A type for parsers

> type Parser a = ParsecT String () Identity a


Top level parsing function

> parser :: String -> Either String Constraint
> parser = either (Left . show) Right . parse constraintParser ""


Constraint parser

> constraintParser :: Parser Constraint
> constraintParser = Ex.buildExpressionParser table ctrParser
>                    where
>                      table = [[ Ex.Infix conjParser Ex.AssocLeft ]]
>                      conjParser = (:&:) <$ comma

> ctrParser :: Parser Constraint
> ctrParser = skipMany space *> choice [
>                      eqParser
>                    , ascriptionParser
>                    , hasParser
>                    , defParser
>                    , scopeParser
>                    , existsParser
>                    , typeDefParser
>                    , isConstExprParser
>                    , staticStorageParser
>                    , eofParser
>                    ]

> eofParser = f <$> eof
>   where f _ = Truth

> eqParser :: Parser Constraint
> eqParser =
>     f <$> typeParser <*> ((Left <$> reservedOp "=") <|> (Right <$> reservedOp ">"))
>         <*> typeParser
>   where
>     f t (Left _) t' = t :=: t'
>     f t (Right _) t' = t :>: t'

> ascriptionParser :: Parser Constraint
> ascriptionParser = build <$> reserved "$typeof$" <*>
>                              (parens nameParser) <*>
>                              reservedOp "="      <*>
>                              typeParser
>                    where
>                      build _ n _ t = n :<-: t

> isConstExprParser :: Parser Constraint
> isConstExprParser = build <$> reserved "$read_only$" <*>
>                         (parens nameParser)
>                    where
>                      build _ n = ReadOnly n

> staticStorageParser :: Parser Constraint
> staticStorageParser = build <$> reserved "$static$" <*>
>                         (parens nameParser)
>                        where
>                         build _ n = Static n

> hasParser :: Parser Constraint
> hasParser = reserved "$has$" *>
>             parens (build <$> typeParser <*>
>                               comma      <*>
>                               nameParser <*>
>                               colon      <*>
>                               typeParser)
>             where
>               build t _ n _ t' = Has t (Field n t')

> defParser :: Parser Constraint
> defParser = build <$> reserved "$def$" <*> nameParser <*>
>                       colon          <*> typeParser <*>
>                       reserved "$in$"  <*> (Truth `option` constraintParser)
>             where
>               build _ n _ t _ ctr = Def n t ctr

> scopeParser :: Parser Constraint
> scopeParser = build <$> brackets (Truth `option` constraintParser)
>              where
>               build c = Scope c

> existsParser :: Parser Constraint
> existsParser = build <$> reserved "$exists$" <*> nameParser <*>
>                          reservedOp "."    <*> (Truth `option` constraintParser)
>                where
>                  build _ n _ ctr = Exists n ctr

> typeDefParser :: Parser Constraint
> typeDefParser = build <$> reserved "$typedef$" <*> typeParser <*>
>                           reserved "$as$"      <*> typeParser
>                 where
>                    build _ n _ t = TypeDef n t


Type parser

> constQualParser :: Parser Ty -> Parser Ty
> constQualParser p = f <$> p <*> (optionMaybe constParser)
>                     where
>                       f t Nothing = t
>                       f t _ = QualTy t Const

> volatileQualParser :: Parser Ty -> Parser Ty
> volatileQualParser p = f <$> p <*> (optionMaybe volatileParser)
>                     where
>                       f t Nothing = t
>                       f t _ = QualTy t Volatile

> typeParser :: Parser Ty
> typeParser = f <$> constQualParser typeParser' <*> (many starParser)
>            where
>             f t ts = foldr (\_ acc -> PtrTy acc) t ts

> typeParser' :: Parser Ty
> typeParser' = f <$> typeParser'' <*> (many starParser)
>              where
>                f t ts = foldr (\ _ ac -> PtrTy ac) t ts

> typeParser'' :: Parser Ty
> typeParser'' = choice [ tyVarParser
>                      , constQualParser (volatileQualParser floatTyParser)
>                      , constQualParser (volatileQualParser intTyParser)
>                      , constQualParser (volatileQualParser tyConParser)
>                      , funTyParser
>                      , constQualParser (volatileQualParser structTyParser)
>                      , constQualParser (volatileQualParser unionTyParser)
>                      , constQualParser (volatileQualParser enumTyParser)
>                     ]

> trivialSpecParser :: Parser Ty
> trivialSpecParser
>   = NamedTy <$> name'
>   where
>     name' = try (reserved "short" >> return (Name "short"))
>               <|> try (reserved "char" >> return (Name "char"))
>               <|> try (reserved "int" >> return (Name "int"))
>               <|> try (reserved "long" >> return (Name "long"))
>               <|> try (reserved "unsigned" >> return (Name "unsigned"))
>               <|> try (reserved "signed" >> return (Name "signed"))
>               <|> try (reserved "double" >> return (Name "double")) -- Because long double.
>               <* skipMany space

> intTyParser :: Parser Ty
> intTyParser
>   = f <$> trivialSpecParser <*> (many trivialSpecParser)
>   where
>     f t ts = foldr (\(NamedTy t') (NamedTy t'') -> NamedTy (Name $ unName t'' ++ " " ++ unName t')) t ts

> floatTyParser :: Parser Ty
> floatTyParser
>   = NamedTy <$> name'
>   where
>     name' = try (reserved "float" >> return (Name "float"))
>               <|> try (reserved "double" >> return (Name "double"))
>               <* skipMany space

> tyVarParser :: Parser Ty
> tyVarParser = VarTy <$> name'
>               where
>                  name' = f <$> string "#alpha"
>                            <*> (show <$> Tk.integer constrLexer)
>                  f x y = Name (x ++ y)

> tyConParser :: Parser Ty
> tyConParser = f <$> (Tk.identifier constrLexer)
>               where
>                 f n = NamedTy (Name n)

> funTyParser :: Parser Ty
> funTyParser = f <$> parens (typeParser `sepBy1` comma)
>               where
>                 f ts = FunTy (last ts) (init ts)

> structTyParser :: Parser Ty
> structTyParser = f <$> reserved "struct" <*>
>                      ((Name "") `option` nameParser)  <*>
>                      ((Left <$> braces (fieldParser `endBy` semi)) <|>
>                       (Right <$> many starParser))
>                where
>                  f _ n (Left fs) = RecTy fs (elabName n)
>                  f _ n (Right ts) = foldr (\ _ ac -> PtrTy ac) (NamedTy (elabName n)) ts
>                  elabName n' = Name ("struct " ++ (unName n'))

> unionTyParser :: Parser Ty
> unionTyParser = f <$> reserved "union" <*>
>                      ((Name "") `option` nameParser)  <*>
>                      ((Left <$> braces (fieldParser `endBy` semi)) <|>
>                       (Right <$> many starParser))
>                where
>                  f _ n (Left fs) = SumTy fs (elabName n)
>                  f _ n (Right ts) = foldr (\ _ ac -> PtrTy ac) (NamedTy (elabName n)) ts
>                  elabName n' = Name ("union " ++ (unName n'))

> enumTyParser :: Parser Ty
> enumTyParser = f <$> reserved "enum" <*>
>                      ((Name "") `option` nameParser)  <*>
>                      ((Left <$> braces (nameParser `endBy` comma)) <|>
>                       (Right <$> many starParser))
>                where
>                  f _ n (Left _) = EnumTy (elabName n)
>                  f _ n (Right ts) = foldr (\ _ ac -> PtrTy ac) (EnumTy (elabName n)) ts
>                  elabName n' = Name ("enum " ++ (unName n'))

> fieldParser :: Parser Field
> fieldParser = flip Field <$> typeParser <*> nameParser

Lexer definition

> constrLexer :: TokenParser st
> constrLexer = Tk.makeTokenParser constrDef

> nameParser :: Parser Name
> nameParser = Name <$> (Tk.identifier constrLexer <|>
>                        Tk.operator constrLexer)

> reserved :: String -> Parser ()
> reserved = Tk.reserved constrLexer

> reservedOp :: String -> Parser ()
> reservedOp = Tk.reservedOp constrLexer

> brackets :: Parser a -> Parser a
> brackets = Tk.brackets constrLexer

> braces :: Parser a -> Parser a
> braces = Tk.braces constrLexer

> parens :: Parser a -> Parser a
> parens = Tk.parens constrLexer

> comma :: Parser ()
> comma = () <$ Tk.comma constrLexer

> semi :: Parser ()
> semi = () <$ Tk.semi constrLexer

> starParser :: Parser ()
> starParser = () <$ (Tk.lexeme constrLexer $ Tk.symbol constrLexer "*")

> constParser :: Parser ()
> constParser = () <$ (Tk.lexeme constrLexer $ Tk.reserved constrLexer "const")

> volatileParser :: Parser ()
> volatileParser = () <$ (Tk.lexeme constrLexer $ Tk.reserved constrLexer "volatile")

> colon :: Parser ()
> colon = () <$ Tk.colon constrLexer

> dot :: Parser ()
> dot = () <$ Tk.dot constrLexer

Constraint language definition

> constrDef :: LanguageDef st
> constrDef = emptyDef {
>     Tk.identStart = letter <|> char '_' <|> char '#'
>   , Tk.reservedOpNames = [":", "=", "->"]
>   , Tk.reservedNames = [
>                        -- Reserved C names we need to distinguish.
>                          "struct"
>                        , "union"
>                        , "enum"
>                        , "unsigned"
>                        , "signed"
>                        , "char"
>                        , "short"
>                        , "int"
>                        , "long"
>                        , "float"
>                        , "double"
>                        , "const"
>                        , "volatile"
>                        -- The surrounding `$'s are to prevend collisions
>                        -- between identifiers in the C program and keywords
>                        -- from our the contraint's language.
>                        , "$exists$"
>                        , "$def$"
>                        , "$in$"
>                        , "$typedef$" -- Not to be confused with C's typedef.
>                        , "$as$"
>                        , "$has$"
>                        , "$typeof$"
>                        , "$read_only$"
>                        , "$static$"
>                        ]
>                      }
