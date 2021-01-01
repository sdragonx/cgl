/*
 Copyright (c) 2005-2020 sdragonx (mail:sdragonx@foxmail.com)

 polygo.hpp

 2019-12-27 18:22:05

 Clipper
   PreserveCollinear    默认的，当输入的被裁剪和裁剪多边形的内容中三个或者更多的点是共线的，
                        Clipper对象会在进行布尔运算之前对多余的共线点进行删除；当设定PreserveCollinear
                        属性防止了这种行为，来允许共线的点的情况出现在结果当中
   StrictlySimple       由Clipper库的裁剪操作返回的多边形都是简单多边形。当StrictlySimply属性被启用，
                        返回的多边形将会是强简单多边形，否则会返回弱简单多边形。
                        算法因为计算强简单多边形太大，导致该选项是默认关闭的
   ReverseSolution      当这个属性被设定为true时，由execute方法返回的函数将会与其正常方向相反
*/
#ifndef POLYGO_HPP_20191227182205
#define POLYGO_HPP_20191227182205

#include <cgl/public.h>
#include <cgl/fast_cast.hpp>
#include <cgl/vec.hpp>
#include <cgl/graphics/gl/glcontext.hpp>
#include <cgl/std/for_each.hpp>

#if defined(CGL_PLATFORM_CBUILDER)
#pragma option push
#pragma option -w-8066        //Unreachable code
#pragma option -w-8070        //Function should return a value
#endif

//#define use_int32
#include <cgl/external/clipper/clipper.cpp>

#define CGL_USING_LIBTESS

#ifdef CGL_USING_LIBTESS

#include <cgl/graphics/tesselation.hpp>

#else

//#undef Dot

//poly2tri
//#include <cgl/external/poly2tri/poly2tri.h>
#include <cgl/external/poly2tri/common/shapes.h>
#include <cgl/external/poly2tri/sweep/cdt.h>
#include <cgl/external/poly2tri/sweep/sweep.h>
#include <cgl/external/poly2tri/sweep/sweep_context.h>
#include <cgl/external/poly2tri/sweep/advancing_front.h>

#include <cgl/external/poly2tri/common/shapes.cc>
#include <cgl/external/poly2tri/sweep/cdt.cc>
#include <cgl/external/poly2tri/sweep/sweep.cc>
#include <cgl/external/poly2tri/sweep/sweep_context.cc>
#include <cgl/external/poly2tri/sweep/advancing_front.cc>

#endif//CGL_USING_LIBTESS

#if defined(CGL_PLATFORM_CBUILDER)
#pragma option pop
#endif

//#define CGL_POLYGON_DEBUG

#ifdef CGL_POLYGON_DEBUG

#include <vcl.h>
extern void print(String s);

void printd(char *format, ...)
{
    char buf[1024];
    va_list body;
    va_start(body, format);
    std::vsprintf(buf, format, body);
    va_end(body);
    OutputDebugStringA(buf);
}

void print(const char* name, const ClipperLib::PolyNode* Node)
{
    char buf[1024];
    sprintf(buf, "%s hole=%i open=%i contour=%i child=%i", name, Node->IsHole(), Node->IsOpen(), Node->Contour.size(), Node->ChildCount());
    print(buf);
}

#endif//CGL_POLYGON_DEBUG

namespace cgl{

typedef std::vector<vec2f> path2f;
typedef std::vector<vec3f> path3f;
typedef std::vector<int> index_buffer;

//namespace graphics{

enum joint_type{
    joint_square,
    joint_round,
    joint_miter,

    JOINT_SQUARE = joint_square,
    JOINT_ROUND = joint_round,
    JOINT_MITER = joint_miter
};

enum endcap_type{
    ENDCAP_CLOSED,
    ENDCAP_CLOSED_LINE,
    ENDCAP_BUTT,
    ENDCAP_SQUARE,
    ENDCAP_ROUND
};

enum endcap_type2{
    endcap_butt,
    endcap_square,
    endcap_round,
    endcap_closed

};

const int CLIPPER_POINT_SHIFT = 5;
const int CLIPPER_POINT_SCALE = 32;
const float CLIPPER_POINT_SCALE_INVERSE = 1.0 / 32.0;

//---------------------------------------------------------------------------
#ifdef TESSELATION_HPP_20191217194242

void transform_points(std::vector<vec2f>& points, const ClipperLib::Path& path)
{
    points.clear();
    for(size_t j=0; j<path.size(); ++j){
        //points.push_back(vec2f(path[j].X/CLIPPER_POINT_SCALE, path[j].Y/CLIPPER_POINT_SCALE));
        points.push_back(vec2f(path[j].X>>CLIPPER_POINT_SHIFT, path[j].Y>>CLIPPER_POINT_SHIFT));
    }
}

void tesselate_paths(std::vector<vec2f>& vs, std::vector<int>& ids, const ClipperLib::Paths& paths)
{
    static std::vector<vec2f> v;
    static tesselator tess;
    tess.init(0);
    for (size_t i = 0; i < paths.size(); ++i){
        const ClipperLib::Path& path = paths[i];
        v.clear();
        transform_points(v, path);
        if(v.size() > 2){
            tess.add_contour(&v[0], v.size());
        }
    }

    if(v.size() > 2){
        tess.tesselate();
        tess.copy(vs, ids);
        //vector_mul(vs, CLIPPER_POINT_SCALE_INVERSE);
    }
}

void tesselate_polytree(std::vector<vec2f>& vs, std::vector<int>& ids,
    const ClipperLib::PolyTree& polytree, bool ignoreFills = false, bool ignoreHoles = true)
{
    if(polytree.Total() == 0){
        return ;
    }

//    if(ignoreFills && ignoreHoles){
//        return ;
//    }

    std::vector<vec2f> points;
    static tesselator tess;

    ClipperLib::PolyNode* node = polytree.GetFirst();
    ClipperLib::PolyNode* subnode;

    tess.init(0);
    while(node){
        if(node->IsHole()){
            if(ignoreHoles){
                node = node->GetNext();
                continue;
            }
        }
        else{
            if(ignoreFills){
                node = node->GetNext();
                continue;
            }
        }

        if(!ignoreFills){
            transform_points(points, node->Contour);
            tess.add_contour(points);
        }

        for(size_t i=0; i < node->Childs.size(); ++i){
            subnode = node->Childs[i];
            if(subnode->IsHole()){
                transform_points(points, subnode->Contour);
                tess.add_contour(points);
            }
        }

        node = node->GetNext();
    }

    if(points.size() > 2){
        tess.tesselate();
        tess.copy(vs, ids);
    }

    tess.dispose();
}

class polygon_renderbuf
{
protected:
    std::vector<vec2f> m_vertices;
    std::vector<int> m_indices;

public:
    polygon_renderbuf() : m_vertices(), m_indices()
    {

    }

    void clear()
    {
        m_vertices.clear();
        m_indices.clear();
    }

//    int tesselate(const ClipperLib::Path& path)
//    {
//        CGL_ASSERT(0);
//        return 0;
//    }

    int tesselate(const ClipperLib::Paths& paths)
    {
        tesselate_paths(m_vertices, m_indices, paths);
        return 0;
    }

    int tesselate(const ClipperLib::PolyTree& polytree)
    {
        tesselate_polytree(m_vertices, m_indices, polytree);
        return 0;
    }

    void paint()
    {
        #ifndef CGL_PLATFORM_GLES
        graphics::draw_elements(GL_TRIANGLES, &m_vertices[0], &m_indices[0], m_indices.size());
        #endif
    }
};

#else
//---------------------------------------------------------------------------
//poly2tri

typedef std::vector<p2t::Point>  p2t_pointbuf;
typedef std::list<p2t_pointbuf>  p2t_polypool;
typedef std::vector<p2t::Point*> p2t_polygon;

void transform_points(std::vector<p2t::Point*>& points_ptr,
    std::list< std::vector<p2t::Point> >& polypool, const ClipperLib::Path& path)
{
    typedef std::vector<p2t::Point> polyline;
    polypool.push_back(polyline());
    polyline& pl = polypool.back();
    pl.reserve(path.size());
    points_ptr.clear();
    for(size_t i=0; i<path.size(); ++i){
        pl.push_back(p2t::Point(path[i].X, path[i].Y));
        points_ptr.push_back(&pl.back());
    }
}

void tesselate_polytree(std::vector<vec2f>& vs, const ClipperLib::PolyTree& polytree, bool ignoreFills = false, bool ignoreHoles = true)
{
    typedef std::vector<p2t::Point> polyline_type;

    if(polytree.Total() == 0){
        return ;
    }

    #ifdef CGL_POLYGON_DEBUG
    print("polytree", &polytree);
    #endif

    std::list<polyline_type> polypool;
    std::vector<p2t::Point*> points_ptr;

    ClipperLib::PolyNode* node = polytree.GetFirst();
    ClipperLib::PolyNode* subnode;

    while(node){
        #ifdef CGL_POLYGON_DEBUG
        print("node", node);
        #endif

        if(node->IsHole()){
            if(ignoreHoles){
                node = node->GetNext();
                continue;
            }
        }
        else{
            if(ignoreFills){
                node = node->GetNext();
                continue;
            }
        }

        points_ptr.clear();
        if(!ignoreFills){
            transform_points(points_ptr, polypool, node->Contour);
        }

        p2t::CDT cdt(points_ptr);

        for(size_t i=0; i < node->Childs.size(); ++i){
            subnode = node->Childs[i];
            #ifdef CGL_POLYGON_DEBUG
            print("subnode", subnode);
            #endif
            if(subnode->IsHole()){
                transform_points(points_ptr, polypool, subnode->Contour);
                cdt.AddHole(points_ptr);
            }
            else{
                #ifdef CGL_POLYGON_DEBUG
                print("is not hole");
                #endif
            }
        }

        try{
            cdt.Triangulate();
        }
        catch(std::runtime_error err){
            CGL_LOG(err);
            ShowMessage(err.what());
            return ;
        }

        //提取三角形
        const std::vector<p2t::Triangle*> triangles = cdt.GetTriangles();
        for(size_t j=0; j<triangles.size(); ++j){
            for(size_t k=0; k<3; ++k){
                p2t::Point* p = triangles[j]->GetPoint(k);
                vs.push_back(vec2f(p->x * CLIPPER_POINT_SCALE_INVERSE, p->y * CLIPPER_POINT_SCALE_INVERSE));
            }
        }

        polypool.clear();
        node = node->GetNext();
    }
    #ifdef CGL_POLYGON_DEBUG
    print("end--------");
    #endif
}


class polygon_renderbuf
{
protected:
    std::vector<vec2f> m_vertices;

public:
    polygon_renderbuf() : m_vertices()
    {

    }

    void clear()
    {
        m_vertices.clear();
    }

    int tesselate(const ClipperLib::PolyTree& tree)
    {
        tesselate_polytree(m_vertices, tree);
        return 0;
    }

    void paint()
    {
        graphics::draw_arrays(GL_TRIANGLES, &m_vertices[0], m_vertices.size());
    }
};

#endif
//---------------------------------------------------------------------------
// path/paths init

template<typename T>
void path_set_points(ClipperLib::Path& path, const std::vector< vec2<T> >& points)
{
    path.clear();
    ClipperLib::IntPoint p;
    for(size_t i=0; i<points.size(); ++i){
        p.X = fast_cast<int>(points[i].x);
        p.Y = fast_cast<int>(points[i].y);
        p.X <<= CLIPPER_POINT_SHIFT;
        p.Y <<= CLIPPER_POINT_SHIFT;
        path.push_back(p);
    }
}

template<typename T>
void path_set_points(ClipperLib::Path& path, const std::vector< vec3<T> >& points)
{
    path.clear();
    ClipperLib::IntPoint p;
    for(size_t i=0; i<points.size(); ++i){
        p.X = fast_cast<int>(points[i].x);
        p.Y = fast_cast<int>(points[i].y);
        p.X <<= CLIPPER_POINT_SHIFT;
        p.Y <<= CLIPPER_POINT_SHIFT;
        path.push_back(p);
    }
}

void path_set_points(ClipperLib::Path& path, const std::vector< math::ctrl_point > points)
{
    path.clear();
    ClipperLib::IntPoint p;
    for(size_t i=0; i<points.size(); ++i){
        p.X = round(points[i].x);
        p.Y = round(points[i].y);
        p.X <<= CLIPPER_POINT_SHIFT;
        p.Y <<= CLIPPER_POINT_SHIFT;
        path.push_back(p);
    }
}

void path_set_rect(ClipperLib::Path & path, int x, int y, int width, int height)
{
    path.clear();
    x <<= CLIPPER_POINT_SHIFT;
    y <<= CLIPPER_POINT_SHIFT;
    width  <<= CLIPPER_POINT_SHIFT;
    height <<= CLIPPER_POINT_SHIFT;
    path.push_back(ClipperLib::IntPoint(x, y));
    path.push_back(ClipperLib::IntPoint(x + width, y));
    path.push_back(ClipperLib::IntPoint(x + width, y + height));
    path.push_back(ClipperLib::IntPoint(x, y + height));
    path.push_back(ClipperLib::IntPoint(x, y));
}

//---------------------------------------------------------------------------
// draw path/paths

void draw_polytree(const ClipperLib::PolyTree& polytree)
{
    std::vector<vec2f> vs;
    #ifdef CGL_USING_LIBTESS
    std::vector<int> ids;
    tesselate_polytree(vs, ids, polytree);
    #ifndef CGL_PLATFORM_GLES
    graphics::draw_elements(GL_TRIANGLES, &vs[0], &ids[0], ids.size());
    #endif
    #else
    tesselate_polytree(vs, polytree);
    graphics::draw_arrays(CGL_TRIANGLES, &vs[0], vs.size());
    #endif
}

void draw_polyline(ClipperLib::Paths& paths, double pen_width = 20.0f)
{
	ClipperLib::PolyTree polytree;

	ClipperLib::ClipperOffset co;
	co.AddPaths(paths, (ClipperLib::JoinType)JOINT_ROUND, (ClipperLib::EndType)ENDCAP_ROUND);
    co.Execute(polytree, pen_width * CLIPPER_POINT_SCALE);

    draw_polytree(polytree);
}

void draw_polygon(ClipperLib::Paths& paths, double pen_width = 20.0f)
{
    ClipperLib::PolyTree polytree;

    ClipperLib::ClipperOffset co;
    co.AddPaths(paths, (ClipperLib::JoinType)JOINT_ROUND, (ClipperLib::EndType)ENDCAP_CLOSED_LINE);
    co.Execute(polytree, pen_width * CLIPPER_POINT_SCALE);

    draw_polytree(polytree);
}

void fill_polygon(ClipperLib::Paths& paths, double pen_width = 20.0f)
{
    ClipperLib::PolyTree polytree;

    ClipperLib::Clipper c;
    c.AddPaths(paths, ClipperLib::ptSubject, true);
    c.Execute(ClipperLib::ctUnion, polytree, ClipperLib::pftEvenOdd);

//    ClipperLib::ClipperOffset co;
//    co.AddPaths(paths, (ClipperLib::JoinType)JOINT_ROUND, (ClipperLib::EndType)ENDCAP_CLOSED);
//    co.Execute(polytree, -pen_width * CLIPPER_POINT_SCALE);

    draw_polytree(polytree);
}

//---------------------------------------------------------------------------
// polygon

class polygon
{
public:
    typedef polygon this_type;
    typedef ClipperLib::IntPoint point_type;
    typedef ClipperLib::Path  path_type;
    typedef ClipperLib::Paths pathset;

protected:
    pathset m_paths;
    polygon_renderbuf m_line, m_fill;

public:
    bool closed;
//    int joint;
//    int endcap;

public:
    polygon() : m_paths()
    {
        this->init();
    }

    polygon(const pathset& paths) : m_paths(paths)
    {
        this->init();
    }

    void init()
    {
        closed = false;
//        joint = JOINT_ROUND;
//        endcap = ENDCAP_ROUND;
    }

    void clear()
    {
        m_paths.clear();
        m_line.clear();
        m_fill.clear();
    }

//assign
    template<typename T>
    this_type& assign(const std::vector< vec2<T> >& path)
    {
        this->clear();
        m_paths.push_back(path_type());
        path_set_points(m_paths.back(), path);
        return *this;
    }

    template<typename T>
    this_type& assign(const std::vector< vec3<T> >& path)
    {
        this->clear();
        m_paths.push_back(path_type());
        path_set_points(m_paths.back(), path);
        return *this;
    }

    this_type& assign(const std::vector<math::ctrl_point>& path)
    {
        this->clear();
        m_paths.push_back(path_type());
        path_set_points(m_paths.back(), path);
        return *this;
    }

    this_type& assign(int x, int y, int width, int height)
    {
        this->clear();
        m_paths.push_back(path_type());
        path_set_rect(m_paths.back(), x, y, width, height);
        return *this;
    }

    this_type& append(const path_type& path, ClipperLib::ClipType op = ClipperLib::ctUnion)
    {
        ClipperLib::Clipper c;
        c.AddPaths(m_paths, ClipperLib::ptSubject, true);
        c.AddPath(path, ClipperLib::ptClip, true);
        c.Execute(op, m_paths, ClipperLib::pftEvenOdd);
        return *this;
    }

    this_type& append(const pathset& paths, ClipperLib::ClipType op = ClipperLib::ctUnion)
    {
        ClipperLib::Clipper c;
        c.AddPaths(m_paths, ClipperLib::ptSubject, true);
        c.AddPaths(paths, ClipperLib::ptClip, true);
        c.Execute(op, m_paths, ClipperLib::pftEvenOdd);
        return *this;
    }

    template<typename T>
    this_type& append(const std::vector< vec2<T> >& path, ClipperLib::ClipType op = ClipperLib::ctUnion)
    {
        path_type p;
        transform(p, path);
        return this->append(p, op);
    }

    void append(int x, int y, int width, int height, ClipperLib::ClipType op = ClipperLib::ctUnion)
    {
        path_type rect;
        path_set_rect(rect, x, y, width, height);
        this->append(rect, op);
    }

//bool operator
    this_type and(const polygon& poly)
    {
        return path_op(poly.m_paths, ClipperLib::ctIntersection);
    }

    this_type or(const polygon& poly)
    {
        return path_op(poly.m_paths, ClipperLib::ctUnion);
    }

    this_type xor(const polygon& poly)
    {
        return path_op(poly.m_paths, ClipperLib::ctXor);
    }

    this_type not(const polygon& poly)
    {
        return path_op(poly.m_paths, ClipperLib::ctDifference);
    }

    this_type& operator|=(const polygon& poly)
    {
        ClipperLib::Clipper c;
        c.AddPaths(m_paths, ClipperLib::ptSubject, true);
        c.AddPaths(poly.m_paths, ClipperLib::ptClip, true);
        c.Execute(ClipperLib::ctUnion, m_paths, ClipperLib::pftEvenOdd);
        return *this;
    }

public://update & paint
    void update(float pen_width = 1.0f, int jointType = JOINT_ROUND, int endcapType = ENDCAP_CLOSED_LINE)
    {
        this->update(m_line, pen_width, jointType, this->closed ? ENDCAP_CLOSED_LINE : endcapType);
        this->update(m_fill, -pen_width, jointType, ENDCAP_CLOSED);
    }

    void paint(vec4ub pen, vec4ub brush = vec4ub())
    {
        #ifndef CGL_PLATFORM_GLES
        glColor4ub(pen.r, pen.g, pen.b, pen.a);
        m_line.paint();
        glColor4ub(brush.r, brush.g, brush.b, brush.a);
        m_fill.paint();
        #endif
    }

private:
    pathset path_op(const pathset& paths, ClipperLib::ClipType op)
    {
        ClipperLib::Clipper c;
        ClipperLib::Paths temp;
        c.AddPaths(m_paths, ClipperLib::ptSubject, true);
        c.AddPaths(paths, ClipperLib::ptClip, true);
        c.Execute(op, temp, ClipperLib::pftEvenOdd);
        return temp;
    }

    void update(polygon_renderbuf& buf, float pen_width = 1.0f, int jointType = JOINT_ROUND, int endcapType = ENDCAP_ROUND)
    {
        ClipperLib::ClipperOffset co;
        ClipperLib::PolyTree polytree;

        co.Clear();

        co.ArcTolerance = 1.0;//默认的ArcTolerance是0.25个单位

        int t = std::clock();
        co.AddPaths(m_paths, (ClipperLib::JoinType)jointType, (ClipperLib::EndType)endcapType);
        co.Execute(polytree, pen_width * CLIPPER_POINT_SCALE);
        t = std::clock() - t;

        int tess = std::clock();
        buf.tesselate(polytree);
        tess = std::clock() - tess;
        CGL_LOG("clip = %i tess = %i", t, tess);
    }
};

//}//end namespace graphics
}//end namespace cgl

#endif //POLYGO_HPP_20191227182205

/*

void draw_path(TCanvas* dc, const ClipperLib::Path& path, TColor color = clBlack)
{    if(!path.empty()){        dc->Pen->Color = color;        dc->MoveTo(path[0].X, path[0].Y);        for (size_t t = 1; t < path.size(); t++){
            dc->LineTo(path[t].X, path[t].Y);
        }
        dc->LineTo(path[0].X, path[0].Y);
    }
}
void draw_path(graphics::draw_context& dc, const ClipperLib::Path& path, vec4ub color = graphics::color::red)
{    if(!path.empty()){        dc.color = color;        dc.begin(GL_LINE_STRIP);        dc.vertex(path[0].X, path[0].Y);
        for (size_t t = 1; t < path.size(); t++){
            dc.vertex(path[t].X, path[t].Y);
        }
        dc.vertex(path[0].X, path[0].Y);
        dc.end();
    }
}
void draw_paths(graphics::draw_context& dc, const ClipperLib::Paths& paths, vec4ub color = graphics::color::white){    for(size_t i=0; i<paths.size(); ++i){        draw_path(dc, paths[i], color);    }}

void tessPath(const ClipperLib::Paths& paths, vec4ub color = graphics::color::white)
{
    std::vector<vec2f> v;
    tesselator tess;
    tess.init(0);
    for (size_t i = 0; i < paths.size(); ++i){
        const ClipperLib::Path& path = paths[i];
        v.clear();
        for(size_t j=0; j<path.size(); ++j){
            v.push_back(vec2f(path[j].X, path[j].Y));
        }
        if(v.size() > 2){
            tess.add_contour(v);
        }
    }

    if(v.size() > 2){
        tess.tesselate();

//        tess.tesselate(v[0].data, v.size());
        glColor4f(1, 1, 1, 0.25);
        luna.gl.bind_texture(0);
        tess.paint();
    }
}


class polyline
{
protected:
    ClipperLib::Path m_path;
    polygon_renderbuf m_buffer;

public:
    void clear()
    {
        m_path.clear();
        m_buffer.clear();
    }

    void push(int x, int y)
    {
        m_path.push_back(ClipperLib::IntPoint(x * CLIPPER_POINT_SCALE, y * CLIPPER_POINT_SCALE));
    }

    void push(const vec2i& p)
    {
        this->push(p.x, p.y);
    }

    void update(float pen_width = 1.0f, int jointType = JOINT_ROUND, int endcapType = ENDCAP_ROUND)
    {
        ClipperLib::ClipperOffset co;
        ClipperLib::Paths paths;
        co.AddPath(m_path, (ClipperLib::JoinType)jointType, (ClipperLib::EndType)endcapType);
        co.Execute(paths, pen_width);
        m_buffer.tesselate(m_path);
    }
};




//2020-02-17 13:25:37
class polygon
{
public:
    typedef polygon this_type;
    typedef ClipperLib::IntPoint point_type;
    typedef ClipperLib::Path  path_type;
    typedef ClipperLib::Paths pathset;

protected:
    pathset m_paths;
    polygon_renderbuf m_line, m_fill;

public:
    bool closed;
//    int joint;
//    int endcap;

public:
    polygon() : m_paths()
    {
        this->init();
    }

    polygon(const pathset& paths) : m_paths(paths)
    {
        this->init();
    }

    void init()
    {
        closed = false;
//        joint = JOINT_ROUND;
//        endcap = ENDCAP_ROUND;
    }

    void clear()
    {
        m_paths.clear();
        m_line.clear();
        m_fill.clear();
    }

//assign
    template<typename T>
    this_type& assign(const std::vector< vec2<T> >& path)
    {
        this->clear();
        m_paths.push_back(path_type());
        path_set_points(m_paths.back(), path);
        return *this;
    }

    template<typename T>
    this_type& assign(const std::vector< vec3<T> >& path)
    {
        this->clear();
        m_paths.push_back(path_type());
        path_set_points(m_paths.back(), path);
        return *this;
    }

    this_type& assign(const std::vector<math::ctrl_point>& path)
    {
        this->clear();
        m_paths.push_back(path_type());
        path_set_points(m_paths.back(), path);
        return *this;
    }

    this_type& assign(int x, int y, int width, int height)
    {
        this->clear();
        m_paths.push_back(path_type());
        path_set_rect(m_paths.back(), x, y, width, height);
        return *this;
    }

    this_type& append(const path_type& path, ClipperLib::ClipType op = ClipperLib::ctUnion)
    {
        ClipperLib::Clipper c;
        c.AddPaths(m_paths, ClipperLib::ptSubject, true);
        c.AddPath(path, ClipperLib::ptClip, true);
        c.Execute(op, m_paths, ClipperLib::pftEvenOdd);
        return *this;
    }

    this_type& append(const pathset& paths, ClipperLib::ClipType op = ClipperLib::ctUnion)
    {
        ClipperLib::Clipper c;
        c.AddPaths(m_paths, ClipperLib::ptSubject, true);
        c.AddPaths(paths, ClipperLib::ptClip, true);
        c.Execute(op, m_paths, ClipperLib::pftEvenOdd);
        return *this;
    }

    template<typename T>
    this_type& append(const std::vector< vec2<T> >& path, ClipperLib::ClipType op = ClipperLib::ctUnion)
    {
        path_type p;
        transform(p, path);
        return this->append(p, op);
    }

    void append(int x, int y, int width, int height, ClipperLib::ClipType op = ClipperLib::ctUnion)
    {
        path_type rect;
        path_set_rect(rect, x, y, width, height);
        this->append(rect, op);
    }

//bool operator
    this_type and(const polygon& poly)
    {
        return path_op(poly.m_paths, ClipperLib::ctIntersection);
    }

    this_type or(const polygon& poly)
    {
        return path_op(poly.m_paths, ClipperLib::ctUnion);
    }

    this_type xor(const polygon& poly)
    {
        return path_op(poly.m_paths, ClipperLib::ctXor);
    }

    this_type not(const polygon& poly)
    {
        return path_op(poly.m_paths, ClipperLib::ctDifference);
    }

    this_type& operator|=(const polygon& poly)
    {
        ClipperLib::Clipper c;
        c.AddPaths(m_paths, ClipperLib::ptSubject, true);
        c.AddPaths(poly.m_paths, ClipperLib::ptClip, true);
        c.Execute(ClipperLib::ctUnion, m_paths, ClipperLib::pftEvenOdd);
        return *this;
    }

public://update & paint
    void update(float pen_width = 1.0f, int jointType = JOINT_ROUND, int endcapType = ENDCAP_CLOSED_LINE)
    {
        this->update(m_line, pen_width, jointType, this->closed ? ENDCAP_CLOSED_LINE : endcapType);
        this->update(m_fill, -pen_width, jointType, ENDCAP_CLOSED);
    }

    void paint(vec4ub pen, vec4ub brush = vec4ub())
    {
        glColor4ub(pen.r, pen.g, pen.b, pen.a);
        m_line.paint();
        glColor4ub(brush.r, brush.g, brush.b, brush.a);
        m_fill.paint();
    }

private:
    pathset path_op(const pathset& paths, ClipperLib::ClipType op)
    {
        ClipperLib::Clipper c;
        ClipperLib::Paths temp;
        c.AddPaths(m_paths, ClipperLib::ptSubject, true);
        c.AddPaths(paths, ClipperLib::ptClip, true);
        c.Execute(op, temp, ClipperLib::pftEvenOdd);
        return temp;
    }

    void update(polygon_renderbuf& buf, float pen_width = 1.0f, int jointType = JOINT_ROUND, int endcapType = ENDCAP_ROUND)
    {
        ClipperLib::ClipperOffset co;
        ClipperLib::PolyTree polytree;

        co.ArcTolerance = 1.0;//默认的ArcTolerance是0.25个单位

        int t = clock();
        co.AddPaths(m_paths, (ClipperLib::JoinType)jointType, (ClipperLib::EndType)endcapType);
        co.Execute(polytree, pen_width * CLIPPER_POINT_SCALE);
        t = clock() - t;

        int tess = clock();
        buf.tesselate(polytree);
        tess = clock() - tess;
        CGL_LOG("clip = %i tess = %i", t, tess);
    }
};
*/