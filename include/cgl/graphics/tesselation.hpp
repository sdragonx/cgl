/*
 Copyright (c) 2005-2020 sdragonx (mail:sdragonx@foxmail.com)

 tesselation.hpp

 2019-12-17 19:42:42

*/
#ifndef TESSELATION_HPP_20191217194242
#define TESSELATION_HPP_20191217194242

#include <cgl/math.hpp>
#include <cgl/vec.hpp>

#include "path.hpp"

#define LIBTESS_CUSTOM_VECTOR

namespace libtess{
    typedef cgl::vec2f Vec2;
    typedef cgl::vec3f Vec3;
}

#include <cgl/utils/libtess/mesh.hpp>
#include <cgl/utils/libtess/tess.hpp>
#include <cgl/utils/libtess/sweep.hpp>

namespace cgl{
namespace graphics{

class tesselator
{
public:
    const static int VERTEX_COUNT = 3;

private:
    libtess::Tesselator tess;

public:
    tesselator() : tess()
    {
    }

    ~tesselator()
    {
        this->dispose();
    }

    int init(void* userdata = 0)
    {
        this->dispose();
        tess.init();
        return 0;
    }

    void dispose()
    {
        tess.dispose();
    }

    int add_contour(const vec2f* vs, size_t size)
    {
        tess.AddContour( 2, vs, sizeof(vec2f), size );
        return 0;
    }

    int add_contour(const std::vector<vec2f>& vs)
    {
        return this->add_contour(&vs[0], vs.size());
    }

    int add_contour(const std::vector<math::ctrl_point>& vs)
    {
        tess.AddContour( 2, &vs[0], sizeof(math::ctrl_point), vs.size());
        return 0;
    }

    int tesselate(libtess::TessWindingRule rule = libtess::TESS_WINDING_ODD)
    {
        //tess.processCDT = true;
        if (!tess.Tesselate(
            rule,
            libtess::TESS_TRIANGLES
//            libtess::TESS_BOUNDARY_CONTOURS
            )){

            return -1;
        }
        return 0;
    }

    void copy(std::vector<vec2f>& vs, std::vector<int> &ids)
    {
        //CGL_LOG("copy vertex end %i", vertexCount);
        vs.clear();
        for(size_t i=0; i<tess.vertices.size(); ++i){
            vs.push_back(vec2f(tess.vertices[i].x, tess.vertices[i].y));
        }
        ids.clear();
        ids.insert(ids.begin(), tess.elements.begin(), tess.elements.end());
    }

    #if 0
    void paint()
    {
        const vec2f* vertices = (vec2f*)(tessGetVertices(tess));
        const int* indices = tessGetVertexIndices(tess);
        const int* elements = tessGetElements(tess);
        const int elementsCount = tessGetElementCount(tess);

        draw_elements(GL_TRIANGLES, vertices, elements, elementsCount * 3);

        //const int nverts = tessGetVertexCount(tess);
        for (int i = 0; i < elementsCount; ++i)
        {
            const int* p = &elements[i*VERTEX_COUNT];
            glBegin(GL_TRIANGLES);
            for (int j = 0; j < VERTEX_COUNT && p[j] != TESS_UNDEF; ++j)
                glVertex2fv(vertices[p[j]].data);
            glEnd();
        }
    }
    #endif
};

void tesselate(std::vector<vec2f>& vs, std::vector<int>& ids, const path_command& path)
{
    std::vector<vec2f> temp;
    tesselator tess;
    tess.init();
    int j = 0;
    const vec2f *p = &path.points[0];
    for(size_t i=0; i<path.commands.size(); ++i){
        switch(path.commands[i]){
        case CGL_MOVE_TO:
            tess.add_contour(temp);
            temp.clear();
            ++j;
            break;
        case CGL_LINE_TO:
            temp.push_back(p[j]);
            ++j;
            break;
        case CGL_CONIC_CURVE:
            math::conic_bezier(temp, p[j], p[j+1], p[j+2], 10);
            j += 3;
            break;
        case CGL_CUBIC_CURVE:
            math::cubic_bezier(temp, p[j], p[j+1], p[j+2], p[j+3], 10);
            j += 4;
            break;
        }
    }
    if(!temp.empty()){
        tess.add_contour(temp);
    }

    tess.tesselate();
    tess.copy(vs, ids);
}

}//end namespace graphics
}//end namespace cgl

#endif //TESSELATION_HPP_20191217194242
