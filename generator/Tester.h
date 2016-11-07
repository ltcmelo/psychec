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

#include "CPlusPlusForwardDeclarations.h"
#include "Runner.h"
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
class Tester final
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
    void testCase25();
    void testCase26();
    void testCase27();
    void testCase28();
    void testCase29();
    void testCase30();
    void testCase31();
    void testCase32();
    void testCase33();
    void testCase34();
    void testCase35();
    void testCase36();
    void testCase37();
    void testCase38();
    void testCase39();
    void testCase40();
    void testCase41();
    void testCase42();
    void testCase43();
    void testCase44();
    void testCase45();
    void testCase46();
    void testCase47();
    void testCase48();
    void testCase49();
    void testCase50();
    void testCase51();
    void testCase52();
    void testCase53();
    void testCase54();
    void testCase55();
    void testCase56();

private:
    using TestData = std::pair<std::function<void(Tester*)>, const char*>;

    void checkAst(const std::string& source, std::string expected);
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
        PSYCHE_TEST(testCase12),
        PSYCHE_TEST(testCase13),
        PSYCHE_TEST(testCase14),
        PSYCHE_TEST(testCase15),
        PSYCHE_TEST(testCase16),
        PSYCHE_TEST(testCase17),
        PSYCHE_TEST(testCase18),
        PSYCHE_TEST(testCase19),
        PSYCHE_TEST(testCase20),
        PSYCHE_TEST(testCase21),
        PSYCHE_TEST(testCase22),
        PSYCHE_TEST(testCase23),
        PSYCHE_TEST(testCase24),
        PSYCHE_TEST(testCase25),
        PSYCHE_TEST(testCase26),
        PSYCHE_TEST(testCase27),
        PSYCHE_TEST(testCase28),
        PSYCHE_TEST(testCase29),
        // PSYCHE_TEST(testCase30), // Not yet supported
        PSYCHE_TEST(testCase31),
        PSYCHE_TEST(testCase32),
        PSYCHE_TEST(testCase33),
        PSYCHE_TEST(testCase34),
        PSYCHE_TEST(testCase35),
        PSYCHE_TEST(testCase36),
        PSYCHE_TEST(testCase37),
        PSYCHE_TEST(testCase38),
        PSYCHE_TEST(testCase39),
        PSYCHE_TEST(testCase40),
        PSYCHE_TEST(testCase41),
        PSYCHE_TEST(testCase42),
        PSYCHE_TEST(testCase43),
        PSYCHE_TEST(testCase44),
        PSYCHE_TEST(testCase45),
        PSYCHE_TEST(testCase46),
        PSYCHE_TEST(testCase47),
        PSYCHE_TEST(testCase48),
        PSYCHE_TEST(testCase49),
        PSYCHE_TEST(testCase50),
        PSYCHE_TEST(testCase51),
        PSYCHE_TEST(testCase52),
        PSYCHE_TEST(testCase53),
        PSYCHE_TEST(testCase54),
        PSYCHE_TEST(testCase55),
        PSYCHE_TEST(testCase56)
    };

    std::string currentTest_;
    AnalysisOptions options_;
};

struct TestFailed {};

} // namespace psyche

#endif
