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

#ifndef PSYCHE_C_API_TEST_SUITE_H__
#define PSYCHE_C_API_TEST_SUITE_H__

#include "Fwds.h"

#include "tests/TestSuite.h"
#include "tests/Tester.h"

#include "C/SyntaxTree.h"
#include "C/syntax/SyntaxNodes.h"
#include "C/compilation/Compilation.h"
#include "C/compilation/SemanticModel.h"

#include <memory>
#include <tuple>
#include <vector>

namespace psy {
namespace C {

class APITestSuite : public TestSuite
{
    friend class SemanticModelTester;

public:
    virtual ~APITestSuite();

    virtual std::tuple<int, int> testAll() override;
    virtual std::string description() const override;
    virtual void printSummary() const override;

private:
    std::vector<std::unique_ptr<Tester>> testers_;
};

} // C
} // psy

#endif
