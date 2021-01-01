/*
 Copyright (c) 2005-2020 sdragonx (mail:sdragonx@foxmail.com)

 colors.hpp

 2020-02-17 17:26:13

*/
#ifndef COLORS_HPP_20200217172613
#define COLORS_HPP_20200217172613

#include <cgl/public.h>

#if defined(CGL_PALTFORM_ANDROID)
    //#define UNITEXT(str) u##str
    #define UNITEXT(str) L""
#else
    #define UNITEXT(str) L##str
#endif

namespace cgl{
namespace graphics{
namespace color{

//RGBA color
//HTML color

const vec4ub none;

const vec4ub aliceBlue          (0xF0, 0xF8, 0xFF, 0xFF); //#F0F8FF
const vec4ub antiqueWhite       (0xFA, 0xEB, 0xD7, 0xFF); //#FAEBD7
//const vec4ub aqua               (0x00, 0xFF, 0xFF, 0xFF); //#00FFFF
const vec4ub aquaMarine         (0x7F, 0xFF, 0xD4, 0xFF); //#7FFFD4
const vec4ub azure              (0xF0, 0xFF, 0xFF, 0xFF); //#F0FFFF
const vec4ub aeige              (0xF5, 0xF5, 0xDC, 0xFF); //#F5F5DC
const vec4ub bisque             (0xFF, 0xE4, 0xC4, 0xFF); //#FFE4C4
const vec4ub black              (0x00, 0x00, 0x00, 0xFF); //#000000
const vec4ub blanchedAlmond     (0xFF, 0xEB, 0xCD, 0xFF); //#FFEBCD
const vec4ub blue               (0x00, 0x00, 0xFF, 0xFF); //#0000FF
const vec4ub blueViolet         (0x8A, 0x2B, 0xE2, 0xFF); //#8A2BE2
const vec4ub brown              (0xA5, 0x2A, 0x2A, 0xFF); //#A52A2A
const vec4ub burlyWood          (0xDE, 0xB8, 0x87, 0xFF); //#DEB887
const vec4ub cadetBlue          (0x5F, 0x9E, 0xA0, 0xFF); //#5F9EA0
const vec4ub chartReuse         (0x7F, 0xFF, 0x00, 0xFF); //#7FFF00
const vec4ub chocolate          (0xD2, 0x69, 0x1E, 0xFF); //#D2691E
const vec4ub coral              (0xFF, 0x7F, 0x50, 0xFF); //#FF7F50
const vec4ub cornflowerBlue     (0x64, 0x95, 0xED, 0xFF); //#6495ED
const vec4ub cornsilk           (0xFF, 0xF8, 0xDC, 0xFF); //#FFF8DC
const vec4ub crimson            (0xDC, 0x14, 0x3C, 0xFF); //#DC143C
const vec4ub cyan               (0x00, 0xFF, 0xFF, 0xFF); //#00FFFF
const vec4ub darkBlue           (0x00, 0x00, 0x8B, 0xFF); //#00008B
const vec4ub darkCyan           (0x00, 0x8B, 0x8B, 0xFF); //#008B8B
const vec4ub darkGoldenRod      (0xB8, 0x86, 0x0B, 0xFF); //#B8860B
const vec4ub darkGray           (0xA9, 0xA9, 0xA9, 0xFF); //#A9A9A9
const vec4ub darkGreen          (0x00, 0x64, 0x00, 0xFF); //#006400
const vec4ub darkKhaki          (0xBD, 0xB7, 0x6B, 0xFF); //#BDB76B
const vec4ub darkMagenta        (0x8B, 0x00, 0x8B, 0xFF); //#8B008B
const vec4ub darkOliveGreen     (0x55, 0x6B, 0x2F, 0xFF); //#556B2F
const vec4ub darkOrange         (0xFF, 0x8C, 0x00, 0xFF); //#FF8C00
const vec4ub darkOrchid         (0x99, 0x32, 0xCC, 0xFF); //#9932CC
const vec4ub darkRed            (0x8B, 0x00, 0x00, 0xFF); //#8B0000
const vec4ub darkSalmon         (0xE9, 0x96, 0x7A, 0xFF); //#E9967A
const vec4ub darkSeaGreen       (0x8F, 0xBC, 0x8F, 0xFF); //#8FBC8F
const vec4ub darkSlateBlue      (0x48, 0x3D, 0x8B, 0xFF); //#483D8B
const vec4ub darkSlateGray      (0x2F, 0x4F, 0x4F, 0xFF); //#2F4F4F
const vec4ub darkTurquoise      (0x00, 0xCE, 0xD1, 0xFF); //#00CED1
const vec4ub darkViolet         (0x94, 0x00, 0xD3, 0xFF); //#9400D3
const vec4ub deepPink           (0xFF, 0x14, 0x93, 0xFF); //#FF1493
const vec4ub deepSkyBlue        (0x00, 0xBF, 0xFF, 0xFF); //#00BFFF
const vec4ub dimGray            (0x69, 0x69, 0x69, 0xFF); //#696969
const vec4ub dodgerBlue         (0x1E, 0x90, 0xFF, 0xFF); //#1E90FF
const vec4ub feldspar           (0xD1, 0x92, 0x75, 0xFF); //#D19275
const vec4ub fireBrick          (0xB2, 0x22, 0x22, 0xFF); //#B22222
const vec4ub floralWhite        (0xFF, 0xFA, 0xF0, 0xFF); //#FFFAF0
const vec4ub forestGreen        (0x22, 0x8B, 0x22, 0xFF); //#228B22
//const vec4ub fuchsia            (0xFF, 0x00, 0xFF, 0xFF); //#FF00FF
const vec4ub gainsBoro          (0xDC, 0xDC, 0xDC, 0xFF); //#DCDCDC
const vec4ub ghostWhite         (0xF8, 0xF8, 0xFF, 0xFF); //#F8F8FF
const vec4ub gold               (0xFF, 0xD7, 0x00, 0xFF); //#FFD700
const vec4ub goldenRod          (0xDA, 0xA5, 0x20, 0xFF); //#DAA520
const vec4ub gray               (0x80, 0x80, 0x80, 0xFF); //#808080
const vec4ub green              (0x00, 0x80, 0x00, 0xFF); //#008000
const vec4ub greenYellow        (0xAD, 0xFF, 0x2F, 0xFF); //#ADFF2F
const vec4ub honeyDew           (0xF0, 0xFF, 0xF0, 0xFF); //#F0FFF0
const vec4ub hotPink            (0xFF, 0x69, 0xB4, 0xFF); //#FF69B4
const vec4ub indianRed          (0xCD, 0x5C, 0x5C, 0xFF); //#CD5C5C
const vec4ub indigo             (0x4B, 0x00, 0x82, 0xFF); //#4B0082
const vec4ub ivory              (0xFF, 0xFF, 0xF0, 0xFF); //#FFFFF0
const vec4ub khaki              (0xF0, 0xE6, 0x8C, 0xFF); //#F0E68C
const vec4ub lavender           (0xE6, 0xE6, 0xFA, 0xFF); //#E6E6FA
const vec4ub lavenderBlush      (0xFF, 0xF0, 0xF5, 0xFF); //#FFF0F5
const vec4ub lawnGreen          (0x7C, 0xFC, 0x00, 0xFF); //#7CFC00
const vec4ub lemonChiffon       (0xFF, 0xFA, 0xCD, 0xFF); //#FFFACD
const vec4ub lightBlue          (0xAD, 0xD8, 0xE6, 0xFF); //#ADD8E6
const vec4ub lightCoral         (0xF0, 0x80, 0x80, 0xFF); //#F08080
const vec4ub lightCyan          (0xE0, 0xFF, 0xFF, 0xFF); //#E0FFFF
const vec4ub lightGoldenRod     (0xFA, 0xFA, 0xD2, 0xFF); //#FAFAD2
const vec4ub lightGrey          (0xD3, 0xD3, 0xD3, 0xFF); //#D3D3D3
const vec4ub lightGreen         (0x90, 0xEE, 0x90, 0xFF); //#90EE90
const vec4ub lightPink          (0xFF, 0xB6, 0xC1, 0xFF); //#FFB6C1
const vec4ub lightSalmon        (0xFF, 0xA0, 0x7A, 0xFF); //#FFA07A
const vec4ub lightSeaGreen      (0x20, 0xB2, 0xAA, 0xFF); //#20B2AA
const vec4ub lightSkyBlue       (0x87, 0xCE, 0xFA, 0xFF); //#87CEFA
const vec4ub lightSlateBlue     (0x84, 0x70, 0xFF, 0xFF); //#8470FF
const vec4ub lightSlateGray     (0x77, 0x88, 0x99, 0xFF); //#778899
const vec4ub lightSteelBlue     (0xB0, 0xC4, 0xDE, 0xFF); //#B0C4DE
const vec4ub lightYellow        (0xFF, 0xFF, 0xE0, 0xFF); //#FFFFE0
const vec4ub lime               (0x00, 0xFF, 0x00, 0xFF); //#00FF00
const vec4ub limeGreen          (0x32, 0xCD, 0x32, 0xFF); //#32CD32
const vec4ub linen              (0xFA, 0xF0, 0xE6, 0xFF); //#FAF0E6
const vec4ub magenta            (0xFF, 0x00, 0xFF, 0xFF); //#FF00FF
const vec4ub maroon             (0x80, 0x00, 0x00, 0xFF); //#800000
const vec4ub mediumAquaMarine   (0x66, 0xCD, 0xAA, 0xFF); //#66CDAA
const vec4ub mediumBlue         (0x00, 0x00, 0xCD, 0xFF); //#0000CD
const vec4ub mediumOrchid       (0xBA, 0x55, 0xD3, 0xFF); //#BA55D3
const vec4ub mediumPurple       (0x93, 0x70, 0xD8, 0xFF); //#9370D8
const vec4ub mediumSeaGreen     (0x3C, 0xB3, 0x71, 0xFF); //#3CB371
const vec4ub mediumSlateBlue    (0x7B, 0x68, 0xEE, 0xFF); //#7B68EE
const vec4ub mediumSpringGreen  (0x00, 0xFA, 0x9A, 0xFF); //#00FA9A
const vec4ub mediumTurquoise    (0x48, 0xD1, 0xCC, 0xFF); //#48D1CC
const vec4ub mediumVioletRed    (0xC7, 0x15, 0x85, 0xFF); //#C71585
const vec4ub midnightBlue       (0x19, 0x19, 0x70, 0xFF); //#191970
const vec4ub mintCream          (0xF5, 0xFF, 0xFA, 0xFF); //#F5FFFA
const vec4ub mistyRose          (0xFF, 0xE4, 0xE1, 0xFF); //#FFE4E1
const vec4ub moccasin           (0xFF, 0xE4, 0xB5, 0xFF); //#FFE4B5
const vec4ub navajoWhite        (0xFF, 0xDE, 0xAD, 0xFF); //#FFDEAD
const vec4ub navy               (0x00, 0x00, 0x80, 0xFF); //#000080
const vec4ub oldLace            (0xFD, 0xF5, 0xE6, 0xFF); //#FDF5E6
const vec4ub olive              (0x80, 0x80, 0x00, 0xFF); //#808000
const vec4ub oliveDrab          (0x6B, 0x8E, 0x23, 0xFF); //#6B8E23
const vec4ub orange             (0xFF, 0xA5, 0x00, 0xFF); //#FFA500
const vec4ub orangeRed          (0xFF, 0x45, 0x00, 0xFF); //#FF4500
const vec4ub orchid             (0xDA, 0x70, 0xD6, 0xFF); //#DA70D6
const vec4ub paleGoldenRod      (0xEE, 0xE8, 0xAA, 0xFF); //#EEE8AA
const vec4ub paleGreen          (0x98, 0xFB, 0x98, 0xFF); //#98FB98
const vec4ub paleTurquoise      (0xAF, 0xEE, 0xEE, 0xFF); //#AFEEEE
const vec4ub paleVioletRed      (0xD8, 0x70, 0x93, 0xFF); //#D87093
const vec4ub papayaWhip         (0xFF, 0xEF, 0xD5, 0xFF); //#FFEFD5
const vec4ub peachPuff          (0xFF, 0xDA, 0xB9, 0xFF); //#FFDAB9
const vec4ub peru               (0xCD, 0x85, 0x3F, 0xFF); //#CD853F
const vec4ub pink               (0xFF, 0xC0, 0xCB, 0xFF); //#FFC0CB
const vec4ub plum               (0xDD, 0xA0, 0xDD, 0xFF); //#DDA0DD
const vec4ub powderBlue         (0xB0, 0xE0, 0xE6, 0xFF); //#B0E0E6
const vec4ub purple             (0x80, 0x00, 0x80, 0xFF); //#800080
const vec4ub red                (0xFF, 0x00, 0x00, 0xFF); //#FF0000
const vec4ub rosyBrown          (0xBC, 0x8F, 0x8F, 0xFF); //#BC8F8F
const vec4ub royalBlue          (0x41, 0x69, 0xE1, 0xFF); //#4169E1
const vec4ub saddleBrown        (0x8B, 0x45, 0x13, 0xFF); //#8B4513
const vec4ub salmon             (0xFA, 0x80, 0x72, 0xFF); //#FA8072
const vec4ub sandyBrown         (0xF4, 0xA4, 0x60, 0xFF); //#F4A460
const vec4ub seaGreen           (0x2E, 0x8B, 0x57, 0xFF); //#2E8B57
const vec4ub seaShell           (0xFF, 0xF5, 0xEE, 0xFF); //#FFF5EE
const vec4ub sienna             (0xA0, 0x52, 0x2D, 0xFF); //#A0522D
const vec4ub silver             (0xC0, 0xC0, 0xC0, 0xFF); //#C0C0C0
const vec4ub skyBlue            (0x87, 0xCE, 0xEB, 0xFF); //#87CEEB
const vec4ub slateBlue          (0x6A, 0x5A, 0xCD, 0xFF); //#6A5ACD
const vec4ub slateGray          (0x70, 0x80, 0x90, 0xFF); //#708090
const vec4ub snow               (0xFF, 0xFA, 0xFA, 0xFF); //#FFFAFA
const vec4ub springGreen        (0x00, 0xFF, 0x7F, 0xFF); //#00FF7F
const vec4ub steelBlue          (0x46, 0x82, 0xB4, 0xFF); //#4682B4
const vec4ub tan                (0xD2, 0xB4, 0x8C, 0xFF); //#D2B48C
const vec4ub teal               (0x00, 0x80, 0x80, 0xFF); //#008080
const vec4ub thistle            (0xD8, 0xBF, 0xD8, 0xFF); //#D8BFD8
const vec4ub tomato             (0xFF, 0x63, 0x47, 0xFF); //#FF6347
const vec4ub turquoise          (0x40, 0xE0, 0xD0, 0xFF); //#40E0D0
const vec4ub violet             (0xEE, 0x82, 0xEE, 0xFF); //#EE82EE
const vec4ub violetRed          (0xD0, 0x20, 0x90, 0xFF); //#D02090
const vec4ub wheat              (0xF5, 0xDE, 0xB3, 0xFF); //#F5DEB3
const vec4ub white              (0xFF, 0xFF, 0xFF, 0xFF); //#FFFFFF
const vec4ub whiteSmoke         (0xF5, 0xF5, 0xF5, 0xFF); //#F5F5F5
const vec4ub yellow             (0xFF, 0xFF, 0x00, 0xFF); //#FFFF00
const vec4ub yellowGreen        (0x9A, 0xCD, 0x32, 0xFF); //#9ACD32

#ifndef CGL_PLATFORM_CB6

struct color_item
{
    vec4ub color;
    const char* name;
    const wchar_t* title;
};


const color_item colors[] = {
    {aliceBlue,         "AliceBlue",            UNITEXT("爱丽丝蓝")},
    {antiqueWhite,      "AntiqueWhite",         UNITEXT("古董白")},
    //{aqua,              "Aqua",                 UNITEXT("水绿色")},
    {aquaMarine,        "AquaMarine",           UNITEXT("碧绿")},
    {azure,             "Azure",                UNITEXT("蔚蓝色")},//青白色
    {aeige,             "Beige",                UNITEXT("米色")},
    {bisque,            "Bisque",               UNITEXT("陶坯黄")},
    {black,             "Black",                UNITEXT("黑色")},
    {blanchedAlmond,    "BlanchedAlmond",       UNITEXT("杏仁白")},
    {blue,              "Blue",                 UNITEXT("蓝色")},
    {blueViolet,        "BlueViolet",           UNITEXT("蓝紫罗兰")},//蓝紫色
    {brown,             "Brown",                UNITEXT("棕色")},//褐色
    {burlyWood,         "BurlyWood",            UNITEXT("硬木褐")},
    {cadetBlue,         "CadetBlue",            UNITEXT("军服蓝")},
    {chartReuse,        "ChartReuse",           UNITEXT("查特酒绿")},//一种药酒，诞生于17世纪的法国
    {chocolate,         "Chocolate",            UNITEXT("巧克力")},
    {coral,             "Coral",                UNITEXT("珊瑚红")},
    {cornflowerBlue,    "CornflowerBlue",       UNITEXT("矢车菊蓝")},//矢车菊是德国和马其顿的国花
    {cornsilk,          "Cornsilk",             UNITEXT("玉米黄")},
    {crimson,           "Crimson",              UNITEXT("绯红色")},//较之猩红(Scarlet)蓝色分量要多些
    {cyan,              "Cyan",                 UNITEXT("青色")},
    {darkBlue,          "DarkBlue",             UNITEXT("深蓝色")},
    {darkCyan,          "DarkCyan",             UNITEXT("深青色")},
    {darkGoldenRod,     "DarkGoldenRod",        UNITEXT("深金菊黄")},//秋麒麟
    {darkGray,          "DarkGray",             UNITEXT("深灰色")},
    {darkGreen,         "DarkGreen",            UNITEXT("深绿色")},
    {darkKhaki,         "DarkKhaki",            UNITEXT("深卡其色")},
    {darkMagenta,       "DarkMagenta",          UNITEXT("深品红色")},
    {darkOliveGreen,    "DarkOliveGreen",       UNITEXT("深橄榄绿")},
    {darkOrange,        "DarkOrange",           UNITEXT("深橙色")},
    {darkOrchid,        "DarkOrchid",           UNITEXT("深洋兰紫")},//兰花紫
    {darkRed,           "DarkRed",              UNITEXT("深红色")},
    {darkSalmon,        "DarkSalmon",           UNITEXT("深鲑红")},
    {darkSeaGreen,      "DarkSeaGreen",         UNITEXT("深海绿")},//深海藻绿
    {darkSlateBlue,     "DarkSlateBlue",        UNITEXT("深岩蓝")},//深岩蓝
    {darkSlateGray,     "DarkSlateGray",        UNITEXT("深岩灰色")},
    {darkTurquoise,     "DarkTurquoise",        UNITEXT("深松石绿")},
    {darkViolet,        "DarkViolet",           UNITEXT("深紫罗兰")},//深紫
    {deepPink,          "DeepPink",             UNITEXT("深粉色")},
    {deepSkyBlue,       "DeepSkyBlue",          UNITEXT("深天蓝")},
    {dimGray,           "DimGray",              UNITEXT("深灰色")},//昏灰 是命名颜色中最深的一种灰色
    {dodgerBlue,        "DodgerBlue",           UNITEXT("道奇蓝")},//湖蓝 洛杉矶道奇棒球队队标的颜色
    {feldspar,          "Feldspar",             UNITEXT("长石色")},
    {fireBrick,         "FireBrick",            UNITEXT("火砖红")},//耐火砖的颜色比普通红砖要深
    {floralWhite,       "FloralWhite",          UNITEXT("花卉白")},
    {forestGreen,       "ForestGreen",          UNITEXT("森林绿")},
    //{fuchsia,           "Fuchsia",              UNITEXT("紫红色")},
    {gainsBoro,         "GainsBoro",            UNITEXT("庚氏灰")},//托马斯·庚斯博罗，18世纪英国画家
    {ghostWhite,        "GhostWhite",           UNITEXT("幽灵白")},
    {gold,              "Gold",                 UNITEXT("金色")},
    {goldenRod,         "GoldenRod",            UNITEXT("金菊黄")},
    {gray,              "Gray",                 UNITEXT("灰色")},
    {green,             "Green",                UNITEXT("绿色")},//视觉上的正绿色
    {greenYellow,       "GreenYellow",          UNITEXT("黄绿色")},//请注意与YellowGreen区分
    {honeyDew,          "HoneyDew",             UNITEXT("蜜瓜绿")},//Honeydew即白兰瓜、蜜瓜
    {hotPink,           "HotPink",              UNITEXT("艳粉色")},//不是暖粉，后者应是偏黄的粉色
    {indianRed,         "IndianRed",            UNITEXT("印度红")},
    {indigo,            "Indigo",               UNITEXT("靛蓝")},
    {ivory,             "Ivory",                UNITEXT("象牙白")},
    {khaki,             "Khaki",                UNITEXT("卡其")},
    {lavender,          "Lavender",             UNITEXT("薰衣草紫")},
    {lavenderBlush,     "LavenderBlush",        UNITEXT("薰衣草红")},
    {lawnGreen,         "LawnGreen",            UNITEXT("草坪绿")},
    {lemonChiffon,      "LemonChiffon",         UNITEXT("柠檬绸黄")},
    {lightBlue,         "LightBlue",            UNITEXT("浅蓝色")},
    {lightCoral,        "LightCoral",           UNITEXT("浅珊瑚")},
    {lightCyan,         "LightCyan",            UNITEXT("浅青色")},
    {lightGoldenRod,    "LightGoldenRodYellow", UNITEXT("浅金菊黄")},
    {lightGrey,         "LightGrey",            UNITEXT("亮灰色")},
    {lightGreen,        "LightGreen",           UNITEXT("浅绿色")},
    {lightPink,         "LightPink",            UNITEXT("浅粉色")},
    {lightSalmon,       "LightSalmon",          UNITEXT("浅鲑红")},
    {lightSeaGreen,     "LightSeaGreen",        UNITEXT("浅海绿")},
    {lightSkyBlue,      "LightSkyBlue",         UNITEXT("浅天蓝")},
    {lightSlateBlue,    "LightSlateBlue",       UNITEXT("浅岩蓝")},
    {lightSlateGray,    "LightSlateGray",       UNITEXT("浅岩灰")},
    {lightSteelBlue,    "LightSteelBlue",       UNITEXT("浅钢青")},
    {lightYellow,       "LightYellow",          UNITEXT("浅黄色")},
    {lime,              "Lime",                 UNITEXT("青柠")},//RGB颜色空间的正绿色
    {limeGreen,         "LimeGreen",            UNITEXT("青柠绿")},
    {linen,             "Linen",                UNITEXT("亚麻")},
    {magenta,           "Magenta",              UNITEXT("品红")},
    {maroon,            "Maroon",               UNITEXT("栗色")},
    {mediumAquaMarine,  "MediumAquaMarine",     UNITEXT("中碧绿")},
    {mediumBlue,        "MediumBlue",           UNITEXT("中蓝色")},
    {mediumOrchid,      "MediumOrchid",         UNITEXT("中洋兰紫")},
    {mediumPurple,      "MediumPurple",         UNITEXT("中紫色")},
    {mediumSeaGreen,    "MediumSeaGreen",       UNITEXT("中海绿")},//中海藻绿
    {mediumSlateBlue,   "MediumSlateBlue",      UNITEXT("中岩蓝")},
    {mediumSpringGreen, "MediumSpringGreen",    UNITEXT("中嫩绿")},
    {mediumTurquoise,   "MediumTurquoise",      UNITEXT("中松石绿")},
    {mediumVioletRed,   "MediumVioletRed",      UNITEXT("中紫罗兰红")},//中紫红
    {midnightBlue,      "MidnightBlue",         UNITEXT("午夜蓝")},
    {mintCream,         "MintCream",            UNITEXT("薄荷色")},//薄荷乳白
    {mistyRose,         "MistyRose",            UNITEXT("迷雾玫瑰")},//雾玫瑰红
    {moccasin,          "Moccasin",             UNITEXT("莫卡辛")},//鹿皮色 Moccasin本意为鹿皮鞋
    {navajoWhite,       "NavajoWhite",          UNITEXT("纳瓦霍白")},//纳瓦霍(Navajo)是北美印第安人的一支
    {navy,              "Navy",                 UNITEXT("海军蓝")},//藏青 navy blue 海军蓝 深蓝色 藏青色
    {oldLace,           "OldLace",              UNITEXT("旧蕾丝白")},
    {olive,             "Olive",                UNITEXT("橄榄色")},
    {oliveDrab,         "OliveDrab",            UNITEXT("橄榄绿")},
    {orange,            "Orange",               UNITEXT("橙色")},
    {orangeRed,         "OrangeRed",            UNITEXT("橘红")},
    {orchid,            "Orchid",               UNITEXT("洋兰紫")},
    {paleGoldenRod,     "PaleGoldenRod",        UNITEXT("白金菊黄")},
    {paleGreen,         "PaleGreen",            UNITEXT("白绿色")},//苍绿色
    {paleTurquoise,     "PaleTurquoise",        UNITEXT("白松石绿")},
    {paleVioletRed,     "PaleVioletRed",        UNITEXT("白紫罗兰")},//白紫红
    {papayaWhip,        "PapayaWhip",           UNITEXT("番木瓜橙")},//本意为番木瓜(一种热带水果)汁
    {peachPuff,         "PeachPuff",            UNITEXT("粉扑桃色")},//Peach是桃，Puff是化妆用的粉扑
    {peru,              "Peru",                 UNITEXT("秘鲁红")},
    {pink,              "Pink",                 UNITEXT("粉红色")},//粉色
    {plum,              "Plum",                 UNITEXT("李子紫")},//Plum意为李子、梅子
    {powderBlue,        "PowderBlue",           UNITEXT("粉末蓝")},
    {purple,            "Purple",               UNITEXT("紫色")},
    {red,               "Red",                  UNITEXT("红色")},
    {rosyBrown,         "RosyBrown",            UNITEXT("玫瑰棕")},//玫瑰褐
    {royalBlue,         "RoyalBlue",            UNITEXT("皇家蓝")},//品蓝
    {saddleBrown,       "SaddleBrown",          UNITEXT("马鞍棕")},//亦作“马鞍棕”，马鞍常见的颜色
    {salmon,            "Salmon",               UNITEXT("鲑红色")},
    {sandyBrown,        "SandyBrown",           UNITEXT("沙棕色")},
    {seaGreen,          "SeaGreen",             UNITEXT("海绿色")},//海藻绿
    {seaShell,          "SeaShell",             UNITEXT("贝壳白")},
    {sienna,            "Sienna",               UNITEXT("土黄赭")},
    {silver,            "Silver",               UNITEXT("银色")},
    {skyBlue,           "SkyBlue",              UNITEXT("天蓝色")},
    {slateBlue,         "SlateBlue",            UNITEXT("岩蓝")},//Slate本意为“板岩”
    {slateGray,         "SlateGray",            UNITEXT("岩灰")},
    {snow,              "Snow",                 UNITEXT("雪白")},
    {springGreen,       "SpringGreen",          UNITEXT("春绿")},
    {steelBlue,         "SteelBlue",            UNITEXT("钢蓝")},
    {tan,               "Tan",                  UNITEXT("棕褐色")},//日晒褐
    {teal,              "Teal",                 UNITEXT("水鸭绿")},//Teal意为绿翅鸭，一种水鸭
    {thistle,           "Thistle",              UNITEXT("蓟紫")},//菊科蓟属，多刺花卉，旧时苏格兰国花
    {tomato,            "Tomato",               UNITEXT("番茄红")},
    {turquoise,         "Turquoise",            UNITEXT("松石绿")},//绿宝石蓝，本意为土耳其石即绿松石，一种宝石
    {violet,            "Violet",               UNITEXT("紫罗兰")},
    {violetRed,         "VioletRed",            UNITEXT("紫罗兰红")},
    {wheat,             "Wheat",                UNITEXT("小麦色")},
    {white,             "White",                UNITEXT("白色")},
    {whiteSmoke,        "WhiteSmoke",           UNITEXT("白烟")},
    {yellow,            "Yellow",               UNITEXT("黄色")},
    {yellowGreen,       "YellowGreen",          UNITEXT("暗黄绿色")},
};

}//end namespace color


static const vec4ub basic_colors[] = {
    color::black,
    color::gray,
    color::red,
    color::yellow,
    color::lime,
    color::cyan,
    color::blue,
    color::magenta,
    color::maroon,
    color::olive,
    color::green,
    color::teal,
    color::navy,
    color::purple,
    color::silver,
    color::white
};

#endif

const char* color_to_string(vec4ub color)
{
//    for(int i=0; i<sizeof(colorlist) / sizeof(void*); ++i){
//        if(colorlist[i].color == color){
//            return colorlist[i].name;
//        }
//    }
    return null;
}

vec4ub string_to_color(const char* name)
{
//    for(int i=0; i<sizeof(colorlist) / sizeof(void*); ++i){
//        if(!csicmp(colorlist[i].name, name, cslen(colorlist[i].name) + 1)){
//            return colorlist[i].color;
//        }
//    }
    return vec4ub();
}


}//end namespace graphics
}//end namespace cgl

#endif //COLORS_HPP_20200217172613
