/*
 Copyright (c) 2005-2020 sdragonx (mail:sdragonx@foxmail.com)

 glbillboard.hpp

 2019-12-25 00:05:21

*/
#ifndef GLBILLBOARD_HPP_20191225000521
#define GLBILLBOARD_HPP_20191225000521

#include <cgl/public.h>

namespace cgl{
namespace graphics{
namespace gl{

//y
/*
void draw_billboard(graphics::glcontext& gl, const graphics::teximage* image, float x, float y, float z, float size_x, float size_y)
{
    gl.push_matrix();
    //gl.translate(x, y, z);

    float u1=image->u1, u2=image->u2, v1=image->v1,v2=image->v2;
    vec4ub color = gl.color;

//    x -= camera.atx;
//    y -= camera.aty;
//    z -= camera.atz;

    float angle =math::angle(x - camera.atx, z - camera.atz);
    frmMain->Caption = angle;// fmod(angle - camera.angleY, 360);
    angle = -camera.angleY;

    float sinval = math::fast_sin(angle);
    float cosval = math::fast_cos(angle);
    float cosx = size_x * cosval;
    float sinx = size_x * sinval;
    float cosy = 0 * cosval;
    float siny = 0 * sinval;

    vtx3f vtx[] = {
        vtx3f(x - cosx - siny, y - size_y, z - cosy + sinx, u1, v1, color),
        vtx3f(x + cosx - siny, y - size_y, z - cosy - sinx, u2, v1, color),
        vtx3f(x + cosx - siny, y + size_y, z - cosy - sinx, u2, v2, color),
        vtx3f(x - cosx - siny, y + size_y, z - cosy + sinx, u1, v2, color)
    };

    gl.bind_texture(image->image);
    gl.draw_arrays(GL_TRIANGLE_FAN, vtx, 0, 4);

//    gl.draw_image(image->image, x - width * 0.5f, y - height - 0.5f, width, height, image->u1, image->v2, image->u2, image->v1);
    gl.pop_matrix();
}








2020-07-13 01:20:14
貌似能用


VOID BillBoard()
{
//用来保存一个4*4的矩阵
float m[16];

//取得当前的模型矩阵,并保存到数组m中
glGetFloatv(GL_MODELVIEW_MATRIX,m);

//用m矩阵的部分参数画一个方形平面
glBegin(GL_QUADS);
glVertex3f(-(m[0]+m[1]),-(m[4]+m[5]),-(m[8]+m[9]));
glVertex3f(m[0]-m[1],m[4]-m[5],m[8]-m[9]);
glVertex3f(m[0]+m[1],m[4]+m[5],m[8]+m[9]);
glVertex3f(m[1]-m[0],m[5]-m[4],m[9]-m[8]);
glEnd();
}


void billboard2()
{
    float modelview[16];
    int i,j;
    // save the current modelview matrix
    glPushMatrix();
    // get the current modelview matrix
    glGetFloatv(GL_MODELVIEW_MATRIX , modelview);
    // undo all rotations
    // beware all scaling is lost as well
    for( i=0; i<3; i++ ){
        for( j=0; j<3; j++ ){
            if ( i==j )
                modelview[i*4+j] = 1.0;
            else
                modelview[i*4+j] = 0.0;
        }
    }
    // set the modelview with no rotations and scaling
    glLoadMatrixf(modelview);
    //drawObject();
    gl.draw_image(tex, -1, -1, 1, 1);
    // restores the modelview matrix
    glPopMatrix();
}
*/

}//end namespace gl
}//end namespace graphics
}//end namespace cgl

#endif //GLBILLBOARD_HPP_20191225000521
