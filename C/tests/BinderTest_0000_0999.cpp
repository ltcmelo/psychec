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

#include "BinderTest.h"

#include "ParserTest.h"

#include "syntax/SyntaxLexemes.h"
#include "parser/Unparser.h"

#include "syntax/SyntaxNodes.h"

#include <algorithm>
#include <cstring>
#include <iostream>
#include <string>
#include <sstream>

using namespace psy;
using namespace C;

void BinderTest::case0001()
{
    CROSS_REFERENCE_TEST(ParserTest::case0028);

    bind("const int ;");
}

void BinderTest::case0002()
{
    bind("int x ;");
}

void BinderTest::case0003()
{
    bind("void x ( ) ;");
}

void BinderTest::case0004()
{
    bind("int x ( ) ;");
}

void BinderTest::case0005()
{
    CROSS_REFERENCE_TEST(ParserTest::case0021);

    bind("int double x ;");
}

void BinderTest::case0006()
{
    CROSS_REFERENCE_TEST(ParserTest::case0416);

    bind("struct x { y ; } ;");
}

void BinderTest::case0007()
{
    CROSS_REFERENCE_TEST(ParserTest::case0422);

    bind("struct x { int ; } ;");
}

void BinderTest::case0008()
{
    CROSS_REFERENCE_TEST(ParserTest::case0204);

    // Function returning a function.

    bind("void x ( ) ( ) { }");
}

void BinderTest::case0009()
{
    CROSS_REFERENCE_TEST(ParserTest::case0205);

    // Function returning a function.

    bind("void x ( ) ( ) ;");
}

void BinderTest::case0010()
{
    CROSS_REFERENCE_TEST(ParserTest::case0206);

    // Function returning an array.

    bind("int x ( ) [ ] ;");
}

void BinderTest::case0011()
{
    CROSS_REFERENCE_TEST(ParserTest::case0207);

    bind("int ( x ) ( ) [ ] ;");
}

void BinderTest::case0012()
{
    CROSS_REFERENCE_TEST(ParserTest::case0212);

    // 6.9.1-5

    bind("void x ( int ) { }");
}

void BinderTest::case0013()
{
    CROSS_REFERENCE_TEST(ParserTest::case0040);

    bind("int ;");
}

void BinderTest::case0014()
{
    CROSS_REFERENCE_TEST(ParserTest::case0406);

    // Anonymous `struct' declaration without any variable declared
    // with such type.

    bind("struct { int x ; } ;");
}

void BinderTest::case0015()
{
    CROSS_REFERENCE_TEST(ParserTest::case0632);

    bind("void x ( ) { __thread x y ; }");
}

void BinderTest::case0016()
{
    CROSS_REFERENCE_TEST(ParserTest::case0050);

    bind("double _Complex int x ;");
}

void BinderTest::case0017()
{
    CROSS_REFERENCE_TEST(ParserTest::case0052);

    bind("int int x ;");
}

void BinderTest::case0018()
{
    CROSS_REFERENCE_TEST(ParserTest::case0053);

    bind("_Complex _Complex x ;");
}

void BinderTest::case0019()
{
    CROSS_REFERENCE_TEST(ParserTest::case0435);

    bind("int struct x { int y ; } z ;");
}

void BinderTest::case0020()
{
    CROSS_REFERENCE_TEST(ParserTest::case0436);

    bind("struct x struct { int y ; } z ;");
}

void BinderTest::case0021()
{
    CROSS_REFERENCE_TEST(ParserTest::case0437);

    parse("int struct x y ;");
}

void BinderTest::case0022()
{
    CROSS_REFERENCE_TEST(ParserTest::case0438);

    parse("struct x int y ;");
}

void BinderTest::case0023()
{
    CROSS_REFERENCE_TEST(ParserTest::case0737);

    bind("int __attribute__ ( ( ) ) double ;");
}

void BinderTest::case0024()
{
    CROSS_REFERENCE_TEST(ParserTest::case0738);

    bind("x __attribute__ ( ( ) ) int ;");
}

void BinderTest::case0025()
{
    CROSS_REFERENCE_TEST(ParserTest::case0297);

    bind("void x ( int y ) int y ; { }");
}

void BinderTest::case0026()
{
    CROSS_REFERENCE_TEST(ParserTest::case0298);

    bind("int x ( int y ) z y ; { }");
}

void BinderTest::case0027() {}
void BinderTest::case0028() {}
void BinderTest::case0029() {}

void BinderTest::case0030() {}
void BinderTest::case0031() {}
void BinderTest::case0032() {}
void BinderTest::case0033() {}
void BinderTest::case0034() {}
void BinderTest::case0035() {}
void BinderTest::case0036() {}
void BinderTest::case0037() {}
void BinderTest::case0038() {}
void BinderTest::case0039() {}

void BinderTest::case0040() {}
void BinderTest::case0041() {}
void BinderTest::case0042() {}
void BinderTest::case0043() {}
void BinderTest::case0044() {}
void BinderTest::case0045() {}
void BinderTest::case0046() {}
void BinderTest::case0047() {}
void BinderTest::case0048() {}
void BinderTest::case0049() {}

void BinderTest::case0050() {}
void BinderTest::case0051() {}
void BinderTest::case0052() {}
void BinderTest::case0053() {}
void BinderTest::case0054() {}
void BinderTest::case0055() {}
void BinderTest::case0056() {}
void BinderTest::case0057() {}
void BinderTest::case0058() {}
void BinderTest::case0059() {}

void BinderTest::case0060() {}
void BinderTest::case0061() {}
void BinderTest::case0062() {}
void BinderTest::case0063() {}
void BinderTest::case0064() {}
void BinderTest::case0065() {}
void BinderTest::case0066() {}
void BinderTest::case0067() {}
void BinderTest::case0068() {}
void BinderTest::case0069() {}

void BinderTest::case0070() {}
void BinderTest::case0071() {}
void BinderTest::case0072() {}
void BinderTest::case0073() {}
void BinderTest::case0074() {}
void BinderTest::case0075() {}
void BinderTest::case0076() {}
void BinderTest::case0077() {}
void BinderTest::case0078() {}
void BinderTest::case0079() {}

void BinderTest::case0080() {}
void BinderTest::case0081() {}
void BinderTest::case0082() {}
void BinderTest::case0083() {}
void BinderTest::case0084() {}
void BinderTest::case0085() {}
void BinderTest::case0086() {}
void BinderTest::case0087() {}
void BinderTest::case0088() {}
void BinderTest::case0089() {}

void BinderTest::case0090() {}
void BinderTest::case0091() {}
void BinderTest::case0092() {}
void BinderTest::case0093() {}
void BinderTest::case0094() {}
void BinderTest::case0095() {}
void BinderTest::case0096() {}
void BinderTest::case0097() {}
void BinderTest::case0098() {}
void BinderTest::case0099() {}

void BinderTest::case0100()
{
    bind("*1;");
}

void BinderTest::case0101()
{
    bind("&1;");
}

void BinderTest::case0102()
{
}

void BinderTest::case0103()
{
}

void BinderTest::case0104()
{
}

void BinderTest::case0105()
{
}

void BinderTest::case0106()
{

}

void BinderTest::case0107()
{

}

void BinderTest::case0108()
{
}

void BinderTest::case0109()
{
}

void BinderTest::case0110()
{
}

void BinderTest::case0111()
{
}

void BinderTest::case0112()
{
}

void BinderTest::case0113()
{
}

void BinderTest::case0114()
{
}

void BinderTest::case0115() {}
void BinderTest::case0116() {}
void BinderTest::case0117() {}
void BinderTest::case0118() {}
void BinderTest::case0119() {}

void BinderTest::case0120() {}
void BinderTest::case0121() {}
void BinderTest::case0122() {}
void BinderTest::case0123() {}
void BinderTest::case0124() {}
void BinderTest::case0125() {}
void BinderTest::case0126() {}
void BinderTest::case0127() {}
void BinderTest::case0128() {}
void BinderTest::case0129() {}

void BinderTest::case0130() {}
void BinderTest::case0131() {}
void BinderTest::case0132() {}
void BinderTest::case0133() {}
void BinderTest::case0134() {}
void BinderTest::case0135() {}
void BinderTest::case0136() {}
void BinderTest::case0137() {}
void BinderTest::case0138() {}
void BinderTest::case0139() {}

void BinderTest::case0140() {}
void BinderTest::case0141() {}
void BinderTest::case0142() {}
void BinderTest::case0143() {}
void BinderTest::case0144() {}
void BinderTest::case0145() {}
void BinderTest::case0146() {}
void BinderTest::case0147() {}
void BinderTest::case0148() {}
void BinderTest::case0149() {}

void BinderTest::case0150() {}
void BinderTest::case0151() {}
void BinderTest::case0152() {}
void BinderTest::case0153() {}
void BinderTest::case0154() {}
void BinderTest::case0155() {}
void BinderTest::case0156() {}
void BinderTest::case0157() {}
void BinderTest::case0158() {}
void BinderTest::case0159() {}

void BinderTest::case0160() {}
void BinderTest::case0161() {}
void BinderTest::case0162() {}
void BinderTest::case0163() {}
void BinderTest::case0164() {}
void BinderTest::case0165() {}
void BinderTest::case0166() {}
void BinderTest::case0167() {}
void BinderTest::case0168() {}
void BinderTest::case0169() {}

void BinderTest::case0170() {}
void BinderTest::case0171() {}
void BinderTest::case0172() {}
void BinderTest::case0173() {}
void BinderTest::case0174() {}
void BinderTest::case0175() {}
void BinderTest::case0176() {}
void BinderTest::case0177() {}
void BinderTest::case0178() {}
void BinderTest::case0179() {}

void BinderTest::case0180() {}
void BinderTest::case0181() {}
void BinderTest::case0182() {}
void BinderTest::case0183() {}
void BinderTest::case0184() {}
void BinderTest::case0185() {}
void BinderTest::case0186() {}
void BinderTest::case0187() {}
void BinderTest::case0188() {}
void BinderTest::case0189() {}

void BinderTest::case0190() {}
void BinderTest::case0191() {}
void BinderTest::case0192() {}
void BinderTest::case0193() {}
void BinderTest::case0194() {}
void BinderTest::case0195() {}
void BinderTest::case0196() {}
void BinderTest::case0197() {}
void BinderTest::case0198() {}
void BinderTest::case0199() {}
