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

#define TEST_DECLARATION_CHECKER(Function) TestFunction { &TypeCheckerTester::Function, #Function }

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

    void check(std::string text, Expectation X);

    using TestFunction = std::pair<std::function<void(TypeCheckerTester*)>, const char*>;

    /*
        Error
            + 0000-0499

        OK
            + 0500-0699 -> no inspection
            + 0700-0999 -> inspect type
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
        TEST_DECLARATION_CHECKER(case0000),
        TEST_DECLARATION_CHECKER(case0001),
        TEST_DECLARATION_CHECKER(case0002),
        TEST_DECLARATION_CHECKER(case0003),
        TEST_DECLARATION_CHECKER(case0004),
        TEST_DECLARATION_CHECKER(case0005),
        TEST_DECLARATION_CHECKER(case0006),
        TEST_DECLARATION_CHECKER(case0007),
        TEST_DECLARATION_CHECKER(case0008),
        TEST_DECLARATION_CHECKER(case0009),
        TEST_DECLARATION_CHECKER(case0010),
        TEST_DECLARATION_CHECKER(case0011),
        TEST_DECLARATION_CHECKER(case0012),
        TEST_DECLARATION_CHECKER(case0013),
        TEST_DECLARATION_CHECKER(case0014),
        TEST_DECLARATION_CHECKER(case0015),
        TEST_DECLARATION_CHECKER(case0016),
        TEST_DECLARATION_CHECKER(case0017),
        TEST_DECLARATION_CHECKER(case0018),
        TEST_DECLARATION_CHECKER(case0019),
        TEST_DECLARATION_CHECKER(case0020),
        TEST_DECLARATION_CHECKER(case0021),
        TEST_DECLARATION_CHECKER(case0022),
        TEST_DECLARATION_CHECKER(case0023),
        TEST_DECLARATION_CHECKER(case0024),
        TEST_DECLARATION_CHECKER(case0025),
        TEST_DECLARATION_CHECKER(case0026),
        TEST_DECLARATION_CHECKER(case0027),
        TEST_DECLARATION_CHECKER(case0028),
        TEST_DECLARATION_CHECKER(case0029),
        TEST_DECLARATION_CHECKER(case0030),
        TEST_DECLARATION_CHECKER(case0031),
        TEST_DECLARATION_CHECKER(case0032),
        TEST_DECLARATION_CHECKER(case0033),
        TEST_DECLARATION_CHECKER(case0034),
        TEST_DECLARATION_CHECKER(case0035),
        TEST_DECLARATION_CHECKER(case0036),
        TEST_DECLARATION_CHECKER(case0037),
        TEST_DECLARATION_CHECKER(case0038),
        TEST_DECLARATION_CHECKER(case0039),
        TEST_DECLARATION_CHECKER(case0040),
        TEST_DECLARATION_CHECKER(case0041),
        TEST_DECLARATION_CHECKER(case0042),
        TEST_DECLARATION_CHECKER(case0043),
        TEST_DECLARATION_CHECKER(case0044),
        TEST_DECLARATION_CHECKER(case0045),
        TEST_DECLARATION_CHECKER(case0046),
        TEST_DECLARATION_CHECKER(case0047),
        TEST_DECLARATION_CHECKER(case0048),
        TEST_DECLARATION_CHECKER(case0049),
        TEST_DECLARATION_CHECKER(case0050),
        TEST_DECLARATION_CHECKER(case0051),
        TEST_DECLARATION_CHECKER(case0052),
        TEST_DECLARATION_CHECKER(case0053),
        TEST_DECLARATION_CHECKER(case0054),
        TEST_DECLARATION_CHECKER(case0055),
        TEST_DECLARATION_CHECKER(case0056),
        TEST_DECLARATION_CHECKER(case0057),
        TEST_DECLARATION_CHECKER(case0058),
        TEST_DECLARATION_CHECKER(case0059),

        TEST_DECLARATION_CHECKER(case0500),
        TEST_DECLARATION_CHECKER(case0501),
        TEST_DECLARATION_CHECKER(case0502),
        TEST_DECLARATION_CHECKER(case0503),
        TEST_DECLARATION_CHECKER(case0504),
        TEST_DECLARATION_CHECKER(case0505),
        TEST_DECLARATION_CHECKER(case0506),
        TEST_DECLARATION_CHECKER(case0507),
        TEST_DECLARATION_CHECKER(case0508),
        TEST_DECLARATION_CHECKER(case0509),
        TEST_DECLARATION_CHECKER(case0510),
        TEST_DECLARATION_CHECKER(case0511),
        TEST_DECLARATION_CHECKER(case0512),
        TEST_DECLARATION_CHECKER(case0513),
        TEST_DECLARATION_CHECKER(case0514),
        TEST_DECLARATION_CHECKER(case0515),
        TEST_DECLARATION_CHECKER(case0516),
        TEST_DECLARATION_CHECKER(case0517),
        TEST_DECLARATION_CHECKER(case0518),
        TEST_DECLARATION_CHECKER(case0519),
        TEST_DECLARATION_CHECKER(case0520),
        TEST_DECLARATION_CHECKER(case0521),
        TEST_DECLARATION_CHECKER(case0522),
        TEST_DECLARATION_CHECKER(case0523),
        TEST_DECLARATION_CHECKER(case0524),
        TEST_DECLARATION_CHECKER(case0525),
        TEST_DECLARATION_CHECKER(case0526),
        TEST_DECLARATION_CHECKER(case0527),
        TEST_DECLARATION_CHECKER(case0528),
        TEST_DECLARATION_CHECKER(case0529),
        TEST_DECLARATION_CHECKER(case0530),
        TEST_DECLARATION_CHECKER(case0531),
        TEST_DECLARATION_CHECKER(case0532),
        TEST_DECLARATION_CHECKER(case0533),
        TEST_DECLARATION_CHECKER(case0534),
        TEST_DECLARATION_CHECKER(case0535),
        TEST_DECLARATION_CHECKER(case0536),
        TEST_DECLARATION_CHECKER(case0537),
        TEST_DECLARATION_CHECKER(case0538),
        TEST_DECLARATION_CHECKER(case0539),
        TEST_DECLARATION_CHECKER(case0540),
        TEST_DECLARATION_CHECKER(case0541),
        TEST_DECLARATION_CHECKER(case0542),
        TEST_DECLARATION_CHECKER(case0543),
        TEST_DECLARATION_CHECKER(case0544),
        TEST_DECLARATION_CHECKER(case0545),
        TEST_DECLARATION_CHECKER(case0546),
        TEST_DECLARATION_CHECKER(case0547),
        TEST_DECLARATION_CHECKER(case0548),
        TEST_DECLARATION_CHECKER(case0549),
        TEST_DECLARATION_CHECKER(case0550),
        TEST_DECLARATION_CHECKER(case0551),
        TEST_DECLARATION_CHECKER(case0552),
        TEST_DECLARATION_CHECKER(case0553),
        TEST_DECLARATION_CHECKER(case0554),
        TEST_DECLARATION_CHECKER(case0555),
        TEST_DECLARATION_CHECKER(case0556),
        TEST_DECLARATION_CHECKER(case0557),
        TEST_DECLARATION_CHECKER(case0558),
        TEST_DECLARATION_CHECKER(case0559),

        TEST_DECLARATION_CHECKER(case0700),
        TEST_DECLARATION_CHECKER(case0701),
        TEST_DECLARATION_CHECKER(case0702),
        TEST_DECLARATION_CHECKER(case0703),
        TEST_DECLARATION_CHECKER(case0704),
        TEST_DECLARATION_CHECKER(case0705),
        TEST_DECLARATION_CHECKER(case0706),
        TEST_DECLARATION_CHECKER(case0707),
        TEST_DECLARATION_CHECKER(case0708),
        TEST_DECLARATION_CHECKER(case0709),
        TEST_DECLARATION_CHECKER(case0710),
        TEST_DECLARATION_CHECKER(case0711),
        TEST_DECLARATION_CHECKER(case0712),
        TEST_DECLARATION_CHECKER(case0713),
        TEST_DECLARATION_CHECKER(case0714),
        TEST_DECLARATION_CHECKER(case0715),
        TEST_DECLARATION_CHECKER(case0716),
        TEST_DECLARATION_CHECKER(case0717),
        TEST_DECLARATION_CHECKER(case0718),
        TEST_DECLARATION_CHECKER(case0719),
        TEST_DECLARATION_CHECKER(case0720),
        TEST_DECLARATION_CHECKER(case0721),
        TEST_DECLARATION_CHECKER(case0722),
        TEST_DECLARATION_CHECKER(case0723),
        TEST_DECLARATION_CHECKER(case0724),
        TEST_DECLARATION_CHECKER(case0725),
        TEST_DECLARATION_CHECKER(case0726),
        TEST_DECLARATION_CHECKER(case0727),
        TEST_DECLARATION_CHECKER(case0728),
        TEST_DECLARATION_CHECKER(case0729),
        TEST_DECLARATION_CHECKER(case0730),
        TEST_DECLARATION_CHECKER(case0731),
        TEST_DECLARATION_CHECKER(case0732),
        TEST_DECLARATION_CHECKER(case0733),
        TEST_DECLARATION_CHECKER(case0734),
        TEST_DECLARATION_CHECKER(case0735),
        TEST_DECLARATION_CHECKER(case0736),
        TEST_DECLARATION_CHECKER(case0737),
        TEST_DECLARATION_CHECKER(case0738),
        TEST_DECLARATION_CHECKER(case0739),
        TEST_DECLARATION_CHECKER(case0740),
        TEST_DECLARATION_CHECKER(case0741),
        TEST_DECLARATION_CHECKER(case0742),
        TEST_DECLARATION_CHECKER(case0743),
        TEST_DECLARATION_CHECKER(case0744),
        TEST_DECLARATION_CHECKER(case0745),
        TEST_DECLARATION_CHECKER(case0746),
        TEST_DECLARATION_CHECKER(case0747),
        TEST_DECLARATION_CHECKER(case0748),
        TEST_DECLARATION_CHECKER(case0749),
        TEST_DECLARATION_CHECKER(case0750),
        TEST_DECLARATION_CHECKER(case0751),
        TEST_DECLARATION_CHECKER(case0752),
        TEST_DECLARATION_CHECKER(case0753),
        TEST_DECLARATION_CHECKER(case0754),
        TEST_DECLARATION_CHECKER(case0755),
        TEST_DECLARATION_CHECKER(case0756),
        TEST_DECLARATION_CHECKER(case0757),
        TEST_DECLARATION_CHECKER(case0758),
        TEST_DECLARATION_CHECKER(case0759),


    };
};

} // C
} // psy

#endif
