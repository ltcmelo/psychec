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

void TestParser::case1000()
{
    parseExpression("1",
                    Expectation().AST( { IntegerConstantExpression }));
}

void TestParser::case1001()
{
    parseExpression(".1",
                    Expectation().AST( { FloatingConstantExpression }));
}

void TestParser::case1002()
{
    parseExpression("1.",
                    Expectation().AST( { FloatingConstantExpression }));
}

void TestParser::case1003()
{
    parseExpression("1.2",
                    Expectation().AST( { FloatingConstantExpression }));
}

void TestParser::case1004()
{
    parseExpression("1.2e3",
                    Expectation().AST( { FloatingConstantExpression }));
}

void TestParser::case1005()
{
    parseExpression("1.2E3",
                    Expectation().AST( { FloatingConstantExpression }));
}

void TestParser::case1006()
{
    parseExpression("1.2e+3",
                    Expectation().AST( { FloatingConstantExpression }));
}

void TestParser::case1007()
{
    parseExpression("1.2E+3",
                    Expectation().AST( { FloatingConstantExpression }));
}

void TestParser::case1008()
{
    parseExpression("1.2e-3",
                    Expectation().AST( { FloatingConstantExpression }));
}

void TestParser::case1009()
{
    parseExpression("1.2E-3",
                    Expectation().AST( { FloatingConstantExpression }));
}

void TestParser::case1010()
{
    parseExpression("1e2",
                    Expectation().AST( { FloatingConstantExpression }));
}

void TestParser::case1011()
{
    parseExpression("1E2",
                    Expectation().AST( { FloatingConstantExpression }));
}

void TestParser::case1012()
{
    parseExpression("1e+2",
                    Expectation().AST( { FloatingConstantExpression }));
}

void TestParser::case1013()
{
    parseExpression("1e-2",
                    Expectation().AST( { FloatingConstantExpression }));
}

void TestParser::case1014()
{
    parseExpression("1E+2",
                    Expectation().AST( { FloatingConstantExpression }));
}

void TestParser::case1015()
{
    parseExpression("1E-2",
                    Expectation().AST( { FloatingConstantExpression }));
}

void TestParser::case1016()
{
    parseExpression("1E+2",
                    Expectation().AST( { FloatingConstantExpression }));
}

void TestParser::case1017()
{
    parseExpression("1.e2",
                    Expectation().AST( { FloatingConstantExpression }));
}

void TestParser::case1018()
{
    parseExpression("1.E2",
                    Expectation().AST( { FloatingConstantExpression }));
}

void TestParser::case1019()
{
    parseExpression("0",
                    Expectation().AST( { IntegerConstantExpression }));
}

void TestParser::case1020()
{
    parseExpression("0L",
                    Expectation().AST( { IntegerConstantExpression }));
}

void TestParser::case1021()
{
    parseExpression("int",
                    Expectation().addDiagnostic(Expectation::ErrorOrWarn::Error,
                                                Parser::DiagnosticsReporter::ID_of_ExpectedFIRSTofExpression));
}

void TestParser::case1022()
{
    parseExpression("{",
                    Expectation().addDiagnostic(Expectation::ErrorOrWarn::Error,
                                                Parser::DiagnosticsReporter::ID_of_ExpectedFIRSTofExpression));
}

void TestParser::case1023()
{
    parseExpression("0x1.2p3",
                    Expectation().AST( { FloatingConstantExpression }));
}

void TestParser::case1024()
{
    parseExpression("0x1.ap2",
                    Expectation().AST( { FloatingConstantExpression }));
}

void TestParser::case1025()
{
    parseExpression("0x1.Ap2",
                    Expectation().AST( { FloatingConstantExpression }));
}

void TestParser::case1026()
{
    parseExpression("0x1.fp2",
                    Expectation().AST( { FloatingConstantExpression }));
}

void TestParser::case1027()
{
    parseExpression("0x1.Fp2",
                    Expectation().AST( { FloatingConstantExpression }));
}

void TestParser::case1028()
{
    parseExpression("0x1.2p3f",
                    Expectation().AST( { FloatingConstantExpression }));
}

void TestParser::case1029()
{
    parseExpression("0x1.2p3F",
                    Expectation().AST( { FloatingConstantExpression }));
}

void TestParser::case1030()
{
    parseExpression("x",
                    Expectation().AST( { IdentifierExpression }));
}

void TestParser::case1031()
{
    parseExpression("_x",
                    Expectation().AST( { IdentifierExpression }));
}

void TestParser::case1032()
{
    parseExpression("x_y",
                    Expectation().AST( { IdentifierExpression }));
}

void TestParser::case1033()
{
    parseExpression("x1",
                    Expectation().AST( { IdentifierExpression }));
}

void TestParser::case1034()
{
    parseExpression("x1_",
                    Expectation().AST( { IdentifierExpression }));
}

void TestParser::case1035()
{
    parseExpression("x1y2",
                    Expectation().AST( { IdentifierExpression }));
}

void TestParser::case1036()
{
    parseExpression("_x1y2_",
                    Expectation().AST( { IdentifierExpression }));
}

void TestParser::case1037()
{
    parseExpression("X",
                    Expectation().AST( { IdentifierExpression }));
}

void TestParser::case1038()
{
    parseExpression("_X",
                    Expectation().AST( { IdentifierExpression }));
}

void TestParser::case1039()
{
    parseExpression("X1",
                    Expectation().AST( { IdentifierExpression }));
}

void TestParser::case1040()
{
    parseExpression("_X1",
                    Expectation().AST( { IdentifierExpression }));
}

void TestParser::case1041()
{
    parseExpression("xYz1",
                    Expectation().AST( { IdentifierExpression }));
}

void TestParser::case1042()
{
    parseExpression("xYz1_",
                    Expectation().AST( { IdentifierExpression }));
}

void TestParser::case1043()
{
    parseExpression("xY_z1",
                    Expectation().AST( { IdentifierExpression }));
}

void TestParser::case1044()
{
    parseExpression("1l",
                    Expectation().AST( { IntegerConstantExpression }));
}

void TestParser::case1045()
{
    parseExpression("1ll",
                    Expectation().AST( { IntegerConstantExpression }));
}

void TestParser::case1046()
{
    parseExpression("1L",
                    Expectation().AST( { IntegerConstantExpression }));
}

void TestParser::case1047()
{
    parseExpression("1LL",
                    Expectation().AST( { IntegerConstantExpression }));
}

void TestParser::case1048()
{
    parseExpression("1u",
                    Expectation().AST( { IntegerConstantExpression }));
}

void TestParser::case1049()
{
    parseExpression("1U",
                    Expectation().AST( { IntegerConstantExpression }));
}

void TestParser::case1050()
{
    parseExpression("1uL",
                    Expectation().AST( { IntegerConstantExpression }));
}

void TestParser::case1051()
{
    parseExpression("1ul",
                    Expectation().AST( { IntegerConstantExpression }));
}

void TestParser::case1052()
{
    parseExpression("0x1u",
                    Expectation().AST( { IntegerConstantExpression }));
}

void TestParser::case1053()
{
    parseExpression("0x1Ul",
                    Expectation().AST( { IntegerConstantExpression }));
}

void TestParser::case1054()
{
    parseExpression("L'x'",
                    Expectation().AST( { CharacterConstantExpression }));
}

void TestParser::case1055()
{
    parseExpression("u'x'",
                    Expectation().AST( { CharacterConstantExpression }));
}

void TestParser::case1056()
{
    parseExpression("U'x'",
                    Expectation().AST( { CharacterConstantExpression }));
}

void TestParser::case1057()
{
    parseExpression("'x'",
                    Expectation().AST( { CharacterConstantExpression }));
}

void TestParser::case1058()
{
    parseExpression("u\"xyz\"",
                    Expectation().AST( { StringLiteralExpression }));
}

void TestParser::case1059()
{
    parseExpression("\"xyz\"",
                    Expectation().AST( { StringLiteralExpression }));
}

void TestParser::case1060()
{
    parseExpression("\"x\" \"y\"",
                    Expectation().AST( { StringLiteralExpression,
                                         StringLiteralExpression}));
}

void TestParser::case1061()
{

}

void TestParser::case1062()
{

}

void TestParser::case1063()
{
}

void TestParser::case1064()
{
}

void TestParser::case1065()
{

}

void TestParser::case1066()
{

}

void TestParser::case1067()
{

}

void TestParser::case1068()
{
}

void TestParser::case1069()
{
}

void TestParser::case1070()
{
}

void TestParser::case1071()
{
}

void TestParser::case1072()
{
}

void TestParser::case1073()
{
}

void TestParser::case1074() {}
void TestParser::case1075() {}
void TestParser::case1076() {}
void TestParser::case1077() {}
void TestParser::case1078() {}
void TestParser::case1079() {}
void TestParser::case1080() {}
void TestParser::case1081() {}
void TestParser::case1082() {}
void TestParser::case1083() {}
void TestParser::case1084() {}
void TestParser::case1085() {}
void TestParser::case1086() {}
void TestParser::case1087() {}
void TestParser::case1088() {}
void TestParser::case1089() {}
void TestParser::case1090() {}
void TestParser::case1091() {}
void TestParser::case1092() {}
void TestParser::case1093() {}
void TestParser::case1094() {}
void TestParser::case1095() {}
void TestParser::case1096() {}
void TestParser::case1097() {}
void TestParser::case1098() {}
void TestParser::case1099() {}

void TestParser::case1100()
{
    parseExpression("( 1 )",
                    Expectation().AST( { ParenthesizedExpression,
                                         IntegerConstantExpression }));
}

void TestParser::case1101()
{
    parseExpression("( x )",
                    Expectation().AST( { ParenthesizedExpression,
                                         IdentifierExpression }));
}

void TestParser::case1102()
{
    parseExpression("( ( x ) )",
                    Expectation().AST( { ParenthesizedExpression,
                                         ParenthesizedExpression,
                                         IdentifierExpression }));
}

void TestParser::case1103()
{
    parseExpression("( 'x' )",
                    Expectation().AST( { ParenthesizedExpression,
                                         CharacterConstantExpression }));
}

void TestParser::case1104()
{
    parseExpression("( x",
                    Expectation().addDiagnostic(Expectation::ErrorOrWarn::Error,
                                                Parser::DiagnosticsReporter::ID_of_ExpectedToken));
}

void TestParser::case1105()
{
    parseExpression("( ( x )",
                    Expectation().addDiagnostic(Expectation::ErrorOrWarn::Error,
                                                Parser::DiagnosticsReporter::ID_of_ExpectedToken));
}

void TestParser::case1106()
{
    parseExpression("( int",
                    Expectation().addDiagnostic(Expectation::ErrorOrWarn::Error,
                                                Parser::DiagnosticsReporter::ID_of_ExpectedToken));
}

void TestParser::case1107()
{
    parseExpression("( int )",
                    Expectation().addDiagnostic(Expectation::ErrorOrWarn::Error,
                                                Parser::DiagnosticsReporter::ID_of_ExpectedFIRSTofExpression));
}

void TestParser::case1108() {}
void TestParser::case1109() {}
void TestParser::case1110() {}
void TestParser::case1111() {}
void TestParser::case1112() {}
void TestParser::case1113() {}
void TestParser::case1114() {}
void TestParser::case1115() {}
void TestParser::case1116() {}
void TestParser::case1117() {}
void TestParser::case1118() {}
void TestParser::case1119() {}
void TestParser::case1120() {}
void TestParser::case1121() {}
void TestParser::case1122() {}
void TestParser::case1123() {}
void TestParser::case1124() {}
void TestParser::case1125() {}
void TestParser::case1126() {}
void TestParser::case1127() {}
void TestParser::case1128() {}
void TestParser::case1129() {}
void TestParser::case1130() {}
void TestParser::case1131() {}
void TestParser::case1132() {}
void TestParser::case1133() {}
void TestParser::case1134() {}
void TestParser::case1135() {}
void TestParser::case1136() {}
void TestParser::case1137() {}
void TestParser::case1138() {}
void TestParser::case1139() {}
void TestParser::case1140() {}
void TestParser::case1141() {}
void TestParser::case1142() {}
void TestParser::case1143() {}
void TestParser::case1144() {}
void TestParser::case1145() {}
void TestParser::case1146() {}
void TestParser::case1147() {}
void TestParser::case1148() {}
void TestParser::case1149() {}
void TestParser::case1150() {}
void TestParser::case1151() {}
void TestParser::case1152() {}
void TestParser::case1153() {}
void TestParser::case1154() {}
void TestParser::case1155() {}
void TestParser::case1156() {}
void TestParser::case1157() {}
void TestParser::case1158() {}
void TestParser::case1159() {}
void TestParser::case1160() {}
void TestParser::case1161() {}
void TestParser::case1162() {}
void TestParser::case1163() {}
void TestParser::case1164() {}
void TestParser::case1165() {}
void TestParser::case1166() {}
void TestParser::case1167() {}
void TestParser::case1168() {}
void TestParser::case1169() {}
void TestParser::case1170() {}
void TestParser::case1171() {}
void TestParser::case1172() {}
void TestParser::case1173() {}
void TestParser::case1174() {}
void TestParser::case1175() {}
void TestParser::case1176() {}
void TestParser::case1177() {}
void TestParser::case1178() {}
void TestParser::case1179() {}
void TestParser::case1180() {}
void TestParser::case1181() {}
void TestParser::case1182() {}
void TestParser::case1183() {}
void TestParser::case1184() {}
void TestParser::case1185() {}
void TestParser::case1186() {}
void TestParser::case1187() {}
void TestParser::case1188() {}
void TestParser::case1189() {}
void TestParser::case1190() {}
void TestParser::case1191() {}
void TestParser::case1192() {}
void TestParser::case1193() {}
void TestParser::case1194() {}
void TestParser::case1195() {}
void TestParser::case1196() {}
void TestParser::case1197() {}
void TestParser::case1198() {}
void TestParser::case1199() {}

void TestParser::case1200()
{
    parseExpression(R"(
                    _Generic( x,
                             default: cbrt)
                    )");
}

void TestParser::case1201()
{
    parseExpression(R"(
                    _Generic( x,
                             long double: cbrtl,
                             default: cbrt,
                             float: cbrtf)
                    )");
}

void TestParser::case1202()
{
    parseExpression(R"(
                    _Generic( x,
                             float: cbrtf)
                    )");
}

void TestParser::case1203()
{
    parseExpression(R"(
                    _Generic( ( x ),
                             long double: cbrtl,
                             default: cbrt)
                    )");
}

void TestParser::case1204()
{
    parseExpression(R"(
                    _Generic( x float: cbrtf)
                    )",
                    Expectation().addDiagnostic(Expectation::ErrorOrWarn::Error,
                                                Parser::DiagnosticsReporter::ID_of_ExpectedToken));
}

void TestParser::case1205()
{
    parseExpression("_Generic( x, float, cbrtf)",
                    Expectation().addDiagnostic(Expectation::ErrorOrWarn::Error,
                                                Parser::DiagnosticsReporter::ID_of_ExpectedToken));
}

void TestParser::case1206()
{
    parseExpression("_Generic( x : cbrtf)",
                    Expectation().addDiagnostic(Expectation::ErrorOrWarn::Error,
                                                Parser::DiagnosticsReporter::ID_of_ExpectedToken));
}

void TestParser::case1207()
{
    parseExpression("_Generic(float: cbrtf)",
                    Expectation().addDiagnostic(Expectation::ErrorOrWarn::Error,
                                                Parser::DiagnosticsReporter::ID_of_ExpectedFIRSTofExpression));
}

void TestParser::case1208()
{
    parseExpression(R"(
                    _Generic( x,
                             int: y,
                             char*: z,
                             const char*: w)
                    )");
}

void TestParser::case1209()
{
    parseExpression(R"(
                    _Generic( x,
                             default: cbrt
                    )",
                    1);
}

void TestParser::case1210()
{
    parseExpression(R"(
                    _Generic( x,
                             default:
                    )",
                    1);
}

void TestParser::case1211()
{
    parseExpression(R"(
                    _Generic( ( x ), int : 1,)
                    )",
                    1);
}

void TestParser::case1212() {}
void TestParser::case1213() {}
void TestParser::case1214() {}
void TestParser::case1215() {}
void TestParser::case1216() {}
void TestParser::case1217() {}
void TestParser::case1218() {}
void TestParser::case1219() {}
void TestParser::case1220() {}
void TestParser::case1221() {}
void TestParser::case1222() {}
void TestParser::case1223() {}
void TestParser::case1224() {}
void TestParser::case1225() {}
void TestParser::case1226() {}
void TestParser::case1227() {}
void TestParser::case1228() {}
void TestParser::case1229() {}
void TestParser::case1230() {}
void TestParser::case1231() {}
void TestParser::case1232() {}
void TestParser::case1233() {}
void TestParser::case1234() {}
void TestParser::case1235() {}
void TestParser::case1236() {}
void TestParser::case1237() {}
void TestParser::case1238() {}
void TestParser::case1239() {}
void TestParser::case1240() {}
void TestParser::case1241() {}
void TestParser::case1242() {}
void TestParser::case1243() {}
void TestParser::case1244() {}
void TestParser::case1245() {}
void TestParser::case1246() {}
void TestParser::case1247() {}
void TestParser::case1248() {}
void TestParser::case1249() {}

void TestParser::case1250()
{
    parseExpression("( struct x ) { y++ }",
                    Expectation().AST( { CompoundLiteralExpression,
                                         TypeName,
                                         StructTypeSpecifier,
                                         AbstractDeclarator,
                                         BraceEnclosedInitializer,
                                         ExpressionInitializer,
                                         PostIncrementExpression,
                                         IdentifierExpression }));
}

void TestParser::case1251()
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

void TestParser::case1252()
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

void TestParser::case1253()
{
    parseExpression("( struct x ) {",
                    Expectation().addDiagnostic(Expectation::ErrorOrWarn::Error,
                                                Parser::DiagnosticsReporter::ID_of_ExpectedFIRSTofExpression));
}

void TestParser::case1254()
{
    parseExpression("( struct x ) { 1,",
                    Expectation().addDiagnostic(Expectation::ErrorOrWarn::Error,
                                                Parser::DiagnosticsReporter::ID_of_ExpectedFIRSTofExpression));
}

void TestParser::case1255()
{
    parseExpression("( struct x ) { 1",
                    Expectation().addDiagnostic(Expectation::ErrorOrWarn::Error,
                                                Parser::DiagnosticsReporter::ID_of_ExpectedToken));
}

void TestParser::case1256()
{
    parseExpression("(long) {",
                    Expectation().addDiagnostic(Expectation::ErrorOrWarn::Error,
                                                Parser::DiagnosticsReporter::ID_of_ExpectedFIRSTofExpression));
}

void TestParser::case1257()
{
}

void TestParser::case1258()
{
}

void TestParser::case1259() {}
void TestParser::case1260() {}
void TestParser::case1261() {}
void TestParser::case1262() {}
void TestParser::case1263() {}
void TestParser::case1264() {}
void TestParser::case1265() {}
void TestParser::case1266() {}
void TestParser::case1267() {}
void TestParser::case1268() {}
void TestParser::case1269() {}
void TestParser::case1270() {}
void TestParser::case1271() {}
void TestParser::case1272() {}
void TestParser::case1273() {}
void TestParser::case1274() {}
void TestParser::case1275() {}
void TestParser::case1276() {}
void TestParser::case1277() {}
void TestParser::case1278() {}
void TestParser::case1279() {}
void TestParser::case1280() {}
void TestParser::case1281() {}
void TestParser::case1282() {}
void TestParser::case1283() {}
void TestParser::case1284() {}
void TestParser::case1285() {}
void TestParser::case1286() {}
void TestParser::case1287() {}
void TestParser::case1288() {}
void TestParser::case1289() {}
void TestParser::case1290() {}
void TestParser::case1291() {}
void TestParser::case1292() {}
void TestParser::case1293() {}
void TestParser::case1294() {}
void TestParser::case1295() {}
void TestParser::case1296() {}
void TestParser::case1297() {}
void TestParser::case1298() {}
void TestParser::case1299() {}

void TestParser::case1300()
{
    parseExpression("++x",
                    Expectation().AST({ PreIncrementExpression,
                                        IdentifierExpression }));
}

void TestParser::case1301()
{
    parseExpression("--x",
                    Expectation().AST({ PreDecrementExpression,
                                        IdentifierExpression }));;
}

void TestParser::case1302()
{
    parseExpression("x++",
                    Expectation().AST({ PostIncrementExpression,
                                        IdentifierExpression }));
}

void TestParser::case1303()
{
    parseExpression("x--",
                    Expectation().AST({ PostDecrementExpression,
                                        IdentifierExpression }));
}

void TestParser::case1304()
{
    parseExpression("++( x )",
                    Expectation().AST({ PreIncrementExpression,
                                        ParenthesizedExpression,
                                        IdentifierExpression }));
}

void TestParser::case1305()
{
    parseExpression("++( ( x ))",
                    Expectation().AST({ PreIncrementExpression,
                                        ParenthesizedExpression,
                                        ParenthesizedExpression,
                                        IdentifierExpression }));
}

void TestParser::case1306()
{
    parseExpression("( x )++",
                    Expectation().AST({ PostIncrementExpression,
                                        ParenthesizedExpression,
                                        IdentifierExpression }));
}

void TestParser::case1307()
{
    parseExpression("( ( x ))++",
                    Expectation().AST({ PostIncrementExpression,
                                        ParenthesizedExpression,
                                        ParenthesizedExpression,
                                        IdentifierExpression }));
}

void TestParser::case1308()
{
    parseExpression("x+",
                    Expectation().addDiagnostic(Expectation::ErrorOrWarn::Error,
                                                Parser::DiagnosticsReporter::ID_of_ExpectedFIRSTofExpression));
}

void TestParser::case1309() {}
void TestParser::case1310() {}
void TestParser::case1311() {}
void TestParser::case1312() {}
void TestParser::case1313() {}
void TestParser::case1314() {}
void TestParser::case1315() {}
void TestParser::case1316() {}
void TestParser::case1317() {}
void TestParser::case1318() {}
void TestParser::case1319() {}
void TestParser::case1320() {}
void TestParser::case1321() {}
void TestParser::case1322() {}
void TestParser::case1323() {}
void TestParser::case1324() {}
void TestParser::case1325() {}
void TestParser::case1326() {}
void TestParser::case1327() {}
void TestParser::case1328() {}
void TestParser::case1329() {}
void TestParser::case1330() {}
void TestParser::case1331() {}
void TestParser::case1332() {}
void TestParser::case1333() {}
void TestParser::case1334() {}
void TestParser::case1335() {}
void TestParser::case1336() {}
void TestParser::case1337() {}
void TestParser::case1338() {}
void TestParser::case1339() {}
void TestParser::case1340() {}
void TestParser::case1341() {}
void TestParser::case1342() {}
void TestParser::case1343() {}
void TestParser::case1344() {}
void TestParser::case1345() {}
void TestParser::case1346() {}
void TestParser::case1347() {}
void TestParser::case1348() {}
void TestParser::case1349() {}

void TestParser::case1350()
{
    parseExpression("-1",
                    Expectation().AST( { UnaryMinusExpression,
                                         IntegerConstantExpression }));
}

void TestParser::case1351()
{
    parseExpression("+1",
                    Expectation().AST( { UnaryPlusExpression,
                                         IntegerConstantExpression }));
}

void TestParser::case1352()
{
    parseExpression("-1.2",
                    Expectation().AST( { UnaryMinusExpression,
                                         FloatingConstantExpression }));
}

void TestParser::case1353()
{
    parseExpression("-.1",
                    Expectation().AST( { UnaryMinusExpression,
                                         FloatingConstantExpression }));
}

void TestParser::case1354()
{
    parseExpression("-1.2e3",
                    Expectation().AST( { UnaryMinusExpression,
                                         FloatingConstantExpression }));
}

void TestParser::case1355() {}
void TestParser::case1356() {}
void TestParser::case1357() {}
void TestParser::case1358() {}
void TestParser::case1359() {}
void TestParser::case1360() {}
void TestParser::case1361() {}
void TestParser::case1362() {}
void TestParser::case1363() {}
void TestParser::case1364() {}
void TestParser::case1365() {}
void TestParser::case1366() {}
void TestParser::case1367() {}
void TestParser::case1368() {}
void TestParser::case1369() {}
void TestParser::case1370() {}
void TestParser::case1371() {}
void TestParser::case1372() {}
void TestParser::case1373() {}
void TestParser::case1374() {}
void TestParser::case1375() {}
void TestParser::case1376() {}
void TestParser::case1377() {}
void TestParser::case1378() {}
void TestParser::case1379() {}
void TestParser::case1380() {}
void TestParser::case1381() {}
void TestParser::case1382() {}
void TestParser::case1383() {}
void TestParser::case1384() {}
void TestParser::case1385() {}
void TestParser::case1386() {}
void TestParser::case1387() {}
void TestParser::case1388() {}
void TestParser::case1389() {}
void TestParser::case1390() {}
void TestParser::case1391() {}
void TestParser::case1392() {}
void TestParser::case1393() {}
void TestParser::case1394() {}
void TestParser::case1395() {}
void TestParser::case1396() {}
void TestParser::case1397() {}
void TestParser::case1398() {}
void TestParser::case1399() {}

void TestParser::case1400()
{
    parseExpression("sizeof x",
                    Expectation().AST( { SizeofExpression,
                                         IdentifierExpression }));
}

void TestParser::case1401()
{
    parseExpression("sizeof ( x )",
                    Expectation().AST( { SizeofExpression,
                                         AmbiguousTypedefNameOrIdentifierExpression })
                                 .replicateAmbiguity("sizeof ( x x )"));
}

void TestParser::case1402()
{
    parseExpression("sizeof ( const x )",
                    Expectation().AST( { SizeofExpression,
                                         TypeName,
                                         ConstQualifier,
                                         TypedefName,
                                         AbstractDeclarator }));
}

void TestParser::case1403()
{
    parseExpression("sizeof ( x * )",
                    Expectation().AST( { SizeofExpression,
                                         TypeName,
                                         TypedefName,
                                         PointerDeclarator,
                                         AbstractDeclarator }));
}

void TestParser::case1404()
{
    parseExpression("sizeof int",
                    Expectation().addDiagnostic(Expectation::ErrorOrWarn::Error,
                                                Parser::DiagnosticsReporter::ID_of_ExpectedFIRSTofExpression));
}

void TestParser::case1405()
{
    parseExpression("sizeof * x",
                    Expectation().AST( { SizeofExpression,
                                         PointerIndirectionExpression,
                                         IdentifierExpression }));
}

void TestParser::case1406()
{
    parseExpression("sizeof x [ 0 ]",
                    Expectation().AST( { SizeofExpression,
                                         ElementAccessExpression,
                                         IdentifierExpression,
                                         IntegerConstantExpression }));
}

void TestParser::case1407()
{
    parseExpression("sizeof ( int )",
                    Expectation().AST( { SizeofExpression,
                                         TypeName,
                                         BuiltinTypeSpecifier,
                                         AbstractDeclarator
                                       }));
}

void TestParser::case1408()
{
    parseExpression("sizeof ( int * )",
                    Expectation().AST( { SizeofExpression,
                                         TypeName,
                                         BuiltinTypeSpecifier,
                                         PointerDeclarator,
                                         AbstractDeclarator }));
}

void TestParser::case1409()
{
    parseExpression("sizeof x . y",
                    Expectation().AST( { SizeofExpression,
                                         DirectMemberAccessExpression,
                                         IdentifierExpression,
                                         IdentifierExpression }));
}

void TestParser::case1410()
{
    parseExpression("sizeof ( x . y )");
}

void TestParser::case1411()
{
    parseExpression("sizeof x [ 0 ]");
}

void TestParser::case1412()
{
    parseExpression("sizeof ( x [ 0 ] )");
}

void TestParser::case1413()
{
    parseExpression("sizeof ( x + 1 )");
}

void TestParser::case1414() {}
void TestParser::case1415() {}
void TestParser::case1416() {}
void TestParser::case1417() {}
void TestParser::case1418() {}
void TestParser::case1419() {}
void TestParser::case1420() {}
void TestParser::case1421() {}
void TestParser::case1422() {}
void TestParser::case1423() {}
void TestParser::case1424() {}
void TestParser::case1425() {}
void TestParser::case1426() {}
void TestParser::case1427() {}
void TestParser::case1428() {}
void TestParser::case1429() {}
void TestParser::case1430() {}
void TestParser::case1431() {}
void TestParser::case1432() {}
void TestParser::case1433() {}
void TestParser::case1434() {}
void TestParser::case1435() {}
void TestParser::case1436() {}
void TestParser::case1437() {}
void TestParser::case1438() {}
void TestParser::case1439() {}
void TestParser::case1440() {}
void TestParser::case1441() {}
void TestParser::case1442() {}
void TestParser::case1443() {}
void TestParser::case1444() {}
void TestParser::case1445() {}
void TestParser::case1446() {}
void TestParser::case1447() {}
void TestParser::case1448() {}
void TestParser::case1449() {}

void TestParser::case1450()
{
    parseExpression("_Alignof ( x )",
                    Expectation().AST( { AlignofExpression,
                                         AmbiguousTypedefNameOrIdentifierExpression })
                                 .replicateAmbiguity("_Alignof ( x x )"));
}

void TestParser::case1451()
{
    parseExpression("_Alignof ( int )",
                    Expectation().AST( { AlignofExpression,
                                         TypeName,
                                         BuiltinTypeSpecifier,
                                         AbstractDeclarator }));
}

void TestParser::case1452()
{
    parseExpression("_Alignof ( int * )",
                    Expectation().AST( { AlignofExpression,
                                         TypeName,
                                         BuiltinTypeSpecifier,
                                         PointerDeclarator,
                                         AbstractDeclarator }));
}

void TestParser::case1453()
{
    // See LanguageExtensions.

    parseExpression("_Alignof x",
                    Expectation().AST( { AlignofExpression,
                                         IdentifierExpression }));
}

void TestParser::case1454()
{
}

void TestParser::case1455()
{
}

void TestParser::case1456()
{
}

void TestParser::case1457()
{
}

void TestParser::case1458() {}
void TestParser::case1459() {}
void TestParser::case1460() {}
void TestParser::case1461() {}
void TestParser::case1462() {}
void TestParser::case1463() {}
void TestParser::case1464() {}
void TestParser::case1465() {}
void TestParser::case1466() {}
void TestParser::case1467() {}
void TestParser::case1468() {}
void TestParser::case1469() {}
void TestParser::case1470() {}
void TestParser::case1471() {}
void TestParser::case1472() {}
void TestParser::case1473() {}
void TestParser::case1474() {}
void TestParser::case1475() {}
void TestParser::case1476() {}
void TestParser::case1477() {}
void TestParser::case1478() {}
void TestParser::case1479() {}
void TestParser::case1480() {}
void TestParser::case1481() {}
void TestParser::case1482() {}
void TestParser::case1483() {}
void TestParser::case1484() {}
void TestParser::case1485() {}
void TestParser::case1486() {}
void TestParser::case1487() {}
void TestParser::case1488() {}
void TestParser::case1489() {}
void TestParser::case1490() {}
void TestParser::case1491() {}
void TestParser::case1492() {}
void TestParser::case1493() {}
void TestParser::case1494() {}
void TestParser::case1495() {}
void TestParser::case1496() {}
void TestParser::case1497() {}
void TestParser::case1498() {}
void TestParser::case1499() {}

void TestParser::case1500()
{
    parseExpression("x . y",
                    Expectation().AST( { DirectMemberAccessExpression,
                                         IdentifierExpression,
                                         IdentifierExpression }));
}

void TestParser::case1501()
{
    parseExpression("x .",
                    Expectation().addDiagnostic(Expectation::ErrorOrWarn::Error,
                                                Parser::DiagnosticsReporter::ID_of_ExpectedFieldName));
}

void TestParser::case1502()
{
    parseExpression("x . 'y'",
                    Expectation().addDiagnostic(Expectation::ErrorOrWarn::Error,
                                                Parser::DiagnosticsReporter::ID_of_ExpectedFieldName));
}

void TestParser::case1503()
{
    parseExpression("x ( ) . y",
                    Expectation().AST( { DirectMemberAccessExpression,
                                         CallExpression,
                                         IdentifierExpression,
                                         IdentifierExpression }));
}

void TestParser::case1504()
{
    parseExpression("x -> y",
                    Expectation().AST( { IndirectMemberAccessExpression,
                                         IdentifierExpression,
                                         IdentifierExpression }));
}

void TestParser::case1505()
{
    parseExpression("x -> ",
                    Expectation().addDiagnostic(Expectation::ErrorOrWarn::Error,
                                                Parser::DiagnosticsReporter::ID_of_ExpectedFieldName));
}

void TestParser::case1506()
{
    parseExpression("x -> 'y'",
                    Expectation().addDiagnostic(Expectation::ErrorOrWarn::Error,
                                                Parser::DiagnosticsReporter::ID_of_ExpectedFieldName));
}

void TestParser::case1507()
{
    parseExpression("x . y . z",
                    Expectation().AST( { DirectMemberAccessExpression,
                                         DirectMemberAccessExpression,
                                         IdentifierExpression,
                                         IdentifierExpression,
                                         IdentifierExpression }));
}

void TestParser::case1508()
{
    parseExpression("x . y -> z",
                    Expectation().AST( { IndirectMemberAccessExpression,
                                         DirectMemberAccessExpression,
                                         IdentifierExpression,
                                         IdentifierExpression,
                                         IdentifierExpression }));
}

void TestParser::case1509()
{
    parseExpression("x -> y -> z",
                    Expectation().AST( { IndirectMemberAccessExpression,
                                         IndirectMemberAccessExpression,
                                         IdentifierExpression,
                                         IdentifierExpression,
                                         IdentifierExpression }));
}

void TestParser::case1510()
{
    parseExpression("( x . y ) -> z",
                    Expectation().AST( { IndirectMemberAccessExpression,
                                         ParenthesizedExpression,
                                         DirectMemberAccessExpression,
                                         IdentifierExpression,
                                         IdentifierExpression,
                                         IdentifierExpression }));
}

void TestParser::case1511()
{
    parseExpression("x . y . z . w",
                    Expectation().AST( { DirectMemberAccessExpression,
                                         DirectMemberAccessExpression,
                                         DirectMemberAccessExpression,
                                         IdentifierExpression,
                                         IdentifierExpression,
                                         IdentifierExpression,
                                         IdentifierExpression }));
}

void TestParser::case1512()
{
    parseExpression("x [ 0 ] . y",
                    Expectation().AST( { DirectMemberAccessExpression,
                                         ElementAccessExpression,
                                         IdentifierExpression,
                                         IntegerConstantExpression,
                                         IdentifierExpression }));

}

void TestParser::case1513() {}
void TestParser::case1514() {}
void TestParser::case1515() {}
void TestParser::case1516() {}
void TestParser::case1517() {}
void TestParser::case1518() {}
void TestParser::case1519() {}
void TestParser::case1520() {}
void TestParser::case1521() {}
void TestParser::case1522() {}
void TestParser::case1523() {}
void TestParser::case1524() {}
void TestParser::case1525() {}
void TestParser::case1526() {}
void TestParser::case1527() {}
void TestParser::case1528() {}
void TestParser::case1529() {}
void TestParser::case1530() {}
void TestParser::case1531() {}
void TestParser::case1532() {}
void TestParser::case1533() {}
void TestParser::case1534() {}
void TestParser::case1535() {}
void TestParser::case1536() {}
void TestParser::case1537() {}
void TestParser::case1538() {}
void TestParser::case1539() {}
void TestParser::case1540() {}
void TestParser::case1541() {}
void TestParser::case1542() {}
void TestParser::case1543() {}
void TestParser::case1544() {}
void TestParser::case1545() {}
void TestParser::case1546() {}
void TestParser::case1547() {}
void TestParser::case1548() {}
void TestParser::case1549() {}
void TestParser::case1550() {}
void TestParser::case1551() {}
void TestParser::case1552() {}
void TestParser::case1553() {}
void TestParser::case1554() {}
void TestParser::case1555() {}
void TestParser::case1556() {}
void TestParser::case1557() {}
void TestParser::case1558() {}
void TestParser::case1559() {}
void TestParser::case1560() {}
void TestParser::case1561() {}
void TestParser::case1562() {}
void TestParser::case1563() {}
void TestParser::case1564() {}
void TestParser::case1565() {}
void TestParser::case1566() {}
void TestParser::case1567() {}
void TestParser::case1568() {}
void TestParser::case1569() {}
void TestParser::case1570() {}
void TestParser::case1571() {}
void TestParser::case1572() {}
void TestParser::case1573() {}
void TestParser::case1574() {}
void TestParser::case1575() {}
void TestParser::case1576() {}
void TestParser::case1577() {}
void TestParser::case1578() {}
void TestParser::case1579() {}
void TestParser::case1580() {}
void TestParser::case1581() {}
void TestParser::case1582() {}
void TestParser::case1583() {}
void TestParser::case1584() {}
void TestParser::case1585() {}
void TestParser::case1586() {}
void TestParser::case1587() {}
void TestParser::case1588() {}
void TestParser::case1589() {}
void TestParser::case1590() {}
void TestParser::case1591() {}
void TestParser::case1592() {}
void TestParser::case1593() {}
void TestParser::case1594() {}
void TestParser::case1595() {}
void TestParser::case1596() {}
void TestParser::case1597() {}
void TestParser::case1598() {}
void TestParser::case1599() {}

void TestParser::case1600()
{
    parseExpression("x ( )",
                    Expectation().AST( { CallExpression,
                                         IdentifierExpression }));
}

void TestParser::case1601()
{
    parseExpression("( x ) ( )",
                    Expectation().AST( { CallExpression,
                                         ParenthesizedExpression,
                                         IdentifierExpression }));
}

void TestParser::case1602()
{
    parseExpression("x ( 1 )",
                    Expectation().AST( { CallExpression,
                                         IdentifierExpression,
                                         IntegerConstantExpression }));
}

void TestParser::case1603()
{
    parseExpression("x ( y )",
                    Expectation().AST( { CallExpression,
                                         IdentifierExpression,
                                         IdentifierExpression }));
}

void TestParser::case1604()
{
    parseExpression("x ( y , 1 )",
                    Expectation().AST( { CallExpression,
                                         IdentifierExpression,
                                         IdentifierExpression,
                                         IntegerConstantExpression }));
}

void TestParser::case1605()
{
    parseExpression("( & x ) ( )",
                    Expectation().AST( { CallExpression,
                                         ParenthesizedExpression,
                                         AddressOfExpression,
                                         IdentifierExpression }));
}

void TestParser::case1606()
{
    parseExpression("( * x ) ( )",
                    Expectation().AST( { CallExpression,
                                         ParenthesizedExpression,
                                         PointerIndirectionExpression,
                                         IdentifierExpression }));
}

void TestParser::case1607()
{
    parseExpression("( * * x ) ( )",
                    Expectation().AST( { CallExpression,
                                         ParenthesizedExpression,
                                         PointerIndirectionExpression,
                                         PointerIndirectionExpression,
                                         IdentifierExpression }));
}

void TestParser::case1608()
{
    parseExpression("x ( y ( ), \"z\" )",
                    Expectation().AST( { CallExpression,
                                         IdentifierExpression,
                                         CallExpression,
                                         IdentifierExpression,
                                         StringLiteralExpression }));
}

void TestParser::case1609()
{
    parseExpression("x ( y",
                    Expectation().addDiagnostic(Expectation::ErrorOrWarn::Error,
                                                Parser::DiagnosticsReporter::ID_of_ExpectedToken));
}

void TestParser::case1610()
{
    parseExpression("x (",
                    Expectation().addDiagnostic(Expectation::ErrorOrWarn::Error,
                                                Parser::DiagnosticsReporter::ID_of_ExpectedFIRSTofExpression));
}

void TestParser::case1611()
{
    parseExpression("x ( y , )",
                    Expectation().addDiagnostic(Expectation::ErrorOrWarn::Error,
                                                Parser::DiagnosticsReporter::ID_of_ExpectedFIRSTofExpression));
}

void TestParser::case1612()
{
    parseExpression("x ( ( 1 ) )",
                    Expectation().AST( { CallExpression,
                                         IdentifierExpression,
                                         ParenthesizedExpression,
                                         IntegerConstantExpression }));
}

void TestParser::case1613()
{
    parseExpression("x ( ( ( y ) ) )",
                    Expectation().AST( { CallExpression,
                                         IdentifierExpression,
                                         ParenthesizedExpression,
                                         ParenthesizedExpression,
                                         IdentifierExpression }));
}

void TestParser::case1614()
{
    parseExpression("( x ( ) )",
                    Expectation().AST( { ParenthesizedExpression,
                                         CallExpression,
                                         IdentifierExpression }));
}

void TestParser::case1615()
{
    parseExpression("( ( x ( ) ) )",
                    Expectation().AST( { ParenthesizedExpression,
                                         ParenthesizedExpression,
                                         CallExpression,
                                         IdentifierExpression }));
}

void TestParser::case1616()
{
    parseExpression("( x ( 1 ) )",
                    Expectation().AST( { ParenthesizedExpression,
                                         CallExpression,
                                         IdentifierExpression,
                                         IntegerConstantExpression }));
}

void TestParser::case1617()
{
    parseExpression("( x ( 1 , y ) )",
                    Expectation().AST( { ParenthesizedExpression,
                                         CallExpression,
                                         IdentifierExpression,
                                         IntegerConstantExpression,
                                         IdentifierExpression }));
}

void TestParser::case1618() {}
void TestParser::case1619() {}
void TestParser::case1620() {}
void TestParser::case1621() {}
void TestParser::case1622() {}
void TestParser::case1623() {}
void TestParser::case1624() {}
void TestParser::case1625() {}
void TestParser::case1626() {}
void TestParser::case1627() {}
void TestParser::case1628() {}
void TestParser::case1629() {}
void TestParser::case1630() {}
void TestParser::case1631() {}
void TestParser::case1632() {}
void TestParser::case1633() {}
void TestParser::case1634() {}
void TestParser::case1635() {}
void TestParser::case1636() {}
void TestParser::case1637() {}
void TestParser::case1638() {}
void TestParser::case1639() {}
void TestParser::case1640() {}
void TestParser::case1641() {}
void TestParser::case1642() {}
void TestParser::case1643() {}
void TestParser::case1644() {}
void TestParser::case1645() {}
void TestParser::case1646() {}
void TestParser::case1647() {}
void TestParser::case1648() {}
void TestParser::case1649() {}

void TestParser::case1650()
{
    parseExpression("* x",
                    Expectation().AST( { PointerIndirectionExpression,
                                         IdentifierExpression }));
}

void TestParser::case1651()
{
    parseExpression("* ( x )",
                    Expectation().AST( { PointerIndirectionExpression,
                                         ParenthesizedExpression,
                                         IdentifierExpression }));
}

void TestParser::case1652()
{
    parseExpression("& x",
                    Expectation().AST( { AddressOfExpression,
                                         IdentifierExpression }));
}

void TestParser::case1653()
{
    // Syntax OK, semantics invalid.

    CROSS_REFERENCE_TEST(TestBinder::case0101);

    parseExpression("* 1",
                    Expectation().AST( { PointerIndirectionExpression,
                                         IntegerConstantExpression }));
}

void TestParser::case1654()
{
    // Syntax OK, semantics invalid.

    CROSS_REFERENCE_TEST(TestBinder::case0102);

    parseExpression("& 1",
                    Expectation().AST( { AddressOfExpression,
                                         IntegerConstantExpression }));
}

void TestParser::case1655() {}
void TestParser::case1656() {}
void TestParser::case1657() {}
void TestParser::case1658() {}
void TestParser::case1659() {}
void TestParser::case1660() {}
void TestParser::case1661() {}
void TestParser::case1662() {}
void TestParser::case1663() {}
void TestParser::case1664() {}
void TestParser::case1665() {}
void TestParser::case1666() {}
void TestParser::case1667() {}
void TestParser::case1668() {}
void TestParser::case1669() {}
void TestParser::case1670() {}
void TestParser::case1671() {}
void TestParser::case1672() {}
void TestParser::case1673() {}
void TestParser::case1674() {}
void TestParser::case1675() {}
void TestParser::case1676() {}
void TestParser::case1677() {}
void TestParser::case1678() {}
void TestParser::case1679() {}
void TestParser::case1680() {}
void TestParser::case1681() {}
void TestParser::case1682() {}
void TestParser::case1683() {}
void TestParser::case1684() {}
void TestParser::case1685() {}
void TestParser::case1686() {}
void TestParser::case1687() {}
void TestParser::case1688() {}
void TestParser::case1689() {}
void TestParser::case1690() {}
void TestParser::case1691() {}
void TestParser::case1692() {}
void TestParser::case1693() {}
void TestParser::case1694() {}
void TestParser::case1695() {}
void TestParser::case1696() {}
void TestParser::case1697() {}
void TestParser::case1698() {}
void TestParser::case1699() {}

void TestParser::case1700()
{
    parseExpression("( int ) x",
                    Expectation().AST({ CastExpression,
                                        TypeName,
                                        BuiltinTypeSpecifier,
                                        AbstractDeclarator,
                                        IdentifierExpression }));
}

void TestParser::case1701()
{
    parseExpression("( int * ) x",
                    Expectation().AST({ CastExpression,
                                        TypeName,
                                        BuiltinTypeSpecifier,
                                        PointerDeclarator,
                                        AbstractDeclarator,
                                        IdentifierExpression }));
}

void TestParser::case1702()
{
    parseExpression("( int const * ) x",
                    Expectation().AST({ CastExpression,
                                        TypeName,
                                        BuiltinTypeSpecifier,
                                        ConstQualifier,
                                        PointerDeclarator,
                                        AbstractDeclarator,
                                        IdentifierExpression }));
}

void TestParser::case1703()
{
    parseExpression("( struct x ) y",
                    Expectation().AST({ CastExpression,
                                        TypeName,
                                        StructTypeSpecifier,
                                        AbstractDeclarator,
                                        IdentifierExpression }));
}

void TestParser::case1704()
{
    parseExpression("( unsigned long ) + x",
                    Expectation().AST({ CastExpression,
                                        TypeName,
                                        BuiltinTypeSpecifier,
                                        BuiltinTypeSpecifier,
                                        AbstractDeclarator,
                                        UnaryPlusExpression,
                                        IdentifierExpression }));
}

void TestParser::case1705()
{
    parseExpression("( unsigned long ) - x",
                    Expectation().AST({ CastExpression,
                                        TypeName,
                                        BuiltinTypeSpecifier,
                                        BuiltinTypeSpecifier,
                                        AbstractDeclarator,
                                        UnaryMinusExpression,
                                        IdentifierExpression }));
}

void TestParser::case1706()
{
    parseExpression("( int ) ++ x",
                    Expectation().AST({ CastExpression,
                                        TypeName,
                                        BuiltinTypeSpecifier,
                                        AbstractDeclarator,
                                        PreIncrementExpression,
                                        IdentifierExpression }));
}

void TestParser::case1707()
{
    parseExpression("( int ) x ++",
                    Expectation().AST({ CastExpression,
                                        TypeName,
                                        BuiltinTypeSpecifier,
                                        AbstractDeclarator,
                                        PostIncrementExpression,
                                        IdentifierExpression }));
}

void TestParser::case1708()
{
    parseExpression("( int ) _Generic ( y", 1);
}

void TestParser::case1709()
{
    parseExpression("( long ) int", 1);
}

void TestParser::case1710()
{
    parseExpression("( struct x ) -", 1);
}

void TestParser::case1711()
{
    parseExpression("( int ) -", 1);
}

void TestParser::case1712()
{
    parseExpression("( int ) x +", 1);
}

void TestParser::case1713()
{
    parseExpression("( int ) x ( y", 1);
}

void TestParser::case1714()
{
    parseExpression("( x ) y ( z )",
                    Expectation().AST({ CastExpression,
                                        TypeName,
                                        TypedefName,
                                        AbstractDeclarator,
                                        CallExpression,
                                        IdentifierExpression,
                                        IdentifierExpression }));
}

void TestParser::case1715()
{
    parseExpression("( x ) ( y ( z ) )",
                    Expectation().AST({ CastExpression,
                                        TypeName,
                                        TypedefName,
                                        AbstractDeclarator,
                                        ParenthesizedExpression,
                                        CallExpression,
                                        IdentifierExpression,
                                        IdentifierExpression }));
}

void TestParser::case1716()
{
    parseExpression("( int ) - 1",
                    Expectation().AST( { CastExpression,
                                         TypeName,
                                         BuiltinTypeSpecifier,
                                         AbstractDeclarator,
                                         UnaryMinusExpression,
                                         IntegerConstantExpression }));
}

void TestParser::case1717()
{
    parseExpression("( x ) - 1",
                    Expectation().replicateAmbiguity()
                                 .AST( { AmbiguousCastOrBinaryExpression,
                                         CastExpression,
                                         TypeName,
                                         TypedefName,
                                         AbstractDeclarator,
                                         UnaryMinusExpression,
                                         IntegerConstantExpression,
                                         SubstractExpression,
                                         ParenthesizedExpression,
                                         IdentifierExpression,
                                         IntegerConstantExpression }));
}

void TestParser::case1718()
{
    parseExpression("( x ) + y",
                    Expectation().replicateAmbiguity()
                                 .AST( { AmbiguousCastOrBinaryExpression,
                                         CastExpression,
                                         TypeName,
                                         TypedefName,
                                         AbstractDeclarator,
                                         UnaryPlusExpression,
                                         IdentifierExpression,
                                         AddExpression,
                                         ParenthesizedExpression,
                                         IdentifierExpression,
                                         IdentifierExpression }));
}

void TestParser::case1719()
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

void TestParser::case1720()
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
void TestParser::case1721() {}
void TestParser::case1722() {}
void TestParser::case1723() {}
void TestParser::case1724() {}
void TestParser::case1725() {}
void TestParser::case1726() {}
void TestParser::case1727() {}
void TestParser::case1728() {}
void TestParser::case1729() {}
void TestParser::case1730() {}
void TestParser::case1731() {}
void TestParser::case1732() {}
void TestParser::case1733() {}
void TestParser::case1734() {}
void TestParser::case1735() {}
void TestParser::case1736() {}
void TestParser::case1737() {}
void TestParser::case1738() {}
void TestParser::case1739() {}
void TestParser::case1740() {}
void TestParser::case1741() {}
void TestParser::case1742() {}
void TestParser::case1743() {}
void TestParser::case1744() {}
void TestParser::case1745() {}
void TestParser::case1746() {}
void TestParser::case1747() {}
void TestParser::case1748() {}
void TestParser::case1749() {}

void TestParser::case1750()
{
    parseExpression("x ? y : z",
                    Expectation().AST( { ConditionalExpression,
                                         IdentifierExpression,
                                         IdentifierExpression,
                                         IdentifierExpression } ));

}

void TestParser::case1751()
{
    parseExpression("x ? y + z : w",
                    Expectation().AST( { ConditionalExpression,
                                         IdentifierExpression,
                                         AddExpression,
                                         IdentifierExpression,
                                         IdentifierExpression,
                                         IdentifierExpression } ));
}

void TestParser::case1752()
{
    // See ExtGNU_Conditionals in LanguageExtensions.

    parseExpression("x ?: z",
                    Expectation().AST( { ConditionalExpression,
                                         IdentifierExpression,
                                         IdentifierExpression } ));
}

void TestParser::case1753()
{
}

void TestParser::case1754() {}
void TestParser::case1755() {}
void TestParser::case1756() {}
void TestParser::case1757() {}
void TestParser::case1758() {}
void TestParser::case1759() {}
void TestParser::case1760() {}
void TestParser::case1761() {}
void TestParser::case1762() {}
void TestParser::case1763() {}
void TestParser::case1764() {}
void TestParser::case1765() {}
void TestParser::case1766() {}
void TestParser::case1767() {}
void TestParser::case1768() {}
void TestParser::case1769() {}
void TestParser::case1770() {}
void TestParser::case1771() {}
void TestParser::case1772() {}
void TestParser::case1773() {}
void TestParser::case1774() {}
void TestParser::case1775() {}
void TestParser::case1776() {}
void TestParser::case1777() {}
void TestParser::case1778() {}
void TestParser::case1779() {}
void TestParser::case1780() {}
void TestParser::case1781() {}
void TestParser::case1782() {}
void TestParser::case1783() {}
void TestParser::case1784() {}
void TestParser::case1785() {}
void TestParser::case1786() {}
void TestParser::case1787() {}
void TestParser::case1788() {}
void TestParser::case1789() {}
void TestParser::case1790() {}
void TestParser::case1791() {}
void TestParser::case1792() {}
void TestParser::case1793() {}
void TestParser::case1794() {}
void TestParser::case1795() {}
void TestParser::case1796() {}
void TestParser::case1797() {}
void TestParser::case1798() {}
void TestParser::case1799() {}

void TestParser::case1800()
{
    parseExpression("x / y",
                    Expectation().AST( { DivideExpression,
                                         IdentifierExpression,
                                         IdentifierExpression } ));
}

void TestParser::case1801()
{
    parseExpression("x << y",
                    Expectation().AST( { LeftShiftExpression,
                                         IdentifierExpression,
                                         IdentifierExpression } ));

}

void TestParser::case1802()
{
    parseExpression("x + y",
                    Expectation().AST( { AddExpression,
                                         IdentifierExpression,
                                         IdentifierExpression } ));

}

void TestParser::case1803()
{
    parseExpression("x || y",
                    Expectation().AST( { LogicalORExpression,
                                         IdentifierExpression,
                                         IdentifierExpression } ));
}

void TestParser::case1804()
{
    // No ambiguity here, this is an expression (not an statement).

    parseExpression("x * y",
                    Expectation().AST( { MultiplyExpression,
                                         IdentifierExpression,
                                         IdentifierExpression } ));

}

void TestParser::case1805()
{
    parseExpression("( x * y ) + z",
                    Expectation().AST( { AddExpression,
                                         ParenthesizedExpression,
                                         MultiplyExpression,
                                         IdentifierExpression,
                                         IdentifierExpression,
                                         IdentifierExpression }));
}

void TestParser::case1806()
{
    parseExpression("x * ( y + z)",
                    Expectation().AST( { MultiplyExpression,
                                         IdentifierExpression,
                                         ParenthesizedExpression,
                                         AddExpression,
                                         IdentifierExpression,
                                         IdentifierExpression }));
}

void TestParser::case1807()
{
    parseExpression("x + y * z",
                    Expectation().AST( { AddExpression,
                                         IdentifierExpression,
                                         MultiplyExpression,
                                         IdentifierExpression,
                                         IdentifierExpression }));
}

void TestParser::case1808()
{
    parseExpression("x * y + z",
                    Expectation().AST( { AddExpression,
                                         MultiplyExpression,
                                         IdentifierExpression,
                                         IdentifierExpression,
                                         IdentifierExpression }));
}

void TestParser::case1809()
{
    parseExpression("x , y",
                    Expectation().AST( { SequencingExpression,
                                         IdentifierExpression,
                                         IdentifierExpression }));
}

void TestParser::case1810()
{
    parseExpression("x , 1, y",
                    Expectation().AST( { SequencingExpression,
                                         SequencingExpression,
                                         IdentifierExpression,
                                         IntegerConstantExpression,
                                         IdentifierExpression }));
}

void TestParser::case1811()
{
}
void TestParser::case1812() {}
void TestParser::case1813() {}
void TestParser::case1814() {}
void TestParser::case1815() {}
void TestParser::case1816() {}
void TestParser::case1817() {}
void TestParser::case1818() {}
void TestParser::case1819() {}
void TestParser::case1820() {}
void TestParser::case1821() {}
void TestParser::case1822() {}
void TestParser::case1823() {}
void TestParser::case1824() {}
void TestParser::case1825() {}
void TestParser::case1826() {}
void TestParser::case1827() {}
void TestParser::case1828() {}
void TestParser::case1829() {}
void TestParser::case1830() {}
void TestParser::case1831() {}
void TestParser::case1832() {}
void TestParser::case1833() {}
void TestParser::case1834() {}
void TestParser::case1835() {}
void TestParser::case1836() {}
void TestParser::case1837() {}
void TestParser::case1838() {}
void TestParser::case1839() {}
void TestParser::case1840() {}
void TestParser::case1841() {}
void TestParser::case1842() {}
void TestParser::case1843() {}
void TestParser::case1844() {}
void TestParser::case1845() {}
void TestParser::case1846() {}
void TestParser::case1847() {}
void TestParser::case1848() {}
void TestParser::case1849() {}

void TestParser::case1850()
{
    parseExpression("x = y",
                    Expectation().AST( { BasicAssignmentExpression,
                                         IdentifierExpression,
                                         IdentifierExpression } ));
}

void TestParser::case1851()
{
    parseExpression("x %= y",
                    Expectation().AST( { ModuloAssignmentExpression,
                                         IdentifierExpression,
                                         IdentifierExpression } ));
}

void TestParser::case1852()
{
    parseExpression("x = y = 1",
                    Expectation().AST( { BasicAssignmentExpression,
                                         IdentifierExpression,
                                         BasicAssignmentExpression,
                                         IdentifierExpression,
                                         IntegerConstantExpression } ));

}

void TestParser::case1853()
{
    parseExpression("x = ( y = 1)",
                    Expectation().AST( { BasicAssignmentExpression,
                                         IdentifierExpression,
                                         ParenthesizedExpression,
                                         BasicAssignmentExpression,
                                         IdentifierExpression,
                                         IntegerConstantExpression } ));
}

void TestParser::case1854()
{
    parseExpression("x = 0",
                    Expectation().AST( { BasicAssignmentExpression,
                                         IdentifierExpression,
                                         IntegerConstantExpression } ));
}

void TestParser::case1855()
{
    parseExpression("x = ( void * ) 0",
                    Expectation().AST( { BasicAssignmentExpression,
                                         IdentifierExpression,
                                         CastExpression,
                                         TypeName,
                                         BuiltinTypeSpecifier,
                                         PointerDeclarator,
                                         AbstractDeclarator,
                                         IntegerConstantExpression} ));
}

void TestParser::case1856()
{
    parseExpression("x = ( ( void * ) 0 )",
                    Expectation().AST( { BasicAssignmentExpression,
                                         IdentifierExpression,
                                         ParenthesizedExpression,
                                         CastExpression,
                                         TypeName,
                                         BuiltinTypeSpecifier,
                                         PointerDeclarator,
                                         AbstractDeclarator,
                                         IntegerConstantExpression} ));
}

void TestParser::case1857() {}
void TestParser::case1858() {}
void TestParser::case1859() {}
void TestParser::case1860() {}
void TestParser::case1861() {}
void TestParser::case1862() {}
void TestParser::case1863() {}
void TestParser::case1864() {}
void TestParser::case1865() {}
void TestParser::case1866() {}
void TestParser::case1867() {}
void TestParser::case1868() {}
void TestParser::case1869() {}
void TestParser::case1870() {}
void TestParser::case1871() {}
void TestParser::case1872() {}
void TestParser::case1873() {}
void TestParser::case1874() {}
void TestParser::case1875() {}
void TestParser::case1876() {}
void TestParser::case1877() {}
void TestParser::case1878() {}
void TestParser::case1879() {}
void TestParser::case1880() {}
void TestParser::case1881() {}
void TestParser::case1882() {}
void TestParser::case1883() {}
void TestParser::case1884() {}
void TestParser::case1885() {}
void TestParser::case1886() {}
void TestParser::case1887() {}
void TestParser::case1888() {}
void TestParser::case1889() {}
void TestParser::case1890() {}
void TestParser::case1891() {}
void TestParser::case1892() {}
void TestParser::case1893() {}
void TestParser::case1894() {}
void TestParser::case1895() {}
void TestParser::case1896() {}
void TestParser::case1897() {}
void TestParser::case1898() {}
void TestParser::case1899() {}

void TestParser::case1900()
{
    // https://gcc.gnu.org/onlinedocs/gcc/Statement-Exprs.html#Statement-Exprs

    parseExpression("({ int x; })");
}

void TestParser::case1901()
{
    // https://gcc.gnu.org/onlinedocs/gcc/Statement-Exprs.html#Statement-Exprs

    parseExpression("({ int x; x; })");
}

void TestParser::case1902()
{
    // https://gcc.gnu.org/onlinedocs/gcc/Statement-Exprs.html#Statement-Exprs

    //parseExpression("({ x y; })");
}

void TestParser::case1903()
{
    // https://gcc.gnu.org/onlinedocs/gcc/Statement-Exprs.html#Statement-Exprs

    // parseExpression("({ x y; y; })");
}

void TestParser::case1904() {}
void TestParser::case1905() {}
void TestParser::case1906() {}
void TestParser::case1907() {}
void TestParser::case1908() {}
void TestParser::case1909() {}
void TestParser::case1910() {}
void TestParser::case1911() {}
void TestParser::case1912() {}
void TestParser::case1913() {}
void TestParser::case1914() {}
void TestParser::case1915() {}
void TestParser::case1916() {}
void TestParser::case1917() {}
void TestParser::case1918() {}
void TestParser::case1919() {}
void TestParser::case1920() {}
void TestParser::case1921() {}
void TestParser::case1922() {}
void TestParser::case1923() {}
void TestParser::case1924() {}
void TestParser::case1925() {}
void TestParser::case1926() {}
void TestParser::case1927() {}
void TestParser::case1928() {}
void TestParser::case1929() {}
void TestParser::case1930() {}
void TestParser::case1931() {}
void TestParser::case1932() {}
void TestParser::case1933() {}
void TestParser::case1934() {}
void TestParser::case1935() {}
void TestParser::case1936() {}
void TestParser::case1937() {}
void TestParser::case1938() {}
void TestParser::case1939() {}
void TestParser::case1940() {}
void TestParser::case1941() {}
void TestParser::case1942() {}
void TestParser::case1943() {}
void TestParser::case1944() {}
void TestParser::case1945() {}
void TestParser::case1946() {}
void TestParser::case1947() {}
void TestParser::case1948() {}
void TestParser::case1949() {}
void TestParser::case1950() {}
void TestParser::case1951() {}
void TestParser::case1952() {}
void TestParser::case1953() {}
void TestParser::case1954() {}
void TestParser::case1955() {}
void TestParser::case1956() {}
void TestParser::case1957() {}
void TestParser::case1958() {}
void TestParser::case1959() {}
void TestParser::case1960() {}
void TestParser::case1961() {}
void TestParser::case1962() {}
void TestParser::case1963() {}
void TestParser::case1964() {}
void TestParser::case1965() {}
void TestParser::case1966() {}
void TestParser::case1967() {}
void TestParser::case1968() {}
void TestParser::case1969() {}
void TestParser::case1970() {}
void TestParser::case1971() {}
void TestParser::case1972() {}
void TestParser::case1973() {}
void TestParser::case1974() {}
void TestParser::case1975() {}
void TestParser::case1976() {}
void TestParser::case1977() {}
void TestParser::case1978() {}
void TestParser::case1979() {}
void TestParser::case1980() {}
void TestParser::case1981() {}
void TestParser::case1982() {}
void TestParser::case1983() {}
void TestParser::case1984() {}
void TestParser::case1985() {}
void TestParser::case1986() {}
void TestParser::case1987() {}
void TestParser::case1988() {}
void TestParser::case1989() {}
void TestParser::case1990() {}
void TestParser::case1991() {}
void TestParser::case1992() {}
void TestParser::case1993() {}
void TestParser::case1994() {}
void TestParser::case1995() {}
void TestParser::case1996() {}
void TestParser::case1997() {}
void TestParser::case1998() {}
void TestParser::case1999() {}


