/******************************************************************************
 Copyright (c) 2016,17 Leandro T. C. Melo (ltcmelo@gmail.com)

 This library is free software; you can redistribute it and/or modify it under
 the terms of the GNU Lesser General Public License as published by the Free
 Software Foundation; either version 2.1 of the License, or (at your option)
 any later version.

 This library is distributed in the hope that it will be useful, but WITHOUT
 ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License
 for more details.

 You should have received a copy of the GNU Lesser General Public License along
 with this library; if not, write to the Free Software Foundation, Inc., 51
 Franklin Street, Fifth Floor, Boston, MA  02110-1301 USA
 *****************************************************************************/

#ifndef PSYCHE_TEST_PARSER_H__
#define PSYCHE_TEST_PARSER_H__

#include "BaseTester.h"
#include "Control.h"
#include "FrontendConfig.h"
#include "DiagnosticCollector.h"
#include "TranslationUnit.h"
#include <functional>
#include <string>
#include <utility>
#include <vector>

#define PARSER_TEST(F) TestData { &TestParser::F, #F }

namespace psyche {

class TestParser final : public BaseTester
{
public:
    TestParser();
    ~TestParser();

    void testAll() override;

private:
    using TestData = std::pair<std::function<void(TestParser*)>, const char*>;

    void reset() override;

    void testSource(const std::string& source);

    void testCase1();
    void testCase2();
    void testCase3();
    void testCase4();
    void testCase5();
    void testCase6();
    void testCase7();
    void testCase8();
    void testCase9();
    void testCase10();
    void testCase11();
    void testCase12();
    void testCase13();
    void testCase14();
    void testCase15();
    void testCase16();
    void testCase17();
    void testCase18();
    void testCase19();
    void testCase20();
    void testCase21();
    void testCase22();
    void testCase23();
    void testCase24();
    void testCase25();
    void testCase26();
    void testCase27();
    void testCase28();
    void testCase29();
    void testCase30();

    std::vector<TestData> tests_
    {
        PARSER_TEST(testCase1),
        PARSER_TEST(testCase2),
        PARSER_TEST(testCase3),
        PARSER_TEST(testCase4),
        PARSER_TEST(testCase5),
        PARSER_TEST(testCase6),
        PARSER_TEST(testCase7),
        PARSER_TEST(testCase8),
        PARSER_TEST(testCase9),
        PARSER_TEST(testCase10),
        PARSER_TEST(testCase11),
        PARSER_TEST(testCase12),
        PARSER_TEST(testCase13),
        PARSER_TEST(testCase14),
        PARSER_TEST(testCase15),
        PARSER_TEST(testCase16),
        PARSER_TEST(testCase17),
        PARSER_TEST(testCase18),
        PARSER_TEST(testCase19),
        PARSER_TEST(testCase20),
        PARSER_TEST(testCase21),
        PARSER_TEST(testCase22),
        PARSER_TEST(testCase23),
        PARSER_TEST(testCase24),
        PARSER_TEST(testCase25),
        PARSER_TEST(testCase26),
        PARSER_TEST(testCase27),
        PARSER_TEST(testCase28),
        PARSER_TEST(testCase29),
        PARSER_TEST(testCase30)
    };

    psyche::DiagnosticCollector collector_;
    psyche::Control control_;
    std::unique_ptr<psyche::StringLiteral> name_;
};

} // namespace psyche

#endif
