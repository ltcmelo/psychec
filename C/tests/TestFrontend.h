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

#ifndef PSYCHE_C_TEST_FRONTEND_H__
#define PSYCHE_C_TEST_FRONTEND_H__

#include "API.h"
#include "SyntaxTree.h"

#include "compilation/Compilation.h"
#include "tests/TestRunner.h"

#include <functional>
#include <string>
#include <utility>
#include <vector>

#define CROSS_REFERENCE_TEST(CASE) { auto x = &CASE; (void)x; } \

namespace psy {
namespace C {

class TestFrontend : public TestRunner
{
public:
    struct Expectation
    {
        enum class ErrorOrWarn
        {
            Error,
            Warn
        };

        Expectation(int numE = 0, int numW = 0)
            : numE_(numE)
            , numW_(numW)
            , hasAmbiguity_(false)
        {}

        void setErrorCnt(int numE);
        void setWarnCnt(int numW);

        Expectation& AST(std::vector<SyntaxKind>&& v);
        Expectation& addDiagnostic(ErrorOrWarn v, std::string descriptorId = "");
        Expectation& replicateAmbiguity(const std::string& s = "");

        int numE_;
        int numW_;
        std::vector<std::string> descriptorsE_;
        std::vector<std::string> descriptorsW_;
        std::vector<SyntaxKind> syntax_;
        std::string ambiguousText_;
        bool hasAmbiguity_;
    };

protected:
    TestFrontend();
    ~TestFrontend();

    void parse(std::string source,
               Expectation X = Expectation(),
               SyntaxTree::SyntaxCategory cat = SyntaxTree::SyntaxCategory::Unspecified);
    void parseDeclaration(std::string source,
                          Expectation X = Expectation());
    void parseExpression(std::string source,
                         Expectation X = Expectation());
    void parseStatement(std::string source,
                        Expectation X = Expectation());

    void parseAndBind(std::string source, int errorCnt = 0, int warnCnt = 0);
    void parseAndBindAndCheck(std::string source, int errorCnt = 0, int warnCnt = 0);

    std::unique_ptr<SyntaxTree> tree_;
    std::unique_ptr<Compilation> compilation_;
};

} // C
} // psy

#endif
