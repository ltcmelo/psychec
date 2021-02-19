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

#ifndef PSYCHE_C_TEST_BINDING_H__
#define PSYCHE_C_TEST_BINDING_H__

#include "TestFrontend.h"

#define TEST_BINDER(Function) TestFunction { &TestBinder::Function, #Function }

namespace psy {
namespace C {

class TestBinder final : public TestFrontend
{
public:
    void testAll() override;

    void case0001();
    void case0002();
    void case0003();
    void case0004();
    void case0005();
    void case0006();
    void case0007();
    void case0008();
    void case0009();
    void case0010();
    void case0011();
    void case0012();
    void case0013();
    void case0014();
    void case0015();
    void case0016();
    void case0017();
    void case0018();
    void case0019();
    void case0020();
    void case0021();
    void case0022();
    void case0023();
    void case0024();
    void case0025();
    void case0026();
    void case0027();
    void case0028();
    void case0029();
    void case0030();
    void case0031();
    void case0032();
    void case0033();
    void case0034();
    void case0035();
    void case0036();
    void case0037();
    void case0038();
    void case0039();
    void case0040();
    void case0041();
    void case0042();
    void case0043();
    void case0044();
    void case0045();
    void case0046();
    void case0047();
    void case0048();
    void case0049();
    void case0050();
    void case0051();
    void case0052();
    void case0053();
    void case0054();
    void case0055();
    void case0056();
    void case0057();
    void case0058();
    void case0059();
    void case0060();
    void case0061();
    void case0062();
    void case0063();
    void case0064();
    void case0065();
    void case0066();
    void case0067();
    void case0068();
    void case0069();
    void case0070();
    void case0071();
    void case0072();
    void case0073();
    void case0074();
    void case0075();
    void case0076();
    void case0077();
    void case0078();
    void case0079();
    void case0080();
    void case0081();
    void case0082();
    void case0083();
    void case0084();
    void case0085();
    void case0086();
    void case0087();
    void case0088();
    void case0089();
    void case0090();
    void case0091();
    void case0092();
    void case0093();
    void case0094();
    void case0095();
    void case0096();
    void case0097();
    void case0098();
    void case0099();

    void case0101();
    void case0102();
    void case0103();
    void case0104();
    void case0105();
    void case0106();
    void case0107();
    void case0108();
    void case0109();
    void case0110();
    void case0111();
    void case0112();
    void case0113();
    void case0114();
    void case0115();
    void case0116();
    void case0117();
    void case0118();
    void case0119();
    void case0120();
    void case0121();
    void case0122();
    void case0123();
    void case0124();
    void case0125();
    void case0126();
    void case0127();
    void case0128();
    void case0129();
    void case0130();
    void case0131();
    void case0132();
    void case0133();
    void case0134();
    void case0135();
    void case0136();
    void case0137();
    void case0138();
    void case0139();
    void case0140();
    void case0141();
    void case0142();
    void case0143();
    void case0144();
    void case0145();
    void case0146();
    void case0147();
    void case0148();
    void case0149();
    void case0150();
    void case0151();
    void case0152();
    void case0153();
    void case0154();
    void case0155();
    void case0156();
    void case0157();
    void case0158();
    void case0159();
    void case0160();
    void case0161();
    void case0162();
    void case0163();
    void case0164();
    void case0165();
    void case0166();
    void case0167();
    void case0168();
    void case0169();
    void case0170();
    void case0171();
    void case0172();
    void case0173();
    void case0174();
    void case0175();
    void case0176();
    void case0177();
    void case0178();
    void case0179();
    void case0180();
    void case0181();
    void case0182();
    void case0183();
    void case0184();
    void case0185();
    void case0186();
    void case0187();
    void case0188();
    void case0189();
    void case0190();
    void case0191();
    void case0192();
    void case0193();
    void case0194();
    void case0195();
    void case0196();
    void case0197();
    void case0198();
    void case0199();

private:
    using TestFunction = std::pair<std::function<void(TestBinder*)>, const char*>;

    void setUp() override;
    void tearDown() override;

    void f();

    std::vector<TestFunction> tests_
    {
        TEST_BINDER(case0001),
        TEST_BINDER(case0002),
        TEST_BINDER(case0003),
        TEST_BINDER(case0004),
        TEST_BINDER(case0005),
        TEST_BINDER(case0006),
        TEST_BINDER(case0007),
        TEST_BINDER(case0008),
        TEST_BINDER(case0009),
        TEST_BINDER(case0010),
        TEST_BINDER(case0011),
        TEST_BINDER(case0012),
        TEST_BINDER(case0013),
        TEST_BINDER(case0014),
        TEST_BINDER(case0015),
        TEST_BINDER(case0016),
        TEST_BINDER(case0017),
        TEST_BINDER(case0018),
        TEST_BINDER(case0019),
        TEST_BINDER(case0020),
        TEST_BINDER(case0021),
        TEST_BINDER(case0022),
        TEST_BINDER(case0023),
        TEST_BINDER(case0024),
        TEST_BINDER(case0025),
        TEST_BINDER(case0026),
        TEST_BINDER(case0027),
        TEST_BINDER(case0028),
        TEST_BINDER(case0029),
        TEST_BINDER(case0030),
        TEST_BINDER(case0031),
        TEST_BINDER(case0032),
        TEST_BINDER(case0033),
        TEST_BINDER(case0034),
        TEST_BINDER(case0035),
        TEST_BINDER(case0036),
        TEST_BINDER(case0037),
        TEST_BINDER(case0038),
        TEST_BINDER(case0039),
        TEST_BINDER(case0040),
        TEST_BINDER(case0041),
        TEST_BINDER(case0042),
        TEST_BINDER(case0043),
        TEST_BINDER(case0044),
        TEST_BINDER(case0045),
        TEST_BINDER(case0046),
        TEST_BINDER(case0047),
        TEST_BINDER(case0048),
        TEST_BINDER(case0049),
        TEST_BINDER(case0050),
        TEST_BINDER(case0051),
        TEST_BINDER(case0052),
        TEST_BINDER(case0053),
        TEST_BINDER(case0054),
        TEST_BINDER(case0055),
        TEST_BINDER(case0056),
        TEST_BINDER(case0057),
        TEST_BINDER(case0058),
        TEST_BINDER(case0059),
        TEST_BINDER(case0060),
        TEST_BINDER(case0061),
        TEST_BINDER(case0062),
        TEST_BINDER(case0063),
        TEST_BINDER(case0064),
        TEST_BINDER(case0065),
        TEST_BINDER(case0066),
        TEST_BINDER(case0067),
        TEST_BINDER(case0068),
        TEST_BINDER(case0069),
        TEST_BINDER(case0070),
        TEST_BINDER(case0071),
        TEST_BINDER(case0072),
        TEST_BINDER(case0073),
        TEST_BINDER(case0074),
        TEST_BINDER(case0075),
        TEST_BINDER(case0076),
        TEST_BINDER(case0077),
        TEST_BINDER(case0078),
        TEST_BINDER(case0079),
        TEST_BINDER(case0080),
        TEST_BINDER(case0081),
        TEST_BINDER(case0082),
        TEST_BINDER(case0083),
        TEST_BINDER(case0084),
        TEST_BINDER(case0085),
        TEST_BINDER(case0086),
        TEST_BINDER(case0087),
        TEST_BINDER(case0088),
        TEST_BINDER(case0089),
        TEST_BINDER(case0090),
        TEST_BINDER(case0091),
        TEST_BINDER(case0092),
        TEST_BINDER(case0093),
        TEST_BINDER(case0094),
        TEST_BINDER(case0095),
        TEST_BINDER(case0096),
        TEST_BINDER(case0097),
        TEST_BINDER(case0098),
        TEST_BINDER(case0099),

        TEST_BINDER(case0101),
        TEST_BINDER(case0102),
        TEST_BINDER(case0103),
        TEST_BINDER(case0104),
        TEST_BINDER(case0105),
        TEST_BINDER(case0106),
        TEST_BINDER(case0107),
        TEST_BINDER(case0108),
        TEST_BINDER(case0109),
        TEST_BINDER(case0110),
        TEST_BINDER(case0111),
        TEST_BINDER(case0112),
        TEST_BINDER(case0113),
        TEST_BINDER(case0114),
        TEST_BINDER(case0115),
        TEST_BINDER(case0116),
        TEST_BINDER(case0117),
        TEST_BINDER(case0118),
        TEST_BINDER(case0119),
        TEST_BINDER(case0120),
        TEST_BINDER(case0121),
        TEST_BINDER(case0122),
        TEST_BINDER(case0123),
        TEST_BINDER(case0124),
        TEST_BINDER(case0125),
        TEST_BINDER(case0126),
        TEST_BINDER(case0127),
        TEST_BINDER(case0128),
        TEST_BINDER(case0129),
        TEST_BINDER(case0130),
        TEST_BINDER(case0131),
        TEST_BINDER(case0132),
        TEST_BINDER(case0133),
        TEST_BINDER(case0134),
        TEST_BINDER(case0135),
        TEST_BINDER(case0136),
        TEST_BINDER(case0137),
        TEST_BINDER(case0138),
        TEST_BINDER(case0139),
        TEST_BINDER(case0140),
        TEST_BINDER(case0141),
        TEST_BINDER(case0142),
        TEST_BINDER(case0143),
        TEST_BINDER(case0144),
        TEST_BINDER(case0145),
        TEST_BINDER(case0146),
        TEST_BINDER(case0147),
        TEST_BINDER(case0148),
        TEST_BINDER(case0149),
        TEST_BINDER(case0150),
        TEST_BINDER(case0151),
        TEST_BINDER(case0152),
        TEST_BINDER(case0153),
        TEST_BINDER(case0154),
        TEST_BINDER(case0155),
        TEST_BINDER(case0156),
        TEST_BINDER(case0157),
        TEST_BINDER(case0158),
        TEST_BINDER(case0159),
        TEST_BINDER(case0160),
        TEST_BINDER(case0161),
        TEST_BINDER(case0162),
        TEST_BINDER(case0163),
        TEST_BINDER(case0164),
        TEST_BINDER(case0165),
        TEST_BINDER(case0166),
        TEST_BINDER(case0167),
        TEST_BINDER(case0168),
        TEST_BINDER(case0169),
        TEST_BINDER(case0170),
        TEST_BINDER(case0171),
        TEST_BINDER(case0172),
        TEST_BINDER(case0173),
        TEST_BINDER(case0174),
        TEST_BINDER(case0175),
        TEST_BINDER(case0176),
        TEST_BINDER(case0177),
        TEST_BINDER(case0178),
        TEST_BINDER(case0179),
        TEST_BINDER(case0180),
        TEST_BINDER(case0181),
        TEST_BINDER(case0182),
        TEST_BINDER(case0183),
        TEST_BINDER(case0184),
        TEST_BINDER(case0185),
        TEST_BINDER(case0186),
        TEST_BINDER(case0187),
        TEST_BINDER(case0188),
        TEST_BINDER(case0189),
        TEST_BINDER(case0190),
        TEST_BINDER(case0191),
        TEST_BINDER(case0192),
        TEST_BINDER(case0193),
        TEST_BINDER(case0194),
        TEST_BINDER(case0195),
        TEST_BINDER(case0196),
        TEST_BINDER(case0197),
        TEST_BINDER(case0198),
        TEST_BINDER(case0199)

    };
};

} // C
} // psy

#endif
