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

    /*
            + 0000-0099 ->
            + 0100-0199 ->
            + 0200-0299 ->

        Globals and locals:
            + 1000-1049 -> objects (plain)
            + 1050-1099 -> `const' objects
            + 1100-1149 -> pointers
            + 1150-1199 -> `const' object pointers
            + 1200-1249 -> `const' object `const' pointers
            + 1250-1299 -> `const' pointers
            + 1300-

        Fields:
            + 2000-2049 -> objects (plain)
            + 2050-2099 -> `const' objects
            + 2100-2149 -> pointers
            + 2150-2199 -> `const' object pointers
            + 2200-2249 -> `const' object `const' pointers
            + 2250-2299 -> `const' pointers
            + 2300-

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

    void case0100();
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

    // 1000-1999

    void case1000();
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

    // 2000-2999

    void case2000();
    void case2001();
    void case2002();
    void case2003();
    void case2004();
    void case2005();
    void case2006();
    void case2007();
    void case2008();
    void case2009();
    void case2010();
    void case2011();
    void case2012();
    void case2013();
    void case2014();
    void case2015();
    void case2016();
    void case2017();
    void case2018();
    void case2019();
    void case2020();
    void case2021();
    void case2022();
    void case2023();
    void case2024();
    void case2025();
    void case2026();
    void case2027();
    void case2028();
    void case2029();
    void case2030();
    void case2031();
    void case2032();
    void case2033();
    void case2034();
    void case2035();
    void case2036();
    void case2037();
    void case2038();
    void case2039();
    void case2040();
    void case2041();
    void case2042();
    void case2043();
    void case2044();
    void case2045();
    void case2046();
    void case2047();
    void case2048();
    void case2049();
    void case2050();
    void case2051();
    void case2052();
    void case2053();
    void case2054();
    void case2055();
    void case2056();
    void case2057();
    void case2058();
    void case2059();
    void case2060();
    void case2061();
    void case2062();
    void case2063();
    void case2064();
    void case2065();
    void case2066();
    void case2067();
    void case2068();
    void case2069();
    void case2070();
    void case2071();
    void case2072();
    void case2073();
    void case2074();
    void case2075();
    void case2076();
    void case2077();
    void case2078();
    void case2079();
    void case2080();
    void case2081();
    void case2082();
    void case2083();
    void case2084();
    void case2085();
    void case2086();
    void case2087();
    void case2088();
    void case2089();
    void case2090();
    void case2091();
    void case2092();
    void case2093();
    void case2094();
    void case2095();
    void case2096();
    void case2097();
    void case2098();
    void case2099();

    void case2100();
    void case2101();
    void case2102();
    void case2103();
    void case2104();
    void case2105();
    void case2106();
    void case2107();
    void case2108();
    void case2109();
    void case2110();
    void case2111();
    void case2112();
    void case2113();
    void case2114();
    void case2115();
    void case2116();
    void case2117();
    void case2118();
    void case2119();
    void case2120();
    void case2121();
    void case2122();
    void case2123();
    void case2124();
    void case2125();
    void case2126();
    void case2127();
    void case2128();
    void case2129();
    void case2130();
    void case2131();
    void case2132();
    void case2133();
    void case2134();
    void case2135();
    void case2136();
    void case2137();
    void case2138();
    void case2139();
    void case2140();
    void case2141();
    void case2142();
    void case2143();
    void case2144();
    void case2145();
    void case2146();
    void case2147();
    void case2148();
    void case2149();
    void case2150();
    void case2151();
    void case2152();
    void case2153();
    void case2154();
    void case2155();
    void case2156();
    void case2157();
    void case2158();
    void case2159();
    void case2160();
    void case2161();
    void case2162();
    void case2163();
    void case2164();
    void case2165();
    void case2166();
    void case2167();
    void case2168();
    void case2169();
    void case2170();
    void case2171();
    void case2172();
    void case2173();
    void case2174();
    void case2175();
    void case2176();
    void case2177();
    void case2178();
    void case2179();
    void case2180();
    void case2181();
    void case2182();
    void case2183();
    void case2184();
    void case2185();
    void case2186();
    void case2187();
    void case2188();
    void case2189();
    void case2190();
    void case2191();
    void case2192();
    void case2193();
    void case2194();
    void case2195();
    void case2196();
    void case2197();
    void case2198();
    void case2199();

    void case2200();
    void case2201();
    void case2202();
    void case2203();
    void case2204();
    void case2205();
    void case2206();
    void case2207();
    void case2208();
    void case2209();
    void case2210();
    void case2211();
    void case2212();
    void case2213();
    void case2214();
    void case2215();
    void case2216();
    void case2217();
    void case2218();
    void case2219();
    void case2220();
    void case2221();
    void case2222();
    void case2223();
    void case2224();
    void case2225();
    void case2226();
    void case2227();
    void case2228();
    void case2229();
    void case2230();
    void case2231();
    void case2232();
    void case2233();
    void case2234();
    void case2235();
    void case2236();
    void case2237();
    void case2238();
    void case2239();
    void case2240();
    void case2241();
    void case2242();
    void case2243();
    void case2244();
    void case2245();
    void case2246();
    void case2247();
    void case2248();
    void case2249();
    void case2250();
    void case2251();
    void case2252();
    void case2253();
    void case2254();
    void case2255();
    void case2256();
    void case2257();
    void case2258();
    void case2259();
    void case2260();
    void case2261();
    void case2262();
    void case2263();
    void case2264();
    void case2265();
    void case2266();
    void case2267();
    void case2268();
    void case2269();
    void case2270();
    void case2271();
    void case2272();
    void case2273();
    void case2274();
    void case2275();
    void case2276();
    void case2277();
    void case2278();
    void case2279();
    void case2280();
    void case2281();
    void case2282();
    void case2283();
    void case2284();
    void case2285();
    void case2286();
    void case2287();
    void case2288();
    void case2289();
    void case2290();
    void case2291();
    void case2292();
    void case2293();
    void case2294();
    void case2295();
    void case2296();
    void case2297();
    void case2298();
    void case2299();

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

        TEST_BINDER(case0100),
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

        TEST_BINDER(case1000),
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
        TEST_BINDER(case1299),

        TEST_BINDER(case2000),
        TEST_BINDER(case2001),
        TEST_BINDER(case2002),
        TEST_BINDER(case2003),
        TEST_BINDER(case2004),
        TEST_BINDER(case2005),
        TEST_BINDER(case2006),
        TEST_BINDER(case2007),
        TEST_BINDER(case2008),
        TEST_BINDER(case2009),
        TEST_BINDER(case2010),
        TEST_BINDER(case2011),
        TEST_BINDER(case2012),
        TEST_BINDER(case2013),
        TEST_BINDER(case2014),
        TEST_BINDER(case2015),
        TEST_BINDER(case2016),
        TEST_BINDER(case2017),
        TEST_BINDER(case2018),
        TEST_BINDER(case2019),
        TEST_BINDER(case2020),
        TEST_BINDER(case2021),
        TEST_BINDER(case2022),
        TEST_BINDER(case2023),
        TEST_BINDER(case2024),
        TEST_BINDER(case2025),
        TEST_BINDER(case2026),
        TEST_BINDER(case2027),
        TEST_BINDER(case2028),
        TEST_BINDER(case2029),
        TEST_BINDER(case2030),
        TEST_BINDER(case2031),
        TEST_BINDER(case2032),
        TEST_BINDER(case2033),
        TEST_BINDER(case2034),
        TEST_BINDER(case2035),
        TEST_BINDER(case2036),
        TEST_BINDER(case2037),
        TEST_BINDER(case2038),
        TEST_BINDER(case2039),
        TEST_BINDER(case2040),
        TEST_BINDER(case2041),
        TEST_BINDER(case2042),
        TEST_BINDER(case2043),
        TEST_BINDER(case2044),
        TEST_BINDER(case2045),
        TEST_BINDER(case2046),
        TEST_BINDER(case2047),
        TEST_BINDER(case2048),
        TEST_BINDER(case2049),
        TEST_BINDER(case2050),
        TEST_BINDER(case2051),
        TEST_BINDER(case2052),
        TEST_BINDER(case2053),
        TEST_BINDER(case2054),
        TEST_BINDER(case2055),
        TEST_BINDER(case2056),
        TEST_BINDER(case2057),
        TEST_BINDER(case2058),
        TEST_BINDER(case2059),
        TEST_BINDER(case2060),
        TEST_BINDER(case2061),
        TEST_BINDER(case2062),
        TEST_BINDER(case2063),
        TEST_BINDER(case2064),
        TEST_BINDER(case2065),
        TEST_BINDER(case2066),
        TEST_BINDER(case2067),
        TEST_BINDER(case2068),
        TEST_BINDER(case2069),
        TEST_BINDER(case2070),
        TEST_BINDER(case2071),
        TEST_BINDER(case2072),
        TEST_BINDER(case2073),
        TEST_BINDER(case2074),
        TEST_BINDER(case2075),
        TEST_BINDER(case2076),
        TEST_BINDER(case2077),
        TEST_BINDER(case2078),
        TEST_BINDER(case2079),
        TEST_BINDER(case2080),
        TEST_BINDER(case2081),
        TEST_BINDER(case2082),
        TEST_BINDER(case2083),
        TEST_BINDER(case2084),
        TEST_BINDER(case2085),
        TEST_BINDER(case2086),
        TEST_BINDER(case2087),
        TEST_BINDER(case2088),
        TEST_BINDER(case2089),
        TEST_BINDER(case2090),
        TEST_BINDER(case2091),
        TEST_BINDER(case2092),
        TEST_BINDER(case2093),
        TEST_BINDER(case2094),
        TEST_BINDER(case2095),
        TEST_BINDER(case2096),
        TEST_BINDER(case2097),
        TEST_BINDER(case2098),
        TEST_BINDER(case2099),

        TEST_BINDER(case2100),
        TEST_BINDER(case2101),
        TEST_BINDER(case2102),
        TEST_BINDER(case2103),
        TEST_BINDER(case2104),
        TEST_BINDER(case2105),
        TEST_BINDER(case2106),
        TEST_BINDER(case2107),
        TEST_BINDER(case2108),
        TEST_BINDER(case2109),
        TEST_BINDER(case2110),
        TEST_BINDER(case2111),
        TEST_BINDER(case2112),
        TEST_BINDER(case2113),
        TEST_BINDER(case2114),
        TEST_BINDER(case2115),
        TEST_BINDER(case2116),
        TEST_BINDER(case2117),
        TEST_BINDER(case2118),
        TEST_BINDER(case2119),
        TEST_BINDER(case2120),
        TEST_BINDER(case2121),
        TEST_BINDER(case2122),
        TEST_BINDER(case2123),
        TEST_BINDER(case2124),
        TEST_BINDER(case2125),
        TEST_BINDER(case2126),
        TEST_BINDER(case2127),
        TEST_BINDER(case2128),
        TEST_BINDER(case2129),
        TEST_BINDER(case2130),
        TEST_BINDER(case2131),
        TEST_BINDER(case2132),
        TEST_BINDER(case2133),
        TEST_BINDER(case2134),
        TEST_BINDER(case2135),
        TEST_BINDER(case2136),
        TEST_BINDER(case2137),
        TEST_BINDER(case2138),
        TEST_BINDER(case2139),
        TEST_BINDER(case2140),
        TEST_BINDER(case2141),
        TEST_BINDER(case2142),
        TEST_BINDER(case2143),
        TEST_BINDER(case2144),
        TEST_BINDER(case2145),
        TEST_BINDER(case2146),
        TEST_BINDER(case2147),
        TEST_BINDER(case2148),
        TEST_BINDER(case2149),
        TEST_BINDER(case2150),
        TEST_BINDER(case2151),
        TEST_BINDER(case2152),
        TEST_BINDER(case2153),
        TEST_BINDER(case2154),
        TEST_BINDER(case2155),
        TEST_BINDER(case2156),
        TEST_BINDER(case2157),
        TEST_BINDER(case2158),
        TEST_BINDER(case2159),
        TEST_BINDER(case2160),
        TEST_BINDER(case2161),
        TEST_BINDER(case2162),
        TEST_BINDER(case2163),
        TEST_BINDER(case2164),
        TEST_BINDER(case2165),
        TEST_BINDER(case2166),
        TEST_BINDER(case2167),
        TEST_BINDER(case2168),
        TEST_BINDER(case2169),
        TEST_BINDER(case2170),
        TEST_BINDER(case2171),
        TEST_BINDER(case2172),
        TEST_BINDER(case2173),
        TEST_BINDER(case2174),
        TEST_BINDER(case2175),
        TEST_BINDER(case2176),
        TEST_BINDER(case2177),
        TEST_BINDER(case2178),
        TEST_BINDER(case2179),
        TEST_BINDER(case2180),
        TEST_BINDER(case2181),
        TEST_BINDER(case2182),
        TEST_BINDER(case2183),
        TEST_BINDER(case2184),
        TEST_BINDER(case2185),
        TEST_BINDER(case2186),
        TEST_BINDER(case2187),
        TEST_BINDER(case2188),
        TEST_BINDER(case2189),
        TEST_BINDER(case2190),
        TEST_BINDER(case2191),
        TEST_BINDER(case2192),
        TEST_BINDER(case2193),
        TEST_BINDER(case2194),
        TEST_BINDER(case2195),
        TEST_BINDER(case2196),
        TEST_BINDER(case2197),
        TEST_BINDER(case2198),
        TEST_BINDER(case2199),

        TEST_BINDER(case2200),
        TEST_BINDER(case2201),
        TEST_BINDER(case2202),
        TEST_BINDER(case2203),
        TEST_BINDER(case2204),
        TEST_BINDER(case2205),
        TEST_BINDER(case2206),
        TEST_BINDER(case2207),
        TEST_BINDER(case2208),
        TEST_BINDER(case2209),
        TEST_BINDER(case2210),
        TEST_BINDER(case2211),
        TEST_BINDER(case2212),
        TEST_BINDER(case2213),
        TEST_BINDER(case2214),
        TEST_BINDER(case2215),
        TEST_BINDER(case2216),
        TEST_BINDER(case2217),
        TEST_BINDER(case2218),
        TEST_BINDER(case2219),
        TEST_BINDER(case2220),
        TEST_BINDER(case2221),
        TEST_BINDER(case2222),
        TEST_BINDER(case2223),
        TEST_BINDER(case2224),
        TEST_BINDER(case2225),
        TEST_BINDER(case2226),
        TEST_BINDER(case2227),
        TEST_BINDER(case2228),
        TEST_BINDER(case2229),
        TEST_BINDER(case2230),
        TEST_BINDER(case2231),
        TEST_BINDER(case2232),
        TEST_BINDER(case2233),
        TEST_BINDER(case2234),
        TEST_BINDER(case2235),
        TEST_BINDER(case2236),
        TEST_BINDER(case2237),
        TEST_BINDER(case2238),
        TEST_BINDER(case2239),
        TEST_BINDER(case2240),
        TEST_BINDER(case2241),
        TEST_BINDER(case2242),
        TEST_BINDER(case2243),
        TEST_BINDER(case2244),
        TEST_BINDER(case2245),
        TEST_BINDER(case2246),
        TEST_BINDER(case2247),
        TEST_BINDER(case2248),
        TEST_BINDER(case2249),
        TEST_BINDER(case2250),
        TEST_BINDER(case2251),
        TEST_BINDER(case2252),
        TEST_BINDER(case2253),
        TEST_BINDER(case2254),
        TEST_BINDER(case2255),
        TEST_BINDER(case2256),
        TEST_BINDER(case2257),
        TEST_BINDER(case2258),
        TEST_BINDER(case2259),
        TEST_BINDER(case2260),
        TEST_BINDER(case2261),
        TEST_BINDER(case2262),
        TEST_BINDER(case2263),
        TEST_BINDER(case2264),
        TEST_BINDER(case2265),
        TEST_BINDER(case2266),
        TEST_BINDER(case2267),
        TEST_BINDER(case2268),
        TEST_BINDER(case2269),
        TEST_BINDER(case2270),
        TEST_BINDER(case2271),
        TEST_BINDER(case2272),
        TEST_BINDER(case2273),
        TEST_BINDER(case2274),
        TEST_BINDER(case2275),
        TEST_BINDER(case2276),
        TEST_BINDER(case2277),
        TEST_BINDER(case2278),
        TEST_BINDER(case2279),
        TEST_BINDER(case2280),
        TEST_BINDER(case2281),
        TEST_BINDER(case2282),
        TEST_BINDER(case2283),
        TEST_BINDER(case2284),
        TEST_BINDER(case2285),
        TEST_BINDER(case2286),
        TEST_BINDER(case2287),
        TEST_BINDER(case2288),
        TEST_BINDER(case2289),
        TEST_BINDER(case2290),
        TEST_BINDER(case2291),
        TEST_BINDER(case2292),
        TEST_BINDER(case2293),
        TEST_BINDER(case2294),
        TEST_BINDER(case2295),
        TEST_BINDER(case2296),
        TEST_BINDER(case2297),
        TEST_BINDER(case2298),
        TEST_BINDER(case2299)
    };
};

} // C
} // psy

#endif
