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

#ifndef PSYCHE_C_REPARSER_TESTER_H__
#define PSYCHE_C_REPARSER_TESTER_H__

#include "Fwds.h"
#include "TestSuite_Internals.h"
#include "tests/Tester.h"

#define TEST_REPARSER(Function) TestFunction { &ReparserTester::Function, #Function }

namespace psy {
namespace C {

class ReparserTester final : public Tester
{
public:
    ReparserTester(TestSuite* suite)
        : Tester(suite)
    {}

    static const std::string Name;
    virtual std::string name() const override { return Name; }

    void testReparser();

    void reparse_withSyntaxCorrelation(std::string text, Expectation X = Expectation());
    void reparse_withTypeSynonymVerification(std::string text, Expectation X = Expectation());
    void reparse_withGuidelineImposition(std::string text, Expectation X = Expectation());

    using TestFunction = std::pair<std::function<void(ReparserTester*)>, const char*>;

    /*
        Syntax Correlation
            + 0000-0019 -> multiplication x pointer declaration
            + 0020-0039 -> call x variable declaration
            + 0040-0059 -> cast x binary expression
            + 0060-0069 -> type name x expression (as type reference)

     */

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

    void case0100();
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

    void case0200();
    void case0201();
    void case0202();
    void case0203();
    void case0204();
    void case0205();
    void case0206();
    void case0207();
    void case0208();
    void case0209();
    void case0210();
    void case0211();
    void case0212();
    void case0213();
    void case0214();
    void case0215();
    void case0216();
    void case0217();
    void case0218();
    void case0219();
    void case0220();
    void case0221();
    void case0222();
    void case0223();
    void case0224();
    void case0225();
    void case0226();
    void case0227();
    void case0228();
    void case0229();
    void case0230();
    void case0231();
    void case0232();
    void case0233();
    void case0234();
    void case0235();
    void case0236();
    void case0237();
    void case0238();
    void case0239();
    void case0240();
    void case0241();
    void case0242();
    void case0243();
    void case0244();
    void case0245();
    void case0246();
    void case0247();
    void case0248();
    void case0249();
    void case0250();
    void case0251();
    void case0252();
    void case0253();
    void case0254();
    void case0255();
    void case0256();
    void case0257();
    void case0258();
    void case0259();
    void case0260();
    void case0261();
    void case0262();
    void case0263();
    void case0264();
    void case0265();
    void case0266();
    void case0267();
    void case0268();
    void case0269();
    void case0270();
    void case0271();
    void case0272();
    void case0273();
    void case0274();
    void case0275();
    void case0276();
    void case0277();
    void case0278();
    void case0279();
    void case0280();
    void case0281();
    void case0282();
    void case0283();
    void case0284();
    void case0285();
    void case0286();
    void case0287();
    void case0288();
    void case0289();
    void case0290();
    void case0291();
    void case0292();
    void case0293();
    void case0294();
    void case0295();
    void case0296();
    void case0297();
    void case0298();
    void case0299();

    std::vector<TestFunction> tests_
    {
        TEST_REPARSER(case0001),
        TEST_REPARSER(case0002),
        TEST_REPARSER(case0003),
        TEST_REPARSER(case0004),
        TEST_REPARSER(case0005),
        TEST_REPARSER(case0006),
        TEST_REPARSER(case0007),
        TEST_REPARSER(case0008),
        TEST_REPARSER(case0009),
        TEST_REPARSER(case0010),
        TEST_REPARSER(case0011),
        TEST_REPARSER(case0012),
        TEST_REPARSER(case0013),
        TEST_REPARSER(case0014),
        TEST_REPARSER(case0015),
        TEST_REPARSER(case0016),
        TEST_REPARSER(case0017),
        TEST_REPARSER(case0018),
        TEST_REPARSER(case0019),
        TEST_REPARSER(case0020),
        TEST_REPARSER(case0021),
        TEST_REPARSER(case0022),
        TEST_REPARSER(case0023),
        TEST_REPARSER(case0024),
        TEST_REPARSER(case0025),
        TEST_REPARSER(case0026),
        TEST_REPARSER(case0027),
        TEST_REPARSER(case0028),
        TEST_REPARSER(case0029),
        TEST_REPARSER(case0030),
        TEST_REPARSER(case0031),
        TEST_REPARSER(case0032),
        TEST_REPARSER(case0033),
        TEST_REPARSER(case0034),
        TEST_REPARSER(case0035),
        TEST_REPARSER(case0036),
        TEST_REPARSER(case0037),
        TEST_REPARSER(case0038),
        TEST_REPARSER(case0039),
        TEST_REPARSER(case0040),
        TEST_REPARSER(case0041),
        TEST_REPARSER(case0042),
        TEST_REPARSER(case0043),
        TEST_REPARSER(case0044),
        TEST_REPARSER(case0045),
        TEST_REPARSER(case0046),
        TEST_REPARSER(case0047),
        TEST_REPARSER(case0048),
        TEST_REPARSER(case0049),
        TEST_REPARSER(case0050),
        TEST_REPARSER(case0051),
        TEST_REPARSER(case0052),
        TEST_REPARSER(case0053),
        TEST_REPARSER(case0054),
        TEST_REPARSER(case0055),
        TEST_REPARSER(case0056),
        TEST_REPARSER(case0057),
        TEST_REPARSER(case0058),
        TEST_REPARSER(case0059),
        TEST_REPARSER(case0060),
        TEST_REPARSER(case0061),
        TEST_REPARSER(case0062),
        TEST_REPARSER(case0063),
        TEST_REPARSER(case0064),
        TEST_REPARSER(case0065),
        TEST_REPARSER(case0066),
        TEST_REPARSER(case0067),
        TEST_REPARSER(case0068),
        TEST_REPARSER(case0069),
        TEST_REPARSER(case0070),
        TEST_REPARSER(case0071),
        TEST_REPARSER(case0072),
        TEST_REPARSER(case0073),
        TEST_REPARSER(case0074),
        TEST_REPARSER(case0075),
        TEST_REPARSER(case0076),
        TEST_REPARSER(case0077),
        TEST_REPARSER(case0078),
        TEST_REPARSER(case0079),
        TEST_REPARSER(case0080),
        TEST_REPARSER(case0081),
        TEST_REPARSER(case0082),
        TEST_REPARSER(case0083),
        TEST_REPARSER(case0084),
        TEST_REPARSER(case0085),
        TEST_REPARSER(case0086),
        TEST_REPARSER(case0087),
        TEST_REPARSER(case0088),
        TEST_REPARSER(case0089),
        TEST_REPARSER(case0090),
        TEST_REPARSER(case0091),
        TEST_REPARSER(case0092),
        TEST_REPARSER(case0093),
        TEST_REPARSER(case0094),
        TEST_REPARSER(case0095),
        TEST_REPARSER(case0096),
        TEST_REPARSER(case0097),
        TEST_REPARSER(case0098),
        TEST_REPARSER(case0099),

        TEST_REPARSER(case0100),
        TEST_REPARSER(case0101),
        TEST_REPARSER(case0102),
        TEST_REPARSER(case0103),
        TEST_REPARSER(case0104),
        TEST_REPARSER(case0105),
        TEST_REPARSER(case0106),
        TEST_REPARSER(case0107),
        TEST_REPARSER(case0108),
        TEST_REPARSER(case0109),
        TEST_REPARSER(case0110),
        TEST_REPARSER(case0111),
        TEST_REPARSER(case0112),
        TEST_REPARSER(case0113),
        TEST_REPARSER(case0114),
        TEST_REPARSER(case0115),
        TEST_REPARSER(case0116),
        TEST_REPARSER(case0117),
        TEST_REPARSER(case0118),
        TEST_REPARSER(case0119),
        TEST_REPARSER(case0120),
        TEST_REPARSER(case0121),
        TEST_REPARSER(case0122),
        TEST_REPARSER(case0123),
        TEST_REPARSER(case0124),
        TEST_REPARSER(case0125),
        TEST_REPARSER(case0126),
        TEST_REPARSER(case0127),
        TEST_REPARSER(case0128),
        TEST_REPARSER(case0129),
        TEST_REPARSER(case0130),
        TEST_REPARSER(case0131),
        TEST_REPARSER(case0132),
        TEST_REPARSER(case0133),
        TEST_REPARSER(case0134),
        TEST_REPARSER(case0135),
        TEST_REPARSER(case0136),
        TEST_REPARSER(case0137),
        TEST_REPARSER(case0138),
        TEST_REPARSER(case0139),
        TEST_REPARSER(case0140),
        TEST_REPARSER(case0141),
        TEST_REPARSER(case0142),
        TEST_REPARSER(case0143),
        TEST_REPARSER(case0144),
        TEST_REPARSER(case0145),
        TEST_REPARSER(case0146),
        TEST_REPARSER(case0147),
        TEST_REPARSER(case0148),
        TEST_REPARSER(case0149),
        TEST_REPARSER(case0150),
        TEST_REPARSER(case0151),
        TEST_REPARSER(case0152),
        TEST_REPARSER(case0153),
        TEST_REPARSER(case0154),
        TEST_REPARSER(case0155),
        TEST_REPARSER(case0156),
        TEST_REPARSER(case0157),
        TEST_REPARSER(case0158),
        TEST_REPARSER(case0159),
        TEST_REPARSER(case0160),
        TEST_REPARSER(case0161),
        TEST_REPARSER(case0162),
        TEST_REPARSER(case0163),
        TEST_REPARSER(case0164),
        TEST_REPARSER(case0165),
        TEST_REPARSER(case0166),
        TEST_REPARSER(case0167),
        TEST_REPARSER(case0168),
        TEST_REPARSER(case0169),
        TEST_REPARSER(case0170),
        TEST_REPARSER(case0171),
        TEST_REPARSER(case0172),
        TEST_REPARSER(case0173),
        TEST_REPARSER(case0174),
        TEST_REPARSER(case0175),
        TEST_REPARSER(case0176),
        TEST_REPARSER(case0177),
        TEST_REPARSER(case0178),
        TEST_REPARSER(case0179),
        TEST_REPARSER(case0180),
        TEST_REPARSER(case0181),
        TEST_REPARSER(case0182),
        TEST_REPARSER(case0183),
        TEST_REPARSER(case0184),
        TEST_REPARSER(case0185),
        TEST_REPARSER(case0186),
        TEST_REPARSER(case0187),
        TEST_REPARSER(case0188),
        TEST_REPARSER(case0189),
        TEST_REPARSER(case0190),
        TEST_REPARSER(case0191),
        TEST_REPARSER(case0192),
        TEST_REPARSER(case0193),
        TEST_REPARSER(case0194),
        TEST_REPARSER(case0195),
        TEST_REPARSER(case0196),
        TEST_REPARSER(case0197),
        TEST_REPARSER(case0198),
        TEST_REPARSER(case0199),

        TEST_REPARSER(case0200),
        TEST_REPARSER(case0201),
        TEST_REPARSER(case0202),
        TEST_REPARSER(case0203),
        TEST_REPARSER(case0204),
        TEST_REPARSER(case0205),
        TEST_REPARSER(case0206),
        TEST_REPARSER(case0207),
        TEST_REPARSER(case0208),
        TEST_REPARSER(case0209),
        TEST_REPARSER(case0210),
        TEST_REPARSER(case0211),
        TEST_REPARSER(case0212),
        TEST_REPARSER(case0213),
        TEST_REPARSER(case0214),
        TEST_REPARSER(case0215),
        TEST_REPARSER(case0216),
        TEST_REPARSER(case0217),
        TEST_REPARSER(case0218),
        TEST_REPARSER(case0219),
        TEST_REPARSER(case0220),
        TEST_REPARSER(case0221),
        TEST_REPARSER(case0222),
        TEST_REPARSER(case0223),
        TEST_REPARSER(case0224),
        TEST_REPARSER(case0225),
        TEST_REPARSER(case0226),
        TEST_REPARSER(case0227),
        TEST_REPARSER(case0228),
        TEST_REPARSER(case0229),
        TEST_REPARSER(case0230),
        TEST_REPARSER(case0231),
        TEST_REPARSER(case0232),
        TEST_REPARSER(case0233),
        TEST_REPARSER(case0234),
        TEST_REPARSER(case0235),
        TEST_REPARSER(case0236),
        TEST_REPARSER(case0237),
        TEST_REPARSER(case0238),
        TEST_REPARSER(case0239),
        TEST_REPARSER(case0240),
        TEST_REPARSER(case0241),
        TEST_REPARSER(case0242),
        TEST_REPARSER(case0243),
        TEST_REPARSER(case0244),
        TEST_REPARSER(case0245),
        TEST_REPARSER(case0246),
        TEST_REPARSER(case0247),
        TEST_REPARSER(case0248),
        TEST_REPARSER(case0249),
        TEST_REPARSER(case0250),
        TEST_REPARSER(case0251),
        TEST_REPARSER(case0252),
        TEST_REPARSER(case0253),
        TEST_REPARSER(case0254),
        TEST_REPARSER(case0255),
        TEST_REPARSER(case0256),
        TEST_REPARSER(case0257),
        TEST_REPARSER(case0258),
        TEST_REPARSER(case0259),
        TEST_REPARSER(case0260),
        TEST_REPARSER(case0261),
        TEST_REPARSER(case0262),
        TEST_REPARSER(case0263),
        TEST_REPARSER(case0264),
        TEST_REPARSER(case0265),
        TEST_REPARSER(case0266),
        TEST_REPARSER(case0267),
        TEST_REPARSER(case0268),
        TEST_REPARSER(case0269),
        TEST_REPARSER(case0270),
        TEST_REPARSER(case0271),
        TEST_REPARSER(case0272),
        TEST_REPARSER(case0273),
        TEST_REPARSER(case0274),
        TEST_REPARSER(case0275),
        TEST_REPARSER(case0276),
        TEST_REPARSER(case0277),
        TEST_REPARSER(case0278),
        TEST_REPARSER(case0279),
        TEST_REPARSER(case0280),
        TEST_REPARSER(case0281),
        TEST_REPARSER(case0282),
        TEST_REPARSER(case0283),
        TEST_REPARSER(case0284),
        TEST_REPARSER(case0285),
        TEST_REPARSER(case0286),
        TEST_REPARSER(case0287),
        TEST_REPARSER(case0288),
        TEST_REPARSER(case0289),
        TEST_REPARSER(case0290),
        TEST_REPARSER(case0291),
        TEST_REPARSER(case0292),
        TEST_REPARSER(case0293),
        TEST_REPARSER(case0294),
        TEST_REPARSER(case0295),
        TEST_REPARSER(case0296),
        TEST_REPARSER(case0297),
        TEST_REPARSER(case0298),
        TEST_REPARSER(case0299),

    };
};

} // C
} // psy

#endif
