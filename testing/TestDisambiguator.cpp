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

/*
 Contributors:
   - Marcus Rodrigues (demaroar@gmail.com)
*/

#include "TestDisambiguator.h"
#include "AST.h"
#include "ASTDumper.h"
#include "Control.h"
#include "Factory.h"
#include "Literals.h"
#include "TranslationUnit.h"
#include <cctype>
#include <iostream>
#include <memory>
#include <sstream>
#include <string.h>

using namespace psyche;
using namespace psyche;

/*
 * The expected AST is obtained as follows:
 *  - The generator always generates a .dot file for the original program's AST
 *    and another .dot file for the "fixed" AST.
 *  - Create a PDF out of the fixed AST .dot file and manually inspect it to
 *    see whether the AST is correct and according to what we want.
 *  - If so, create a new test case and copy the contents of the fixed .dot
 *    file into it as a raw string.
 *
 * Follow the already existing examples for additional info.
 */
void TestDisambiguator::checkAst(const std::string &source, std::string expected, bool nonHeu)
{
    Factory factory;
    Driver driver(factory);
    flags_.flag_.noHeuristics = nonHeu;
    driver.process("testfile", source, flags_);
    PSYCHE_EXPECT_TRUE(driver.ast());

    std::ostringstream oss;
    ASTDumper(driver.tu()).dump(driver.ast(), "test", oss);

    compareText(expected, oss.str());
}

void TestDisambiguator::compareText(std::string expected, std::string actual) const
{
    // Remove all spaces to avoid silly errors in comparisson.
    auto check = [](const char c) { return std::isspace(c); };
    expected.erase(std::remove_if(expected.begin(), expected.end(), check), expected.end());
    actual.erase(std::remove_if(actual.begin(), actual.end(), check), actual.end());

    PSYCHE_EXPECT_STR_EQ(expected, actual);
}

void TestDisambiguator::reset()
{
    flags_ = ExecutionOptions();
    flags_.flag_.noHeuristics = true;
}

void TestDisambiguator::testAll()
{
    run<TestDisambiguator>(tests_);
}

void TestDisambiguator::testCase1()
{
    std::string source = R"raw(
void f(int) {
    x * y;
    y->m;
}
    )raw";

    std::string expectedAst = R"raw(
digraph AST { ordering=out;
n1 [label="TranslationUnitAST"];
n2 [label="FunctionDefinitionAST"];
n3 [label="SimpleSpecifierAST"];
n4 [label="DeclaratorAST"];
n5 [label="DeclaratorIdAST"];
n6 [label="SimpleNameAST"];
n7 [label="FunctionDeclaratorAST"];
n8 [label="ParameterDeclarationClauseAST"];
n9 [label="ParameterDeclarationAST"];
n10 [label="SimpleSpecifierAST"];
n11 [label="CompoundStatementAST"];
n12 [label="DeclarationStatementAST"];
n13 [label="SimpleDeclarationAST"];
n14 [label="NamedTypeSpecifierAST"];
n15 [label="SimpleNameAST"];
n16 [label="DeclaratorAST"];
n17 [label="PointerAST"];
n18 [label="DeclaratorIdAST"];
n19 [label="SimpleNameAST"];
n20 [label="ExpressionStatementAST"];
n21 [label="MemberAccessAST"];
n22 [label="IdExpressionAST"];
n23 [label="SimpleNameAST"];
n24 [label="SimpleNameAST"];
n1 -> n2
n2 -> n3
n3 -> t1
n2 -> n4
n4 -> n5
n5 -> n6
n6 -> t2
n4 -> n7
n7 -> t3
n7 -> n8
n8 -> n9
n9 -> n10
n10 -> t4
n7 -> t5
n2 -> n11
n11 -> t6
n11 -> n12
n12 -> n13
n13 -> n14
n14 -> n15
n15 -> t7
n13 -> n16
n16 -> n17
n17 -> t8
n16 -> n18
n18 -> n19
n19 -> t9
n13 -> t10
n11 -> n20
n20 -> n21
n21 -> n22
n22 -> n23
n23 -> t11
n21 -> t12
n21 -> n24
n24 -> t13
n20 -> t14
n11 -> t15
{ rank=same;
  t1 [shape=rect label = "void"];
  t2 [shape=rect label = "f"]; t1 -> t2 [arrowhead="vee" color="transparent"];
  t3 [shape=rect label = "("]; t2 -> t3 [arrowhead="vee" color="transparent"];
  t4 [shape=rect label = "int"]; t3 -> t4 [arrowhead="vee" color="transparent"];
  t5 [shape=rect label = ")"]; t4 -> t5 [arrowhead="vee" color="transparent"];
  t6 [shape=rect label = "{"]; t5 -> t6 [arrowhead="vee" color="transparent"];
  t7 [shape=rect label = "x"]; t6 -> t7 [arrowhead="vee" color="transparent"];
  t8 [shape=rect label = "*"]; t7 -> t8 [arrowhead="vee" color="transparent"];
  t9 [shape=rect label = "y"]; t8 -> t9 [arrowhead="vee" color="transparent"];
  t10 [shape=rect label = ";"]; t9 -> t10 [arrowhead="vee" color="transparent"];
  t11 [shape=rect label = "y"]; t10 -> t11 [arrowhead="vee" color="transparent"];
  t12 [shape=rect label = "->"]; t11 -> t12 [arrowhead="vee" color="transparent"];
  t13 [shape=rect label = "m"]; t12 -> t13 [arrowhead="vee" color="transparent"];
  t14 [shape=rect label = ";"]; t13 -> t14 [arrowhead="vee" color="transparent"];
  t15 [shape=rect label = "}"]; t14 -> t15 [arrowhead="vee" color="transparent"];
}
}
    )raw";

    checkAst(source, expectedAst);
}

void TestDisambiguator::testCase2()
{
      std::string source = R"raw(
void f(int) {
    x * y;
    x++;
}
      )raw";

      std::string expectedAst = R"raw(
digraph AST { ordering=out;
n1 [label="TranslationUnitAST"];
n2 [label="FunctionDefinitionAST"];
n3 [label="SimpleSpecifierAST"];
n4 [label="DeclaratorAST"];
n5 [label="DeclaratorIdAST"];
n6 [label="SimpleNameAST"];
n7 [label="FunctionDeclaratorAST"];
n8 [label="ParameterDeclarationClauseAST"];
n9 [label="ParameterDeclarationAST"];
n10 [label="SimpleSpecifierAST"];
n11 [label="CompoundStatementAST"];
n12 [label="ExpressionStatementAST"];
n13 [label="BinaryExpressionAST"];
n14 [label="IdExpressionAST"];
n15 [label="SimpleNameAST"];
n16 [label="IdExpressionAST"];
n17 [label="SimpleNameAST"];
n18 [label="ExpressionStatementAST"];
n19 [label="PostIncrDecrAST"];
n20 [label="IdExpressionAST"];
n21 [label="SimpleNameAST"];
n1 -> n2
n2 -> n3
n3 -> t1
n2 -> n4
n4 -> n5
n5 -> n6
n6 -> t2
n4 -> n7
n7 -> t3
n7 -> n8
n8 -> n9
n9 -> n10
n10 -> t4
n7 -> t5
n2 -> n11
n11 -> t6
n11 -> n12
n12 -> n13
n13 -> n14
n14 -> n15
n15 -> t7
n13 -> t8
n13 -> n16
n16 -> n17
n17 -> t9
n12 -> t10
n11 -> n18
n18 -> n19
n19 -> n20
n20 -> n21
n21 -> t11
n19 -> t12
n18 -> t13
n11 -> t14
{ rank=same;
t1 [shape=rect label = "void"];
t2 [shape=rect label = "f"]; t1 -> t2 [arrowhead="vee" color="transparent"];
t3 [shape=rect label = "("]; t2 -> t3 [arrowhead="vee" color="transparent"];
t4 [shape=rect label = "int"]; t3 -> t4 [arrowhead="vee" color="transparent"];
t5 [shape=rect label = ")"]; t4 -> t5 [arrowhead="vee" color="transparent"];
t6 [shape=rect label = "{"]; t5 -> t6 [arrowhead="vee" color="transparent"];
t7 [shape=rect label = "x"]; t6 -> t7 [arrowhead="vee" color="transparent"];
t8 [shape=rect label = "*"]; t7 -> t8 [arrowhead="vee" color="transparent"];
t9 [shape=rect label = "y"]; t8 -> t9 [arrowhead="vee" color="transparent"];
t10 [shape=rect label = ";"]; t9 -> t10 [arrowhead="vee" color="transparent"];
t11 [shape=rect label = "x"]; t10 -> t11 [arrowhead="vee" color="transparent"];
t12 [shape=rect label = "++"]; t11 -> t12 [arrowhead="vee" color="transparent"];
t13 [shape=rect label = ";"]; t12 -> t13 [arrowhead="vee" color="transparent"];
t14 [shape=rect label = "}"]; t13 -> t14 [arrowhead="vee" color="transparent"];
}
}
              )raw";

    checkAst(source, expectedAst);
}

void TestDisambiguator::testCase3()
{
    std::string source = R"raw(
void f(int) {
    int * y;
}
    )raw";

    std::string expectedAst = R"raw(
digraph AST { ordering=out;
n1 [label="TranslationUnitAST"];
n2 [label="FunctionDefinitionAST"];
n3 [label="SimpleSpecifierAST"];
n4 [label="DeclaratorAST"];
n5 [label="DeclaratorIdAST"];
n6 [label="SimpleNameAST"];
n7 [label="FunctionDeclaratorAST"];
n8 [label="ParameterDeclarationClauseAST"];
n9 [label="ParameterDeclarationAST"];
n10 [label="SimpleSpecifierAST"];
n11 [label="CompoundStatementAST"];
n12 [label="DeclarationStatementAST"];
n13 [label="SimpleDeclarationAST"];
n14 [label="SimpleSpecifierAST"];
n15 [label="DeclaratorAST"];
n16 [label="PointerAST"];
n17 [label="DeclaratorIdAST"];
n18 [label="SimpleNameAST"];
n1 -> n2
n2 -> n3
n3 -> t1
n2 -> n4
n4 -> n5
n5 -> n6
n6 -> t2
n4 -> n7
n7 -> t3
n7 -> n8
n8 -> n9
n9 -> n10
n10 -> t4
n7 -> t5
n2 -> n11
n11 -> t6
n11 -> n12
n12 -> n13
n13 -> n14
n14 -> t7
n13 -> n15
n15 -> n16
n16 -> t8
n15 -> n17
n17 -> n18
n18 -> t9
n13 -> t10
n11 -> t11
{ rank=same;
  t1 [shape=rect label = "void"];
  t2 [shape=rect label = "f"]; t1 -> t2 [arrowhead="vee" color="transparent"];
  t3 [shape=rect label = "("]; t2 -> t3 [arrowhead="vee" color="transparent"];
  t4 [shape=rect label = "int"]; t3 -> t4 [arrowhead="vee" color="transparent"];
  t5 [shape=rect label = ")"]; t4 -> t5 [arrowhead="vee" color="transparent"];
  t6 [shape=rect label = "{"]; t5 -> t6 [arrowhead="vee" color="transparent"];
  t7 [shape=rect label = "int"]; t6 -> t7 [arrowhead="vee" color="transparent"];
  t8 [shape=rect label = "*"]; t7 -> t8 [arrowhead="vee" color="transparent"];
  t9 [shape=rect label = "y"]; t8 -> t9 [arrowhead="vee" color="transparent"];
  t10 [shape=rect label = ";"]; t9 -> t10 [arrowhead="vee" color="transparent"];
  t11 [shape=rect label = "}"]; t10 -> t11 [arrowhead="vee" color="transparent"];
}
}
    )raw";

    checkAst(source, expectedAst);
}

void TestDisambiguator::testCase4()
{
    std::string source = R"raw(
void f(int) {
    int y;
}
    )raw";

    std::string expectedAst = R"raw(
digraph AST { ordering=out;
n1 [label="TranslationUnitAST"];
n2 [label="FunctionDefinitionAST"];
n3 [label="SimpleSpecifierAST"];
n4 [label="DeclaratorAST"];
n5 [label="DeclaratorIdAST"];
n6 [label="SimpleNameAST"];
n7 [label="FunctionDeclaratorAST"];
n8 [label="ParameterDeclarationClauseAST"];
n9 [label="ParameterDeclarationAST"];
n10 [label="SimpleSpecifierAST"];
n11 [label="CompoundStatementAST"];
n12 [label="DeclarationStatementAST"];
n13 [label="SimpleDeclarationAST"];
n14 [label="SimpleSpecifierAST"];
n15 [label="DeclaratorAST"];
n16 [label="DeclaratorIdAST"];
n17 [label="SimpleNameAST"];
n1 -> n2
n2 -> n3
n3 -> t1
n2 -> n4
n4 -> n5
n5 -> n6
n6 -> t2
n4 -> n7
n7 -> t3
n7 -> n8
n8 -> n9
n9 -> n10
n10 -> t4
n7 -> t5
n2 -> n11
n11 -> t6
n11 -> n12
n12 -> n13
n13 -> n14
n14 -> t7
n13 -> n15
n15 -> n16
n16 -> n17
n17 -> t8
n13 -> t9
n11 -> t10
{ rank=same;
  t1 [shape=rect label = "void"];
  t2 [shape=rect label = "f"]; t1 -> t2 [arrowhead="vee" color="transparent"];
  t3 [shape=rect label = "("]; t2 -> t3 [arrowhead="vee" color="transparent"];
  t4 [shape=rect label = "int"]; t3 -> t4 [arrowhead="vee" color="transparent"];
  t5 [shape=rect label = ")"]; t4 -> t5 [arrowhead="vee" color="transparent"];
  t6 [shape=rect label = "{"]; t5 -> t6 [arrowhead="vee" color="transparent"];
  t7 [shape=rect label = "int"]; t6 -> t7 [arrowhead="vee" color="transparent"];
  t8 [shape=rect label = "y"]; t7 -> t8 [arrowhead="vee" color="transparent"];
  t9 [shape=rect label = ";"]; t8 -> t9 [arrowhead="vee" color="transparent"];
  t10 [shape=rect label = "}"]; t9 -> t10 [arrowhead="vee" color="transparent"];
}
}
    )raw";

    checkAst(source, expectedAst);
}

void TestDisambiguator::testCase5()
{
    std::string source = R"raw(
void f(int) {
    x y;
}
    )raw";

    std::string expectedAst = R"raw(
digraph AST { ordering=out;
n1 [label="TranslationUnitAST"];
n2 [label="FunctionDefinitionAST"];
n3 [label="SimpleSpecifierAST"];
n4 [label="DeclaratorAST"];
n5 [label="DeclaratorIdAST"];
n6 [label="SimpleNameAST"];
n7 [label="FunctionDeclaratorAST"];
n8 [label="ParameterDeclarationClauseAST"];
n9 [label="ParameterDeclarationAST"];
n10 [label="SimpleSpecifierAST"];
n11 [label="CompoundStatementAST"];
n12 [label="DeclarationStatementAST"];
n13 [label="SimpleDeclarationAST"];
n14 [label="NamedTypeSpecifierAST"];
n15 [label="SimpleNameAST"];
n16 [label="DeclaratorAST"];
n17 [label="DeclaratorIdAST"];
n18 [label="SimpleNameAST"];
n1 -> n2
n2 -> n3
n3 -> t1
n2 -> n4
n4 -> n5
n5 -> n6
n6 -> t2
n4 -> n7
n7 -> t3
n7 -> n8
n8 -> n9
n9 -> n10
n10 -> t4
n7 -> t5
n2 -> n11
n11 -> t6
n11 -> n12
n12 -> n13
n13 -> n14
n14 -> n15
n15 -> t7
n13 -> n16
n16 -> n17
n17 -> n18
n18 -> t8
n13 -> t9
n11 -> t10
{ rank=same;
  t1 [shape=rect label = "void"];
  t2 [shape=rect label = "f"]; t1 -> t2 [arrowhead="vee" color="transparent"];
  t3 [shape=rect label = "("]; t2 -> t3 [arrowhead="vee" color="transparent"];
  t4 [shape=rect label = "int"]; t3 -> t4 [arrowhead="vee" color="transparent"];
  t5 [shape=rect label = ")"]; t4 -> t5 [arrowhead="vee" color="transparent"];
  t6 [shape=rect label = "{"]; t5 -> t6 [arrowhead="vee" color="transparent"];
  t7 [shape=rect label = "x"]; t6 -> t7 [arrowhead="vee" color="transparent"];
  t8 [shape=rect label = "y"]; t7 -> t8 [arrowhead="vee" color="transparent"];
  t9 [shape=rect label = ";"]; t8 -> t9 [arrowhead="vee" color="transparent"];
  t10 [shape=rect label = "}"]; t9 -> t10 [arrowhead="vee" color="transparent"];
}
}
    )raw";

    checkAst(source, expectedAst);
}

void TestDisambiguator::testCase6()
{
    std::string source = R"raw(
void f(int) {
    x * y = 0;
}
    )raw";

    std::string expectedAst = R"raw(
digraph AST { ordering=out;
n1 [label="TranslationUnitAST"];
n2 [label="FunctionDefinitionAST"];
n3 [label="SimpleSpecifierAST"];
n4 [label="DeclaratorAST"];
n5 [label="DeclaratorIdAST"];
n6 [label="SimpleNameAST"];
n7 [label="FunctionDeclaratorAST"];
n8 [label="ParameterDeclarationClauseAST"];
n9 [label="ParameterDeclarationAST"];
n10 [label="SimpleSpecifierAST"];
n11 [label="CompoundStatementAST"];
n12 [label="DeclarationStatementAST"];
n13 [label="SimpleDeclarationAST"];
n14 [label="NamedTypeSpecifierAST"];
n15 [label="SimpleNameAST"];
n16 [label="DeclaratorAST"];
n17 [label="PointerAST"];
n18 [label="DeclaratorIdAST"];
n19 [label="SimpleNameAST"];
n20 [label="NumericLiteralAST"];
n1 -> n2
n2 -> n3
n3 -> t1
n2 -> n4
n4 -> n5
n5 -> n6
n6 -> t2
n4 -> n7
n7 -> t3
n7 -> n8
n8 -> n9
n9 -> n10
n10 -> t4
n7 -> t5
n2 -> n11
n11 -> t6
n11 -> n12
n12 -> n13
n13 -> n14
n14 -> n15
n15 -> t7
n13 -> n16
n16 -> n17
n17 -> t8
n16 -> n18
n18 -> n19
n19 -> t9
n16 -> t10
n16 -> n20
n20 -> t11
n13 -> t12
n11 -> t13
{ rank=same;
  t1 [shape=rect label = "void"];
  t2 [shape=rect label = "f"]; t1 -> t2 [arrowhead="vee" color="transparent"];
  t3 [shape=rect label = "("]; t2 -> t3 [arrowhead="vee" color="transparent"];
  t4 [shape=rect label = "int"]; t3 -> t4 [arrowhead="vee" color="transparent"];
  t5 [shape=rect label = ")"]; t4 -> t5 [arrowhead="vee" color="transparent"];
  t6 [shape=rect label = "{"]; t5 -> t6 [arrowhead="vee" color="transparent"];
  t7 [shape=rect label = "x"]; t6 -> t7 [arrowhead="vee" color="transparent"];
  t8 [shape=rect label = "*"]; t7 -> t8 [arrowhead="vee" color="transparent"];
  t9 [shape=rect label = "y"]; t8 -> t9 [arrowhead="vee" color="transparent"];
  t10 [shape=rect label = "="]; t9 -> t10 [arrowhead="vee" color="transparent"];
  t11 [shape=rect label = "0"]; t10 -> t11 [arrowhead="vee" color="transparent"];
  t12 [shape=rect label = ";"]; t11 -> t12 [arrowhead="vee" color="transparent"];
  t13 [shape=rect label = "}"]; t12 -> t13 [arrowhead="vee" color="transparent"];
}
}
    )raw";

    checkAst(source, expectedAst);
}

void TestDisambiguator::testCase7()
{
    std::string source = R"raw(
void f(int) {
    x * y;
    *y = 5;
}
    )raw";

    std::string expectedAst = R"raw(
digraph AST { ordering=out;
n1 [label="TranslationUnitAST"];
n2 [label="FunctionDefinitionAST"];
n3 [label="SimpleSpecifierAST"];
n4 [label="DeclaratorAST"];
n5 [label="DeclaratorIdAST"];
n6 [label="SimpleNameAST"];
n7 [label="FunctionDeclaratorAST"];
n8 [label="ParameterDeclarationClauseAST"];
n9 [label="ParameterDeclarationAST"];
n10 [label="SimpleSpecifierAST"];
n11 [label="CompoundStatementAST"];
n12 [label="DeclarationStatementAST"];
n13 [label="SimpleDeclarationAST"];
n14 [label="NamedTypeSpecifierAST"];
n15 [label="SimpleNameAST"];
n16 [label="DeclaratorAST"];
n17 [label="PointerAST"];
n18 [label="DeclaratorIdAST"];
n19 [label="SimpleNameAST"];
n20 [label="ExpressionStatementAST"];
n21 [label="BinaryExpressionAST"];
n22 [label="UnaryExpressionAST"];
n23 [label="IdExpressionAST"];
n24 [label="SimpleNameAST"];
n25 [label="NumericLiteralAST"];
n1 -> n2
n2 -> n3
n3 -> t1
n2 -> n4
n4 -> n5
n5 -> n6
n6 -> t2
n4 -> n7
n7 -> t3
n7 -> n8
n8 -> n9
n9 -> n10
n10 -> t4
n7 -> t5
n2 -> n11
n11 -> t6
n11 -> n12
n12 -> n13
n13 -> n14
n14 -> n15
n15 -> t7
n13 -> n16
n16 -> n17
n17 -> t8
n16 -> n18
n18 -> n19
n19 -> t9
n13 -> t10
n11 -> n20
n20 -> n21
n21 -> n22
n22 -> t11
n22 -> n23
n23 -> n24
n24 -> t12
n21 -> t13
n21 -> n25
n25 -> t14
n20 -> t15
n11 -> t16
{ rank=same;
  t1 [shape=rect label = "void"];
  t2 [shape=rect label = "f"]; t1 -> t2 [arrowhead="vee" color="transparent"];
  t3 [shape=rect label = "("]; t2 -> t3 [arrowhead="vee" color="transparent"];
  t4 [shape=rect label = "int"]; t3 -> t4 [arrowhead="vee" color="transparent"];
  t5 [shape=rect label = ")"]; t4 -> t5 [arrowhead="vee" color="transparent"];
  t6 [shape=rect label = "{"]; t5 -> t6 [arrowhead="vee" color="transparent"];
  t7 [shape=rect label = "x"]; t6 -> t7 [arrowhead="vee" color="transparent"];
  t8 [shape=rect label = "*"]; t7 -> t8 [arrowhead="vee" color="transparent"];
  t9 [shape=rect label = "y"]; t8 -> t9 [arrowhead="vee" color="transparent"];
  t10 [shape=rect label = ";"]; t9 -> t10 [arrowhead="vee" color="transparent"];
  t11 [shape=rect label = "*"]; t10 -> t11 [arrowhead="vee" color="transparent"];
  t12 [shape=rect label = "y"]; t11 -> t12 [arrowhead="vee" color="transparent"];
  t13 [shape=rect label = "="]; t12 -> t13 [arrowhead="vee" color="transparent"];
  t14 [shape=rect label = "5"]; t13 -> t14 [arrowhead="vee" color="transparent"];
  t15 [shape=rect label = ";"]; t14 -> t15 [arrowhead="vee" color="transparent"];
  t16 [shape=rect label = "}"]; t15 -> t16 [arrowhead="vee" color="transparent"];
}
}
    )raw";

    checkAst(source, expectedAst);
}

void TestDisambiguator::testCase8()
{
    std::string source = R"raw(
void f() {
    A* a;
    if (a->x)
        a;
}
    )raw";

    std::string expectedAst = R"raw(
digraph AST { ordering=out;
n1 [label="TranslationUnitAST"];
n2 [label="FunctionDefinitionAST"];
n3 [label="SimpleSpecifierAST"];
n4 [label="DeclaratorAST"];
n5 [label="DeclaratorIdAST"];
n6 [label="SimpleNameAST"];
n7 [label="FunctionDeclaratorAST"];
n8 [label="CompoundStatementAST"];
n9 [label="DeclarationStatementAST"];
n10 [label="SimpleDeclarationAST"];
n11 [label="NamedTypeSpecifierAST"];
n12 [label="SimpleNameAST"];
n13 [label="DeclaratorAST"];
n14 [label="PointerAST"];
n15 [label="DeclaratorIdAST"];
n16 [label="SimpleNameAST"];
n17 [label="IfStatementAST"];
n18 [label="MemberAccessAST"];
n19 [label="IdExpressionAST"];
n20 [label="SimpleNameAST"];
n21 [label="SimpleNameAST"];
n22 [label="ExpressionStatementAST"];
n23 [label="IdExpressionAST"];
n24 [label="SimpleNameAST"];
n1 -> n2
n2 -> n3
n3 -> t1
n2 -> n4
n4 -> n5
n5 -> n6
n6 -> t2
n4 -> n7
n7 -> t3
n7 -> t4
n2 -> n8
n8 -> t5
n8 -> n9
n9 -> n10
n10 -> n11
n11 -> n12
n12 -> t6
n10 -> n13
n13 -> n14
n14 -> t7
n13 -> n15
n15 -> n16
n16 -> t8
n10 -> t9
n8 -> n17
n17 -> t10
n17 -> t11
n17 -> n18
n18 -> n19
n19 -> n20
n20 -> t12
n18 -> t13
n18 -> n21
n21 -> t14
n17 -> t15
n17 -> n22
n22 -> n23
n23 -> n24
n24 -> t16
n22 -> t17
n8 -> t18
{ rank=same;
  t1 [shape=rect label = "void"];
  t2 [shape=rect label = "f"]; t1 -> t2 [arrowhead="vee" color="transparent"];
  t3 [shape=rect label = "("]; t2 -> t3 [arrowhead="vee" color="transparent"];
  t4 [shape=rect label = ")"]; t3 -> t4 [arrowhead="vee" color="transparent"];
  t5 [shape=rect label = "{"]; t4 -> t5 [arrowhead="vee" color="transparent"];
  t6 [shape=rect label = "A"]; t5 -> t6 [arrowhead="vee" color="transparent"];
  t7 [shape=rect label = "*"]; t6 -> t7 [arrowhead="vee" color="transparent"];
  t8 [shape=rect label = "a"]; t7 -> t8 [arrowhead="vee" color="transparent"];
  t9 [shape=rect label = ";"]; t8 -> t9 [arrowhead="vee" color="transparent"];
  t10 [shape=rect label = "if"]; t9 -> t10 [arrowhead="vee" color="transparent"];
  t11 [shape=rect label = "("]; t10 -> t11 [arrowhead="vee" color="transparent"];
  t12 [shape=rect label = "a"]; t11 -> t12 [arrowhead="vee" color="transparent"];
  t13 [shape=rect label = "->"]; t12 -> t13 [arrowhead="vee" color="transparent"];
  t14 [shape=rect label = "x"]; t13 -> t14 [arrowhead="vee" color="transparent"];
  t15 [shape=rect label = ")"]; t14 -> t15 [arrowhead="vee" color="transparent"];
  t16 [shape=rect label = "a"]; t15 -> t16 [arrowhead="vee" color="transparent"];
  t17 [shape=rect label = ";"]; t16 -> t17 [arrowhead="vee" color="transparent"];
  t18 [shape=rect label = "}"]; t17 -> t18 [arrowhead="vee" color="transparent"];
}
}
    )raw";

    checkAst(source, expectedAst);
}

void TestDisambiguator::testCase9()
{
    std::string source = R"raw(
void f() {
    a(x) = 1; /* Must be a variable declaration (in C only). */
}
    )raw";

    std::string expectedAst = R"raw(
digraph AST { ordering=out;
n1 [label="TranslationUnitAST"];
n2 [label="FunctionDefinitionAST"];
n3 [label="SimpleSpecifierAST"];
n4 [label="DeclaratorAST"];
n5 [label="DeclaratorIdAST"];
n6 [label="SimpleNameAST"];
n7 [label="FunctionDeclaratorAST"];
n8 [label="CompoundStatementAST"];
n9 [label="DeclarationStatementAST"];
n10 [label="SimpleDeclarationAST"];
n11 [label="NamedTypeSpecifierAST"];
n12 [label="SimpleNameAST"];
n13 [label="DeclaratorAST"];
n14 [label="NestedDeclaratorAST"];
n15 [label="DeclaratorAST"];
n16 [label="DeclaratorIdAST"];
n17 [label="SimpleNameAST"];
n18 [label="NumericLiteralAST"];
n1 -> n2
n2 -> n3
n3 -> t1
n2 -> n4
n4 -> n5
n5 -> n6
n6 -> t2
n4 -> n7
n7 -> t3
n7 -> t4
n2 -> n8
n8 -> t5
n8 -> n9
n9 -> n10
n10 -> n11
n11 -> n12
n12 -> t6
n10 -> n13
n13 -> n14
n14 -> t7
n14 -> n15
n15 -> n16
n16 -> n17
n17 -> t8
n14 -> t9
n13 -> t10
n13 -> n18
n18 -> t11
n10 -> t12
n8 -> t13
{ rank=same;
  t1 [shape=rect label = "void"];
  t2 [shape=rect label = "f"]; t1 -> t2 [arrowhead="vee" color="transparent"];
  t3 [shape=rect label = "("]; t2 -> t3 [arrowhead="vee" color="transparent"];
  t4 [shape=rect label = ")"]; t3 -> t4 [arrowhead="vee" color="transparent"];
  t5 [shape=rect label = "{"]; t4 -> t5 [arrowhead="vee" color="transparent"];
  t6 [shape=rect label = "a"]; t5 -> t6 [arrowhead="vee" color="transparent"];
  t7 [shape=rect label = "("]; t6 -> t7 [arrowhead="vee" color="transparent"];
  t8 [shape=rect label = "x"]; t7 -> t8 [arrowhead="vee" color="transparent"];
  t9 [shape=rect label = ")"]; t8 -> t9 [arrowhead="vee" color="transparent"];
  t10 [shape=rect label = "="]; t9 -> t10 [arrowhead="vee" color="transparent"];
  t11 [shape=rect label = "1"]; t10 -> t11 [arrowhead="vee" color="transparent"];
  t12 [shape=rect label = ";"]; t11 -> t12 [arrowhead="vee" color="transparent"];
  t13 [shape=rect label = "}"]; t12 -> t13 [arrowhead="vee" color="transparent"];
}
}
    )raw";

    checkAst(source, expectedAst);
}

void TestDisambiguator::testCase10()
{
    std::string source = R"raw(
void f() {
    int(x) = 1;
}
    )raw";

    std::string expectedAst = R"raw(
digraph AST { ordering=out;
n1 [label="TranslationUnitAST"];
n2 [label="FunctionDefinitionAST"];
n3 [label="SimpleSpecifierAST"];
n4 [label="DeclaratorAST"];
n5 [label="DeclaratorIdAST"];
n6 [label="SimpleNameAST"];
n7 [label="FunctionDeclaratorAST"];
n8 [label="CompoundStatementAST"];
n9 [label="DeclarationStatementAST"];
n10 [label="SimpleDeclarationAST"];
n11 [label="SimpleSpecifierAST"];
n12 [label="DeclaratorAST"];
n13 [label="NestedDeclaratorAST"];
n14 [label="DeclaratorAST"];
n15 [label="DeclaratorIdAST"];
n16 [label="SimpleNameAST"];
n17 [label="NumericLiteralAST"];
n1 -> n2
n2 -> n3
n3 -> t1
n2 -> n4
n4 -> n5
n5 -> n6
n6 -> t2
n4 -> n7
n7 -> t3
n7 -> t4
n2 -> n8
n8 -> t5
n8 -> n9
n9 -> n10
n10 -> n11
n11 -> t6
n10 -> n12
n12 -> n13
n13 -> t7
n13 -> n14
n14 -> n15
n15 -> n16
n16 -> t8
n13 -> t9
n12 -> t10
n12 -> n17
n17 -> t11
n10 -> t12
n8 -> t13
{ rank=same;
  t1 [shape=rect label = "void"];
  t2 [shape=rect label = "f"]; t1 -> t2 [arrowhead="vee" color="transparent"];
  t3 [shape=rect label = "("]; t2 -> t3 [arrowhead="vee" color="transparent"];
  t4 [shape=rect label = ")"]; t3 -> t4 [arrowhead="vee" color="transparent"];
  t5 [shape=rect label = "{"]; t4 -> t5 [arrowhead="vee" color="transparent"];
  t6 [shape=rect label = "int"]; t5 -> t6 [arrowhead="vee" color="transparent"];
  t7 [shape=rect label = "("]; t6 -> t7 [arrowhead="vee" color="transparent"];
  t8 [shape=rect label = "x"]; t7 -> t8 [arrowhead="vee" color="transparent"];
  t9 [shape=rect label = ")"]; t8 -> t9 [arrowhead="vee" color="transparent"];
  t10 [shape=rect label = "="]; t9 -> t10 [arrowhead="vee" color="transparent"];
  t11 [shape=rect label = "1"]; t10 -> t11 [arrowhead="vee" color="transparent"];
  t12 [shape=rect label = ";"]; t11 -> t12 [arrowhead="vee" color="transparent"];
  t13 [shape=rect label = "}"]; t12 -> t13 [arrowhead="vee" color="transparent"];
}
}

    )raw";

    checkAst(source, expectedAst);
}

void TestDisambiguator::testCase11()
{
    std::string source = R"raw(
typedef int a;
void f() {
    a(x); /* Variable declaration */
}
    )raw";

    std::string expectedAst = R"raw(
digraph AST { ordering=out;
n1 [label="TranslationUnitAST"];
n2 [label="SimpleDeclarationAST"];
n3 [label="SimpleSpecifierAST"];
n4 [label="SimpleSpecifierAST"];
n5 [label="DeclaratorAST"];
n6 [label="DeclaratorIdAST"];
n7 [label="SimpleNameAST"];
n8 [label="FunctionDefinitionAST"];
n9 [label="SimpleSpecifierAST"];
n10 [label="DeclaratorAST"];
n11 [label="DeclaratorIdAST"];
n12 [label="SimpleNameAST"];
n13 [label="FunctionDeclaratorAST"];
n14 [label="CompoundStatementAST"];
n15 [label="DeclarationStatementAST"];
n16 [label="SimpleDeclarationAST"];
n17 [label="NamedTypeSpecifierAST"];
n18 [label="SimpleNameAST"];
n19 [label="DeclaratorAST"];
n20 [label="NestedDeclaratorAST"];
n21 [label="DeclaratorAST"];
n22 [label="DeclaratorIdAST"];
n23 [label="SimpleNameAST"];
n1 -> n2
n2 -> n3
n3 -> t1
n2 -> n4
n4 -> t2
n2 -> n5
n5 -> n6
n6 -> n7
n7 -> t3
n2 -> t4
n1 -> n8
n8 -> n9
n9 -> t5
n8 -> n10
n10 -> n11
n11 -> n12
n12 -> t6
n10 -> n13
n13 -> t7
n13 -> t8
n8 -> n14
n14 -> t9
n14 -> n15
n15 -> n16
n16 -> n17
n17 -> n18
n18 -> t10
n16 -> n19
n19 -> n20
n20 -> t11
n20 -> n21
n21 -> n22
n22 -> n23
n23 -> t12
n20 -> t13
n16 -> t14
n14 -> t15
{ rank=same;
t1 [shape=rect label = "typedef"];
t2 [shape=rect label = "int"]; t1 -> t2 [arrowhead="vee" color="transparent"];
t3 [shape=rect label = "a"]; t2 -> t3 [arrowhead="vee" color="transparent"];
t4 [shape=rect label = ";"]; t3 -> t4 [arrowhead="vee" color="transparent"];
t5 [shape=rect label = "void"]; t4 -> t5 [arrowhead="vee" color="transparent"];
t6 [shape=rect label = "f"]; t5 -> t6 [arrowhead="vee" color="transparent"];
t7 [shape=rect label = "("]; t6 -> t7 [arrowhead="vee" color="transparent"];
t8 [shape=rect label = ")"]; t7 -> t8 [arrowhead="vee" color="transparent"];
t9 [shape=rect label = "{"]; t8 -> t9 [arrowhead="vee" color="transparent"];
t10 [shape=rect label = "a"]; t9 -> t10 [arrowhead="vee" color="transparent"];
t11 [shape=rect label = "("]; t10 -> t11 [arrowhead="vee" color="transparent"];
t12 [shape=rect label = "x"]; t11 -> t12 [arrowhead="vee" color="transparent"];
t13 [shape=rect label = ")"]; t12 -> t13 [arrowhead="vee" color="transparent"];
t14 [shape=rect label = ";"]; t13 -> t14 [arrowhead="vee" color="transparent"];
t15 [shape=rect label = "}"]; t14 -> t15 [arrowhead="vee" color="transparent"];
}
}
    )raw";

    checkAst(source, expectedAst);
}

void TestDisambiguator::testCase12()
{
    // This one is a function declaration with implicit return type. But we
    // don't handle it yet - some work in the parser is required.

    std::string source = R"raw(
typedef int x;
void f() {
    a(x); /* Function declaration. */
}
    )raw";

    std::string expectedAst = R"raw(
    )raw";

    checkAst(source, expectedAst);
}

void TestDisambiguator::testCase13()
{
    std::string source = R"raw(
void f();
void g() {
    f(x); /* Function call, of course. */
}
    )raw";

    std::string expectedAst = R"raw(
digraph AST { ordering=out;
n1 [label="TranslationUnitAST"];
n2 [label="SimpleDeclarationAST"];
n3 [label="SimpleSpecifierAST"];
n4 [label="DeclaratorAST"];
n5 [label="DeclaratorIdAST"];
n6 [label="SimpleNameAST"];
n7 [label="FunctionDeclaratorAST"];
n8 [label="FunctionDefinitionAST"];
n9 [label="SimpleSpecifierAST"];
n10 [label="DeclaratorAST"];
n11 [label="DeclaratorIdAST"];
n12 [label="SimpleNameAST"];
n13 [label="FunctionDeclaratorAST"];
n14 [label="CompoundStatementAST"];
n15 [label="ExpressionStatementAST"];
n16 [label="CallAST"];
n17 [label="IdExpressionAST"];
n18 [label="SimpleNameAST"];
n19 [label="IdExpressionAST"];
n20 [label="SimpleNameAST"];
n1 -> n2
n2 -> n3
n3 -> t1
n2 -> n4
n4 -> n5
n5 -> n6
n6 -> t2
n4 -> n7
n7 -> t3
n7 -> t4
n2 -> t5
n1 -> n8
n8 -> n9
n9 -> t6
n8 -> n10
n10 -> n11
n11 -> n12
n12 -> t7
n10 -> n13
n13 -> t8
n13 -> t9
n8 -> n14
n14 -> t10
n14 -> n15
n15 -> n16
n16 -> n17
n17 -> n18
n18 -> t11
n16 -> t12
n16 -> n19
n19 -> n20
n20 -> t13
n16 -> t14
n15 -> t15
n14 -> t16
{ rank=same;
  t1 [shape=rect label = "void"];
  t2 [shape=rect label = "f"]; t1 -> t2 [arrowhead="vee" color="transparent"];
  t3 [shape=rect label = "("]; t2 -> t3 [arrowhead="vee" color="transparent"];
  t4 [shape=rect label = ")"]; t3 -> t4 [arrowhead="vee" color="transparent"];
  t5 [shape=rect label = ";"]; t4 -> t5 [arrowhead="vee" color="transparent"];
  t6 [shape=rect label = "void"]; t5 -> t6 [arrowhead="vee" color="transparent"];
  t7 [shape=rect label = "g"]; t6 -> t7 [arrowhead="vee" color="transparent"];
  t8 [shape=rect label = "("]; t7 -> t8 [arrowhead="vee" color="transparent"];
  t9 [shape=rect label = ")"]; t8 -> t9 [arrowhead="vee" color="transparent"];
  t10 [shape=rect label = "{"]; t9 -> t10 [arrowhead="vee" color="transparent"];
  t11 [shape=rect label = "f"]; t10 -> t11 [arrowhead="vee" color="transparent"];
  t12 [shape=rect label = "("]; t11 -> t12 [arrowhead="vee" color="transparent"];
  t13 [shape=rect label = "x"]; t12 -> t13 [arrowhead="vee" color="transparent"];
  t14 [shape=rect label = ")"]; t13 -> t14 [arrowhead="vee" color="transparent"];
  t15 [shape=rect label = ";"]; t14 -> t15 [arrowhead="vee" color="transparent"];
  t16 [shape=rect label = "}"]; t15 -> t16 [arrowhead="vee" color="transparent"];
}
}
    )raw";

    checkAst(source, expectedAst);
}

void TestDisambiguator::testCase14()
{
    std::string source = R"raw(
void g() {
    int x;
    f(x); /* Function call */
}
    )raw";

    std::string expectedAst = R"raw(
digraph AST { ordering=out;
n1 [label="TranslationUnitAST"];
n2 [label="FunctionDefinitionAST"];
n3 [label="SimpleSpecifierAST"];
n4 [label="DeclaratorAST"];
n5 [label="DeclaratorIdAST"];
n6 [label="SimpleNameAST"];
n7 [label="FunctionDeclaratorAST"];
n8 [label="CompoundStatementAST"];
n9 [label="DeclarationStatementAST"];
n10 [label="SimpleDeclarationAST"];
n11 [label="SimpleSpecifierAST"];
n12 [label="DeclaratorAST"];
n13 [label="DeclaratorIdAST"];
n14 [label="SimpleNameAST"];
n15 [label="ExpressionStatementAST"];
n16 [label="CallAST"];
n17 [label="IdExpressionAST"];
n18 [label="SimpleNameAST"];
n19 [label="IdExpressionAST"];
n20 [label="SimpleNameAST"];
n1 -> n2
n2 -> n3
n3 -> t1
n2 -> n4
n4 -> n5
n5 -> n6
n6 -> t2
n4 -> n7
n7 -> t3
n7 -> t4
n2 -> n8
n8 -> t5
n8 -> n9
n9 -> n10
n10 -> n11
n11 -> t6
n10 -> n12
n12 -> n13
n13 -> n14
n14 -> t7
n10 -> t8
n8 -> n15
n15 -> n16
n16 -> n17
n17 -> n18
n18 -> t9
n16 -> t10
n16 -> n19
n19 -> n20
n20 -> t11
n16 -> t12
n15 -> t13
n8 -> t14
{ rank=same;
  t1 [shape=rect label = "void"];
  t2 [shape=rect label = "g"]; t1 -> t2 [arrowhead="vee" color="transparent"];
  t3 [shape=rect label = "("]; t2 -> t3 [arrowhead="vee" color="transparent"];
  t4 [shape=rect label = ")"]; t3 -> t4 [arrowhead="vee" color="transparent"];
  t5 [shape=rect label = "{"]; t4 -> t5 [arrowhead="vee" color="transparent"];
  t6 [shape=rect label = "int"]; t5 -> t6 [arrowhead="vee" color="transparent"];
  t7 [shape=rect label = "x"]; t6 -> t7 [arrowhead="vee" color="transparent"];
  t8 [shape=rect label = ";"]; t7 -> t8 [arrowhead="vee" color="transparent"];
  t9 [shape=rect label = "f"]; t8 -> t9 [arrowhead="vee" color="transparent"];
  t10 [shape=rect label = "("]; t9 -> t10 [arrowhead="vee" color="transparent"];
  t11 [shape=rect label = "x"]; t10 -> t11 [arrowhead="vee" color="transparent"];
  t12 [shape=rect label = ")"]; t11 -> t12 [arrowhead="vee" color="transparent"];
  t13 [shape=rect label = ";"]; t12 -> t13 [arrowhead="vee" color="transparent"];
  t14 [shape=rect label = "}"]; t13 -> t14 [arrowhead="vee" color="transparent"];
}
}
    )raw";

    checkAst(source, expectedAst);
}

void TestDisambiguator::testCase15()
{
  std::string source = R"raw(
int Bubble(int i){}

void main()
{
 for (int i = 0; i < 100; i++) Bubble(i);
}
    )raw";

    std::string expectedAst = R"raw(
digraph AST { ordering=out;
n1 [label="TranslationUnitAST"];
n2 [label="FunctionDefinitionAST"];
n3 [label="SimpleSpecifierAST"];
n4 [label="DeclaratorAST"];
n5 [label="DeclaratorIdAST"];
n6 [label="SimpleNameAST"];
n7 [label="FunctionDeclaratorAST"];
n8 [label="ParameterDeclarationClauseAST"];
n9 [label="ParameterDeclarationAST"];
n10 [label="SimpleSpecifierAST"];
n11 [label="DeclaratorAST"];
n12 [label="DeclaratorIdAST"];
n13 [label="SimpleNameAST"];
n14 [label="CompoundStatementAST"];
n15 [label="FunctionDefinitionAST"];
n16 [label="SimpleSpecifierAST"];
n17 [label="DeclaratorAST"];
n18 [label="DeclaratorIdAST"];
n19 [label="SimpleNameAST"];
n20 [label="FunctionDeclaratorAST"];
n21 [label="CompoundStatementAST"];
n22 [label="ForStatementAST"];
n23 [label="DeclarationStatementAST"];
n24 [label="SimpleDeclarationAST"];
n25 [label="SimpleSpecifierAST"];
n26 [label="DeclaratorAST"];
n27 [label="DeclaratorIdAST"];
n28 [label="SimpleNameAST"];
n29 [label="NumericLiteralAST"];
n30 [label="BinaryExpressionAST"];
n31 [label="IdExpressionAST"];
n32 [label="SimpleNameAST"];
n33 [label="NumericLiteralAST"];
n34 [label="PostIncrDecrAST"];
n35 [label="IdExpressionAST"];
n36 [label="SimpleNameAST"];
n37 [label="ExpressionStatementAST"];
n38 [label="CallAST"];
n39 [label="IdExpressionAST"];
n40 [label="SimpleNameAST"];
n41 [label="IdExpressionAST"];
n42 [label="SimpleNameAST"];
n1 -> n2
n2 -> n3
n3 -> t1
n2 -> n4
n4 -> n5
n5 -> n6
n6 -> t2
n4 -> n7
n7 -> t3
n7 -> n8
n8 -> n9
n9 -> n10
n10 -> t4
n9 -> n11
n11 -> n12
n12 -> n13
n13 -> t5
n7 -> t6
n2 -> n14
n14 -> t7
n14 -> t8
n1 -> n15
n15 -> n16
n16 -> t9
n15 -> n17
n17 -> n18
n18 -> n19
n19 -> t10
n17 -> n20
n20 -> t11
n20 -> t12
n15 -> n21
n21 -> t13
n21 -> n22
n22 -> t14
n22 -> t15
n22 -> n23
n23 -> n24
n24 -> n25
n25 -> t16
n24 -> n26
n26 -> n27
n27 -> n28
n28 -> t17
n26 -> t18
n26 -> n29
n29 -> t19
n24 -> t20
n22 -> n30
n30 -> n31
n31 -> n32
n32 -> t21
n30 -> t22
n30 -> n33
n33 -> t23
n22 -> t24
n22 -> n34
n34 -> n35
n35 -> n36
n36 -> t25
n34 -> t26
n22 -> t27
n22 -> n37
n37 -> n38
n38 -> n39
n39 -> n40
n40 -> t28
n38 -> t29
n38 -> n41
n41 -> n42
n42 -> t30
n38 -> t31
n37 -> t32
n21 -> t33
{ rank=same;
  t1 [shape=rect label = "int"];
  t2 [shape=rect label = "Bubble"]; t1 -> t2 [arrowhead="vee" color="transparent"];
  t3 [shape=rect label = "("]; t2 -> t3 [arrowhead="vee" color="transparent"];
  t4 [shape=rect label = "int"]; t3 -> t4 [arrowhead="vee" color="transparent"];
  t5 [shape=rect label = "i"]; t4 -> t5 [arrowhead="vee" color="transparent"];
  t6 [shape=rect label = ")"]; t5 -> t6 [arrowhead="vee" color="transparent"];
  t7 [shape=rect label = "{"]; t6 -> t7 [arrowhead="vee" color="transparent"];
  t8 [shape=rect label = "}"]; t7 -> t8 [arrowhead="vee" color="transparent"];
  t9 [shape=rect label = "void"]; t8 -> t9 [arrowhead="vee" color="transparent"];
  t10 [shape=rect label = "main"]; t9 -> t10 [arrowhead="vee" color="transparent"];
  t11 [shape=rect label = "("]; t10 -> t11 [arrowhead="vee" color="transparent"];
  t12 [shape=rect label = ")"]; t11 -> t12 [arrowhead="vee" color="transparent"];
  t13 [shape=rect label = "{"]; t12 -> t13 [arrowhead="vee" color="transparent"];
  t14 [shape=rect label = "for"]; t13 -> t14 [arrowhead="vee" color="transparent"];
  t15 [shape=rect label = "("]; t14 -> t15 [arrowhead="vee" color="transparent"];
  t16 [shape=rect label = "int"]; t15 -> t16 [arrowhead="vee" color="transparent"];
  t17 [shape=rect label = "i"]; t16 -> t17 [arrowhead="vee" color="transparent"];
  t18 [shape=rect label = "="]; t17 -> t18 [arrowhead="vee" color="transparent"];
  t19 [shape=rect label = "0"]; t18 -> t19 [arrowhead="vee" color="transparent"];
  t20 [shape=rect label = ";"]; t19 -> t20 [arrowhead="vee" color="transparent"];
  t21 [shape=rect label = "i"]; t20 -> t21 [arrowhead="vee" color="transparent"];
  t22 [shape=rect label = "<"]; t21 -> t22 [arrowhead="vee" color="transparent"];
  t23 [shape=rect label = "100"]; t22 -> t23 [arrowhead="vee" color="transparent"];
  t24 [shape=rect label = ";"]; t23 -> t24 [arrowhead="vee" color="transparent"];
  t25 [shape=rect label = "i"]; t24 -> t25 [arrowhead="vee" color="transparent"];
  t26 [shape=rect label = "++"]; t25 -> t26 [arrowhead="vee" color="transparent"];
  t27 [shape=rect label = ")"]; t26 -> t27 [arrowhead="vee" color="transparent"];
  t28 [shape=rect label = "Bubble"]; t27 -> t28 [arrowhead="vee" color="transparent"];
  t29 [shape=rect label = "("]; t28 -> t29 [arrowhead="vee" color="transparent"];
  t30 [shape=rect label = "i"]; t29 -> t30 [arrowhead="vee" color="transparent"];
  t31 [shape=rect label = ")"]; t30 -> t31 [arrowhead="vee" color="transparent"];
  t32 [shape=rect label = ";"]; t31 -> t32 [arrowhead="vee" color="transparent"];
  t33 [shape=rect label = "}"]; t32 -> t33 [arrowhead="vee" color="transparent"];
}
}
    )raw";

    checkAst(source, expectedAst);
}

void TestDisambiguator::testCase16()
{
    std::string source = R"raw(
void f() {
    unsigned b;
    (a)-b; // Binary expression.
    a+b;
}
    )raw";

    // TODO: Implement this disambiguation.
}

void TestDisambiguator::testCase17()
{
    std::string source = R"raw(
void f() {
    unsigned b;
    (a)-b; // Coercion.
    a c;
}
    )raw";

    // TODO: Implement this disambiguation.
}

void TestDisambiguator::testCase18()
{
    std::string source = R"raw(
void f() {
    x * y, * z, w; // Must be declaration.
}
    )raw";

    std::string expectedAst = R"raw(
digraph AST { ordering=out;
n1 [label="TranslationUnitAST"];
n2 [label="FunctionDefinitionAST"];
n3 [label="SimpleSpecifierAST"];
n4 [label="DeclaratorAST"];
n5 [label="DeclaratorIdAST"];
n6 [label="SimpleNameAST"];
n7 [label="FunctionDeclaratorAST"];
n8 [label="CompoundStatementAST"];
n9 [label="DeclarationStatementAST"];
n10 [label="SimpleDeclarationAST"];
n11 [label="NamedTypeSpecifierAST"];
n12 [label="SimpleNameAST"];
n13 [label="DeclaratorAST"];
n14 [label="PointerAST"];
n15 [label="DeclaratorIdAST"];
n16 [label="SimpleNameAST"];
n17 [label="DeclaratorAST"];
n18 [label="PointerAST"];
n19 [label="DeclaratorIdAST"];
n20 [label="SimpleNameAST"];
n21 [label="DeclaratorAST"];
n22 [label="DeclaratorIdAST"];
n23 [label="SimpleNameAST"];
n1 -> n2
n2 -> n3
n3 -> t1
n2 -> n4
n4 -> n5
n5 -> n6
n6 -> t2
n4 -> n7
n7 -> t3
n7 -> t4
n2 -> n8
n8 -> t5
n8 -> n9
n9 -> n10
n10 -> n11
n11 -> n12
n12 -> t6
n10 -> n13
n13 -> n14
n14 -> t7
n13 -> n15
n15 -> n16
n16 -> t8
n10 -> n17
n17 -> n18
n18 -> t10
n17 -> n19
n19 -> n20
n20 -> t11
n10 -> n21
n21 -> n22
n22 -> n23
n23 -> t13
n10 -> t14
n8 -> t15
{ rank=same;
t1 [shape=rect label = "void"];
t2 [shape=rect label = "f"]; t1 -> t2 [arrowhead="vee" color="transparent"];
t3 [shape=rect label = "("]; t2 -> t3 [arrowhead="vee" color="transparent"];
t4 [shape=rect label = ")"]; t3 -> t4 [arrowhead="vee" color="transparent"];
t5 [shape=rect label = "{"]; t4 -> t5 [arrowhead="vee" color="transparent"];
t6 [shape=rect label = "x"]; t5 -> t6 [arrowhead="vee" color="transparent"];
t7 [shape=rect label = "*"]; t6 -> t7 [arrowhead="vee" color="transparent"];
t8 [shape=rect label = "y"]; t7 -> t8 [arrowhead="vee" color="transparent"];
t9 [shape=rect label = ","]; t8 -> t9 [arrowhead="vee" color="transparent"];
t10 [shape=rect label = "*"]; t9 -> t10 [arrowhead="vee" color="transparent"];
t11 [shape=rect label = "z"]; t10 -> t11 [arrowhead="vee" color="transparent"];
t12 [shape=rect label = ","]; t11 -> t12 [arrowhead="vee" color="transparent"];
t13 [shape=rect label = "w"]; t12 -> t13 [arrowhead="vee" color="transparent"];
t14 [shape=rect label = ";"]; t13 -> t14 [arrowhead="vee" color="transparent"];
t15 [shape=rect label = "}"]; t14 -> t15 [arrowhead="vee" color="transparent"];
}
}
    )raw";

    checkAst(source, expectedAst);
}

void TestDisambiguator::testCase19()
{
    std::string source = R"raw(
void f() {
    x * y, z, * w; // Must be declaration.
}
    )raw";

    std::string expectedAst = R"raw(
digraph AST { ordering=out;
n1 [label="TranslationUnitAST"];
n2 [label="FunctionDefinitionAST"];
n3 [label="SimpleSpecifierAST"];
n4 [label="DeclaratorAST"];
n5 [label="DeclaratorIdAST"];
n6 [label="SimpleNameAST"];
n7 [label="FunctionDeclaratorAST"];
n8 [label="CompoundStatementAST"];
n9 [label="DeclarationStatementAST"];
n10 [label="SimpleDeclarationAST"];
n11 [label="NamedTypeSpecifierAST"];
n12 [label="SimpleNameAST"];
n13 [label="DeclaratorAST"];
n14 [label="PointerAST"];
n15 [label="DeclaratorIdAST"];
n16 [label="SimpleNameAST"];
n17 [label="DeclaratorAST"];
n18 [label="DeclaratorIdAST"];
n19 [label="SimpleNameAST"];
n20 [label="DeclaratorAST"];
n21 [label="PointerAST"];
n22 [label="DeclaratorIdAST"];
n23 [label="SimpleNameAST"];
n1 -> n2
n2 -> n3
n3 -> t1
n2 -> n4
n4 -> n5
n5 -> n6
n6 -> t2
n4 -> n7
n7 -> t3
n7 -> t4
n2 -> n8
n8 -> t5
n8 -> n9
n9 -> n10
n10 -> n11
n11 -> n12
n12 -> t6
n10 -> n13
n13 -> n14
n14 -> t7
n13 -> n15
n15 -> n16
n16 -> t8
n10 -> n17
n17 -> n18
n18 -> n19
n19 -> t10
n10 -> n20
n20 -> n21
n21 -> t12
n20 -> n22
n22 -> n23
n23 -> t13
n10 -> t14
n8 -> t15
{ rank=same;
t1 [shape=rect label = "void"];
t2 [shape=rect label = "f"]; t1 -> t2 [arrowhead="vee" color="transparent"];
t3 [shape=rect label = "("]; t2 -> t3 [arrowhead="vee" color="transparent"];
t4 [shape=rect label = ")"]; t3 -> t4 [arrowhead="vee" color="transparent"];
t5 [shape=rect label = "{"]; t4 -> t5 [arrowhead="vee" color="transparent"];
t6 [shape=rect label = "x"]; t5 -> t6 [arrowhead="vee" color="transparent"];
t7 [shape=rect label = "*"]; t6 -> t7 [arrowhead="vee" color="transparent"];
t8 [shape=rect label = "y"]; t7 -> t8 [arrowhead="vee" color="transparent"];
t9 [shape=rect label = ","]; t8 -> t9 [arrowhead="vee" color="transparent"];
t10 [shape=rect label = "z"]; t9 -> t10 [arrowhead="vee" color="transparent"];
t11 [shape=rect label = ","]; t10 -> t11 [arrowhead="vee" color="transparent"];
t12 [shape=rect label = "*"]; t11 -> t12 [arrowhead="vee" color="transparent"];
t13 [shape=rect label = "w"]; t12 -> t13 [arrowhead="vee" color="transparent"];
t14 [shape=rect label = ";"]; t13 -> t14 [arrowhead="vee" color="transparent"];
t15 [shape=rect label = "}"]; t14 -> t15 [arrowhead="vee" color="transparent"];
}
}

    )raw";

    checkAst(source, expectedAst);
}

void TestDisambiguator::testCase20()
{
    std::string source = R"raw(
void f() {
    x * y, z, w; // Ambiguous: declarations or three expressions.
}
    )raw";

    std::string expectedAst = R"raw(
                              digraph AST { ordering=out;
                              n1 [label="TranslationUnitAST"];
                              n2 [label="FunctionDefinitionAST"];
                              n3 [label="SimpleSpecifierAST"];
                              n4 [label="DeclaratorAST"];
                              n5 [label="DeclaratorIdAST"];
                              n6 [label="SimpleNameAST"];
                              n7 [label="FunctionDeclaratorAST"];
                              n8 [label="CompoundStatementAST"];
                              n9 [label="AmbiguousStatementAST"];
                              n10 [label="DeclarationStatementAST"];
                              n11 [label="SimpleDeclarationAST"];
                              n12 [label="NamedTypeSpecifierAST"];
                              n13 [label="SimpleNameAST"];
                              n14 [label="DeclaratorAST"];
                              n15 [label="PointerAST"];
                              n16 [label="DeclaratorIdAST"];
                              n17 [label="SimpleNameAST"];
                              n18 [label="DeclaratorAST"];
                              n19 [label="DeclaratorIdAST"];
                              n20 [label="SimpleNameAST"];
                              n21 [label="DeclaratorAST"];
                              n22 [label="DeclaratorIdAST"];
                              n23 [label="SimpleNameAST"];
                              n24 [label="ExpressionStatementAST"];
                              n25 [label="BinaryExpressionAST"];
                              n26 [label="BinaryExpressionAST"];
                              n27 [label="BinaryExpressionAST"];
                              n28 [label="IdExpressionAST"];
                              n29 [label="SimpleNameAST"];
                              n30 [label="IdExpressionAST"];
                              n31 [label="SimpleNameAST"];
                              n32 [label="IdExpressionAST"];
                              n33 [label="SimpleNameAST"];
                              n34 [label="IdExpressionAST"];
                              n35 [label="SimpleNameAST"];
                              n1 -> n2
                              n2 -> n3
                              n3 -> t1
                              n2 -> n4
                              n4 -> n5
                              n5 -> n6
                              n6 -> t2
                              n4 -> n7
                              n7 -> t3
                              n7 -> t4
                              n2 -> n8
                              n8 -> t5
                              n8 -> n9
                              n9 -> n10
                              n10 -> n11
                              n11 -> n12
                              n12 -> n13
                              n13 -> t6
                              n11 -> n14
                              n14 -> n15
                              n15 -> t7
                              n14 -> n16
                              n16 -> n17
                              n17 -> t8
                              n11 -> n18
                              n18 -> n19
                              n19 -> n20
                              n20 -> t10
                              n11 -> n21
                              n21 -> n22
                              n22 -> n23
                              n23 -> t12
                              n11 -> t13
                              n9 -> n24
                              n24 -> n25
                              n25 -> n26
                              n26 -> n27
                              n27 -> n28
                              n28 -> n29
                              n29 -> t6
                              n27 -> t7
                              n27 -> n30
                              n30 -> n31
                              n31 -> t8
                              n26 -> t9
                              n26 -> n32
                              n32 -> n33
                              n33 -> t10
                              n25 -> t11
                              n25 -> n34
                              n34 -> n35
                              n35 -> t12
                              n24 -> t13
                              n8 -> t14
                              { rank=same;
                                t1 [shape=rect label = "void"];
                                t2 [shape=rect label = "f"]; t1 -> t2 [arrowhead="vee" color="transparent"];
                                t3 [shape=rect label = "("]; t2 -> t3 [arrowhead="vee" color="transparent"];
                                t4 [shape=rect label = ")"]; t3 -> t4 [arrowhead="vee" color="transparent"];
                                t5 [shape=rect label = "{"]; t4 -> t5 [arrowhead="vee" color="transparent"];
                                t6 [shape=rect label = "x"]; t5 -> t6 [arrowhead="vee" color="transparent"];
                                t7 [shape=rect label = "*"]; t6 -> t7 [arrowhead="vee" color="transparent"];
                                t8 [shape=rect label = "y"]; t7 -> t8 [arrowhead="vee" color="transparent"];
                                t9 [shape=rect label = ","]; t8 -> t9 [arrowhead="vee" color="transparent"];
                                t10 [shape=rect label = "z"]; t9 -> t10 [arrowhead="vee" color="transparent"];
                                t11 [shape=rect label = ","]; t10 -> t11 [arrowhead="vee" color="transparent"];
                                t12 [shape=rect label = "w"]; t11 -> t12 [arrowhead="vee" color="transparent"];
                                t13 [shape=rect label = ";"]; t12 -> t13 [arrowhead="vee" color="transparent"];
                                t14 [shape=rect label = "}"]; t13 -> t14 [arrowhead="vee" color="transparent"];
                              }
                              }
    )raw";

    checkAst(source, expectedAst);
}

void TestDisambiguator::testCase21()
{
    // Without heuristics, it's ambiguous.
    std::string source = R"raw(
void f() {
    A * x;
    A * y;
}
    )raw";

    std::string expectedAst = R"raw(
  digraph AST { ordering=out;
  n1 [label="TranslationUnitAST"];
  n2 [label="FunctionDefinitionAST"];
  n3 [label="SimpleSpecifierAST"];
  n4 [label="DeclaratorAST"];
  n5 [label="DeclaratorIdAST"];
  n6 [label="SimpleNameAST"];
  n7 [label="FunctionDeclaratorAST"];
  n8 [label="CompoundStatementAST"];
  n9 [label="AmbiguousStatementAST"];
  n10 [label="DeclarationStatementAST"];
  n11 [label="SimpleDeclarationAST"];
  n12 [label="NamedTypeSpecifierAST"];
  n13 [label="SimpleNameAST"];
  n14 [label="DeclaratorAST"];
  n15 [label="PointerAST"];
  n16 [label="DeclaratorIdAST"];
  n17 [label="SimpleNameAST"];
  n18 [label="ExpressionStatementAST"];
  n19 [label="BinaryExpressionAST"];
  n20 [label="IdExpressionAST"];
  n21 [label="SimpleNameAST"];
  n22 [label="IdExpressionAST"];
  n23 [label="SimpleNameAST"];
  n24 [label="AmbiguousStatementAST"];
  n25 [label="DeclarationStatementAST"];
  n26 [label="SimpleDeclarationAST"];
  n27 [label="NamedTypeSpecifierAST"];
  n28 [label="SimpleNameAST"];
  n29 [label="DeclaratorAST"];
  n30 [label="PointerAST"];
  n31 [label="DeclaratorIdAST"];
  n32 [label="SimpleNameAST"];
  n33 [label="ExpressionStatementAST"];
  n34 [label="BinaryExpressionAST"];
  n35 [label="IdExpressionAST"];
  n36 [label="SimpleNameAST"];
  n37 [label="IdExpressionAST"];
  n38 [label="SimpleNameAST"];
  n1 -> n2
  n2 -> n3
  n3 -> t1
  n2 -> n4
  n4 -> n5
  n5 -> n6
  n6 -> t2
  n4 -> n7
  n7 -> t3
  n7 -> t4
  n2 -> n8
  n8 -> t5
  n8 -> n9
  n9 -> n10
  n10 -> n11
  n11 -> n12
  n12 -> n13
  n13 -> t6
  n11 -> n14
  n14 -> n15
  n15 -> t7
  n14 -> n16
  n16 -> n17
  n17 -> t8
  n11 -> t9
  n9 -> n18
  n18 -> n19
  n19 -> n20
  n20 -> n21
  n21 -> t6
  n19 -> t7
  n19 -> n22
  n22 -> n23
  n23 -> t8
  n18 -> t9
  n8 -> n24
  n24 -> n25
  n25 -> n26
  n26 -> n27
  n27 -> n28
  n28 -> t10
  n26 -> n29
  n29 -> n30
  n30 -> t11
  n29 -> n31
  n31 -> n32
  n32 -> t12
  n26 -> t13
  n24 -> n33
  n33 -> n34
  n34 -> n35
  n35 -> n36
  n36 -> t10
  n34 -> t11
  n34 -> n37
  n37 -> n38
  n38 -> t12
  n33 -> t13
  n8 -> t14
  { rank=same;
    t1 [shape=rect label = "void"];
    t2 [shape=rect label = "f"]; t1 -> t2 [arrowhead="vee" color="transparent"];
    t3 [shape=rect label = "("]; t2 -> t3 [arrowhead="vee" color="transparent"];
    t4 [shape=rect label = ")"]; t3 -> t4 [arrowhead="vee" color="transparent"];
    t5 [shape=rect label = "{"]; t4 -> t5 [arrowhead="vee" color="transparent"];
    t6 [shape=rect label = "A"]; t5 -> t6 [arrowhead="vee" color="transparent"];
    t7 [shape=rect label = "*"]; t6 -> t7 [arrowhead="vee" color="transparent"];
    t8 [shape=rect label = "x"]; t7 -> t8 [arrowhead="vee" color="transparent"];
    t9 [shape=rect label = ";"]; t8 -> t9 [arrowhead="vee" color="transparent"];
    t10 [shape=rect label = "A"]; t9 -> t10 [arrowhead="vee" color="transparent"];
    t11 [shape=rect label = "*"]; t10 -> t11 [arrowhead="vee" color="transparent"];
    t12 [shape=rect label = "y"]; t11 -> t12 [arrowhead="vee" color="transparent"];
    t13 [shape=rect label = ";"]; t12 -> t13 [arrowhead="vee" color="transparent"];
    t14 [shape=rect label = "}"]; t13 -> t14 [arrowhead="vee" color="transparent"];
  }
  }
)raw";

    checkAst(source, expectedAst);
}

void TestDisambiguator::testCase22()
{
    // With heuristics, those are pointers.
    std::string source = R"raw(
void f() {
    A * x;
    A * y;
}
    )raw";

    std::string expectedAst = R"raw(
digraph AST { ordering=out;
n1 [label="TranslationUnitAST"];
n2 [label="FunctionDefinitionAST"];
n3 [label="SimpleSpecifierAST"];
n4 [label="DeclaratorAST"];
n5 [label="DeclaratorIdAST"];
n6 [label="SimpleNameAST"];
n7 [label="FunctionDeclaratorAST"];
n8 [label="CompoundStatementAST"];
n9 [label="DeclarationStatementAST"];
n10 [label="SimpleDeclarationAST"];
n11 [label="NamedTypeSpecifierAST"];
n12 [label="SimpleNameAST"];
n13 [label="DeclaratorAST"];
n14 [label="PointerAST"];
n15 [label="DeclaratorIdAST"];
n16 [label="SimpleNameAST"];
n17 [label="DeclarationStatementAST"];
n18 [label="SimpleDeclarationAST"];
n19 [label="NamedTypeSpecifierAST"];
n20 [label="SimpleNameAST"];
n21 [label="DeclaratorAST"];
n22 [label="PointerAST"];
n23 [label="DeclaratorIdAST"];
n24 [label="SimpleNameAST"];
n1 -> n2
n2 -> n3
n3 -> t1
n2 -> n4
n4 -> n5
n5 -> n6
n6 -> t2
n4 -> n7
n7 -> t3
n7 -> t4
n2 -> n8
n8 -> t5
n8 -> n9
n9 -> n10
n10 -> n11
n11 -> n12
n12 -> t6
n10 -> n13
n13 -> n14
n14 -> t7
n13 -> n15
n15 -> n16
n16 -> t8
n10 -> t9
n8 -> n17
n17 -> n18
n18 -> n19
n19 -> n20
n20 -> t10
n18 -> n21
n21 -> n22
n22 -> t11
n21 -> n23
n23 -> n24
n24 -> t12
n18 -> t13
n8 -> t14
{ rank=same;
t1 [shape=rect label = "void"];
t2 [shape=rect label = "f"]; t1 -> t2 [arrowhead="vee" color="transparent"];
t3 [shape=rect label = "("]; t2 -> t3 [arrowhead="vee" color="transparent"];
t4 [shape=rect label = ")"]; t3 -> t4 [arrowhead="vee" color="transparent"];
t5 [shape=rect label = "{"]; t4 -> t5 [arrowhead="vee" color="transparent"];
t6 [shape=rect label = "A"]; t5 -> t6 [arrowhead="vee" color="transparent"];
t7 [shape=rect label = "*"]; t6 -> t7 [arrowhead="vee" color="transparent"];
t8 [shape=rect label = "x"]; t7 -> t8 [arrowhead="vee" color="transparent"];
t9 [shape=rect label = ";"]; t8 -> t9 [arrowhead="vee" color="transparent"];
t10 [shape=rect label = "A"]; t9 -> t10 [arrowhead="vee" color="transparent"];
t11 [shape=rect label = "*"]; t10 -> t11 [arrowhead="vee" color="transparent"];
t12 [shape=rect label = "y"]; t11 -> t12 [arrowhead="vee" color="transparent"];
t13 [shape=rect label = ";"]; t12 -> t13 [arrowhead="vee" color="transparent"];
t14 [shape=rect label = "}"]; t13 -> t14 [arrowhead="vee" color="transparent"];
}
}
    )raw";

    checkAst(source, expectedAst, false);
}


void TestDisambiguator::testCase23()
{
    std::string source = R"raw(
void f() {
    x z;
    x * y;
}
    )raw";

    std::string expectedAst = R"raw(
digraph AST { ordering=out;
n1 [label="TranslationUnitAST"];
n2 [label="FunctionDefinitionAST"];
n3 [label="SimpleSpecifierAST"];
n4 [label="DeclaratorAST"];
n5 [label="DeclaratorIdAST"];
n6 [label="SimpleNameAST"];
n7 [label="FunctionDeclaratorAST"];
n8 [label="CompoundStatementAST"];
n9 [label="DeclarationStatementAST"];
n10 [label="SimpleDeclarationAST"];
n11 [label="NamedTypeSpecifierAST"];
n12 [label="SimpleNameAST"];
n13 [label="DeclaratorAST"];
n14 [label="DeclaratorIdAST"];
n15 [label="SimpleNameAST"];
n16 [label="DeclarationStatementAST"];
n17 [label="SimpleDeclarationAST"];
n18 [label="NamedTypeSpecifierAST"];
n19 [label="SimpleNameAST"];
n20 [label="DeclaratorAST"];
n21 [label="PointerAST"];
n22 [label="DeclaratorIdAST"];
n23 [label="SimpleNameAST"];
n1 -> n2
n2 -> n3
n3 -> t1
n2 -> n4
n4 -> n5
n5 -> n6
n6 -> t2
n4 -> n7
n7 -> t3
n7 -> t4
n2 -> n8
n8 -> t5
n8 -> n9
n9 -> n10
n10 -> n11
n11 -> n12
n12 -> t6
n10 -> n13
n13 -> n14
n14 -> n15
n15 -> t7
n10 -> t8
n8 -> n16
n16 -> n17
n17 -> n18
n18 -> n19
n19 -> t9
n17 -> n20
n20 -> n21
n21 -> t10
n20 -> n22
n22 -> n23
n23 -> t11
n17 -> t12
n8 -> t13
{ rank=same;
t1 [shape=rect label = "void"];
t2 [shape=rect label = "f"]; t1 -> t2 [arrowhead="vee" color="transparent"];
t3 [shape=rect label = "("]; t2 -> t3 [arrowhead="vee" color="transparent"];
t4 [shape=rect label = ")"]; t3 -> t4 [arrowhead="vee" color="transparent"];
t5 [shape=rect label = "{"]; t4 -> t5 [arrowhead="vee" color="transparent"];
t6 [shape=rect label = "x"]; t5 -> t6 [arrowhead="vee" color="transparent"];
t7 [shape=rect label = "z"]; t6 -> t7 [arrowhead="vee" color="transparent"];
t8 [shape=rect label = ";"]; t7 -> t8 [arrowhead="vee" color="transparent"];
t9 [shape=rect label = "x"]; t8 -> t9 [arrowhead="vee" color="transparent"];
t10 [shape=rect label = "*"]; t9 -> t10 [arrowhead="vee" color="transparent"];
t11 [shape=rect label = "y"]; t10 -> t11 [arrowhead="vee" color="transparent"];
t12 [shape=rect label = ";"]; t11 -> t12 [arrowhead="vee" color="transparent"];
t13 [shape=rect label = "}"]; t12 -> t13 [arrowhead="vee" color="transparent"];
}
}
    )raw";

    checkAst(source, expectedAst, false);
}

void TestDisambiguator::testCase24()
{
    std::string source = R"raw(
void g() {
    x * y;
    x z;
}
    )raw";

    std::string expectedAst = R"raw(
digraph AST { ordering=out;
n1 [label="TranslationUnitAST"];
n2 [label="FunctionDefinitionAST"];
n3 [label="SimpleSpecifierAST"];
n4 [label="DeclaratorAST"];
n5 [label="DeclaratorIdAST"];
n6 [label="SimpleNameAST"];
n7 [label="FunctionDeclaratorAST"];
n8 [label="CompoundStatementAST"];
n9 [label="DeclarationStatementAST"];
n10 [label="SimpleDeclarationAST"];
n11 [label="NamedTypeSpecifierAST"];
n12 [label="SimpleNameAST"];
n13 [label="DeclaratorAST"];
n14 [label="PointerAST"];
n15 [label="DeclaratorIdAST"];
n16 [label="SimpleNameAST"];
n17 [label="DeclarationStatementAST"];
n18 [label="SimpleDeclarationAST"];
n19 [label="NamedTypeSpecifierAST"];
n20 [label="SimpleNameAST"];
n21 [label="DeclaratorAST"];
n22 [label="DeclaratorIdAST"];
n23 [label="SimpleNameAST"];
n1 -> n2
n2 -> n3
n3 -> t1
n2 -> n4
n4 -> n5
n5 -> n6
n6 -> t2
n4 -> n7
n7 -> t3
n7 -> t4
n2 -> n8
n8 -> t5
n8 -> n9
n9 -> n10
n10 -> n11
n11 -> n12
n12 -> t6
n10 -> n13
n13 -> n14
n14 -> t7
n13 -> n15
n15 -> n16
n16 -> t8
n10 -> t9
n8 -> n17
n17 -> n18
n18 -> n19
n19 -> n20
n20 -> t10
n18 -> n21
n21 -> n22
n22 -> n23
n23 -> t11
n18 -> t12
n8 -> t13
{ rank=same;
t1 [shape=rect label = "void"];
t2 [shape=rect label = "g"]; t1 -> t2 [arrowhead="vee" color="transparent"];
t3 [shape=rect label = "("]; t2 -> t3 [arrowhead="vee" color="transparent"];
t4 [shape=rect label = ")"]; t3 -> t4 [arrowhead="vee" color="transparent"];
t5 [shape=rect label = "{"]; t4 -> t5 [arrowhead="vee" color="transparent"];
t6 [shape=rect label = "x"]; t5 -> t6 [arrowhead="vee" color="transparent"];
t7 [shape=rect label = "*"]; t6 -> t7 [arrowhead="vee" color="transparent"];
t8 [shape=rect label = "y"]; t7 -> t8 [arrowhead="vee" color="transparent"];
t9 [shape=rect label = ";"]; t8 -> t9 [arrowhead="vee" color="transparent"];
t10 [shape=rect label = "x"]; t9 -> t10 [arrowhead="vee" color="transparent"];
t11 [shape=rect label = "z"]; t10 -> t11 [arrowhead="vee" color="transparent"];
t12 [shape=rect label = ";"]; t11 -> t12 [arrowhead="vee" color="transparent"];
t13 [shape=rect label = "}"]; t12 -> t13 [arrowhead="vee" color="transparent"];
}
}
    )raw";

    checkAst(source, expectedAst, false);
}
