/*
 Copyright (c) 2005-2020 sdragonx (mail:sdragonx@foxmail.com)

 glconeofsight.hpp

 2019-12-24 23:58:36

*/
#ifndef GLCONEOFSIGHT_HPP_20191224235836
#define GLCONEOFSIGHT_HPP_20191224235836

#include <cgl/public.h>

namespace cgl{
namespace graphics{
namespace gl{

class glcone
{
public:
    vec4f frustumPlanes[6];//left bottom right top far near

public:

    //计算当前视锥体范围
    void calculateFrustumPlanes( void );

    //判断点是否在视锥体内
    bool isBoundingSphereInFrustum(float x, float y, float z);

    //绘制视锥体，这个绘制的不准确，简单测试用的
    void draw_frustum(graphics::glcontext& dc)
    {
        vec4f* plants = frustumPlanes;
        vtx3f vs[] = {
            vtx3f(plants[0].x, plants[1].y, plants[4].z, 0.0, 0.0, dc.color),
            vtx3f(plants[2].x, plants[1].y, plants[4].z, 1.0, 0.0, dc.color),
            vtx3f(plants[2].x, plants[3].y, plants[4].z, 1.0, 1.0, dc.color),
            vtx3f(plants[0].x, plants[3].y, plants[4].z, 0.0, 1.0, dc.color)

            vtx3f(plants[0].x, plants[1].y, plants[4].z, 0.0, 0.0, dc.color),
            vtx3f(plants[2].x, plants[1].y, plants[4].z, 1.0, 0.0, dc.color),
            vtx3f(plants[2].x, plants[3].y, plants[4].z, 1.0, 1.0, dc.color),
            vtx3f(plants[0].x, plants[3].y, plants[4].z, 0.0, 1.0, dc.color)
        };
        dc.draw_arrays(GL_QUADS, vs, 0, 4);
    }

};

bool glcone::isBoundingSphereInFrustum( float x, float y, float z)
{
    for( int i = 0; i < 6; ++i ){
        if(frustumPlanes[i].x * x + frustumPlanes[i].y * y + frustumPlanes[i].z * z + frustumPlanes[i].w <= 0)
            return false;
    }
    return true;
}

/*
判断立方体是否在视锥体内
bool CubeInFrustum( float x, float y, float z, float size )
{
   int p;

   for( p = 0; p < 6; p++ )
   {//分别判断8个顶点
      if( frustum[p][0] * (x - size) + frustum[p][1] * (y - size) + frustum[p][2] * (z - size) + frustum[p][3] > 0 )
         continue;
      if( frustum[p][0] * (x + size) + frustum[p][1] * (y - size) + frustum[p][2] * (z - size) + frustum[p][3] > 0 )
         continue;
      if( frustum[p][0] * (x - size) + frustum[p][1] * (y + size) + frustum[p][2] * (z - size) + frustum[p][3] > 0 )
         continue;
      if( frustum[p][0] * (x + size) + frustum[p][1] * (y + size) + frustum[p][2] * (z - size) + frustum[p][3] > 0 )
         continue;
      if( frustum[p][0] * (x - size) + frustum[p][1] * (y - size) + frustum[p][2] * (z + size) + frustum[p][3] > 0 )
         continue;
      if( frustum[p][0] * (x + size) + frustum[p][1] * (y - size) + frustum[p][2] * (z + size) + frustum[p][3] > 0 )
         continue;
      if( frustum[p][0] * (x - size) + frustum[p][1] * (y + size) + frustum[p][2] * (z + size) + frustum[p][3] > 0 )
         continue;
      if( frustum[p][0] * (x + size) + frustum[p][1] * (y + size) + frustum[p][2] * (z + size) + frustum[p][3] > 0 )
         continue;
      return false;
   }
   return true;
}
*/

void glcone::calculateFrustumPlanes( void )
{
    mat4f p;   // projection matrix
    mat4f mv;  // model-view matrix
    mat4f mvp; // model-view-projection matrix
    float t;

    glGetFloatv( GL_PROJECTION_MATRIX, p.data );
    glGetFloatv( GL_MODELVIEW_MATRIX, mv.data );

    //
    // Concatenate the projection matrix and the model-view matrix to produce
    // a combined model-view-projection matrix.
    //

    mvp = p * mv;
    //
    // Extract the frustum's right clipping plane and normalize it.
    //

    frustumPlanes[0].x = mvp[ 3] - mvp[ 0];
    frustumPlanes[0].y = mvp[ 7] - mvp[ 4];
    frustumPlanes[0].z = mvp[11] - mvp[ 8];
    frustumPlanes[0].w = mvp[15] - mvp[12];

    //frustumPlanes[0].normalize();

    t = (float) sqrt( frustumPlanes[0].x * frustumPlanes[0].x +
                      frustumPlanes[0].y * frustumPlanes[0].y +
                      frustumPlanes[0].z * frustumPlanes[0].z );

    frustumPlanes[0].x /= t;
    frustumPlanes[0].y /= t;
    frustumPlanes[0].z /= t;
    frustumPlanes[0].w /= t;

    //
    // Extract the frustum's left clipping plane and normalize it.
    //

    frustumPlanes[1].x = mvp[ 3] + mvp[ 0];
    frustumPlanes[1].y = mvp[ 7] + mvp[ 4];
    frustumPlanes[1].z = mvp[11] + mvp[ 8];
    frustumPlanes[1].w = mvp[15] + mvp[12];

    t = (float) sqrt( frustumPlanes[1].x * frustumPlanes[1].x +
                      frustumPlanes[1].y * frustumPlanes[1].y +
                      frustumPlanes[1].z * frustumPlanes[1].z );

    frustumPlanes[1].x /= t;
    frustumPlanes[1].y /= t;
    frustumPlanes[1].z /= t;
    frustumPlanes[1].w /= t;



    //
    // Extract the frustum's bottom clipping plane and normalize it.
    //

    frustumPlanes[2].x = mvp[ 3] + mvp[ 1];
    frustumPlanes[2].y = mvp[ 7] + mvp[ 5];
    frustumPlanes[2].z = mvp[11] + mvp[ 9];
    frustumPlanes[2].w = mvp[15] + mvp[13];

    t = (float) sqrt( frustumPlanes[2].x * frustumPlanes[2].x +
                      frustumPlanes[2].y * frustumPlanes[2].y +
                      frustumPlanes[2].z * frustumPlanes[2].z );

    frustumPlanes[2].x /= t;
    frustumPlanes[2].y /= t;
    frustumPlanes[2].z /= t;
    frustumPlanes[2].w /= t;

    //
    // Extract the frustum's top clipping plane and normalize it.
    //

    frustumPlanes[3].x = mvp[ 3] - mvp[ 1];
    frustumPlanes[3].y = mvp[ 7] - mvp[ 5];
    frustumPlanes[3].z = mvp[11] - mvp[ 9];
    frustumPlanes[3].w = mvp[15] - mvp[13];

    t = (float) sqrt( frustumPlanes[3].x * frustumPlanes[3].x +
                      frustumPlanes[3].y * frustumPlanes[3].y +
                      frustumPlanes[3].z * frustumPlanes[3].z );

    frustumPlanes[3].x /= t;
    frustumPlanes[3].y /= t;
    frustumPlanes[3].z /= t;
    frustumPlanes[3].w /= t;



    //
    // Extract the frustum's far clipping plane and normalize it.
    //

    frustumPlanes[4].x = mvp[ 3] - mvp[ 2];
    frustumPlanes[4].y = mvp[ 7] - mvp[ 6];
    frustumPlanes[4].z = mvp[11] - mvp[10];
    frustumPlanes[4].w = mvp[15] - mvp[14];

    t = (float) sqrt( frustumPlanes[4].x * frustumPlanes[4].x +
                      frustumPlanes[4].y * frustumPlanes[4].y +
                      frustumPlanes[4].z * frustumPlanes[4].z );

    frustumPlanes[4].x /= t;
    frustumPlanes[4].y /= t;
    frustumPlanes[4].z /= t;
    frustumPlanes[4].w /= t;

    //
    // Extract the frustum's near clipping plane and normalize it.
    //

    frustumPlanes[5].x = mvp[ 3] + mvp[ 2];
    frustumPlanes[5].y = mvp[ 7] + mvp[ 6];
    frustumPlanes[5].z = mvp[11] + mvp[10];
    frustumPlanes[5].w = mvp[15] + mvp[14];

    t = (float) sqrt( frustumPlanes[5].x * frustumPlanes[5].x +
                      frustumPlanes[5].y * frustumPlanes[5].y +
                      frustumPlanes[5].z * frustumPlanes[5].z );

    frustumPlanes[5].x /= t;
    frustumPlanes[5].y /= t;
    frustumPlanes[5].z /= t;
    frustumPlanes[5].w /= t;

}

}//end namespace gl
}//end namespace graphics
}//end namespace cgl

#endif //GLCONEOFSIGHT_HPP_20191224235836
