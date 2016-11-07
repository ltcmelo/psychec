/******************************************************************************
 * Copyright (c) 2016 Leandro T. C. Melo (ltcmelo@gmail.com)
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301
 * USA
 *****************************************************************************/

/*
 Contributors:
   - Marcus Rodrigues (demaroar@gmail.com)
*/

#include "Tester.h"
#include "AST.h"
#include "Control.h"
#include "Dumper.h"
#include "Literals.h"
#include "TranslationUnit.h"
#include <cctype>
#include <iostream>
#include <memory>
#include <sstream>
#include <string.h>

using namespace psyche;
using namespace CPlusPlus;

namespace {

// The name/file for all test cases.
StringLiteral name("testfile", strlen("testfile"));

} // anonymous

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
void Tester::checkAst(const std::string &source, std::string expected)
{
    Control control;
    std::unique_ptr<TranslationUnit> program =
            analyseProgram(source, control, name, options_);
    PSYCHE_EXPECT_TRUE(program);

    std::ostringstream oss;
    Dumper(program.get()).dump(program->ast()->asTranslationUnit(), "test", oss);

    compareText(expected, oss.str());
}

/*
 * The expected contraints must be in accordance with Rodrigo's type inference
 * engine input.
 */
void Tester::checkConstraints(const std::string &source, std::string expected)
{
    Control control;
    std::unique_ptr<TranslationUnit> program =
            analyseProgram(source, control, name, options_);
    PSYCHE_EXPECT_TRUE(program);

    compareText(expected, options_.constraints_);
}

void Tester::compareText(std::string expected, std::string actual) const
{
    // Remove all spaces to avoid silly errors in comparisson. Use a lambda
    // instead of the function directly because isspace is overloaded as a
    // template.
    auto checkSpace = [](const char c) { return std::isspace(c); };
    expected.erase(std::remove_if(expected.begin(), expected.end(), checkSpace), expected.end());
    actual.erase(std::remove_if(actual.begin(), actual.end(), checkSpace), actual.end());

    PSYCHE_EXPECT_STR_EQ(expected, actual);
}

void Tester::reset()
{
    options_ = AnalysisOptions();
}

void Tester::testAll()
{
    std::cout << "Running tests..." << std::endl;
    for (auto testData : tests_) {
        reset();
        currentTest_ = testData.second;
        testData.first(this);
        std::cout << "\t" << currentTest_ << " passed!" << std::endl;
    }
}

void Tester::testCase1()
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

void Tester::testCase2()
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

void Tester::testCase3()
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

void Tester::testCase4()
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

void Tester::testCase5()
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

void Tester::testCase6()
{
    std::string source = R"raw(
void f(int) {
  x y;
}
    )raw";

    std::string expectedConstraints = R"raw(
$exists$ #alpha1.
 $typedef$ int $as$ #alpha1,
  $exists$ #alpha2.
   $typedef$ void $as$ #alpha2,
    $def$ f : (int, void)  $in$
     $def$ unnamed1 : int $in$
      $exists$ #alpha3.
       $typedef$ x $as$ #alpha3,
        $def$ y : #alpha3 $in$
         #alpha2 = void,
          #alpha3 = x
    )raw";

    checkConstraints(source, expectedConstraints);
}

void Tester::testCase7()
{
    std::string source = R"raw(
void f(int) {
    x * y;
    x + y;
}
    )raw";

    std::string expectedConstraints = R"raw(
$exists$ #alpha1.
 $typedef$ int $as$ #alpha1,
  $exists$ #alpha2.
   $typedef$ void $as$ #alpha2,
    $def$ f : (int, void)  $in$
     $def$ unnamed1 : int $in$
      $exists$ #alpha3.
       $exists$ #alpha4.
        $exists$ #alpha5.
         $typeof$(x) = #alpha4, $typeof$(y) = #alpha5, true = true, $typeof$(*) = (#alpha4, #alpha5, #alpha3),
          $exists$ #alpha6.
           $exists$ #alpha7.
            $exists$ #alpha8.
             $typeof$(x) = #alpha7, $typeof$(y) = #alpha8, true = true,
              #alpha2 = void
    )raw";

    checkConstraints(source, expectedConstraints);
}

void Tester::testCase8()
{
    std::string source = R"raw(
int f(int* a) {
  T x = *a;
  return x;
}
    )raw";

    std::string expectedConstraints = R"raw(
$exists$ #alpha1.
 $typedef$ int* $as$ #alpha1,
  $exists$ #alpha2.
   $typedef$ int $as$ #alpha2,
    $def$ f : (int*, int)  $in$
     $def$ a : int* $in$
      $exists$ #alpha3.
       $typedef$ T $as$ #alpha3,
        $exists$ #alpha4.
         $exists$ #alpha5.
          $typeof$(a) = #alpha5, #alpha5 = #alpha4*,
           #alpha3 = #alpha4, #alpha3 = int,
            $def$ x : #alpha3 $in$
             $typeof$(x) = int,
              #alpha3 = T

    )raw";

    checkConstraints(source, expectedConstraints);
}

void Tester::testCase9()
{
    std::string source = R"raw(
void f() {
    if (x)
        ;
    y;
}
    )raw";

    std::string expectedConstraints = R"raw(
$exists$ #alpha1.
 $typedef$ void $as$ #alpha1,
  $def$ f : (void)  $in$
   $exists$ #alpha2.
    $typeof$(x) = #alpha2,
     $exists$ #alpha3.
      $typeof$(y) = #alpha3,
       #alpha1 = void
    )raw";

    checkConstraints(source, expectedConstraints);
}

void Tester::testCase10()
{
    std::string source = R"raw(
void f() {
    if (x)
    {}
    y;
}
    )raw";

    std::string expectedConstraints = R"raw(
$exists$ #alpha1.
 $typedef$ void $as$ #alpha1,
  $def$ f : (void)  $in$
   $exists$ #alpha2.
    $typeof$(x) = #alpha2,
     $exists$ #alpha3.
      $typeof$(y) = #alpha3,
       #alpha1 = void
    )raw";

    checkConstraints(source, expectedConstraints);
}

void Tester::testCase11()
{
    std::string source = R"raw(
void f() {
    int *a;
}
    )raw";

    std::string expectedConstraints = R"raw(
$exists$ #alpha1.
 $typedef$ void $as$ #alpha1,
  $def$ f : (void)  $in$
   $exists$ #alpha2.
    $typedef$ int* $as$ #alpha2,
     $def$ a : #alpha2 $in$
      #alpha1 = void,
       #alpha2 = int*
    )raw";

    checkConstraints(source, expectedConstraints);
}

void Tester::testCase12()
{
    std::string source = R"raw(
void f() {
    int a[2];
}
    )raw";

    std::string expectedConstraints = R"raw(
$exists$ #alpha1.
 $typedef$ void $as$ #alpha1,
  $def$ f : (void)  $in$
   $exists$ #alpha2.
    $typedef$ int* $as$ #alpha2,
     $def$ a : #alpha2 $in$
      int = int, #alpha1 = void,
       #alpha2 = int*
    )raw";

    checkConstraints(source, expectedConstraints);
}

void Tester::testCase13()
{
    std::string source = R"raw(
void f(int a, char b) {
    return 0;
}
    )raw";

    std::string expectedConstraints = R"raw(
$exists$ #alpha1.
 $exists$ #alpha2.
  $typedef$ int $as$ #alpha1,
   $typedef$ char $as$ #alpha2,
    $exists$ #alpha3.
     $typedef$ void $as$ #alpha3,
      $def$ f : (int, char, void)  $in$
       $def$ a : int $in$
        $def$ b : char $in$
         void = void
    )raw";

   checkConstraints(source, expectedConstraints);
}

void Tester::testCase14()
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

void Tester::testCase15()
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

void Tester::testCase16()
{
    std::string source = R"raw(
int f() {
    T a[1];
    a[1] = 10;
}
    )raw";

    std::string expectedConstraints = R"raw(
$exists$ #alpha1.
$typedef$ int $as$ #alpha1,
$def$ f : (int)  $in$
 $exists$ #alpha2.
  $typedef$ T* $as$ #alpha2,
   $def$ a : #alpha2 $in$
    int = int, $exists$ #alpha3.
     $exists$ #alpha4.
      $exists$ #alpha5.
       $exists$ #alpha6.
        $exists$ #alpha7.
         $typeof$(a) = #alpha5, int = #alpha6, #alpha5 = #alpha7*, #alpha3 = #alpha7, #alpha6 = int, int = #alpha4, #alpha3 = #alpha4, #alpha3 = int,
          #alpha1 = int,
           #alpha2 = T*
    )raw";

   checkConstraints(source, expectedConstraints);
}

void Tester::testCase17()
{
    std::string source = R"raw(
int f() {
    T* t ;
    *t = 10;
}
    )raw";

    std::string expectedConstraints = R"raw(
$exists$ #alpha1.
 $typedef$ int $as$ #alpha1,
  $def$ f : (int)  $in$
   $exists$ #alpha2.
    $typedef$ T* $as$ #alpha2,
     $def$ t : #alpha2 $in$
      $exists$ #alpha3.
       $exists$ #alpha4.
        $exists$ #alpha5.
         $typeof$(t) = #alpha5, #alpha5 = #alpha3*, int = #alpha4, #alpha3 = #alpha4, #alpha3 = int,
          #alpha1 = int,
           #alpha2 = T*
    )raw";

   checkConstraints(source, expectedConstraints);
}

void Tester::testCase18()
{
    std::string source = R"raw(
int main() {
    T x;
    char c = 'a';
    x.c = c;
    x.i = 1;
}
    )raw";

    std::string expectedConstraints = R"raw(
$exists$ #alpha1.
$typedef$ int $as$ #alpha1,
$def$ main : (int)  $in$
$exists$ #alpha2.
$typedef$ T $as$ #alpha2,
 $def$ x : #alpha2 $in$
  $exists$ #alpha3.
   $typedef$ char $as$ #alpha3,
    $exists$ #alpha4.
     char = #alpha4,
      #alpha3 = #alpha4, #alpha3 = int,
       $def$ c : #alpha3 $in$
        $exists$ #alpha5.
         $exists$ #alpha6.
          $exists$ #alpha7.
           $exists$ #alpha8.
            $typeof$(x) = #alpha7, $has$ (#alpha7, c : #alpha8), #alpha5 = #alpha8, $typeof$(c) = #alpha6, #alpha5 = #alpha6, #alpha5 = char,
             $exists$ #alpha9.
              $exists$ #alpha10.
               $exists$ #alpha11.
                $exists$ #alpha12.
                 $typeof$(x) = #alpha11, $has$ (#alpha11, i : #alpha12), #alpha9 = #alpha12, int = #alpha10, #alpha9 = #alpha10, #alpha9 = int,
                  #alpha1 = int,
                   #alpha3 = char,
                    #alpha2 = T
    )raw";

    checkConstraints(source, expectedConstraints);
}

void Tester::testCase19()
{
    std::string source = R"raw(
int main() {
    T x;
    char c = 'a';
    x->c = c;
    x->i = 1;
}
    )raw";

    std::string expectedConstraints = R"raw(
$exists$ #alpha1.
 $typedef$ int $as$ #alpha1,
  $def$ main : (int)  $in$
   $exists$ #alpha2.
    $typedef$ T $as$ #alpha2,
     $def$ x : #alpha2 $in$
      $exists$ #alpha3.
       $typedef$ char $as$ #alpha3,
        $exists$ #alpha4.
         char = #alpha4,
          #alpha3 = #alpha4, #alpha3 = int,
           $def$ c : #alpha3 $in$
            $exists$ #alpha5.
             $exists$ #alpha6.
              $exists$ #alpha7.
               $exists$ #alpha8.
                $exists$ #alpha9.
                 $typeof$(x) = #alpha7, #alpha7 = #alpha8*, $has$ (#alpha8, c : #alpha9), #alpha5 = #alpha9, $typeof$(c) = #alpha6, #alpha5 = #alpha6, #alpha5 = char,
                  $exists$ #alpha10.
                   $exists$ #alpha11.
                    $exists$ #alpha12.
                     $exists$ #alpha13.
                      $exists$ #alpha14.
                       $typeof$(x) = #alpha12, #alpha12 = #alpha13*, $has$ (#alpha13, i : #alpha14), #alpha10 = #alpha14, int = #alpha11, #alpha10 = #alpha11, #alpha10 = int,
                        #alpha1 = int,
                         #alpha3 = char,
                          #alpha2 = T
    )raw";

   checkConstraints(source, expectedConstraints);
}

void Tester::testCase20()
{
    std::string source = R"raw(
int main() {
    T x;
    char c = 'a';
    *(x.c) = c;
    *(x.i) = 1;
}
    )raw";

    std::string expectedConstraints = R"raw(
$exists$ #alpha1.
 $typedef$ int $as$ #alpha1,
  $def$ main : (int)  $in$
   $exists$ #alpha2.
    $typedef$ T $as$ #alpha2,
     $def$ x : #alpha2 $in$
      $exists$ #alpha3.
       $typedef$ char $as$ #alpha3,
        $exists$ #alpha4.
         char = #alpha4,
          #alpha3 = #alpha4, #alpha3 = int,
           $def$ c : #alpha3 $in$
            $exists$ #alpha5.
             $exists$ #alpha6.
              $exists$ #alpha7.
               $exists$ #alpha8.
                $exists$ #alpha9.
                 $typeof$(x) = #alpha8, $has$ (#alpha8, c : #alpha9), #alpha7 = #alpha9, #alpha7 = #alpha5*, $typeof$(c) = #alpha6, #alpha5 = #alpha6, #alpha5 = char,
                  $exists$ #alpha10.
                   $exists$ #alpha11.
                    $exists$ #alpha12.
                     $exists$ #alpha13.
                      $exists$ #alpha14.
                       $typeof$(x) = #alpha13, $has$ (#alpha13, i : #alpha14), #alpha12 = #alpha14, #alpha12 = #alpha10*, int = #alpha11, #alpha10 = #alpha11, #alpha10 = int,
                        #alpha1 = int,
                         #alpha3 = char,
                          #alpha2 = T
    )raw";

   checkConstraints(source, expectedConstraints);
}

void Tester::testCase21()
{
    std::string source = R"raw(
void for_each(int* t, void (*f)(void*)) {
}
void a(int* (*f)(char, double)) {
}
    )raw";

    std::string expectedConstraints = R"raw(
$exists$ #alpha1.
 $exists$ #alpha2.
  $typedef$ int* $as$ #alpha1,
   $typedef$ (void*,void)* $as$ #alpha2,
    $exists$ #alpha3.
     $typedef$ void $as$ #alpha3,
      $def$ for_each : (int*, (void*,void)*, void)  $in$
       $def$ t : int* $in$
        $def$ f : (void*,void)* $in$
         #alpha3 = void
 ,
 $exists$ #alpha4.
  $typedef$ (char,double,int*)* $as$ #alpha4,
   $exists$ #alpha5.
    $typedef$ void $as$ #alpha5,
     $def$ a : ((char,double,int*)*, void)  $in$
      $def$ f : (char,double,int*)* $in$
       #alpha5 = void
    )raw";

    checkConstraints(source, expectedConstraints);
}

void Tester::testCase22()
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

void Tester::testCase23()
{
    std::string source = R"raw(
void f() {
    ;
}
    )raw";

    std::string expectedConstraints = R"raw(
$exists$ #alpha1.
 $typedef$ void $as$ #alpha1,
  $def$ f : (void)  $in$
   #alpha1 = void
    )raw";

   checkConstraints(source, expectedConstraints);
}

void Tester::testCase24()
{
    std::string source = R"raw(
void f() {
    int a;
    for (;;) {}
}
    )raw";

    std::string expectedConstraints = R"raw(
$exists$ #alpha1.
 $typedef$ void $as$ #alpha1,
  $def$ f : (void)  $in$
   $exists$ #alpha2.
    $typedef$ int $as$ #alpha2,
     $def$ a : #alpha2 $in$
      #alpha1 = void,
       #alpha2 = int
    )raw";

   checkConstraints(source, expectedConstraints);
}

void Tester::testCase25()
{
    std::string source = R"raw(
void f() {}
    )raw";

    std::string expectedConstraints = R"raw(
$exists$ #alpha1.
 $typedef$ void $as$ #alpha1,
  $def$ f : (void)  $in$
   #alpha1 = void
    )raw";

   checkConstraints(source, expectedConstraints);
}

void Tester::testCase26()
{
    std::string source = R"raw(
void f() {
    c = 2, d = 0.1;
}
    )raw";

    std::string expectedConstraints = R"raw(
$exists$ #alpha1.
 $typedef$ void $as$ #alpha1,
  $def$ f : (void)  $in$
   $exists$ #alpha2.
    $exists$ #alpha3.
     $exists$ #alpha4.
      $typeof$(c) = #alpha3, int = #alpha4, #alpha3 = #alpha4, #alpha3 = int,
       $exists$ #alpha5.
        $exists$ #alpha6.
         $typeof$(d) = #alpha5, double = #alpha6, #alpha5 = #alpha6, #alpha5 = double,
          #alpha1 = void
    )raw";

   checkConstraints(source, expectedConstraints);
}

void Tester::testCase27()
{
    std::string source = R"raw(
void f() {
    a(x) = 1; /* Must be a variable declaration ($in$ C only). */
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

void Tester::testCase28()
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

void Tester::testCase29()
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

void Tester::testCase30()
{
    // This one is a function declaration with implicit return type. But we
    // don't handle it yet - some work $in$ the parser is required.

    std::string source = R"raw(
$typedef$ int x;
void f() {
    a(x); /* Function declaration. */
}
    )raw";

    std::string expectedAst = R"raw(
    )raw";

    checkAst(source, expectedAst);
}

void Tester::testCase31()
{
    std::string source = R"raw(
T f(int a) {
    return &a;
}
    )raw";

    std::string expectedConstraints = R"raw(
$exists$ #alpha1.
 $typedef$ int $as$ #alpha1,
  $exists$ #alpha2.
   $typedef$ T $as$ #alpha2,
    $def$ f : (int, T)  $in$
     $def$ a : int $in$
      $exists$ #alpha3.
       $exists$ #alpha4.
        $typeof$(a) = #alpha4, #alpha3 = #alpha4*, T = #alpha3

    )raw";

   checkConstraints(source, expectedConstraints);
}

void Tester::testCase32()
{
    std::string source = R"raw(
T* f(int a) {
  return &a;
}
    )raw";

    std::string expectedConstraints = R"raw(
$exists$ #alpha1.
 $typedef$ int $as$ #alpha1,
  $exists$ #alpha2.
   $typedef$ T* $as$ #alpha2,
    $def$ f : (int, T*)  $in$
     $def$ a : int $in$
      $exists$ #alpha3.
       $exists$ #alpha4.
        $typeof$(a) = #alpha4, #alpha3 = #alpha4*, T* = #alpha3
    )raw";

   checkConstraints(source, expectedConstraints);
}

void Tester::testCase33()
{
    std::string source = R"raw(
void f() {
    for (T i = 10; ;) {}
    int j;
}
    )raw";

    std::string expectedConstraints = R"raw(
$exists$ #alpha1.
 $typedef$ void $as$ #alpha1,
  $def$ f : (void)  $in$
   $exists$ #alpha2.
    $typedef$ T $as$ #alpha2,
     $exists$ #alpha3.
      int = #alpha3,
       #alpha2 = #alpha3, #alpha2 = int,
        $def$ i : #alpha2 $in$
         $exists$ #alpha4.
          $typedef$ T $as$ #alpha4,
           $exists$ #alpha5.
            int = #alpha5,
             #alpha4 = #alpha5, #alpha4 = int,
              $def$ i : #alpha4 $in$
               #alpha4 = T,
                $exists$ #alpha6.
                 $typedef$ int $as$ #alpha6,
                  $def$ j : #alpha6 $in$
                   #alpha1 = void,
                    #alpha6 = int,
                     #alpha2 = T
    )raw";

   checkConstraints(source, expectedConstraints);
}

void Tester::testCase34()
{
    std::string source = R"raw(
void f() {
    T i;
    for (i = 10; ;) {}
    int j;
}
    )raw";

    std::string expectedConstraints = R"raw(
$exists$ #alpha1.
 $typedef$ void $as$ #alpha1,
  $def$ f : (void)  $in$
   $exists$ #alpha2.
    $typedef$ T $as$ #alpha2,
     $def$ i : #alpha2 $in$
      $exists$ #alpha3.
       $exists$ #alpha4.
        $typeof$(i) = #alpha3, int = #alpha4, #alpha3 = #alpha4, #alpha3 = int,
         $exists$ #alpha5.
          $typedef$ int $as$ #alpha5,
           $def$ j : #alpha5 $in$
            #alpha1 = void,
             #alpha5 = int,
              #alpha2 = T

    )raw";

   checkConstraints(source, expectedConstraints);
}

void Tester::testCase35()
{
    std::string source = R"raw(
void f() {
    for (T i = 10; ;) { T2 j = 0; }
    int j;
}
    )raw";

    std::string expectedConstraints = R"raw(
$exists$ #alpha1.
$typedef$ void $as$ #alpha1,
$def$ f : (void)  $in$
 $exists$ #alpha2.
  $typedef$ T $as$ #alpha2,
   $exists$ #alpha3.
    int = #alpha3,
     #alpha2 = #alpha3, #alpha2 = int,
      $def$ i : #alpha2 $in$
       $exists$ #alpha4.
        $typedef$ T2 $as$ #alpha4,
         $exists$ #alpha5.
          #alpha5 = #alpha5,
           #alpha4 = #alpha5,
            $def$ j : #alpha4 $in$
             $exists$ #alpha6.
              $typedef$ T $as$ #alpha6,
               $exists$ #alpha7.
                int = #alpha7,
                 #alpha6 = #alpha7, #alpha6 = int,
                  $def$ i : #alpha6 $in$
                   #alpha6 = T,
                    $exists$ #alpha8.
                     $typedef$ int $as$ #alpha8,
                      $def$ j : #alpha8 $in$
                       #alpha1 = void,
                        #alpha8 = int,
                         #alpha4 = T2,
                          #alpha2 = T
    )raw";

   checkConstraints(source, expectedConstraints);
}

void Tester::testCase36()
{
    std::string source = R"raw(
void f() {
    T i;
    for (i = 10; ;) { T2 j = 0; }
    int j;
}
    )raw";

    std::string expectedConstraints = R"raw(
$exists$ #alpha1.
 $typedef$ void $as$ #alpha1,
  $def$ f : (void)  $in$
   $exists$ #alpha2.
    $typedef$ T $as$ #alpha2,
     $def$ i : #alpha2 $in$
      $exists$ #alpha3.
       $exists$ #alpha4.
        $typeof$(i) = #alpha3, int = #alpha4, #alpha3 = #alpha4, #alpha3 = int,
         $exists$ #alpha5.
          $typedef$ T2 $as$ #alpha5,
           $exists$ #alpha6.
            #alpha6 = #alpha6,
             #alpha5 = #alpha6,
              $def$ j : #alpha5 $in$
               $exists$ #alpha7.
                $typedef$ int $as$ #alpha7,
                 $def$ j : #alpha7 $in$
                  #alpha1 = void,
                   #alpha7 = int,
                    #alpha5 = T2,
                     #alpha2 = T
    )raw";

   checkConstraints(source, expectedConstraints);
}

void Tester::testCase37()
{
    std::string source = R"raw(
void f() {
    T a;
    a + 2;
}
    )raw";

    std::string expectedConstraints = R"raw(
$exists$ #alpha1.
 $typedef$ void $as$ #alpha1,
  $def$ f : (void)  $in$
   $exists$ #alpha2.
    $typedef$ T $as$ #alpha2,
     $def$ a : #alpha2 $in$
      $exists$ #alpha3.
       $exists$ #alpha4.
        $exists$ #alpha5.
         $typeof$(a) = #alpha4, int = #alpha5, #alpha4 = #alpha5, #alpha4 = int, #alpha3 = int,
          #alpha1 = void,
           #alpha2 = T

    )raw";

   checkConstraints(source, expectedConstraints);
}

void Tester::testCase38()
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

void Tester::testCase39()
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

void Tester::testCase40()
{
    std::string source = R"raw(
void f() {
    for (T i = 10; ; i++) {}
}
    )raw";

    std::string expectedConstraints = R"raw(
$exists$ #alpha1.
$typedef$ void $as$ #alpha1,
$def$ f : (void)  $in$
 $exists$ #alpha2.
  $typedef$ T $as$ #alpha2,
   $exists$ #alpha3.
    int = #alpha3,
     #alpha2 = #alpha3, #alpha2 = int,
      $def$ i : #alpha2 $in$
       $exists$ #alpha4.
        $exists$ #alpha5.
         $typeof$(i) = #alpha5,
          $exists$ #alpha6.
           $typedef$ T $as$ #alpha6,
            $exists$ #alpha7.
             int = #alpha7,
              #alpha6 = #alpha7, #alpha6 = int,
               $def$ i : #alpha6 $in$
                #alpha6 = T,
                 #alpha1 = void,
                  #alpha2 = T
    )raw";

   checkConstraints(source, expectedConstraints);
}

void Tester::testCase41()
{
    std::string source = R"raw(
T1 f() {
    T2 a = 12;
}
    )raw";

    std::string expectedConstraints = R"raw(
$exists$ #alpha1.
 $typedef$ T1 $as$ #alpha1,
  $def$ f : (T1)  $in$
   $exists$ #alpha2.
    $typedef$ T2 $as$ #alpha2,
     $exists$ #alpha3.
      int = #alpha3,
       #alpha2 = #alpha3, #alpha2 = int,
        $def$ a : #alpha2 $in$
         #alpha1 = T1,
          #alpha2 = T2
    )raw";

   checkConstraints(source, expectedConstraints);
}

void Tester::testCase42()
{
    std::string source = R"raw(
void f() {
    T a = malloc(4);
}
    )raw";

    std::string expectedConstraints = R"raw(
$exists$ #alpha1.
 $typedef$ void $as$ #alpha1,
  $def$ f : (void)  $in$
   $exists$ #alpha2.
    $typedef$ T $as$ #alpha2,
     $exists$ #alpha3.
      $exists$ #alpha4.
       int = #alpha4, $typeof$(malloc) = (#alpha4, #alpha3),
        #alpha2 = #alpha3,
         $def$ a : #alpha2 $in$
          #alpha1 = void,
           #alpha2 = T
    )raw";

   checkConstraints(source, expectedConstraints);
}
void Tester::testCase43()
{
    std::string source = R"raw(
void f() {
    T* a = malloc(4);
}
    )raw";

    std::string expectedConstraints = R"raw(
$exists$ #alpha1.
 $typedef$ void $as$ #alpha1,
  $def$ f : (void)  $in$
   $exists$ #alpha2.
    $typedef$ T* $as$ #alpha2,
     $exists$ #alpha3.
      $exists$ #alpha4.
       int = #alpha4, $typeof$(malloc) = (#alpha4, #alpha3),
        #alpha2 = #alpha3,
         $def$ a : #alpha2 $in$
          #alpha1 = void,
           #alpha2 = T*
    )raw";

   checkConstraints(source, expectedConstraints);
}

void Tester::testCase44()
{
    std::string source = R"raw(
void f() {
    T a = sizeof(void*);
}
    )raw";

    std::string expectedConstraints = R"raw(
$exists$ #alpha1.
 $typedef$ void $as$ #alpha1,
  $def$ f : (void)  $in$
   $exists$ #alpha2.
    $typedef$ T $as$ #alpha2,
     $exists$ #alpha3.
      #alpha3 = int,
       #alpha2 = #alpha3, #alpha2 = int,
        $def$ a : #alpha2 $in$
         #alpha1 = void,
          #alpha2 = T
    )raw";

   checkConstraints(source, expectedConstraints);
}

void Tester::testCase45()
{
    std::string source = R"raw(
void f(T a) {
    for (;;)
        for (;;) {
            a = 0;
        }
}
    )raw";

    std::string expectedConstraints = R"raw(
$exists$ #alpha1.
 $typedef$ T $as$ #alpha1,
  $exists$ #alpha2.
   $typedef$ void $as$ #alpha2,
    $def$ f : (T, void)  $in$
     $def$ a : T $in$
      $exists$ #alpha3.
       $exists$ #alpha4.
        $typeof$(a) = #alpha3, #alpha4 = #alpha4, #alpha3 = #alpha4,
         #alpha2 = void
    )raw";

   checkConstraints(source, expectedConstraints);
}

void Tester::testCase46()
{
    std::string source = R"raw(
void f() {
    int a;
    T1 b;
    T2 c = b ? a : a;
}
)raw";

    std::string expectedConstraints = R"raw(
$exists$ #alpha1.
 $typedef$ void $as$ #alpha1,
  $def$ f : (void)  $in$
   $exists$ #alpha2.
    $typedef$ int $as$ #alpha2,
     $def$ a : #alpha2 $in$
      $exists$ #alpha3.
       $typedef$ T1 $as$ #alpha3,
        $def$ b : #alpha3 $in$
         $exists$ #alpha4.
          $typedef$ T2 $as$ #alpha4,
           $exists$ #alpha5.
            $exists$ #alpha6.
             $typeof$(b) = #alpha6, $typeof$(a) = #alpha5, $typeof$(a) = #alpha5,
              #alpha4 = #alpha5, #alpha4 = int,
               $def$ c : #alpha4 $in$
                #alpha1 = void,
                 #alpha4 = T2,
                  #alpha3 = T1,
                   #alpha2 = int
    )raw";

   checkConstraints(source, expectedConstraints);
}

void Tester::testCase47()
{
    std::string source = R"raw(
void f() {
    A a = true;
}
)raw";

    std::string expectedConstraints = R"raw(
$exists$ #alpha1.
 $typedef$ void $as$ #alpha1,
  $def$ f : (void)  $in$
   $exists$ #alpha2.
    $typedef$ A $as$ #alpha2,
     $exists$ #alpha3.
      int = #alpha3,
       #alpha2 = #alpha3, #alpha2 = int,
        $def$ a : #alpha2 $in$
         #alpha1 = void,
          #alpha2 = A
    )raw";

   checkConstraints(source, expectedConstraints);
}

void Tester::testCase48()
{
    std::string source = R"raw(
void f() {
    T1 a;
    T2 b;
    T3 c;
    printf("this is a %% weird %s, %f, format %-2.2d test", a, b, c);
}
)raw";

    std::string expectedConstraints = R"raw(
            $exists$ #alpha1.
             $typedef$ void $as$ #alpha1,
              $def$ f : (void)  $in$
               $exists$ #alpha2.
                $typedef$ T1 $as$ #alpha2,
                 $def$ a : #alpha2 $in$
                  $exists$ #alpha3.
                   $typedef$ T2 $as$ #alpha3,
                    $def$ b : #alpha3 $in$
                     $exists$ #alpha4.
                      $typedef$ T3 $as$ #alpha4,
                       $def$ c : #alpha4 $in$
                        $exists$ #alpha5.
                         char const* = char*, $typeof$(a) = char*, $typeof$(b) = double, $typeof$(c) = int,
                          #alpha1 = void,
                           #alpha4 = T3,
                            #alpha3 = T2,
                             #alpha2 = T1
    )raw";

   checkConstraints(source, expectedConstraints);
}

void Tester::testCase49()
{
    // This test uses `fprintf` instead of `printf` $as$ the previous case. The
    // diference is $in$ the position of the format specifiers param.
    std::string source = R"raw(
void f() {
    T1 a;
    T2 b;
    T3 c;
    fprintf(0, "this is a %% weird %s, %f, format %-2.2d test", a, b, c);
}
)raw";

    std::string expectedConstraints = R"raw(
            $exists$ #alpha1.
             $typedef$ void $as$ #alpha1,
              $def$ f : (void)  $in$
               $exists$ #alpha2.
                $typedef$ T1 $as$ #alpha2,
                 $def$ a : #alpha2 $in$
                  $exists$ #alpha3.
                   $typedef$ T2 $as$ #alpha3,
                    $def$ b : #alpha3 $in$
                     $exists$ #alpha4.
                      $typedef$ T3 $as$ #alpha4,
                       $def$ c : #alpha4 $in$
                        $exists$ #alpha5.
                         $exists$ #alpha6.
                          #alpha6 = #alpha6,
                           char const* = char*, $typeof$(a) = char*, $typeof$(b) = double, $typeof$(c) = int,
                            #alpha1 = void,
                             #alpha4 = T3,
                              #alpha3 = T2,
                               #alpha2 = T1
    )raw";

   checkConstraints(source, expectedConstraints);
}

void Tester::testCase50()
{
    std::string source = R"raw(
void f() {
    T1 msg;
    printf(msg);
    T2 a = 1;
}
)raw";

    std::string expectedConstraints = R"raw(
            $exists$ #alpha1.
             $typedef$ void $as$ #alpha1,
              $def$ f : (void)  $in$
               $exists$ #alpha2.
                $typedef$ T1 $as$ #alpha2,
                 $def$ msg : #alpha2 $in$
                  $exists$ #alpha3.
                   $typeof$(msg) = char*,
                    $exists$ #alpha4.
                     $typedef$ T2 $as$ #alpha4,
                      $exists$ #alpha5.
                       int = #alpha5,
                        #alpha4 = #alpha5, #alpha4 = int,
                         $def$ a : #alpha4 $in$
                          #alpha1 = void,
                           #alpha4 = T2,
                            #alpha2 = T1
    )raw";

   checkConstraints(source, expectedConstraints);
}

void Tester::testCase51()
{
    std::string source = R"raw(
void f() {
    printf("foo bar");
    T2 a = 1;
}
)raw";

    std::string expectedConstraints = R"raw(
            $exists$ #alpha1.
             $typedef$ void $as$ #alpha1,
              $def$ f : (void)  $in$
               $exists$ #alpha2.
                char const* = char*,
                 $exists$ #alpha3.
                  $typedef$ T2 $as$ #alpha3,
                   $exists$ #alpha4.
                    int = #alpha4,
                     #alpha3 = #alpha4, #alpha3 = int,
                      $def$ a : #alpha3 $in$
                       #alpha1 = void,
                        #alpha3 = T2
    )raw";

   checkConstraints(source, expectedConstraints);
}

void Tester::testCase52()
{
    std::string source = R"raw(
void f() {
    T1 a;
    T2 b;
    T3 c;
    T4 d;
    switch (a) {
    case 1:
        b = 1.0;
        break;
    case 2:
        c = "string";
        break;
    default:
        d = 9;
        break;
    }
}
)raw";

    std::string expectedConstraints = R"raw(
            $exists$ #alpha1.
             $typedef$ void $as$ #alpha1,
              $def$ f : (void)  $in$
               $exists$ #alpha2.
                $typedef$ T1 $as$ #alpha2,
                 $def$ a : #alpha2 $in$
                  $exists$ #alpha3.
                   $typedef$ T2 $as$ #alpha3,
                    $def$ b : #alpha3 $in$
                     $exists$ #alpha4.
                      $typedef$ T3 $as$ #alpha4,
                       $def$ c : #alpha4 $in$
                        $exists$ #alpha5.
                         $typedef$ T4 $as$ #alpha5,
                          $def$ d : #alpha5 $in$
                           $exists$ #alpha6.
                            $typeof$(a) = #alpha6,
                             int = int,
                              $exists$ #alpha7.
                               $exists$ #alpha8.
                                $typeof$(b) = #alpha7, double = #alpha8, #alpha7 = #alpha8, #alpha7 = double,
                                 int = int,
                                  $exists$ #alpha9.
                                   $exists$ #alpha10.
                                    $typeof$(c) = #alpha9, char const* = #alpha10, #alpha9 = #alpha10,
                                     $exists$ #alpha11.
                                      $exists$ #alpha12.
                                       $typeof$(d) = #alpha11, int = #alpha12, #alpha11 = #alpha12, #alpha11 = int,
                                        #alpha1 = void,
                                         #alpha5 = T4,
                                          #alpha4 = T3,
                                           #alpha3 = T2,
                                            #alpha2 = T1
    )raw";

   checkConstraints(source, expectedConstraints);
}

void Tester::testCase53()
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

void Tester::testCase54()
{
    std::string source = R"raw(
void f() {
  T1 a;
  T2 b;
  T3 c;
  printf("this is a %% weird %s,"
         "%f, "
         "format %-2.2d test", a, b, c);
}
  )raw";

    std::string expectedConstraints = R"raw(
            $exists$ #alpha1.
             $typedef$ void $as$ #alpha1,
              $def$ f : (void)  $in$
               $exists$ #alpha2.
                $typedef$ T1 $as$ #alpha2,
                 $def$ a : #alpha2 $in$
                  $exists$ #alpha3.
                   $typedef$ T2 $as$ #alpha3,
                    $def$ b : #alpha3 $in$
                     $exists$ #alpha4.
                      $typedef$ T3 $as$ #alpha4,
                       $def$ c : #alpha4 $in$
                        $exists$ #alpha5.
                         char const* = char*, $typeof$(a) = char*, $typeof$(b) = double, $typeof$(c) = int,
                          #alpha1 = void,
                           #alpha4 = T3,
                            #alpha3 = T2,
                             #alpha2 = T1
    )raw";

   checkConstraints(source, expectedConstraints);
}

void Tester::testCase55() {
  std::string source = R"raw(
// example 1
m() {} // gen #alpha = int

// example 2
void h() { /* no ret */ } // #alpha = void

// example 3
int f() { /* no ret */ } // #alpha = int

// example 4
Foo g() { /* no ret */ } // #alpha = Foo and foo is orphan => uintptr_t = Foo

// example 5
n(); // similar $as$ example 1, but a declaration instead of a definition

  )raw";

    std::string expectedConstraints = R"raw(
$exists$ #alpha1.
$typedef$ int $as$ #alpha1,
$def$ m : (int)  $in$
 #alpha1 = int,
  #alpha5 = (int)
,
$exists$ #alpha2.
$typedef$ void $as$ #alpha2,
 $def$ h : (void)  $in$
  #alpha2 = void
,
$exists$ #alpha3.
$typedef$ int $as$ #alpha3,
 $def$ f : (int)  $in$
  #alpha3 = int
,
$exists$ #alpha4.
$typedef$ Foo $as$ #alpha4,
 $def$ g : (Foo)  $in$
  #alpha4 = Foo
,
$exists$ #alpha5.
$typedef$ (int) $as$ #alpha5,
 $def$ n : #alpha5 $in$
    )raw";

    checkConstraints(source, expectedConstraints);
}

void Tester::testCase56() {
    std::string source = R"raw(
typedef struct A { float x; int y; } A;
void f() {
    A a;
    T1 i = a.x;
    A* ap;
    T2 j = ap->y;
}
    )raw";

  std::string expectedConstraints = R"raw(
        $typedef$ struct A $as$ struct A{ float x; int y; },
         $exists$ #alpha1.
          #alpha1 = struct A,
           $exists$ #alpha2.
            $typedef$ float $as$ #alpha2,
             $def$ x : #alpha2 $in$
              $has$ (#alpha1, x : #alpha2),
               $exists$ #alpha3.
                $typedef$ int $as$ #alpha3,
                 $def$ y : #alpha3 $in$
                  $has$ (#alpha1, y : #alpha3),
                   $typedef$ A $as$ struct A{ float x; int y; }
                    ,
         $exists$ #alpha4.
          $typedef$ void $as$ #alpha4,
           $def$ f : (void)  $in$
            $exists$ #alpha5.
             $typedef$ A $as$ #alpha5,
              $def$ a : #alpha5 $in$
               $exists$ #alpha6.
                $typedef$ T1 $as$ #alpha6,
                 $exists$ #alpha7.
                  $exists$ #alpha8.
                   $exists$ #alpha9.
                    $typeof$(a) = #alpha8, $has$ (#alpha8, x : #alpha9), #alpha7 = #alpha9,
                     #alpha6 = #alpha7, #alpha6 = float,
                      $def$ i : #alpha6 $in$
                       $exists$ #alpha10.
                        $typedef$ A* $as$ #alpha10,
                         $def$ ap : #alpha10 $in$
                          $exists$ #alpha11.
                           $typedef$ T2 $as$ #alpha11,
                            $exists$ #alpha12.
                             $exists$ #alpha13.
                              $exists$ #alpha14.
                               $exists$ #alpha15.
                                $typeof$(ap) = #alpha13, #alpha13 = #alpha14*, $has$ (#alpha14, y : #alpha15), #alpha12 = #alpha15,
                                 #alpha11 = #alpha12, #alpha11 = int,
                                  $def$ j : #alpha11 $in$
                                   #alpha4 = void,
                                    #alpha11 = T2,
                                     #alpha10 = A*,
                                      #alpha6 = T1,
                                       #alpha5 = A,
                                        #alpha3 = int,
                                         #alpha2 = float
    )raw";

    checkConstraints(source, expectedConstraints);
}
