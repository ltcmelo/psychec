// Copyright (c) 2016/17/18/19/20/21/22 Leandro T. C. Melo <ltcmelo@gmail.com>
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

#include "TestSuite.h"

#include "C/tests/TestSuite_Internals.h"
#include "C/tests/TestSuite_API.h"

#include <iostream>

using namespace psy;

void TestSuite::runTests()
{
    std::cout << "TESTS..." << std::endl;

    C::InternalsTestSuite suite0;
    auto [passed0, failed0] = suite0.testAll();

    C::APITestSuite suite1;
    auto [passed1, failed1] = suite1.testAll();

    std::cout << suite0.description() << std::endl;
    suite0.printSummary();

    std::cout << suite1.description() << std::endl;
    suite1.printSummary();

    auto accErrorCnt = failed0 + failed1;
    if (!accErrorCnt)
        std::cout << "All passed" << std::endl;
    else
        std::cout << std::string(17, '.') << " \n"
                  << "> Total failures: "
                  << failed0 + failed1
                  << std::endl;
}
