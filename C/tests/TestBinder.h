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

#ifndef PSYCHE_C_TEST_BINDING_H__
#define PSYCHE_C_TEST_BINDING_H__

#include "TestFrontend.h"
#include "C/Fwds.h"

#define TEST_BINDER(Function) TestFunction { &TestBinder::Function, #Function }

namespace psy {
namespace C {

class TestBinder final : public TestFrontend
{
public:
    static const std::string Name;

    virtual std::string name() const override { return Name; }

    void testAll() override;

    virtual void bind(std::string text,
                      Expectation X = Expectation()) override;

    static void TEMP(Symbol* sym);


    /*
        ...:
            + 0000-0099 ->
            + 0100-0199 ->
            + 0200-0299 ->

        ...:
            + 1000-1099 ->
            + 1100-1199 ->
            + 1200-1299 ->
     */

    void case0001();
    void case0002();
    void case0003();
    void case0004();
    void case0005();
    void case0006();
    void case0007();
    void case0008();
    void case0009();
    void case0010();
    void case0011();
    void case0012();
    void case0013();
    void case0014();
    void case0015();
    void case0016();
    void case0017();
    void case0018();
    void case0019();
    void case0020();
    void case0021();
    void case0022();
    void case0023();
    void case0024();
    void case0025();
    void case0026();
    void case0027();
    void case0028();
    void case0029();
    void case0030();
    void case0031();
    void case0032();
    void case0033();
    void case0034();
    void case0035();
    void case0036();
    void case0037();
    void case0038();
    void case0039();
    void case0040();
    void case0041();
    void case0042();
    void case0043();
    void case0044();
    void case0045();
    void case0046();
    void case0047();
    void case0048();
    void case0049();
    void case0050();
    void case0051();
    void case0052();
    void case0053();
    void case0054();
    void case0055();
    void case0056();
    void case0057();
    void case0058();
    void case0059();
    void case0060();
    void case0061();
    void case0062();
    void case0063();
    void case0064();
    void case0065();
    void case0066();
    void case0067();
    void case0068();
    void case0069();
    void case0070();
    void case0071();
    void case0072();
    void case0073();
    void case0074();
    void case0075();
    void case0076();
    void case0077();
    void case0078();
    void case0079();
    void case0080();
    void case0081();
    void case0082();
    void case0083();
    void case0084();
    void case0085();
    void case0086();
    void case0087();
    void case0088();
    void case0089();
    void case0090();
    void case0091();
    void case0092();
    void case0093();
    void case0094();
    void case0095();
    void case0096();
    void case0097();
    void case0098();
    void case0099();

    void case0101();
    void case0102();
    void case0103();
    void case0104();
    void case0105();
    void case0106();
    void case0107();
    void case0108();
    void case0109();
    void case0110();
    void case0111();
    void case0112();
    void case0113();
    void case0114();
    void case0115();
    void case0116();
    void case0117();
    void case0118();
    void case0119();
    void case0120();
    void case0121();
    void case0122();
    void case0123();
    void case0124();
    void case0125();
    void case0126();
    void case0127();
    void case0128();
    void case0129();
    void case0130();
    void case0131();
    void case0132();
    void case0133();
    void case0134();
    void case0135();
    void case0136();
    void case0137();
    void case0138();
    void case0139();
    void case0140();
    void case0141();
    void case0142();
    void case0143();
    void case0144();
    void case0145();
    void case0146();
    void case0147();
    void case0148();
    void case0149();
    void case0150();
    void case0151();
    void case0152();
    void case0153();
    void case0154();
    void case0155();
    void case0156();
    void case0157();
    void case0158();
    void case0159();
    void case0160();
    void case0161();
    void case0162();
    void case0163();
    void case0164();
    void case0165();
    void case0166();
    void case0167();
    void case0168();
    void case0169();
    void case0170();
    void case0171();
    void case0172();
    void case0173();
    void case0174();
    void case0175();
    void case0176();
    void case0177();
    void case0178();
    void case0179();
    void case0180();
    void case0181();
    void case0182();
    void case0183();
    void case0184();
    void case0185();
    void case0186();
    void case0187();
    void case0188();
    void case0189();
    void case0190();
    void case0191();
    void case0192();
    void case0193();
    void case0194();
    void case0195();
    void case0196();
    void case0197();
    void case0198();
    void case0199();

    void case1001();
    void case1002();
    void case1003();
    void case1004();
    void case1005();
    void case1006();
    void case1007();
    void case1008();
    void case1009();
    void case1010();
    void case1011();
    void case1012();
    void case1013();
    void case1014();
    void case1015();
    void case1016();
    void case1017();
    void case1018();
    void case1019();
    void case1020();
    void case1021();
    void case1022();
    void case1023();
    void case1024();
    void case1025();
    void case1026();
    void case1027();
    void case1028();
    void case1029();
    void case1030();
    void case1031();
    void case1032();
    void case1033();
    void case1034();
    void case1035();
    void case1036();
    void case1037();
    void case1038();
    void case1039();
    void case1040();
    void case1041();
    void case1042();
    void case1043();
    void case1044();
    void case1045();
    void case1046();
    void case1047();
    void case1048();
    void case1049();
    void case1050();
    void case1051();
    void case1052();
    void case1053();
    void case1054();
    void case1055();
    void case1056();
    void case1057();
    void case1058();
    void case1059();
    void case1060();
    void case1061();
    void case1062();
    void case1063();
    void case1064();
    void case1065();
    void case1066();
    void case1067();
    void case1068();
    void case1069();
    void case1070();
    void case1071();
    void case1072();
    void case1073();
    void case1074();
    void case1075();
    void case1076();
    void case1077();
    void case1078();
    void case1079();
    void case1080();
    void case1081();
    void case1082();
    void case1083();
    void case1084();
    void case1085();
    void case1086();
    void case1087();
    void case1088();
    void case1089();
    void case1090();
    void case1091();
    void case1092();
    void case1093();
    void case1094();
    void case1095();
    void case1096();
    void case1097();
    void case1098();
    void case1099();

    void case1100();
    void case1101();
    void case1102();
    void case1103();
    void case1104();
    void case1105();
    void case1106();
    void case1107();
    void case1108();
    void case1109();
    void case1110();
    void case1111();
    void case1112();
    void case1113();
    void case1114();
    void case1115();
    void case1116();
    void case1117();
    void case1118();
    void case1119();
    void case1120();
    void case1121();
    void case1122();
    void case1123();
    void case1124();
    void case1125();
    void case1126();
    void case1127();
    void case1128();
    void case1129();
    void case1130();
    void case1131();
    void case1132();
    void case1133();
    void case1134();
    void case1135();
    void case1136();
    void case1137();
    void case1138();
    void case1139();
    void case1140();
    void case1141();
    void case1142();
    void case1143();
    void case1144();
    void case1145();
    void case1146();
    void case1147();
    void case1148();
    void case1149();
    void case1150();
    void case1151();
    void case1152();
    void case1153();
    void case1154();
    void case1155();
    void case1156();
    void case1157();
    void case1158();
    void case1159();
    void case1160();
    void case1161();
    void case1162();
    void case1163();
    void case1164();
    void case1165();
    void case1166();
    void case1167();
    void case1168();
    void case1169();
    void case1170();
    void case1171();
    void case1172();
    void case1173();
    void case1174();
    void case1175();
    void case1176();
    void case1177();
    void case1178();
    void case1179();
    void case1180();
    void case1181();
    void case1182();
    void case1183();
    void case1184();
    void case1185();
    void case1186();
    void case1187();
    void case1188();
    void case1189();
    void case1190();
    void case1191();
    void case1192();
    void case1193();
    void case1194();
    void case1195();
    void case1196();
    void case1197();
    void case1198();
    void case1199();

    void case1200();
    void case1201();
    void case1202();
    void case1203();
    void case1204();
    void case1205();
    void case1206();
    void case1207();
    void case1208();
    void case1209();
    void case1210();
    void case1211();
    void case1212();
    void case1213();
    void case1214();
    void case1215();
    void case1216();
    void case1217();
    void case1218();
    void case1219();
    void case1220();
    void case1221();
    void case1222();
    void case1223();
    void case1224();
    void case1225();
    void case1226();
    void case1227();
    void case1228();
    void case1229();
    void case1230();
    void case1231();
    void case1232();
    void case1233();
    void case1234();
    void case1235();
    void case1236();
    void case1237();
    void case1238();
    void case1239();
    void case1240();
    void case1241();
    void case1242();
    void case1243();
    void case1244();
    void case1245();
    void case1246();
    void case1247();
    void case1248();
    void case1249();
    void case1250();
    void case1251();
    void case1252();
    void case1253();
    void case1254();
    void case1255();
    void case1256();
    void case1257();
    void case1258();
    void case1259();
    void case1260();
    void case1261();
    void case1262();
    void case1263();
    void case1264();
    void case1265();
    void case1266();
    void case1267();
    void case1268();
    void case1269();
    void case1270();
    void case1271();
    void case1272();
    void case1273();
    void case1274();
    void case1275();
    void case1276();
    void case1277();
    void case1278();
    void case1279();
    void case1280();
    void case1281();
    void case1282();
    void case1283();
    void case1284();
    void case1285();
    void case1286();
    void case1287();
    void case1288();
    void case1289();
    void case1290();
    void case1291();
    void case1292();
    void case1293();
    void case1294();
    void case1295();
    void case1296();
    void case1297();
    void case1298();
    void case1299();

private:
    using TestFunction = std::pair<std::function<void(TestBinder*)>, const char*>;

    void setUp() override;
    void tearDown() override;

    void f();

    std::vector<TestFunction> tests_
    {
        TEST_BINDER(case0001),
        TEST_BINDER(case0002),
        TEST_BINDER(case0003),
        TEST_BINDER(case0004),
        TEST_BINDER(case0005),
        TEST_BINDER(case0006),
        TEST_BINDER(case0007),
        TEST_BINDER(case0008),
        TEST_BINDER(case0009),
        TEST_BINDER(case0010),
        TEST_BINDER(case0011),
        TEST_BINDER(case0012),
        TEST_BINDER(case0013),
        TEST_BINDER(case0014),
        TEST_BINDER(case0015),
        TEST_BINDER(case0016),
        TEST_BINDER(case0017),
        TEST_BINDER(case0018),
        TEST_BINDER(case0019),
        TEST_BINDER(case0020),
        TEST_BINDER(case0021),
        TEST_BINDER(case0022),
        TEST_BINDER(case0023),
        TEST_BINDER(case0024),
        TEST_BINDER(case0025),
        TEST_BINDER(case0026),
        TEST_BINDER(case0027),
        TEST_BINDER(case0028),
        TEST_BINDER(case0029),
        TEST_BINDER(case0030),
        TEST_BINDER(case0031),
        TEST_BINDER(case0032),
        TEST_BINDER(case0033),
        TEST_BINDER(case0034),
        TEST_BINDER(case0035),
        TEST_BINDER(case0036),
        TEST_BINDER(case0037),
        TEST_BINDER(case0038),
        TEST_BINDER(case0039),
        TEST_BINDER(case0040),
        TEST_BINDER(case0041),
        TEST_BINDER(case0042),
        TEST_BINDER(case0043),
        TEST_BINDER(case0044),
        TEST_BINDER(case0045),
        TEST_BINDER(case0046),
        TEST_BINDER(case0047),
        TEST_BINDER(case0048),
        TEST_BINDER(case0049),
        TEST_BINDER(case0050),
        TEST_BINDER(case0051),
        TEST_BINDER(case0052),
        TEST_BINDER(case0053),
        TEST_BINDER(case0054),
        TEST_BINDER(case0055),
        TEST_BINDER(case0056),
        TEST_BINDER(case0057),
        TEST_BINDER(case0058),
        TEST_BINDER(case0059),
        TEST_BINDER(case0060),
        TEST_BINDER(case0061),
        TEST_BINDER(case0062),
        TEST_BINDER(case0063),
        TEST_BINDER(case0064),
        TEST_BINDER(case0065),
        TEST_BINDER(case0066),
        TEST_BINDER(case0067),
        TEST_BINDER(case0068),
        TEST_BINDER(case0069),
        TEST_BINDER(case0070),
        TEST_BINDER(case0071),
        TEST_BINDER(case0072),
        TEST_BINDER(case0073),
        TEST_BINDER(case0074),
        TEST_BINDER(case0075),
        TEST_BINDER(case0076),
        TEST_BINDER(case0077),
        TEST_BINDER(case0078),
        TEST_BINDER(case0079),
        TEST_BINDER(case0080),
        TEST_BINDER(case0081),
        TEST_BINDER(case0082),
        TEST_BINDER(case0083),
        TEST_BINDER(case0084),
        TEST_BINDER(case0085),
        TEST_BINDER(case0086),
        TEST_BINDER(case0087),
        TEST_BINDER(case0088),
        TEST_BINDER(case0089),
        TEST_BINDER(case0090),
        TEST_BINDER(case0091),
        TEST_BINDER(case0092),
        TEST_BINDER(case0093),
        TEST_BINDER(case0094),
        TEST_BINDER(case0095),
        TEST_BINDER(case0096),
        TEST_BINDER(case0097),
        TEST_BINDER(case0098),
        TEST_BINDER(case0099),

        TEST_BINDER(case0101),
        TEST_BINDER(case0102),
        TEST_BINDER(case0103),
        TEST_BINDER(case0104),
        TEST_BINDER(case0105),
        TEST_BINDER(case0106),
        TEST_BINDER(case0107),
        TEST_BINDER(case0108),
        TEST_BINDER(case0109),
        TEST_BINDER(case0110),
        TEST_BINDER(case0111),
        TEST_BINDER(case0112),
        TEST_BINDER(case0113),
        TEST_BINDER(case0114),
        TEST_BINDER(case0115),
        TEST_BINDER(case0116),
        TEST_BINDER(case0117),
        TEST_BINDER(case0118),
        TEST_BINDER(case0119),
        TEST_BINDER(case0120),
        TEST_BINDER(case0121),
        TEST_BINDER(case0122),
        TEST_BINDER(case0123),
        TEST_BINDER(case0124),
        TEST_BINDER(case0125),
        TEST_BINDER(case0126),
        TEST_BINDER(case0127),
        TEST_BINDER(case0128),
        TEST_BINDER(case0129),
        TEST_BINDER(case0130),
        TEST_BINDER(case0131),
        TEST_BINDER(case0132),
        TEST_BINDER(case0133),
        TEST_BINDER(case0134),
        TEST_BINDER(case0135),
        TEST_BINDER(case0136),
        TEST_BINDER(case0137),
        TEST_BINDER(case0138),
        TEST_BINDER(case0139),
        TEST_BINDER(case0140),
        TEST_BINDER(case0141),
        TEST_BINDER(case0142),
        TEST_BINDER(case0143),
        TEST_BINDER(case0144),
        TEST_BINDER(case0145),
        TEST_BINDER(case0146),
        TEST_BINDER(case0147),
        TEST_BINDER(case0148),
        TEST_BINDER(case0149),
        TEST_BINDER(case0150),
        TEST_BINDER(case0151),
        TEST_BINDER(case0152),
        TEST_BINDER(case0153),
        TEST_BINDER(case0154),
        TEST_BINDER(case0155),
        TEST_BINDER(case0156),
        TEST_BINDER(case0157),
        TEST_BINDER(case0158),
        TEST_BINDER(case0159),
        TEST_BINDER(case0160),
        TEST_BINDER(case0161),
        TEST_BINDER(case0162),
        TEST_BINDER(case0163),
        TEST_BINDER(case0164),
        TEST_BINDER(case0165),
        TEST_BINDER(case0166),
        TEST_BINDER(case0167),
        TEST_BINDER(case0168),
        TEST_BINDER(case0169),
        TEST_BINDER(case0170),
        TEST_BINDER(case0171),
        TEST_BINDER(case0172),
        TEST_BINDER(case0173),
        TEST_BINDER(case0174),
        TEST_BINDER(case0175),
        TEST_BINDER(case0176),
        TEST_BINDER(case0177),
        TEST_BINDER(case0178),
        TEST_BINDER(case0179),
        TEST_BINDER(case0180),
        TEST_BINDER(case0181),
        TEST_BINDER(case0182),
        TEST_BINDER(case0183),
        TEST_BINDER(case0184),
        TEST_BINDER(case0185),
        TEST_BINDER(case0186),
        TEST_BINDER(case0187),
        TEST_BINDER(case0188),
        TEST_BINDER(case0189),
        TEST_BINDER(case0190),
        TEST_BINDER(case0191),
        TEST_BINDER(case0192),
        TEST_BINDER(case0193),
        TEST_BINDER(case0194),
        TEST_BINDER(case0195),
        TEST_BINDER(case0196),
        TEST_BINDER(case0197),
        TEST_BINDER(case0198),
        TEST_BINDER(case0199),

        TEST_BINDER(case1001),
        TEST_BINDER(case1002),
        TEST_BINDER(case1003),
        TEST_BINDER(case1004),
        TEST_BINDER(case1005),
        TEST_BINDER(case1006),
        TEST_BINDER(case1007),
        TEST_BINDER(case1008),
        TEST_BINDER(case1009),
        TEST_BINDER(case1010),
        TEST_BINDER(case1011),
        TEST_BINDER(case1012),
        TEST_BINDER(case1013),
        TEST_BINDER(case1014),
        TEST_BINDER(case1015),
        TEST_BINDER(case1016),
        TEST_BINDER(case1017),
        TEST_BINDER(case1018),
        TEST_BINDER(case1019),
        TEST_BINDER(case1020),
        TEST_BINDER(case1021),
        TEST_BINDER(case1022),
        TEST_BINDER(case1023),
        TEST_BINDER(case1024),
        TEST_BINDER(case1025),
        TEST_BINDER(case1026),
        TEST_BINDER(case1027),
        TEST_BINDER(case1028),
        TEST_BINDER(case1029),
        TEST_BINDER(case1030),
        TEST_BINDER(case1031),
        TEST_BINDER(case1032),
        TEST_BINDER(case1033),
        TEST_BINDER(case1034),
        TEST_BINDER(case1035),
        TEST_BINDER(case1036),
        TEST_BINDER(case1037),
        TEST_BINDER(case1038),
        TEST_BINDER(case1039),
        TEST_BINDER(case1040),
        TEST_BINDER(case1041),
        TEST_BINDER(case1042),
        TEST_BINDER(case1043),
        TEST_BINDER(case1044),
        TEST_BINDER(case1045),
        TEST_BINDER(case1046),
        TEST_BINDER(case1047),
        TEST_BINDER(case1048),
        TEST_BINDER(case1049),
        TEST_BINDER(case1050),
        TEST_BINDER(case1051),
        TEST_BINDER(case1052),
        TEST_BINDER(case1053),
        TEST_BINDER(case1054),
        TEST_BINDER(case1055),
        TEST_BINDER(case1056),
        TEST_BINDER(case1057),
        TEST_BINDER(case1058),
        TEST_BINDER(case1059),
        TEST_BINDER(case1060),
        TEST_BINDER(case1061),
        TEST_BINDER(case1062),
        TEST_BINDER(case1063),
        TEST_BINDER(case1064),
        TEST_BINDER(case1065),
        TEST_BINDER(case1066),
        TEST_BINDER(case1067),
        TEST_BINDER(case1068),
        TEST_BINDER(case1069),
        TEST_BINDER(case1070),
        TEST_BINDER(case1071),
        TEST_BINDER(case1072),
        TEST_BINDER(case1073),
        TEST_BINDER(case1074),
        TEST_BINDER(case1075),
        TEST_BINDER(case1076),
        TEST_BINDER(case1077),
        TEST_BINDER(case1078),
        TEST_BINDER(case1079),
        TEST_BINDER(case1080),
        TEST_BINDER(case1081),
        TEST_BINDER(case1082),
        TEST_BINDER(case1083),
        TEST_BINDER(case1084),
        TEST_BINDER(case1085),
        TEST_BINDER(case1086),
        TEST_BINDER(case1087),
        TEST_BINDER(case1088),
        TEST_BINDER(case1089),
        TEST_BINDER(case1090),
        TEST_BINDER(case1091),
        TEST_BINDER(case1092),
        TEST_BINDER(case1093),
        TEST_BINDER(case1094),
        TEST_BINDER(case1095),
        TEST_BINDER(case1096),
        TEST_BINDER(case1097),
        TEST_BINDER(case1098),
        TEST_BINDER(case1099),

        TEST_BINDER(case1100),
        TEST_BINDER(case1101),
        TEST_BINDER(case1102),
        TEST_BINDER(case1103),
        TEST_BINDER(case1104),
        TEST_BINDER(case1105),
        TEST_BINDER(case1106),
        TEST_BINDER(case1107),
        TEST_BINDER(case1108),
        TEST_BINDER(case1109),
        TEST_BINDER(case1110),
        TEST_BINDER(case1111),
        TEST_BINDER(case1112),
        TEST_BINDER(case1113),
        TEST_BINDER(case1114),
        TEST_BINDER(case1115),
        TEST_BINDER(case1116),
        TEST_BINDER(case1117),
        TEST_BINDER(case1118),
        TEST_BINDER(case1119),
        TEST_BINDER(case1120),
        TEST_BINDER(case1121),
        TEST_BINDER(case1122),
        TEST_BINDER(case1123),
        TEST_BINDER(case1124),
        TEST_BINDER(case1125),
        TEST_BINDER(case1126),
        TEST_BINDER(case1127),
        TEST_BINDER(case1128),
        TEST_BINDER(case1129),
        TEST_BINDER(case1130),
        TEST_BINDER(case1131),
        TEST_BINDER(case1132),
        TEST_BINDER(case1133),
        TEST_BINDER(case1134),
        TEST_BINDER(case1135),
        TEST_BINDER(case1136),
        TEST_BINDER(case1137),
        TEST_BINDER(case1138),
        TEST_BINDER(case1139),
        TEST_BINDER(case1140),
        TEST_BINDER(case1141),
        TEST_BINDER(case1142),
        TEST_BINDER(case1143),
        TEST_BINDER(case1144),
        TEST_BINDER(case1145),
        TEST_BINDER(case1146),
        TEST_BINDER(case1147),
        TEST_BINDER(case1148),
        TEST_BINDER(case1149),
        TEST_BINDER(case1150),
        TEST_BINDER(case1151),
        TEST_BINDER(case1152),
        TEST_BINDER(case1153),
        TEST_BINDER(case1154),
        TEST_BINDER(case1155),
        TEST_BINDER(case1156),
        TEST_BINDER(case1157),
        TEST_BINDER(case1158),
        TEST_BINDER(case1159),
        TEST_BINDER(case1160),
        TEST_BINDER(case1161),
        TEST_BINDER(case1162),
        TEST_BINDER(case1163),
        TEST_BINDER(case1164),
        TEST_BINDER(case1165),
        TEST_BINDER(case1166),
        TEST_BINDER(case1167),
        TEST_BINDER(case1168),
        TEST_BINDER(case1169),
        TEST_BINDER(case1170),
        TEST_BINDER(case1171),
        TEST_BINDER(case1172),
        TEST_BINDER(case1173),
        TEST_BINDER(case1174),
        TEST_BINDER(case1175),
        TEST_BINDER(case1176),
        TEST_BINDER(case1177),
        TEST_BINDER(case1178),
        TEST_BINDER(case1179),
        TEST_BINDER(case1180),
        TEST_BINDER(case1181),
        TEST_BINDER(case1182),
        TEST_BINDER(case1183),
        TEST_BINDER(case1184),
        TEST_BINDER(case1185),
        TEST_BINDER(case1186),
        TEST_BINDER(case1187),
        TEST_BINDER(case1188),
        TEST_BINDER(case1189),
        TEST_BINDER(case1190),
        TEST_BINDER(case1191),
        TEST_BINDER(case1192),
        TEST_BINDER(case1193),
        TEST_BINDER(case1194),
        TEST_BINDER(case1195),
        TEST_BINDER(case1196),
        TEST_BINDER(case1197),
        TEST_BINDER(case1198),
        TEST_BINDER(case1199),

        TEST_BINDER(case1200),
        TEST_BINDER(case1201),
        TEST_BINDER(case1202),
        TEST_BINDER(case1203),
        TEST_BINDER(case1204),
        TEST_BINDER(case1205),
        TEST_BINDER(case1206),
        TEST_BINDER(case1207),
        TEST_BINDER(case1208),
        TEST_BINDER(case1209),
        TEST_BINDER(case1210),
        TEST_BINDER(case1211),
        TEST_BINDER(case1212),
        TEST_BINDER(case1213),
        TEST_BINDER(case1214),
        TEST_BINDER(case1215),
        TEST_BINDER(case1216),
        TEST_BINDER(case1217),
        TEST_BINDER(case1218),
        TEST_BINDER(case1219),
        TEST_BINDER(case1220),
        TEST_BINDER(case1221),
        TEST_BINDER(case1222),
        TEST_BINDER(case1223),
        TEST_BINDER(case1224),
        TEST_BINDER(case1225),
        TEST_BINDER(case1226),
        TEST_BINDER(case1227),
        TEST_BINDER(case1228),
        TEST_BINDER(case1229),
        TEST_BINDER(case1230),
        TEST_BINDER(case1231),
        TEST_BINDER(case1232),
        TEST_BINDER(case1233),
        TEST_BINDER(case1234),
        TEST_BINDER(case1235),
        TEST_BINDER(case1236),
        TEST_BINDER(case1237),
        TEST_BINDER(case1238),
        TEST_BINDER(case1239),
        TEST_BINDER(case1240),
        TEST_BINDER(case1241),
        TEST_BINDER(case1242),
        TEST_BINDER(case1243),
        TEST_BINDER(case1244),
        TEST_BINDER(case1245),
        TEST_BINDER(case1246),
        TEST_BINDER(case1247),
        TEST_BINDER(case1248),
        TEST_BINDER(case1249),
        TEST_BINDER(case1250),
        TEST_BINDER(case1251),
        TEST_BINDER(case1252),
        TEST_BINDER(case1253),
        TEST_BINDER(case1254),
        TEST_BINDER(case1255),
        TEST_BINDER(case1256),
        TEST_BINDER(case1257),
        TEST_BINDER(case1258),
        TEST_BINDER(case1259),
        TEST_BINDER(case1260),
        TEST_BINDER(case1261),
        TEST_BINDER(case1262),
        TEST_BINDER(case1263),
        TEST_BINDER(case1264),
        TEST_BINDER(case1265),
        TEST_BINDER(case1266),
        TEST_BINDER(case1267),
        TEST_BINDER(case1268),
        TEST_BINDER(case1269),
        TEST_BINDER(case1270),
        TEST_BINDER(case1271),
        TEST_BINDER(case1272),
        TEST_BINDER(case1273),
        TEST_BINDER(case1274),
        TEST_BINDER(case1275),
        TEST_BINDER(case1276),
        TEST_BINDER(case1277),
        TEST_BINDER(case1278),
        TEST_BINDER(case1279),
        TEST_BINDER(case1280),
        TEST_BINDER(case1281),
        TEST_BINDER(case1282),
        TEST_BINDER(case1283),
        TEST_BINDER(case1284),
        TEST_BINDER(case1285),
        TEST_BINDER(case1286),
        TEST_BINDER(case1287),
        TEST_BINDER(case1288),
        TEST_BINDER(case1289),
        TEST_BINDER(case1290),
        TEST_BINDER(case1291),
        TEST_BINDER(case1292),
        TEST_BINDER(case1293),
        TEST_BINDER(case1294),
        TEST_BINDER(case1295),
        TEST_BINDER(case1296),
        TEST_BINDER(case1297),
        TEST_BINDER(case1298),
        TEST_BINDER(case1299)

    };
};

} // C
} // psy

#endif
