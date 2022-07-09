// Copyright (c) 2020/21/22 Leandro T. C. Melo <ltcmelo@gmail.com>
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

#include "ParserTester.h"

#include "BinderTester.h"

#include "parser/Parser.h"
#include "parser/Unparser.h"
#include "syntax/SyntaxLexeme_ALL.h"
#include "syntax/SyntaxNodes.h"

#include <algorithm>
#include <cstring>
#include <iostream>
#include <string>
#include <sstream>

using namespace psy;
using namespace C;

void ParserTester::case0001()
{
    parse("int x ;",
          Expectation().AST({ TranslationUnit,
                              VariableAndOrFunctionDeclaration,
                              BuiltinTypeSpecifier,
                              IdentifierDeclarator }));
}

void ParserTester::case0002()
{
    parse("x y ;",
          Expectation().AST({ TranslationUnit,
                              VariableAndOrFunctionDeclaration,
                              TypedefName,
                              IdentifierDeclarator }));
}

void ParserTester::case0003()
{
    parse("int x , y ;",
          Expectation().AST({ TranslationUnit,
                              VariableAndOrFunctionDeclaration,
                              BuiltinTypeSpecifier,
                              IdentifierDeclarator,
                              IdentifierDeclarator }));
}

void ParserTester::case0004()
{
    parse("int x = 1;",
          Expectation().AST({ TranslationUnit,
                              VariableAndOrFunctionDeclaration,
                              BuiltinTypeSpecifier,
                              IdentifierDeclarator,
                              ExpressionInitializer,
                              IntegerConstantExpression }));
}

void ParserTester::case0005()
{
    parse("int x = 1, y ;") ;
}

void ParserTester::case0006()
{
    parse("int x = 1, y = 2;") ;
}

void ParserTester::case0007()
{
    parse("int x , y = 3;") ;
}

void ParserTester::case0008()
{
    parse("x y = 1 ;") ;
}

void ParserTester::case0009()
{
    parse("const int x ;",
          Expectation().AST({ TranslationUnit,
                              VariableAndOrFunctionDeclaration,
                              ConstQualifier,
                              BuiltinTypeSpecifier,
                              IdentifierDeclarator }));
}

void ParserTester::case0010()
{
    parse("int const x ;",
          Expectation().AST({ TranslationUnit,
                              VariableAndOrFunctionDeclaration,
                              BuiltinTypeSpecifier,
                              ConstQualifier,
                              IdentifierDeclarator }));
}

void ParserTester::case0011()
{
    parse("const int volatile x ;",
          Expectation().AST({ TranslationUnit,
                              VariableAndOrFunctionDeclaration,
                              ConstQualifier,
                              BuiltinTypeSpecifier,
                              VolatileQualifier,
                              IdentifierDeclarator }));
}

void ParserTester::case0012()
{
    parse("int * x ;",
          Expectation().AST({ TranslationUnit,
                              VariableAndOrFunctionDeclaration,
                              BuiltinTypeSpecifier,
                              PointerDeclarator,
                              IdentifierDeclarator }));
}

void ParserTester::case0013()
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

void ParserTester::case0014()
{
    parse("int * const x ;",
          Expectation().AST({ TranslationUnit,
                              VariableAndOrFunctionDeclaration,
                              BuiltinTypeSpecifier,
                              PointerDeclarator,
                              ConstQualifier,
                              IdentifierDeclarator }));
}

void ParserTester::case0015()
{
    parse("int const * x ;",
          Expectation().AST({ TranslationUnit,
                              VariableAndOrFunctionDeclaration,
                              BuiltinTypeSpecifier,
                              ConstQualifier,
                              PointerDeclarator,
                              IdentifierDeclarator }));
}

void ParserTester::case0016()
{
    parse("const int * x ;",
          Expectation().AST({ TranslationUnit,
                              VariableAndOrFunctionDeclaration,
                              ConstQualifier,
                              BuiltinTypeSpecifier,
                              PointerDeclarator,
                              IdentifierDeclarator }));
}

void ParserTester::case0017()
{
    parse("int * x , y ;",
          Expectation().AST({ TranslationUnit,
                              VariableAndOrFunctionDeclaration,
                              BuiltinTypeSpecifier,
                              PointerDeclarator,
                              IdentifierDeclarator,
                              IdentifierDeclarator }));
}

void ParserTester::case0018()
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

void ParserTester::case0019()
{
    parse("int * x = 0, * y = 0 ;") ;
}

void ParserTester::case0020()
{
    parse("const int * x = 0, y ;") ;
}

void ParserTester::case0021()
{
    // Not a syntactic error, but a semantic one; covered in:
    CROSS_REFERENCE_TEST(BinderTester::case0005);

    parse("int double x ;") ;
}

void ParserTester::case0022()
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

void ParserTester::case0023()
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

void ParserTester::case0024()
{
    parse("x * double y ;",
          Expectation().diagnostic(Expectation::ErrorOrWarn::Error,
                                   Parser::DiagnosticsReporter::ID_of_ExpectedFIRSTofDirectDeclarator));
}

void ParserTester::case0025()
{
    parse("double x * y ;",
          Expectation().diagnostic(Expectation::ErrorOrWarn::Error,
                                   Parser::DiagnosticsReporter::ID_of_ExpectedFOLLOWofDeclarator));
}

void ParserTester::case0026()
{
    parse("x * const double y ;",
          Expectation().diagnostic(Expectation::ErrorOrWarn::Error,
                                   Parser::DiagnosticsReporter::ID_of_ExpectedFIRSTofDirectDeclarator));
}

void ParserTester::case0027()
{
    parse("double * const x y ;",
          Expectation().diagnostic(Expectation::ErrorOrWarn::Error,
                                   Parser::DiagnosticsReporter::ID_of_ExpectedFOLLOWofDeclarator));
}

void ParserTester::case0028()
{
    // Not a syntactic error, but diagnoses a semantic warning.
    CROSS_REFERENCE_TEST(BinderTester::case0001) ;

    parse("const int ;",
          Expectation().AST({ TranslationUnit,
                              IncompleteDeclaration,
                              ConstQualifier,
                              BuiltinTypeSpecifier }));
}

void ParserTester::case0029()
{
    parse("const int , double x ;",
          Expectation().diagnostic(Expectation::ErrorOrWarn::Error,
                                      Parser::DiagnosticsReporter::ID_of_ExpectedFIRSTofDirectDeclarator));
}

void ParserTester::case0030()
{
    parse("int x = 1 {",
          Expectation().diagnostic(Expectation::ErrorOrWarn::Error,
                                      Parser::DiagnosticsReporter::ID_of_ExpectedFOLLOWofDeclaratorAndInitializer));
}

void ParserTester::case0031()
{
    parse("int ( x ) ;",
          Expectation().AST({ TranslationUnit,
                              VariableAndOrFunctionDeclaration,
                              BuiltinTypeSpecifier,
                              ParenthesizedDeclarator,
                              IdentifierDeclarator }));
}

void ParserTester::case0032()
{
    parse("int ( ( x ) );",
          Expectation().AST({ TranslationUnit,
                              VariableAndOrFunctionDeclaration,
                              BuiltinTypeSpecifier,
                              ParenthesizedDeclarator,
                              ParenthesizedDeclarator,
                              IdentifierDeclarator }));
}

void ParserTester::case0033()
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

void ParserTester::case0034()
{
    parse("x ( y ) ;",
          Expectation().AST({ TranslationUnit,
                              VariableAndOrFunctionDeclaration,
                              TypedefName,
                              ParenthesizedDeclarator,
                              IdentifierDeclarator }));
}

void ParserTester::case0035()
{
    parse("x ( ( y ) );",
          Expectation().AST({ TranslationUnit,
                              VariableAndOrFunctionDeclaration,
                              TypedefName,
                              ParenthesizedDeclarator,
                              ParenthesizedDeclarator,
                              IdentifierDeclarator }));
}

void ParserTester::case0036()
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

void ParserTester::case0037()
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

void ParserTester::case0038()
{
    parse("int ( * x ) ;",
          Expectation().AST({ TranslationUnit,
                              VariableAndOrFunctionDeclaration,
                              BuiltinTypeSpecifier,
                              ParenthesizedDeclarator,
                              PointerDeclarator,
                              IdentifierDeclarator }));
}

void ParserTester::case0039()
{
    parse("x ( * y ) ;",
          Expectation().AST({ TranslationUnit,
                              VariableAndOrFunctionDeclaration,
                              TypedefName,
                              ParenthesizedDeclarator,
                              PointerDeclarator,
                              IdentifierDeclarator }));
}

void ParserTester::case0040()
{
    CROSS_REFERENCE_TEST(BinderTester::case0013); // Semantic warning.

    parse("int ;");
}

void ParserTester::case0041()
{
    parse("x ;",
          Expectation().diagnostic(Expectation::ErrorOrWarn::Warn,
                                      Parser::DiagnosticsReporter::ID_of_ExpectedTypeSpecifier));
}

void ParserTester::case0042()
{
    parse("_Atomic int x ;",
          Expectation().AST({ TranslationUnit,
                              VariableAndOrFunctionDeclaration,
                              AtomicQualifier,
                              BuiltinTypeSpecifier,
                              IdentifierDeclarator }));
}

void ParserTester::case0043()
{
    parse("_Atomic x y ;",
          Expectation().AST({ TranslationUnit,
                              VariableAndOrFunctionDeclaration,
                              AtomicQualifier,
                              TypedefName,
                              IdentifierDeclarator }));
}

void ParserTester::case0044()
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

void ParserTester::case0045()
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

void ParserTester::case0046()
{
    parse("_Complex double x ;",
          Expectation().AST({ TranslationUnit,
                              VariableAndOrFunctionDeclaration,
                              BuiltinTypeSpecifier,
                              BuiltinTypeSpecifier,
                              IdentifierDeclarator }));
}

void ParserTester::case0047()
{
    parse("_Complex float x ;",
          Expectation().AST({ TranslationUnit,
                              VariableAndOrFunctionDeclaration,
                              BuiltinTypeSpecifier,
                              BuiltinTypeSpecifier,
                              IdentifierDeclarator }));
}

void ParserTester::case0048()
{
    parse("double _Complex x ;",
          Expectation().AST({ TranslationUnit,
                              VariableAndOrFunctionDeclaration,
                              BuiltinTypeSpecifier,
                              BuiltinTypeSpecifier,
                              IdentifierDeclarator }));
}

void ParserTester::case0049()
{
    parse("float _Complex x ;",
          Expectation().AST({ TranslationUnit,
                              VariableAndOrFunctionDeclaration,
                              BuiltinTypeSpecifier,
                              BuiltinTypeSpecifier,
                              IdentifierDeclarator }));
}

void ParserTester::case0050()
{
    // Not a syntactic error, but a semantic one; covered in:
    CROSS_REFERENCE_TEST(BinderTester::case0016);

    parse("double _Complex int x ;",
          Expectation().AST({ TranslationUnit,
                              VariableAndOrFunctionDeclaration,
                              BuiltinTypeSpecifier,
                              BuiltinTypeSpecifier,
                              BuiltinTypeSpecifier,
                              IdentifierDeclarator }));
}

void ParserTester::case0051()
{
    parse("_Complex x ;",
          Expectation().AST({ TranslationUnit,
                              VariableAndOrFunctionDeclaration,
                              BuiltinTypeSpecifier,
                              IdentifierDeclarator }));
}

void ParserTester::case0052()
{
    // Not a syntactic error, but a semantic one; covered in:
    CROSS_REFERENCE_TEST(BinderTester::case0016);

    parse("int int x ;",
          Expectation().AST({ TranslationUnit,
                              VariableAndOrFunctionDeclaration,
                              BuiltinTypeSpecifier,
                              BuiltinTypeSpecifier,
                              IdentifierDeclarator }));
}

void ParserTester::case0053()
{
    // Not a syntactic error, but a semantic one; covered in:
    CROSS_REFERENCE_TEST(BinderTester::case0018);

    parse("_Complex _Complex x ;",
          Expectation().AST({ TranslationUnit,
                              VariableAndOrFunctionDeclaration,
                              BuiltinTypeSpecifier,
                              BuiltinTypeSpecifier,
                              IdentifierDeclarator }));
}

void ParserTester::case0054()
{
    parse("x int ;",
          Expectation().diagnostic(Expectation::ErrorOrWarn::Error,
                                      Parser::DiagnosticsReporter::ID_of_ExpectedFOLLOWofDeclarator)
                       .diagnostic(Expectation::ErrorOrWarn::Warn,
                                      Parser::DiagnosticsReporter::ID_of_ExpectedTypeSpecifier));
}

void ParserTester::case0055()
{
    parse("const x ;",
          Expectation().AST({ TranslationUnit,
                              VariableAndOrFunctionDeclaration,
                              ConstQualifier,
                              IdentifierDeclarator }));
}

void ParserTester::case0056()
{
    parse("const * ;",
          Expectation().diagnostic(Expectation::ErrorOrWarn::Error,
                                      Parser::DiagnosticsReporter::ID_of_ExpectedFIRSTofDirectDeclarator));
}

void ParserTester::case0057()
{
    parse("__complex__ x ;",
          Expectation().AST( { TranslationUnit,
                               VariableAndOrFunctionDeclaration,
                               BuiltinTypeSpecifier,
                               IdentifierDeclarator }));
}

void ParserTester::case0058()
{
    parse("__complex__ double x ;",
          Expectation().AST( { TranslationUnit,
                               VariableAndOrFunctionDeclaration,
                               BuiltinTypeSpecifier,
                               BuiltinTypeSpecifier,
                               IdentifierDeclarator }));
}

void ParserTester::case0059()
{
    parse("double __complex__ x ;",
          Expectation().AST( { TranslationUnit,
                               VariableAndOrFunctionDeclaration,
                               BuiltinTypeSpecifier,
                               BuiltinTypeSpecifier,
                               IdentifierDeclarator }));
}

void ParserTester::case0060()
{
    parse("__complex__ float x ;",
          Expectation().AST( { TranslationUnit,
                               VariableAndOrFunctionDeclaration,
                               BuiltinTypeSpecifier,
                               BuiltinTypeSpecifier,
                               IdentifierDeclarator }));
}

void ParserTester::case0061()
{
    parse("float __complex__ x ;",
          Expectation().AST( { TranslationUnit,
                               VariableAndOrFunctionDeclaration,
                               BuiltinTypeSpecifier,
                               BuiltinTypeSpecifier,
                               IdentifierDeclarator }));

}

void ParserTester::case0062()
{
    parse("__complex__ int x ;",
          Expectation().AST( { TranslationUnit,
                               VariableAndOrFunctionDeclaration,
                               BuiltinTypeSpecifier,
                               BuiltinTypeSpecifier,
                               IdentifierDeclarator }));
}

void ParserTester::case0063()
{
    parse("int __complex__ x ;",
          Expectation().AST( { TranslationUnit,
                               VariableAndOrFunctionDeclaration,
                               BuiltinTypeSpecifier,
                               BuiltinTypeSpecifier,
                               IdentifierDeclarator }));
}

void ParserTester::case0064()
{

}

void ParserTester::case0065()
{

}

void ParserTester::case0066()
{

}

void ParserTester::case0067()
{

}

void ParserTester::case0068()
{
}

void ParserTester::case0069()
{

}

void ParserTester::case0070()
{

}

void ParserTester::case0071()
{

}

void ParserTester::case0072()
{
}

void ParserTester::case0073()
{

}

void ParserTester::case0074()
{

}

void ParserTester::case0075()
{

}

void ParserTester::case0076()
{

}

void ParserTester::case0077()
{

}

void ParserTester::case0078()
{

}

void ParserTester::case0079()
{

}

void ParserTester::case0080()
{

}

void ParserTester::case0081()
{
}

void ParserTester::case0082()
{
}

void ParserTester::case0083()
{

}

void ParserTester::case0084()
{

}

void ParserTester::case0085()
{

}

void ParserTester::case0086()
{

}

void ParserTester::case0087()
{

}

void ParserTester::case0088()
{

}

void ParserTester::case0089()
{
}

void ParserTester::case0090()
{
}

void ParserTester::case0091()
{
}

void ParserTester::case0092()
{
}

void ParserTester::case0093()
{

}

void ParserTester::case0094()
{
}

void ParserTester::case0095()
{
}

void ParserTester::case0096()
{
}

void ParserTester::case0097()
{

}

void ParserTester::case0098()
{
}

void ParserTester::case0099()
{

}

void ParserTester::case0100()
{
    parse("void ( * ) ( ) ;",
          Expectation().diagnostic(Expectation::ErrorOrWarn::Error,
                                      Parser::DiagnosticsReporter::ID_of_ExpectedFIRSTofDirectDeclarator));
}

void ParserTester::case0101()
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

void ParserTester::case0102()
{
    parse("void ( * x ) ( ) { }",
          Expectation().diagnostic(Expectation::ErrorOrWarn::Error,
                                   Parser::DiagnosticsReporter::ID_of_ExpectedFOLLOWofDeclarator));
}

void ParserTester::case0103()
{
    parse("void ( ( * x ) ) ( ) { }",
          Expectation().diagnostic(Expectation::ErrorOrWarn::Error,
                                   Parser::DiagnosticsReporter::ID_of_ExpectedFOLLOWofDeclarator));
}

void ParserTester::case0104()
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

void ParserTester::case0105()
{
    parse("void ( * ) ( int ) ;",
          Expectation().diagnostic(Expectation::ErrorOrWarn::Error,
                                      Parser::DiagnosticsReporter::ID_of_ExpectedFIRSTofDirectDeclarator));
}

void ParserTester::case0106()
{
    parse("void ( * x ) ( int ) ;") ;
}

void ParserTester::case0107()
{
    parse("x ( * y ) ( int ) ;") ;
}

void ParserTester::case0108()
{
    // The `signal' function.

    parse("void ( * x ( int , void ( * y ) ( int ) ) ) ( int ) ;") ;
}

void ParserTester::case0109()
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

void ParserTester::case0110()
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

void ParserTester::case0111()
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

void ParserTester::case0112()
{
    parse("int ( * x ) [ 4 ] [ y + 1 ] ;");
}

void ParserTester::case0113()
{
    parse("void ( * * * x ) ( int ) ;");
}

void ParserTester::case0114()
{
    parse("x ( * * * y ) ( z ) ;");
}

void ParserTester::case0115()
{
    parse("x ( * ( * ( * y ) ) ) ( z ) ;");
}

void ParserTester::case0116()
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

void ParserTester::case0117()
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

void ParserTester::case0118()
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

void ParserTester::case0119()
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

void ParserTester::case0120()
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

void ParserTester::case0121()
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

void ParserTester::case0122()
{
    parse("void ( * * ( x ) ) ( ) = 0 ;");
}

void ParserTester::case0123()
{
    parse("void ( * ( * ( x ) ) ) ( ) = 0 ;");
}

void ParserTester::case0124()
{

}

void ParserTester::case0125()
{

}

void ParserTester::case0126()
{

}

void ParserTester::case0127()
{

}

void ParserTester::case0128()
{

}

void ParserTester::case0129()
{

}

void ParserTester::case0130()
{

}

void ParserTester::case0131()
{

}

void ParserTester::case0132()
{

}

void ParserTester::case0133()
{

}

void ParserTester::case0134()
{

}

void ParserTester::case0135()
{

}

void ParserTester::case0136()
{

}

void ParserTester::case0137()
{

}

void ParserTester::case0138()
{

}

void ParserTester::case0139()
{

}

void ParserTester::case0140()
{

}

void ParserTester::case0141()
{

}

void ParserTester::case0142()
{

}

void ParserTester::case0143()
{

}

void ParserTester::case0144()
{

}

void ParserTester::case0145()
{

}

void ParserTester::case0146()
{

}

void ParserTester::case0147()
{

}

void ParserTester::case0148()
{

}

void ParserTester::case0149()
{

}

void ParserTester::case0150()
{

}

void ParserTester::case0151()
{

}

void ParserTester::case0152()
{

}

void ParserTester::case0153()
{

}

void ParserTester::case0154()
{

}

void ParserTester::case0155()
{

}

void ParserTester::case0156()
{

}

void ParserTester::case0157()
{

}

void ParserTester::case0158()
{

}

void ParserTester::case0159()
{

}

void ParserTester::case0160()
{

}

void ParserTester::case0161()
{

}

void ParserTester::case0162()
{

}

void ParserTester::case0163()
{

}

void ParserTester::case0164()
{

}

void ParserTester::case0165()
{

}

void ParserTester::case0166()
{

}

void ParserTester::case0167()
{

}

void ParserTester::case0168()
{

}

void ParserTester::case0169()
{

}

void ParserTester::case0170()
{

}

void ParserTester::case0171()
{

}

void ParserTester::case0172()
{

}

void ParserTester::case0173()
{

}

void ParserTester::case0174()
{

}

void ParserTester::case0175()
{

}

void ParserTester::case0176()
{

}

void ParserTester::case0177()
{

}

void ParserTester::case0178()
{

}

void ParserTester::case0179()
{

}

void ParserTester::case0180()
{

}

void ParserTester::case0181()
{

}

void ParserTester::case0182()
{

}

void ParserTester::case0183()
{

}

void ParserTester::case0184()
{

}

void ParserTester::case0185()
{

}

void ParserTester::case0186()
{

}

void ParserTester::case0187()
{

}

void ParserTester::case0188()
{

}

void ParserTester::case0189()
{

}

void ParserTester::case0190()
{

}

void ParserTester::case0191()
{

}

void ParserTester::case0192()
{

}

void ParserTester::case0193()
{

}

void ParserTester::case0194()
{

}

void ParserTester::case0195()
{

}

void ParserTester::case0196()
{

}

void ParserTester::case0197()
{

}

void ParserTester::case0198()
{

}

void ParserTester::case0199()
{

}

void ParserTester::case0200()
{
    parse("void x ( ) ;",
          Expectation().AST({ TranslationUnit,
                              VariableAndOrFunctionDeclaration,
                              BuiltinTypeSpecifier,
                              FunctionDeclarator,
                              IdentifierDeclarator,
                              ParameterSuffix }));
}

void ParserTester::case0201()
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

void ParserTester::case0202()
{
    parse("void x { }",
          Expectation().diagnostic(Expectation::ErrorOrWarn::Error,
                                      Parser::DiagnosticsReporter::ID_of_ExpectedFOLLOWofDeclarator));
}

void ParserTester::case0203()
{
    parse("void x ( ) = 1 { }",
          Expectation().diagnostic(Expectation::ErrorOrWarn::Error,
                                      Parser::DiagnosticsReporter::ID_of_UnexpectedInitializerOfDeclarator));
}

void ParserTester::case0204()
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

void ParserTester::case0205()
{
    // Not a syntactic error, but a semantic one; covered in:
    CROSS_REFERENCE_TEST(BinderTester::case0008) ;

    parse("void x ( ) ( ) { }") ;
}

void ParserTester::case0206()
{
    // Not a syntactic error, but a semantic one; covered in:
    CROSS_REFERENCE_TEST(BinderTester::case0009) ;

    parse("void x ( ) ( ) ;") ;
}

void ParserTester::case0207()
{
    // Not a syntactic error, but a semantic one; covered in:
    CROSS_REFERENCE_TEST(BinderTester::case0010) ;

    parse("int x ( ) [ ] ;") ;
}

void ParserTester::case0208()
{
    // Not a syntactic error, but a semantic one; covered in:
    CROSS_REFERENCE_TEST(ParserTester::case0111) ;

    parse("int ( x ) ( ) [ ] ;") ;
}

void ParserTester::case0209()
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

void ParserTester::case0210()
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

void ParserTester::case0211()
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

void ParserTester::case0212()
{
    // Not a syntactic error, but a semantic one; covered in:
    CROSS_REFERENCE_TEST(BinderTester::case0012) ;

    parse("void x ( int ) { }") ;
}

void ParserTester::case0213()
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

void ParserTester::case0214()
{
    parse("double x( int y , double z ) ;") ;
}

void ParserTester::case0215()
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

void ParserTester::case0216()
{
    parse("x ( int y ) ;",
          Expectation().diagnostic(Expectation::ErrorOrWarn::Warn,
                                   Parser::DiagnosticsReporter::ID_of_ExpectedTypeSpecifier));
}

void ParserTester::case0217()
{
    parse("x ( y z ) ;",
          Expectation().diagnostic(Expectation::ErrorOrWarn::Warn,
                                   Parser::DiagnosticsReporter::ID_of_ExpectedTypeSpecifier));
}

void ParserTester::case0218()
{
    parse("void x ( y ) ;",
          Expectation().AST({ TranslationUnit,
                              VariableAndOrFunctionDeclaration,
                              BuiltinTypeSpecifier,
                              FunctionDeclarator,
                              IdentifierDeclarator,
                              ParameterSuffix,
                              ParameterDeclaration,
                              TypedefName,
                              AbstractDeclarator }));
}

void ParserTester::case0219()
{
    parse("void x ( y z ) ;") ;
}

void ParserTester::case0220()
{
    parse("void x ( ( int z ) ) ;",
          Expectation().diagnostic(Expectation::ErrorOrWarn::Error,
                                   Parser::DiagnosticsReporter::ID_of_ExpectedFIRSTofParameterDeclaration));
}

void ParserTester::case0221()
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

void ParserTester::case0222()
{
    parse("void x ( int * ) ;",
          Expectation().AST({ TranslationUnit,
                              VariableAndOrFunctionDeclaration,
                              BuiltinTypeSpecifier,
                              FunctionDeclarator,
                              IdentifierDeclarator,
                              ParameterSuffix,
                              ParameterDeclaration,
                              BuiltinTypeSpecifier,
                              PointerDeclarator,
                              AbstractDeclarator }));
}

void ParserTester::case0223()
{
    parse("void x ( int ( * ) ) ;") ;
}

void ParserTester::case0224()
{
    parse("void x ( int * [ 1 ] ) ;",
          Expectation().AST({ TranslationUnit,
                              VariableAndOrFunctionDeclaration,
                              BuiltinTypeSpecifier,
                              FunctionDeclarator,
                              IdentifierDeclarator,
                              ParameterSuffix,
                              ParameterDeclaration,
                              BuiltinTypeSpecifier,
                              PointerDeclarator,
                              ArrayDeclarator,
                              AbstractDeclarator,
                              SubscriptSuffix,
                              IntegerConstantExpression }));
}

void ParserTester::case0225()
{
    parse("void x ( int ( * ) [ 1 ] ) ;",
          Expectation().AST({ TranslationUnit,
                              VariableAndOrFunctionDeclaration,
                              BuiltinTypeSpecifier,
                              FunctionDeclarator,
                              IdentifierDeclarator,
                              ParameterSuffix,
                              ParameterDeclaration,
                              BuiltinTypeSpecifier,
                              ArrayDeclarator,
                              ParenthesizedDeclarator,
                              PointerDeclarator,
                              AbstractDeclarator,
                              SubscriptSuffix,
                              IntegerConstantExpression }));
}

void ParserTester::case0226()
{
    parse("void x ( int ( * ) [ * ] ) ;") ;
}

void ParserTester::case0227()
{
    parse("void x ( int * ( ));") ;
}

void ParserTester::case0228()
{
    parse("void x ( int * ( void ));") ;
}

void ParserTester::case0229()
{
    parse("void x ( int * ( int ));") ;
}

void ParserTester::case0230()
{
    parse("void x ( int * ( int , double ));") ;
}

void ParserTester::case0231()
{
    parse("void x ( int ( * const [ ] ) ( unsigned int , ... ));") ;
}

void ParserTester::case0232()
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

void ParserTester::case0233()
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

void ParserTester::case0234()
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

void ParserTester::case0235()
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

void ParserTester::case0236()
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

void ParserTester::case0237()
{
    parse("_Noreturn x y ( ) { }") ;
}

void ParserTester::case0238()
{
    parse("_Noreturn void x ( ) { }") ;
}

void ParserTester::case0239()
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

void ParserTester::case0240()
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

void ParserTester::case0241()
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

void ParserTester::case0242()
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

void ParserTester::case0243()
{
    parse("int [ ] x ( ) { }",
          Expectation().diagnostic(Expectation::ErrorOrWarn::Error,
                                   Parser::DiagnosticsReporter::ID_of_ExpectedFIRSTofDirectDeclarator));
}

void ParserTester::case0244()
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

void ParserTester::case0245()
{
    parse("void x ( y ) { }",
          Expectation().AST({ TranslationUnit,
                              FunctionDefinition,
                              BuiltinTypeSpecifier,
                              FunctionDeclarator,
                              IdentifierDeclarator,
                              ParameterSuffix,
                              ParameterDeclaration,
                              IdentifierDeclarator,
                              CompoundStatement }));
}

void ParserTester::case0246()
{
    parse("void x ( y , int ) ;",
          Expectation().AST({ TranslationUnit,
                              VariableAndOrFunctionDeclaration,
                              BuiltinTypeSpecifier,
                              FunctionDeclarator,
                              IdentifierDeclarator,
                              ParameterSuffix,
                              ParameterDeclaration,
                              TypedefName,
                              AbstractDeclarator,
                              ParameterDeclaration,
                              BuiltinTypeSpecifier,
                              AbstractDeclarator }));
}

void ParserTester::case0247()
{
    parse("void x ( int , y ) ;",
          Expectation().AST({ TranslationUnit,
                              VariableAndOrFunctionDeclaration,
                              BuiltinTypeSpecifier,
                              FunctionDeclarator,
                              IdentifierDeclarator,
                              ParameterSuffix,
                              ParameterDeclaration,
                              BuiltinTypeSpecifier,
                              AbstractDeclarator,
                              ParameterDeclaration,
                              TypedefName,
                              AbstractDeclarator }));
}

void ParserTester::case0248()
{
    parse("void x ( y , z ) ;",
          Expectation().AST({ TranslationUnit,
                              VariableAndOrFunctionDeclaration,
                              BuiltinTypeSpecifier,
                              FunctionDeclarator,
                              IdentifierDeclarator,
                              ParameterSuffix,
                              ParameterDeclaration,
                              TypedefName,
                              AbstractDeclarator,
                              ParameterDeclaration,
                              TypedefName,
                              AbstractDeclarator }));
}

void ParserTester::case0249()
{
    parse("void x ( y * ) ;",
          Expectation().AST({ TranslationUnit,
                              VariableAndOrFunctionDeclaration,
                              BuiltinTypeSpecifier,
                              FunctionDeclarator,
                              IdentifierDeclarator,
                              ParameterSuffix,
                              ParameterDeclaration,
                              TypedefName,
                              PointerDeclarator,
                              AbstractDeclarator }));
}

void ParserTester::case0250()
{
    parse("void x ( y * , int ) ;",
          Expectation().AST({ TranslationUnit,
                              VariableAndOrFunctionDeclaration,
                              BuiltinTypeSpecifier,
                              FunctionDeclarator,
                              IdentifierDeclarator,
                              ParameterSuffix,
                              ParameterDeclaration,
                              TypedefName,
                              PointerDeclarator,
                              AbstractDeclarator,
                              ParameterDeclaration,
                              BuiltinTypeSpecifier,
                              AbstractDeclarator }));
}

void ParserTester::case0251()
{
    parse("void x ( int , y * ) ;",
          Expectation().AST({ TranslationUnit,
                              VariableAndOrFunctionDeclaration,
                              BuiltinTypeSpecifier,
                              FunctionDeclarator,
                              IdentifierDeclarator,
                              ParameterSuffix,
                              ParameterDeclaration,
                              BuiltinTypeSpecifier,
                              AbstractDeclarator,
                              ParameterDeclaration,
                              TypedefName,
                              PointerDeclarator,
                              AbstractDeclarator }));
}

void ParserTester::case0252()
{
    parse("void x ( y * , z * ) ;",
          Expectation().AST({ TranslationUnit,
                              VariableAndOrFunctionDeclaration,
                              BuiltinTypeSpecifier,
                              FunctionDeclarator,
                              IdentifierDeclarator,
                              ParameterSuffix,
                              ParameterDeclaration,
                              TypedefName,
                              PointerDeclarator,
                              AbstractDeclarator,
                              ParameterDeclaration,
                              TypedefName,
                              PointerDeclarator,
                              AbstractDeclarator }));
}

void ParserTester::case0253()
{
    parse("void x ( y const ) ;",
          Expectation().AST({ TranslationUnit,
                              VariableAndOrFunctionDeclaration,
                              BuiltinTypeSpecifier,
                              FunctionDeclarator,
                              IdentifierDeclarator,
                              ParameterSuffix,
                              ParameterDeclaration,
                              TypedefName,
                              ConstQualifier,
                              AbstractDeclarator }));
}

void ParserTester::case0254()
{
    parse("void x ( const y ) ;",
          Expectation().AST({ TranslationUnit,
                              VariableAndOrFunctionDeclaration,
                              BuiltinTypeSpecifier,
                              FunctionDeclarator,
                              IdentifierDeclarator,
                              ParameterSuffix,
                              ParameterDeclaration,
                              ConstQualifier,
                              TypedefName,
                              AbstractDeclarator }));
}

void ParserTester::case0255()
{
    parse("void x ( y const , int ) ;",
          Expectation().AST({ TranslationUnit,
                              VariableAndOrFunctionDeclaration,
                              BuiltinTypeSpecifier,
                              FunctionDeclarator,
                              IdentifierDeclarator,
                              ParameterSuffix,
                              ParameterDeclaration,
                              TypedefName,
                              ConstQualifier,
                              AbstractDeclarator,
                              ParameterDeclaration,
                              BuiltinTypeSpecifier,
                              AbstractDeclarator }));
}

void ParserTester::case0256()
{
    parse("void x ( int , y const ) ;",
          Expectation().AST({ TranslationUnit,
                              VariableAndOrFunctionDeclaration,
                              BuiltinTypeSpecifier,
                              FunctionDeclarator,
                              IdentifierDeclarator,
                              ParameterSuffix,
                              ParameterDeclaration,
                              BuiltinTypeSpecifier,
                              AbstractDeclarator,
                              ParameterDeclaration,
                              TypedefName,
                              ConstQualifier,
                              AbstractDeclarator }));
}


void ParserTester::case0257()
{
    parse("void x ( int * y , int ( * z ) ( w ) ) ;",
          Expectation().AST({ TranslationUnit,
                              VariableAndOrFunctionDeclaration,
                              BuiltinTypeSpecifier,
                              FunctionDeclarator,
                              IdentifierDeclarator,
                              ParameterSuffix,
                              ParameterDeclaration,
                              BuiltinTypeSpecifier,
                              PointerDeclarator,
                              IdentifierDeclarator,
                              ParameterDeclaration,
                              BuiltinTypeSpecifier,
                              FunctionDeclarator,
                              ParenthesizedDeclarator,
                              PointerDeclarator,
                              IdentifierDeclarator,
                              ParameterSuffix,
                              ParameterDeclaration,
                              TypedefName,
                              AbstractDeclarator }));
}

void ParserTester::case0258()
{
    parse("void x ( int * y , int ( * z ) ( w * ) ) ;",
          Expectation().AST({ TranslationUnit,
                              VariableAndOrFunctionDeclaration,
                              BuiltinTypeSpecifier,
                              FunctionDeclarator,
                              IdentifierDeclarator,
                              ParameterSuffix,
                              ParameterDeclaration,
                              BuiltinTypeSpecifier,
                              PointerDeclarator,
                              IdentifierDeclarator,
                              ParameterDeclaration,
                              BuiltinTypeSpecifier,
                              FunctionDeclarator,
                              ParenthesizedDeclarator,
                              PointerDeclarator,
                              IdentifierDeclarator,
                              ParameterSuffix,
                              ParameterDeclaration,
                              TypedefName,
                              PointerDeclarator,
                              AbstractDeclarator }));
}

void ParserTester::case0259()
{
    parse("void x ( int ( * y ) ( z ) , int * w ) ;",
          Expectation().AST({ TranslationUnit,
                              VariableAndOrFunctionDeclaration,
                              BuiltinTypeSpecifier,
                              FunctionDeclarator,
                              IdentifierDeclarator,
                              ParameterSuffix,
                              ParameterDeclaration,
                              BuiltinTypeSpecifier,
                              FunctionDeclarator,
                              ParenthesizedDeclarator,
                              PointerDeclarator,
                              IdentifierDeclarator,
                              ParameterSuffix,
                              ParameterDeclaration,
                              TypedefName,
                              AbstractDeclarator,
                              ParameterDeclaration,
                              BuiltinTypeSpecifier,
                              PointerDeclarator,
                              IdentifierDeclarator }));
}

void ParserTester::case0260()
{
    parse("void x ( int ( * y ) ( z * ) , int * w ) ;",
          Expectation().AST({ TranslationUnit,
                              VariableAndOrFunctionDeclaration,
                              BuiltinTypeSpecifier,
                              FunctionDeclarator,
                              IdentifierDeclarator,
                              ParameterSuffix,
                              ParameterDeclaration,
                              BuiltinTypeSpecifier,
                              FunctionDeclarator,
                              ParenthesizedDeclarator,
                              PointerDeclarator,
                              IdentifierDeclarator,
                              ParameterSuffix,
                              ParameterDeclaration,
                              TypedefName,
                              PointerDeclarator,
                              AbstractDeclarator,
                              ParameterDeclaration,
                              BuiltinTypeSpecifier,
                              PointerDeclarator,
                              IdentifierDeclarator }));
}

void ParserTester::case0261()
{
    parse("void x ( int [ ] ) ;",
          Expectation().AST({ TranslationUnit,
                              VariableAndOrFunctionDeclaration,
                              BuiltinTypeSpecifier,
                              FunctionDeclarator,
                              IdentifierDeclarator,
                              ParameterSuffix,
                              ParameterDeclaration,
                              BuiltinTypeSpecifier,
                              ArrayDeclarator,
                              AbstractDeclarator,
                              SubscriptSuffix })) ;
}

void ParserTester::case0262()
{
    parse("void x ( int [ 1 ] ) ;",
          Expectation().AST({ TranslationUnit,
                              VariableAndOrFunctionDeclaration,
                              BuiltinTypeSpecifier,
                              FunctionDeclarator,
                              IdentifierDeclarator,
                              ParameterSuffix,
                              ParameterDeclaration,
                              BuiltinTypeSpecifier,
                              ArrayDeclarator,
                              AbstractDeclarator,
                              SubscriptSuffix,
                              IntegerConstantExpression })) ;
}

void ParserTester::case0263()
{

}

void ParserTester::case0264()
{

}

void ParserTester::case0265()
{

}

void ParserTester::case0266()
{

}

void ParserTester::case0267()
{

}

void ParserTester::case0268()
{

}

void ParserTester::case0269()
{

}

void ParserTester::case0270()
{

}

void ParserTester::case0271()
{

}

void ParserTester::case0272()
{

}

void ParserTester::case0273()
{

}

void ParserTester::case0274()
{

}

void ParserTester::case0275()
{

}

void ParserTester::case0276()
{

}

void ParserTester::case0277()
{

}

void ParserTester::case0278()
{

}

void ParserTester::case0279()
{

}

void ParserTester::case0280()
{
    parse("void x ( y ) int y ; { }",
          Expectation().AST({ TranslationUnit,
                              FunctionDefinition,
                              BuiltinTypeSpecifier,
                              FunctionDeclarator,
                              IdentifierDeclarator,
                              ParameterSuffix,
                              ParameterDeclaration,
                              IdentifierDeclarator,
                              ExtKR_ParameterDeclaration,
                              BuiltinTypeSpecifier,
                              IdentifierDeclarator,
                              CompoundStatement }));
}

void ParserTester::case0281()
{
    parse("void x ( y ) z y ; { }",
          Expectation().AST({ TranslationUnit,
                              FunctionDefinition,
                              BuiltinTypeSpecifier,
                              FunctionDeclarator,
                              IdentifierDeclarator,
                              ParameterSuffix,
                              ParameterDeclaration,
                              IdentifierDeclarator,
                              ExtKR_ParameterDeclaration,
                              TypedefName,
                              IdentifierDeclarator,
                              CompoundStatement }));
}

void ParserTester::case0282()
{
    parse("void x ( y , z ) int y ; { }",
          Expectation().AST({ TranslationUnit,
                              FunctionDefinition,
                              BuiltinTypeSpecifier,
                              FunctionDeclarator,
                              IdentifierDeclarator,
                              ParameterSuffix,
                              ParameterDeclaration,
                              IdentifierDeclarator,
                              ParameterDeclaration,
                              IdentifierDeclarator,
                              ExtKR_ParameterDeclaration,
                              BuiltinTypeSpecifier,
                              IdentifierDeclarator,
                              CompoundStatement }));
}

void ParserTester::case0283()
{
    parse("void x ( y , z ) int y ; int z ; { }",
          Expectation().AST({ TranslationUnit,
                              FunctionDefinition,
                              BuiltinTypeSpecifier,
                              FunctionDeclarator,
                              IdentifierDeclarator,
                              ParameterSuffix,
                              ParameterDeclaration,
                              IdentifierDeclarator,
                              ParameterDeclaration,
                              IdentifierDeclarator,
                              ExtKR_ParameterDeclaration,
                              BuiltinTypeSpecifier,
                              IdentifierDeclarator,
                              ExtKR_ParameterDeclaration,
                              BuiltinTypeSpecifier,
                              IdentifierDeclarator,
                              CompoundStatement }));
}

void ParserTester::case0284()
{
    parse("void x ( y , z ) int y , z ; { }",
          Expectation().AST({ TranslationUnit,
                              FunctionDefinition,
                              BuiltinTypeSpecifier,
                              FunctionDeclarator,
                              IdentifierDeclarator,
                              ParameterSuffix,
                              ParameterDeclaration,
                              IdentifierDeclarator,
                              ParameterDeclaration,
                              IdentifierDeclarator,
                              ExtKR_ParameterDeclaration,
                              BuiltinTypeSpecifier,
                              IdentifierDeclarator,
                              IdentifierDeclarator,
                              CompoundStatement }));
}

void ParserTester::case0285()
{
    parse("void x ( y , z ) int y ; int z ( int ) ; { }",
          Expectation().AST({ TranslationUnit,
                              FunctionDefinition,
                              BuiltinTypeSpecifier,
                              FunctionDeclarator,
                              IdentifierDeclarator,
                              ParameterSuffix,
                              ParameterDeclaration,
                              IdentifierDeclarator,
                              ParameterDeclaration,
                              IdentifierDeclarator,
                              ExtKR_ParameterDeclaration,
                              BuiltinTypeSpecifier,
                              IdentifierDeclarator,
                              ExtKR_ParameterDeclaration,
                              BuiltinTypeSpecifier,
                              FunctionDeclarator,
                              IdentifierDeclarator,
                              ParameterSuffix,
                              ParameterDeclaration,
                              BuiltinTypeSpecifier,
                              AbstractDeclarator,
                              CompoundStatement }));
}

void ParserTester::case0286()
{
    parse("void x ( y ) struct z y ; { }",
          Expectation().AST({ TranslationUnit,
                              FunctionDefinition,
                              BuiltinTypeSpecifier,
                              FunctionDeclarator,
                              IdentifierDeclarator,
                              ParameterSuffix,
                              ParameterDeclaration,
                              IdentifierDeclarator,
                              ExtKR_ParameterDeclaration,
                              StructTypeSpecifier,
                              IdentifierDeclarator,
                              CompoundStatement }));
}

void ParserTester::case0287()
{
    parse("void x ( y ) int * y ; { }",
          Expectation().AST({ TranslationUnit,
                              FunctionDefinition,
                              BuiltinTypeSpecifier,
                              FunctionDeclarator,
                              IdentifierDeclarator,
                              ParameterSuffix,
                              ParameterDeclaration,
                              IdentifierDeclarator,
                              ExtKR_ParameterDeclaration,
                              BuiltinTypeSpecifier,
                              PointerDeclarator,
                              IdentifierDeclarator,
                              CompoundStatement }));
}

void ParserTester::case0288()
{
    parse("int x ( y , z ) int * y , z ; { }",
          Expectation().AST({ TranslationUnit,
                              FunctionDefinition,
                              BuiltinTypeSpecifier,
                              FunctionDeclarator,
                              IdentifierDeclarator,
                              ParameterSuffix,
                              ParameterDeclaration,
                              IdentifierDeclarator,
                              ParameterDeclaration,
                              IdentifierDeclarator,
                              ExtKR_ParameterDeclaration,
                              BuiltinTypeSpecifier,
                              PointerDeclarator,
                              IdentifierDeclarator,
                              IdentifierDeclarator,
                              CompoundStatement }));
}

void ParserTester::case0289()
{
    parse("int x ( y , z ) int * y , * z ; { }",
          Expectation().AST({ TranslationUnit,
                              FunctionDefinition,
                              BuiltinTypeSpecifier,
                              FunctionDeclarator,
                              IdentifierDeclarator,
                              ParameterSuffix,
                              ParameterDeclaration,
                              IdentifierDeclarator,
                              ParameterDeclaration,
                              IdentifierDeclarator,
                              ExtKR_ParameterDeclaration,
                              BuiltinTypeSpecifier,
                              PointerDeclarator,
                              IdentifierDeclarator,
                              PointerDeclarator,
                              IdentifierDeclarator,
                              CompoundStatement }));
}

void ParserTester::case0290()
{
    parse("void x ( y ) int y ( float [ 1 ] ) ; { }",
          Expectation().AST({ TranslationUnit,
                              FunctionDefinition,
                              BuiltinTypeSpecifier,
                              FunctionDeclarator,
                              IdentifierDeclarator,
                              ParameterSuffix,
                              ParameterDeclaration,
                              IdentifierDeclarator,
                              ExtKR_ParameterDeclaration,
                              BuiltinTypeSpecifier,
                              FunctionDeclarator,
                              IdentifierDeclarator,
                              ParameterSuffix,
                              ParameterDeclaration,
                              BuiltinTypeSpecifier,
                              ArrayDeclarator,
                              AbstractDeclarator,
                              SubscriptSuffix,
                              IntegerConstantExpression,
                              CompoundStatement }));
}

void ParserTester::case0291()
{
}

void ParserTester::case0292()
{
}

void ParserTester::case0293()
{
    parse("void x ( y , z ) int y , int z { }",
          Expectation()
              .diagnostic(Expectation::ErrorOrWarn::Error,
                          Parser::DiagnosticsReporter::ID_of_ExpectedFOLLOWofDeclarator));
}

void ParserTester::case0294()
{
    parse("int x ( y ) y { }",
          Expectation()
              .diagnostic(Expectation::ErrorOrWarn::Error,
                          Parser::DiagnosticsReporter::ID_of_ExpectedFOLLOWofDeclarator));
}

void ParserTester::case0295()
{
    parse("int x ( y ) int { }",
          Expectation()
              .diagnostic(Expectation::ErrorOrWarn::Error,
                          Parser::DiagnosticsReporter::ID_of_ExpectedFOLLOWofDeclarator));
}

void ParserTester::case0296()
{
    parse("int x ( y ) int y { }",
          Expectation()
              .diagnostic(Expectation::ErrorOrWarn::Error,
                          Parser::DiagnosticsReporter::ID_of_ExpectedFOLLOWofDeclarator));
}

void ParserTester::case0297()
{
    CROSS_REFERENCE_TEST(BinderTester::case0025); // Semantic error.

    parse("void x ( int y ) int y ; { }",
          Expectation().AST({ TranslationUnit,
                              FunctionDefinition,
                              BuiltinTypeSpecifier,
                              FunctionDeclarator,
                              IdentifierDeclarator,
                              ParameterSuffix,
                              ParameterDeclaration,
                              BuiltinTypeSpecifier,
                              IdentifierDeclarator,
                              ExtKR_ParameterDeclaration,
                              BuiltinTypeSpecifier,
                              IdentifierDeclarator,
                              CompoundStatement }));
}

void ParserTester::case0298()
{
    CROSS_REFERENCE_TEST(BinderTester::case0026);  // Semantic error.

    parse("int x ( int y ) z y ; { }",
          Expectation().AST({ TranslationUnit,
                              FunctionDefinition,
                              BuiltinTypeSpecifier,
                              FunctionDeclarator,
                              IdentifierDeclarator,
                              ParameterSuffix,
                              ParameterDeclaration,
                              BuiltinTypeSpecifier,
                              IdentifierDeclarator,
                              ExtKR_ParameterDeclaration,
                              TypedefName,
                              IdentifierDeclarator,
                              CompoundStatement }));
}

void ParserTester::case0299()
{
}

void ParserTester::case0300()
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

void ParserTester::case0301()
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

void ParserTester::case0302()
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

void ParserTester::case0303()
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

void ParserTester::case0304()
{
    parse("float x [ 11 ] , * y [ 17 ] ;") ;
}

void ParserTester::case0305()
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

void ParserTester::case0306()
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

void ParserTester::case0307()
{
    parse("int x [ y + z ] ;") ;
}

void ParserTester::case0308()
{
    parse("int x [ y ] [ z ] [ 6 ] [ w ];") ;
}

void ParserTester::case0309()
{
    parse("int x [ y ] [ z ] [ 6 ] [ w + 2 ] ;") ;
}

void ParserTester::case0310()
{
    parse("int ( * const x [ 10 ] ) ( unsigned int , ... ) ;") ;
}

void ParserTester::case0311()
{
    parse("int ( * const [ ] ) ( unsigned int , ... ) ;",
          Expectation().diagnostic(Expectation::ErrorOrWarn::Error,
                                   Parser::DiagnosticsReporter::ID_of_ExpectedFIRSTofDirectDeclarator));
}

void ParserTester::case0312()
{
    // TODO: Warn about array size.

    parse("int ( * const x [ ] ) ( unsigned int , ... ) ;") ;
}

void ParserTester::case0313()
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

void ParserTester::case0314()
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

void ParserTester::case0315()
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
                              AbstractDeclarator,
                              SubscriptSuffix,
                              IntegerConstantExpression }));
}

void ParserTester::case0316()
{
    parse("int x [ static 1 ] ;",
          Expectation().diagnostic(Expectation::ErrorOrWarn::Error,
                                      Parser::DiagnosticsReporter::ID_of_UnexpectedStaticOrTypeQualifierInArrayDeclarator));
}

void ParserTester::case0317()
{
    parse("int x ( const int y [ const 1 ] ) ;");
}

void ParserTester::case0318()
{
    parse("int x ( const int [ const 1 ] ) ;");
}

void ParserTester::case0319()
{
    parse("void x ( int y [ static restrict 1 ] ) ;");
}

void ParserTester::case0320()
{
    parse("void x ( int [ static restrict 1 ] ) ;");
}

void ParserTester::case0321()
{
    parse("void x ( int [ static struct ] ) ;",
          Expectation().diagnostic(Expectation::ErrorOrWarn::Error,
                                      Parser::DiagnosticsReporter::ID_of_ExpectedFIRSTofExpression));
}

void ParserTester::case0322()
{
    parse("int x ( int y [ static const 1 ] ) ;");
}

void ParserTester::case0323()
{
    parse("int x ( int [ static const 1 ] ) ;");
}

void ParserTester::case0324()
{
    parse("int x ( int y [ const static 1 ] ) ;");
}

void ParserTester::case0325()
{
    parse("int x ( int [ const static 1 ] ) ;");
}

void ParserTester::case0326()
{
    parse("int x ( int y [ const * ] ) ;");
}

void ParserTester::case0327()
{
    parse("int x ( int y [ const volatile * ] ) ;");
}

void ParserTester::case0328()
{
    parse("int x ( int y [ const static volatile * ] ) ;",
          Expectation().diagnostic(Expectation::ErrorOrWarn::Error,
                                      Parser::DiagnosticsReporter::ID_of_ExpectedFIRSTofDirectDeclarator));
}

void ParserTester::case0329()
{
    parse("int y [ const  * ] ;",
          Expectation().setErrorCnt(1));
}

void ParserTester::case0330()
{
    parse("int y [ * ] ;",
          Expectation().setErrorCnt(1));
}

void ParserTester::case0331()
{
    parse("int y [ const ] ;",
          Expectation().diagnostic(Expectation::ErrorOrWarn::Error,
                                      Parser::DiagnosticsReporter::ID_of_UnexpectedStaticOrTypeQualifierInArrayDeclarator));
}

void ParserTester::case0332()
{
    parse("int x [ 1 ] const ;",
          Expectation().diagnostic(Expectation::ErrorOrWarn::Error,
                                      Parser::DiagnosticsReporter::ID_of_ExpectedFOLLOWofDeclarator));
}

void ParserTester::case0333()
{
    parse("int x [ 1 ] y ;",
          Expectation().diagnostic(Expectation::ErrorOrWarn::Error,
                                      Parser::DiagnosticsReporter::ID_of_ExpectedFOLLOWofDeclarator));
}

void ParserTester::case0334()
{
}

void ParserTester::case0335()
{
}

void ParserTester::case0336()
{
}

void ParserTester::case0337()
{

}

void ParserTester::case0338()
{
}

void ParserTester::case0339()
{
}

void ParserTester::case0340()
{
}

void ParserTester::case0341()
{
}

void ParserTester::case0342() { }
void ParserTester::case0343() { }
void ParserTester::case0344() { }
void ParserTester::case0345() { }
void ParserTester::case0346() { }
void ParserTester::case0347() { }
void ParserTester::case0348() { }
void ParserTester::case0349() { }

void ParserTester::case0350() { }
void ParserTester::case0351() { }
void ParserTester::case0352() { }
void ParserTester::case0353() { }
void ParserTester::case0354() { }
void ParserTester::case0355() { }
void ParserTester::case0356() { }
void ParserTester::case0357() { }
void ParserTester::case0358() { }
void ParserTester::case0359() { }

void ParserTester::case0360() { }
void ParserTester::case0361() { }
void ParserTester::case0362() { }
void ParserTester::case0363() { }
void ParserTester::case0364() { }
void ParserTester::case0365() { }
void ParserTester::case0366() { }
void ParserTester::case0367() { }
void ParserTester::case0368() { }
void ParserTester::case0369() { }

void ParserTester::case0370() { }
void ParserTester::case0371() { }
void ParserTester::case0372() { }
void ParserTester::case0373() { }
void ParserTester::case0374() { }
void ParserTester::case0375() { }
void ParserTester::case0376() { }
void ParserTester::case0377() { }
void ParserTester::case0378() { }
void ParserTester::case0379() { }

void ParserTester::case0380() { }
void ParserTester::case0381() { }
void ParserTester::case0382() { }
void ParserTester::case0383() { }
void ParserTester::case0384() { }
void ParserTester::case0385() { }
void ParserTester::case0386() { }
void ParserTester::case0387() { }
void ParserTester::case0388() { }
void ParserTester::case0389() { }

void ParserTester::case0390() { }
void ParserTester::case0391() { }
void ParserTester::case0392() { }
void ParserTester::case0393() { }
void ParserTester::case0394() { }
void ParserTester::case0395() { }
void ParserTester::case0396() { }
void ParserTester::case0397() { }
void ParserTester::case0398() { }
void ParserTester::case0399() { }

void ParserTester::case0400()
{
    parse("struct x ;",
          Expectation().AST({ TranslationUnit,
                              StructDeclaration,
                              StructTypeSpecifier }));
}

void ParserTester::case0401()
{
    parse("struct x y ;",
          Expectation().AST({ TranslationUnit,
                              VariableAndOrFunctionDeclaration,
                              StructTypeSpecifier,
                              IdentifierDeclarator }));
}

void ParserTester::case0402()
{
    parse("struct x * y ;",
          Expectation().AST({ TranslationUnit,
                              VariableAndOrFunctionDeclaration,
                              StructTypeSpecifier,
                              PointerDeclarator,
                              IdentifierDeclarator }));
}

void ParserTester::case0403()
{
    // TODO: Warn of empty member list.

    parse("struct x { } ;") ;
}

void ParserTester::case0404()
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

void ParserTester::case0405()
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

void ParserTester::case0406()
{
    CROSS_REFERENCE_TEST(BinderTester::case0014) ;

    parse("struct { int x ; } ;",
          Expectation().AST({ TranslationUnit,
                              StructDeclaration,
                              StructTypeSpecifier,
                              FieldDeclaration,
                              BuiltinTypeSpecifier,
                              IdentifierDeclarator }));
}

void ParserTester::case0407()
{
    parse("struct { x y ; } ;",
          Expectation().AST({ TranslationUnit,
                              StructDeclaration,
                              StructTypeSpecifier,
                              FieldDeclaration,
                              TypedefName,
                              IdentifierDeclarator }));
}

void ParserTester::case0408()
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

void ParserTester::case0409()
{
    parse("struct;",
          Expectation().diagnostic(Expectation::ErrorOrWarn::Error,
                                      Parser::DiagnosticsReporter::ID_of_ExpectedFOLLOWofStructOrUnionOrEnum));
}

void ParserTester::case0410()
{
    parse("struct x { void ( * y ) ( int , double ) ; } ; ") ;
}

void ParserTester::case0411()
{
    parse("struct x { void ( * y ) ( z , double ) ; } ; ") ;
}

void ParserTester::case0412()
{
    parse("struct x { void ( * y ) ( z ) ; } ; ") ;
}

void ParserTester::case0413()
{
    parse("struct x { void ( * ) ( int ) ; } ;",
          Expectation().diagnostic(Expectation::ErrorOrWarn::Error,
                                      Parser::DiagnosticsReporter::ID_of_ExpectedFIRSTofDirectDeclarator));
}

void ParserTester::case0414()
{
    parse("struct x (",
          Expectation().setErrorCnt(1)) ;
}

void ParserTester::case0415()
{
    parse("struct x { :",
          Expectation().diagnostic(Expectation::ErrorOrWarn::Error,
                                      Parser::DiagnosticsReporter::ID_of_ExpectedFIRSTofSpecifierQualifier));
}

void ParserTester::case0416()
{
    /*
     * Notes:
     *     - `typedef int y; struct x { y ; } ;'
     *        Would be valid, but the declaration doesn't declare anything.
     *
     *     - `struct x { y ; } ;'
     *        Would be invalid, because a specifier-qualifier-list is mandatory.
     */

    CROSS_REFERENCE_TEST(BinderTester::case0006); // Semantic warning.

    parse("struct x { y ; } ;");
}

void ParserTester::case0417()
{
    parse("struct x { ( y ) ; } ;",
          Expectation().diagnostic(Expectation::ErrorOrWarn::Error,
                                      Parser::DiagnosticsReporter::ID_of_ExpectedFIRSTofSpecifierQualifier));
}

void ParserTester::case0418()
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

void ParserTester::case0419()
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

void ParserTester::case0420()
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

void ParserTester::case0421()
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

void ParserTester::case0422()
{
    CROSS_REFERENCE_TEST(BinderTester::case0007); // Semantic warning.

    parse("struct x { int ; } ;");
}

void ParserTester::case0423()
{
//    parse("struct x { void ( * x ) ( ) : 1; } ;");
}

void ParserTester::case0424()
{
    parse("struct x { int y : 1, z : 2 ; } ;");
}

void ParserTester::case0425()
{
    parse("struct x { int y : 1; z w : 2 ; } ;");
}

void ParserTester::case0426()
{
    parse("struct x { int y : 1, z ; } ;");
}

void ParserTester::case0427()
{
    parse("struct x { int y : 1, z , w : 2 ; } ;");
}

void ParserTester::case0428()
{
    parse("struct { _Static_assert ( 1 , \"\" ) ; }; ");
}

void ParserTester::case0429()
{
    parse("struct x volatile y ;");
}

void ParserTester::case0430()
{
    parse("struct x { int y ; } volatile z ;");
}

void ParserTester::case0431()
{
    parse("struct { int x ; } volatile y ;");
}

void ParserTester::case0432()
{
    parse("const struct { int x ; } volatile y ;");
}

void ParserTester::case0433()
{
    parse("struct { int x ; } struct y z ;",
          Expectation().diagnostic(Expectation::ErrorOrWarn::Error,
                                      Parser::DiagnosticsReporter::ID_of_ExpectedFIRSTofDirectDeclarator));
}

void ParserTester::case0434()
{
    parse("struct x { int y ; } int z ;",
          Expectation().diagnostic(Expectation::ErrorOrWarn::Error,
                                      Parser::DiagnosticsReporter::ID_of_ExpectedFIRSTofDirectDeclarator));
}

void ParserTester::case0435()
{
    // Covered as a semantic error.
    CROSS_REFERENCE_TEST(BinderTester::case0019);

    parse("int struct x { int y ; } z ;");
}

void ParserTester::case0436()
{
    // Covered as a semantic error.
    CROSS_REFERENCE_TEST(BinderTester::case0020);

    parse("struct x struct { int y ; } z ;");
}

void ParserTester::case0437()
{
    // Covered as a semantic error.
    CROSS_REFERENCE_TEST(BinderTester::case0021);

    parse("int struct x y ;");
}

void ParserTester::case0438()
{
    // Covered as a semantic error.
    CROSS_REFERENCE_TEST(BinderTester::case0022);

    parse("struct x int y ;");
}

void ParserTester::case0439()
{
    parse("struct { int x = 1 ; } ;",
          Expectation().diagnostic(Expectation::ErrorOrWarn::Error,
                                      Parser::DiagnosticsReporter::ID_of_ExpectedFOLLOWofStructDeclarator));
}

void ParserTester::case0440()
{
    parse("struct { x y = 1 ; } ;",
          Expectation().diagnostic(Expectation::ErrorOrWarn::Error,
                                      Parser::DiagnosticsReporter::ID_of_ExpectedFOLLOWofStructDeclarator));
}

void ParserTester::case0441()
{
    parse("struct { int x : 1 = 1 ; } ;",
          Expectation().diagnostic(Expectation::ErrorOrWarn::Error,
                                      Parser::DiagnosticsReporter::ID_of_ExpectedFOLLOWofStructDeclarator));
}

void ParserTester::case0442()
{
    parse("union x ;",
          Expectation().AST({ TranslationUnit,
                              UnionDeclaration,
                              UnionTypeSpecifier }));
}

void ParserTester::case0443()
{
    parse("struct x { int y ; } ;",
          Expectation().AST({ TranslationUnit,
                              StructDeclaration,
                              StructTypeSpecifier,
                              FieldDeclaration,
                              BuiltinTypeSpecifier,
                              IdentifierDeclarator }));
}

void ParserTester::case0444()
{
    parse("struct x { int y ; double z ; } ;",
          Expectation().AST({ TranslationUnit,
                              StructDeclaration,
                              StructTypeSpecifier,
                              FieldDeclaration,
                              BuiltinTypeSpecifier,
                              IdentifierDeclarator,
                              FieldDeclaration,
                              BuiltinTypeSpecifier,
                              IdentifierDeclarator }));
}

void ParserTester::case0445()
{
    parse("struct x { struct y { int z ; } w ; } ;",
          Expectation().AST({ TranslationUnit,
                              StructDeclaration,
                              StructTypeSpecifier,
                              FieldDeclaration,
                              TypeDeclarationAsSpecifier,
                              StructTypeSpecifier,
                              FieldDeclaration,
                              BuiltinTypeSpecifier,
                              IdentifierDeclarator,
                              IdentifierDeclarator }));
}

void ParserTester::case0446()
{
    // Covered as a semantic error.
    CROSS_REFERENCE_TEST(BinderTester::case0015);

    parse("struct x { struct y { int z ; } ; } ;",
          Expectation().AST({ TranslationUnit,
                              StructDeclaration,
                              StructTypeSpecifier,
                              StructDeclaration,
                              StructTypeSpecifier,
                              FieldDeclaration,
                              BuiltinTypeSpecifier,
                              IdentifierDeclarator }));
}

void ParserTester::case0447()
{

}

void ParserTester::case0448()
{

}

void ParserTester::case0449()
{

}

void ParserTester::case0450()
{

}

void ParserTester::case0451()
{

}

void ParserTester::case0452()
{

}

void ParserTester::case0453()
{

}

void ParserTester::case0454()
{

}

void ParserTester::case0455()
{

}

void ParserTester::case0456()
{

}

void ParserTester::case0457()
{

}

void ParserTester::case0458()
{

}

void ParserTester::case0459()
{

}

void ParserTester::case0460()
{

}

void ParserTester::case0461()
{

}

void ParserTester::case0462()
{

}

void ParserTester::case0463()
{

}

void ParserTester::case0464()
{

}

void ParserTester::case0465()
{

}

void ParserTester::case0466()
{

}

void ParserTester::case0467()
{

}

void ParserTester::case0468()
{

}

void ParserTester::case0469()
{

}

void ParserTester::case0470()
{

}

void ParserTester::case0471()
{

}

void ParserTester::case0472()
{

}

void ParserTester::case0473()
{

}

void ParserTester::case0474()
{

}

void ParserTester::case0475()
{

}

void ParserTester::case0476()
{

}

void ParserTester::case0477()
{

}

void ParserTester::case0478()
{

}

void ParserTester::case0479()
{

}

void ParserTester::case0480()
{

}

void ParserTester::case0481()
{

}

void ParserTester::case0482()
{

}

void ParserTester::case0483()
{

}

void ParserTester::case0484()
{

}

void ParserTester::case0485()
{

}

void ParserTester::case0486()
{

}

void ParserTester::case0487()
{

}

void ParserTester::case0488()
{

}

void ParserTester::case0489()
{

}

void ParserTester::case0490()
{

}

void ParserTester::case0491()
{

}

void ParserTester::case0492()
{

}

void ParserTester::case0493()
{

}

void ParserTester::case0494()
{

}

void ParserTester::case0495()
{

}

void ParserTester::case0496()
{

}

void ParserTester::case0497()
{

}

void ParserTester::case0498()
{

}

void ParserTester::case0499()
{

}

void ParserTester::case0500()
{
    parse("enum x ;",
          Expectation().AST({ TranslationUnit,
                              IncompleteDeclaration,
                              EnumTypeSpecifier }));
}

void ParserTester::case0501()
{
    parse("enum x y ;");
}

void ParserTester::case0502()
{
    parse("enum x { } ;");
}

void ParserTester::case0503()
{
    parse("enum x { y } ;");
}

void ParserTester::case0504()
{
    parse("enum x { y , z } ;");
}

void ParserTester::case0505()
{
    parse("enum x { y , z, } ;");
}

void ParserTester::case0506()
{
    parse("enum x { y , z = w } ;");
}

void ParserTester::case0507()
{
    parse("enum x { y , z = w, } ;");
}

void ParserTester::case0508()
{
    parse("enum x { y = 1 } ;");
}

void ParserTester::case0509()
{
    parse("enum x { y = 1, } ;");
}

void ParserTester::case0510()
{
    parse("enum x { y = 1, z } ;");
}

void ParserTester::case0511()
{
    parse("enum x { y = 1, z, w = 3 } ;");
}

void ParserTester::case0512()
{
    parse("enum x (",
          Expectation().diagnostic(Expectation::ErrorOrWarn::Error,
                                      Parser::DiagnosticsReporter::ID_of_ExpectedFIRSTofDirectDeclarator));
}

void ParserTester::case0513()
{
    parse("enum x { (",
          Expectation().diagnostic(Expectation::ErrorOrWarn::Error,
                                      Parser::DiagnosticsReporter::ID_of_ExpectedFIRSTofEnumerationConstant));
}

void ParserTester::case0514()
{
    parse("enum x { y, , z, } ;",
          Expectation().diagnostic(Expectation::ErrorOrWarn::Error,
                                      Parser::DiagnosticsReporter::ID_of_ExpectedFIRSTofEnumerationConstant));
}

void ParserTester::case0515()
{
}

void ParserTester::case0516()
{
}

void ParserTester::case0517()
{

}

void ParserTester::case0518()
{

}

void ParserTester::case0519()
{

}

void ParserTester::case0520()
{

}

void ParserTester::case0521()
{

}

void ParserTester::case0522()
{

}

void ParserTester::case0523()
{

}

void ParserTester::case0524()
{

}

void ParserTester::case0525()
{

}

void ParserTester::case0526()
{

}

void ParserTester::case0527()
{

}

void ParserTester::case0528()
{

}

void ParserTester::case0529()
{

}

void ParserTester::case0530()
{

}

void ParserTester::case0531()
{

}

void ParserTester::case0532()
{

}

void ParserTester::case0533()
{

}

void ParserTester::case0534()
{

}

void ParserTester::case0535()
{

}

void ParserTester::case0536()
{

}

void ParserTester::case0537()
{

}

void ParserTester::case0538()
{

}

void ParserTester::case0539()
{

}

void ParserTester::case0540()
{

}

void ParserTester::case0541()
{

}

void ParserTester::case0542()
{

}

void ParserTester::case0543()
{

}

void ParserTester::case0544()
{

}

void ParserTester::case0545()
{

}

void ParserTester::case0546()
{

}

void ParserTester::case0547()
{

}

void ParserTester::case0548()
{

}

void ParserTester::case0549()
{

}

void ParserTester::case0550()
{

}

void ParserTester::case0551()
{

}

void ParserTester::case0552()
{

}

void ParserTester::case0553()
{

}

void ParserTester::case0554()
{

}

void ParserTester::case0555()
{

}

void ParserTester::case0556()
{

}

void ParserTester::case0557()
{

}

void ParserTester::case0558()
{

}

void ParserTester::case0559()
{

}

void ParserTester::case0560()
{

}

void ParserTester::case0561()
{

}

void ParserTester::case0562()
{

}

void ParserTester::case0563()
{

}

void ParserTester::case0564()
{

}

void ParserTester::case0565()
{

}

void ParserTester::case0566()
{

}

void ParserTester::case0567()
{

}

void ParserTester::case0568()
{

}

void ParserTester::case0569()
{

}

void ParserTester::case0570()
{

}

void ParserTester::case0571()
{

}

void ParserTester::case0572()
{

}

void ParserTester::case0573()
{

}

void ParserTester::case0574()
{

}

void ParserTester::case0575()
{

}

void ParserTester::case0576()
{

}

void ParserTester::case0577()
{

}

void ParserTester::case0578()
{

}

void ParserTester::case0579()
{

}

void ParserTester::case0580()
{

}

void ParserTester::case0581()
{

}

void ParserTester::case0582()
{

}

void ParserTester::case0583()
{

}

void ParserTester::case0584()
{

}

void ParserTester::case0585()
{

}

void ParserTester::case0586()
{

}

void ParserTester::case0587()
{

}

void ParserTester::case0588()
{

}

void ParserTester::case0589()
{

}

void ParserTester::case0590()
{

}

void ParserTester::case0591()
{

}

void ParserTester::case0592()
{

}

void ParserTester::case0593()
{

}

void ParserTester::case0594()
{

}

void ParserTester::case0595()
{

}

void ParserTester::case0596()
{

}

void ParserTester::case0597()
{

}

void ParserTester::case0598()
{

}

void ParserTester::case0599()
{

}

void ParserTester::case0600()
{
    parse("extern void * x ( int ) ;") ;
}

void ParserTester::case0601()
{
    parse("extern void * x ( y ) ;") ;
}

void ParserTester::case0602()
{
    parse("extern const volatile int x ;") ;
}

void ParserTester::case0603()
{
    parse("extern const volatile x y ;") ;
}

void ParserTester::case0604()
{
    parse("extern int x [] ;") ;
}

void ParserTester::case0605()
{
    parse("extern int x [ 100 ] ;") ;
}

void ParserTester::case0606()
{
    parse("extern x [ ] ;") ;
}

void ParserTester::case0607()
{
    parse("extern x [ 100 ] ;") ;
}

void ParserTester::case0608()
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

void ParserTester::case0609()
{
    parse("extern int * x ;") ;
}

void ParserTester::case0610()
{
    parse("extern x * y ;") ;
}

void ParserTester::case0611()
{
    parse("extern int x ;") ;
}

void ParserTester::case0612()
{
    parse("extern x y ;") ;
}

void ParserTester::case0613()
{
    parse("extern int ( * x ) [ y ] ;") ;
}

void ParserTester::case0614()
{
    parse("extern int ( * x ) [ y ] = & z ;") ;
}

void ParserTester::case0615()
{
    parse("extern int x = 1 ;") ;
}

void ParserTester::case0616()
{
    parse("typedef int x ;") ;
}

void ParserTester::case0617()
{
    parse("typedef x y ;") ;
}

void ParserTester::case0618()
{
    parse("typedef int * x ;") ;
}

void ParserTester::case0619()
{
    parse("typedef x * y ;") ;
}

void ParserTester::case0620()
{
    parse("typedef int ( * y ) ( double ) ;") ;
}

void ParserTester::case0621()
{
    parse("typedef x ( * y ) ( double ) ;") ;
}

void ParserTester::case0622()
{
    parse("typedef int x __attribute__ ( ( aligned ( 8 ) ));") ;
}

void ParserTester::case0623()
{
    parse("typedef x y __attribute__ ( ( deprecated ));") ;
}

void ParserTester::case0624()
{
    parse(R"(extern int __attribute__ ( ( alias ( "x" ) ) ) y ; )") ;
}

void ParserTester::case0625()
{
    parse("extern int x __attribute__ ( ( deprecated ));") ;
}

void ParserTester::case0626()
{
    parse("typedef __attribute__ ( ( alloc_size ( 1 , 2 ) ) ) void * ( * x ) ( int , int ) ;") ;
}

void ParserTester::case0627()
{
    parse("typedef __attribute__ ( ( alloc_size ( 1 , 2 ) ) ) void * ( * x ) ( y , z ) ;") ;
}

void ParserTester::case0628()
{
    parse("_Thread_local int x ;");
}

void ParserTester::case0629()
{
    parse("_Thread_local x y ;");
}

void ParserTester::case0630()
{
    parse("__thread int x ;");
}

void ParserTester::case0631()
{
    parse("void x ( ) { static __thread x y ; }");
}

void ParserTester::case0632()
{
    // Not a syntactic error, but a semantic one; covered in:
    CROSS_REFERENCE_TEST(BinderTester::case0015);

    parse("void x ( ) { __thread x y ; }");
}

void ParserTester::case0633()
{

}

void ParserTester::case0634()
{

}

void ParserTester::case0635()
{

}

void ParserTester::case0636()
{

}

void ParserTester::case0637()
{

}

void ParserTester::case0638()
{

}

void ParserTester::case0639()
{

}

void ParserTester::case0640()
{

}

void ParserTester::case0641()
{

}

void ParserTester::case0642()
{

}

void ParserTester::case0643()
{

}

void ParserTester::case0644()
{

}

void ParserTester::case0645()
{

}

void ParserTester::case0646()
{

}

void ParserTester::case0647()
{

}

void ParserTester::case0648()
{

}

void ParserTester::case0649()
{

}

void ParserTester::case0650()
{
    parse("_Static_assert ( 1 != 2 , \"x\" ) ;") ;
}

void ParserTester::case0651()
{

}

void ParserTester::case0652()
{

}

void ParserTester::case0653()
{

}

void ParserTester::case0654()
{

}

void ParserTester::case0655()
{

}

void ParserTester::case0656()
{

}

void ParserTester::case0657()
{

}

void ParserTester::case0658()
{

}

void ParserTester::case0659()
{

}

void ParserTester::case0660()
{

}

void ParserTester::case0661()
{

}

void ParserTester::case0662()
{

}

void ParserTester::case0663()
{

}

void ParserTester::case0664()
{

}

void ParserTester::case0665()
{

}

void ParserTester::case0666()
{

}

void ParserTester::case0667()
{

}

void ParserTester::case0668()
{

}

void ParserTester::case0669()
{

}

void ParserTester::case0670()
{
    parse("__extension__ int x ;");
}

void ParserTester::case0671()
{
    parse("__extension__ struct x { } ;");
}

void ParserTester::case0672()
{
    parse("struct x { __extension__ int y ; } ;");
}

void ParserTester::case0673()
{
    parse("int x = __extension__ 0 ;");
}

void ParserTester::case0674()
{
    parse("__extension__ void  x ( ) { }");
}

void ParserTester::case0675()
{
    parse("__extension__ asm ( \"nop\" ) ;");
}

void ParserTester::case0676()
{
    parseStatement("{ __extension__ int x ; }");
}

void ParserTester::case0677()
{
    parseStatement("__extension__ for ( ; ; ) { } ",
                   Expectation().diagnostic(Expectation::ErrorOrWarn::Error,
                                               Parser::DiagnosticsReporter::ID_of_UnexpectedGNUExtensionFlag));
}

void ParserTester::case0678()
{
    // Accepted by GCC, ICC, but not by Clang (11.0.0).

    parseStatement("for ( __extension__  int x ; ; ) { } ");
}

void ParserTester::case0679()
{

}

void ParserTester::case0680()
{

}

void ParserTester::case0681()
{

}

void ParserTester::case0682()
{

}

void ParserTester::case0683()
{

}

void ParserTester::case0684()
{

}

void ParserTester::case0685()
{

}

void ParserTester::case0686()
{

}

void ParserTester::case0687()
{

}

void ParserTester::case0688()
{

}

void ParserTester::case0689()
{

}

void ParserTester::case0690()
{

}

void ParserTester::case0691()
{

}

void ParserTester::case0692()
{

}

void ParserTester::case0693()
{

}

void ParserTester::case0694()
{

}

void ParserTester::case0695()
{

}

void ParserTester::case0696()
{

}

void ParserTester::case0697()
{

}

void ParserTester::case0698()
{

}

void ParserTester::case0699()
{

}

void ParserTester::case0700()
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

void ParserTester::case0701()
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

void ParserTester::case0702()
{
    // The attribute belongs to the variable; a type attribute would've
    // been placed immediately after the `struct' keyword, or,
    // alternatively, after the closing brace.

    CROSS_REFERENCE_TEST(ParserTester::case0703) ;
    CROSS_REFERENCE_TEST(ParserTester::case0704) ;

    parse("struct x __attribute__ ( ( vector_size ( 16 ) ) ) y ;") ;

}

void ParserTester::case0703()
{
    // The attribute belongs to the type.

    CROSS_REFERENCE_TEST(ParserTester::case0702) ;

    parse("struct __attribute__ ( ( aligned ) ) x { } ;") ;
}

void ParserTester::case0704()
{
    // The attribute belongs to the type.

    CROSS_REFERENCE_TEST(ParserTester::case0702) ;

    parse("struct x { } __attribute__ ( ( aligned ) ) ;") ;
}

void ParserTester::case0705()
{
    // The attribute belongs to the type.

    CROSS_REFERENCE_TEST(ParserTester::case0702) ;

    parse("struct x { short y [ 3 ] ; } __attribute__ ( ( aligned ( 8 ) ) ) ;") ;
}

void ParserTester::case0706()
{
    parse("struct x { int y [ 2 ] __attribute__ ( ( aligned ( 8 ) ) ) ; } ;") ;
}

void ParserTester::case0707()
{
    parse("struct x { short y [ 3 ] ; } __attribute__ ( ( aligned (8) ) ) z ;") ;
}

void ParserTester::case0708()
{
    parse("__attribute__ ( ( availability ( macosx , introduced = 10.12 ) ) ) void x ( ) ;") ;
}

void ParserTester::case0709()
{
    parse("__attribute__ ( ( availability ( macos , introduced = 10.4 , deprecated = 10.6 , obsoleted = 10.7) ) ) void x ( ) ;") ;
}

void ParserTester::case0710()
{
    parse("__attribute__ ( ( availability ( ios , introduced = 10 , message =\"whatever\" ) ) ) void x ( ) ;") ;
}

void ParserTester::case0711()
{
    parse("void x ( void ) __attribute__ ( ( availability ( macos , introduced = 10.4) ));") ;
}

void ParserTester::case0712()
{
    parse("void x ( void ) __attribute__ ( ( availability ( macos , introduced = 10.4 , deprecated = 10.6 , obsoleted=10.7) ));") ;
}

void ParserTester::case0713()
{
    parse("void ( __attribute__ ( ( noreturn ) ) * * * * x ) ( void ) ;") ;
}

void ParserTester::case0714()
{
    parse("char * __attribute__ ( ( aligned ( 8 ) ) ) * x ;") ;
}

void ParserTester::case0715()
{
    parse("__attribute__ ( ( format ( printf , 111 , 222 ) ) ) x ( const char * , ... ) ;") ;
}

void ParserTester::case0716()
{
    parse(R"(
          void x ( void ),
          __attribute__ ( ( format ( printf , 111, 222) ) ) y ( const char * , ... ),
          z ( void ) ;
          )") ;
}

void ParserTester::case0717()
{
    parse(R"(
          __attribute__ ( (noreturn) ) void x ( void ),
          __attribute__ ( ( format ( printf , 111, 222) ) ) y ( const char * , ... ),
          z ( void ) ;
          )") ;
}

void ParserTester::case0718()
{
    parse("int x __attribute__ ( ( aligned (16) ) ) = 0 ;") ;
}

void ParserTester::case0719()
{
    parse("x y __attribute__ ( ( aligned (16) ) ) = 0 ;") ;
}

void ParserTester::case0720()
{
    parse("short x [3] __attribute__ ( ( aligned ( __BIGGEST_ALIGNMENT__ ) ));") ;
}

void ParserTester::case0721()
{
    parse("x y [3] __attribute__ ( ( aligned ( __BIGGEST_ALIGNMENT__ ) ));") ;
}

void ParserTester::case0722()
{
    parse("enum __attribute__ ( ( aligned ) ) x { y } ;") ;
}

void ParserTester::case0723()
{
    parse("enum x { y } __attribute__ ( ( aligned ) );") ;
}

void ParserTester::case0724()
{
    parse("enum x { y __attribute__( ( deprecated ) ) } ;") ;
}

void ParserTester::case0725()
{
    parse("enum x { y __attribute__( ( deprecated ) ) = 1} ;") ;
}

void ParserTester::case0726()
{
    parse("enum x { y __attribute__( ( deprecated ) ), z } ;") ;
}

void ParserTester::case0727()
{
    parse("enum x { y __attribute__( ( deprecated ) ) = 1, z } ;") ;
}

void ParserTester::case0728()
{
    parse("struct x { int y : 1 __attribute__ ( ( ) ) ; } ;");
}

void ParserTester::case0729()
{
    parse("struct x { int __attribute__(()) y : 2 ; } ;");
}

void ParserTester::case0730()
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

void ParserTester::case0731()
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

void ParserTester::case0732()
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

void ParserTester::case0733()
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

void ParserTester::case0734()
{
    parse("struct x { } __attribute__ ( ( aligned ) ) y ;");
}

void ParserTester::case0735()
{
    parse("int __attribute__ ( ( ) ) x ;",
          Expectation().AST( { TranslationUnit,
                               VariableAndOrFunctionDeclaration,
                               BuiltinTypeSpecifier,
                               ExtGNU_AttributeSpecifier,
                               ExtGNU_Attribute,
                               IdentifierDeclarator }));
}

void ParserTester::case0736()
{
    parse("x __attribute__ ( ( ) ) y ;",
          Expectation().AST( { TranslationUnit,
                               VariableAndOrFunctionDeclaration,
                               TypedefName,
                               ExtGNU_AttributeSpecifier,
                               ExtGNU_Attribute,
                               IdentifierDeclarator }));
}

void ParserTester::case0737()
{
    // Not a syntactic error, but a semantic one; covered in:
    CROSS_REFERENCE_TEST(BinderTester::case0023);

    // Similar but without the `__attribute__'.
    CROSS_REFERENCE_TEST(ParserTester::case0021);

    parse("int __attribute__ ( ( ) ) double ;",
          Expectation().AST( { TranslationUnit,
                               IncompleteDeclaration,
                               BuiltinTypeSpecifier,
                               ExtGNU_AttributeSpecifier,
                               ExtGNU_Attribute,
                               BuiltinTypeSpecifier }));
}

void ParserTester::case0738()
{
    // Not a syntactic error, but a semantic one; covered in:
    CROSS_REFERENCE_TEST(BinderTester::case0024);

    parse("x __attribute__ ( ( ) ) int ;",
          Expectation().AST( { TranslationUnit,
                               IncompleteDeclaration,
                               TypedefName,
                               ExtGNU_AttributeSpecifier,
                               ExtGNU_Attribute,
                               BuiltinTypeSpecifier }));
}

void ParserTester::case0739()
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

void ParserTester::case0740()
{
    parse("x __attribute__ ( ( noinline ) ) f ( ) { return 0 ; }");
}

void ParserTester::case0741()
{

}

void ParserTester::case0742()
{

}

void ParserTester::case0743()
{

}

void ParserTester::case0744()
{

}

void ParserTester::case0745()
{

}

void ParserTester::case0746()
{

}

void ParserTester::case0747()
{

}

void ParserTester::case0748()
{

}

void ParserTester::case0749()
{

}

void ParserTester::case0750()
{
}

void ParserTester::case0751()
{

}

void ParserTester::case0752()
{

}

void ParserTester::case0753()
{

}

void ParserTester::case0754()
{

}

void ParserTester::case0755()
{

}

void ParserTester::case0756()
{

}

void ParserTester::case0757()
{

}

void ParserTester::case0758()
{

}

void ParserTester::case0759()
{

}

void ParserTester::case0760()
{

}

void ParserTester::case0761()
{

}

void ParserTester::case0762()
{

}

void ParserTester::case0763()
{

}

void ParserTester::case0764()
{

}

void ParserTester::case0765()
{

}

void ParserTester::case0766()
{

}

void ParserTester::case0767()
{

}

void ParserTester::case0768()
{

}

void ParserTester::case0769()
{

}

void ParserTester::case0770()
{

}

void ParserTester::case0771()
{

}

void ParserTester::case0772()
{

}

void ParserTester::case0773()
{

}

void ParserTester::case0774()
{

}

void ParserTester::case0775()
{

}

void ParserTester::case0776()
{

}

void ParserTester::case0777()
{

}

void ParserTester::case0778()
{

}

void ParserTester::case0779()
{

}

void ParserTester::case0780()
{
    parse(R"(
          asm("nop \n");
          )");
}

void ParserTester::case0781()
{
    parse("int x asm ( \"y\") = 1 ;");
}

void ParserTester::case0782()
{
    parse("int x asm ( \"y\") __attribute__ (()) = 1 ;");
}

void ParserTester::case0783()
{
    parse("int x __attribute__ (()) asm",
          Expectation().setErrorCnt(2));
}

void ParserTester::case0784()
{
    parse("int __attribute__ (()) x asm ( \"y\") = 1 ;");
}

void ParserTester::case0785()
{
    parse("int x ( int y ) asm ( \"z\" );");
}

void ParserTester::case0786()
{

}

void ParserTester::case0787()
{

}

void ParserTester::case0788()
{

}

void ParserTester::case0789()
{

}

void ParserTester::case0790()
{

}

void ParserTester::case0791()
{

}

void ParserTester::case0792()
{

}

void ParserTester::case0793()
{

}

void ParserTester::case0794()
{

}

void ParserTester::case0795()
{

}

void ParserTester::case0796()
{

}

void ParserTester::case0797()
{

}

void ParserTester::case0798()
{

}

void ParserTester::case0799()
{

}

void ParserTester::case0800()
{
    parse("x y = { z } ;") ;
}

void ParserTester::case0801()
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

void ParserTester::case0802()
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

void ParserTester::case0803()
{
    parse("struct x y = { 1, 2 } ;") ;
}

void ParserTester::case0804()
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

void ParserTester::case0805()
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

void ParserTester::case0806()
{
     parse("int x[] = { 1, 2 } ;") ;
}

void ParserTester::case0807()
{
    parse("int x[2] = { 1, 2 } ;") ;
}

void ParserTester::case0808()
{
    parse("int x[][2] = { { 1, 2 } } ;") ;
}

void ParserTester::case0809()
{
    parse("int x[][2] = { { 1, 2 }, { 3, 4 } } ;") ;
}

void ParserTester::case0810()
{
    parse("x y[][2] = { { 1, 2 }, { 3, 4 } } ;") ;
}

void ParserTester::case0811()
{
    parse("x y = { { 1, 2 }, { 3, 4 } } ;") ;
}

void ParserTester::case0812()
{
    parse("x y = { { 1, 2, 3 }, { 4 }, { 6, 7, 8} } ;") ;
}

void ParserTester::case0813()
{
    parse("x y = { 1, 2 }, y = { 3, 4, 5 } ;") ;
}

void ParserTester::case0814()
{
    parse("int x[] = { 1, 2 }, y[] = { 3, 4, 5 } ;") ;
}

void ParserTester::case0815()
{
    parse("x y[] = { 1, 2 }, z[] = { 3, 4, 5 } ;") ;
}

void ParserTester::case0816()
{
    parse("x y = { 1, , 2 } ;",
          Expectation().diagnostic(Expectation::ErrorOrWarn::Error,
                                      Parser::DiagnosticsReporter::ID_of_ExpectedFIRSTofExpression));
}

void ParserTester::case0817()
{
    // Trailing comman is OK here.

    parse("x y = { 1, 2, } ;") ;
}

void ParserTester::case0818()
{
    parse("struct x y = { } ;",
          Expectation().diagnostic(Expectation::ErrorOrWarn::Error,
                                      Parser::DiagnosticsReporter::ID_of_ExpectedBraceEnclosedInitializerList));
}

void ParserTester::case0819()
{

}

void ParserTester::case0820()
{

}

void ParserTester::case0821()
{

}

void ParserTester::case0822()
{

}

void ParserTester::case0823()
{

}

void ParserTester::case0824()
{

}

void ParserTester::case0825()
{

}

void ParserTester::case0826()
{

}

void ParserTester::case0827()
{

}

void ParserTester::case0828()
{

}

void ParserTester::case0829()
{

}

void ParserTester::case0830()
{

}

void ParserTester::case0831()
{

}

void ParserTester::case0832()
{

}

void ParserTester::case0833()
{

}

void ParserTester::case0834()
{

}

void ParserTester::case0835()
{

}

void ParserTester::case0836()
{

}

void ParserTester::case0837()
{

}

void ParserTester::case0838()
{

}

void ParserTester::case0839()
{

}

void ParserTester::case0840()
{

}

void ParserTester::case0841()
{

}

void ParserTester::case0842()
{

}

void ParserTester::case0843()
{

}

void ParserTester::case0844()
{

}

void ParserTester::case0845()
{

}

void ParserTester::case0846()
{

}

void ParserTester::case0847()
{

}

void ParserTester::case0848()
{

}

void ParserTester::case0849()
{

}

void ParserTester::case0850()
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

void ParserTester::case0851()
{
    parse("struct x y = { . z = 1, . w = 2 } ;") ;
}

void ParserTester::case0852()
{
    parse("struct x y = { . = 1 } ;",
          Expectation().diagnostic(Expectation::ErrorOrWarn::Error,
                                      Parser::DiagnosticsReporter::ID_of_ExpectedFieldDesignator));
}

void ParserTester::case0853()
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

void ParserTester::case0854()
{
    parse("int x [ 1 ] = { [ 1 ] = 1 , [ 2 ] = 2 } ;");
}

void ParserTester::case0855()
{
    parse("x y [ 1 ] = { [ 1 ] = 1 , 2 , [ 3 ] = 3 } ;");
}

void ParserTester::case0856()
{
    parse("x y [ 1 ] = { [ 1 ] = 1 , 2 , [ 3 ] = 3 , z } ;");
}

void ParserTester::case0857()
{
    parse("x y [ 1 ] = { [ 1 ] = 1 , 2 , [ 3 ] = 3 } ;");
}

void ParserTester::case0858()
{
    parse("x y [ ] = { [ 1 ] = z , } ;");
}

void ParserTester::case0859()
{
    parse("x y [ ] = { [ 1 ] } ;",
          Expectation().diagnostic(Expectation::ErrorOrWarn::Warn,
                                      Parser::DiagnosticsReporter::ID_of_ExpectedFOLLOWofDesignatedInitializer)
                       .diagnostic(Expectation::ErrorOrWarn::Error,
                                      Parser::DiagnosticsReporter::ID_of_ExpectedFIRSTofExpression));
}

void ParserTester::case0860()
{
    parse("x y [ ] = { [ 1 ] 2 } ;",
          Expectation().diagnostic(Expectation::ErrorOrWarn::Warn,
                                      Parser::DiagnosticsReporter::ID_of_ExpectedFOLLOWofDesignatedInitializer));
}

void ParserTester::case0861()
{
    parse("x y [ ] = { [ 1 ] = } ;",
          Expectation().setErrorCnt(1));
}

void ParserTester::case0862()
{
    parse("x y [ ] = { [ ] = 1 } ;",
          Expectation().setErrorCnt(1));
}

void ParserTester::case0863()
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

void ParserTester::case0864()
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

void ParserTester::case0865()
{

}

void ParserTester::case0866()
{

}

void ParserTester::case0867()
{

}

void ParserTester::case0868()
{

}

void ParserTester::case0869()
{

}

void ParserTester::case0870()
{

}

void ParserTester::case0871()
{

}

void ParserTester::case0872()
{

}

void ParserTester::case0873()
{

}

void ParserTester::case0874()
{

}

void ParserTester::case0875()
{

}

void ParserTester::case0876()
{

}

void ParserTester::case0877()
{

}

void ParserTester::case0878()
{

}

void ParserTester::case0879()
{

}

void ParserTester::case0880()
{

}

void ParserTester::case0881()
{

}

void ParserTester::case0882()
{

}

void ParserTester::case0883()
{

}

void ParserTester::case0884()
{

}

void ParserTester::case0885()
{

}

void ParserTester::case0886()
{

}

void ParserTester::case0887()
{

}

void ParserTester::case0888()
{

}

void ParserTester::case0889()
{

}

void ParserTester::case0890()
{

}

void ParserTester::case0891()
{

}

void ParserTester::case0892()
{

}

void ParserTester::case0893()
{

}

void ParserTester::case0894()
{

}

void ParserTester::case0895()
{

}

void ParserTester::case0896()
{

}

void ParserTester::case0897()
{

}

void ParserTester::case0898()
{

}

void ParserTester::case0899()
{

}

void ParserTester::case0900()
{
    parse("__typeof__ ( int ) x ;");
}

void ParserTester::case0901()
{
    parse("__typeof__ ( int * ) x ;");
}

void ParserTester::case0902()
{
    parse("__typeof__ ( int * ) x [ 1 ] ;");
}

void ParserTester::case0903()
{
    parse("__typeof__ ( __typeof__ ( int * ) [ 1 ] ) x ;");
}

void ParserTester::case0904()
{
    /*
     Despite the parsing similarities between `typeof' and `sizeof',
     mind the difference between this test case and the following:

     `sizeof ( x ) y ;'

     This `sizeof' expression is invalid, since it'd imply parsing
     the expression operand as a cast-expression, but that isn't
     allowed in such context: only a unary-expression is.
     */

    CROSS_REFERENCE_TEST(ParserTester::case2115);

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

void ParserTester::case0905()
{
    parse("__typeof__ ( x * ) y ;");
}

void ParserTester::case0906()
{
    parse("__typeof__ ( * x ) y ;");
}

void ParserTester::case0907()
{
    parse("__typeof__ ( x [ 0 ] ) y ;");
}

void ParserTester::case0908()
{
    parse("__typeof__ ( x [ 0 ] [ 1 ] ) y ;");
}

void ParserTester::case0909()
{
    parse("__typeof__ ( x [ 0 ] [ 1 ] ( ) ) y ;");
}

void ParserTester::case0910()
{
    parse("int x = ( __typeof__ ( int ) ) 1 ;");
}

void ParserTester::case0911()
{
    parse("void x ( __typeof__ ( int ) y ) ;");
}

void ParserTester::case0912()
{
    parse("void x ( ) { __typeof__ ( x * ) y ; }");
}

void ParserTester::case0913()
{
    parse("void x ( ) { __typeof__ ( int ) x ; }");
}

void ParserTester::case0914()
{
    parse("void x ( ) { sizeof ( __typeof__ ( int ) ) ; }");
}

void ParserTester::case0915()
{

}

void ParserTester::case0916()
{

}

void ParserTester::case0917()
{

}

void ParserTester::case0918()
{

}

void ParserTester::case0919()
{

}

void ParserTester::case0920()
{

}

void ParserTester::case0921()
{

}

void ParserTester::case0922()
{

}

void ParserTester::case0923()
{

}

void ParserTester::case0924()
{

}

void ParserTester::case0925()
{

}

void ParserTester::case0926()
{

}

void ParserTester::case0927()
{

}

void ParserTester::case0928()
{

}

void ParserTester::case0929()
{

}

void ParserTester::case0930()
{

}

void ParserTester::case0931()
{

}

void ParserTester::case0932()
{

}

void ParserTester::case0933()
{

}

void ParserTester::case0934()
{

}

void ParserTester::case0935()
{

}

void ParserTester::case0936()
{

}

void ParserTester::case0937()
{

}

void ParserTester::case0938()
{

}

void ParserTester::case0939()
{

}

void ParserTester::case0940()
{

}

void ParserTester::case0941()
{

}

void ParserTester::case0942()
{

}

void ParserTester::case0943()
{

}

void ParserTester::case0944()
{

}

void ParserTester::case0945()
{

}

void ParserTester::case0946()
{

}

void ParserTester::case0947()
{

}

void ParserTester::case0948()
{

}

void ParserTester::case0949()
{

}

void ParserTester::case0950()
{
    parse("_Alignas ( 16 ) int x ;");
}

void ParserTester::case0951()
{
    parse("_Alignas ( double ) int x ;");
}

void ParserTester::case0952()
{
    parse("_Alignas ( double * ) int y ;");
}

void ParserTester::case0953()
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

void ParserTester::case0954()
{
    parse("_Alignas ( x * ) int y ;");
}

void ParserTester::case0955()
{
    parse("_Alignas ( x * ) y z ;");
}

void ParserTester::case0956()
{
    parse("struct x { _Alignas ( 16 ) int y ; } ;");
}

void ParserTester::case0957()
{

}

void ParserTester::case0958()
{

}

void ParserTester::case0959()
{

}

void ParserTester::case0960()
{

}

void ParserTester::case0961()
{

}

void ParserTester::case0962()
{

}

void ParserTester::case0963()
{

}

void ParserTester::case0964()
{

}

void ParserTester::case0965()
{

}

void ParserTester::case0966()
{

}

void ParserTester::case0967()
{

}

void ParserTester::case0968()
{

}

void ParserTester::case0969()
{

}

void ParserTester::case0970()
{

}

void ParserTester::case0971()
{

}

void ParserTester::case0972()
{

}

void ParserTester::case0973()
{

}

void ParserTester::case0974()
{

}

void ParserTester::case0975()
{

}

void ParserTester::case0976()
{

}

void ParserTester::case0977()
{

}

void ParserTester::case0978()
{

}

void ParserTester::case0979()
{

}

void ParserTester::case0980()
{

}

void ParserTester::case0981()
{

}

void ParserTester::case0982()
{

}

void ParserTester::case0983()
{

}

void ParserTester::case0984()
{

}

void ParserTester::case0985()
{

}

void ParserTester::case0986()
{

}

void ParserTester::case0987()
{

}

void ParserTester::case0988()
{

}

void ParserTester::case0989()
{

}

void ParserTester::case0990()
{

}

void ParserTester::case0991()
{

}

void ParserTester::case0992()
{

}

void ParserTester::case0993()
{

}

void ParserTester::case0994()
{

}

void ParserTester::case0995()
{

}

void ParserTester::case0996()
{

}

void ParserTester::case0997()
{

}

void ParserTester::case0998()
{

}

void ParserTester::case0999()
{

}
