// Copyright (c) 2020/21 Leandro T. C. Melo <ltcmelo@gmail.com>
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

#include "TestTypeChecker.h"

#include "parser/Unparser.h"
#include "syntax/SyntaxLexemes.h"
#include "syntax/SyntaxNodes.h"

#include <algorithm>
#include <cstring>
#include <iostream>
#include <string>
#include <sstream>

using namespace psy;
using namespace C;

const std::string TestTypeChecker::Name = "";

void TestTypeChecker::testAll()
{
    run<TestTypeChecker>(tests_);
    std::cout << std::endl;
}

void TestTypeChecker::setUp()
{
}

void TestTypeChecker::tearDown()
{
}

void TestTypeChecker::case0001()
{
    typeCheck("void f() ();");
}

void TestTypeChecker::case0002()
{
    typeCheck("void f() () {}");
}

void TestTypeChecker::case0003()
{
    typeCheck("int k() [];");
}

void TestTypeChecker::case0004()
{
    typeCheck("int k() [] {}");
}

void TestTypeChecker::case0005()
{
    typeCheck("int (k)() [] {}");
}

void TestTypeChecker::case0006()
{
    typeCheck("int (k()) [] {}");
}

void TestTypeChecker::case0007()
{
    typeCheck("void k() {}");
}
void TestTypeChecker::case0008() {}
void TestTypeChecker::case0009() {}
