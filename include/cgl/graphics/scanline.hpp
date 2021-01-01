/*
 Copyright (c) 2005-2020 sdragonx (mail:sdragonx@foxmail.com)

 scanline.hpp

 2020-03-27 06:19:48

*/
#ifndef SCANLINE_HPP_20200327061948
#define SCANLINE_HPP_20200327061948

#include <cgl/public.h>
#include <cgl/math/rasterization.hpp>

#include <map>
#include <vector>

namespace cgl{
namespace graphics{

class scanline
{
public:
    struct point_type{
        int id;
        int x;
        short alpha;
        char  key;    //是否关键像素
        float slope;    //斜率
        char position;
        bool operator<(const point_type& other)const { return x < other.x; }
        point_type() : x(-1), alpha(0), key(0)
        {
            id = -1;
        }
    };

    typedef std::vector<point_type> line_type;
    typedef std::map<int, line_type> line_buffer;

    typedef line_type::iterator pointer;
    typedef TYPENAME line_buffer::iterator iterator;

public:
    line_buffer lines;
    int current_id;

public:
    void clear();

    int solid_scan(int x1, int y1, int x2, int y2);
    int solid_scan(float x1, float y1, float x2, float y2);

    int smooth_scan(int x1, int y1, int x2, int y2);
    int smooth_scan(float x1, float y1, float x2, float y2);

private:
    void add_point(int x, int y);
    void add_point(int x, int y, int alpha, int key = 0, int pos = 0);

    int solid_line(int x1, int y1, int x2, int y2, bool finalpos);
    int smooth_line(int x1, int y1, int x2, int y2);
};

void scanline::clear()
{
    lines.clear();
}

void scanline::add_point(int x, int y)
{
    point_type p;
    p.x = x;
    p.alpha = lines[y].size() & 255;
    p.key = CGL_TRUE;
    p.id = current_id;
    lines[y].push_back(p);
}

void scanline::add_point(int x, int y, int alpha, int key, int pos)
{
    point_type p;
    p.x = x;
    p.alpha = alpha;
    p.key = key;
    p.id = current_id;
    p.position = pos;
    lines[y].push_back(p);
}

//列举扫描行并添加到列表中
//只处理y方向
int scanline::solid_line(int x1, int y1, int x2, int y2, bool finalpos)
{
    using namespace std;

    const static int DDA_BITS = 16;
    const static int DDA_ONE  = 1 << DDA_BITS;

    if(y2 < y1){
        std::swap(x1, x2);
        std::swap(y1, y2);
    }

    int dx = x2 - x1;
    int dy = y2 - y1;
    int k = abs(dy);
    if(k == 0){//水平线
        this->add_point(x1, y1);
        this->add_point(x1, y2);
        return 0;
    }

    int x_step, y_step;
    x_step = (dx << DDA_BITS) / k;
    y_step = DDA_ONE;

    x1 <<= DDA_BITS;
    y1 <<= DDA_BITS;
//    x1 += DDA_ONE >> 1;//四舍五入，移动半个像素
//    y1 += DDA_ONE >> 1;

    int i;
    int x, y;
    for (i = 0; i < k; ++i){
        x = x1 >> DDA_BITS;
        y = y1 >> DDA_BITS;
        this->add_point(x, y);
        x1 += x_step;
        y1 += y_step;
    }

    //最后压入p2
    if(finalpos){
        this->add_point(x2, y2);
    }

    return k;
}

int scanline::solid_scan(int x1, int y1, int x2, int y2)
{
    return this->solid_line(x1, y1, x2, y2, false);
}

int scanline::solid_scan(float x1, float y1, float x2, float y2)
{
    return this->solid_line(round(x1), round(y1), round(x2), round(y2), false);
}

//反锯齿直线
//不绘制竖线和横线
//尾部不接p2
#if 1

#define AA_PIXELS 2

int scanline::smooth_line(int x1, int y1, int x2, int y2)
{
    using namespace std;

    const static int AA_BITS = 16;
    const static int AA_ONE  = 1 << AA_BITS;
    const static int AA_MASK = 0xFF;

    if(y2 < y1){
        std::swap(x1, x2);
        std::swap(y1, y2);
    }

    int dx = (x2 - x1);
    int dy = (y2 - y1);

    int x_step, y_step;
    int k = abs(dx);
    int i;
    int x, y;
    int alpha;

    if(k < abs(dy)){//竖向扫描
        k = abs(dy);
        x_step = (dx << AA_BITS) / k;
        y_step = y1 < y2 ? AA_ONE : -AA_ONE;

        x1 <<= AA_BITS;
        y1 <<= AA_BITS;
//        x1 += AA_ONE >> 1;//四舍五入
//        y1 += AA_ONE >> 1;

        for (i = 0; i < k; ++i){
            x = x1 >> AA_BITS;
            y = y1 >> AA_BITS;
            alpha = (x1>>8) & AA_MASK;
            //横向两个像素
            #if AA_PIXELS == 1
            add_point(x - 0, y, 255 - alpha, 1);
            #elif AA_PIXELS == 2
            add_point(x - 0, y, 255 - alpha, 1, CGL_LEFT);
            add_point(x + 1, y, alpha, CGL_RIGHT);
            #endif

            //添加两个实心像素
//            add_point(x - 1, y, -1);
//            add_point(x + 2, y, -1);
            x1 += x_step;
            y1 += y_step;
        }
    }
    else{//横向扫描
        if(k == 0){
            return 0;
        }

        x_step = x1 < x2 ? AA_ONE : -AA_ONE;
        y_step = (dy << AA_BITS) / k;

        int last_x = x1;
        int last_y = INT_MIN;
        //add_point(last_x - 1, last_y, -1);

        x1 <<= AA_BITS;
        y1 <<= AA_BITS;
//        x1 += AA_ONE >> 1;//四舍五入
//        y1 += AA_ONE >> 1;

        for (i = 0; i < k; ++i){
            x = x1 >> AA_BITS;
            y = y1 >> AA_BITS;
            alpha = (y1>>8) & AA_MASK;
            #if AA_PIXELS == 1
            if(last_y != y){
                add_point(x, y, 255 - alpha, 1);
                last_y = y;
            }
            else{
                add_point(x, y, 255 - alpha, 0);
            }
            #elif AA_PIXELS == 2
            //竖向两个像素
            //add_point(x, y, 255 - alpha);
            if(last_y != y){
                add_point(x, y, 255 - alpha, 1, CGL_UP);
                last_y = y;
            }
            else{
                add_point(x, y, 255 - alpha, 0, CGL_UP);
            }
            add_point(x, y + 1, alpha, CGL_DOWN);
            #endif

            x1 += x_step;
            y1 += y_step;
        }
    }

    return k;
}

#else

//三个像素做AA，图像会大一圈
int scanline::smooth_line(int x1, int y1, int x2, int y2)
{
    using namespace std;

    const static int AA_BITS = 16;
    const static int AA_ONE  = 1 << AA_BITS;
    const static int AA_MASK = 0xFF;

    int dx = x2 - x1;
    int dy = y2 - y1;

    int x_step, y_step;
    int k = abs(dx);
    int i;
    int x, y;
    int alpha;

    if(k < abs(dy)){//竖向扫描
        k = abs(dy);
        x_step = (dx << AA_BITS) / k;
        y_step = y1 < y2 ? AA_ONE : -AA_ONE;

        x1 <<= AA_BITS;
        y1 <<= AA_BITS;
//        x1 += AA_ONE >> 1;//四舍五入
//        y1 += AA_ONE >> 1;
//        x1 -= AA_ONE >> 1;

        for (i = 0; i < k; ++i){
            x = x1 >> AA_BITS;
            y = y1 >> AA_BITS;
            alpha = (x1>>8) & AA_MASK;
            //横向三个像素
            add_point(x - 1, y, 255 - alpha);
            add_point(x - 0, y, 255, CGL_TRUE);
            add_point(x + 1, y, alpha);

            //添加两个实心像素
//            add_point(x - 2, y, -1);
//            add_point(x + 2, y, -1);
            x1 += x_step;
            y1 += y_step;
        }
    }
    else{//横向扫描
        if(k == 0){
            return 0;
        }

        x_step = x1 < x2 ? AA_ONE : -AA_ONE;
        y_step = (dy << AA_BITS) / k;

        int last_x = x1;
        int last_y = INT_MIN;
//        add_point(last_x - 1, last_y, -1);

        x1 <<= AA_BITS;
        y1 <<= AA_BITS;
//        x1 += AA_ONE >> 1;//四舍五入
//        y1 += AA_ONE >> 1;

        for (i = 0; i < k; ++i){
            x = x1 >> AA_BITS;
            y = y1 >> AA_BITS;
            alpha = (y1>>8) & AA_MASK;
            //竖向三个像素
            add_point(x, y - 1, 255 - alpha);
            if(last_y != y){
                add_point(x, y, alpha, CGL_TRUE);
                last_y = y;
            }
            else{
                add_point(x, y + 0, 255);
            }
            //add_point(x, y + 0, 255, CGL_TRUE);
            add_point(x, y + 1, alpha);

            //添加两个实心像素
            if(y != last_y){
//                add_point(x, last_y, -1);
//                add_point(x - 1, y, -1);
//                last_x = x;
//                last_y = y;
            }

            x1 += x_step;
            y1 += y_step;
        }
    }

    return k;
}

#endif

int scanline::smooth_scan(int x1, int y1, int x2, int y2)
{
    return this->smooth_line(x1, y1, x2, y2);
}

int scanline::smooth_scan(float x1, float y1, float x2, float y2)
{
    return this->smooth_line(round(x1), round(y1), round(x2), round(y2));
}

}//end namespace graphics
}//end namespace cgl

#endif //SCANLINE_HPP_20200327061948
