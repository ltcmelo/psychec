// Copyright (c) 2021/22 Leandro T. C. Melo <ltcmelo@gmail.com>
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

#include "BinderTest.h"

#include "ParserTest.h"

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

void BinderTest::case2000()
{
    bind(R"(
struct
{
   double x ;
};
         )",
         Expectation()
            .binding(BindingSummary("x", ValueKind::Field)
                    .specType("double", NamedTypeKind::Builtin, BuiltinTypeKind::Double)));
}

void BinderTest::case2001()
{
    bind(R"(
struct
{
    int x ;
};
         )",
         Expectation()
            .binding(BindingSummary("x", ValueKind::Field)
                     .specType("int", NamedTypeKind::Builtin, BuiltinTypeKind::Int)));
}

void BinderTest::case2002()
{
    bind(R"(
struct w
{
    int x ;
    int y ;
};
         )",
         Expectation()
            .binding(BindingSummary("x", ValueKind::Field)
                    .specType("int", NamedTypeKind::Builtin, BuiltinTypeKind::Int))
            .binding(BindingSummary("y", ValueKind::Field)
                    .specType("int", NamedTypeKind::Builtin, BuiltinTypeKind::Int)));
}

void BinderTest::case2003()
{
    bind(R"(
struct
{
    int x , y ;
};
         )",
         Expectation()
             .binding(BindingSummary("x", ValueKind::Field)
                     .specType("int", NamedTypeKind::Builtin, BuiltinTypeKind::Int))
             .binding(BindingSummary("y", ValueKind::Field)
                     .specType("int", NamedTypeKind::Builtin, BuiltinTypeKind::Int)));
}

void BinderTest::case2004()
{
    bind(R"(
struct
{
    x y ;
};
         )",
         Expectation()
             .binding(BindingSummary("y", ValueKind::Field)
                 .specType("x", NamedTypeKind::Synonym, BuiltinTypeKind::None)));
}

void BinderTest::case2005()
{
    bind(R"(
struct
{
    x y , z ;
};
         )",
         Expectation()
             .binding(BindingSummary("y", ValueKind::Field)
                      .specType("x", NamedTypeKind::Synonym, BuiltinTypeKind::None))
             .binding(BindingSummary("z", ValueKind::Field)
                      .specType("x", NamedTypeKind::Synonym, BuiltinTypeKind::None)));
}

void BinderTest::case2006()
{
    // Non-field correspondent
    CROSS_REFERENCE_TEST(BinderTest::case2006);

    // Syntax error
    CROSS_REFERENCE_TEST(ParserTest::case0439);
}

void BinderTest::case2007()
{
    // Non-field correspondent
    CROSS_REFERENCE_TEST(BinderTest::case2007);

    // Syntax error
    CROSS_REFERENCE_TEST(ParserTest::case0440);
}

void BinderTest::case2008()
{
    bind(R"(
struct
{
    int float x ;
};
         )",
         Expectation().diagnostic(
             Expectation::ErrorOrWarn::Error,
             Semantics_TypeSpecifiers::ID_TwoOrMoreDataTypesInDeclarationSpecifiers));
}

void BinderTest::case2009()
{
    bind(R"(
struct
{
    struct x y ;
};
         )",
         Expectation()
            .binding(BindingSummary("y", ValueKind::Field)
                    .specType("struct x", NamedTypeKind::Tag, BuiltinTypeKind::None)));
}

void BinderTest::case2010()
{
    bind(R"(
struct
{
    union x y ;
};
         )",
         Expectation()
            .binding(BindingSummary("y", ValueKind::Field)
                    .specType("union x", NamedTypeKind::Tag, BuiltinTypeKind::None)));
}

void BinderTest::case2011()
{
    bind(R"(
struct
{
    enum x y ;
};
         )",
         Expectation()
            .binding(BindingSummary("y", ValueKind::Field)
                    .specType("enum x", NamedTypeKind::Tag, BuiltinTypeKind::None)));
}

void BinderTest::case2012()
{
    bind(R"(
struct
{
    struct x y , z ;
};
         )",
         Expectation()
            .binding(BindingSummary("y", ValueKind::Field)
                    .specType("struct x", NamedTypeKind::Tag, BuiltinTypeKind::None))
            .binding(BindingSummary("z", ValueKind::Field)
                    .specType("struct x", NamedTypeKind::Tag, BuiltinTypeKind::None)));
}

void BinderTest::case2013()
{
    bind(R"(
struct
{
    struct x { int y ; } z ;
};
         )",
         Expectation()
            .binding(BindingSummary("z", ValueKind::Field)
                    .specType("struct x", NamedTypeKind::Tag, BuiltinTypeKind::None)));
}

void BinderTest::case2014(){ }
void BinderTest::case2015(){ }
void BinderTest::case2016(){ }
void BinderTest::case2017(){ }
void BinderTest::case2018(){ }
void BinderTest::case2019(){ }
void BinderTest::case2020(){ }
void BinderTest::case2021(){ }
void BinderTest::case2022(){ }
void BinderTest::case2023(){ }
void BinderTest::case2024(){ }
void BinderTest::case2025(){ }
void BinderTest::case2026(){ }
void BinderTest::case2027(){ }
void BinderTest::case2028(){ }
void BinderTest::case2029(){ }
void BinderTest::case2030(){ }
void BinderTest::case2031(){ }
void BinderTest::case2032(){ }
void BinderTest::case2033(){ }
void BinderTest::case2034(){ }
void BinderTest::case2035(){ }
void BinderTest::case2036(){ }
void BinderTest::case2037(){ }
void BinderTest::case2038(){ }
void BinderTest::case2039(){ }
void BinderTest::case2040(){ }
void BinderTest::case2041(){ }
void BinderTest::case2042(){ }
void BinderTest::case2043(){ }
void BinderTest::case2044(){ }
void BinderTest::case2045(){ }
void BinderTest::case2046(){ }
void BinderTest::case2047(){ }
void BinderTest::case2048(){ }
void BinderTest::case2049(){ }

void BinderTest::case2050()
{
    bind("struct { const int x ; } ;",
         Expectation()
             .binding(BindingSummary("x", ValueKind::Field)
                      .specType("int", NamedTypeKind::Builtin, BuiltinTypeKind::Int, CVR::Const)));
}

void BinderTest::case2051()
{
    bind("struct { const x y ; } ;",
         Expectation()
             .binding(BindingSummary("y", ValueKind::Field)
                      .specType("x", NamedTypeKind::Synonym, BuiltinTypeKind::None, CVR::Const)));
}

void BinderTest::case2052()
{
    bind("struct { const x ; } ;",
         Expectation().diagnostic(
             Expectation::ErrorOrWarn::Error,
             Binder::DiagnosticsReporter::ID_of_UselessDeclaration));
}

void BinderTest::case2053()
{
    bind("struct { int const x ; } ;",
         Expectation()
             .binding(BindingSummary("x", ValueKind::Field)
                      .specType("int", NamedTypeKind::Builtin, BuiltinTypeKind::Int, CVR::Const)));
}

void BinderTest::case2054()
{
    bind("struct { x const y ; } ;",
         Expectation()
             .binding(BindingSummary("y", ValueKind::Field)
                      .specType("x", NamedTypeKind::Synonym, BuiltinTypeKind::None, CVR::Const)));
}

void BinderTest::case2055(){ }
void BinderTest::case2056(){ }
void BinderTest::case2057(){ }
void BinderTest::case2058(){ }
void BinderTest::case2059(){ }
void BinderTest::case2060(){ }
void BinderTest::case2061(){ }
void BinderTest::case2062(){ }
void BinderTest::case2063(){ }
void BinderTest::case2064(){ }
void BinderTest::case2065(){ }
void BinderTest::case2066(){ }
void BinderTest::case2067(){ }
void BinderTest::case2068(){ }
void BinderTest::case2069(){ }
void BinderTest::case2070(){ }
void BinderTest::case2071(){ }
void BinderTest::case2072(){ }
void BinderTest::case2073(){ }
void BinderTest::case2074(){ }
void BinderTest::case2075(){ }
void BinderTest::case2076(){ }
void BinderTest::case2077(){ }
void BinderTest::case2078(){ }
void BinderTest::case2079(){ }
void BinderTest::case2080(){ }
void BinderTest::case2081(){ }
void BinderTest::case2082(){ }
void BinderTest::case2083(){ }
void BinderTest::case2084(){ }
void BinderTest::case2085(){ }
void BinderTest::case2086(){ }
void BinderTest::case2087(){ }
void BinderTest::case2088(){ }
void BinderTest::case2089(){ }
void BinderTest::case2090(){ }
void BinderTest::case2091(){ }
void BinderTest::case2092(){ }
void BinderTest::case2093(){ }
void BinderTest::case2094(){ }
void BinderTest::case2095(){ }
void BinderTest::case2096(){ }
void BinderTest::case2097(){ }
void BinderTest::case2098(){ }
void BinderTest::case2099(){ }

void BinderTest::case2100()
{
    bind("struct { int * x ; } ;",
         Expectation()
             .binding(BindingSummary("x", ValueKind::Field)
                      .specType("int", NamedTypeKind::Builtin, BuiltinTypeKind::Int)
                      .derivType(TypeKind::Pointer)));
}

void BinderTest::case2101()
{
    bind("struct { x * y ; } ;",
         Expectation()
             .binding(BindingSummary("y", ValueKind::Field)
                      .specType("x", NamedTypeKind::Synonym)
                      .derivType(TypeKind::Pointer)));
}

void BinderTest::case2102()
{
    bind("struct { int * x ; y * z ; } ;",
         Expectation()
             .binding(BindingSummary("x", ValueKind::Field)
                      .specType("int", NamedTypeKind::Builtin, BuiltinTypeKind::Int)
                      .derivType(TypeKind::Pointer))
             .binding(BindingSummary("z", ValueKind::Field)
                      .specType("y", NamedTypeKind::Synonym)
                      .derivType(TypeKind::Pointer)));
}

void BinderTest::case2103()
{
    bind("struct { int * x , * y ; } ;",
         Expectation()
             .binding(BindingSummary("x", ValueKind::Field)
                      .specType("int", NamedTypeKind::Builtin, BuiltinTypeKind::Int)
                      .derivType(TypeKind::Pointer))
             .binding(BindingSummary("y", ValueKind::Field)
                      .specType("int", NamedTypeKind::Builtin, BuiltinTypeKind::Int)
                      .derivType(TypeKind::Pointer)));
}

void BinderTest::case2104()
{
    bind("struct { int ( * x ) [ 1 ]; } ;",
         Expectation()
             .binding(BindingSummary("x", ValueKind::Field)
                      .specType("int", NamedTypeKind::Builtin, BuiltinTypeKind::Int)
                      .derivType(TypeKind::Array)
                      .derivType(TypeKind::Pointer)));
}

void BinderTest::case2105()
{
    bind("struct { int * * x ; } ;",
         Expectation()
             .binding(BindingSummary("x", ValueKind::Field)
                      .specType("int", NamedTypeKind::Builtin, BuiltinTypeKind::Int)
                      .derivType(TypeKind::Pointer)
                      .derivType(TypeKind::Pointer)));
}

void BinderTest::case2106()
{
    bind("struct { int * * * x ; } ;",
         Expectation()
             .binding(BindingSummary("x", ValueKind::Field)
                      .specType("int", NamedTypeKind::Builtin, BuiltinTypeKind::Int)
                      .derivType(TypeKind::Pointer)
                      .derivType(TypeKind::Pointer)
                      .derivType(TypeKind::Pointer)));
}

void BinderTest::case2107()
{
    bind("struct { int * ( * x ) [ 1 ] ; };",
         Expectation()
             .binding(BindingSummary("x", ValueKind::Field)
                      .specType("int", NamedTypeKind::Builtin, BuiltinTypeKind::Int, CVR::None)
                      .derivType(TypeKind::Pointer, CVR::None)
                      .derivType(TypeKind::Array, CVR::None)
                      .derivType(TypeKind::Pointer, CVR::None)));
}

void BinderTest::case2108(){ }
void BinderTest::case2109(){ }
void BinderTest::case2110(){ }
void BinderTest::case2111(){ }
void BinderTest::case2112(){ }
void BinderTest::case2113(){ }
void BinderTest::case2114(){ }
void BinderTest::case2115(){ }
void BinderTest::case2116(){ }
void BinderTest::case2117(){ }
void BinderTest::case2118(){ }
void BinderTest::case2119(){ }
void BinderTest::case2120(){ }
void BinderTest::case2121(){ }
void BinderTest::case2122(){ }
void BinderTest::case2123(){ }
void BinderTest::case2124(){ }
void BinderTest::case2125(){ }
void BinderTest::case2126(){ }
void BinderTest::case2127(){ }
void BinderTest::case2128(){ }
void BinderTest::case2129(){ }
void BinderTest::case2130(){ }
void BinderTest::case2131(){ }
void BinderTest::case2132(){ }
void BinderTest::case2133(){ }
void BinderTest::case2134(){ }
void BinderTest::case2135(){ }
void BinderTest::case2136(){ }
void BinderTest::case2137(){ }
void BinderTest::case2138(){ }
void BinderTest::case2139(){ }
void BinderTest::case2140(){ }
void BinderTest::case2141(){ }
void BinderTest::case2142(){ }
void BinderTest::case2143(){ }
void BinderTest::case2144(){ }
void BinderTest::case2145(){ }
void BinderTest::case2146(){ }
void BinderTest::case2147(){ }
void BinderTest::case2148(){ }
void BinderTest::case2149(){ }

void BinderTest::case2150()
{
    bind("struct { const int * x ; } ;",
         Expectation()
             .binding(BindingSummary("x", ValueKind::Field)
                      .specType("int", NamedTypeKind::Builtin, BuiltinTypeKind::Int, CVR::Const)
                      .derivType(TypeKind::Pointer)));
}

void BinderTest::case2151()
{
    bind("struct { const x * y ; } ;",
         Expectation()
             .binding(BindingSummary("y", ValueKind::Field)
                      .specType("x", NamedTypeKind::Synonym, BuiltinTypeKind::None, CVR::Const)
                      .derivType(TypeKind::Pointer)));
}

void BinderTest::case2152(){ }
void BinderTest::case2153(){ }
void BinderTest::case2154(){ }
void BinderTest::case2155(){ }
void BinderTest::case2156(){ }
void BinderTest::case2157(){ }
void BinderTest::case2158(){ }
void BinderTest::case2159(){ }
void BinderTest::case2160(){ }
void BinderTest::case2161(){ }
void BinderTest::case2162(){ }
void BinderTest::case2163(){ }
void BinderTest::case2164(){ }
void BinderTest::case2165(){ }
void BinderTest::case2166(){ }
void BinderTest::case2167(){ }
void BinderTest::case2168(){ }
void BinderTest::case2169(){ }
void BinderTest::case2170(){ }
void BinderTest::case2171(){ }
void BinderTest::case2172(){ }
void BinderTest::case2173(){ }
void BinderTest::case2174(){ }
void BinderTest::case2175(){ }
void BinderTest::case2176(){ }
void BinderTest::case2177(){ }
void BinderTest::case2178(){ }
void BinderTest::case2179(){ }
void BinderTest::case2180(){ }
void BinderTest::case2181(){ }
void BinderTest::case2182(){ }
void BinderTest::case2183(){ }
void BinderTest::case2184(){ }
void BinderTest::case2185(){ }
void BinderTest::case2186(){ }
void BinderTest::case2187(){ }
void BinderTest::case2188(){ }
void BinderTest::case2189(){ }
void BinderTest::case2190(){ }
void BinderTest::case2191(){ }
void BinderTest::case2192(){ }
void BinderTest::case2193(){ }
void BinderTest::case2194(){ }
void BinderTest::case2195(){ }
void BinderTest::case2196(){ }
void BinderTest::case2197(){ }
void BinderTest::case2198(){ }
void BinderTest::case2199(){ }

void BinderTest::case2200()
{
    bind("struct { const int * const x ; } ;",
         Expectation()
             .binding(BindingSummary("x", ValueKind::Field)
                      .specType("int", NamedTypeKind::Builtin, BuiltinTypeKind::Int, CVR::Const)
                      .derivType(TypeKind::Pointer, CVR::Const)));
}


void BinderTest::case2201(){ }
void BinderTest::case2202(){ }
void BinderTest::case2203(){ }
void BinderTest::case2204(){ }
void BinderTest::case2205(){ }
void BinderTest::case2206(){ }
void BinderTest::case2207(){ }
void BinderTest::case2208(){ }
void BinderTest::case2209(){ }
void BinderTest::case2210(){ }
void BinderTest::case2211(){ }
void BinderTest::case2212(){ }
void BinderTest::case2213(){ }
void BinderTest::case2214(){ }
void BinderTest::case2215(){ }
void BinderTest::case2216(){ }
void BinderTest::case2217(){ }
void BinderTest::case2218(){ }
void BinderTest::case2219(){ }
void BinderTest::case2220(){ }
void BinderTest::case2221(){ }
void BinderTest::case2222(){ }
void BinderTest::case2223(){ }
void BinderTest::case2224(){ }
void BinderTest::case2225(){ }
void BinderTest::case2226(){ }
void BinderTest::case2227(){ }
void BinderTest::case2228(){ }
void BinderTest::case2229(){ }
void BinderTest::case2230(){ }
void BinderTest::case2231(){ }
void BinderTest::case2232(){ }
void BinderTest::case2233(){ }
void BinderTest::case2234(){ }
void BinderTest::case2235(){ }
void BinderTest::case2236(){ }
void BinderTest::case2237(){ }
void BinderTest::case2238(){ }
void BinderTest::case2239(){ }
void BinderTest::case2240(){ }
void BinderTest::case2241(){ }
void BinderTest::case2242(){ }
void BinderTest::case2243(){ }
void BinderTest::case2244(){ }
void BinderTest::case2245(){ }
void BinderTest::case2246(){ }
void BinderTest::case2247(){ }
void BinderTest::case2248(){ }
void BinderTest::case2249(){ }

void BinderTest::case2250()
{
    bind("struct { int * const x ; } ;",
         Expectation()
             .binding(BindingSummary("x", ValueKind::Field)
                      .specType("int", NamedTypeKind::Builtin, BuiltinTypeKind::Int, CVR::None)
                      .derivType(TypeKind::Pointer, CVR::Const)));
}

void BinderTest::case2251()
{
    bind("struct { x * const y ; } ;",
         Expectation()
             .binding(BindingSummary("y", ValueKind::Field)
                      .specType("x", NamedTypeKind::Synonym, BuiltinTypeKind::None, CVR::None)
                      .derivType(TypeKind::Pointer, CVR::Const)));
}

void BinderTest::case2252(){ }
void BinderTest::case2253(){ }
void BinderTest::case2254(){ }
void BinderTest::case2255(){ }
void BinderTest::case2256(){ }
void BinderTest::case2257(){ }
void BinderTest::case2258(){ }
void BinderTest::case2259(){ }
void BinderTest::case2260(){ }
void BinderTest::case2261(){ }
void BinderTest::case2262(){ }
void BinderTest::case2263(){ }
void BinderTest::case2264(){ }
void BinderTest::case2265(){ }
void BinderTest::case2266(){ }
void BinderTest::case2267(){ }
void BinderTest::case2268(){ }
void BinderTest::case2269(){ }
void BinderTest::case2270(){ }
void BinderTest::case2271(){ }
void BinderTest::case2272(){ }
void BinderTest::case2273(){ }
void BinderTest::case2274(){ }
void BinderTest::case2275(){ }
void BinderTest::case2276(){ }
void BinderTest::case2277(){ }
void BinderTest::case2278(){ }
void BinderTest::case2279(){ }
void BinderTest::case2280(){ }
void BinderTest::case2281(){ }
void BinderTest::case2282(){ }
void BinderTest::case2283(){ }
void BinderTest::case2284(){ }
void BinderTest::case2285(){ }
void BinderTest::case2286(){ }
void BinderTest::case2287(){ }
void BinderTest::case2288(){ }
void BinderTest::case2289(){ }
void BinderTest::case2290(){ }
void BinderTest::case2291(){ }
void BinderTest::case2292(){ }
void BinderTest::case2293(){ }
void BinderTest::case2294(){ }
void BinderTest::case2295(){ }
void BinderTest::case2296(){ }
void BinderTest::case2297(){ }
void BinderTest::case2298(){ }
void BinderTest::case2299(){ }

void BinderTest::case2300()
{
    bind(R"(
struct
{
    int x [ 1 ] ;
};
         )",
         Expectation()
             .binding(BindingSummary("x", ValueKind::Field)
                      .specType("int", NamedTypeKind::Builtin, BuiltinTypeKind::Int, CVR::None)
                      .derivType(TypeKind::Array, CVR::None)));
}

void BinderTest::case2301()
{
    bind("struct { x y [ 1 ] ; } ;",
         Expectation()
             .binding(BindingSummary("y", ValueKind::Field)
                      .specType("x", NamedTypeKind::Synonym, BuiltinTypeKind::None, CVR::None)
                      .derivType(TypeKind::Array, CVR::None)));
}

void BinderTest::case2302()
{
    bind(R"(
struct
{
    int x [ 1 ] , y [ 2 ] ;
};
         )",
         Expectation()
             .binding(BindingSummary("x", ValueKind::Field)
                      .specType("int", NamedTypeKind::Builtin, BuiltinTypeKind::Int, CVR::None)
                      .derivType(TypeKind::Array, CVR::None))
             .binding(BindingSummary("y", ValueKind::Field)
                      .specType("int", NamedTypeKind::Builtin, BuiltinTypeKind::Int, CVR::None)
                      .derivType(TypeKind::Array, CVR::None)));
}

void BinderTest::case2303()
{
    bind(R"(
struct
{
    x y [ 1 ] , z [ 2 ] ;
};
         )",
         Expectation()
             .binding(BindingSummary("y", ValueKind::Field)
                      .specType("x", NamedTypeKind::Synonym, BuiltinTypeKind::None, CVR::None)
                      .derivType(TypeKind::Array, CVR::None))
             .binding(BindingSummary("z", ValueKind::Field)
                      .specType("x", NamedTypeKind::Synonym, BuiltinTypeKind::None, CVR::None)
                      .derivType(TypeKind::Array, CVR::None)));
}

void BinderTest::case2304()
{
    bind("struct { int * x [ 1 ] ; } ;",
         Expectation()
             .binding(BindingSummary("x", ValueKind::Field)
                      .specType("int", NamedTypeKind::Builtin, BuiltinTypeKind::Int, CVR::None)
                      .derivType(TypeKind::Pointer, CVR::None)
                      .derivType(TypeKind::Array, CVR::None)));
}

void BinderTest::case2305()
{
    bind("struct { int x [ 1 ] , * y [ 2 ] ; } ;",
         Expectation()
             .binding(BindingSummary("x", ValueKind::Field)
                      .specType("int", NamedTypeKind::Builtin, BuiltinTypeKind::Int, CVR::None)
                      .derivType(TypeKind::Array, CVR::None))
             .binding(BindingSummary("y", ValueKind::Field)
                      .specType("int", NamedTypeKind::Builtin, BuiltinTypeKind::Int, CVR::None)
                      .derivType(TypeKind::Pointer, CVR::None)
                      .derivType(TypeKind::Array, CVR::None)));
}

void BinderTest::case2306()
{
    bind("struct { int * * x [ 1 ] ; } ;",
         Expectation()
             .binding(BindingSummary("x", ValueKind::Field)
                      .specType("int", NamedTypeKind::Builtin, BuiltinTypeKind::Int, CVR::None)
                      .derivType(TypeKind::Pointer, CVR::None)
                      .derivType(TypeKind::Pointer, CVR::None)
                      .derivType(TypeKind::Array, CVR::None)));
}

void BinderTest::case2307(){ }
void BinderTest::case2308(){ }
void BinderTest::case2309(){ }
void BinderTest::case2310(){ }
void BinderTest::case2311(){ }
void BinderTest::case2312(){ }
void BinderTest::case2313(){ }
void BinderTest::case2314(){ }
void BinderTest::case2315(){ }
void BinderTest::case2316(){ }
void BinderTest::case2317(){ }
void BinderTest::case2318(){ }
void BinderTest::case2319(){ }
void BinderTest::case2320(){ }
void BinderTest::case2321(){ }
void BinderTest::case2322(){ }
void BinderTest::case2323(){ }
void BinderTest::case2324(){ }
void BinderTest::case2325(){ }
void BinderTest::case2326(){ }
void BinderTest::case2327(){ }
void BinderTest::case2328(){ }
void BinderTest::case2329(){ }
void BinderTest::case2330(){ }
void BinderTest::case2331(){ }
void BinderTest::case2332(){ }
void BinderTest::case2333(){ }
void BinderTest::case2334(){ }
void BinderTest::case2335(){ }
void BinderTest::case2336(){ }
void BinderTest::case2337(){ }
void BinderTest::case2338(){ }
void BinderTest::case2339(){ }
void BinderTest::case2340(){ }
void BinderTest::case2341(){ }
void BinderTest::case2342(){ }
void BinderTest::case2343(){ }
void BinderTest::case2344(){ }
void BinderTest::case2345(){ }
void BinderTest::case2346(){ }
void BinderTest::case2347(){ }
void BinderTest::case2348(){ }
void BinderTest::case2349(){ }

void BinderTest::case2350()
{
    bind("struct { const int x [ 1 ] ; } ;",
         Expectation()
             .binding(BindingSummary("x", ValueKind::Field)
                      .specType("int", NamedTypeKind::Builtin, BuiltinTypeKind::Int, CVR::Const)
                      .derivType(TypeKind::Array, CVR::None)));
}

void BinderTest::case2351(){ }
void BinderTest::case2352(){ }
void BinderTest::case2353(){ }
void BinderTest::case2354(){ }
void BinderTest::case2355(){ }
void BinderTest::case2356(){ }
void BinderTest::case2357(){ }
void BinderTest::case2358(){ }
void BinderTest::case2359(){ }
void BinderTest::case2360(){ }
void BinderTest::case2361(){ }
void BinderTest::case2362(){ }
void BinderTest::case2363(){ }
void BinderTest::case2364(){ }
void BinderTest::case2365(){ }
void BinderTest::case2366(){ }
void BinderTest::case2367(){ }
void BinderTest::case2368(){ }
void BinderTest::case2369(){ }
void BinderTest::case2370(){ }
void BinderTest::case2371(){ }
void BinderTest::case2372(){ }
void BinderTest::case2373(){ }
void BinderTest::case2374(){ }
void BinderTest::case2375(){ }
void BinderTest::case2376(){ }
void BinderTest::case2377(){ }
void BinderTest::case2378(){ }
void BinderTest::case2379(){ }
void BinderTest::case2380(){ }
void BinderTest::case2381(){ }
void BinderTest::case2382(){ }
void BinderTest::case2383(){ }
void BinderTest::case2384(){ }
void BinderTest::case2385(){ }
void BinderTest::case2386(){ }
void BinderTest::case2387(){ }
void BinderTest::case2388(){ }
void BinderTest::case2389(){ }
void BinderTest::case2390(){ }
void BinderTest::case2391(){ }
void BinderTest::case2392(){ }
void BinderTest::case2393(){ }
void BinderTest::case2394(){ }
void BinderTest::case2395(){ }
void BinderTest::case2396(){ }
void BinderTest::case2397(){ }
void BinderTest::case2398(){ }
void BinderTest::case2399(){ }
