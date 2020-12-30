/*
 Copyright (c) 2005-2020 sdragonx (mail:sdragonx@foxmail.com)

 blocktexture.hpp

 2017-02-08 16:25:06

 小块纹理组成大图像，貌似用处不大了
 截取屏幕的时候，屏幕清除成黑色，然后绘制图像
 graphics.clear(0, 0, 0, 0);

*/
#ifndef BLOCKTEXTURE_HPP_20170208162506
#define BLOCKTEXTURE_HPP_20170208162506

#include "platform.hpp"

namespace cgl{
namespace graphics{

//支持非二次方随意大小纹理
class block_texture
{
public:
    GLuint texture[8][8];   //512*8 = 4096
    const static int TEX_SIZE = 512;
    const static int SIZE_MARK = TEX_SIZE-1;
    int m_width, m_height;
public:
    texture_buffer()
    {
        memset(texture, 0, sizeof(texture));
    }
    
    void resize(int width, int height)
    {
        int tx, ty;
        for(int y=0; y<height; y+=TEX_SIZE)
        {
            for(int x=0; x<width; x+=TEX_SIZE)
            {
                tx = x/TEX_SIZE;
                ty = y/TEX_SIZE;
                if(!texture[ty][tx]){
                    texture[ty][tx] =
                        gl.TexImage2D(TEX_SIZE, TEX_SIZE, GL_RGBA,
                            GL_RGBA, GL_UNSIGNED_BYTE, 0, GL_NEAREST);
                }
            }
        }
    }

    void paint(int width, int height)
    {
        glPushMatrix();
        for(int y=0; y<height; y+=TEX_SIZE)
        {
            for(int x=0; x<width; x+=TEX_SIZE)
            {
                #if 0
                glColor3f(1, 0, 0);
                glBindTexture(GL_TEXTURE_2D, 0);
                glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
                gl.glRectf(x, height-y-TEX_SIZE, x+TEX_SIZE, height-y);
                glColor4f(1, 1, 1, 1);
                #endif
                glBindTexture(GL_TEXTURE_2D, texture[y/TEX_SIZE][x/TEX_SIZE]);
                //这个混合模式算是最好的，但源图的黑色阴影还是淡化了
                glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
                gl.glRectf(x, height-y-TEX_SIZE, x+TEX_SIZE, height-y, 0, 1, 1, 0);
                glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            }
        }
        glPopMatrix();
    }

    void copy_screen(int width, int height)
    {
        for(int y=0; y<height; y+=TEX_SIZE)
        {
            for(int x=0; x<width; x+=TEX_SIZE)
            {
                glBindTexture(GL_TEXTURE_2D, texture[y/TEX_SIZE][x/TEX_SIZE]);
                glCopyTexSubImage2D(GL_TEXTURE_2D, 0,
                    0, 0,   //纹理坐标偏移量
                    x, y,   //屏幕偏移量
                    TEX_SIZE, TEX_SIZE);
            }
        }
    }
};

}//end namespace graphics
}//end namespace cgl

#endif //BLOCKTEXTURE_HPP_20170208162506
