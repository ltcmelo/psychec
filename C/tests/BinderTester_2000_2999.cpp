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

#include "BinderTester.h"

#include "ParserTester.h"

#include "binder/ConstraintsInDeclarations.h"
#include "binder/ConstraintsInTypeSpecifiers.h"
#include "parser/Unparser.h"
#include "symbols/Symbol.h"
#include "symbols/Symbol_ALL.h"
#include "syntax/SyntaxLexeme_ALL.h"
#include "syntax/SyntaxNodes.h"

#include <algorithm>
#include <cstring>
#include <iostream>
#include <string>
#include <sstream>

using namespace psy;
using namespace C;

void BinderTester::case2000()
{
    bind(R"(
struct
{
   double x ;
};
         )",
         Expectation()
            .binding(DeclSummary().Value("x", ValueKind::Field)
                    .TySpec.basis("double", NamedTypeKind::Builtin, BuiltinTypeKind::Double)));
}

void BinderTester::case2001()
{
    bind(R"(
struct
{
    int x ;
};
         )",
         Expectation()
            .binding(DeclSummary().Value("x", ValueKind::Field)
                     .TySpec.basis("int", NamedTypeKind::Builtin, BuiltinTypeKind::Int)));
}

void BinderTester::case2002()
{
    bind(R"(
struct w
{
    int x ;
    int y ;
};
         )",
         Expectation()
            .binding(DeclSummary().Value("x", ValueKind::Field)
                    .TySpec.basis("int", NamedTypeKind::Builtin, BuiltinTypeKind::Int))
            .binding(DeclSummary().Value("y", ValueKind::Field)
                    .TySpec.basis("int", NamedTypeKind::Builtin, BuiltinTypeKind::Int)));
}

void BinderTester::case2003()
{
    bind(R"(
struct
{
    int x , y ;
};
         )",
         Expectation()
             .binding(DeclSummary().Value("x", ValueKind::Field)
                     .TySpec.basis("int", NamedTypeKind::Builtin, BuiltinTypeKind::Int))
             .binding(DeclSummary().Value("y", ValueKind::Field)
                     .TySpec.basis("int", NamedTypeKind::Builtin, BuiltinTypeKind::Int)));
}

void BinderTester::case2004()
{
    bind(R"(
struct
{
    x y ;
};
         )",
         Expectation()
             .binding(DeclSummary().Value("y", ValueKind::Field)
                 .TySpec.basis("x", NamedTypeKind::Synonym, BuiltinTypeKind::UNSPECIFIED)));
}

void BinderTester::case2005()
{
    bind(R"(
struct
{
    x y , z ;
};
         )",
         Expectation()
             .binding(DeclSummary().Value("y", ValueKind::Field)
                      .TySpec.basis("x", NamedTypeKind::Synonym, BuiltinTypeKind::UNSPECIFIED))
             .binding(DeclSummary().Value("z", ValueKind::Field)
                      .TySpec.basis("x", NamedTypeKind::Synonym, BuiltinTypeKind::UNSPECIFIED)));
}

void BinderTester::case2006()
{
    // Non-field correspondent
    CROSS_REFERENCE_TEST(BinderTester::case2006);

    // Syntax error
    CROSS_REFERENCE_TEST(ParserTester::case0439);
}

void BinderTester::case2007()
{
    // Non-field correspondent
    CROSS_REFERENCE_TEST(BinderTester::case2007);

    // Syntax error
    CROSS_REFERENCE_TEST(ParserTester::case0440);
}

void BinderTester::case2008()
{
    bind(R"(
struct
{
    int float x ;
};
         )",
         Expectation().diagnostic(
             Expectation::ErrorOrWarn::Error,
             ConstraintsInTypeSpecifiers::ID_TwoOrMoreDataTypesInDeclarationSpecifiers));
}

void BinderTester::case2009()
{
    bind(R"(
struct
{
    struct x y ;
};
         )",
         Expectation()
            .binding(DeclSummary().Value("y", ValueKind::Field)
                    .TySpec.basis("struct x", NamedTypeKind::Structure, BuiltinTypeKind::UNSPECIFIED)));
}

void BinderTester::case2010()
{
    bind(R"(
struct
{
    union x y ;
};
         )",
         Expectation()
            .binding(DeclSummary().Value("y", ValueKind::Field)
                    .TySpec.basis("union x", NamedTypeKind::Union, BuiltinTypeKind::UNSPECIFIED)));
}

void BinderTester::case2011()
{
    bind(R"(
struct
{
    enum x y ;
};
         )",
         Expectation()
            .binding(DeclSummary().Value("y", ValueKind::Field)
                    .TySpec.basis("enum x", NamedTypeKind::Enumeration, BuiltinTypeKind::UNSPECIFIED)));
}

void BinderTester::case2012()
{
    bind(R"(
struct
{
    struct x y , z ;
};
         )",
         Expectation()
            .binding(DeclSummary().Value("y", ValueKind::Field)
                    .TySpec.basis("struct x", NamedTypeKind::Structure, BuiltinTypeKind::UNSPECIFIED))
            .binding(DeclSummary().Value("z", ValueKind::Field)
                    .TySpec.basis("struct x", NamedTypeKind::Structure, BuiltinTypeKind::UNSPECIFIED)));
}

void BinderTester::case2013()
{
    bind(R"(
struct
{
    struct x { int y ; } z ;
};
         )",
         Expectation()
            .binding(DeclSummary().Value("z", ValueKind::Field)
                    .TySpec.basis("struct x", NamedTypeKind::Structure, BuiltinTypeKind::UNSPECIFIED)));
}

void BinderTester::case2014(){ }
void BinderTester::case2015(){ }
void BinderTester::case2016(){ }
void BinderTester::case2017(){ }
void BinderTester::case2018(){ }
void BinderTester::case2019(){ }
void BinderTester::case2020(){ }
void BinderTester::case2021(){ }
void BinderTester::case2022(){ }
void BinderTester::case2023(){ }
void BinderTester::case2024(){ }
void BinderTester::case2025(){ }
void BinderTester::case2026(){ }
void BinderTester::case2027(){ }
void BinderTester::case2028(){ }
void BinderTester::case2029(){ }
void BinderTester::case2030(){ }
void BinderTester::case2031(){ }
void BinderTester::case2032(){ }
void BinderTester::case2033(){ }
void BinderTester::case2034(){ }
void BinderTester::case2035(){ }
void BinderTester::case2036(){ }
void BinderTester::case2037(){ }
void BinderTester::case2038(){ }
void BinderTester::case2039(){ }
void BinderTester::case2040(){ }
void BinderTester::case2041(){ }
void BinderTester::case2042(){ }
void BinderTester::case2043(){ }
void BinderTester::case2044(){ }
void BinderTester::case2045(){ }
void BinderTester::case2046(){ }
void BinderTester::case2047(){ }
void BinderTester::case2048(){ }
void BinderTester::case2049(){ }

void BinderTester::case2050()
{
    bind("struct { const int x ; } ;",
         Expectation()
             .binding(DeclSummary()
                      .Value("x", ValueKind::Field)
                      .TySpec.basis("int", NamedTypeKind::Builtin, BuiltinTypeKind::Int, CVR::Const)));
}

void BinderTester::case2051()
{
    bind("struct { const x y ; } ;",
         Expectation()
             .binding(DeclSummary()
                      .Value("y", ValueKind::Field)
                      .TySpec.basis("x", NamedTypeKind::Synonym, BuiltinTypeKind::UNSPECIFIED, CVR::Const)));
}

void BinderTester::case2052()
{
    bind("struct { const x ; } ;",
         Expectation().diagnostic(
             Expectation::ErrorOrWarn::Error,
             ConstraintsInDeclarations::ID_of_UselessDeclaration));
}

void BinderTester::case2053()
{
    bind("struct { int const x ; } ;",
         Expectation()
             .binding(DeclSummary()
                      .Value("x", ValueKind::Field)
                      .TySpec.basis("int", NamedTypeKind::Builtin, BuiltinTypeKind::Int, CVR::Const)));
}

void BinderTester::case2054()
{
    bind("struct { x const y ; } ;",
         Expectation()
             .binding(DeclSummary()
                      .Value("y", ValueKind::Field)
                      .TySpec.basis("x", NamedTypeKind::Synonym, BuiltinTypeKind::UNSPECIFIED, CVR::Const)));
}

void BinderTester::case2055(){ }
void BinderTester::case2056(){ }
void BinderTester::case2057(){ }
void BinderTester::case2058(){ }
void BinderTester::case2059(){ }
void BinderTester::case2060(){ }
void BinderTester::case2061(){ }
void BinderTester::case2062(){ }
void BinderTester::case2063(){ }
void BinderTester::case2064(){ }
void BinderTester::case2065(){ }
void BinderTester::case2066(){ }
void BinderTester::case2067(){ }
void BinderTester::case2068(){ }
void BinderTester::case2069(){ }
void BinderTester::case2070(){ }
void BinderTester::case2071(){ }
void BinderTester::case2072(){ }
void BinderTester::case2073(){ }
void BinderTester::case2074(){ }
void BinderTester::case2075(){ }
void BinderTester::case2076(){ }
void BinderTester::case2077(){ }
void BinderTester::case2078(){ }
void BinderTester::case2079(){ }
void BinderTester::case2080(){ }
void BinderTester::case2081(){ }
void BinderTester::case2082(){ }
void BinderTester::case2083(){ }
void BinderTester::case2084(){ }
void BinderTester::case2085(){ }
void BinderTester::case2086(){ }
void BinderTester::case2087(){ }
void BinderTester::case2088(){ }
void BinderTester::case2089(){ }
void BinderTester::case2090(){ }
void BinderTester::case2091(){ }
void BinderTester::case2092(){ }
void BinderTester::case2093(){ }
void BinderTester::case2094(){ }
void BinderTester::case2095(){ }
void BinderTester::case2096(){ }
void BinderTester::case2097(){ }
void BinderTester::case2098(){ }
void BinderTester::case2099(){ }

void BinderTester::case2100()
{
    bind("struct { int * x ; } ;",
         Expectation()
             .binding(DeclSummary().Value("x", ValueKind::Field)
                      .TySpec.basis("int", NamedTypeKind::Builtin, BuiltinTypeKind::Int)
                      .TySpec.deriv(TypeKind::Pointer)));
}

void BinderTester::case2101()
{
    bind("struct { x * y ; } ;",
         Expectation()
             .binding(DeclSummary().Value("y", ValueKind::Field)
                      .TySpec.basis("x", NamedTypeKind::Synonym)
                      .TySpec.deriv(TypeKind::Pointer)));
}

void BinderTester::case2102()
{
    bind("struct { int * x ; y * z ; } ;",
         Expectation()
             .binding(DeclSummary().Value("x", ValueKind::Field)
                      .TySpec.basis("int", NamedTypeKind::Builtin, BuiltinTypeKind::Int)
                      .TySpec.deriv(TypeKind::Pointer))
             .binding(DeclSummary().Value("z", ValueKind::Field)
                      .TySpec.basis("y", NamedTypeKind::Synonym)
                      .TySpec.deriv(TypeKind::Pointer)));
}

void BinderTester::case2103()
{
    bind("struct { int * x , * y ; } ;",
         Expectation()
             .binding(DeclSummary().Value("x", ValueKind::Field)
                      .TySpec.basis("int", NamedTypeKind::Builtin, BuiltinTypeKind::Int)
                      .TySpec.deriv(TypeKind::Pointer))
             .binding(DeclSummary().Value("y", ValueKind::Field)
                      .TySpec.basis("int", NamedTypeKind::Builtin, BuiltinTypeKind::Int)
                      .TySpec.deriv(TypeKind::Pointer)));
}

void BinderTester::case2104()
{
    bind("struct { int ( * x ) [ 1 ]; } ;",
         Expectation()
             .binding(DeclSummary().Value("x", ValueKind::Field)
                      .TySpec.basis("int", NamedTypeKind::Builtin, BuiltinTypeKind::Int)
                      .TySpec.deriv(TypeKind::Array)
                      .TySpec.deriv(TypeKind::Pointer)));
}

void BinderTester::case2105()
{
    bind("struct { int * * x ; } ;",
         Expectation()
             .binding(DeclSummary().Value("x", ValueKind::Field)
                      .TySpec.basis("int", NamedTypeKind::Builtin, BuiltinTypeKind::Int)
                      .TySpec.deriv(TypeKind::Pointer)
                      .TySpec.deriv(TypeKind::Pointer)));
}

void BinderTester::case2106()
{
    bind("struct { int * * * x ; } ;",
         Expectation()
             .binding(DeclSummary().Value("x", ValueKind::Field)
                      .TySpec.basis("int", NamedTypeKind::Builtin, BuiltinTypeKind::Int)
                      .TySpec.deriv(TypeKind::Pointer)
                      .TySpec.deriv(TypeKind::Pointer)
                      .TySpec.deriv(TypeKind::Pointer)));
}

void BinderTester::case2107()
{
    bind("struct { int * ( * x ) [ 1 ] ; };",
         Expectation()
             .binding(DeclSummary().Value("x", ValueKind::Field)
                      .TySpec.basis("int", NamedTypeKind::Builtin, BuiltinTypeKind::Int, CVR::None)
                      .TySpec.deriv(TypeKind::Pointer, CVR::None)
                      .TySpec.deriv(TypeKind::Array, CVR::None)
                      .TySpec.deriv(TypeKind::Pointer, CVR::None)));
}

void BinderTester::case2108(){ }
void BinderTester::case2109(){ }
void BinderTester::case2110(){ }
void BinderTester::case2111(){ }
void BinderTester::case2112(){ }
void BinderTester::case2113(){ }
void BinderTester::case2114(){ }
void BinderTester::case2115(){ }
void BinderTester::case2116(){ }
void BinderTester::case2117(){ }
void BinderTester::case2118(){ }
void BinderTester::case2119(){ }
void BinderTester::case2120(){ }
void BinderTester::case2121(){ }
void BinderTester::case2122(){ }
void BinderTester::case2123(){ }
void BinderTester::case2124(){ }
void BinderTester::case2125(){ }
void BinderTester::case2126(){ }
void BinderTester::case2127(){ }
void BinderTester::case2128(){ }
void BinderTester::case2129(){ }
void BinderTester::case2130(){ }
void BinderTester::case2131(){ }
void BinderTester::case2132(){ }
void BinderTester::case2133(){ }
void BinderTester::case2134(){ }
void BinderTester::case2135(){ }
void BinderTester::case2136(){ }
void BinderTester::case2137(){ }
void BinderTester::case2138(){ }
void BinderTester::case2139(){ }
void BinderTester::case2140(){ }
void BinderTester::case2141(){ }
void BinderTester::case2142(){ }
void BinderTester::case2143(){ }
void BinderTester::case2144(){ }
void BinderTester::case2145(){ }
void BinderTester::case2146(){ }
void BinderTester::case2147(){ }
void BinderTester::case2148(){ }
void BinderTester::case2149(){ }

void BinderTester::case2150()
{
    bind("struct { const int * x ; } ;",
         Expectation()
             .binding(DeclSummary().Value("x", ValueKind::Field)
                      .TySpec.basis("int", NamedTypeKind::Builtin, BuiltinTypeKind::Int, CVR::Const)
                      .TySpec.deriv(TypeKind::Pointer)));
}

void BinderTester::case2151()
{
    bind("struct { const x * y ; } ;",
         Expectation()
             .binding(DeclSummary().Value("y", ValueKind::Field)
                      .TySpec.basis("x", NamedTypeKind::Synonym, BuiltinTypeKind::UNSPECIFIED, CVR::Const)
                      .TySpec.deriv(TypeKind::Pointer)));
}

void BinderTester::case2152(){ }
void BinderTester::case2153(){ }
void BinderTester::case2154(){ }
void BinderTester::case2155(){ }
void BinderTester::case2156(){ }
void BinderTester::case2157(){ }
void BinderTester::case2158(){ }
void BinderTester::case2159(){ }
void BinderTester::case2160(){ }
void BinderTester::case2161(){ }
void BinderTester::case2162(){ }
void BinderTester::case2163(){ }
void BinderTester::case2164(){ }
void BinderTester::case2165(){ }
void BinderTester::case2166(){ }
void BinderTester::case2167(){ }
void BinderTester::case2168(){ }
void BinderTester::case2169(){ }
void BinderTester::case2170(){ }
void BinderTester::case2171(){ }
void BinderTester::case2172(){ }
void BinderTester::case2173(){ }
void BinderTester::case2174(){ }
void BinderTester::case2175(){ }
void BinderTester::case2176(){ }
void BinderTester::case2177(){ }
void BinderTester::case2178(){ }
void BinderTester::case2179(){ }
void BinderTester::case2180(){ }
void BinderTester::case2181(){ }
void BinderTester::case2182(){ }
void BinderTester::case2183(){ }
void BinderTester::case2184(){ }
void BinderTester::case2185(){ }
void BinderTester::case2186(){ }
void BinderTester::case2187(){ }
void BinderTester::case2188(){ }
void BinderTester::case2189(){ }
void BinderTester::case2190(){ }
void BinderTester::case2191(){ }
void BinderTester::case2192(){ }
void BinderTester::case2193(){ }
void BinderTester::case2194(){ }
void BinderTester::case2195(){ }
void BinderTester::case2196(){ }
void BinderTester::case2197(){ }
void BinderTester::case2198(){ }
void BinderTester::case2199(){ }

void BinderTester::case2200()
{
    bind("struct { const int * const x ; } ;",
         Expectation()
             .binding(DeclSummary().Value("x", ValueKind::Field)
                      .TySpec.basis("int", NamedTypeKind::Builtin, BuiltinTypeKind::Int, CVR::Const)
                      .TySpec.deriv(TypeKind::Pointer, CVR::Const)));
}


void BinderTester::case2201(){ }
void BinderTester::case2202(){ }
void BinderTester::case2203(){ }
void BinderTester::case2204(){ }
void BinderTester::case2205(){ }
void BinderTester::case2206(){ }
void BinderTester::case2207(){ }
void BinderTester::case2208(){ }
void BinderTester::case2209(){ }
void BinderTester::case2210(){ }
void BinderTester::case2211(){ }
void BinderTester::case2212(){ }
void BinderTester::case2213(){ }
void BinderTester::case2214(){ }
void BinderTester::case2215(){ }
void BinderTester::case2216(){ }
void BinderTester::case2217(){ }
void BinderTester::case2218(){ }
void BinderTester::case2219(){ }
void BinderTester::case2220(){ }
void BinderTester::case2221(){ }
void BinderTester::case2222(){ }
void BinderTester::case2223(){ }
void BinderTester::case2224(){ }
void BinderTester::case2225(){ }
void BinderTester::case2226(){ }
void BinderTester::case2227(){ }
void BinderTester::case2228(){ }
void BinderTester::case2229(){ }
void BinderTester::case2230(){ }
void BinderTester::case2231(){ }
void BinderTester::case2232(){ }
void BinderTester::case2233(){ }
void BinderTester::case2234(){ }
void BinderTester::case2235(){ }
void BinderTester::case2236(){ }
void BinderTester::case2237(){ }
void BinderTester::case2238(){ }
void BinderTester::case2239(){ }
void BinderTester::case2240(){ }
void BinderTester::case2241(){ }
void BinderTester::case2242(){ }
void BinderTester::case2243(){ }
void BinderTester::case2244(){ }
void BinderTester::case2245(){ }
void BinderTester::case2246(){ }
void BinderTester::case2247(){ }
void BinderTester::case2248(){ }
void BinderTester::case2249(){ }

void BinderTester::case2250()
{
    bind("struct { int * const x ; } ;",
         Expectation()
             .binding(DeclSummary().Value("x", ValueKind::Field)
                      .TySpec.basis("int", NamedTypeKind::Builtin, BuiltinTypeKind::Int, CVR::None)
                      .TySpec.deriv(TypeKind::Pointer, CVR::Const)));
}

void BinderTester::case2251()
{
    bind("struct { x * const y ; } ;",
         Expectation()
             .binding(DeclSummary().Value("y", ValueKind::Field)
                      .TySpec.basis("x", NamedTypeKind::Synonym, BuiltinTypeKind::UNSPECIFIED, CVR::None)
                      .TySpec.deriv(TypeKind::Pointer, CVR::Const)));
}

void BinderTester::case2252(){ }
void BinderTester::case2253(){ }
void BinderTester::case2254(){ }
void BinderTester::case2255(){ }
void BinderTester::case2256(){ }
void BinderTester::case2257(){ }
void BinderTester::case2258(){ }
void BinderTester::case2259(){ }
void BinderTester::case2260(){ }
void BinderTester::case2261(){ }
void BinderTester::case2262(){ }
void BinderTester::case2263(){ }
void BinderTester::case2264(){ }
void BinderTester::case2265(){ }
void BinderTester::case2266(){ }
void BinderTester::case2267(){ }
void BinderTester::case2268(){ }
void BinderTester::case2269(){ }
void BinderTester::case2270(){ }
void BinderTester::case2271(){ }
void BinderTester::case2272(){ }
void BinderTester::case2273(){ }
void BinderTester::case2274(){ }
void BinderTester::case2275(){ }
void BinderTester::case2276(){ }
void BinderTester::case2277(){ }
void BinderTester::case2278(){ }
void BinderTester::case2279(){ }
void BinderTester::case2280(){ }
void BinderTester::case2281(){ }
void BinderTester::case2282(){ }
void BinderTester::case2283(){ }
void BinderTester::case2284(){ }
void BinderTester::case2285(){ }
void BinderTester::case2286(){ }
void BinderTester::case2287(){ }
void BinderTester::case2288(){ }
void BinderTester::case2289(){ }
void BinderTester::case2290(){ }
void BinderTester::case2291(){ }
void BinderTester::case2292(){ }
void BinderTester::case2293(){ }
void BinderTester::case2294(){ }
void BinderTester::case2295(){ }
void BinderTester::case2296(){ }
void BinderTester::case2297(){ }
void BinderTester::case2298(){ }
void BinderTester::case2299(){ }

void BinderTester::case2300()
{
    bind(R"(
struct
{
    int x [ 1 ] ;
};
         )",
         Expectation()
             .binding(DeclSummary().Value("x", ValueKind::Field)
                      .TySpec.basis("int", NamedTypeKind::Builtin, BuiltinTypeKind::Int, CVR::None)
                      .TySpec.deriv(TypeKind::Array, CVR::None)));
}

void BinderTester::case2301()
{
    bind("struct { x y [ 1 ] ; } ;",
         Expectation()
             .binding(DeclSummary().Value("y", ValueKind::Field)
                      .TySpec.basis("x", NamedTypeKind::Synonym, BuiltinTypeKind::UNSPECIFIED, CVR::None)
                      .TySpec.deriv(TypeKind::Array, CVR::None)));
}

void BinderTester::case2302()
{
    bind(R"(
struct
{
    int x [ 1 ] , y [ 2 ] ;
};
         )",
         Expectation()
             .binding(DeclSummary().Value("x", ValueKind::Field)
                      .TySpec.basis("int", NamedTypeKind::Builtin, BuiltinTypeKind::Int, CVR::None)
                      .TySpec.deriv(TypeKind::Array, CVR::None))
             .binding(DeclSummary().Value("y", ValueKind::Field)
                      .TySpec.basis("int", NamedTypeKind::Builtin, BuiltinTypeKind::Int, CVR::None)
                      .TySpec.deriv(TypeKind::Array, CVR::None)));
}

void BinderTester::case2303()
{
    bind(R"(
struct
{
    x y [ 1 ] , z [ 2 ] ;
};
         )",
         Expectation()
             .binding(DeclSummary().Value("y", ValueKind::Field)
                      .TySpec.basis("x", NamedTypeKind::Synonym, BuiltinTypeKind::UNSPECIFIED, CVR::None)
                      .TySpec.deriv(TypeKind::Array, CVR::None))
             .binding(DeclSummary().Value("z", ValueKind::Field)
                      .TySpec.basis("x", NamedTypeKind::Synonym, BuiltinTypeKind::UNSPECIFIED, CVR::None)
                      .TySpec.deriv(TypeKind::Array, CVR::None)));
}

void BinderTester::case2304()
{
    bind("struct { int * x [ 1 ] ; } ;",
         Expectation()
             .binding(DeclSummary().Value("x", ValueKind::Field)
                      .TySpec.basis("int", NamedTypeKind::Builtin, BuiltinTypeKind::Int, CVR::None)
                      .TySpec.deriv(TypeKind::Pointer, CVR::None)
                      .TySpec.deriv(TypeKind::Array, CVR::None)));
}

void BinderTester::case2305()
{
    bind("struct { int x [ 1 ] , * y [ 2 ] ; } ;",
         Expectation()
             .binding(DeclSummary().Value("x", ValueKind::Field)
                      .TySpec.basis("int", NamedTypeKind::Builtin, BuiltinTypeKind::Int, CVR::None)
                      .TySpec.deriv(TypeKind::Array, CVR::None))
             .binding(DeclSummary().Value("y", ValueKind::Field)
                      .TySpec.basis("int", NamedTypeKind::Builtin, BuiltinTypeKind::Int, CVR::None)
                      .TySpec.deriv(TypeKind::Pointer, CVR::None)
                      .TySpec.deriv(TypeKind::Array, CVR::None)));
}

void BinderTester::case2306()
{
    bind("struct { int * * x [ 1 ] ; } ;",
         Expectation()
             .binding(DeclSummary().Value("x", ValueKind::Field)
                      .TySpec.basis("int", NamedTypeKind::Builtin, BuiltinTypeKind::Int, CVR::None)
                      .TySpec.deriv(TypeKind::Pointer, CVR::None)
                      .TySpec.deriv(TypeKind::Pointer, CVR::None)
                      .TySpec.deriv(TypeKind::Array, CVR::None)));
}

void BinderTester::case2307(){ }
void BinderTester::case2308(){ }
void BinderTester::case2309(){ }
void BinderTester::case2310(){ }
void BinderTester::case2311(){ }
void BinderTester::case2312(){ }
void BinderTester::case2313(){ }
void BinderTester::case2314(){ }
void BinderTester::case2315(){ }
void BinderTester::case2316(){ }
void BinderTester::case2317(){ }
void BinderTester::case2318(){ }
void BinderTester::case2319(){ }
void BinderTester::case2320(){ }
void BinderTester::case2321(){ }
void BinderTester::case2322(){ }
void BinderTester::case2323(){ }
void BinderTester::case2324(){ }
void BinderTester::case2325(){ }
void BinderTester::case2326(){ }
void BinderTester::case2327(){ }
void BinderTester::case2328(){ }
void BinderTester::case2329(){ }
void BinderTester::case2330(){ }
void BinderTester::case2331(){ }
void BinderTester::case2332(){ }
void BinderTester::case2333(){ }
void BinderTester::case2334(){ }
void BinderTester::case2335(){ }
void BinderTester::case2336(){ }
void BinderTester::case2337(){ }
void BinderTester::case2338(){ }
void BinderTester::case2339(){ }
void BinderTester::case2340(){ }
void BinderTester::case2341(){ }
void BinderTester::case2342(){ }
void BinderTester::case2343(){ }
void BinderTester::case2344(){ }
void BinderTester::case2345(){ }
void BinderTester::case2346(){ }
void BinderTester::case2347(){ }
void BinderTester::case2348(){ }
void BinderTester::case2349(){ }

void BinderTester::case2350()
{
    bind("struct { const int x [ 1 ] ; } ;",
         Expectation()
             .binding(DeclSummary().Value("x", ValueKind::Field)
                      .TySpec.basis("int", NamedTypeKind::Builtin, BuiltinTypeKind::Int, CVR::Const)
                      .TySpec.deriv(TypeKind::Array, CVR::None)));
}

void BinderTester::case2351(){ }
void BinderTester::case2352(){ }
void BinderTester::case2353(){ }
void BinderTester::case2354(){ }
void BinderTester::case2355(){ }
void BinderTester::case2356(){ }
void BinderTester::case2357(){ }
void BinderTester::case2358(){ }
void BinderTester::case2359(){ }
void BinderTester::case2360(){ }
void BinderTester::case2361(){ }
void BinderTester::case2362(){ }
void BinderTester::case2363(){ }
void BinderTester::case2364(){ }
void BinderTester::case2365(){ }
void BinderTester::case2366(){ }
void BinderTester::case2367(){ }
void BinderTester::case2368(){ }
void BinderTester::case2369(){ }
void BinderTester::case2370(){ }
void BinderTester::case2371(){ }
void BinderTester::case2372(){ }
void BinderTester::case2373(){ }
void BinderTester::case2374(){ }
void BinderTester::case2375(){ }
void BinderTester::case2376(){ }
void BinderTester::case2377(){ }
void BinderTester::case2378(){ }
void BinderTester::case2379(){ }
void BinderTester::case2380(){ }
void BinderTester::case2381(){ }
void BinderTester::case2382(){ }
void BinderTester::case2383(){ }
void BinderTester::case2384(){ }
void BinderTester::case2385(){ }
void BinderTester::case2386(){ }
void BinderTester::case2387(){ }
void BinderTester::case2388(){ }
void BinderTester::case2389(){ }
void BinderTester::case2390(){ }
void BinderTester::case2391(){ }
void BinderTester::case2392(){ }
void BinderTester::case2393(){ }
void BinderTester::case2394(){ }
void BinderTester::case2395(){ }
void BinderTester::case2396(){ }
void BinderTester::case2397(){ }
void BinderTester::case2398(){ }
void BinderTester::case2399(){ }
