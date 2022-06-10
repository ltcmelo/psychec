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

#include "BinderTester.h"

#include "ParserTester.h"

#include "binder/ConstraintsInTypeSpecifiers.h"
#include "parser/Unparser.h"
#include "symbols/Symbol.h"
#include "symbols/Symbol_ALL.h"
#include "syntax/SyntaxLexeme_ALL.h"
#include "syntax/SyntaxNodes.h"

#include <algorithm>
#include <cstring>
#include <iostream>
#include <string>
#include <sstream>

using namespace psy;
using namespace C;

void BinderTester::case1000()
{
    bind("double x ;",
         Expectation()
            .binding(DeclSummary().Value("x", ValueKind::Variable)
                    .TySpec.basis("double", NamedTypeKind::Builtin, BuiltinTypeKind::Double)));
}

void BinderTester::case1001()
{
    bind("int x ;",
         Expectation()
            .binding(DeclSummary().Value("x", ValueKind::Variable)
                     .TySpec.basis("int", NamedTypeKind::Builtin, BuiltinTypeKind::Int)));
}

void BinderTester::case1002()
{
    bind("int x ; int y ;",
         Expectation()
            .binding(DeclSummary().Value("x", ValueKind::Variable)
                    .TySpec.basis("int", NamedTypeKind::Builtin, BuiltinTypeKind::Int))
            .binding(DeclSummary().Value("y", ValueKind::Variable)
                    .TySpec.basis("int", NamedTypeKind::Builtin, BuiltinTypeKind::Int)));
}

void BinderTester::case1003()
{
    bind("int x , y ;",
         Expectation()
             .binding(DeclSummary().Value("x", ValueKind::Variable)
                     .TySpec.basis("int", NamedTypeKind::Builtin, BuiltinTypeKind::Int))
             .binding(DeclSummary().Value("y", ValueKind::Variable)
                     .TySpec.basis("int", NamedTypeKind::Builtin, BuiltinTypeKind::Int)));
}

void BinderTester::case1004()
{
    bind("x y ;",
         Expectation()
             .binding(DeclSummary().Value("y", ValueKind::Variable)
                 .TySpec.basis("x", NamedTypeKind::Synonym, BuiltinTypeKind::UNSPECIFIED)));
}

void BinderTester::case1005()
{
    bind("x y , z ;",
         Expectation()
             .binding(DeclSummary().Value("y", ValueKind::Variable)
                      .TySpec.basis("x", NamedTypeKind::Synonym, BuiltinTypeKind::UNSPECIFIED))
             .binding(DeclSummary().Value("z", ValueKind::Variable)
                      .TySpec.basis("x", NamedTypeKind::Synonym, BuiltinTypeKind::UNSPECIFIED)));

}

void BinderTester::case1006()
{
    bind("int x = 1 ;",
         Expectation()
            .binding(DeclSummary().Value("x", ValueKind::Variable)
                     .TySpec.basis("int", NamedTypeKind::Builtin, BuiltinTypeKind::Int)));
}

void BinderTester::case1007()
{
    bind("x y = 1 ;",
         Expectation()
             .binding(DeclSummary().Value("y", ValueKind::Variable)
                      .TySpec.basis("x", NamedTypeKind::Synonym, BuiltinTypeKind::UNSPECIFIED)));
}

void BinderTester::case1008()
{
    bind("int float x ;",
         Expectation().diagnostic(
             Expectation::ErrorOrWarn::Error,
             ConstraintsInTypeSpecifiers::ID_TwoOrMoreDataTypesInDeclarationSpecifiers));
}

void BinderTester::case1009()
{
    bind("struct x y ;",
         Expectation()
            .binding(DeclSummary().Value("y", ValueKind::Variable)
                    .TySpec.basis("struct x", NamedTypeKind::Structure, BuiltinTypeKind::UNSPECIFIED)));
}

void BinderTester::case1010()
{
    bind("union x y ;",
         Expectation()
            .binding(DeclSummary().Value("y", ValueKind::Variable)
                    .TySpec.basis("union x", NamedTypeKind::Union, BuiltinTypeKind::UNSPECIFIED)));
}

void BinderTester::case1011()
{
    bind("enum x y ;",
         Expectation()
            .binding(DeclSummary().Value("y", ValueKind::Variable)
                    .TySpec.basis("enum x", NamedTypeKind::Enumeration, BuiltinTypeKind::UNSPECIFIED)));
}

void BinderTester::case1012()
{
    bind("struct x y , z ;",
         Expectation()
            .binding(DeclSummary().Value("y", ValueKind::Variable)
                    .TySpec.basis("struct x", NamedTypeKind::Structure, BuiltinTypeKind::UNSPECIFIED))
            .binding(DeclSummary().Value("z", ValueKind::Variable)
                    .TySpec.basis("struct x", NamedTypeKind::Structure, BuiltinTypeKind::UNSPECIFIED)));
}

void BinderTester::case1013()
{
    bind("struct x { int y ; } z ;",
         Expectation()
            .binding(DeclSummary().Value("z", ValueKind::Variable)
                    .TySpec.basis("struct x", NamedTypeKind::Structure, BuiltinTypeKind::UNSPECIFIED)));
}

void BinderTester::case1014()
{
    bind("long x ;",
         Expectation()
            .binding(DeclSummary().Value("x", ValueKind::Variable)
                    .TySpec.basis("long", NamedTypeKind::Builtin, BuiltinTypeKind::Long)));
}

void BinderTester::case1015()
{
    bind("long int x ;",
         Expectation()
            .binding(DeclSummary().Value("x", ValueKind::Variable)
                    .TySpec.basis("long", NamedTypeKind::Builtin, BuiltinTypeKind::Long)));
}

void BinderTester::case1016()
{
    bind("int long x ;",
         Expectation()
            .binding(DeclSummary().Value("x", ValueKind::Variable)
                    .TySpec.basis("long", NamedTypeKind::Builtin, BuiltinTypeKind::Long)));
}

void BinderTester::case1017()
{
    bind("signed x ;",
         Expectation()
            .binding(DeclSummary().Value("x", ValueKind::Variable)
                    .TySpec.basis("signed int", NamedTypeKind::Builtin, BuiltinTypeKind::Int_S)));
}

void BinderTester::case1018()
{
    bind("signed int x ;",
         Expectation()
            .binding(DeclSummary().Value("x", ValueKind::Variable)
                    .TySpec.basis("signed int", NamedTypeKind::Builtin, BuiltinTypeKind::Int_S)));
}

void BinderTester::case1019()
{
}

void BinderTester::case1020() {}
void BinderTester::case1021() {}
void BinderTester::case1022() {}
void BinderTester::case1023() {}
void BinderTester::case1024() {}
void BinderTester::case1025() {}
void BinderTester::case1026() {}
void BinderTester::case1027() {}
void BinderTester::case1028() {}
void BinderTester::case1029() {}
void BinderTester::case1030() {}
void BinderTester::case1031() {}
void BinderTester::case1032() {}
void BinderTester::case1033() {}
void BinderTester::case1034() {}
void BinderTester::case1035() {}
void BinderTester::case1036() {}
void BinderTester::case1037() {}
void BinderTester::case1038() {}
void BinderTester::case1039() {}
void BinderTester::case1040() {}
void BinderTester::case1041() {}
void BinderTester::case1042() {}
void BinderTester::case1043() {}
void BinderTester::case1044() {}
void BinderTester::case1045() {}
void BinderTester::case1046() {}
void BinderTester::case1047() {}
void BinderTester::case1048() {}
void BinderTester::case1049() {}

void BinderTester::case1050()
{
    bind("const int x ;",
         Expectation()
             .binding(DeclSummary().Value("x", ValueKind::Variable)
                      .TySpec.basis("int", NamedTypeKind::Builtin, BuiltinTypeKind::Int, CVR::Const)));
}

void BinderTester::case1051()
{
    bind("const x y ;",
         Expectation()
             .binding(DeclSummary().Value("y", ValueKind::Variable)
                      .TySpec.basis("x", NamedTypeKind::Synonym, BuiltinTypeKind::UNSPECIFIED, CVR::Const)));
}

void BinderTester::case1052()
{
    bind("const x ;",
         Expectation().diagnostic(
             Expectation::ErrorOrWarn::Error,
             ConstraintsInTypeSpecifiers::ID_TypeSpecifierMissingDefaultsToInt));
}

void BinderTester::case1053()
{
    bind("int const x ;",
         Expectation()
             .binding(DeclSummary().Value("x", ValueKind::Variable)
                      .TySpec.basis("int", NamedTypeKind::Builtin, BuiltinTypeKind::Int, CVR::Const)));
}

void BinderTester::case1054()
{
    bind("x const y ;",
         Expectation()
             .binding(DeclSummary().Value("y", ValueKind::Variable)
                      .TySpec.basis("x", NamedTypeKind::Synonym, BuiltinTypeKind::UNSPECIFIED, CVR::Const)));
}

void BinderTester::case1055() {}
void BinderTester::case1056() {}
void BinderTester::case1057() {}
void BinderTester::case1058() {}
void BinderTester::case1059() {}
void BinderTester::case1060() {}
void BinderTester::case1061() {}
void BinderTester::case1062() {}
void BinderTester::case1063() {}
void BinderTester::case1064() {}
void BinderTester::case1065() {}
void BinderTester::case1066() {}
void BinderTester::case1067() {}
void BinderTester::case1068() {}
void BinderTester::case1069() {}
void BinderTester::case1070() {}
void BinderTester::case1071() {}
void BinderTester::case1072() {}
void BinderTester::case1073() {}
void BinderTester::case1074() {}
void BinderTester::case1075() {}
void BinderTester::case1076() {}
void BinderTester::case1077() {}
void BinderTester::case1078() {}
void BinderTester::case1079() {}
void BinderTester::case1080() {}
void BinderTester::case1081() {}
void BinderTester::case1082() {}
void BinderTester::case1083() {}
void BinderTester::case1084() {}
void BinderTester::case1085() {}
void BinderTester::case1086() {}
void BinderTester::case1087() {}
void BinderTester::case1088() {}
void BinderTester::case1089() {}
void BinderTester::case1090() {}
void BinderTester::case1091() {}
void BinderTester::case1092() {}
void BinderTester::case1093() {}
void BinderTester::case1094() {}
void BinderTester::case1095() {}
void BinderTester::case1096() {}
void BinderTester::case1097() {}
void BinderTester::case1098() {}
void BinderTester::case1099() {}

void BinderTester::case1100()
{
    bind("int * x ;",
         Expectation()
             .binding(DeclSummary().Value("x", ValueKind::Variable)
                      .TySpec.basis("int", NamedTypeKind::Builtin, BuiltinTypeKind::Int)
                      .TySpec.deriv(TypeKind::Pointer)));
}

void BinderTester::case1101()
{
    bind("x * y ;",
         Expectation()
             .binding(DeclSummary().Value("y", ValueKind::Variable)
                      .TySpec.basis("x", NamedTypeKind::Synonym)
                      .TySpec.deriv(TypeKind::Pointer)));
}

void BinderTester::case1102()
{
    bind("int * x ; y * z ;",
         Expectation()
             .binding(DeclSummary().Value("x", ValueKind::Variable)
                      .TySpec.basis("int", NamedTypeKind::Builtin, BuiltinTypeKind::Int)
                      .TySpec.deriv(TypeKind::Pointer))
             .binding(DeclSummary().Value("z", ValueKind::Variable)
                      .TySpec.basis("y", NamedTypeKind::Synonym)
                      .TySpec.deriv(TypeKind::Pointer)));
}

void BinderTester::case1103()
{
    bind("int * x , * y ;",
         Expectation()
             .binding(DeclSummary().Value("x", ValueKind::Variable)
                      .TySpec.basis("int", NamedTypeKind::Builtin, BuiltinTypeKind::Int)
                      .TySpec.deriv(TypeKind::Pointer))
             .binding(DeclSummary().Value("y", ValueKind::Variable)
                      .TySpec.basis("int", NamedTypeKind::Builtin, BuiltinTypeKind::Int)
                      .TySpec.deriv(TypeKind::Pointer)));
}

void BinderTester::case1104()
{
    bind("int ( * x ) [ 1 ];",
         Expectation()
             .binding(DeclSummary().Value("x", ValueKind::Variable)
                      .TySpec.basis("int", NamedTypeKind::Builtin, BuiltinTypeKind::Int)
                      .TySpec.deriv(TypeKind::Array)
                      .TySpec.deriv(TypeKind::Pointer)));
}

void BinderTester::case1105()
{
    bind("int * * x ;",
         Expectation()
             .binding(DeclSummary().Value("x", ValueKind::Variable)
                      .TySpec.basis("int", NamedTypeKind::Builtin, BuiltinTypeKind::Int)
                      .TySpec.deriv(TypeKind::Pointer)
                      .TySpec.deriv(TypeKind::Pointer)));
}

void BinderTester::case1106()
{
    bind("int * * * x ;",
         Expectation()
             .binding(DeclSummary().Value("x", ValueKind::Variable)
                      .TySpec.basis("int", NamedTypeKind::Builtin, BuiltinTypeKind::Int)
                      .TySpec.deriv(TypeKind::Pointer)
                      .TySpec.deriv(TypeKind::Pointer)
                      .TySpec.deriv(TypeKind::Pointer)));
}

void BinderTester::case1107()
{
    bind("int * ( * x ) [ 1 ] ;",
         Expectation()
             .binding(DeclSummary().Value("x", ValueKind::Variable)
                      .TySpec.basis("int", NamedTypeKind::Builtin, BuiltinTypeKind::Int, CVR::None)
                      .TySpec.deriv(TypeKind::Pointer, CVR::None)
                      .TySpec.deriv(TypeKind::Array, CVR::None)
                      .TySpec.deriv(TypeKind::Pointer, CVR::None)));
}

void BinderTester::case1108() {}
void BinderTester::case1109() {}
void BinderTester::case1110() {}
void BinderTester::case1111() {}
void BinderTester::case1112() {}
void BinderTester::case1113() {}
void BinderTester::case1114() {}
void BinderTester::case1115() {}
void BinderTester::case1116() {}
void BinderTester::case1117() {}
void BinderTester::case1118() {}
void BinderTester::case1119() {}
void BinderTester::case1120() {}
void BinderTester::case1121() {}
void BinderTester::case1122() {}
void BinderTester::case1123() {}
void BinderTester::case1124() {}
void BinderTester::case1125() {}
void BinderTester::case1126() {}
void BinderTester::case1127() {}
void BinderTester::case1128() {}
void BinderTester::case1129() {}

void BinderTester::case1130()
{
    bind("int ( * x ) ( ) ;",
         Expectation()
             .binding(DeclSummary()
                      .Value("x", ValueKind::Variable)
                      .TySpec.basis("int", NamedTypeKind::Builtin, BuiltinTypeKind::Int)
                      .TySpec.deriv(TypeKind::Function)
                      .TySpec.deriv(TypeKind::Pointer)));
}

void BinderTester::case1131()
{
    bind("x ( * y ) ( ) ;",
         Expectation()
             .binding(DeclSummary()
                      .Value("y", ValueKind::Variable)
                      .TySpec.basis("x", NamedTypeKind::Synonym)
                      .TySpec.deriv(TypeKind::Function)
                      .TySpec.deriv(TypeKind::Pointer)));
}

void BinderTester::case1132()
{
    bind("int ( * x ) ( double ) ;",
         Expectation()
              .binding(DeclSummary()
                      .Value("x", ValueKind::Variable)
                      .TySpec.basis("int", NamedTypeKind::Builtin, BuiltinTypeKind::Int)
                      .TySpec.deriv(TypeKind::Function)
                      .TySpec.Parameter().basis("double", NamedTypeKind::Builtin, BuiltinTypeKind::Double)
                      .TySpec.deriv(TypeKind::Pointer))
              .binding(DeclSummary()
                     .Value("", ValueKind::Parameter, ScopeKind::FunctionPrototype)
                     .TySpec.basis("double", NamedTypeKind::Builtin, BuiltinTypeKind::Double)));
}

void BinderTester::case1133()
{
    bind("x ( * y ) ( double ) ;",
         Expectation()
             .binding(DeclSummary()
                      .Value("y", ValueKind::Variable)
                      .TySpec.basis("x", NamedTypeKind::Synonym)
                      .TySpec.deriv(TypeKind::Function)
                      .TySpec.Parameter().basis("double", NamedTypeKind::Builtin, BuiltinTypeKind::Double)
                      .TySpec.deriv(TypeKind::Pointer))
         .binding(DeclSummary()
                  .Value("", ValueKind::Parameter, ScopeKind::FunctionPrototype)
                  .TySpec.basis("double", NamedTypeKind::Builtin, BuiltinTypeKind::Double)));
}

void BinderTester::case1134()
{
    bind("int ( * x ) ( double , char ) ;",
         Expectation()
             .binding(DeclSummary()
                      .Value("x", ValueKind::Variable)
                      .TySpec.basis("int", NamedTypeKind::Builtin, BuiltinTypeKind::Int)
                      .TySpec.deriv(TypeKind::Function)
                      .TySpec.Parameter().basis("double", NamedTypeKind::Builtin, BuiltinTypeKind::Double)
                      .TySpec.Parameter().basis("char", NamedTypeKind::Builtin, BuiltinTypeKind::Char)
                      .TySpec.deriv(TypeKind::Pointer))
         .binding(DeclSummary()
                 .Value("", ValueKind::Parameter, ScopeKind::FunctionPrototype)
                 .TySpec.basis("double", NamedTypeKind::Builtin, BuiltinTypeKind::Double))
         .binding(DeclSummary()
                 .Value("", ValueKind::Parameter, ScopeKind::FunctionPrototype)
                 .TySpec.basis("char", NamedTypeKind::Builtin, BuiltinTypeKind::Char)));
}

void BinderTester::case1135()
{
    bind("int ( * x ) ( y , char ) ;",
         Expectation()
             .binding(DeclSummary()
                      .Value("x", ValueKind::Variable)
                      .TySpec.basis("int", NamedTypeKind::Builtin, BuiltinTypeKind::Int)
                      .TySpec.deriv(TypeKind::Function)
                      .TySpec.Parameter().basis("y", NamedTypeKind::Synonym)
                      .TySpec.Parameter().basis("char", NamedTypeKind::Builtin, BuiltinTypeKind::Char)
                      .TySpec.deriv(TypeKind::Pointer))
         .binding(DeclSummary()
                 .Value("", ValueKind::Parameter, ScopeKind::FunctionPrototype)
                 .TySpec.basis("y", NamedTypeKind::Synonym))
         .binding(DeclSummary()
                 .Value("", ValueKind::Parameter, ScopeKind::FunctionPrototype)
                 .TySpec.basis("char", NamedTypeKind::Builtin, BuiltinTypeKind::Char)));
}

void BinderTester::case1136()
{
    bind("void ( * x ) ( int ( * ) ( double) ) ;",
         Expectation()
             .binding(DeclSummary()
                      .Value("x", ValueKind::Variable)
                      .TySpec.basis("void", NamedTypeKind::Builtin, BuiltinTypeKind::Void)
                      .TySpec.deriv(TypeKind::Function)
                      .TySpec.Parameter().basis("int", NamedTypeKind::Builtin, BuiltinTypeKind::Int)
                      .TySpec._AtParam_().deriv(TypeKind::Function)
                      .TySpec._AtParam_().Parameter().basis("double", NamedTypeKind::Builtin, BuiltinTypeKind::Double)
                      .TySpec._AtParam_().deriv(TypeKind::Pointer)
                      .TySpec.deriv(TypeKind::Pointer))
             .binding(DeclSummary()
                     .Value("", ValueKind::Parameter)
                     .TySpec.basis("int", NamedTypeKind::Builtin, BuiltinTypeKind::Int)
                     .TySpec.deriv(TypeKind::Function)
                     .TySpec.Parameter().basis("double", NamedTypeKind::Builtin, BuiltinTypeKind::Double)
                     .TySpec.deriv(TypeKind::Pointer))
             .binding(DeclSummary()
                    .Value("", ValueKind::Parameter, ScopeKind::FunctionPrototype)
                    .TySpec.basis("double", NamedTypeKind::Builtin, BuiltinTypeKind::Double)));
}

void BinderTester::case1137() {}
void BinderTester::case1138() {}
void BinderTester::case1139() {}
void BinderTester::case1140() {}
void BinderTester::case1141() {}
void BinderTester::case1142() {}
void BinderTester::case1143() {}
void BinderTester::case1144() {}
void BinderTester::case1145() {}
void BinderTester::case1146() {}
void BinderTester::case1147() {}
void BinderTester::case1148() {}
void BinderTester::case1149() {}

void BinderTester::case1150()
{
    bind("const int * x ;",
         Expectation()
             .binding(DeclSummary().Value("x", ValueKind::Variable)
                      .TySpec.basis("int", NamedTypeKind::Builtin, BuiltinTypeKind::Int, CVR::Const)
                      .TySpec.deriv(TypeKind::Pointer)));
}

void BinderTester::case1151()
{
    bind("const x * y ;",
         Expectation()
             .binding(DeclSummary().Value("y", ValueKind::Variable)
                      .TySpec.basis("x", NamedTypeKind::Synonym, BuiltinTypeKind::UNSPECIFIED, CVR::Const)
                      .TySpec.deriv(TypeKind::Pointer)));
}

void BinderTester::case1152() {}
void BinderTester::case1153() {}
void BinderTester::case1154() {}
void BinderTester::case1155() {}
void BinderTester::case1156() {}
void BinderTester::case1157() {}
void BinderTester::case1158() {}
void BinderTester::case1159() {}
void BinderTester::case1160() {}
void BinderTester::case1161() {}
void BinderTester::case1162() {}
void BinderTester::case1163() {}
void BinderTester::case1164() {}
void BinderTester::case1165() {}
void BinderTester::case1166() {}
void BinderTester::case1167() {}
void BinderTester::case1168() {}
void BinderTester::case1169() {}
void BinderTester::case1170() {}
void BinderTester::case1171() {}
void BinderTester::case1172() {}
void BinderTester::case1173() {}
void BinderTester::case1174() {}
void BinderTester::case1175() {}
void BinderTester::case1176() {}
void BinderTester::case1177() {}
void BinderTester::case1178() {}
void BinderTester::case1179() {}
void BinderTester::case1180() {}
void BinderTester::case1181() {}
void BinderTester::case1182() {}
void BinderTester::case1183() {}
void BinderTester::case1184() {}
void BinderTester::case1185() {}
void BinderTester::case1186() {}
void BinderTester::case1187() {}
void BinderTester::case1188() {}
void BinderTester::case1189() {}
void BinderTester::case1190() {}
void BinderTester::case1191() {}
void BinderTester::case1192() {}
void BinderTester::case1193() {}
void BinderTester::case1194() {}
void BinderTester::case1195() {}
void BinderTester::case1196() {}
void BinderTester::case1197() {}
void BinderTester::case1198() {}
void BinderTester::case1199()
{}

void BinderTester::case1200()
{
    bind("const int * const x ;",
         Expectation()
             .binding(DeclSummary().Value("x", ValueKind::Variable)
                      .TySpec.basis("int", NamedTypeKind::Builtin, BuiltinTypeKind::Int, CVR::Const)
                      .TySpec.deriv(TypeKind::Pointer, CVR::Const)));
}

void BinderTester::case1201() { }
void BinderTester::case1202() { }
void BinderTester::case1203() { }
void BinderTester::case1204() { }
void BinderTester::case1205() { }
void BinderTester::case1206() { }
void BinderTester::case1207() { }
void BinderTester::case1208() { }
void BinderTester::case1209() { }
void BinderTester::case1210() { }
void BinderTester::case1211() { }
void BinderTester::case1212() { }
void BinderTester::case1213() { }
void BinderTester::case1214() { }
void BinderTester::case1215() { }
void BinderTester::case1216() { }
void BinderTester::case1217() { }
void BinderTester::case1218() { }
void BinderTester::case1219() { }
void BinderTester::case1220() { }
void BinderTester::case1221() { }
void BinderTester::case1222() { }
void BinderTester::case1223() { }
void BinderTester::case1224() { }
void BinderTester::case1225() { }
void BinderTester::case1226() { }
void BinderTester::case1227() { }
void BinderTester::case1228() { }
void BinderTester::case1229() { }
void BinderTester::case1230() { }
void BinderTester::case1231() { }
void BinderTester::case1232() { }
void BinderTester::case1233() { }
void BinderTester::case1234() { }
void BinderTester::case1235() { }
void BinderTester::case1236() { }
void BinderTester::case1237() { }
void BinderTester::case1238() { }
void BinderTester::case1239() { }
void BinderTester::case1240() { }
void BinderTester::case1241() { }
void BinderTester::case1242() { }
void BinderTester::case1243() { }
void BinderTester::case1244() { }
void BinderTester::case1245() { }
void BinderTester::case1246() { }
void BinderTester::case1247() { }
void BinderTester::case1248() { }
void BinderTester::case1249() { }

void BinderTester::case1250()
{
    bind("int * const x ;",
         Expectation()
             .binding(DeclSummary().Value("x", ValueKind::Variable)
                      .TySpec.basis("int", NamedTypeKind::Builtin, BuiltinTypeKind::Int, CVR::None)
                      .TySpec.deriv(TypeKind::Pointer, CVR::Const)));
}

void BinderTester::case1251()
{
    bind("x * const y ;",
         Expectation()
             .binding(DeclSummary().Value("y", ValueKind::Variable)
                      .TySpec.basis("x", NamedTypeKind::Synonym, BuiltinTypeKind::UNSPECIFIED, CVR::None)
                      .TySpec.deriv(TypeKind::Pointer, CVR::Const)));
}

void BinderTester::case1252() { }
void BinderTester::case1253() { }
void BinderTester::case1254() { }
void BinderTester::case1255() { }
void BinderTester::case1256() { }
void BinderTester::case1257() { }
void BinderTester::case1258() { }
void BinderTester::case1259() { }
void BinderTester::case1260() { }
void BinderTester::case1261() { }
void BinderTester::case1262() { }
void BinderTester::case1263() { }
void BinderTester::case1264() { }
void BinderTester::case1265() { }
void BinderTester::case1266() { }
void BinderTester::case1267() { }
void BinderTester::case1268() { }
void BinderTester::case1269() { }
void BinderTester::case1270() { }
void BinderTester::case1271() { }
void BinderTester::case1272() { }
void BinderTester::case1273() { }
void BinderTester::case1274() { }
void BinderTester::case1275() { }
void BinderTester::case1276() { }
void BinderTester::case1277() { }
void BinderTester::case1278() { }
void BinderTester::case1279() { }
void BinderTester::case1280() { }
void BinderTester::case1281() { }
void BinderTester::case1282() { }
void BinderTester::case1283() { }
void BinderTester::case1284() { }
void BinderTester::case1285() { }
void BinderTester::case1286() { }
void BinderTester::case1287() { }
void BinderTester::case1288() { }
void BinderTester::case1289() { }
void BinderTester::case1290() { }
void BinderTester::case1291() { }
void BinderTester::case1292() { }
void BinderTester::case1293() { }
void BinderTester::case1294() { }
void BinderTester::case1295() { }
void BinderTester::case1296() { }
void BinderTester::case1297() { }
void BinderTester::case1298() { }
void BinderTester::case1299() { }

void BinderTester::case1300()
{
    bind("int x [ 1 ] ;",
         Expectation()
             .binding(DeclSummary().Value("x", ValueKind::Variable)
                      .TySpec.basis("int", NamedTypeKind::Builtin, BuiltinTypeKind::Int, CVR::None)
                      .TySpec.deriv(TypeKind::Array, CVR::None)));
}

void BinderTester::case1301()
{
    bind("x y [ 1 ] ;",
         Expectation()
             .binding(DeclSummary().Value("y", ValueKind::Variable)
                      .TySpec.basis("x", NamedTypeKind::Synonym, BuiltinTypeKind::UNSPECIFIED, CVR::None)
                      .TySpec.deriv(TypeKind::Array, CVR::None)));
}

void BinderTester::case1302()
{
    bind("int x [ 1 ] , y [ 2 ] ;",
         Expectation()
             .binding(DeclSummary().Value("x", ValueKind::Variable)
                      .TySpec.basis("int", NamedTypeKind::Builtin, BuiltinTypeKind::Int, CVR::None)
                      .TySpec.deriv(TypeKind::Array, CVR::None))
             .binding(DeclSummary().Value("y", ValueKind::Variable)
                      .TySpec.basis("int", NamedTypeKind::Builtin, BuiltinTypeKind::Int, CVR::None)
                      .TySpec.deriv(TypeKind::Array, CVR::None)));
}

void BinderTester::case1303()
{
    bind("x y [ 1 ] , z [ 2 ] ;",
         Expectation()
             .binding(DeclSummary().Value("y", ValueKind::Variable)
                      .TySpec.basis("x", NamedTypeKind::Synonym, BuiltinTypeKind::UNSPECIFIED, CVR::None)
                      .TySpec.deriv(TypeKind::Array, CVR::None))
             .binding(DeclSummary().Value("z", ValueKind::Variable)
                      .TySpec.basis("x", NamedTypeKind::Synonym, BuiltinTypeKind::UNSPECIFIED, CVR::None)
                      .TySpec.deriv(TypeKind::Array, CVR::None)));
}

void BinderTester::case1304()
{
    bind("int * x [ 1 ] ;",
         Expectation()
             .binding(DeclSummary().Value("x", ValueKind::Variable)
                      .TySpec.basis("int", NamedTypeKind::Builtin, BuiltinTypeKind::Int, CVR::None)
                      .TySpec.deriv(TypeKind::Pointer, CVR::None)
                      .TySpec.deriv(TypeKind::Array, CVR::None)));
}

void BinderTester::case1305()
{
    bind("int x [ 1 ] , * y [ 2 ] ;",
         Expectation()
             .binding(DeclSummary().Value("x", ValueKind::Variable)
                      .TySpec.basis("int", NamedTypeKind::Builtin, BuiltinTypeKind::Int, CVR::None)
                      .TySpec.deriv(TypeKind::Array, CVR::None))
             .binding(DeclSummary().Value("y", ValueKind::Variable)
                      .TySpec.basis("int", NamedTypeKind::Builtin, BuiltinTypeKind::Int, CVR::None)
                      .TySpec.deriv(TypeKind::Pointer, CVR::None)
                      .TySpec.deriv(TypeKind::Array, CVR::None)));
}

void BinderTester::case1306()
{
    bind("int * * x [ 1 ] ;",
         Expectation()
             .binding(DeclSummary().Value("x", ValueKind::Variable)
                      .TySpec.basis("int", NamedTypeKind::Builtin, BuiltinTypeKind::Int, CVR::None)
                      .TySpec.deriv(TypeKind::Pointer, CVR::None)
                      .TySpec.deriv(TypeKind::Pointer, CVR::None)
                      .TySpec.deriv(TypeKind::Array, CVR::None)));
}

void BinderTester::case1307()
{
}

void BinderTester::case1308(){ }
void BinderTester::case1309(){ }
void BinderTester::case1310(){ }
void BinderTester::case1311(){ }
void BinderTester::case1312(){ }
void BinderTester::case1313(){ }
void BinderTester::case1314(){ }
void BinderTester::case1315(){ }
void BinderTester::case1316(){ }
void BinderTester::case1317(){ }
void BinderTester::case1318(){ }
void BinderTester::case1319(){ }
void BinderTester::case1320(){ }
void BinderTester::case1321(){ }
void BinderTester::case1322(){ }
void BinderTester::case1323(){ }
void BinderTester::case1324(){ }
void BinderTester::case1325(){ }
void BinderTester::case1326(){ }
void BinderTester::case1327(){ }
void BinderTester::case1328(){ }
void BinderTester::case1329(){ }
void BinderTester::case1330(){ }
void BinderTester::case1331(){ }
void BinderTester::case1332(){ }
void BinderTester::case1333(){ }
void BinderTester::case1334(){ }
void BinderTester::case1335(){ }
void BinderTester::case1336(){ }
void BinderTester::case1337(){ }
void BinderTester::case1338(){ }
void BinderTester::case1339(){ }
void BinderTester::case1340(){ }
void BinderTester::case1341(){ }
void BinderTester::case1342(){ }
void BinderTester::case1343(){ }
void BinderTester::case1344(){ }
void BinderTester::case1345(){ }
void BinderTester::case1346(){ }
void BinderTester::case1347(){ }
void BinderTester::case1348(){ }
void BinderTester::case1349(){ }

void BinderTester::case1350()
{
    bind("const int x [ 1 ] ;",
         Expectation()
             .binding(DeclSummary().Value("x", ValueKind::Variable)
                      .TySpec.basis("int", NamedTypeKind::Builtin, BuiltinTypeKind::Int, CVR::Const)
                      .TySpec.deriv(TypeKind::Array, CVR::None)));
}

void BinderTester::case1351(){ }
void BinderTester::case1352(){ }
void BinderTester::case1353(){ }
void BinderTester::case1354(){ }
void BinderTester::case1355(){ }
void BinderTester::case1356(){ }
void BinderTester::case1357(){ }
void BinderTester::case1358(){ }
void BinderTester::case1359(){ }
void BinderTester::case1360(){ }
void BinderTester::case1361(){ }
void BinderTester::case1362(){ }
void BinderTester::case1363(){ }
void BinderTester::case1364(){ }
void BinderTester::case1365(){ }
void BinderTester::case1366(){ }
void BinderTester::case1367(){ }
void BinderTester::case1368(){ }
void BinderTester::case1369(){ }
void BinderTester::case1370(){ }
void BinderTester::case1371(){ }
void BinderTester::case1372(){ }
void BinderTester::case1373(){ }
void BinderTester::case1374(){ }
void BinderTester::case1375(){ }
void BinderTester::case1376(){ }
void BinderTester::case1377(){ }
void BinderTester::case1378(){ }
void BinderTester::case1379(){ }
void BinderTester::case1380(){ }
void BinderTester::case1381(){ }
void BinderTester::case1382(){ }
void BinderTester::case1383(){ }
void BinderTester::case1384(){ }
void BinderTester::case1385(){ }
void BinderTester::case1386(){ }
void BinderTester::case1387(){ }
void BinderTester::case1388(){ }
void BinderTester::case1389(){ }
void BinderTester::case1390(){ }
void BinderTester::case1391(){ }
void BinderTester::case1392(){ }
void BinderTester::case1393(){ }
void BinderTester::case1394(){ }
void BinderTester::case1395(){ }
void BinderTester::case1396(){ }
void BinderTester::case1397(){ }
void BinderTester::case1398(){ }
void BinderTester::case1399(){ }
