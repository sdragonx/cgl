/*
 Copyright (c) 2005-2020 sdragonx (mail:sdragonx@foxmail.com)

 blocktexture.hpp

 2017-02-08 16:25:06

 С��������ɴ�ͼ��ò���ô�������
 ��ȡ��Ļ��ʱ����Ļ����ɺ�ɫ��Ȼ�����ͼ��
 graphics.clear(0, 0, 0, 0);

*/
#ifndef BLOCKTEXTURE_HPP_20170208162506
#define BLOCKTEXTURE_HPP_20170208162506

#include "platform.hpp"

namespace cgl{
namespace graphics{

//֧�ַǶ��η������С����
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
                //������ģʽ������õģ���Դͼ�ĺ�ɫ��Ӱ���ǵ�����
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
                    0, 0,   //��������ƫ����
                    x, y,   //��Ļƫ����
                    TEX_SIZE, TEX_SIZE);
            }
        }
    }
};

}//end namespace graphics
}//end namespace cgl

#endif //BLOCKTEXTURE_HPP_20170208162506
