#pragma once
#include <Arduino.h>
#include "Catalog.h"

const char *constellation_txt[] = {
  "And",  "Ant",  "Aps",  "Aql",  "Aqr",  "Ara",  "Ari",  "Aur",  "Boo",  "Cae",  "Cam",  "Cap",  "Car",  "Cas",  "Cen",  "Cep",  "Cet",  "Cha",  "Cir",  "CMa",  "CMi",
  "Cnc",  "Col",  "Com",  "CrA",  "CrB",  "Crt",  "Cru",  "Crv",  "CVn",  "Cyg",  "Del",  "Dor",  "Dra",  "Equ",  "Eri",  "For",  "Gem",  "Gru",  "Her",
  "Hor",  "Hya",  "Hyi",  "Ind",  "Lac",  "Leo",  "Lep",  "Lib",  "LMi",  "Lup",  "Lyn",  "Lyr",  "Men",  "Mic",  "Mon",  "Mus",  "Nor",  "Oct",  "Oph",
  "Ori",  "Pav",  "Peg",  "Per",  "Phe",  "Pic",  "PsA",  "Psc",  "Pup",  "Pyx",  "Ret",  "Scl",  "Sco",  "Sct",  "Ser",  "Sex",  "Sge",  "Sgr",  "Tau",
  "Tel",  "TrA",  "Tri",  "Tuc",  "UMa",  "UMi",  "Vel",  "Vir",  "Vol",  "Vul" };

const char * catalog_txt[] = { "", "M ", "NGC " };
//Herschel
extern const char * Herschel_info_txt[] = { "Bright Nebulae", "Faint Nebulae", "Very Faint Nebulae","Planetary Nebulae","Very Large Nebulae",
"Very Rich" , "Bright & Faint", "Scattered" };
const unsigned short Herschel_NGC[400] =
{ 40,129,136,157,185,205,225,246,247,253,278,288,381,404,436,457,488,524,559,584,596,598,613,615,637,651,654,659,663,720,752,772,779,869,884,891,908,936,1022,1023,1027,1052,1055,1084,1245,1342,1407,1444,1501,1502,1513,1528,1535,1545,1647,1664,1788,1817,1857,1907,1931,1961,1964,1980,1999,2022,2024,2126,2129,2158,2169,2185,2186,2194,2204,2215,2232,2244,2251,2264,2266,2281,2286,2301,2304,2311,2324,2335,2343,2353,2354,2355,2360,2362,2371,2372,2392,2395,2403,2419,2420,2421,2422,2423,2438,2440,2479,2482,2489,2506,2509,2527,2539,2548,2567,2571,2613,2627,2655,2681,2683,2742,2768,2775,2782,2787,2811,2841,2859,2903,2950,2964,2974,2976,2985,3034,3077,3079,3115,3147,3166,3169,3184,3190,3193,3198,3226,3227,3242,3245,3277,3294,3310,3344,3377,3379,3384,3395,3412,3414,3432,3486,3489,3504,3521,3556,3593,3607,3608,3610,3613,3619,3621,3626,3628,3631,3640,3655,3665,3675,3686,3726,3729,3810,3813,3877,3893,3898,3900,3912,3938,3941,3945,3949,3953,3962,3982,3992,3998,4026,4027,4030,4036,4038,4041,4051,4085,4088,4102,4111,4143,4147,4150,4151,4179,4203,4214,4216,4245,4251,4258,4261,4273,4274,4278,4281,4293,4303,4314,4346,4350,4361,4365,4371,4394,4414,4419,4429,4435,4438,4442,4448,4449,4450,4459,4473,4477,4478,4485,4490,4494,4526,4527,4535,4536,4546,4548,4550,4559,4565,4570,4594,4596,4618,4631,4636,4643,4654,4656,4660,4665,4666,4689,4697,4698,4699,4725,4753,4754,4762,4781,4800,4845,4856,4866,4900,4958,4995,5005,5033,5054,5195,5248,5273,5322,5363,5364,5466,5473,5474,5557,5566,5576,5631,5634,5676,5689,5694,5746,5846,5866,5897,5907,5982,6118,6144,6171,6207,6217,6229,6235,6284,6287,6293,6304,6316,6342,6355,6356,6369,6401,6426,6440,6445,6451,6514,6517,6520,6522,6528,6540,6543,6544,6553,6568,6569,6583,6624,6629,6633,6638,6642,6645,6664,6712,6755,6756,6781,6802,6818,6823,6826,6830,6834,6866,6882,6885,6905,6910,6934,6939,6940,6946,7000,7006,7008,7009,7044,7062,7086,7128,7142,7160,7209,7217,7243,7296,7331,7380,7448,7479,7510,7606,7662,7686,7723,7727,7789,7790,7814 };
extern const byte Herschel_info[400] =
{ 4,8,6,2,2,5,8,5,5,5,1,6,8,2,8,7,3,1,7,1,2,5,1,1,7,1,7,8,6,1,7,1,1,6,6,5,1,4,1,1,8,1,1,1,6,8,1,8,4,7,7,7,4,8,8,8,5,7,7,7,1,3,4,5,4,4,5,8,8,6,8,4,7,6,7,7,8,7,8,5,6,8,8,6,6,8,7,8,8,8,7,6,7,7,2,2,4,8,5,1,6,7,8,7,4,4,7,7,7,6,8,8,7,6,7,6,2,7,1,1,1,1,1,1,1,1,2,1,1,1,4,1,1,1,1,4,1,5,1,1,1,1,1,2,2,1,2,2,4,1,2,1,4,1,2,1,1,1,1,2,1,1,2,1,1,5,1,2,2,1,1,1,1,2,5,1,2,1,1,1,2,2,1,1,1,1,2,1,1,2,1,1,1,1,5,1,4,4,1,1,2,1,1,4,1,4,1,1,1,1,4,1,1,1,1,1,1,1,1,1,4,2,2,1,1,2,4,1,1,1,2,1,1,1,2,1,1,2,1,1,2,1,1,2,1,2,2,2,1,1,1,1,2,2,5,1,2,1,1,5,1,1,1,1,4,2,1,2,1,2,1,1,2,1,1,1,1,1,1,2,1,1,2,1,1,1,1,1,1,1,2,1,1,1,1,1,2,6,1,1,1,1,1,1,1,1,1,2,1,1,1,6,2,2,2,6,6,2,1,4,2,2,2,6,1,1,1,1,1,4,1,2,1,2,6,5,2,7,1,2,2,4,2,4,7,2,7,1,2,8,1,2,6,8,1,7,7,3,6,4,7,4,7,8,7,8,8,4,8,1,6,7,4,5,1,1,4,6,7,6,7,7,8,7,2,8,7,1,8,2,1,7,1,4,8,1,1,6,7,2 };
const unsigned short Herschel_ra[400] =
{ 306,810,861,969,1083,1128,1215,1338,1338,1353,1476,1506,1956,1998,2172,2277,2373,2463,2583,2664,2709,2733,2760,2778,2949,2964,3015,3024,3078,3318,3441,3498,3516,4065,4167,4179,4224,4353,4683,4716,4764,4758,4776,4905,5736,6252,6537,6768,7278,7290,7386,7542,7563,7713,8496,8622,9135,9276,9498,9741,9843,10104,9936,9990,10023,10182,10182,10743,10743,10929,10971,11061,11082,11130,11205,11352,11523,11691,11760,11952,12015,12174,12153,12276,12369,12384,12612,12726,12777,12969,12966,13026,13062,13098,13272,13272,13386,13329,13704,13644,13662,13689,13629,13644,13788,13797,14241,14184,14286,14331,14355,14496,14652,14736,14898,14907,15333,15456,15882,6900,15888,16311,16434,16431,16527,16647,16617,16758,16839,17079,17373,17400,17400,17496,17580,17757,17982,17958,18084,18384,18336,18351,18456,18462,18471,18501,18621,18621,18672,18735,18906,19002,19071,19221,19353,19356,19371,19413,19449,19458,19491,19734,19731,19815,19896,20061,20160,20229,20232,20268,20268,20295,20277,20325,20331,20349,20355,20409,20463,20505,20553,20721,20730,20952,20961,21105,21183,21201,21198,21225,21306,21309,21318,21333,21336,21366,21417,21450,21459,21507,21510,21534,21567,21579,21591,21618,21684,21690,21714,21735,21813,21828,21840,21840,21909,21975,22503,22002,22056,22071,22095,22104,22122,22122,22131,22134,22161,22182,22200,22230,22242,22257,22260,22272,22302,22320,22332,22347,22356,22359,22368,22374,22374,22377,22395,22419,22428,22434,22446,22449,22467,22548,22548,22554,22557,22587,22587,22587,22605,22617,22632,22719,22722,22776,22794,22809,22824,22842,22848,22860,22887,22878,22956,22980,23004,22995,23043,23094,23091,23112,23154,23172,23265,23301,23310,23346,23493,23610,23655,23736,23829,24237,24453,24597,24828,25008,25011,25296,25290,25296,25692,25734,25755,25953,26010,26130,26211,26301,26469,27120,27153,27435,27438,28128,29379,29526,29691,30039,29844,30168,30312,30312,30663,30813,30942,31002,31146,31203,31221,31389,31668,31872,31977,31989,32022,32367,32373,32409,32412,32448,32493,32364,32529,32589,32703,32712,32784,33015,33081,33153,33237,33264,33294,33420,33909,34359,34386,34680,35052,35433,35433,35502,35667,35706,36063,36288,36297,36606,36639,36951,36912,36975,37017,37710,37773,37773,37842,38133,38445,38694,39072,39141,39369,39696,39768,39996,40386,40644,40947,41328,41472,41676,41895,42105,42234,42492,42519,43035,43035,21 };
const short Herschel_dec[400] =
{ 4335,3597,3675,-440,2884,2485,3691,-711,-1259,-1466,2838,-1508,3678,2127,3513,3484,300,556,3782,-413,-403,1824,-1700,-385,3827,3079,3699,3628,3661,-721,2245,1126,-348,3415,3413,2527,-1233,-38,-307,2332,3680,-452,16,-373,2823,2229,-1036,3151,3647,3731,2963,3067,-668,3008,1139,2617,-156,998,2358,2117,2053,4164,-1201,-244,-315,543,-8,2995,1398,1446,838,-348,327,770,-1045,-404,-197,294,204,596,1622,2467,-173,31,1085,-209,68,-600,-566,-588,-1462,832,-867,-1388,1775,1775,1261,821,3937,2340,1301,-1163,-818,-735,-804,-1075,-985,-1430,-1684,-571,-1024,-1679,-679,-262,-1771,-1705,-1272,-1694,4705,3091,2018,3641,3616,435,2419,4165,-954,3072,2084,1304,3545,1925,-151,4088,4351,4196,4138,3357,-392,4419,220,223,2500,1325,1329,2749,1209,1207,-1057,1726,1726,2255,3226,1511,855,771,774,1995,821,1695,2214,1755,850,1695,14,3357,786,1100,1106,3544,3497,3482,-1888,1118,833,3208,211,1011,2342,2632,1050,2839,3204,705,2209,2866,2940,3382,1637,1606,2664,2236,3657,2888,3157,-738,3324,3219,3344,3074,-1021,49,3730,-1045,3745,2688,3038,3049,3179,2601,2569,1129,1841,2381,95,2009,2196,805,1793,1707,2855,366,337,1793,1774,340,1120,285,1810,2836,1018,-1051,456,719,1109,1890,919,683,801,797,605,1734,2662,1041,855,822,835,756,2518,2515,1563,478,176,508,148,-149,886,750,1694,1576,451,-639,627,2485,1969,177,135,803,1946,686,199,12,841,-268,525,-456,1546,55,695,691,-584,2808,111,-794,867,166,-375,-386,2239,2211,-937,2851,548,2155,3626,329,315,1726,3308,3234,2203,251,210,3408,-255,2981,2937,-1541,130,108,3357,-1150,3391,3572,-109,-1444,-663,2216,4698,2857,-1315,-1315,-1282,-1530,-1716,-1675,-1108,-1541,-974,-1336,-1327,192,-1179,-1200,-1789,-1378,-423,-1566,-1798,-1796,-1570,3998,-1499,-1444,-1224,-1810,-1311,-1777,-1366,392,-1468,-1349,-904,-464,-433,249,275,386,1210,-823,1392,3024,1378,1757,2631,1584,1580,1197,2437,434,3628,1688,3598,2648,960,3261,-626,2537,2770,3082,3209,3934,3742,2775,1867,2978,3122,2050,3469,943,723,3618,-434,2534,2931,-766,-686,3386,3656,951 };
const byte Herschel_obj[400] =
{ 4,3,3,2,2,2,3,4,2,2,2,1,3,2,3,3,2,2,3,2,2,2,2,2,3,4,3,3,3,2,3,2,2,3,3,2,2,2,2,2,3,2,2,2,3,3,2,3,4,3,3,3,4,3,3,3,0,3,3,3,0,2,2,0,0,4,0,3,3,3,3,0,3,3,3,3,3,3,3,0,3,3,3,3,3,3,3,3,3,3,3,3,3,3,4,4,4,3,2,1,3,3,3,3,4,4,3,3,3,3,3,3,3,3,3,3,2,3,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,4,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,4,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1,2,2,2,2,2,2,1,2,2,1,2,2,2,1,2,2,2,1,1,2,2,1,1,1,1,1,1,1,1,1,1,4,1,1,1,4,3,3,1,3,1,1,3,4,1,1,3,1,3,1,4,3,1,1,3,3,1,3,3,4,3,4,3,4,3,3,3,3,3,4,3,1,3,3,2,0,1,4,4,3,3,3,3,3,3,3,2,3,3,2,3,2,2,3,2,4,3,2,2,3,3,2 };
const byte Hershel_constellation[400] =
{ 16,14,14,17,14,1,14,17,17,71,14,71,14,1,14,14,67,67,14,17,17,81,71,17,14,63,14,14,14,17,1,7,17,63,63,1,17,17,17,63,14,17,17,36,63,63,36,63,11,11,63,63,36,63,78,8,60,78,8,8,8,11,47,60,60,60,60,8,38,38,60,60,60,60,20,55,55,55,55,55,38,8,55,55,38,55,68,55,55,55,20,38,20,20,38,38,38,38,11,51,38,68,68,68,68,68,68,68,68,55,68,68,68,42,68,68,69,69,11,83,51,83,83,22,51,83,43,83,49,46,83,46,75,83,83,83,83,83,75,34,75,75,83,46,46,83,46,46,42,49,49,49,83,49,46,46,46,46,46,49,49,49,46,49,46,83,46,46,46,83,83,83,42,46,46,83,46,83,83,83,46,83,83,46,83,83,83,46,46,46,83,83,83,83,83,27,83,83,83,83,29,86,83,29,83,83,83,83,83,83,30,24,24,30,86,24,30,86,24,24,30,86,86,24,24,86,24,86,24,30,24,29,86,86,24,24,24,86,86,86,86,24,86,24,24,86,86,86,30,30,24,86,86,86,86,86,24,86,24,24,86,86,86,30,30,86,86,86,30,86,86,86,24,86,86,86,24,86,86,86,86,30,86,86,86,86,86,86,30,30,86,30,9,30,83,86,86,9,83,83,9,86,86,83,86,9,9,42,86,86,34,48,34,34,74,72,59,40,84,40,59,59,59,59,59,59,59,59,59,59,59,59,77,77,77,77,59,77,77,77,77,34,77,77,77,77,77,77,77,59,77,77,77,73,73,4,4,4,88,77,88,31,88,31,31,88,88,32,31,32,16,88,16,31,32,31,5,31,31,31,31,16,16,45,62,45,45,62,16,62,62,16,5,1,1,5,5,14,14,62 };
const byte Hershel_dMag[400] =
{ 102,100,113,112,117,94,91,85,110,89,113,72,92,107,88,75,111,111,75,108,115,67,102,116,75,110,91,98,71,105,70,109,113,44,47,115,110,107,112,105,75,112,115,110,69,71,106,64,133,53,88,62,93,80,60,75,110,79,85,99,95,117,116,0,100,115,107,100,72,110,64,110,95,92,91,86,40,62,85,47,98,69,80,58,101,96,88,91,80,53,90,95,94,105,110,110,95,94,95,115,102,90,45,69,113,115,95,87,94,85,93,80,82,53,83,75,110,83,107,104,96,112,105,107,117,109,117,93,107,91,109,110,110,114,106,88,109,112,93,109,114,117,96,113,115,110,115,114,90,112,120,114,101,110,105,95,102,120,104,110,114,110,115,109,105,110,113,96,111,112,112,117,105,105,109,112,107,113,114,115,114,108,117,108,117,109,113,115,115,115,115,98,108,110,107,113,113,108,113,107,115,110,107,115,110,110,118,109,118,97,110,94,116,116,116,110,103,104,111,102,86,103,116,108,103,113,115,101,108,116,110,108,111,116,112,97,114,112,103,108,108,114,92,100,109,101,107,109,116,97,96,109,115,110,109,100,108,117,106,102,109,87,114,117,93,104,106,110,112,109,111,114,115,105,113,93,89,108,105,110,112,111,115,115,114,113,109,112,98,103,115,84,113,115,100,107,110,85,114,114,116,104,117,114,104,112,114,110,101,105,108,73,113,109,115,105,92,113,115,87,104,105,99,95,98,100,100,105,95,99,110,115,104,110,85,69,105,81,95,105,110,88,100,100,85,104,115,95,106,55,102,105,85,89,100,90,107,110,110,100,98,88,90,103,90,55,91,120,75,100,100,65,105,0,103,135,84,113,116,94,112,104,66,76,110,76,94,97,88,112,116,88,115,90,80,111,107,96,71,120 };

const unsigned short Messier_ra[110] =
{ 10035, 38805, 24666, 29508, 27558, 31803, 32217, 32514, 31176, 30513, 33933, 30216, 30051, 31728, 38700, 32964, 33024, 32997, 30678, 32478, 32538, 33492, 32304, 32907, 33348, 33756, 35988, 33135, 36717, 39012, 1281, 1281, 2817, 4860, 11067, 10083, 10572, 9861, 38766, 22272, 12210, 10062, 10068, 15603, 6810, 13854, 13698, 14814, 22494, 12696, 24297, 42126, 23787, 34053, 35400, 34698, 34008, 22731, 22860, 22911, 22257, 30636, 23874, 23301, 20367, 20406, 15912, 22785, 33432, 33696, 35814, 37605, 37767, 2901, 36183, 3072, 4881, 10401, 9735, 29310, 17868, 17874, 24510, 22353, 22362, 22386, 22524, 22560, 22671, 22704, 22662, 31113, 13938, 23127, 19320, 19404, 20244, 22014, 22164, 22287, 25299, 27195, 2796, 22800, 19434, 22170, 29775, 20145, 21528, 1212 };
const short Messier_dec[110] =
{ 1321,49,1703,-1528,125,-1907,-1991,-1417,-1049,-234,-344,-3,2188,-165,730,-733,-949,-1012,-1544,-1378,-1290,-1326,-1139,-1051,-1125,-516,1363,-1388,2312,-1369,2476,2452,1839,2567,1460,2048,1953,2150,2906,3485,-1156,-273,-284,1199,1447,-791,-810,-252,480,-460,2832,3695,1090,-1771,-1742,1811,1982,709,699,693,268,-1793,2522,1301,785,779,709,-1515,-1899,-1902,1127,-688,-682,947,-1205,3094,1,3,-1407,-1261,4144,4181,-1688,773,1091,777,744,865,753,790,870,2588,-1328,2467,702,709,3301,894,865,949,3261,3345,3642,-623,755,2838,-777,3340,3203,2501 };
const byte Messier_obj[110] =
{ 4,1,1,1,1,3,3,0,1,1,3,1,1,1,1,0,0,3,1,0,3,1,3,3,3,3,4,1,3,1,2,2,2,3,3,3,3,3,3,3,3,0,0,3,3,3,3,3,2,3,2,3,1,1,1,1,4,2,2,2,2,1,2,2,2,2,3,1,1,1,1,1,3,2,1,4,2,0,1,1,2,2,2,2,2,2,2,2,2,2,2,1,3,2,2,2,4,2,2,2,2,2,3,2,2,2,1,2,2,2 };
const byte Messier_constellation[110] =
{ 78, 5, 30, 72, 74, 72, 72, 77, 59, 59, 73, 59, 40, 59, 62, 74, 77, 77, 59, 77, 77, 77, 77, 77, 77, 73, 88, 77, 31, 12, 1, 1, 81, 63, 38, 8, 8, 8, 31, 83, 20, 60, 60, 22, 78, 68, 68, 42, 86, 55, 30, 14, 24, 77, 77, 52, 52, 86, 86, 86, 86, 59, 30, 24, 46, 46, 22, 42, 77, 77, 76, 5, 5, 67, 77, 63, 17, 60, 47, 72, 83, 83, 42, 86, 24, 86, 86, 24, 86, 86, 24, 40, 68, 30, 46, 46, 83, 24, 24, 24, 83, 34, 14, 86, 46, 30, 59, 83, 83, 1 };
const byte Messier_dMag[110] =
{90, 75, 70, 75, 70, 45, 35, 50, 90, 75, 70, 80, 70, 95, 75, 65, 70, 80, 85, 50, 70, 65, 60, 115, 49, 95, 75, 85, 90, 85, 45, 100, 70, 60, 55, 65, 60, 70, 55, 90, 50, 50, 70, 40, 14, 65, 45, 55, 100, 70, 80, 80, 85, 85, 70, 95, 95, 110, 115, 105, 105, 80, 85, 90, 105, 100, 75, 90, 90, 90, 85, 100, 90, 105, 95, 120, 105, 80, 85, 85, 85, 95, 85, 110, 105, 110, 110, 110, 115, 110, 115, 75, 65, 95, 110, 105, 120, 110, 105, 105, 85, 105, 70, 95, 110, 95, 100, 110, 110, 100
};

const byte Star_letter[292] = { 1,2,3,4,1,3,4,6,7,8,11,1,2,4,1,2,3,6,1,2,1,2,5,7,8,9,1,2,4,7,2,4,1,2,5,8,9,20,22,24,2,3,4,5,7,7,1,1,2,3,5,7,8,9,10,11,12,13,1,2,3,4,6,7,9,1,2,3,7,15,19,1,1,2,4,5,6,7,15,18,1,2,2,1,2,1,1,2,3,4,2,3,4,5,1,2,3,4,5,6,1,2,2,3,4,6,7,9,1,2,3,4,8,1,1,2,3,4,5,7,12,14,1,2,3,5,1,2,4,6,12,16,1,3,5,6,13,14,16,1,2,3,1,1,2,3,3,4,5,6,7,8,15,1,2,5,6,12,1,2,1,2,3,5,6,7,9,1,1,2,3,1,2,1,2,4,5,6,7,8,10,13,1,2,3,4,5,6,7,9,10,11,16,1,2,1,2,3,5,6,7,8,12,1,2,3,4,5,6,17,1,2,3,1,1,6,13,14,16,17,18,19,1,1,4,5,7,8,9,10,11,12,16,18,19,20,1,7,14,3,3,4,5,6,7,11,14,16,18,19,21,1,2,5,6,7,8,11,1,1,2,3,1,2,1,1,2,3,4,5,6,7,8,9,11,12,13,15,23,1,2,3,3,4,10,11,12,21,1,3,4,5 };
const byte Star_constellation[292] = { 1,1,1,1,4,4,4,4,4,4,4,5,5,5,6,6,6,6,7,7,8,8,8,8,8,8,9,9,9,9,12,12,13,13,13,13,13,13,13,13,14,14,14,14,14,14,15,15,15,15,15,15,15,15,15,15,15,15,16,16,16,16,16,16,16,17,17,17,17,17,17,19,20,20,20,20,20,20,20,20,21,21,22,23,23,26,28,28,28,28,29,29,29,29,31,31,31,31,31,31,33,33,34,34,34,34,34,34,36,36,36,36,36,38,38,38,38,38,38,38,38,38,39,39,39,39,40,40,40,40,40,40,42,42,42,42,42,42,42,43,43,43,44,46,46,46,46,46,46,46,46,46,46,47,47,47,47,47,48,48,50,50,50,50,50,50,50,51,52,52,52,56,56,59,59,59,59,59,59,59,59,59,60,60,60,60,60,60,60,60,60,60,60,61,61,62,62,62,62,62,62,62,62,63,63,63,63,63,63,63,64,64,64,65,66,68,68,68,68,68,68,68,70,72,72,72,72,72,72,72,72,72,72,72,72,72,74,74,74,76,77,77,77,77,77,77,77,77,77,77,77,78,78,78,78,78,78,78,79,80,80,80,81,81,82,83,83,83,83,83,83,83,83,83,83,83,83,83,83,84,84,84,85,85,85,85,85,85,86,86,86,86 };
const unsigned short Star_ra[292] = { 246,2085,3711,1176,35721,35583,34962,34359,35769,36336,34383,39768,38742,41238,31548,31350,31353,30549,3810,3435,9492,10779,9051,9189,10785,8904,25668,27051,27459,25038,36624,39207,11514,16596,15072,19284,16710,17607,14301,18408,270,1698,2568,3423,1209,1470,26385,26385,25308,22839,24591,26259,25398,24012,26970,20871,24885,24882,38352,38658,42579,40470,39921,37356,41085,5463,1305,4896,2055,4176,3120,26466,12153,11478,12846,12552,11403,13317,12690,12645,13776,13410,14889,10185,10524,28038,22392,23028,22530,22050,22626,22071,22491,21900,37239,35115,36660,35544,37380,38187,8217,10008,31509,32295,34578,30864,29517,27744,2925,9231,7137,6693,5343,13632,13635,13956,11925,13200,12114,11241,11484,12153,39840,40875,39414,41052,31038,29706,31047,30033,31986,31047,17025,23961,15798,16056,19485,20787,25386,3561,762,6819,37119,18246,21267,18594,18594,20220,17571,18495,18216,20220,17430,9981,9846,9159,10407,9384,26721,27504,26451,26949,28047,27675,27360,28797,25776,16827,33504,33897,34161,22710,22983,31647,31899,29226,29346,29910,30906,31257,30528,32364,10650,9441,9750,9957,10083,10218,9729,10059,10431,10050,8691,36762,37338,41538,41511,393,39120,40839,40884,39903,41097,6123,5640,5535,6681,7128,7020,5550,786,1977,2649,12249,41325,14502,11928,14079,13110,14625,13473,12294,7632,29670,28806,30300,30957,31713,32022,31869,31602,30351,28764,29430,29871,31518,28323,33036,31722,35961,32568,33024,33120,34272,32922,33234,34128,34488,34053,34404,33765,8271,9786,8058,10125,6822,8052,7215,33201,30246,28644,27558,3387,3882,40149,19908,19851,21411,22059,23220,24111,24816,17181,16170,18510,18666,20349,15300,20085,4437,26721,27621,14682,15738,16860,16434,19401,17901,24150,22845,23262,23463 };
const short Star_dec[292] = { 1744,2136,2539,1850,531,636,187,832,60,50,-187,20,-265,-849,-2888,-3269,-3338,-3241,1407,1247,2759,2697,2630,2473,2233,1990,1152,2425,2000,1105,-792,-951,-3079,-4099,-3510,-3818,-3524,-3897,-3062,-4199,3548,3642,3613,3819,3391,3468,-3550,-3551,-3579,-2824,-3153,-2511,-2139,-2119,-2515,-3780,-2493,-2420,3754,4233,4656,3504,3491,3709,3971,244,-1079,194,-588,-179,-842,-3782,-918,-963,-1536,-1622,-1797,-1722,-1330,-1564,314,498,552,-2035,-2054,1604,-3776,-3500,-3415,-3437,-1358,-989,-931,-1284,2716,1677,2414,2707,2037,1812,-3297,-3690,3139,3089,4060,3944,3691,3539,-3405,-294,-749,-493,-2380,1914,1914,1682,985,1319,1509,1351,1351,774,-2820,-2706,-2197,-3040,864,1290,1491,1896,1664,2209,-441,-1371,386,358,-950,-1810,-1520,-3624,-4603,-4425,-2801,720,876,1192,1192,1233,1427,1406,1007,927,594,-970,-1154,-1297,-790,-947,-959,-518,-2797,-2573,-2451,-2600,-3115,-2257,-2757,2064,2326,2001,1961,-4133,-4075,754,274,-139,-199,-566,-857,-1381,562,-494,444,-467,380,18,-48,-3,-96,-245,-500,596,418,-3315,-3947,911,1684,910,591,649,1812,371,1475,2990,2456,3209,2866,2400,1912,2329,-2500,-2716,-2560,-3604,-1701,-2281,-2569,-1389,-2214,-1423,-2562,-2964,-3691,-1534,-1283,-2023,-2566,-2580,-2393,-2338,-2214,-2277,-1554,-1465,-1667,-2203,386,-66,-876,1169,-1775,-1690,-2017,-1687,-2114,-1474,-1253,-1259,-1541,-1580,-1620,990,1716,1150,1268,1445,952,749,-2642,-4138,-3755,-4041,1774,2098,-3583,3706,3384,3223,3423,3360,3298,2961,3102,2884,2576,2491,1987,3644,2671,5355,4451,4311,-2801,-3199,-3300,-2556,-2916,-3207,-652,-34,205,659 };

void getcatdms(const short& v, short& v1, uint8_t& v2)
{
  v2 = abs(v) % 60;
  v1 = v / 60;
}

void getcatdf(const short& v, float& v1)
{
  v1 = (double)v / 60.;
}

void getcathms(const unsigned short& v, uint8_t& v1, uint8_t& v2, uint8_t& v3)
{
  unsigned long vd = (long)v * 2;
  v3 = vd % 60;
  v2 = (vd / 60) % 60;
  v1 = vd / 3600;
}

void getcathf(const unsigned short& v, float& v1)
{
  v1 = (double)v / 1800.;
}