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

#ifndef PSYCHE_C_TYPE_CANONICALIZER_RESOLVER_TESTER_H__
#define PSYCHE_C_TYPE_CANONICALIZER_RESOLVER_TESTER_H__

#include "Fwds.h"
#include "TestSuite_Internals.h"
#include "tests/Tester.h"

#define TEST_TYPE_CANONICALIZER_AND_RESOLVER(Function) TestFunction { &TypeCanonicalizerAndResolverTester::Function, #Function }

namespace psy {
namespace C {

class TypeCanonicalizerAndResolverTester final : public Tester
{
public:
    TypeCanonicalizerAndResolverTester(TestSuite* suite)
        : Tester(suite)
    {}

    static const std::string Name;
    virtual std::string name() const override { return Name; }

    void testTypeCanonicalizerAndResolver();

    void canonicalizerAndResolveTypes(std::string text, Expectation X);

    using TestFunction = std::pair<std::function<void(TypeCanonicalizerAndResolverTester*)>, const char*>;

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

    std::vector<TestFunction> tests_
    {
        TEST_TYPE_CANONICALIZER_AND_RESOLVER(case0000),
        TEST_TYPE_CANONICALIZER_AND_RESOLVER(case0001),
        TEST_TYPE_CANONICALIZER_AND_RESOLVER(case0002),
        TEST_TYPE_CANONICALIZER_AND_RESOLVER(case0003),
        TEST_TYPE_CANONICALIZER_AND_RESOLVER(case0004),
        TEST_TYPE_CANONICALIZER_AND_RESOLVER(case0005),
        TEST_TYPE_CANONICALIZER_AND_RESOLVER(case0006),
        TEST_TYPE_CANONICALIZER_AND_RESOLVER(case0007),
        TEST_TYPE_CANONICALIZER_AND_RESOLVER(case0008),
        TEST_TYPE_CANONICALIZER_AND_RESOLVER(case0009),
        TEST_TYPE_CANONICALIZER_AND_RESOLVER(case0010),
        TEST_TYPE_CANONICALIZER_AND_RESOLVER(case0011),
        TEST_TYPE_CANONICALIZER_AND_RESOLVER(case0012),
        TEST_TYPE_CANONICALIZER_AND_RESOLVER(case0013),
        TEST_TYPE_CANONICALIZER_AND_RESOLVER(case0014),
        TEST_TYPE_CANONICALIZER_AND_RESOLVER(case0015),
        TEST_TYPE_CANONICALIZER_AND_RESOLVER(case0016),
        TEST_TYPE_CANONICALIZER_AND_RESOLVER(case0017),
        TEST_TYPE_CANONICALIZER_AND_RESOLVER(case0018),
        TEST_TYPE_CANONICALIZER_AND_RESOLVER(case0019),
        TEST_TYPE_CANONICALIZER_AND_RESOLVER(case0020),
        TEST_TYPE_CANONICALIZER_AND_RESOLVER(case0021),
        TEST_TYPE_CANONICALIZER_AND_RESOLVER(case0022),
        TEST_TYPE_CANONICALIZER_AND_RESOLVER(case0023),
        TEST_TYPE_CANONICALIZER_AND_RESOLVER(case0024),
        TEST_TYPE_CANONICALIZER_AND_RESOLVER(case0025),
        TEST_TYPE_CANONICALIZER_AND_RESOLVER(case0026),
        TEST_TYPE_CANONICALIZER_AND_RESOLVER(case0027),
        TEST_TYPE_CANONICALIZER_AND_RESOLVER(case0028),
        TEST_TYPE_CANONICALIZER_AND_RESOLVER(case0029),
        TEST_TYPE_CANONICALIZER_AND_RESOLVER(case0030),
        TEST_TYPE_CANONICALIZER_AND_RESOLVER(case0031),
        TEST_TYPE_CANONICALIZER_AND_RESOLVER(case0032),
        TEST_TYPE_CANONICALIZER_AND_RESOLVER(case0033),
        TEST_TYPE_CANONICALIZER_AND_RESOLVER(case0034),
        TEST_TYPE_CANONICALIZER_AND_RESOLVER(case0035),
        TEST_TYPE_CANONICALIZER_AND_RESOLVER(case0036),
        TEST_TYPE_CANONICALIZER_AND_RESOLVER(case0037),
        TEST_TYPE_CANONICALIZER_AND_RESOLVER(case0038),
        TEST_TYPE_CANONICALIZER_AND_RESOLVER(case0039),
        TEST_TYPE_CANONICALIZER_AND_RESOLVER(case0040),
        TEST_TYPE_CANONICALIZER_AND_RESOLVER(case0041),
        TEST_TYPE_CANONICALIZER_AND_RESOLVER(case0042),
        TEST_TYPE_CANONICALIZER_AND_RESOLVER(case0043),
        TEST_TYPE_CANONICALIZER_AND_RESOLVER(case0044),
        TEST_TYPE_CANONICALIZER_AND_RESOLVER(case0045),
        TEST_TYPE_CANONICALIZER_AND_RESOLVER(case0046),
        TEST_TYPE_CANONICALIZER_AND_RESOLVER(case0047),
        TEST_TYPE_CANONICALIZER_AND_RESOLVER(case0048),
        TEST_TYPE_CANONICALIZER_AND_RESOLVER(case0049),
        TEST_TYPE_CANONICALIZER_AND_RESOLVER(case0050),
        TEST_TYPE_CANONICALIZER_AND_RESOLVER(case0051),
        TEST_TYPE_CANONICALIZER_AND_RESOLVER(case0052),
        TEST_TYPE_CANONICALIZER_AND_RESOLVER(case0053),
        TEST_TYPE_CANONICALIZER_AND_RESOLVER(case0054),
        TEST_TYPE_CANONICALIZER_AND_RESOLVER(case0055),
        TEST_TYPE_CANONICALIZER_AND_RESOLVER(case0056),
        TEST_TYPE_CANONICALIZER_AND_RESOLVER(case0057),
        TEST_TYPE_CANONICALIZER_AND_RESOLVER(case0058),
        TEST_TYPE_CANONICALIZER_AND_RESOLVER(case0059),
    };
};

} // C
} // psy

#endif
