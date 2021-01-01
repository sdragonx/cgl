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
    {aliceBlue,         "AliceBlue",            UNITEXT("����˿��")},
    {antiqueWhite,      "AntiqueWhite",         UNITEXT("�Ŷ���")},
    //{aqua,              "Aqua",                 UNITEXT("ˮ��ɫ")},
    {aquaMarine,        "AquaMarine",           UNITEXT("����")},
    {azure,             "Azure",                UNITEXT("ε��ɫ")},//���ɫ
    {aeige,             "Beige",                UNITEXT("��ɫ")},
    {bisque,            "Bisque",               UNITEXT("������")},
    {black,             "Black",                UNITEXT("��ɫ")},
    {blanchedAlmond,    "BlanchedAlmond",       UNITEXT("���ʰ�")},
    {blue,              "Blue",                 UNITEXT("��ɫ")},
    {blueViolet,        "BlueViolet",           UNITEXT("��������")},//����ɫ
    {brown,             "Brown",                UNITEXT("��ɫ")},//��ɫ
    {burlyWood,         "BurlyWood",            UNITEXT("Ӳľ��")},
    {cadetBlue,         "CadetBlue",            UNITEXT("������")},
    {chartReuse,        "ChartReuse",           UNITEXT("���ؾ���")},//һ��ҩ�ƣ�������17���͵ķ���
    {chocolate,         "Chocolate",            UNITEXT("�ɿ���")},
    {coral,             "Coral",                UNITEXT("ɺ����")},
    {cornflowerBlue,    "CornflowerBlue",       UNITEXT("ʸ������")},//ʸ�����ǵ¹�������ٵĹ���
    {cornsilk,          "Cornsilk",             UNITEXT("���׻�")},
    {crimson,           "Crimson",              UNITEXT("糺�ɫ")},//��֮�ɺ�(Scarlet)��ɫ����Ҫ��Щ
    {cyan,              "Cyan",                 UNITEXT("��ɫ")},
    {darkBlue,          "DarkBlue",             UNITEXT("����ɫ")},
    {darkCyan,          "DarkCyan",             UNITEXT("����ɫ")},
    {darkGoldenRod,     "DarkGoldenRod",        UNITEXT("���ջ�")},//������
    {darkGray,          "DarkGray",             UNITEXT("���ɫ")},
    {darkGreen,         "DarkGreen",            UNITEXT("����ɫ")},
    {darkKhaki,         "DarkKhaki",            UNITEXT("���ɫ")},
    {darkMagenta,       "DarkMagenta",          UNITEXT("��Ʒ��ɫ")},
    {darkOliveGreen,    "DarkOliveGreen",       UNITEXT("�������")},
    {darkOrange,        "DarkOrange",           UNITEXT("���ɫ")},
    {darkOrchid,        "DarkOrchid",           UNITEXT("��������")},//������
    {darkRed,           "DarkRed",              UNITEXT("���ɫ")},
    {darkSalmon,        "DarkSalmon",           UNITEXT("���ٺ�")},
    {darkSeaGreen,      "DarkSeaGreen",         UNITEXT("���")},//�����
    {darkSlateBlue,     "DarkSlateBlue",        UNITEXT("������")},//������
    {darkSlateGray,     "DarkSlateGray",        UNITEXT("���һ�ɫ")},
    {darkTurquoise,     "DarkTurquoise",        UNITEXT("����ʯ��")},
    {darkViolet,        "DarkViolet",           UNITEXT("��������")},//����
    {deepPink,          "DeepPink",             UNITEXT("���ɫ")},
    {deepSkyBlue,       "DeepSkyBlue",          UNITEXT("������")},
    {dimGray,           "DimGray",              UNITEXT("���ɫ")},//��� ��������ɫ�������һ�ֻ�ɫ
    {dodgerBlue,        "DodgerBlue",           UNITEXT("������")},//���� ��ɼ�������Ӷӱ����ɫ
    {feldspar,          "Feldspar",             UNITEXT("��ʯɫ")},
    {fireBrick,         "FireBrick",            UNITEXT("��ש��")},//�ͻ�ש����ɫ����ͨ��שҪ��
    {floralWhite,       "FloralWhite",          UNITEXT("���ܰ�")},
    {forestGreen,       "ForestGreen",          UNITEXT("ɭ����")},
    //{fuchsia,           "Fuchsia",              UNITEXT("�Ϻ�ɫ")},
    {gainsBoro,         "GainsBoro",            UNITEXT("���ϻ�")},//����˹����˹���ޣ�18����Ӣ������
    {ghostWhite,        "GhostWhite",           UNITEXT("�����")},
    {gold,              "Gold",                 UNITEXT("��ɫ")},
    {goldenRod,         "GoldenRod",            UNITEXT("��ջ�")},
    {gray,              "Gray",                 UNITEXT("��ɫ")},
    {green,             "Green",                UNITEXT("��ɫ")},//�Ӿ��ϵ�����ɫ
    {greenYellow,       "GreenYellow",          UNITEXT("����ɫ")},//��ע����YellowGreen����
    {honeyDew,          "HoneyDew",             UNITEXT("�۹���")},//Honeydew�������ϡ��۹�
    {hotPink,           "HotPink",              UNITEXT("�޷�ɫ")},//����ů�ۣ�����Ӧ��ƫ�Ƶķ�ɫ
    {indianRed,         "IndianRed",            UNITEXT("ӡ�Ⱥ�")},
    {indigo,            "Indigo",               UNITEXT("����")},
    {ivory,             "Ivory",                UNITEXT("������")},
    {khaki,             "Khaki",                UNITEXT("����")},
    {lavender,          "Lavender",             UNITEXT("޹�²���")},
    {lavenderBlush,     "LavenderBlush",        UNITEXT("޹�²ݺ�")},
    {lawnGreen,         "LawnGreen",            UNITEXT("��ƺ��")},
    {lemonChiffon,      "LemonChiffon",         UNITEXT("���ʳ��")},
    {lightBlue,         "LightBlue",            UNITEXT("ǳ��ɫ")},
    {lightCoral,        "LightCoral",           UNITEXT("ǳɺ��")},
    {lightCyan,         "LightCyan",            UNITEXT("ǳ��ɫ")},
    {lightGoldenRod,    "LightGoldenRodYellow", UNITEXT("ǳ��ջ�")},
    {lightGrey,         "LightGrey",            UNITEXT("����ɫ")},
    {lightGreen,        "LightGreen",           UNITEXT("ǳ��ɫ")},
    {lightPink,         "LightPink",            UNITEXT("ǳ��ɫ")},
    {lightSalmon,       "LightSalmon",          UNITEXT("ǳ�ٺ�")},
    {lightSeaGreen,     "LightSeaGreen",        UNITEXT("ǳ����")},
    {lightSkyBlue,      "LightSkyBlue",         UNITEXT("ǳ����")},
    {lightSlateBlue,    "LightSlateBlue",       UNITEXT("ǳ����")},
    {lightSlateGray,    "LightSlateGray",       UNITEXT("ǳ�һ�")},
    {lightSteelBlue,    "LightSteelBlue",       UNITEXT("ǳ����")},
    {lightYellow,       "LightYellow",          UNITEXT("ǳ��ɫ")},
    {lime,              "Lime",                 UNITEXT("����")},//RGB��ɫ�ռ������ɫ
    {limeGreen,         "LimeGreen",            UNITEXT("������")},
    {linen,             "Linen",                UNITEXT("����")},
    {magenta,           "Magenta",              UNITEXT("Ʒ��")},
    {maroon,            "Maroon",               UNITEXT("��ɫ")},
    {mediumAquaMarine,  "MediumAquaMarine",     UNITEXT("�б���")},
    {mediumBlue,        "MediumBlue",           UNITEXT("����ɫ")},
    {mediumOrchid,      "MediumOrchid",         UNITEXT("��������")},
    {mediumPurple,      "MediumPurple",         UNITEXT("����ɫ")},
    {mediumSeaGreen,    "MediumSeaGreen",       UNITEXT("�к���")},//�к�����
    {mediumSlateBlue,   "MediumSlateBlue",      UNITEXT("������")},
    {mediumSpringGreen, "MediumSpringGreen",    UNITEXT("������")},
    {mediumTurquoise,   "MediumTurquoise",      UNITEXT("����ʯ��")},
    {mediumVioletRed,   "MediumVioletRed",      UNITEXT("����������")},//���Ϻ�
    {midnightBlue,      "MidnightBlue",         UNITEXT("��ҹ��")},
    {mintCream,         "MintCream",            UNITEXT("����ɫ")},//�������
    {mistyRose,         "MistyRose",            UNITEXT("����õ��")},//��õ���
    {moccasin,          "Moccasin",             UNITEXT("Ī����")},//¹Ƥɫ Moccasin����Ϊ¹ƤЬ
    {navajoWhite,       "NavajoWhite",          UNITEXT("���߻���")},//���߻�(Navajo)�Ǳ���ӡ�ڰ��˵�һ֧
    {navy,              "Navy",                 UNITEXT("������")},//���� navy blue ������ ����ɫ ����ɫ
    {oldLace,           "OldLace",              UNITEXT("����˿��")},
    {olive,             "Olive",                UNITEXT("���ɫ")},
    {oliveDrab,         "OliveDrab",            UNITEXT("�����")},
    {orange,            "Orange",               UNITEXT("��ɫ")},
    {orangeRed,         "OrangeRed",            UNITEXT("�ٺ�")},
    {orchid,            "Orchid",               UNITEXT("������")},
    {paleGoldenRod,     "PaleGoldenRod",        UNITEXT("�׽�ջ�")},
    {paleGreen,         "PaleGreen",            UNITEXT("����ɫ")},//����ɫ
    {paleTurquoise,     "PaleTurquoise",        UNITEXT("����ʯ��")},
    {paleVioletRed,     "PaleVioletRed",        UNITEXT("��������")},//���Ϻ�
    {papayaWhip,        "PapayaWhip",           UNITEXT("��ľ�ϳ�")},//����Ϊ��ľ��(һ���ȴ�ˮ��)֭
    {peachPuff,         "PeachPuff",            UNITEXT("������ɫ")},//Peach���ң�Puff�ǻ�ױ�õķ���
    {peru,              "Peru",                 UNITEXT("��³��")},
    {pink,              "Pink",                 UNITEXT("�ۺ�ɫ")},//��ɫ
    {plum,              "Plum",                 UNITEXT("������")},//Plum��Ϊ���ӡ�÷��
    {powderBlue,        "PowderBlue",           UNITEXT("��ĩ��")},
    {purple,            "Purple",               UNITEXT("��ɫ")},
    {red,               "Red",                  UNITEXT("��ɫ")},
    {rosyBrown,         "RosyBrown",            UNITEXT("õ����")},//õ���
    {royalBlue,         "RoyalBlue",            UNITEXT("�ʼ���")},//Ʒ��
    {saddleBrown,       "SaddleBrown",          UNITEXT("����")},//���������ء�������������ɫ
    {salmon,            "Salmon",               UNITEXT("�ٺ�ɫ")},
    {sandyBrown,        "SandyBrown",           UNITEXT("ɳ��ɫ")},
    {seaGreen,          "SeaGreen",             UNITEXT("����ɫ")},//������
    {seaShell,          "SeaShell",             UNITEXT("���ǰ�")},
    {sienna,            "Sienna",               UNITEXT("������")},
    {silver,            "Silver",               UNITEXT("��ɫ")},
    {skyBlue,           "SkyBlue",              UNITEXT("����ɫ")},
    {slateBlue,         "SlateBlue",            UNITEXT("����")},//Slate����Ϊ�����ҡ�
    {slateGray,         "SlateGray",            UNITEXT("�һ�")},
    {snow,              "Snow",                 UNITEXT("ѩ��")},
    {springGreen,       "SpringGreen",          UNITEXT("����")},
    {steelBlue,         "SteelBlue",            UNITEXT("����")},
    {tan,               "Tan",                  UNITEXT("�غ�ɫ")},//��ɹ��
    {teal,              "Teal",                 UNITEXT("ˮѼ��")},//Teal��Ϊ�̳�Ѽ��һ��ˮѼ
    {thistle,           "Thistle",              UNITEXT("����")},//�տƼ�������̻��ܣ���ʱ�ո�������
    {tomato,            "Tomato",               UNITEXT("���Ѻ�")},
    {turquoise,         "Turquoise",            UNITEXT("��ʯ��")},//�̱�ʯ��������Ϊ������ʯ������ʯ��һ�ֱ�ʯ
    {violet,            "Violet",               UNITEXT("������")},
    {violetRed,         "VioletRed",            UNITEXT("��������")},
    {wheat,             "Wheat",                UNITEXT("С��ɫ")},
    {white,             "White",                UNITEXT("��ɫ")},
    {whiteSmoke,        "WhiteSmoke",           UNITEXT("����")},
    {yellow,            "Yellow",               UNITEXT("��ɫ")},
    {yellowGreen,       "YellowGreen",          UNITEXT("������ɫ")},
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
