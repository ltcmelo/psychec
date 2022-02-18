// Copyright (c) 2021 Leandro T. C. Melo <ltcmelo@gmail.com>
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

#include "TestBinder.h"

#include "TestParser.h"

#include "binder/Semantics_TypeSpecifiers.h"
#include "parser/Unparser.h"
#include "symbols/Symbol.h"
#include "symbols/Symbols.h"
#include "syntax/SyntaxLexemes.h"
#include "syntax/SyntaxNodes.h"

#include <algorithm>
#include <cstring>
#include <iostream>
#include <string>
#include <sstream>

using namespace psy;
using namespace C;

void TestBinder::case1000()
{
    bind("double x ;",
         Expectation()
            .obj("x", ValueKind::Variable, "double", TypeKind::Builtin, BuiltinTypeKind::Double));
}

void TestBinder::case1001()
{
    bind("int x ;",
         Expectation()
            .obj("x", ValueKind::Variable, "int", TypeKind::Builtin, BuiltinTypeKind::Int));
}

void TestBinder::case1002()
{
    bind("int x ; int y ;",
         Expectation()
            .obj("x", ValueKind::Variable, "int", TypeKind::Builtin, BuiltinTypeKind::Int)
            .obj("x", ValueKind::Variable, "int", TypeKind::Builtin, BuiltinTypeKind::Int));
}

void TestBinder::case1003()
{
    bind("int x , y ;",
         Expectation()
            .obj("x", ValueKind::Variable, "int", TypeKind::Builtin, BuiltinTypeKind::Int)
            .obj("y", ValueKind::Variable, "int", TypeKind::Builtin, BuiltinTypeKind::Int));
}

void TestBinder::case1004()
{
    bind("x y ;",
         Expectation()
            .obj("y", ValueKind::Variable, "x", TypeKind::Synonym));
}

void TestBinder::case1005()
{
    bind("x y , z ;",
         Expectation()
            .obj("y", ValueKind::Variable, "x", TypeKind::Synonym)
            .obj("z", ValueKind::Variable, "x", TypeKind::Synonym));
}

void TestBinder::case1006()
{
    bind("int x = 1 ;",
         Expectation()
            .obj("x", ValueKind::Variable, "int", TypeKind::Builtin, BuiltinTypeKind::Int));
}

void TestBinder::case1007()
{
    bind("x y = 1 ;",
         Expectation()
            .obj("y", ValueKind::Variable, "x", TypeKind::Synonym));
}

void TestBinder::case1008()
{
    bind("int float x ;",
         Expectation().addDiagnostic(
             Expectation::ErrorOrWarn::Error,
             Semantics_TypeSpecifiers::ID_TwoOrMoreDataTypesInDeclarationSpecifiers));
}

void TestBinder::case1009()
{
}

void TestBinder::case1010()
{
}

void TestBinder::case1011()
{
}

void TestBinder::case1012() {}
void TestBinder::case1013() {}
void TestBinder::case1014() {}
void TestBinder::case1015() {}
void TestBinder::case1016() {}
void TestBinder::case1017() {}
void TestBinder::case1018() {}
void TestBinder::case1019() {}
void TestBinder::case1020() {}
void TestBinder::case1021() {}
void TestBinder::case1022() {}
void TestBinder::case1023() {}
void TestBinder::case1024() {}
void TestBinder::case1025() {}
void TestBinder::case1026() {}
void TestBinder::case1027() {}
void TestBinder::case1028() {}
void TestBinder::case1029() {}
void TestBinder::case1030() {}
void TestBinder::case1031() {}
void TestBinder::case1032() {}
void TestBinder::case1033() {}
void TestBinder::case1034() {}
void TestBinder::case1035() {}
void TestBinder::case1036() {}
void TestBinder::case1037() {}
void TestBinder::case1038() {}
void TestBinder::case1039() {}
void TestBinder::case1040() {}
void TestBinder::case1041() {}
void TestBinder::case1042() {}
void TestBinder::case1043() {}
void TestBinder::case1044() {}
void TestBinder::case1045() {}
void TestBinder::case1046() {}
void TestBinder::case1047() {}
void TestBinder::case1048() {}
void TestBinder::case1049() {}

void TestBinder::case1050()
{
    bind("const int x ;",
         Expectation()
            .qualObj("x", ValueKind::Variable, "int", Expectation::Qual::Const, TypeKind::Builtin, BuiltinTypeKind::Int));
}

void TestBinder::case1051()
{
    bind("const x y ;",
         Expectation()
            .qualObj("y", ValueKind::Variable, "x", Expectation::Qual::Const, TypeKind::Synonym));
}

void TestBinder::case1052()
{
    bind("const x ;",
         Expectation().addDiagnostic(
             Expectation::ErrorOrWarn::Error,
             Semantics_TypeSpecifiers::ID_TypeSpecifierMissingDefaultsToInt));
}

void TestBinder::case1053()
{
    bind("int const x ;",
         Expectation()
            .qualObj("x", ValueKind::Variable, "int", Expectation::Qual::Const, TypeKind::Builtin, BuiltinTypeKind::Int));
}

void TestBinder::case1054()
{
    bind("x const y ;",
         Expectation()
            .qualObj("y", ValueKind::Variable, "x", Expectation::Qual::Const, TypeKind::Synonym));
}

void TestBinder::case1055() {}
void TestBinder::case1056() {}
void TestBinder::case1057() {}
void TestBinder::case1058() {}
void TestBinder::case1059() {}
void TestBinder::case1060() {}
void TestBinder::case1061() {}
void TestBinder::case1062() {}
void TestBinder::case1063() {}
void TestBinder::case1064() {}
void TestBinder::case1065() {}
void TestBinder::case1066() {}
void TestBinder::case1067() {}
void TestBinder::case1068() {}
void TestBinder::case1069() {}
void TestBinder::case1070() {}
void TestBinder::case1071() {}
void TestBinder::case1072() {}
void TestBinder::case1073() {}
void TestBinder::case1074() {}
void TestBinder::case1075() {}
void TestBinder::case1076() {}
void TestBinder::case1077() {}
void TestBinder::case1078() {}
void TestBinder::case1079() {}
void TestBinder::case1080() {}
void TestBinder::case1081() {}
void TestBinder::case1082() {}
void TestBinder::case1083() {}
void TestBinder::case1084() {}
void TestBinder::case1085() {}
void TestBinder::case1086() {}
void TestBinder::case1087() {}
void TestBinder::case1088() {}
void TestBinder::case1089() {}
void TestBinder::case1090() {}
void TestBinder::case1091() {}
void TestBinder::case1092() {}
void TestBinder::case1093() {}
void TestBinder::case1094() {}
void TestBinder::case1095() {}
void TestBinder::case1096() {}
void TestBinder::case1097() {}
void TestBinder::case1098() {}
void TestBinder::case1099() {}

void TestBinder::case1100()
{
    bind("int * x ;",
         Expectation()
            .objPtr_1("x", ValueKind::Variable, TypeKind::Builtin, BuiltinTypeKind::Int));
}

void TestBinder::case1101()
{
    bind("x * y ;",
         Expectation()
            .objPtr_1("y", ValueKind::Variable, TypeKind::Synonym));
}

void TestBinder::case1102()
{
}

void TestBinder::case1103()
{
}
void TestBinder::case1104()
{
    //    bind("int * x ; int * y ;",
    //         Expectation().SymbolNameKind(
    //             {
    //                 std::make_tuple("x", ValueKind::Variable),
    //                 std::make_tuple("y", ValueKind::Variable)
    //             }));
}
void TestBinder::case1105()
{
    //    bind("int x , * y ;",
    //         Expectation().SymbolNameKind(
    //             {
    //                 std::make_tuple("x", ValueKind::Variable),
    //                 std::make_tuple("y", ValueKind::Variable)
    //             }));
}

void TestBinder::case1106() {}
void TestBinder::case1107() {}
void TestBinder::case1108() {}
void TestBinder::case1109() {}
void TestBinder::case1110() {}
void TestBinder::case1111() {}
void TestBinder::case1112() {}
void TestBinder::case1113() {}
void TestBinder::case1114() {}
void TestBinder::case1115() {}
void TestBinder::case1116() {}
void TestBinder::case1117() {}
void TestBinder::case1118() {}
void TestBinder::case1119() {}
void TestBinder::case1120() {}
void TestBinder::case1121() {}
void TestBinder::case1122() {}
void TestBinder::case1123() {}
void TestBinder::case1124() {}
void TestBinder::case1125() {}
void TestBinder::case1126() {}
void TestBinder::case1127() {}
void TestBinder::case1128() {}
void TestBinder::case1129() {}
void TestBinder::case1130() {}
void TestBinder::case1131() {}
void TestBinder::case1132() {}
void TestBinder::case1133() {}
void TestBinder::case1134() {}
void TestBinder::case1135() {}
void TestBinder::case1136() {}
void TestBinder::case1137() {}
void TestBinder::case1138() {}
void TestBinder::case1139() {}
void TestBinder::case1140() {}
void TestBinder::case1141() {}
void TestBinder::case1142() {}
void TestBinder::case1143() {}
void TestBinder::case1144() {}
void TestBinder::case1145() {}
void TestBinder::case1146() {}
void TestBinder::case1147() {}
void TestBinder::case1148() {}
void TestBinder::case1149() {}

void TestBinder::case1150()
{
    bind("const int * x ;",
         Expectation()
            .qualObjPtr_1("x", ValueKind::Variable, Expectation::Qual::Const, TypeKind::Builtin, BuiltinTypeKind::Int));
}

void TestBinder::case1151()
{
    bind("const x * y ;",
         Expectation()
            .qualObjPtr_1("y", ValueKind::Variable, Expectation::Qual::Const, TypeKind::Synonym));
}

void TestBinder::case1152() {}
void TestBinder::case1153() {}
void TestBinder::case1154() {}
void TestBinder::case1155() {}
void TestBinder::case1156() {}
void TestBinder::case1157() {}
void TestBinder::case1158() {}
void TestBinder::case1159() {}
void TestBinder::case1160() {}
void TestBinder::case1161() {}
void TestBinder::case1162() {}
void TestBinder::case1163() {}
void TestBinder::case1164() {}
void TestBinder::case1165() {}
void TestBinder::case1166() {}
void TestBinder::case1167() {}
void TestBinder::case1168() {}
void TestBinder::case1169() {}
void TestBinder::case1170() {}
void TestBinder::case1171() {}
void TestBinder::case1172() {}
void TestBinder::case1173() {}
void TestBinder::case1174() {}
void TestBinder::case1175() {}
void TestBinder::case1176() {}
void TestBinder::case1177() {}
void TestBinder::case1178() {}
void TestBinder::case1179() {}
void TestBinder::case1180() {}
void TestBinder::case1181() {}
void TestBinder::case1182() {}
void TestBinder::case1183() {}
void TestBinder::case1184() {}
void TestBinder::case1185() {}
void TestBinder::case1186() {}
void TestBinder::case1187() {}
void TestBinder::case1188() {}
void TestBinder::case1189() {}
void TestBinder::case1190() {}
void TestBinder::case1191() {}
void TestBinder::case1192() {}
void TestBinder::case1193() {}
void TestBinder::case1194() {}
void TestBinder::case1195() {}
void TestBinder::case1196() {}
void TestBinder::case1197() {}
void TestBinder::case1198() {}
void TestBinder::case1199()
{}

void TestBinder::case1200()
{
    bind("const int * const x ;",
         Expectation()
            .qualObjQualPtr_1("x", ValueKind::Variable, Expectation::Qual::Const, Expectation::Qual::Const, TypeKind::Builtin, BuiltinTypeKind::Int));
}

void TestBinder::case1201() { }
void TestBinder::case1202() { }
void TestBinder::case1203() { }
void TestBinder::case1204() { }
void TestBinder::case1205() { }
void TestBinder::case1206() { }
void TestBinder::case1207() { }
void TestBinder::case1208() { }
void TestBinder::case1209() { }
void TestBinder::case1210() { }
void TestBinder::case1211() { }
void TestBinder::case1212() { }
void TestBinder::case1213() { }
void TestBinder::case1214() { }
void TestBinder::case1215() { }
void TestBinder::case1216() { }
void TestBinder::case1217() { }
void TestBinder::case1218() { }
void TestBinder::case1219() { }
void TestBinder::case1220() { }
void TestBinder::case1221() { }
void TestBinder::case1222() { }
void TestBinder::case1223() { }
void TestBinder::case1224() { }
void TestBinder::case1225() { }
void TestBinder::case1226() { }
void TestBinder::case1227() { }
void TestBinder::case1228() { }
void TestBinder::case1229() { }
void TestBinder::case1230() { }
void TestBinder::case1231() { }
void TestBinder::case1232() { }
void TestBinder::case1233() { }
void TestBinder::case1234() { }
void TestBinder::case1235() { }
void TestBinder::case1236() { }
void TestBinder::case1237() { }
void TestBinder::case1238() { }
void TestBinder::case1239() { }
void TestBinder::case1240() { }
void TestBinder::case1241() { }
void TestBinder::case1242() { }
void TestBinder::case1243() { }
void TestBinder::case1244() { }
void TestBinder::case1245() { }
void TestBinder::case1246() { }
void TestBinder::case1247() { }
void TestBinder::case1248() { }
void TestBinder::case1249() { }

void TestBinder::case1250()
{
    bind("int * const x ;",
         Expectation()
            .qualPtr_1("x", ValueKind::Variable, Expectation::Qual::Const, TypeKind::Builtin, BuiltinTypeKind::Int));
}

void TestBinder::case1251()
{
    bind("x * const y ;",
         Expectation()
            .qualPtr_1("y", ValueKind::Variable, Expectation::Qual::Const, TypeKind::Synonym));
}

void TestBinder::case1252() { }
void TestBinder::case1253() { }
void TestBinder::case1254() { }
void TestBinder::case1255() { }
void TestBinder::case1256() { }
void TestBinder::case1257() { }
void TestBinder::case1258() { }
void TestBinder::case1259() { }
void TestBinder::case1260() { }
void TestBinder::case1261() { }
void TestBinder::case1262() { }
void TestBinder::case1263() { }
void TestBinder::case1264() { }
void TestBinder::case1265() { }
void TestBinder::case1266() { }
void TestBinder::case1267() { }
void TestBinder::case1268() { }
void TestBinder::case1269() { }
void TestBinder::case1270() { }
void TestBinder::case1271() { }
void TestBinder::case1272() { }
void TestBinder::case1273() { }
void TestBinder::case1274() { }
void TestBinder::case1275() { }
void TestBinder::case1276() { }
void TestBinder::case1277() { }
void TestBinder::case1278() { }
void TestBinder::case1279() { }
void TestBinder::case1280() { }
void TestBinder::case1281() { }
void TestBinder::case1282() { }
void TestBinder::case1283() { }
void TestBinder::case1284() { }
void TestBinder::case1285() { }
void TestBinder::case1286() { }
void TestBinder::case1287() { }
void TestBinder::case1288() { }
void TestBinder::case1289() { }
void TestBinder::case1290() { }
void TestBinder::case1291() { }
void TestBinder::case1292() { }
void TestBinder::case1293() { }
void TestBinder::case1294() { }
void TestBinder::case1295() { }
void TestBinder::case1296() { }
void TestBinder::case1297() { }
void TestBinder::case1298() { }
void TestBinder::case1299() { }

void TestBinder::case1300(){ }
void TestBinder::case1301(){ }
void TestBinder::case1302(){ }
void TestBinder::case1303(){ }
void TestBinder::case1304(){ }
void TestBinder::case1305(){ }
void TestBinder::case1306(){ }
void TestBinder::case1307(){ }
void TestBinder::case1308(){ }
void TestBinder::case1309(){ }
void TestBinder::case1310(){ }
void TestBinder::case1311(){ }
void TestBinder::case1312(){ }
void TestBinder::case1313(){ }
void TestBinder::case1314(){ }
void TestBinder::case1315(){ }
void TestBinder::case1316(){ }
void TestBinder::case1317(){ }
void TestBinder::case1318(){ }
void TestBinder::case1319(){ }
void TestBinder::case1320(){ }
void TestBinder::case1321(){ }
void TestBinder::case1322(){ }
void TestBinder::case1323(){ }
void TestBinder::case1324(){ }
void TestBinder::case1325(){ }
void TestBinder::case1326(){ }
void TestBinder::case1327(){ }
void TestBinder::case1328(){ }
void TestBinder::case1329(){ }
void TestBinder::case1330(){ }
void TestBinder::case1331(){ }
void TestBinder::case1332(){ }
void TestBinder::case1333(){ }
void TestBinder::case1334(){ }
void TestBinder::case1335(){ }
void TestBinder::case1336(){ }
void TestBinder::case1337(){ }
void TestBinder::case1338(){ }
void TestBinder::case1339(){ }
void TestBinder::case1340(){ }
void TestBinder::case1341(){ }
void TestBinder::case1342(){ }
void TestBinder::case1343(){ }
void TestBinder::case1344(){ }
void TestBinder::case1345(){ }
void TestBinder::case1346(){ }
void TestBinder::case1347(){ }
void TestBinder::case1348(){ }
void TestBinder::case1349(){ }
void TestBinder::case1350(){ }
void TestBinder::case1351(){ }
void TestBinder::case1352(){ }
void TestBinder::case1353(){ }
void TestBinder::case1354(){ }
void TestBinder::case1355(){ }
void TestBinder::case1356(){ }
void TestBinder::case1357(){ }
void TestBinder::case1358(){ }
void TestBinder::case1359(){ }
void TestBinder::case1360(){ }
void TestBinder::case1361(){ }
void TestBinder::case1362(){ }
void TestBinder::case1363(){ }
void TestBinder::case1364(){ }
void TestBinder::case1365(){ }
void TestBinder::case1366(){ }
void TestBinder::case1367(){ }
void TestBinder::case1368(){ }
void TestBinder::case1369(){ }
void TestBinder::case1370(){ }
void TestBinder::case1371(){ }
void TestBinder::case1372(){ }
void TestBinder::case1373(){ }
void TestBinder::case1374(){ }
void TestBinder::case1375(){ }
void TestBinder::case1376(){ }
void TestBinder::case1377(){ }
void TestBinder::case1378(){ }
void TestBinder::case1379(){ }
void TestBinder::case1380(){ }
void TestBinder::case1381(){ }
void TestBinder::case1382(){ }
void TestBinder::case1383(){ }
void TestBinder::case1384(){ }
void TestBinder::case1385(){ }
void TestBinder::case1386(){ }
void TestBinder::case1387(){ }
void TestBinder::case1388(){ }
void TestBinder::case1389(){ }
void TestBinder::case1390(){ }
void TestBinder::case1391(){ }
void TestBinder::case1392(){ }
void TestBinder::case1393(){ }
void TestBinder::case1394(){ }
void TestBinder::case1395(){ }
void TestBinder::case1396(){ }
void TestBinder::case1397(){ }
void TestBinder::case1398(){ }
void TestBinder::case1399(){ }
