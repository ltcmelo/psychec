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

#include "BinderTest.h"

#include "ParserTest.h"

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

void BinderTest::case1000()
{
    bind("double x ;",
         Expectation()
            .binding(Binding("x", ValueKind::Variable)
                    .specType("double", TypeKind::Builtin, BuiltinTypeKind::Double)));

}

void BinderTest::case1001()
{
    bind("int x ;",
         Expectation()
            .binding(Binding("x", ValueKind::Variable)
                     .specType("int", TypeKind::Builtin, BuiltinTypeKind::Int)));
}

void BinderTest::case1002()
{
    bind("int x ; int y ;",
         Expectation()
            .binding(Binding("x", ValueKind::Variable)
                    .specType("int", TypeKind::Builtin, BuiltinTypeKind::Int))
            .binding(Binding("y", ValueKind::Variable)
                    .specType("int", TypeKind::Builtin, BuiltinTypeKind::Int)));
}

void BinderTest::case1003()
{
    bind("int x , y ;",
         Expectation()
             .binding(Binding("x", ValueKind::Variable)
                     .specType("int", TypeKind::Builtin, BuiltinTypeKind::Int))
             .binding(Binding("y", ValueKind::Variable)
                     .specType("int", TypeKind::Builtin, BuiltinTypeKind::Int)));
}

void BinderTest::case1004()
{
    bind("x y ;",
         Expectation()
             .binding(Binding("y", ValueKind::Variable)
                 .specType("x", TypeKind::Synonym, BuiltinTypeKind::None)));
}

void BinderTest::case1005()
{
    bind("x y , z ;",
         Expectation()
             .binding(Binding("y", ValueKind::Variable)
                      .specType("x", TypeKind::Synonym, BuiltinTypeKind::None))
             .binding(Binding("z", ValueKind::Variable)
                      .specType("x", TypeKind::Synonym, BuiltinTypeKind::None)));

}

void BinderTest::case1006()
{
    bind("int x = 1 ;",
         Expectation()
            .binding(Binding("x", ValueKind::Variable)
                     .specType("int", TypeKind::Builtin, BuiltinTypeKind::Int)));
}

void BinderTest::case1007()
{
    bind("x y = 1 ;",
         Expectation()
             .binding(Binding("y", ValueKind::Variable)
                      .specType("x", TypeKind::Synonym, BuiltinTypeKind::None)));
}

void BinderTest::case1008()
{
    bind("int float x ;",
         Expectation().diagnostic(
             Expectation::ErrorOrWarn::Error,
             Semantics_TypeSpecifiers::ID_TwoOrMoreDataTypesInDeclarationSpecifiers));
}

void BinderTest::case1009()
{
}

void BinderTest::case1010()
{
}

void BinderTest::case1011()
{
}

void BinderTest::case1012() {}
void BinderTest::case1013() {}
void BinderTest::case1014() {}
void BinderTest::case1015() {}
void BinderTest::case1016() {}
void BinderTest::case1017() {}
void BinderTest::case1018() {}
void BinderTest::case1019() {}
void BinderTest::case1020() {}
void BinderTest::case1021() {}
void BinderTest::case1022() {}
void BinderTest::case1023() {}
void BinderTest::case1024() {}
void BinderTest::case1025() {}
void BinderTest::case1026() {}
void BinderTest::case1027() {}
void BinderTest::case1028() {}
void BinderTest::case1029() {}
void BinderTest::case1030() {}
void BinderTest::case1031() {}
void BinderTest::case1032() {}
void BinderTest::case1033() {}
void BinderTest::case1034() {}
void BinderTest::case1035() {}
void BinderTest::case1036() {}
void BinderTest::case1037() {}
void BinderTest::case1038() {}
void BinderTest::case1039() {}
void BinderTest::case1040() {}
void BinderTest::case1041() {}
void BinderTest::case1042() {}
void BinderTest::case1043() {}
void BinderTest::case1044() {}
void BinderTest::case1045() {}
void BinderTest::case1046() {}
void BinderTest::case1047() {}
void BinderTest::case1048() {}
void BinderTest::case1049() {}

void BinderTest::case1050()
{
    bind("const int x ;",
         Expectation()
             .binding(Binding("x", ValueKind::Variable)
                      .specType("int", TypeKind::Builtin, BuiltinTypeKind::Int, CVR::Const)));
}

void BinderTest::case1051()
{
    bind("const x y ;",
         Expectation()
             .binding(Binding("y", ValueKind::Variable)
                      .specType("x", TypeKind::Synonym, BuiltinTypeKind::None, CVR::Const)));
}

void BinderTest::case1052()
{
    bind("const x ;",
         Expectation().diagnostic(
             Expectation::ErrorOrWarn::Error,
             Semantics_TypeSpecifiers::ID_TypeSpecifierMissingDefaultsToInt));
}

void BinderTest::case1053()
{
    bind("int const x ;",
         Expectation()
             .binding(Binding("x", ValueKind::Variable)
                      .specType("int", TypeKind::Builtin, BuiltinTypeKind::Int, CVR::Const)));
}

void BinderTest::case1054()
{
    bind("x const y ;",
         Expectation()
             .binding(Binding("y", ValueKind::Variable)
                      .specType("x", TypeKind::Synonym, BuiltinTypeKind::None, CVR::Const)));
}

void BinderTest::case1055() {}
void BinderTest::case1056() {}
void BinderTest::case1057() {}
void BinderTest::case1058() {}
void BinderTest::case1059() {}
void BinderTest::case1060() {}
void BinderTest::case1061() {}
void BinderTest::case1062() {}
void BinderTest::case1063() {}
void BinderTest::case1064() {}
void BinderTest::case1065() {}
void BinderTest::case1066() {}
void BinderTest::case1067() {}
void BinderTest::case1068() {}
void BinderTest::case1069() {}
void BinderTest::case1070() {}
void BinderTest::case1071() {}
void BinderTest::case1072() {}
void BinderTest::case1073() {}
void BinderTest::case1074() {}
void BinderTest::case1075() {}
void BinderTest::case1076() {}
void BinderTest::case1077() {}
void BinderTest::case1078() {}
void BinderTest::case1079() {}
void BinderTest::case1080() {}
void BinderTest::case1081() {}
void BinderTest::case1082() {}
void BinderTest::case1083() {}
void BinderTest::case1084() {}
void BinderTest::case1085() {}
void BinderTest::case1086() {}
void BinderTest::case1087() {}
void BinderTest::case1088() {}
void BinderTest::case1089() {}
void BinderTest::case1090() {}
void BinderTest::case1091() {}
void BinderTest::case1092() {}
void BinderTest::case1093() {}
void BinderTest::case1094() {}
void BinderTest::case1095() {}
void BinderTest::case1096() {}
void BinderTest::case1097() {}
void BinderTest::case1098() {}
void BinderTest::case1099() {}

void BinderTest::case1100()
{
    bind("int * x ;",
         Expectation()
             .binding(Binding("x", ValueKind::Variable)
                      .specType("int", TypeKind::Builtin, BuiltinTypeKind::Int)
                      .derivType(TypeKind::Pointer)));
}

void BinderTest::case1101()
{
    bind("x * y ;",
         Expectation()
             .binding(Binding("y", ValueKind::Variable)
                      .specType("x", TypeKind::Synonym)
                      .derivType(TypeKind::Pointer)));
}

void BinderTest::case1102()
{
    bind("int * x ; y * z ;",
         Expectation()
             .binding(Binding("x", ValueKind::Variable)
                      .specType("int", TypeKind::Builtin, BuiltinTypeKind::Int)
                      .derivType(TypeKind::Pointer))
             .binding(Binding("z", ValueKind::Variable)
                      .specType("y", TypeKind::Synonym)
                      .derivType(TypeKind::Pointer)));
}

void BinderTest::case1103()
{
    bind("int * x , * y ;",
         Expectation()
             .binding(Binding("x", ValueKind::Variable)
                      .specType("int", TypeKind::Builtin, BuiltinTypeKind::Int)
                      .derivType(TypeKind::Pointer))
             .binding(Binding("y", ValueKind::Variable)
                      .specType("int", TypeKind::Builtin, BuiltinTypeKind::Int)
                      .derivType(TypeKind::Pointer)));
}

void BinderTest::case1104()
{
    bind("int ( * x ) [ 1 ];",
         Expectation()
             .binding(Binding("x", ValueKind::Variable)
                      .specType("int", TypeKind::Builtin, BuiltinTypeKind::Int)
                      .derivType(TypeKind::Array)
                      .derivType(TypeKind::Pointer)));
}

void BinderTest::case1105()
{
}

void BinderTest::case1106() {}
void BinderTest::case1107() {}
void BinderTest::case1108() {}
void BinderTest::case1109() {}
void BinderTest::case1110() {}
void BinderTest::case1111() {}
void BinderTest::case1112() {}
void BinderTest::case1113() {}
void BinderTest::case1114() {}
void BinderTest::case1115() {}
void BinderTest::case1116() {}
void BinderTest::case1117() {}
void BinderTest::case1118() {}
void BinderTest::case1119() {}
void BinderTest::case1120() {}
void BinderTest::case1121() {}
void BinderTest::case1122() {}
void BinderTest::case1123() {}
void BinderTest::case1124() {}
void BinderTest::case1125() {}
void BinderTest::case1126() {}
void BinderTest::case1127() {}
void BinderTest::case1128() {}
void BinderTest::case1129() {}
void BinderTest::case1130() {}
void BinderTest::case1131() {}
void BinderTest::case1132() {}
void BinderTest::case1133() {}
void BinderTest::case1134() {}
void BinderTest::case1135() {}
void BinderTest::case1136() {}
void BinderTest::case1137() {}
void BinderTest::case1138() {}
void BinderTest::case1139() {}
void BinderTest::case1140() {}
void BinderTest::case1141() {}
void BinderTest::case1142() {}
void BinderTest::case1143() {}
void BinderTest::case1144() {}
void BinderTest::case1145() {}
void BinderTest::case1146() {}
void BinderTest::case1147() {}
void BinderTest::case1148() {}
void BinderTest::case1149() {}

void BinderTest::case1150()
{
    bind("const int * x ;",
         Expectation()
            .qualObjPtr_1("x", ValueKind::Variable, Expectation::Qual::Const, TypeKind::Builtin, BuiltinTypeKind::Int));
}

void BinderTest::case1151()
{
    bind("const x * y ;",
         Expectation()
            .qualObjPtr_1("y", ValueKind::Variable, Expectation::Qual::Const, TypeKind::Synonym));
}

void BinderTest::case1152() {}
void BinderTest::case1153() {}
void BinderTest::case1154() {}
void BinderTest::case1155() {}
void BinderTest::case1156() {}
void BinderTest::case1157() {}
void BinderTest::case1158() {}
void BinderTest::case1159() {}
void BinderTest::case1160() {}
void BinderTest::case1161() {}
void BinderTest::case1162() {}
void BinderTest::case1163() {}
void BinderTest::case1164() {}
void BinderTest::case1165() {}
void BinderTest::case1166() {}
void BinderTest::case1167() {}
void BinderTest::case1168() {}
void BinderTest::case1169() {}
void BinderTest::case1170() {}
void BinderTest::case1171() {}
void BinderTest::case1172() {}
void BinderTest::case1173() {}
void BinderTest::case1174() {}
void BinderTest::case1175() {}
void BinderTest::case1176() {}
void BinderTest::case1177() {}
void BinderTest::case1178() {}
void BinderTest::case1179() {}
void BinderTest::case1180() {}
void BinderTest::case1181() {}
void BinderTest::case1182() {}
void BinderTest::case1183() {}
void BinderTest::case1184() {}
void BinderTest::case1185() {}
void BinderTest::case1186() {}
void BinderTest::case1187() {}
void BinderTest::case1188() {}
void BinderTest::case1189() {}
void BinderTest::case1190() {}
void BinderTest::case1191() {}
void BinderTest::case1192() {}
void BinderTest::case1193() {}
void BinderTest::case1194() {}
void BinderTest::case1195() {}
void BinderTest::case1196() {}
void BinderTest::case1197() {}
void BinderTest::case1198() {}
void BinderTest::case1199()
{}

void BinderTest::case1200()
{
    bind("const int * const x ;",
         Expectation()
            .qualObjQualPtr_1("x", ValueKind::Variable, Expectation::Qual::Const, Expectation::Qual::Const, TypeKind::Builtin, BuiltinTypeKind::Int));
}

void BinderTest::case1201() { }
void BinderTest::case1202() { }
void BinderTest::case1203() { }
void BinderTest::case1204() { }
void BinderTest::case1205() { }
void BinderTest::case1206() { }
void BinderTest::case1207() { }
void BinderTest::case1208() { }
void BinderTest::case1209() { }
void BinderTest::case1210() { }
void BinderTest::case1211() { }
void BinderTest::case1212() { }
void BinderTest::case1213() { }
void BinderTest::case1214() { }
void BinderTest::case1215() { }
void BinderTest::case1216() { }
void BinderTest::case1217() { }
void BinderTest::case1218() { }
void BinderTest::case1219() { }
void BinderTest::case1220() { }
void BinderTest::case1221() { }
void BinderTest::case1222() { }
void BinderTest::case1223() { }
void BinderTest::case1224() { }
void BinderTest::case1225() { }
void BinderTest::case1226() { }
void BinderTest::case1227() { }
void BinderTest::case1228() { }
void BinderTest::case1229() { }
void BinderTest::case1230() { }
void BinderTest::case1231() { }
void BinderTest::case1232() { }
void BinderTest::case1233() { }
void BinderTest::case1234() { }
void BinderTest::case1235() { }
void BinderTest::case1236() { }
void BinderTest::case1237() { }
void BinderTest::case1238() { }
void BinderTest::case1239() { }
void BinderTest::case1240() { }
void BinderTest::case1241() { }
void BinderTest::case1242() { }
void BinderTest::case1243() { }
void BinderTest::case1244() { }
void BinderTest::case1245() { }
void BinderTest::case1246() { }
void BinderTest::case1247() { }
void BinderTest::case1248() { }
void BinderTest::case1249() { }

void BinderTest::case1250()
{
    bind("int * const x ;",
         Expectation()
            .qualPtr_1("x", ValueKind::Variable, Expectation::Qual::Const, TypeKind::Builtin, BuiltinTypeKind::Int));
}

void BinderTest::case1251()
{
    bind("x * const y ;",
         Expectation()
            .qualPtr_1("y", ValueKind::Variable, Expectation::Qual::Const, TypeKind::Synonym));
}

void BinderTest::case1252() { }
void BinderTest::case1253() { }
void BinderTest::case1254() { }
void BinderTest::case1255() { }
void BinderTest::case1256() { }
void BinderTest::case1257() { }
void BinderTest::case1258() { }
void BinderTest::case1259() { }
void BinderTest::case1260() { }
void BinderTest::case1261() { }
void BinderTest::case1262() { }
void BinderTest::case1263() { }
void BinderTest::case1264() { }
void BinderTest::case1265() { }
void BinderTest::case1266() { }
void BinderTest::case1267() { }
void BinderTest::case1268() { }
void BinderTest::case1269() { }
void BinderTest::case1270() { }
void BinderTest::case1271() { }
void BinderTest::case1272() { }
void BinderTest::case1273() { }
void BinderTest::case1274() { }
void BinderTest::case1275() { }
void BinderTest::case1276() { }
void BinderTest::case1277() { }
void BinderTest::case1278() { }
void BinderTest::case1279() { }
void BinderTest::case1280() { }
void BinderTest::case1281() { }
void BinderTest::case1282() { }
void BinderTest::case1283() { }
void BinderTest::case1284() { }
void BinderTest::case1285() { }
void BinderTest::case1286() { }
void BinderTest::case1287() { }
void BinderTest::case1288() { }
void BinderTest::case1289() { }
void BinderTest::case1290() { }
void BinderTest::case1291() { }
void BinderTest::case1292() { }
void BinderTest::case1293() { }
void BinderTest::case1294() { }
void BinderTest::case1295() { }
void BinderTest::case1296() { }
void BinderTest::case1297() { }
void BinderTest::case1298() { }
void BinderTest::case1299() { }

void BinderTest::case1300()
{
    bind("int x [ 1 ] ;",
         Expectation()
            .arr_1("x", ValueKind::Variable, TypeKind::Builtin, BuiltinTypeKind::Int));
}

void BinderTest::case1301()
{
    bind("x y [ 1 ] ;",
         Expectation()
            .arr_1("y", ValueKind::Variable, TypeKind::Synonym));
}

void BinderTest::case1302()
{
    bind("int x [ 1 ] , y [ 2 ] ;",
         Expectation()
            .arr_1("x", ValueKind::Variable, TypeKind::Builtin, BuiltinTypeKind::Int)
            .arr_1("y", ValueKind::Variable, TypeKind::Builtin, BuiltinTypeKind::Int));
}

void BinderTest::case1303()
{
    bind("x y [ 1 ] , z [ 2 ] ;",
         Expectation()
            .arr_1("y", ValueKind::Variable, TypeKind::Synonym)
            .arr_1("z", ValueKind::Variable, TypeKind::Synonym));
}

void BinderTest::case1304()
{
    bind("int * x [ 1 ] ;",
         Expectation()
            .arr_1("x", ValueKind::Variable, TypeKind::Pointer));
}

void BinderTest::case1305()
{
    bind("int x [ 1 ] , * y [ 2 ] ;",
         Expectation()
            .arr_1("x", ValueKind::Variable, TypeKind::Builtin, BuiltinTypeKind::Int)
            .arr_1("y", ValueKind::Variable, TypeKind::Pointer));

}
void BinderTest::case1306(){ }
void BinderTest::case1307(){ }
void BinderTest::case1308(){ }
void BinderTest::case1309(){ }
void BinderTest::case1310(){ }
void BinderTest::case1311(){ }
void BinderTest::case1312(){ }
void BinderTest::case1313(){ }
void BinderTest::case1314(){ }
void BinderTest::case1315(){ }
void BinderTest::case1316(){ }
void BinderTest::case1317(){ }
void BinderTest::case1318(){ }
void BinderTest::case1319(){ }
void BinderTest::case1320(){ }
void BinderTest::case1321(){ }
void BinderTest::case1322(){ }
void BinderTest::case1323(){ }
void BinderTest::case1324(){ }
void BinderTest::case1325(){ }
void BinderTest::case1326(){ }
void BinderTest::case1327(){ }
void BinderTest::case1328(){ }
void BinderTest::case1329(){ }
void BinderTest::case1330(){ }
void BinderTest::case1331(){ }
void BinderTest::case1332(){ }
void BinderTest::case1333(){ }
void BinderTest::case1334(){ }
void BinderTest::case1335(){ }
void BinderTest::case1336(){ }
void BinderTest::case1337(){ }
void BinderTest::case1338(){ }
void BinderTest::case1339(){ }
void BinderTest::case1340(){ }
void BinderTest::case1341(){ }
void BinderTest::case1342(){ }
void BinderTest::case1343(){ }
void BinderTest::case1344(){ }
void BinderTest::case1345(){ }
void BinderTest::case1346(){ }
void BinderTest::case1347(){ }
void BinderTest::case1348(){ }
void BinderTest::case1349(){ }

void BinderTest::case1350()
{
    bind("const int x [ 1 ] ;",
         Expectation()
            .arr_1_ofQualTy("x", ValueKind::Variable, Expectation::Qual::Const, TypeKind::Builtin, BuiltinTypeKind::Int));
}

void BinderTest::case1351(){ }
void BinderTest::case1352(){ }
void BinderTest::case1353(){ }
void BinderTest::case1354(){ }
void BinderTest::case1355(){ }
void BinderTest::case1356(){ }
void BinderTest::case1357(){ }
void BinderTest::case1358(){ }
void BinderTest::case1359(){ }
void BinderTest::case1360(){ }
void BinderTest::case1361(){ }
void BinderTest::case1362(){ }
void BinderTest::case1363(){ }
void BinderTest::case1364(){ }
void BinderTest::case1365(){ }
void BinderTest::case1366(){ }
void BinderTest::case1367(){ }
void BinderTest::case1368(){ }
void BinderTest::case1369(){ }
void BinderTest::case1370(){ }
void BinderTest::case1371(){ }
void BinderTest::case1372(){ }
void BinderTest::case1373(){ }
void BinderTest::case1374(){ }
void BinderTest::case1375(){ }
void BinderTest::case1376(){ }
void BinderTest::case1377(){ }
void BinderTest::case1378(){ }
void BinderTest::case1379(){ }
void BinderTest::case1380(){ }
void BinderTest::case1381(){ }
void BinderTest::case1382(){ }
void BinderTest::case1383(){ }
void BinderTest::case1384(){ }
void BinderTest::case1385(){ }
void BinderTest::case1386(){ }
void BinderTest::case1387(){ }
void BinderTest::case1388(){ }
void BinderTest::case1389(){ }
void BinderTest::case1390(){ }
void BinderTest::case1391(){ }
void BinderTest::case1392(){ }
void BinderTest::case1393(){ }
void BinderTest::case1394(){ }
void BinderTest::case1395(){ }
void BinderTest::case1396(){ }
void BinderTest::case1397(){ }
void BinderTest::case1398(){ }
void BinderTest::case1399(){ }
