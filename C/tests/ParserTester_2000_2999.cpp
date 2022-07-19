// Copyright (c) 2020/21/22 Leandro T. C. Melo <ltcmelo@gmail.com>
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.

#include "ParserTester.h"

#include "BinderTester.h"

#include "parser/Parser.h"
#include "parser/Unparser.h"

using namespace psy;
using namespace C;

void ParserTester::case2000()
{
    parseStatement("x : 1 ; ",
                   Expectation().AST( { IdentifierLabelStatement,
                                        ExpressionStatement,
                                        IntegerConstantExpression }));
}

void ParserTester::case2001()
{
    parseStatement("x : :",
                   Expectation().diagnostic(Expectation::ErrorOrWarn::Error,
                                               Parser::DiagnosticsReporter::ID_of_ExpectedFIRSTofExpression));
}

void ParserTester::case2002()
{
    parseStatement("goto x ;",
                   Expectation().AST( { GotoStatement }));
}

void ParserTester::case2003()
{
    parseStatement("goto ;",
                   Expectation().diagnostic(Expectation::ErrorOrWarn::Error,
                                               Parser::DiagnosticsReporter::ID_of_ExpectedTokenOfCategoryIdentifier));
}

void ParserTester::case2004()
{
    parseStatement("goto 1 ;",
                   Expectation().diagnostic(Expectation::ErrorOrWarn::Error,
                                               Parser::DiagnosticsReporter::ID_of_ExpectedTokenOfCategoryIdentifier));
}

void ParserTester::case2005()
{
    parseStatement("goto x",
                   Expectation().diagnostic(Expectation::ErrorOrWarn::Error,
                                               Parser::DiagnosticsReporter::ID_of_ExpectedToken));
}

void ParserTester::case2006()
{

}

void ParserTester::case2007()
{

}

void ParserTester::case2008()
{

}

void ParserTester::case2009()
{

}

void ParserTester::case2010()
{

}

void ParserTester::case2011()
{

}

void ParserTester::case2012()
{

}

void ParserTester::case2013()
{

}

void ParserTester::case2014()
{

}

void ParserTester::case2015()
{

}

void ParserTester::case2016()
{

}

void ParserTester::case2017()
{

}

void ParserTester::case2018()
{

}

void ParserTester::case2019()
{

}

void ParserTester::case2020()
{
    parseStatement("{ }",
                   Expectation().AST( { CompoundStatement }));
}

void ParserTester::case2021()
{
    parseStatement("{",
                   Expectation().diagnostic(Expectation::ErrorOrWarn::Error,
                                               Parser::DiagnosticsReporter::ID_of_ExpectedToken));
}

void ParserTester::case2022()
{
    parseStatement("{ / }",
                   Expectation().setErrorCnt(1));
}

void ParserTester::case2023()
{
    parseStatement("{ { } }",
                   Expectation().AST( { CompoundStatement,
                                        CompoundStatement }));
}

void ParserTester::case2024()
{
    parseDeclaration("void x ( ) { }",
                     Expectation().AST( { FunctionDefinition,
                                          BuiltinTypeSpecifier,
                                          FunctionDeclarator,
                                          IdentifierDeclarator,
                                          ParameterSuffix,
                                          CompoundStatement }));
}

void ParserTester::case2025()
{

}

void ParserTester::case2026()
{

}

void ParserTester::case2027()
{

}

void ParserTester::case2028()
{

}

void ParserTester::case2029()
{

}

void ParserTester::case2030()
{

}

void ParserTester::case2031()
{
}

void ParserTester::case2032()
{
}

void ParserTester::case2033()
{
}

void ParserTester::case2034()
{
}

void ParserTester::case2035()
{
}

void ParserTester::case2036()
{
}

void ParserTester::case2037()
{
}

void ParserTester::case2038()
{
}

void ParserTester::case2039()
{
}

void ParserTester::case2040()
{
}

void ParserTester::case2041()
{
}

void ParserTester::case2042()
{
}

void ParserTester::case2043()
{
}

void ParserTester::case2044()
{
}

void ParserTester::case2045()
{
}

void ParserTester::case2046()
{
}

void ParserTester::case2047()
{

}

void ParserTester::case2048()
{

}

void ParserTester::case2049()
{

}

void ParserTester::case2050()
{

}

void ParserTester::case2051()
{

}

void ParserTester::case2052()
{

}

void ParserTester::case2053()
{

}

void ParserTester::case2054()
{

}

void ParserTester::case2055()
{

}

void ParserTester::case2056()
{

}

void ParserTester::case2057()
{

}

void ParserTester::case2058()
{

}

void ParserTester::case2059()
{

}

void ParserTester::case2060()
{

}

void ParserTester::case2061()
{

}

void ParserTester::case2062()
{

}

void ParserTester::case2063()
{
}

void ParserTester::case2064()
{
}

void ParserTester::case2065()
{

}

void ParserTester::case2066()
{

}

void ParserTester::case2067()
{

}

void ParserTester::case2068()
{
}

void ParserTester::case2069()
{
}

void ParserTester::case2070()
{
}

void ParserTester::case2071()
{
}

void ParserTester::case2072()
{
}

void ParserTester::case2073()
{
}

void ParserTester::case2074() {}
void ParserTester::case2075() {}
void ParserTester::case2076() {}
void ParserTester::case2077() {}
void ParserTester::case2078() {}
void ParserTester::case2079() {}
void ParserTester::case2080() {}
void ParserTester::case2081() {}
void ParserTester::case2082() {}
void ParserTester::case2083() {}
void ParserTester::case2084() {}
void ParserTester::case2085() {}
void ParserTester::case2086() {}
void ParserTester::case2087() {}
void ParserTester::case2088() {}
void ParserTester::case2089() {}
void ParserTester::case2090() {}
void ParserTester::case2091() {}
void ParserTester::case2092() {}
void ParserTester::case2093() {}
void ParserTester::case2094() {}
void ParserTester::case2095() {}
void ParserTester::case2096() {}
void ParserTester::case2097() {}
void ParserTester::case2098() {}
void ParserTester::case2099() {}

void ParserTester::case2100()
{
    parseStatement("{ x ++ ; }",
                   Expectation().AST( { CompoundStatement,
                                        ExpressionStatement,
                                        PostIncrementExpression,
                                        IdentifierName }));
}

void ParserTester::case2101()
{
    parseStatement("{ ++ x ; }",
                   Expectation().AST( { CompoundStatement,
                                        ExpressionStatement,
                                        PreIncrementExpression,
                                        IdentifierName }));
}

void ParserTester::case2102()
{
    parseStatement("{ x ( 1 ) ; }",
                   Expectation().AST( { CompoundStatement,
                                        ExpressionStatement,
                                        CallExpression,
                                        IdentifierName,
                                        IntegerConstantExpression }));
}

void ParserTester::case2103()
{
    parseStatement("{ x ( 1 , 2 ) ; }");
}

void ParserTester::case2104()
{
    parseStatement("{ x ( y , 2 ) ; }");
}

void ParserTester::case2105()
{
    parseStatement("{ x ( 1 , 2 ) ; }");
}

void ParserTester::case2106()
{
    parseStatement("{ ( x ( 1 ) ) ; }",
                   Expectation().AST( { CompoundStatement,
                                        ExpressionStatement,
                                        ParenthesizedExpression,
                                        CallExpression,
                                        IdentifierName,
                                        IntegerConstantExpression }));
}

void ParserTester::case2107()
{
    parseStatement("{ ( ( x ( 1 ) ) ) ; }");
}

void ParserTester::case2108()
{
    parseStatement("{ ( x ( 1 , 2 ) ) ; }");
}

void ParserTester::case2109()
{
    parseStatement("{ 'x' ; }");
}

void ParserTester::case2110()
{
    parseStatement("{ \"x\" ; }");
}

void ParserTester::case2111()
{
    parseStatement("{ x ; }");
}

void ParserTester::case2112()
{
    parseStatement("{ 1 ; }");
}

void ParserTester::case2113()
{
    parseStatement("{ 1 ( ; }",
                   Expectation().setErrorCnt(1));
}

void ParserTester::case2114()
{
    parseStatement("{ 'x' ; }");
}

void ParserTester::case2115()
{
    /*
     Despite the parsing similarities between `typeof' and `sizeof',
     mind the difference between this test case and the following:

     `typeof ( x ) y ;'

     This would be a valid declaration-statement, and the `y' doesn't
     belong to the parsing of `typeof': it's the declarator.
     */

    CROSS_REFERENCE_TEST(ParserTester::case0904);

    parseStatement("sizeof ( x ) y ;",
                   Expectation().diagnostic(Expectation::ErrorOrWarn::Error,
                                               Parser::DiagnosticsReporter::ID_of_ExpectedToken));
}

void ParserTester::case2116() {}
void ParserTester::case2117() {}
void ParserTester::case2118() {}
void ParserTester::case2119() {}
void ParserTester::case2120() {}
void ParserTester::case2121() {}
void ParserTester::case2122() {}
void ParserTester::case2123() {}
void ParserTester::case2124() {}
void ParserTester::case2125() {}
void ParserTester::case2126() {}
void ParserTester::case2127() {}
void ParserTester::case2128() {}
void ParserTester::case2129() {}
void ParserTester::case2130() {}
void ParserTester::case2131() {}
void ParserTester::case2132() {}
void ParserTester::case2133() {}
void ParserTester::case2134() {}
void ParserTester::case2135() {}
void ParserTester::case2136() {}
void ParserTester::case2137() {}
void ParserTester::case2138() {}
void ParserTester::case2139() {}
void ParserTester::case2140() {}
void ParserTester::case2141() {}
void ParserTester::case2142() {}
void ParserTester::case2143() {}
void ParserTester::case2144() {}
void ParserTester::case2145() {}
void ParserTester::case2146() {}
void ParserTester::case2147() {}
void ParserTester::case2148() {}
void ParserTester::case2149() {}
void ParserTester::case2150() {}
void ParserTester::case2151() {}
void ParserTester::case2152() {}
void ParserTester::case2153() {}
void ParserTester::case2154() {}
void ParserTester::case2155() {}
void ParserTester::case2156() {}
void ParserTester::case2157() {}
void ParserTester::case2158() {}
void ParserTester::case2159() {}
void ParserTester::case2160() {}
void ParserTester::case2161() {}
void ParserTester::case2162() {}
void ParserTester::case2163() {}
void ParserTester::case2164() {}
void ParserTester::case2165() {}
void ParserTester::case2166() {}
void ParserTester::case2167() {}
void ParserTester::case2168() {}
void ParserTester::case2169() {}
void ParserTester::case2170() {}
void ParserTester::case2171() {}
void ParserTester::case2172() {}
void ParserTester::case2173() {}
void ParserTester::case2174() {}
void ParserTester::case2175() {}
void ParserTester::case2176() {}
void ParserTester::case2177() {}
void ParserTester::case2178() {}
void ParserTester::case2179() {}
void ParserTester::case2180() {}
void ParserTester::case2181() {}
void ParserTester::case2182() {}
void ParserTester::case2183() {}
void ParserTester::case2184() {}
void ParserTester::case2185() {}
void ParserTester::case2186() {}
void ParserTester::case2187() {}
void ParserTester::case2188() {}
void ParserTester::case2189() {}
void ParserTester::case2190() {}
void ParserTester::case2191() {}
void ParserTester::case2192() {}
void ParserTester::case2193() {}
void ParserTester::case2194() {}
void ParserTester::case2195() {}
void ParserTester::case2196() {}
void ParserTester::case2197() {}
void ParserTester::case2198() {}
void ParserTester::case2199() {}

void ParserTester::case2200()
{
    parseStatement("{ _Static_assert ( 1 != 2 , \"x\" ) ; }");
}

void ParserTester::case2201()
{
    parseStatement("{ int x ; }",
                   Expectation().AST( { CompoundStatement,
                                        DeclarationStatement,
                                        VariableAndOrFunctionDeclaration,
                                        BuiltinTypeSpecifier,
                                        IdentifierDeclarator }));
}

void ParserTester::case2202()
{
    parseStatement("{ int x , y ; }",
                   Expectation().AST( { CompoundStatement,
                                        DeclarationStatement,
                                        VariableAndOrFunctionDeclaration,
                                        BuiltinTypeSpecifier,
                                        IdentifierDeclarator,
                                        IdentifierDeclarator }));
}

void ParserTester::case2203()
{
    parseStatement("{ const int x ; }",
                   Expectation().AST( { CompoundStatement,
                                        DeclarationStatement,
                                        VariableAndOrFunctionDeclaration,
                                        ConstQualifier,
                                        BuiltinTypeSpecifier,
                                        IdentifierDeclarator }));
}

void ParserTester::case2204()
{
    parseStatement("{ int * x ; }",
                   Expectation().AST( { CompoundStatement,
                                        DeclarationStatement,
                                        VariableAndOrFunctionDeclaration,
                                        BuiltinTypeSpecifier,
                                        PointerDeclarator,
                                        IdentifierDeclarator }));
}

void ParserTester::case2205()
{
    parseStatement("{ struct x ; }",
                   Expectation().AST( { CompoundStatement,
                                        DeclarationStatement,
                                        StructDeclaration,
                                        StructTypeSpecifier }));
}

void ParserTester::case2206()
{
    parseStatement("{ union x y ; }",
                   Expectation().AST( { CompoundStatement,
                                        DeclarationStatement,
                                        VariableAndOrFunctionDeclaration,
                                        UnionTypeSpecifier,
                                        IdentifierDeclarator }));
}

void ParserTester::case2207()
{
    parseStatement("{ x y ; }",
                   Expectation().AST( { CompoundStatement,
                                        DeclarationStatement,
                                        VariableAndOrFunctionDeclaration,
                                        TypedefName,
                                        IdentifierDeclarator }));
}

void ParserTester::case2208()
{
    parseStatement("{ x y , z ; }",
                   Expectation().AST( { CompoundStatement,
                                        DeclarationStatement,
                                        VariableAndOrFunctionDeclaration,
                                        TypedefName,
                                        IdentifierDeclarator,
                                        IdentifierDeclarator }));
}

void ParserTester::case2209()
{
    parseStatement("{ const x y ; }",
                   Expectation().AST( { CompoundStatement,
                                        DeclarationStatement,
                                        VariableAndOrFunctionDeclaration,
                                        ConstQualifier,
                                        TypedefName,
                                        IdentifierDeclarator }));
}

void ParserTester::case2210()
{
    parseStatement("{ const x * const y ; }",
                   Expectation().AST( { CompoundStatement,
                                        DeclarationStatement,
                                        VariableAndOrFunctionDeclaration,
                                        ConstQualifier,
                                        TypedefName,
                                        PointerDeclarator,
                                        ConstQualifier,
                                        IdentifierDeclarator }));
}

void ParserTester::case2211()
{
    parseStatement("{ x * y ; }",
                   Expectation().AST( { CompoundStatement,
                                        AmbiguousMultiplicationOrPointerDeclaration,
                                        DeclarationStatement,
                                        VariableAndOrFunctionDeclaration,
                                        TypedefName,
                                        PointerDeclarator,
                                        IdentifierDeclarator,
                                        ExpressionStatement,
                                        MultiplyExpression,
                                        IdentifierName,
                                        IdentifierName })
                   .ambiguity("{ x * y ; x * y ; }"));
}

void ParserTester::case2212()
{
    parseStatement("{ x ( y ) ; }",
                   Expectation().AST( { CompoundStatement,
                                        AmbiguousCallOrVariableDeclaration,
                                        DeclarationStatement,
                                        VariableAndOrFunctionDeclaration,
                                        TypedefName,
                                        ParenthesizedDeclarator,
                                        IdentifierDeclarator,
                                        ExpressionStatement,
                                        CallExpression,
                                        IdentifierName,
                                        IdentifierName })
                   .ambiguity("{ x ( y ) ; x ( y ) ; }"));
}

void ParserTester::case2213()
{
    CROSS_REFERENCE_TEST(ParserTester::case1813);

    parseStatement("{ x * y = 0 ; }",
                   Expectation().AST( { CompoundStatement,
                                        DeclarationStatement,
                                        VariableAndOrFunctionDeclaration,
                                        TypedefName,
                                        PointerDeclarator,
                                        IdentifierDeclarator,
                                        ExpressionInitializer,
                                        IntegerConstantExpression }));

}

void ParserTester::case2214() {}
void ParserTester::case2215() {}
void ParserTester::case2216() {}
void ParserTester::case2217() {}
void ParserTester::case2218() {}
void ParserTester::case2219() {}
void ParserTester::case2220() {}
void ParserTester::case2221() {}
void ParserTester::case2222() {}
void ParserTester::case2223() {}
void ParserTester::case2224() {}
void ParserTester::case2225() {}
void ParserTester::case2226() {}
void ParserTester::case2227() {}
void ParserTester::case2228() {}
void ParserTester::case2229() {}
void ParserTester::case2230() {}
void ParserTester::case2231() {}
void ParserTester::case2232() {}
void ParserTester::case2233() {}
void ParserTester::case2234() {}
void ParserTester::case2235() {}
void ParserTester::case2236() {}
void ParserTester::case2237() {}
void ParserTester::case2238() {}
void ParserTester::case2239() {}
void ParserTester::case2240() {}
void ParserTester::case2241() {}
void ParserTester::case2242() {}
void ParserTester::case2243() {}
void ParserTester::case2244() {}
void ParserTester::case2245() {}
void ParserTester::case2246() {}
void ParserTester::case2247() {}
void ParserTester::case2248() {}
void ParserTester::case2249() {}
void ParserTester::case2250() {}
void ParserTester::case2251() {}
void ParserTester::case2252() {}
void ParserTester::case2253() {}
void ParserTester::case2254() {}
void ParserTester::case2255() {}
void ParserTester::case2256() {}
void ParserTester::case2257() {}
void ParserTester::case2258() {}
void ParserTester::case2259() {}
void ParserTester::case2260() {}
void ParserTester::case2261() {}
void ParserTester::case2262() {}
void ParserTester::case2263() {}
void ParserTester::case2264() {}
void ParserTester::case2265() {}
void ParserTester::case2266() {}
void ParserTester::case2267() {}
void ParserTester::case2268() {}
void ParserTester::case2269() {}
void ParserTester::case2270() {}
void ParserTester::case2271() {}
void ParserTester::case2272() {}
void ParserTester::case2273() {}
void ParserTester::case2274() {}
void ParserTester::case2275() {}
void ParserTester::case2276() {}
void ParserTester::case2277() {}
void ParserTester::case2278() {}
void ParserTester::case2279() {}
void ParserTester::case2280() {}
void ParserTester::case2281() {}
void ParserTester::case2282() {}
void ParserTester::case2283() {}
void ParserTester::case2284() {}
void ParserTester::case2285() {}
void ParserTester::case2286() {}
void ParserTester::case2287() {}
void ParserTester::case2288() {}
void ParserTester::case2289() {}
void ParserTester::case2290() {}
void ParserTester::case2291() {}
void ParserTester::case2292() {}
void ParserTester::case2293() {}
void ParserTester::case2294() {}
void ParserTester::case2295() {}
void ParserTester::case2296() {}
void ParserTester::case2297() {}
void ParserTester::case2298() {}
void ParserTester::case2299() {}

void ParserTester::case2300()
{
    parseStatement("if ( x ) ;");
}

void ParserTester::case2301()
{
    parseStatement("if ( x ) { } ");
}

void ParserTester::case2302()
{
    parseStatement("if ( x ( ) ) { }");
}

void ParserTester::case2303()
{
    parseStatement("if ( x ) y ;");
}

void ParserTester::case2304()
{
    parseStatement("if ( x ) { y ; }");
}

void ParserTester::case2305()
{
    parseStatement("if ( x ++ ) { }");
}

void ParserTester::case2306()
{
    parseStatement("if ( ++ x ) { }");
}

void ParserTester::case2307()
{
    parseStatement("if ( 1 ) { } else ;");
}

void ParserTester::case2308()
{
    parseStatement("if ( 1 ) ; else ;");
}

void ParserTester::case2309()
{
    parseStatement("if ( 1 ) ; else { }");
}

void ParserTester::case2310()
{
    parseStatement("if ( 1 ) { } else { }");
}

void ParserTester::case2311()
{
    parseStatement("if ( 1 ) { x ; } else { y ; }");
}

void ParserTester::case2312()
{
    parseStatement("if ( 1 ) else { }",
                   Expectation().diagnostic(Expectation::ErrorOrWarn::Error,
                                               Parser::DiagnosticsReporter::ID_of_ExpectedFIRSTofExpression));
}

void ParserTester::case2313()
{
    parseStatement("if ( 1 ) 2 else { }",
                   Expectation().diagnostic(Expectation::ErrorOrWarn::Error,
                                               Parser::DiagnosticsReporter::ID_of_ExpectedToken));
}

void ParserTester::case2314()
{
    parseStatement("if else x ;",
                   Expectation().diagnostic(Expectation::ErrorOrWarn::Error,
                                               Parser::DiagnosticsReporter::ID_of_ExpectedToken));
}

void ParserTester::case2315()
{
    parseStatement("if ( 1 ) { } else ",
                   Expectation().diagnostic(Expectation::ErrorOrWarn::Error,
                                               Parser::DiagnosticsReporter::ID_of_ExpectedFIRSTofExpression));
}

void ParserTester::case2316()
{
    parseStatement("if ( 1 ) { } else (",
                   Expectation().diagnostic(Expectation::ErrorOrWarn::Error,
                                               Parser::DiagnosticsReporter::ID_of_ExpectedFIRSTofExpression));
}

void ParserTester::case2317()
{
    parseStatement("if ( 1 ) if ( 2 ) x ;");
}

void ParserTester::case2318()
{
    parseStatement("if ( 1 ) if ( 2 ) x ; else y ;");
}

void ParserTester::case2319()
{
    parseStatement("if ( 1 ) { if ( 2 ) x ; } else y ;");
}

void ParserTester::case2320()
{
    parseStatement("if ( 1 ) { if ( 2 ) x ; } else { y ; }");
}

void ParserTester::case2321() {}
void ParserTester::case2322() {}
void ParserTester::case2323() {}
void ParserTester::case2324() {}
void ParserTester::case2325() {}
void ParserTester::case2326() {}
void ParserTester::case2327() {}
void ParserTester::case2328() {}
void ParserTester::case2329() {}
void ParserTester::case2330() {}
void ParserTester::case2331() {}
void ParserTester::case2332() {}
void ParserTester::case2333() {}
void ParserTester::case2334() {}
void ParserTester::case2335() {}
void ParserTester::case2336() {}
void ParserTester::case2337() {}
void ParserTester::case2338() {}
void ParserTester::case2339() {}
void ParserTester::case2340() {}
void ParserTester::case2341() {}
void ParserTester::case2342() {}
void ParserTester::case2343() {}
void ParserTester::case2344() {}
void ParserTester::case2345() {}
void ParserTester::case2346() {}
void ParserTester::case2347() {}
void ParserTester::case2348() {}
void ParserTester::case2349() {}

void ParserTester::case2350()
{
    parseStatement("switch ( 1 ) { }");
}

void ParserTester::case2351()
{
    parseStatement("switch ( x ) { case 2 : y ( ) ; }");
}

void ParserTester::case2352()
{
    parseStatement(R"(
                   switch ( x ) {
                       case 1 : y ;
                       case 2 : z ( ) ;
                   }
                   )");
}

void ParserTester::case2353()
{
    parseStatement("switch ( x ) case 2 : y ( ) ;");
}

void ParserTester::case2354()
{
    parseStatement(R"(
                   switch ( x ) {
                       case 1 : y ;
                       default : z ( ) ;
                   }
                   )");
}

void ParserTester::case2355()
{
    parseStatement(R"(
                   switch ( x ) {
                       default : y ;
                   }
                   )");
}

void ParserTester::case2356()
{
    parseStatement("case 1 : x ;",
                   Expectation().diagnostic(Expectation::ErrorOrWarn::Error,
                                               Parser::DiagnosticsReporter::ID_of_UnexpectedCaseLabelOutsideSwitch));
}

void ParserTester::case2357()
{
    parseStatement("default : x ;",
                   Expectation().diagnostic(Expectation::ErrorOrWarn::Error,
                                               Parser::DiagnosticsReporter::ID_of_UnexpectedDefaultLabelOutsideSwitch));
}

void ParserTester::case2358()
{
}

void ParserTester::case2359() {}
void ParserTester::case2360() {}
void ParserTester::case2361() {}
void ParserTester::case2362() {}
void ParserTester::case2363() {}
void ParserTester::case2364() {}
void ParserTester::case2365() {}
void ParserTester::case2366() {}
void ParserTester::case2367() {}
void ParserTester::case2368() {}
void ParserTester::case2369() {}
void ParserTester::case2370() {}
void ParserTester::case2371() {}
void ParserTester::case2372() {}
void ParserTester::case2373() {}
void ParserTester::case2374() {}
void ParserTester::case2375() {}
void ParserTester::case2376() {}
void ParserTester::case2377() {}
void ParserTester::case2378() {}
void ParserTester::case2379() {}
void ParserTester::case2380() {}
void ParserTester::case2381() {}
void ParserTester::case2382() {}
void ParserTester::case2383() {}
void ParserTester::case2384() {}
void ParserTester::case2385() {}
void ParserTester::case2386() {}
void ParserTester::case2387() {}
void ParserTester::case2388() {}
void ParserTester::case2389() {}
void ParserTester::case2390() {}
void ParserTester::case2391() {}
void ParserTester::case2392() {}
void ParserTester::case2393() {}
void ParserTester::case2394() {}
void ParserTester::case2395() {}
void ParserTester::case2396() {}
void ParserTester::case2397() {}
void ParserTester::case2398() {}
void ParserTester::case2399() {}

void ParserTester::case2400()
{
    parseStatement("while ( 1 ) { }");
}

void ParserTester::case2401()
{
    parseStatement("while ( 1 ) ;");
}

void ParserTester::case2402()
{
    parseStatement("while ( * x ++ != '\\0' ) ;");
}

void ParserTester::case2403()
{
    parseStatement("while ( 1 ) x = y ;");
}

void ParserTester::case2404()
{
    parseStatement("do { } while ( 1 ) ;");
}

void ParserTester::case2405()
{
    parseStatement("do { } while ( 1 )",
                   Expectation().diagnostic(Expectation::ErrorOrWarn::Error,
                                               Parser::DiagnosticsReporter::ID_of_ExpectedToken));
}

void ParserTester::case2406()
{
    parseStatement("do { } while ( ) ;",
                   Expectation().diagnostic(Expectation::ErrorOrWarn::Error,
                                               Parser::DiagnosticsReporter::ID_of_ExpectedFIRSTofExpression));
}

void ParserTester::case2407()
{
    parseStatement("while ( )",
                   Expectation().diagnostic(Expectation::ErrorOrWarn::Error,
                                               Parser::DiagnosticsReporter::ID_of_ExpectedFIRSTofExpression));
}

void ParserTester::case2408()
{
    parseStatement("do ; while ( 1 )",
                   Expectation().diagnostic(Expectation::ErrorOrWarn::Error,
                                               Parser::DiagnosticsReporter::ID_of_ExpectedToken));
}

void ParserTester::case2409()
{
    parseStatement("do x ( ) ; while ( y / z % 0 ) ;");
}

void ParserTester::case2410()
{
    parseStatement("do x ; while ( y ) ;");
}

void ParserTester::case2411()
{
    parseStatement("do ; while ( 1 ) ;");
}

void ParserTester::case2412()
{
    parseStatement("while ( * x ++ != 0 ) y ;");
}

void ParserTester::case2413()
{
    parseStatement("while ( x != y ) z ;");
}

void ParserTester::case2414()
{
    parseStatement("while ( ! x ( ) ) { y ; z ; }");
}

void ParserTester::case2415()
{
        parseStatement("while ( * x ++ != '0' ) ;");
}

void ParserTester::case2416() {}
void ParserTester::case2417() {}
void ParserTester::case2418() {}
void ParserTester::case2419() {}
void ParserTester::case2420() {}
void ParserTester::case2421() {}
void ParserTester::case2422() {}
void ParserTester::case2423() {}
void ParserTester::case2424() {}
void ParserTester::case2425() {}
void ParserTester::case2426() {}
void ParserTester::case2427() {}
void ParserTester::case2428() {}
void ParserTester::case2429() {}
void ParserTester::case2430() {}
void ParserTester::case2431() {}
void ParserTester::case2432() {}
void ParserTester::case2433() {}
void ParserTester::case2434() {}
void ParserTester::case2435() {}
void ParserTester::case2436() {}
void ParserTester::case2437() {}
void ParserTester::case2438() {}
void ParserTester::case2439() {}
void ParserTester::case2440() {}
void ParserTester::case2441() {}
void ParserTester::case2442() {}
void ParserTester::case2443() {}
void ParserTester::case2444() {}
void ParserTester::case2445() {}
void ParserTester::case2446() {}
void ParserTester::case2447() {}
void ParserTester::case2448() {}
void ParserTester::case2449() {}

void ParserTester::case2450()
{
    parseStatement("for ( ; ; ) 1 ;",
                   Expectation().AST( { ForStatement,
                                        ExpressionStatement,
                                        ExpressionStatement,
                                        IntegerConstantExpression }));
}

void ParserTester::case2451()
{
    parseStatement("for ( ; ) ;",
                   Expectation().diagnostic(Expectation::ErrorOrWarn::Error,
                                               Parser::DiagnosticsReporter::ID_of_ExpectedFIRSTofExpression));
}

void ParserTester::case2452()
{
    parseStatement("for ( ; ; ) { }",
                   Expectation().AST( { ForStatement,
                                        ExpressionStatement,
                                        CompoundStatement }));
}

void ParserTester::case2453()
{
    parseStatement("for ( ; ; )",
                   Expectation().diagnostic(Expectation::ErrorOrWarn::Error,
                                               Parser::DiagnosticsReporter::ID_of_ExpectedFIRSTofExpression));
}

void ParserTester::case2454()
{
    parseStatement("for ( x ; y ; z ) { }",
                   Expectation().AST( { ForStatement,
                                        ExpressionStatement,
                                        IdentifierName,
                                        IdentifierName,
                                        IdentifierName,
                                        CompoundStatement }));
}

void ParserTester::case2455()
{
    parseStatement("for ( int x ; y ; z ) { }",
                   Expectation().AST( { ForStatement,
                                        DeclarationStatement,
                                        VariableAndOrFunctionDeclaration,
                                        BuiltinTypeSpecifier,
                                        IdentifierDeclarator,
                                        IdentifierName,
                                        IdentifierName,
                                        CompoundStatement }));
}

void ParserTester::case2456()
{
    parseStatement("for ( x y ; 1 ; 2 ) { }",
                   Expectation().AST( { ForStatement,
                                        DeclarationStatement,
                                        VariableAndOrFunctionDeclaration,
                                        TypedefName,
                                        IdentifierDeclarator,
                                        IntegerConstantExpression,
                                        IntegerConstantExpression,
                                        CompoundStatement }));
}

void ParserTester::case2457()
{
    parseStatement("for ( x y ; 1 ; ) { }",
                   Expectation().AST( { ForStatement,
                                        DeclarationStatement,
                                        VariableAndOrFunctionDeclaration,
                                        TypedefName,
                                        IdentifierDeclarator,
                                        IntegerConstantExpression,
                                        CompoundStatement }));
}

void ParserTester::case2458()
{
    parseStatement("for ( x y ; ; ) { }",
                   Expectation().AST( { ForStatement,
                                        DeclarationStatement,
                                        VariableAndOrFunctionDeclaration,
                                        TypedefName,
                                        IdentifierDeclarator,
                                        CompoundStatement }));
}

void ParserTester::case2459()
{
    parseStatement("for ( 1 ; x != 0 ; ++ y ) { }",
                   Expectation().AST( { ForStatement,
                                        ExpressionStatement,
                                        IntegerConstantExpression,
                                        NotEqualsExpression,
                                        IdentifierName,
                                        IntegerConstantExpression,
                                        PreIncrementExpression,
                                        IdentifierName,
                                        CompoundStatement }));
}
void ParserTester::case2460()
{
    parseStatement("for ( int x = 1 ; y != 0 ; ++ z ) { }",
                   Expectation().AST( { ForStatement,
                                        DeclarationStatement,
                                        VariableAndOrFunctionDeclaration,
                                        BuiltinTypeSpecifier,
                                        IdentifierDeclarator,
                                        ExpressionInitializer,
                                        IntegerConstantExpression,
                                        NotEqualsExpression,
                                        IdentifierName,
                                        IntegerConstantExpression,
                                        PreIncrementExpression,
                                        IdentifierName,
                                        CompoundStatement }));
}

void ParserTester::case2461() {}
void ParserTester::case2462() {}
void ParserTester::case2463() {}
void ParserTester::case2464() {}
void ParserTester::case2465() {}
void ParserTester::case2466() {}
void ParserTester::case2467() {}
void ParserTester::case2468() {}
void ParserTester::case2469() {}
void ParserTester::case2470() {}
void ParserTester::case2471() {}
void ParserTester::case2472() {}
void ParserTester::case2473() {}
void ParserTester::case2474() {}
void ParserTester::case2475() {}
void ParserTester::case2476() {}
void ParserTester::case2477() {}
void ParserTester::case2478() {}
void ParserTester::case2479() {}
void ParserTester::case2480() {}
void ParserTester::case2481() {}
void ParserTester::case2482() {}
void ParserTester::case2483() {}
void ParserTester::case2484() {}
void ParserTester::case2485() {}
void ParserTester::case2486() {}
void ParserTester::case2487() {}
void ParserTester::case2488() {}
void ParserTester::case2489() {}
void ParserTester::case2490() {}
void ParserTester::case2491() {}
void ParserTester::case2492() {}
void ParserTester::case2493() {}
void ParserTester::case2494() {}
void ParserTester::case2495() {}
void ParserTester::case2496() {}
void ParserTester::case2497() {}
void ParserTester::case2498() {}
void ParserTester::case2499() {}

void ParserTester::case2500()
{
    parseStatement("break ;",
                   Expectation().diagnostic(Expectation::ErrorOrWarn::Error,
                                               Parser::DiagnosticsReporter::ID_of_UnexpectedBreakOutsideSwitchOrLoop));
}

void ParserTester::case2501()
{
    parseStatement("continue ;",
                   Expectation().diagnostic(Expectation::ErrorOrWarn::Error,
                                               Parser::DiagnosticsReporter::ID_of_UnexpectedContinueOutsideLoop));
}

void ParserTester::case2502()
{
    parseStatement("return ;");
}

void ParserTester::case2503()
{
    parseStatement("return x ;");
}

void ParserTester::case2504()
{
    parseStatement("return 0 ;");
}

void ParserTester::case2505()
{
    parseStatement("return x ( ) ;");
}

void ParserTester::case2506()
{
    parseStatement(R"(
                   for ( ; ; )
                       break ;
                   )");
}

void ParserTester::case2507()
{
    parseStatement(R"(
                   for ( ; ; )
                   {
                       break ;
                   }
                   )");
}

void ParserTester::case2508()
{
    parseStatement(R"(
                   for ( ; ; )
                       continue ;
                   )");
}

void ParserTester::case2509()
{
    parseStatement(R"(
                   while ( 1 )
                       break ;
                   )");
}

void ParserTester::case2510()
{
    parseStatement(R"(
                   while ( 1 )
                   {
                       break ;
                   }
                   )");
}

void ParserTester::case2511()
{
    parseStatement(R"(
                   for ( ; ; )
                   {
                       continue ;
                   }
                   )");
}

void ParserTester::case2512()
{
    parseStatement(R"(
                   switch ( x )
                   {
                       case 1 :
                       case 2 : break ;
                   }
                   )");
}

void ParserTester::case2513()
{
    parseStatement(R"(
                   switch ( x )
                   {
                       case 1 : break ;
                       case 2 : break ;
                   }
                   )");
}

void ParserTester::case2514()
{
    parseStatement(R"(
                   switch ( x )
                   {
                       case 1 :
                       case 2 : break ;
                       default : break ;
                   }
                   )");
}

void ParserTester::case2515()
{
    parse("break",
          Expectation().diagnostic(Expectation::ErrorOrWarn::Error,
                                      Parser::DiagnosticsReporter::ID_of_ExpectedFIRSTofDirectDeclarator)
                       .diagnostic(Expectation::ErrorOrWarn::Warn,
                                      Parser::DiagnosticsReporter::ID_of_ExpectedTypeSpecifier));
}

void ParserTester::case2516()
{
    parse("continue ;",
          Expectation().diagnostic(Expectation::ErrorOrWarn::Error,
                                      Parser::DiagnosticsReporter::ID_of_ExpectedFIRSTofDirectDeclarator)
                       .diagnostic(Expectation::ErrorOrWarn::Warn,
                                      Parser::DiagnosticsReporter::ID_of_ExpectedTypeSpecifier));

}

void ParserTester::case2517()
{
    parseStatement(R"(
                   for ( ; ; ) {
                       switch ( 1 ) {
                           default :
                               continue ;
                       }
                   }
                   )");
}

void ParserTester::case2518()
{
    parseStatement(R"(
                   for ( ; ; ) {
                       switch ( 1 ) {
                           default :
                               if ( x ) continue ;
                       }
                   }
                   )");
}

void ParserTester::case2519()
{
    parseStatement(R"(
                   switch ( x ) {
                       break;
                   }
                   )");
}

void ParserTester::case2520()
{
    parseStatement(R"(
                   switch ( 0 ) {
                       case 1 : ;
                       default:
                           for ( ; ; ) {
                               case 2 : ;
                           }
                   }
                   )");
}

void ParserTester::case2521() {}
void ParserTester::case2522() {}
void ParserTester::case2523() {}
void ParserTester::case2524() {}
void ParserTester::case2525() {}
void ParserTester::case2526() {}
void ParserTester::case2527() {}
void ParserTester::case2528() {}
void ParserTester::case2529() {}
void ParserTester::case2530() {}
void ParserTester::case2531() {}
void ParserTester::case2532() {}
void ParserTester::case2533() {}
void ParserTester::case2534() {}
void ParserTester::case2535() {}
void ParserTester::case2536() {}
void ParserTester::case2537() {}
void ParserTester::case2538() {}
void ParserTester::case2539() {}
void ParserTester::case2540() {}
void ParserTester::case2541() {}
void ParserTester::case2542() {}
void ParserTester::case2543() {}
void ParserTester::case2544() {}
void ParserTester::case2545() {}
void ParserTester::case2546() {}
void ParserTester::case2547() {}
void ParserTester::case2548() {}
void ParserTester::case2549() {}
void ParserTester::case2550() {}
void ParserTester::case2551() {}
void ParserTester::case2552() {}
void ParserTester::case2553() {}
void ParserTester::case2554() {}
void ParserTester::case2555() {}
void ParserTester::case2556() {}
void ParserTester::case2557() {}
void ParserTester::case2558() {}
void ParserTester::case2559() {}
void ParserTester::case2560() {}
void ParserTester::case2561() {}
void ParserTester::case2562() {}
void ParserTester::case2563() {}
void ParserTester::case2564() {}
void ParserTester::case2565() {}
void ParserTester::case2566() {}
void ParserTester::case2567() {}
void ParserTester::case2568() {}
void ParserTester::case2569() {}
void ParserTester::case2570() {}
void ParserTester::case2571() {}
void ParserTester::case2572() {}
void ParserTester::case2573() {}
void ParserTester::case2574() {}
void ParserTester::case2575() {}
void ParserTester::case2576() {}
void ParserTester::case2577() {}
void ParserTester::case2578() {}
void ParserTester::case2579() {}
void ParserTester::case2580() {}
void ParserTester::case2581() {}
void ParserTester::case2582() {}
void ParserTester::case2583() {}
void ParserTester::case2584() {}
void ParserTester::case2585() {}
void ParserTester::case2586() {}
void ParserTester::case2587() {}
void ParserTester::case2588() {}
void ParserTester::case2589() {}
void ParserTester::case2590() {}
void ParserTester::case2591() {}
void ParserTester::case2592() {}
void ParserTester::case2593() {}
void ParserTester::case2594() {}
void ParserTester::case2595() {}
void ParserTester::case2596() {}
void ParserTester::case2597() {}
void ParserTester::case2598() {}
void ParserTester::case2599() {}

void ParserTester::case2600()
{
    parseStatement(R"(
                   asm ( "nop" );
                   )");
}

void ParserTester::case2601()
{
    parseStatement(R"(
                   asm ( "nop"
                         "nop" );
                   )");
}

void ParserTester::case2602()
{
    parseStatement(R"(
                   asm ( "nop" )
                   )",
                   Expectation().diagnostic(Expectation::ErrorOrWarn::Error,
                                               Parser::DiagnosticsReporter::ID_of_ExpectedToken));
}

void ParserTester::case2603()
{
    parseStatement(R"(
                   asm ( )
                   )",
                   Expectation().diagnostic(Expectation::ErrorOrWarn::Error,
                                               Parser::DiagnosticsReporter::ID_of_ExpectedTokenOfCategoryStringLiteral));
}

void ParserTester::case2604()
{
    // From https://www.felixcloutier.com/documents/gcc-asm.html.

    parseStatement(R"(
                   asm ( "movq %0, %0" : "+rm" ( foo ) ) ;
                   )");
}

void ParserTester::case2605()
{
    // From https://www.felixcloutier.com/documents/gcc-asm.html.

    parseStatement(R"(
                   asm ( "addl %0, %1" : "+r" ( foo ) : "g" ( bar ) ) ;
                   )");
}

void ParserTester::case2606()
{
    // From https://www.felixcloutier.com/documents/gcc-asm.html.

    parseStatement(R"(
                   asm ( "lfence " :  :  : "memory" ) ;
                   )");
}

void ParserTester::case2607()
{
    parseStatement(R"(
                   asm ( "movl $123 , %0 \n  "
                         "addl %1 , %0"
                         : "=&r" ( foo )
                         : "r" ( bar ) ) ;
                   )");
}

void ParserTester::case2608()
{
    parseStatement(R"(
                   asm goto ( "jmp %l0\n"
                              :
                              :
                              :
                              : x , y );
                   )");
}

void ParserTester::case2609()
{
    parseStatement(R"(
                   asm goto ( "jmp %l0\n"
                              :
                              :
                              :
                              : x );
                   )");
}

void ParserTester::case2610()
{
    // From https://www.felixcloutier.com/documents/gcc-asm.html.

    parseStatement(R"(
                   asm volatile ( "syscall" : "=a" (ret_val) : : "rcx" , "r11" ) ;
                   )");
}

void ParserTester::case2611()
{
    // From https://www.felixcloutier.com/documents/gcc-asm.html.

    parseStatement(R"(
                   asm ( "imulq %[rhs]"
                         : "=a" (*lo) , "=d" (*hi)
                         : [ lhs ] "0" ( left ) , [ rhs ] "rm" ( right )) ;
                   )");
}

void ParserTester::case2612()
{
    parseStatement(R"(
                   asm ( "mov %[e], %[d]"
                      : [ d ] "=rm" ( d )
                      : [ e ] "rm" ( * e ) );
                   )");
}

void ParserTester::case2613() {}
void ParserTester::case2614() {}
void ParserTester::case2615() {}
void ParserTester::case2616() {}
void ParserTester::case2617() {}
void ParserTester::case2618() {}
void ParserTester::case2619() {}
void ParserTester::case2620() {}
void ParserTester::case2621() {}
void ParserTester::case2622() {}
void ParserTester::case2623() {}
void ParserTester::case2624() {}
void ParserTester::case2625() {}
void ParserTester::case2626() {}
void ParserTester::case2627() {}
void ParserTester::case2628() {}
void ParserTester::case2629() {}
void ParserTester::case2630() {}
void ParserTester::case2631() {}
void ParserTester::case2632() {}
void ParserTester::case2633() {}
void ParserTester::case2634() {}
void ParserTester::case2635() {}
void ParserTester::case2636() {}
void ParserTester::case2637() {}
void ParserTester::case2638() {}
void ParserTester::case2639() {}
void ParserTester::case2640() {}
void ParserTester::case2641() {}
void ParserTester::case2642() {}
void ParserTester::case2643() {}
void ParserTester::case2644() {}
void ParserTester::case2645() {}
void ParserTester::case2646() {}
void ParserTester::case2647() {}
void ParserTester::case2648() {}
void ParserTester::case2649() {}
void ParserTester::case2650() {}
void ParserTester::case2651() {}
void ParserTester::case2652() {}
void ParserTester::case2653() {}
void ParserTester::case2654() {}
void ParserTester::case2655() {}
void ParserTester::case2656() {}
void ParserTester::case2657() {}
void ParserTester::case2658() {}
void ParserTester::case2659() {}
void ParserTester::case2660() {}
void ParserTester::case2661() {}
void ParserTester::case2662() {}
void ParserTester::case2663() {}
void ParserTester::case2664() {}
void ParserTester::case2665() {}
void ParserTester::case2666() {}
void ParserTester::case2667() {}
void ParserTester::case2668() {}
void ParserTester::case2669() {}
void ParserTester::case2670() {}
void ParserTester::case2671() {}
void ParserTester::case2672() {}
void ParserTester::case2673() {}
void ParserTester::case2674() {}
void ParserTester::case2675() {}
void ParserTester::case2676() {}
void ParserTester::case2677() {}
void ParserTester::case2678() {}
void ParserTester::case2679() {}
void ParserTester::case2680() {}
void ParserTester::case2681() {}
void ParserTester::case2682() {}
void ParserTester::case2683() {}
void ParserTester::case2684() {}
void ParserTester::case2685() {}
void ParserTester::case2686() {}
void ParserTester::case2687() {}
void ParserTester::case2688() {}
void ParserTester::case2689() {}
void ParserTester::case2690() {}
void ParserTester::case2691() {}
void ParserTester::case2692() {}
void ParserTester::case2693() {}
void ParserTester::case2694() {}
void ParserTester::case2695() {}
void ParserTester::case2696() {}
void ParserTester::case2697() {}
void ParserTester::case2698() {}
void ParserTester::case2699() {}

void ParserTester::case2700() {}
void ParserTester::case2701() {}
void ParserTester::case2702() {}
void ParserTester::case2703() {}
void ParserTester::case2704() {}
void ParserTester::case2705() {}
void ParserTester::case2706() {}
void ParserTester::case2707() {}
void ParserTester::case2708() {}
void ParserTester::case2709() {}
void ParserTester::case2710() {}
void ParserTester::case2711() {}
void ParserTester::case2712() {}
void ParserTester::case2713() {}
void ParserTester::case2714() {}
void ParserTester::case2715() {}
void ParserTester::case2716() {}
void ParserTester::case2717() {}
void ParserTester::case2718() {}
void ParserTester::case2719() {}
void ParserTester::case2720() {}
void ParserTester::case2721() {}
void ParserTester::case2722() {}
void ParserTester::case2723() {}
void ParserTester::case2724() {}
void ParserTester::case2725() {}
void ParserTester::case2726() {}
void ParserTester::case2727() {}
void ParserTester::case2728() {}
void ParserTester::case2729() {}
void ParserTester::case2730() {}
void ParserTester::case2731() {}
void ParserTester::case2732() {}
void ParserTester::case2733() {}
void ParserTester::case2734() {}
void ParserTester::case2735() {}
void ParserTester::case2736() {}
void ParserTester::case2737() {}
void ParserTester::case2738() {}
void ParserTester::case2739() {}
void ParserTester::case2740() {}
void ParserTester::case2741() {}
void ParserTester::case2742() {}
void ParserTester::case2743() {}
void ParserTester::case2744() {}
void ParserTester::case2745() {}
void ParserTester::case2746() {}
void ParserTester::case2747() {}
void ParserTester::case2748() {}
void ParserTester::case2749() {}
void ParserTester::case2750() {}
void ParserTester::case2751() {}
void ParserTester::case2752() {}
void ParserTester::case2753() {}
void ParserTester::case2754() {}
void ParserTester::case2755() {}
void ParserTester::case2756() {}
void ParserTester::case2757() {}
void ParserTester::case2758() {}
void ParserTester::case2759() {}
void ParserTester::case2760() {}
void ParserTester::case2761() {}
void ParserTester::case2762() {}
void ParserTester::case2763() {}
void ParserTester::case2764() {}
void ParserTester::case2765() {}
void ParserTester::case2766() {}
void ParserTester::case2767() {}
void ParserTester::case2768() {}
void ParserTester::case2769() {}
void ParserTester::case2770() {}
void ParserTester::case2771() {}
void ParserTester::case2772() {}
void ParserTester::case2773() {}
void ParserTester::case2774() {}
void ParserTester::case2775() {}
void ParserTester::case2776() {}
void ParserTester::case2777() {}
void ParserTester::case2778() {}
void ParserTester::case2779() {}
void ParserTester::case2780() {}
void ParserTester::case2781() {}
void ParserTester::case2782() {}
void ParserTester::case2783() {}
void ParserTester::case2784() {}
void ParserTester::case2785() {}
void ParserTester::case2786() {}
void ParserTester::case2787() {}
void ParserTester::case2788() {}
void ParserTester::case2789() {}
void ParserTester::case2790() {}
void ParserTester::case2791() {}
void ParserTester::case2792() {}
void ParserTester::case2793() {}
void ParserTester::case2794() {}
void ParserTester::case2795() {}
void ParserTester::case2796() {}
void ParserTester::case2797() {}
void ParserTester::case2798() {}
void ParserTester::case2799() {}

void ParserTester::case2800() {}
void ParserTester::case2801() {}
void ParserTester::case2802() {}
void ParserTester::case2803() {}
void ParserTester::case2804() {}
void ParserTester::case2805() {}
void ParserTester::case2806() {}
void ParserTester::case2807() {}
void ParserTester::case2808() {}
void ParserTester::case2809() {}
void ParserTester::case2810() {}
void ParserTester::case2811() {}
void ParserTester::case2812() {}
void ParserTester::case2813() {}
void ParserTester::case2814() {}
void ParserTester::case2815() {}
void ParserTester::case2816() {}
void ParserTester::case2817() {}
void ParserTester::case2818() {}
void ParserTester::case2819() {}
void ParserTester::case2820() {}
void ParserTester::case2821() {}
void ParserTester::case2822() {}
void ParserTester::case2823() {}
void ParserTester::case2824() {}
void ParserTester::case2825() {}
void ParserTester::case2826() {}
void ParserTester::case2827() {}
void ParserTester::case2828() {}
void ParserTester::case2829() {}
void ParserTester::case2830() {}
void ParserTester::case2831() {}
void ParserTester::case2832() {}
void ParserTester::case2833() {}
void ParserTester::case2834() {}
void ParserTester::case2835() {}
void ParserTester::case2836() {}
void ParserTester::case2837() {}
void ParserTester::case2838() {}
void ParserTester::case2839() {}
void ParserTester::case2840() {}
void ParserTester::case2841() {}
void ParserTester::case2842() {}
void ParserTester::case2843() {}
void ParserTester::case2844() {}
void ParserTester::case2845() {}
void ParserTester::case2846() {}
void ParserTester::case2847() {}
void ParserTester::case2848() {}
void ParserTester::case2849() {}
void ParserTester::case2850() {}
void ParserTester::case2851() {}
void ParserTester::case2852() {}
void ParserTester::case2853() {}
void ParserTester::case2854() {}
void ParserTester::case2855() {}
void ParserTester::case2856() {}
void ParserTester::case2857() {}
void ParserTester::case2858() {}
void ParserTester::case2859() {}
void ParserTester::case2860() {}
void ParserTester::case2861() {}
void ParserTester::case2862() {}
void ParserTester::case2863() {}
void ParserTester::case2864() {}
void ParserTester::case2865() {}
void ParserTester::case2866() {}
void ParserTester::case2867() {}
void ParserTester::case2868() {}
void ParserTester::case2869() {}
void ParserTester::case2870() {}
void ParserTester::case2871() {}
void ParserTester::case2872() {}
void ParserTester::case2873() {}
void ParserTester::case2874() {}
void ParserTester::case2875() {}
void ParserTester::case2876() {}
void ParserTester::case2877() {}
void ParserTester::case2878() {}
void ParserTester::case2879() {}
void ParserTester::case2880() {}
void ParserTester::case2881() {}
void ParserTester::case2882() {}
void ParserTester::case2883() {}
void ParserTester::case2884() {}
void ParserTester::case2885() {}
void ParserTester::case2886() {}
void ParserTester::case2887() {}
void ParserTester::case2888() {}
void ParserTester::case2889() {}
void ParserTester::case2890() {}
void ParserTester::case2891() {}
void ParserTester::case2892() {}
void ParserTester::case2893() {}
void ParserTester::case2894() {}
void ParserTester::case2895() {}
void ParserTester::case2896() {}
void ParserTester::case2897() {}
void ParserTester::case2898() {}
void ParserTester::case2899() {}

void ParserTester::case2900() {}
void ParserTester::case2901() {}
void ParserTester::case2902() {}
void ParserTester::case2903() {}
void ParserTester::case2904() {}
void ParserTester::case2905() {}
void ParserTester::case2906() {}
void ParserTester::case2907() {}
void ParserTester::case2908() {}
void ParserTester::case2909() {}
void ParserTester::case2910() {}
void ParserTester::case2911() {}
void ParserTester::case2912() {}
void ParserTester::case2913() {}
void ParserTester::case2914() {}
void ParserTester::case2915() {}
void ParserTester::case2916() {}
void ParserTester::case2917() {}
void ParserTester::case2918() {}
void ParserTester::case2919() {}
void ParserTester::case2920() {}
void ParserTester::case2921() {}
void ParserTester::case2922() {}
void ParserTester::case2923() {}
void ParserTester::case2924() {}
void ParserTester::case2925() {}
void ParserTester::case2926() {}
void ParserTester::case2927() {}
void ParserTester::case2928() {}
void ParserTester::case2929() {}
void ParserTester::case2930() {}
void ParserTester::case2931() {}
void ParserTester::case2932() {}
void ParserTester::case2933() {}
void ParserTester::case2934() {}
void ParserTester::case2935() {}
void ParserTester::case2936() {}
void ParserTester::case2937() {}
void ParserTester::case2938() {}
void ParserTester::case2939() {}
void ParserTester::case2940() {}
void ParserTester::case2941() {}
void ParserTester::case2942() {}
void ParserTester::case2943() {}
void ParserTester::case2944() {}
void ParserTester::case2945() {}
void ParserTester::case2946() {}
void ParserTester::case2947() {}
void ParserTester::case2948() {}
void ParserTester::case2949() {}
void ParserTester::case2950() {}
void ParserTester::case2951() {}
void ParserTester::case2952() {}
void ParserTester::case2953() {}
void ParserTester::case2954() {}
void ParserTester::case2955() {}
void ParserTester::case2956() {}
void ParserTester::case2957() {}
void ParserTester::case2958() {}
void ParserTester::case2959() {}
void ParserTester::case2960() {}
void ParserTester::case2961() {}
void ParserTester::case2962() {}
void ParserTester::case2963() {}
void ParserTester::case2964() {}
void ParserTester::case2965() {}
void ParserTester::case2966() {}
void ParserTester::case2967() {}
void ParserTester::case2968() {}
void ParserTester::case2969() {}
void ParserTester::case2970() {}
void ParserTester::case2971() {}
void ParserTester::case2972() {}
void ParserTester::case2973() {}
void ParserTester::case2974() {}
void ParserTester::case2975() {}
void ParserTester::case2976() {}
void ParserTester::case2977() {}
void ParserTester::case2978() {}
void ParserTester::case2979() {}
void ParserTester::case2980() {}
void ParserTester::case2981() {}
void ParserTester::case2982() {}
void ParserTester::case2983() {}
void ParserTester::case2984() {}
void ParserTester::case2985() {}
void ParserTester::case2986() {}
void ParserTester::case2987() {}
void ParserTester::case2988() {}
void ParserTester::case2989() {}
void ParserTester::case2990() {}
void ParserTester::case2991() {}
void ParserTester::case2992() {}
void ParserTester::case2993() {}
void ParserTester::case2994() {}
void ParserTester::case2995() {}
void ParserTester::case2996() {}
void ParserTester::case2997() {}
void ParserTester::case2998() {}
void ParserTester::case2999() {}
