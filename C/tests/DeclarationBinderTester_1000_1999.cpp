// Copyright (c) 2021/22 Leandro T. C. Melo <ltcmelo@gmail.com>
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

void DeclarationBinderTester::case1000()
{
    bind("double x ;",
         Expectation()
            .declaration(Decl().Object("x", ObjectDeclarationCategory::Variable)
                    .ty_.Basic(BasicTypeKind::Double)));
}

void DeclarationBinderTester::case1001()
{
    bind("int x ;",
         Expectation()
            .declaration(Decl().Object("x", ObjectDeclarationCategory::Variable)
                     .ty_.Basic(BasicTypeKind::Int_S)));
}

void DeclarationBinderTester::case1002()
{
    bind("int x ; int y ;",
         Expectation()
            .declaration(Decl().Object("x", ObjectDeclarationCategory::Variable)
                    .ty_.Basic(BasicTypeKind::Int_S))
            .declaration(Decl().Object("y", ObjectDeclarationCategory::Variable)
                    .ty_.Basic(BasicTypeKind::Int_S)));
}

void DeclarationBinderTester::case1003()
{
    bind("int x , y ;",
         Expectation()
             .declaration(Decl().Object("x", ObjectDeclarationCategory::Variable)
                     .ty_.Basic(BasicTypeKind::Int_S))
             .declaration(Decl().Object("y", ObjectDeclarationCategory::Variable)
                     .ty_.Basic(BasicTypeKind::Int_S)));
}

void DeclarationBinderTester::case1004()
{
    bind("x y ;",
         Expectation()
             .declaration(Decl().Object("y", ObjectDeclarationCategory::Variable)
                 .ty_.Typedef("x")));
}

void DeclarationBinderTester::case1005()
{
    bind("x y , z ;",
         Expectation()
             .declaration(Decl().Object("y", ObjectDeclarationCategory::Variable)
                      .ty_.Typedef("x"))
             .declaration(Decl().Object("z", ObjectDeclarationCategory::Variable)
                      .ty_.Typedef("x")));

}

void DeclarationBinderTester::case1006()
{
    bind("int x = 1 ;",
         Expectation()
            .declaration(Decl().Object("x", ObjectDeclarationCategory::Variable)
                     .ty_.Basic(BasicTypeKind::Int_S)));
}

void DeclarationBinderTester::case1007()
{
    bind("x y = 1 ;",
         Expectation()
             .declaration(Decl().Object("y", ObjectDeclarationCategory::Variable)
                      .ty_.Typedef("x")));
}

void DeclarationBinderTester::case1008()
{
    bind("long x ;",
         Expectation()
            .declaration(Decl().Object("x", ObjectDeclarationCategory::Variable)
                    .ty_.Basic(BasicTypeKind::Long_S)));
}

void DeclarationBinderTester::case1009()
{
    bind("struct x y ;",
         Expectation()
            .declaration(Decl()
                     .Object("y", ObjectDeclarationCategory::Variable)
                     .ty_.Tag("x", TagTypeKind::Struct)));
}

void DeclarationBinderTester::case1010()
{
    bind("union x y ;",
         Expectation()
            .declaration(Decl().Object("y", ObjectDeclarationCategory::Variable)
                     .ty_.Tag("x", TagTypeKind::Union)));
}

void DeclarationBinderTester::case1011()
{
    bind("enum x y ;",
         Expectation()
            .declaration(Decl().Object("y", ObjectDeclarationCategory::Variable)
                     .ty_.Tag("x", TagTypeKind::Enum)));
}

void DeclarationBinderTester::case1012()
{
    bind("struct x y , z ;",
         Expectation()
            .declaration(Decl().Object("y", ObjectDeclarationCategory::Variable)
                     .ty_.Tag("x", TagTypeKind::Struct))
            .declaration(Decl().Object("z", ObjectDeclarationCategory::Variable)
                     .ty_.Tag("x", TagTypeKind::Struct)));
}

void DeclarationBinderTester::case1013()
{
    bind("struct x { int y ; } z ;",
         Expectation()
            .declaration(Decl().Object("z", ObjectDeclarationCategory::Variable)
                     .ty_.Tag("x", TagTypeKind::Struct)));
}

void DeclarationBinderTester::case1014()
{
    bind("enum x { y } z ;",
         Expectation()
            .declaration(Decl().Object("z", ObjectDeclarationCategory::Variable)
                     .ty_.Tag("x", TagTypeKind::Enum)));
}

void DeclarationBinderTester::case1015()
{
    bind("_Bool x ;",
         Expectation()
             .declaration(Decl().Object("x", ObjectDeclarationCategory::Variable)
                              .ty_.Basic(BasicTypeKind::Bool)));
}

void DeclarationBinderTester::case1016()
{
    bind("x ;",
         Expectation()
             .diagnostic(Expectation::ErrorOrWarn::Error,
                         DeclarationBinder::DiagnosticsReporter::ID_TypeSpecifierMissingDefaultsToInt)
             .declaration(Decl()
                          .Object("x", ObjectDeclarationCategory::Variable)
                          .ty_.Basic(BasicTypeKind::Int_S)));
}

void DeclarationBinderTester::case1017()
{
    bind("* x ;",
         Expectation()
             .diagnostic(Expectation::ErrorOrWarn::Error,
                         DeclarationBinder::DiagnosticsReporter::ID_TypeSpecifierMissingDefaultsToInt)
             .declaration(Decl()
                          .Object("x", ObjectDeclarationCategory::Variable)
                          .ty_.Basic(BasicTypeKind::Int_S)
                          .ty_.Derived(TypeKind::Pointer)));
}

void DeclarationBinderTester::case1018()
{
    bind("* ( x ) ;",
         Expectation()
             .diagnostic(Expectation::ErrorOrWarn::Error,
                         DeclarationBinder::DiagnosticsReporter::ID_TypeSpecifierMissingDefaultsToInt)
             .declaration(Decl()
                          .Object("x", ObjectDeclarationCategory::Variable)
                          .ty_.Basic(BasicTypeKind::Int_S)
                          .ty_.Derived(TypeKind::Pointer)));
}

void DeclarationBinderTester::case1019(){}
void DeclarationBinderTester::case1020(){}
void DeclarationBinderTester::case1021(){}
void DeclarationBinderTester::case1022(){}
void DeclarationBinderTester::case1023(){}
void DeclarationBinderTester::case1024(){}
void DeclarationBinderTester::case1025(){}
void DeclarationBinderTester::case1026(){}
void DeclarationBinderTester::case1027(){}
void DeclarationBinderTester::case1028(){}
void DeclarationBinderTester::case1029(){}
void DeclarationBinderTester::case1030(){}
void DeclarationBinderTester::case1031(){}
void DeclarationBinderTester::case1032(){}
void DeclarationBinderTester::case1033(){}
void DeclarationBinderTester::case1034(){}
void DeclarationBinderTester::case1035(){}
void DeclarationBinderTester::case1036(){}
void DeclarationBinderTester::case1037(){}
void DeclarationBinderTester::case1038(){}
void DeclarationBinderTester::case1039(){}
void DeclarationBinderTester::case1040(){}
void DeclarationBinderTester::case1041(){}
void DeclarationBinderTester::case1042(){}
void DeclarationBinderTester::case1043(){}
void DeclarationBinderTester::case1044(){}
void DeclarationBinderTester::case1045(){}
void DeclarationBinderTester::case1046(){}
void DeclarationBinderTester::case1047(){}
void DeclarationBinderTester::case1048(){}
void DeclarationBinderTester::case1049() {}

void DeclarationBinderTester::case1050()
{
    bind("const int x ;",
         Expectation()
             .declaration(Decl().Object("x", ObjectDeclarationCategory::Variable)
                      .ty_.Basic(BasicTypeKind::Int_S, CVR::Const)));
}

void DeclarationBinderTester::case1051()
{
    bind("const x y ;",
         Expectation()
             .declaration(Decl().Object("y", ObjectDeclarationCategory::Variable)
                      .ty_.Typedef("x", CVR::Const)));
}

void DeclarationBinderTester::case1052()
{
    bind("const x ;",
         Expectation().diagnostic(
             Expectation::ErrorOrWarn::Error,
             DeclarationBinder::DiagnosticsReporter::ID_TypeSpecifierMissingDefaultsToInt));
}

void DeclarationBinderTester::case1053()
{
    bind("int const x ;",
         Expectation()
             .declaration(Decl().Object("x", ObjectDeclarationCategory::Variable)
                      .ty_.Basic(BasicTypeKind::Int_S, CVR::Const)));
}

void DeclarationBinderTester::case1054()
{
    bind("x const y ;",
         Expectation()
             .declaration(Decl().Object("y", ObjectDeclarationCategory::Variable)
                      .ty_.Typedef("x", CVR::Const)));
}

void DeclarationBinderTester::case1055()
{
    bind("int volatile x ;",
         Expectation()
             .declaration(Decl()
                      .Object("x", ObjectDeclarationCategory::Variable)
                      .ty_.Basic(BasicTypeKind::Int_S, CVR::Volatile)));
}

void DeclarationBinderTester::case1056()
{
    bind("int volatile const x ;",
         Expectation()
             .declaration(Decl()
                      .Object("x", ObjectDeclarationCategory::Variable)
                      .ty_.Basic(BasicTypeKind::Int_S, CVR::ConstAndVolatile)));
}

void DeclarationBinderTester::case1057()
{
    bind("int restrict x ;",
         Expectation().diagnostic(
             Expectation::ErrorOrWarn::Error,
             DeclarationBinder::DiagnosticsReporter::ID_InvalidUseOfRestrict));
}

void DeclarationBinderTester::case1058()
{
    bind("int const restrict x ;",
         Expectation().diagnostic(
             Expectation::ErrorOrWarn::Error,
             DeclarationBinder::DiagnosticsReporter::ID_InvalidUseOfRestrict));

}

void DeclarationBinderTester::case1059()
{
    bind("_Atomic int x ;",
         Expectation()
             .declaration(Decl().Object("x", ObjectDeclarationCategory::Variable)
                      .ty_.Basic(BasicTypeKind::Int_S, CVR::Atomic)));
}

void DeclarationBinderTester::case1060()
{
    bind("int const x , y ;",
         Expectation()
             .declaration(Decl()
                          .Object("x", ObjectDeclarationCategory::Variable)
                          .ty_.Basic(BasicTypeKind::Int_S, CVR::Const))
         .declaration(Decl()
                      .Object("y", ObjectDeclarationCategory::Variable)
                      .ty_.Basic(BasicTypeKind::Int_S, CVR::Const)));
}

void DeclarationBinderTester::case1061() {}
void DeclarationBinderTester::case1062() {}
void DeclarationBinderTester::case1063() {}
void DeclarationBinderTester::case1064() {}
void DeclarationBinderTester::case1065() {}
void DeclarationBinderTester::case1066() {}
void DeclarationBinderTester::case1067() {}
void DeclarationBinderTester::case1068() {}
void DeclarationBinderTester::case1069() {}
void DeclarationBinderTester::case1070() {}
void DeclarationBinderTester::case1071() {}
void DeclarationBinderTester::case1072() {}
void DeclarationBinderTester::case1073() {}
void DeclarationBinderTester::case1074() {}
void DeclarationBinderTester::case1075() {}
void DeclarationBinderTester::case1076() {}
void DeclarationBinderTester::case1077() {}
void DeclarationBinderTester::case1078() {}
void DeclarationBinderTester::case1079() {}
void DeclarationBinderTester::case1080() {}
void DeclarationBinderTester::case1081() {}
void DeclarationBinderTester::case1082() {}
void DeclarationBinderTester::case1083() {}
void DeclarationBinderTester::case1084() {}
void DeclarationBinderTester::case1085() {}
void DeclarationBinderTester::case1086() {}
void DeclarationBinderTester::case1087() {}
void DeclarationBinderTester::case1088() {}
void DeclarationBinderTester::case1089() {}
void DeclarationBinderTester::case1090() {}
void DeclarationBinderTester::case1091() {}
void DeclarationBinderTester::case1092() {}
void DeclarationBinderTester::case1093() {}
void DeclarationBinderTester::case1094() {}
void DeclarationBinderTester::case1095() {}
void DeclarationBinderTester::case1096() {}
void DeclarationBinderTester::case1097() {}
void DeclarationBinderTester::case1098() {}
void DeclarationBinderTester::case1099() {}

void DeclarationBinderTester::case1100()
{
    bind("int * x ;",
         Expectation()
             .declaration(Decl().Object("x", ObjectDeclarationCategory::Variable)
                      .ty_.Basic(BasicTypeKind::Int_S)
                      .ty_.Derived(TypeKind::Pointer)));
}

void DeclarationBinderTester::case1101()
{
    bind("x * y ;",
         Expectation()
             .declaration(Decl().Object("y", ObjectDeclarationCategory::Variable)
                      .ty_.Typedef("x")
                      .ty_.Derived(TypeKind::Pointer)));
}

void DeclarationBinderTester::case1102()
{
    bind("int * x ; y * z ;",
         Expectation()
             .declaration(Decl().Object("x", ObjectDeclarationCategory::Variable)
                      .ty_.Basic(BasicTypeKind::Int_S)
                      .ty_.Derived(TypeKind::Pointer))
             .declaration(Decl().Object("z", ObjectDeclarationCategory::Variable)
                      .ty_.Typedef("y")
                      .ty_.Derived(TypeKind::Pointer)));
}

void DeclarationBinderTester::case1103()
{
    bind("int * x , * y ;",
         Expectation()
             .declaration(Decl().Object("x", ObjectDeclarationCategory::Variable)
                      .ty_.Basic(BasicTypeKind::Int_S)
                      .ty_.Derived(TypeKind::Pointer))
             .declaration(Decl().Object("y", ObjectDeclarationCategory::Variable)
                      .ty_.Basic(BasicTypeKind::Int_S)
                      .ty_.Derived(TypeKind::Pointer)));
}

void DeclarationBinderTester::case1104()
{
    bind("int ( * x ) [ 1 ];",
         Expectation()
             .declaration(Decl().Object("x", ObjectDeclarationCategory::Variable)
                      .ty_.Basic(BasicTypeKind::Int_S)
                      .ty_.Derived(TypeKind::Array)
                      .ty_.Derived(TypeKind::Pointer)));
}

void DeclarationBinderTester::case1105()
{
    bind("int * * x ;",
         Expectation()
             .declaration(Decl().Object("x", ObjectDeclarationCategory::Variable)
                      .ty_.Basic(BasicTypeKind::Int_S)
                      .ty_.Derived(TypeKind::Pointer)
                      .ty_.Derived(TypeKind::Pointer)));
}

void DeclarationBinderTester::case1106()
{
    bind("int * * * x ;",
         Expectation()
             .declaration(Decl().Object("x", ObjectDeclarationCategory::Variable)
                      .ty_.Basic(BasicTypeKind::Int_S)
                      .ty_.Derived(TypeKind::Pointer)
                      .ty_.Derived(TypeKind::Pointer)
                      .ty_.Derived(TypeKind::Pointer)));
}

void DeclarationBinderTester::case1107()
{
    bind("int * ( * x ) [ 1 ] ;",
         Expectation()
             .declaration(Decl().Object("x", ObjectDeclarationCategory::Variable)
                      .ty_.Basic(BasicTypeKind::Int_S, CVR::None)
                      .ty_.Derived(TypeKind::Pointer, CVR::None)
                      .ty_.Derived(TypeKind::Array, CVR::None)
                      .ty_.Derived(TypeKind::Pointer, CVR::None)));
}

void DeclarationBinderTester::case1108()
{
    bind("int * x , y ;",
         Expectation()
             .declaration(Decl().Object("x", ObjectDeclarationCategory::Variable)
                      .ty_.Basic(BasicTypeKind::Int_S)
                      .ty_.Derived(TypeKind::Pointer))
             .declaration(Decl().Object("y", ObjectDeclarationCategory::Variable)
                      .ty_.Basic(BasicTypeKind::Int_S)));
}

void DeclarationBinderTester::case1109()
{
    bind("int * * x , * * y ;",
         Expectation()
             .declaration(Decl().Object("x", ObjectDeclarationCategory::Variable)
                      .ty_.Basic(BasicTypeKind::Int_S)
                      .ty_.Derived(TypeKind::Pointer)
                      .ty_.Derived(TypeKind::Pointer))
             .declaration(Decl().Object("y", ObjectDeclarationCategory::Variable)
                      .ty_.Basic(BasicTypeKind::Int_S)
                      .ty_.Derived(TypeKind::Pointer)
                      .ty_.Derived(TypeKind::Pointer)));
}

void DeclarationBinderTester::case1110()
{
    bind("int * * x , * y ;",
         Expectation()
             .declaration(Decl().Object("x", ObjectDeclarationCategory::Variable)
                      .ty_.Basic(BasicTypeKind::Int_S)
                      .ty_.Derived(TypeKind::Pointer)
                      .ty_.Derived(TypeKind::Pointer))
             .declaration(Decl().Object("y", ObjectDeclarationCategory::Variable)
                      .ty_.Basic(BasicTypeKind::Int_S)
                      .ty_.Derived(TypeKind::Pointer)));
}

void DeclarationBinderTester::case1111()
{
    bind("int * * x , * * * y ;",
         Expectation()
             .declaration(Decl().Object("x", ObjectDeclarationCategory::Variable)
                      .ty_.Basic(BasicTypeKind::Int_S)
                      .ty_.Derived(TypeKind::Pointer)
                      .ty_.Derived(TypeKind::Pointer))
             .declaration(Decl().Object("y", ObjectDeclarationCategory::Variable)
                      .ty_.Basic(BasicTypeKind::Int_S)
                      .ty_.Derived(TypeKind::Pointer)
                      .ty_.Derived(TypeKind::Pointer)
                      .ty_.Derived(TypeKind::Pointer)));
}

void DeclarationBinderTester::case1112()
{
    bind("int * x , * * * y ;",
         Expectation()
             .declaration(Decl().Object("x", ObjectDeclarationCategory::Variable)
                      .ty_.Basic(BasicTypeKind::Int_S)
                      .ty_.Derived(TypeKind::Pointer))
             .declaration(Decl().Object("y", ObjectDeclarationCategory::Variable)
                      .ty_.Basic(BasicTypeKind::Int_S)
                      .ty_.Derived(TypeKind::Pointer)
                      .ty_.Derived(TypeKind::Pointer)
                      .ty_.Derived(TypeKind::Pointer)));
}

void DeclarationBinderTester::case1113()
{
    bind("int * * * x , * y ;",
         Expectation()
             .declaration(Decl().Object("x", ObjectDeclarationCategory::Variable)
                      .ty_.Basic(BasicTypeKind::Int_S)
                      .ty_.Derived(TypeKind::Pointer)
                      .ty_.Derived(TypeKind::Pointer)
                      .ty_.Derived(TypeKind::Pointer))
             .declaration(Decl().Object("y", ObjectDeclarationCategory::Variable)
                      .ty_.Basic(BasicTypeKind::Int_S)
                      .ty_.Derived(TypeKind::Pointer)));
}

void DeclarationBinderTester::case1114()
{
    bind("int * * * x , y ;",
         Expectation()
             .declaration(Decl().Object("x", ObjectDeclarationCategory::Variable)
                      .ty_.Basic(BasicTypeKind::Int_S)
                      .ty_.Derived(TypeKind::Pointer)
                      .ty_.Derived(TypeKind::Pointer)
                      .ty_.Derived(TypeKind::Pointer))
             .declaration(Decl().Object("y", ObjectDeclarationCategory::Variable)
                      .ty_.Basic(BasicTypeKind::Int_S)));
}

void DeclarationBinderTester::case1115()
{
    bind("int x , * * * y ;",
         Expectation()
             .declaration(Decl().Object("x", ObjectDeclarationCategory::Variable)
                      .ty_.Basic(BasicTypeKind::Int_S))
             .declaration(Decl().Object("y", ObjectDeclarationCategory::Variable)
                      .ty_.Basic(BasicTypeKind::Int_S)
                      .ty_.Derived(TypeKind::Pointer)
                      .ty_.Derived(TypeKind::Pointer)
                      .ty_.Derived(TypeKind::Pointer)));
}

void DeclarationBinderTester::case1116()
{
    bind("int * x ; int y ;",
         Expectation()
              .declaration(Decl()
                           .Object("x", ObjectDeclarationCategory::Variable)
                           .ty_.Basic(BasicTypeKind::Int_S)
                           .ty_.Derived(TypeKind::Pointer))
              .declaration(Decl()
                           .Object("y", ObjectDeclarationCategory::Variable)
                           .ty_.Basic(BasicTypeKind::Int_S)));
}

void DeclarationBinderTester::case1117()
{
    bind("int * * x ; int y ;",
         Expectation()
              .declaration(Decl()
                           .Object("x", ObjectDeclarationCategory::Variable)
                           .ty_.Basic(BasicTypeKind::Int_S)
                           .ty_.Derived(TypeKind::Pointer)
                           .ty_.Derived(TypeKind::Pointer))
              .declaration(Decl()
                           .Object("y", ObjectDeclarationCategory::Variable)
                           .ty_.Basic(BasicTypeKind::Int_S)));
}

void DeclarationBinderTester::case1118()
{
    bind("int * * * x ; int y ;",
         Expectation()
              .declaration(Decl()
                           .Object("x", ObjectDeclarationCategory::Variable)
                           .ty_.Basic(BasicTypeKind::Int_S)
                           .ty_.Derived(TypeKind::Pointer)
                           .ty_.Derived(TypeKind::Pointer)
                           .ty_.Derived(TypeKind::Pointer))
              .declaration(Decl()
                           .Object("y", ObjectDeclarationCategory::Variable)
                           .ty_.Basic(BasicTypeKind::Int_S)));
}

void DeclarationBinderTester::case1119() {}
void DeclarationBinderTester::case1120() {}
void DeclarationBinderTester::case1121() {}
void DeclarationBinderTester::case1122() {}
void DeclarationBinderTester::case1123() {}
void DeclarationBinderTester::case1124() {}
void DeclarationBinderTester::case1125() {}
void DeclarationBinderTester::case1126() {}
void DeclarationBinderTester::case1127() {}
void DeclarationBinderTester::case1128() {}
void DeclarationBinderTester::case1129() {}

void DeclarationBinderTester::case1130()
{
    bind("int ( * x ) ( ) ;",
         Expectation()
             .declaration(Decl()
                      .Object("x", ObjectDeclarationCategory::Variable)
                      .ty_.Basic(BasicTypeKind::Int_S)
                      .ty_.Derived(TypeKind::Function)
                      .ty_.Derived(TypeKind::Pointer)));
}

void DeclarationBinderTester::case1131()
{
    bind("x ( * y ) ( ) ;",
         Expectation()
             .declaration(Decl()
                      .Object("y", ObjectDeclarationCategory::Variable)
                      .ty_.Typedef("x")
                      .ty_.Derived(TypeKind::Function)
                      .ty_.Derived(TypeKind::Pointer)));
}

void DeclarationBinderTester::case1132()
{
    bind("int ( * x ) ( double ) ;",
         Expectation()
              .declaration(Decl()
                      .Object("x", ObjectDeclarationCategory::Variable)
                      .ty_.Basic(BasicTypeKind::Int_S)
                      .ty_.Derived(TypeKind::Function)
                      .ty_.addParam().Basic(BasicTypeKind::Double)
                      .ty_.Derived(TypeKind::Pointer))
              .declaration(Decl()
                     .Object("", ObjectDeclarationCategory::Parameter, ScopeKind::FunctionPrototype)
                     .ty_.Basic(BasicTypeKind::Double)));
}

void DeclarationBinderTester::case1133()
{
    bind("x ( * y ) ( double ) ;",
         Expectation()
             .declaration(Decl()
                      .Object("y", ObjectDeclarationCategory::Variable)
                      .ty_.Typedef("x")
                      .ty_.Derived(TypeKind::Function)
                      .ty_.addParam().Basic(BasicTypeKind::Double)
                      .ty_.Derived(TypeKind::Pointer))
         .declaration(Decl()
                  .Object("", ObjectDeclarationCategory::Parameter, ScopeKind::FunctionPrototype)
                  .ty_.Basic(BasicTypeKind::Double)));
}

void DeclarationBinderTester::case1134()
{
    bind("int ( * x ) ( double , char ) ;",
         Expectation()
             .declaration(Decl()
                      .Object("x", ObjectDeclarationCategory::Variable)
                      .ty_.Basic(BasicTypeKind::Int_S)
                      .ty_.Derived(TypeKind::Function)
                      .ty_.addParam().Basic(BasicTypeKind::Double)
                      .ty_.addParam().Basic(BasicTypeKind::Char)
                      .ty_.Derived(TypeKind::Pointer))
         .declaration(Decl()
                 .Object("", ObjectDeclarationCategory::Parameter, ScopeKind::FunctionPrototype)
                 .ty_.Basic(BasicTypeKind::Double))
         .declaration(Decl()
                 .Object("", ObjectDeclarationCategory::Parameter, ScopeKind::FunctionPrototype)
                 .ty_.Basic(BasicTypeKind::Char)));
}

void DeclarationBinderTester::case1135()
{
    bind("int ( * x ) ( y , char ) ;",
         Expectation()
             .declaration(Decl()
                      .Object("x", ObjectDeclarationCategory::Variable)
                      .ty_.Basic(BasicTypeKind::Int_S)
                      .ty_.Derived(TypeKind::Function)
                      .ty_.addParam().Typedef("y")
                      .ty_.addParam().Basic(BasicTypeKind::Char)
                      .ty_.Derived(TypeKind::Pointer))
         .declaration(Decl()
                 .Object("", ObjectDeclarationCategory::Parameter, ScopeKind::FunctionPrototype)
                 .ty_.Typedef("y"))
         .declaration(Decl()
                 .Object("", ObjectDeclarationCategory::Parameter, ScopeKind::FunctionPrototype)
                 .ty_.Basic(BasicTypeKind::Char)));
}

void DeclarationBinderTester::case1136()
{
    bind("void ( * x ) ( int ( * ) ( double) ) ;",
         Expectation()
             .declaration(Decl()
                      .Object("x", ObjectDeclarationCategory::Variable)
                      .ty_.Void()
                      .ty_.Derived(TypeKind::Function)
                      .ty_.addParam().Basic(BasicTypeKind::Int_S)
                      .ty_.atParam().Derived(TypeKind::Function)
                      .ty_.atParam().addParam().Basic(BasicTypeKind::Double)
                      .ty_.atParam().Derived(TypeKind::Pointer)
                      .ty_.Derived(TypeKind::Pointer))
             .declaration(Decl()
                     .Object("", ObjectDeclarationCategory::Parameter, ScopeKind::FunctionPrototype)
                     .ty_.Basic(BasicTypeKind::Int_S)
                     .ty_.Derived(TypeKind::Function)
                     .ty_.addParam().Basic(BasicTypeKind::Double)
                     .ty_.Derived(TypeKind::Pointer))
             .declaration(Decl()
                    .Object("", ObjectDeclarationCategory::Parameter, ScopeKind::FunctionPrototype)
                    .ty_.Basic(BasicTypeKind::Double)));
}

void DeclarationBinderTester::case1137() {}
void DeclarationBinderTester::case1138() {}
void DeclarationBinderTester::case1139() {}
void DeclarationBinderTester::case1140() {}
void DeclarationBinderTester::case1141() {}
void DeclarationBinderTester::case1142() {}
void DeclarationBinderTester::case1143() {}
void DeclarationBinderTester::case1144() {}
void DeclarationBinderTester::case1145() {}
void DeclarationBinderTester::case1146() {}
void DeclarationBinderTester::case1147() {}
void DeclarationBinderTester::case1148() {}
void DeclarationBinderTester::case1149() {}

void DeclarationBinderTester::case1150()
{
    bind("const int * x ;",
         Expectation()
             .declaration(Decl().Object("x", ObjectDeclarationCategory::Variable)
                      .ty_.Basic(BasicTypeKind::Int_S, CVR::Const)
                      .ty_.Derived(TypeKind::Pointer)));
}

void DeclarationBinderTester::case1151()
{
    bind("const x * y ;",
         Expectation()
             .declaration(Decl().Object("y", ObjectDeclarationCategory::Variable)
                      .ty_.Typedef("x", CVR::Const)
                      .ty_.Derived(TypeKind::Pointer)));
}

void DeclarationBinderTester::case1152()
{
}

void DeclarationBinderTester::case1153() {}
void DeclarationBinderTester::case1154() {}
void DeclarationBinderTester::case1155() {}
void DeclarationBinderTester::case1156() {}
void DeclarationBinderTester::case1157() {}
void DeclarationBinderTester::case1158() {}
void DeclarationBinderTester::case1159() {}
void DeclarationBinderTester::case1160() {}
void DeclarationBinderTester::case1161() {}
void DeclarationBinderTester::case1162() {}
void DeclarationBinderTester::case1163() {}
void DeclarationBinderTester::case1164() {}
void DeclarationBinderTester::case1165() {}
void DeclarationBinderTester::case1166() {}
void DeclarationBinderTester::case1167() {}
void DeclarationBinderTester::case1168() {}
void DeclarationBinderTester::case1169() {}
void DeclarationBinderTester::case1170() {}
void DeclarationBinderTester::case1171() {}
void DeclarationBinderTester::case1172() {}
void DeclarationBinderTester::case1173() {}
void DeclarationBinderTester::case1174() {}
void DeclarationBinderTester::case1175() {}
void DeclarationBinderTester::case1176() {}
void DeclarationBinderTester::case1177() {}
void DeclarationBinderTester::case1178() {}
void DeclarationBinderTester::case1179() {}
void DeclarationBinderTester::case1180() {}
void DeclarationBinderTester::case1181() {}
void DeclarationBinderTester::case1182() {}
void DeclarationBinderTester::case1183() {}
void DeclarationBinderTester::case1184() {}
void DeclarationBinderTester::case1185() {}
void DeclarationBinderTester::case1186() {}
void DeclarationBinderTester::case1187() {}
void DeclarationBinderTester::case1188() {}
void DeclarationBinderTester::case1189() {}
void DeclarationBinderTester::case1190() {}
void DeclarationBinderTester::case1191() {}
void DeclarationBinderTester::case1192() {}
void DeclarationBinderTester::case1193() {}
void DeclarationBinderTester::case1194() {}
void DeclarationBinderTester::case1195() {}
void DeclarationBinderTester::case1196() {}
void DeclarationBinderTester::case1197() {}
void DeclarationBinderTester::case1198() {}
void DeclarationBinderTester::case1199()
{}

void DeclarationBinderTester::case1200()
{
    bind("const int * const x ;",
         Expectation()
             .declaration(Decl().Object("x", ObjectDeclarationCategory::Variable)
                      .ty_.Basic(BasicTypeKind::Int_S, CVR::Const)
                      .ty_.Derived(TypeKind::Pointer, CVR::Const)));
}

void DeclarationBinderTester::case1201()
{
    bind("int const * restrict x ;",
         Expectation()
             .declaration(Decl().Object("x", ObjectDeclarationCategory::Variable)
                      .ty_.Basic(BasicTypeKind::Int_S, CVR::Const)
                      .ty_.Derived(TypeKind::Pointer, CVR::Restrict)));
}

void DeclarationBinderTester::case1202()
{
    bind("const int * restrict x ;",
         Expectation()
             .declaration(Decl().Object("x", ObjectDeclarationCategory::Variable)
                      .ty_.Basic(BasicTypeKind::Int_S, CVR::Const)
                      .ty_.Derived(TypeKind::Pointer, CVR::Restrict)));
}

void DeclarationBinderTester::case1203()
{
    bind("const int * const restrict x ;",
         Expectation()
             .declaration(Decl().Object("x", ObjectDeclarationCategory::Variable)
                      .ty_.Basic(BasicTypeKind::Int_S, CVR::Const)
                      .ty_.Derived(TypeKind::Pointer, CVR::ConstAndRestrict)));
}

void DeclarationBinderTester::case1204() { }
void DeclarationBinderTester::case1205() { }
void DeclarationBinderTester::case1206() { }
void DeclarationBinderTester::case1207() { }
void DeclarationBinderTester::case1208() { }
void DeclarationBinderTester::case1209() { }
void DeclarationBinderTester::case1210() { }
void DeclarationBinderTester::case1211() { }
void DeclarationBinderTester::case1212() { }
void DeclarationBinderTester::case1213() { }
void DeclarationBinderTester::case1214() { }
void DeclarationBinderTester::case1215() { }
void DeclarationBinderTester::case1216() { }
void DeclarationBinderTester::case1217() { }
void DeclarationBinderTester::case1218() { }
void DeclarationBinderTester::case1219() { }
void DeclarationBinderTester::case1220() { }
void DeclarationBinderTester::case1221() { }
void DeclarationBinderTester::case1222() { }
void DeclarationBinderTester::case1223() { }
void DeclarationBinderTester::case1224() { }
void DeclarationBinderTester::case1225() { }
void DeclarationBinderTester::case1226() { }
void DeclarationBinderTester::case1227() { }
void DeclarationBinderTester::case1228() { }
void DeclarationBinderTester::case1229() { }
void DeclarationBinderTester::case1230() { }
void DeclarationBinderTester::case1231() { }
void DeclarationBinderTester::case1232() { }
void DeclarationBinderTester::case1233() { }
void DeclarationBinderTester::case1234() { }
void DeclarationBinderTester::case1235() { }
void DeclarationBinderTester::case1236() { }
void DeclarationBinderTester::case1237() { }
void DeclarationBinderTester::case1238() { }
void DeclarationBinderTester::case1239() { }
void DeclarationBinderTester::case1240() { }
void DeclarationBinderTester::case1241() { }
void DeclarationBinderTester::case1242() { }
void DeclarationBinderTester::case1243() { }
void DeclarationBinderTester::case1244() { }
void DeclarationBinderTester::case1245() { }
void DeclarationBinderTester::case1246() { }
void DeclarationBinderTester::case1247() { }
void DeclarationBinderTester::case1248() { }
void DeclarationBinderTester::case1249() { }

void DeclarationBinderTester::case1250()
{
    bind("int * const x ;",
         Expectation()
             .declaration(Decl().Object("x", ObjectDeclarationCategory::Variable)
                      .ty_.Basic(BasicTypeKind::Int_S, CVR::None)
                      .ty_.Derived(TypeKind::Pointer, CVR::Const)));
}

void DeclarationBinderTester::case1251()
{
    bind("x * const y ;",
         Expectation()
             .declaration(Decl().Object("y", ObjectDeclarationCategory::Variable)
                      .ty_.Typedef("x", CVR::None)
                      .ty_.Derived(TypeKind::Pointer, CVR::Const)));
}

void DeclarationBinderTester::case1252()
{
    bind("int * restrict x ;",
         Expectation()
             .declaration(Decl().Object("x", ObjectDeclarationCategory::Variable)
                      .ty_.Basic(BasicTypeKind::Int_S, CVR::None)
                      .ty_.Derived(TypeKind::Pointer, CVR::Restrict)));
}

void DeclarationBinderTester::case1253()
{
    bind("x * restrict y ;",
         Expectation()
             .declaration(Decl().Object("y", ObjectDeclarationCategory::Variable)
                      .ty_.Typedef("x", CVR::None)
                      .ty_.Derived(TypeKind::Pointer, CVR::Restrict)));
}

void DeclarationBinderTester::case1254() { }
void DeclarationBinderTester::case1255() { }
void DeclarationBinderTester::case1256() { }
void DeclarationBinderTester::case1257() { }
void DeclarationBinderTester::case1258() { }
void DeclarationBinderTester::case1259() { }
void DeclarationBinderTester::case1260() { }
void DeclarationBinderTester::case1261() { }
void DeclarationBinderTester::case1262() { }
void DeclarationBinderTester::case1263() { }
void DeclarationBinderTester::case1264() { }
void DeclarationBinderTester::case1265() { }
void DeclarationBinderTester::case1266() { }
void DeclarationBinderTester::case1267() { }
void DeclarationBinderTester::case1268() { }
void DeclarationBinderTester::case1269() { }
void DeclarationBinderTester::case1270() { }
void DeclarationBinderTester::case1271() { }
void DeclarationBinderTester::case1272() { }
void DeclarationBinderTester::case1273() { }
void DeclarationBinderTester::case1274() { }
void DeclarationBinderTester::case1275() { }
void DeclarationBinderTester::case1276() { }
void DeclarationBinderTester::case1277() { }
void DeclarationBinderTester::case1278() { }
void DeclarationBinderTester::case1279() { }
void DeclarationBinderTester::case1280() { }
void DeclarationBinderTester::case1281() { }
void DeclarationBinderTester::case1282() { }
void DeclarationBinderTester::case1283() { }
void DeclarationBinderTester::case1284() { }
void DeclarationBinderTester::case1285() { }
void DeclarationBinderTester::case1286() { }
void DeclarationBinderTester::case1287() { }
void DeclarationBinderTester::case1288() { }
void DeclarationBinderTester::case1289() { }
void DeclarationBinderTester::case1290() { }
void DeclarationBinderTester::case1291() { }
void DeclarationBinderTester::case1292() { }
void DeclarationBinderTester::case1293() { }
void DeclarationBinderTester::case1294() { }
void DeclarationBinderTester::case1295() { }
void DeclarationBinderTester::case1296() { }
void DeclarationBinderTester::case1297() { }
void DeclarationBinderTester::case1298() { }
void DeclarationBinderTester::case1299() { }

void DeclarationBinderTester::case1300()
{
    bind("int x [ 1 ] ;",
         Expectation()
             .declaration(Decl().Object("x", ObjectDeclarationCategory::Variable)
                      .ty_.Basic(BasicTypeKind::Int_S, CVR::None)
                      .ty_.Derived(TypeKind::Array, CVR::None)));
}

void DeclarationBinderTester::case1301()
{
    bind("x y [ 1 ] ;",
         Expectation()
             .declaration(Decl().Object("y", ObjectDeclarationCategory::Variable)
                      .ty_.Typedef("x", CVR::None)
                      .ty_.Derived(TypeKind::Array, CVR::None)));
}

void DeclarationBinderTester::case1302()
{
    bind("int x [ 1 ] , y [ 2 ] ;",
         Expectation()
             .declaration(Decl().Object("x", ObjectDeclarationCategory::Variable)
                      .ty_.Basic(BasicTypeKind::Int_S, CVR::None)
                      .ty_.Derived(TypeKind::Array, CVR::None))
             .declaration(Decl().Object("y", ObjectDeclarationCategory::Variable)
                      .ty_.Basic(BasicTypeKind::Int_S, CVR::None)
                      .ty_.Derived(TypeKind::Array, CVR::None)));
}

void DeclarationBinderTester::case1303()
{
    bind("x y [ 1 ] , z [ 2 ] ;",
         Expectation()
             .declaration(Decl().Object("y", ObjectDeclarationCategory::Variable)
                      .ty_.Typedef("x", CVR::None)
                      .ty_.Derived(TypeKind::Array, CVR::None))
             .declaration(Decl().Object("z", ObjectDeclarationCategory::Variable)
                      .ty_.Typedef("x", CVR::None)
                      .ty_.Derived(TypeKind::Array, CVR::None)));
}

void DeclarationBinderTester::case1304()
{
    bind("int * x [ 1 ] ;",
         Expectation()
             .declaration(Decl().Object("x", ObjectDeclarationCategory::Variable)
                      .ty_.Basic(BasicTypeKind::Int_S, CVR::None)
                      .ty_.Derived(TypeKind::Pointer, CVR::None)
                      .ty_.Derived(TypeKind::Array, CVR::None)));
}

void DeclarationBinderTester::case1305()
{
    bind("int x [ 1 ] , * y [ 2 ] ;",
         Expectation()
             .declaration(Decl().Object("x", ObjectDeclarationCategory::Variable)
                      .ty_.Basic(BasicTypeKind::Int_S, CVR::None)
                      .ty_.Derived(TypeKind::Array, CVR::None))
             .declaration(Decl().Object("y", ObjectDeclarationCategory::Variable)
                      .ty_.Basic(BasicTypeKind::Int_S, CVR::None)
                      .ty_.Derived(TypeKind::Pointer, CVR::None)
                      .ty_.Derived(TypeKind::Array, CVR::None)));
}

void DeclarationBinderTester::case1306()
{
    bind("int * * x [ 1 ] ;",
         Expectation()
             .declaration(Decl().Object("x", ObjectDeclarationCategory::Variable)
                      .ty_.Basic(BasicTypeKind::Int_S, CVR::None)
                      .ty_.Derived(TypeKind::Pointer, CVR::None)
                      .ty_.Derived(TypeKind::Pointer, CVR::None)
                      .ty_.Derived(TypeKind::Array, CVR::None)));
}

void DeclarationBinderTester::case1307()
{
}

void DeclarationBinderTester::case1308(){ }
void DeclarationBinderTester::case1309(){ }
void DeclarationBinderTester::case1310(){ }
void DeclarationBinderTester::case1311(){ }
void DeclarationBinderTester::case1312(){ }
void DeclarationBinderTester::case1313(){ }
void DeclarationBinderTester::case1314(){ }
void DeclarationBinderTester::case1315(){ }
void DeclarationBinderTester::case1316(){ }
void DeclarationBinderTester::case1317(){ }
void DeclarationBinderTester::case1318(){ }
void DeclarationBinderTester::case1319(){ }
void DeclarationBinderTester::case1320(){ }
void DeclarationBinderTester::case1321(){ }
void DeclarationBinderTester::case1322(){ }
void DeclarationBinderTester::case1323(){ }
void DeclarationBinderTester::case1324(){ }
void DeclarationBinderTester::case1325(){ }
void DeclarationBinderTester::case1326(){ }
void DeclarationBinderTester::case1327(){ }
void DeclarationBinderTester::case1328(){ }
void DeclarationBinderTester::case1329(){ }
void DeclarationBinderTester::case1330(){ }
void DeclarationBinderTester::case1331(){ }
void DeclarationBinderTester::case1332(){ }
void DeclarationBinderTester::case1333(){ }
void DeclarationBinderTester::case1334(){ }
void DeclarationBinderTester::case1335(){ }
void DeclarationBinderTester::case1336(){ }
void DeclarationBinderTester::case1337(){ }
void DeclarationBinderTester::case1338(){ }
void DeclarationBinderTester::case1339(){ }
void DeclarationBinderTester::case1340(){ }
void DeclarationBinderTester::case1341(){ }
void DeclarationBinderTester::case1342(){ }
void DeclarationBinderTester::case1343(){ }
void DeclarationBinderTester::case1344(){ }
void DeclarationBinderTester::case1345(){ }
void DeclarationBinderTester::case1346(){ }
void DeclarationBinderTester::case1347(){ }
void DeclarationBinderTester::case1348(){ }
void DeclarationBinderTester::case1349(){ }

void DeclarationBinderTester::case1350()
{
    bind("const int x [ 1 ] ;",
         Expectation()
             .declaration(Decl().Object("x", ObjectDeclarationCategory::Variable)
                      .ty_.Basic(BasicTypeKind::Int_S, CVR::Const)
                      .ty_.Derived(TypeKind::Array, CVR::None)));
}

void DeclarationBinderTester::case1351(){ }
void DeclarationBinderTester::case1352(){ }
void DeclarationBinderTester::case1353(){ }
void DeclarationBinderTester::case1354(){ }
void DeclarationBinderTester::case1355(){ }
void DeclarationBinderTester::case1356(){ }
void DeclarationBinderTester::case1357(){ }
void DeclarationBinderTester::case1358(){ }
void DeclarationBinderTester::case1359(){ }
void DeclarationBinderTester::case1360(){ }
void DeclarationBinderTester::case1361(){ }
void DeclarationBinderTester::case1362(){ }
void DeclarationBinderTester::case1363(){ }
void DeclarationBinderTester::case1364(){ }
void DeclarationBinderTester::case1365(){ }
void DeclarationBinderTester::case1366(){ }
void DeclarationBinderTester::case1367(){ }
void DeclarationBinderTester::case1368(){ }
void DeclarationBinderTester::case1369(){ }
void DeclarationBinderTester::case1370(){ }
void DeclarationBinderTester::case1371(){ }
void DeclarationBinderTester::case1372(){ }
void DeclarationBinderTester::case1373(){ }
void DeclarationBinderTester::case1374(){ }
void DeclarationBinderTester::case1375(){ }
void DeclarationBinderTester::case1376(){ }
void DeclarationBinderTester::case1377(){ }
void DeclarationBinderTester::case1378(){ }
void DeclarationBinderTester::case1379(){ }
void DeclarationBinderTester::case1380(){ }
void DeclarationBinderTester::case1381(){ }
void DeclarationBinderTester::case1382(){ }
void DeclarationBinderTester::case1383(){ }
void DeclarationBinderTester::case1384(){ }
void DeclarationBinderTester::case1385(){ }
void DeclarationBinderTester::case1386(){ }
void DeclarationBinderTester::case1387(){ }
void DeclarationBinderTester::case1388(){ }
void DeclarationBinderTester::case1389(){ }
void DeclarationBinderTester::case1390(){ }
void DeclarationBinderTester::case1391(){ }
void DeclarationBinderTester::case1392(){ }
void DeclarationBinderTester::case1393(){ }
void DeclarationBinderTester::case1394(){ }
void DeclarationBinderTester::case1395(){ }
void DeclarationBinderTester::case1396(){ }
void DeclarationBinderTester::case1397(){ }
void DeclarationBinderTester::case1398(){ }
void DeclarationBinderTester::case1399(){ }
