/*
 Copyright (c) 2005-2020 sdragonx (mail:sdragonx@foxmail.com)

 svg_file.hpp

 2020-03-14 22:00:10

*/
#ifndef SVG_FILE_HPP_20200314220010
#define SVG_FILE_HPP_20200314220010

#include <stdint.h>
#include <windows.h>
#include <vector>
#include <cgl/graphics/color.hpp>
#include <cgl/vec.hpp>
#include <cgl/xml.hpp>

#include <cgl/graphics/tesselation.hpp>
using namespace cgl;

#include "svg_color.hpp"

#include <vcl.h>

namespace cgl{
namespace graphics{

bool is_equal(const char* a, const char* b)
{
    return strcmp(a, b) == 0;
}

enum SHAPE_TYPE{
    SHAPE_UNKNOWN,
    SHAPE_G,        //group
    SHAPE_POINT,
    SHAPE_LINE,
    SHAPE_RECT,
    SHAPE_CIRCLE,
    SHAPE_ELLIPSE,
    SHAPE_POLYLINE,
    SHAPE_POLYGON,
    SHAPE_PATH,
};

struct svg_pen
{
    bool stroke;    //是否描边
    bool fill;      //是否填充
    int width;      //pen width
    svg_color stroke_color;
    svg_color fill_color;
    std::vector<float> dash;

    svg_pen() : dash()
    {
        stroke = false;
        fill = false;
        width = 1;
        stroke_color = SVG_RGBA(0, 0, 0, 0);
        fill_color   = SVG_RGBA(0, 0, 0, 0);
    }
};

struct svg_shape
{
    int type;       //shape type
    svg_pen pen;

    std::string name;
    std::vector<vec2f> points;
    path_command path;

    svg_shape() : pen()
    {
    }
};

int svg_get_shape_type(const char* name);

class svg_file
{
public:
    int width;
    int height;
    vec4i view_box;
    svg_pen pen;
    std::vector<svg_shape> shapes;

    int open(const char* filename)
    {
        xml_document<char> doc;
        doc.open(filename);

        xml_node<char> svg = doc.subnode("svg");
        if(!svg){
            return -1;
        }

        const char* value = svg.attribute("width").value();
        width = 1024;
        height = 1024;

        //解析默认画笔
        parse_pen(pen, svg);

        xml_node<char> node = svg.subnode();
        parse_shapes(node);

        return 0;
    }

    void parse_shapes(xml_node<char>& node)
    {
        while(node){
            parse_node(node);
            node = node.next();
        };
    }

    void parse_node(xml_node<char>& node)
    {
        int type = svg_get_shape_type(node.name());

        switch(type){
        case SHAPE_G:
            parse_g(node);
            break;
        case SHAPE_POINT:
            break;
        case SHAPE_LINE:
            parse_line(node);
            break;
        case SHAPE_RECT:
            parse_rect(node);
            break;
        case SHAPE_CIRCLE:
            parse_circle(node);
            break;
        case SHAPE_ELLIPSE:
            parse_ellipse(node);
            break;
        case SHAPE_POLYLINE:
            parse_polyline(node);
            break;
        case SHAPE_POLYGON:
            parse_polygon(node);
            break;
        case SHAPE_PATH:
            parse_path(node);
            break;
        default:
            print("unknown shape", node.name());
            break;
        }
    }

    void parse_g(xml_node<char>& node)
    {
        xml_node<char> subnode = node.subnode();
        parse_shapes(subnode);
    }

    void parse_line(xml_node<char>& node)
    {
        svg_shape shape;
        shape.type = SHAPE_LINE;
        vec2f p;

        p.x = node.attribute("x1").to_float();
        p.y = node.attribute("y1").to_float();
        shape.points.push_back(p);
        p.x = node.attribute("x2").to_float();
        p.y = node.attribute("y2").to_float();
        shape.points.push_back(p);

        parse_pen(shape.pen, node);

        shapes.push_back(shape);
        print("insert shape", node.name());
    }

    void parse_rect(xml_node<char>& node)
    {
        svg_shape shape;
        shape.type = SHAPE_RECT;
        vec2f p;

        p.x = node.attribute("cx").to_float();
        p.y = node.attribute("cy").to_float();
        shape.points.push_back(p);
        p.x = node.attribute("width").to_float();
        p.y = node.attribute("height").to_float();
        shape.points.push_back(p);

        parse_pen(shape.pen, node);

        shapes.push_back(shape);
        print("insert shape", node.name());
    }

    void parse_circle(xml_node<char>& node)
    {
        svg_shape shape;
        shape.type = SHAPE_CIRCLE;
        vec2f p;

        p.x = node.attribute("cx").to_float();
        p.y = node.attribute("cy").to_float();
        shape.points.push_back(p);
        p.x = p.y = node.attribute("r").to_float();
        shape.points.push_back(p);

        parse_pen(shape.pen, node);

        shapes.push_back(shape);
        print("insert shape", node.name());
    }

    void parse_ellipse(xml_node<char>& node)
    {
        svg_shape shape;
        shape.type = SHAPE_ELLIPSE;
        vec2f p;

        p.x = node.attribute("cx").to_float();
        p.y = node.attribute("cy").to_float();
        shape.points.push_back(p);
        p.x = node.attribute("rx").to_float();
        p.x = node.attribute("ry").to_float();
        shape.points.push_back(p);

        parse_pen(shape.pen, node);

        shapes.push_back(shape);
        print("insert shape", node.name());
    }

    void parse_polyline(xml_node<char>& node)
    {
        svg_shape shape;
        shape.type = SHAPE_POLYLINE;
        vec2f p;

        const char* text = node.attribute("points").value();

        parse_points(shape.points, text);

        parse_pen(shape.pen, node);

        shapes.push_back(shape);
        print("insert shape", node.name());
    }

    void parse_polygon(xml_node<char>& node)
    {
        svg_shape shape;
        shape.type = SHAPE_POLYGON;
        vec2f p;

        const char* text = node.attribute("points").value();

        parse_points(shape.points, text);

        parse_pen(shape.pen, node);

        shapes.push_back(shape);
        print("insert shape", node.name());
    }

    void parse_path(xml_node<char>& node)
    {
        svg_shape shape;
        shape.type = SHAPE_PATH;
        vec2f p;

        const char* text = node.attribute("d").value();

        parse_path_data(shape, text);
        parse_pen(shape.pen, node);

        shapes.push_back(shape);
        print("insert shape", node.name());
    }

public://styles
    void parse_pen(svg_pen& shape_pen, xml_node<char>& node);
    bool parse_color(uint32_t& color, const char* value);

    void parse_points(std::vector<vec2f>& points, const char* text);
    void parse_path_data(svg_shape& shape, const char* text);
};

void skip_space_and_break(const char* &text)
{
    while(*text){
        switch(*text){
        case '\t':
        case '\r':
        case '\n':
        case ' ':
        case ',':
            ++text;
            break;
        default:
            return ;
        }
    }
}

bool is_path_cmd(char ch)
{
    switch(ch){
    case '\0':
    case 'A': case 'C': case 'H': case 'L': case 'M': case 'Q': case 'S': case 'T': case 'V': case 'Z':
    case 'a': case 'c': case 'h': case 'l': case 'm': case 'q': case 's': case 't': case 'v': case 'z':
        return true;
    default:
        return false;
    }
}

void move_to_next_cmd(const char* &text)
{
    for( ; !is_path_cmd(*text); ++text);
}

const char* parse_float(float& n, const char* text)
{
    bool x = false;
    string_t s;
    skip_space_and_break(text);
    for( ; *text; ++text){
        switch(*text){
        case '0': case '1': case '2': case '3': case '4': case '5':
        case '6': case '7': case '8': case '9': case '.':
            s.push_back(*text);
            break;
        case '-':
            if(s.empty()){
                s.push_back(*text);
            }
            else{
                n = s.to_decimal();
                return text;
            }
            break;
        case ' ':
        case ','://break
            n = s.to_decimal();
            return text;
        default://cmd
            n = s.to_decimal();
            return text;
        }
    }
    //end string
    n = s.to_decimal();
    return text;
}

void parse_params(std::vector<float>& params, const char* &text)
{
    float n;
    for( ; *text; ){
        skip_space_and_break(text);
        if(is_path_cmd(*text)){
            return ;
        }
        text = parse_float(n, text);
        params.push_back(n);
    }
}

const char* parse_point(vec2f& p, const char* text)
{
    text = parse_float(p.x, text);
    return parse_float(p.y, text);
}

void svg_file::parse_points(std::vector<vec2f>& points, const char* text)
{
    vec2f p;
    string_t s;
    for( ; *text; ){
        text = parse_point(p, text);
        points.push_back(p);
    }
}

int svg_arc_to(path_command &path, vec2f start, float radiusX, float radiusY, float angle, int size, int sweep, vec2f end);

//M = moveto
//L = lineto
//H = horizontal lineto
//V = vertical lineto
//C = curveto
//S = smooth curveto
//Q = quadratic Belzier curve
//T = smooth quadratic Belzier curveto
//A = elliptical Arc
//Z = closepath
void svg_file::parse_path_data(svg_shape& shape, const char* text)
{
    vec2f p;
    vec2f p1, p2, p3;
    float value;
    string_t s;
    std::vector<float> params(7);

    for( ; *text; ){
        move_to_next_cmd(text);
        switch(*text){
        case '\0':
            return ;
        case 'M':
            text = parse_point(p, ++text);
            shape.path.move_to(p);
            break;
        case 'm':
            text = parse_point(p1, ++text);
            p += p1;
            shape.path.move_to(p);
            break;
        case 'L':
            text = parse_point(p, ++text);
            shape.path.line_to(p);
            break;
        case  'l':
            text = parse_point(p1, ++text);
            p += p1;
            shape.path.line_to(p);
            break;
        case 'C':
            text = parse_point(p1, ++text);
            text = parse_point(p2, text);
            text = parse_point(p3, text);
            shape.path.cubic_curve_to(p1, p2, p3);
            p = p3;
            break;
        case 'c':
            text = parse_point(p1, ++text);
            text = parse_point(p2, text);
            text = parse_point(p3, text);
            p1 += p;
            p2 += p;
            p3 += p;
            shape.path.cubic_curve_to(p1, p2, p3);
            p = p3;
            break;
        case 'Q':
            text = parse_point(p1, ++text);
            text = parse_point(p2, text);
            shape.path.conic_curve_to(p1, p2);
            p = p2;
            break;
        case 'q':
            text = parse_point(p1, ++text);
            text = parse_point(p2, text);
            p1 += p;
            p2 += p;
            shape.path.conic_curve_to(p1, p2);
            p = p2;
            break;
        case 'H':
            text = parse_float(p.x, ++text);
            shape.path.line_to(p);
            break;
        case 'h':
            text = parse_float(p1.x, ++text);
            p.x += p1.x;
            shape.path.line_to(p);
            break;
        case 'V':
            text = parse_float(p.y, ++text);
            shape.path.line_to(p);
            break;
         case 'v':
            text = parse_float(p1.y, ++text);
            p.y += p1.y;
            shape.path.line_to(p);
            break;
        case 'A':
            params.clear();
            parse_params(params, ++text);
            svg_arc_to(shape.path, p, params[0], params[1], params[2], params[3], params[4], vec2f(params[5], params[6]));
            p.x = params[5];
            p.y = params[6];
            break;
        case 'a':
            params.clear();
            parse_params(params, ++text);
            svg_arc_to(shape.path, p, params[0], params[1], params[2], params[3], params[4], vec2f(p.x + params[5], p.y + params[6]));
            p.x += params[5];
            p.y += params[6];
            break;
        case 'Z': case 'z':
            return ;
        default:
            s.push_back(*text);
            ++text;
            break;
        }
    }
    print(s.c_str());
}

//
// svg_get_shape_type
//

int svg_get_shape_type(const char* name)
{
    if(is_equal("g", name)){
        return SHAPE_G;
    }
    else if(is_equal("path", name)){
        return SHAPE_PATH;
    }
    else if(is_equal("rect", name)){
        return SHAPE_RECT;
    }
    else if(is_equal("circle", name)){
        return SHAPE_CIRCLE;
    }
    else if(is_equal("ellipse", name)){
        return SHAPE_ELLIPSE;
    }
    else if(is_equal("line", name)){
        return SHAPE_LINE;
    }
    else if(is_equal("polyline", name)){
        return SHAPE_POLYLINE;
    }
    else if(is_equal("polygon", name)){
        return SHAPE_POLYGON;
    }
    else if(is_equal("linearGradient", name)){
        return 0;
    }
    else if(is_equal("radialGradient", name)){
        return 0;
    }
    else if(is_equal("stop", name)){
        return 0;
    }
    else if(is_equal("defs", name)){
        return 0;
    }

    return SHAPE_UNKNOWN;
}

void svg_file::parse_pen(svg_pen& shape_pen, xml_node<char>& node)
{
    xml_node<char> subnode;
    xml_attribute<char> attrib;
    const char* text;
    uint32_t value;

    //fill
    attrib = node.attribute("fill");
    if(attrib){
        text = attrib.value("black");
        if(is_equal(text, "none")){
            shape_pen.fill = false;
            //print("shape.fill == none");
        }
        else{
            shape_pen.fill = true;
            if(parse_color(value, text)){
                shape_pen.fill_color= value;
            }
            else{
                print("error brush");
            }
        }
    }
    else{//使用全局设置
        shape_pen.fill = pen.fill;
        shape_pen.fill_color = pen.fill_color;
    }

    //stroke
    attrib = node.attribute("stroke");
    if(attrib){
        text = node.attribute("stroke").value("black");
        if(is_equal(text, "none")){
            shape_pen.stroke = false;
            //print("shape_pen == none");
        }
        else{
            shape_pen.stroke = true;
            if(parse_color(value, text)){
                shape_pen.stroke_color = value;
            }
            else{
                print("error pen");
            }
        }
    }
    else{//使用全局设置
        shape_pen.stroke = pen.stroke;
        shape_pen.stroke_color = pen.stroke_color;
    }

    //stroke width
    attrib = node.attribute("stroke-width");
    if(attrib){
        text = attrib.value("1");
        value = strtoul(text, 0, 10);
        shape_pen.width = value;
    }
    else{
        shape_pen.width = pen.width;
    }

    //dash array
    attrib = node.attribute("stroke-dasharray");
    if(attrib){
        text = attrib.value();
        std::vector<string_t> ls;
        stringsplit(ls, text, strlen(text), ',');
        for(size_t i=0; i<ls.size(); ++i){
            shape_pen.dash.push_back(ls[i].to_decimal());
        }
    }

    //stroke-dasharray="3,5,8,13"
    //repeatCount="indefinite" values="0;29"
}


//value:
//  currentColor
//  #123a
//  red
//  url(#effect_name)
bool svg_file::parse_color(uint32_t& color, const char* value)
{
    if(value[0] == '#'){
        color = strtoul(value + 1, 0, 16);
        color |= 0xFF000000;
        //print("num color", color);
        return true;
    }

    for(int i=0; i < sizeof(SVG_COLORS) / sizeof(SVG_COLOR); ++i){
        if(is_equal(value, SVG_COLORS[i].name)){
            color = SVG_COLORS[i].color;
            //print("base color", color);
            return true;
        }
    }

    if(is_equal(value, "currentColor")){
        color = 0xFF000000;
        return true;
    }

    print("error color", value);
    return false;
}

//
// svg_arc_to
//

enum SvgArcSweep
{
    SvgArcSweep_Negative = 0,
    SvgArcSweep_Positive = 1
};

enum SvgArcSize
{
    SvgArcSize_Small = 0,
    SvgArcSize_Large = 1
};

const double RadiansPerDegree = M_PI / 180.0;
const double DoublePI = M_PI * 2;

double CalculateVectorAngle(double ux, double uy, double vx, double vy)
{
    using namespace std;
    float ta = atan2(uy, ux);
    float tb = atan2(vy, vx);

    if (tb >= ta)
    {
        return tb - ta;
    }

    return DoublePI - (ta - tb);
}

int svg_arc_to(path_command &path, vec2f start, float radiusX, float radiusY, float angle, int size, int sweep, vec2f end)
{
    using namespace std;

    float RadiusX = fabs(radiusX);
    float RadiusY = fabs(radiusY);
    float Angle = angle;
    int Sweep = sweep;
    int Size = size;


    if (start == end){
        return 0;
    }

    if (RadiusX == 0.0f && RadiusY == 0.0f)
    {
        //path.AddLine(start, end);
        return 0;
    }

    typedef float var;

    var sinPhi = sin(Angle * RadiansPerDegree);
    var cosPhi = cos(Angle * RadiansPerDegree);

    var x1dash = cosPhi * (start.x - end.x) / 2.0 + sinPhi * (start.y - end.y) / 2.0;
    var y1dash = -sinPhi * (start.x - end.x) / 2.0 + cosPhi * (start.y - end.y) / 2.0;

    double root;
    var numerator = RadiusX * RadiusX * RadiusY * RadiusY - RadiusX * RadiusX * y1dash * y1dash - RadiusY * RadiusY * x1dash * x1dash;

    var rx = RadiusX;
    var ry = RadiusY;

    if (numerator < 0.0)
    {
        var s = (float)sqrt(1.0 - numerator / (RadiusX * RadiusX * RadiusY * RadiusY));

        rx *= s;
        ry *= s;
        root = 0.0;
    }
    else
    {
        root = ((Size == SvgArcSize_Large && Sweep == SvgArcSweep_Positive) ||
            (Size == SvgArcSize_Small && Sweep == SvgArcSweep_Negative) ? -1.0 : 1.0) *
            sqrt(numerator / (RadiusX * RadiusX * y1dash * y1dash + RadiusY * RadiusY * x1dash * x1dash));
    }

    var cxdash = root * rx * y1dash / ry;
    var cydash = -root * ry * x1dash / rx;

    var cx = cosPhi * cxdash - sinPhi * cydash + (start.x + end.x) / 2.0;
    var cy = sinPhi * cxdash + cosPhi * cydash + (start.y + end.y) / 2.0;

    var theta1 = CalculateVectorAngle(1.0, 0.0, (x1dash - cxdash) / rx, (y1dash - cydash) / ry);
    var dtheta = CalculateVectorAngle((x1dash - cxdash) / rx, (y1dash - cydash) / ry, (-x1dash - cxdash) / rx, (-y1dash - cydash) / ry);

    if (Sweep == SvgArcSweep_Negative && dtheta > 0)
    {
        dtheta -= 2.0 * M_PI;
    }
    else if (Sweep == SvgArcSweep_Positive && dtheta < 0)
    {
        dtheta += 2.0 * M_PI;
    }

    var segments = (int)ceil((double)fabs(dtheta / (M_PI / 2.0)));
    var delta = dtheta / segments;
    var t = 8.0 / 3.0 * std::sin(delta / 4.0) * std::sin(delta / 4.0) / std::sin(delta / 2.0);

    var startX = start.x;
    var startY = start.y;

    for (var i = 0; i < segments; ++i)
    {
        var cosTheta1 = std::cos(theta1);
        var sinTheta1 = std::sin(theta1);
        var theta2 = theta1 + delta;
        var cosTheta2 = std::cos(theta2);
        var sinTheta2 = std::sin(theta2);

        var endpointX = cosPhi * rx * cosTheta2 - sinPhi * ry * sinTheta2 + cx;
        var endpointY = sinPhi * rx * cosTheta2 + cosPhi * ry * sinTheta2 + cy;

        var dx1 = t * (-cosPhi * rx * sinTheta1 - sinPhi * ry * cosTheta1);
        var dy1 = t * (-sinPhi * rx * sinTheta1 + cosPhi * ry * cosTheta1);

        var dxe = t * (cosPhi * rx * sinTheta2 + sinPhi * ry * cosTheta2);
        var dye = t * (sinPhi * rx * sinTheta2 - cosPhi * ry * cosTheta2);

//        path.AddBezier(startX, startY, (float)(startX + dx1), (float)(startY + dy1),
//            (float)(endpointX + dxe), (float)(endpointY + dye), (float)endpointX, (float)endpointY);
        path.cubic_curve_to((float)(startX + dx1), (float)(startY + dy1),
            (float)(endpointX + dxe), (float)(endpointY + dye), (float)endpointX, (float)endpointY);

        theta1 = theta2;
        startX = (float)endpointX;
        startY = (float)endpointY;
    }
    return 0;
}

}//end namespace graphics
}//end namespace cgl

#endif //SVG_FILE_HPP_20200314220010

