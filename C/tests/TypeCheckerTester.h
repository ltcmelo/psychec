// Copyright (c) 2024 Leandro T. C. Melo <ltcmelo@gmail.com>
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

#ifndef PSYCHE_C_DECLARATION_CHECKER_TESTER_H__
#define PSYCHE_C_DECLARATION_CHECKER_TESTER_H__

#include "Fwds.h"
#include "TestSuite_Internals.h"
#include "tests/Tester.h"

#define TEST_TYPE_CHECKER(Function) TestFunction { &TypeCheckerTester::Function, #Function }

namespace psy {
namespace C {

class TypeCheckerTester final : public Tester
{
public:
    TypeCheckerTester(TestSuite* suite)
        : Tester(suite)
    {}

    static const std::string Name;
    virtual std::string name() const override { return Name; }

    void testTypeChecker();

    void checkTypes(std::string text, Expectation X);

    using TestFunction = std::pair<std::function<void(TypeCheckerTester*)>, const char*>;

    /*
        + 0000-0099 -> member access
        + 0100-0129 -> postfix inc/dec
        + 0130-0199 -> array subscript
        + 0200-0299 -> compound assignment and binary
        + 0300-0399 -> function call
        + 0500-0599 -> unary (prefix)
        + 0700-0799 -> simple assignment
        +
     */

    void case0000();
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

    void case0300();
    void case0301();
    void case0302();
    void case0303();
    void case0304();
    void case0305();
    void case0306();
    void case0307();
    void case0308();
    void case0309();
    void case0310();
    void case0311();
    void case0312();
    void case0313();
    void case0314();
    void case0315();
    void case0316();
    void case0317();
    void case0318();
    void case0319();
    void case0320();
    void case0321();
    void case0322();
    void case0323();
    void case0324();
    void case0325();
    void case0326();
    void case0327();
    void case0328();
    void case0329();
    void case0330();
    void case0331();
    void case0332();
    void case0333();
    void case0334();
    void case0335();
    void case0336();
    void case0337();
    void case0338();
    void case0339();
    void case0340();
    void case0341();
    void case0342();
    void case0343();
    void case0344();
    void case0345();
    void case0346();
    void case0347();
    void case0348();
    void case0349();
    void case0350();
    void case0351();
    void case0352();
    void case0353();
    void case0354();
    void case0355();
    void case0356();
    void case0357();
    void case0358();
    void case0359();
    void case0360();
    void case0361();
    void case0362();
    void case0363();
    void case0364();
    void case0365();
    void case0366();
    void case0367();
    void case0368();
    void case0369();

    void case0500();
    void case0501();
    void case0502();
    void case0503();
    void case0504();
    void case0505();
    void case0506();
    void case0507();
    void case0508();
    void case0509();
    void case0510();
    void case0511();
    void case0512();
    void case0513();
    void case0514();
    void case0515();
    void case0516();
    void case0517();
    void case0518();
    void case0519();
    void case0520();
    void case0521();
    void case0522();
    void case0523();
    void case0524();
    void case0525();
    void case0526();
    void case0527();
    void case0528();
    void case0529();
    void case0530();
    void case0531();
    void case0532();
    void case0533();
    void case0534();
    void case0535();
    void case0536();
    void case0537();
    void case0538();
    void case0539();
    void case0540();
    void case0541();
    void case0542();
    void case0543();
    void case0544();
    void case0545();
    void case0546();
    void case0547();
    void case0548();
    void case0549();
    void case0550();
    void case0551();
    void case0552();
    void case0553();
    void case0554();
    void case0555();
    void case0556();
    void case0557();
    void case0558();
    void case0559();
    void case0560();
    void case0561();
    void case0562();
    void case0563();
    void case0564();
    void case0565();
    void case0566();
    void case0567();
    void case0568();
    void case0569();

    void case0700();
    void case0701();
    void case0702();
    void case0703();
    void case0704();
    void case0705();
    void case0706();
    void case0707();
    void case0708();
    void case0709();
    void case0710();
    void case0711();
    void case0712();
    void case0713();
    void case0714();
    void case0715();
    void case0716();
    void case0717();
    void case0718();
    void case0719();
    void case0720();
    void case0721();
    void case0722();
    void case0723();
    void case0724();
    void case0725();
    void case0726();
    void case0727();
    void case0728();
    void case0729();
    void case0730();
    void case0731();
    void case0732();
    void case0733();
    void case0734();
    void case0735();
    void case0736();
    void case0737();
    void case0738();
    void case0739();
    void case0740();
    void case0741();
    void case0742();
    void case0743();
    void case0744();
    void case0745();
    void case0746();
    void case0747();
    void case0748();
    void case0749();
    void case0750();
    void case0751();
    void case0752();
    void case0753();
    void case0754();
    void case0755();
    void case0756();
    void case0757();
    void case0758();
    void case0759();

    std::vector<TestFunction> tests_
    {
        TEST_TYPE_CHECKER(case0000),
        TEST_TYPE_CHECKER(case0001),
        TEST_TYPE_CHECKER(case0002),
        TEST_TYPE_CHECKER(case0003),
        TEST_TYPE_CHECKER(case0004),
        TEST_TYPE_CHECKER(case0005),
        TEST_TYPE_CHECKER(case0006),
        TEST_TYPE_CHECKER(case0007),
        TEST_TYPE_CHECKER(case0008),
        TEST_TYPE_CHECKER(case0009),
        TEST_TYPE_CHECKER(case0010),
        TEST_TYPE_CHECKER(case0011),
        TEST_TYPE_CHECKER(case0012),
        TEST_TYPE_CHECKER(case0013),
        TEST_TYPE_CHECKER(case0014),
        TEST_TYPE_CHECKER(case0015),
        TEST_TYPE_CHECKER(case0016),
        TEST_TYPE_CHECKER(case0017),
        TEST_TYPE_CHECKER(case0018),
        TEST_TYPE_CHECKER(case0019),
        TEST_TYPE_CHECKER(case0020),
        TEST_TYPE_CHECKER(case0021),
        TEST_TYPE_CHECKER(case0022),
        TEST_TYPE_CHECKER(case0023),
        TEST_TYPE_CHECKER(case0024),
        TEST_TYPE_CHECKER(case0025),
        TEST_TYPE_CHECKER(case0026),
        TEST_TYPE_CHECKER(case0027),
        TEST_TYPE_CHECKER(case0028),
        TEST_TYPE_CHECKER(case0029),
        TEST_TYPE_CHECKER(case0030),
        TEST_TYPE_CHECKER(case0031),
        TEST_TYPE_CHECKER(case0032),
        TEST_TYPE_CHECKER(case0033),
        TEST_TYPE_CHECKER(case0034),
        TEST_TYPE_CHECKER(case0035),
        TEST_TYPE_CHECKER(case0036),
        TEST_TYPE_CHECKER(case0037),
        TEST_TYPE_CHECKER(case0038),
        TEST_TYPE_CHECKER(case0039),
        TEST_TYPE_CHECKER(case0040),
        TEST_TYPE_CHECKER(case0041),
        TEST_TYPE_CHECKER(case0042),
        TEST_TYPE_CHECKER(case0043),
        TEST_TYPE_CHECKER(case0044),
        TEST_TYPE_CHECKER(case0045),
        TEST_TYPE_CHECKER(case0046),
        TEST_TYPE_CHECKER(case0047),
        TEST_TYPE_CHECKER(case0048),
        TEST_TYPE_CHECKER(case0049),
        TEST_TYPE_CHECKER(case0050),
        TEST_TYPE_CHECKER(case0051),
        TEST_TYPE_CHECKER(case0052),
        TEST_TYPE_CHECKER(case0053),
        TEST_TYPE_CHECKER(case0054),
        TEST_TYPE_CHECKER(case0055),
        TEST_TYPE_CHECKER(case0056),
        TEST_TYPE_CHECKER(case0057),
        TEST_TYPE_CHECKER(case0058),
        TEST_TYPE_CHECKER(case0059),

        TEST_TYPE_CHECKER(case0100),
        TEST_TYPE_CHECKER(case0101),
        TEST_TYPE_CHECKER(case0102),
        TEST_TYPE_CHECKER(case0103),
        TEST_TYPE_CHECKER(case0104),
        TEST_TYPE_CHECKER(case0105),
        TEST_TYPE_CHECKER(case0106),
        TEST_TYPE_CHECKER(case0107),
        TEST_TYPE_CHECKER(case0108),
        TEST_TYPE_CHECKER(case0109),
        TEST_TYPE_CHECKER(case0130),
        TEST_TYPE_CHECKER(case0131),
        TEST_TYPE_CHECKER(case0132),
        TEST_TYPE_CHECKER(case0133),
        TEST_TYPE_CHECKER(case0134),
        TEST_TYPE_CHECKER(case0135),
        TEST_TYPE_CHECKER(case0136),
        TEST_TYPE_CHECKER(case0137),
        TEST_TYPE_CHECKER(case0138),
        TEST_TYPE_CHECKER(case0139),
        TEST_TYPE_CHECKER(case0140),
        TEST_TYPE_CHECKER(case0141),
        TEST_TYPE_CHECKER(case0142),
        TEST_TYPE_CHECKER(case0143),
        TEST_TYPE_CHECKER(case0144),
        TEST_TYPE_CHECKER(case0145),
        TEST_TYPE_CHECKER(case0146),
        TEST_TYPE_CHECKER(case0147),
        TEST_TYPE_CHECKER(case0148),
        TEST_TYPE_CHECKER(case0149),
        TEST_TYPE_CHECKER(case0150),
        TEST_TYPE_CHECKER(case0151),
        TEST_TYPE_CHECKER(case0152),
        TEST_TYPE_CHECKER(case0153),
        TEST_TYPE_CHECKER(case0154),
        TEST_TYPE_CHECKER(case0155),
        TEST_TYPE_CHECKER(case0156),
        TEST_TYPE_CHECKER(case0157),
        TEST_TYPE_CHECKER(case0158),
        TEST_TYPE_CHECKER(case0159),

        TEST_TYPE_CHECKER(case0200),
        TEST_TYPE_CHECKER(case0201),
        TEST_TYPE_CHECKER(case0202),
        TEST_TYPE_CHECKER(case0203),
        TEST_TYPE_CHECKER(case0204),
        TEST_TYPE_CHECKER(case0205),
        TEST_TYPE_CHECKER(case0206),
        TEST_TYPE_CHECKER(case0207),
        TEST_TYPE_CHECKER(case0208),
        TEST_TYPE_CHECKER(case0209),
        TEST_TYPE_CHECKER(case0210),
        TEST_TYPE_CHECKER(case0211),
        TEST_TYPE_CHECKER(case0212),
        TEST_TYPE_CHECKER(case0213),
        TEST_TYPE_CHECKER(case0214),
        TEST_TYPE_CHECKER(case0215),
        TEST_TYPE_CHECKER(case0216),
        TEST_TYPE_CHECKER(case0217),
        TEST_TYPE_CHECKER(case0218),
        TEST_TYPE_CHECKER(case0219),
        TEST_TYPE_CHECKER(case0220),
        TEST_TYPE_CHECKER(case0221),
        TEST_TYPE_CHECKER(case0222),
        TEST_TYPE_CHECKER(case0223),
        TEST_TYPE_CHECKER(case0224),
        TEST_TYPE_CHECKER(case0225),
        TEST_TYPE_CHECKER(case0226),
        TEST_TYPE_CHECKER(case0227),
        TEST_TYPE_CHECKER(case0228),
        TEST_TYPE_CHECKER(case0229),
        TEST_TYPE_CHECKER(case0230),
        TEST_TYPE_CHECKER(case0231),
        TEST_TYPE_CHECKER(case0232),
        TEST_TYPE_CHECKER(case0233),
        TEST_TYPE_CHECKER(case0234),
        TEST_TYPE_CHECKER(case0235),
        TEST_TYPE_CHECKER(case0236),
        TEST_TYPE_CHECKER(case0237),
        TEST_TYPE_CHECKER(case0238),
        TEST_TYPE_CHECKER(case0239),
        TEST_TYPE_CHECKER(case0240),
        TEST_TYPE_CHECKER(case0241),
        TEST_TYPE_CHECKER(case0242),
        TEST_TYPE_CHECKER(case0243),
        TEST_TYPE_CHECKER(case0244),
        TEST_TYPE_CHECKER(case0245),
        TEST_TYPE_CHECKER(case0246),
        TEST_TYPE_CHECKER(case0247),
        TEST_TYPE_CHECKER(case0248),
        TEST_TYPE_CHECKER(case0249),
        TEST_TYPE_CHECKER(case0250),
        TEST_TYPE_CHECKER(case0251),
        TEST_TYPE_CHECKER(case0252),
        TEST_TYPE_CHECKER(case0253),
        TEST_TYPE_CHECKER(case0254),
        TEST_TYPE_CHECKER(case0255),
        TEST_TYPE_CHECKER(case0256),
        TEST_TYPE_CHECKER(case0257),
        TEST_TYPE_CHECKER(case0258),
        TEST_TYPE_CHECKER(case0259),

        TEST_TYPE_CHECKER(case0300),
        TEST_TYPE_CHECKER(case0301),
        TEST_TYPE_CHECKER(case0302),
        TEST_TYPE_CHECKER(case0303),
        TEST_TYPE_CHECKER(case0304),
        TEST_TYPE_CHECKER(case0305),
        TEST_TYPE_CHECKER(case0306),
        TEST_TYPE_CHECKER(case0307),
        TEST_TYPE_CHECKER(case0308),
        TEST_TYPE_CHECKER(case0309),
        TEST_TYPE_CHECKER(case0310),
        TEST_TYPE_CHECKER(case0311),
        TEST_TYPE_CHECKER(case0312),
        TEST_TYPE_CHECKER(case0313),
        TEST_TYPE_CHECKER(case0314),
        TEST_TYPE_CHECKER(case0315),
        TEST_TYPE_CHECKER(case0316),
        TEST_TYPE_CHECKER(case0317),
        TEST_TYPE_CHECKER(case0318),
        TEST_TYPE_CHECKER(case0319),
        TEST_TYPE_CHECKER(case0320),
        TEST_TYPE_CHECKER(case0321),
        TEST_TYPE_CHECKER(case0322),
        TEST_TYPE_CHECKER(case0323),
        TEST_TYPE_CHECKER(case0324),
        TEST_TYPE_CHECKER(case0325),
        TEST_TYPE_CHECKER(case0326),
        TEST_TYPE_CHECKER(case0327),
        TEST_TYPE_CHECKER(case0328),
        TEST_TYPE_CHECKER(case0329),
        TEST_TYPE_CHECKER(case0330),
        TEST_TYPE_CHECKER(case0331),
        TEST_TYPE_CHECKER(case0332),
        TEST_TYPE_CHECKER(case0333),
        TEST_TYPE_CHECKER(case0334),
        TEST_TYPE_CHECKER(case0335),
        TEST_TYPE_CHECKER(case0336),
        TEST_TYPE_CHECKER(case0337),
        TEST_TYPE_CHECKER(case0338),
        TEST_TYPE_CHECKER(case0339),
        TEST_TYPE_CHECKER(case0340),
        TEST_TYPE_CHECKER(case0341),
        TEST_TYPE_CHECKER(case0342),
        TEST_TYPE_CHECKER(case0343),
        TEST_TYPE_CHECKER(case0344),
        TEST_TYPE_CHECKER(case0345),
        TEST_TYPE_CHECKER(case0346),
        TEST_TYPE_CHECKER(case0347),
        TEST_TYPE_CHECKER(case0348),
        TEST_TYPE_CHECKER(case0349),
        TEST_TYPE_CHECKER(case0350),
        TEST_TYPE_CHECKER(case0351),
        TEST_TYPE_CHECKER(case0352),
        TEST_TYPE_CHECKER(case0353),
        TEST_TYPE_CHECKER(case0354),
        TEST_TYPE_CHECKER(case0355),
        TEST_TYPE_CHECKER(case0356),
        TEST_TYPE_CHECKER(case0357),
        TEST_TYPE_CHECKER(case0358),
        TEST_TYPE_CHECKER(case0359),
        TEST_TYPE_CHECKER(case0360),
        TEST_TYPE_CHECKER(case0361),
        TEST_TYPE_CHECKER(case0362),
        TEST_TYPE_CHECKER(case0363),
        TEST_TYPE_CHECKER(case0364),
        TEST_TYPE_CHECKER(case0365),
        TEST_TYPE_CHECKER(case0366),
        TEST_TYPE_CHECKER(case0367),
        TEST_TYPE_CHECKER(case0368),
        TEST_TYPE_CHECKER(case0369),

        TEST_TYPE_CHECKER(case0500),
        TEST_TYPE_CHECKER(case0501),
        TEST_TYPE_CHECKER(case0502),
        TEST_TYPE_CHECKER(case0503),
        TEST_TYPE_CHECKER(case0504),
        TEST_TYPE_CHECKER(case0505),
        TEST_TYPE_CHECKER(case0506),
        TEST_TYPE_CHECKER(case0507),
        TEST_TYPE_CHECKER(case0508),
        TEST_TYPE_CHECKER(case0509),
        TEST_TYPE_CHECKER(case0510),
        TEST_TYPE_CHECKER(case0511),
        TEST_TYPE_CHECKER(case0512),
        TEST_TYPE_CHECKER(case0513),
        TEST_TYPE_CHECKER(case0514),
        TEST_TYPE_CHECKER(case0515),
        TEST_TYPE_CHECKER(case0516),
        TEST_TYPE_CHECKER(case0517),
        TEST_TYPE_CHECKER(case0518),
        TEST_TYPE_CHECKER(case0519),
        TEST_TYPE_CHECKER(case0520),
        TEST_TYPE_CHECKER(case0521),
        TEST_TYPE_CHECKER(case0522),
        TEST_TYPE_CHECKER(case0523),
        TEST_TYPE_CHECKER(case0524),
        TEST_TYPE_CHECKER(case0525),
        TEST_TYPE_CHECKER(case0526),
        TEST_TYPE_CHECKER(case0527),
        TEST_TYPE_CHECKER(case0528),
        TEST_TYPE_CHECKER(case0529),
        TEST_TYPE_CHECKER(case0530),
        TEST_TYPE_CHECKER(case0531),
        TEST_TYPE_CHECKER(case0532),
        TEST_TYPE_CHECKER(case0533),
        TEST_TYPE_CHECKER(case0534),
        TEST_TYPE_CHECKER(case0535),
        TEST_TYPE_CHECKER(case0536),
        TEST_TYPE_CHECKER(case0537),
        TEST_TYPE_CHECKER(case0538),
        TEST_TYPE_CHECKER(case0539),
        TEST_TYPE_CHECKER(case0540),
        TEST_TYPE_CHECKER(case0541),
        TEST_TYPE_CHECKER(case0542),
        TEST_TYPE_CHECKER(case0543),
        TEST_TYPE_CHECKER(case0544),
        TEST_TYPE_CHECKER(case0545),
        TEST_TYPE_CHECKER(case0546),
        TEST_TYPE_CHECKER(case0547),
        TEST_TYPE_CHECKER(case0548),
        TEST_TYPE_CHECKER(case0549),
        TEST_TYPE_CHECKER(case0550),
        TEST_TYPE_CHECKER(case0551),
        TEST_TYPE_CHECKER(case0552),
        TEST_TYPE_CHECKER(case0553),
        TEST_TYPE_CHECKER(case0554),
        TEST_TYPE_CHECKER(case0555),
        TEST_TYPE_CHECKER(case0556),
        TEST_TYPE_CHECKER(case0557),
        TEST_TYPE_CHECKER(case0558),
        TEST_TYPE_CHECKER(case0559),
        TEST_TYPE_CHECKER(case0560),
        TEST_TYPE_CHECKER(case0561),
        TEST_TYPE_CHECKER(case0562),
        TEST_TYPE_CHECKER(case0563),
        TEST_TYPE_CHECKER(case0564),
        TEST_TYPE_CHECKER(case0565),
        TEST_TYPE_CHECKER(case0566),
        TEST_TYPE_CHECKER(case0567),
        TEST_TYPE_CHECKER(case0568),
        TEST_TYPE_CHECKER(case0569),

        TEST_TYPE_CHECKER(case0700),
        TEST_TYPE_CHECKER(case0701),
        TEST_TYPE_CHECKER(case0702),
        TEST_TYPE_CHECKER(case0703),
        TEST_TYPE_CHECKER(case0704),
        TEST_TYPE_CHECKER(case0705),
        TEST_TYPE_CHECKER(case0706),
        TEST_TYPE_CHECKER(case0707),
        TEST_TYPE_CHECKER(case0708),
        TEST_TYPE_CHECKER(case0709),
        TEST_TYPE_CHECKER(case0710),
        TEST_TYPE_CHECKER(case0711),
        TEST_TYPE_CHECKER(case0712),
        TEST_TYPE_CHECKER(case0713),
        TEST_TYPE_CHECKER(case0714),
        TEST_TYPE_CHECKER(case0715),
        TEST_TYPE_CHECKER(case0716),
        TEST_TYPE_CHECKER(case0717),
        TEST_TYPE_CHECKER(case0718),
        TEST_TYPE_CHECKER(case0719),
        TEST_TYPE_CHECKER(case0720),
        TEST_TYPE_CHECKER(case0721),
        TEST_TYPE_CHECKER(case0722),
        TEST_TYPE_CHECKER(case0723),
        TEST_TYPE_CHECKER(case0724),
        TEST_TYPE_CHECKER(case0725),
        TEST_TYPE_CHECKER(case0726),
        TEST_TYPE_CHECKER(case0727),
        TEST_TYPE_CHECKER(case0728),
        TEST_TYPE_CHECKER(case0729),
        TEST_TYPE_CHECKER(case0730),
        TEST_TYPE_CHECKER(case0731),
        TEST_TYPE_CHECKER(case0732),
        TEST_TYPE_CHECKER(case0733),
        TEST_TYPE_CHECKER(case0734),
        TEST_TYPE_CHECKER(case0735),
        TEST_TYPE_CHECKER(case0736),
        TEST_TYPE_CHECKER(case0737),
        TEST_TYPE_CHECKER(case0738),
        TEST_TYPE_CHECKER(case0739),
        TEST_TYPE_CHECKER(case0740),
        TEST_TYPE_CHECKER(case0741),
        TEST_TYPE_CHECKER(case0742),
        TEST_TYPE_CHECKER(case0743),
        TEST_TYPE_CHECKER(case0744),
        TEST_TYPE_CHECKER(case0745),
        TEST_TYPE_CHECKER(case0746),
        TEST_TYPE_CHECKER(case0747),
        TEST_TYPE_CHECKER(case0748),
        TEST_TYPE_CHECKER(case0749),
        TEST_TYPE_CHECKER(case0750),
        TEST_TYPE_CHECKER(case0751),
        TEST_TYPE_CHECKER(case0752),
        TEST_TYPE_CHECKER(case0753),
        TEST_TYPE_CHECKER(case0754),
        TEST_TYPE_CHECKER(case0755),
        TEST_TYPE_CHECKER(case0756),
        TEST_TYPE_CHECKER(case0757),
        TEST_TYPE_CHECKER(case0758),
        TEST_TYPE_CHECKER(case0759),


    };
};

} // C
} // psy

#endif
