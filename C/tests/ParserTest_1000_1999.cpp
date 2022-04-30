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

#include "ParserTest.h"

#include "BinderTest.h"

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

void ParserTest::case1000()
{
    parseExpression("1",
                    Expectation().AST( { IntegerConstantExpression }));
}

void ParserTest::case1001()
{
    parseExpression(".1",
                    Expectation().AST( { FloatingConstantExpression }));
}

void ParserTest::case1002()
{
    parseExpression("1.",
                    Expectation().AST( { FloatingConstantExpression }));
}

void ParserTest::case1003()
{
    parseExpression("1.2",
                    Expectation().AST( { FloatingConstantExpression }));
}

void ParserTest::case1004()
{
    parseExpression("1.2e3",
                    Expectation().AST( { FloatingConstantExpression }));
}

void ParserTest::case1005()
{
    parseExpression("1.2E3",
                    Expectation().AST( { FloatingConstantExpression }));
}

void ParserTest::case1006()
{
    parseExpression("1.2e+3",
                    Expectation().AST( { FloatingConstantExpression }));
}

void ParserTest::case1007()
{
    parseExpression("1.2E+3",
                    Expectation().AST( { FloatingConstantExpression }));
}

void ParserTest::case1008()
{
    parseExpression("1.2e-3",
                    Expectation().AST( { FloatingConstantExpression }));
}

void ParserTest::case1009()
{
    parseExpression("1.2E-3",
                    Expectation().AST( { FloatingConstantExpression }));
}

void ParserTest::case1010()
{
    parseExpression("1e2",
                    Expectation().AST( { FloatingConstantExpression }));
}

void ParserTest::case1011()
{
    parseExpression("1E2",
                    Expectation().AST( { FloatingConstantExpression }));
}

void ParserTest::case1012()
{
    parseExpression("1e+2",
                    Expectation().AST( { FloatingConstantExpression }));
}

void ParserTest::case1013()
{
    parseExpression("1e-2",
                    Expectation().AST( { FloatingConstantExpression }));
}

void ParserTest::case1014()
{
    parseExpression("1E+2",
                    Expectation().AST( { FloatingConstantExpression }));
}

void ParserTest::case1015()
{
    parseExpression("1E-2",
                    Expectation().AST( { FloatingConstantExpression }));
}

void ParserTest::case1016()
{
    parseExpression("1E+2",
                    Expectation().AST( { FloatingConstantExpression }));
}

void ParserTest::case1017()
{
    parseExpression("1.e2",
                    Expectation().AST( { FloatingConstantExpression }));
}

void ParserTest::case1018()
{
    parseExpression("1.E2",
                    Expectation().AST( { FloatingConstantExpression }));
}

void ParserTest::case1019()
{
    parseExpression("0",
                    Expectation().AST( { IntegerConstantExpression }));
}

void ParserTest::case1020()
{
    parseExpression("0L",
                    Expectation().AST( { IntegerConstantExpression }));
}

void ParserTest::case1021()
{
    parseExpression("int",
                    Expectation().diagnostic(Expectation::ErrorOrWarn::Error,
                                                Parser::DiagnosticsReporter::ID_of_ExpectedFIRSTofExpression));
}

void ParserTest::case1022()
{
    parseExpression("{",
                    Expectation().diagnostic(Expectation::ErrorOrWarn::Error,
                                                Parser::DiagnosticsReporter::ID_of_ExpectedFIRSTofExpression));
}

void ParserTest::case1023()
{
    parseExpression("0x1.2p3",
                    Expectation().AST( { FloatingConstantExpression }));
}

void ParserTest::case1024()
{
    parseExpression("0x1.ap2",
                    Expectation().AST( { FloatingConstantExpression }));
}

void ParserTest::case1025()
{
    parseExpression("0x1.Ap2",
                    Expectation().AST( { FloatingConstantExpression }));
}

void ParserTest::case1026()
{
    parseExpression("0x1.fp2",
                    Expectation().AST( { FloatingConstantExpression }));
}

void ParserTest::case1027()
{
    parseExpression("0x1.Fp2",
                    Expectation().AST( { FloatingConstantExpression }));
}

void ParserTest::case1028()
{
    parseExpression("0x1.2p3f",
                    Expectation().AST( { FloatingConstantExpression }));
}

void ParserTest::case1029()
{
    parseExpression("0x1.2p3F",
                    Expectation().AST( { FloatingConstantExpression }));
}

void ParserTest::case1030()
{
    parseExpression("x",
                    Expectation().AST( { IdentifierName }));
}

void ParserTest::case1031()
{
    parseExpression("_x",
                    Expectation().AST( { IdentifierName }));
}

void ParserTest::case1032()
{
    parseExpression("x_y",
                    Expectation().AST( { IdentifierName }));
}

void ParserTest::case1033()
{
    parseExpression("x1",
                    Expectation().AST( { IdentifierName }));
}

void ParserTest::case1034()
{
    parseExpression("x1_",
                    Expectation().AST( { IdentifierName }));
}

void ParserTest::case1035()
{
    parseExpression("x1y2",
                    Expectation().AST( { IdentifierName }));
}

void ParserTest::case1036()
{
    parseExpression("_x1y2_",
                    Expectation().AST( { IdentifierName }));
}

void ParserTest::case1037()
{
    parseExpression("X",
                    Expectation().AST( { IdentifierName }));
}

void ParserTest::case1038()
{
    parseExpression("_X",
                    Expectation().AST( { IdentifierName }));
}

void ParserTest::case1039()
{
    parseExpression("X1",
                    Expectation().AST( { IdentifierName }));
}

void ParserTest::case1040()
{
    parseExpression("_X1",
                    Expectation().AST( { IdentifierName }));
}

void ParserTest::case1041()
{
    parseExpression("xYz1",
                    Expectation().AST( { IdentifierName }));
}

void ParserTest::case1042()
{
    parseExpression("xYz1_",
                    Expectation().AST( { IdentifierName }));
}

void ParserTest::case1043()
{
    parseExpression("xY_z1",
                    Expectation().AST( { IdentifierName }));
}

void ParserTest::case1044()
{
    parseExpression("1l",
                    Expectation().AST( { IntegerConstantExpression }));
}

void ParserTest::case1045()
{
    parseExpression("1ll",
                    Expectation().AST( { IntegerConstantExpression }));
}

void ParserTest::case1046()
{
    parseExpression("1L",
                    Expectation().AST( { IntegerConstantExpression }));
}

void ParserTest::case1047()
{
    parseExpression("1LL",
                    Expectation().AST( { IntegerConstantExpression }));
}

void ParserTest::case1048()
{
    parseExpression("1u",
                    Expectation().AST( { IntegerConstantExpression }));
}

void ParserTest::case1049()
{
    parseExpression("1U",
                    Expectation().AST( { IntegerConstantExpression }));
}

void ParserTest::case1050()
{
    parseExpression("1uL",
                    Expectation().AST( { IntegerConstantExpression }));
}

void ParserTest::case1051()
{
    parseExpression("1ul",
                    Expectation().AST( { IntegerConstantExpression }));
}

void ParserTest::case1052()
{
    parseExpression("0x1u",
                    Expectation().AST( { IntegerConstantExpression }));
}

void ParserTest::case1053()
{
    parseExpression("0x1Ul",
                    Expectation().AST( { IntegerConstantExpression }));
}

void ParserTest::case1054()
{
    parseExpression("L'x'",
                    Expectation().AST( { CharacterConstantExpression }));
}

void ParserTest::case1055()
{
    parseExpression("u'x'",
                    Expectation().AST( { CharacterConstantExpression }));
}

void ParserTest::case1056()
{
    parseExpression("U'x'",
                    Expectation().AST( { CharacterConstantExpression }));
}

void ParserTest::case1057()
{
    parseExpression("'x'",
                    Expectation().AST( { CharacterConstantExpression }));
}

void ParserTest::case1058()
{
    parseExpression("u\"xyz\"",
                    Expectation().AST( { StringLiteralExpression }));
}

void ParserTest::case1059()
{
    parseExpression("\"xyz\"",
                    Expectation().AST( { StringLiteralExpression }));
}

void ParserTest::case1060()
{
    parseExpression("\"x\" \"y\"",
                    Expectation().AST( { StringLiteralExpression,
                                         StringLiteralExpression}));
}

void ParserTest::case1061()
{
    parseExpression("1Lu",
                    Expectation().AST( { IntegerConstantExpression }));

}

void ParserTest::case1062()
{
    parseExpression("0x1lU",
                    Expectation().AST( { IntegerConstantExpression }));
}

void ParserTest::case1063()
{
    parseExpression("0x1lUll",
                    Expectation().diagnostic(
                        Expectation::ErrorOrWarn::Error,
                        Parser::DiagnosticsReporter::ID_of_ExpectedFIRSTofExpression));
}

void ParserTest::case1064()
{
    parseExpression("1lul",
                    Expectation().diagnostic(
                        Expectation::ErrorOrWarn::Error,
                        Parser::DiagnosticsReporter::ID_of_ExpectedFIRSTofExpression));
}

void ParserTest::case1065()
{
    parseExpression("1ulu",
                    Expectation().diagnostic(
                        Expectation::ErrorOrWarn::Error,
                        Parser::DiagnosticsReporter::ID_of_ExpectedFIRSTofExpression));
}

void ParserTest::case1066()
{
    parseExpression("1ullu",
                    Expectation().diagnostic(
                        Expectation::ErrorOrWarn::Error,
                        Parser::DiagnosticsReporter::ID_of_ExpectedFIRSTofExpression));
}

void ParserTest::case1067()
{
    parseExpression("1uu",
                    Expectation().diagnostic(
                        Expectation::ErrorOrWarn::Error,
                        Parser::DiagnosticsReporter::ID_of_ExpectedFIRSTofExpression));
}

void ParserTest::case1068()
{
    parseExpression("1lll",
                    Expectation().diagnostic(
                        Expectation::ErrorOrWarn::Error,
                        Parser::DiagnosticsReporter::ID_of_ExpectedFIRSTofExpression));
}

void ParserTest::case1069()
{
}

void ParserTest::case1070()
{
}

void ParserTest::case1071()
{
}

void ParserTest::case1072()
{
}

void ParserTest::case1073()
{
}

void ParserTest::case1074() {}
void ParserTest::case1075() {}
void ParserTest::case1076() {}
void ParserTest::case1077() {}
void ParserTest::case1078() {}
void ParserTest::case1079() {}
void ParserTest::case1080() {}
void ParserTest::case1081() {}
void ParserTest::case1082() {}
void ParserTest::case1083() {}
void ParserTest::case1084() {}
void ParserTest::case1085() {}
void ParserTest::case1086() {}
void ParserTest::case1087() {}
void ParserTest::case1088() {}
void ParserTest::case1089() {}
void ParserTest::case1090() {}
void ParserTest::case1091() {}
void ParserTest::case1092() {}
void ParserTest::case1093() {}
void ParserTest::case1094() {}
void ParserTest::case1095() {}
void ParserTest::case1096() {}
void ParserTest::case1097() {}
void ParserTest::case1098() {}
void ParserTest::case1099() {}

void ParserTest::case1100()
{
    parseExpression("( 1 )",
                    Expectation().AST( { ParenthesizedExpression,
                                         IntegerConstantExpression }));
}

void ParserTest::case1101()
{
    parseExpression("( x )",
                    Expectation().AST( { ParenthesizedExpression,
                                         IdentifierName }));
}

void ParserTest::case1102()
{
    parseExpression("( ( x ) )",
                    Expectation().AST( { ParenthesizedExpression,
                                         ParenthesizedExpression,
                                         IdentifierName }));
}

void ParserTest::case1103()
{
    parseExpression("( 'x' )",
                    Expectation().AST( { ParenthesizedExpression,
                                         CharacterConstantExpression }));
}

void ParserTest::case1104()
{
    parseExpression("( x",
                    Expectation().diagnostic(Expectation::ErrorOrWarn::Error,
                                                Parser::DiagnosticsReporter::ID_of_ExpectedToken));
}

void ParserTest::case1105()
{
    parseExpression("( ( x )",
                    Expectation().diagnostic(Expectation::ErrorOrWarn::Error,
                                                Parser::DiagnosticsReporter::ID_of_ExpectedToken));
}

void ParserTest::case1106()
{
    parseExpression("( int",
                    Expectation().diagnostic(Expectation::ErrorOrWarn::Error,
                                                Parser::DiagnosticsReporter::ID_of_ExpectedToken));
}

void ParserTest::case1107()
{
    parseExpression("( int )",
                    Expectation().diagnostic(Expectation::ErrorOrWarn::Error,
                                                Parser::DiagnosticsReporter::ID_of_ExpectedFIRSTofExpression));
}

void ParserTest::case1108() {}
void ParserTest::case1109() {}
void ParserTest::case1110() {}
void ParserTest::case1111() {}
void ParserTest::case1112() {}
void ParserTest::case1113() {}
void ParserTest::case1114() {}
void ParserTest::case1115() {}
void ParserTest::case1116() {}
void ParserTest::case1117() {}
void ParserTest::case1118() {}
void ParserTest::case1119() {}
void ParserTest::case1120() {}
void ParserTest::case1121() {}
void ParserTest::case1122() {}
void ParserTest::case1123() {}
void ParserTest::case1124() {}
void ParserTest::case1125() {}
void ParserTest::case1126() {}
void ParserTest::case1127() {}
void ParserTest::case1128() {}
void ParserTest::case1129() {}
void ParserTest::case1130() {}
void ParserTest::case1131() {}
void ParserTest::case1132() {}
void ParserTest::case1133() {}
void ParserTest::case1134() {}
void ParserTest::case1135() {}
void ParserTest::case1136() {}
void ParserTest::case1137() {}
void ParserTest::case1138() {}
void ParserTest::case1139() {}
void ParserTest::case1140() {}
void ParserTest::case1141() {}
void ParserTest::case1142() {}
void ParserTest::case1143() {}
void ParserTest::case1144() {}
void ParserTest::case1145() {}
void ParserTest::case1146() {}
void ParserTest::case1147() {}
void ParserTest::case1148() {}
void ParserTest::case1149() {}
void ParserTest::case1150() {}
void ParserTest::case1151() {}
void ParserTest::case1152() {}
void ParserTest::case1153() {}
void ParserTest::case1154() {}
void ParserTest::case1155() {}
void ParserTest::case1156() {}
void ParserTest::case1157() {}
void ParserTest::case1158() {}
void ParserTest::case1159() {}
void ParserTest::case1160() {}
void ParserTest::case1161() {}
void ParserTest::case1162() {}
void ParserTest::case1163() {}
void ParserTest::case1164() {}
void ParserTest::case1165() {}
void ParserTest::case1166() {}
void ParserTest::case1167() {}
void ParserTest::case1168() {}
void ParserTest::case1169() {}
void ParserTest::case1170() {}
void ParserTest::case1171() {}
void ParserTest::case1172() {}
void ParserTest::case1173() {}
void ParserTest::case1174() {}
void ParserTest::case1175() {}
void ParserTest::case1176() {}
void ParserTest::case1177() {}
void ParserTest::case1178() {}
void ParserTest::case1179() {}
void ParserTest::case1180() {}
void ParserTest::case1181() {}
void ParserTest::case1182() {}
void ParserTest::case1183() {}
void ParserTest::case1184() {}
void ParserTest::case1185() {}
void ParserTest::case1186() {}
void ParserTest::case1187() {}
void ParserTest::case1188() {}
void ParserTest::case1189() {}
void ParserTest::case1190() {}
void ParserTest::case1191() {}
void ParserTest::case1192() {}
void ParserTest::case1193() {}
void ParserTest::case1194() {}
void ParserTest::case1195() {}
void ParserTest::case1196() {}
void ParserTest::case1197() {}
void ParserTest::case1198() {}
void ParserTest::case1199() {}

void ParserTest::case1200()
{
    parseExpression(R"(
                    _Generic( x,
                             default: cbrt)
                    )");
}

void ParserTest::case1201()
{
    parseExpression(R"(
                    _Generic( x,
                             long double: cbrtl,
                             default: cbrt,
                             float: cbrtf)
                    )");
}

void ParserTest::case1202()
{
    parseExpression(R"(
                    _Generic( x,
                             float: cbrtf)
                    )");
}

void ParserTest::case1203()
{
    parseExpression(R"(
                    _Generic( ( x ),
                             long double: cbrtl,
                             default: cbrt)
                    )");
}

void ParserTest::case1204()
{
    parseExpression(R"(
                    _Generic( x float: cbrtf)
                    )",
                    Expectation().diagnostic(Expectation::ErrorOrWarn::Error,
                                                Parser::DiagnosticsReporter::ID_of_ExpectedToken));
}

void ParserTest::case1205()
{
    parseExpression("_Generic( x, float, cbrtf)",
                    Expectation().diagnostic(Expectation::ErrorOrWarn::Error,
                                                Parser::DiagnosticsReporter::ID_of_ExpectedToken));
}

void ParserTest::case1206()
{
    parseExpression("_Generic( x : cbrtf)",
                    Expectation().diagnostic(Expectation::ErrorOrWarn::Error,
                                                Parser::DiagnosticsReporter::ID_of_ExpectedToken));
}

void ParserTest::case1207()
{
    parseExpression("_Generic(float: cbrtf)",
                    Expectation().diagnostic(Expectation::ErrorOrWarn::Error,
                                                Parser::DiagnosticsReporter::ID_of_ExpectedFIRSTofExpression));
}

void ParserTest::case1208()
{
    parseExpression(R"(
                    _Generic( x,
                             int: y,
                             char*: z,
                             const char*: w)
                    )");
}

void ParserTest::case1209()
{
    parseExpression(R"(
                    _Generic( x,
                             default: cbrt
                    )",
                    Expectation().setErrorCnt(1));
}

void ParserTest::case1210()
{
    parseExpression(R"(
                    _Generic( x,
                             default:
                    )",
                    Expectation().setErrorCnt(1));
}

void ParserTest::case1211()
{
    parseExpression(R"(
                    _Generic( ( x ), int : 1,)
                    )",
                    Expectation().setErrorCnt(1));
}

void ParserTest::case1212() {}
void ParserTest::case1213() {}
void ParserTest::case1214() {}
void ParserTest::case1215() {}
void ParserTest::case1216() {}
void ParserTest::case1217() {}
void ParserTest::case1218() {}
void ParserTest::case1219() {}
void ParserTest::case1220() {}
void ParserTest::case1221() {}
void ParserTest::case1222() {}
void ParserTest::case1223() {}
void ParserTest::case1224() {}
void ParserTest::case1225() {}
void ParserTest::case1226() {}
void ParserTest::case1227() {}
void ParserTest::case1228() {}
void ParserTest::case1229() {}
void ParserTest::case1230() {}
void ParserTest::case1231() {}
void ParserTest::case1232() {}
void ParserTest::case1233() {}
void ParserTest::case1234() {}
void ParserTest::case1235() {}
void ParserTest::case1236() {}
void ParserTest::case1237() {}
void ParserTest::case1238() {}
void ParserTest::case1239() {}
void ParserTest::case1240() {}
void ParserTest::case1241() {}
void ParserTest::case1242() {}
void ParserTest::case1243() {}
void ParserTest::case1244() {}
void ParserTest::case1245() {}
void ParserTest::case1246() {}
void ParserTest::case1247() {}
void ParserTest::case1248() {}
void ParserTest::case1249() {}

void ParserTest::case1250()
{
    parseExpression("( struct x ) { y++ }",
                    Expectation().AST( { CompoundLiteralExpression,
                                         TypeName,
                                         StructTypeSpecifier,
                                         AbstractDeclarator,
                                         BraceEnclosedInitializer,
                                         ExpressionInitializer,
                                         PostIncrementExpression,
                                         IdentifierName }));
}

void ParserTest::case1251()
{
    parseExpression("( int ) { 1 }",
                    Expectation().AST( { CompoundLiteralExpression,
                                         TypeName,
                                         BuiltinTypeSpecifier,
                                         AbstractDeclarator,
                                         BraceEnclosedInitializer,
                                         ExpressionInitializer,
                                         IntegerConstantExpression }));
}

void ParserTest::case1252()
{
    parseExpression("( x ) { 1, 2 }",
                    Expectation().AST( { CompoundLiteralExpression,
                                         TypeName,
                                         TypedefName,
                                         AbstractDeclarator,
                                         BraceEnclosedInitializer,
                                         ExpressionInitializer,
                                         IntegerConstantExpression,
                                         ExpressionInitializer,
                                         IntegerConstantExpression }));
}

void ParserTest::case1253()
{
    parseExpression("( struct x ) {",
                    Expectation().diagnostic(Expectation::ErrorOrWarn::Error,
                                                Parser::DiagnosticsReporter::ID_of_ExpectedFIRSTofExpression));
}

void ParserTest::case1254()
{
    parseExpression("( struct x ) { 1,",
                    Expectation().diagnostic(Expectation::ErrorOrWarn::Error,
                                                Parser::DiagnosticsReporter::ID_of_ExpectedFIRSTofExpression));
}

void ParserTest::case1255()
{
    parseExpression("( struct x ) { 1",
                    Expectation().diagnostic(Expectation::ErrorOrWarn::Error,
                                                Parser::DiagnosticsReporter::ID_of_ExpectedToken));
}

void ParserTest::case1256()
{
    parseExpression("( long ) {",
                    Expectation().diagnostic(Expectation::ErrorOrWarn::Error,
                                                Parser::DiagnosticsReporter::ID_of_ExpectedFIRSTofExpression));
}

void ParserTest::case1257()
{
}

void ParserTest::case1258()
{
}

void ParserTest::case1259() {}
void ParserTest::case1260() {}
void ParserTest::case1261() {}
void ParserTest::case1262() {}
void ParserTest::case1263() {}
void ParserTest::case1264() {}
void ParserTest::case1265() {}
void ParserTest::case1266() {}
void ParserTest::case1267() {}
void ParserTest::case1268() {}
void ParserTest::case1269() {}
void ParserTest::case1270() {}
void ParserTest::case1271() {}
void ParserTest::case1272() {}
void ParserTest::case1273() {}
void ParserTest::case1274() {}
void ParserTest::case1275() {}
void ParserTest::case1276() {}
void ParserTest::case1277() {}
void ParserTest::case1278() {}
void ParserTest::case1279() {}
void ParserTest::case1280() {}
void ParserTest::case1281() {}
void ParserTest::case1282() {}
void ParserTest::case1283() {}
void ParserTest::case1284() {}
void ParserTest::case1285() {}
void ParserTest::case1286() {}
void ParserTest::case1287() {}
void ParserTest::case1288() {}
void ParserTest::case1289() {}
void ParserTest::case1290() {}
void ParserTest::case1291() {}
void ParserTest::case1292() {}
void ParserTest::case1293() {}
void ParserTest::case1294() {}
void ParserTest::case1295() {}
void ParserTest::case1296() {}
void ParserTest::case1297() {}
void ParserTest::case1298() {}
void ParserTest::case1299() {}

void ParserTest::case1300()
{
    parseExpression("++x",
                    Expectation().AST({ PreIncrementExpression,
                                        IdentifierName }));
}

void ParserTest::case1301()
{
    parseExpression("--x",
                    Expectation().AST({ PreDecrementExpression,
                                        IdentifierName }));;
}

void ParserTest::case1302()
{
    parseExpression("x++",
                    Expectation().AST({ PostIncrementExpression,
                                        IdentifierName }));
}

void ParserTest::case1303()
{
    parseExpression("x--",
                    Expectation().AST({ PostDecrementExpression,
                                        IdentifierName }));
}

void ParserTest::case1304()
{
    parseExpression("++( x )",
                    Expectation().AST({ PreIncrementExpression,
                                        ParenthesizedExpression,
                                        IdentifierName }));
}

void ParserTest::case1305()
{
    parseExpression("++( ( x ))",
                    Expectation().AST({ PreIncrementExpression,
                                        ParenthesizedExpression,
                                        ParenthesizedExpression,
                                        IdentifierName }));
}

void ParserTest::case1306()
{
    parseExpression("( x )++",
                    Expectation().AST({ PostIncrementExpression,
                                        ParenthesizedExpression,
                                        IdentifierName }));
}

void ParserTest::case1307()
{
    parseExpression("( ( x ))++",
                    Expectation().AST({ PostIncrementExpression,
                                        ParenthesizedExpression,
                                        ParenthesizedExpression,
                                        IdentifierName }));
}

void ParserTest::case1308()
{
    parseExpression("x+",
                    Expectation().diagnostic(Expectation::ErrorOrWarn::Error,
                                                Parser::DiagnosticsReporter::ID_of_ExpectedFIRSTofExpression));
}

void ParserTest::case1309() {}
void ParserTest::case1310() {}
void ParserTest::case1311() {}
void ParserTest::case1312() {}
void ParserTest::case1313() {}
void ParserTest::case1314() {}
void ParserTest::case1315() {}
void ParserTest::case1316() {}
void ParserTest::case1317() {}
void ParserTest::case1318() {}
void ParserTest::case1319() {}
void ParserTest::case1320() {}
void ParserTest::case1321() {}
void ParserTest::case1322() {}
void ParserTest::case1323() {}
void ParserTest::case1324() {}
void ParserTest::case1325() {}
void ParserTest::case1326() {}
void ParserTest::case1327() {}
void ParserTest::case1328() {}
void ParserTest::case1329() {}
void ParserTest::case1330() {}
void ParserTest::case1331() {}
void ParserTest::case1332() {}
void ParserTest::case1333() {}
void ParserTest::case1334() {}
void ParserTest::case1335() {}
void ParserTest::case1336() {}
void ParserTest::case1337() {}
void ParserTest::case1338() {}
void ParserTest::case1339() {}
void ParserTest::case1340() {}
void ParserTest::case1341() {}
void ParserTest::case1342() {}
void ParserTest::case1343() {}
void ParserTest::case1344() {}
void ParserTest::case1345() {}
void ParserTest::case1346() {}
void ParserTest::case1347() {}
void ParserTest::case1348() {}
void ParserTest::case1349() {}

void ParserTest::case1350()
{
    parseExpression("-1",
                    Expectation().AST( { UnaryMinusExpression,
                                         IntegerConstantExpression }));
}

void ParserTest::case1351()
{
    parseExpression("+1",
                    Expectation().AST( { UnaryPlusExpression,
                                         IntegerConstantExpression }));
}

void ParserTest::case1352()
{
    parseExpression("-1.2",
                    Expectation().AST( { UnaryMinusExpression,
                                         FloatingConstantExpression }));
}

void ParserTest::case1353()
{
    parseExpression("-.1",
                    Expectation().AST( { UnaryMinusExpression,
                                         FloatingConstantExpression }));
}

void ParserTest::case1354()
{
    parseExpression("-1.2e3",
                    Expectation().AST( { UnaryMinusExpression,
                                         FloatingConstantExpression }));
}

void ParserTest::case1355() {}
void ParserTest::case1356() {}
void ParserTest::case1357() {}
void ParserTest::case1358() {}
void ParserTest::case1359() {}
void ParserTest::case1360() {}
void ParserTest::case1361() {}
void ParserTest::case1362() {}
void ParserTest::case1363() {}
void ParserTest::case1364() {}
void ParserTest::case1365() {}
void ParserTest::case1366() {}
void ParserTest::case1367() {}
void ParserTest::case1368() {}
void ParserTest::case1369() {}
void ParserTest::case1370() {}
void ParserTest::case1371() {}
void ParserTest::case1372() {}
void ParserTest::case1373() {}
void ParserTest::case1374() {}
void ParserTest::case1375() {}
void ParserTest::case1376() {}
void ParserTest::case1377() {}
void ParserTest::case1378() {}
void ParserTest::case1379() {}
void ParserTest::case1380() {}
void ParserTest::case1381() {}
void ParserTest::case1382() {}
void ParserTest::case1383() {}
void ParserTest::case1384() {}
void ParserTest::case1385() {}
void ParserTest::case1386() {}
void ParserTest::case1387() {}
void ParserTest::case1388() {}
void ParserTest::case1389() {}
void ParserTest::case1390() {}
void ParserTest::case1391() {}
void ParserTest::case1392() {}
void ParserTest::case1393() {}
void ParserTest::case1394() {}
void ParserTest::case1395() {}
void ParserTest::case1396() {}
void ParserTest::case1397() {}
void ParserTest::case1398() {}
void ParserTest::case1399() {}

void ParserTest::case1400()
{
    parseExpression("sizeof x",
                    Expectation().AST( { SizeofExpression,
                                         ExpressionAsTypeReference,
                                         IdentifierName }));
}

void ParserTest::case1401()
{
    parseExpression("sizeof ( x )",
                    Expectation().AST( { SizeofExpression,
                                         AmbiguousTypeNameOrExpressionAsTypeReference,
                                         ExpressionAsTypeReference,
                                         ParenthesizedExpression,
                                         IdentifierName,
                                         TypeNameAsTypeReference,
                                         TypeName,
                                         TypedefName })
                                 .ambiguity("sizeof ( x ) ( x )"));
}

void ParserTest::case1402()
{
    parseExpression("sizeof ( const x )",
                    Expectation().AST( { SizeofExpression,
                                         TypeNameAsTypeReference,
                                         TypeName,
                                         ConstQualifier,
                                         TypedefName,
                                         AbstractDeclarator }));
}

void ParserTest::case1403()
{
    parseExpression("sizeof ( x * )",
                    Expectation().AST( { SizeofExpression,
                                         TypeNameAsTypeReference,
                                         TypeName,
                                         TypedefName,
                                         PointerDeclarator,
                                         AbstractDeclarator }));
}

void ParserTest::case1404()
{
    parseExpression("sizeof int",
                    Expectation().diagnostic(Expectation::ErrorOrWarn::Error,
                                                Parser::DiagnosticsReporter::ID_of_ExpectedFIRSTofExpression));
}

void ParserTest::case1405()
{
    parseExpression("sizeof * x",
                    Expectation().AST( { SizeofExpression,
                                         ExpressionAsTypeReference,
                                         PointerIndirectionExpression,
                                         IdentifierName }));
}

void ParserTest::case1406()
{
    parseExpression("sizeof x [ 0 ]",
                    Expectation().AST( { SizeofExpression,
                                         ExpressionAsTypeReference,
                                         ElementAccessExpression,
                                         IdentifierName,
                                         IntegerConstantExpression }));
}

void ParserTest::case1407()
{
    parseExpression("sizeof ( int )",
                    Expectation().AST( { SizeofExpression,
                                         TypeNameAsTypeReference,
                                         TypeName,
                                         BuiltinTypeSpecifier,
                                         AbstractDeclarator
                                       }));
}

void ParserTest::case1408()
{
    parseExpression("sizeof ( int * )",
                    Expectation().AST( { SizeofExpression,
                                         TypeNameAsTypeReference,
                                         TypeName,
                                         BuiltinTypeSpecifier,
                                         PointerDeclarator,
                                         AbstractDeclarator }));
}

void ParserTest::case1409()
{
    parseExpression("sizeof x . y",
                    Expectation().AST( { SizeofExpression,
                                         ExpressionAsTypeReference,
                                         DirectMemberAccessExpression,
                                         IdentifierName,
                                         IdentifierName }));
}

void ParserTest::case1410()
{
    parseExpression("sizeof ( x . y )");
}

void ParserTest::case1411()
{
    parseExpression("sizeof x [ 0 ]");
}

void ParserTest::case1412()
{
    parseExpression("sizeof ( x [ 0 ] )");
}

void ParserTest::case1413()
{
    parseExpression("sizeof ( x + 1 )");
}

void ParserTest::case1414()
{
    parseExpression("sizeof ( int ) { 1 , 2 } ",
                    Expectation().AST( { SizeofExpression,
                                         ExpressionAsTypeReference,
                                         CompoundLiteralExpression,
                                         TypeName,
                                         BuiltinTypeSpecifier,
                                         AbstractDeclarator,
                                         BraceEnclosedInitializer,
                                         ExpressionInitializer,
                                         IntegerConstantExpression,
                                         ExpressionInitializer,
                                         IntegerConstantExpression }));
}

void ParserTest::case1415()
{
    parseExpression("sizeof ( x ) { 1 , 2 } ",
                    Expectation().AST( { SizeofExpression,
                                         ExpressionAsTypeReference,
                                         CompoundLiteralExpression,
                                         TypeName,
                                         TypedefName,
                                         AbstractDeclarator,
                                         BraceEnclosedInitializer,
                                         ExpressionInitializer,
                                         IntegerConstantExpression,
                                         ExpressionInitializer,
                                         IntegerConstantExpression }));
}

void ParserTest::case1416()
{
    parseExpression("sizeof ( _Atomic ( int ) )");
}

void ParserTest::case1417()
{
    parseExpression("sizeof ( _Atomic ( x ) )");
}

void ParserTest::case1418() {}
void ParserTest::case1419() {}
void ParserTest::case1420() {}
void ParserTest::case1421() {}
void ParserTest::case1422() {}
void ParserTest::case1423() {}
void ParserTest::case1424() {}
void ParserTest::case1425() {}
void ParserTest::case1426() {}
void ParserTest::case1427() {}
void ParserTest::case1428() {}
void ParserTest::case1429() {}
void ParserTest::case1430() {}
void ParserTest::case1431() {}
void ParserTest::case1432() {}
void ParserTest::case1433() {}
void ParserTest::case1434() {}
void ParserTest::case1435() {}
void ParserTest::case1436() {}
void ParserTest::case1437() {}
void ParserTest::case1438() {}
void ParserTest::case1439() {}
void ParserTest::case1440() {}
void ParserTest::case1441() {}
void ParserTest::case1442() {}
void ParserTest::case1443() {}
void ParserTest::case1444() {}
void ParserTest::case1445() {}
void ParserTest::case1446() {}
void ParserTest::case1447() {}
void ParserTest::case1448() {}
void ParserTest::case1449() {}

void ParserTest::case1450()
{
    parseExpression("_Alignof ( x )",
                    Expectation().AST( { AlignofExpression,
                                         AmbiguousTypeNameOrExpressionAsTypeReference,
                                         ExpressionAsTypeReference,
                                         ParenthesizedExpression,
                                         IdentifierName,
                                         TypeNameAsTypeReference,
                                         TypeName,
                                         TypedefName })
                                 .ambiguity("_Alignof ( x ) ( x )"));
}

void ParserTest::case1451()
{
    parseExpression("_Alignof ( int )",
                    Expectation().AST( { AlignofExpression,
                                         TypeNameAsTypeReference,
                                         TypeName,
                                         BuiltinTypeSpecifier,
                                         AbstractDeclarator }));
}

void ParserTest::case1452()
{
    parseExpression("_Alignof ( int * )",
                    Expectation().AST( { AlignofExpression,
                                         TypeNameAsTypeReference,
                                         TypeName,
                                         BuiltinTypeSpecifier,
                                         PointerDeclarator,
                                         AbstractDeclarator }));
}

void ParserTest::case1453()
{
    // See LanguageExtensions.

    parseExpression("_Alignof x",
                    Expectation().AST( { AlignofExpression,
                                         ExpressionAsTypeReference,
                                         IdentifierName }));
}

void ParserTest::case1454()
{
}

void ParserTest::case1455()
{
}

void ParserTest::case1456()
{
}

void ParserTest::case1457()
{
}

void ParserTest::case1458() {}
void ParserTest::case1459() {}
void ParserTest::case1460() {}
void ParserTest::case1461() {}
void ParserTest::case1462() {}
void ParserTest::case1463() {}
void ParserTest::case1464() {}
void ParserTest::case1465() {}
void ParserTest::case1466() {}
void ParserTest::case1467() {}
void ParserTest::case1468() {}
void ParserTest::case1469() {}
void ParserTest::case1470() {}
void ParserTest::case1471() {}
void ParserTest::case1472() {}
void ParserTest::case1473() {}
void ParserTest::case1474() {}
void ParserTest::case1475() {}
void ParserTest::case1476() {}
void ParserTest::case1477() {}
void ParserTest::case1478() {}
void ParserTest::case1479() {}
void ParserTest::case1480() {}
void ParserTest::case1481() {}
void ParserTest::case1482() {}
void ParserTest::case1483() {}
void ParserTest::case1484() {}
void ParserTest::case1485() {}
void ParserTest::case1486() {}
void ParserTest::case1487() {}
void ParserTest::case1488() {}
void ParserTest::case1489() {}
void ParserTest::case1490() {}
void ParserTest::case1491() {}
void ParserTest::case1492() {}
void ParserTest::case1493() {}
void ParserTest::case1494() {}
void ParserTest::case1495() {}
void ParserTest::case1496() {}
void ParserTest::case1497() {}
void ParserTest::case1498() {}
void ParserTest::case1499() {}

void ParserTest::case1500()
{
    parseExpression("x . y",
                    Expectation().AST( { DirectMemberAccessExpression,
                                         IdentifierName,
                                         IdentifierName }));
}

void ParserTest::case1501()
{
    parseExpression("x .",
                    Expectation().diagnostic(Expectation::ErrorOrWarn::Error,
                                                Parser::DiagnosticsReporter::ID_of_ExpectedFieldName));
}

void ParserTest::case1502()
{
    parseExpression("x . 'y'",
                    Expectation().diagnostic(Expectation::ErrorOrWarn::Error,
                                                Parser::DiagnosticsReporter::ID_of_ExpectedFieldName));
}

void ParserTest::case1503()
{
    parseExpression("x ( ) . y",
                    Expectation().AST( { DirectMemberAccessExpression,
                                         CallExpression,
                                         IdentifierName,
                                         IdentifierName }));
}

void ParserTest::case1504()
{
    parseExpression("x -> y",
                    Expectation().AST( { IndirectMemberAccessExpression,
                                         IdentifierName,
                                         IdentifierName }));
}

void ParserTest::case1505()
{
    parseExpression("x -> ",
                    Expectation().diagnostic(Expectation::ErrorOrWarn::Error,
                                                Parser::DiagnosticsReporter::ID_of_ExpectedFieldName));
}

void ParserTest::case1506()
{
    parseExpression("x -> 'y'",
                    Expectation().diagnostic(Expectation::ErrorOrWarn::Error,
                                                Parser::DiagnosticsReporter::ID_of_ExpectedFieldName));
}

void ParserTest::case1507()
{
    parseExpression("x . y . z",
                    Expectation().AST( { DirectMemberAccessExpression,
                                         DirectMemberAccessExpression,
                                         IdentifierName,
                                         IdentifierName,
                                         IdentifierName }));
}

void ParserTest::case1508()
{
    parseExpression("x . y -> z",
                    Expectation().AST( { IndirectMemberAccessExpression,
                                         DirectMemberAccessExpression,
                                         IdentifierName,
                                         IdentifierName,
                                         IdentifierName }));
}

void ParserTest::case1509()
{
    parseExpression("x -> y -> z",
                    Expectation().AST( { IndirectMemberAccessExpression,
                                         IndirectMemberAccessExpression,
                                         IdentifierName,
                                         IdentifierName,
                                         IdentifierName }));
}

void ParserTest::case1510()
{
    parseExpression("( x . y ) -> z",
                    Expectation().AST( { IndirectMemberAccessExpression,
                                         ParenthesizedExpression,
                                         DirectMemberAccessExpression,
                                         IdentifierName,
                                         IdentifierName,
                                         IdentifierName }));
}

void ParserTest::case1511()
{
    parseExpression("x . y . z . w",
                    Expectation().AST( { DirectMemberAccessExpression,
                                         DirectMemberAccessExpression,
                                         DirectMemberAccessExpression,
                                         IdentifierName,
                                         IdentifierName,
                                         IdentifierName,
                                         IdentifierName }));
}

void ParserTest::case1512()
{
    parseExpression("x [ 0 ] . y",
                    Expectation().AST( { DirectMemberAccessExpression,
                                         ElementAccessExpression,
                                         IdentifierName,
                                         IntegerConstantExpression,
                                         IdentifierName }));

}

void ParserTest::case1513()
{
    parseExpression("( struct x ) { . y = 1 } . z",
                    Expectation().AST( { DirectMemberAccessExpression,
                                         CompoundLiteralExpression,
                                         TypeName,
                                         StructTypeSpecifier,
                                         AbstractDeclarator,
                                         BraceEnclosedInitializer,
                                         DesignatedInitializer,
                                         FieldDesignator,
                                         ExpressionInitializer,
                                         IntegerConstantExpression,
                                         IdentifierName }));

}

void ParserTest::case1514()
{
    parseExpression("( struct x ) { 1 } . z",
                    Expectation().AST( { DirectMemberAccessExpression,
                                         CompoundLiteralExpression,
                                         TypeName,
                                         StructTypeSpecifier,
                                         AbstractDeclarator,
                                         BraceEnclosedInitializer,
                                         ExpressionInitializer,
                                         IntegerConstantExpression,
                                         IdentifierName }));
}

void ParserTest::case1515() {}
void ParserTest::case1516() {}
void ParserTest::case1517() {}
void ParserTest::case1518() {}
void ParserTest::case1519() {}
void ParserTest::case1520() {}
void ParserTest::case1521() {}
void ParserTest::case1522() {}
void ParserTest::case1523() {}
void ParserTest::case1524() {}
void ParserTest::case1525() {}
void ParserTest::case1526() {}
void ParserTest::case1527() {}
void ParserTest::case1528() {}
void ParserTest::case1529() {}
void ParserTest::case1530() {}
void ParserTest::case1531() {}
void ParserTest::case1532() {}
void ParserTest::case1533() {}
void ParserTest::case1534() {}
void ParserTest::case1535() {}
void ParserTest::case1536() {}
void ParserTest::case1537() {}
void ParserTest::case1538() {}
void ParserTest::case1539() {}
void ParserTest::case1540() {}
void ParserTest::case1541() {}
void ParserTest::case1542() {}
void ParserTest::case1543() {}
void ParserTest::case1544() {}
void ParserTest::case1545() {}
void ParserTest::case1546() {}
void ParserTest::case1547() {}
void ParserTest::case1548() {}
void ParserTest::case1549() {}
void ParserTest::case1550() {}
void ParserTest::case1551() {}
void ParserTest::case1552() {}
void ParserTest::case1553() {}
void ParserTest::case1554() {}
void ParserTest::case1555() {}
void ParserTest::case1556() {}
void ParserTest::case1557() {}
void ParserTest::case1558() {}
void ParserTest::case1559() {}
void ParserTest::case1560() {}
void ParserTest::case1561() {}
void ParserTest::case1562() {}
void ParserTest::case1563() {}
void ParserTest::case1564() {}
void ParserTest::case1565() {}
void ParserTest::case1566() {}
void ParserTest::case1567() {}
void ParserTest::case1568() {}
void ParserTest::case1569() {}
void ParserTest::case1570() {}
void ParserTest::case1571() {}
void ParserTest::case1572() {}
void ParserTest::case1573() {}
void ParserTest::case1574() {}
void ParserTest::case1575() {}
void ParserTest::case1576() {}
void ParserTest::case1577() {}
void ParserTest::case1578() {}
void ParserTest::case1579() {}
void ParserTest::case1580() {}
void ParserTest::case1581() {}
void ParserTest::case1582() {}
void ParserTest::case1583() {}
void ParserTest::case1584() {}
void ParserTest::case1585() {}
void ParserTest::case1586() {}
void ParserTest::case1587() {}
void ParserTest::case1588() {}
void ParserTest::case1589() {}
void ParserTest::case1590() {}
void ParserTest::case1591() {}
void ParserTest::case1592() {}
void ParserTest::case1593() {}
void ParserTest::case1594() {}
void ParserTest::case1595() {}
void ParserTest::case1596() {}
void ParserTest::case1597() {}
void ParserTest::case1598() {}
void ParserTest::case1599() {}

void ParserTest::case1600()
{
    parseExpression("x ( )",
                    Expectation().AST( { CallExpression,
                                         IdentifierName }));
}

void ParserTest::case1601()
{
    parseExpression("( x ) ( )",
                    Expectation().AST( { CallExpression,
                                         ParenthesizedExpression,
                                         IdentifierName }));
}

void ParserTest::case1602()
{
    parseExpression("x ( 1 )",
                    Expectation().AST( { CallExpression,
                                         IdentifierName,
                                         IntegerConstantExpression }));
}

void ParserTest::case1603()
{
    parseExpression("x ( y )",
                    Expectation().AST( { CallExpression,
                                         IdentifierName,
                                         IdentifierName }));
}

void ParserTest::case1604()
{
    parseExpression("x ( y , 1 )",
                    Expectation().AST( { CallExpression,
                                         IdentifierName,
                                         IdentifierName,
                                         IntegerConstantExpression }));
}

void ParserTest::case1605()
{
    parseExpression("( & x ) ( )",
                    Expectation().AST( { CallExpression,
                                         ParenthesizedExpression,
                                         AddressOfExpression,
                                         IdentifierName }));
}

void ParserTest::case1606()
{
    parseExpression("( * x ) ( )",
                    Expectation().AST( { CallExpression,
                                         ParenthesizedExpression,
                                         PointerIndirectionExpression,
                                         IdentifierName }));
}

void ParserTest::case1607()
{
    parseExpression("( * * x ) ( )",
                    Expectation().AST( { CallExpression,
                                         ParenthesizedExpression,
                                         PointerIndirectionExpression,
                                         PointerIndirectionExpression,
                                         IdentifierName }));
}

void ParserTest::case1608()
{
    parseExpression("x ( y ( ), \"z\" )",
                    Expectation().AST( { CallExpression,
                                         IdentifierName,
                                         CallExpression,
                                         IdentifierName,
                                         StringLiteralExpression }));
}

void ParserTest::case1609()
{
    parseExpression("x ( y",
                    Expectation().diagnostic(Expectation::ErrorOrWarn::Error,
                                                Parser::DiagnosticsReporter::ID_of_ExpectedToken));
}

void ParserTest::case1610()
{
    parseExpression("x (",
                    Expectation().diagnostic(Expectation::ErrorOrWarn::Error,
                                                Parser::DiagnosticsReporter::ID_of_ExpectedFIRSTofExpression));
}

void ParserTest::case1611()
{
    parseExpression("x ( y , )",
                    Expectation().diagnostic(Expectation::ErrorOrWarn::Error,
                                                Parser::DiagnosticsReporter::ID_of_ExpectedFIRSTofExpression));
}

void ParserTest::case1612()
{
    parseExpression("x ( ( 1 ) )",
                    Expectation().AST( { CallExpression,
                                         IdentifierName,
                                         ParenthesizedExpression,
                                         IntegerConstantExpression }));
}

void ParserTest::case1613()
{
    parseExpression("x ( ( ( y ) ) )",
                    Expectation().AST( { CallExpression,
                                         IdentifierName,
                                         ParenthesizedExpression,
                                         ParenthesizedExpression,
                                         IdentifierName }));
}

void ParserTest::case1614()
{
    parseExpression("( x ( ) )",
                    Expectation().AST( { ParenthesizedExpression,
                                         CallExpression,
                                         IdentifierName }));
}

void ParserTest::case1615()
{
    parseExpression("( ( x ( ) ) )",
                    Expectation().AST( { ParenthesizedExpression,
                                         ParenthesizedExpression,
                                         CallExpression,
                                         IdentifierName }));
}

void ParserTest::case1616()
{
    parseExpression("( x ( 1 ) )",
                    Expectation().AST( { ParenthesizedExpression,
                                         CallExpression,
                                         IdentifierName,
                                         IntegerConstantExpression }));
}

void ParserTest::case1617()
{
    parseExpression("( x ( 1 , y ) )",
                    Expectation().AST( { ParenthesizedExpression,
                                         CallExpression,
                                         IdentifierName,
                                         IntegerConstantExpression,
                                         IdentifierName }));
}

void ParserTest::case1618() {}
void ParserTest::case1619() {}
void ParserTest::case1620() {}
void ParserTest::case1621() {}
void ParserTest::case1622() {}
void ParserTest::case1623() {}
void ParserTest::case1624() {}
void ParserTest::case1625() {}
void ParserTest::case1626() {}
void ParserTest::case1627() {}
void ParserTest::case1628() {}
void ParserTest::case1629() {}
void ParserTest::case1630() {}
void ParserTest::case1631() {}
void ParserTest::case1632() {}
void ParserTest::case1633() {}
void ParserTest::case1634() {}
void ParserTest::case1635() {}
void ParserTest::case1636() {}
void ParserTest::case1637() {}
void ParserTest::case1638() {}
void ParserTest::case1639() {}
void ParserTest::case1640() {}
void ParserTest::case1641() {}
void ParserTest::case1642() {}
void ParserTest::case1643() {}
void ParserTest::case1644() {}
void ParserTest::case1645() {}
void ParserTest::case1646() {}
void ParserTest::case1647() {}
void ParserTest::case1648() {}
void ParserTest::case1649() {}

void ParserTest::case1650()
{
    parseExpression("* x",
                    Expectation().AST( { PointerIndirectionExpression,
                                         IdentifierName }));
}

void ParserTest::case1651()
{
    parseExpression("* ( x )",
                    Expectation().AST( { PointerIndirectionExpression,
                                         ParenthesizedExpression,
                                         IdentifierName }));
}

void ParserTest::case1652()
{
    parseExpression("& x",
                    Expectation().AST( { AddressOfExpression,
                                         IdentifierName }));
}

void ParserTest::case1653()
{
    // Syntax OK, semantics invalid.

    CROSS_REFERENCE_TEST(BinderTest::case0101);

    parseExpression("* 1",
                    Expectation().AST( { PointerIndirectionExpression,
                                         IntegerConstantExpression }));
}

void ParserTest::case1654()
{
    // Syntax OK, semantics invalid.

    CROSS_REFERENCE_TEST(BinderTest::case0102);

    parseExpression("& 1",
                    Expectation().AST( { AddressOfExpression,
                                         IntegerConstantExpression }));
}

void ParserTest::case1655() {}
void ParserTest::case1656() {}
void ParserTest::case1657() {}
void ParserTest::case1658() {}
void ParserTest::case1659() {}
void ParserTest::case1660() {}
void ParserTest::case1661() {}
void ParserTest::case1662() {}
void ParserTest::case1663() {}
void ParserTest::case1664() {}
void ParserTest::case1665() {}
void ParserTest::case1666() {}
void ParserTest::case1667() {}
void ParserTest::case1668() {}
void ParserTest::case1669() {}
void ParserTest::case1670() {}
void ParserTest::case1671() {}
void ParserTest::case1672() {}
void ParserTest::case1673() {}
void ParserTest::case1674() {}
void ParserTest::case1675() {}
void ParserTest::case1676() {}
void ParserTest::case1677() {}
void ParserTest::case1678() {}
void ParserTest::case1679() {}
void ParserTest::case1680() {}
void ParserTest::case1681() {}
void ParserTest::case1682() {}
void ParserTest::case1683() {}
void ParserTest::case1684() {}
void ParserTest::case1685() {}
void ParserTest::case1686() {}
void ParserTest::case1687() {}
void ParserTest::case1688() {}
void ParserTest::case1689() {}
void ParserTest::case1690() {}
void ParserTest::case1691() {}
void ParserTest::case1692() {}
void ParserTest::case1693() {}
void ParserTest::case1694() {}
void ParserTest::case1695() {}
void ParserTest::case1696() {}
void ParserTest::case1697() {}
void ParserTest::case1698() {}
void ParserTest::case1699() {}

void ParserTest::case1700()
{
    parseExpression("( int ) x",
                    Expectation().AST({ CastExpression,
                                        TypeName,
                                        BuiltinTypeSpecifier,
                                        AbstractDeclarator,
                                        IdentifierName }));
}

void ParserTest::case1701()
{
    parseExpression("( int * ) x",
                    Expectation().AST({ CastExpression,
                                        TypeName,
                                        BuiltinTypeSpecifier,
                                        PointerDeclarator,
                                        AbstractDeclarator,
                                        IdentifierName }));
}

void ParserTest::case1702()
{
    parseExpression("( int const * ) x",
                    Expectation().AST({ CastExpression,
                                        TypeName,
                                        BuiltinTypeSpecifier,
                                        ConstQualifier,
                                        PointerDeclarator,
                                        AbstractDeclarator,
                                        IdentifierName }));
}

void ParserTest::case1703()
{
    parseExpression("( struct x ) y",
                    Expectation().AST({ CastExpression,
                                        TypeName,
                                        StructTypeSpecifier,
                                        AbstractDeclarator,
                                        IdentifierName }));
}

void ParserTest::case1704()
{
    parseExpression("( unsigned long ) + x",
                    Expectation().AST({ CastExpression,
                                        TypeName,
                                        BuiltinTypeSpecifier,
                                        BuiltinTypeSpecifier,
                                        AbstractDeclarator,
                                        UnaryPlusExpression,
                                        IdentifierName }));
}

void ParserTest::case1705()
{
    parseExpression("( unsigned long ) - x",
                    Expectation().AST({ CastExpression,
                                        TypeName,
                                        BuiltinTypeSpecifier,
                                        BuiltinTypeSpecifier,
                                        AbstractDeclarator,
                                        UnaryMinusExpression,
                                        IdentifierName }));
}

void ParserTest::case1706()
{
    parseExpression("( int ) ++ x",
                    Expectation().AST({ CastExpression,
                                        TypeName,
                                        BuiltinTypeSpecifier,
                                        AbstractDeclarator,
                                        PreIncrementExpression,
                                        IdentifierName }));
}

void ParserTest::case1707()
{
    parseExpression("( int ) x ++",
                    Expectation().AST({ CastExpression,
                                        TypeName,
                                        BuiltinTypeSpecifier,
                                        AbstractDeclarator,
                                        PostIncrementExpression,
                                        IdentifierName }));
}

void ParserTest::case1708()
{
    parseExpression("( int ) _Generic ( y",
                    Expectation().setErrorCnt(1));
}

void ParserTest::case1709()
{
    parseExpression("( long ) int",
                    Expectation().setErrorCnt(1));
}

void ParserTest::case1710()
{
    parseExpression("( struct x ) -",
                    Expectation().setErrorCnt(1));
}

void ParserTest::case1711()
{
    parseExpression("( int ) -",
                    Expectation().setErrorCnt(1));
}

void ParserTest::case1712()
{
    parseExpression("( int ) x +",
                    Expectation().setErrorCnt(1));
}

void ParserTest::case1713()
{
    parseExpression("( int ) x ( y",
                    Expectation().setErrorCnt(1));
}

void ParserTest::case1714()
{
    parseExpression("( x ) y ( z )",
                    Expectation().AST({ CastExpression,
                                        TypeName,
                                        TypedefName,
                                        AbstractDeclarator,
                                        CallExpression,
                                        IdentifierName,
                                        IdentifierName }));
}

void ParserTest::case1715()
{
    parseExpression("( x ) ( y ( z ) )",
                    Expectation().AST({ CastExpression,
                                        TypeName,
                                        TypedefName,
                                        AbstractDeclarator,
                                        ParenthesizedExpression,
                                        CallExpression,
                                        IdentifierName,
                                        IdentifierName }));
}

void ParserTest::case1716()
{
    parseExpression("( int ) - 1",
                    Expectation().AST( { CastExpression,
                                         TypeName,
                                         BuiltinTypeSpecifier,
                                         AbstractDeclarator,
                                         UnaryMinusExpression,
                                         IntegerConstantExpression }));
}

void ParserTest::case1717()
{
    parseExpression("( x ) - 1",
                    Expectation().ambiguity()
                                 .AST( { AmbiguousCastOrBinaryExpression,
                                         CastExpression,
                                         TypeName,
                                         TypedefName,
                                         AbstractDeclarator,
                                         UnaryMinusExpression,
                                         IntegerConstantExpression,
                                         SubstractExpression,
                                         ParenthesizedExpression,
                                         IdentifierName,
                                         IntegerConstantExpression }));
}

void ParserTest::case1718()
{
    parseExpression("( x ) + y",
                    Expectation().ambiguity()
                                 .AST( { AmbiguousCastOrBinaryExpression,
                                         CastExpression,
                                         TypeName,
                                         TypedefName,
                                         AbstractDeclarator,
                                         UnaryPlusExpression,
                                         IdentifierName,
                                         AddExpression,
                                         ParenthesizedExpression,
                                         IdentifierName,
                                         IdentifierName }));
}

void ParserTest::case1719()
{
    parseExpression("( const x ) - 1",
                    Expectation().AST( { CastExpression,
                                         TypeName,
                                         ConstQualifier,
                                         TypedefName,
                                         AbstractDeclarator,
                                         UnaryMinusExpression,
                                         IntegerConstantExpression }));
}

void ParserTest::case1720()
{
    parseExpression("( x * ) - 1",
                    Expectation().AST( { CastExpression,
                                         TypeName,
                                         TypedefName,
                                         PointerDeclarator,
                                         AbstractDeclarator,
                                         UnaryMinusExpression,
                                         IntegerConstantExpression }));
}
void ParserTest::case1721() {}
void ParserTest::case1722() {}
void ParserTest::case1723() {}
void ParserTest::case1724() {}
void ParserTest::case1725() {}
void ParserTest::case1726() {}
void ParserTest::case1727() {}
void ParserTest::case1728() {}
void ParserTest::case1729() {}
void ParserTest::case1730() {}
void ParserTest::case1731() {}
void ParserTest::case1732() {}
void ParserTest::case1733() {}
void ParserTest::case1734() {}
void ParserTest::case1735() {}
void ParserTest::case1736() {}
void ParserTest::case1737() {}
void ParserTest::case1738() {}
void ParserTest::case1739() {}
void ParserTest::case1740() {}
void ParserTest::case1741() {}
void ParserTest::case1742() {}
void ParserTest::case1743() {}
void ParserTest::case1744() {}
void ParserTest::case1745() {}
void ParserTest::case1746() {}
void ParserTest::case1747() {}
void ParserTest::case1748() {}
void ParserTest::case1749() {}

void ParserTest::case1750()
{
    parseExpression("x ? y : z",
                    Expectation().AST( { ConditionalExpression,
                                         IdentifierName,
                                         IdentifierName,
                                         IdentifierName } ));

}

void ParserTest::case1751()
{
    parseExpression("x ? y + z : w",
                    Expectation().AST( { ConditionalExpression,
                                         IdentifierName,
                                         AddExpression,
                                         IdentifierName,
                                         IdentifierName,
                                         IdentifierName } ));
}

void ParserTest::case1752()
{
    // See ExtGNU_Conditionals in LanguageExtensions.

    parseExpression("x ?: z",
                    Expectation().AST( { ConditionalExpression,
                                         IdentifierName,
                                         IdentifierName } ));
}

void ParserTest::case1753()
{
}

void ParserTest::case1754() {}
void ParserTest::case1755() {}
void ParserTest::case1756() {}
void ParserTest::case1757() {}
void ParserTest::case1758() {}
void ParserTest::case1759() {}
void ParserTest::case1760() {}
void ParserTest::case1761() {}
void ParserTest::case1762() {}
void ParserTest::case1763() {}
void ParserTest::case1764() {}
void ParserTest::case1765() {}
void ParserTest::case1766() {}
void ParserTest::case1767() {}
void ParserTest::case1768() {}
void ParserTest::case1769() {}
void ParserTest::case1770() {}
void ParserTest::case1771() {}
void ParserTest::case1772() {}
void ParserTest::case1773() {}
void ParserTest::case1774() {}
void ParserTest::case1775() {}
void ParserTest::case1776() {}
void ParserTest::case1777() {}
void ParserTest::case1778() {}
void ParserTest::case1779() {}
void ParserTest::case1780() {}
void ParserTest::case1781() {}
void ParserTest::case1782() {}
void ParserTest::case1783() {}
void ParserTest::case1784() {}
void ParserTest::case1785() {}
void ParserTest::case1786() {}
void ParserTest::case1787() {}
void ParserTest::case1788() {}
void ParserTest::case1789() {}
void ParserTest::case1790() {}
void ParserTest::case1791() {}
void ParserTest::case1792() {}
void ParserTest::case1793() {}
void ParserTest::case1794() {}
void ParserTest::case1795() {}
void ParserTest::case1796() {}
void ParserTest::case1797() {}
void ParserTest::case1798() {}
void ParserTest::case1799() {}

void ParserTest::case1800()
{
    parseExpression("x / y",
                    Expectation().AST( { DivideExpression,
                                         IdentifierName,
                                         IdentifierName } ));
}

void ParserTest::case1801()
{
    parseExpression("x << y",
                    Expectation().AST( { LeftShiftExpression,
                                         IdentifierName,
                                         IdentifierName } ));

}

void ParserTest::case1802()
{
    parseExpression("x + y",
                    Expectation().AST( { AddExpression,
                                         IdentifierName,
                                         IdentifierName } ));

}

void ParserTest::case1803()
{
    parseExpression("x || y",
                    Expectation().AST( { LogicalORExpression,
                                         IdentifierName,
                                         IdentifierName } ));
}

void ParserTest::case1804()
{
    // No ambiguity here, this is an expression (not an statement).

    parseExpression("x * y",
                    Expectation().AST( { MultiplyExpression,
                                         IdentifierName,
                                         IdentifierName } ));

}

void ParserTest::case1805()
{
    parseExpression("( x * y ) + z",
                    Expectation().AST( { AddExpression,
                                         ParenthesizedExpression,
                                         MultiplyExpression,
                                         IdentifierName,
                                         IdentifierName,
                                         IdentifierName }));
}

void ParserTest::case1806()
{
    parseExpression("x * ( y + z)",
                    Expectation().AST( { MultiplyExpression,
                                         IdentifierName,
                                         ParenthesizedExpression,
                                         AddExpression,
                                         IdentifierName,
                                         IdentifierName }));
}

void ParserTest::case1807()
{
    parseExpression("x + y * z",
                    Expectation().AST( { AddExpression,
                                         IdentifierName,
                                         MultiplyExpression,
                                         IdentifierName,
                                         IdentifierName }));
}

void ParserTest::case1808()
{
    parseExpression("x * y + z",
                    Expectation().AST( { AddExpression,
                                         MultiplyExpression,
                                         IdentifierName,
                                         IdentifierName,
                                         IdentifierName }));
}

void ParserTest::case1809()
{
    parseExpression("x , y",
                    Expectation().AST( { SequencingExpression,
                                         IdentifierName,
                                         IdentifierName }));
}

void ParserTest::case1810()
{
    parseExpression("x , 1, y",
                    Expectation().AST( { SequencingExpression,
                                         SequencingExpression,
                                         IdentifierName,
                                         IntegerConstantExpression,
                                         IdentifierName }));
}

void ParserTest::case1811()
{
}
void ParserTest::case1812() {}
void ParserTest::case1813() {}
void ParserTest::case1814() {}
void ParserTest::case1815() {}
void ParserTest::case1816() {}
void ParserTest::case1817() {}
void ParserTest::case1818() {}
void ParserTest::case1819() {}
void ParserTest::case1820() {}
void ParserTest::case1821() {}
void ParserTest::case1822() {}
void ParserTest::case1823() {}
void ParserTest::case1824() {}
void ParserTest::case1825() {}
void ParserTest::case1826() {}
void ParserTest::case1827() {}
void ParserTest::case1828() {}
void ParserTest::case1829() {}
void ParserTest::case1830() {}
void ParserTest::case1831() {}
void ParserTest::case1832() {}
void ParserTest::case1833() {}
void ParserTest::case1834() {}
void ParserTest::case1835() {}
void ParserTest::case1836() {}
void ParserTest::case1837() {}
void ParserTest::case1838() {}
void ParserTest::case1839() {}
void ParserTest::case1840() {}
void ParserTest::case1841() {}
void ParserTest::case1842() {}
void ParserTest::case1843() {}
void ParserTest::case1844() {}
void ParserTest::case1845() {}
void ParserTest::case1846() {}
void ParserTest::case1847() {}
void ParserTest::case1848() {}
void ParserTest::case1849() {}

void ParserTest::case1850()
{
    parseExpression("x = y",
                    Expectation().AST( { BasicAssignmentExpression,
                                         IdentifierName,
                                         IdentifierName } ));
}

void ParserTest::case1851()
{
    parseExpression("x %= y",
                    Expectation().AST( { ModuloAssignmentExpression,
                                         IdentifierName,
                                         IdentifierName } ));
}

void ParserTest::case1852()
{
    parseExpression("x = y = 1",
                    Expectation().AST( { BasicAssignmentExpression,
                                         IdentifierName,
                                         BasicAssignmentExpression,
                                         IdentifierName,
                                         IntegerConstantExpression } ));

}

void ParserTest::case1853()
{
    parseExpression("x = ( y = 1)",
                    Expectation().AST( { BasicAssignmentExpression,
                                         IdentifierName,
                                         ParenthesizedExpression,
                                         BasicAssignmentExpression,
                                         IdentifierName,
                                         IntegerConstantExpression } ));
}

void ParserTest::case1854()
{
    parseExpression("x = 0",
                    Expectation().AST( { BasicAssignmentExpression,
                                         IdentifierName,
                                         IntegerConstantExpression } ));
}

void ParserTest::case1855()
{
    parseExpression("x = ( void * ) 0",
                    Expectation().AST( { BasicAssignmentExpression,
                                         IdentifierName,
                                         CastExpression,
                                         TypeName,
                                         BuiltinTypeSpecifier,
                                         PointerDeclarator,
                                         AbstractDeclarator,
                                         IntegerConstantExpression} ));
}

void ParserTest::case1856()
{
    parseExpression("x = ( ( void * ) 0 )",
                    Expectation().AST( { BasicAssignmentExpression,
                                         IdentifierName,
                                         ParenthesizedExpression,
                                         CastExpression,
                                         TypeName,
                                         BuiltinTypeSpecifier,
                                         PointerDeclarator,
                                         AbstractDeclarator,
                                         IntegerConstantExpression} ));
}

void ParserTest::case1857() {}
void ParserTest::case1858() {}
void ParserTest::case1859() {}
void ParserTest::case1860() {}
void ParserTest::case1861() {}
void ParserTest::case1862() {}
void ParserTest::case1863() {}
void ParserTest::case1864() {}
void ParserTest::case1865() {}
void ParserTest::case1866() {}
void ParserTest::case1867() {}
void ParserTest::case1868() {}
void ParserTest::case1869() {}
void ParserTest::case1870() {}
void ParserTest::case1871() {}
void ParserTest::case1872() {}
void ParserTest::case1873() {}
void ParserTest::case1874() {}
void ParserTest::case1875() {}
void ParserTest::case1876() {}
void ParserTest::case1877() {}
void ParserTest::case1878() {}
void ParserTest::case1879() {}
void ParserTest::case1880() {}
void ParserTest::case1881() {}
void ParserTest::case1882() {}
void ParserTest::case1883() {}
void ParserTest::case1884() {}
void ParserTest::case1885() {}
void ParserTest::case1886() {}
void ParserTest::case1887() {}
void ParserTest::case1888() {}
void ParserTest::case1889() {}
void ParserTest::case1890() {}
void ParserTest::case1891() {}
void ParserTest::case1892() {}
void ParserTest::case1893() {}
void ParserTest::case1894() {}
void ParserTest::case1895() {}
void ParserTest::case1896() {}
void ParserTest::case1897() {}
void ParserTest::case1898() {}
void ParserTest::case1899() {}

void ParserTest::case1900()
{
    // https://gcc.gnu.org/onlinedocs/gcc/Statement-Exprs.html

    parseExpression("({ int x; })");
}

void ParserTest::case1901()
{
    // https://gcc.gnu.org/onlinedocs/gcc/Statement-Exprs.html

    parseExpression("({ int x; x; })");
}

void ParserTest::case1902()
{
    // https://gcc.gnu.org/onlinedocs/gcc/Statement-Exprs.html

    parseExpression("({ x y; })");
}

void ParserTest::case1903()
{
    // https://gcc.gnu.org/onlinedocs/gcc/Statement-Exprs.html

    parseExpression("({ x y; y; })");
}

void ParserTest::case1904() {}
void ParserTest::case1905() {}
void ParserTest::case1906() {}
void ParserTest::case1907() {}
void ParserTest::case1908() {}
void ParserTest::case1909() {}
void ParserTest::case1910() {}
void ParserTest::case1911() {}
void ParserTest::case1912() {}
void ParserTest::case1913() {}
void ParserTest::case1914() {}
void ParserTest::case1915() {}
void ParserTest::case1916() {}
void ParserTest::case1917() {}
void ParserTest::case1918() {}
void ParserTest::case1919() {}
void ParserTest::case1920() {}
void ParserTest::case1921() {}
void ParserTest::case1922() {}
void ParserTest::case1923() {}
void ParserTest::case1924() {}
void ParserTest::case1925() {}
void ParserTest::case1926() {}
void ParserTest::case1927() {}
void ParserTest::case1928() {}
void ParserTest::case1929() {}
void ParserTest::case1930() {}
void ParserTest::case1931() {}
void ParserTest::case1932() {}
void ParserTest::case1933() {}
void ParserTest::case1934() {}
void ParserTest::case1935() {}
void ParserTest::case1936() {}
void ParserTest::case1937() {}
void ParserTest::case1938() {}
void ParserTest::case1939() {}
void ParserTest::case1940() {}
void ParserTest::case1941() {}
void ParserTest::case1942() {}
void ParserTest::case1943() {}
void ParserTest::case1944() {}
void ParserTest::case1945() {}
void ParserTest::case1946() {}
void ParserTest::case1947() {}
void ParserTest::case1948() {}
void ParserTest::case1949() {}

void ParserTest::case1950()
{
    parseExpression("__builtin_va_arg ( x , int )",
                    Expectation().AST( { VAArgumentExpression,
                                         IdentifierName,
                                         TypeName,
                                         BuiltinTypeSpecifier,
                                         AbstractDeclarator }));
}

void ParserTest::case1951()
{
    parseExpression("__builtin_va_arg ( x , y )",
                    Expectation().AST( { VAArgumentExpression,
                                         IdentifierName,
                                         TypeName,
                                         TypedefName,
                                         AbstractDeclarator }));
}

void ParserTest::case1952() {}
void ParserTest::case1953() {}
void ParserTest::case1954() {}
void ParserTest::case1955() {}
void ParserTest::case1956() {}
void ParserTest::case1957() {}
void ParserTest::case1958() {}
void ParserTest::case1959() {}

void ParserTest::case1960()
{
    parseExpression("__builtin_offsetof ( struct x , y )",
                    Expectation().AST( { OffsetOfExpression,
                                         TypeName,
                                         StructTypeSpecifier,
                                         AbstractDeclarator,
                                         OffsetOfDesignator }));
}

void ParserTest::case1961()
{
    parseExpression("__builtin_offsetof ( struct x , y . z )",
                    Expectation().AST( { OffsetOfExpression,
                                         TypeName,
                                         StructTypeSpecifier,
                                         AbstractDeclarator,
                                         OffsetOfDesignator,
                                         FieldDesignator }));
}

void ParserTest::case1962()
{
    parseExpression("__builtin_offsetof ( struct x , y [ 0 ] )",
                    Expectation().AST( { OffsetOfExpression,
                                         TypeName,
                                         StructTypeSpecifier,
                                         AbstractDeclarator,
                                         OffsetOfDesignator,
                                         ArrayDesignator,
                                         IntegerConstantExpression }));
}

void ParserTest::case1963()
{
    parseExpression("__builtin_offsetof ( struct x , y  [ 0 ] . z )",
                    Expectation().AST( { OffsetOfExpression,
                                         TypeName,
                                         StructTypeSpecifier,
                                         AbstractDeclarator,
                                         OffsetOfDesignator,
                                         ArrayDesignator,
                                         IntegerConstantExpression,
                                         FieldDesignator }));
}

void ParserTest::case1964()
{
    parseExpression("__builtin_offsetof ( union x , y )",
                    Expectation().AST( { OffsetOfExpression,
                                         TypeName,
                                         UnionTypeSpecifier,
                                         AbstractDeclarator,
                                         OffsetOfDesignator }));
}

void ParserTest::case1965() {}
void ParserTest::case1966() {}
void ParserTest::case1967() {}
void ParserTest::case1968() {}
void ParserTest::case1969() {}

void ParserTest::case1970()
{
    parseExpression("__builtin_choose_expr ( 0 , x , y )",
                    Expectation().AST( { ExtGNU_ChooseExpression,
                                         IntegerConstantExpression,
                                         IdentifierName,
                                         IdentifierName }));
}

void ParserTest::case1971()
{
    parseExpression("__builtin_choose_expr ( 1 , sizeof ( int ) , sizeof ( double ) )",
                    Expectation().AST( { ExtGNU_ChooseExpression,
                                         IntegerConstantExpression,
                                         SizeofExpression,
                                         TypeNameAsTypeReference,
                                         TypeName,
                                         BuiltinTypeSpecifier,
                                         AbstractDeclarator,
                                         SizeofExpression,
                                         TypeNameAsTypeReference,
                                         TypeName,
                                         BuiltinTypeSpecifier,
                                         AbstractDeclarator }));
}

void ParserTest::case1972()
{
    parseExpression(R"(
                     __builtin_choose_expr ( 0 ,
                             __builtin_choose_expr ( 1 ,
                                    sizeof ( int ) ,
                                    sizeof ( double ) ) ,
                             __builtin_choose_expr ( 2 , x , y ) )
                    )",
                    Expectation().AST( { ExtGNU_ChooseExpression,
                                         IntegerConstantExpression,
                                         ExtGNU_ChooseExpression,
                                         IntegerConstantExpression,
                                         SizeofExpression,
                                         TypeNameAsTypeReference,
                                         TypeName,
                                         BuiltinTypeSpecifier,
                                         AbstractDeclarator,
                                         SizeofExpression,
                                         TypeNameAsTypeReference,
                                         TypeName,
                                         BuiltinTypeSpecifier,
                                         AbstractDeclarator,
                                         ExtGNU_ChooseExpression,
                                         IntegerConstantExpression,
                                         IdentifierName,
                                         IdentifierName }));
}

void ParserTest::case1973()
{
    parseExpression("__builtin_choose_expr ( 1 , x + y , z + w )",
                    Expectation().AST( { ExtGNU_ChooseExpression,
                                         IntegerConstantExpression,
                                         AddExpression,
                                         IdentifierName,
                                         IdentifierName,
                                         AddExpression,
                                         IdentifierName,
                                         IdentifierName }));
}

void ParserTest::case1974() {}
void ParserTest::case1975() {}
void ParserTest::case1976() {}
void ParserTest::case1977() {}
void ParserTest::case1978() {}
void ParserTest::case1979() {}

void ParserTest::case1980()
{
    parseExpression("__FUNCTION__",
                    Expectation().AST( { PredefinedName }));
}

void ParserTest::case1981()
{
    parseExpression("__PRETTY_FUNCTION__",
                    Expectation().AST( { PredefinedName }));
}

void ParserTest::case1982()
{
    parseExpression("sizeof ( __FUNCTION__ )",
                    Expectation().AST( { SizeofExpression,
                                         ExpressionAsTypeReference,
                                         ParenthesizedExpression,
                                         PredefinedName }));
}

void ParserTest::case1983()
{
    parseExpression("sizeof ( __PRETTY_FUNCTION__ )",
                    Expectation().AST( { SizeofExpression,
                                         ExpressionAsTypeReference,
                                         ParenthesizedExpression,
                                         PredefinedName}));
}

void ParserTest::case1984()
{
    parseExpression("sizeof __FUNCTION__",
                    Expectation().AST( { SizeofExpression,
                                         ExpressionAsTypeReference,
                                         PredefinedName }));
}

void ParserTest::case1985()
{
    parseExpression("sizeof __PRETTY_FUNCTION__",
                    Expectation().AST( { SizeofExpression,
                                         ExpressionAsTypeReference,
                                         PredefinedName }));
}

void ParserTest::case1986()
{
    parseExpression("( __FUNCTION__ )",
                    Expectation().AST( { ParenthesizedExpression,
                                         PredefinedName }));
}

void ParserTest::case1987()
{
    parseExpression("( __PRETTY_FUNCTION__ )",
                    Expectation().AST( { ParenthesizedExpression,
                                         PredefinedName }));
}

void ParserTest::case1988()
{
    parseExpression("x = __FUNCTION__",
                    Expectation().AST( { BasicAssignmentExpression,
                                         IdentifierName,
                                         PredefinedName }));
}

void ParserTest::case1989()
{
    parseExpression("x = __PRETTY_FUNCTION__",
                    Expectation().AST( { BasicAssignmentExpression,
                                         IdentifierName,
                                         PredefinedName }));
}

void ParserTest::case1990()
{
    parseExpression("__real__ x",
                    Expectation().AST( { ExtGNU_RealExpression,
                                         IdentifierName }));
}

void ParserTest::case1991()
{
    parseExpression("__real__ ( x )",
                    Expectation().AST( { ExtGNU_RealExpression,
                                         ParenthesizedExpression,
                                         IdentifierName }));
}

void ParserTest::case1992()
{
    parseExpression("__real__ ( __real__ x )",
                    Expectation().AST( { ExtGNU_RealExpression,
                                         ParenthesizedExpression,
                                         ExtGNU_RealExpression,
                                         IdentifierName }));
}
void ParserTest::case1993()
{
    parseExpression("__imag__ x",
                    Expectation().AST( { ExtGNU_ImagExpression,
                                         IdentifierName }));
}

void ParserTest::case1994()
{
    parseExpression("__imag__ ( x )",
                    Expectation().AST( { ExtGNU_ImagExpression,
                                         ParenthesizedExpression,
                                         IdentifierName }));
}

void ParserTest::case1995()
{
    parseExpression("__imag__ __real__ x",
                    Expectation().AST( { ExtGNU_ImagExpression,
                                         ExtGNU_RealExpression,
                                         IdentifierName }));
}

void ParserTest::case1996()
{
    parseExpression("__real__ ( int ) x",
                    Expectation().AST( { ExtGNU_RealExpression,
                                         CastExpression,
                                         TypeName,
                                         BuiltinTypeSpecifier,
                                         AbstractDeclarator,
                                         IdentifierName }));
}

void ParserTest::case1997() {}
void ParserTest::case1998() {}
void ParserTest::case1999() {}
