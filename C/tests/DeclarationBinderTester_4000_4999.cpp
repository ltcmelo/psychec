// Copyright (c) 2024 Leandro T. C. Melo <ltcmelo@gmail.com>
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

#include "DeclarationBinderTester.h"
#include "ParserTester.h"

#include "sema/DeclarationBinder.h"
#include "parser/Unparser.h"
#include "symbols/Symbol.h"
#include "symbols/Symbol_ALL.h"
#include "syntax/Lexeme_ALL.h"
#include "syntax/SyntaxNodes.h"

#include <algorithm>
#include <cstring>
#include <iostream>
#include <string>
#include <sstream>

using namespace psy;
using namespace C;

void DeclarationBinderTester::case4000()
{
    bind("signed char x ;",
         Expectation()
             .declaration(Decl()
                .Object("x", ObjectDeclarationCategory::Variable)
                .ty_.Basic(BasicTypeKind::Char_S)));
}

void DeclarationBinderTester::case4001()
{
    bind("long int x ;",
         Expectation()
            .declaration(Decl().Object("x", ObjectDeclarationCategory::Variable)
                    .ty_.Basic(BasicTypeKind::Long_S)));
}

void DeclarationBinderTester::case4002()
{
    bind("int long x ;",
         Expectation()
            .declaration(Decl().Object("x", ObjectDeclarationCategory::Variable)
                    .ty_.Basic(BasicTypeKind::Long_S)));
}

void DeclarationBinderTester::case4003()
{
    bind("signed x ;",
         Expectation()
            .declaration(Decl().Object("x", ObjectDeclarationCategory::Variable)
                    .ty_.Basic(BasicTypeKind::Int_S)));
}

void DeclarationBinderTester::case4004()
{
    bind("signed int x ;",
         Expectation()
            .declaration(Decl().Object("x", ObjectDeclarationCategory::Variable)
                    .ty_.Basic(BasicTypeKind::Int_S)));
}

void DeclarationBinderTester::case4005()
{
    bind("long long x ;",
         Expectation()
             .declaration(Decl().Object("x", ObjectDeclarationCategory::Variable)
                              .ty_.Basic(BasicTypeKind::LongLong_S)));
}

void DeclarationBinderTester::case4006()
{
    bind("long long signed x ;",
         Expectation()
             .declaration(Decl().Object("x", ObjectDeclarationCategory::Variable)
                              .ty_.Basic(BasicTypeKind::LongLong_S)));
}

void DeclarationBinderTester::case4007()
{
    bind("long signed long x ;",
         Expectation()
             .declaration(Decl().Object("x", ObjectDeclarationCategory::Variable)
                              .ty_.Basic(BasicTypeKind::LongLong_S)));
}

void DeclarationBinderTester::case4008()
{
    bind("signed long long x ;",
         Expectation()
             .declaration(Decl().Object("x", ObjectDeclarationCategory::Variable)
                              .ty_.Basic(BasicTypeKind::LongLong_S)));
}

void DeclarationBinderTester::case4009()
{
    bind("long long unsigned x ;",
         Expectation()
             .declaration(Decl().Object("x", ObjectDeclarationCategory::Variable)
                              .ty_.Basic(BasicTypeKind::LongLong_U)));
}

void DeclarationBinderTester::case4010()
{
    bind("long unsigned long x ;",
         Expectation()
             .declaration(Decl().Object("x", ObjectDeclarationCategory::Variable)
                              .ty_.Basic(BasicTypeKind::LongLong_U)));
}

void DeclarationBinderTester::case4011()
{
    bind("unsigned long long x ;",
         Expectation()
             .declaration(Decl().Object("x", ObjectDeclarationCategory::Variable)
                              .ty_.Basic(BasicTypeKind::LongLong_U)));
}

void DeclarationBinderTester::case4012()
{
    bind("long double x ;",
         Expectation()
             .declaration(Decl().Object("x", ObjectDeclarationCategory::Variable)
                              .ty_.Basic(BasicTypeKind::LongDouble)));
}

void DeclarationBinderTester::case4013()
{
    bind("double long x ;",
         Expectation()
             .declaration(Decl().Object("x", ObjectDeclarationCategory::Variable)
                              .ty_.Basic(BasicTypeKind::LongDouble)));
}

void DeclarationBinderTester::case4014()
{
    bind("signed double x ;",
         Expectation()
         .diagnostic(
             Expectation::ErrorOrWarn::Error,
             DeclarationBinder::DiagnosticsReporter::ID_InvalidType)
         .declaration(Decl()
                      .Object("x", ObjectDeclarationCategory::Variable)
                      .ty_.Basic(BasicTypeKind::Int_S)));
}

void DeclarationBinderTester::case4015()
{
    bind("double signed x ;",
         Expectation()
         .diagnostic(
             Expectation::ErrorOrWarn::Error,
             DeclarationBinder::DiagnosticsReporter::ID_InvalidType)
         .declaration(Decl()
                      .Object("x", ObjectDeclarationCategory::Variable)
                      .ty_.Basic(BasicTypeKind::Double)));
}

void DeclarationBinderTester::case4016()
{
    bind("long double char x ;",
         Expectation()
         .diagnostic(
             Expectation::ErrorOrWarn::Error,
             DeclarationBinder::DiagnosticsReporter::ID_InvalidType)
         .declaration(Decl()
                      .Object("x", ObjectDeclarationCategory::Variable)
                      .ty_.Basic(BasicTypeKind::LongDouble)));
}

void DeclarationBinderTester::case4017()
{
    bind("long double signed x ;",
         Expectation()
         .diagnostic(
             Expectation::ErrorOrWarn::Error,
             DeclarationBinder::DiagnosticsReporter::ID_InvalidType)
         .declaration(Decl()
                      .Object("x", ObjectDeclarationCategory::Variable)
                      .ty_.Basic(BasicTypeKind::LongDouble)));
}

void DeclarationBinderTester::case4018()
{
    bind("signed long double x ;",
         Expectation()
         .diagnostic(
             Expectation::ErrorOrWarn::Error,
             DeclarationBinder::DiagnosticsReporter::ID_InvalidType)
         .declaration(Decl()
                      .Object("x", ObjectDeclarationCategory::Variable)
                      .ty_.Basic(BasicTypeKind::Long_S)));
}

void DeclarationBinderTester::case4019()
{
    bind("unsigned double x ;",
         Expectation()
         .diagnostic(
             Expectation::ErrorOrWarn::Error,
             DeclarationBinder::DiagnosticsReporter::ID_InvalidType)
         .declaration(Decl()
                      .Object("x", ObjectDeclarationCategory::Variable)
                      .ty_.Basic(BasicTypeKind::Int_U)));
}

void DeclarationBinderTester::case4020()
{
    bind("double unsigned x ;",
         Expectation()
         .diagnostic(
             Expectation::ErrorOrWarn::Error,
             DeclarationBinder::DiagnosticsReporter::ID_InvalidType)
         .declaration(Decl()
                      .Object("x", ObjectDeclarationCategory::Variable)
                      .ty_.Basic(BasicTypeKind::Double)));
}

void DeclarationBinderTester::case4021()
{
    bind("double float x ;",
         Expectation()
         .diagnostic(
             Expectation::ErrorOrWarn::Error,
             DeclarationBinder::DiagnosticsReporter::ID_InvalidType)
         .declaration(Decl()
                      .Object("x", ObjectDeclarationCategory::Variable)
                      .ty_.Basic(BasicTypeKind::Double)));
}

void DeclarationBinderTester::case4022()
{
    bind("float double x ;",
         Expectation()
         .diagnostic(
             Expectation::ErrorOrWarn::Error,
             DeclarationBinder::DiagnosticsReporter::ID_InvalidType)
         .declaration(Decl()
                      .Object("x", ObjectDeclarationCategory::Variable)
                      .ty_.Basic(BasicTypeKind::Float)));
}

void DeclarationBinderTester::case4023()
{
    bind("float unsigned x ;",
         Expectation()
         .diagnostic(
             Expectation::ErrorOrWarn::Error,
             DeclarationBinder::DiagnosticsReporter::ID_InvalidType)
         .declaration(Decl()
                      .Object("x", ObjectDeclarationCategory::Variable)
                      .ty_.Basic(BasicTypeKind::Float)));
}

void DeclarationBinderTester::case4024()
{
    bind("float signed x ;",
         Expectation()
         .diagnostic(
             Expectation::ErrorOrWarn::Error,
             DeclarationBinder::DiagnosticsReporter::ID_InvalidType)
         .declaration(Decl()
                      .Object("x", ObjectDeclarationCategory::Variable)
                      .ty_.Basic(BasicTypeKind::Float)));
}

void DeclarationBinderTester::case4025()
{
    bind("unsigned float x ;",
         Expectation().diagnostic(
             Expectation::ErrorOrWarn::Error,
             DeclarationBinder::DiagnosticsReporter::ID_InvalidType));
}

void DeclarationBinderTester::case4026()
{
    bind("signed float x ;",
         Expectation().diagnostic(
             Expectation::ErrorOrWarn::Error,
             DeclarationBinder::DiagnosticsReporter::ID_InvalidType));
}

void DeclarationBinderTester::case4027()
{
    bind("char short x ;",
         Expectation()
         .diagnostic(
             Expectation::ErrorOrWarn::Error,
             DeclarationBinder::DiagnosticsReporter::ID_InvalidType)
         .declaration(Decl()
                      .Object("x", ObjectDeclarationCategory::Variable)
                      .ty_.Basic(BasicTypeKind::Char)));
}

void DeclarationBinderTester::case4028()
{
    bind("char long x ;",
         Expectation()
         .diagnostic(
             Expectation::ErrorOrWarn::Error,
             DeclarationBinder::DiagnosticsReporter::ID_InvalidType)
         .declaration(Decl()
                      .Object("x", ObjectDeclarationCategory::Variable)
                      .ty_.Basic(BasicTypeKind::Char)));
}

void DeclarationBinderTester::case4029()
{
    bind("char double x ;",
         Expectation().diagnostic(
             Expectation::ErrorOrWarn::Error,
             DeclarationBinder::DiagnosticsReporter::ID_InvalidType));
}

void DeclarationBinderTester::case4030()
{
    bind("char float x ;",
         Expectation().diagnostic(
             Expectation::ErrorOrWarn::Error,
             DeclarationBinder::DiagnosticsReporter::ID_InvalidType));
}

void DeclarationBinderTester::case4031()
{
    bind("int signed x ;",
         Expectation()
             .declaration(Decl()
                .Object("x", ObjectDeclarationCategory::Variable)
                .ty_.Basic(BasicTypeKind::Int_S)));
}

void DeclarationBinderTester::case4032()
{
    bind("signed int x ;",
         Expectation()
             .declaration(Decl()
                .Object("x", ObjectDeclarationCategory::Variable)
                .ty_.Basic(BasicTypeKind::Int_S)));
}

void DeclarationBinderTester::case4033()
{
    bind("signed int double x ;",
         Expectation().diagnostic(
             Expectation::ErrorOrWarn::Error,
             DeclarationBinder::DiagnosticsReporter::ID_InvalidType));
}

void DeclarationBinderTester::case4034()
{
    bind("signed int int x ;",
         Expectation().diagnostic(
             Expectation::ErrorOrWarn::Error,
             DeclarationBinder::DiagnosticsReporter::ID_InvalidType));
}

void DeclarationBinderTester::case4035()
{
    bind("int float x ;",
         Expectation().diagnostic(
             Expectation::ErrorOrWarn::Error,
             DeclarationBinder::DiagnosticsReporter::ID_InvalidType));
}

void DeclarationBinderTester::case4036()
{
    bind("char signed x ;",
         Expectation()
             .declaration(Decl()
                .Object("x", ObjectDeclarationCategory::Variable)
                .ty_.Basic(BasicTypeKind::Char_S)));
}

void DeclarationBinderTester::case4037()
{
    bind("unsigned char x ;",
         Expectation()
             .declaration(Decl()
                .Object("x", ObjectDeclarationCategory::Variable)
                .ty_.Basic(BasicTypeKind::Char_U)));
}

void DeclarationBinderTester::case4038()
{
    bind("char unsigned x ;",
         Expectation()
             .declaration(Decl()
                .Object("x", ObjectDeclarationCategory::Variable)
                .ty_.Basic(BasicTypeKind::Char_U)));
}

void DeclarationBinderTester::case4039()
{
    bind("unsigned long int x ;",
         Expectation()
             .declaration(Decl()
                .Object("x", ObjectDeclarationCategory::Variable)
                .ty_.Basic(BasicTypeKind::Long_U)));
}

void DeclarationBinderTester::case4040()
{
    bind("long int unsigned x ;",
         Expectation()
             .declaration(Decl()
                .Object("x", ObjectDeclarationCategory::Variable)
                .ty_.Basic(BasicTypeKind::Long_U)));
}

void DeclarationBinderTester::case4041()
{
    bind("int int x ;",
         Expectation().diagnostic(
             Expectation::ErrorOrWarn::Error,
             DeclarationBinder::DiagnosticsReporter::ID_InvalidType));
}

void DeclarationBinderTester::case4042()
{
    bind("_Complex long double double x ;",
         Expectation().diagnostic(
             Expectation::ErrorOrWarn::Error,
             DeclarationBinder::DiagnosticsReporter::ID_InvalidType));
}

void DeclarationBinderTester::case4043()
{
    bind("long double double x ;",
         Expectation().diagnostic(
             Expectation::ErrorOrWarn::Error,
             DeclarationBinder::DiagnosticsReporter::ID_InvalidType));
}

void DeclarationBinderTester::case4044()
{
    bind("double long double x ;",
         Expectation().diagnostic(
             Expectation::ErrorOrWarn::Error,
             DeclarationBinder::DiagnosticsReporter::ID_InvalidType));

}

void DeclarationBinderTester::case4045()
{
    bind("long int double x ;",
         Expectation().diagnostic(
             Expectation::ErrorOrWarn::Error,
             DeclarationBinder::DiagnosticsReporter::ID_InvalidType));
}

void DeclarationBinderTester::case4046()
{
    bind("double long int x ;",
         Expectation().diagnostic(
             Expectation::ErrorOrWarn::Error,
             DeclarationBinder::DiagnosticsReporter::ID_InvalidType));
}

void DeclarationBinderTester::case4047()
{
    bind("long double _Complex x ;",
         Expectation()
             .declaration(Decl().Object("x", ObjectDeclarationCategory::Variable)
                              .ty_.Basic(BasicTypeKind::LongDoubleComplex)));
}

void DeclarationBinderTester::case4048()
{
    bind("double _Complex x ;",
         Expectation()
             .declaration(Decl().Object("x", ObjectDeclarationCategory::Variable)
                              .ty_.Basic(BasicTypeKind::DoubleComplex)));
}

void DeclarationBinderTester::case4049()
{
    bind("float _Complex x ;",
         Expectation()
             .declaration(Decl().Object("x", ObjectDeclarationCategory::Variable)
                              .ty_.Basic(BasicTypeKind::FloatComplex)));
}

void DeclarationBinderTester::case4050()
{
    bind("struct x int y ;",
         Expectation()
         .diagnostic(
             Expectation::ErrorOrWarn::Error,
             DeclarationBinder::DiagnosticsReporter::ID_InvalidType));
}

void DeclarationBinderTester::case4051()
{
    bind("char int x ;",
         Expectation()
         .diagnostic(
             Expectation::ErrorOrWarn::Error,
             DeclarationBinder::DiagnosticsReporter::ID_InvalidType)
         .declaration(Decl()
                      .Object("x", ObjectDeclarationCategory::Variable)
                      .ty_.Basic(BasicTypeKind::Char)));
}

void DeclarationBinderTester::case4052()
{
    bind("long long long x ;",
         Expectation()
         .diagnostic(
             Expectation::ErrorOrWarn::Error,
             DeclarationBinder::DiagnosticsReporter::ID_InvalidType)
         .declaration(Decl()
                      .Object("x", ObjectDeclarationCategory::Variable)
                      .ty_.Basic(BasicTypeKind::LongLong_S)));
}

void DeclarationBinderTester::case4053()
{
    bind("signed long long long x ;",
         Expectation()
         .diagnostic(
             Expectation::ErrorOrWarn::Error,
             DeclarationBinder::DiagnosticsReporter::ID_InvalidType)
         .declaration(Decl()
                      .Object("x", ObjectDeclarationCategory::Variable)
                      .ty_.Basic(BasicTypeKind::LongLong_S)));
}

void DeclarationBinderTester::case4054()
{
    bind("unsigned long long long x ;",
         Expectation()
         .diagnostic(
             Expectation::ErrorOrWarn::Error,
             DeclarationBinder::DiagnosticsReporter::ID_InvalidType)
         .declaration(Decl()
                      .Object("x", ObjectDeclarationCategory::Variable)
                      .ty_.Basic(BasicTypeKind::LongLong_U)));
}

void DeclarationBinderTester::case4055()
{
    bind("long long signed long x ;",
         Expectation()
         .diagnostic(
             Expectation::ErrorOrWarn::Error,
             DeclarationBinder::DiagnosticsReporter::ID_InvalidType)
         .declaration(Decl()
                      .Object("x", ObjectDeclarationCategory::Variable)
                      .ty_.Basic(BasicTypeKind::LongLong_S)));
}

void DeclarationBinderTester::case4056()
{
    bind("long long unsigned long x ;",
         Expectation()
         .diagnostic(
             Expectation::ErrorOrWarn::Error,
             DeclarationBinder::DiagnosticsReporter::ID_InvalidType)
         .declaration(Decl()
                      .Object("x", ObjectDeclarationCategory::Variable)
                      .ty_.Basic(BasicTypeKind::LongLong_U)));
}

void DeclarationBinderTester::case4057(){}
void DeclarationBinderTester::case4058(){}
void DeclarationBinderTester::case4059(){}
void DeclarationBinderTester::case4060(){}
void DeclarationBinderTester::case4061(){}
void DeclarationBinderTester::case4062(){}
void DeclarationBinderTester::case4063(){}
void DeclarationBinderTester::case4064(){}
void DeclarationBinderTester::case4065(){}
void DeclarationBinderTester::case4066(){}
void DeclarationBinderTester::case4067(){}
void DeclarationBinderTester::case4068(){}
void DeclarationBinderTester::case4069(){}
void DeclarationBinderTester::case4070(){}
void DeclarationBinderTester::case4071(){}
void DeclarationBinderTester::case4072(){}
void DeclarationBinderTester::case4073(){}
void DeclarationBinderTester::case4074(){}
void DeclarationBinderTester::case4075(){}
void DeclarationBinderTester::case4076(){}
void DeclarationBinderTester::case4077(){}
void DeclarationBinderTester::case4078(){}
void DeclarationBinderTester::case4079(){}
void DeclarationBinderTester::case4080(){}
void DeclarationBinderTester::case4081(){}
void DeclarationBinderTester::case4082(){}
void DeclarationBinderTester::case4083(){}
void DeclarationBinderTester::case4084(){}
void DeclarationBinderTester::case4085(){}
void DeclarationBinderTester::case4086(){}
void DeclarationBinderTester::case4087(){}
void DeclarationBinderTester::case4088(){}
void DeclarationBinderTester::case4089(){}
void DeclarationBinderTester::case4090(){}
void DeclarationBinderTester::case4091(){}
void DeclarationBinderTester::case4092(){}
void DeclarationBinderTester::case4093(){}
void DeclarationBinderTester::case4094(){}
void DeclarationBinderTester::case4095(){}
void DeclarationBinderTester::case4096(){}
void DeclarationBinderTester::case4097(){}
void DeclarationBinderTester::case4098(){}
void DeclarationBinderTester::case4099(){}
