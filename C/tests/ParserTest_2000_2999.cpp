// Copyright (c) 2020/21 Leandro T. C. Melo <ltcmelo@gmail.com>
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

#include "ParserTest.h"

#include "BinderTest.h"

#include "parser/Parser.h"
#include "parser/Unparser.h"

using namespace psy;
using namespace C;

void ParserTest::case2000()
{
    parseStatement("x : 1 ; ",
                   Expectation().AST( { IdentifierLabelStatement,
                                        ExpressionStatement,
                                        IntegerConstantExpression }));
}

void ParserTest::case2001()
{
    parseStatement("x : :",
                   Expectation().addDiagnostic(Expectation::ErrorOrWarn::Error,
                                               Parser::DiagnosticsReporter::ID_of_ExpectedFIRSTofExpression));
}

void ParserTest::case2002()
{
    parseStatement("goto x ;",
                   Expectation().AST( { GotoStatement }));
}

void ParserTest::case2003()
{
    parseStatement("goto ;",
                   Expectation().addDiagnostic(Expectation::ErrorOrWarn::Error,
                                               Parser::DiagnosticsReporter::ID_of_ExpectedTokenOfCategoryIdentifier));
}

void ParserTest::case2004()
{
    parseStatement("goto 1 ;",
                   Expectation().addDiagnostic(Expectation::ErrorOrWarn::Error,
                                               Parser::DiagnosticsReporter::ID_of_ExpectedTokenOfCategoryIdentifier));
}

void ParserTest::case2005()
{
    parseStatement("goto x",
                   Expectation().addDiagnostic(Expectation::ErrorOrWarn::Error,
                                               Parser::DiagnosticsReporter::ID_of_ExpectedToken));
}

void ParserTest::case2006()
{

}

void ParserTest::case2007()
{

}

void ParserTest::case2008()
{

}

void ParserTest::case2009()
{

}

void ParserTest::case2010()
{

}

void ParserTest::case2011()
{

}

void ParserTest::case2012()
{

}

void ParserTest::case2013()
{

}

void ParserTest::case2014()
{

}

void ParserTest::case2015()
{

}

void ParserTest::case2016()
{

}

void ParserTest::case2017()
{

}

void ParserTest::case2018()
{

}

void ParserTest::case2019()
{

}

void ParserTest::case2020()
{
    parseStatement("{ }",
                   Expectation().AST( { CompoundStatement }));
}

void ParserTest::case2021()
{
    parseStatement("{",
                   Expectation().addDiagnostic(Expectation::ErrorOrWarn::Error,
                                               Parser::DiagnosticsReporter::ID_of_ExpectedToken));
}

void ParserTest::case2022()
{
    parseStatement("{ / }",
                   Expectation().setErrorCnt(1));
}

void ParserTest::case2023()
{
    parseStatement("{ { } }",
                   Expectation().AST( { CompoundStatement,
                                        CompoundStatement }));
}

void ParserTest::case2024()
{
    parseDeclaration("void x ( ) { }",
                     Expectation().AST( { FunctionDefinition,
                                          BuiltinTypeSpecifier,
                                          FunctionDeclarator,
                                          IdentifierDeclarator,
                                          ParameterSuffix,
                                          CompoundStatement }));
}

void ParserTest::case2025()
{

}

void ParserTest::case2026()
{

}

void ParserTest::case2027()
{

}

void ParserTest::case2028()
{

}

void ParserTest::case2029()
{

}

void ParserTest::case2030()
{

}

void ParserTest::case2031()
{
}

void ParserTest::case2032()
{
}

void ParserTest::case2033()
{
}

void ParserTest::case2034()
{
}

void ParserTest::case2035()
{
}

void ParserTest::case2036()
{
}

void ParserTest::case2037()
{
}

void ParserTest::case2038()
{
}

void ParserTest::case2039()
{
}

void ParserTest::case2040()
{
}

void ParserTest::case2041()
{
}

void ParserTest::case2042()
{
}

void ParserTest::case2043()
{
}

void ParserTest::case2044()
{
}

void ParserTest::case2045()
{
}

void ParserTest::case2046()
{
}

void ParserTest::case2047()
{

}

void ParserTest::case2048()
{

}

void ParserTest::case2049()
{

}

void ParserTest::case2050()
{

}

void ParserTest::case2051()
{

}

void ParserTest::case2052()
{

}

void ParserTest::case2053()
{

}

void ParserTest::case2054()
{

}

void ParserTest::case2055()
{

}

void ParserTest::case2056()
{

}

void ParserTest::case2057()
{

}

void ParserTest::case2058()
{

}

void ParserTest::case2059()
{

}

void ParserTest::case2060()
{

}

void ParserTest::case2061()
{

}

void ParserTest::case2062()
{

}

void ParserTest::case2063()
{
}

void ParserTest::case2064()
{
}

void ParserTest::case2065()
{

}

void ParserTest::case2066()
{

}

void ParserTest::case2067()
{

}

void ParserTest::case2068()
{
}

void ParserTest::case2069()
{
}

void ParserTest::case2070()
{
}

void ParserTest::case2071()
{
}

void ParserTest::case2072()
{
}

void ParserTest::case2073()
{
}

void ParserTest::case2074() {}
void ParserTest::case2075() {}
void ParserTest::case2076() {}
void ParserTest::case2077() {}
void ParserTest::case2078() {}
void ParserTest::case2079() {}
void ParserTest::case2080() {}
void ParserTest::case2081() {}
void ParserTest::case2082() {}
void ParserTest::case2083() {}
void ParserTest::case2084() {}
void ParserTest::case2085() {}
void ParserTest::case2086() {}
void ParserTest::case2087() {}
void ParserTest::case2088() {}
void ParserTest::case2089() {}
void ParserTest::case2090() {}
void ParserTest::case2091() {}
void ParserTest::case2092() {}
void ParserTest::case2093() {}
void ParserTest::case2094() {}
void ParserTest::case2095() {}
void ParserTest::case2096() {}
void ParserTest::case2097() {}
void ParserTest::case2098() {}
void ParserTest::case2099() {}

void ParserTest::case2100()
{
    parseStatement("{ x ++ ; }",
                   Expectation().AST( { CompoundStatement,
                                        ExpressionStatement,
                                        PostIncrementExpression,
                                        IdentifierName }));
}

void ParserTest::case2101()
{
    parseStatement("{ ++ x ; }",
                   Expectation().AST( { CompoundStatement,
                                        ExpressionStatement,
                                        PreIncrementExpression,
                                        IdentifierName }));
}

void ParserTest::case2102()
{
    parseStatement("{ x ( 1 ) ; }",
                   Expectation().AST( { CompoundStatement,
                                        ExpressionStatement,
                                        CallExpression,
                                        IdentifierName,
                                        IntegerConstantExpression }));
}

void ParserTest::case2103()
{
    parseStatement("{ x ( 1 , 2 ) ; }");
}

void ParserTest::case2104()
{
    parseStatement("{ x ( y , 2 ) ; }");
}

void ParserTest::case2105()
{
    parseStatement("{ x ( 1 , 2 ) ; }");
}

void ParserTest::case2106()
{
    parseStatement("{ ( x ( 1 ) ) ; }",
                   Expectation().AST( { CompoundStatement,
                                        ExpressionStatement,
                                        ParenthesizedExpression,
                                        CallExpression,
                                        IdentifierName,
                                        IntegerConstantExpression }));
}

void ParserTest::case2107()
{
    parseStatement("{ ( ( x ( 1 ) ) ) ; }");
}

void ParserTest::case2108()
{
    parseStatement("{ ( x ( 1 , 2 ) ) ; }");
}

void ParserTest::case2109()
{
    parseStatement("{ 'x' ; }");
}

void ParserTest::case2110()
{
    parseStatement("{ \"x\" ; }");
}

void ParserTest::case2111()
{
    parseStatement("{ x ; }");
}

void ParserTest::case2112()
{
    parseStatement("{ 1 ; }");
}

void ParserTest::case2113()
{
    parseStatement("{ 1 ( ; }",
                   Expectation().setErrorCnt(1));
}

void ParserTest::case2114()
{
    parseStatement("{ 'x' ; }");
}

void ParserTest::case2115()
{
    /*
     Despite the parsing similarities between `typeof' and `sizeof',
     mind the difference between this test case and the following:

     `typeof ( x ) y ;'

     This would be a valid declaration-statement, and the `y' doesn't
     belong to the parsing of `typeof': it's the declarator.
     */

    CROSS_REFERENCE_TEST(ParserTest::case0904);

    parseStatement("sizeof ( x ) y ;",
                   Expectation().addDiagnostic(Expectation::ErrorOrWarn::Error,
                                               Parser::DiagnosticsReporter::ID_of_ExpectedToken));
}

void ParserTest::case2116() {}
void ParserTest::case2117() {}
void ParserTest::case2118() {}
void ParserTest::case2119() {}
void ParserTest::case2120() {}
void ParserTest::case2121() {}
void ParserTest::case2122() {}
void ParserTest::case2123() {}
void ParserTest::case2124() {}
void ParserTest::case2125() {}
void ParserTest::case2126() {}
void ParserTest::case2127() {}
void ParserTest::case2128() {}
void ParserTest::case2129() {}
void ParserTest::case2130() {}
void ParserTest::case2131() {}
void ParserTest::case2132() {}
void ParserTest::case2133() {}
void ParserTest::case2134() {}
void ParserTest::case2135() {}
void ParserTest::case2136() {}
void ParserTest::case2137() {}
void ParserTest::case2138() {}
void ParserTest::case2139() {}
void ParserTest::case2140() {}
void ParserTest::case2141() {}
void ParserTest::case2142() {}
void ParserTest::case2143() {}
void ParserTest::case2144() {}
void ParserTest::case2145() {}
void ParserTest::case2146() {}
void ParserTest::case2147() {}
void ParserTest::case2148() {}
void ParserTest::case2149() {}
void ParserTest::case2150() {}
void ParserTest::case2151() {}
void ParserTest::case2152() {}
void ParserTest::case2153() {}
void ParserTest::case2154() {}
void ParserTest::case2155() {}
void ParserTest::case2156() {}
void ParserTest::case2157() {}
void ParserTest::case2158() {}
void ParserTest::case2159() {}
void ParserTest::case2160() {}
void ParserTest::case2161() {}
void ParserTest::case2162() {}
void ParserTest::case2163() {}
void ParserTest::case2164() {}
void ParserTest::case2165() {}
void ParserTest::case2166() {}
void ParserTest::case2167() {}
void ParserTest::case2168() {}
void ParserTest::case2169() {}
void ParserTest::case2170() {}
void ParserTest::case2171() {}
void ParserTest::case2172() {}
void ParserTest::case2173() {}
void ParserTest::case2174() {}
void ParserTest::case2175() {}
void ParserTest::case2176() {}
void ParserTest::case2177() {}
void ParserTest::case2178() {}
void ParserTest::case2179() {}
void ParserTest::case2180() {}
void ParserTest::case2181() {}
void ParserTest::case2182() {}
void ParserTest::case2183() {}
void ParserTest::case2184() {}
void ParserTest::case2185() {}
void ParserTest::case2186() {}
void ParserTest::case2187() {}
void ParserTest::case2188() {}
void ParserTest::case2189() {}
void ParserTest::case2190() {}
void ParserTest::case2191() {}
void ParserTest::case2192() {}
void ParserTest::case2193() {}
void ParserTest::case2194() {}
void ParserTest::case2195() {}
void ParserTest::case2196() {}
void ParserTest::case2197() {}
void ParserTest::case2198() {}
void ParserTest::case2199() {}

void ParserTest::case2200()
{
    parseStatement("{ _Static_assert ( 1 != 2 , \"x\" ) ; }");
}

void ParserTest::case2201()
{
    parseStatement("{ int x ; }",
                   Expectation().AST( { CompoundStatement,
                                        DeclarationStatement,
                                        VariableAndOrFunctionDeclaration,
                                        BuiltinTypeSpecifier,
                                        IdentifierDeclarator }));
}

void ParserTest::case2202()
{
    parseStatement("{ int x , y ; }",
                   Expectation().AST( { CompoundStatement,
                                        DeclarationStatement,
                                        VariableAndOrFunctionDeclaration,
                                        BuiltinTypeSpecifier,
                                        IdentifierDeclarator,
                                        IdentifierDeclarator }));
}

void ParserTest::case2203()
{
    parseStatement("{ const int x ; }",
                   Expectation().AST( { CompoundStatement,
                                        DeclarationStatement,
                                        VariableAndOrFunctionDeclaration,
                                        ConstQualifier,
                                        BuiltinTypeSpecifier,
                                        IdentifierDeclarator }));
}

void ParserTest::case2204()
{
    parseStatement("{ int * x ; }",
                   Expectation().AST( { CompoundStatement,
                                        DeclarationStatement,
                                        VariableAndOrFunctionDeclaration,
                                        BuiltinTypeSpecifier,
                                        PointerDeclarator,
                                        IdentifierDeclarator }));
}

void ParserTest::case2205()
{
    parseStatement("{ struct x ; }",
                   Expectation().AST( { CompoundStatement,
                                        DeclarationStatement,
                                        IncompleteDeclaration,
                                        StructTypeSpecifier }));
}

void ParserTest::case2206()
{
    parseStatement("{ union x y ; }",
                   Expectation().AST( { CompoundStatement,
                                        DeclarationStatement,
                                        VariableAndOrFunctionDeclaration,
                                        UnionTypeSpecifier,
                                        IdentifierDeclarator }));
}

void ParserTest::case2207()
{
    parseStatement("{ x y ; }",
                   Expectation().AST( { CompoundStatement,
                                        DeclarationStatement,
                                        VariableAndOrFunctionDeclaration,
                                        TypedefName,
                                        IdentifierDeclarator }));
}

void ParserTest::case2208()
{
    parseStatement("{ x y , z ; }",
                   Expectation().AST( { CompoundStatement,
                                        DeclarationStatement,
                                        VariableAndOrFunctionDeclaration,
                                        TypedefName,
                                        IdentifierDeclarator,
                                        IdentifierDeclarator }));
}

void ParserTest::case2209()
{
    parseStatement("{ const x y ; }",
                   Expectation().AST( { CompoundStatement,
                                        DeclarationStatement,
                                        VariableAndOrFunctionDeclaration,
                                        ConstQualifier,
                                        TypedefName,
                                        IdentifierDeclarator }));
}

void ParserTest::case2210()
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

void ParserTest::case2211()
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
                   .replicateAmbiguity("{ x * y ; x * y ; }"));
}

void ParserTest::case2212()
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
                   .replicateAmbiguity("{ x ( y ) ; x ( y ) ; }"));
}

void ParserTest::case2213() {}
void ParserTest::case2214() {}
void ParserTest::case2215() {}
void ParserTest::case2216() {}
void ParserTest::case2217() {}
void ParserTest::case2218() {}
void ParserTest::case2219() {}
void ParserTest::case2220() {}
void ParserTest::case2221() {}
void ParserTest::case2222() {}
void ParserTest::case2223() {}
void ParserTest::case2224() {}
void ParserTest::case2225() {}
void ParserTest::case2226() {}
void ParserTest::case2227() {}
void ParserTest::case2228() {}
void ParserTest::case2229() {}
void ParserTest::case2230() {}
void ParserTest::case2231() {}
void ParserTest::case2232() {}
void ParserTest::case2233() {}
void ParserTest::case2234() {}
void ParserTest::case2235() {}
void ParserTest::case2236() {}
void ParserTest::case2237() {}
void ParserTest::case2238() {}
void ParserTest::case2239() {}
void ParserTest::case2240() {}
void ParserTest::case2241() {}
void ParserTest::case2242() {}
void ParserTest::case2243() {}
void ParserTest::case2244() {}
void ParserTest::case2245() {}
void ParserTest::case2246() {}
void ParserTest::case2247() {}
void ParserTest::case2248() {}
void ParserTest::case2249() {}
void ParserTest::case2250() {}
void ParserTest::case2251() {}
void ParserTest::case2252() {}
void ParserTest::case2253() {}
void ParserTest::case2254() {}
void ParserTest::case2255() {}
void ParserTest::case2256() {}
void ParserTest::case2257() {}
void ParserTest::case2258() {}
void ParserTest::case2259() {}
void ParserTest::case2260() {}
void ParserTest::case2261() {}
void ParserTest::case2262() {}
void ParserTest::case2263() {}
void ParserTest::case2264() {}
void ParserTest::case2265() {}
void ParserTest::case2266() {}
void ParserTest::case2267() {}
void ParserTest::case2268() {}
void ParserTest::case2269() {}
void ParserTest::case2270() {}
void ParserTest::case2271() {}
void ParserTest::case2272() {}
void ParserTest::case2273() {}
void ParserTest::case2274() {}
void ParserTest::case2275() {}
void ParserTest::case2276() {}
void ParserTest::case2277() {}
void ParserTest::case2278() {}
void ParserTest::case2279() {}
void ParserTest::case2280() {}
void ParserTest::case2281() {}
void ParserTest::case2282() {}
void ParserTest::case2283() {}
void ParserTest::case2284() {}
void ParserTest::case2285() {}
void ParserTest::case2286() {}
void ParserTest::case2287() {}
void ParserTest::case2288() {}
void ParserTest::case2289() {}
void ParserTest::case2290() {}
void ParserTest::case2291() {}
void ParserTest::case2292() {}
void ParserTest::case2293() {}
void ParserTest::case2294() {}
void ParserTest::case2295() {}
void ParserTest::case2296() {}
void ParserTest::case2297() {}
void ParserTest::case2298() {}
void ParserTest::case2299() {}

void ParserTest::case2300()
{
    parseStatement("if ( x ) ;");
}

void ParserTest::case2301()
{
    parseStatement("if ( x ) { } ");
}

void ParserTest::case2302()
{
    parseStatement("if ( x ( ) ) { }");
}

void ParserTest::case2303()
{
    parseStatement("if ( x ) y ;");
}

void ParserTest::case2304()
{
    parseStatement("if ( x ) { y ; }");
}

void ParserTest::case2305()
{
    parseStatement("if ( x ++ ) { }");
}

void ParserTest::case2306()
{
    parseStatement("if ( ++ x ) { }");
}

void ParserTest::case2307()
{
    parseStatement("if ( 1 ) { } else ;");
}

void ParserTest::case2308()
{
    parseStatement("if ( 1 ) ; else ;");
}

void ParserTest::case2309()
{
    parseStatement("if ( 1 ) ; else { }");
}

void ParserTest::case2310()
{
    parseStatement("if ( 1 ) { } else { }");
}

void ParserTest::case2311()
{
    parseStatement("if ( 1 ) { x ; } else { y ; }");
}

void ParserTest::case2312()
{
    parseStatement("if ( 1 ) else { }",
                   Expectation().addDiagnostic(Expectation::ErrorOrWarn::Error,
                                               Parser::DiagnosticsReporter::ID_of_ExpectedFIRSTofExpression));
}

void ParserTest::case2313()
{
    parseStatement("if ( 1 ) 2 else { }",
                   Expectation().addDiagnostic(Expectation::ErrorOrWarn::Error,
                                               Parser::DiagnosticsReporter::ID_of_ExpectedToken));
}

void ParserTest::case2314()
{
    parseStatement("if else x ;",
                   Expectation().addDiagnostic(Expectation::ErrorOrWarn::Error,
                                               Parser::DiagnosticsReporter::ID_of_ExpectedToken));
}

void ParserTest::case2315()
{
    parseStatement("if ( 1 ) { } else ",
                   Expectation().addDiagnostic(Expectation::ErrorOrWarn::Error,
                                               Parser::DiagnosticsReporter::ID_of_ExpectedFIRSTofExpression));
}

void ParserTest::case2316()
{
    parseStatement("if ( 1 ) { } else (",
                   Expectation().addDiagnostic(Expectation::ErrorOrWarn::Error,
                                               Parser::DiagnosticsReporter::ID_of_ExpectedFIRSTofExpression));
}

void ParserTest::case2317()
{
    parseStatement("if ( 1 ) if ( 2 ) x ;");
}

void ParserTest::case2318()
{
    parseStatement("if ( 1 ) if ( 2 ) x ; else y ;");
}

void ParserTest::case2319()
{
    parseStatement("if ( 1 ) { if ( 2 ) x ; } else y ;");
}

void ParserTest::case2320()
{
    parseStatement("if ( 1 ) { if ( 2 ) x ; } else { y ; }");
}

void ParserTest::case2321() {}
void ParserTest::case2322() {}
void ParserTest::case2323() {}
void ParserTest::case2324() {}
void ParserTest::case2325() {}
void ParserTest::case2326() {}
void ParserTest::case2327() {}
void ParserTest::case2328() {}
void ParserTest::case2329() {}
void ParserTest::case2330() {}
void ParserTest::case2331() {}
void ParserTest::case2332() {}
void ParserTest::case2333() {}
void ParserTest::case2334() {}
void ParserTest::case2335() {}
void ParserTest::case2336() {}
void ParserTest::case2337() {}
void ParserTest::case2338() {}
void ParserTest::case2339() {}
void ParserTest::case2340() {}
void ParserTest::case2341() {}
void ParserTest::case2342() {}
void ParserTest::case2343() {}
void ParserTest::case2344() {}
void ParserTest::case2345() {}
void ParserTest::case2346() {}
void ParserTest::case2347() {}
void ParserTest::case2348() {}
void ParserTest::case2349() {}

void ParserTest::case2350()
{
    parseStatement("switch ( 1 ) { }");
}

void ParserTest::case2351()
{
    parseStatement("switch ( x ) { case 2 : y ( ) ; }");
}

void ParserTest::case2352()
{
    parseStatement(R"(
                   switch ( x ) {
                       case 1 : y ;
                       case 2 : z ( ) ;
                   }
                   )");
}

void ParserTest::case2353()
{
    parseStatement("switch ( x ) case 2 : y ( ) ;");
}

void ParserTest::case2354()
{
    parseStatement(R"(
                   switch ( x ) {
                       case 1 : y ;
                       default : z ( ) ;
                   }
                   )");
}

void ParserTest::case2355()
{
    parseStatement(R"(
                   switch ( x ) {
                       default : y ;
                   }
                   )");
}

void ParserTest::case2356()
{
    parseStatement("case 1 : x ;",
                   Expectation().addDiagnostic(Expectation::ErrorOrWarn::Error,
                                               Parser::DiagnosticsReporter::ID_of_UnexpectedCaseLabelOutsideSwitch));
}

void ParserTest::case2357()
{
    parseStatement("default : x ;",
                   Expectation().addDiagnostic(Expectation::ErrorOrWarn::Error,
                                               Parser::DiagnosticsReporter::ID_of_UnexpectedDefaultLabelOutsideSwitch));
}

void ParserTest::case2358()
{
}

void ParserTest::case2359() {}
void ParserTest::case2360() {}
void ParserTest::case2361() {}
void ParserTest::case2362() {}
void ParserTest::case2363() {}
void ParserTest::case2364() {}
void ParserTest::case2365() {}
void ParserTest::case2366() {}
void ParserTest::case2367() {}
void ParserTest::case2368() {}
void ParserTest::case2369() {}
void ParserTest::case2370() {}
void ParserTest::case2371() {}
void ParserTest::case2372() {}
void ParserTest::case2373() {}
void ParserTest::case2374() {}
void ParserTest::case2375() {}
void ParserTest::case2376() {}
void ParserTest::case2377() {}
void ParserTest::case2378() {}
void ParserTest::case2379() {}
void ParserTest::case2380() {}
void ParserTest::case2381() {}
void ParserTest::case2382() {}
void ParserTest::case2383() {}
void ParserTest::case2384() {}
void ParserTest::case2385() {}
void ParserTest::case2386() {}
void ParserTest::case2387() {}
void ParserTest::case2388() {}
void ParserTest::case2389() {}
void ParserTest::case2390() {}
void ParserTest::case2391() {}
void ParserTest::case2392() {}
void ParserTest::case2393() {}
void ParserTest::case2394() {}
void ParserTest::case2395() {}
void ParserTest::case2396() {}
void ParserTest::case2397() {}
void ParserTest::case2398() {}
void ParserTest::case2399() {}

void ParserTest::case2400()
{
    parseStatement("while ( 1 ) { }");
}

void ParserTest::case2401()
{
    parseStatement("while ( 1 ) ;");
}

void ParserTest::case2402()
{
    parseStatement("while ( * x ++ != '\\0' ) ;");
}

void ParserTest::case2403()
{
    parseStatement("while ( 1 ) x = y ;");
}

void ParserTest::case2404()
{
    parseStatement("do { } while ( 1 ) ;");
}

void ParserTest::case2405()
{
    parseStatement("do { } while ( 1 )",
                   Expectation().addDiagnostic(Expectation::ErrorOrWarn::Error,
                                               Parser::DiagnosticsReporter::ID_of_ExpectedToken));
}

void ParserTest::case2406()
{
    parseStatement("do { } while ( ) ;",
                   Expectation().addDiagnostic(Expectation::ErrorOrWarn::Error,
                                               Parser::DiagnosticsReporter::ID_of_ExpectedFIRSTofExpression));
}

void ParserTest::case2407()
{
    parseStatement("while ( )",
                   Expectation().addDiagnostic(Expectation::ErrorOrWarn::Error,
                                               Parser::DiagnosticsReporter::ID_of_ExpectedFIRSTofExpression));
}

void ParserTest::case2408()
{
    parseStatement("do ; while ( 1 )",
                   Expectation().addDiagnostic(Expectation::ErrorOrWarn::Error,
                                               Parser::DiagnosticsReporter::ID_of_ExpectedToken));
}

void ParserTest::case2409()
{
    parseStatement("do x ( ) ; while ( y / z % 0 ) ;");
}

void ParserTest::case2410()
{
    parseStatement("do x ; while ( y ) ;");
}

void ParserTest::case2411()
{
    parseStatement("do ; while ( 1 ) ;");
}

void ParserTest::case2412()
{
    parseStatement("while ( * x ++ != 0 ) y ;");
}

void ParserTest::case2413()
{
    parseStatement("while ( x != y ) z ;");
}

void ParserTest::case2414()
{
    parseStatement("while ( ! x ( ) ) { y ; z ; }");
}

void ParserTest::case2415()
{
        parseStatement("while ( * x ++ != '0' ) ;");
}

void ParserTest::case2416() {}
void ParserTest::case2417() {}
void ParserTest::case2418() {}
void ParserTest::case2419() {}
void ParserTest::case2420() {}
void ParserTest::case2421() {}
void ParserTest::case2422() {}
void ParserTest::case2423() {}
void ParserTest::case2424() {}
void ParserTest::case2425() {}
void ParserTest::case2426() {}
void ParserTest::case2427() {}
void ParserTest::case2428() {}
void ParserTest::case2429() {}
void ParserTest::case2430() {}
void ParserTest::case2431() {}
void ParserTest::case2432() {}
void ParserTest::case2433() {}
void ParserTest::case2434() {}
void ParserTest::case2435() {}
void ParserTest::case2436() {}
void ParserTest::case2437() {}
void ParserTest::case2438() {}
void ParserTest::case2439() {}
void ParserTest::case2440() {}
void ParserTest::case2441() {}
void ParserTest::case2442() {}
void ParserTest::case2443() {}
void ParserTest::case2444() {}
void ParserTest::case2445() {}
void ParserTest::case2446() {}
void ParserTest::case2447() {}
void ParserTest::case2448() {}
void ParserTest::case2449() {}

void ParserTest::case2450()
{
    parseStatement("for ( ; ; ) 1 ;",
                   Expectation().AST( { ForStatement,
                                        ExpressionStatement,
                                        ExpressionStatement,
                                        IntegerConstantExpression }));
}

void ParserTest::case2451()
{
    parseStatement("for ( ; ) ;",
                   Expectation().addDiagnostic(Expectation::ErrorOrWarn::Error,
                                               Parser::DiagnosticsReporter::ID_of_ExpectedFIRSTofExpression));
}

void ParserTest::case2452()
{
    parseStatement("for ( ; ; ) { }",
                   Expectation().AST( { ForStatement,
                                        ExpressionStatement,
                                        CompoundStatement }));
}

void ParserTest::case2453()
{
    parseStatement("for ( ; ; )",
                   Expectation().addDiagnostic(Expectation::ErrorOrWarn::Error,
                                               Parser::DiagnosticsReporter::ID_of_ExpectedFIRSTofExpression));
}

void ParserTest::case2454()
{
    parseStatement("for ( x ; y ; z ) { }",
                   Expectation().AST( { ForStatement,
                                        ExpressionStatement,
                                        IdentifierName,
                                        IdentifierName,
                                        IdentifierName,
                                        CompoundStatement }));
}

void ParserTest::case2455()
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

void ParserTest::case2456()
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

void ParserTest::case2457()
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

void ParserTest::case2458()
{
    parseStatement("for ( x y ; ; ) { }",
                   Expectation().AST( { ForStatement,
                                        DeclarationStatement,
                                        VariableAndOrFunctionDeclaration,
                                        TypedefName,
                                        IdentifierDeclarator,
                                        CompoundStatement }));
}

void ParserTest::case2459()
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
void ParserTest::case2460()
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

void ParserTest::case2461() {}
void ParserTest::case2462() {}
void ParserTest::case2463() {}
void ParserTest::case2464() {}
void ParserTest::case2465() {}
void ParserTest::case2466() {}
void ParserTest::case2467() {}
void ParserTest::case2468() {}
void ParserTest::case2469() {}
void ParserTest::case2470() {}
void ParserTest::case2471() {}
void ParserTest::case2472() {}
void ParserTest::case2473() {}
void ParserTest::case2474() {}
void ParserTest::case2475() {}
void ParserTest::case2476() {}
void ParserTest::case2477() {}
void ParserTest::case2478() {}
void ParserTest::case2479() {}
void ParserTest::case2480() {}
void ParserTest::case2481() {}
void ParserTest::case2482() {}
void ParserTest::case2483() {}
void ParserTest::case2484() {}
void ParserTest::case2485() {}
void ParserTest::case2486() {}
void ParserTest::case2487() {}
void ParserTest::case2488() {}
void ParserTest::case2489() {}
void ParserTest::case2490() {}
void ParserTest::case2491() {}
void ParserTest::case2492() {}
void ParserTest::case2493() {}
void ParserTest::case2494() {}
void ParserTest::case2495() {}
void ParserTest::case2496() {}
void ParserTest::case2497() {}
void ParserTest::case2498() {}
void ParserTest::case2499() {}

void ParserTest::case2500()
{
    parseStatement("break ;",
                   Expectation().addDiagnostic(Expectation::ErrorOrWarn::Error,
                                               Parser::DiagnosticsReporter::ID_of_UnexpectedBreakOutsideSwitchOrLoop));
}

void ParserTest::case2501()
{
    parseStatement("continue ;",
                   Expectation().addDiagnostic(Expectation::ErrorOrWarn::Error,
                                               Parser::DiagnosticsReporter::ID_of_UnexpectedContinueOutsideLoop));
}

void ParserTest::case2502()
{
    parseStatement("return ;");
}

void ParserTest::case2503()
{
    parseStatement("return x ;");
}

void ParserTest::case2504()
{
    parseStatement("return 0 ;");
}

void ParserTest::case2505()
{
    parseStatement("return x ( ) ;");
}

void ParserTest::case2506()
{
    parseStatement(R"(
                   for ( ; ; )
                       break ;
                   )");
}

void ParserTest::case2507()
{
    parseStatement(R"(
                   for ( ; ; )
                   {
                       break ;
                   }
                   )");
}

void ParserTest::case2508()
{
    parseStatement(R"(
                   for ( ; ; )
                       continue ;
                   )");
}

void ParserTest::case2509()
{
    parseStatement(R"(
                   while ( 1 )
                       break ;
                   )");
}

void ParserTest::case2510()
{
    parseStatement(R"(
                   while ( 1 )
                   {
                       break ;
                   }
                   )");
}

void ParserTest::case2511()
{
    parseStatement(R"(
                   for ( ; ; )
                   {
                       continue ;
                   }
                   )");
}

void ParserTest::case2512()
{
    parseStatement(R"(
                   switch ( x )
                   {
                       case 1 :
                       case 2 : break ;
                   }
                   )");
}

void ParserTest::case2513()
{
    parseStatement(R"(
                   switch ( x )
                   {
                       case 1 : break ;
                       case 2 : break ;
                   }
                   )");
}

void ParserTest::case2514()
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

void ParserTest::case2515()
{
    parse("break",
          Expectation().addDiagnostic(Expectation::ErrorOrWarn::Error,
                                      Parser::DiagnosticsReporter::ID_of_ExpectedFIRSTofDirectDeclarator)
                       .addDiagnostic(Expectation::ErrorOrWarn::Warn,
                                      Parser::DiagnosticsReporter::ID_of_ExpectedTypeSpecifier));
}

void ParserTest::case2516()
{
    parse("continue ;",
          Expectation().addDiagnostic(Expectation::ErrorOrWarn::Error,
                                      Parser::DiagnosticsReporter::ID_of_ExpectedFIRSTofDirectDeclarator)
                       .addDiagnostic(Expectation::ErrorOrWarn::Warn,
                                      Parser::DiagnosticsReporter::ID_of_ExpectedTypeSpecifier));

}

void ParserTest::case2517()
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

void ParserTest::case2518()
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

void ParserTest::case2519()
{
    parseStatement(R"(
                   switch ( x ) {
                       break;
                   }
                   )");
}

void ParserTest::case2520()
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

void ParserTest::case2521() {}
void ParserTest::case2522() {}
void ParserTest::case2523() {}
void ParserTest::case2524() {}
void ParserTest::case2525() {}
void ParserTest::case2526() {}
void ParserTest::case2527() {}
void ParserTest::case2528() {}
void ParserTest::case2529() {}
void ParserTest::case2530() {}
void ParserTest::case2531() {}
void ParserTest::case2532() {}
void ParserTest::case2533() {}
void ParserTest::case2534() {}
void ParserTest::case2535() {}
void ParserTest::case2536() {}
void ParserTest::case2537() {}
void ParserTest::case2538() {}
void ParserTest::case2539() {}
void ParserTest::case2540() {}
void ParserTest::case2541() {}
void ParserTest::case2542() {}
void ParserTest::case2543() {}
void ParserTest::case2544() {}
void ParserTest::case2545() {}
void ParserTest::case2546() {}
void ParserTest::case2547() {}
void ParserTest::case2548() {}
void ParserTest::case2549() {}
void ParserTest::case2550() {}
void ParserTest::case2551() {}
void ParserTest::case2552() {}
void ParserTest::case2553() {}
void ParserTest::case2554() {}
void ParserTest::case2555() {}
void ParserTest::case2556() {}
void ParserTest::case2557() {}
void ParserTest::case2558() {}
void ParserTest::case2559() {}
void ParserTest::case2560() {}
void ParserTest::case2561() {}
void ParserTest::case2562() {}
void ParserTest::case2563() {}
void ParserTest::case2564() {}
void ParserTest::case2565() {}
void ParserTest::case2566() {}
void ParserTest::case2567() {}
void ParserTest::case2568() {}
void ParserTest::case2569() {}
void ParserTest::case2570() {}
void ParserTest::case2571() {}
void ParserTest::case2572() {}
void ParserTest::case2573() {}
void ParserTest::case2574() {}
void ParserTest::case2575() {}
void ParserTest::case2576() {}
void ParserTest::case2577() {}
void ParserTest::case2578() {}
void ParserTest::case2579() {}
void ParserTest::case2580() {}
void ParserTest::case2581() {}
void ParserTest::case2582() {}
void ParserTest::case2583() {}
void ParserTest::case2584() {}
void ParserTest::case2585() {}
void ParserTest::case2586() {}
void ParserTest::case2587() {}
void ParserTest::case2588() {}
void ParserTest::case2589() {}
void ParserTest::case2590() {}
void ParserTest::case2591() {}
void ParserTest::case2592() {}
void ParserTest::case2593() {}
void ParserTest::case2594() {}
void ParserTest::case2595() {}
void ParserTest::case2596() {}
void ParserTest::case2597() {}
void ParserTest::case2598() {}
void ParserTest::case2599() {}

void ParserTest::case2600()
{
    parseStatement(R"(
                   asm ( "nop" );
                   )");
}

void ParserTest::case2601()
{
    parseStatement(R"(
                   asm ( "nop"
                         "nop" );
                   )");
}

void ParserTest::case2602()
{
    parseStatement(R"(
                   asm ( "nop" )
                   )",
                   Expectation().addDiagnostic(Expectation::ErrorOrWarn::Error,
                                               Parser::DiagnosticsReporter::ID_of_ExpectedToken));
}

void ParserTest::case2603()
{
    parseStatement(R"(
                   asm ( )
                   )",
                   Expectation().addDiagnostic(Expectation::ErrorOrWarn::Error,
                                               Parser::DiagnosticsReporter::ID_of_ExpectedTokenOfCategoryStringLiteral));
}

void ParserTest::case2604()
{
    // From https://www.felixcloutier.com/documents/gcc-asm.html.

    parseStatement(R"(
                   asm ( "movq %0, %0" : "+rm" ( foo ) ) ;
                   )");
}

void ParserTest::case2605()
{
    // From https://www.felixcloutier.com/documents/gcc-asm.html.

    parseStatement(R"(
                   asm ( "addl %0, %1" : "+r" ( foo ) : "g" ( bar ) ) ;
                   )");
}

void ParserTest::case2606()
{
    // From https://www.felixcloutier.com/documents/gcc-asm.html.

    parseStatement(R"(
                   asm ( "lfence " :  :  : "memory" ) ;
                   )");
}

void ParserTest::case2607()
{
    parseStatement(R"(
                   asm ( "movl $123 , %0 \n  "
                         "addl %1 , %0"
                         : "=&r" ( foo )
                         : "r" ( bar ) ) ;
                   )");
}

void ParserTest::case2608()
{
    parseStatement(R"(
                   asm goto ( "jmp %l0\n"
                              :
                              :
                              :
                              : x , y );
                   )");
}

void ParserTest::case2609()
{
    parseStatement(R"(
                   asm goto ( "jmp %l0\n"
                              :
                              :
                              :
                              : x );
                   )");
}

void ParserTest::case2610()
{
    // From https://www.felixcloutier.com/documents/gcc-asm.html.

    parseStatement(R"(
                   asm volatile ( "syscall" : "=a" (ret_val) : : "rcx" , "r11" ) ;
                   )");
}

void ParserTest::case2611()
{
    // From https://www.felixcloutier.com/documents/gcc-asm.html.

    parseStatement(R"(
                   asm ( "imulq %[rhs]"
                         : "=a" (*lo) , "=d" (*hi)
                         : [ lhs ] "0" ( left ) , [ rhs ] "rm" ( right )) ;
                   )");
}

void ParserTest::case2612()
{
    parseStatement(R"(
                   asm ( "mov %[e], %[d]"
                      : [ d ] "=rm" ( d )
                      : [ e ] "rm" ( * e ) );
                   )");
}

void ParserTest::case2613() {}
void ParserTest::case2614() {}
void ParserTest::case2615() {}
void ParserTest::case2616() {}
void ParserTest::case2617() {}
void ParserTest::case2618() {}
void ParserTest::case2619() {}
void ParserTest::case2620() {}
void ParserTest::case2621() {}
void ParserTest::case2622() {}
void ParserTest::case2623() {}
void ParserTest::case2624() {}
void ParserTest::case2625() {}
void ParserTest::case2626() {}
void ParserTest::case2627() {}
void ParserTest::case2628() {}
void ParserTest::case2629() {}
void ParserTest::case2630() {}
void ParserTest::case2631() {}
void ParserTest::case2632() {}
void ParserTest::case2633() {}
void ParserTest::case2634() {}
void ParserTest::case2635() {}
void ParserTest::case2636() {}
void ParserTest::case2637() {}
void ParserTest::case2638() {}
void ParserTest::case2639() {}
void ParserTest::case2640() {}
void ParserTest::case2641() {}
void ParserTest::case2642() {}
void ParserTest::case2643() {}
void ParserTest::case2644() {}
void ParserTest::case2645() {}
void ParserTest::case2646() {}
void ParserTest::case2647() {}
void ParserTest::case2648() {}
void ParserTest::case2649() {}
void ParserTest::case2650() {}
void ParserTest::case2651() {}
void ParserTest::case2652() {}
void ParserTest::case2653() {}
void ParserTest::case2654() {}
void ParserTest::case2655() {}
void ParserTest::case2656() {}
void ParserTest::case2657() {}
void ParserTest::case2658() {}
void ParserTest::case2659() {}
void ParserTest::case2660() {}
void ParserTest::case2661() {}
void ParserTest::case2662() {}
void ParserTest::case2663() {}
void ParserTest::case2664() {}
void ParserTest::case2665() {}
void ParserTest::case2666() {}
void ParserTest::case2667() {}
void ParserTest::case2668() {}
void ParserTest::case2669() {}
void ParserTest::case2670() {}
void ParserTest::case2671() {}
void ParserTest::case2672() {}
void ParserTest::case2673() {}
void ParserTest::case2674() {}
void ParserTest::case2675() {}
void ParserTest::case2676() {}
void ParserTest::case2677() {}
void ParserTest::case2678() {}
void ParserTest::case2679() {}
void ParserTest::case2680() {}
void ParserTest::case2681() {}
void ParserTest::case2682() {}
void ParserTest::case2683() {}
void ParserTest::case2684() {}
void ParserTest::case2685() {}
void ParserTest::case2686() {}
void ParserTest::case2687() {}
void ParserTest::case2688() {}
void ParserTest::case2689() {}
void ParserTest::case2690() {}
void ParserTest::case2691() {}
void ParserTest::case2692() {}
void ParserTest::case2693() {}
void ParserTest::case2694() {}
void ParserTest::case2695() {}
void ParserTest::case2696() {}
void ParserTest::case2697() {}
void ParserTest::case2698() {}
void ParserTest::case2699() {}

void ParserTest::case2700() {}
void ParserTest::case2701() {}
void ParserTest::case2702() {}
void ParserTest::case2703() {}
void ParserTest::case2704() {}
void ParserTest::case2705() {}
void ParserTest::case2706() {}
void ParserTest::case2707() {}
void ParserTest::case2708() {}
void ParserTest::case2709() {}
void ParserTest::case2710() {}
void ParserTest::case2711() {}
void ParserTest::case2712() {}
void ParserTest::case2713() {}
void ParserTest::case2714() {}
void ParserTest::case2715() {}
void ParserTest::case2716() {}
void ParserTest::case2717() {}
void ParserTest::case2718() {}
void ParserTest::case2719() {}
void ParserTest::case2720() {}
void ParserTest::case2721() {}
void ParserTest::case2722() {}
void ParserTest::case2723() {}
void ParserTest::case2724() {}
void ParserTest::case2725() {}
void ParserTest::case2726() {}
void ParserTest::case2727() {}
void ParserTest::case2728() {}
void ParserTest::case2729() {}
void ParserTest::case2730() {}
void ParserTest::case2731() {}
void ParserTest::case2732() {}
void ParserTest::case2733() {}
void ParserTest::case2734() {}
void ParserTest::case2735() {}
void ParserTest::case2736() {}
void ParserTest::case2737() {}
void ParserTest::case2738() {}
void ParserTest::case2739() {}
void ParserTest::case2740() {}
void ParserTest::case2741() {}
void ParserTest::case2742() {}
void ParserTest::case2743() {}
void ParserTest::case2744() {}
void ParserTest::case2745() {}
void ParserTest::case2746() {}
void ParserTest::case2747() {}
void ParserTest::case2748() {}
void ParserTest::case2749() {}
void ParserTest::case2750() {}
void ParserTest::case2751() {}
void ParserTest::case2752() {}
void ParserTest::case2753() {}
void ParserTest::case2754() {}
void ParserTest::case2755() {}
void ParserTest::case2756() {}
void ParserTest::case2757() {}
void ParserTest::case2758() {}
void ParserTest::case2759() {}
void ParserTest::case2760() {}
void ParserTest::case2761() {}
void ParserTest::case2762() {}
void ParserTest::case2763() {}
void ParserTest::case2764() {}
void ParserTest::case2765() {}
void ParserTest::case2766() {}
void ParserTest::case2767() {}
void ParserTest::case2768() {}
void ParserTest::case2769() {}
void ParserTest::case2770() {}
void ParserTest::case2771() {}
void ParserTest::case2772() {}
void ParserTest::case2773() {}
void ParserTest::case2774() {}
void ParserTest::case2775() {}
void ParserTest::case2776() {}
void ParserTest::case2777() {}
void ParserTest::case2778() {}
void ParserTest::case2779() {}
void ParserTest::case2780() {}
void ParserTest::case2781() {}
void ParserTest::case2782() {}
void ParserTest::case2783() {}
void ParserTest::case2784() {}
void ParserTest::case2785() {}
void ParserTest::case2786() {}
void ParserTest::case2787() {}
void ParserTest::case2788() {}
void ParserTest::case2789() {}
void ParserTest::case2790() {}
void ParserTest::case2791() {}
void ParserTest::case2792() {}
void ParserTest::case2793() {}
void ParserTest::case2794() {}
void ParserTest::case2795() {}
void ParserTest::case2796() {}
void ParserTest::case2797() {}
void ParserTest::case2798() {}
void ParserTest::case2799() {}

void ParserTest::case2800() {}
void ParserTest::case2801() {}
void ParserTest::case2802() {}
void ParserTest::case2803() {}
void ParserTest::case2804() {}
void ParserTest::case2805() {}
void ParserTest::case2806() {}
void ParserTest::case2807() {}
void ParserTest::case2808() {}
void ParserTest::case2809() {}
void ParserTest::case2810() {}
void ParserTest::case2811() {}
void ParserTest::case2812() {}
void ParserTest::case2813() {}
void ParserTest::case2814() {}
void ParserTest::case2815() {}
void ParserTest::case2816() {}
void ParserTest::case2817() {}
void ParserTest::case2818() {}
void ParserTest::case2819() {}
void ParserTest::case2820() {}
void ParserTest::case2821() {}
void ParserTest::case2822() {}
void ParserTest::case2823() {}
void ParserTest::case2824() {}
void ParserTest::case2825() {}
void ParserTest::case2826() {}
void ParserTest::case2827() {}
void ParserTest::case2828() {}
void ParserTest::case2829() {}
void ParserTest::case2830() {}
void ParserTest::case2831() {}
void ParserTest::case2832() {}
void ParserTest::case2833() {}
void ParserTest::case2834() {}
void ParserTest::case2835() {}
void ParserTest::case2836() {}
void ParserTest::case2837() {}
void ParserTest::case2838() {}
void ParserTest::case2839() {}
void ParserTest::case2840() {}
void ParserTest::case2841() {}
void ParserTest::case2842() {}
void ParserTest::case2843() {}
void ParserTest::case2844() {}
void ParserTest::case2845() {}
void ParserTest::case2846() {}
void ParserTest::case2847() {}
void ParserTest::case2848() {}
void ParserTest::case2849() {}
void ParserTest::case2850() {}
void ParserTest::case2851() {}
void ParserTest::case2852() {}
void ParserTest::case2853() {}
void ParserTest::case2854() {}
void ParserTest::case2855() {}
void ParserTest::case2856() {}
void ParserTest::case2857() {}
void ParserTest::case2858() {}
void ParserTest::case2859() {}
void ParserTest::case2860() {}
void ParserTest::case2861() {}
void ParserTest::case2862() {}
void ParserTest::case2863() {}
void ParserTest::case2864() {}
void ParserTest::case2865() {}
void ParserTest::case2866() {}
void ParserTest::case2867() {}
void ParserTest::case2868() {}
void ParserTest::case2869() {}
void ParserTest::case2870() {}
void ParserTest::case2871() {}
void ParserTest::case2872() {}
void ParserTest::case2873() {}
void ParserTest::case2874() {}
void ParserTest::case2875() {}
void ParserTest::case2876() {}
void ParserTest::case2877() {}
void ParserTest::case2878() {}
void ParserTest::case2879() {}
void ParserTest::case2880() {}
void ParserTest::case2881() {}
void ParserTest::case2882() {}
void ParserTest::case2883() {}
void ParserTest::case2884() {}
void ParserTest::case2885() {}
void ParserTest::case2886() {}
void ParserTest::case2887() {}
void ParserTest::case2888() {}
void ParserTest::case2889() {}
void ParserTest::case2890() {}
void ParserTest::case2891() {}
void ParserTest::case2892() {}
void ParserTest::case2893() {}
void ParserTest::case2894() {}
void ParserTest::case2895() {}
void ParserTest::case2896() {}
void ParserTest::case2897() {}
void ParserTest::case2898() {}
void ParserTest::case2899() {}

void ParserTest::case2900() {}
void ParserTest::case2901() {}
void ParserTest::case2902() {}
void ParserTest::case2903() {}
void ParserTest::case2904() {}
void ParserTest::case2905() {}
void ParserTest::case2906() {}
void ParserTest::case2907() {}
void ParserTest::case2908() {}
void ParserTest::case2909() {}
void ParserTest::case2910() {}
void ParserTest::case2911() {}
void ParserTest::case2912() {}
void ParserTest::case2913() {}
void ParserTest::case2914() {}
void ParserTest::case2915() {}
void ParserTest::case2916() {}
void ParserTest::case2917() {}
void ParserTest::case2918() {}
void ParserTest::case2919() {}
void ParserTest::case2920() {}
void ParserTest::case2921() {}
void ParserTest::case2922() {}
void ParserTest::case2923() {}
void ParserTest::case2924() {}
void ParserTest::case2925() {}
void ParserTest::case2926() {}
void ParserTest::case2927() {}
void ParserTest::case2928() {}
void ParserTest::case2929() {}
void ParserTest::case2930() {}
void ParserTest::case2931() {}
void ParserTest::case2932() {}
void ParserTest::case2933() {}
void ParserTest::case2934() {}
void ParserTest::case2935() {}
void ParserTest::case2936() {}
void ParserTest::case2937() {}
void ParserTest::case2938() {}
void ParserTest::case2939() {}
void ParserTest::case2940() {}
void ParserTest::case2941() {}
void ParserTest::case2942() {}
void ParserTest::case2943() {}
void ParserTest::case2944() {}
void ParserTest::case2945() {}
void ParserTest::case2946() {}
void ParserTest::case2947() {}
void ParserTest::case2948() {}
void ParserTest::case2949() {}
void ParserTest::case2950() {}
void ParserTest::case2951() {}
void ParserTest::case2952() {}
void ParserTest::case2953() {}
void ParserTest::case2954() {}
void ParserTest::case2955() {}
void ParserTest::case2956() {}
void ParserTest::case2957() {}
void ParserTest::case2958() {}
void ParserTest::case2959() {}
void ParserTest::case2960() {}
void ParserTest::case2961() {}
void ParserTest::case2962() {}
void ParserTest::case2963() {}
void ParserTest::case2964() {}
void ParserTest::case2965() {}
void ParserTest::case2966() {}
void ParserTest::case2967() {}
void ParserTest::case2968() {}
void ParserTest::case2969() {}
void ParserTest::case2970() {}
void ParserTest::case2971() {}
void ParserTest::case2972() {}
void ParserTest::case2973() {}
void ParserTest::case2974() {}
void ParserTest::case2975() {}
void ParserTest::case2976() {}
void ParserTest::case2977() {}
void ParserTest::case2978() {}
void ParserTest::case2979() {}
void ParserTest::case2980() {}
void ParserTest::case2981() {}
void ParserTest::case2982() {}
void ParserTest::case2983() {}
void ParserTest::case2984() {}
void ParserTest::case2985() {}
void ParserTest::case2986() {}
void ParserTest::case2987() {}
void ParserTest::case2988() {}
void ParserTest::case2989() {}
void ParserTest::case2990() {}
void ParserTest::case2991() {}
void ParserTest::case2992() {}
void ParserTest::case2993() {}
void ParserTest::case2994() {}
void ParserTest::case2995() {}
void ParserTest::case2996() {}
void ParserTest::case2997() {}
void ParserTest::case2998() {}
void ParserTest::case2999() {}
