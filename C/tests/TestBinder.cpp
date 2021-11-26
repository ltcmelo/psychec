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

#include "TestBinder.h"

#include "syntax/SyntaxLexemes.h"
#include "TestParser.h"
#include "Unparser.h"

#include "syntax/SyntaxNodes.h"

#include <algorithm>
#include <cstring>
#include <iostream>
#include <string>
#include <sstream>

using namespace psy;
using namespace C;

void TestBinder::testAll()
{
    std::cout << "    binding" << std::endl;
    run<TestBinder>(tests_);
    std::cout << std::endl;
}

void TestBinder::setUp()
{}

void TestBinder::tearDown()
{}

void TestBinder::case0001()
{
    CROSS_REFERENCE_TEST(TestParser::case0028);

    parseAndBind("const int ;", 0, 1);
}

void TestBinder::case0002()
{
    parseAndBind("int x ;");
}

void TestBinder::case0003()
{
    parseAndBind("void x ( ) ;");
}

void TestBinder::case0004()
{
    parseAndBind("int x ( ) ;");
}

void TestBinder::case0005()
{
    CROSS_REFERENCE_TEST(TestParser::case0021);

    parseAndBind("int double x ;", 1);
}

void TestBinder::case0006()
{
    CROSS_REFERENCE_TEST(TestParser::case0416);

    parseAndBind("struct x { y ; } ;");
}

void TestBinder::case0007()
{
    CROSS_REFERENCE_TEST(TestParser::case0422);

    parseAndBind("struct x { int ; } ;");
}

void TestBinder::case0008()
{
    CROSS_REFERENCE_TEST(TestParser::case0204);

    // Function returning a function.

    parseAndBind("void x ( ) ( ) { }", 1);
}

void TestBinder::case0009()
{
    CROSS_REFERENCE_TEST(TestParser::case0205);

    // Function returning a function.

    parseAndBind("void x ( ) ( ) ;", 1);
}

void TestBinder::case0010()
{
    CROSS_REFERENCE_TEST(TestParser::case0206);

    // Function returning an array.

    parseAndBind("int x ( ) [ ] ;");
}

void TestBinder::case0011()
{
    CROSS_REFERENCE_TEST(TestParser::case0207);

    parseAndBind("int ( x ) ( ) [ ] ;");
}

void TestBinder::case0012()
{
    CROSS_REFERENCE_TEST(TestParser::case0212);

    // 6.9.1-5

    parseAndBind("void x ( int ) { }", 1);
}

void TestBinder::case0013()
{
    CROSS_REFERENCE_TEST(TestParser::case0040);

    parseAndBind("int ;", 1);
}

void TestBinder::case0014()
{
    CROSS_REFERENCE_TEST(TestParser::case0406);

    // Anonymous `struct' declaration without any variable declared
    // with such type.

    parseAndBind("struct { int x ; } ;");
}

void TestBinder::case0015()
{
    CROSS_REFERENCE_TEST(TestParser::case0632);

    parseAndBind("void x ( ) { __thread x y ; }");
}

void TestBinder::case0016()
{
    CROSS_REFERENCE_TEST(TestParser::case0050);

    parseAndBind("double _Complex int x ;");
}

void TestBinder::case0017()
{
    CROSS_REFERENCE_TEST(TestParser::case0052);

    parseAndBind("int int x ;");
}

void TestBinder::case0018()
{
    CROSS_REFERENCE_TEST(TestParser::case0053);

    parseAndBind("_Complex _Complex x ;");
}

void TestBinder::case0019()
{
    CROSS_REFERENCE_TEST(TestParser::case0435);

    parseAndBind("int struct x { int y ; } z ;");
}

void TestBinder::case0020()
{
    CROSS_REFERENCE_TEST(TestParser::case0436);

    parseAndBind("struct x struct { int y ; } z ;");
}

void TestBinder::case0021()
{
    CROSS_REFERENCE_TEST(TestParser::case0437);

    parse("int struct x y ;");
}

void TestBinder::case0022()
{
    CROSS_REFERENCE_TEST(TestParser::case0438);

    parse("struct x int y ;");
}

void TestBinder::case0023() {}
void TestBinder::case0024() {}
void TestBinder::case0025() {}
void TestBinder::case0026() {}
void TestBinder::case0027() {}
void TestBinder::case0028() {}
void TestBinder::case0029() {}

void TestBinder::case0030() {}
void TestBinder::case0031() {}
void TestBinder::case0032() {}
void TestBinder::case0033() {}
void TestBinder::case0034() {}
void TestBinder::case0035() {}
void TestBinder::case0036() {}
void TestBinder::case0037() {}
void TestBinder::case0038() {}
void TestBinder::case0039() {}

void TestBinder::case0040() {}
void TestBinder::case0041() {}
void TestBinder::case0042() {}
void TestBinder::case0043() {}
void TestBinder::case0044() {}
void TestBinder::case0045() {}
void TestBinder::case0046() {}
void TestBinder::case0047() {}
void TestBinder::case0048() {}
void TestBinder::case0049() {}

void TestBinder::case0050() {}
void TestBinder::case0051() {}
void TestBinder::case0052() {}
void TestBinder::case0053() {}
void TestBinder::case0054() {}
void TestBinder::case0055() {}
void TestBinder::case0056() {}
void TestBinder::case0057() {}
void TestBinder::case0058() {}
void TestBinder::case0059() {}

void TestBinder::case0060() {}
void TestBinder::case0061() {}
void TestBinder::case0062() {}
void TestBinder::case0063() {}
void TestBinder::case0064() {}
void TestBinder::case0065() {}
void TestBinder::case0066() {}
void TestBinder::case0067() {}
void TestBinder::case0068() {}
void TestBinder::case0069() {}

void TestBinder::case0070() {}
void TestBinder::case0071() {}
void TestBinder::case0072() {}
void TestBinder::case0073() {}
void TestBinder::case0074() {}
void TestBinder::case0075() {}
void TestBinder::case0076() {}
void TestBinder::case0077() {}
void TestBinder::case0078() {}
void TestBinder::case0079() {}

void TestBinder::case0080() {}
void TestBinder::case0081() {}
void TestBinder::case0082() {}
void TestBinder::case0083() {}
void TestBinder::case0084() {}
void TestBinder::case0085() {}
void TestBinder::case0086() {}
void TestBinder::case0087() {}
void TestBinder::case0088() {}
void TestBinder::case0089() {}

void TestBinder::case0090() {}
void TestBinder::case0091() {}
void TestBinder::case0092() {}
void TestBinder::case0093() {}
void TestBinder::case0094() {}
void TestBinder::case0095() {}
void TestBinder::case0096() {}
void TestBinder::case0097() {}
void TestBinder::case0098() {}
void TestBinder::case0099() {}

void TestBinder::case0101()
{
    parseAndBind("*1;", 1);
}

void TestBinder::case0102()
{
    parseAndBind("&1;", 1);
}

void TestBinder::case0103()
{
}

void TestBinder::case0104()
{
}

void TestBinder::case0105()
{
}

void TestBinder::case0106()
{

}

void TestBinder::case0107()
{

}

void TestBinder::case0108()
{
}

void TestBinder::case0109()
{
}

void TestBinder::case0110()
{
}

void TestBinder::case0111()
{
}

void TestBinder::case0112()
{
}

void TestBinder::case0113()
{
}

void TestBinder::case0114()
{
}

void TestBinder::case0115() {}
void TestBinder::case0116() {}
void TestBinder::case0117() {}
void TestBinder::case0118() {}
void TestBinder::case0119() {}

void TestBinder::case0120() {}
void TestBinder::case0121() {}
void TestBinder::case0122() {}
void TestBinder::case0123() {}
void TestBinder::case0124() {}
void TestBinder::case0125() {}
void TestBinder::case0126() {}
void TestBinder::case0127() {}
void TestBinder::case0128() {}
void TestBinder::case0129() {}

void TestBinder::case0130() {}
void TestBinder::case0131() {}
void TestBinder::case0132() {}
void TestBinder::case0133() {}
void TestBinder::case0134() {}
void TestBinder::case0135() {}
void TestBinder::case0136() {}
void TestBinder::case0137() {}
void TestBinder::case0138() {}
void TestBinder::case0139() {}

void TestBinder::case0140() {}
void TestBinder::case0141() {}
void TestBinder::case0142() {}
void TestBinder::case0143() {}
void TestBinder::case0144() {}
void TestBinder::case0145() {}
void TestBinder::case0146() {}
void TestBinder::case0147() {}
void TestBinder::case0148() {}
void TestBinder::case0149() {}

void TestBinder::case0150() {}
void TestBinder::case0151() {}
void TestBinder::case0152() {}
void TestBinder::case0153() {}
void TestBinder::case0154() {}
void TestBinder::case0155() {}
void TestBinder::case0156() {}
void TestBinder::case0157() {}
void TestBinder::case0158() {}
void TestBinder::case0159() {}

void TestBinder::case0160() {}
void TestBinder::case0161() {}
void TestBinder::case0162() {}
void TestBinder::case0163() {}
void TestBinder::case0164() {}
void TestBinder::case0165() {}
void TestBinder::case0166() {}
void TestBinder::case0167() {}
void TestBinder::case0168() {}
void TestBinder::case0169() {}

void TestBinder::case0170() {}
void TestBinder::case0171() {}
void TestBinder::case0172() {}
void TestBinder::case0173() {}
void TestBinder::case0174() {}
void TestBinder::case0175() {}
void TestBinder::case0176() {}
void TestBinder::case0177() {}
void TestBinder::case0178() {}
void TestBinder::case0179() {}

void TestBinder::case0180() {}
void TestBinder::case0181() {}
void TestBinder::case0182() {}
void TestBinder::case0183() {}
void TestBinder::case0184() {}
void TestBinder::case0185() {}
void TestBinder::case0186() {}
void TestBinder::case0187() {}
void TestBinder::case0188() {}
void TestBinder::case0189() {}

void TestBinder::case0190() {}
void TestBinder::case0191() {}
void TestBinder::case0192() {}
void TestBinder::case0193() {}
void TestBinder::case0194() {}
void TestBinder::case0195() {}
void TestBinder::case0196() {}
void TestBinder::case0197() {}
void TestBinder::case0198() {}
void TestBinder::case0199() {}
