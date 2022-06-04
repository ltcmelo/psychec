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

#include "ParserTester.h"

using namespace psy;
using namespace C;

const std::string ParserTester::Name = "PARSER";

void ParserTester::testParser()
{
    return run<ParserTester>(tests_);
}

void ParserTester::parseDeclaration(std::string text, Expectation X)
{
    (static_cast<InternalsTestSuite*>(suite_)->parseDeclaration(text, X));
}

void ParserTester::parseExpression(std::string text, Expectation X)
{
    (static_cast<InternalsTestSuite*>(suite_)->parseExpression(text, X));
}

void ParserTester::parseStatement(std::string text, Expectation X)
{
    (static_cast<InternalsTestSuite*>(suite_)->parseStatement(text, X));
}

void ParserTester::parse(std::string text, Expectation X, SyntaxTree::SyntaxCategory synCat)
{
    (static_cast<InternalsTestSuite*>(suite_)->parse(text, X, synCat));
}

void ParserTester::setUp()
{}

void ParserTester::tearDown()
{
}
