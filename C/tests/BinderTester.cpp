// Copyright (c) 2020/21/22 Leandro T. C. Melo <ltcmelo@gmail.com>
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

#include "BinderTester.h"

#include "symbols/Symbol.h"
#include "parser/Unparser.h"
#include "symbols/Symbol_ALL.h"
#include "syntax/SyntaxLexeme_ALL.h"
#include "syntax/SyntaxNamePrinter.h"
#include "syntax/SyntaxNodes.h"

using namespace psy;
using namespace C;

const std::string BinderTester::Name = "BINDER";

void BinderTester::testBinder()
{
    // TEMPORARY
    std::vector<TestFunction> active;
    for (auto testData : tests_) {
        auto n = std::string(testData.second);
        auto p = n.find("case09");
        if (p != std::string::npos) {
            std::cout << "\t\tskip (TEMP) " << n << std::endl;
            continue;
        }
        active.push_back(testData);
    }

    return run<BinderTester>(active);
}

void BinderTester::bind(std::string text, Expectation X)
{
    (static_cast<InternalsTestSuite*>(suite_)->bind(text, X));
}

void BinderTester::setUp()
{}

void BinderTester::tearDown()
{}
