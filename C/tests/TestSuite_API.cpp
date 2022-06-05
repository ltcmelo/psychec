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

#include "TestSuite_API.h"

#include "SemanticModelTester.h"

using namespace psy;
using namespace C;

APITestSuite::~APITestSuite()
{}

std::tuple<int, int> APITestSuite::testAll()
{
    auto SM = std::make_unique<SemanticModelTester>(this);
    SM->testSemanticModel();

    auto res = std::make_tuple(SM->totalPassed(),
                               SM->totalFailed());

    testers_.emplace_back(SM.release());

    return res;
}

std::string APITestSuite::description() const
{
    return "C API test suite";
}

void APITestSuite::printSummary() const
{
    for (auto const& tester : testers_) {
        std::cout << "    " << tester->name() << " passed: " << tester->totalPassed() << std::endl
                  << "    " << std::string(tester->name().length(), ' ') << " failed: " << tester->totalFailed() << std::endl;
    }
}
