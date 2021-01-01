/*
 Copyright (c) 2005-2020 sdragonx (mail:sdragonx@foxmail.com)

 teximage.hpp

 2016-07-06 09:47:44

 attachment of atlas

 tileimage
 用于表示大纹理中的tile块

 */
#ifndef IMAGEDIV_H_20160706094744
#define IMAGEDIV_H_20160706094744

#include <cgl/public.h>
#include <vector>

namespace cgl{
namespace graphics{

//image align
//
//CGL_LEFT
//CGL_RIGHT
//CGL_HORIZONTAL //horizontal水平居中
//CGL_TOP
//CGL_BOTTOM
//CGL_VERTICAL   //vertical垂直居中
//CGL_CENTER     //居中
//CGL_CLIENT     //填充区域

const int IMAGE_FLIP_X = 0x0100;
const int IMAGE_FLIP_Y = 0x0200;

const int IMAGE_MIN_FILTER_NEAREST = 0x010000;
const int IMAGE_MIN_FILTER_LINEAR  = 0x020000;    //default
const int IMAGE_MAG_FILTER_NEAREST = 0x040000;
const int IMAGE_MAG_FILTER_LINEAR  = 0x080000;    //default

#pragma pack(push, 1)
class teximage
{
public:
    typedef teximage this_type;

public:
    union{
        intptr_t id;
        void* object;
    };
    uint16_t x, y, width, height;
    float u1, v1, u2, v2;

public:
    teximage() : id(), x(), y(), width(), height(), u1(0.0f), v1(0.0f), u2(1.0f), v2(1.0f)
    {
    }

    this_type& operator=(const this_type& other)
    {
        return this->assign(other);
    }

    bool is_null()const
    {
        return !id;
    }

    this_type& assign(intptr_t image, int _x, int _y, int _width, int _height,
        float _u1 = 0.0f, float _v1 = 0.0f, float _u2 = 1.0f, float _v2 = 1.0f)
    {
        id = image;
        x = _x;
        y = _y;
        width = _width;
        height = _height;
        u1 = _u1; v1 = _v1; u2 = _u2; v2 = _v2;
        return *this;
    }

    this_type& assign(void* _object, int _x, int _y, int _width, int _height,
        float _u1 = 0.0f, float _v1 = 0.0f, float _u2 = 1.0f, float _v2 = 1.0f)
    {
        object = _object;
        x = _x;
        y = _y;
        width = _width;
        height = _height;
        u1 = _u1; v1 = _v1; u2 = _u2; v2 = _v2;
        return *this;
    }

    this_type& assign(const this_type& other)
    {
        return this->assign(other.id, other.x, other.y, other.width, other.height,
            other.u1, other.v1, other.u2, other.v2);
    }

    void dispose()
    {
        memset(this, 0, sizeof(this_type));
    }

    //切片，返回切片数量
    int split(
        this_type* ls,
        int split_x,
        int split_y,
        int fix = 0,    //修正几个像素
        int maxframe = INT_MAX)
    {
        int frame = 0;
        int w = width  / split_x;
        int h = height / split_y;
        double s = (u2 - u1) / double(split_x);
        double t = (v2 - v1) / double(split_y);

        //对纹理坐标进行一个像素的修正，防止出现裂缝
        //double fix_x = w ? s / w : 0.0f;
        //double fix_y = h ? t / h : 0.0f;
        double fix_x = 0.0f;
        double fix_y = 0.0f;
        if(fix){
            fix_x = w ? s / w : 0.0f;
            fix_y = h ? t / h : 0.0f;
        }

        double u;
        double v = v1;
        for(int i = 0; i < split_y; ++i){
            u = u1;
            for(int j = 0; j < split_x; ++j){
                if(frame < maxframe){
                    this_type &tile = ls[frame];
                    tile.id  = id;
                    tile.width  = w;
                    tile.height = h;
                    tile.u1 = u + fix_x;
                    tile.u2 = u + s - fix_x;
                    tile.v1 = v + fix_y;
                    tile.v2 = v + t - fix_y;
                }
                else{
                    break;
                }
                ++frame;
                u += s;
            }
            v += t;
        }
        return frame;
    }

    /*
    //切片，返回切片数量
    int split(this_type* ls, int split_x, int split_y, int maxframe = INT_MAX)
    {
        int frame = 0;
        int w = width  / split_x;
        int h = height / split_y;
        double s = (u2 - u1) / double(split_x);
        double t = (v2 - v1) / double(split_y);
        //对纹理坐标进行一个像素的修正，防止出现裂缝
        double fix_x = 1.0 / 1024.0;// width ? 1.0 / width : 0.0f;
        double fix_y = 1.0 / 1024.0;//height ? 1.0 / height : 0.0f;

        double u;
        double v = v1;
        for(int i = 0; i < split_y; ++i){
            u = u1;
            for(int j = 0; j < split_x; ++j){
                //frame = i * split_x + j;
                if(frame < maxframe){
                    this_type &tile = ls[frame];
                    tile.id  = id;
                    tile.width  = w;
                    tile.height = h;
                    tile.u1 = u + fix_x;//u1 + s * j + fix_x;
                    tile.u2 = u + s - fix_x;//u1 + s * (j + 1) - fix_x;
                    tile.v1 = v + fix_y;//v1 + t * i + fix_y;
                    tile.v2 = v + t - fix_y;//v1 + t * (i + 1) - fix_y;
                }
                else{
                    break;
                }
                ++frame;
                u += s;
            }
            v += t;
        }
        return frame;
    }
    //*/

    int split(std::vector<this_type>& ls, int split_x, int split_y, int fix = 0, int maxframe = INT_MAX)
    {
        int frame = split_x * split_y;
        if(maxframe < frame)frame = maxframe;
        ls.resize(frame);

        return this->split(&ls[0], split_x, split_y, fix, frame);
    }

    //用于2D API操作，不需要uv坐标
    int split2D(this_type* ls, int split_x, int split_y, int maxframe = INT_MAX)
    {
        int frame = 0;
        int w = width  / split_x;
        int h = height / split_y;
        for(int i = 0; i < split_y; ++i){
            for(int j = 0; j < split_x; ++j){
                frame = i * split_x + j;
                if(frame < maxframe){
                    this_type &tile = ls[frame];
                    tile.id  = id;
                    tile.x = x + w * j;
                    tile.y = y + h * i;
                    tile.width  = w;
                    tile.height = h;
                }
                else{
                    break;
                }
            }
        }
        return frame;
    }

    int split2D(std::vector<this_type>& ls, int x, int y, int maxframe = INT_MAX)
    {
        int frame = x * y;
        if(maxframe < frame)frame = maxframe;
        ls.resize(frame);

        return this->split2D(&ls[0], x, y, frame);
    }
};
#pragma pack(pop)

}//end namespace graphics
}//end namespace cgl

#endif //IMAGEDIV_H_20160706094744
