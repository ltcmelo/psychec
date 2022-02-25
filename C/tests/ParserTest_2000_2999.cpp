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

void TestParser::case2000()
{
    parseStatement("x : 1 ; ",
                   Expectation().AST( { IdentifierLabelStatement,
                                        ExpressionStatement,
                                        IntegerConstantExpression }));
}

void TestParser::case2001()
{
    parseStatement("x : :",
                   Expectation().addDiagnostic(Expectation::ErrorOrWarn::Error,
                                               Parser::DiagnosticsReporter::ID_of_ExpectedFIRSTofExpression));
}

void TestParser::case2002()
{
    parseStatement("goto x ;",
                   Expectation().AST( { GotoStatement }));
}

void TestParser::case2003()
{
    parseStatement("goto ;",
                   Expectation().addDiagnostic(Expectation::ErrorOrWarn::Error,
                                               Parser::DiagnosticsReporter::ID_of_ExpectedTokenOfCategoryIdentifier));
}

void TestParser::case2004()
{
    parseStatement("goto 1 ;",
                   Expectation().addDiagnostic(Expectation::ErrorOrWarn::Error,
                                               Parser::DiagnosticsReporter::ID_of_ExpectedTokenOfCategoryIdentifier));
}

void TestParser::case2005()
{
    parseStatement("goto x",
                   Expectation().addDiagnostic(Expectation::ErrorOrWarn::Error,
                                               Parser::DiagnosticsReporter::ID_of_ExpectedToken));
}

void TestParser::case2006()
{

}

void TestParser::case2007()
{

}

void TestParser::case2008()
{

}

void TestParser::case2009()
{

}

void TestParser::case2010()
{

}

void TestParser::case2011()
{

}

void TestParser::case2012()
{

}

void TestParser::case2013()
{

}

void TestParser::case2014()
{

}

void TestParser::case2015()
{

}

void TestParser::case2016()
{

}

void TestParser::case2017()
{

}

void TestParser::case2018()
{

}

void TestParser::case2019()
{

}

void TestParser::case2020()
{
    parseStatement("{ }",
                   Expectation().AST( { CompoundStatement }));
}

void TestParser::case2021()
{
    parseStatement("{",
                   Expectation().addDiagnostic(Expectation::ErrorOrWarn::Error,
                                               Parser::DiagnosticsReporter::ID_of_ExpectedToken));
}

void TestParser::case2022()
{
    parseStatement("{ / }",
                   Expectation().setErrorCnt(1));
}

void TestParser::case2023()
{
    parseStatement("{ { } }",
                   Expectation().AST( { CompoundStatement,
                                        CompoundStatement }));
}

void TestParser::case2024()
{
    parseDeclaration("void x ( ) { }",
                     Expectation().AST( { FunctionDefinition,
                                          BuiltinTypeSpecifier,
                                          FunctionDeclarator,
                                          IdentifierDeclarator,
                                          ParameterSuffix,
                                          CompoundStatement }));
}

void TestParser::case2025()
{

}

void TestParser::case2026()
{

}

void TestParser::case2027()
{

}

void TestParser::case2028()
{

}

void TestParser::case2029()
{

}

void TestParser::case2030()
{

}

void TestParser::case2031()
{
}

void TestParser::case2032()
{
}

void TestParser::case2033()
{
}

void TestParser::case2034()
{
}

void TestParser::case2035()
{
}

void TestParser::case2036()
{
}

void TestParser::case2037()
{
}

void TestParser::case2038()
{
}

void TestParser::case2039()
{
}

void TestParser::case2040()
{
}

void TestParser::case2041()
{
}

void TestParser::case2042()
{
}

void TestParser::case2043()
{
}

void TestParser::case2044()
{
}

void TestParser::case2045()
{
}

void TestParser::case2046()
{
}

void TestParser::case2047()
{

}

void TestParser::case2048()
{

}

void TestParser::case2049()
{

}

void TestParser::case2050()
{

}

void TestParser::case2051()
{

}

void TestParser::case2052()
{

}

void TestParser::case2053()
{

}

void TestParser::case2054()
{

}

void TestParser::case2055()
{

}

void TestParser::case2056()
{

}

void TestParser::case2057()
{

}

void TestParser::case2058()
{

}

void TestParser::case2059()
{

}

void TestParser::case2060()
{

}

void TestParser::case2061()
{

}

void TestParser::case2062()
{

}

void TestParser::case2063()
{
}

void TestParser::case2064()
{
}

void TestParser::case2065()
{

}

void TestParser::case2066()
{

}

void TestParser::case2067()
{

}

void TestParser::case2068()
{
}

void TestParser::case2069()
{
}

void TestParser::case2070()
{
}

void TestParser::case2071()
{
}

void TestParser::case2072()
{
}

void TestParser::case2073()
{
}

void TestParser::case2074() {}
void TestParser::case2075() {}
void TestParser::case2076() {}
void TestParser::case2077() {}
void TestParser::case2078() {}
void TestParser::case2079() {}
void TestParser::case2080() {}
void TestParser::case2081() {}
void TestParser::case2082() {}
void TestParser::case2083() {}
void TestParser::case2084() {}
void TestParser::case2085() {}
void TestParser::case2086() {}
void TestParser::case2087() {}
void TestParser::case2088() {}
void TestParser::case2089() {}
void TestParser::case2090() {}
void TestParser::case2091() {}
void TestParser::case2092() {}
void TestParser::case2093() {}
void TestParser::case2094() {}
void TestParser::case2095() {}
void TestParser::case2096() {}
void TestParser::case2097() {}
void TestParser::case2098() {}
void TestParser::case2099() {}

void TestParser::case2100()
{
    parseStatement("{ x ++ ; }",
                   Expectation().AST( { CompoundStatement,
                                        ExpressionStatement,
                                        PostIncrementExpression,
                                        IdentifierName }));
}

void TestParser::case2101()
{
    parseStatement("{ ++ x ; }",
                   Expectation().AST( { CompoundStatement,
                                        ExpressionStatement,
                                        PreIncrementExpression,
                                        IdentifierName }));
}

void TestParser::case2102()
{
    parseStatement("{ x ( 1 ) ; }",
                   Expectation().AST( { CompoundStatement,
                                        ExpressionStatement,
                                        CallExpression,
                                        IdentifierName,
                                        IntegerConstantExpression }));
}

void TestParser::case2103()
{
    parseStatement("{ x ( 1 , 2 ) ; }");
}

void TestParser::case2104()
{
    parseStatement("{ x ( y , 2 ) ; }");
}

void TestParser::case2105()
{
    parseStatement("{ x ( 1 , 2 ) ; }");
}

void TestParser::case2106()
{
    parseStatement("{ ( x ( 1 ) ) ; }",
                   Expectation().AST( { CompoundStatement,
                                        ExpressionStatement,
                                        ParenthesizedExpression,
                                        CallExpression,
                                        IdentifierName,
                                        IntegerConstantExpression }));
}

void TestParser::case2107()
{
    parseStatement("{ ( ( x ( 1 ) ) ) ; }");
}

void TestParser::case2108()
{
    parseStatement("{ ( x ( 1 , 2 ) ) ; }");
}

void TestParser::case2109()
{
    parseStatement("{ 'x' ; }");
}

void TestParser::case2110()
{
    parseStatement("{ \"x\" ; }");
}

void TestParser::case2111()
{
    parseStatement("{ x ; }");
}

void TestParser::case2112()
{
    parseStatement("{ 1 ; }");
}

void TestParser::case2113()
{
    parseStatement("{ 1 ( ; }",
                   Expectation().setErrorCnt(1));
}

void TestParser::case2114()
{
    parseStatement("{ 'x' ; }");
}

void TestParser::case2115()
{
    /*
     Despite the parsing similarities between `typeof' and `sizeof',
     mind the difference between this test case and the following:

     `typeof ( x ) y ;'

     This would be a valid declaration-statement, and the `y' doesn't
     belong to the parsing of `typeof': it's the declarator.
     */

    CROSS_REFERENCE_TEST(TestParser::case0904);

    parseStatement("sizeof ( x ) y ;",
                   Expectation().addDiagnostic(Expectation::ErrorOrWarn::Error,
                                               Parser::DiagnosticsReporter::ID_of_ExpectedToken));
}

void TestParser::case2116() {}
void TestParser::case2117() {}
void TestParser::case2118() {}
void TestParser::case2119() {}
void TestParser::case2120() {}
void TestParser::case2121() {}
void TestParser::case2122() {}
void TestParser::case2123() {}
void TestParser::case2124() {}
void TestParser::case2125() {}
void TestParser::case2126() {}
void TestParser::case2127() {}
void TestParser::case2128() {}
void TestParser::case2129() {}
void TestParser::case2130() {}
void TestParser::case2131() {}
void TestParser::case2132() {}
void TestParser::case2133() {}
void TestParser::case2134() {}
void TestParser::case2135() {}
void TestParser::case2136() {}
void TestParser::case2137() {}
void TestParser::case2138() {}
void TestParser::case2139() {}
void TestParser::case2140() {}
void TestParser::case2141() {}
void TestParser::case2142() {}
void TestParser::case2143() {}
void TestParser::case2144() {}
void TestParser::case2145() {}
void TestParser::case2146() {}
void TestParser::case2147() {}
void TestParser::case2148() {}
void TestParser::case2149() {}
void TestParser::case2150() {}
void TestParser::case2151() {}
void TestParser::case2152() {}
void TestParser::case2153() {}
void TestParser::case2154() {}
void TestParser::case2155() {}
void TestParser::case2156() {}
void TestParser::case2157() {}
void TestParser::case2158() {}
void TestParser::case2159() {}
void TestParser::case2160() {}
void TestParser::case2161() {}
void TestParser::case2162() {}
void TestParser::case2163() {}
void TestParser::case2164() {}
void TestParser::case2165() {}
void TestParser::case2166() {}
void TestParser::case2167() {}
void TestParser::case2168() {}
void TestParser::case2169() {}
void TestParser::case2170() {}
void TestParser::case2171() {}
void TestParser::case2172() {}
void TestParser::case2173() {}
void TestParser::case2174() {}
void TestParser::case2175() {}
void TestParser::case2176() {}
void TestParser::case2177() {}
void TestParser::case2178() {}
void TestParser::case2179() {}
void TestParser::case2180() {}
void TestParser::case2181() {}
void TestParser::case2182() {}
void TestParser::case2183() {}
void TestParser::case2184() {}
void TestParser::case2185() {}
void TestParser::case2186() {}
void TestParser::case2187() {}
void TestParser::case2188() {}
void TestParser::case2189() {}
void TestParser::case2190() {}
void TestParser::case2191() {}
void TestParser::case2192() {}
void TestParser::case2193() {}
void TestParser::case2194() {}
void TestParser::case2195() {}
void TestParser::case2196() {}
void TestParser::case2197() {}
void TestParser::case2198() {}
void TestParser::case2199() {}

void TestParser::case2200()
{
    parseStatement("{ _Static_assert ( 1 != 2 , \"x\" ) ; }");
}

void TestParser::case2201()
{
    parseStatement("{ int x ; }",
                   Expectation().AST( { CompoundStatement,
                                        DeclarationStatement,
                                        VariableAndOrFunctionDeclaration,
                                        BuiltinTypeSpecifier,
                                        IdentifierDeclarator }));
}

void TestParser::case2202()
{
    parseStatement("{ int x , y ; }",
                   Expectation().AST( { CompoundStatement,
                                        DeclarationStatement,
                                        VariableAndOrFunctionDeclaration,
                                        BuiltinTypeSpecifier,
                                        IdentifierDeclarator,
                                        IdentifierDeclarator }));
}

void TestParser::case2203()
{
    parseStatement("{ const int x ; }",
                   Expectation().AST( { CompoundStatement,
                                        DeclarationStatement,
                                        VariableAndOrFunctionDeclaration,
                                        ConstQualifier,
                                        BuiltinTypeSpecifier,
                                        IdentifierDeclarator }));
}

void TestParser::case2204()
{
    parseStatement("{ int * x ; }",
                   Expectation().AST( { CompoundStatement,
                                        DeclarationStatement,
                                        VariableAndOrFunctionDeclaration,
                                        BuiltinTypeSpecifier,
                                        PointerDeclarator,
                                        IdentifierDeclarator }));
}

void TestParser::case2205()
{
    parseStatement("{ struct x ; }",
                   Expectation().AST( { CompoundStatement,
                                        DeclarationStatement,
                                        IncompleteDeclaration,
                                        StructTypeSpecifier }));
}

void TestParser::case2206()
{
    parseStatement("{ union x y ; }",
                   Expectation().AST( { CompoundStatement,
                                        DeclarationStatement,
                                        VariableAndOrFunctionDeclaration,
                                        UnionTypeSpecifier,
                                        IdentifierDeclarator }));
}

void TestParser::case2207()
{
    parseStatement("{ x y ; }",
                   Expectation().AST( { CompoundStatement,
                                        DeclarationStatement,
                                        VariableAndOrFunctionDeclaration,
                                        TypedefName,
                                        IdentifierDeclarator }));
}

void TestParser::case2208()
{
    parseStatement("{ x y , z ; }",
                   Expectation().AST( { CompoundStatement,
                                        DeclarationStatement,
                                        VariableAndOrFunctionDeclaration,
                                        TypedefName,
                                        IdentifierDeclarator,
                                        IdentifierDeclarator }));
}

void TestParser::case2209()
{
    parseStatement("{ const x y ; }",
                   Expectation().AST( { CompoundStatement,
                                        DeclarationStatement,
                                        VariableAndOrFunctionDeclaration,
                                        ConstQualifier,
                                        TypedefName,
                                        IdentifierDeclarator }));
}

void TestParser::case2210()
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

void TestParser::case2211()
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

void TestParser::case2212()
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

void TestParser::case2213() {}
void TestParser::case2214() {}
void TestParser::case2215() {}
void TestParser::case2216() {}
void TestParser::case2217() {}
void TestParser::case2218() {}
void TestParser::case2219() {}
void TestParser::case2220() {}
void TestParser::case2221() {}
void TestParser::case2222() {}
void TestParser::case2223() {}
void TestParser::case2224() {}
void TestParser::case2225() {}
void TestParser::case2226() {}
void TestParser::case2227() {}
void TestParser::case2228() {}
void TestParser::case2229() {}
void TestParser::case2230() {}
void TestParser::case2231() {}
void TestParser::case2232() {}
void TestParser::case2233() {}
void TestParser::case2234() {}
void TestParser::case2235() {}
void TestParser::case2236() {}
void TestParser::case2237() {}
void TestParser::case2238() {}
void TestParser::case2239() {}
void TestParser::case2240() {}
void TestParser::case2241() {}
void TestParser::case2242() {}
void TestParser::case2243() {}
void TestParser::case2244() {}
void TestParser::case2245() {}
void TestParser::case2246() {}
void TestParser::case2247() {}
void TestParser::case2248() {}
void TestParser::case2249() {}
void TestParser::case2250() {}
void TestParser::case2251() {}
void TestParser::case2252() {}
void TestParser::case2253() {}
void TestParser::case2254() {}
void TestParser::case2255() {}
void TestParser::case2256() {}
void TestParser::case2257() {}
void TestParser::case2258() {}
void TestParser::case2259() {}
void TestParser::case2260() {}
void TestParser::case2261() {}
void TestParser::case2262() {}
void TestParser::case2263() {}
void TestParser::case2264() {}
void TestParser::case2265() {}
void TestParser::case2266() {}
void TestParser::case2267() {}
void TestParser::case2268() {}
void TestParser::case2269() {}
void TestParser::case2270() {}
void TestParser::case2271() {}
void TestParser::case2272() {}
void TestParser::case2273() {}
void TestParser::case2274() {}
void TestParser::case2275() {}
void TestParser::case2276() {}
void TestParser::case2277() {}
void TestParser::case2278() {}
void TestParser::case2279() {}
void TestParser::case2280() {}
void TestParser::case2281() {}
void TestParser::case2282() {}
void TestParser::case2283() {}
void TestParser::case2284() {}
void TestParser::case2285() {}
void TestParser::case2286() {}
void TestParser::case2287() {}
void TestParser::case2288() {}
void TestParser::case2289() {}
void TestParser::case2290() {}
void TestParser::case2291() {}
void TestParser::case2292() {}
void TestParser::case2293() {}
void TestParser::case2294() {}
void TestParser::case2295() {}
void TestParser::case2296() {}
void TestParser::case2297() {}
void TestParser::case2298() {}
void TestParser::case2299() {}

void TestParser::case2300()
{
    parseStatement("if ( x ) ;");
}

void TestParser::case2301()
{
    parseStatement("if ( x ) { } ");
}

void TestParser::case2302()
{
    parseStatement("if ( x ( ) ) { }");
}

void TestParser::case2303()
{
    parseStatement("if ( x ) y ;");
}

void TestParser::case2304()
{
    parseStatement("if ( x ) { y ; }");
}

void TestParser::case2305()
{
    parseStatement("if ( x ++ ) { }");
}

void TestParser::case2306()
{
    parseStatement("if ( ++ x ) { }");
}

void TestParser::case2307()
{
    parseStatement("if ( 1 ) { } else ;");
}

void TestParser::case2308()
{
    parseStatement("if ( 1 ) ; else ;");
}

void TestParser::case2309()
{
    parseStatement("if ( 1 ) ; else { }");
}

void TestParser::case2310()
{
    parseStatement("if ( 1 ) { } else { }");
}

void TestParser::case2311()
{
    parseStatement("if ( 1 ) { x ; } else { y ; }");
}

void TestParser::case2312()
{
    parseStatement("if ( 1 ) else { }",
                   Expectation().addDiagnostic(Expectation::ErrorOrWarn::Error,
                                               Parser::DiagnosticsReporter::ID_of_ExpectedFIRSTofExpression));
}

void TestParser::case2313()
{
    parseStatement("if ( 1 ) 2 else { }",
                   Expectation().addDiagnostic(Expectation::ErrorOrWarn::Error,
                                               Parser::DiagnosticsReporter::ID_of_ExpectedToken));
}

void TestParser::case2314()
{
    parseStatement("if else x ;",
                   Expectation().addDiagnostic(Expectation::ErrorOrWarn::Error,
                                               Parser::DiagnosticsReporter::ID_of_ExpectedToken));
}

void TestParser::case2315()
{
    parseStatement("if ( 1 ) { } else ",
                   Expectation().addDiagnostic(Expectation::ErrorOrWarn::Error,
                                               Parser::DiagnosticsReporter::ID_of_ExpectedFIRSTofExpression));
}

void TestParser::case2316()
{
    parseStatement("if ( 1 ) { } else (",
                   Expectation().addDiagnostic(Expectation::ErrorOrWarn::Error,
                                               Parser::DiagnosticsReporter::ID_of_ExpectedFIRSTofExpression));
}

void TestParser::case2317()
{
    parseStatement("if ( 1 ) if ( 2 ) x ;");
}

void TestParser::case2318()
{
    parseStatement("if ( 1 ) if ( 2 ) x ; else y ;");
}

void TestParser::case2319()
{
    parseStatement("if ( 1 ) { if ( 2 ) x ; } else y ;");
}

void TestParser::case2320()
{
    parseStatement("if ( 1 ) { if ( 2 ) x ; } else { y ; }");
}

void TestParser::case2321() {}
void TestParser::case2322() {}
void TestParser::case2323() {}
void TestParser::case2324() {}
void TestParser::case2325() {}
void TestParser::case2326() {}
void TestParser::case2327() {}
void TestParser::case2328() {}
void TestParser::case2329() {}
void TestParser::case2330() {}
void TestParser::case2331() {}
void TestParser::case2332() {}
void TestParser::case2333() {}
void TestParser::case2334() {}
void TestParser::case2335() {}
void TestParser::case2336() {}
void TestParser::case2337() {}
void TestParser::case2338() {}
void TestParser::case2339() {}
void TestParser::case2340() {}
void TestParser::case2341() {}
void TestParser::case2342() {}
void TestParser::case2343() {}
void TestParser::case2344() {}
void TestParser::case2345() {}
void TestParser::case2346() {}
void TestParser::case2347() {}
void TestParser::case2348() {}
void TestParser::case2349() {}

void TestParser::case2350()
{
    parseStatement("switch ( 1 ) { }");
}

void TestParser::case2351()
{
    parseStatement("switch ( x ) { case 2 : y ( ) ; }");
}

void TestParser::case2352()
{
    parseStatement(R"(
                   switch ( x ) {
                       case 1 : y ;
                       case 2 : z ( ) ;
                   }
                   )");
}

void TestParser::case2353()
{
    parseStatement("switch ( x ) case 2 : y ( ) ;");
}

void TestParser::case2354()
{
    parseStatement(R"(
                   switch ( x ) {
                       case 1 : y ;
                       default : z ( ) ;
                   }
                   )");
}

void TestParser::case2355()
{
    parseStatement(R"(
                   switch ( x ) {
                       default : y ;
                   }
                   )");
}

void TestParser::case2356()
{
    parseStatement("case 1 : x ;",
                   Expectation().addDiagnostic(Expectation::ErrorOrWarn::Error,
                                               Parser::DiagnosticsReporter::ID_of_UnexpectedCaseLabelOutsideSwitch));
}

void TestParser::case2357()
{
    parseStatement("default : x ;",
                   Expectation().addDiagnostic(Expectation::ErrorOrWarn::Error,
                                               Parser::DiagnosticsReporter::ID_of_UnexpectedDefaultLabelOutsideSwitch));
}

void TestParser::case2358()
{
}

void TestParser::case2359() {}
void TestParser::case2360() {}
void TestParser::case2361() {}
void TestParser::case2362() {}
void TestParser::case2363() {}
void TestParser::case2364() {}
void TestParser::case2365() {}
void TestParser::case2366() {}
void TestParser::case2367() {}
void TestParser::case2368() {}
void TestParser::case2369() {}
void TestParser::case2370() {}
void TestParser::case2371() {}
void TestParser::case2372() {}
void TestParser::case2373() {}
void TestParser::case2374() {}
void TestParser::case2375() {}
void TestParser::case2376() {}
void TestParser::case2377() {}
void TestParser::case2378() {}
void TestParser::case2379() {}
void TestParser::case2380() {}
void TestParser::case2381() {}
void TestParser::case2382() {}
void TestParser::case2383() {}
void TestParser::case2384() {}
void TestParser::case2385() {}
void TestParser::case2386() {}
void TestParser::case2387() {}
void TestParser::case2388() {}
void TestParser::case2389() {}
void TestParser::case2390() {}
void TestParser::case2391() {}
void TestParser::case2392() {}
void TestParser::case2393() {}
void TestParser::case2394() {}
void TestParser::case2395() {}
void TestParser::case2396() {}
void TestParser::case2397() {}
void TestParser::case2398() {}
void TestParser::case2399() {}

void TestParser::case2400()
{
    parseStatement("while ( 1 ) { }");
}

void TestParser::case2401()
{
    parseStatement("while ( 1 ) ;");
}

void TestParser::case2402()
{
    parseStatement("while ( * x ++ != '\\0' ) ;");
}

void TestParser::case2403()
{
    parseStatement("while ( 1 ) x = y ;");
}

void TestParser::case2404()
{
    parseStatement("do { } while ( 1 ) ;");
}

void TestParser::case2405()
{
    parseStatement("do { } while ( 1 )",
                   Expectation().addDiagnostic(Expectation::ErrorOrWarn::Error,
                                               Parser::DiagnosticsReporter::ID_of_ExpectedToken));
}

void TestParser::case2406()
{
    parseStatement("do { } while ( ) ;",
                   Expectation().addDiagnostic(Expectation::ErrorOrWarn::Error,
                                               Parser::DiagnosticsReporter::ID_of_ExpectedFIRSTofExpression));
}

void TestParser::case2407()
{
    parseStatement("while ( )",
                   Expectation().addDiagnostic(Expectation::ErrorOrWarn::Error,
                                               Parser::DiagnosticsReporter::ID_of_ExpectedFIRSTofExpression));
}

void TestParser::case2408()
{
    parseStatement("do ; while ( 1 )",
                   Expectation().addDiagnostic(Expectation::ErrorOrWarn::Error,
                                               Parser::DiagnosticsReporter::ID_of_ExpectedToken));
}

void TestParser::case2409()
{
    parseStatement("do x ( ) ; while ( y / z % 0 ) ;");
}

void TestParser::case2410()
{
    parseStatement("do x ; while ( y ) ;");
}

void TestParser::case2411()
{
    parseStatement("do ; while ( 1 ) ;");
}

void TestParser::case2412()
{
    parseStatement("while ( * x ++ != 0 ) y ;");
}

void TestParser::case2413()
{
    parseStatement("while ( x != y ) z ;");
}

void TestParser::case2414()
{
    parseStatement("while ( ! x ( ) ) { y ; z ; }");
}

void TestParser::case2415()
{
        parseStatement("while ( * x ++ != '0' ) ;");
}

void TestParser::case2416() {}
void TestParser::case2417() {}
void TestParser::case2418() {}
void TestParser::case2419() {}
void TestParser::case2420() {}
void TestParser::case2421() {}
void TestParser::case2422() {}
void TestParser::case2423() {}
void TestParser::case2424() {}
void TestParser::case2425() {}
void TestParser::case2426() {}
void TestParser::case2427() {}
void TestParser::case2428() {}
void TestParser::case2429() {}
void TestParser::case2430() {}
void TestParser::case2431() {}
void TestParser::case2432() {}
void TestParser::case2433() {}
void TestParser::case2434() {}
void TestParser::case2435() {}
void TestParser::case2436() {}
void TestParser::case2437() {}
void TestParser::case2438() {}
void TestParser::case2439() {}
void TestParser::case2440() {}
void TestParser::case2441() {}
void TestParser::case2442() {}
void TestParser::case2443() {}
void TestParser::case2444() {}
void TestParser::case2445() {}
void TestParser::case2446() {}
void TestParser::case2447() {}
void TestParser::case2448() {}
void TestParser::case2449() {}

void TestParser::case2450()
{
    parseStatement("for ( ; ; ) 1 ;",
                   Expectation().AST( { ForStatement,
                                        ExpressionStatement,
                                        ExpressionStatement,
                                        IntegerConstantExpression }));
}

void TestParser::case2451()
{
    parseStatement("for ( ; ) ;",
                   Expectation().addDiagnostic(Expectation::ErrorOrWarn::Error,
                                               Parser::DiagnosticsReporter::ID_of_ExpectedFIRSTofExpression));
}

void TestParser::case2452()
{
    parseStatement("for ( ; ; ) { }",
                   Expectation().AST( { ForStatement,
                                        ExpressionStatement,
                                        CompoundStatement }));
}

void TestParser::case2453()
{
    parseStatement("for ( ; ; )",
                   Expectation().addDiagnostic(Expectation::ErrorOrWarn::Error,
                                               Parser::DiagnosticsReporter::ID_of_ExpectedFIRSTofExpression));
}

void TestParser::case2454()
{
    parseStatement("for ( x ; y ; z ) { }",
                   Expectation().AST( { ForStatement,
                                        ExpressionStatement,
                                        IdentifierName,
                                        IdentifierName,
                                        IdentifierName,
                                        CompoundStatement }));
}

void TestParser::case2455()
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

void TestParser::case2456()
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

void TestParser::case2457()
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

void TestParser::case2458()
{
    parseStatement("for ( x y ; ; ) { }",
                   Expectation().AST( { ForStatement,
                                        DeclarationStatement,
                                        VariableAndOrFunctionDeclaration,
                                        TypedefName,
                                        IdentifierDeclarator,
                                        CompoundStatement }));
}

void TestParser::case2459()
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
void TestParser::case2460()
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

void TestParser::case2461() {}
void TestParser::case2462() {}
void TestParser::case2463() {}
void TestParser::case2464() {}
void TestParser::case2465() {}
void TestParser::case2466() {}
void TestParser::case2467() {}
void TestParser::case2468() {}
void TestParser::case2469() {}
void TestParser::case2470() {}
void TestParser::case2471() {}
void TestParser::case2472() {}
void TestParser::case2473() {}
void TestParser::case2474() {}
void TestParser::case2475() {}
void TestParser::case2476() {}
void TestParser::case2477() {}
void TestParser::case2478() {}
void TestParser::case2479() {}
void TestParser::case2480() {}
void TestParser::case2481() {}
void TestParser::case2482() {}
void TestParser::case2483() {}
void TestParser::case2484() {}
void TestParser::case2485() {}
void TestParser::case2486() {}
void TestParser::case2487() {}
void TestParser::case2488() {}
void TestParser::case2489() {}
void TestParser::case2490() {}
void TestParser::case2491() {}
void TestParser::case2492() {}
void TestParser::case2493() {}
void TestParser::case2494() {}
void TestParser::case2495() {}
void TestParser::case2496() {}
void TestParser::case2497() {}
void TestParser::case2498() {}
void TestParser::case2499() {}

void TestParser::case2500()
{
    parseStatement("break ;",
                   Expectation().addDiagnostic(Expectation::ErrorOrWarn::Error,
                                               Parser::DiagnosticsReporter::ID_of_UnexpectedBreakOutsideSwitchOrLoop));
}

void TestParser::case2501()
{
    parseStatement("continue ;",
                   Expectation().addDiagnostic(Expectation::ErrorOrWarn::Error,
                                               Parser::DiagnosticsReporter::ID_of_UnexpectedContinueOutsideLoop));
}

void TestParser::case2502()
{
    parseStatement("return ;");
}

void TestParser::case2503()
{
    parseStatement("return x ;");
}

void TestParser::case2504()
{
    parseStatement("return 0 ;");
}

void TestParser::case2505()
{
    parseStatement("return x ( ) ;");
}

void TestParser::case2506()
{
    parseStatement(R"(
                   for ( ; ; )
                       break ;
                   )");
}

void TestParser::case2507()
{
    parseStatement(R"(
                   for ( ; ; )
                   {
                       break ;
                   }
                   )");
}

void TestParser::case2508()
{
    parseStatement(R"(
                   for ( ; ; )
                       continue ;
                   )");
}

void TestParser::case2509()
{
    parseStatement(R"(
                   while ( 1 )
                       break ;
                   )");
}

void TestParser::case2510()
{
    parseStatement(R"(
                   while ( 1 )
                   {
                       break ;
                   }
                   )");
}

void TestParser::case2511()
{
    parseStatement(R"(
                   for ( ; ; )
                   {
                       continue ;
                   }
                   )");
}

void TestParser::case2512()
{
    parseStatement(R"(
                   switch ( x )
                   {
                       case 1 :
                       case 2 : break ;
                   }
                   )");
}

void TestParser::case2513()
{
    parseStatement(R"(
                   switch ( x )
                   {
                       case 1 : break ;
                       case 2 : break ;
                   }
                   )");
}

void TestParser::case2514()
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

void TestParser::case2515()
{
    parse("break",
          Expectation().addDiagnostic(Expectation::ErrorOrWarn::Error,
                                      Parser::DiagnosticsReporter::ID_of_ExpectedFIRSTofDirectDeclarator)
                       .addDiagnostic(Expectation::ErrorOrWarn::Warn,
                                      Parser::DiagnosticsReporter::ID_of_ExpectedTypeSpecifier));
}

void TestParser::case2516()
{
    parse("continue ;",
          Expectation().addDiagnostic(Expectation::ErrorOrWarn::Error,
                                      Parser::DiagnosticsReporter::ID_of_ExpectedFIRSTofDirectDeclarator)
                       .addDiagnostic(Expectation::ErrorOrWarn::Warn,
                                      Parser::DiagnosticsReporter::ID_of_ExpectedTypeSpecifier));

}

void TestParser::case2517()
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

void TestParser::case2518()
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

void TestParser::case2519()
{
    parseStatement(R"(
                   switch ( x ) {
                       break;
                   }
                   )");
}

void TestParser::case2520()
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

void TestParser::case2521() {}
void TestParser::case2522() {}
void TestParser::case2523() {}
void TestParser::case2524() {}
void TestParser::case2525() {}
void TestParser::case2526() {}
void TestParser::case2527() {}
void TestParser::case2528() {}
void TestParser::case2529() {}
void TestParser::case2530() {}
void TestParser::case2531() {}
void TestParser::case2532() {}
void TestParser::case2533() {}
void TestParser::case2534() {}
void TestParser::case2535() {}
void TestParser::case2536() {}
void TestParser::case2537() {}
void TestParser::case2538() {}
void TestParser::case2539() {}
void TestParser::case2540() {}
void TestParser::case2541() {}
void TestParser::case2542() {}
void TestParser::case2543() {}
void TestParser::case2544() {}
void TestParser::case2545() {}
void TestParser::case2546() {}
void TestParser::case2547() {}
void TestParser::case2548() {}
void TestParser::case2549() {}
void TestParser::case2550() {}
void TestParser::case2551() {}
void TestParser::case2552() {}
void TestParser::case2553() {}
void TestParser::case2554() {}
void TestParser::case2555() {}
void TestParser::case2556() {}
void TestParser::case2557() {}
void TestParser::case2558() {}
void TestParser::case2559() {}
void TestParser::case2560() {}
void TestParser::case2561() {}
void TestParser::case2562() {}
void TestParser::case2563() {}
void TestParser::case2564() {}
void TestParser::case2565() {}
void TestParser::case2566() {}
void TestParser::case2567() {}
void TestParser::case2568() {}
void TestParser::case2569() {}
void TestParser::case2570() {}
void TestParser::case2571() {}
void TestParser::case2572() {}
void TestParser::case2573() {}
void TestParser::case2574() {}
void TestParser::case2575() {}
void TestParser::case2576() {}
void TestParser::case2577() {}
void TestParser::case2578() {}
void TestParser::case2579() {}
void TestParser::case2580() {}
void TestParser::case2581() {}
void TestParser::case2582() {}
void TestParser::case2583() {}
void TestParser::case2584() {}
void TestParser::case2585() {}
void TestParser::case2586() {}
void TestParser::case2587() {}
void TestParser::case2588() {}
void TestParser::case2589() {}
void TestParser::case2590() {}
void TestParser::case2591() {}
void TestParser::case2592() {}
void TestParser::case2593() {}
void TestParser::case2594() {}
void TestParser::case2595() {}
void TestParser::case2596() {}
void TestParser::case2597() {}
void TestParser::case2598() {}
void TestParser::case2599() {}

void TestParser::case2600()
{
    parseStatement(R"(
                   asm ( "nop" );
                   )");
}

void TestParser::case2601()
{
    parseStatement(R"(
                   asm ( "nop"
                         "nop" );
                   )");
}

void TestParser::case2602()
{
    parseStatement(R"(
                   asm ( "nop" )
                   )",
                   Expectation().addDiagnostic(Expectation::ErrorOrWarn::Error,
                                               Parser::DiagnosticsReporter::ID_of_ExpectedToken));
}

void TestParser::case2603()
{
    parseStatement(R"(
                   asm ( )
                   )",
                   Expectation().addDiagnostic(Expectation::ErrorOrWarn::Error,
                                               Parser::DiagnosticsReporter::ID_of_ExpectedTokenOfCategoryStringLiteral));
}

void TestParser::case2604()
{
    // From https://www.felixcloutier.com/documents/gcc-asm.html.

    parseStatement(R"(
                   asm ( "movq %0, %0" : "+rm" ( foo ) ) ;
                   )");
}

void TestParser::case2605()
{
    // From https://www.felixcloutier.com/documents/gcc-asm.html.

    parseStatement(R"(
                   asm ( "addl %0, %1" : "+r" ( foo ) : "g" ( bar ) ) ;
                   )");
}

void TestParser::case2606()
{
    // From https://www.felixcloutier.com/documents/gcc-asm.html.

    parseStatement(R"(
                   asm ( "lfence " :  :  : "memory" ) ;
                   )");
}

void TestParser::case2607()
{
    parseStatement(R"(
                   asm ( "movl $123 , %0 \n  "
                         "addl %1 , %0"
                         : "=&r" ( foo )
                         : "r" ( bar ) ) ;
                   )");
}

void TestParser::case2608()
{
    parseStatement(R"(
                   asm goto ( "jmp %l0\n"
                              :
                              :
                              :
                              : x , y );
                   )");
}

void TestParser::case2609()
{
    parseStatement(R"(
                   asm goto ( "jmp %l0\n"
                              :
                              :
                              :
                              : x );
                   )");
}

void TestParser::case2610()
{
    // From https://www.felixcloutier.com/documents/gcc-asm.html.

    parseStatement(R"(
                   asm volatile ( "syscall" : "=a" (ret_val) : : "rcx" , "r11" ) ;
                   )");
}

void TestParser::case2611()
{
    // From https://www.felixcloutier.com/documents/gcc-asm.html.

    parseStatement(R"(
                   asm ( "imulq %[rhs]"
                         : "=a" (*lo) , "=d" (*hi)
                         : [ lhs ] "0" ( left ) , [ rhs ] "rm" ( right )) ;
                   )");
}

void TestParser::case2612()
{
    parseStatement(R"(
                   asm ( "mov %[e], %[d]"
                      : [ d ] "=rm" ( d )
                      : [ e ] "rm" ( * e ) );
                   )");
}

void TestParser::case2613() {}
void TestParser::case2614() {}
void TestParser::case2615() {}
void TestParser::case2616() {}
void TestParser::case2617() {}
void TestParser::case2618() {}
void TestParser::case2619() {}
void TestParser::case2620() {}
void TestParser::case2621() {}
void TestParser::case2622() {}
void TestParser::case2623() {}
void TestParser::case2624() {}
void TestParser::case2625() {}
void TestParser::case2626() {}
void TestParser::case2627() {}
void TestParser::case2628() {}
void TestParser::case2629() {}
void TestParser::case2630() {}
void TestParser::case2631() {}
void TestParser::case2632() {}
void TestParser::case2633() {}
void TestParser::case2634() {}
void TestParser::case2635() {}
void TestParser::case2636() {}
void TestParser::case2637() {}
void TestParser::case2638() {}
void TestParser::case2639() {}
void TestParser::case2640() {}
void TestParser::case2641() {}
void TestParser::case2642() {}
void TestParser::case2643() {}
void TestParser::case2644() {}
void TestParser::case2645() {}
void TestParser::case2646() {}
void TestParser::case2647() {}
void TestParser::case2648() {}
void TestParser::case2649() {}
void TestParser::case2650() {}
void TestParser::case2651() {}
void TestParser::case2652() {}
void TestParser::case2653() {}
void TestParser::case2654() {}
void TestParser::case2655() {}
void TestParser::case2656() {}
void TestParser::case2657() {}
void TestParser::case2658() {}
void TestParser::case2659() {}
void TestParser::case2660() {}
void TestParser::case2661() {}
void TestParser::case2662() {}
void TestParser::case2663() {}
void TestParser::case2664() {}
void TestParser::case2665() {}
void TestParser::case2666() {}
void TestParser::case2667() {}
void TestParser::case2668() {}
void TestParser::case2669() {}
void TestParser::case2670() {}
void TestParser::case2671() {}
void TestParser::case2672() {}
void TestParser::case2673() {}
void TestParser::case2674() {}
void TestParser::case2675() {}
void TestParser::case2676() {}
void TestParser::case2677() {}
void TestParser::case2678() {}
void TestParser::case2679() {}
void TestParser::case2680() {}
void TestParser::case2681() {}
void TestParser::case2682() {}
void TestParser::case2683() {}
void TestParser::case2684() {}
void TestParser::case2685() {}
void TestParser::case2686() {}
void TestParser::case2687() {}
void TestParser::case2688() {}
void TestParser::case2689() {}
void TestParser::case2690() {}
void TestParser::case2691() {}
void TestParser::case2692() {}
void TestParser::case2693() {}
void TestParser::case2694() {}
void TestParser::case2695() {}
void TestParser::case2696() {}
void TestParser::case2697() {}
void TestParser::case2698() {}
void TestParser::case2699() {}

void TestParser::case2700() {}
void TestParser::case2701() {}
void TestParser::case2702() {}
void TestParser::case2703() {}
void TestParser::case2704() {}
void TestParser::case2705() {}
void TestParser::case2706() {}
void TestParser::case2707() {}
void TestParser::case2708() {}
void TestParser::case2709() {}
void TestParser::case2710() {}
void TestParser::case2711() {}
void TestParser::case2712() {}
void TestParser::case2713() {}
void TestParser::case2714() {}
void TestParser::case2715() {}
void TestParser::case2716() {}
void TestParser::case2717() {}
void TestParser::case2718() {}
void TestParser::case2719() {}
void TestParser::case2720() {}
void TestParser::case2721() {}
void TestParser::case2722() {}
void TestParser::case2723() {}
void TestParser::case2724() {}
void TestParser::case2725() {}
void TestParser::case2726() {}
void TestParser::case2727() {}
void TestParser::case2728() {}
void TestParser::case2729() {}
void TestParser::case2730() {}
void TestParser::case2731() {}
void TestParser::case2732() {}
void TestParser::case2733() {}
void TestParser::case2734() {}
void TestParser::case2735() {}
void TestParser::case2736() {}
void TestParser::case2737() {}
void TestParser::case2738() {}
void TestParser::case2739() {}
void TestParser::case2740() {}
void TestParser::case2741() {}
void TestParser::case2742() {}
void TestParser::case2743() {}
void TestParser::case2744() {}
void TestParser::case2745() {}
void TestParser::case2746() {}
void TestParser::case2747() {}
void TestParser::case2748() {}
void TestParser::case2749() {}
void TestParser::case2750() {}
void TestParser::case2751() {}
void TestParser::case2752() {}
void TestParser::case2753() {}
void TestParser::case2754() {}
void TestParser::case2755() {}
void TestParser::case2756() {}
void TestParser::case2757() {}
void TestParser::case2758() {}
void TestParser::case2759() {}
void TestParser::case2760() {}
void TestParser::case2761() {}
void TestParser::case2762() {}
void TestParser::case2763() {}
void TestParser::case2764() {}
void TestParser::case2765() {}
void TestParser::case2766() {}
void TestParser::case2767() {}
void TestParser::case2768() {}
void TestParser::case2769() {}
void TestParser::case2770() {}
void TestParser::case2771() {}
void TestParser::case2772() {}
void TestParser::case2773() {}
void TestParser::case2774() {}
void TestParser::case2775() {}
void TestParser::case2776() {}
void TestParser::case2777() {}
void TestParser::case2778() {}
void TestParser::case2779() {}
void TestParser::case2780() {}
void TestParser::case2781() {}
void TestParser::case2782() {}
void TestParser::case2783() {}
void TestParser::case2784() {}
void TestParser::case2785() {}
void TestParser::case2786() {}
void TestParser::case2787() {}
void TestParser::case2788() {}
void TestParser::case2789() {}
void TestParser::case2790() {}
void TestParser::case2791() {}
void TestParser::case2792() {}
void TestParser::case2793() {}
void TestParser::case2794() {}
void TestParser::case2795() {}
void TestParser::case2796() {}
void TestParser::case2797() {}
void TestParser::case2798() {}
void TestParser::case2799() {}

void TestParser::case2800() {}
void TestParser::case2801() {}
void TestParser::case2802() {}
void TestParser::case2803() {}
void TestParser::case2804() {}
void TestParser::case2805() {}
void TestParser::case2806() {}
void TestParser::case2807() {}
void TestParser::case2808() {}
void TestParser::case2809() {}
void TestParser::case2810() {}
void TestParser::case2811() {}
void TestParser::case2812() {}
void TestParser::case2813() {}
void TestParser::case2814() {}
void TestParser::case2815() {}
void TestParser::case2816() {}
void TestParser::case2817() {}
void TestParser::case2818() {}
void TestParser::case2819() {}
void TestParser::case2820() {}
void TestParser::case2821() {}
void TestParser::case2822() {}
void TestParser::case2823() {}
void TestParser::case2824() {}
void TestParser::case2825() {}
void TestParser::case2826() {}
void TestParser::case2827() {}
void TestParser::case2828() {}
void TestParser::case2829() {}
void TestParser::case2830() {}
void TestParser::case2831() {}
void TestParser::case2832() {}
void TestParser::case2833() {}
void TestParser::case2834() {}
void TestParser::case2835() {}
void TestParser::case2836() {}
void TestParser::case2837() {}
void TestParser::case2838() {}
void TestParser::case2839() {}
void TestParser::case2840() {}
void TestParser::case2841() {}
void TestParser::case2842() {}
void TestParser::case2843() {}
void TestParser::case2844() {}
void TestParser::case2845() {}
void TestParser::case2846() {}
void TestParser::case2847() {}
void TestParser::case2848() {}
void TestParser::case2849() {}
void TestParser::case2850() {}
void TestParser::case2851() {}
void TestParser::case2852() {}
void TestParser::case2853() {}
void TestParser::case2854() {}
void TestParser::case2855() {}
void TestParser::case2856() {}
void TestParser::case2857() {}
void TestParser::case2858() {}
void TestParser::case2859() {}
void TestParser::case2860() {}
void TestParser::case2861() {}
void TestParser::case2862() {}
void TestParser::case2863() {}
void TestParser::case2864() {}
void TestParser::case2865() {}
void TestParser::case2866() {}
void TestParser::case2867() {}
void TestParser::case2868() {}
void TestParser::case2869() {}
void TestParser::case2870() {}
void TestParser::case2871() {}
void TestParser::case2872() {}
void TestParser::case2873() {}
void TestParser::case2874() {}
void TestParser::case2875() {}
void TestParser::case2876() {}
void TestParser::case2877() {}
void TestParser::case2878() {}
void TestParser::case2879() {}
void TestParser::case2880() {}
void TestParser::case2881() {}
void TestParser::case2882() {}
void TestParser::case2883() {}
void TestParser::case2884() {}
void TestParser::case2885() {}
void TestParser::case2886() {}
void TestParser::case2887() {}
void TestParser::case2888() {}
void TestParser::case2889() {}
void TestParser::case2890() {}
void TestParser::case2891() {}
void TestParser::case2892() {}
void TestParser::case2893() {}
void TestParser::case2894() {}
void TestParser::case2895() {}
void TestParser::case2896() {}
void TestParser::case2897() {}
void TestParser::case2898() {}
void TestParser::case2899() {}

void TestParser::case2900() {}
void TestParser::case2901() {}
void TestParser::case2902() {}
void TestParser::case2903() {}
void TestParser::case2904() {}
void TestParser::case2905() {}
void TestParser::case2906() {}
void TestParser::case2907() {}
void TestParser::case2908() {}
void TestParser::case2909() {}
void TestParser::case2910() {}
void TestParser::case2911() {}
void TestParser::case2912() {}
void TestParser::case2913() {}
void TestParser::case2914() {}
void TestParser::case2915() {}
void TestParser::case2916() {}
void TestParser::case2917() {}
void TestParser::case2918() {}
void TestParser::case2919() {}
void TestParser::case2920() {}
void TestParser::case2921() {}
void TestParser::case2922() {}
void TestParser::case2923() {}
void TestParser::case2924() {}
void TestParser::case2925() {}
void TestParser::case2926() {}
void TestParser::case2927() {}
void TestParser::case2928() {}
void TestParser::case2929() {}
void TestParser::case2930() {}
void TestParser::case2931() {}
void TestParser::case2932() {}
void TestParser::case2933() {}
void TestParser::case2934() {}
void TestParser::case2935() {}
void TestParser::case2936() {}
void TestParser::case2937() {}
void TestParser::case2938() {}
void TestParser::case2939() {}
void TestParser::case2940() {}
void TestParser::case2941() {}
void TestParser::case2942() {}
void TestParser::case2943() {}
void TestParser::case2944() {}
void TestParser::case2945() {}
void TestParser::case2946() {}
void TestParser::case2947() {}
void TestParser::case2948() {}
void TestParser::case2949() {}
void TestParser::case2950() {}
void TestParser::case2951() {}
void TestParser::case2952() {}
void TestParser::case2953() {}
void TestParser::case2954() {}
void TestParser::case2955() {}
void TestParser::case2956() {}
void TestParser::case2957() {}
void TestParser::case2958() {}
void TestParser::case2959() {}
void TestParser::case2960() {}
void TestParser::case2961() {}
void TestParser::case2962() {}
void TestParser::case2963() {}
void TestParser::case2964() {}
void TestParser::case2965() {}
void TestParser::case2966() {}
void TestParser::case2967() {}
void TestParser::case2968() {}
void TestParser::case2969() {}
void TestParser::case2970() {}
void TestParser::case2971() {}
void TestParser::case2972() {}
void TestParser::case2973() {}
void TestParser::case2974() {}
void TestParser::case2975() {}
void TestParser::case2976() {}
void TestParser::case2977() {}
void TestParser::case2978() {}
void TestParser::case2979() {}
void TestParser::case2980() {}
void TestParser::case2981() {}
void TestParser::case2982() {}
void TestParser::case2983() {}
void TestParser::case2984() {}
void TestParser::case2985() {}
void TestParser::case2986() {}
void TestParser::case2987() {}
void TestParser::case2988() {}
void TestParser::case2989() {}
void TestParser::case2990() {}
void TestParser::case2991() {}
void TestParser::case2992() {}
void TestParser::case2993() {}
void TestParser::case2994() {}
void TestParser::case2995() {}
void TestParser::case2996() {}
void TestParser::case2997() {}
void TestParser::case2998() {}
void TestParser::case2999() {}
