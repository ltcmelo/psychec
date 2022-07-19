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

void ParserTester::case1000()
{
    parseExpression("1",
                    Expectation().AST( { IntegerConstantExpression }));
}

void ParserTester::case1001()
{
    parseExpression(".1",
                    Expectation().AST( { FloatingConstantExpression }));
}

void ParserTester::case1002()
{
    parseExpression("1.",
                    Expectation().AST( { FloatingConstantExpression }));
}

void ParserTester::case1003()
{
    parseExpression("1.2",
                    Expectation().AST( { FloatingConstantExpression }));
}

void ParserTester::case1004()
{
    parseExpression("1.2e3",
                    Expectation().AST( { FloatingConstantExpression }));
}

void ParserTester::case1005()
{
    parseExpression("1.2E3",
                    Expectation().AST( { FloatingConstantExpression }));
}

void ParserTester::case1006()
{
    parseExpression("1.2e+3",
                    Expectation().AST( { FloatingConstantExpression }));
}

void ParserTester::case1007()
{
    parseExpression("1.2E+3",
                    Expectation().AST( { FloatingConstantExpression }));
}

void ParserTester::case1008()
{
    parseExpression("1.2e-3",
                    Expectation().AST( { FloatingConstantExpression }));
}

void ParserTester::case1009()
{
    parseExpression("1.2E-3",
                    Expectation().AST( { FloatingConstantExpression }));
}

void ParserTester::case1010()
{
    parseExpression("1e2",
                    Expectation().AST( { FloatingConstantExpression }));
}

void ParserTester::case1011()
{
    parseExpression("1E2",
                    Expectation().AST( { FloatingConstantExpression }));
}

void ParserTester::case1012()
{
    parseExpression("1e+2",
                    Expectation().AST( { FloatingConstantExpression }));
}

void ParserTester::case1013()
{
    parseExpression("1e-2",
                    Expectation().AST( { FloatingConstantExpression }));
}

void ParserTester::case1014()
{
    parseExpression("1E+2",
                    Expectation().AST( { FloatingConstantExpression }));
}

void ParserTester::case1015()
{
    parseExpression("1E-2",
                    Expectation().AST( { FloatingConstantExpression }));
}

void ParserTester::case1016()
{
    parseExpression("1E+2",
                    Expectation().AST( { FloatingConstantExpression }));
}

void ParserTester::case1017()
{
    parseExpression("1.e2",
                    Expectation().AST( { FloatingConstantExpression }));
}

void ParserTester::case1018()
{
    parseExpression("1.E2",
                    Expectation().AST( { FloatingConstantExpression }));
}

void ParserTester::case1019()
{
    parseExpression("0",
                    Expectation().AST( { IntegerConstantExpression }));
}

void ParserTester::case1020()
{
    parseExpression("0L",
                    Expectation().AST( { IntegerConstantExpression }));
}

void ParserTester::case1021()
{
    parseExpression("int",
                    Expectation().diagnostic(Expectation::ErrorOrWarn::Error,
                                                Parser::DiagnosticsReporter::ID_of_ExpectedFIRSTofExpression));
}

void ParserTester::case1022()
{
    parseExpression("{",
                    Expectation().diagnostic(Expectation::ErrorOrWarn::Error,
                                                Parser::DiagnosticsReporter::ID_of_ExpectedFIRSTofExpression));
}

void ParserTester::case1023()
{
    parseExpression("0x1.2p3",
                    Expectation().AST( { FloatingConstantExpression }));
}

void ParserTester::case1024()
{
    parseExpression("0x1.ap2",
                    Expectation().AST( { FloatingConstantExpression }));
}

void ParserTester::case1025()
{
    parseExpression("0x1.Ap2",
                    Expectation().AST( { FloatingConstantExpression }));
}

void ParserTester::case1026()
{
    parseExpression("0x1.fp2",
                    Expectation().AST( { FloatingConstantExpression }));
}

void ParserTester::case1027()
{
    parseExpression("0x1.Fp2",
                    Expectation().AST( { FloatingConstantExpression }));
}

void ParserTester::case1028()
{
    parseExpression("0x1.2p3f",
                    Expectation().AST( { FloatingConstantExpression }));
}

void ParserTester::case1029()
{
    parseExpression("0x1.2p3F",
                    Expectation().AST( { FloatingConstantExpression }));
}

void ParserTester::case1030()
{
    parseExpression("x",
                    Expectation().AST( { IdentifierName }));
}

void ParserTester::case1031()
{
    parseExpression("_x",
                    Expectation().AST( { IdentifierName }));
}

void ParserTester::case1032()
{
    parseExpression("x_y",
                    Expectation().AST( { IdentifierName }));
}

void ParserTester::case1033()
{
    parseExpression("x1",
                    Expectation().AST( { IdentifierName }));
}

void ParserTester::case1034()
{
    parseExpression("x1_",
                    Expectation().AST( { IdentifierName }));
}

void ParserTester::case1035()
{
    parseExpression("x1y2",
                    Expectation().AST( { IdentifierName }));
}

void ParserTester::case1036()
{
    parseExpression("_x1y2_",
                    Expectation().AST( { IdentifierName }));
}

void ParserTester::case1037()
{
    parseExpression("X",
                    Expectation().AST( { IdentifierName }));
}

void ParserTester::case1038()
{
    parseExpression("_X",
                    Expectation().AST( { IdentifierName }));
}

void ParserTester::case1039()
{
    parseExpression("X1",
                    Expectation().AST( { IdentifierName }));
}

void ParserTester::case1040()
{
    parseExpression("_X1",
                    Expectation().AST( { IdentifierName }));
}

void ParserTester::case1041()
{
    parseExpression("xYz1",
                    Expectation().AST( { IdentifierName }));
}

void ParserTester::case1042()
{
    parseExpression("xYz1_",
                    Expectation().AST( { IdentifierName }));
}

void ParserTester::case1043()
{
    parseExpression("xY_z1",
                    Expectation().AST( { IdentifierName }));
}

void ParserTester::case1044()
{
    parseExpression("1l",
                    Expectation().AST( { IntegerConstantExpression }));
}

void ParserTester::case1045()
{
    parseExpression("1ll",
                    Expectation().AST( { IntegerConstantExpression }));
}

void ParserTester::case1046()
{
    parseExpression("1L",
                    Expectation().AST( { IntegerConstantExpression }));
}

void ParserTester::case1047()
{
    parseExpression("1LL",
                    Expectation().AST( { IntegerConstantExpression }));
}

void ParserTester::case1048()
{
    parseExpression("1u",
                    Expectation().AST( { IntegerConstantExpression }));
}

void ParserTester::case1049()
{
    parseExpression("1U",
                    Expectation().AST( { IntegerConstantExpression }));
}

void ParserTester::case1050()
{
    parseExpression("1uL",
                    Expectation().AST( { IntegerConstantExpression }));
}

void ParserTester::case1051()
{
    parseExpression("1ul",
                    Expectation().AST( { IntegerConstantExpression }));
}

void ParserTester::case1052()
{
    parseExpression("0x1u",
                    Expectation().AST( { IntegerConstantExpression }));
}

void ParserTester::case1053()
{
    parseExpression("0x1Ul",
                    Expectation().AST( { IntegerConstantExpression }));
}

void ParserTester::case1054()
{
    parseExpression("L'x'",
                    Expectation().AST( { CharacterConstantExpression }));
}

void ParserTester::case1055()
{
    parseExpression("u'x'",
                    Expectation().AST( { CharacterConstantExpression }));
}

void ParserTester::case1056()
{
    parseExpression("U'x'",
                    Expectation().AST( { CharacterConstantExpression }));
}

void ParserTester::case1057()
{
    parseExpression("'x'",
                    Expectation().AST( { CharacterConstantExpression }));
}

void ParserTester::case1058()
{
    parseExpression("u\"xyz\"",
                    Expectation().AST( { StringLiteralExpression }));
}

void ParserTester::case1059()
{
    parseExpression("\"xyz\"",
                    Expectation().AST( { StringLiteralExpression }));
}

void ParserTester::case1060()
{
    parseExpression("\"x\" \"y\"",
                    Expectation().AST( { StringLiteralExpression,
                                         StringLiteralExpression}));
}

void ParserTester::case1061()
{
    parseExpression("1Lu",
                    Expectation().AST( { IntegerConstantExpression }));

}

void ParserTester::case1062()
{
    parseExpression("0x1lU",
                    Expectation().AST( { IntegerConstantExpression }));
}

void ParserTester::case1063()
{
    parseExpression("0x1lUll",
                    Expectation().diagnostic(
                        Expectation::ErrorOrWarn::Error,
                        Parser::DiagnosticsReporter::ID_of_ExpectedFIRSTofExpression));
}

void ParserTester::case1064()
{
    parseExpression("1lul",
                    Expectation().diagnostic(
                        Expectation::ErrorOrWarn::Error,
                        Parser::DiagnosticsReporter::ID_of_ExpectedFIRSTofExpression));
}

void ParserTester::case1065()
{
    parseExpression("1ulu",
                    Expectation().diagnostic(
                        Expectation::ErrorOrWarn::Error,
                        Parser::DiagnosticsReporter::ID_of_ExpectedFIRSTofExpression));
}

void ParserTester::case1066()
{
    parseExpression("1ullu",
                    Expectation().diagnostic(
                        Expectation::ErrorOrWarn::Error,
                        Parser::DiagnosticsReporter::ID_of_ExpectedFIRSTofExpression));
}

void ParserTester::case1067()
{
    parseExpression("1uu",
                    Expectation().diagnostic(
                        Expectation::ErrorOrWarn::Error,
                        Parser::DiagnosticsReporter::ID_of_ExpectedFIRSTofExpression));
}

void ParserTester::case1068()
{
    parseExpression("1lll",
                    Expectation().diagnostic(
                        Expectation::ErrorOrWarn::Error,
                        Parser::DiagnosticsReporter::ID_of_ExpectedFIRSTofExpression));
}

void ParserTester::case1069()
{
    parseExpression(".1f",
                    Expectation().AST( { FloatingConstantExpression }));
}

void ParserTester::case1070()
{
    parseExpression(".1l",
                    Expectation().AST( { FloatingConstantExpression }));
}

void ParserTester::case1071()
{
    parseExpression("1i",
                    Expectation().AST( { ImaginaryIntegerConstantExpression }));
}

void ParserTester::case1072()
{
    parseExpression(".1i",
                    Expectation().AST( { ImaginaryFloatingConstantExpression }));
}

void ParserTester::case1073()
{
    parseExpression("1.i",
                    Expectation().AST( { ImaginaryFloatingConstantExpression }));
}

void ParserTester::case1074()
{
    parseExpression("1.2i",
                    Expectation().AST( { ImaginaryFloatingConstantExpression }));
}

void ParserTester::case1075()
{
    parseExpression("1.2e3i",
                    Expectation().AST( { ImaginaryFloatingConstantExpression }));
}

void ParserTester::case1076()
{
    parseExpression("1uli",
                    Expectation().AST( { ImaginaryIntegerConstantExpression }));
}

void ParserTester::case1077()
{
    parseExpression("1j",
                    Expectation().AST( { ImaginaryIntegerConstantExpression }));
}

void ParserTester::case1078()
{
    parseExpression(".1j",
                    Expectation().AST( { ImaginaryFloatingConstantExpression }));
}

void ParserTester::case1079()
{
    parseExpression("1.j",
                    Expectation().AST( { ImaginaryFloatingConstantExpression }));
}

void ParserTester::case1080()
{
    parseExpression("1.2j",
                    Expectation().AST( { ImaginaryFloatingConstantExpression }));
}

void ParserTester::case1081()
{
    parseExpression("1.2e3j",
                    Expectation().AST( { ImaginaryFloatingConstantExpression }));
}

void ParserTester::case1082()
{
    parseExpression("1ulj",
                    Expectation().AST( { ImaginaryIntegerConstantExpression }));
}

void ParserTester::case1083()
{
    parseExpression("1iu",
                    Expectation().AST( { ImaginaryIntegerConstantExpression }));
}

void ParserTester::case1084()
{
    parseExpression("1ju",
                    Expectation().AST( { ImaginaryIntegerConstantExpression }));
}

void ParserTester::case1085()
{
    parseExpression("1.2if",
                    Expectation().AST( { ImaginaryFloatingConstantExpression }));
}

void ParserTester::case1086()
{
    parseExpression("1.2jf",
                    Expectation().AST( { ImaginaryFloatingConstantExpression }));
}

void ParserTester::case1087()
{
    parseExpression("1.2e3if",
                    Expectation().AST( { ImaginaryFloatingConstantExpression }));
}

void ParserTester::case1088()
{
    parseExpression("1.2e3jf",
                    Expectation().AST( { ImaginaryFloatingConstantExpression }));
}

void ParserTester::case1089()
{
    parseExpression("1ii",
                    Expectation().diagnostic(
                        Expectation::ErrorOrWarn::Error,
                        Parser::DiagnosticsReporter::ID_of_ExpectedFIRSTofExpression));
}

void ParserTester::case1090()
{
    parseExpression("1jj",
                    Expectation().diagnostic(
                        Expectation::ErrorOrWarn::Error,
                        Parser::DiagnosticsReporter::ID_of_ExpectedFIRSTofExpression));
}

void ParserTester::case1091()
{
    parseExpression("1ij",
                    Expectation().diagnostic(
                        Expectation::ErrorOrWarn::Error,
                        Parser::DiagnosticsReporter::ID_of_ExpectedFIRSTofExpression));
}

void ParserTester::case1092()
{
    parseExpression("1ji",
                    Expectation().diagnostic(
                        Expectation::ErrorOrWarn::Error,
                        Parser::DiagnosticsReporter::ID_of_ExpectedFIRSTofExpression));
}

void ParserTester::case1093()
{
    parseExpression("1uliul",
                    Expectation().diagnostic(
                        Expectation::ErrorOrWarn::Error,
                        Parser::DiagnosticsReporter::ID_of_ExpectedFIRSTofExpression));
}

void ParserTester::case1094()
{
    parseExpression("1uljul",
                    Expectation().diagnostic(
                        Expectation::ErrorOrWarn::Error,
                        Parser::DiagnosticsReporter::ID_of_ExpectedFIRSTofExpression));
}

void ParserTester::case1095()
{
    parseExpression(".1fllf",
                    Expectation().diagnostic(
                        Expectation::ErrorOrWarn::Error,
                        Parser::DiagnosticsReporter::ID_of_ExpectedFIRSTofExpression));
}

void ParserTester::case1096()
{
    parseExpression("0.1fllf",
                    Expectation().diagnostic(
                        Expectation::ErrorOrWarn::Error,
                        Parser::DiagnosticsReporter::ID_of_ExpectedFIRSTofExpression));
}

void ParserTester::case1097() {}
void ParserTester::case1098() {}
void ParserTester::case1099() {}

void ParserTester::case1100()
{
    parseExpression("( 1 )",
                    Expectation().AST( { ParenthesizedExpression,
                                         IntegerConstantExpression }));
}

void ParserTester::case1101()
{
    parseExpression("( x )",
                    Expectation().AST( { ParenthesizedExpression,
                                         IdentifierName }));
}

void ParserTester::case1102()
{
    parseExpression("( ( x ) )",
                    Expectation().AST( { ParenthesizedExpression,
                                         ParenthesizedExpression,
                                         IdentifierName }));
}

void ParserTester::case1103()
{
    parseExpression("( 'x' )",
                    Expectation().AST( { ParenthesizedExpression,
                                         CharacterConstantExpression }));
}

void ParserTester::case1104()
{
    parseExpression("( x",
                    Expectation().diagnostic(Expectation::ErrorOrWarn::Error,
                                             Parser::DiagnosticsReporter::ID_of_ExpectedToken));
}

void ParserTester::case1105()
{
    parseExpression("( ( x )",
                    Expectation().diagnostic(Expectation::ErrorOrWarn::Error,
                                             Parser::DiagnosticsReporter::ID_of_ExpectedToken));
}

void ParserTester::case1106()
{
    parseExpression("( int",
                    Expectation().diagnostic(Expectation::ErrorOrWarn::Error,
                                             Parser::DiagnosticsReporter::ID_of_ExpectedToken));
}

void ParserTester::case1107()
{
    parseExpression("( int )",
                    Expectation().diagnostic(Expectation::ErrorOrWarn::Error,
                                             Parser::DiagnosticsReporter::ID_of_ExpectedFIRSTofExpression));
}

void ParserTester::case1108() {}
void ParserTester::case1109() {}
void ParserTester::case1110() {}
void ParserTester::case1111() {}
void ParserTester::case1112() {}
void ParserTester::case1113() {}
void ParserTester::case1114() {}
void ParserTester::case1115() {}
void ParserTester::case1116() {}
void ParserTester::case1117() {}
void ParserTester::case1118() {}
void ParserTester::case1119() {}
void ParserTester::case1120() {}
void ParserTester::case1121() {}
void ParserTester::case1122() {}
void ParserTester::case1123() {}
void ParserTester::case1124() {}
void ParserTester::case1125() {}
void ParserTester::case1126() {}
void ParserTester::case1127() {}
void ParserTester::case1128() {}
void ParserTester::case1129() {}
void ParserTester::case1130() {}
void ParserTester::case1131() {}
void ParserTester::case1132() {}
void ParserTester::case1133() {}
void ParserTester::case1134() {}
void ParserTester::case1135() {}
void ParserTester::case1136() {}
void ParserTester::case1137() {}
void ParserTester::case1138() {}
void ParserTester::case1139() {}
void ParserTester::case1140() {}
void ParserTester::case1141() {}
void ParserTester::case1142() {}
void ParserTester::case1143() {}
void ParserTester::case1144() {}
void ParserTester::case1145() {}
void ParserTester::case1146() {}
void ParserTester::case1147() {}
void ParserTester::case1148() {}
void ParserTester::case1149() {}
void ParserTester::case1150() {}
void ParserTester::case1151() {}
void ParserTester::case1152() {}
void ParserTester::case1153() {}
void ParserTester::case1154() {}
void ParserTester::case1155() {}
void ParserTester::case1156() {}
void ParserTester::case1157() {}
void ParserTester::case1158() {}
void ParserTester::case1159() {}
void ParserTester::case1160() {}
void ParserTester::case1161() {}
void ParserTester::case1162() {}
void ParserTester::case1163() {}
void ParserTester::case1164() {}
void ParserTester::case1165() {}
void ParserTester::case1166() {}
void ParserTester::case1167() {}
void ParserTester::case1168() {}
void ParserTester::case1169() {}
void ParserTester::case1170() {}
void ParserTester::case1171() {}
void ParserTester::case1172() {}
void ParserTester::case1173() {}
void ParserTester::case1174() {}
void ParserTester::case1175() {}
void ParserTester::case1176() {}
void ParserTester::case1177() {}
void ParserTester::case1178() {}
void ParserTester::case1179() {}
void ParserTester::case1180() {}
void ParserTester::case1181() {}
void ParserTester::case1182() {}
void ParserTester::case1183() {}
void ParserTester::case1184() {}
void ParserTester::case1185() {}
void ParserTester::case1186() {}
void ParserTester::case1187() {}
void ParserTester::case1188() {}
void ParserTester::case1189() {}
void ParserTester::case1190() {}
void ParserTester::case1191() {}
void ParserTester::case1192() {}
void ParserTester::case1193() {}
void ParserTester::case1194() {}
void ParserTester::case1195() {}
void ParserTester::case1196() {}
void ParserTester::case1197() {}
void ParserTester::case1198() {}
void ParserTester::case1199() {}

void ParserTester::case1200()
{
    parseExpression(R"(
                    _Generic( x,
                             default: cbrt)
                    )");
}

void ParserTester::case1201()
{
    parseExpression(R"(
                    _Generic( x,
                             long double: cbrtl,
                             default: cbrt,
                             float: cbrtf)
                    )");
}

void ParserTester::case1202()
{
    parseExpression(R"(
                    _Generic( x,
                             float: cbrtf)
                    )");
}

void ParserTester::case1203()
{
    parseExpression(R"(
                    _Generic( ( x ),
                             long double: cbrtl,
                             default: cbrt)
                    )");
}

void ParserTester::case1204()
{
    parseExpression(R"(
                    _Generic( x float: cbrtf)
                    )",
                    Expectation().diagnostic(Expectation::ErrorOrWarn::Error,
                                             Parser::DiagnosticsReporter::ID_of_ExpectedToken));
}

void ParserTester::case1205()
{
    parseExpression("_Generic( x, float, cbrtf)",
                    Expectation().diagnostic(Expectation::ErrorOrWarn::Error,
                                             Parser::DiagnosticsReporter::ID_of_ExpectedToken));
}

void ParserTester::case1206()
{
    parseExpression("_Generic( x : cbrtf)",
                    Expectation().diagnostic(Expectation::ErrorOrWarn::Error,
                                             Parser::DiagnosticsReporter::ID_of_ExpectedToken));
}

void ParserTester::case1207()
{
    parseExpression("_Generic(float: cbrtf)",
                    Expectation().diagnostic(Expectation::ErrorOrWarn::Error,
                                             Parser::DiagnosticsReporter::ID_of_ExpectedFIRSTofExpression));
}

void ParserTester::case1208()
{
    parseExpression(R"(
                    _Generic( x,
                             int: y,
                             char*: z,
                             const char*: w)
                    )");
}

void ParserTester::case1209()
{
    parseExpression(R"(
                    _Generic( x,
                             default: cbrt
                    )",
                    Expectation().setErrorCnt(1));
}

void ParserTester::case1210()
{
    parseExpression(R"(
                    _Generic( x,
                             default:
                    )",
                    Expectation().setErrorCnt(1));
}

void ParserTester::case1211()
{
    parseExpression(R"(
                    _Generic( ( x ), int : 1,)
                    )",
                    Expectation().setErrorCnt(1));
}

void ParserTester::case1212() {}
void ParserTester::case1213() {}
void ParserTester::case1214() {}
void ParserTester::case1215() {}
void ParserTester::case1216() {}
void ParserTester::case1217() {}
void ParserTester::case1218() {}
void ParserTester::case1219() {}
void ParserTester::case1220() {}
void ParserTester::case1221() {}
void ParserTester::case1222() {}
void ParserTester::case1223() {}
void ParserTester::case1224() {}
void ParserTester::case1225() {}
void ParserTester::case1226() {}
void ParserTester::case1227() {}
void ParserTester::case1228() {}
void ParserTester::case1229() {}
void ParserTester::case1230() {}
void ParserTester::case1231() {}
void ParserTester::case1232() {}
void ParserTester::case1233() {}
void ParserTester::case1234() {}
void ParserTester::case1235() {}
void ParserTester::case1236() {}
void ParserTester::case1237() {}
void ParserTester::case1238() {}
void ParserTester::case1239() {}
void ParserTester::case1240() {}
void ParserTester::case1241() {}
void ParserTester::case1242() {}
void ParserTester::case1243() {}
void ParserTester::case1244() {}
void ParserTester::case1245() {}
void ParserTester::case1246() {}
void ParserTester::case1247() {}
void ParserTester::case1248() {}
void ParserTester::case1249() {}

void ParserTester::case1250()
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

void ParserTester::case1251()
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

void ParserTester::case1252()
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

void ParserTester::case1253()
{
    parseExpression("( struct x ) {",
                    Expectation().diagnostic(Expectation::ErrorOrWarn::Error,
                                             Parser::DiagnosticsReporter::ID_of_ExpectedFIRSTofExpression));
}

void ParserTester::case1254()
{
    parseExpression("( struct x ) { 1,",
                    Expectation().diagnostic(Expectation::ErrorOrWarn::Error,
                                             Parser::DiagnosticsReporter::ID_of_ExpectedFIRSTofExpression));
}

void ParserTester::case1255()
{
    parseExpression("( struct x ) { 1",
                    Expectation().diagnostic(Expectation::ErrorOrWarn::Error,
                                             Parser::DiagnosticsReporter::ID_of_ExpectedToken));
}

void ParserTester::case1256()
{
    parseExpression("( long ) {",
                    Expectation().diagnostic(Expectation::ErrorOrWarn::Error,
                                             Parser::DiagnosticsReporter::ID_of_ExpectedFIRSTofExpression));
}

void ParserTester::case1257()
{
}

void ParserTester::case1258()
{
}

void ParserTester::case1259() {}
void ParserTester::case1260() {}
void ParserTester::case1261() {}
void ParserTester::case1262() {}
void ParserTester::case1263() {}
void ParserTester::case1264() {}
void ParserTester::case1265() {}
void ParserTester::case1266() {}
void ParserTester::case1267() {}
void ParserTester::case1268() {}
void ParserTester::case1269() {}
void ParserTester::case1270() {}
void ParserTester::case1271() {}
void ParserTester::case1272() {}
void ParserTester::case1273() {}
void ParserTester::case1274() {}
void ParserTester::case1275() {}
void ParserTester::case1276() {}
void ParserTester::case1277() {}
void ParserTester::case1278() {}
void ParserTester::case1279() {}
void ParserTester::case1280() {}
void ParserTester::case1281() {}
void ParserTester::case1282() {}
void ParserTester::case1283() {}
void ParserTester::case1284() {}
void ParserTester::case1285() {}
void ParserTester::case1286() {}
void ParserTester::case1287() {}
void ParserTester::case1288() {}
void ParserTester::case1289() {}
void ParserTester::case1290() {}
void ParserTester::case1291() {}
void ParserTester::case1292() {}
void ParserTester::case1293() {}
void ParserTester::case1294() {}
void ParserTester::case1295() {}
void ParserTester::case1296() {}
void ParserTester::case1297() {}
void ParserTester::case1298() {}
void ParserTester::case1299() {}

void ParserTester::case1300()
{
    parseExpression("++x",
                    Expectation().AST({ PreIncrementExpression,
                                        IdentifierName }));
}

void ParserTester::case1301()
{
    parseExpression("--x",
                    Expectation().AST({ PreDecrementExpression,
                                        IdentifierName }));;
}

void ParserTester::case1302()
{
    parseExpression("x++",
                    Expectation().AST({ PostIncrementExpression,
                                        IdentifierName }));
}

void ParserTester::case1303()
{
    parseExpression("x--",
                    Expectation().AST({ PostDecrementExpression,
                                        IdentifierName }));
}

void ParserTester::case1304()
{
    parseExpression("++( x )",
                    Expectation().AST({ PreIncrementExpression,
                                        ParenthesizedExpression,
                                        IdentifierName }));
}

void ParserTester::case1305()
{
    parseExpression("++( ( x ))",
                    Expectation().AST({ PreIncrementExpression,
                                        ParenthesizedExpression,
                                        ParenthesizedExpression,
                                        IdentifierName }));
}

void ParserTester::case1306()
{
    parseExpression("( x )++",
                    Expectation().AST({ PostIncrementExpression,
                                        ParenthesizedExpression,
                                        IdentifierName }));
}

void ParserTester::case1307()
{
    parseExpression("( ( x ))++",
                    Expectation().AST({ PostIncrementExpression,
                                        ParenthesizedExpression,
                                        ParenthesizedExpression,
                                        IdentifierName }));
}

void ParserTester::case1308()
{
    parseExpression("x +",
                    Expectation().diagnostic(Expectation::ErrorOrWarn::Error,
                                             Parser::DiagnosticsReporter::ID_of_ExpectedFIRSTofExpression));
}

void ParserTester::case1309() {}
void ParserTester::case1310() {}
void ParserTester::case1311() {}
void ParserTester::case1312() {}
void ParserTester::case1313() {}
void ParserTester::case1314() {}
void ParserTester::case1315() {}
void ParserTester::case1316() {}
void ParserTester::case1317() {}
void ParserTester::case1318() {}
void ParserTester::case1319() {}
void ParserTester::case1320() {}
void ParserTester::case1321() {}
void ParserTester::case1322() {}
void ParserTester::case1323() {}
void ParserTester::case1324() {}
void ParserTester::case1325() {}
void ParserTester::case1326() {}
void ParserTester::case1327() {}
void ParserTester::case1328() {}
void ParserTester::case1329() {}
void ParserTester::case1330() {}
void ParserTester::case1331() {}
void ParserTester::case1332() {}
void ParserTester::case1333() {}
void ParserTester::case1334() {}
void ParserTester::case1335() {}
void ParserTester::case1336() {}
void ParserTester::case1337() {}
void ParserTester::case1338() {}
void ParserTester::case1339() {}
void ParserTester::case1340() {}
void ParserTester::case1341() {}
void ParserTester::case1342() {}
void ParserTester::case1343() {}
void ParserTester::case1344() {}
void ParserTester::case1345() {}
void ParserTester::case1346() {}
void ParserTester::case1347() {}
void ParserTester::case1348() {}
void ParserTester::case1349() {}

void ParserTester::case1350()
{
    parseExpression("-1",
                    Expectation().AST( { UnaryMinusExpression,
                                         IntegerConstantExpression }));
}

void ParserTester::case1351()
{
    parseExpression("+1",
                    Expectation().AST( { UnaryPlusExpression,
                                         IntegerConstantExpression }));
}

void ParserTester::case1352()
{
    parseExpression("-1.2",
                    Expectation().AST( { UnaryMinusExpression,
                                         FloatingConstantExpression }));
}

void ParserTester::case1353()
{
    parseExpression("-.1",
                    Expectation().AST( { UnaryMinusExpression,
                                         FloatingConstantExpression }));
}

void ParserTester::case1354()
{
    parseExpression("-1.2e3",
                    Expectation().AST( { UnaryMinusExpression,
                                         FloatingConstantExpression }));
}

void ParserTester::case1355() {}
void ParserTester::case1356() {}
void ParserTester::case1357() {}
void ParserTester::case1358() {}
void ParserTester::case1359() {}
void ParserTester::case1360() {}
void ParserTester::case1361() {}
void ParserTester::case1362() {}
void ParserTester::case1363() {}
void ParserTester::case1364() {}
void ParserTester::case1365() {}
void ParserTester::case1366() {}
void ParserTester::case1367() {}
void ParserTester::case1368() {}
void ParserTester::case1369() {}
void ParserTester::case1370() {}
void ParserTester::case1371() {}
void ParserTester::case1372() {}
void ParserTester::case1373() {}
void ParserTester::case1374() {}
void ParserTester::case1375() {}
void ParserTester::case1376() {}
void ParserTester::case1377() {}
void ParserTester::case1378() {}
void ParserTester::case1379() {}
void ParserTester::case1380() {}
void ParserTester::case1381() {}
void ParserTester::case1382() {}
void ParserTester::case1383() {}
void ParserTester::case1384() {}
void ParserTester::case1385() {}
void ParserTester::case1386() {}
void ParserTester::case1387() {}
void ParserTester::case1388() {}
void ParserTester::case1389() {}
void ParserTester::case1390() {}
void ParserTester::case1391() {}
void ParserTester::case1392() {}
void ParserTester::case1393() {}
void ParserTester::case1394() {}
void ParserTester::case1395() {}
void ParserTester::case1396() {}
void ParserTester::case1397() {}
void ParserTester::case1398() {}
void ParserTester::case1399() {}

void ParserTester::case1400()
{
    parseExpression("sizeof x",
                    Expectation().AST( { SizeofExpression,
                                         ExpressionAsTypeReference,
                                         IdentifierName }));
}

void ParserTester::case1401()
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

void ParserTester::case1402()
{
    parseExpression("sizeof ( const x )",
                    Expectation().AST( { SizeofExpression,
                                         TypeNameAsTypeReference,
                                         TypeName,
                                         ConstQualifier,
                                         TypedefName,
                                         AbstractDeclarator }));
}

void ParserTester::case1403()
{
    parseExpression("sizeof ( x * )",
                    Expectation().AST( { SizeofExpression,
                                         TypeNameAsTypeReference,
                                         TypeName,
                                         TypedefName,
                                         PointerDeclarator,
                                         AbstractDeclarator }));
}

void ParserTester::case1404()
{
    parseExpression("sizeof int",
                    Expectation().diagnostic(Expectation::ErrorOrWarn::Error,
                                                Parser::DiagnosticsReporter::ID_of_ExpectedFIRSTofExpression));
}

void ParserTester::case1405()
{
    parseExpression("sizeof * x",
                    Expectation().AST( { SizeofExpression,
                                         ExpressionAsTypeReference,
                                         PointerIndirectionExpression,
                                         IdentifierName }));
}

void ParserTester::case1406()
{
    parseExpression("sizeof x [ 0 ]",
                    Expectation().AST( { SizeofExpression,
                                         ExpressionAsTypeReference,
                                         ElementAccessExpression,
                                         IdentifierName,
                                         IntegerConstantExpression }));
}

void ParserTester::case1407()
{
    parseExpression("sizeof ( int )",
                    Expectation().AST( { SizeofExpression,
                                         TypeNameAsTypeReference,
                                         TypeName,
                                         BuiltinTypeSpecifier,
                                         AbstractDeclarator
                                       }));
}

void ParserTester::case1408()
{
    parseExpression("sizeof ( int * )",
                    Expectation().AST( { SizeofExpression,
                                         TypeNameAsTypeReference,
                                         TypeName,
                                         BuiltinTypeSpecifier,
                                         PointerDeclarator,
                                         AbstractDeclarator }));
}

void ParserTester::case1409()
{
    parseExpression("sizeof x . y",
                    Expectation().AST( { SizeofExpression,
                                         ExpressionAsTypeReference,
                                         DirectMemberAccessExpression,
                                         IdentifierName,
                                         IdentifierName }));
}

void ParserTester::case1410()
{
    parseExpression("sizeof ( x . y )");
}

void ParserTester::case1411()
{
    parseExpression("sizeof x [ 0 ]");
}

void ParserTester::case1412()
{
    parseExpression("sizeof ( x [ 0 ] )");
}

void ParserTester::case1413()
{
    parseExpression("sizeof ( x + 1 )");
}

void ParserTester::case1414()
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

void ParserTester::case1415()
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

void ParserTester::case1416()
{
    parseExpression("sizeof ( _Atomic ( int ) )");
}

void ParserTester::case1417()
{
    parseExpression("sizeof ( _Atomic ( x ) )");
}

void ParserTester::case1418() {}
void ParserTester::case1419() {}
void ParserTester::case1420() {}
void ParserTester::case1421() {}
void ParserTester::case1422() {}
void ParserTester::case1423() {}
void ParserTester::case1424() {}
void ParserTester::case1425() {}
void ParserTester::case1426() {}
void ParserTester::case1427() {}
void ParserTester::case1428() {}
void ParserTester::case1429() {}
void ParserTester::case1430() {}
void ParserTester::case1431() {}
void ParserTester::case1432() {}
void ParserTester::case1433() {}
void ParserTester::case1434() {}
void ParserTester::case1435() {}
void ParserTester::case1436() {}
void ParserTester::case1437() {}
void ParserTester::case1438() {}
void ParserTester::case1439() {}
void ParserTester::case1440() {}
void ParserTester::case1441() {}
void ParserTester::case1442() {}
void ParserTester::case1443() {}
void ParserTester::case1444() {}
void ParserTester::case1445() {}
void ParserTester::case1446() {}
void ParserTester::case1447() {}
void ParserTester::case1448() {}
void ParserTester::case1449() {}

void ParserTester::case1450()
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

void ParserTester::case1451()
{
    parseExpression("_Alignof ( int )",
                    Expectation().AST( { AlignofExpression,
                                         TypeNameAsTypeReference,
                                         TypeName,
                                         BuiltinTypeSpecifier,
                                         AbstractDeclarator }));
}

void ParserTester::case1452()
{
    parseExpression("_Alignof ( int * )",
                    Expectation().AST( { AlignofExpression,
                                         TypeNameAsTypeReference,
                                         TypeName,
                                         BuiltinTypeSpecifier,
                                         PointerDeclarator,
                                         AbstractDeclarator }));
}

void ParserTester::case1453()
{
    // See LanguageExtensions.

    parseExpression("_Alignof x",
                    Expectation().AST( { AlignofExpression,
                                         ExpressionAsTypeReference,
                                         IdentifierName }));
}

void ParserTester::case1454()
{
}

void ParserTester::case1455()
{
}

void ParserTester::case1456()
{
}

void ParserTester::case1457()
{
}

void ParserTester::case1458() {}
void ParserTester::case1459() {}
void ParserTester::case1460() {}
void ParserTester::case1461() {}
void ParserTester::case1462() {}
void ParserTester::case1463() {}
void ParserTester::case1464() {}
void ParserTester::case1465() {}
void ParserTester::case1466() {}
void ParserTester::case1467() {}
void ParserTester::case1468() {}
void ParserTester::case1469() {}
void ParserTester::case1470() {}
void ParserTester::case1471() {}
void ParserTester::case1472() {}
void ParserTester::case1473() {}
void ParserTester::case1474() {}
void ParserTester::case1475() {}
void ParserTester::case1476() {}
void ParserTester::case1477() {}
void ParserTester::case1478() {}
void ParserTester::case1479() {}
void ParserTester::case1480() {}
void ParserTester::case1481() {}
void ParserTester::case1482() {}
void ParserTester::case1483() {}
void ParserTester::case1484() {}
void ParserTester::case1485() {}
void ParserTester::case1486() {}
void ParserTester::case1487() {}
void ParserTester::case1488() {}
void ParserTester::case1489() {}
void ParserTester::case1490() {}
void ParserTester::case1491() {}
void ParserTester::case1492() {}
void ParserTester::case1493() {}
void ParserTester::case1494() {}
void ParserTester::case1495() {}
void ParserTester::case1496() {}
void ParserTester::case1497() {}
void ParserTester::case1498() {}
void ParserTester::case1499() {}

void ParserTester::case1500()
{
    parseExpression("x . y",
                    Expectation().AST( { DirectMemberAccessExpression,
                                         IdentifierName,
                                         IdentifierName }));
}

void ParserTester::case1501()
{
    parseExpression("x .",
                    Expectation().diagnostic(Expectation::ErrorOrWarn::Error,
                                             Parser::DiagnosticsReporter::ID_of_ExpectedFieldName));
}

void ParserTester::case1502()
{
    parseExpression("x . 'y'",
                    Expectation().diagnostic(Expectation::ErrorOrWarn::Error,
                                             Parser::DiagnosticsReporter::ID_of_ExpectedFieldName));
}

void ParserTester::case1503()
{
    parseExpression("x ( ) . y",
                    Expectation().AST( { DirectMemberAccessExpression,
                                         CallExpression,
                                         IdentifierName,
                                         IdentifierName }));
}

void ParserTester::case1504()
{
    parseExpression("x -> y",
                    Expectation().AST( { IndirectMemberAccessExpression,
                                         IdentifierName,
                                         IdentifierName }));
}

void ParserTester::case1505()
{
    parseExpression("x -> ",
                    Expectation().diagnostic(Expectation::ErrorOrWarn::Error,
                                             Parser::DiagnosticsReporter::ID_of_ExpectedFieldName));
}

void ParserTester::case1506()
{
    parseExpression("x -> 'y'",
                    Expectation().diagnostic(Expectation::ErrorOrWarn::Error,
                                             Parser::DiagnosticsReporter::ID_of_ExpectedFieldName));
}

void ParserTester::case1507()
{
    parseExpression("x . y . z",
                    Expectation().AST( { DirectMemberAccessExpression,
                                         DirectMemberAccessExpression,
                                         IdentifierName,
                                         IdentifierName,
                                         IdentifierName }));
}

void ParserTester::case1508()
{
    parseExpression("x . y -> z",
                    Expectation().AST( { IndirectMemberAccessExpression,
                                         DirectMemberAccessExpression,
                                         IdentifierName,
                                         IdentifierName,
                                         IdentifierName }));
}

void ParserTester::case1509()
{
    parseExpression("x -> y -> z",
                    Expectation().AST( { IndirectMemberAccessExpression,
                                         IndirectMemberAccessExpression,
                                         IdentifierName,
                                         IdentifierName,
                                         IdentifierName }));
}

void ParserTester::case1510()
{
    parseExpression("( x . y ) -> z",
                    Expectation().AST( { IndirectMemberAccessExpression,
                                         ParenthesizedExpression,
                                         DirectMemberAccessExpression,
                                         IdentifierName,
                                         IdentifierName,
                                         IdentifierName }));
}

void ParserTester::case1511()
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

void ParserTester::case1512()
{
    parseExpression("x [ 0 ] . y",
                    Expectation().AST( { DirectMemberAccessExpression,
                                         ElementAccessExpression,
                                         IdentifierName,
                                         IntegerConstantExpression,
                                         IdentifierName }));

}

void ParserTester::case1513()
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

void ParserTester::case1514()
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

void ParserTester::case1515() {}
void ParserTester::case1516() {}
void ParserTester::case1517() {}
void ParserTester::case1518() {}
void ParserTester::case1519() {}
void ParserTester::case1520() {}
void ParserTester::case1521() {}
void ParserTester::case1522() {}
void ParserTester::case1523() {}
void ParserTester::case1524() {}
void ParserTester::case1525() {}
void ParserTester::case1526() {}
void ParserTester::case1527() {}
void ParserTester::case1528() {}
void ParserTester::case1529() {}
void ParserTester::case1530() {}
void ParserTester::case1531() {}
void ParserTester::case1532() {}
void ParserTester::case1533() {}
void ParserTester::case1534() {}
void ParserTester::case1535() {}
void ParserTester::case1536() {}
void ParserTester::case1537() {}
void ParserTester::case1538() {}
void ParserTester::case1539() {}
void ParserTester::case1540() {}
void ParserTester::case1541() {}
void ParserTester::case1542() {}
void ParserTester::case1543() {}
void ParserTester::case1544() {}
void ParserTester::case1545() {}
void ParserTester::case1546() {}
void ParserTester::case1547() {}
void ParserTester::case1548() {}
void ParserTester::case1549() {}
void ParserTester::case1550() {}
void ParserTester::case1551() {}
void ParserTester::case1552() {}
void ParserTester::case1553() {}
void ParserTester::case1554() {}
void ParserTester::case1555() {}
void ParserTester::case1556() {}
void ParserTester::case1557() {}
void ParserTester::case1558() {}
void ParserTester::case1559() {}
void ParserTester::case1560() {}
void ParserTester::case1561() {}
void ParserTester::case1562() {}
void ParserTester::case1563() {}
void ParserTester::case1564() {}
void ParserTester::case1565() {}
void ParserTester::case1566() {}
void ParserTester::case1567() {}
void ParserTester::case1568() {}
void ParserTester::case1569() {}
void ParserTester::case1570() {}
void ParserTester::case1571() {}
void ParserTester::case1572() {}
void ParserTester::case1573() {}
void ParserTester::case1574() {}
void ParserTester::case1575() {}
void ParserTester::case1576() {}
void ParserTester::case1577() {}
void ParserTester::case1578() {}
void ParserTester::case1579() {}
void ParserTester::case1580() {}
void ParserTester::case1581() {}
void ParserTester::case1582() {}
void ParserTester::case1583() {}
void ParserTester::case1584() {}
void ParserTester::case1585() {}
void ParserTester::case1586() {}
void ParserTester::case1587() {}
void ParserTester::case1588() {}
void ParserTester::case1589() {}
void ParserTester::case1590() {}
void ParserTester::case1591() {}
void ParserTester::case1592() {}
void ParserTester::case1593() {}
void ParserTester::case1594() {}
void ParserTester::case1595() {}
void ParserTester::case1596() {}
void ParserTester::case1597() {}
void ParserTester::case1598() {}
void ParserTester::case1599() {}

void ParserTester::case1600()
{
    parseExpression("x ( )",
                    Expectation().AST( { CallExpression,
                                         IdentifierName }));
}

void ParserTester::case1601()
{
    parseExpression("( x ) ( )",
                    Expectation().AST( { CallExpression,
                                         ParenthesizedExpression,
                                         IdentifierName }));
}

void ParserTester::case1602()
{
    parseExpression("x ( 1 )",
                    Expectation().AST( { CallExpression,
                                         IdentifierName,
                                         IntegerConstantExpression }));
}

void ParserTester::case1603()
{
    parseExpression("x ( y )",
                    Expectation().AST( { CallExpression,
                                         IdentifierName,
                                         IdentifierName }));
}

void ParserTester::case1604()
{
    parseExpression("x ( y , 1 )",
                    Expectation().AST( { CallExpression,
                                         IdentifierName,
                                         IdentifierName,
                                         IntegerConstantExpression }));
}

void ParserTester::case1605()
{
    parseExpression("( & x ) ( )",
                    Expectation().AST( { CallExpression,
                                         ParenthesizedExpression,
                                         AddressOfExpression,
                                         IdentifierName }));
}

void ParserTester::case1606()
{
    parseExpression("( * x ) ( )",
                    Expectation().AST( { CallExpression,
                                         ParenthesizedExpression,
                                         PointerIndirectionExpression,
                                         IdentifierName }));
}

void ParserTester::case1607()
{
    parseExpression("( * * x ) ( )",
                    Expectation().AST( { CallExpression,
                                         ParenthesizedExpression,
                                         PointerIndirectionExpression,
                                         PointerIndirectionExpression,
                                         IdentifierName }));
}

void ParserTester::case1608()
{
    parseExpression("x ( y ( ), \"z\" )",
                    Expectation().AST( { CallExpression,
                                         IdentifierName,
                                         CallExpression,
                                         IdentifierName,
                                         StringLiteralExpression }));
}

void ParserTester::case1609()
{
    parseExpression("x ( y",
                    Expectation().diagnostic(Expectation::ErrorOrWarn::Error,
                                                Parser::DiagnosticsReporter::ID_of_ExpectedToken));
}

void ParserTester::case1610()
{
    parseExpression("x (",
                    Expectation().diagnostic(Expectation::ErrorOrWarn::Error,
                                                Parser::DiagnosticsReporter::ID_of_ExpectedFIRSTofExpression));
}

void ParserTester::case1611()
{
    parseExpression("x ( y , )",
                    Expectation().diagnostic(Expectation::ErrorOrWarn::Error,
                                                Parser::DiagnosticsReporter::ID_of_ExpectedFIRSTofExpression));
}

void ParserTester::case1612()
{
    parseExpression("x ( ( 1 ) )",
                    Expectation().AST( { CallExpression,
                                         IdentifierName,
                                         ParenthesizedExpression,
                                         IntegerConstantExpression }));
}

void ParserTester::case1613()
{
    parseExpression("x ( ( ( y ) ) )",
                    Expectation().AST( { CallExpression,
                                         IdentifierName,
                                         ParenthesizedExpression,
                                         ParenthesizedExpression,
                                         IdentifierName }));
}

void ParserTester::case1614()
{
    parseExpression("( x ( ) )",
                    Expectation().AST( { ParenthesizedExpression,
                                         CallExpression,
                                         IdentifierName }));
}

void ParserTester::case1615()
{
    parseExpression("( ( x ( ) ) )",
                    Expectation().AST( { ParenthesizedExpression,
                                         ParenthesizedExpression,
                                         CallExpression,
                                         IdentifierName }));
}

void ParserTester::case1616()
{
    parseExpression("( x ( 1 ) )",
                    Expectation().AST( { ParenthesizedExpression,
                                         CallExpression,
                                         IdentifierName,
                                         IntegerConstantExpression }));
}

void ParserTester::case1617()
{
    parseExpression("( x ( 1 , y ) )",
                    Expectation().AST( { ParenthesizedExpression,
                                         CallExpression,
                                         IdentifierName,
                                         IntegerConstantExpression,
                                         IdentifierName }));
}

void ParserTester::case1618() {}
void ParserTester::case1619() {}
void ParserTester::case1620() {}
void ParserTester::case1621() {}
void ParserTester::case1622() {}
void ParserTester::case1623() {}
void ParserTester::case1624() {}
void ParserTester::case1625() {}
void ParserTester::case1626() {}
void ParserTester::case1627() {}
void ParserTester::case1628() {}
void ParserTester::case1629() {}
void ParserTester::case1630() {}
void ParserTester::case1631() {}
void ParserTester::case1632() {}
void ParserTester::case1633() {}
void ParserTester::case1634() {}
void ParserTester::case1635() {}
void ParserTester::case1636() {}
void ParserTester::case1637() {}
void ParserTester::case1638() {}
void ParserTester::case1639() {}
void ParserTester::case1640() {}
void ParserTester::case1641() {}
void ParserTester::case1642() {}
void ParserTester::case1643() {}
void ParserTester::case1644() {}
void ParserTester::case1645() {}
void ParserTester::case1646() {}
void ParserTester::case1647() {}
void ParserTester::case1648() {}
void ParserTester::case1649() {}

void ParserTester::case1650()
{
    parseExpression("* x",
                    Expectation().AST( { PointerIndirectionExpression,
                                         IdentifierName }));
}

void ParserTester::case1651()
{
    parseExpression("* ( x )",
                    Expectation().AST( { PointerIndirectionExpression,
                                         ParenthesizedExpression,
                                         IdentifierName }));
}

void ParserTester::case1652()
{
    parseExpression("& x",
                    Expectation().AST( { AddressOfExpression,
                                         IdentifierName }));
}

void ParserTester::case1653()
{
    // Syntax OK, semantics invalid.

    CROSS_REFERENCE_TEST(BinderTester::case0101);

    parseExpression("* 1",
                    Expectation().AST( { PointerIndirectionExpression,
                                         IntegerConstantExpression }));
}

void ParserTester::case1654()
{
    // Syntax OK, semantics invalid.

    CROSS_REFERENCE_TEST(BinderTester::case0102);

    parseExpression("& 1",
                    Expectation().AST( { AddressOfExpression,
                                         IntegerConstantExpression }));
}

void ParserTester::case1655() {}
void ParserTester::case1656() {}
void ParserTester::case1657() {}
void ParserTester::case1658() {}
void ParserTester::case1659() {}
void ParserTester::case1660() {}
void ParserTester::case1661() {}
void ParserTester::case1662() {}
void ParserTester::case1663() {}
void ParserTester::case1664() {}
void ParserTester::case1665() {}
void ParserTester::case1666() {}
void ParserTester::case1667() {}
void ParserTester::case1668() {}
void ParserTester::case1669() {}
void ParserTester::case1670() {}
void ParserTester::case1671() {}
void ParserTester::case1672() {}
void ParserTester::case1673() {}
void ParserTester::case1674() {}
void ParserTester::case1675() {}
void ParserTester::case1676() {}
void ParserTester::case1677() {}
void ParserTester::case1678() {}
void ParserTester::case1679() {}
void ParserTester::case1680() {}
void ParserTester::case1681() {}
void ParserTester::case1682() {}
void ParserTester::case1683() {}
void ParserTester::case1684() {}
void ParserTester::case1685() {}
void ParserTester::case1686() {}
void ParserTester::case1687() {}
void ParserTester::case1688() {}
void ParserTester::case1689() {}
void ParserTester::case1690() {}
void ParserTester::case1691() {}
void ParserTester::case1692() {}
void ParserTester::case1693() {}
void ParserTester::case1694() {}
void ParserTester::case1695() {}
void ParserTester::case1696() {}
void ParserTester::case1697() {}
void ParserTester::case1698() {}
void ParserTester::case1699() {}

void ParserTester::case1700()
{
    parseExpression("( int ) x",
                    Expectation().AST({ CastExpression,
                                        TypeName,
                                        BuiltinTypeSpecifier,
                                        AbstractDeclarator,
                                        IdentifierName }));
}

void ParserTester::case1701()
{
    parseExpression("( int * ) x",
                    Expectation().AST({ CastExpression,
                                        TypeName,
                                        BuiltinTypeSpecifier,
                                        PointerDeclarator,
                                        AbstractDeclarator,
                                        IdentifierName }));
}

void ParserTester::case1702()
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

void ParserTester::case1703()
{
    parseExpression("( struct x ) y",
                    Expectation().AST({ CastExpression,
                                        TypeName,
                                        StructTypeSpecifier,
                                        AbstractDeclarator,
                                        IdentifierName }));
}

void ParserTester::case1704()
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

void ParserTester::case1705()
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

void ParserTester::case1706()
{
    parseExpression("( int ) ++ x",
                    Expectation().AST({ CastExpression,
                                        TypeName,
                                        BuiltinTypeSpecifier,
                                        AbstractDeclarator,
                                        PreIncrementExpression,
                                        IdentifierName }));
}

void ParserTester::case1707()
{
    parseExpression("( int ) x ++",
                    Expectation().AST({ CastExpression,
                                        TypeName,
                                        BuiltinTypeSpecifier,
                                        AbstractDeclarator,
                                        PostIncrementExpression,
                                        IdentifierName }));
}

void ParserTester::case1708()
{
    parseExpression("( int ) _Generic ( y",
                    Expectation().setErrorCnt(1));
}

void ParserTester::case1709()
{
    parseExpression("( long ) int",
                    Expectation().setErrorCnt(1));
}

void ParserTester::case1710()
{
    parseExpression("( struct x ) -",
                    Expectation().setErrorCnt(1));
}

void ParserTester::case1711()
{
    parseExpression("( int ) -",
                    Expectation().setErrorCnt(1));
}

void ParserTester::case1712()
{
    parseExpression("( int ) x +",
                    Expectation().setErrorCnt(1));
}

void ParserTester::case1713()
{
    parseExpression("( int ) x ( y",
                    Expectation().setErrorCnt(1));
}

void ParserTester::case1714()
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

void ParserTester::case1715()
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

void ParserTester::case1716()
{
    parseExpression("( int ) - 1",
                    Expectation().AST( { CastExpression,
                                         TypeName,
                                         BuiltinTypeSpecifier,
                                         AbstractDeclarator,
                                         UnaryMinusExpression,
                                         IntegerConstantExpression }));
}

void ParserTester::case1717()
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

void ParserTester::case1718()
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

void ParserTester::case1719()
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

void ParserTester::case1720()
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
void ParserTester::case1721() {}
void ParserTester::case1722() {}
void ParserTester::case1723() {}
void ParserTester::case1724() {}
void ParserTester::case1725() {}
void ParserTester::case1726() {}
void ParserTester::case1727() {}
void ParserTester::case1728() {}
void ParserTester::case1729() {}
void ParserTester::case1730() {}
void ParserTester::case1731() {}
void ParserTester::case1732() {}
void ParserTester::case1733() {}
void ParserTester::case1734() {}
void ParserTester::case1735() {}
void ParserTester::case1736() {}
void ParserTester::case1737() {}
void ParserTester::case1738() {}
void ParserTester::case1739() {}
void ParserTester::case1740() {}
void ParserTester::case1741() {}
void ParserTester::case1742() {}
void ParserTester::case1743() {}
void ParserTester::case1744() {}
void ParserTester::case1745() {}
void ParserTester::case1746() {}
void ParserTester::case1747() {}
void ParserTester::case1748() {}
void ParserTester::case1749() {}

void ParserTester::case1750()
{
    parseExpression("x ? y : z",
                    Expectation().AST( { ConditionalExpression,
                                         IdentifierName,
                                         IdentifierName,
                                         IdentifierName } ));

}

void ParserTester::case1751()
{
    parseExpression("x ? y + z : w",
                    Expectation().AST( { ConditionalExpression,
                                         IdentifierName,
                                         AddExpression,
                                         IdentifierName,
                                         IdentifierName,
                                         IdentifierName } ));
}

void ParserTester::case1752()
{
    // See ExtGNU_Conditionals in LanguageExtensions.

    parseExpression("x ?: z",
                    Expectation().AST( { ConditionalExpression,
                                         IdentifierName,
                                         IdentifierName } ));
}

void ParserTester::case1753()
{
}

void ParserTester::case1754() {}
void ParserTester::case1755() {}
void ParserTester::case1756() {}
void ParserTester::case1757() {}
void ParserTester::case1758() {}
void ParserTester::case1759() {}
void ParserTester::case1760() {}
void ParserTester::case1761() {}
void ParserTester::case1762() {}
void ParserTester::case1763() {}
void ParserTester::case1764() {}
void ParserTester::case1765() {}
void ParserTester::case1766() {}
void ParserTester::case1767() {}
void ParserTester::case1768() {}
void ParserTester::case1769() {}
void ParserTester::case1770() {}
void ParserTester::case1771() {}
void ParserTester::case1772() {}
void ParserTester::case1773() {}
void ParserTester::case1774() {}
void ParserTester::case1775() {}
void ParserTester::case1776() {}
void ParserTester::case1777() {}
void ParserTester::case1778() {}
void ParserTester::case1779() {}
void ParserTester::case1780() {}
void ParserTester::case1781() {}
void ParserTester::case1782() {}
void ParserTester::case1783() {}
void ParserTester::case1784() {}
void ParserTester::case1785() {}
void ParserTester::case1786() {}
void ParserTester::case1787() {}
void ParserTester::case1788() {}
void ParserTester::case1789() {}
void ParserTester::case1790() {}
void ParserTester::case1791() {}
void ParserTester::case1792() {}
void ParserTester::case1793() {}
void ParserTester::case1794() {}
void ParserTester::case1795() {}
void ParserTester::case1796() {}
void ParserTester::case1797() {}
void ParserTester::case1798() {}
void ParserTester::case1799() {}

void ParserTester::case1800()
{
    parseExpression("x / y",
                    Expectation().AST( { DivideExpression,
                                         IdentifierName,
                                         IdentifierName } ));
}

void ParserTester::case1801()
{
    parseExpression("x << y",
                    Expectation().AST( { LeftShiftExpression,
                                         IdentifierName,
                                         IdentifierName } ));

}

void ParserTester::case1802()
{
    parseExpression("x + y",
                    Expectation().AST( { AddExpression,
                                         IdentifierName,
                                         IdentifierName } ));

}

void ParserTester::case1803()
{
    parseExpression("x || y",
                    Expectation().AST( { LogicalORExpression,
                                         IdentifierName,
                                         IdentifierName } ));
}

void ParserTester::case1804()
{
    // No ambiguity here, this is an expression (not an statement).

    parseExpression("x * y",
                    Expectation().AST( { MultiplyExpression,
                                         IdentifierName,
                                         IdentifierName } ));

}

void ParserTester::case1805()
{
    parseExpression("( x * y ) + z",
                    Expectation().AST( { AddExpression,
                                         ParenthesizedExpression,
                                         MultiplyExpression,
                                         IdentifierName,
                                         IdentifierName,
                                         IdentifierName }));
}

void ParserTester::case1806()
{
    parseExpression("x * ( y + z)",
                    Expectation().AST( { MultiplyExpression,
                                         IdentifierName,
                                         ParenthesizedExpression,
                                         AddExpression,
                                         IdentifierName,
                                         IdentifierName }));
}

void ParserTester::case1807()
{
    parseExpression("x + y * z",
                    Expectation().AST( { AddExpression,
                                         IdentifierName,
                                         MultiplyExpression,
                                         IdentifierName,
                                         IdentifierName }));
}

void ParserTester::case1808()
{
    parseExpression("x * y + z",
                    Expectation().AST( { AddExpression,
                                         MultiplyExpression,
                                         IdentifierName,
                                         IdentifierName,
                                         IdentifierName }));
}

void ParserTester::case1809()
{
    parseExpression("x , y",
                    Expectation().AST( { SequencingExpression,
                                         IdentifierName,
                                         IdentifierName }));
}

void ParserTester::case1810()
{
    parseExpression("x , 1, y",
                    Expectation().AST( { SequencingExpression,
                                         SequencingExpression,
                                         IdentifierName,
                                         IntegerConstantExpression,
                                         IdentifierName }));
}

void ParserTester::case1811()
{
    parseExpression("x * y + z >> w",
                    Expectation().AST( { RightShiftExpression,
                                         AddExpression,
                                         MultiplyExpression,
                                         IdentifierName,
                                         IdentifierName,
                                         IdentifierName,
                                         IdentifierName }));
}

void ParserTester::case1812()
{
    parseExpression("x >> y + z * w",
                    Expectation().AST( { RightShiftExpression,
                                         IdentifierName,
                                         AddExpression,
                                         IdentifierName,
                                         MultiplyExpression,
                                         IdentifierName,
                                         IdentifierName }));
}

void ParserTester::case1813()
{
    /*
     * Can't be parsed as an assignment-expression.
     * Should only parse till `x'.
     *
     * (6.5.16) assignment-expression:
     *     conditional-expression
     *     unary-expression assignment-operator assignment-expression
     */

    CROSS_REFERENCE_TEST(ParserTester::case2213);

    parseExpression("x * y = z",
                    Expectation().unfinishedParse());
}

void ParserTester::case1814() {}
void ParserTester::case1815() {}
void ParserTester::case1816() {}
void ParserTester::case1817() {}
void ParserTester::case1818() {}
void ParserTester::case1819() {}
void ParserTester::case1820() {}
void ParserTester::case1821() {}
void ParserTester::case1822() {}
void ParserTester::case1823() {}
void ParserTester::case1824() {}
void ParserTester::case1825() {}
void ParserTester::case1826() {}
void ParserTester::case1827() {}
void ParserTester::case1828() {}
void ParserTester::case1829() {}
void ParserTester::case1830() {}
void ParserTester::case1831() {}
void ParserTester::case1832() {}
void ParserTester::case1833() {}
void ParserTester::case1834() {}
void ParserTester::case1835() {}
void ParserTester::case1836() {}
void ParserTester::case1837() {}
void ParserTester::case1838() {}
void ParserTester::case1839() {}
void ParserTester::case1840() {}
void ParserTester::case1841() {}
void ParserTester::case1842() {}
void ParserTester::case1843() {}
void ParserTester::case1844() {}
void ParserTester::case1845() {}
void ParserTester::case1846() {}
void ParserTester::case1847() {}
void ParserTester::case1848() {}
void ParserTester::case1849() {}

void ParserTester::case1850()
{
    parseExpression("x = y",
                    Expectation().AST( { BasicAssignmentExpression,
                                         IdentifierName,
                                         IdentifierName } ));
}

void ParserTester::case1851()
{
    parseExpression("x %= y",
                    Expectation().AST( { ModuloAssignmentExpression,
                                         IdentifierName,
                                         IdentifierName } ));
}

void ParserTester::case1852()
{
    parseExpression("x = y = 1",
                    Expectation().AST( { BasicAssignmentExpression,
                                         IdentifierName,
                                         BasicAssignmentExpression,
                                         IdentifierName,
                                         IntegerConstantExpression } ));

}

void ParserTester::case1853()
{
    parseExpression("x = ( y = 1)",
                    Expectation().AST( { BasicAssignmentExpression,
                                         IdentifierName,
                                         ParenthesizedExpression,
                                         BasicAssignmentExpression,
                                         IdentifierName,
                                         IntegerConstantExpression } ));
}

void ParserTester::case1854()
{
    parseExpression("x = 0",
                    Expectation().AST( { BasicAssignmentExpression,
                                         IdentifierName,
                                         IntegerConstantExpression } ));
}

void ParserTester::case1855()
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

void ParserTester::case1856()
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

void ParserTester::case1857() {}
void ParserTester::case1858() {}
void ParserTester::case1859() {}
void ParserTester::case1860() {}
void ParserTester::case1861() {}
void ParserTester::case1862() {}
void ParserTester::case1863() {}
void ParserTester::case1864() {}
void ParserTester::case1865() {}
void ParserTester::case1866() {}
void ParserTester::case1867() {}
void ParserTester::case1868() {}
void ParserTester::case1869() {}
void ParserTester::case1870() {}
void ParserTester::case1871() {}
void ParserTester::case1872() {}
void ParserTester::case1873() {}
void ParserTester::case1874() {}
void ParserTester::case1875() {}
void ParserTester::case1876() {}
void ParserTester::case1877() {}
void ParserTester::case1878() {}
void ParserTester::case1879() {}
void ParserTester::case1880() {}
void ParserTester::case1881() {}
void ParserTester::case1882() {}
void ParserTester::case1883() {}
void ParserTester::case1884() {}
void ParserTester::case1885() {}
void ParserTester::case1886() {}
void ParserTester::case1887() {}
void ParserTester::case1888() {}
void ParserTester::case1889() {}
void ParserTester::case1890() {}
void ParserTester::case1891() {}
void ParserTester::case1892() {}
void ParserTester::case1893() {}
void ParserTester::case1894() {}
void ParserTester::case1895() {}
void ParserTester::case1896() {}
void ParserTester::case1897() {}
void ParserTester::case1898() {}
void ParserTester::case1899() {}

void ParserTester::case1900()
{
    // https://gcc.gnu.org/onlinedocs/gcc/Statement-Exprs.html

    parseExpression("({ int x; })");
}

void ParserTester::case1901()
{
    // https://gcc.gnu.org/onlinedocs/gcc/Statement-Exprs.html

    parseExpression("({ int x; x; })");
}

void ParserTester::case1902()
{
    // https://gcc.gnu.org/onlinedocs/gcc/Statement-Exprs.html

    parseExpression("({ x y; })");
}

void ParserTester::case1903()
{
    // https://gcc.gnu.org/onlinedocs/gcc/Statement-Exprs.html

    parseExpression("({ x y; y; })");
}

void ParserTester::case1904() {}
void ParserTester::case1905() {}
void ParserTester::case1906() {}
void ParserTester::case1907() {}
void ParserTester::case1908() {}
void ParserTester::case1909() {}
void ParserTester::case1910() {}
void ParserTester::case1911() {}
void ParserTester::case1912() {}
void ParserTester::case1913() {}
void ParserTester::case1914() {}
void ParserTester::case1915() {}
void ParserTester::case1916() {}
void ParserTester::case1917() {}
void ParserTester::case1918() {}
void ParserTester::case1919() {}
void ParserTester::case1920() {}
void ParserTester::case1921() {}
void ParserTester::case1922() {}
void ParserTester::case1923() {}
void ParserTester::case1924() {}
void ParserTester::case1925() {}
void ParserTester::case1926() {}
void ParserTester::case1927() {}
void ParserTester::case1928() {}
void ParserTester::case1929() {}
void ParserTester::case1930() {}
void ParserTester::case1931() {}
void ParserTester::case1932() {}
void ParserTester::case1933() {}
void ParserTester::case1934() {}
void ParserTester::case1935() {}
void ParserTester::case1936() {}
void ParserTester::case1937() {}
void ParserTester::case1938() {}
void ParserTester::case1939() {}
void ParserTester::case1940() {}
void ParserTester::case1941() {}
void ParserTester::case1942() {}
void ParserTester::case1943() {}
void ParserTester::case1944() {}
void ParserTester::case1945() {}
void ParserTester::case1946() {}
void ParserTester::case1947() {}
void ParserTester::case1948() {}
void ParserTester::case1949() {}

void ParserTester::case1950()
{
    parseExpression("__builtin_va_arg ( x , int )",
                    Expectation().AST( { VAArgumentExpression,
                                         IdentifierName,
                                         TypeName,
                                         BuiltinTypeSpecifier,
                                         AbstractDeclarator }));
}

void ParserTester::case1951()
{
    parseExpression("__builtin_va_arg ( x , y )",
                    Expectation().AST( { VAArgumentExpression,
                                         IdentifierName,
                                         TypeName,
                                         TypedefName,
                                         AbstractDeclarator }));
}

void ParserTester::case1952() {}
void ParserTester::case1953() {}
void ParserTester::case1954() {}
void ParserTester::case1955() {}
void ParserTester::case1956() {}
void ParserTester::case1957() {}
void ParserTester::case1958() {}
void ParserTester::case1959() {}

void ParserTester::case1960()
{
    parseExpression("__builtin_offsetof ( struct x , y )",
                    Expectation().AST( { OffsetOfExpression,
                                         TypeName,
                                         StructTypeSpecifier,
                                         AbstractDeclarator,
                                         OffsetOfDesignator }));
}

void ParserTester::case1961()
{
    parseExpression("__builtin_offsetof ( struct x , y . z )",
                    Expectation().AST( { OffsetOfExpression,
                                         TypeName,
                                         StructTypeSpecifier,
                                         AbstractDeclarator,
                                         OffsetOfDesignator,
                                         FieldDesignator }));
}

void ParserTester::case1962()
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

void ParserTester::case1963()
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

void ParserTester::case1964()
{
    parseExpression("__builtin_offsetof ( union x , y )",
                    Expectation().AST( { OffsetOfExpression,
                                         TypeName,
                                         UnionTypeSpecifier,
                                         AbstractDeclarator,
                                         OffsetOfDesignator }));
}

void ParserTester::case1965() {}
void ParserTester::case1966() {}
void ParserTester::case1967() {}
void ParserTester::case1968() {}
void ParserTester::case1969() {}

void ParserTester::case1970()
{
    parseExpression("__builtin_choose_expr ( 0 , x , y )",
                    Expectation().AST( { ExtGNU_ChooseExpression,
                                         IntegerConstantExpression,
                                         IdentifierName,
                                         IdentifierName }));
}

void ParserTester::case1971()
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

void ParserTester::case1972()
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

void ParserTester::case1973()
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

void ParserTester::case1974() {}
void ParserTester::case1975() {}
void ParserTester::case1976() {}
void ParserTester::case1977() {}
void ParserTester::case1978() {}
void ParserTester::case1979() {}

void ParserTester::case1980()
{
    parseExpression("__FUNCTION__",
                    Expectation().AST( { PredefinedName }));
}

void ParserTester::case1981()
{
    parseExpression("__PRETTY_FUNCTION__",
                    Expectation().AST( { PredefinedName }));
}

void ParserTester::case1982()
{
    parseExpression("sizeof ( __FUNCTION__ )",
                    Expectation().AST( { SizeofExpression,
                                         ExpressionAsTypeReference,
                                         ParenthesizedExpression,
                                         PredefinedName }));
}

void ParserTester::case1983()
{
    parseExpression("sizeof ( __PRETTY_FUNCTION__ )",
                    Expectation().AST( { SizeofExpression,
                                         ExpressionAsTypeReference,
                                         ParenthesizedExpression,
                                         PredefinedName}));
}

void ParserTester::case1984()
{
    parseExpression("sizeof __FUNCTION__",
                    Expectation().AST( { SizeofExpression,
                                         ExpressionAsTypeReference,
                                         PredefinedName }));
}

void ParserTester::case1985()
{
    parseExpression("sizeof __PRETTY_FUNCTION__",
                    Expectation().AST( { SizeofExpression,
                                         ExpressionAsTypeReference,
                                         PredefinedName }));
}

void ParserTester::case1986()
{
    parseExpression("( __FUNCTION__ )",
                    Expectation().AST( { ParenthesizedExpression,
                                         PredefinedName }));
}

void ParserTester::case1987()
{
    parseExpression("( __PRETTY_FUNCTION__ )",
                    Expectation().AST( { ParenthesizedExpression,
                                         PredefinedName }));
}

void ParserTester::case1988()
{
    parseExpression("x = __FUNCTION__",
                    Expectation().AST( { BasicAssignmentExpression,
                                         IdentifierName,
                                         PredefinedName }));
}

void ParserTester::case1989()
{
    parseExpression("x = __PRETTY_FUNCTION__",
                    Expectation().AST( { BasicAssignmentExpression,
                                         IdentifierName,
                                         PredefinedName }));
}

void ParserTester::case1990()
{
    parseExpression("__real__ x",
                    Expectation().AST( { ExtGNU_RealExpression,
                                         IdentifierName }));
}

void ParserTester::case1991()
{
    parseExpression("__real__ ( x )",
                    Expectation().AST( { ExtGNU_RealExpression,
                                         ParenthesizedExpression,
                                         IdentifierName }));
}

void ParserTester::case1992()
{
    parseExpression("__real__ ( __real__ x )",
                    Expectation().AST( { ExtGNU_RealExpression,
                                         ParenthesizedExpression,
                                         ExtGNU_RealExpression,
                                         IdentifierName }));
}
void ParserTester::case1993()
{
    parseExpression("__imag__ x",
                    Expectation().AST( { ExtGNU_ImagExpression,
                                         IdentifierName }));
}

void ParserTester::case1994()
{
    parseExpression("__imag__ ( x )",
                    Expectation().AST( { ExtGNU_ImagExpression,
                                         ParenthesizedExpression,
                                         IdentifierName }));
}

void ParserTester::case1995()
{
    parseExpression("__imag__ __real__ x",
                    Expectation().AST( { ExtGNU_ImagExpression,
                                         ExtGNU_RealExpression,
                                         IdentifierName }));
}

void ParserTester::case1996()
{
    parseExpression("__real__ ( int ) x",
                    Expectation().AST( { ExtGNU_RealExpression,
                                         CastExpression,
                                         TypeName,
                                         BuiltinTypeSpecifier,
                                         AbstractDeclarator,
                                         IdentifierName }));
}

void ParserTester::case1997() {}
void ParserTester::case1998() {}
void ParserTester::case1999() {}
