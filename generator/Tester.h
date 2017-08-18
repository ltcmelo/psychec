/******************************************************************************
 * Copyright (c) 2016 Leandro T. C. Melo (ltcmelo@gmail.com)
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301
 * USA
 *****************************************************************************/

#ifndef PSYCHE_TESTER_H__
#define PSYCHE_TESTER_H__

#include "Config.h"
#include "CPlusPlusForwardDeclarations.h"
#include "Driver.h"
#include <algorithm>
#include <functional>
#include <iostream>
#include <string>
#include <vector>

// Use the macros below to control the testing worflow.

#define PSYCHE_TEST(F) TestData { &Tester::F, #F }

#define PSYCHE_TEST_FAIL(MSG) \
    do { \
        std::cout << "[psyche] Test " << currentTest_ << " -> FAILED\n" \
                  << "         Reason: " << MSG << "\n" \
                  << "         " << __FILE__ << ":" << __LINE__ << std::endl; \
        throw TestFailed(); \
    } while (0)

#define PSYCHE_EXPECT_EQ(EXPECTED, ACTUAL, EQ) \
    do { \
        if (!(EQ(EXPECTED, ACTUAL))) { \
            std::cout << "[psyche] Test " << currentTest_ << " -> FAILED\n" \
                      << "         Expected: " << EXPECTED << "\n" \
                      << "         Actual  : " << ACTUAL << "\n" \
                      << "         " << __FILE__ << ":" << __LINE__ << std::endl; \
            throw TestFailed(); \
        } \
    } while (0)

#define PSYCHE_EXPECT(EXPR, BOOLEAN) \
    do { \
        if (bool(EXPR) != BOOLEAN) { \
            std::cout << "[psyche] Test " << currentTest_ << " -> FAILED\n" \
                      << "        Expression is NOT " << #BOOLEAN << "\n" \
                      << "        " << __FILE__ << ":" << __LINE__ << std::endl; \
            throw TestFailed(); \
        } \
    } while (0)

#define PSYCHE_EQ_OPR(A, B) A == B
#define PSYCHE_STD_EQUAL(A, B) std::equal(A.begin(), A.end(), B.begin())
#define PSYCHE_EXPECT_PTR_EQ(EXPECTED, ACTUAL) PSYCHE_EXPECT_EQ(EXPECTED, ACTUAL, PSYCHE_EQ_OPR)
#define PSYCHE_EXPECT_STR_EQ(EXPECTED, ACTUAL) PSYCHE_EXPECT_EQ(EXPECTED, ACTUAL, PSYCHE_EQ_OPR)
#define PSYCHE_EXPECT_INT_EQ(EXPECTED, ACTUAL) PSYCHE_EXPECT_EQ(EXPECTED, ACTUAL, PSYCHE_EQ_OPR)
#define PSYCHE_EXPECT_CONTAINER_EQ(EXPECTED, ACTUAL) PSYCHE_EXPECT_EQ(EXPECTED, ACTUAL, PSYCHE_STD_EQUAL)
#define PSYCHE_EXPECT_TRUE(EXPR) PSYCHE_EXPECT(EXPR, true)
#define PSYCHE_EXPECT_FALSE(EXPR) PSYCHE_EXPECT(EXPR, false)

namespace psyche {

/*!
 * \brief The Tester class
 *
 * A very minimal and simple testing infrastructure.
 */
class PSYCHEC_API Tester final
{
public:
    /*!
     * \brief testAll
     *
     * Test everything.
     */
    void testAll();

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

private:
    using TestData = std::pair<std::function<void(Tester*)>, const char*>;

    void checkAst(const std::string& source, std::string expected, bool nonHeu = true);
    void checkConstraints(const std::string& source, std::string expected);

    void compareText(std::string expected, std::string actual) const;

    void reset();

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

struct PSYCHEC_API TestFailed {};

} // namespace psyche

#endif
