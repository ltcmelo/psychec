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

#include "TestParser.h"
#include "AST.h"
#include "IO.h"
#include "Literals.h"
#include <iostream>
#include <string>
#include <cstring>

using namespace psyche;
using namespace psyche;

TestParser::TestParser()
    : name_(new StringLiteral("<test>", strlen("<test>")))
{
    control_.setDiagnosticCollector(&collector_);
}

TestParser::~TestParser()
{}

void TestParser::testAll()
{
    run<TestParser>(tests_);
}

void TestParser::reset()
{
    control_.diagnosticCollector()->reset();
}

void TestParser::testSource(const std::string& source)
{
    // TODO: Get through the driver, to ensure the default dialect is the same.
    Dialect dialect;
    dialect.c99 = 1;
    dialect.ext_KeywordsGNU = 1;
    dialect.ext_EnumeratorAttributes = 1;
    dialect.ext_AvailabilityAttribute = 1;
    dialect.nullptrOnNULL = 1;

    auto unit = std::make_unique<TranslationUnit>(&control_, name_.get());
    unit->setDialect(dialect);
    unit->setSource(source.c_str(), source.length());
    auto ret = unit->parse();

    PSYCHE_EXPECT_TRUE(ret);
    PSYCHE_EXPECT_TRUE(unit->ast());
    PSYCHE_EXPECT_TRUE(unit->ast()->asTranslationUnit());
    PSYCHE_EXPECT_INT_EQ(0, collector_.seenBlockingIssue());
}

/*
 * Preprocessed output generated with the following options:
 *
 *  -std=c99
 *  -D _Nonnull=
 *  -D _Nullable=
 *  -D __extension__
 *  -U __BLOCKS__  (disable clang's block language)
 *
 * Convenience for copy & paste:
 *  -std=c99 -D _Nonnull= -D _Nullable= -D __extension__ -U __BLOCKS__
 *
 */

void TestParser::testCase1()
{
    const std::string& source = readFile("testing/data/stdio_clang-pp_osx.i");
    testSource(source);
}

void TestParser::testCase2()
{
    const std::string& source = readFile("testing/data/stdio_gcc-pp_osx.i");
    testSource(source);
}

void TestParser::testCase3()
{
    const std::string& source = readFile("testing/data/stdlib_clang-pp_osx.i");
    testSource(source);
}

void TestParser::testCase4()
{
    const std::string& source = readFile("testing/data/stdlib_gcc-pp_osx.i");
    testSource(source);
}

void TestParser::testCase5()
{
    const std::string& source = readFile("testing/data/stddef_clang-pp_osx.i");
    testSource(source);
}

void TestParser::testCase6()
{
    const std::string& source = readFile("testing/data/stddef_gcc-pp_osx.i");
    testSource(source);
}

void TestParser::testCase7()
{
    const std::string& source = readFile("testing/data/stdint_clang-pp_osx.i");
    testSource(source);
}

void TestParser::testCase8()
{
    const std::string& source = readFile("testing/data/stdint_gcc-pp_osx.i");
    testSource(source);
}

void TestParser::testCase9()
{   
    std::string s = R"(
enum Enum
{
    A __attribute__((deprecated)),
    B __attribute__((deprecated)) = 9,
    C
};

int f()
{
    return A;
}
     )";

    testSource(s);
}

void TestParser::testCase10()
{
    const std::string& source = readFile("testing/data/wchar_gcc-pp_osx.i");
    testSource(source);
}

void TestParser::testCase11()
{
    const std::string& source = readFile("testing/data/wchar_clang-pp_osx.i");
    testSource(source);
}

void TestParser::testCase12()
{
    const std::string& source = readFile("testing/data/wctype_gcc-pp_osx.i");
    testSource(source);
}

void TestParser::testCase13()
{
    const std::string& source = readFile("testing/data/wctype_clang-pp_osx.i");
    testSource(source);
}

void TestParser::testCase14()
{
    std::string s = R"(
__attribute__((availability(macosx,introduced=10.12.1))) int a();

__attribute__((availability(macos,introduced=10.4.1,deprecated=10.6.0,obsoleted=10.7.0))) int b();

__attribute__((availability(ios,introduced=10.1))) int d();

__attribute__((availability(macosx,introduced=10.12.1))) int c();
     )";

    testSource(s);
}

void TestParser::testCase15()
{
    const std::string& source = readFile("testing/data/string_gcc-pp_osx.i");
    testSource(source);
}

void TestParser::testCase16()
{
    const std::string& source = readFile("testing/data/string_clang-pp_osx.i");
    testSource(source);
}

void TestParser::testCase17()
{
    const std::string& source = readFile("testing/data/math_gcc-pp_osx.i");
    testSource(source);
}

void TestParser::testCase18()
{
    const std::string& source = readFile("testing/data/math_clang-pp_osx.i");
    testSource(source);
}

void TestParser::testCase19()
{
    const std::string& source = readFile("testing/data/time_gcc-pp_osx.i");
    testSource(source);
}

void TestParser::testCase20()
{
    const std::string& source = readFile("testing/data/time_clang-pp_osx.i");
    testSource(source);
}

void TestParser::testCase21()
{
    const std::string& source = readFile("testing/data/inttypes_gcc-pp_osx.i");
    testSource(source);
}

void TestParser::testCase22()
{
    const std::string& source = readFile("testing/data/inttypes_clang-pp_osx.i");
    testSource(source);
}

void TestParser::testCase23()
{
    const std::string& source = readFile("testing/data/ctype_gcc-pp_osx.i");
    testSource(source);
}

void TestParser::testCase24()
{
    const std::string& source = readFile("testing/data/ctype_clang-pp_osx.i");
    testSource(source);
}

void TestParser::testCase25()
{
    const std::string& source = readFile("testing/data/assert_gcc-pp_osx.i");
    testSource(source);
}

void TestParser::testCase26()
{
    const std::string& source = readFile("testing/data/assert_clang-pp_osx.i");
    testSource(source);
}

void TestParser::testCase27()
{
    const std::string& source = readFile("testing/data/stdlib_gcc-pp_ubuntu.i");
    testSource(source);
}

void TestParser::testCase28()
{
    const std::string& source = readFile("testing/data/stdio_gcc-pp_ubuntu.i");
    testSource(source);
}

void TestParser::testCase29()
{
    const std::string& source = readFile("testing/data/stddef_gcc-pp_ubuntu.i");
    testSource(source);
}

void TestParser::testCase30()
{
    const std::string& source = readFile("testing/data/string_gcc-pp_ubuntu.i");
    testSource(source);
}
