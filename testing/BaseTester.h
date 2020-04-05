/******************************************************************************
 Copyright (c) 2016-20 Leandro T. C. Melo (ltcmelo@gmail.com)

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

#ifndef PSYCHE_BASE_TESTER_H__
#define PSYCHE_BASE_TESTER_H__

#include <iostream>
#include <string>

#define PSYCHE_TEST_FAIL(MSG) \
    do { \
        std::cout << "\n\t"<< currentTest_ << " -> FAILED\n" \
                  << "\tReason: " << MSG << "\n" \
                  << "in " << __FILE__ << ":" << __LINE__ << std::endl; \
        throw TestFailed(); \
    } while (0)

#define PSYCHE_EXPECT_EQ(EXPECTED, ACTUAL, EQ) \
    do { \
        if (!(EQ(EXPECTED, ACTUAL))) { \
            std::cout << "\n\t" << currentTest_ << " -> FAILED\n" \
                      << "\t\tExpected: " << EXPECTED << "\n" \
                      << "\t\tActual  : " << ACTUAL << "\n" \
                      << "in " << __FILE__ << ":" << __LINE__ << std::endl; \
            throw TestFailed(); \
        } \
    } while (0)

#define PSYCHE_EXPECT(EXPR, BOOLEAN) \
    do { \
        if (bool(EXPR) != BOOLEAN) { \
            std::cout << "\n\t" << currentTest_ << " -> FAILED\n" \
                      << "\t\tExpression is NOT " << #BOOLEAN << "\n" \
                      << "in " << __FILE__ << ":" << __LINE__ << std::endl; \
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

class BaseTester
{
public:
    virtual ~BaseTester() {}

    static void runSuite();

protected:
    template <class TesterT, class TestContT>
    void run(const TestContT& tests) {
        for (auto testData : tests) {
            reset();
            currentTest_ = testData.second;
            std::cout << "\t" << currentTest_ << "...";
            testData.first(static_cast<TesterT*>(this));
            std::cout << " passed!" << std::endl;
        }
    }

    virtual void testAll() = 0;
    virtual void reset() {}

    std::string currentTest_;
};

struct TestFailed {};

} // namespace psyche

#endif
