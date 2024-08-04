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

#ifndef PSYCHE_C_DECLARATION_RESOLVER_TESTER_H__
#define PSYCHE_C_DECLARATION_RESOLVER_TESTER_H__

#include "Fwds.h"
#include "TestSuite_Internals.h"
#include "tests/Tester.h"

#define TEST_DECLARATION_RESOLVER(Function) TestFunction { &TypeResolverTester::Function, #Function }

namespace psy {
namespace C {

class TypeResolverTester final : public Tester
{
public:
    TypeResolverTester(TestSuite* suite)
        : Tester(suite)
    {}

    static const std::string Name;
    virtual std::string name() const override { return Name; }

    void testTypeResolver();

    void resolve(std::string text, Expectation X);

    using TestFunction = std::pair<std::function<void(TypeResolverTester*)>, const char*>;

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

    std::vector<TestFunction> tests_
    {
        TEST_DECLARATION_RESOLVER(case0000),
        TEST_DECLARATION_RESOLVER(case0001),
        TEST_DECLARATION_RESOLVER(case0002),
        TEST_DECLARATION_RESOLVER(case0003),
        TEST_DECLARATION_RESOLVER(case0004),
        TEST_DECLARATION_RESOLVER(case0005),
        TEST_DECLARATION_RESOLVER(case0006),
        TEST_DECLARATION_RESOLVER(case0007),
        TEST_DECLARATION_RESOLVER(case0008),
        TEST_DECLARATION_RESOLVER(case0009),
        TEST_DECLARATION_RESOLVER(case0010),
        TEST_DECLARATION_RESOLVER(case0011),
        TEST_DECLARATION_RESOLVER(case0012),
        TEST_DECLARATION_RESOLVER(case0013),
        TEST_DECLARATION_RESOLVER(case0014),
        TEST_DECLARATION_RESOLVER(case0015),
        TEST_DECLARATION_RESOLVER(case0016),
        TEST_DECLARATION_RESOLVER(case0017),
        TEST_DECLARATION_RESOLVER(case0018),
        TEST_DECLARATION_RESOLVER(case0019),
        TEST_DECLARATION_RESOLVER(case0020),
        TEST_DECLARATION_RESOLVER(case0021),
        TEST_DECLARATION_RESOLVER(case0022),
        TEST_DECLARATION_RESOLVER(case0023),
        TEST_DECLARATION_RESOLVER(case0024),
        TEST_DECLARATION_RESOLVER(case0025),
        TEST_DECLARATION_RESOLVER(case0026),
        TEST_DECLARATION_RESOLVER(case0027),
        TEST_DECLARATION_RESOLVER(case0028),
        TEST_DECLARATION_RESOLVER(case0029),
    };
};

} // C
} // psy

#endif
