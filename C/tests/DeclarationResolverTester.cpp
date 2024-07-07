// Copyright (c) 2024 Leandro T. C. Melo <ltcmelo@gmail.com>
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

#include "DeclarationResolverTester.h"

using namespace psy;
using namespace C;

const std::string DeclarationResolverTester::Name = "DECLARATION-RESOLVER";

void DeclarationResolverTester::testDeclarationResolver()
{
    return run<DeclarationResolverTester>(tests_);
}

void DeclarationResolverTester::resolve(std::string text, Expectation X)
{
    (static_cast<InternalsTestSuite*>(suite_)->resolve(text, X));
}

void DeclarationResolverTester::case0000(){}
void DeclarationResolverTester::case0001(){}
void DeclarationResolverTester::case0002(){}
void DeclarationResolverTester::case0003(){}
void DeclarationResolverTester::case0004(){}
void DeclarationResolverTester::case0005(){}
void DeclarationResolverTester::case0006(){}
void DeclarationResolverTester::case0007(){}
void DeclarationResolverTester::case0008(){}
void DeclarationResolverTester::case0009(){}
void DeclarationResolverTester::case0010(){}
void DeclarationResolverTester::case0011(){}
void DeclarationResolverTester::case0012(){}
void DeclarationResolverTester::case0013(){}
void DeclarationResolverTester::case0014(){}
void DeclarationResolverTester::case0015(){}
void DeclarationResolverTester::case0016(){}
void DeclarationResolverTester::case0017(){}
void DeclarationResolverTester::case0018(){}
void DeclarationResolverTester::case0019(){}
void DeclarationResolverTester::case0020(){}
void DeclarationResolverTester::case0021(){}
void DeclarationResolverTester::case0022(){}
void DeclarationResolverTester::case0023(){}
void DeclarationResolverTester::case0024(){}
void DeclarationResolverTester::case0025(){}
void DeclarationResolverTester::case0026(){}
void DeclarationResolverTester::case0027(){}
void DeclarationResolverTester::case0028(){}
void DeclarationResolverTester::case0029(){}
