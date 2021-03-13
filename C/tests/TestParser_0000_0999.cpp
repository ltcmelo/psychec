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

#include "TestParser.h"

#include "TestBinder.h"
#include "Unparser.h"

#include "parser/Parser.h"
#include "syntax/SyntaxLexemes.h"
#include "syntax/SyntaxNodes.h"

#include <algorithm>
#include <cstring>
#include <iostream>
#include <string>
#include <sstream>

using namespace psy;
using namespace C;

void TestParser::case0001()
{
    parse("int x ;",
          Expectation().AST({ TranslationUnit,
                              VariableAndOrFunctionDeclaration,
                              BuiltinTypeSpecifier,
                              IdentifierDeclarator }));
}

void TestParser::case0002()
{
    parse("x y ;",
          Expectation().AST({ TranslationUnit,
                              VariableAndOrFunctionDeclaration,
                              TypedefName,
                              IdentifierDeclarator }));
}

void TestParser::case0003()
{
    parse("int x , y ;",
          Expectation().AST({ TranslationUnit,
                              VariableAndOrFunctionDeclaration,
                              BuiltinTypeSpecifier,
                              IdentifierDeclarator,
                              IdentifierDeclarator }));
}

void TestParser::case0004()
{
    parse("int x = 1;",
          Expectation().AST({ TranslationUnit,
                              VariableAndOrFunctionDeclaration,
                              BuiltinTypeSpecifier,
                              IdentifierDeclarator,
                              ExpressionInitializer,
                              IntegerConstantExpression }));
}

void TestParser::case0005()
{
    parse("int x = 1, y ;") ;
}

void TestParser::case0006()
{
    parse("int x = 1, y = 2;") ;
}

void TestParser::case0007()
{
    parse("int x , y = 3;") ;
}

void TestParser::case0008()
{
    parse("x y = 123;") ;
}

void TestParser::case0009()
{
    parse("const int x ;",
          Expectation().AST({ TranslationUnit,
                              VariableAndOrFunctionDeclaration,
                              ConstQualifier,
                              BuiltinTypeSpecifier,
                              IdentifierDeclarator }));
}

void TestParser::case0010()
{
    parse("int const x ;",
          Expectation().AST({ TranslationUnit,
                              VariableAndOrFunctionDeclaration,
                              BuiltinTypeSpecifier,
                              ConstQualifier,
                              IdentifierDeclarator }));
}

void TestParser::case0011()
{
    parse("const int volatile x ;",
          Expectation().AST({ TranslationUnit,
                              VariableAndOrFunctionDeclaration,
                              ConstQualifier,
                              BuiltinTypeSpecifier,
                              VolatileQualifier,
                              IdentifierDeclarator }));
}

void TestParser::case0012()
{
    parse("int * x ;",
          Expectation().AST({ TranslationUnit,
                              VariableAndOrFunctionDeclaration,
                              BuiltinTypeSpecifier,
                              PointerDeclarator,
                              IdentifierDeclarator }));
}

void TestParser::case0013()
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

void TestParser::case0014()
{
    parse("int * const x ;",
          Expectation().AST({ TranslationUnit,
                              VariableAndOrFunctionDeclaration,
                              BuiltinTypeSpecifier,
                              PointerDeclarator,
                              ConstQualifier,
                              IdentifierDeclarator }));
}

void TestParser::case0015()
{
    parse("int const * x ;",
          Expectation().AST({ TranslationUnit,
                              VariableAndOrFunctionDeclaration,
                              BuiltinTypeSpecifier,
                              ConstQualifier,
                              PointerDeclarator,
                              IdentifierDeclarator }));
}

void TestParser::case0016()
{
    parse("const int * x ;",
          Expectation().AST({ TranslationUnit,
                              VariableAndOrFunctionDeclaration,
                              ConstQualifier,
                              BuiltinTypeSpecifier,
                              PointerDeclarator,
                              IdentifierDeclarator }));
}

void TestParser::case0017()
{
    parse("int * x , y ;",
          Expectation().AST({ TranslationUnit,
                              VariableAndOrFunctionDeclaration,
                              BuiltinTypeSpecifier,
                              PointerDeclarator,
                              IdentifierDeclarator,
                              IdentifierDeclarator }));
}

void TestParser::case0018()
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

void TestParser::case0019()
{
    parse("int * x = 0, * y = 0 ;") ;
}

void TestParser::case0020()
{
    parse("const int * x = 0, y ;") ;
}

void TestParser::case0021()
{
    // Not a syntactic error, but a semantic one; covered in:
    CROSS_REFERENCE_TEST(TestBinder::case0005);

    parse("int double x ;") ;
}

void TestParser::case0022()
{
    /*
     * Notes:
     *     - `long int y ;'
     *        Would be valid.
     *
     *     - `typedef int x ; long x y;'
     *        Would be invalid.
     *
     *     - `x ; int y ;'
     *        Would be valid, with `x' missing a type-specifier.
     */

    parse("x int y ;",
          Expectation().addDiagnostic(Expectation::ErrorOrWarn::Error,
                                      Parser::DiagnosticsReporter::ID_of_ExpectedFOLLOWofDeclarator)
                       .addDiagnostic(Expectation::ErrorOrWarn::Warn,
                                      Parser::DiagnosticsReporter::ID_of_ExpectedTypeSpecifier));
}

void TestParser::case0023()
{
    /*
     * Notes:
     *     - `long int y ;'
     *        Would be valid.
     *
     *     - `typedef int x ; long x y ;'
     *        Would not invalid.
     */

    parse("long x y ;",
          Expectation().addDiagnostic(Expectation::ErrorOrWarn::Error,
                                      Parser::DiagnosticsReporter::ID_of_ExpectedFOLLOWofDeclarator));
}

void TestParser::case0024()
{
    parse("x * double y ;",
          Expectation().addDiagnostic(Expectation::ErrorOrWarn::Error,
                                      Parser::DiagnosticsReporter::ID_of_ExpectedFOLLOWofDeclarator)
                       .addDiagnostic(Expectation::ErrorOrWarn::Warn,
                                      Parser::DiagnosticsReporter::ID_of_ExpectedTypeSpecifier));
}

void TestParser::case0025()
{
    parse("double x * y ;",
          Expectation().addDiagnostic(Expectation::ErrorOrWarn::Error,
                                      Parser::DiagnosticsReporter::ID_of_ExpectedFOLLOWofDeclarator));
}

void TestParser::case0026()
{
    parse("x * const double y ;",
          Expectation().addDiagnostic(Expectation::ErrorOrWarn::Error,
                                      Parser::DiagnosticsReporter::ID_of_ExpectedFOLLOWofDeclarator)
                       .addDiagnostic(Expectation::ErrorOrWarn::Warn,
                                      Parser::DiagnosticsReporter::ID_of_ExpectedTypeSpecifier));
}

void TestParser::case0027()
{
    parse("double * const x y ;",
          Expectation().addDiagnostic(Expectation::ErrorOrWarn::Error,
                                      Parser::DiagnosticsReporter::ID_of_ExpectedFOLLOWofDeclarator));
}

void TestParser::case0028()
{
    // Not a syntactic error, but diagnoses a semantic warning.
    CROSS_REFERENCE_TEST(TestBinder::case0001) ;

    parse("const int ;") ;
}

void TestParser::case0029()
{
    parse("const int , double x ;",
          Expectation().addDiagnostic(Expectation::ErrorOrWarn::Error,
                                      Parser::DiagnosticsReporter::ID_of_ExpectedFIRSTofDirectDeclarator));
}

void TestParser::case0030()
{
    parse("int x = 1 {",
          Expectation().addDiagnostic(Expectation::ErrorOrWarn::Error,
                                      Parser::DiagnosticsReporter::ID_of_ExpectedFOLLOWofInitializedDeclarator));
}

void TestParser::case0031()
{
    parse("int ( x ) ;",
          Expectation().AST({ TranslationUnit,
                              VariableAndOrFunctionDeclaration,
                              BuiltinTypeSpecifier,
                              ParenthesizedDeclarator,
                              IdentifierDeclarator }));
}

void TestParser::case0032()
{
    parse("int ( ( x ) );",
          Expectation().AST({ TranslationUnit,
                              VariableAndOrFunctionDeclaration,
                              BuiltinTypeSpecifier,
                              ParenthesizedDeclarator,
                              ParenthesizedDeclarator,
                              IdentifierDeclarator }));
}

void TestParser::case0033()
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

void TestParser::case0034()
{
    parse("x ( y ) ;",
          Expectation().AST({ TranslationUnit,
                              VariableAndOrFunctionDeclaration,
                              TypedefName,
                              ParenthesizedDeclarator,
                              IdentifierDeclarator }));
}

void TestParser::case0035()
{
    parse("x ( ( y ) );",
          Expectation().AST({ TranslationUnit,
                              VariableAndOrFunctionDeclaration,
                              TypedefName,
                              ParenthesizedDeclarator,
                              ParenthesizedDeclarator,
                              IdentifierDeclarator }));
}

void TestParser::case0036()
{
    parse("* x ;",
          Expectation()
              .addDiagnostic(Expectation::ErrorOrWarn::Warn,
                             Parser::DiagnosticsReporter::ID_of_ExpectedTypeSpecifier)
              .AST({ TranslationUnit,
                     VariableAndOrFunctionDeclaration,
                     PointerDeclarator,
                     IdentifierDeclarator }));
}

void TestParser::case0037()
{
    parse("* ( x ) ;",
          Expectation()
              .addDiagnostic(Expectation::ErrorOrWarn::Warn,
                             Parser::DiagnosticsReporter::ID_of_ExpectedTypeSpecifier)
              .AST({ TranslationUnit,
                     VariableAndOrFunctionDeclaration,
                     PointerDeclarator,
                     ParenthesizedDeclarator,
                     IdentifierDeclarator }));
}

void TestParser::case0038()
{
    parse("int ( * x ) ;",
          Expectation().AST({ TranslationUnit,
                              VariableAndOrFunctionDeclaration,
                              BuiltinTypeSpecifier,
                              ParenthesizedDeclarator,
                              PointerDeclarator,
                              IdentifierDeclarator }));
}

void TestParser::case0039()
{
    parse("x ( * y ) ;",
          Expectation().AST({ TranslationUnit,
                              VariableAndOrFunctionDeclaration,
                              TypedefName,
                              ParenthesizedDeclarator,
                              PointerDeclarator,
                              IdentifierDeclarator }));
}

void TestParser::case0040()
{
    CROSS_REFERENCE_TEST(TestBinder::case0013); // Semantic warning.

    parse("int ;");
}

void TestParser::case0041()
{
    parse("x ;",
          Expectation().addDiagnostic(Expectation::ErrorOrWarn::Warn,
                                      Parser::DiagnosticsReporter::ID_of_ExpectedTypeSpecifier));
}

void TestParser::case0042()
{

}

void TestParser::case0043()
{
}

void TestParser::case0044()
{

}

void TestParser::case0045()
{
}

void TestParser::case0046()
{

}

void TestParser::case0047()
{

}

void TestParser::case0048()
{

}

void TestParser::case0049()
{

}

void TestParser::case0050()
{

}

void TestParser::case0051()
{
}

void TestParser::case0052()
{
}

void TestParser::case0053()
{

}

void TestParser::case0054()
{
}

void TestParser::case0055()
{

}

void TestParser::case0056()
{

}

void TestParser::case0057()
{

}

void TestParser::case0058()
{

}

void TestParser::case0059()
{
}

void TestParser::case0060()
{
}

void TestParser::case0061()
{

}

void TestParser::case0062()
{

}

void TestParser::case0063()
{
}

void TestParser::case0064()
{

}

void TestParser::case0065()
{

}

void TestParser::case0066()
{

}

void TestParser::case0067()
{

}

void TestParser::case0068()
{
}

void TestParser::case0069()
{

}

void TestParser::case0070()
{

}

void TestParser::case0071()
{

}

void TestParser::case0072()
{
}

void TestParser::case0073()
{

}

void TestParser::case0074()
{

}

void TestParser::case0075()
{

}

void TestParser::case0076()
{

}

void TestParser::case0077()
{

}

void TestParser::case0078()
{

}

void TestParser::case0079()
{

}

void TestParser::case0080()
{

}

void TestParser::case0081()
{
}

void TestParser::case0082()
{
}

void TestParser::case0083()
{

}

void TestParser::case0084()
{

}

void TestParser::case0085()
{

}

void TestParser::case0086()
{

}

void TestParser::case0087()
{

}

void TestParser::case0088()
{

}

void TestParser::case0089()
{
}

void TestParser::case0090()
{
}

void TestParser::case0091()
{
}

void TestParser::case0092()
{
}

void TestParser::case0093()
{

}

void TestParser::case0094()
{
}

void TestParser::case0095()
{
}

void TestParser::case0096()
{
}

void TestParser::case0097()
{

}

void TestParser::case0098()
{
}

void TestParser::case0099()
{

}

void TestParser::case0100()
{
    parse("void ( * ) ( ) ;",
          Expectation().addDiagnostic(Expectation::ErrorOrWarn::Error,
                                      Parser::DiagnosticsReporter::ID_of_ExpectedFIRSTofDirectDeclarator));
}

void TestParser::case0101()
{
    parse("void ( * x) ( ) ;",
          Expectation().AST({ TranslationUnit,
                              VariableAndOrFunctionDeclaration,
                              BuiltinTypeSpecifier,
                              FunctionDeclarator,
                              ParenthesizedDeclarator,
                              PointerDeclarator,
                              IdentifierDeclarator,
                              ParameterSuffix }));
}

void TestParser::case0102()
{
    parse("void ( * x ) ( ) { }",
          Expectation().addDiagnostic(Expectation::ErrorOrWarn::Error,
                                      Parser::DiagnosticsReporter::ID_of_ExpectedFOLLOWofDeclarator));
}

void TestParser::case0103()
{
    parse("void ( ( * x ) ) ( ) { }",
          Expectation().addDiagnostic(Expectation::ErrorOrWarn::Error,
                                      Parser::DiagnosticsReporter::ID_of_ExpectedFOLLOWofDeclarator));
}

void TestParser::case0104()
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

void TestParser::case0105()
{
    parse("void ( * ) ( int ) ;",
          Expectation().addDiagnostic(Expectation::ErrorOrWarn::Error,
                                      Parser::DiagnosticsReporter::ID_of_ExpectedFIRSTofDirectDeclarator));
}

void TestParser::case0106()
{
    parse("void ( * x ) ( int ) ;") ;
}

void TestParser::case0107()
{
    parse("x ( * y ) ( int ) ;") ;
}

void TestParser::case0108()
{
    // The `signal' function.

    parse("void ( * x ( int , void ( * y ) ( int ) ) ) ( int ) ;") ;
}

void TestParser::case0109()
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

void TestParser::case0110()
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

void TestParser::case0111()
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

void TestParser::case0112()
{
    parse("int ( * x ) [ 4 ] [ y + 1 ] ;") ;
}

void TestParser::case0113()
{
    parse("void ( * * * x ) ( int ) ;") ;
}

void TestParser::case0114()
{
    parse("x ( * * * y ) ( z ) ;") ;
}

void TestParser::case0115()
{
    parse("x ( * ( * ( * y ) ) ) ( z ) ;") ;
}

void TestParser::case0116()
{
    parse("int ( * x ) [ 9 ] ;",
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

void TestParser::case0117()
{
    parse("x ( * y ) [ 9 ];",
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

void TestParser::case0118()
{
    parse("int ( * x ) [ 9 ] [ 3 ] ;",
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

void TestParser::case0119()
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

void TestParser::case0120()
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

void TestParser::case0121()
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

void TestParser::case0122()
{
    parse("void ( * * ( x ) ) ( ) = 0 ;");
}

void TestParser::case0123()
{
    parse("void ( * ( * ( x ) ) ) ( ) = 0 ;");
}

void TestParser::case0124()
{

}

void TestParser::case0125()
{

}

void TestParser::case0126()
{

}

void TestParser::case0127()
{

}

void TestParser::case0128()
{

}

void TestParser::case0129()
{

}

void TestParser::case0130()
{

}

void TestParser::case0131()
{

}

void TestParser::case0132()
{

}

void TestParser::case0133()
{

}

void TestParser::case0134()
{

}

void TestParser::case0135()
{

}

void TestParser::case0136()
{

}

void TestParser::case0137()
{

}

void TestParser::case0138()
{

}

void TestParser::case0139()
{

}

void TestParser::case0140()
{

}

void TestParser::case0141()
{

}

void TestParser::case0142()
{

}

void TestParser::case0143()
{

}

void TestParser::case0144()
{

}

void TestParser::case0145()
{

}

void TestParser::case0146()
{

}

void TestParser::case0147()
{

}

void TestParser::case0148()
{

}

void TestParser::case0149()
{

}

void TestParser::case0150()
{

}

void TestParser::case0151()
{

}

void TestParser::case0152()
{

}

void TestParser::case0153()
{

}

void TestParser::case0154()
{

}

void TestParser::case0155()
{

}

void TestParser::case0156()
{

}

void TestParser::case0157()
{

}

void TestParser::case0158()
{

}

void TestParser::case0159()
{

}

void TestParser::case0160()
{

}

void TestParser::case0161()
{

}

void TestParser::case0162()
{

}

void TestParser::case0163()
{

}

void TestParser::case0164()
{

}

void TestParser::case0165()
{

}

void TestParser::case0166()
{

}

void TestParser::case0167()
{

}

void TestParser::case0168()
{

}

void TestParser::case0169()
{

}

void TestParser::case0170()
{

}

void TestParser::case0171()
{

}

void TestParser::case0172()
{

}

void TestParser::case0173()
{

}

void TestParser::case0174()
{

}

void TestParser::case0175()
{

}

void TestParser::case0176()
{

}

void TestParser::case0177()
{

}

void TestParser::case0178()
{

}

void TestParser::case0179()
{

}

void TestParser::case0180()
{

}

void TestParser::case0181()
{

}

void TestParser::case0182()
{

}

void TestParser::case0183()
{

}

void TestParser::case0184()
{

}

void TestParser::case0185()
{

}

void TestParser::case0186()
{

}

void TestParser::case0187()
{

}

void TestParser::case0188()
{

}

void TestParser::case0189()
{

}

void TestParser::case0190()
{

}

void TestParser::case0191()
{

}

void TestParser::case0192()
{

}

void TestParser::case0193()
{

}

void TestParser::case0194()
{

}

void TestParser::case0195()
{

}

void TestParser::case0196()
{

}

void TestParser::case0197()
{

}

void TestParser::case0198()
{

}

void TestParser::case0199()
{

}

void TestParser::case0200()
{
    parse("void x ( ) ;",
          Expectation().AST({ TranslationUnit,
                              VariableAndOrFunctionDeclaration,
                              BuiltinTypeSpecifier,
                              FunctionDeclarator,
                              IdentifierDeclarator,
                              ParameterSuffix }));
}

void TestParser::case0201()
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

void TestParser::case0202()
{
    parse("void x { }",
          Expectation().addDiagnostic(Expectation::ErrorOrWarn::Error,
                                      Parser::DiagnosticsReporter::ID_of_ExpectedFOLLOWofDeclarator));
}

void TestParser::case0203()
{
    parse("void x ( ) = 1 { }",
          Expectation().addDiagnostic(Expectation::ErrorOrWarn::Error,
                                      Parser::DiagnosticsReporter::ID_of_UnexpectedInitializerOfDeclarator));
}

void TestParser::case0204()
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

void TestParser::case0205()
{
    // Not a syntactic error, but a semantic one; covered in:
    CROSS_REFERENCE_TEST(TestBinder::case0008) ;

    parse("void x ( ) ( ) { }") ;
}

void TestParser::case0206()
{
    // Not a syntactic error, but a semantic one; covered in:
    CROSS_REFERENCE_TEST(TestBinder::case0009) ;

    parse("void x ( ) ( ) ;") ;
}

void TestParser::case0207()
{
    // Not a syntactic error, but a semantic one; covered in:
    CROSS_REFERENCE_TEST(TestBinder::case0010) ;

    parse("int x ( ) [ ] ;") ;
}

void TestParser::case0208()
{
    // Not a syntactic error, but a semantic one; covered in:
    CROSS_REFERENCE_TEST(TestParser::case0111) ;

    parse("int ( x ) ( ) [ ] ;") ;
}

void TestParser::case0209()
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

void TestParser::case0210()
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

void TestParser::case0211()
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

void TestParser::case0212()
{
    // Not a syntactic error, but a semantic one; covered in:
    CROSS_REFERENCE_TEST(TestBinder::case0012) ;

    parse("void x ( int ) { }") ;
}

void TestParser::case0213()
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

void TestParser::case0214()
{
    parse("double x( int y , double z ) ;") ;
}

void TestParser::case0215()
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

void TestParser::case0216()
{
    parse("x ( int y ) ;",
          Expectation().addDiagnostic(Expectation::ErrorOrWarn::Warn,
                                      Parser::DiagnosticsReporter::ID_of_ExpectedTypeSpecifier));
}

void TestParser::case0217()
{
    parse("x ( y z ) ;",
          Expectation().addDiagnostic(Expectation::ErrorOrWarn::Warn,
                                      Parser::DiagnosticsReporter::ID_of_ExpectedTypeSpecifier));
}

void TestParser::case0218()
{
    parse("void x ( y ) ;") ;
}

void TestParser::case0219()
{
    parse("void x ( y z ) ;") ;
}

void TestParser::case0220()
{
    parse("void x ( ( int z ));",
          Expectation().addDiagnostic(Expectation::ErrorOrWarn::Warn,
                                      Parser::DiagnosticsReporter::ID_of_ExpectedTypeSpecifier));
}

void TestParser::case0221()
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

void TestParser::case0222()
{
    parse("void x ( int * ) ;") ;
}

void TestParser::case0223()
{
    parse("void x ( int ( * ));") ;
}

void TestParser::case0224()
{
    parse("void x ( int * [ 10 ] ) ;") ;
}

void TestParser::case0225()
{
    parse("void x (int ( * ) [ 10 ] ) ;") ;
}

void TestParser::case0226()
{
    parse("void x ( int ( * ) [ * ] ) ;") ;
}

void TestParser::case0227()
{
    parse("void x ( int * ( ));") ;
}

void TestParser::case0228()
{
    parse("void x ( int * ( void ));") ;
}

void TestParser::case0229()
{
    parse("void x ( int * ( int ));") ;
}

void TestParser::case0230()
{
    parse("void x ( int * ( int , double ));") ;
}

void TestParser::case0231()
{
    parse("void x ( int ( * const [ ] ) ( unsigned int , ... ));") ;
}

void TestParser::case0232()
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

void TestParser::case0233()
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

void TestParser::case0234()
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

void TestParser::case0235()
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

void TestParser::case0236()
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

void TestParser::case0237()
{
    parse("_Noreturn x y ( ) { }") ;
}

void TestParser::case0238()
{
    parse("_Noreturn void x ( ) { }") ;
}

void TestParser::case0239()
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

void TestParser::case0240()
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

void TestParser::case0241()
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

void TestParser::case0242()
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

void TestParser::case0243()
{
    parse("int [ ] x ( ) { }",
          Expectation().addDiagnostic(Expectation::ErrorOrWarn::Error,
                                      Parser::DiagnosticsReporter::ID_of_ExpectedFIRSTofDirectDeclarator));
}

void TestParser::case0244()
{

}

void TestParser::case0245()
{

}

void TestParser::case0246()
{

}

void TestParser::case0247()
{

}

void TestParser::case0248()
{

}

void TestParser::case0249()
{

}

void TestParser::case0250()
{

}

void TestParser::case0251()
{

}

void TestParser::case0252()
{

}

void TestParser::case0253()
{

}

void TestParser::case0254()
{

}

void TestParser::case0255()
{

}

void TestParser::case0256()
{

}

void TestParser::case0257()
{

}

void TestParser::case0258()
{

}

void TestParser::case0259()
{

}

void TestParser::case0260()
{

}

void TestParser::case0261()
{

}

void TestParser::case0262()
{

}

void TestParser::case0263()
{

}

void TestParser::case0264()
{

}

void TestParser::case0265()
{

}

void TestParser::case0266()
{

}

void TestParser::case0267()
{

}

void TestParser::case0268()
{

}

void TestParser::case0269()
{

}

void TestParser::case0270()
{

}

void TestParser::case0271()
{

}

void TestParser::case0272()
{

}

void TestParser::case0273()
{

}

void TestParser::case0274()
{

}

void TestParser::case0275()
{

}

void TestParser::case0276()
{

}

void TestParser::case0277()
{

}

void TestParser::case0278()
{

}

void TestParser::case0279()
{

}

void TestParser::case0280()
{

}

void TestParser::case0281()
{

}

void TestParser::case0282()
{

}

void TestParser::case0283()
{

}

void TestParser::case0284()
{

}

void TestParser::case0285()
{

}

void TestParser::case0286()
{

}

void TestParser::case0287()
{

}

void TestParser::case0288()
{

}

void TestParser::case0289()
{

}

void TestParser::case0290()
{

}

void TestParser::case0291()
{

}

void TestParser::case0292()
{

}

void TestParser::case0293()
{

}

void TestParser::case0294()
{

}

void TestParser::case0295()
{

}

void TestParser::case0296()
{

}

void TestParser::case0297()
{

}

void TestParser::case0298()
{

}

void TestParser::case0299()
{

}

void TestParser::case0300()
{
    parse("int x [ y ] ;",
          Expectation().AST({ TranslationUnit,
                              VariableAndOrFunctionDeclaration,
                              BuiltinTypeSpecifier,
                              ArrayDeclarator,
                              IdentifierDeclarator,
                              SubscriptSuffix,
                              IdentifierExpression }));
}

void TestParser::case0301()
{
    parse("x y [ z ] ;",
          Expectation().AST({ TranslationUnit,
                              VariableAndOrFunctionDeclaration,
                              TypedefName,
                              ArrayDeclarator,
                              IdentifierDeclarator,
                              SubscriptSuffix,
                              IdentifierExpression }));
}

void TestParser::case0302()
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

void TestParser::case0303()
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

void TestParser::case0304()
{
    parse("float x [ 11 ] , * y [ 17 ] ;") ;
}

void TestParser::case0305()
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
                              IdentifierExpression,
                              SubscriptSuffix,
                              IntegerConstantExpression,
                              SubscriptSuffix,
                              IdentifierExpression }));
}

void TestParser::case0306()
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
                              IdentifierExpression,
                              SubscriptSuffix,
                              IdentifierExpression,
                              SubscriptSuffix,
                              IntegerConstantExpression }));
}

void TestParser::case0307()
{
    parse("int x [ y + z ] ;") ;
}

void TestParser::case0308()
{
    parse("int x [ y ] [ z ] [ 6 ] [ w ];") ;
}

void TestParser::case0309()
{
    parse("int x [ y ] [ z ] [ 6 ] [ w + 2 ] ;") ;
}

void TestParser::case0310()
{
    parse("int ( * const x [ 10 ] ) ( unsigned int , ... ) ;") ;
}

void TestParser::case0311()
{
    parse("int ( * const [ ] ) ( unsigned int , ... ) ;",
          Expectation().addDiagnostic(Expectation::ErrorOrWarn::Error,
                                      Parser::DiagnosticsReporter::ID_of_ExpectedFIRSTofDirectDeclarator));
}

void TestParser::case0312()
{
    // TODO: Warn about array size.

    parse("int ( * const x [ ] ) ( unsigned int , ... ) ;") ;
}

void TestParser::case0313()
{
    parse("int * x [ 9 ];",
          Expectation().AST({ TranslationUnit,
                              VariableAndOrFunctionDeclaration,
                              BuiltinTypeSpecifier,
                              PointerDeclarator,
                              ArrayDeclarator,
                              IdentifierDeclarator,
                              SubscriptSuffix,
                              IntegerConstantExpression }));
}

void TestParser::case0314()
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

void TestParser::case0315()
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

void TestParser::case0316()
{
    parse("int x [ static 1 ] ;",
          Expectation().addDiagnostic(Expectation::ErrorOrWarn::Error,
                                      Parser::DiagnosticsReporter::ID_of_UnexpectedStaticOrTypeQualifierInArrayDeclarator));
}

void TestParser::case0317()
{
    parse("int x ( const int y [ const 1 ] ) ;");
}

void TestParser::case0318()
{
    parse("int x ( const int [ const 1 ] ) ;");
}

void TestParser::case0319()
{
    parse("void x ( int y [ static restrict 1 ] ) ;");
}

void TestParser::case0320()
{
    parse("void x ( int [ static restrict 1 ] ) ;");
}

void TestParser::case0321()
{
    parse("void x ( int [ static struct ] ) ;",
          Expectation().addDiagnostic(Expectation::ErrorOrWarn::Error,
                                      Parser::DiagnosticsReporter::ID_of_ExpectedFIRSTofExpression));
}

void TestParser::case0322()
{
    parse("int x ( int y [ static const 1 ] ) ;");
}

void TestParser::case0323()
{
    parse("int x ( int [ static const 1 ] ) ;");
}

void TestParser::case0324()
{
    parse("int x ( int y [ const static 1 ] ) ;");
}

void TestParser::case0325()
{
    parse("int x ( int [ const static 1 ] ) ;");
}

void TestParser::case0326()
{
    parse("int x ( int y [ const * ] ) ;");
}

void TestParser::case0327()
{
    parse("int x ( int y [ const volatile * ] ) ;");
}

void TestParser::case0328()
{
    parse("int x ( int y [ const static volatile * ] ) ;",
          Expectation().addDiagnostic(Expectation::ErrorOrWarn::Error,
                                      Parser::DiagnosticsReporter::ID_of_ExpectedFIRSTofDirectDeclarator));
}

void TestParser::case0329()
{
    parse("int y [ const  * ] ;", 1);
}

void TestParser::case0330()
{
    parse("int y [ * ] ;", 1);
}

void TestParser::case0331()
{
    parse("int y [ const ] ;",
          Expectation().addDiagnostic(Expectation::ErrorOrWarn::Error,
                                      Parser::DiagnosticsReporter::ID_of_UnexpectedStaticOrTypeQualifierInArrayDeclarator));
}

void TestParser::case0332()
{
}

void TestParser::case0333()
{
}

void TestParser::case0334()
{
}

void TestParser::case0335()
{
}

void TestParser::case0336()
{
}

void TestParser::case0337()
{

}

void TestParser::case0338()
{
}

void TestParser::case0339()
{
}

void TestParser::case0340()
{
}

void TestParser::case0341()
{
}

void TestParser::case0342() { }
void TestParser::case0343() { }
void TestParser::case0344() { }
void TestParser::case0345() { }
void TestParser::case0346() { }
void TestParser::case0347() { }
void TestParser::case0348() { }
void TestParser::case0349() { }

void TestParser::case0350() { }
void TestParser::case0351() { }
void TestParser::case0352() { }
void TestParser::case0353() { }
void TestParser::case0354() { }
void TestParser::case0355() { }
void TestParser::case0356() { }
void TestParser::case0357() { }
void TestParser::case0358() { }
void TestParser::case0359() { }

void TestParser::case0360() { }
void TestParser::case0361() { }
void TestParser::case0362() { }
void TestParser::case0363() { }
void TestParser::case0364() { }
void TestParser::case0365() { }
void TestParser::case0366() { }
void TestParser::case0367() { }
void TestParser::case0368() { }
void TestParser::case0369() { }

void TestParser::case0370() { }
void TestParser::case0371() { }
void TestParser::case0372() { }
void TestParser::case0373() { }
void TestParser::case0374() { }
void TestParser::case0375() { }
void TestParser::case0376() { }
void TestParser::case0377() { }
void TestParser::case0378() { }
void TestParser::case0379() { }

void TestParser::case0380() { }
void TestParser::case0381() { }
void TestParser::case0382() { }
void TestParser::case0383() { }
void TestParser::case0384() { }
void TestParser::case0385() { }
void TestParser::case0386() { }
void TestParser::case0387() { }
void TestParser::case0388() { }
void TestParser::case0389() { }

void TestParser::case0390() { }
void TestParser::case0391() { }
void TestParser::case0392() { }
void TestParser::case0393() { }
void TestParser::case0394() { }
void TestParser::case0395() { }
void TestParser::case0396() { }
void TestParser::case0397() { }
void TestParser::case0398() { }
void TestParser::case0399() { }

void TestParser::case0400()
{
    parse("struct x ;") ;
}

void TestParser::case0401()
{
    parse("struct x y ;",
          Expectation().AST({ TranslationUnit,
                              VariableAndOrFunctionDeclaration,
                              StructTypeSpecifier,
                              IdentifierDeclarator }));
}

void TestParser::case0402()
{
    parse("struct x * y ;",
          Expectation().AST({ TranslationUnit,
                              VariableAndOrFunctionDeclaration,
                              StructTypeSpecifier,
                              PointerDeclarator,
                              IdentifierDeclarator }));
}

void TestParser::case0403()
{
    // TODO: Warn of empty member list.

    parse("struct x { } ;") ;
}

void TestParser::case0404()
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

void TestParser::case0405()
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

void TestParser::case0406()
{
    CROSS_REFERENCE_TEST(TestBinder::case0014) ;

    parse("struct { int x ; } ;",
          Expectation().AST({ TranslationUnit,
                              StructDeclaration,
                              StructTypeSpecifier,
                              FieldDeclaration,
                              BuiltinTypeSpecifier,
                              IdentifierDeclarator }));
}

void TestParser::case0407()
{
    parse("struct { x y ; } ;",
          Expectation().AST({ TranslationUnit,
                              StructDeclaration,
                              StructTypeSpecifier,
                              FieldDeclaration,
                              TypedefName,
                              IdentifierDeclarator }));
}

void TestParser::case0408()
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

void TestParser::case0409()
{
    parse("struct;",
          Expectation().addDiagnostic(Expectation::ErrorOrWarn::Error,
                                      Parser::DiagnosticsReporter::ID_of_ExpectedFOLLOWofStructOrUnionOrEnum));
}

void TestParser::case0410()
{
    parse("struct x { void ( * y ) ( int , double ) ; } ; ") ;
}

void TestParser::case0411()
{
    parse("struct x { void ( * y ) ( z , double ) ; } ; ") ;
}

void TestParser::case0412()
{
    parse("struct x { void ( * y ) ( z ) ; } ; ") ;
}

void TestParser::case0413()
{
    parse("struct x { void ( * ) ( int ) ; } ;",
          Expectation().addDiagnostic(Expectation::ErrorOrWarn::Error,
                                      Parser::DiagnosticsReporter::ID_of_ExpectedFIRSTofDirectDeclarator));
}

void TestParser::case0414()
{
    parse("struct x (", 1) ;
}

void TestParser::case0415()
{
    parse("struct x { :",
          Expectation().addDiagnostic(Expectation::ErrorOrWarn::Error,
                                      Parser::DiagnosticsReporter::ID_of_ExpectedFIRSTofSpecifierQualifier));
}

void TestParser::case0416()
{
    /*
     * Notes:
     *     - `typedef int y; struct x { y ; } ;'
     *        Would be valid, but the declaration doesn't declare anything.
     *
     *     - `struct x { y ; } ;'
     *        Would be invalid, because a specifier-qualifier-list is mandatory.
     */

    CROSS_REFERENCE_TEST(TestBinder::case0006); // Semantic warning.

    parse("struct x { y ; } ;");
}

void TestParser::case0417()
{
    parse("struct x { ( y ) ; } ;",
          Expectation().addDiagnostic(Expectation::ErrorOrWarn::Error,
                                      Parser::DiagnosticsReporter::ID_of_ExpectedFIRSTofSpecifierQualifier));
}

void TestParser::case0418()
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

void TestParser::case0419()
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

void TestParser::case0420()
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

void TestParser::case0421()
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

void TestParser::case0422()
{
    CROSS_REFERENCE_TEST(TestBinder::case0007); // Semantic warning.

    parse("struct x { int ; } ;");
}

void TestParser::case0423()
{
//    parse("struct x { void ( * x ) ( ) : 1; } ;");
}

void TestParser::case0424()
{
    parse("struct x { int y : 1, z : 2 ; } ;");
}

void TestParser::case0425()
{
    parse("struct x { int y : 1; z w : 2 ; } ;");
}

void TestParser::case0426()
{
    parse("struct x { int y : 1, z ; } ;");
}

void TestParser::case0427()
{
    parse("struct x { int y : 1, z , w : 2 ; } ;");
}

void TestParser::case0428()
{

}

void TestParser::case0429()
{

}

void TestParser::case0430()
{

}

void TestParser::case0431()
{

}

void TestParser::case0432()
{

}

void TestParser::case0433()
{

}

void TestParser::case0434()
{

}

void TestParser::case0435()
{

}

void TestParser::case0436()
{

}

void TestParser::case0437()
{

}

void TestParser::case0438()
{

}

void TestParser::case0439()
{

}

void TestParser::case0440()
{

}

void TestParser::case0441()
{

}

void TestParser::case0442()
{

}

void TestParser::case0443()
{

}

void TestParser::case0444()
{

}

void TestParser::case0445()
{

}

void TestParser::case0446()
{

}

void TestParser::case0447()
{

}

void TestParser::case0448()
{

}

void TestParser::case0449()
{

}

void TestParser::case0450()
{

}

void TestParser::case0451()
{

}

void TestParser::case0452()
{

}

void TestParser::case0453()
{

}

void TestParser::case0454()
{

}

void TestParser::case0455()
{

}

void TestParser::case0456()
{

}

void TestParser::case0457()
{

}

void TestParser::case0458()
{

}

void TestParser::case0459()
{

}

void TestParser::case0460()
{

}

void TestParser::case0461()
{

}

void TestParser::case0462()
{

}

void TestParser::case0463()
{

}

void TestParser::case0464()
{

}

void TestParser::case0465()
{

}

void TestParser::case0466()
{

}

void TestParser::case0467()
{

}

void TestParser::case0468()
{

}

void TestParser::case0469()
{

}

void TestParser::case0470()
{

}

void TestParser::case0471()
{

}

void TestParser::case0472()
{

}

void TestParser::case0473()
{

}

void TestParser::case0474()
{

}

void TestParser::case0475()
{

}

void TestParser::case0476()
{

}

void TestParser::case0477()
{

}

void TestParser::case0478()
{

}

void TestParser::case0479()
{

}

void TestParser::case0480()
{

}

void TestParser::case0481()
{

}

void TestParser::case0482()
{

}

void TestParser::case0483()
{

}

void TestParser::case0484()
{

}

void TestParser::case0485()
{

}

void TestParser::case0486()
{

}

void TestParser::case0487()
{

}

void TestParser::case0488()
{

}

void TestParser::case0489()
{

}

void TestParser::case0490()
{

}

void TestParser::case0491()
{

}

void TestParser::case0492()
{

}

void TestParser::case0493()
{

}

void TestParser::case0494()
{

}

void TestParser::case0495()
{

}

void TestParser::case0496()
{

}

void TestParser::case0497()
{

}

void TestParser::case0498()
{

}

void TestParser::case0499()
{

}

void TestParser::case0500()
{
    parse("enum x ;") ;
}

void TestParser::case0501()
{
    parse("enum x y ;") ;
}

void TestParser::case0502()
{
    parse("enum x { } ;") ;
}

void TestParser::case0503()
{
    parse("enum x { y } ;") ;
}

void TestParser::case0504()
{
    parse("enum x { y , z } ;") ;
}

void TestParser::case0505()
{
    parse("enum x { y , z, } ;") ;
}

void TestParser::case0506()
{
    parse("enum x { y , z = w } ;") ;
}

void TestParser::case0507()
{
    parse("enum x { y , z = w, } ;") ;
}

void TestParser::case0508()
{
    parse("enum x { y = 1 } ;") ;
}

void TestParser::case0509()
{
    parse("enum x { y = 1, } ;") ;
}

void TestParser::case0510()
{
    parse("enum x { y = 1, z } ;") ;
}

void TestParser::case0511()
{
    parse("enum x { y = 1, z, w = 3 } ;") ;
}

void TestParser::case0512()
{
    parse("enum x (",
          Expectation().addDiagnostic(Expectation::ErrorOrWarn::Error,
                                      Parser::DiagnosticsReporter::ID_of_ExpectedFIRSTofDirectDeclarator));
}

void TestParser::case0513()
{
    parse("enum x { (",
          Expectation().addDiagnostic(Expectation::ErrorOrWarn::Error,
                                      Parser::DiagnosticsReporter::ID_of_ExpectedFIRSTofEnumerationConstant));
}

void TestParser::case0514()
{
    parse("enum x { y, , z, } ;",
          Expectation().addDiagnostic(Expectation::ErrorOrWarn::Error,
                                      Parser::DiagnosticsReporter::ID_of_ExpectedFIRSTofEnumerationConstant));
}

void TestParser::case0515()
{
}

void TestParser::case0516()
{
}

void TestParser::case0517()
{

}

void TestParser::case0518()
{

}

void TestParser::case0519()
{

}

void TestParser::case0520()
{

}

void TestParser::case0521()
{

}

void TestParser::case0522()
{

}

void TestParser::case0523()
{

}

void TestParser::case0524()
{

}

void TestParser::case0525()
{

}

void TestParser::case0526()
{

}

void TestParser::case0527()
{

}

void TestParser::case0528()
{

}

void TestParser::case0529()
{

}

void TestParser::case0530()
{

}

void TestParser::case0531()
{

}

void TestParser::case0532()
{

}

void TestParser::case0533()
{

}

void TestParser::case0534()
{

}

void TestParser::case0535()
{

}

void TestParser::case0536()
{

}

void TestParser::case0537()
{

}

void TestParser::case0538()
{

}

void TestParser::case0539()
{

}

void TestParser::case0540()
{

}

void TestParser::case0541()
{

}

void TestParser::case0542()
{

}

void TestParser::case0543()
{

}

void TestParser::case0544()
{

}

void TestParser::case0545()
{

}

void TestParser::case0546()
{

}

void TestParser::case0547()
{

}

void TestParser::case0548()
{

}

void TestParser::case0549()
{

}

void TestParser::case0550()
{

}

void TestParser::case0551()
{

}

void TestParser::case0552()
{

}

void TestParser::case0553()
{

}

void TestParser::case0554()
{

}

void TestParser::case0555()
{

}

void TestParser::case0556()
{

}

void TestParser::case0557()
{

}

void TestParser::case0558()
{

}

void TestParser::case0559()
{

}

void TestParser::case0560()
{

}

void TestParser::case0561()
{

}

void TestParser::case0562()
{

}

void TestParser::case0563()
{

}

void TestParser::case0564()
{

}

void TestParser::case0565()
{

}

void TestParser::case0566()
{

}

void TestParser::case0567()
{

}

void TestParser::case0568()
{

}

void TestParser::case0569()
{

}

void TestParser::case0570()
{

}

void TestParser::case0571()
{

}

void TestParser::case0572()
{

}

void TestParser::case0573()
{

}

void TestParser::case0574()
{

}

void TestParser::case0575()
{

}

void TestParser::case0576()
{

}

void TestParser::case0577()
{

}

void TestParser::case0578()
{

}

void TestParser::case0579()
{

}

void TestParser::case0580()
{

}

void TestParser::case0581()
{

}

void TestParser::case0582()
{

}

void TestParser::case0583()
{

}

void TestParser::case0584()
{

}

void TestParser::case0585()
{

}

void TestParser::case0586()
{

}

void TestParser::case0587()
{

}

void TestParser::case0588()
{

}

void TestParser::case0589()
{

}

void TestParser::case0590()
{

}

void TestParser::case0591()
{

}

void TestParser::case0592()
{

}

void TestParser::case0593()
{

}

void TestParser::case0594()
{

}

void TestParser::case0595()
{

}

void TestParser::case0596()
{

}

void TestParser::case0597()
{

}

void TestParser::case0598()
{

}

void TestParser::case0599()
{

}

void TestParser::case0600()
{
    parse("extern void * x ( int ) ;") ;
}

void TestParser::case0601()
{
    parse("extern void * x ( y ) ;") ;
}

void TestParser::case0602()
{
    parse("extern const volatile int x ;") ;
}

void TestParser::case0603()
{
    parse("extern const volatile x y ;") ;
}

void TestParser::case0604()
{
    parse("extern int x [] ;") ;
}

void TestParser::case0605()
{
    parse("extern int x [ 100 ] ;") ;
}

void TestParser::case0606()
{
    parse("extern x [ ] ;") ;
}

void TestParser::case0607()
{
    parse("extern x [ 100 ] ;") ;
}

void TestParser::case0608()
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

void TestParser::case0609()
{
    parse("extern int * x ;") ;
}

void TestParser::case0610()
{
    parse("extern x * y ;") ;
}

void TestParser::case0611()
{
    parse("extern int x ;") ;
}

void TestParser::case0612()
{
    parse("extern x y ;") ;
}

void TestParser::case0613()
{
    parse("extern int ( * x ) [ y ] ;") ;
}

void TestParser::case0614()
{
    parse("extern int ( * x ) [ y ] = & z ;") ;
}

void TestParser::case0615()
{
    parse("extern int x = 1 ;") ;
}

void TestParser::case0616()
{
    parse("typedef int x ;") ;
}

void TestParser::case0617()
{
    parse("typedef x y ;") ;
}

void TestParser::case0618()
{
    parse("typedef int * x ;") ;
}

void TestParser::case0619()
{
    parse("typedef x * y ;") ;
}

void TestParser::case0620()
{
    parse("typedef int ( * y ) ( double ) ;") ;
}

void TestParser::case0621()
{
    parse("typedef x ( * y ) ( double ) ;") ;
}

void TestParser::case0622()
{
    parse("typedef int x __attribute__ ( ( aligned ( 8 ) ));") ;
}

void TestParser::case0623()
{
    parse("typedef x y __attribute__ ( ( deprecated ));") ;
}

void TestParser::case0624()
{
    parse(R"(extern int __attribute__ ( ( alias ( "x" ) ) ) y ; )") ;
}

void TestParser::case0625()
{
    parse("extern int x __attribute__ ( ( deprecated ));") ;
}

void TestParser::case0626()
{
    parse("typedef __attribute__ ( ( alloc_size ( 1 , 2 ) ) ) void * ( * x ) ( int , int ) ;") ;
}

void TestParser::case0627()
{
    parse("typedef __attribute__ ( ( alloc_size ( 1 , 2 ) ) ) void * ( * x ) ( y , z ) ;") ;
}

void TestParser::case0628()
{
    parse("_Thread_local int x ;");
}

void TestParser::case0629()
{
    parse("_Thread_local x y ;");
}

void TestParser::case0630()
{
    parse("__thread int x ;");
}

void TestParser::case0631()
{
    parse("void x ( ) { static __thread x y ; }");
}

void TestParser::case0632()
{
    // Not a syntactic error, but a semantic one; covered in:
    CROSS_REFERENCE_TEST(TestBinder::case0015);

    parse("void x ( ) { __thread x y ; }");
}

void TestParser::case0633()
{

}

void TestParser::case0634()
{

}

void TestParser::case0635()
{

}

void TestParser::case0636()
{

}

void TestParser::case0637()
{

}

void TestParser::case0638()
{

}

void TestParser::case0639()
{

}

void TestParser::case0640()
{

}

void TestParser::case0641()
{

}

void TestParser::case0642()
{

}

void TestParser::case0643()
{

}

void TestParser::case0644()
{

}

void TestParser::case0645()
{

}

void TestParser::case0646()
{

}

void TestParser::case0647()
{

}

void TestParser::case0648()
{

}

void TestParser::case0649()
{

}

void TestParser::case0650()
{
    parse("_Static_assert ( 1 != 2 , \"x\" ) ;") ;
}

void TestParser::case0651()
{

}

void TestParser::case0652()
{

}

void TestParser::case0653()
{

}

void TestParser::case0654()
{

}

void TestParser::case0655()
{

}

void TestParser::case0656()
{

}

void TestParser::case0657()
{

}

void TestParser::case0658()
{

}

void TestParser::case0659()
{

}

void TestParser::case0660()
{

}

void TestParser::case0661()
{

}

void TestParser::case0662()
{

}

void TestParser::case0663()
{

}

void TestParser::case0664()
{

}

void TestParser::case0665()
{

}

void TestParser::case0666()
{

}

void TestParser::case0667()
{

}

void TestParser::case0668()
{

}

void TestParser::case0669()
{

}

void TestParser::case0670()
{
    parse("__extension__ int x ;");
}

void TestParser::case0671()
{
    parse("__extension__ struct x { } ;");
}

void TestParser::case0672()
{
    parse("struct x { __extension__ int y ; } ;");
}

void TestParser::case0673()
{
    parse("int x = __extension__ 0 ;");
}

void TestParser::case0674()
{
    parse("__extension__ void  x ( ) { }");
}

void TestParser::case0675()
{
    parse("__extension__ asm ( \"nop\" ) ;");
}

void TestParser::case0676()
{
    parseStatement("{ __extension__ int x ; }");
}

void TestParser::case0677()
{
    parseStatement("__extension__ for ( ; ; ) { } ",
                   Expectation().addDiagnostic(Expectation::ErrorOrWarn::Error,
                                               Parser::DiagnosticsReporter::ID_of_UnexpectedGNUExtensionFlag));
}

void TestParser::case0678()
{
    // Accepted by GCC, ICC, but not by Clang (11.0.0).

    parseStatement("for ( __extension__  int x ; ; ) { } ");
}

void TestParser::case0679()
{

}

void TestParser::case0680()
{

}

void TestParser::case0681()
{

}

void TestParser::case0682()
{

}

void TestParser::case0683()
{

}

void TestParser::case0684()
{

}

void TestParser::case0685()
{

}

void TestParser::case0686()
{

}

void TestParser::case0687()
{

}

void TestParser::case0688()
{

}

void TestParser::case0689()
{

}

void TestParser::case0690()
{

}

void TestParser::case0691()
{

}

void TestParser::case0692()
{

}

void TestParser::case0693()
{

}

void TestParser::case0694()
{

}

void TestParser::case0695()
{

}

void TestParser::case0696()
{

}

void TestParser::case0697()
{

}

void TestParser::case0698()
{

}

void TestParser::case0699()
{

}

void TestParser::case0700()
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

void TestParser::case0701()
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

void TestParser::case0702()
{
    // The attribute belongs to the variable; a type attribute would've
    // been placed immediately after the `struct' keyword, or,
    // alternatively, after the closing brace.

    CROSS_REFERENCE_TEST(TestParser::case0703) ;
    CROSS_REFERENCE_TEST(TestParser::case0704) ;

    parse("struct x __attribute__ ( ( vector_size ( 16 ) ) ) y ;") ;

}

void TestParser::case0703()
{
    // The attribute belongs to the type.

    CROSS_REFERENCE_TEST(TestParser::case0702) ;

    parse("struct __attribute__ ( ( aligned ) ) x { } ;") ;
}

void TestParser::case0704()
{
    // The attribute belongs to the type.

    CROSS_REFERENCE_TEST(TestParser::case0702) ;

    parse("struct x { } __attribute__ ( ( aligned ) ) ;") ;
}

void TestParser::case0705()
{
    // The attribute belongs to the type.

    CROSS_REFERENCE_TEST(TestParser::case0702) ;

    parse("struct x { short y [ 3 ] ; } __attribute__ ( ( aligned ( 8 ) ) ) ;") ;
}

void TestParser::case0706()
{
    parse("struct x { int y [ 2 ] __attribute__ ( ( aligned ( 8 ) ) ) ; } ;") ;
}

void TestParser::case0707()
{
    parse("struct x { short y [ 3 ] ; } __attribute__ ( ( aligned (8) ) ) z ;") ;
}

void TestParser::case0708()
{
    parse("__attribute__ ( ( availability ( macosx , introduced = 10.12 ) ) ) void x ( ) ;") ;
}

void TestParser::case0709()
{
    parse("__attribute__ ( ( availability ( macos , introduced = 10.4 , deprecated = 10.6 , obsoleted = 10.7) ) ) void x ( ) ;") ;
}

void TestParser::case0710()
{
    parse("__attribute__ ( ( availability ( ios , introduced = 10 , message =\"whatever\" ) ) ) void x ( ) ;") ;
}

void TestParser::case0711()
{
    parse("void x ( void ) __attribute__ ( ( availability ( macos , introduced = 10.4) ));") ;
}

void TestParser::case0712()
{
    parse("void x ( void ) __attribute__ ( ( availability ( macos , introduced = 10.4 , deprecated = 10.6 , obsoleted=10.7) ));") ;
}

void TestParser::case0713()
{
    parse("void ( __attribute__ ( ( noreturn ) ) * * * * x ) ( void ) ;") ;
}

void TestParser::case0714()
{
    parse("char * __attribute__ ( ( aligned ( 8 ) ) ) * x ;") ;
}

void TestParser::case0715()
{
    parse("__attribute__ ( ( format ( printf , 111 , 222 ) ) ) x ( const char * , ... ) ;") ;
}

void TestParser::case0716()
{
    parse(R"(
          void x ( void ),
          __attribute__ ( ( format ( printf , 111, 222) ) ) y ( const char * , ... ),
          z ( void ) ;
          )") ;
}

void TestParser::case0717()
{
    parse(R"(
          __attribute__ ( (noreturn) ) void x ( void ),
          __attribute__ ( ( format ( printf , 111, 222) ) ) y ( const char * , ... ),
          z ( void ) ;
          )") ;
}

void TestParser::case0718()
{
    parse("int x __attribute__ ( ( aligned (16) ) ) = 0 ;") ;
}

void TestParser::case0719()
{
    parse("x y __attribute__ ( ( aligned (16) ) ) = 0 ;") ;
}

void TestParser::case0720()
{
    parse("short x [3] __attribute__ ( ( aligned ( __BIGGEST_ALIGNMENT__ ) ));") ;
}

void TestParser::case0721()
{
    parse("x y [3] __attribute__ ( ( aligned ( __BIGGEST_ALIGNMENT__ ) ));") ;
}

void TestParser::case0722()
{
    parse("enum __attribute__ ( ( aligned ) ) x { y } ;") ;
}

void TestParser::case0723()
{
    parse("enum x { y } __attribute__ ( ( aligned ) );") ;
}

void TestParser::case0724()
{
    parse("enum x { y __attribute__( ( deprecated ) ) } ;") ;
}

void TestParser::case0725()
{
    parse("enum x { y __attribute__( ( deprecated ) ) = 1} ;") ;
}

void TestParser::case0726()
{
    parse("enum x { y __attribute__( ( deprecated ) ), z } ;") ;
}

void TestParser::case0727()
{
    parse("enum x { y __attribute__( ( deprecated ) ) = 1, z } ;") ;
}

void TestParser::case0728()
{
    parse("struct x { int y : 1 __attribute__ ( ( ) ) ; } ;");
}

void TestParser::case0729()
{
    parse("struct x { int __attribute__(()) y : 2 ; } ;");
}

void TestParser::case0730()
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

void TestParser::case0731()
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

void TestParser::case0732()
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

void TestParser::case0733()
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

void TestParser::case0734()
{
    parse("struct x { } __attribute__ ( ( aligned ) ) y ;");
}

void TestParser::case0735()
{

}

void TestParser::case0736()
{

}

void TestParser::case0737()
{

}

void TestParser::case0738()
{

}

void TestParser::case0739()
{

}

void TestParser::case0740()
{

}

void TestParser::case0741()
{

}

void TestParser::case0742()
{

}

void TestParser::case0743()
{

}

void TestParser::case0744()
{

}

void TestParser::case0745()
{

}

void TestParser::case0746()
{

}

void TestParser::case0747()
{

}

void TestParser::case0748()
{

}

void TestParser::case0749()
{

}

void TestParser::case0750()
{
}

void TestParser::case0751()
{

}

void TestParser::case0752()
{

}

void TestParser::case0753()
{

}

void TestParser::case0754()
{

}

void TestParser::case0755()
{

}

void TestParser::case0756()
{

}

void TestParser::case0757()
{

}

void TestParser::case0758()
{

}

void TestParser::case0759()
{

}

void TestParser::case0760()
{

}

void TestParser::case0761()
{

}

void TestParser::case0762()
{

}

void TestParser::case0763()
{

}

void TestParser::case0764()
{

}

void TestParser::case0765()
{

}

void TestParser::case0766()
{

}

void TestParser::case0767()
{

}

void TestParser::case0768()
{

}

void TestParser::case0769()
{

}

void TestParser::case0770()
{

}

void TestParser::case0771()
{

}

void TestParser::case0772()
{

}

void TestParser::case0773()
{

}

void TestParser::case0774()
{

}

void TestParser::case0775()
{

}

void TestParser::case0776()
{

}

void TestParser::case0777()
{

}

void TestParser::case0778()
{

}

void TestParser::case0779()
{

}

void TestParser::case0780()
{
    parse(R"(
          asm("nop \n");
          )");
}

void TestParser::case0781()
{
    parse("int x asm ( \"y\") = 1 ;");
}

void TestParser::case0782()
{
    parse("int x asm ( \"y\") __attribute__ (()) = 1 ;");
}

void TestParser::case0783()
{
    parse("int x __attribute__ (()) asm", 2);
}

void TestParser::case0784()
{
    parse("int __attribute__ (()) x asm ( \"y\") = 1 ;");
}

void TestParser::case0785()
{
    parse("int x ( int y ) asm ( \"z\" );");
}

void TestParser::case0786()
{

}

void TestParser::case0787()
{

}

void TestParser::case0788()
{

}

void TestParser::case0789()
{

}

void TestParser::case0790()
{

}

void TestParser::case0791()
{

}

void TestParser::case0792()
{

}

void TestParser::case0793()
{

}

void TestParser::case0794()
{

}

void TestParser::case0795()
{

}

void TestParser::case0796()
{

}

void TestParser::case0797()
{

}

void TestParser::case0798()
{

}

void TestParser::case0799()
{

}

void TestParser::case0800()
{
    parse("x y = { z } ;") ;
}

void TestParser::case0801()
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

void TestParser::case0802()
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

void TestParser::case0803()
{
    parse("struct x y = { 1, 2 } ;") ;
}

void TestParser::case0804()
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

void TestParser::case0805()
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

void TestParser::case0806()
{
     parse("int x[] = { 1, 2 } ;") ;
}

void TestParser::case0807()
{
    parse("int x[2] = { 1, 2 } ;") ;
}

void TestParser::case0808()
{
    parse("int x[][2] = { { 1, 2 } } ;") ;
}

void TestParser::case0809()
{
    parse("int x[][2] = { { 1, 2 }, { 3, 4 } } ;") ;
}

void TestParser::case0810()
{
    parse("x y[][2] = { { 1, 2 }, { 3, 4 } } ;") ;
}

void TestParser::case0811()
{
    parse("x y = { { 1, 2 }, { 3, 4 } } ;") ;
}

void TestParser::case0812()
{
    parse("x y = { { 1, 2, 3 }, { 4 }, { 6, 7, 8} } ;") ;
}

void TestParser::case0813()
{
    parse("x y = { 1, 2 }, y = { 3, 4, 5 } ;") ;
}

void TestParser::case0814()
{
    parse("int x[] = { 1, 2 }, y[] = { 3, 4, 5 } ;") ;
}

void TestParser::case0815()
{
    parse("x y[] = { 1, 2 }, z[] = { 3, 4, 5 } ;") ;
}

void TestParser::case0816()
{
    parse("x y = { 1, , 2 } ;",
          Expectation().addDiagnostic(Expectation::ErrorOrWarn::Error,
                                      Parser::DiagnosticsReporter::ID_of_ExpectedFIRSTofExpression));
}

void TestParser::case0817()
{
    // Trailing comman is OK here.

    parse("x y = { 1, 2, } ;") ;
}

void TestParser::case0818()
{
    parse("struct x y = { } ;",
          Expectation().addDiagnostic(Expectation::ErrorOrWarn::Error,
                                      Parser::DiagnosticsReporter::ID_of_ExpectedBraceEnclosedInitializerList));
}

void TestParser::case0819()
{

}

void TestParser::case0820()
{

}

void TestParser::case0821()
{

}

void TestParser::case0822()
{

}

void TestParser::case0823()
{

}

void TestParser::case0824()
{

}

void TestParser::case0825()
{

}

void TestParser::case0826()
{

}

void TestParser::case0827()
{

}

void TestParser::case0828()
{

}

void TestParser::case0829()
{

}

void TestParser::case0830()
{

}

void TestParser::case0831()
{

}

void TestParser::case0832()
{

}

void TestParser::case0833()
{

}

void TestParser::case0834()
{

}

void TestParser::case0835()
{

}

void TestParser::case0836()
{

}

void TestParser::case0837()
{

}

void TestParser::case0838()
{

}

void TestParser::case0839()
{

}

void TestParser::case0840()
{

}

void TestParser::case0841()
{

}

void TestParser::case0842()
{

}

void TestParser::case0843()
{

}

void TestParser::case0844()
{

}

void TestParser::case0845()
{

}

void TestParser::case0846()
{

}

void TestParser::case0847()
{

}

void TestParser::case0848()
{

}

void TestParser::case0849()
{

}

void TestParser::case0850()
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

void TestParser::case0851()
{
    parse("struct x y = { . z = 1, . w = 2 } ;") ;
}

void TestParser::case0852()
{
    parse("struct x y = { . = 1 } ;",
          Expectation().addDiagnostic(Expectation::ErrorOrWarn::Error,
                                      Parser::DiagnosticsReporter::ID_of_ExpectedFieldDesignator));
}

void TestParser::case0853()
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

void TestParser::case0854()
{
    parse("int x [ 9 ] = { [ 1 ] = 1 , [ 2 ] = 2 } ;");
}

void TestParser::case0855()
{
    parse("x y [ 9 ] = { [ 1 ] = 1 , 2 , [ 3 ] = 3 } ;");
}

void TestParser::case0856()
{
    parse("x y [ 9 ] = { [ 1 ] = 1 , 2 , [ 3 ] = 3 , z } ;");
}

void TestParser::case0857()
{
    parse("x y [ 9 ] = { [ 1 ] = 1 , 2 , [ 3 ] = 3 } ;");
}

void TestParser::case0858()
{
    parse("x y [ ] = { [ 1 ] = z , } ;");
}

void TestParser::case0859()
{
    parse("x y [ ] = { [ 1 ] } ;",
          Expectation().addDiagnostic(Expectation::ErrorOrWarn::Warn,
                                      Parser::DiagnosticsReporter::ID_of_ExpectedFOLLOWofDesignatedInitializer)
                       .addDiagnostic(Expectation::ErrorOrWarn::Error,
                                      Parser::DiagnosticsReporter::ID_of_ExpectedFIRSTofExpression));
}

void TestParser::case0860()
{
    parse("x y [ ] = { [ 1 ] 2 } ;",
          Expectation().addDiagnostic(Expectation::ErrorOrWarn::Warn,
                                      Parser::DiagnosticsReporter::ID_of_ExpectedFOLLOWofDesignatedInitializer));
}

void TestParser::case0861()
{
    parse("x y [ ] = { [ 1 ] = } ;", 1);
}

void TestParser::case0862()
{
    parse("x y [ ] = { [ ] = 1 } ;", 1);
}

void TestParser::case0863()
{

}

void TestParser::case0864()
{

}

void TestParser::case0865()
{

}

void TestParser::case0866()
{

}

void TestParser::case0867()
{

}

void TestParser::case0868()
{

}

void TestParser::case0869()
{

}

void TestParser::case0870()
{

}

void TestParser::case0871()
{

}

void TestParser::case0872()
{

}

void TestParser::case0873()
{

}

void TestParser::case0874()
{

}

void TestParser::case0875()
{

}

void TestParser::case0876()
{

}

void TestParser::case0877()
{

}

void TestParser::case0878()
{

}

void TestParser::case0879()
{

}

void TestParser::case0880()
{

}

void TestParser::case0881()
{

}

void TestParser::case0882()
{

}

void TestParser::case0883()
{

}

void TestParser::case0884()
{

}

void TestParser::case0885()
{

}

void TestParser::case0886()
{

}

void TestParser::case0887()
{

}

void TestParser::case0888()
{

}

void TestParser::case0889()
{

}

void TestParser::case0890()
{

}

void TestParser::case0891()
{

}

void TestParser::case0892()
{

}

void TestParser::case0893()
{

}

void TestParser::case0894()
{

}

void TestParser::case0895()
{

}

void TestParser::case0896()
{

}

void TestParser::case0897()
{

}

void TestParser::case0898()
{

}

void TestParser::case0899()
{

}

void TestParser::case0900()
{
    parse("__typeof__ ( int ) x ;");
}

void TestParser::case0901()
{
    parse("__typeof__ ( int * ) x ;");
}

void TestParser::case0902()
{
    parse("__typeof__ ( int * ) x [ 1 ] ;");
}

void TestParser::case0903()
{
    parse("__typeof__ ( __typeof__ ( int * ) [ 1 ] ) x ;");
}

void TestParser::case0904()
{
    parse("__typeof__ ( x ) y ;",
          Expectation().replicateAmbiguity("__typeof__ ( x x ) y ;")
                       .AST( { TranslationUnit,
                               VariableAndOrFunctionDeclaration,
                               ExtGNU_Typeof,
                               AmbiguousTypeNameOrExpressionAsTypeReference,
                               IdentifierDeclarator } ));
}

void TestParser::case0905()
{
    parse("__typeof__ ( x * ) y ;");
}

void TestParser::case0906()
{
    parse("__typeof__ ( * x ) y ;");
}

void TestParser::case0907()
{
    parse("__typeof__ ( x [ 0 ] ) y ;");
}

void TestParser::case0908()
{
    parse("__typeof__ ( x [ 0 ] [ 1 ] ) y ;");
}

void TestParser::case0909()
{
    parse("__typeof__ ( x [ 0 ] [ 1 ] ( ) ) y ;");
}

void TestParser::case0910()
{
    parse("int x = ( __typeof__ ( int ) ) 1 ;");
}

void TestParser::case0911()
{
    parse("void x ( __typeof__ ( int ) y ) ;");
}

void TestParser::case0912()
{
    parse("void x ( ) { __typeof__ ( x * ) y ; }");
}

void TestParser::case0913()
{
    parse("void x ( ) { __typeof__ ( int ) x ; }");
}

void TestParser::case0914()
{
    parse("void x ( ) { sizeof ( __typeof__ ( int ) ) ; }");
}

void TestParser::case0915()
{

}

void TestParser::case0916()
{

}

void TestParser::case0917()
{

}

void TestParser::case0918()
{

}

void TestParser::case0919()
{

}

void TestParser::case0920()
{

}

void TestParser::case0921()
{

}

void TestParser::case0922()
{

}

void TestParser::case0923()
{

}

void TestParser::case0924()
{

}

void TestParser::case0925()
{

}

void TestParser::case0926()
{

}

void TestParser::case0927()
{

}

void TestParser::case0928()
{

}

void TestParser::case0929()
{

}

void TestParser::case0930()
{

}

void TestParser::case0931()
{

}

void TestParser::case0932()
{

}

void TestParser::case0933()
{

}

void TestParser::case0934()
{

}

void TestParser::case0935()
{

}

void TestParser::case0936()
{

}

void TestParser::case0937()
{

}

void TestParser::case0938()
{

}

void TestParser::case0939()
{

}

void TestParser::case0940()
{

}

void TestParser::case0941()
{

}

void TestParser::case0942()
{

}

void TestParser::case0943()
{

}

void TestParser::case0944()
{

}

void TestParser::case0945()
{

}

void TestParser::case0946()
{

}

void TestParser::case0947()
{

}

void TestParser::case0948()
{

}

void TestParser::case0949()
{

}

void TestParser::case0950()
{
    parse("_Alignas ( 16 ) int x ;");
}

void TestParser::case0951()
{
    parse("_Alignas ( double ) int x ;");
}

void TestParser::case0952()
{
    parse("_Alignas ( double * ) int y ;");
}

void TestParser::case0953()
{
    parse("_Alignas ( x ) int y ;",
          Expectation().replicateAmbiguity("_Alignas ( x x ) int y ;")
                       .AST( { TranslationUnit,
                               VariableAndOrFunctionDeclaration,
                               AlignmentSpecifier,
                               AmbiguousTypeNameOrExpressionAsTypeReference,
                               BuiltinTypeSpecifier,
                               IdentifierDeclarator } ));
}

void TestParser::case0954()
{
    parse("_Alignas ( x * ) int y ;");
}

void TestParser::case0955()
{
    parse("_Alignas ( x * ) y z ;");
}

void TestParser::case0956()
{
    parse("struct x { _Alignas ( 16 ) int y ; } ;");
}

void TestParser::case0957()
{

}

void TestParser::case0958()
{

}

void TestParser::case0959()
{

}

void TestParser::case0960()
{

}

void TestParser::case0961()
{

}

void TestParser::case0962()
{

}

void TestParser::case0963()
{

}

void TestParser::case0964()
{

}

void TestParser::case0965()
{

}

void TestParser::case0966()
{

}

void TestParser::case0967()
{

}

void TestParser::case0968()
{

}

void TestParser::case0969()
{

}

void TestParser::case0970()
{

}

void TestParser::case0971()
{

}

void TestParser::case0972()
{

}

void TestParser::case0973()
{

}

void TestParser::case0974()
{

}

void TestParser::case0975()
{

}

void TestParser::case0976()
{

}

void TestParser::case0977()
{

}

void TestParser::case0978()
{

}

void TestParser::case0979()
{

}

void TestParser::case0980()
{

}

void TestParser::case0981()
{

}

void TestParser::case0982()
{

}

void TestParser::case0983()
{

}

void TestParser::case0984()
{

}

void TestParser::case0985()
{

}

void TestParser::case0986()
{

}

void TestParser::case0987()
{

}

void TestParser::case0988()
{

}

void TestParser::case0989()
{

}

void TestParser::case0990()
{

}

void TestParser::case0991()
{

}

void TestParser::case0992()
{

}

void TestParser::case0993()
{

}

void TestParser::case0994()
{

}

void TestParser::case0995()
{

}

void TestParser::case0996()
{

}

void TestParser::case0997()
{

}

void TestParser::case0998()
{

}

void TestParser::case0999()
{

}
