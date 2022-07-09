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

#ifndef PSYCHE_TESTER_H__
#define PSYCHE_TESTER_H__

#include <iostream>
#include <string>
#include <type_traits>

#define PSY__internals__EQ_OPTR(A, B) A == B
#define PSY__internals__EQ_STD(A, B) std::equal(A.begin(), A.end(), B.begin())

#define PSY__internals__FAIL(MSG) \
    do { \
        std::cout << "\n!\tFAIL\n" \
                  << "\tReason: " << MSG << "\n" \
                  << "\t\t" << __FILE__ << ":" << __LINE__ << std::endl; \
        throw TestFailed(); \
    } while (0)


#define PSY__internals__EXPECT_EQ(ACTUAL, EXPECTED, EQ) \
    do { \
        if (!(EQ(ACTUAL, EXPECTED))) { \
            std::cout << "\n!\tFAIL\n" \
                      << "\t\tActual  : " << ACTUAL << "\n" \
                      << "\t\tExpected: " << EXPECTED << "\n" \
                      << "\t\t" << __FILE__ << ":" << __LINE__ << std::endl; \
            throw TestFailed(); \
        } \
    } while (0)

#define PSY__internals__EXPECT_EQ_UNDER_TYPE(ACTUAL, EXPECTED, UNDER_TYPE) \
    do { \
        if (!(PSY__internals__EQ_OPTR(std::underlying_type_t<UNDER_TYPE>(ACTUAL), \
                                      std::underlying_type_t<UNDER_TYPE>(EXPECTED)))) { \
            std::cout << "\n!\tFAIL\n" \
                      << "\t\tActual  : " << to_string(ACTUAL) << "\n" \
                      << "\t\tExpected: " << to_string(EXPECTED) << "\n" \
                      << "\t\t" << __FILE__ << ":" << __LINE__ << std::endl; \
            throw TestFailed(); \
        } \
    } while (0)

#define PSY__internals__EXPECT_BOOL(EXPR, BOOLEAN) \
    do { \
        if (bool(EXPR) != BOOLEAN) { \
            std::cout << "\n!\tFAIL\n" \
                      << "\t\tExpression is NOT " << #BOOLEAN << "\n" \
                      << "\t\t" << __FILE__ << ":" << __LINE__ << std::endl; \
            throw TestFailed(); \
        } \
    } while (0)

#define PSY_EXPECT_EQ_PTR(ACTUAL, EXPECTED) PSY__internals__EXPECT_EQ(ACTUAL, EXPECTED, PSY__internals__EQ_OPTR)
#define PSY_EXPECT_EQ_STR(ACTUAL, EXPECTED) PSY__internals__EXPECT_EQ(ACTUAL, EXPECTED, PSY__internals__EQ_OPTR)
#define PSY_EXPECT_EQ_INT(ACTUAL, EXPECTED) PSY__internals__EXPECT_EQ(ACTUAL, EXPECTED, PSY__internals__EQ_OPTR)
#define PSY_EXPECT_EQ_ENU(ACTUAL, EXPECTED, ENUM_TYPE) PSY__internals__EXPECT_EQ_UNDER_TYPE(ACTUAL, EXPECTED, ENUM_TYPE)
#define PSY_EXPECT_EQ_STD(ACTUAL, EXPECTED) PSY__internals__EXPECT_EQ(ACTUAL, EXPECTED, PSY__internals__EQ_STD)

#define PSY_EXPECT_TRUE(EXPR) PSY__internals__EXPECT_BOOL(EXPR, true)
#define PSY_EXPECT_FALSE(EXPR) PSY__internals__EXPECT_BOOL(EXPR, false)

namespace psy {

struct TestFailed {};

class TestSuite;

class Tester
{
public:
    virtual ~Tester() {}

    virtual std::string name() const = 0;

    virtual void setUp() {}
    virtual void tearDown() {}

    int totalPassed() const { return cntPassed_; }
    int totalFailed() const { return cntFailed_; }

protected:
    Tester(TestSuite* suite)
        : suite_(suite)
        , cntPassed_(0)
        , cntFailed_(0)
    {}

    TestSuite* suite_;
    int cntPassed_;
    int cntFailed_;

    template <class TesterT, class TestContT>
    void run(const TestContT& tests)
    {
        for (auto testData : tests) {
            setUp();

            curTestFunc_ = testData.second;
            std::cout << "\t" << TesterT::Name << "-" << curTestFunc_ << "... ";

            try {
                auto curTestFunc = testData.first;
                curTestFunc(static_cast<TesterT*>(this));
                std::cout << "OK";
                ++cntPassed_;
            } catch (const TestFailed&) {
                ++cntFailed_;
            }
            std::cout << "\n\t-------------------------------------------------" << std::endl;

            tearDown();
        }
    }

    std::string curTestFunc_;
};

} // psy

#endif
