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

#ifndef PSYCHE_C_TYPE_CHECKER_TEST_H__
#define PSYCHE_C_TYPE_CHECKER_TEST_H__

#include "Test.h"

#define TEST_TYPE_CHECKER(Function) TestFunction { &TypeCheckerTest::Function, #Function }

namespace psy {
namespace C {

class TypeCheckerTest final : public Test
{
public:
    static const std::string Name;

    virtual std::string name() const override { return Name; }

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

private:
    using TestFunction = std::pair<std::function<void(TypeCheckerTest*)>, const char*>;

    void setUp() override;
    void tearDown() override;

    std::vector<TestFunction> tests_
    {
        TEST_TYPE_CHECKER(case0001),
        TEST_TYPE_CHECKER(case0002),
        TEST_TYPE_CHECKER(case0003),
        TEST_TYPE_CHECKER(case0004),
        TEST_TYPE_CHECKER(case0005),
        TEST_TYPE_CHECKER(case0006),
        TEST_TYPE_CHECKER(case0007),
        TEST_TYPE_CHECKER(case0008),
        TEST_TYPE_CHECKER(case0009)
    };
};

} // C
} // psy

#endif
