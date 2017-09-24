/******************************************************************************
 Copyright (c) 2016,17 Leandro T. C. Melo (ltcmelo@gmail.com)

 This library is free software; you can redistribute it and/or modify it under
 the terms of the GNU Lesser General Public License as published by the Free
 Software Foundation; either version 2.1 of the License, or (at your option)
 any later version.

 This library is distributed in the hope that it will be useful, but WITHOUT
 ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License
 for more details.

 You should have received a copy of the GNU Lesser General Public License along
 with this library; if not, write to the Free Software Foundation, Inc., 51
 Franklin Street, Fifth Floor, Boston, MA  02110-1301 USA
 *****************************************************************************/

#ifndef PSYCHE_TEST_AMBIGUITY_RESOLVER_H__
#define PSYCHE_TEST_AMBIGUITY_RESOLVER_H__

#include "BaseTester.h"
#include "FrontendConfig.h"
#include "Driver.h"
#include <algorithm>
#include <functional>
#include <iostream>
#include <string>
#include <vector>

#define PSYCHE_TEST(F) TestData { &TestDisambiguator::F, #F }

namespace psyche {

class TestDisambiguator final : public BaseTester
{
public:
    void testAll() override;

private:
    void testCase1();
    void testCase2();
    void testCase3();
    void testCase4();
    void testCase5();
    void testCase6();
    void testCase7();
    void testCase8();
    void testCase9();
    void testCase10();
    void testCase11();
    void testCase12();
    void testCase13();
    void testCase14();
    void testCase15();
    void testCase16();
    void testCase17();
    void testCase18();
    void testCase19();
    void testCase20();
    void testCase21();
    void testCase22();
    void testCase23();
    void testCase24();

    using TestData = std::pair<std::function<void(TestDisambiguator*)>, const char*>;

    void checkAst(const std::string& source, std::string expected, bool nonHeu = true);
    void checkConstraints(const std::string& source, std::string expected);

    void compareText(std::string expected, std::string actual) const;

    void reset() override;

    /*
     * Add the name of all test functions to the vector below. Use the macro.
     */
    std::vector<TestData> tests_
    {
        PSYCHE_TEST(testCase1),
        PSYCHE_TEST(testCase2),
        PSYCHE_TEST(testCase3),
        PSYCHE_TEST(testCase4),
        PSYCHE_TEST(testCase5),
        PSYCHE_TEST(testCase6),
        PSYCHE_TEST(testCase7),
        PSYCHE_TEST(testCase8),
        PSYCHE_TEST(testCase9),
        PSYCHE_TEST(testCase10),
        PSYCHE_TEST(testCase11),
        // PSYCHE_TEST(testCase12), // Not yet supported
        PSYCHE_TEST(testCase13),
        PSYCHE_TEST(testCase14),
        PSYCHE_TEST(testCase15),
        // PSYCHE_TEST(testCase16), // Not yet supported
        // PSYCHE_TEST(testCase17)  // Not yet supported
        PSYCHE_TEST(testCase18),
        PSYCHE_TEST(testCase19),
        PSYCHE_TEST(testCase20),
        PSYCHE_TEST(testCase21),
        PSYCHE_TEST(testCase22),
        PSYCHE_TEST(testCase23),
        PSYCHE_TEST(testCase24),
    };

    std::string currentTest_;
    ExecutionOptions flags_;
};

} // namespace psyche

#endif
