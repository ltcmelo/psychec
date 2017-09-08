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

#include "Config.h"
#include "BaseTester.h"
#include "Control.h"
#include "CPlusPlusForwardDeclarations.h"
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

    std::vector<TestData> tests_
    {
        PARSER_TEST(testCase1),
        PARSER_TEST(testCase2),
    };

    CPlusPlus::DiagnosticCollector collector_;
    CPlusPlus::Control control_;
    std::unique_ptr<CPlusPlus::StringLiteral> name_;
    std::unique_ptr<CPlusPlus::TranslationUnit> unit_;
};

} // namespace psyche

#endif
