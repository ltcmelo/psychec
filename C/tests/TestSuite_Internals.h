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

#ifndef PSYCHE_C_TEST_H__
#define PSYCHE_C_TEST_H__

#include "API.h"

#include "TestExpectation.h"

#include "C/SyntaxTree.h"
#include "tests/TestSuite.h"

#define CROSS_REFERENCE_TEST(CASE) { auto x = &CASE; (void)x; } \

namespace psy {
namespace C {

class InternalsTestSuite : public TestSuite
{
public:
    virtual ~InternalsTestSuite();

protected:
    InternalsTestSuite();

    bool checkErrorAndWarn(Expectation X);

    void parseDeclaration(std::string text, Expectation X = Expectation());
    void parseExpression(std::string text, Expectation X = Expectation());
    void parseStatement(std::string text, Expectation X = Expectation());

    virtual void parse(std::string text,
                       Expectation X = Expectation(),
                       SyntaxTree::SyntaxCategory synCat = SyntaxTree::SyntaxCategory::UNSPECIFIED);

    virtual void bind(std::string text, Expectation X = Expectation()) {}

    std::unique_ptr<SyntaxTree> tree_;
    std::unique_ptr<Compilation> compilation_;
};

} // C
} // psy

#endif
