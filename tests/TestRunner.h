// Copyright (c) 2016-2020 Leandro T. C. Melo <ltcmelo@gmail.com>
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

#ifndef PSYCHE_TEST_RUNNER_H__
#define PSYCHE_TEST_RUNNER_H__

#include <iostream>
#include <string>

#define PSYCHE_TEST_FAIL(MSG) \
    do { \
        std::cout << "\n!\tFAIL\n" \
                  << "\tReason: " << MSG << "\n" \
                  << "\t\t" << __FILE__ << ":" << __LINE__ << std::endl; \
        throw TestFailed(); \
    } while (0)

#define PSYCHE_EXPECT_EQ(EXPECTED, ACTUAL, EQ) \
    do { \
        if (!(EQ(EXPECTED, ACTUAL))) { \
            std::cout << "\n!\tFAIL\n" \
                      << "\t\tExpected: " << EXPECTED << "\n" \
                      << "\t\tActual  : " << ACTUAL << "\n" \
                      << "\t\t" << __FILE__ << ":" << __LINE__ << std::endl; \
            throw TestFailed(); \
        } \
    } while (0)

#define PSYCHE_EXPECT(EXPR, BOOLEAN) \
    do { \
        if (bool(EXPR) != BOOLEAN) { \
            std::cout << "\n!\tFAIL\n" \
                      << "\t\tExpression is NOT " << #BOOLEAN << "\n" \
                      << "\t\t" << __FILE__ << ":" << __LINE__ << std::endl; \
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


namespace psy {

struct TestFailed {};

class TestRunner
{
public:
    virtual ~TestRunner() {}

    static void runSuite();

protected:
    template <class TesterT, class TestContT> void run(const TestContT& tests)
    {
        cntOK_ = 0;
        cntER_ = 0;

        for (auto testData : tests) {
            setUp();

            curTestName_ = testData.second;
            std::cout << "\t" << curTestName_ << "... ";

            try {
                auto curTestFunc = testData.first;
                curTestFunc(static_cast<TesterT*>(this));
                std::cout << "OK";
                ++cntOK_;
            } catch (const TestFailed&) {
                ++cntER_;
            }
            std::cout << "\n\t-------------------------------------------------" << std::endl;

            tearDown();
        }
        std::cout << "Succeeded: " << cntOK_ << std::endl
                  << "Failed   : " << cntER_ << std::endl;
    }

    virtual void testAll() = 0;

    virtual void setUp() {}
    virtual void tearDown() {}

    std::string curTestName_;
    int cntOK_;
    int cntER_;
};

} // psy

#endif
