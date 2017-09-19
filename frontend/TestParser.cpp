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
using namespace CPlusPlus;

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
    dialect.gnuKeywordExt = 1;
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
 *  -D _Nonnull=
 *  -D _Nullable=
 *  -U __BLOCKS__  (disable clang's block language)
 *
 */

void TestParser::testCase1()
{
    const std::string& source = readFile("test-data/stdio_clang-pp_osx.i");
    testSource(source);
}

void TestParser::testCase2()
{
    const std::string& source = readFile("test-data/stdio_gcc-pp_osx.i");
    testSource(source);
}

void TestParser::testCase3()
{
    const std::string& source = readFile("test-data/stdlib_clang-pp_osx.i");
    testSource(source);
}

void TestParser::testCase4()
{
    const std::string& source = readFile("test-data/stdlib_gcc-pp_osx.i");
    testSource(source);
}

void TestParser::testCase5()
{
    const std::string& source = readFile("test-data/stddef_clang-pp_osx.i");
    testSource(source);
}

void TestParser::testCase6()
{
    const std::string& source = readFile("test-data/stddef_gcc-pp_osx.i");
    testSource(source);
}

void TestParser::testCase7()
{
    const std::string& source = readFile("test-data/stdint_clang-pp_osx.i");
    testSource(source);
}

void TestParser::testCase8()
{
    const std::string& source = readFile("test-data/stdint_gcc-pp_osx.i");
    testSource(source);
}

void TestParser::testCase9()
{

}

void TestParser::testCase10()
{

}

void TestParser::testCase11()
{

}

void TestParser::testCase12()
{

}

void TestParser::testCase13()
{

}

void TestParser::testCase14()
{

}

void TestParser::testCase15()
{

}
