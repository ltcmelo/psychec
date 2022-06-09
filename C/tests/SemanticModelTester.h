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
        + 0000-0099 -> variable and/or function
        + 0100-0199 -> function (definition)
        + 0200-0299 -> parameter
        + 0300-0399 -> struct/union and enumeration
        + 0400-0499 -> field and enumerator

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
    };
};

} // C
} // psy

#endif
