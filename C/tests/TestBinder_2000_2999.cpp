// Copyright (c) 2021 Leandro T. C. Melo <ltcmelo@gmail.com>
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

#include "TestBinder.h"

#include "TestParser.h"

#include "binder/Semantics_TypeSpecifiers.h"
#include "parser/Unparser.h"
#include "symbols/Symbol.h"
#include "symbols/Symbols.h"
#include "syntax/SyntaxLexemes.h"
#include "syntax/SyntaxNodes.h"

#include <algorithm>
#include <cstring>
#include <iostream>
#include <string>
#include <sstream>

using namespace psy;
using namespace C;

void TestBinder::case2000()
{
    bind(R"(
struct w
{
   double x ;
};
         )",
         Expectation()
            .obj("x", ValueKind::Field, "double", TypeKind::Builtin, BuiltinTypeKind::Double));
}

void TestBinder::case2001()
{
    bind(R"(
struct w
{
    int x ;
};
         )",
         Expectation()
            .obj("x", ValueKind::Field, "int", TypeKind::Builtin, BuiltinTypeKind::Int));
}

void TestBinder::case2002()
{
    bind(R"(
struct w
{
    int x ;
    int y ;
};
         )",
         Expectation()
            .obj("x", ValueKind::Field, "int", TypeKind::Builtin, BuiltinTypeKind::Int)
            .obj("x", ValueKind::Field, "int", TypeKind::Builtin, BuiltinTypeKind::Int));
}

void TestBinder::case2003()
{
    bind(R"(
struct w
{
    int x , y ;
};
         )",
         Expectation()
            .obj("x", ValueKind::Field, "int", TypeKind::Builtin, BuiltinTypeKind::Int)
            .obj("y", ValueKind::Field, "int", TypeKind::Builtin, BuiltinTypeKind::Int));
}

void TestBinder::case2004()
{
    bind(R"(
struct w
{
    x y ;
};
         )",
         Expectation()
            .obj("y", ValueKind::Field, "x", TypeKind::Synonym));
}

void TestBinder::case2005()
{
    bind(R"(
struct w
{
    x y , z ;
};
         )",
         Expectation()
            .obj("y", ValueKind::Field, "x", TypeKind::Synonym)
            .obj("z", ValueKind::Field, "x", TypeKind::Synonym));
}

void TestBinder::case2006()
{
    // Non-field correspondent
    CROSS_REFERENCE_TEST(TestBinder::case1006);

    // Syntax error
    CROSS_REFERENCE_TEST(TestParser::case0439);
}

void TestBinder::case2007()
{
    // Non-field correspondent
    CROSS_REFERENCE_TEST(TestBinder::case1007);

    // Syntax error
    CROSS_REFERENCE_TEST(TestParser::case0440);
}

void TestBinder::case2008()
{
    bind(R"(
struct w
{
    int float x ;
};
         )",
         Expectation().addDiagnostic(
             Expectation::ErrorOrWarn::Error,
             Semantics_TypeSpecifiers::ID_TwoOrMoreDataTypesInDeclarationSpecifiers));
}

void TestBinder::case2009(){ }
void TestBinder::case2010(){ }
void TestBinder::case2011(){ }
void TestBinder::case2012(){ }
void TestBinder::case2013(){ }
void TestBinder::case2014(){ }
void TestBinder::case2015(){ }
void TestBinder::case2016(){ }
void TestBinder::case2017(){ }
void TestBinder::case2018(){ }
void TestBinder::case2019(){ }
void TestBinder::case2020(){ }
void TestBinder::case2021(){ }
void TestBinder::case2022(){ }
void TestBinder::case2023(){ }
void TestBinder::case2024(){ }
void TestBinder::case2025(){ }
void TestBinder::case2026(){ }
void TestBinder::case2027(){ }
void TestBinder::case2028(){ }
void TestBinder::case2029(){ }
void TestBinder::case2030(){ }
void TestBinder::case2031(){ }
void TestBinder::case2032(){ }
void TestBinder::case2033(){ }
void TestBinder::case2034(){ }
void TestBinder::case2035(){ }
void TestBinder::case2036(){ }
void TestBinder::case2037(){ }
void TestBinder::case2038(){ }
void TestBinder::case2039(){ }
void TestBinder::case2040(){ }
void TestBinder::case2041(){ }
void TestBinder::case2042(){ }
void TestBinder::case2043(){ }
void TestBinder::case2044(){ }
void TestBinder::case2045(){ }
void TestBinder::case2046(){ }
void TestBinder::case2047(){ }
void TestBinder::case2048(){ }
void TestBinder::case2049(){ }

void TestBinder::case2050()
{
    bind(R"(
struct x
{
    int * const y ;
};
         )",
         Expectation()
            .qualPtr_1("y", ValueKind::Field, Expectation::Qual::Const, TypeKind::Builtin, BuiltinTypeKind::Int));
}

void TestBinder::case2051()
{
    bind(R"(
struct x
{
    y * const z ;
};
         )",
         Expectation()
            .qualPtr_1("z", ValueKind::Field, Expectation::Qual::Const, TypeKind::Synonym));
}

void TestBinder::case2052(){ }
void TestBinder::case2053(){ }
void TestBinder::case2054(){ }
void TestBinder::case2055(){ }
void TestBinder::case2056(){ }
void TestBinder::case2057(){ }
void TestBinder::case2058(){ }
void TestBinder::case2059(){ }
void TestBinder::case2060(){ }
void TestBinder::case2061(){ }
void TestBinder::case2062(){ }
void TestBinder::case2063(){ }
void TestBinder::case2064(){ }
void TestBinder::case2065(){ }
void TestBinder::case2066(){ }
void TestBinder::case2067(){ }
void TestBinder::case2068(){ }
void TestBinder::case2069(){ }
void TestBinder::case2070(){ }
void TestBinder::case2071(){ }
void TestBinder::case2072(){ }
void TestBinder::case2073(){ }
void TestBinder::case2074(){ }
void TestBinder::case2075(){ }
void TestBinder::case2076(){ }
void TestBinder::case2077(){ }
void TestBinder::case2078(){ }
void TestBinder::case2079(){ }
void TestBinder::case2080(){ }
void TestBinder::case2081(){ }
void TestBinder::case2082(){ }
void TestBinder::case2083(){ }
void TestBinder::case2084(){ }
void TestBinder::case2085(){ }
void TestBinder::case2086(){ }
void TestBinder::case2087(){ }
void TestBinder::case2088(){ }
void TestBinder::case2089(){ }
void TestBinder::case2090(){ }
void TestBinder::case2091(){ }
void TestBinder::case2092(){ }
void TestBinder::case2093(){ }
void TestBinder::case2094(){ }
void TestBinder::case2095(){ }
void TestBinder::case2096(){ }
void TestBinder::case2097(){ }
void TestBinder::case2098(){ }
void TestBinder::case2099(){ }

void TestBinder::case2100(){ }
void TestBinder::case2101(){ }
void TestBinder::case2102(){ }
void TestBinder::case2103(){ }
void TestBinder::case2104(){ }
void TestBinder::case2105(){ }
void TestBinder::case2106(){ }
void TestBinder::case2107(){ }
void TestBinder::case2108(){ }
void TestBinder::case2109(){ }
void TestBinder::case2110(){ }
void TestBinder::case2111(){ }
void TestBinder::case2112(){ }
void TestBinder::case2113(){ }
void TestBinder::case2114(){ }
void TestBinder::case2115(){ }
void TestBinder::case2116(){ }
void TestBinder::case2117(){ }
void TestBinder::case2118(){ }
void TestBinder::case2119(){ }
void TestBinder::case2120(){ }
void TestBinder::case2121(){ }
void TestBinder::case2122(){ }
void TestBinder::case2123(){ }
void TestBinder::case2124(){ }
void TestBinder::case2125(){ }
void TestBinder::case2126(){ }
void TestBinder::case2127(){ }
void TestBinder::case2128(){ }
void TestBinder::case2129(){ }
void TestBinder::case2130(){ }
void TestBinder::case2131(){ }
void TestBinder::case2132(){ }
void TestBinder::case2133(){ }
void TestBinder::case2134(){ }
void TestBinder::case2135(){ }
void TestBinder::case2136(){ }
void TestBinder::case2137(){ }
void TestBinder::case2138(){ }
void TestBinder::case2139(){ }
void TestBinder::case2140(){ }
void TestBinder::case2141(){ }
void TestBinder::case2142(){ }
void TestBinder::case2143(){ }
void TestBinder::case2144(){ }
void TestBinder::case2145(){ }
void TestBinder::case2146(){ }
void TestBinder::case2147(){ }
void TestBinder::case2148(){ }
void TestBinder::case2149(){ }
void TestBinder::case2150(){ }
void TestBinder::case2151(){ }
void TestBinder::case2152(){ }
void TestBinder::case2153(){ }
void TestBinder::case2154(){ }
void TestBinder::case2155(){ }
void TestBinder::case2156(){ }
void TestBinder::case2157(){ }
void TestBinder::case2158(){ }
void TestBinder::case2159(){ }
void TestBinder::case2160(){ }
void TestBinder::case2161(){ }
void TestBinder::case2162(){ }
void TestBinder::case2163(){ }
void TestBinder::case2164(){ }
void TestBinder::case2165(){ }
void TestBinder::case2166(){ }
void TestBinder::case2167(){ }
void TestBinder::case2168(){ }
void TestBinder::case2169(){ }
void TestBinder::case2170(){ }
void TestBinder::case2171(){ }
void TestBinder::case2172(){ }
void TestBinder::case2173(){ }
void TestBinder::case2174(){ }
void TestBinder::case2175(){ }
void TestBinder::case2176(){ }
void TestBinder::case2177(){ }
void TestBinder::case2178(){ }
void TestBinder::case2179(){ }
void TestBinder::case2180(){ }
void TestBinder::case2181(){ }
void TestBinder::case2182(){ }
void TestBinder::case2183(){ }
void TestBinder::case2184(){ }
void TestBinder::case2185(){ }
void TestBinder::case2186(){ }
void TestBinder::case2187(){ }
void TestBinder::case2188(){ }
void TestBinder::case2189(){ }
void TestBinder::case2190(){ }
void TestBinder::case2191(){ }
void TestBinder::case2192(){ }
void TestBinder::case2193(){ }
void TestBinder::case2194(){ }
void TestBinder::case2195(){ }
void TestBinder::case2196(){ }
void TestBinder::case2197(){ }
void TestBinder::case2198(){ }
void TestBinder::case2199(){ }

void TestBinder::case2200(){ }
void TestBinder::case2201(){ }
void TestBinder::case2202(){ }
void TestBinder::case2203(){ }
void TestBinder::case2204(){ }
void TestBinder::case2205(){ }
void TestBinder::case2206(){ }
void TestBinder::case2207(){ }
void TestBinder::case2208(){ }
void TestBinder::case2209(){ }
void TestBinder::case2210(){ }
void TestBinder::case2211(){ }
void TestBinder::case2212(){ }
void TestBinder::case2213(){ }
void TestBinder::case2214(){ }
void TestBinder::case2215(){ }
void TestBinder::case2216(){ }
void TestBinder::case2217(){ }
void TestBinder::case2218(){ }
void TestBinder::case2219(){ }
void TestBinder::case2220(){ }
void TestBinder::case2221(){ }
void TestBinder::case2222(){ }
void TestBinder::case2223(){ }
void TestBinder::case2224(){ }
void TestBinder::case2225(){ }
void TestBinder::case2226(){ }
void TestBinder::case2227(){ }
void TestBinder::case2228(){ }
void TestBinder::case2229(){ }
void TestBinder::case2230(){ }
void TestBinder::case2231(){ }
void TestBinder::case2232(){ }
void TestBinder::case2233(){ }
void TestBinder::case2234(){ }
void TestBinder::case2235(){ }
void TestBinder::case2236(){ }
void TestBinder::case2237(){ }
void TestBinder::case2238(){ }
void TestBinder::case2239(){ }
void TestBinder::case2240(){ }
void TestBinder::case2241(){ }
void TestBinder::case2242(){ }
void TestBinder::case2243(){ }
void TestBinder::case2244(){ }
void TestBinder::case2245(){ }
void TestBinder::case2246(){ }
void TestBinder::case2247(){ }
void TestBinder::case2248(){ }
void TestBinder::case2249(){ }
void TestBinder::case2250(){ }
void TestBinder::case2251(){ }
void TestBinder::case2252(){ }
void TestBinder::case2253(){ }
void TestBinder::case2254(){ }
void TestBinder::case2255(){ }
void TestBinder::case2256(){ }
void TestBinder::case2257(){ }
void TestBinder::case2258(){ }
void TestBinder::case2259(){ }
void TestBinder::case2260(){ }
void TestBinder::case2261(){ }
void TestBinder::case2262(){ }
void TestBinder::case2263(){ }
void TestBinder::case2264(){ }
void TestBinder::case2265(){ }
void TestBinder::case2266(){ }
void TestBinder::case2267(){ }
void TestBinder::case2268(){ }
void TestBinder::case2269(){ }
void TestBinder::case2270(){ }
void TestBinder::case2271(){ }
void TestBinder::case2272(){ }
void TestBinder::case2273(){ }
void TestBinder::case2274(){ }
void TestBinder::case2275(){ }
void TestBinder::case2276(){ }
void TestBinder::case2277(){ }
void TestBinder::case2278(){ }
void TestBinder::case2279(){ }
void TestBinder::case2280(){ }
void TestBinder::case2281(){ }
void TestBinder::case2282(){ }
void TestBinder::case2283(){ }
void TestBinder::case2284(){ }
void TestBinder::case2285(){ }
void TestBinder::case2286(){ }
void TestBinder::case2287(){ }
void TestBinder::case2288(){ }
void TestBinder::case2289(){ }
void TestBinder::case2290(){ }
void TestBinder::case2291(){ }
void TestBinder::case2292(){ }
void TestBinder::case2293(){ }
void TestBinder::case2294(){ }
void TestBinder::case2295(){ }
void TestBinder::case2296(){ }
void TestBinder::case2297(){ }
void TestBinder::case2298(){ }
void TestBinder::case2299(){ }

void TestBinder::case2300(){ }
void TestBinder::case2301(){ }
void TestBinder::case2302(){ }
void TestBinder::case2303(){ }
void TestBinder::case2304(){ }
void TestBinder::case2305(){ }
void TestBinder::case2306(){ }
void TestBinder::case2307(){ }
void TestBinder::case2308(){ }
void TestBinder::case2309(){ }
void TestBinder::case2310(){ }
void TestBinder::case2311(){ }
void TestBinder::case2312(){ }
void TestBinder::case2313(){ }
void TestBinder::case2314(){ }
void TestBinder::case2315(){ }
void TestBinder::case2316(){ }
void TestBinder::case2317(){ }
void TestBinder::case2318(){ }
void TestBinder::case2319(){ }
void TestBinder::case2320(){ }
void TestBinder::case2321(){ }
void TestBinder::case2322(){ }
void TestBinder::case2323(){ }
void TestBinder::case2324(){ }
void TestBinder::case2325(){ }
void TestBinder::case2326(){ }
void TestBinder::case2327(){ }
void TestBinder::case2328(){ }
void TestBinder::case2329(){ }
void TestBinder::case2330(){ }
void TestBinder::case2331(){ }
void TestBinder::case2332(){ }
void TestBinder::case2333(){ }
void TestBinder::case2334(){ }
void TestBinder::case2335(){ }
void TestBinder::case2336(){ }
void TestBinder::case2337(){ }
void TestBinder::case2338(){ }
void TestBinder::case2339(){ }
void TestBinder::case2340(){ }
void TestBinder::case2341(){ }
void TestBinder::case2342(){ }
void TestBinder::case2343(){ }
void TestBinder::case2344(){ }
void TestBinder::case2345(){ }
void TestBinder::case2346(){ }
void TestBinder::case2347(){ }
void TestBinder::case2348(){ }
void TestBinder::case2349(){ }
void TestBinder::case2350(){ }
void TestBinder::case2351(){ }
void TestBinder::case2352(){ }
void TestBinder::case2353(){ }
void TestBinder::case2354(){ }
void TestBinder::case2355(){ }
void TestBinder::case2356(){ }
void TestBinder::case2357(){ }
void TestBinder::case2358(){ }
void TestBinder::case2359(){ }
void TestBinder::case2360(){ }
void TestBinder::case2361(){ }
void TestBinder::case2362(){ }
void TestBinder::case2363(){ }
void TestBinder::case2364(){ }
void TestBinder::case2365(){ }
void TestBinder::case2366(){ }
void TestBinder::case2367(){ }
void TestBinder::case2368(){ }
void TestBinder::case2369(){ }
void TestBinder::case2370(){ }
void TestBinder::case2371(){ }
void TestBinder::case2372(){ }
void TestBinder::case2373(){ }
void TestBinder::case2374(){ }
void TestBinder::case2375(){ }
void TestBinder::case2376(){ }
void TestBinder::case2377(){ }
void TestBinder::case2378(){ }
void TestBinder::case2379(){ }
void TestBinder::case2380(){ }
void TestBinder::case2381(){ }
void TestBinder::case2382(){ }
void TestBinder::case2383(){ }
void TestBinder::case2384(){ }
void TestBinder::case2385(){ }
void TestBinder::case2386(){ }
void TestBinder::case2387(){ }
void TestBinder::case2388(){ }
void TestBinder::case2389(){ }
void TestBinder::case2390(){ }
void TestBinder::case2391(){ }
void TestBinder::case2392(){ }
void TestBinder::case2393(){ }
void TestBinder::case2394(){ }
void TestBinder::case2395(){ }
void TestBinder::case2396(){ }
void TestBinder::case2397(){ }
void TestBinder::case2398(){ }
void TestBinder::case2399(){ }
