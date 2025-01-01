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

#include "BinderTester.h"
#include "ParserTester.h"

#include "binder/Binder.h"
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

void BinderTester::case4000()
{
    bind("signed char x ;",
         Expectation()
             .declaration(Decl()
                .Object("x", ObjectDeclarationKind::Variable)
                .ty_.Basic(BasicTypeKind::Char_S)));
}

void BinderTester::case4001()
{
    bind("long int x ;",
         Expectation()
            .declaration(Decl().Object("x", ObjectDeclarationKind::Variable)
                    .ty_.Basic(BasicTypeKind::Long_S)));
}

void BinderTester::case4002()
{
    bind("int long x ;",
         Expectation()
            .declaration(Decl().Object("x", ObjectDeclarationKind::Variable)
                    .ty_.Basic(BasicTypeKind::Long_S)));
}

void BinderTester::case4003()
{
    bind("signed x ;",
         Expectation()
            .declaration(Decl().Object("x", ObjectDeclarationKind::Variable)
                    .ty_.Basic(BasicTypeKind::Int_S)));
}

void BinderTester::case4004()
{
    bind("signed int x ;",
         Expectation()
            .declaration(Decl().Object("x", ObjectDeclarationKind::Variable)
                    .ty_.Basic(BasicTypeKind::Int_S)));
}

void BinderTester::case4005()
{
    bind("long long x ;",
         Expectation()
             .declaration(Decl().Object("x", ObjectDeclarationKind::Variable)
                              .ty_.Basic(BasicTypeKind::LongLong_S)));
}

void BinderTester::case4006()
{
    bind("long long signed x ;",
         Expectation()
             .declaration(Decl().Object("x", ObjectDeclarationKind::Variable)
                              .ty_.Basic(BasicTypeKind::LongLong_S)));
}

void BinderTester::case4007()
{
    bind("long signed long x ;",
         Expectation()
             .declaration(Decl().Object("x", ObjectDeclarationKind::Variable)
                              .ty_.Basic(BasicTypeKind::LongLong_S)));
}

void BinderTester::case4008()
{
    bind("signed long long x ;",
         Expectation()
             .declaration(Decl().Object("x", ObjectDeclarationKind::Variable)
                              .ty_.Basic(BasicTypeKind::LongLong_S)));
}

void BinderTester::case4009()
{
    bind("long long unsigned x ;",
         Expectation()
             .declaration(Decl().Object("x", ObjectDeclarationKind::Variable)
                              .ty_.Basic(BasicTypeKind::LongLong_U)));
}

void BinderTester::case4010()
{
    bind("long unsigned long x ;",
         Expectation()
             .declaration(Decl().Object("x", ObjectDeclarationKind::Variable)
                              .ty_.Basic(BasicTypeKind::LongLong_U)));
}

void BinderTester::case4011()
{
    bind("unsigned long long x ;",
         Expectation()
             .declaration(Decl().Object("x", ObjectDeclarationKind::Variable)
                              .ty_.Basic(BasicTypeKind::LongLong_U)));
}

void BinderTester::case4012()
{
    bind("long double x ;",
         Expectation()
             .declaration(Decl().Object("x", ObjectDeclarationKind::Variable)
                              .ty_.Basic(BasicTypeKind::LongDouble)));
}

void BinderTester::case4013()
{
    bind("double long x ;",
         Expectation()
             .declaration(Decl().Object("x", ObjectDeclarationKind::Variable)
                              .ty_.Basic(BasicTypeKind::LongDouble)));
}

void BinderTester::case4014()
{
    bind("signed double x ;",
         Expectation()
         .diagnostic(
             Expectation::ErrorOrWarn::Error,
             Binder::DiagnosticsReporter::ID_InvalidType)
         .declaration(Decl()
                      .Object("x", ObjectDeclarationKind::Variable)
                      .ty_.Basic(BasicTypeKind::Int_S)));
}

void BinderTester::case4015()
{
    bind("double signed x ;",
         Expectation()
         .diagnostic(
             Expectation::ErrorOrWarn::Error,
             Binder::DiagnosticsReporter::ID_InvalidType)
         .declaration(Decl()
                      .Object("x", ObjectDeclarationKind::Variable)
                      .ty_.Basic(BasicTypeKind::Double)));
}

void BinderTester::case4016()
{
    bind("long double char x ;",
         Expectation()
         .diagnostic(
             Expectation::ErrorOrWarn::Error,
             Binder::DiagnosticsReporter::ID_InvalidType)
         .declaration(Decl()
                      .Object("x", ObjectDeclarationKind::Variable)
                      .ty_.Basic(BasicTypeKind::LongDouble)));
}

void BinderTester::case4017()
{
    bind("long double signed x ;",
         Expectation()
         .diagnostic(
             Expectation::ErrorOrWarn::Error,
             Binder::DiagnosticsReporter::ID_InvalidType)
         .declaration(Decl()
                      .Object("x", ObjectDeclarationKind::Variable)
                      .ty_.Basic(BasicTypeKind::LongDouble)));
}

void BinderTester::case4018()
{
    bind("signed long double x ;",
         Expectation()
         .diagnostic(
             Expectation::ErrorOrWarn::Error,
             Binder::DiagnosticsReporter::ID_InvalidType)
         .declaration(Decl()
                      .Object("x", ObjectDeclarationKind::Variable)
                      .ty_.Basic(BasicTypeKind::Long_S)));
}

void BinderTester::case4019()
{
    bind("unsigned double x ;",
         Expectation()
         .diagnostic(
             Expectation::ErrorOrWarn::Error,
             Binder::DiagnosticsReporter::ID_InvalidType)
         .declaration(Decl()
                      .Object("x", ObjectDeclarationKind::Variable)
                      .ty_.Basic(BasicTypeKind::Int_U)));
}

void BinderTester::case4020()
{
    bind("double unsigned x ;",
         Expectation()
         .diagnostic(
             Expectation::ErrorOrWarn::Error,
             Binder::DiagnosticsReporter::ID_InvalidType)
         .declaration(Decl()
                      .Object("x", ObjectDeclarationKind::Variable)
                      .ty_.Basic(BasicTypeKind::Double)));
}

void BinderTester::case4021()
{
    bind("double float x ;",
         Expectation()
         .diagnostic(
             Expectation::ErrorOrWarn::Error,
             Binder::DiagnosticsReporter::ID_InvalidType)
         .declaration(Decl()
                      .Object("x", ObjectDeclarationKind::Variable)
                      .ty_.Basic(BasicTypeKind::Double)));
}

void BinderTester::case4022()
{
    bind("float double x ;",
         Expectation()
         .diagnostic(
             Expectation::ErrorOrWarn::Error,
             Binder::DiagnosticsReporter::ID_InvalidType)
         .declaration(Decl()
                      .Object("x", ObjectDeclarationKind::Variable)
                      .ty_.Basic(BasicTypeKind::Float)));
}

void BinderTester::case4023()
{
    bind("float unsigned x ;",
         Expectation()
         .diagnostic(
             Expectation::ErrorOrWarn::Error,
             Binder::DiagnosticsReporter::ID_InvalidType)
         .declaration(Decl()
                      .Object("x", ObjectDeclarationKind::Variable)
                      .ty_.Basic(BasicTypeKind::Float)));
}

void BinderTester::case4024()
{
    bind("float signed x ;",
         Expectation()
         .diagnostic(
             Expectation::ErrorOrWarn::Error,
             Binder::DiagnosticsReporter::ID_InvalidType)
         .declaration(Decl()
                      .Object("x", ObjectDeclarationKind::Variable)
                      .ty_.Basic(BasicTypeKind::Float)));
}

void BinderTester::case4025()
{
    bind("unsigned float x ;",
         Expectation().diagnostic(
             Expectation::ErrorOrWarn::Error,
             Binder::DiagnosticsReporter::ID_InvalidType));
}

void BinderTester::case4026()
{
    bind("signed float x ;",
         Expectation().diagnostic(
             Expectation::ErrorOrWarn::Error,
             Binder::DiagnosticsReporter::ID_InvalidType));
}

void BinderTester::case4027()
{
    bind("char short x ;",
         Expectation()
         .diagnostic(
             Expectation::ErrorOrWarn::Error,
             Binder::DiagnosticsReporter::ID_InvalidType)
         .declaration(Decl()
                      .Object("x", ObjectDeclarationKind::Variable)
                      .ty_.Basic(BasicTypeKind::Char)));
}

void BinderTester::case4028()
{
    bind("char long x ;",
         Expectation()
         .diagnostic(
             Expectation::ErrorOrWarn::Error,
             Binder::DiagnosticsReporter::ID_InvalidType)
         .declaration(Decl()
                      .Object("x", ObjectDeclarationKind::Variable)
                      .ty_.Basic(BasicTypeKind::Char)));
}

void BinderTester::case4029()
{
    bind("char double x ;",
         Expectation().diagnostic(
             Expectation::ErrorOrWarn::Error,
             Binder::DiagnosticsReporter::ID_InvalidType));
}

void BinderTester::case4030()
{
    bind("char float x ;",
         Expectation().diagnostic(
             Expectation::ErrorOrWarn::Error,
             Binder::DiagnosticsReporter::ID_InvalidType));
}

void BinderTester::case4031()
{
    bind("int signed x ;",
         Expectation()
             .declaration(Decl()
                .Object("x", ObjectDeclarationKind::Variable)
                .ty_.Basic(BasicTypeKind::Int_S)));
}

void BinderTester::case4032()
{
    bind("signed int x ;",
         Expectation()
             .declaration(Decl()
                .Object("x", ObjectDeclarationKind::Variable)
                .ty_.Basic(BasicTypeKind::Int_S)));
}

void BinderTester::case4033()
{
    bind("signed int double x ;",
         Expectation().diagnostic(
             Expectation::ErrorOrWarn::Error,
             Binder::DiagnosticsReporter::ID_InvalidType));
}

void BinderTester::case4034()
{
    bind("signed int int x ;",
         Expectation().diagnostic(
             Expectation::ErrorOrWarn::Error,
             Binder::DiagnosticsReporter::ID_InvalidType));
}

void BinderTester::case4035()
{
    bind("int float x ;",
         Expectation().diagnostic(
             Expectation::ErrorOrWarn::Error,
             Binder::DiagnosticsReporter::ID_InvalidType));
}

void BinderTester::case4036()
{
    bind("char signed x ;",
         Expectation()
             .declaration(Decl()
                .Object("x", ObjectDeclarationKind::Variable)
                .ty_.Basic(BasicTypeKind::Char_S)));
}

void BinderTester::case4037()
{
    bind("unsigned char x ;",
         Expectation()
             .declaration(Decl()
                .Object("x", ObjectDeclarationKind::Variable)
                .ty_.Basic(BasicTypeKind::Char_U)));
}

void BinderTester::case4038()
{
    bind("char unsigned x ;",
         Expectation()
             .declaration(Decl()
                .Object("x", ObjectDeclarationKind::Variable)
                .ty_.Basic(BasicTypeKind::Char_U)));
}

void BinderTester::case4039()
{
    bind("unsigned long int x ;",
         Expectation()
             .declaration(Decl()
                .Object("x", ObjectDeclarationKind::Variable)
                .ty_.Basic(BasicTypeKind::Long_U)));
}

void BinderTester::case4040()
{
    bind("long int unsigned x ;",
         Expectation()
             .declaration(Decl()
                .Object("x", ObjectDeclarationKind::Variable)
                .ty_.Basic(BasicTypeKind::Long_U)));
}

void BinderTester::case4041()
{
    bind("int int x ;",
         Expectation().diagnostic(
             Expectation::ErrorOrWarn::Error,
             Binder::DiagnosticsReporter::ID_InvalidType));
}

void BinderTester::case4042()
{
    bind("_Complex long double double x ;",
         Expectation().diagnostic(
             Expectation::ErrorOrWarn::Error,
             Binder::DiagnosticsReporter::ID_InvalidType));
}

void BinderTester::case4043()
{
    bind("long double double x ;",
         Expectation().diagnostic(
             Expectation::ErrorOrWarn::Error,
             Binder::DiagnosticsReporter::ID_InvalidType));
}

void BinderTester::case4044()
{
    bind("double long double x ;",
         Expectation().diagnostic(
             Expectation::ErrorOrWarn::Error,
             Binder::DiagnosticsReporter::ID_InvalidType));

}

void BinderTester::case4045()
{
    bind("long int double x ;",
         Expectation().diagnostic(
             Expectation::ErrorOrWarn::Error,
             Binder::DiagnosticsReporter::ID_InvalidType));
}

void BinderTester::case4046()
{
    bind("double long int x ;",
         Expectation().diagnostic(
             Expectation::ErrorOrWarn::Error,
             Binder::DiagnosticsReporter::ID_InvalidType));
}

void BinderTester::case4047()
{
    bind("long double _Complex x ;",
         Expectation()
             .declaration(Decl().Object("x", ObjectDeclarationKind::Variable)
                              .ty_.Basic(BasicTypeKind::LongDoubleComplex)));
}

void BinderTester::case4048()
{
    bind("double _Complex x ;",
         Expectation()
             .declaration(Decl().Object("x", ObjectDeclarationKind::Variable)
                              .ty_.Basic(BasicTypeKind::DoubleComplex)));
}

void BinderTester::case4049()
{
    bind("float _Complex x ;",
         Expectation()
             .declaration(Decl().Object("x", ObjectDeclarationKind::Variable)
                              .ty_.Basic(BasicTypeKind::FloatComplex)));
}

void BinderTester::case4050()
{
    bind("struct x int y ;",
         Expectation()
         .diagnostic(
             Expectation::ErrorOrWarn::Error,
             Binder::DiagnosticsReporter::ID_InvalidType));
}

void BinderTester::case4051()
{
    bind("char int x ;",
         Expectation()
         .diagnostic(
             Expectation::ErrorOrWarn::Error,
             Binder::DiagnosticsReporter::ID_InvalidType)
         .declaration(Decl()
                      .Object("x", ObjectDeclarationKind::Variable)
                      .ty_.Basic(BasicTypeKind::Char)));
}

void BinderTester::case4052()
{
    bind("long long long x ;",
         Expectation()
         .diagnostic(
             Expectation::ErrorOrWarn::Error,
             Binder::DiagnosticsReporter::ID_InvalidType)
         .declaration(Decl()
                      .Object("x", ObjectDeclarationKind::Variable)
                      .ty_.Basic(BasicTypeKind::LongLong_S)));
}

void BinderTester::case4053()
{
    bind("signed long long long x ;",
         Expectation()
         .diagnostic(
             Expectation::ErrorOrWarn::Error,
             Binder::DiagnosticsReporter::ID_InvalidType)
         .declaration(Decl()
                      .Object("x", ObjectDeclarationKind::Variable)
                      .ty_.Basic(BasicTypeKind::LongLong_S)));
}

void BinderTester::case4054()
{
    bind("unsigned long long long x ;",
         Expectation()
         .diagnostic(
             Expectation::ErrorOrWarn::Error,
             Binder::DiagnosticsReporter::ID_InvalidType)
         .declaration(Decl()
                      .Object("x", ObjectDeclarationKind::Variable)
                      .ty_.Basic(BasicTypeKind::LongLong_U)));
}

void BinderTester::case4055()
{
    bind("long long signed long x ;",
         Expectation()
         .diagnostic(
             Expectation::ErrorOrWarn::Error,
             Binder::DiagnosticsReporter::ID_InvalidType)
         .declaration(Decl()
                      .Object("x", ObjectDeclarationKind::Variable)
                      .ty_.Basic(BasicTypeKind::LongLong_S)));
}

void BinderTester::case4056()
{
    bind("long long unsigned long x ;",
         Expectation()
         .diagnostic(
             Expectation::ErrorOrWarn::Error,
             Binder::DiagnosticsReporter::ID_InvalidType)
         .declaration(Decl()
                      .Object("x", ObjectDeclarationKind::Variable)
                      .ty_.Basic(BasicTypeKind::LongLong_U)));
}

void BinderTester::case4057(){}
void BinderTester::case4058(){}
void BinderTester::case4059(){}
void BinderTester::case4060(){}
void BinderTester::case4061(){}
void BinderTester::case4062(){}
void BinderTester::case4063(){}
void BinderTester::case4064(){}
void BinderTester::case4065(){}
void BinderTester::case4066(){}
void BinderTester::case4067(){}
void BinderTester::case4068(){}
void BinderTester::case4069(){}
void BinderTester::case4070(){}
void BinderTester::case4071(){}
void BinderTester::case4072(){}
void BinderTester::case4073(){}
void BinderTester::case4074(){}
void BinderTester::case4075(){}
void BinderTester::case4076(){}
void BinderTester::case4077(){}
void BinderTester::case4078(){}
void BinderTester::case4079(){}
void BinderTester::case4080(){}
void BinderTester::case4081(){}
void BinderTester::case4082(){}
void BinderTester::case4083(){}
void BinderTester::case4084(){}
void BinderTester::case4085(){}
void BinderTester::case4086(){}
void BinderTester::case4087(){}
void BinderTester::case4088(){}
void BinderTester::case4089(){}
void BinderTester::case4090(){}
void BinderTester::case4091(){}
void BinderTester::case4092(){}
void BinderTester::case4093(){}
void BinderTester::case4094(){}
void BinderTester::case4095(){}
void BinderTester::case4096(){}
void BinderTester::case4097(){}
void BinderTester::case4098(){}
void BinderTester::case4099(){}
