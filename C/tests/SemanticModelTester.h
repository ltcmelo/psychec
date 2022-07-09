// Copyright (c) 2022 Leandro T. C. Melo <ltcmelo@gmail.com>
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

#ifndef PSYCHE_C_SEMANTIC_MODEL_TESTER_H__
#define PSYCHE_C_SEMANTIC_MODEL_TESTER_H__

#include "Fwds.h"
#include "TestSuite_Internals.h"
#include "tests/Tester.h"

#include "C/SyntaxTree.h"
#include "C/syntax/SyntaxNodes.h"
#include "C/compilation/SemanticModel.h"

#define TEST_SEMANTIC_MODEL(Function) TestFunction { &SemanticModelTester::Function, #Function }

namespace psy {
namespace C {

class APITestSuite;

class SemanticModelTester final : public Tester
{
public:
    SemanticModelTester(TestSuite* suite) : Tester(suite) {}

    APITestSuite* suite();

    static const std::string Name;
    virtual std::string name() const override { return Name; }
    virtual void setUp() override;
    virtual void tearDown() override;

    std::unique_ptr<SyntaxTree> tree_;
    std::unique_ptr<Compilation> compilation_;

    template <class DeclT>
    std::tuple<const DeclT*,const SemanticModel*> declAndSemaModel(const std::string& s);

    void testSemanticModel();

    using TestFunction = std::pair<std::function<void(SemanticModelTester*)>, const char*>;

    /*
        + 0000-0089 -> variable
        + 0090-0099 -> variable and function (declaration)
        + 0100-0149 -> function (declaration)
        + 0150-0199 -> function (definition)
        + 0200-0299 -> parameter
        + 0300-0349 -> struct/union
        + 0350-0399 -> field
        + 0400-0449 -> enum
        + 0450-0499 -> enumerator
        + 0500-0509 -> library
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
    //...
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
    //...
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
    void case0370();
    void case0371();
    void case0372();
    void case0373();
    void case0374();
    void case0375();
    void case0376();
    void case0377();
    void case0378();
    void case0379();
    void case0380();
    void case0381();
    void case0382();
    void case0383();
    void case0384();
    void case0385();
    void case0386();
    void case0387();
    void case0388();
    void case0389();
    void case0390();
    void case0391();
    void case0392();
    void case0393();
    void case0394();
    void case0395();
    void case0396();
    void case0397();
    void case0398();
    void case0399();

    void case0400();
    void case0401();
    void case0402();
    void case0403();
    void case0404();
    void case0405();
    void case0406();
    void case0407();
    void case0408();
    void case0409();
    void case0410();
    void case0411();
    void case0412();
    void case0413();
    void case0414();
    void case0415();
    void case0416();
    void case0417();
    void case0418();
    void case0419();
    void case0420();
    void case0421();
    void case0422();
    void case0423();
    void case0424();
    void case0425();
    void case0426();
    void case0427();
    void case0428();
    void case0429();
    void case0430();
    void case0431();
    void case0432();
    void case0433();
    void case0434();
    void case0435();
    void case0436();
    void case0437();
    void case0438();
    void case0439();
    void case0440();
    void case0441();
    void case0442();
    void case0443();
    void case0444();
    void case0445();
    void case0446();
    void case0447();
    void case0448();
    void case0449();
    void case0450();
    void case0451();
    void case0452();
    void case0453();
    void case0454();
    void case0455();
    void case0456();
    void case0457();
    void case0458();
    void case0459();
    void case0460();
    void case0461();
    void case0462();
    void case0463();
    void case0464();
    void case0465();
    void case0466();
    void case0467();
    void case0468();
    void case0469();
    void case0470();
    void case0471();
    void case0472();
    void case0473();
    void case0474();
    void case0475();
    void case0476();
    void case0477();
    void case0478();
    void case0479();
    void case0480();
    void case0481();
    void case0482();
    void case0483();
    void case0484();
    void case0485();
    void case0486();
    void case0487();
    void case0488();
    void case0489();
    void case0490();
    void case0491();
    void case0492();
    void case0493();
    void case0494();
    void case0495();
    void case0496();
    void case0497();
    void case0498();
    void case0499();

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

    std::vector<TestFunction> tests_
    {
        TEST_SEMANTIC_MODEL(case0001),
        TEST_SEMANTIC_MODEL(case0002),
        TEST_SEMANTIC_MODEL(case0003),
        TEST_SEMANTIC_MODEL(case0004),
        TEST_SEMANTIC_MODEL(case0005),
        TEST_SEMANTIC_MODEL(case0006),
        TEST_SEMANTIC_MODEL(case0007),
        TEST_SEMANTIC_MODEL(case0008),
        TEST_SEMANTIC_MODEL(case0009),
        TEST_SEMANTIC_MODEL(case0010),
        //...
        TEST_SEMANTIC_MODEL(case0090),
        TEST_SEMANTIC_MODEL(case0091),
        TEST_SEMANTIC_MODEL(case0092),
        TEST_SEMANTIC_MODEL(case0093),
        TEST_SEMANTIC_MODEL(case0094),
        TEST_SEMANTIC_MODEL(case0095),
        TEST_SEMANTIC_MODEL(case0096),
        TEST_SEMANTIC_MODEL(case0097),
        TEST_SEMANTIC_MODEL(case0098),
        TEST_SEMANTIC_MODEL(case0099),

        TEST_SEMANTIC_MODEL(case0101),
        TEST_SEMANTIC_MODEL(case0102),
        TEST_SEMANTIC_MODEL(case0103),
        TEST_SEMANTIC_MODEL(case0104),
        TEST_SEMANTIC_MODEL(case0105),
        TEST_SEMANTIC_MODEL(case0106),
        TEST_SEMANTIC_MODEL(case0107),
        TEST_SEMANTIC_MODEL(case0108),
        TEST_SEMANTIC_MODEL(case0109),
        TEST_SEMANTIC_MODEL(case0110),
        //...
        TEST_SEMANTIC_MODEL(case0150),
        TEST_SEMANTIC_MODEL(case0151),
        TEST_SEMANTIC_MODEL(case0152),
        TEST_SEMANTIC_MODEL(case0153),
        TEST_SEMANTIC_MODEL(case0154),
        TEST_SEMANTIC_MODEL(case0155),
        TEST_SEMANTIC_MODEL(case0156),
        TEST_SEMANTIC_MODEL(case0157),
        TEST_SEMANTIC_MODEL(case0158),
        TEST_SEMANTIC_MODEL(case0159),

        TEST_SEMANTIC_MODEL(case0201),
        TEST_SEMANTIC_MODEL(case0202),
        TEST_SEMANTIC_MODEL(case0203),
        TEST_SEMANTIC_MODEL(case0204),
        TEST_SEMANTIC_MODEL(case0205),
        TEST_SEMANTIC_MODEL(case0206),
        TEST_SEMANTIC_MODEL(case0207),
        TEST_SEMANTIC_MODEL(case0208),
        TEST_SEMANTIC_MODEL(case0209),
        TEST_SEMANTIC_MODEL(case0210),
        TEST_SEMANTIC_MODEL(case0211),
        TEST_SEMANTIC_MODEL(case0212),
        TEST_SEMANTIC_MODEL(case0213),
        TEST_SEMANTIC_MODEL(case0214),
        TEST_SEMANTIC_MODEL(case0215),
        TEST_SEMANTIC_MODEL(case0216),
        TEST_SEMANTIC_MODEL(case0217),
        TEST_SEMANTIC_MODEL(case0218),
        TEST_SEMANTIC_MODEL(case0219),
        TEST_SEMANTIC_MODEL(case0220),

        TEST_SEMANTIC_MODEL(case0300),
        TEST_SEMANTIC_MODEL(case0301),
        TEST_SEMANTIC_MODEL(case0302),
        TEST_SEMANTIC_MODEL(case0303),
        TEST_SEMANTIC_MODEL(case0304),
        TEST_SEMANTIC_MODEL(case0305),
        TEST_SEMANTIC_MODEL(case0306),
        TEST_SEMANTIC_MODEL(case0307),
        TEST_SEMANTIC_MODEL(case0308),
        TEST_SEMANTIC_MODEL(case0309),
        TEST_SEMANTIC_MODEL(case0310),
        TEST_SEMANTIC_MODEL(case0311),
        TEST_SEMANTIC_MODEL(case0312),
        TEST_SEMANTIC_MODEL(case0313),
        TEST_SEMANTIC_MODEL(case0314),
        TEST_SEMANTIC_MODEL(case0315),
        TEST_SEMANTIC_MODEL(case0316),
        TEST_SEMANTIC_MODEL(case0317),
        TEST_SEMANTIC_MODEL(case0318),
        TEST_SEMANTIC_MODEL(case0319),
        TEST_SEMANTIC_MODEL(case0320),
        TEST_SEMANTIC_MODEL(case0321),
        TEST_SEMANTIC_MODEL(case0322),
        TEST_SEMANTIC_MODEL(case0323),
        TEST_SEMANTIC_MODEL(case0324),
        TEST_SEMANTIC_MODEL(case0325),
        TEST_SEMANTIC_MODEL(case0326),
        TEST_SEMANTIC_MODEL(case0327),
        TEST_SEMANTIC_MODEL(case0328),
        TEST_SEMANTIC_MODEL(case0329),
        TEST_SEMANTIC_MODEL(case0330),
        TEST_SEMANTIC_MODEL(case0331),
        TEST_SEMANTIC_MODEL(case0332),
        TEST_SEMANTIC_MODEL(case0333),
        TEST_SEMANTIC_MODEL(case0334),
        TEST_SEMANTIC_MODEL(case0335),
        TEST_SEMANTIC_MODEL(case0336),
        TEST_SEMANTIC_MODEL(case0337),
        TEST_SEMANTIC_MODEL(case0338),
        TEST_SEMANTIC_MODEL(case0339),
        TEST_SEMANTIC_MODEL(case0340),
        TEST_SEMANTIC_MODEL(case0341),
        TEST_SEMANTIC_MODEL(case0342),
        TEST_SEMANTIC_MODEL(case0343),
        TEST_SEMANTIC_MODEL(case0344),
        TEST_SEMANTIC_MODEL(case0345),
        TEST_SEMANTIC_MODEL(case0346),
        TEST_SEMANTIC_MODEL(case0347),
        TEST_SEMANTIC_MODEL(case0348),
        TEST_SEMANTIC_MODEL(case0349),
        TEST_SEMANTIC_MODEL(case0350),
        TEST_SEMANTIC_MODEL(case0351),
        TEST_SEMANTIC_MODEL(case0352),
        TEST_SEMANTIC_MODEL(case0353),
        TEST_SEMANTIC_MODEL(case0354),
        TEST_SEMANTIC_MODEL(case0355),
        TEST_SEMANTIC_MODEL(case0356),
        TEST_SEMANTIC_MODEL(case0357),
        TEST_SEMANTIC_MODEL(case0358),
        TEST_SEMANTIC_MODEL(case0359),
        TEST_SEMANTIC_MODEL(case0360),
        TEST_SEMANTIC_MODEL(case0361),
        TEST_SEMANTIC_MODEL(case0362),
        TEST_SEMANTIC_MODEL(case0363),
        TEST_SEMANTIC_MODEL(case0364),
        TEST_SEMANTIC_MODEL(case0365),
        TEST_SEMANTIC_MODEL(case0366),
        TEST_SEMANTIC_MODEL(case0367),
        TEST_SEMANTIC_MODEL(case0368),
        TEST_SEMANTIC_MODEL(case0369),
        TEST_SEMANTIC_MODEL(case0370),
        TEST_SEMANTIC_MODEL(case0371),
        TEST_SEMANTIC_MODEL(case0372),
        TEST_SEMANTIC_MODEL(case0373),
        TEST_SEMANTIC_MODEL(case0374),
        TEST_SEMANTIC_MODEL(case0375),
        TEST_SEMANTIC_MODEL(case0376),
        TEST_SEMANTIC_MODEL(case0377),
        TEST_SEMANTIC_MODEL(case0378),
        TEST_SEMANTIC_MODEL(case0379),
        TEST_SEMANTIC_MODEL(case0380),
        TEST_SEMANTIC_MODEL(case0381),
        TEST_SEMANTIC_MODEL(case0382),
        TEST_SEMANTIC_MODEL(case0383),
        TEST_SEMANTIC_MODEL(case0384),
        TEST_SEMANTIC_MODEL(case0385),
        TEST_SEMANTIC_MODEL(case0386),
        TEST_SEMANTIC_MODEL(case0387),
        TEST_SEMANTIC_MODEL(case0388),
        TEST_SEMANTIC_MODEL(case0389),
        TEST_SEMANTIC_MODEL(case0390),
        TEST_SEMANTIC_MODEL(case0391),
        TEST_SEMANTIC_MODEL(case0392),
        TEST_SEMANTIC_MODEL(case0393),
        TEST_SEMANTIC_MODEL(case0394),
        TEST_SEMANTIC_MODEL(case0395),
        TEST_SEMANTIC_MODEL(case0396),
        TEST_SEMANTIC_MODEL(case0397),
        TEST_SEMANTIC_MODEL(case0398),
        TEST_SEMANTIC_MODEL(case0399),

        TEST_SEMANTIC_MODEL(case0400),
        TEST_SEMANTIC_MODEL(case0401),
        TEST_SEMANTIC_MODEL(case0402),
        TEST_SEMANTIC_MODEL(case0403),
        TEST_SEMANTIC_MODEL(case0404),
        TEST_SEMANTIC_MODEL(case0405),
        TEST_SEMANTIC_MODEL(case0406),
        TEST_SEMANTIC_MODEL(case0407),
        TEST_SEMANTIC_MODEL(case0408),
        TEST_SEMANTIC_MODEL(case0409),
        TEST_SEMANTIC_MODEL(case0410),
        TEST_SEMANTIC_MODEL(case0411),
        TEST_SEMANTIC_MODEL(case0412),
        TEST_SEMANTIC_MODEL(case0413),
        TEST_SEMANTIC_MODEL(case0414),
        TEST_SEMANTIC_MODEL(case0415),
        TEST_SEMANTIC_MODEL(case0416),
        TEST_SEMANTIC_MODEL(case0417),
        TEST_SEMANTIC_MODEL(case0418),
        TEST_SEMANTIC_MODEL(case0419),
        TEST_SEMANTIC_MODEL(case0420),
        TEST_SEMANTIC_MODEL(case0421),
        TEST_SEMANTIC_MODEL(case0422),
        TEST_SEMANTIC_MODEL(case0423),
        TEST_SEMANTIC_MODEL(case0424),
        TEST_SEMANTIC_MODEL(case0425),
        TEST_SEMANTIC_MODEL(case0426),
        TEST_SEMANTIC_MODEL(case0427),
        TEST_SEMANTIC_MODEL(case0428),
        TEST_SEMANTIC_MODEL(case0429),
        TEST_SEMANTIC_MODEL(case0430),
        TEST_SEMANTIC_MODEL(case0431),
        TEST_SEMANTIC_MODEL(case0432),
        TEST_SEMANTIC_MODEL(case0433),
        TEST_SEMANTIC_MODEL(case0434),
        TEST_SEMANTIC_MODEL(case0435),
        TEST_SEMANTIC_MODEL(case0436),
        TEST_SEMANTIC_MODEL(case0437),
        TEST_SEMANTIC_MODEL(case0438),
        TEST_SEMANTIC_MODEL(case0439),
        TEST_SEMANTIC_MODEL(case0440),
        TEST_SEMANTIC_MODEL(case0441),
        TEST_SEMANTIC_MODEL(case0442),
        TEST_SEMANTIC_MODEL(case0443),
        TEST_SEMANTIC_MODEL(case0444),
        TEST_SEMANTIC_MODEL(case0445),
        TEST_SEMANTIC_MODEL(case0446),
        TEST_SEMANTIC_MODEL(case0447),
        TEST_SEMANTIC_MODEL(case0448),
        TEST_SEMANTIC_MODEL(case0449),
        TEST_SEMANTIC_MODEL(case0450),
        TEST_SEMANTIC_MODEL(case0451),
        TEST_SEMANTIC_MODEL(case0452),
        TEST_SEMANTIC_MODEL(case0453),
        TEST_SEMANTIC_MODEL(case0454),
        TEST_SEMANTIC_MODEL(case0455),
        TEST_SEMANTIC_MODEL(case0456),
        TEST_SEMANTIC_MODEL(case0457),
        TEST_SEMANTIC_MODEL(case0458),
        TEST_SEMANTIC_MODEL(case0459),
        TEST_SEMANTIC_MODEL(case0460),
        TEST_SEMANTIC_MODEL(case0461),
        TEST_SEMANTIC_MODEL(case0462),
        TEST_SEMANTIC_MODEL(case0463),
        TEST_SEMANTIC_MODEL(case0464),
        TEST_SEMANTIC_MODEL(case0465),
        TEST_SEMANTIC_MODEL(case0466),
        TEST_SEMANTIC_MODEL(case0467),
        TEST_SEMANTIC_MODEL(case0468),
        TEST_SEMANTIC_MODEL(case0469),
        TEST_SEMANTIC_MODEL(case0470),
        TEST_SEMANTIC_MODEL(case0471),
        TEST_SEMANTIC_MODEL(case0472),
        TEST_SEMANTIC_MODEL(case0473),
        TEST_SEMANTIC_MODEL(case0474),
        TEST_SEMANTIC_MODEL(case0475),
        TEST_SEMANTIC_MODEL(case0476),
        TEST_SEMANTIC_MODEL(case0477),
        TEST_SEMANTIC_MODEL(case0478),
        TEST_SEMANTIC_MODEL(case0479),
        TEST_SEMANTIC_MODEL(case0480),
        TEST_SEMANTIC_MODEL(case0481),
        TEST_SEMANTIC_MODEL(case0482),
        TEST_SEMANTIC_MODEL(case0483),
        TEST_SEMANTIC_MODEL(case0484),
        TEST_SEMANTIC_MODEL(case0485),
        TEST_SEMANTIC_MODEL(case0486),
        TEST_SEMANTIC_MODEL(case0487),
        TEST_SEMANTIC_MODEL(case0488),
        TEST_SEMANTIC_MODEL(case0489),
        TEST_SEMANTIC_MODEL(case0490),
        TEST_SEMANTIC_MODEL(case0491),
        TEST_SEMANTIC_MODEL(case0492),
        TEST_SEMANTIC_MODEL(case0493),
        TEST_SEMANTIC_MODEL(case0494),
        TEST_SEMANTIC_MODEL(case0495),
        TEST_SEMANTIC_MODEL(case0496),
        TEST_SEMANTIC_MODEL(case0497),
        TEST_SEMANTIC_MODEL(case0498),
        TEST_SEMANTIC_MODEL(case0499),

        TEST_SEMANTIC_MODEL(case0500),
        TEST_SEMANTIC_MODEL(case0501),
        TEST_SEMANTIC_MODEL(case0502),
        TEST_SEMANTIC_MODEL(case0503),
        TEST_SEMANTIC_MODEL(case0504),
        TEST_SEMANTIC_MODEL(case0505),
        TEST_SEMANTIC_MODEL(case0506),
        TEST_SEMANTIC_MODEL(case0507),
        TEST_SEMANTIC_MODEL(case0508),
        TEST_SEMANTIC_MODEL(case0509),
    };
};

} // C
} // psy

#endif
