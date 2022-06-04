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

#include <string>
#include <iostream>

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
