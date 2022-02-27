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

#include "ParserTest.h"

#include "BinderTest.h"

#include "parser/Parser.h"
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

void ParserTest::case0001()
{
    parse("int x ;",
          Expectation().AST({ TranslationUnit,
                              VariableAndOrFunctionDeclaration,
                              BuiltinTypeSpecifier,
                              IdentifierDeclarator }));
}

void ParserTest::case0002()
{
    parse("x y ;",
          Expectation().AST({ TranslationUnit,
                              VariableAndOrFunctionDeclaration,
                              TypedefName,
                              IdentifierDeclarator }));
}

void ParserTest::case0003()
{
    parse("int x , y ;",
          Expectation().AST({ TranslationUnit,
                              VariableAndOrFunctionDeclaration,
                              BuiltinTypeSpecifier,
                              IdentifierDeclarator,
                              IdentifierDeclarator }));
}

void ParserTest::case0004()
{
    parse("int x = 1;",
          Expectation().AST({ TranslationUnit,
                              VariableAndOrFunctionDeclaration,
                              BuiltinTypeSpecifier,
                              IdentifierDeclarator,
                              ExpressionInitializer,
                              IntegerConstantExpression }));
}

void ParserTest::case0005()
{
    parse("int x = 1, y ;") ;
}

void ParserTest::case0006()
{
    parse("int x = 1, y = 2;") ;
}

void ParserTest::case0007()
{
    parse("int x , y = 3;") ;
}

void ParserTest::case0008()
{
    parse("x y = 1 ;") ;
}

void ParserTest::case0009()
{
    parse("const int x ;",
          Expectation().AST({ TranslationUnit,
                              VariableAndOrFunctionDeclaration,
                              ConstQualifier,
                              BuiltinTypeSpecifier,
                              IdentifierDeclarator }));
}

void ParserTest::case0010()
{
    parse("int const x ;",
          Expectation().AST({ TranslationUnit,
                              VariableAndOrFunctionDeclaration,
                              BuiltinTypeSpecifier,
                              ConstQualifier,
                              IdentifierDeclarator }));
}

void ParserTest::case0011()
{
    parse("const int volatile x ;",
          Expectation().AST({ TranslationUnit,
                              VariableAndOrFunctionDeclaration,
                              ConstQualifier,
                              BuiltinTypeSpecifier,
                              VolatileQualifier,
                              IdentifierDeclarator }));
}

void ParserTest::case0012()
{
    parse("int * x ;",
          Expectation().AST({ TranslationUnit,
                              VariableAndOrFunctionDeclaration,
                              BuiltinTypeSpecifier,
                              PointerDeclarator,
                              IdentifierDeclarator }));
}

void ParserTest::case0013()
{
    parse("int * x = 0 ;",
          Expectation().AST({ TranslationUnit,
                              VariableAndOrFunctionDeclaration,
                              BuiltinTypeSpecifier,
                              PointerDeclarator,
                              IdentifierDeclarator,
                              ExpressionInitializer,
                              IntegerConstantExpression }));
}

void ParserTest::case0014()
{
    parse("int * const x ;",
          Expectation().AST({ TranslationUnit,
                              VariableAndOrFunctionDeclaration,
                              BuiltinTypeSpecifier,
                              PointerDeclarator,
                              ConstQualifier,
                              IdentifierDeclarator }));
}

void ParserTest::case0015()
{
    parse("int const * x ;",
          Expectation().AST({ TranslationUnit,
                              VariableAndOrFunctionDeclaration,
                              BuiltinTypeSpecifier,
                              ConstQualifier,
                              PointerDeclarator,
                              IdentifierDeclarator }));
}

void ParserTest::case0016()
{
    parse("const int * x ;",
          Expectation().AST({ TranslationUnit,
                              VariableAndOrFunctionDeclaration,
                              ConstQualifier,
                              BuiltinTypeSpecifier,
                              PointerDeclarator,
                              IdentifierDeclarator }));
}

void ParserTest::case0017()
{
    parse("int * x , y ;",
          Expectation().AST({ TranslationUnit,
                              VariableAndOrFunctionDeclaration,
                              BuiltinTypeSpecifier,
                              PointerDeclarator,
                              IdentifierDeclarator,
                              IdentifierDeclarator }));
}

void ParserTest::case0018()
{
    parse("int * x , * y ;",
          Expectation().AST({ TranslationUnit,
                              VariableAndOrFunctionDeclaration,
                              BuiltinTypeSpecifier,
                              PointerDeclarator,
                              IdentifierDeclarator,
                              PointerDeclarator,
                              IdentifierDeclarator }));
}

void ParserTest::case0019()
{
    parse("int * x = 0, * y = 0 ;") ;
}

void ParserTest::case0020()
{
    parse("const int * x = 0, y ;") ;
}

void ParserTest::case0021()
{
    // Not a syntactic error, but a semantic one; covered in:
    CROSS_REFERENCE_TEST(BinderTest::case0005);

    parse("int double x ;") ;
}

void ParserTest::case0022()
{
    /*
     * Notes:
     *     - `long int y ;'
     *        Good.
     *
     *     - `typedef int x ; long x y;'
     *        Good.
     *
     *     - `x ; int y ;'
     *       `x' is missing a type-specifier.
     */

    parse("x int y ;",
          Expectation().diagnostic(Expectation::ErrorOrWarn::Error,
                                      Parser::DiagnosticsReporter::ID_of_ExpectedFOLLOWofDeclarator)
                       .diagnostic(Expectation::ErrorOrWarn::Warn,
                                      Parser::DiagnosticsReporter::ID_of_ExpectedTypeSpecifier));
}

void ParserTest::case0023()
{
    /*
     * Notes:
     *     - `long int y ;'
     *        Good.
     *
     *     - `typedef int x ; long x y ;'
     *        Not Good.
     */

    parse("long x y ;",
          Expectation().diagnostic(Expectation::ErrorOrWarn::Error,
                                      Parser::DiagnosticsReporter::ID_of_ExpectedFOLLOWofDeclarator));
}

void ParserTest::case0024()
{
    parse("x * double y ;",
          Expectation().diagnostic(Expectation::ErrorOrWarn::Error,
                                      Parser::DiagnosticsReporter::ID_of_ExpectedFOLLOWofDeclarator)
                       .diagnostic(Expectation::ErrorOrWarn::Warn,
                                      Parser::DiagnosticsReporter::ID_of_ExpectedTypeSpecifier));
}

void ParserTest::case0025()
{
    parse("double x * y ;",
          Expectation().diagnostic(Expectation::ErrorOrWarn::Error,
                                      Parser::DiagnosticsReporter::ID_of_ExpectedFOLLOWofDeclarator));
}

void ParserTest::case0026()
{
    parse("x * const double y ;",
          Expectation().diagnostic(Expectation::ErrorOrWarn::Error,
                                      Parser::DiagnosticsReporter::ID_of_ExpectedFOLLOWofDeclarator)
                       .diagnostic(Expectation::ErrorOrWarn::Warn,
                                      Parser::DiagnosticsReporter::ID_of_ExpectedTypeSpecifier));
}

void ParserTest::case0027()
{
    parse("double * const x y ;",
          Expectation().diagnostic(Expectation::ErrorOrWarn::Error,
                                      Parser::DiagnosticsReporter::ID_of_ExpectedFOLLOWofDeclarator));
}

void ParserTest::case0028()
{
    // Not a syntactic error, but diagnoses a semantic warning.
    CROSS_REFERENCE_TEST(BinderTest::case0001) ;

    parse("const int ;",
          Expectation().AST({ TranslationUnit,
                              IncompleteDeclaration,
                              ConstQualifier,
                              BuiltinTypeSpecifier }));
}

void ParserTest::case0029()
{
    parse("const int , double x ;",
          Expectation().diagnostic(Expectation::ErrorOrWarn::Error,
                                      Parser::DiagnosticsReporter::ID_of_ExpectedFIRSTofDirectDeclarator));
}

void ParserTest::case0030()
{
    parse("int x = 1 {",
          Expectation().diagnostic(Expectation::ErrorOrWarn::Error,
                                      Parser::DiagnosticsReporter::ID_of_ExpectedFOLLOWofDeclaratorAndInitializer));
}

void ParserTest::case0031()
{
    parse("int ( x ) ;",
          Expectation().AST({ TranslationUnit,
                              VariableAndOrFunctionDeclaration,
                              BuiltinTypeSpecifier,
                              ParenthesizedDeclarator,
                              IdentifierDeclarator }));
}

void ParserTest::case0032()
{
    parse("int ( ( x ) );",
          Expectation().AST({ TranslationUnit,
                              VariableAndOrFunctionDeclaration,
                              BuiltinTypeSpecifier,
                              ParenthesizedDeclarator,
                              ParenthesizedDeclarator,
                              IdentifierDeclarator }));
}

void ParserTest::case0033()
{
    parse("int ( ( ( x ) ) );",
          Expectation().AST({ TranslationUnit,
                              VariableAndOrFunctionDeclaration,
                              BuiltinTypeSpecifier,
                              ParenthesizedDeclarator,
                              ParenthesizedDeclarator,
                              ParenthesizedDeclarator,
                              IdentifierDeclarator }));
}

void ParserTest::case0034()
{
    parse("x ( y ) ;",
          Expectation().AST({ TranslationUnit,
                              VariableAndOrFunctionDeclaration,
                              TypedefName,
                              ParenthesizedDeclarator,
                              IdentifierDeclarator }));
}

void ParserTest::case0035()
{
    parse("x ( ( y ) );",
          Expectation().AST({ TranslationUnit,
                              VariableAndOrFunctionDeclaration,
                              TypedefName,
                              ParenthesizedDeclarator,
                              ParenthesizedDeclarator,
                              IdentifierDeclarator }));
}

void ParserTest::case0036()
{
    parse("* x ;",
          Expectation()
              .diagnostic(Expectation::ErrorOrWarn::Warn,
                             Parser::DiagnosticsReporter::ID_of_ExpectedTypeSpecifier)
              .AST({ TranslationUnit,
                     VariableAndOrFunctionDeclaration,
                     PointerDeclarator,
                     IdentifierDeclarator }));
}

void ParserTest::case0037()
{
    parse("* ( x ) ;",
          Expectation()
              .diagnostic(Expectation::ErrorOrWarn::Warn,
                             Parser::DiagnosticsReporter::ID_of_ExpectedTypeSpecifier)
              .AST({ TranslationUnit,
                     VariableAndOrFunctionDeclaration,
                     PointerDeclarator,
                     ParenthesizedDeclarator,
                     IdentifierDeclarator }));
}

void ParserTest::case0038()
{
    parse("int ( * x ) ;",
          Expectation().AST({ TranslationUnit,
                              VariableAndOrFunctionDeclaration,
                              BuiltinTypeSpecifier,
                              ParenthesizedDeclarator,
                              PointerDeclarator,
                              IdentifierDeclarator }));
}

void ParserTest::case0039()
{
    parse("x ( * y ) ;",
          Expectation().AST({ TranslationUnit,
                              VariableAndOrFunctionDeclaration,
                              TypedefName,
                              ParenthesizedDeclarator,
                              PointerDeclarator,
                              IdentifierDeclarator }));
}

void ParserTest::case0040()
{
    CROSS_REFERENCE_TEST(BinderTest::case0013); // Semantic warning.

    parse("int ;");
}

void ParserTest::case0041()
{
    parse("x ;",
          Expectation().diagnostic(Expectation::ErrorOrWarn::Warn,
                                      Parser::DiagnosticsReporter::ID_of_ExpectedTypeSpecifier));
}

void ParserTest::case0042()
{
    parse("_Atomic int x ;",
          Expectation().AST({ TranslationUnit,
                              VariableAndOrFunctionDeclaration,
                              AtomicQualifier,
                              BuiltinTypeSpecifier,
                              IdentifierDeclarator }));
}

void ParserTest::case0043()
{
    parse("_Atomic x y ;",
          Expectation().AST({ TranslationUnit,
                              VariableAndOrFunctionDeclaration,
                              AtomicQualifier,
                              TypedefName,
                              IdentifierDeclarator }));
}

void ParserTest::case0044()
{
    parse("_Atomic ( int ) x ;",
          Expectation().AST({ TranslationUnit,
                              VariableAndOrFunctionDeclaration,
                              AtomicTypeSpecifier,
                              TypeName,
                              BuiltinTypeSpecifier,
                              AbstractDeclarator,
                              IdentifierDeclarator }));
}

void ParserTest::case0045()
{
    parse("_Atomic ( x ) y ;",
          Expectation().AST({ TranslationUnit,
                              VariableAndOrFunctionDeclaration,
                              AtomicTypeSpecifier,
                              TypeName,
                              TypedefName,
                              AbstractDeclarator,
                              IdentifierDeclarator }));
}

void ParserTest::case0046()
{
    parse("_Complex double x ;",
          Expectation().AST({ TranslationUnit,
                              VariableAndOrFunctionDeclaration,
                              BuiltinTypeSpecifier,
                              BuiltinTypeSpecifier,
                              IdentifierDeclarator }));
}

void ParserTest::case0047()
{
    parse("_Complex float x ;",
          Expectation().AST({ TranslationUnit,
                              VariableAndOrFunctionDeclaration,
                              BuiltinTypeSpecifier,
                              BuiltinTypeSpecifier,
                              IdentifierDeclarator }));
}

void ParserTest::case0048()
{
    parse("double _Complex x ;",
          Expectation().AST({ TranslationUnit,
                              VariableAndOrFunctionDeclaration,
                              BuiltinTypeSpecifier,
                              BuiltinTypeSpecifier,
                              IdentifierDeclarator }));
}

void ParserTest::case0049()
{
    parse("float _Complex x ;",
          Expectation().AST({ TranslationUnit,
                              VariableAndOrFunctionDeclaration,
                              BuiltinTypeSpecifier,
                              BuiltinTypeSpecifier,
                              IdentifierDeclarator }));
}

void ParserTest::case0050()
{
    // Not a syntactic error, but a semantic one; covered in:
    CROSS_REFERENCE_TEST(BinderTest::case0016);

    parse("double _Complex int x ;",
          Expectation().AST({ TranslationUnit,
                              VariableAndOrFunctionDeclaration,
                              BuiltinTypeSpecifier,
                              BuiltinTypeSpecifier,
                              BuiltinTypeSpecifier,
                              IdentifierDeclarator }));
}

void ParserTest::case0051()
{
    parse("_Complex x ;",
          Expectation().AST({ TranslationUnit,
                              VariableAndOrFunctionDeclaration,
                              BuiltinTypeSpecifier,
                              IdentifierDeclarator }));
}

void ParserTest::case0052()
{
    // Not a syntactic error, but a semantic one; covered in:
    CROSS_REFERENCE_TEST(BinderTest::case0016);

    parse("int int x ;",
          Expectation().AST({ TranslationUnit,
                              VariableAndOrFunctionDeclaration,
                              BuiltinTypeSpecifier,
                              BuiltinTypeSpecifier,
                              IdentifierDeclarator }));
}

void ParserTest::case0053()
{
    // Not a syntactic error, but a semantic one; covered in:
    CROSS_REFERENCE_TEST(BinderTest::case0018);

    parse("_Complex _Complex x ;",
          Expectation().AST({ TranslationUnit,
                              VariableAndOrFunctionDeclaration,
                              BuiltinTypeSpecifier,
                              BuiltinTypeSpecifier,
                              IdentifierDeclarator }));
}

void ParserTest::case0054()
{
    parse("x int ;",
          Expectation().diagnostic(Expectation::ErrorOrWarn::Error,
                                      Parser::DiagnosticsReporter::ID_of_ExpectedFOLLOWofDeclarator)
                       .diagnostic(Expectation::ErrorOrWarn::Warn,
                                      Parser::DiagnosticsReporter::ID_of_ExpectedTypeSpecifier));
}

void ParserTest::case0055()
{
    parse("const x ;",
          Expectation().AST({ TranslationUnit,
                              VariableAndOrFunctionDeclaration,
                              ConstQualifier,
                              IdentifierDeclarator }));
}

void ParserTest::case0056()
{
    parse("const * ;",
          Expectation().diagnostic(Expectation::ErrorOrWarn::Error,
                                      Parser::DiagnosticsReporter::ID_of_ExpectedFIRSTofDirectDeclarator));
}

void ParserTest::case0057()
{

}

void ParserTest::case0058()
{

}

void ParserTest::case0059()
{
}

void ParserTest::case0060()
{
}

void ParserTest::case0061()
{

}

void ParserTest::case0062()
{

}

void ParserTest::case0063()
{
}

void ParserTest::case0064()
{

}

void ParserTest::case0065()
{

}

void ParserTest::case0066()
{

}

void ParserTest::case0067()
{

}

void ParserTest::case0068()
{
}

void ParserTest::case0069()
{

}

void ParserTest::case0070()
{

}

void ParserTest::case0071()
{

}

void ParserTest::case0072()
{
}

void ParserTest::case0073()
{

}

void ParserTest::case0074()
{

}

void ParserTest::case0075()
{

}

void ParserTest::case0076()
{

}

void ParserTest::case0077()
{

}

void ParserTest::case0078()
{

}

void ParserTest::case0079()
{

}

void ParserTest::case0080()
{

}

void ParserTest::case0081()
{
}

void ParserTest::case0082()
{
}

void ParserTest::case0083()
{

}

void ParserTest::case0084()
{

}

void ParserTest::case0085()
{

}

void ParserTest::case0086()
{

}

void ParserTest::case0087()
{

}

void ParserTest::case0088()
{

}

void ParserTest::case0089()
{
}

void ParserTest::case0090()
{
}

void ParserTest::case0091()
{
}

void ParserTest::case0092()
{
}

void ParserTest::case0093()
{

}

void ParserTest::case0094()
{
}

void ParserTest::case0095()
{
}

void ParserTest::case0096()
{
}

void ParserTest::case0097()
{

}

void ParserTest::case0098()
{
}

void ParserTest::case0099()
{

}

void ParserTest::case0100()
{
    parse("void ( * ) ( ) ;",
          Expectation().diagnostic(Expectation::ErrorOrWarn::Error,
                                      Parser::DiagnosticsReporter::ID_of_ExpectedFIRSTofDirectDeclarator));
}

void ParserTest::case0101()
{
    parse("void ( * x ) ( ) ;",
          Expectation().AST({ TranslationUnit,
                              VariableAndOrFunctionDeclaration,
                              BuiltinTypeSpecifier,
                              FunctionDeclarator,
                              ParenthesizedDeclarator,
                              PointerDeclarator,
                              IdentifierDeclarator,
                              ParameterSuffix }));
}

void ParserTest::case0102()
{
    parse("void ( * x ) ( ) { }",
          Expectation().diagnostic(Expectation::ErrorOrWarn::Error,
                                      Parser::DiagnosticsReporter::ID_of_ExpectedFOLLOWofDeclarator));
}

void ParserTest::case0103()
{
    parse("void ( ( * x ) ) ( ) { }",
          Expectation().diagnostic(Expectation::ErrorOrWarn::Error,
                                      Parser::DiagnosticsReporter::ID_of_ExpectedFOLLOWofDeclarator));
}

void ParserTest::case0104()
{
    parse("void ( ( * x ) ) ( ) ;",
          Expectation().AST({ TranslationUnit,
                              VariableAndOrFunctionDeclaration,
                              BuiltinTypeSpecifier,
                              FunctionDeclarator,
                              ParenthesizedDeclarator,
                              ParenthesizedDeclarator,
                              PointerDeclarator,
                              IdentifierDeclarator,
                              ParameterSuffix }));
}

void ParserTest::case0105()
{
    parse("void ( * ) ( int ) ;",
          Expectation().diagnostic(Expectation::ErrorOrWarn::Error,
                                      Parser::DiagnosticsReporter::ID_of_ExpectedFIRSTofDirectDeclarator));
}

void ParserTest::case0106()
{
    parse("void ( * x ) ( int ) ;") ;
}

void ParserTest::case0107()
{
    parse("x ( * y ) ( int ) ;") ;
}

void ParserTest::case0108()
{
    // The `signal' function.

    parse("void ( * x ( int , void ( * y ) ( int ) ) ) ( int ) ;") ;
}

void ParserTest::case0109()
{
    parse("void ( ( * x ) ) ( ) ;",
          Expectation().AST({ TranslationUnit,
                              VariableAndOrFunctionDeclaration,
                              BuiltinTypeSpecifier,
                              FunctionDeclarator,
                              ParenthesizedDeclarator,
                              ParenthesizedDeclarator,
                              PointerDeclarator,
                              IdentifierDeclarator,
                              ParameterSuffix }));
}

void ParserTest::case0110()
{
    parse("void ( ( ( * x ) ( ) ) );",
          Expectation().AST({ TranslationUnit,
                              VariableAndOrFunctionDeclaration,
                              BuiltinTypeSpecifier,
                              ParenthesizedDeclarator,
                              ParenthesizedDeclarator,
                              FunctionDeclarator,
                              ParenthesizedDeclarator,
                              PointerDeclarator,
                              IdentifierDeclarator,
                              ParameterSuffix }));
}

void ParserTest::case0111()
{
    parse("void ( ( ( ( * x ) ) ( ) ) );",
          Expectation().AST({ TranslationUnit,
                              VariableAndOrFunctionDeclaration,
                              BuiltinTypeSpecifier,
                              ParenthesizedDeclarator,
                              ParenthesizedDeclarator,
                              FunctionDeclarator,
                              ParenthesizedDeclarator,
                              ParenthesizedDeclarator,
                              PointerDeclarator,
                              IdentifierDeclarator,
                              ParameterSuffix }));
}

void ParserTest::case0112()
{
    parse("int ( * x ) [ 4 ] [ y + 1 ] ;");
}

void ParserTest::case0113()
{
    parse("void ( * * * x ) ( int ) ;");
}

void ParserTest::case0114()
{
    parse("x ( * * * y ) ( z ) ;");
}

void ParserTest::case0115()
{
    parse("x ( * ( * ( * y ) ) ) ( z ) ;");
}

void ParserTest::case0116()
{
    parse("int ( * x ) [ 1 ] ;",
          Expectation().AST({ TranslationUnit,
                              VariableAndOrFunctionDeclaration,
                              BuiltinTypeSpecifier,
                              ArrayDeclarator,
                              ParenthesizedDeclarator,
                              PointerDeclarator,
                              IdentifierDeclarator,
                              SubscriptSuffix,
                              IntegerConstantExpression }));
}

void ParserTest::case0117()
{
    parse("x ( * y ) [ 1 ];",
          Expectation().AST({ TranslationUnit,
                              VariableAndOrFunctionDeclaration,
                              TypedefName,
                              ArrayDeclarator,
                              ParenthesizedDeclarator,
                              PointerDeclarator,
                              IdentifierDeclarator,
                              SubscriptSuffix,
                              IntegerConstantExpression }));
}

void ParserTest::case0118()
{
    parse("int ( * x ) [ 1 ] [ 3 ] ;",
          Expectation().AST({ TranslationUnit,
                              VariableAndOrFunctionDeclaration,
                              BuiltinTypeSpecifier,
                              ArrayDeclarator,
                              ArrayDeclarator,
                              ParenthesizedDeclarator,
                              PointerDeclarator,
                              IdentifierDeclarator,
                              SubscriptSuffix,
                              IntegerConstantExpression,
                              SubscriptSuffix,
                              IntegerConstantExpression }));
}

void ParserTest::case0119()
{
    parse("void ( * x ) ( ) = 0 ;",
          Expectation().AST({ TranslationUnit,
                              VariableAndOrFunctionDeclaration,
                              BuiltinTypeSpecifier,
                              FunctionDeclarator,
                              ParenthesizedDeclarator,
                              PointerDeclarator,
                              IdentifierDeclarator,
                              ParameterSuffix,
                              ExpressionInitializer,
                              IntegerConstantExpression }));
}

void ParserTest::case0120()
{
    parse("void ( * * x ) ( ) = 0 ;",
          Expectation().AST({ TranslationUnit,
                              VariableAndOrFunctionDeclaration,
                              BuiltinTypeSpecifier,
                              FunctionDeclarator,
                              ParenthesizedDeclarator,
                              PointerDeclarator,
                              PointerDeclarator,
                              IdentifierDeclarator,
                              ParameterSuffix,
                              ExpressionInitializer,
                              IntegerConstantExpression }));
}

void ParserTest::case0121()
{
    parse("void ( * ( x ) ) ( ) = 0 ;",
          Expectation().AST({ TranslationUnit,
                              VariableAndOrFunctionDeclaration,
                              BuiltinTypeSpecifier,
                              FunctionDeclarator,
                              ParenthesizedDeclarator,
                              PointerDeclarator,
                              ParenthesizedDeclarator,
                              IdentifierDeclarator,
                              ParameterSuffix,
                              ExpressionInitializer,
                              IntegerConstantExpression }));
}

void ParserTest::case0122()
{
    parse("void ( * * ( x ) ) ( ) = 0 ;");
}

void ParserTest::case0123()
{
    parse("void ( * ( * ( x ) ) ) ( ) = 0 ;");
}

void ParserTest::case0124()
{

}

void ParserTest::case0125()
{

}

void ParserTest::case0126()
{

}

void ParserTest::case0127()
{

}

void ParserTest::case0128()
{

}

void ParserTest::case0129()
{

}

void ParserTest::case0130()
{

}

void ParserTest::case0131()
{

}

void ParserTest::case0132()
{

}

void ParserTest::case0133()
{

}

void ParserTest::case0134()
{

}

void ParserTest::case0135()
{

}

void ParserTest::case0136()
{

}

void ParserTest::case0137()
{

}

void ParserTest::case0138()
{

}

void ParserTest::case0139()
{

}

void ParserTest::case0140()
{

}

void ParserTest::case0141()
{

}

void ParserTest::case0142()
{

}

void ParserTest::case0143()
{

}

void ParserTest::case0144()
{

}

void ParserTest::case0145()
{

}

void ParserTest::case0146()
{

}

void ParserTest::case0147()
{

}

void ParserTest::case0148()
{

}

void ParserTest::case0149()
{

}

void ParserTest::case0150()
{

}

void ParserTest::case0151()
{

}

void ParserTest::case0152()
{

}

void ParserTest::case0153()
{

}

void ParserTest::case0154()
{

}

void ParserTest::case0155()
{

}

void ParserTest::case0156()
{

}

void ParserTest::case0157()
{

}

void ParserTest::case0158()
{

}

void ParserTest::case0159()
{

}

void ParserTest::case0160()
{

}

void ParserTest::case0161()
{

}

void ParserTest::case0162()
{

}

void ParserTest::case0163()
{

}

void ParserTest::case0164()
{

}

void ParserTest::case0165()
{

}

void ParserTest::case0166()
{

}

void ParserTest::case0167()
{

}

void ParserTest::case0168()
{

}

void ParserTest::case0169()
{

}

void ParserTest::case0170()
{

}

void ParserTest::case0171()
{

}

void ParserTest::case0172()
{

}

void ParserTest::case0173()
{

}

void ParserTest::case0174()
{

}

void ParserTest::case0175()
{

}

void ParserTest::case0176()
{

}

void ParserTest::case0177()
{

}

void ParserTest::case0178()
{

}

void ParserTest::case0179()
{

}

void ParserTest::case0180()
{

}

void ParserTest::case0181()
{

}

void ParserTest::case0182()
{

}

void ParserTest::case0183()
{

}

void ParserTest::case0184()
{

}

void ParserTest::case0185()
{

}

void ParserTest::case0186()
{

}

void ParserTest::case0187()
{

}

void ParserTest::case0188()
{

}

void ParserTest::case0189()
{

}

void ParserTest::case0190()
{

}

void ParserTest::case0191()
{

}

void ParserTest::case0192()
{

}

void ParserTest::case0193()
{

}

void ParserTest::case0194()
{

}

void ParserTest::case0195()
{

}

void ParserTest::case0196()
{

}

void ParserTest::case0197()
{

}

void ParserTest::case0198()
{

}

void ParserTest::case0199()
{

}

void ParserTest::case0200()
{
    parse("void x ( ) ;",
          Expectation().AST({ TranslationUnit,
                              VariableAndOrFunctionDeclaration,
                              BuiltinTypeSpecifier,
                              FunctionDeclarator,
                              IdentifierDeclarator,
                              ParameterSuffix }));
}

void ParserTest::case0201()
{
    parse("void x ( ) { }",
          Expectation().AST({ TranslationUnit,
                              FunctionDefinition,
                              BuiltinTypeSpecifier,
                              FunctionDeclarator,
                              IdentifierDeclarator,
                              ParameterSuffix,
                              CompoundStatement }));
}

void ParserTest::case0202()
{
    parse("void x { }",
          Expectation().diagnostic(Expectation::ErrorOrWarn::Error,
                                      Parser::DiagnosticsReporter::ID_of_ExpectedFOLLOWofDeclarator));
}

void ParserTest::case0203()
{
    parse("void x ( ) = 1 { }",
          Expectation().diagnostic(Expectation::ErrorOrWarn::Error,
                                      Parser::DiagnosticsReporter::ID_of_UnexpectedInitializerOfDeclarator));
}

void ParserTest::case0204()
{
    parse("void ( x ) ( ) { }",
          Expectation().AST({ TranslationUnit,
                              FunctionDefinition,
                              BuiltinTypeSpecifier,
                              FunctionDeclarator,
                              ParenthesizedDeclarator,
                              IdentifierDeclarator,
                              ParameterSuffix,
                              CompoundStatement }));
}

void ParserTest::case0205()
{
    // Not a syntactic error, but a semantic one; covered in:
    CROSS_REFERENCE_TEST(BinderTest::case0008) ;

    parse("void x ( ) ( ) { }") ;
}

void ParserTest::case0206()
{
    // Not a syntactic error, but a semantic one; covered in:
    CROSS_REFERENCE_TEST(BinderTest::case0009) ;

    parse("void x ( ) ( ) ;") ;
}

void ParserTest::case0207()
{
    // Not a syntactic error, but a semantic one; covered in:
    CROSS_REFERENCE_TEST(BinderTest::case0010) ;

    parse("int x ( ) [ ] ;") ;
}

void ParserTest::case0208()
{
    // Not a syntactic error, but a semantic one; covered in:
    CROSS_REFERENCE_TEST(ParserTest::case0111) ;

    parse("int ( x ) ( ) [ ] ;") ;
}

void ParserTest::case0209()
{
    parse("int x ( void ) ;",
          Expectation().AST({ TranslationUnit,
                              VariableAndOrFunctionDeclaration,
                              BuiltinTypeSpecifier,
                              FunctionDeclarator,
                              IdentifierDeclarator,
                              ParameterSuffix,
                              ParameterDeclaration,
                              BuiltinTypeSpecifier,
                              AbstractDeclarator }));
}

void ParserTest::case0210()
{
    parse("int x ( void ) { }",
          Expectation().AST({ TranslationUnit,
                              FunctionDefinition,
                              BuiltinTypeSpecifier,
                              FunctionDeclarator,
                              IdentifierDeclarator,
                              ParameterSuffix,
                              ParameterDeclaration,
                              BuiltinTypeSpecifier,
                              AbstractDeclarator,
                              CompoundStatement }));
}

void ParserTest::case0211()
{
    parse("void x ( int ) ;",
          Expectation().AST({ TranslationUnit,
                              VariableAndOrFunctionDeclaration,
                              BuiltinTypeSpecifier,
                              FunctionDeclarator,
                              IdentifierDeclarator,
                              ParameterSuffix,
                              ParameterDeclaration,
                              BuiltinTypeSpecifier,
                              AbstractDeclarator }));
}

void ParserTest::case0212()
{
    // Not a syntactic error, but a semantic one; covered in:
    CROSS_REFERENCE_TEST(BinderTest::case0012) ;

    parse("void x ( int ) { }") ;
}

void ParserTest::case0213()
{
    parse("void x ( int y ) { }",
          Expectation().AST({ TranslationUnit,
                              FunctionDefinition,
                              BuiltinTypeSpecifier,
                              FunctionDeclarator,
                              IdentifierDeclarator,
                              ParameterSuffix,
                              ParameterDeclaration,
                              BuiltinTypeSpecifier,
                              IdentifierDeclarator,
                              CompoundStatement }));
}

void ParserTest::case0214()
{
    parse("double x( int y , double z ) ;") ;
}

void ParserTest::case0215()
{
    parse("double x ( double ) ;",
          Expectation().AST({ TranslationUnit,
                              VariableAndOrFunctionDeclaration,
                              BuiltinTypeSpecifier,
                              FunctionDeclarator,
                              IdentifierDeclarator,
                              ParameterSuffix,
                              ParameterDeclaration,
                              BuiltinTypeSpecifier,
                              AbstractDeclarator }));
}

void ParserTest::case0216()
{
    parse("x ( int y ) ;",
          Expectation().diagnostic(Expectation::ErrorOrWarn::Warn,
                                      Parser::DiagnosticsReporter::ID_of_ExpectedTypeSpecifier));
}

void ParserTest::case0217()
{
    parse("x ( y z ) ;",
          Expectation().diagnostic(Expectation::ErrorOrWarn::Warn,
                                      Parser::DiagnosticsReporter::ID_of_ExpectedTypeSpecifier));
}

void ParserTest::case0218()
{
    parse("void x ( y ) ;") ;
}

void ParserTest::case0219()
{
    parse("void x ( y z ) ;") ;
}

void ParserTest::case0220()
{
    parse("void x ( ( int z ));",
          Expectation().diagnostic(Expectation::ErrorOrWarn::Warn,
                                      Parser::DiagnosticsReporter::ID_of_ExpectedTypeSpecifier));
}

void ParserTest::case0221()
{
    parse("x y ( void ) { }",
          Expectation().AST({ TranslationUnit,
                              FunctionDefinition,
                              TypedefName,
                              FunctionDeclarator,
                              IdentifierDeclarator,
                              ParameterSuffix,
                              ParameterDeclaration,
                              BuiltinTypeSpecifier,
                              AbstractDeclarator,
                              CompoundStatement }));
}

void ParserTest::case0222()
{
    parse("void x ( int * ) ;") ;
}

void ParserTest::case0223()
{
    parse("void x ( int ( * ));") ;
}

void ParserTest::case0224()
{
    parse("void x ( int * [ 10 ] ) ;") ;
}

void ParserTest::case0225()
{
    parse("void x (int ( * ) [ 10 ] ) ;") ;
}

void ParserTest::case0226()
{
    parse("void x ( int ( * ) [ * ] ) ;") ;
}

void ParserTest::case0227()
{
    parse("void x ( int * ( ));") ;
}

void ParserTest::case0228()
{
    parse("void x ( int * ( void ));") ;
}

void ParserTest::case0229()
{
    parse("void x ( int * ( int ));") ;
}

void ParserTest::case0230()
{
    parse("void x ( int * ( int , double ));") ;
}

void ParserTest::case0231()
{
    parse("void x ( int ( * const [ ] ) ( unsigned int , ... ));") ;
}

void ParserTest::case0232()
{
    parse("int x , y ( ) ;",
          Expectation().AST({ TranslationUnit,
                              VariableAndOrFunctionDeclaration,
                              BuiltinTypeSpecifier,
                              IdentifierDeclarator,
                              FunctionDeclarator,
                              IdentifierDeclarator,
                              ParameterSuffix }));
}

void ParserTest::case0233()
{
    parse("void ( ( x ) ) ( ) { }",
          Expectation().AST({ TranslationUnit,
                              FunctionDefinition,
                              BuiltinTypeSpecifier,
                              FunctionDeclarator,
                              ParenthesizedDeclarator,
                              ParenthesizedDeclarator,
                              IdentifierDeclarator,
                              ParameterSuffix,
                              CompoundStatement }));
}

void ParserTest::case0234()
{
    parse("void ( ( ( x ) ) ( ) ) { }",
          Expectation().AST({ TranslationUnit,
                              FunctionDefinition,
                              BuiltinTypeSpecifier,
                              ParenthesizedDeclarator,
                              FunctionDeclarator,
                              ParenthesizedDeclarator,
                              ParenthesizedDeclarator,
                              IdentifierDeclarator,
                              ParameterSuffix,
                              CompoundStatement }));
}

void ParserTest::case0235()
{
    parse("inline double x ( double y ) { }",
          Expectation().AST({ TranslationUnit,
                              FunctionDefinition,
                              InlineSpecifier,
                              BuiltinTypeSpecifier,
                              FunctionDeclarator,
                              IdentifierDeclarator,
                              ParameterSuffix,
                              ParameterDeclaration,
                              BuiltinTypeSpecifier,
                              IdentifierDeclarator,
                              CompoundStatement }));
}

void ParserTest::case0236()
{
    parse("inline x y ( double z ) { }",
          Expectation().AST({ TranslationUnit,
                              FunctionDefinition,
                              InlineSpecifier,
                              TypedefName,
                              FunctionDeclarator,
                              IdentifierDeclarator,
                              ParameterSuffix,
                              ParameterDeclaration,
                              BuiltinTypeSpecifier,
                              IdentifierDeclarator,
                              CompoundStatement }));
}

void ParserTest::case0237()
{
    parse("_Noreturn x y ( ) { }") ;
}

void ParserTest::case0238()
{
    parse("_Noreturn void x ( ) { }") ;
}

void ParserTest::case0239()
{
    parse("int * x ( ) ;",
          Expectation().AST({ TranslationUnit,
                              VariableAndOrFunctionDeclaration,
                              BuiltinTypeSpecifier,
                              PointerDeclarator,
                              FunctionDeclarator,
                              IdentifierDeclarator,
                              ParameterSuffix }));
}

void ParserTest::case0240()
{
    parse("x * y ( ) ;",
          Expectation().AST({ TranslationUnit,
                              VariableAndOrFunctionDeclaration,
                              TypedefName,
                              PointerDeclarator,
                              FunctionDeclarator,
                              IdentifierDeclarator,
                              ParameterSuffix }));
}

void ParserTest::case0241()
{
    parse("int * x ( ) { }",
          Expectation().AST({ TranslationUnit,
                              FunctionDefinition,
                              BuiltinTypeSpecifier,
                              PointerDeclarator,
                              FunctionDeclarator,
                              IdentifierDeclarator,
                              ParameterSuffix,
                              CompoundStatement }));
}

void ParserTest::case0242()
{
    parse("x * y ( ) { }",
          Expectation().AST({ TranslationUnit,
                              FunctionDefinition,
                              TypedefName,
                              PointerDeclarator,
                              FunctionDeclarator,
                              IdentifierDeclarator,
                              ParameterSuffix,
                              CompoundStatement }));
}

void ParserTest::case0243()
{
    parse("int [ ] x ( ) { }",
          Expectation().diagnostic(Expectation::ErrorOrWarn::Error,
                                      Parser::DiagnosticsReporter::ID_of_ExpectedFIRSTofDirectDeclarator));
}

void ParserTest::case0244()
{
    // A function taking a parameter of type double and returning
    // a pointer to a function taking int as a parameter and
    // returning void.

    parse("void ( * x ( double ) ) ( int ) { }",
          Expectation().AST({ TranslationUnit,
                              FunctionDefinition,
                              BuiltinTypeSpecifier,
                              FunctionDeclarator,
                              ParenthesizedDeclarator,
                              PointerDeclarator,
                              FunctionDeclarator,
                              IdentifierDeclarator,
                              ParameterSuffix,
                              ParameterDeclaration,
                              BuiltinTypeSpecifier,
                              AbstractDeclarator,
                              ParameterSuffix,
                              ParameterDeclaration,
                              BuiltinTypeSpecifier,
                              AbstractDeclarator,
                              CompoundStatement }));
}

void ParserTest::case0245()
{

}

void ParserTest::case0246()
{

}

void ParserTest::case0247()
{

}

void ParserTest::case0248()
{

}

void ParserTest::case0249()
{

}

void ParserTest::case0250()
{

}

void ParserTest::case0251()
{

}

void ParserTest::case0252()
{

}

void ParserTest::case0253()
{

}

void ParserTest::case0254()
{

}

void ParserTest::case0255()
{

}

void ParserTest::case0256()
{

}

void ParserTest::case0257()
{

}

void ParserTest::case0258()
{

}

void ParserTest::case0259()
{

}

void ParserTest::case0260()
{

}

void ParserTest::case0261()
{

}

void ParserTest::case0262()
{

}

void ParserTest::case0263()
{

}

void ParserTest::case0264()
{

}

void ParserTest::case0265()
{

}

void ParserTest::case0266()
{

}

void ParserTest::case0267()
{

}

void ParserTest::case0268()
{

}

void ParserTest::case0269()
{

}

void ParserTest::case0270()
{

}

void ParserTest::case0271()
{

}

void ParserTest::case0272()
{

}

void ParserTest::case0273()
{

}

void ParserTest::case0274()
{

}

void ParserTest::case0275()
{

}

void ParserTest::case0276()
{

}

void ParserTest::case0277()
{

}

void ParserTest::case0278()
{

}

void ParserTest::case0279()
{

}

void ParserTest::case0280()
{

}

void ParserTest::case0281()
{

}

void ParserTest::case0282()
{

}

void ParserTest::case0283()
{

}

void ParserTest::case0284()
{

}

void ParserTest::case0285()
{

}

void ParserTest::case0286()
{

}

void ParserTest::case0287()
{

}

void ParserTest::case0288()
{

}

void ParserTest::case0289()
{

}

void ParserTest::case0290()
{

}

void ParserTest::case0291()
{

}

void ParserTest::case0292()
{

}

void ParserTest::case0293()
{

}

void ParserTest::case0294()
{

}

void ParserTest::case0295()
{

}

void ParserTest::case0296()
{

}

void ParserTest::case0297()
{

}

void ParserTest::case0298()
{

}

void ParserTest::case0299()
{

}

void ParserTest::case0300()
{
    parse("int x [ y ] ;",
          Expectation().AST({ TranslationUnit,
                              VariableAndOrFunctionDeclaration,
                              BuiltinTypeSpecifier,
                              ArrayDeclarator,
                              IdentifierDeclarator,
                              SubscriptSuffix,
                              IdentifierName }));
}

void ParserTest::case0301()
{
    parse("x y [ z ] ;",
          Expectation().AST({ TranslationUnit,
                              VariableAndOrFunctionDeclaration,
                              TypedefName,
                              ArrayDeclarator,
                              IdentifierDeclarator,
                              SubscriptSuffix,
                              IdentifierName }));
}

void ParserTest::case0302()
{
    parse("int x [ 2 ] ;",
          Expectation().AST({ TranslationUnit,
                              VariableAndOrFunctionDeclaration,
                              BuiltinTypeSpecifier,
                              ArrayDeclarator,
                              IdentifierDeclarator,
                              SubscriptSuffix,
                              IntegerConstantExpression }));
}

void ParserTest::case0303()
{
    parse("x y [ 2 ] ;",
          Expectation().AST({ TranslationUnit,
                              VariableAndOrFunctionDeclaration,
                              TypedefName,
                              ArrayDeclarator,
                              IdentifierDeclarator,
                              SubscriptSuffix,
                              IntegerConstantExpression }));
}

void ParserTest::case0304()
{
    parse("float x [ 11 ] , * y [ 17 ] ;") ;
}

void ParserTest::case0305()
{
    parse("int x [ y ] [ 6 ] [ z ] ;",
          Expectation().AST({ TranslationUnit,
                              VariableAndOrFunctionDeclaration,
                              BuiltinTypeSpecifier,
                              ArrayDeclarator,
                              ArrayDeclarator,
                              ArrayDeclarator,
                              IdentifierDeclarator,
                              SubscriptSuffix,
                              IdentifierName,
                              SubscriptSuffix,
                              IntegerConstantExpression,
                              SubscriptSuffix,
                              IdentifierName }));
}

void ParserTest::case0306()
{
    parse("int x [ y ] [ z ] [ 6 ] ;",
          Expectation().AST({ TranslationUnit,
                              VariableAndOrFunctionDeclaration,
                              BuiltinTypeSpecifier,
                              ArrayDeclarator,
                              ArrayDeclarator,
                              ArrayDeclarator,
                              IdentifierDeclarator,
                              SubscriptSuffix,
                              IdentifierName,
                              SubscriptSuffix,
                              IdentifierName,
                              SubscriptSuffix,
                              IntegerConstantExpression }));
}

void ParserTest::case0307()
{
    parse("int x [ y + z ] ;") ;
}

void ParserTest::case0308()
{
    parse("int x [ y ] [ z ] [ 6 ] [ w ];") ;
}

void ParserTest::case0309()
{
    parse("int x [ y ] [ z ] [ 6 ] [ w + 2 ] ;") ;
}

void ParserTest::case0310()
{
    parse("int ( * const x [ 10 ] ) ( unsigned int , ... ) ;") ;
}

void ParserTest::case0311()
{
    parse("int ( * const [ ] ) ( unsigned int , ... ) ;",
          Expectation().diagnostic(Expectation::ErrorOrWarn::Error,
                                      Parser::DiagnosticsReporter::ID_of_ExpectedFIRSTofDirectDeclarator));
}

void ParserTest::case0312()
{
    // TODO: Warn about array size.

    parse("int ( * const x [ ] ) ( unsigned int , ... ) ;") ;
}

void ParserTest::case0313()
{
    parse("int * x [ 1 ];",
          Expectation().AST({ TranslationUnit,
                              VariableAndOrFunctionDeclaration,
                              BuiltinTypeSpecifier,
                              PointerDeclarator,
                              ArrayDeclarator,
                              IdentifierDeclarator,
                              SubscriptSuffix,
                              IntegerConstantExpression }));
}

void ParserTest::case0314()
{
    parse("void x ( int y [ static 1 ] ) ;",
          Expectation().AST({ TranslationUnit,
                              VariableAndOrFunctionDeclaration,
                              BuiltinTypeSpecifier,
                              FunctionDeclarator,
                              IdentifierDeclarator,
                              ParameterSuffix,
                              ParameterDeclaration,
                              BuiltinTypeSpecifier,
                              ArrayDeclarator,
                              IdentifierDeclarator,
                              SubscriptSuffix,
                              IntegerConstantExpression }));
}

void ParserTest::case0315()
{
    parse("void x ( int [ static 1 ] ) ;",
          Expectation().AST({ TranslationUnit,
                              VariableAndOrFunctionDeclaration,
                              BuiltinTypeSpecifier,
                              FunctionDeclarator,
                              IdentifierDeclarator,
                              ParameterSuffix,
                              ParameterDeclaration,
                              BuiltinTypeSpecifier,
                              ArrayDeclarator,
                              SubscriptSuffix,
                              IntegerConstantExpression }));
}

void ParserTest::case0316()
{
    parse("int x [ static 1 ] ;",
          Expectation().diagnostic(Expectation::ErrorOrWarn::Error,
                                      Parser::DiagnosticsReporter::ID_of_UnexpectedStaticOrTypeQualifierInArrayDeclarator));
}

void ParserTest::case0317()
{
    parse("int x ( const int y [ const 1 ] ) ;");
}

void ParserTest::case0318()
{
    parse("int x ( const int [ const 1 ] ) ;");
}

void ParserTest::case0319()
{
    parse("void x ( int y [ static restrict 1 ] ) ;");
}

void ParserTest::case0320()
{
    parse("void x ( int [ static restrict 1 ] ) ;");
}

void ParserTest::case0321()
{
    parse("void x ( int [ static struct ] ) ;",
          Expectation().diagnostic(Expectation::ErrorOrWarn::Error,
                                      Parser::DiagnosticsReporter::ID_of_ExpectedFIRSTofExpression));
}

void ParserTest::case0322()
{
    parse("int x ( int y [ static const 1 ] ) ;");
}

void ParserTest::case0323()
{
    parse("int x ( int [ static const 1 ] ) ;");
}

void ParserTest::case0324()
{
    parse("int x ( int y [ const static 1 ] ) ;");
}

void ParserTest::case0325()
{
    parse("int x ( int [ const static 1 ] ) ;");
}

void ParserTest::case0326()
{
    parse("int x ( int y [ const * ] ) ;");
}

void ParserTest::case0327()
{
    parse("int x ( int y [ const volatile * ] ) ;");
}

void ParserTest::case0328()
{
    parse("int x ( int y [ const static volatile * ] ) ;",
          Expectation().diagnostic(Expectation::ErrorOrWarn::Error,
                                      Parser::DiagnosticsReporter::ID_of_ExpectedFIRSTofDirectDeclarator));
}

void ParserTest::case0329()
{
    parse("int y [ const  * ] ;",
          Expectation().setErrorCnt(1));
}

void ParserTest::case0330()
{
    parse("int y [ * ] ;",
          Expectation().setErrorCnt(1));
}

void ParserTest::case0331()
{
    parse("int y [ const ] ;",
          Expectation().diagnostic(Expectation::ErrorOrWarn::Error,
                                      Parser::DiagnosticsReporter::ID_of_UnexpectedStaticOrTypeQualifierInArrayDeclarator));
}

void ParserTest::case0332()
{
    parse("int x [ 1 ] const ;",
          Expectation().diagnostic(Expectation::ErrorOrWarn::Error,
                                      Parser::DiagnosticsReporter::ID_of_ExpectedFOLLOWofDeclarator));
}

void ParserTest::case0333()
{
    parse("int x [ 1 ] y ;",
          Expectation().diagnostic(Expectation::ErrorOrWarn::Error,
                                      Parser::DiagnosticsReporter::ID_of_ExpectedFOLLOWofDeclarator));
}

void ParserTest::case0334()
{
}

void ParserTest::case0335()
{
}

void ParserTest::case0336()
{
}

void ParserTest::case0337()
{

}

void ParserTest::case0338()
{
}

void ParserTest::case0339()
{
}

void ParserTest::case0340()
{
}

void ParserTest::case0341()
{
}

void ParserTest::case0342() { }
void ParserTest::case0343() { }
void ParserTest::case0344() { }
void ParserTest::case0345() { }
void ParserTest::case0346() { }
void ParserTest::case0347() { }
void ParserTest::case0348() { }
void ParserTest::case0349() { }

void ParserTest::case0350() { }
void ParserTest::case0351() { }
void ParserTest::case0352() { }
void ParserTest::case0353() { }
void ParserTest::case0354() { }
void ParserTest::case0355() { }
void ParserTest::case0356() { }
void ParserTest::case0357() { }
void ParserTest::case0358() { }
void ParserTest::case0359() { }

void ParserTest::case0360() { }
void ParserTest::case0361() { }
void ParserTest::case0362() { }
void ParserTest::case0363() { }
void ParserTest::case0364() { }
void ParserTest::case0365() { }
void ParserTest::case0366() { }
void ParserTest::case0367() { }
void ParserTest::case0368() { }
void ParserTest::case0369() { }

void ParserTest::case0370() { }
void ParserTest::case0371() { }
void ParserTest::case0372() { }
void ParserTest::case0373() { }
void ParserTest::case0374() { }
void ParserTest::case0375() { }
void ParserTest::case0376() { }
void ParserTest::case0377() { }
void ParserTest::case0378() { }
void ParserTest::case0379() { }

void ParserTest::case0380() { }
void ParserTest::case0381() { }
void ParserTest::case0382() { }
void ParserTest::case0383() { }
void ParserTest::case0384() { }
void ParserTest::case0385() { }
void ParserTest::case0386() { }
void ParserTest::case0387() { }
void ParserTest::case0388() { }
void ParserTest::case0389() { }

void ParserTest::case0390() { }
void ParserTest::case0391() { }
void ParserTest::case0392() { }
void ParserTest::case0393() { }
void ParserTest::case0394() { }
void ParserTest::case0395() { }
void ParserTest::case0396() { }
void ParserTest::case0397() { }
void ParserTest::case0398() { }
void ParserTest::case0399() { }

void ParserTest::case0400()
{
    parse("struct x ;",
          Expectation().AST({ TranslationUnit,
                              StructDeclaration,
                              StructTypeSpecifier }));
}

void ParserTest::case0401()
{
    parse("struct x y ;",
          Expectation().AST({ TranslationUnit,
                              VariableAndOrFunctionDeclaration,
                              StructTypeSpecifier,
                              IdentifierDeclarator }));
}

void ParserTest::case0402()
{
    parse("struct x * y ;",
          Expectation().AST({ TranslationUnit,
                              VariableAndOrFunctionDeclaration,
                              StructTypeSpecifier,
                              PointerDeclarator,
                              IdentifierDeclarator }));
}

void ParserTest::case0403()
{
    // TODO: Warn of empty member list.

    parse("struct x { } ;") ;
}

void ParserTest::case0404()
{
    parse("struct { int x ; } y ;",
          Expectation().AST({ TranslationUnit,
                              VariableAndOrFunctionDeclaration,
                              TypeDeclarationAsSpecifier,
                              StructTypeSpecifier,
                              FieldDeclaration,
                              BuiltinTypeSpecifier,
                              IdentifierDeclarator,
                              IdentifierDeclarator }));
}

void ParserTest::case0405()
{
    parse("struct { x y ; } z;",
          Expectation().AST({ TranslationUnit,
                              VariableAndOrFunctionDeclaration,
                              TypeDeclarationAsSpecifier,
                              StructTypeSpecifier,
                              FieldDeclaration,
                              TypedefName,
                              IdentifierDeclarator,
                              IdentifierDeclarator }));
}

void ParserTest::case0406()
{
    CROSS_REFERENCE_TEST(BinderTest::case0014) ;

    parse("struct { int x ; } ;",
          Expectation().AST({ TranslationUnit,
                              StructDeclaration,
                              StructTypeSpecifier,
                              FieldDeclaration,
                              BuiltinTypeSpecifier,
                              IdentifierDeclarator }));
}

void ParserTest::case0407()
{
    parse("struct { x y ; } ;",
          Expectation().AST({ TranslationUnit,
                              StructDeclaration,
                              StructTypeSpecifier,
                              FieldDeclaration,
                              TypedefName,
                              IdentifierDeclarator }));
}

void ParserTest::case0408()
{
    parse("struct x { void ( * y ) ( ) ; } ;",
          Expectation().AST({ TranslationUnit,
                              StructDeclaration,
                              StructTypeSpecifier,
                              FieldDeclaration,
                              BuiltinTypeSpecifier,
                              FunctionDeclarator,
                              ParenthesizedDeclarator,
                              PointerDeclarator,
                              IdentifierDeclarator,
                              ParameterSuffix }));
}

void ParserTest::case0409()
{
    parse("struct;",
          Expectation().diagnostic(Expectation::ErrorOrWarn::Error,
                                      Parser::DiagnosticsReporter::ID_of_ExpectedFOLLOWofStructOrUnionOrEnum));
}

void ParserTest::case0410()
{
    parse("struct x { void ( * y ) ( int , double ) ; } ; ") ;
}

void ParserTest::case0411()
{
    parse("struct x { void ( * y ) ( z , double ) ; } ; ") ;
}

void ParserTest::case0412()
{
    parse("struct x { void ( * y ) ( z ) ; } ; ") ;
}

void ParserTest::case0413()
{
    parse("struct x { void ( * ) ( int ) ; } ;",
          Expectation().diagnostic(Expectation::ErrorOrWarn::Error,
                                      Parser::DiagnosticsReporter::ID_of_ExpectedFIRSTofDirectDeclarator));
}

void ParserTest::case0414()
{
    parse("struct x (",
          Expectation().setErrorCnt(1)) ;
}

void ParserTest::case0415()
{
    parse("struct x { :",
          Expectation().diagnostic(Expectation::ErrorOrWarn::Error,
                                      Parser::DiagnosticsReporter::ID_of_ExpectedFIRSTofSpecifierQualifier));
}

void ParserTest::case0416()
{
    /*
     * Notes:
     *     - `typedef int y; struct x { y ; } ;'
     *        Would be valid, but the declaration doesn't declare anything.
     *
     *     - `struct x { y ; } ;'
     *        Would be invalid, because a specifier-qualifier-list is mandatory.
     */

    CROSS_REFERENCE_TEST(BinderTest::case0006); // Semantic warning.

    parse("struct x { y ; } ;");
}

void ParserTest::case0417()
{
    parse("struct x { ( y ) ; } ;",
          Expectation().diagnostic(Expectation::ErrorOrWarn::Error,
                                      Parser::DiagnosticsReporter::ID_of_ExpectedFIRSTofSpecifierQualifier));
}

void ParserTest::case0418()
{
    parse("struct x { int y : 1 ; } ;",
          Expectation().AST({ TranslationUnit,
                              StructDeclaration,
                              StructTypeSpecifier,
                              FieldDeclaration,
                              BuiltinTypeSpecifier,
                              BitfieldDeclarator,
                              IdentifierDeclarator,
                              IntegerConstantExpression }));
}

void ParserTest::case0419()
{
    parse("struct x { y z : 1 ; } ;",
          Expectation().AST({ TranslationUnit,
                              StructDeclaration,
                              StructTypeSpecifier,
                              FieldDeclaration,
                              TypedefName,
                              BitfieldDeclarator,
                              IdentifierDeclarator,
                              IntegerConstantExpression }));
}

void ParserTest::case0420()
{
    parse("struct x { int : 1 ; } ;",
          Expectation().AST({ TranslationUnit,
                              StructDeclaration,
                              StructTypeSpecifier,
                              FieldDeclaration,
                              BuiltinTypeSpecifier,
                              BitfieldDeclarator,
                              IntegerConstantExpression }));
}

void ParserTest::case0421()
{
    parse("struct x { y : 1 ; } ;",
          Expectation().AST({ TranslationUnit,
                              StructDeclaration,
                              StructTypeSpecifier,
                              FieldDeclaration,
                              TypedefName,
                              BitfieldDeclarator,
                              IntegerConstantExpression }));
}

void ParserTest::case0422()
{
    CROSS_REFERENCE_TEST(BinderTest::case0007); // Semantic warning.

    parse("struct x { int ; } ;");
}

void ParserTest::case0423()
{
//    parse("struct x { void ( * x ) ( ) : 1; } ;");
}

void ParserTest::case0424()
{
    parse("struct x { int y : 1, z : 2 ; } ;");
}

void ParserTest::case0425()
{
    parse("struct x { int y : 1; z w : 2 ; } ;");
}

void ParserTest::case0426()
{
    parse("struct x { int y : 1, z ; } ;");
}

void ParserTest::case0427()
{
    parse("struct x { int y : 1, z , w : 2 ; } ;");
}

void ParserTest::case0428()
{
    parse("struct { _Static_assert ( 1 , \"\" ) ; }; ");
}

void ParserTest::case0429()
{
    parse("struct x volatile y ;");
}

void ParserTest::case0430()
{
    parse("struct x { int y ; } volatile z ;");
}

void ParserTest::case0431()
{
    parse("struct { int x ; } volatile y ;");
}

void ParserTest::case0432()
{
    parse("const struct { int x ; } volatile y ;");
}

void ParserTest::case0433()
{
    parse("struct { int x ; } struct y z ;",
          Expectation().diagnostic(Expectation::ErrorOrWarn::Error,
                                      Parser::DiagnosticsReporter::ID_of_ExpectedFIRSTofDirectDeclarator));
}

void ParserTest::case0434()
{
    parse("struct x { int y ; } int z ;",
          Expectation().diagnostic(Expectation::ErrorOrWarn::Error,
                                      Parser::DiagnosticsReporter::ID_of_ExpectedFIRSTofDirectDeclarator));
}

void ParserTest::case0435()
{
    // Covered as a semantic error.
    CROSS_REFERENCE_TEST(BinderTest::case0019);

    parse("int struct x { int y ; } z ;");
}

void ParserTest::case0436()
{
    // Covered as a semantic error.
    CROSS_REFERENCE_TEST(BinderTest::case0020);

    parse("struct x struct { int y ; } z ;");
}

void ParserTest::case0437()
{
    // Covered as a semantic error.
    CROSS_REFERENCE_TEST(BinderTest::case0021);

    parse("int struct x y ;");
}

void ParserTest::case0438()
{
    // Covered as a semantic error.
    CROSS_REFERENCE_TEST(BinderTest::case0022);

    parse("struct x int y ;");
}

void ParserTest::case0439()
{
    parse("struct { int x = 1 ; } ;",
          Expectation().diagnostic(Expectation::ErrorOrWarn::Error,
                                      Parser::DiagnosticsReporter::ID_of_ExpectedFOLLOWofStructDeclarator));
}

void ParserTest::case0440()
{
    parse("struct { x y = 1 ; } ;",
          Expectation().diagnostic(Expectation::ErrorOrWarn::Error,
                                      Parser::DiagnosticsReporter::ID_of_ExpectedFOLLOWofStructDeclarator));
}

void ParserTest::case0441()
{
    parse("struct { int x : 1 = 1 ; } ;",
          Expectation().diagnostic(Expectation::ErrorOrWarn::Error,
                                      Parser::DiagnosticsReporter::ID_of_ExpectedFOLLOWofStructDeclarator));
}

void ParserTest::case0442()
{
    parse("union x ;",
          Expectation().AST({ TranslationUnit,
                              UnionDeclaration,
                              UnionTypeSpecifier }));
}

void ParserTest::case0443()
{

}

void ParserTest::case0444()
{

}

void ParserTest::case0445()
{

}

void ParserTest::case0446()
{

}

void ParserTest::case0447()
{

}

void ParserTest::case0448()
{

}

void ParserTest::case0449()
{

}

void ParserTest::case0450()
{

}

void ParserTest::case0451()
{

}

void ParserTest::case0452()
{

}

void ParserTest::case0453()
{

}

void ParserTest::case0454()
{

}

void ParserTest::case0455()
{

}

void ParserTest::case0456()
{

}

void ParserTest::case0457()
{

}

void ParserTest::case0458()
{

}

void ParserTest::case0459()
{

}

void ParserTest::case0460()
{

}

void ParserTest::case0461()
{

}

void ParserTest::case0462()
{

}

void ParserTest::case0463()
{

}

void ParserTest::case0464()
{

}

void ParserTest::case0465()
{

}

void ParserTest::case0466()
{

}

void ParserTest::case0467()
{

}

void ParserTest::case0468()
{

}

void ParserTest::case0469()
{

}

void ParserTest::case0470()
{

}

void ParserTest::case0471()
{

}

void ParserTest::case0472()
{

}

void ParserTest::case0473()
{

}

void ParserTest::case0474()
{

}

void ParserTest::case0475()
{

}

void ParserTest::case0476()
{

}

void ParserTest::case0477()
{

}

void ParserTest::case0478()
{

}

void ParserTest::case0479()
{

}

void ParserTest::case0480()
{

}

void ParserTest::case0481()
{

}

void ParserTest::case0482()
{

}

void ParserTest::case0483()
{

}

void ParserTest::case0484()
{

}

void ParserTest::case0485()
{

}

void ParserTest::case0486()
{

}

void ParserTest::case0487()
{

}

void ParserTest::case0488()
{

}

void ParserTest::case0489()
{

}

void ParserTest::case0490()
{

}

void ParserTest::case0491()
{

}

void ParserTest::case0492()
{

}

void ParserTest::case0493()
{

}

void ParserTest::case0494()
{

}

void ParserTest::case0495()
{

}

void ParserTest::case0496()
{

}

void ParserTest::case0497()
{

}

void ParserTest::case0498()
{

}

void ParserTest::case0499()
{

}

void ParserTest::case0500()
{
    parse("enum x ;",
          Expectation().AST({ TranslationUnit,
                              IncompleteDeclaration,
                              EnumTypeSpecifier }));
}

void ParserTest::case0501()
{
    parse("enum x y ;");
}

void ParserTest::case0502()
{
    parse("enum x { } ;");
}

void ParserTest::case0503()
{
    parse("enum x { y } ;");
}

void ParserTest::case0504()
{
    parse("enum x { y , z } ;");
}

void ParserTest::case0505()
{
    parse("enum x { y , z, } ;");
}

void ParserTest::case0506()
{
    parse("enum x { y , z = w } ;");
}

void ParserTest::case0507()
{
    parse("enum x { y , z = w, } ;");
}

void ParserTest::case0508()
{
    parse("enum x { y = 1 } ;");
}

void ParserTest::case0509()
{
    parse("enum x { y = 1, } ;");
}

void ParserTest::case0510()
{
    parse("enum x { y = 1, z } ;");
}

void ParserTest::case0511()
{
    parse("enum x { y = 1, z, w = 3 } ;");
}

void ParserTest::case0512()
{
    parse("enum x (",
          Expectation().diagnostic(Expectation::ErrorOrWarn::Error,
                                      Parser::DiagnosticsReporter::ID_of_ExpectedFIRSTofDirectDeclarator));
}

void ParserTest::case0513()
{
    parse("enum x { (",
          Expectation().diagnostic(Expectation::ErrorOrWarn::Error,
                                      Parser::DiagnosticsReporter::ID_of_ExpectedFIRSTofEnumerationConstant));
}

void ParserTest::case0514()
{
    parse("enum x { y, , z, } ;",
          Expectation().diagnostic(Expectation::ErrorOrWarn::Error,
                                      Parser::DiagnosticsReporter::ID_of_ExpectedFIRSTofEnumerationConstant));
}

void ParserTest::case0515()
{
}

void ParserTest::case0516()
{
}

void ParserTest::case0517()
{

}

void ParserTest::case0518()
{

}

void ParserTest::case0519()
{

}

void ParserTest::case0520()
{

}

void ParserTest::case0521()
{

}

void ParserTest::case0522()
{

}

void ParserTest::case0523()
{

}

void ParserTest::case0524()
{

}

void ParserTest::case0525()
{

}

void ParserTest::case0526()
{

}

void ParserTest::case0527()
{

}

void ParserTest::case0528()
{

}

void ParserTest::case0529()
{

}

void ParserTest::case0530()
{

}

void ParserTest::case0531()
{

}

void ParserTest::case0532()
{

}

void ParserTest::case0533()
{

}

void ParserTest::case0534()
{

}

void ParserTest::case0535()
{

}

void ParserTest::case0536()
{

}

void ParserTest::case0537()
{

}

void ParserTest::case0538()
{

}

void ParserTest::case0539()
{

}

void ParserTest::case0540()
{

}

void ParserTest::case0541()
{

}

void ParserTest::case0542()
{

}

void ParserTest::case0543()
{

}

void ParserTest::case0544()
{

}

void ParserTest::case0545()
{

}

void ParserTest::case0546()
{

}

void ParserTest::case0547()
{

}

void ParserTest::case0548()
{

}

void ParserTest::case0549()
{

}

void ParserTest::case0550()
{

}

void ParserTest::case0551()
{

}

void ParserTest::case0552()
{

}

void ParserTest::case0553()
{

}

void ParserTest::case0554()
{

}

void ParserTest::case0555()
{

}

void ParserTest::case0556()
{

}

void ParserTest::case0557()
{

}

void ParserTest::case0558()
{

}

void ParserTest::case0559()
{

}

void ParserTest::case0560()
{

}

void ParserTest::case0561()
{

}

void ParserTest::case0562()
{

}

void ParserTest::case0563()
{

}

void ParserTest::case0564()
{

}

void ParserTest::case0565()
{

}

void ParserTest::case0566()
{

}

void ParserTest::case0567()
{

}

void ParserTest::case0568()
{

}

void ParserTest::case0569()
{

}

void ParserTest::case0570()
{

}

void ParserTest::case0571()
{

}

void ParserTest::case0572()
{

}

void ParserTest::case0573()
{

}

void ParserTest::case0574()
{

}

void ParserTest::case0575()
{

}

void ParserTest::case0576()
{

}

void ParserTest::case0577()
{

}

void ParserTest::case0578()
{

}

void ParserTest::case0579()
{

}

void ParserTest::case0580()
{

}

void ParserTest::case0581()
{

}

void ParserTest::case0582()
{

}

void ParserTest::case0583()
{

}

void ParserTest::case0584()
{

}

void ParserTest::case0585()
{

}

void ParserTest::case0586()
{

}

void ParserTest::case0587()
{

}

void ParserTest::case0588()
{

}

void ParserTest::case0589()
{

}

void ParserTest::case0590()
{

}

void ParserTest::case0591()
{

}

void ParserTest::case0592()
{

}

void ParserTest::case0593()
{

}

void ParserTest::case0594()
{

}

void ParserTest::case0595()
{

}

void ParserTest::case0596()
{

}

void ParserTest::case0597()
{

}

void ParserTest::case0598()
{

}

void ParserTest::case0599()
{

}

void ParserTest::case0600()
{
    parse("extern void * x ( int ) ;") ;
}

void ParserTest::case0601()
{
    parse("extern void * x ( y ) ;") ;
}

void ParserTest::case0602()
{
    parse("extern const volatile int x ;") ;
}

void ParserTest::case0603()
{
    parse("extern const volatile x y ;") ;
}

void ParserTest::case0604()
{
    parse("extern int x [] ;") ;
}

void ParserTest::case0605()
{
    parse("extern int x [ 100 ] ;") ;
}

void ParserTest::case0606()
{
    parse("extern x [ ] ;") ;
}

void ParserTest::case0607()
{
    parse("extern x [ 100 ] ;") ;
}

void ParserTest::case0608()
{
    parse("extern double x ( double ) ;",
          Expectation().AST({ TranslationUnit,
                              VariableAndOrFunctionDeclaration,
                              ExternStorageClass,
                              BuiltinTypeSpecifier,
                              FunctionDeclarator,
                              IdentifierDeclarator,
                              ParameterSuffix,
                              ParameterDeclaration,
                              BuiltinTypeSpecifier,
                              AbstractDeclarator }));
}

void ParserTest::case0609()
{
    parse("extern int * x ;") ;
}

void ParserTest::case0610()
{
    parse("extern x * y ;") ;
}

void ParserTest::case0611()
{
    parse("extern int x ;") ;
}

void ParserTest::case0612()
{
    parse("extern x y ;") ;
}

void ParserTest::case0613()
{
    parse("extern int ( * x ) [ y ] ;") ;
}

void ParserTest::case0614()
{
    parse("extern int ( * x ) [ y ] = & z ;") ;
}

void ParserTest::case0615()
{
    parse("extern int x = 1 ;") ;
}

void ParserTest::case0616()
{
    parse("typedef int x ;") ;
}

void ParserTest::case0617()
{
    parse("typedef x y ;") ;
}

void ParserTest::case0618()
{
    parse("typedef int * x ;") ;
}

void ParserTest::case0619()
{
    parse("typedef x * y ;") ;
}

void ParserTest::case0620()
{
    parse("typedef int ( * y ) ( double ) ;") ;
}

void ParserTest::case0621()
{
    parse("typedef x ( * y ) ( double ) ;") ;
}

void ParserTest::case0622()
{
    parse("typedef int x __attribute__ ( ( aligned ( 8 ) ));") ;
}

void ParserTest::case0623()
{
    parse("typedef x y __attribute__ ( ( deprecated ));") ;
}

void ParserTest::case0624()
{
    parse(R"(extern int __attribute__ ( ( alias ( "x" ) ) ) y ; )") ;
}

void ParserTest::case0625()
{
    parse("extern int x __attribute__ ( ( deprecated ));") ;
}

void ParserTest::case0626()
{
    parse("typedef __attribute__ ( ( alloc_size ( 1 , 2 ) ) ) void * ( * x ) ( int , int ) ;") ;
}

void ParserTest::case0627()
{
    parse("typedef __attribute__ ( ( alloc_size ( 1 , 2 ) ) ) void * ( * x ) ( y , z ) ;") ;
}

void ParserTest::case0628()
{
    parse("_Thread_local int x ;");
}

void ParserTest::case0629()
{
    parse("_Thread_local x y ;");
}

void ParserTest::case0630()
{
    parse("__thread int x ;");
}

void ParserTest::case0631()
{
    parse("void x ( ) { static __thread x y ; }");
}

void ParserTest::case0632()
{
    // Not a syntactic error, but a semantic one; covered in:
    CROSS_REFERENCE_TEST(BinderTest::case0015);

    parse("void x ( ) { __thread x y ; }");
}

void ParserTest::case0633()
{

}

void ParserTest::case0634()
{

}

void ParserTest::case0635()
{

}

void ParserTest::case0636()
{

}

void ParserTest::case0637()
{

}

void ParserTest::case0638()
{

}

void ParserTest::case0639()
{

}

void ParserTest::case0640()
{

}

void ParserTest::case0641()
{

}

void ParserTest::case0642()
{

}

void ParserTest::case0643()
{

}

void ParserTest::case0644()
{

}

void ParserTest::case0645()
{

}

void ParserTest::case0646()
{

}

void ParserTest::case0647()
{

}

void ParserTest::case0648()
{

}

void ParserTest::case0649()
{

}

void ParserTest::case0650()
{
    parse("_Static_assert ( 1 != 2 , \"x\" ) ;") ;
}

void ParserTest::case0651()
{

}

void ParserTest::case0652()
{

}

void ParserTest::case0653()
{

}

void ParserTest::case0654()
{

}

void ParserTest::case0655()
{

}

void ParserTest::case0656()
{

}

void ParserTest::case0657()
{

}

void ParserTest::case0658()
{

}

void ParserTest::case0659()
{

}

void ParserTest::case0660()
{

}

void ParserTest::case0661()
{

}

void ParserTest::case0662()
{

}

void ParserTest::case0663()
{

}

void ParserTest::case0664()
{

}

void ParserTest::case0665()
{

}

void ParserTest::case0666()
{

}

void ParserTest::case0667()
{

}

void ParserTest::case0668()
{

}

void ParserTest::case0669()
{

}

void ParserTest::case0670()
{
    parse("__extension__ int x ;");
}

void ParserTest::case0671()
{
    parse("__extension__ struct x { } ;");
}

void ParserTest::case0672()
{
    parse("struct x { __extension__ int y ; } ;");
}

void ParserTest::case0673()
{
    parse("int x = __extension__ 0 ;");
}

void ParserTest::case0674()
{
    parse("__extension__ void  x ( ) { }");
}

void ParserTest::case0675()
{
    parse("__extension__ asm ( \"nop\" ) ;");
}

void ParserTest::case0676()
{
    parseStatement("{ __extension__ int x ; }");
}

void ParserTest::case0677()
{
    parseStatement("__extension__ for ( ; ; ) { } ",
                   Expectation().diagnostic(Expectation::ErrorOrWarn::Error,
                                               Parser::DiagnosticsReporter::ID_of_UnexpectedGNUExtensionFlag));
}

void ParserTest::case0678()
{
    // Accepted by GCC, ICC, but not by Clang (11.0.0).

    parseStatement("for ( __extension__  int x ; ; ) { } ");
}

void ParserTest::case0679()
{

}

void ParserTest::case0680()
{

}

void ParserTest::case0681()
{

}

void ParserTest::case0682()
{

}

void ParserTest::case0683()
{

}

void ParserTest::case0684()
{

}

void ParserTest::case0685()
{

}

void ParserTest::case0686()
{

}

void ParserTest::case0687()
{

}

void ParserTest::case0688()
{

}

void ParserTest::case0689()
{

}

void ParserTest::case0690()
{

}

void ParserTest::case0691()
{

}

void ParserTest::case0692()
{

}

void ParserTest::case0693()
{

}

void ParserTest::case0694()
{

}

void ParserTest::case0695()
{

}

void ParserTest::case0696()
{

}

void ParserTest::case0697()
{

}

void ParserTest::case0698()
{

}

void ParserTest::case0699()
{

}

void ParserTest::case0700()
{
    parse("void x ( void ) __attribute__ ( ( noreturn ));",
          Expectation().AST({ TranslationUnit,
                              VariableAndOrFunctionDeclaration,
                              BuiltinTypeSpecifier,
                              FunctionDeclarator,
                              IdentifierDeclarator,
                              ParameterSuffix,
                              ParameterDeclaration,
                              BuiltinTypeSpecifier,
                              AbstractDeclarator,
                              ExtGNU_AttributeSpecifier,
                              ExtGNU_Attribute }));
}

void ParserTest::case0701()
{
    parse("void __attribute__ ( ( noreturn ) ) x ( void ) ;",
          Expectation().AST({ TranslationUnit,
                              VariableAndOrFunctionDeclaration,
                              BuiltinTypeSpecifier,
                              ExtGNU_AttributeSpecifier,
                              ExtGNU_Attribute,
                              FunctionDeclarator,
                              IdentifierDeclarator,
                              ParameterSuffix,
                              ParameterDeclaration,
                              BuiltinTypeSpecifier,
                              AbstractDeclarator }));
}

void ParserTest::case0702()
{
    // The attribute belongs to the variable; a type attribute would've
    // been placed immediately after the `struct' keyword, or,
    // alternatively, after the closing brace.

    CROSS_REFERENCE_TEST(ParserTest::case0703) ;
    CROSS_REFERENCE_TEST(ParserTest::case0704) ;

    parse("struct x __attribute__ ( ( vector_size ( 16 ) ) ) y ;") ;

}

void ParserTest::case0703()
{
    // The attribute belongs to the type.

    CROSS_REFERENCE_TEST(ParserTest::case0702) ;

    parse("struct __attribute__ ( ( aligned ) ) x { } ;") ;
}

void ParserTest::case0704()
{
    // The attribute belongs to the type.

    CROSS_REFERENCE_TEST(ParserTest::case0702) ;

    parse("struct x { } __attribute__ ( ( aligned ) ) ;") ;
}

void ParserTest::case0705()
{
    // The attribute belongs to the type.

    CROSS_REFERENCE_TEST(ParserTest::case0702) ;

    parse("struct x { short y [ 3 ] ; } __attribute__ ( ( aligned ( 8 ) ) ) ;") ;
}

void ParserTest::case0706()
{
    parse("struct x { int y [ 2 ] __attribute__ ( ( aligned ( 8 ) ) ) ; } ;") ;
}

void ParserTest::case0707()
{
    parse("struct x { short y [ 3 ] ; } __attribute__ ( ( aligned (8) ) ) z ;") ;
}

void ParserTest::case0708()
{
    parse("__attribute__ ( ( availability ( macosx , introduced = 10.12 ) ) ) void x ( ) ;") ;
}

void ParserTest::case0709()
{
    parse("__attribute__ ( ( availability ( macos , introduced = 10.4 , deprecated = 10.6 , obsoleted = 10.7) ) ) void x ( ) ;") ;
}

void ParserTest::case0710()
{
    parse("__attribute__ ( ( availability ( ios , introduced = 10 , message =\"whatever\" ) ) ) void x ( ) ;") ;
}

void ParserTest::case0711()
{
    parse("void x ( void ) __attribute__ ( ( availability ( macos , introduced = 10.4) ));") ;
}

void ParserTest::case0712()
{
    parse("void x ( void ) __attribute__ ( ( availability ( macos , introduced = 10.4 , deprecated = 10.6 , obsoleted=10.7) ));") ;
}

void ParserTest::case0713()
{
    parse("void ( __attribute__ ( ( noreturn ) ) * * * * x ) ( void ) ;") ;
}

void ParserTest::case0714()
{
    parse("char * __attribute__ ( ( aligned ( 8 ) ) ) * x ;") ;
}

void ParserTest::case0715()
{
    parse("__attribute__ ( ( format ( printf , 111 , 222 ) ) ) x ( const char * , ... ) ;") ;
}

void ParserTest::case0716()
{
    parse(R"(
          void x ( void ),
          __attribute__ ( ( format ( printf , 111, 222) ) ) y ( const char * , ... ),
          z ( void ) ;
          )") ;
}

void ParserTest::case0717()
{
    parse(R"(
          __attribute__ ( (noreturn) ) void x ( void ),
          __attribute__ ( ( format ( printf , 111, 222) ) ) y ( const char * , ... ),
          z ( void ) ;
          )") ;
}

void ParserTest::case0718()
{
    parse("int x __attribute__ ( ( aligned (16) ) ) = 0 ;") ;
}

void ParserTest::case0719()
{
    parse("x y __attribute__ ( ( aligned (16) ) ) = 0 ;") ;
}

void ParserTest::case0720()
{
    parse("short x [3] __attribute__ ( ( aligned ( __BIGGEST_ALIGNMENT__ ) ));") ;
}

void ParserTest::case0721()
{
    parse("x y [3] __attribute__ ( ( aligned ( __BIGGEST_ALIGNMENT__ ) ));") ;
}

void ParserTest::case0722()
{
    parse("enum __attribute__ ( ( aligned ) ) x { y } ;") ;
}

void ParserTest::case0723()
{
    parse("enum x { y } __attribute__ ( ( aligned ) );") ;
}

void ParserTest::case0724()
{
    parse("enum x { y __attribute__( ( deprecated ) ) } ;") ;
}

void ParserTest::case0725()
{
    parse("enum x { y __attribute__( ( deprecated ) ) = 1} ;") ;
}

void ParserTest::case0726()
{
    parse("enum x { y __attribute__( ( deprecated ) ), z } ;") ;
}

void ParserTest::case0727()
{
    parse("enum x { y __attribute__( ( deprecated ) ) = 1, z } ;") ;
}

void ParserTest::case0728()
{
    parse("struct x { int y : 1 __attribute__ ( ( ) ) ; } ;");
}

void ParserTest::case0729()
{
    parse("struct x { int __attribute__(()) y : 2 ; } ;");
}

void ParserTest::case0730()
{
    // The attribute belongs to the declarator.

    parse("int x __attribute__ ( ( ) ) ;",
          Expectation().AST( { TranslationUnit,
                               VariableAndOrFunctionDeclaration,
                               BuiltinTypeSpecifier,
                               IdentifierDeclarator,
                               ExtGNU_AttributeSpecifier,
                               ExtGNU_Attribute }));
}

void ParserTest::case0731()
{
    // The attribute belongs to the declaration.

    parse("int __attribute__ ( ( ) ) x ;",
          Expectation().AST( { TranslationUnit,
                               VariableAndOrFunctionDeclaration,
                               BuiltinTypeSpecifier,
                               ExtGNU_AttributeSpecifier,
                               ExtGNU_Attribute,
                               IdentifierDeclarator }));
}

void ParserTest::case0732()
{
    // The attribute belongs to the declaration.

    parse("__attribute__ ( ( ) ) int x ;",
          Expectation().AST( { TranslationUnit,
                               VariableAndOrFunctionDeclaration,
                               ExtGNU_AttributeSpecifier,
                               ExtGNU_Attribute,
                               BuiltinTypeSpecifier,
                               IdentifierDeclarator }));
}

void ParserTest::case0733()
{
    parse("const __attribute__ ( ( ) ) int __attribute__ ( ( ) ) x ;",
          Expectation().AST( { TranslationUnit,
                               VariableAndOrFunctionDeclaration,
                               ConstQualifier,
                               ExtGNU_AttributeSpecifier,
                               ExtGNU_Attribute,
                               BuiltinTypeSpecifier,
                               ExtGNU_AttributeSpecifier,
                               ExtGNU_Attribute,
                               IdentifierDeclarator }));
}

void ParserTest::case0734()
{
    parse("struct x { } __attribute__ ( ( aligned ) ) y ;");
}

void ParserTest::case0735()
{
    parse("int __attribute__ ( ( ) ) x ;",
          Expectation().AST( { TranslationUnit,
                               VariableAndOrFunctionDeclaration,
                               BuiltinTypeSpecifier,
                               ExtGNU_AttributeSpecifier,
                               ExtGNU_Attribute,
                               IdentifierDeclarator }));
}

void ParserTest::case0736()
{
    parse("x __attribute__ ( ( ) ) y ;",
          Expectation().AST( { TranslationUnit,
                               VariableAndOrFunctionDeclaration,
                               TypedefName,
                               ExtGNU_AttributeSpecifier,
                               ExtGNU_Attribute,
                               IdentifierDeclarator }));
}

void ParserTest::case0737()
{
    // Not a syntactic error, but a semantic one; covered in:
    CROSS_REFERENCE_TEST(BinderTest::case0023);

    // Similar but without the `__attribute__'.
    CROSS_REFERENCE_TEST(ParserTest::case0021);

    parse("int __attribute__ ( ( ) ) double ;",
          Expectation().AST( { TranslationUnit,
                               IncompleteDeclaration,
                               BuiltinTypeSpecifier,
                               ExtGNU_AttributeSpecifier,
                               ExtGNU_Attribute,
                               BuiltinTypeSpecifier }));
}

void ParserTest::case0738()
{
    // Not a syntactic error, but a semantic one; covered in:
    CROSS_REFERENCE_TEST(BinderTest::case0024);

    parse("x __attribute__ ( ( ) ) int ;",
          Expectation().AST( { TranslationUnit,
                               IncompleteDeclaration,
                               TypedefName,
                               ExtGNU_AttributeSpecifier,
                               ExtGNU_Attribute,
                               BuiltinTypeSpecifier }));
}

void ParserTest::case0739()
{
    /*
     * Notes:
     *     - Here, and without a `typedef' for `x', GCC 11.2 diagnoses a warning
     *       (no type or storage class) while Clang 13.0 diagnoses an error.
     */

    parse("x __attribute__ ( ( ) ) ;",
          Expectation().AST( { TranslationUnit,
                               IncompleteDeclaration,
                               TypedefName,
                               ExtGNU_AttributeSpecifier,
                               ExtGNU_Attribute }));
}

void ParserTest::case0740()
{
    parse("x __attribute__ ( ( noinline ) ) f ( ) { return 0 ; }");
}

void ParserTest::case0741()
{

}

void ParserTest::case0742()
{

}

void ParserTest::case0743()
{

}

void ParserTest::case0744()
{

}

void ParserTest::case0745()
{

}

void ParserTest::case0746()
{

}

void ParserTest::case0747()
{

}

void ParserTest::case0748()
{

}

void ParserTest::case0749()
{

}

void ParserTest::case0750()
{
}

void ParserTest::case0751()
{

}

void ParserTest::case0752()
{

}

void ParserTest::case0753()
{

}

void ParserTest::case0754()
{

}

void ParserTest::case0755()
{

}

void ParserTest::case0756()
{

}

void ParserTest::case0757()
{

}

void ParserTest::case0758()
{

}

void ParserTest::case0759()
{

}

void ParserTest::case0760()
{

}

void ParserTest::case0761()
{

}

void ParserTest::case0762()
{

}

void ParserTest::case0763()
{

}

void ParserTest::case0764()
{

}

void ParserTest::case0765()
{

}

void ParserTest::case0766()
{

}

void ParserTest::case0767()
{

}

void ParserTest::case0768()
{

}

void ParserTest::case0769()
{

}

void ParserTest::case0770()
{

}

void ParserTest::case0771()
{

}

void ParserTest::case0772()
{

}

void ParserTest::case0773()
{

}

void ParserTest::case0774()
{

}

void ParserTest::case0775()
{

}

void ParserTest::case0776()
{

}

void ParserTest::case0777()
{

}

void ParserTest::case0778()
{

}

void ParserTest::case0779()
{

}

void ParserTest::case0780()
{
    parse(R"(
          asm("nop \n");
          )");
}

void ParserTest::case0781()
{
    parse("int x asm ( \"y\") = 1 ;");
}

void ParserTest::case0782()
{
    parse("int x asm ( \"y\") __attribute__ (()) = 1 ;");
}

void ParserTest::case0783()
{
    parse("int x __attribute__ (()) asm",
          Expectation().setErrorCnt(2));
}

void ParserTest::case0784()
{
    parse("int __attribute__ (()) x asm ( \"y\") = 1 ;");
}

void ParserTest::case0785()
{
    parse("int x ( int y ) asm ( \"z\" );");
}

void ParserTest::case0786()
{

}

void ParserTest::case0787()
{

}

void ParserTest::case0788()
{

}

void ParserTest::case0789()
{

}

void ParserTest::case0790()
{

}

void ParserTest::case0791()
{

}

void ParserTest::case0792()
{

}

void ParserTest::case0793()
{

}

void ParserTest::case0794()
{

}

void ParserTest::case0795()
{

}

void ParserTest::case0796()
{

}

void ParserTest::case0797()
{

}

void ParserTest::case0798()
{

}

void ParserTest::case0799()
{

}

void ParserTest::case0800()
{
    parse("x y = { z } ;") ;
}

void ParserTest::case0801()
{
    parse("int x = { 1 } ;",
          Expectation().AST( { TranslationUnit,
                               VariableAndOrFunctionDeclaration,
                               BuiltinTypeSpecifier,
                               IdentifierDeclarator,
                               BraceEnclosedInitializer,
                               ExpressionInitializer,
                               IntegerConstantExpression }));
}

void ParserTest::case0802()
{
    parse("struct x y = { 1 } ;",
          Expectation().AST( { TranslationUnit,
                               VariableAndOrFunctionDeclaration,
                               StructTypeSpecifier,
                               IdentifierDeclarator,
                               BraceEnclosedInitializer,
                               ExpressionInitializer,
                               IntegerConstantExpression }));
}

void ParserTest::case0803()
{
    parse("struct x y = { 1, 2 } ;") ;
}

void ParserTest::case0804()
{
    parse("x y = { 1, 2 } ;",
          Expectation().AST( { TranslationUnit,
                               VariableAndOrFunctionDeclaration,
                               TypedefName,
                               IdentifierDeclarator,
                               BraceEnclosedInitializer,
                               ExpressionInitializer,
                               IntegerConstantExpression,
                               ExpressionInitializer,
                               IntegerConstantExpression }));
}

void ParserTest::case0805()
{
    parse("x y = { { 1 } } ;",
          Expectation().AST( { TranslationUnit,
                               VariableAndOrFunctionDeclaration,
                               TypedefName,
                               IdentifierDeclarator,
                               BraceEnclosedInitializer,
                               BraceEnclosedInitializer,
                               ExpressionInitializer,
                               IntegerConstantExpression }));
}

void ParserTest::case0806()
{
     parse("int x[] = { 1, 2 } ;") ;
}

void ParserTest::case0807()
{
    parse("int x[2] = { 1, 2 } ;") ;
}

void ParserTest::case0808()
{
    parse("int x[][2] = { { 1, 2 } } ;") ;
}

void ParserTest::case0809()
{
    parse("int x[][2] = { { 1, 2 }, { 3, 4 } } ;") ;
}

void ParserTest::case0810()
{
    parse("x y[][2] = { { 1, 2 }, { 3, 4 } } ;") ;
}

void ParserTest::case0811()
{
    parse("x y = { { 1, 2 }, { 3, 4 } } ;") ;
}

void ParserTest::case0812()
{
    parse("x y = { { 1, 2, 3 }, { 4 }, { 6, 7, 8} } ;") ;
}

void ParserTest::case0813()
{
    parse("x y = { 1, 2 }, y = { 3, 4, 5 } ;") ;
}

void ParserTest::case0814()
{
    parse("int x[] = { 1, 2 }, y[] = { 3, 4, 5 } ;") ;
}

void ParserTest::case0815()
{
    parse("x y[] = { 1, 2 }, z[] = { 3, 4, 5 } ;") ;
}

void ParserTest::case0816()
{
    parse("x y = { 1, , 2 } ;",
          Expectation().diagnostic(Expectation::ErrorOrWarn::Error,
                                      Parser::DiagnosticsReporter::ID_of_ExpectedFIRSTofExpression));
}

void ParserTest::case0817()
{
    // Trailing comman is OK here.

    parse("x y = { 1, 2, } ;") ;
}

void ParserTest::case0818()
{
    parse("struct x y = { } ;",
          Expectation().diagnostic(Expectation::ErrorOrWarn::Error,
                                      Parser::DiagnosticsReporter::ID_of_ExpectedBraceEnclosedInitializerList));
}

void ParserTest::case0819()
{

}

void ParserTest::case0820()
{

}

void ParserTest::case0821()
{

}

void ParserTest::case0822()
{

}

void ParserTest::case0823()
{

}

void ParserTest::case0824()
{

}

void ParserTest::case0825()
{

}

void ParserTest::case0826()
{

}

void ParserTest::case0827()
{

}

void ParserTest::case0828()
{

}

void ParserTest::case0829()
{

}

void ParserTest::case0830()
{

}

void ParserTest::case0831()
{

}

void ParserTest::case0832()
{

}

void ParserTest::case0833()
{

}

void ParserTest::case0834()
{

}

void ParserTest::case0835()
{

}

void ParserTest::case0836()
{

}

void ParserTest::case0837()
{

}

void ParserTest::case0838()
{

}

void ParserTest::case0839()
{

}

void ParserTest::case0840()
{

}

void ParserTest::case0841()
{

}

void ParserTest::case0842()
{

}

void ParserTest::case0843()
{

}

void ParserTest::case0844()
{

}

void ParserTest::case0845()
{

}

void ParserTest::case0846()
{

}

void ParserTest::case0847()
{

}

void ParserTest::case0848()
{

}

void ParserTest::case0849()
{

}

void ParserTest::case0850()
{
    parse("struct x y = { . z = 1 } ;",
          Expectation().AST( { TranslationUnit,
                               VariableAndOrFunctionDeclaration,
                               StructTypeSpecifier,
                               IdentifierDeclarator,
                               BraceEnclosedInitializer,
                               DesignatedInitializer,
                               FieldDesignator,
                               ExpressionInitializer,
                               IntegerConstantExpression }));
}

void ParserTest::case0851()
{
    parse("struct x y = { . z = 1, . w = 2 } ;") ;
}

void ParserTest::case0852()
{
    parse("struct x y = { . = 1 } ;",
          Expectation().diagnostic(Expectation::ErrorOrWarn::Error,
                                      Parser::DiagnosticsReporter::ID_of_ExpectedFieldDesignator));
}

void ParserTest::case0853()
{
    parse("x y [ ] = { [ 1 ] = 1 } ;",
          Expectation().AST( { TranslationUnit,
                               VariableAndOrFunctionDeclaration,
                               TypedefName,
                               ArrayDeclarator,
                               IdentifierDeclarator,
                               SubscriptSuffix,
                               BraceEnclosedInitializer,
                               DesignatedInitializer,
                               ArrayDesignator,
                               IntegerConstantExpression,
                               ExpressionInitializer,
                               IntegerConstantExpression }));
}

void ParserTest::case0854()
{
    parse("int x [ 1 ] = { [ 1 ] = 1 , [ 2 ] = 2 } ;");
}

void ParserTest::case0855()
{
    parse("x y [ 1 ] = { [ 1 ] = 1 , 2 , [ 3 ] = 3 } ;");
}

void ParserTest::case0856()
{
    parse("x y [ 1 ] = { [ 1 ] = 1 , 2 , [ 3 ] = 3 , z } ;");
}

void ParserTest::case0857()
{
    parse("x y [ 1 ] = { [ 1 ] = 1 , 2 , [ 3 ] = 3 } ;");
}

void ParserTest::case0858()
{
    parse("x y [ ] = { [ 1 ] = z , } ;");
}

void ParserTest::case0859()
{
    parse("x y [ ] = { [ 1 ] } ;",
          Expectation().diagnostic(Expectation::ErrorOrWarn::Warn,
                                      Parser::DiagnosticsReporter::ID_of_ExpectedFOLLOWofDesignatedInitializer)
                       .diagnostic(Expectation::ErrorOrWarn::Error,
                                      Parser::DiagnosticsReporter::ID_of_ExpectedFIRSTofExpression));
}

void ParserTest::case0860()
{
    parse("x y [ ] = { [ 1 ] 2 } ;",
          Expectation().diagnostic(Expectation::ErrorOrWarn::Warn,
                                      Parser::DiagnosticsReporter::ID_of_ExpectedFOLLOWofDesignatedInitializer));
}

void ParserTest::case0861()
{
    parse("x y [ ] = { [ 1 ] = } ;",
          Expectation().setErrorCnt(1));
}

void ParserTest::case0862()
{
    parse("x y [ ] = { [ ] = 1 } ;",
          Expectation().setErrorCnt(1));
}

void ParserTest::case0863()
{
    parseExpression("( struct x ) { . y [ 0 ] = 1 }",
                    Expectation().AST( { CompoundLiteralExpression,
                                         TypeName,
                                         StructTypeSpecifier,
                                         AbstractDeclarator,
                                         BraceEnclosedInitializer,
                                         DesignatedInitializer,
                                         FieldDesignator,
                                         ArrayDesignator,
                                         IntegerConstantExpression,
                                         ExpressionInitializer,
                                         IntegerConstantExpression }));
}

void ParserTest::case0864()
{
    parseExpression("( struct x ) { . y [ 0 ] . z = 1 }",
                    Expectation().AST( { CompoundLiteralExpression,
                                         TypeName,
                                         StructTypeSpecifier,
                                         AbstractDeclarator,
                                         BraceEnclosedInitializer,
                                         DesignatedInitializer,
                                         FieldDesignator,
                                         ArrayDesignator,
                                         IntegerConstantExpression,
                                         FieldDesignator,
                                         ExpressionInitializer,
                                         IntegerConstantExpression }));
}

void ParserTest::case0865()
{

}

void ParserTest::case0866()
{

}

void ParserTest::case0867()
{

}

void ParserTest::case0868()
{

}

void ParserTest::case0869()
{

}

void ParserTest::case0870()
{

}

void ParserTest::case0871()
{

}

void ParserTest::case0872()
{

}

void ParserTest::case0873()
{

}

void ParserTest::case0874()
{

}

void ParserTest::case0875()
{

}

void ParserTest::case0876()
{

}

void ParserTest::case0877()
{

}

void ParserTest::case0878()
{

}

void ParserTest::case0879()
{

}

void ParserTest::case0880()
{

}

void ParserTest::case0881()
{

}

void ParserTest::case0882()
{

}

void ParserTest::case0883()
{

}

void ParserTest::case0884()
{

}

void ParserTest::case0885()
{

}

void ParserTest::case0886()
{

}

void ParserTest::case0887()
{

}

void ParserTest::case0888()
{

}

void ParserTest::case0889()
{

}

void ParserTest::case0890()
{

}

void ParserTest::case0891()
{

}

void ParserTest::case0892()
{

}

void ParserTest::case0893()
{

}

void ParserTest::case0894()
{

}

void ParserTest::case0895()
{

}

void ParserTest::case0896()
{

}

void ParserTest::case0897()
{

}

void ParserTest::case0898()
{

}

void ParserTest::case0899()
{

}

void ParserTest::case0900()
{
    parse("__typeof__ ( int ) x ;");
}

void ParserTest::case0901()
{
    parse("__typeof__ ( int * ) x ;");
}

void ParserTest::case0902()
{
    parse("__typeof__ ( int * ) x [ 1 ] ;");
}

void ParserTest::case0903()
{
    parse("__typeof__ ( __typeof__ ( int * ) [ 1 ] ) x ;");
}

void ParserTest::case0904()
{
    /*
     Despite the parsing similarities between `typeof' and `sizeof',
     mind the difference between this test case and the following:

     `sizeof ( x ) y ;'

     This `sizeof' expression is invalid, since it'd imply parsing
     the expression operand as a cast-expression, but that isn't
     allowed in such context: only a unary-expression is.
     */

    CROSS_REFERENCE_TEST(ParserTest::case2115);

    parse("__typeof__ ( x ) y ;",
          Expectation().ambiguity("__typeof__ ( x ) ( x ) y ;")
                       .AST( { TranslationUnit,
                               VariableAndOrFunctionDeclaration,
                               ExtGNU_Typeof,
                               AmbiguousTypeNameOrExpressionAsTypeReference,
                               ExpressionAsTypeReference,
                               ParenthesizedExpression,
                               IdentifierName,
                               TypeNameAsTypeReference,
                               TypeName,
                               TypedefName,
                               IdentifierDeclarator } ));
}

void ParserTest::case0905()
{
    parse("__typeof__ ( x * ) y ;");
}

void ParserTest::case0906()
{
    parse("__typeof__ ( * x ) y ;");
}

void ParserTest::case0907()
{
    parse("__typeof__ ( x [ 0 ] ) y ;");
}

void ParserTest::case0908()
{
    parse("__typeof__ ( x [ 0 ] [ 1 ] ) y ;");
}

void ParserTest::case0909()
{
    parse("__typeof__ ( x [ 0 ] [ 1 ] ( ) ) y ;");
}

void ParserTest::case0910()
{
    parse("int x = ( __typeof__ ( int ) ) 1 ;");
}

void ParserTest::case0911()
{
    parse("void x ( __typeof__ ( int ) y ) ;");
}

void ParserTest::case0912()
{
    parse("void x ( ) { __typeof__ ( x * ) y ; }");
}

void ParserTest::case0913()
{
    parse("void x ( ) { __typeof__ ( int ) x ; }");
}

void ParserTest::case0914()
{
    parse("void x ( ) { sizeof ( __typeof__ ( int ) ) ; }");
}

void ParserTest::case0915()
{

}

void ParserTest::case0916()
{

}

void ParserTest::case0917()
{

}

void ParserTest::case0918()
{

}

void ParserTest::case0919()
{

}

void ParserTest::case0920()
{

}

void ParserTest::case0921()
{

}

void ParserTest::case0922()
{

}

void ParserTest::case0923()
{

}

void ParserTest::case0924()
{

}

void ParserTest::case0925()
{

}

void ParserTest::case0926()
{

}

void ParserTest::case0927()
{

}

void ParserTest::case0928()
{

}

void ParserTest::case0929()
{

}

void ParserTest::case0930()
{

}

void ParserTest::case0931()
{

}

void ParserTest::case0932()
{

}

void ParserTest::case0933()
{

}

void ParserTest::case0934()
{

}

void ParserTest::case0935()
{

}

void ParserTest::case0936()
{

}

void ParserTest::case0937()
{

}

void ParserTest::case0938()
{

}

void ParserTest::case0939()
{

}

void ParserTest::case0940()
{

}

void ParserTest::case0941()
{

}

void ParserTest::case0942()
{

}

void ParserTest::case0943()
{

}

void ParserTest::case0944()
{

}

void ParserTest::case0945()
{

}

void ParserTest::case0946()
{

}

void ParserTest::case0947()
{

}

void ParserTest::case0948()
{

}

void ParserTest::case0949()
{

}

void ParserTest::case0950()
{
    parse("_Alignas ( 16 ) int x ;");
}

void ParserTest::case0951()
{
    parse("_Alignas ( double ) int x ;");
}

void ParserTest::case0952()
{
    parse("_Alignas ( double * ) int y ;");
}

void ParserTest::case0953()
{
    parse("_Alignas ( x ) int y ;",
          Expectation().ambiguity("_Alignas ( x ) ( x ) int y ;")
                       .AST( { TranslationUnit,
                               VariableAndOrFunctionDeclaration,
                               AlignmentSpecifier,
                               AmbiguousTypeNameOrExpressionAsTypeReference,
                               ExpressionAsTypeReference,
                               ParenthesizedExpression,
                               IdentifierName,
                               TypeNameAsTypeReference,
                               TypeName,
                               TypedefName,
                               BuiltinTypeSpecifier,
                               IdentifierDeclarator } ));
}

void ParserTest::case0954()
{
    parse("_Alignas ( x * ) int y ;");
}

void ParserTest::case0955()
{
    parse("_Alignas ( x * ) y z ;");
}

void ParserTest::case0956()
{
    parse("struct x { _Alignas ( 16 ) int y ; } ;");
}

void ParserTest::case0957()
{

}

void ParserTest::case0958()
{

}

void ParserTest::case0959()
{

}

void ParserTest::case0960()
{

}

void ParserTest::case0961()
{

}

void ParserTest::case0962()
{

}

void ParserTest::case0963()
{

}

void ParserTest::case0964()
{

}

void ParserTest::case0965()
{

}

void ParserTest::case0966()
{

}

void ParserTest::case0967()
{

}

void ParserTest::case0968()
{

}

void ParserTest::case0969()
{

}

void ParserTest::case0970()
{

}

void ParserTest::case0971()
{

}

void ParserTest::case0972()
{

}

void ParserTest::case0973()
{

}

void ParserTest::case0974()
{

}

void ParserTest::case0975()
{

}

void ParserTest::case0976()
{

}

void ParserTest::case0977()
{

}

void ParserTest::case0978()
{

}

void ParserTest::case0979()
{

}

void ParserTest::case0980()
{

}

void ParserTest::case0981()
{

}

void ParserTest::case0982()
{

}

void ParserTest::case0983()
{

}

void ParserTest::case0984()
{

}

void ParserTest::case0985()
{

}

void ParserTest::case0986()
{

}

void ParserTest::case0987()
{

}

void ParserTest::case0988()
{

}

void ParserTest::case0989()
{

}

void ParserTest::case0990()
{

}

void ParserTest::case0991()
{

}

void ParserTest::case0992()
{

}

void ParserTest::case0993()
{

}

void ParserTest::case0994()
{

}

void ParserTest::case0995()
{

}

void ParserTest::case0996()
{

}

void ParserTest::case0997()
{

}

void ParserTest::case0998()
{

}

void ParserTest::case0999()
{

}
