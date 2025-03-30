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

#include "DeclarationBinderTester.h"

#include "ParserTester.h"

#include "sema/DeclarationBinder.h"
#include "parser/Unparser.h"
#include "symbols/Symbol.h"
#include "symbols/Symbol_ALL.h"
#include "syntax/Lexeme_ALL.h"
#include "syntax/SyntaxNodes.h"

#include <algorithm>
#include <cstring>
#include <iostream>
#include <string>
#include <sstream>

using namespace psy;
using namespace C;

void DeclarationBinderTester::case2000()
{
    bind(R"(
struct
{
   double x ;
};
         )",
         Expectation()
            .declaration(Decl().Member("x", MemberDeclarationCategory::Field)
                    .ty_.Basic(BasicTypeKind::Double)));
}

void DeclarationBinderTester::case2001()
{
    bind(R"(
struct
{
    int x ;
};
         )",
         Expectation()
            .declaration(Decl().Member("x", MemberDeclarationCategory::Field)
                     .ty_.Basic(BasicTypeKind::Int_S)));
}

void DeclarationBinderTester::case2002()
{
    bind(R"(
struct w
{
    int x ;
    int y ;
};
         )",
         Expectation()
            .declaration(Decl().Member("x", MemberDeclarationCategory::Field)
                    .ty_.Basic(BasicTypeKind::Int_S))
            .declaration(Decl().Member("y", MemberDeclarationCategory::Field)
                    .ty_.Basic(BasicTypeKind::Int_S)));
}

void DeclarationBinderTester::case2003()
{
    bind(R"(
struct
{
    int x , y ;
};
         )",
         Expectation()
             .declaration(Decl().Member("x", MemberDeclarationCategory::Field)
                     .ty_.Basic(BasicTypeKind::Int_S))
             .declaration(Decl().Member("y", MemberDeclarationCategory::Field)
                     .ty_.Basic(BasicTypeKind::Int_S)));
}

void DeclarationBinderTester::case2004()
{
    bind(R"(
struct
{
    x y ;
};
         )",
         Expectation()
             .declaration(Decl().Member("y", MemberDeclarationCategory::Field)
                 .ty_.Typedef("x")));
}

void DeclarationBinderTester::case2005()
{
    bind(R"(
struct
{
    x y , z ;
};
         )",
         Expectation()
             .declaration(Decl().Member("y", MemberDeclarationCategory::Field)
                      .ty_.Typedef("x"))
             .declaration(Decl().Member("z", MemberDeclarationCategory::Field)
                      .ty_.Typedef("x")));
}

void DeclarationBinderTester::case2006()
{
    // Non-field correspondent
    CROSS_REFERENCE_TEST(DeclarationBinderTester::case2006);

    // Syntax error
    CROSS_REFERENCE_TEST(ParserTester::case0439);
}

void DeclarationBinderTester::case2007()
{
    // Non-field correspondent
    CROSS_REFERENCE_TEST(DeclarationBinderTester::case2007);

    // Syntax error
    CROSS_REFERENCE_TEST(ParserTester::case0440);
}

void DeclarationBinderTester::case2008()
{
    bind(R"(
struct
{
    int float x ;
};
         )",
         Expectation().diagnostic(
             Expectation::ErrorOrWarn::Error,
             DeclarationBinder::DiagnosticsReporter::ID_of_InvalidType));
}

void DeclarationBinderTester::case2009()
{
    bind(R"(
struct
{
    struct x y ;
};
         )",
         Expectation()
            .declaration(Decl().Member("y", MemberDeclarationCategory::Field)
                     .ty_.Tag("x", TagTypeKind::Struct)));
}

void DeclarationBinderTester::case2010()
{
    bind(R"(
struct
{
    union x y ;
};
         )",
         Expectation()
            .declaration(Decl().Member("y", MemberDeclarationCategory::Field)
                     .ty_.Tag("x", TagTypeKind::Union)));
}

void DeclarationBinderTester::case2011()
{
    bind(R"(
struct
{
    enum x y ;
};
         )",
         Expectation()
            .declaration(Decl().Member("y", MemberDeclarationCategory::Field)
                     .ty_.Tag("x", TagTypeKind::Enum)));
}

void DeclarationBinderTester::case2012()
{
    bind(R"(
struct
{
    struct x y , z ;
};
         )",
         Expectation()
            .declaration(Decl().Member("y", MemberDeclarationCategory::Field)
                     .ty_.Tag("x", TagTypeKind::Struct))
            .declaration(Decl().Member("z", MemberDeclarationCategory::Field)
                     .ty_.Tag("x", TagTypeKind::Struct)));
}

void DeclarationBinderTester::case2013()
{
    bind(R"(
struct
{
    struct x { int y ; } z ;
};
         )",
         Expectation()
            .declaration(
                Decl().Member("z", MemberDeclarationCategory::Field).ty_.Tag("x", TagTypeKind::Struct))
            .declaration(
                Decl().Member("y", MemberDeclarationCategory::Field).ty_.Basic(BasicTypeKind::Int_S))
         );
}

void DeclarationBinderTester::case2014()
{
    bind(R"(
struct s
{
    struct x { int y ; } z ;
};
         )",
         Expectation()
            .declaration(
                Decl().Member("z", MemberDeclarationCategory::Field).ty_.Tag("x", TagTypeKind::Struct))
            .declaration(
                Decl().Member("y", MemberDeclarationCategory::Field).ty_.Basic(BasicTypeKind::Int_S))
         );
}

void DeclarationBinderTester::case2015()
{
    bind(R"(
struct x
{
    struct { int y ; } z ;
};
         )",
         Expectation()
            .declaration(Decl().Member("z", MemberDeclarationCategory::Field)
                     .ty_.Tag("", TagTypeKind::Struct)));
}

void DeclarationBinderTester::case2016()
{
    bind(R"(
struct
{
    struct { int y ; } z ;
};
         )",
         Expectation()
            .declaration(
                Decl().Member("z", MemberDeclarationCategory::Field).ty_.Tag("", TagTypeKind::Struct))
            .declaration(
                Decl().Member("y", MemberDeclarationCategory::Field).ty_.Basic(BasicTypeKind::Int_S))
         );
}

void DeclarationBinderTester::case2017()
{
    bind(R"(
struct
{
    struct
    {
         struct
         {
             int x ;
         } y ;
    } ;
};
         )",
         Expectation()
            .declaration(
                Decl().Member("y", MemberDeclarationCategory::Field).ty_.Tag("", TagTypeKind::Struct))
            .declaration(
                Decl().Member("x", MemberDeclarationCategory::Field).ty_.Basic(BasicTypeKind::Int_S))
         );
}

void DeclarationBinderTester::case2018()
{
    bind(R"(
struct
{
    struct
    {
         struct
         {
             int x ;
         } y ;
    } z ;
};
         )",
         Expectation()
            .declaration(
                Decl().Member("y", MemberDeclarationCategory::Field).ty_.Tag("", TagTypeKind::Struct))
            .declaration(
                Decl().Member("x", MemberDeclarationCategory::Field).ty_.Basic(BasicTypeKind::Int_S))
            .declaration(
                Decl().Member("z", MemberDeclarationCategory::Field).ty_.Tag("", TagTypeKind::Struct))
         );
}

void DeclarationBinderTester::case2019()
{
    bind(R"(
struct
{
    struct w
    {
         struct { int x ; } y ;
    } z ;
};
         )",
         Expectation()
            .declaration(
                Decl().Member("y", MemberDeclarationCategory::Field).ty_.Tag("", TagTypeKind::Struct))
            .declaration(
                Decl().Member("x", MemberDeclarationCategory::Field).ty_.Basic(BasicTypeKind::Int_S))
            .declaration(
                Decl().Member("z", MemberDeclarationCategory::Field).ty_.Tag("w", TagTypeKind::Struct))
         );
}

void DeclarationBinderTester::case2020()
{
    bind(R"(
struct
{
    struct w
    {
         struct s { int x ; } y ;
    } z ;
};
         )",
         Expectation()
            .declaration(
                Decl().Member("y", MemberDeclarationCategory::Field).ty_.Tag("s", TagTypeKind::Struct))
            .declaration(
                Decl().Member("x", MemberDeclarationCategory::Field).ty_.Basic(BasicTypeKind::Int_S))
            .declaration(
                Decl().Member("z", MemberDeclarationCategory::Field).ty_.Tag("w", TagTypeKind::Struct))
         );
}

void DeclarationBinderTester::case2021()
{
    bind(R"(
struct
{
    struct w
    {
         struct s
         {
             int x ;
         } y ;
    } z ;
} _ ;
         )",
         Expectation()
            .declaration(
                Decl().Member("y", MemberDeclarationCategory::Field).ty_.Tag("s", TagTypeKind::Struct))
            .declaration(
                Decl().Member("x", MemberDeclarationCategory::Field).ty_.Basic(BasicTypeKind::Int_S))
            .declaration(
                Decl().Member("z", MemberDeclarationCategory::Field).ty_.Tag("w", TagTypeKind::Struct))
         );
}

void DeclarationBinderTester::case2022()
{
    bind(R"(
struct
{
    struct
    {
         struct s
         {
             int x ;
         } y ;
    } z ;
} _ ;
         )",
         Expectation()
            .declaration(
                Decl().Member("y", MemberDeclarationCategory::Field).ty_.Tag("s", TagTypeKind::Struct))
            .declaration(
                Decl().Member("x", MemberDeclarationCategory::Field).ty_.Basic(BasicTypeKind::Int_S))
            .declaration(
                Decl().Member("z", MemberDeclarationCategory::Field).ty_.Tag("", TagTypeKind::Struct))
         );
}

void DeclarationBinderTester::case2023()
{
    bind(R"(
struct
{
    struct
    {
         struct
         {
             int x ;
         } y ;
    } z ;
} _ ;
         )",
         Expectation()
            .declaration(
                Decl().Member("y", MemberDeclarationCategory::Field).ty_.Tag("", TagTypeKind::Struct))
            .declaration(
                Decl().Member("x", MemberDeclarationCategory::Field).ty_.Basic(BasicTypeKind::Int_S))
            .declaration(
                Decl().Member("z", MemberDeclarationCategory::Field).ty_.Tag("", TagTypeKind::Struct))
         );
}

void DeclarationBinderTester::case2024(){ }
void DeclarationBinderTester::case2025(){ }
void DeclarationBinderTester::case2026(){ }
void DeclarationBinderTester::case2027(){ }
void DeclarationBinderTester::case2028(){ }
void DeclarationBinderTester::case2029(){ }
void DeclarationBinderTester::case2030(){ }
void DeclarationBinderTester::case2031(){ }
void DeclarationBinderTester::case2032(){ }
void DeclarationBinderTester::case2033(){ }
void DeclarationBinderTester::case2034(){ }
void DeclarationBinderTester::case2035(){ }
void DeclarationBinderTester::case2036(){ }
void DeclarationBinderTester::case2037(){ }
void DeclarationBinderTester::case2038(){ }
void DeclarationBinderTester::case2039(){ }
void DeclarationBinderTester::case2040(){ }
void DeclarationBinderTester::case2041(){ }
void DeclarationBinderTester::case2042(){ }
void DeclarationBinderTester::case2043(){ }
void DeclarationBinderTester::case2044(){ }
void DeclarationBinderTester::case2045(){ }
void DeclarationBinderTester::case2046(){ }
void DeclarationBinderTester::case2047(){ }
void DeclarationBinderTester::case2048(){ }
void DeclarationBinderTester::case2049(){ }

void DeclarationBinderTester::case2050()
{
    bind("struct { const int x ; } ;",
         Expectation()
             .declaration(Decl()
                      .Member("x", MemberDeclarationCategory::Field)
                      .ty_.Basic(BasicTypeKind::Int_S, CVR::Const)));
}

void DeclarationBinderTester::case2051()
{
    bind("struct { const x y ; } ;",
         Expectation()
             .declaration(Decl()
                      .Member("y", MemberDeclarationCategory::Field)
                      .ty_.Typedef("x", CVR::Const)));
}

void DeclarationBinderTester::case2052()
{
    bind("struct { const x ; } ;",
         Expectation().diagnostic(
             Expectation::ErrorOrWarn::Warn,
             DeclarationBinder::DiagnosticsReporter::ID_of_UselessDeclaration));
}

void DeclarationBinderTester::case2053()
{
    bind("struct { int const x ; } ;",
         Expectation()
             .declaration(Decl()
                      .Member("x", MemberDeclarationCategory::Field)
                      .ty_.Basic(BasicTypeKind::Int_S, CVR::Const)));
}

void DeclarationBinderTester::case2054()
{
    bind("struct { x const y ; } ;",
         Expectation()
             .declaration(Decl()
                      .Member("y", MemberDeclarationCategory::Field)
                      .ty_.Typedef("x", CVR::Const)));
}

void DeclarationBinderTester::case2055(){ }
void DeclarationBinderTester::case2056(){ }
void DeclarationBinderTester::case2057(){ }
void DeclarationBinderTester::case2058(){ }
void DeclarationBinderTester::case2059(){ }
void DeclarationBinderTester::case2060(){ }
void DeclarationBinderTester::case2061(){ }
void DeclarationBinderTester::case2062(){ }
void DeclarationBinderTester::case2063(){ }
void DeclarationBinderTester::case2064(){ }
void DeclarationBinderTester::case2065(){ }
void DeclarationBinderTester::case2066(){ }
void DeclarationBinderTester::case2067(){ }
void DeclarationBinderTester::case2068(){ }
void DeclarationBinderTester::case2069(){ }
void DeclarationBinderTester::case2070(){ }
void DeclarationBinderTester::case2071(){ }
void DeclarationBinderTester::case2072(){ }
void DeclarationBinderTester::case2073(){ }
void DeclarationBinderTester::case2074(){ }
void DeclarationBinderTester::case2075(){ }
void DeclarationBinderTester::case2076(){ }
void DeclarationBinderTester::case2077(){ }
void DeclarationBinderTester::case2078(){ }
void DeclarationBinderTester::case2079(){ }
void DeclarationBinderTester::case2080(){ }
void DeclarationBinderTester::case2081(){ }
void DeclarationBinderTester::case2082(){ }
void DeclarationBinderTester::case2083(){ }
void DeclarationBinderTester::case2084(){ }
void DeclarationBinderTester::case2085(){ }
void DeclarationBinderTester::case2086(){ }
void DeclarationBinderTester::case2087(){ }
void DeclarationBinderTester::case2088(){ }
void DeclarationBinderTester::case2089(){ }
void DeclarationBinderTester::case2090(){ }
void DeclarationBinderTester::case2091(){ }
void DeclarationBinderTester::case2092(){ }
void DeclarationBinderTester::case2093(){ }
void DeclarationBinderTester::case2094(){ }
void DeclarationBinderTester::case2095(){ }
void DeclarationBinderTester::case2096(){ }
void DeclarationBinderTester::case2097(){ }
void DeclarationBinderTester::case2098(){ }
void DeclarationBinderTester::case2099(){ }

void DeclarationBinderTester::case2100()
{
    bind("struct { int * x ; } ;",
         Expectation()
             .declaration(Decl().Member("x", MemberDeclarationCategory::Field)
                      .ty_.Basic(BasicTypeKind::Int_S)
                      .ty_.Derived(TypeKind::Pointer)));
}

void DeclarationBinderTester::case2101()
{
    bind("struct { x * y ; } ;",
         Expectation()
             .declaration(Decl().Member("y", MemberDeclarationCategory::Field)
                      .ty_.Typedef("x")
                      .ty_.Derived(TypeKind::Pointer)));
}

void DeclarationBinderTester::case2102()
{
    bind("struct { int * x ; y * z ; } ;",
         Expectation()
             .declaration(Decl().Member("x", MemberDeclarationCategory::Field)
                      .ty_.Basic(BasicTypeKind::Int_S)
                      .ty_.Derived(TypeKind::Pointer))
             .declaration(Decl().Member("z", MemberDeclarationCategory::Field)
                      .ty_.Typedef("y")
                      .ty_.Derived(TypeKind::Pointer)));
}

void DeclarationBinderTester::case2103()
{
    bind("struct { int * x , * y ; } ;",
         Expectation()
             .declaration(Decl().Member("x", MemberDeclarationCategory::Field)
                      .ty_.Basic(BasicTypeKind::Int_S)
                      .ty_.Derived(TypeKind::Pointer))
             .declaration(Decl().Member("y", MemberDeclarationCategory::Field)
                      .ty_.Basic(BasicTypeKind::Int_S)
                      .ty_.Derived(TypeKind::Pointer)));
}

void DeclarationBinderTester::case2104()
{
    bind("struct { int ( * x ) [ 1 ]; } ;",
         Expectation()
             .declaration(Decl().Member("x", MemberDeclarationCategory::Field)
                      .ty_.Basic(BasicTypeKind::Int_S)
                      .ty_.Derived(TypeKind::Array)
                      .ty_.Derived(TypeKind::Pointer)));
}

void DeclarationBinderTester::case2105()
{
    bind("struct { int * * x ; } ;",
         Expectation()
             .declaration(Decl().Member("x", MemberDeclarationCategory::Field)
                      .ty_.Basic(BasicTypeKind::Int_S)
                      .ty_.Derived(TypeKind::Pointer)
                      .ty_.Derived(TypeKind::Pointer)));
}

void DeclarationBinderTester::case2106()
{
    bind("struct { int * * * x ; } ;",
         Expectation()
             .declaration(Decl().Member("x", MemberDeclarationCategory::Field)
                      .ty_.Basic(BasicTypeKind::Int_S)
                      .ty_.Derived(TypeKind::Pointer)
                      .ty_.Derived(TypeKind::Pointer)
                      .ty_.Derived(TypeKind::Pointer)));
}

void DeclarationBinderTester::case2107()
{
    bind("struct { int * ( * x ) [ 1 ] ; };",
         Expectation()
             .declaration(Decl().Member("x", MemberDeclarationCategory::Field)
                      .ty_.Basic(BasicTypeKind::Int_S, CVR::None)
                      .ty_.Derived(TypeKind::Pointer, CVR::None)
                      .ty_.Derived(TypeKind::Array, CVR::None)
                      .ty_.Derived(TypeKind::Pointer, CVR::None)));
}

void DeclarationBinderTester::case2108(){ }
void DeclarationBinderTester::case2109(){ }
void DeclarationBinderTester::case2110(){ }
void DeclarationBinderTester::case2111(){ }
void DeclarationBinderTester::case2112(){ }
void DeclarationBinderTester::case2113(){ }
void DeclarationBinderTester::case2114(){ }
void DeclarationBinderTester::case2115(){ }
void DeclarationBinderTester::case2116(){ }
void DeclarationBinderTester::case2117(){ }
void DeclarationBinderTester::case2118(){ }
void DeclarationBinderTester::case2119(){ }
void DeclarationBinderTester::case2120(){ }
void DeclarationBinderTester::case2121(){ }
void DeclarationBinderTester::case2122(){ }
void DeclarationBinderTester::case2123(){ }
void DeclarationBinderTester::case2124(){ }
void DeclarationBinderTester::case2125(){ }
void DeclarationBinderTester::case2126(){ }
void DeclarationBinderTester::case2127(){ }
void DeclarationBinderTester::case2128(){ }
void DeclarationBinderTester::case2129(){ }
void DeclarationBinderTester::case2130(){ }
void DeclarationBinderTester::case2131(){ }
void DeclarationBinderTester::case2132(){ }
void DeclarationBinderTester::case2133(){ }
void DeclarationBinderTester::case2134(){ }
void DeclarationBinderTester::case2135(){ }
void DeclarationBinderTester::case2136(){ }
void DeclarationBinderTester::case2137(){ }
void DeclarationBinderTester::case2138(){ }
void DeclarationBinderTester::case2139(){ }
void DeclarationBinderTester::case2140(){ }
void DeclarationBinderTester::case2141(){ }
void DeclarationBinderTester::case2142(){ }
void DeclarationBinderTester::case2143(){ }
void DeclarationBinderTester::case2144(){ }
void DeclarationBinderTester::case2145(){ }
void DeclarationBinderTester::case2146(){ }
void DeclarationBinderTester::case2147(){ }
void DeclarationBinderTester::case2148(){ }
void DeclarationBinderTester::case2149(){ }

void DeclarationBinderTester::case2150()
{
    bind("struct { const int * x ; } ;",
         Expectation()
             .declaration(Decl().Member("x", MemberDeclarationCategory::Field)
                      .ty_.Basic(BasicTypeKind::Int_S, CVR::Const)
                      .ty_.Derived(TypeKind::Pointer)));
}

void DeclarationBinderTester::case2151()
{
    bind("struct { const x * y ; } ;",
         Expectation()
             .declaration(Decl().Member("y", MemberDeclarationCategory::Field)
                      .ty_.Typedef("x", CVR::Const)
                      .ty_.Derived(TypeKind::Pointer)));
}

void DeclarationBinderTester::case2152(){ }
void DeclarationBinderTester::case2153(){ }
void DeclarationBinderTester::case2154(){ }
void DeclarationBinderTester::case2155(){ }
void DeclarationBinderTester::case2156(){ }
void DeclarationBinderTester::case2157(){ }
void DeclarationBinderTester::case2158(){ }
void DeclarationBinderTester::case2159(){ }
void DeclarationBinderTester::case2160(){ }
void DeclarationBinderTester::case2161(){ }
void DeclarationBinderTester::case2162(){ }
void DeclarationBinderTester::case2163(){ }
void DeclarationBinderTester::case2164(){ }
void DeclarationBinderTester::case2165(){ }
void DeclarationBinderTester::case2166(){ }
void DeclarationBinderTester::case2167(){ }
void DeclarationBinderTester::case2168(){ }
void DeclarationBinderTester::case2169(){ }
void DeclarationBinderTester::case2170(){ }
void DeclarationBinderTester::case2171(){ }
void DeclarationBinderTester::case2172(){ }
void DeclarationBinderTester::case2173(){ }
void DeclarationBinderTester::case2174(){ }
void DeclarationBinderTester::case2175(){ }
void DeclarationBinderTester::case2176(){ }
void DeclarationBinderTester::case2177(){ }
void DeclarationBinderTester::case2178(){ }
void DeclarationBinderTester::case2179(){ }
void DeclarationBinderTester::case2180(){ }
void DeclarationBinderTester::case2181(){ }
void DeclarationBinderTester::case2182(){ }
void DeclarationBinderTester::case2183(){ }
void DeclarationBinderTester::case2184(){ }
void DeclarationBinderTester::case2185(){ }
void DeclarationBinderTester::case2186(){ }
void DeclarationBinderTester::case2187(){ }
void DeclarationBinderTester::case2188(){ }
void DeclarationBinderTester::case2189(){ }
void DeclarationBinderTester::case2190(){ }
void DeclarationBinderTester::case2191(){ }
void DeclarationBinderTester::case2192(){ }
void DeclarationBinderTester::case2193(){ }
void DeclarationBinderTester::case2194(){ }
void DeclarationBinderTester::case2195(){ }
void DeclarationBinderTester::case2196(){ }
void DeclarationBinderTester::case2197(){ }
void DeclarationBinderTester::case2198(){ }
void DeclarationBinderTester::case2199(){ }

void DeclarationBinderTester::case2200()
{
    bind("struct { const int * const x ; } ;",
         Expectation()
             .declaration(Decl().Member("x", MemberDeclarationCategory::Field)
                      .ty_.Basic(BasicTypeKind::Int_S, CVR::Const)
                      .ty_.Derived(TypeKind::Pointer, CVR::Const)));
}


void DeclarationBinderTester::case2201(){ }
void DeclarationBinderTester::case2202(){ }
void DeclarationBinderTester::case2203(){ }
void DeclarationBinderTester::case2204(){ }
void DeclarationBinderTester::case2205(){ }
void DeclarationBinderTester::case2206(){ }
void DeclarationBinderTester::case2207(){ }
void DeclarationBinderTester::case2208(){ }
void DeclarationBinderTester::case2209(){ }
void DeclarationBinderTester::case2210(){ }
void DeclarationBinderTester::case2211(){ }
void DeclarationBinderTester::case2212(){ }
void DeclarationBinderTester::case2213(){ }
void DeclarationBinderTester::case2214(){ }
void DeclarationBinderTester::case2215(){ }
void DeclarationBinderTester::case2216(){ }
void DeclarationBinderTester::case2217(){ }
void DeclarationBinderTester::case2218(){ }
void DeclarationBinderTester::case2219(){ }
void DeclarationBinderTester::case2220(){ }
void DeclarationBinderTester::case2221(){ }
void DeclarationBinderTester::case2222(){ }
void DeclarationBinderTester::case2223(){ }
void DeclarationBinderTester::case2224(){ }
void DeclarationBinderTester::case2225(){ }
void DeclarationBinderTester::case2226(){ }
void DeclarationBinderTester::case2227(){ }
void DeclarationBinderTester::case2228(){ }
void DeclarationBinderTester::case2229(){ }
void DeclarationBinderTester::case2230(){ }
void DeclarationBinderTester::case2231(){ }
void DeclarationBinderTester::case2232(){ }
void DeclarationBinderTester::case2233(){ }
void DeclarationBinderTester::case2234(){ }
void DeclarationBinderTester::case2235(){ }
void DeclarationBinderTester::case2236(){ }
void DeclarationBinderTester::case2237(){ }
void DeclarationBinderTester::case2238(){ }
void DeclarationBinderTester::case2239(){ }
void DeclarationBinderTester::case2240(){ }
void DeclarationBinderTester::case2241(){ }
void DeclarationBinderTester::case2242(){ }
void DeclarationBinderTester::case2243(){ }
void DeclarationBinderTester::case2244(){ }
void DeclarationBinderTester::case2245(){ }
void DeclarationBinderTester::case2246(){ }
void DeclarationBinderTester::case2247(){ }
void DeclarationBinderTester::case2248(){ }
void DeclarationBinderTester::case2249(){ }

void DeclarationBinderTester::case2250()
{
    bind("struct { int * const x ; } ;",
         Expectation()
             .declaration(Decl().Member("x", MemberDeclarationCategory::Field)
                      .ty_.Basic(BasicTypeKind::Int_S, CVR::None)
                      .ty_.Derived(TypeKind::Pointer, CVR::Const)));
}

void DeclarationBinderTester::case2251()
{
    bind("struct { x * const y ; } ;",
         Expectation()
             .declaration(Decl().Member("y", MemberDeclarationCategory::Field)
                      .ty_.Typedef("x", CVR::None)
                      .ty_.Derived(TypeKind::Pointer, CVR::Const)));
}

void DeclarationBinderTester::case2252(){ }
void DeclarationBinderTester::case2253(){ }
void DeclarationBinderTester::case2254(){ }
void DeclarationBinderTester::case2255(){ }
void DeclarationBinderTester::case2256(){ }
void DeclarationBinderTester::case2257(){ }
void DeclarationBinderTester::case2258(){ }
void DeclarationBinderTester::case2259(){ }
void DeclarationBinderTester::case2260(){ }
void DeclarationBinderTester::case2261(){ }
void DeclarationBinderTester::case2262(){ }
void DeclarationBinderTester::case2263(){ }
void DeclarationBinderTester::case2264(){ }
void DeclarationBinderTester::case2265(){ }
void DeclarationBinderTester::case2266(){ }
void DeclarationBinderTester::case2267(){ }
void DeclarationBinderTester::case2268(){ }
void DeclarationBinderTester::case2269(){ }
void DeclarationBinderTester::case2270(){ }
void DeclarationBinderTester::case2271(){ }
void DeclarationBinderTester::case2272(){ }
void DeclarationBinderTester::case2273(){ }
void DeclarationBinderTester::case2274(){ }
void DeclarationBinderTester::case2275(){ }
void DeclarationBinderTester::case2276(){ }
void DeclarationBinderTester::case2277(){ }
void DeclarationBinderTester::case2278(){ }
void DeclarationBinderTester::case2279(){ }
void DeclarationBinderTester::case2280(){ }
void DeclarationBinderTester::case2281(){ }
void DeclarationBinderTester::case2282(){ }
void DeclarationBinderTester::case2283(){ }
void DeclarationBinderTester::case2284(){ }
void DeclarationBinderTester::case2285(){ }
void DeclarationBinderTester::case2286(){ }
void DeclarationBinderTester::case2287(){ }
void DeclarationBinderTester::case2288(){ }
void DeclarationBinderTester::case2289(){ }
void DeclarationBinderTester::case2290(){ }
void DeclarationBinderTester::case2291(){ }
void DeclarationBinderTester::case2292(){ }
void DeclarationBinderTester::case2293(){ }
void DeclarationBinderTester::case2294(){ }
void DeclarationBinderTester::case2295(){ }
void DeclarationBinderTester::case2296(){ }
void DeclarationBinderTester::case2297(){ }
void DeclarationBinderTester::case2298(){ }
void DeclarationBinderTester::case2299(){ }

void DeclarationBinderTester::case2300()
{
    bind(R"(
struct
{
    int x [ 1 ] ;
};
         )",
         Expectation()
             .declaration(Decl().Member("x", MemberDeclarationCategory::Field)
                      .ty_.Basic(BasicTypeKind::Int_S, CVR::None)
                      .ty_.Derived(TypeKind::Array, CVR::None)));
}

void DeclarationBinderTester::case2301()
{
    bind("struct { x y [ 1 ] ; } ;",
         Expectation()
             .declaration(Decl().Member("y", MemberDeclarationCategory::Field)
                      .ty_.Typedef("x", CVR::None)
                      .ty_.Derived(TypeKind::Array, CVR::None)));
}

void DeclarationBinderTester::case2302()
{
    bind(R"(
struct
{
    int x [ 1 ] , y [ 2 ] ;
};
         )",
         Expectation()
             .declaration(Decl().Member("x", MemberDeclarationCategory::Field)
                      .ty_.Basic(BasicTypeKind::Int_S, CVR::None)
                      .ty_.Derived(TypeKind::Array, CVR::None))
             .declaration(Decl().Member("y", MemberDeclarationCategory::Field)
                      .ty_.Basic(BasicTypeKind::Int_S, CVR::None)
                      .ty_.Derived(TypeKind::Array, CVR::None)));
}

void DeclarationBinderTester::case2303()
{
    bind(R"(
struct
{
    x y [ 1 ] , z [ 2 ] ;
};
         )",
         Expectation()
             .declaration(Decl().Member("y", MemberDeclarationCategory::Field)
                      .ty_.Typedef("x", CVR::None)
                      .ty_.Derived(TypeKind::Array, CVR::None))
             .declaration(Decl().Member("z", MemberDeclarationCategory::Field)
                      .ty_.Typedef("x", CVR::None)
                      .ty_.Derived(TypeKind::Array, CVR::None)));
}

void DeclarationBinderTester::case2304()
{
    bind("struct { int * x [ 1 ] ; } ;",
         Expectation()
             .declaration(Decl().Member("x", MemberDeclarationCategory::Field)
                      .ty_.Basic(BasicTypeKind::Int_S, CVR::None)
                      .ty_.Derived(TypeKind::Pointer, CVR::None)
                      .ty_.Derived(TypeKind::Array, CVR::None)));
}

void DeclarationBinderTester::case2305()
{
    bind("struct { int x [ 1 ] , * y [ 2 ] ; } ;",
         Expectation()
             .declaration(Decl().Member("x", MemberDeclarationCategory::Field)
                      .ty_.Basic(BasicTypeKind::Int_S, CVR::None)
                      .ty_.Derived(TypeKind::Array, CVR::None))
             .declaration(Decl().Member("y", MemberDeclarationCategory::Field)
                      .ty_.Basic(BasicTypeKind::Int_S, CVR::None)
                      .ty_.Derived(TypeKind::Pointer, CVR::None)
                      .ty_.Derived(TypeKind::Array, CVR::None)));
}

void DeclarationBinderTester::case2306()
{
    bind("struct { int * * x [ 1 ] ; } ;",
         Expectation()
             .declaration(Decl().Member("x", MemberDeclarationCategory::Field)
                      .ty_.Basic(BasicTypeKind::Int_S, CVR::None)
                      .ty_.Derived(TypeKind::Pointer, CVR::None)
                      .ty_.Derived(TypeKind::Pointer, CVR::None)
                      .ty_.Derived(TypeKind::Array, CVR::None)));
}

void DeclarationBinderTester::case2307(){ }
void DeclarationBinderTester::case2308(){ }
void DeclarationBinderTester::case2309(){ }
void DeclarationBinderTester::case2310(){ }
void DeclarationBinderTester::case2311(){ }
void DeclarationBinderTester::case2312(){ }
void DeclarationBinderTester::case2313(){ }
void DeclarationBinderTester::case2314(){ }
void DeclarationBinderTester::case2315(){ }
void DeclarationBinderTester::case2316(){ }
void DeclarationBinderTester::case2317(){ }
void DeclarationBinderTester::case2318(){ }
void DeclarationBinderTester::case2319(){ }
void DeclarationBinderTester::case2320(){ }
void DeclarationBinderTester::case2321(){ }
void DeclarationBinderTester::case2322(){ }
void DeclarationBinderTester::case2323(){ }
void DeclarationBinderTester::case2324(){ }
void DeclarationBinderTester::case2325(){ }
void DeclarationBinderTester::case2326(){ }
void DeclarationBinderTester::case2327(){ }
void DeclarationBinderTester::case2328(){ }
void DeclarationBinderTester::case2329(){ }
void DeclarationBinderTester::case2330(){ }
void DeclarationBinderTester::case2331(){ }
void DeclarationBinderTester::case2332(){ }
void DeclarationBinderTester::case2333(){ }
void DeclarationBinderTester::case2334(){ }
void DeclarationBinderTester::case2335(){ }
void DeclarationBinderTester::case2336(){ }
void DeclarationBinderTester::case2337(){ }
void DeclarationBinderTester::case2338(){ }
void DeclarationBinderTester::case2339(){ }
void DeclarationBinderTester::case2340(){ }
void DeclarationBinderTester::case2341(){ }
void DeclarationBinderTester::case2342(){ }
void DeclarationBinderTester::case2343(){ }
void DeclarationBinderTester::case2344(){ }
void DeclarationBinderTester::case2345(){ }
void DeclarationBinderTester::case2346(){ }
void DeclarationBinderTester::case2347(){ }
void DeclarationBinderTester::case2348(){ }
void DeclarationBinderTester::case2349(){ }

void DeclarationBinderTester::case2350()
{
    bind("struct { const int x [ 1 ] ; } ;",
         Expectation()
             .declaration(Decl().Member("x", MemberDeclarationCategory::Field)
                      .ty_.Basic(BasicTypeKind::Int_S, CVR::Const)
                      .ty_.Derived(TypeKind::Array, CVR::None)));
}

void DeclarationBinderTester::case2351(){ }
void DeclarationBinderTester::case2352(){ }
void DeclarationBinderTester::case2353(){ }
void DeclarationBinderTester::case2354(){ }
void DeclarationBinderTester::case2355(){ }
void DeclarationBinderTester::case2356(){ }
void DeclarationBinderTester::case2357(){ }
void DeclarationBinderTester::case2358(){ }
void DeclarationBinderTester::case2359(){ }
void DeclarationBinderTester::case2360(){ }
void DeclarationBinderTester::case2361(){ }
void DeclarationBinderTester::case2362(){ }
void DeclarationBinderTester::case2363(){ }
void DeclarationBinderTester::case2364(){ }
void DeclarationBinderTester::case2365(){ }
void DeclarationBinderTester::case2366(){ }
void DeclarationBinderTester::case2367(){ }
void DeclarationBinderTester::case2368(){ }
void DeclarationBinderTester::case2369(){ }
void DeclarationBinderTester::case2370(){ }
void DeclarationBinderTester::case2371(){ }
void DeclarationBinderTester::case2372(){ }
void DeclarationBinderTester::case2373(){ }
void DeclarationBinderTester::case2374(){ }
void DeclarationBinderTester::case2375(){ }
void DeclarationBinderTester::case2376(){ }
void DeclarationBinderTester::case2377(){ }
void DeclarationBinderTester::case2378(){ }
void DeclarationBinderTester::case2379(){ }
void DeclarationBinderTester::case2380(){ }
void DeclarationBinderTester::case2381(){ }
void DeclarationBinderTester::case2382(){ }
void DeclarationBinderTester::case2383(){ }
void DeclarationBinderTester::case2384(){ }
void DeclarationBinderTester::case2385(){ }
void DeclarationBinderTester::case2386(){ }
void DeclarationBinderTester::case2387(){ }
void DeclarationBinderTester::case2388(){ }
void DeclarationBinderTester::case2389(){ }
void DeclarationBinderTester::case2390(){ }
void DeclarationBinderTester::case2391(){ }
void DeclarationBinderTester::case2392(){ }
void DeclarationBinderTester::case2393(){ }
void DeclarationBinderTester::case2394(){ }
void DeclarationBinderTester::case2395(){ }
void DeclarationBinderTester::case2396(){ }
void DeclarationBinderTester::case2397(){ }
void DeclarationBinderTester::case2398(){ }
void DeclarationBinderTester::case2399(){ }

void DeclarationBinderTester::case2400()
{
    bind(R"(
struct x
{
    short y : 1 ;
};
         )",
         Expectation()
            .declaration(Decl()
                         .Member("y", MemberDeclarationCategory::Field)
                         .ty_.Basic(BasicTypeKind::Short_S)));
}

void DeclarationBinderTester::case2401()
{
    bind(R"(
struct x
{
    short y : 1 , z : 1 ;
};
         )",
         Expectation()
         .declaration(Decl()
                      .Member("y", MemberDeclarationCategory::Field)
                      .ty_.Basic(BasicTypeKind::Short_S))
         .declaration(Decl()
                      .Member("z", MemberDeclarationCategory::Field)
                      .ty_.Basic(BasicTypeKind::Short_S)));
}

void DeclarationBinderTester::case2402()
{
    bind(R"(
struct x
{
    short y : 1 , : 7 ;
};
         )",
         Expectation()
         .declaration(Decl()
                      .Member("y", MemberDeclarationCategory::Field)
                      .ty_.Basic(BasicTypeKind::Short_S)));
}

void DeclarationBinderTester::case2403(){}
void DeclarationBinderTester::case2404(){}
void DeclarationBinderTester::case2405(){}
void DeclarationBinderTester::case2406(){}
void DeclarationBinderTester::case2407(){}
void DeclarationBinderTester::case2408(){}
void DeclarationBinderTester::case2409(){}
void DeclarationBinderTester::case2410(){}
void DeclarationBinderTester::case2411(){}
void DeclarationBinderTester::case2412(){}
void DeclarationBinderTester::case2413(){}
void DeclarationBinderTester::case2414(){}
void DeclarationBinderTester::case2415(){}
void DeclarationBinderTester::case2416(){}
void DeclarationBinderTester::case2417(){}
void DeclarationBinderTester::case2418(){}
void DeclarationBinderTester::case2419(){}
void DeclarationBinderTester::case2420(){}
void DeclarationBinderTester::case2421(){}
void DeclarationBinderTester::case2422(){}
void DeclarationBinderTester::case2423(){}
void DeclarationBinderTester::case2424(){}
void DeclarationBinderTester::case2425(){}
void DeclarationBinderTester::case2426(){}
void DeclarationBinderTester::case2427(){}
void DeclarationBinderTester::case2428(){}
void DeclarationBinderTester::case2429(){}
void DeclarationBinderTester::case2430(){}
void DeclarationBinderTester::case2431(){}
void DeclarationBinderTester::case2432(){}
void DeclarationBinderTester::case2433(){}
void DeclarationBinderTester::case2434(){}
void DeclarationBinderTester::case2435(){}
void DeclarationBinderTester::case2436(){}
void DeclarationBinderTester::case2437(){}
void DeclarationBinderTester::case2438(){}
void DeclarationBinderTester::case2439(){}
void DeclarationBinderTester::case2440(){}
void DeclarationBinderTester::case2441(){}
void DeclarationBinderTester::case2442(){}
void DeclarationBinderTester::case2443(){}
void DeclarationBinderTester::case2444(){}
void DeclarationBinderTester::case2445(){}
void DeclarationBinderTester::case2446(){}
void DeclarationBinderTester::case2447(){}
void DeclarationBinderTester::case2448(){}
void DeclarationBinderTester::case2449(){}
