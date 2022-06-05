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
        + 0200-0279 ->
        + 0280-0299 ->
        + 0300-0399 ->

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
    };
};

} // C
} // psy

#endif
