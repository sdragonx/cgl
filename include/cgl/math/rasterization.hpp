/*
 Copyright (c) 2005-2020 sdragonx (mail:sdragonx@foxmail.com)

 rasterization.hpp

 2019-11-18 19:58:17

 rasterizer

 ��դ��

*/
#ifndef RASTERIZATION_HPP_20191118195817
#define RASTERIZATION_HPP_20191118195817

#include <cgl/public.h>
#include <cgl/vec.hpp>
#include <cgl/math/rasterization.hpp>

namespace cgl{
namespace math{


//---------------------------------------------------------------------------
//���ֱ������ĵ�
//���ݳ��ȣ�max(abs(x2-x1), abs(y2-y1)) + 1
//Bresenham's line algorithm(����ɭ��ķֱ���㷨)
//int init_lineBLA(int& x1, int& y1, int x2, int y2, int bits, int& xstep, int& ystep)

int line_points(vec2i *ls, int x1, int y1, int x2, int y2)
{
    using namespace std;

    const static int bits = 16;
    const static int one = 1 << bits;

    vec2i point;
    int i;

    int dx = x2 - x1;
    int dy = y2 - y1;
    int x_step, y_step;
    int k = abs(dx);

    if(k < abs(dy)){
        k = abs(dy);
        x_step = (dx << bits) / k;
        y_step = y1 < y2 ? one : -one;
    }
    else{
        if(k == 0){
            return 0;
        }

        x_step = x1 < x2 ? one : -one;
        y_step = (dy << bits) / k;
    }

    memset(ls, 0, sizeof(vec2i) * (k + 1));

    x1 <<= bits;
    y1 <<= bits;
    x1 += one >> 1;//��������
    y1 += one >> 1;

    for (i = 0; i < k; ++i){
        point.x = x1 >> bits;
        point.y = y1 >> bits;
        ls[i] = point;
        x1 += x_step;
        y1 += y_step;
    }

    //���ѹ��p2
    ls[k].x = x2;
    ls[k].y = y2;

    return k;
}

int line_points(vec2i *ls, const vec2i& p1, const vec2i& p2)
{
    return line_points(ls, p1.x, p1.y, p2.x, p2.y);
}

int line_points(std::vector<vec2i>& ls, int x1, int y1, int x2, int y2)
{
    using namespace std;
    int k = std::max(abs(x2 - x1), abs(y2 - y1));
    ++k;
    ls.resize(k);
    return line_points(&ls[0], x1, y1, x2, y2);
}

int line_points(std::vector<vec2i>& ls, const vec2i& p1, const vec2i& p2)
{
    return line_points(ls, p1.x, p1.y, p2.x, p2.y);
}

//
// set_point(context, x, y)
//

//default T == std::vector<vec2i>
template<typename T, typename CTX>
void set_point(CTX& context, int x, int y, T user)
{
    context.push_back(vec2i(x, y));
}

template<typename T, typename CTX>
inline void plot_point4(CTX& context, int cx, int cy, int x, int y, T user)
{
    math::set_point<T, CTX>(context, cx + x, cy + y, user);
    math::set_point<T, CTX>(context, cx + x, cy - y, user);
    math::set_point<T, CTX>(context, cx - x, cy + y, user);
    math::set_point<T, CTX>(context, cx - x, cy - y, user);
}

template<typename T, typename CTX>
inline void plot_point8(CTX& context, int cx, int cy, int x, int y, T user)
{
    math::set_point<T, CTX>(context, cx +  x, cy +  y, user);
    math::set_point<T, CTX>(context, cx +  y, cy +  x, user);    //����y=x�Գ�
    math::set_point<T, CTX>(context, cx +  y, cy + -x, user);
    math::set_point<T, CTX>(context, cx +  x, cy + -y, user);    //����x=0�Գ�
    math::set_point<T, CTX>(context, cx + -x, cy + -y, user);    //����ԭ��Գ�
    math::set_point<T, CTX>(context, cx + -y, cy + -x, user);
    math::set_point<T, CTX>(context, cx + -y, cy +  x, user);
    math::set_point<T, CTX>(context, cx + -x, cy +  y, user);    //����y=0�Գ�
}

//
// set_point(context, x, y, alpha, user)
//

//default T == std::vector<vec3i>
template<typename T, typename CTX>
void set_point(CTX& context, int x, int y, int alpha, T user)
{
    context.push_back(vec3i(x, y, alpha));
}

template<typename T, typename CTX>
inline void plot_point4(CTX& context, int cx, int cy, int x, int y, int alpha, T user)
{
    math::set_point<T, CTX>(context, cx + x, cy + y, alpha, user);
    math::set_point<T, CTX>(context, cx + x, cy - y, alpha, user);
    math::set_point<T, CTX>(context, cx - x, cy + y, alpha, user);
    math::set_point<T, CTX>(context, cx - x, cy - y, alpha, user);
}

template<typename T, typename CTX>
inline void plot_point8(CTX& context, int cx, int cy, int x, int y, int alpha, T user)
{
    math::set_point<T, CTX>(context, cx +  x, cy +  y, alpha, user);
    math::set_point<T, CTX>(context, cx +  y, cy +  x, alpha, user);    //����y=x�Գ�
    math::set_point<T, CTX>(context, cx +  y, cy + -x, alpha, user);
    math::set_point<T, CTX>(context, cx +  x, cy + -y, alpha, user);    //����x=0�Գ�
    math::set_point<T, CTX>(context, cx + -x, cy + -y, alpha, user);    //����ԭ��Գ�
    math::set_point<T, CTX>(context, cx + -y, cy + -x, alpha, user);
    math::set_point<T, CTX>(context, cx + -y, cy +  x, alpha, user);
    math::set_point<T, CTX>(context, cx + -x, cy +  y, alpha, user);    //����y=0�Գ�
}

//
// dda_points(T, x1, y1, x2, y2, finalpos)
//

template<typename T, typename CTX>
int solid_line(CTX& context, int x1, int y1, int x2, int y2, T user, bool finalpos = true)
{
    using namespace std;

    const static int DDA_BITS = 16;
    const static int DDA_ONE  = 1 << DDA_BITS;

    int dx = x2 - x1;
    int dy = y2 - y1;
    int x_step, y_step;
    int k = abs(dx);

    if(k < abs(dy)){
        k = abs(dy);
        x_step = (dx << DDA_BITS) / k;
        y_step = y1 < y2 ? DDA_ONE : -DDA_ONE;
    }
    else{
        if(k == 0){
            return 0;
        }
        x_step = x1 < x2 ? DDA_ONE : -DDA_ONE;
        y_step = (dy << DDA_BITS) / k;
    }

    x1 <<= DDA_BITS;
    y1 <<= DDA_BITS;
    x1 += DDA_ONE >> 1;//�������룬�ƶ��������
    y1 += DDA_ONE >> 1;

    int i;
    int x, y;
    for (i = 0; i < k; ++i){
        x = x1 >> DDA_BITS;
        y = y1 >> DDA_BITS;
        set_point<T>(context, x, y, user);
        x1 += x_step;
        y1 += y_step;
    }

    //���ѹ��p2
    if(finalpos){
        set_point<T>(context, x2, y2, user);
    }

    return k;
}

//
// smooth_line(T, x1, y1, x2, y2, color)
//

template<typename T, typename CTX>
int smooth_line_impl(CTX& context, int x1, int y1, int x2, int y2, T user)
{
    using namespace std;

    const static int AA_BITS = 16;
    const static int AA_ONE  = 1 << AA_BITS;
    const static int AA_MASK = 0xFF;

    int dx = x2 - x1;
    int dy = y2 - y1;

    /*
    if(dx == 0){
        if(dy != 0){
            //pixelbuf.vline(x1, y1, y2, color);
        }
        return 0;
    }
    else if(dy == 0){
        //pixelbuf.hline(x1, x2, y1, color);
        return 0;
    }
    */

    int x_step, y_step;
    int k = abs(dx);
    int i;
    int x, y;
    int alpha;

    if(k < abs(dy)){
        k = abs(dy) >> AA_BITS;
        x_step = dx / k;
        y_step = y1 < y2 ? AA_ONE : -AA_ONE;

//        x1 += AA_ONE >> 1;//��������
//        y1 += AA_ONE >> 1;

        for (i = 0; i < k; ++i){
            x = x1 >> AA_BITS;
            y = y1 >> AA_BITS;
            alpha = (x1 >> 8) & AA_MASK;
//            alpha = x1 & AA_MASK;
            set_point<T, CTX>(context, x, y, 255 - alpha, user);
            set_point<T, CTX>(context, x + 1, y, alpha, user);
            x1 += x_step;
            y1 += y_step;
        }
    }
    else{
        //б��û��AAЧ��

        k = abs(dx) >> AA_BITS;
        if(k == 0){
            return 0;
        }

        x_step = x1 < x2 ? AA_ONE : -AA_ONE;
        y_step = dy / k;

//        x1 += AA_ONE >> 1;//��������
//        y1 += AA_ONE >> 1;

        for (i = 0; i < k; ++i){
            x = x1 >> AA_BITS;
            y = y1 >> AA_BITS;
            alpha = (y1 >> 8) & AA_MASK;
//            alpha = y1 & AA_MASK;
            set_point<T, CTX>(context, x, y, 255 - alpha, user);
            set_point<T, CTX>(context, x, y + 1, alpha, user);
            x1 += x_step;
            y1 += y_step;
        }
    }

    set_point<T, CTX>(context, x2 >> AA_BITS, y2 >> AA_BITS, 255, user);

    return k;
}

template<typename T, typename CTX>
int smooth_line(CTX& context, int x1, int y1, int x2, int y2, T user)
{
    const int AA_BITS = 16;
    return smooth_line_impl<T, CTX>(context, x1<<AA_BITS, y1<<AA_BITS, x2<<AA_BITS, y2<<AA_BITS, user);
}

template<typename T, typename CTX>
int smooth_line(CTX& context, float x1, float y1, float x2, float y2, T user)
{
    float scale = 65535.0f;
    return smooth_line_impl<T, CTX>(context,
        round(x1 * scale), round(y1 * scale), round(x2 * scale), round(y2 * scale), user);
}

//---------------------------------------------------------------------------
namespace internal{

inline void plot_point4(std::vector<vec2i>& ls, int ox, int oy, int x, int y)
{
    ls.push_back(vec2i(ox + x, oy + y));
    ls.push_back(vec2i(ox - x, oy + y));
    ls.push_back(vec2i(ox - x, oy - y));
    ls.push_back(vec2i(ox + x, oy - y));
}

//������Բ��vec3.z = alpha
inline void plot_point(std::vector<vec3i>& ls, int x, int y, float alpha)
{
    ls.push_back(vec3i(x, y, round(alpha * 255.0f)));
}

//���Ʒ�����Բ
inline void plot_point4(std::vector<vec3i>& ls, int cx, int cy, int x, int y, float alpha)
{
    plot_point(ls, cx +  x, cy +  y, alpha);
    plot_point(ls, cx +  y, cy + -x, alpha);
    plot_point(ls, cx + -y, cy + -x, alpha);
    plot_point(ls, cx + -y, cy +  x, alpha);
}

//���Ʒ�����Բ
inline void plot_points(std::vector<vec3i>& ls, int cx, int cy, int x, int y, float alpha)
{
    plot_point(ls, cx +  x, cy +  y, alpha);
    plot_point(ls, cx +  y, cy +  x, alpha);    //����y=x�Գ�
    plot_point(ls, cx +  y, cy + -x, alpha);
    plot_point(ls, cx +  x, cy + -y, alpha);    //����x=0�Գ�
    plot_point(ls, cx + -x, cy + -y, alpha);    //����ԭ��Գ�
    plot_point(ls, cx + -y, cy + -x, alpha);
    plot_point(ls, cx + -y, cy +  x, alpha);
    plot_point(ls, cx + -x, cy +  y, alpha);    //����y=0�Գ�
}

}//internal

void smooth_circle(std::vector<vec3i>& ls, int cx, int cy, int r)
{
    int y = r;
    float e;
    //��1/8Բ
    for(int x=0; x<=y; x++){
        //���������㵽����Բ�ľ��룬��Ϊ���Ȳ���
        e = y - sqrt((float)r * r - (x + 1) * (x + 1));

        if(e >= 1.0f){
            --e;
            --y;
        }
        internal::plot_points(ls, cx, cy, x, y, 1.0f - e);
        internal::plot_points(ls, cx, cy, x, y - 1, e);
    }
}

//����ݻ�Բ
template<typename T, typename CTX>
void smooth_circle(CTX& context, int cx, int cy, int r, T user)
{
    if(r == 0)return ;
    int x = 0;
    int y = r;
    //int p = 1.25 - r;
    int p = 3 - 2 * r;
    int a = (r * 4 * M_SQRT2);
    int n = 0;

    math::set_point<T, CTX>(context, cx, cy + r, 128, user);
    math::set_point<T, CTX>(context, cx, cy - r, 128, user);
    math::set_point<T, CTX>(context, cx + r, cy, 128, user);
    math::set_point<T, CTX>(context, cx - r, cy, 128, user);

    math::set_point<T, CTX>(context, cx, cy + r + 1, 128, user);
    math::set_point<T, CTX>(context, cx, cy - r - 1, 128, user);
    math::set_point<T, CTX>(context, cx + r + 1, cy, 128, user);
    math::set_point<T, CTX>(context, cx - r - 1, cy, 128, user);

    while(x < y){
        if(p < 0){
            p += 4 * x + 6;
        }
        else{
            p += 4 * ( x - y ) + 10;
            n += 1 - y * 2;
            --y;
        }
        ++x;
        n += 1 + x * 2;
        int alpha = ( n + 1 + y * 2 ) * 1448 / 1024 * 255 / a;
        math::plot_point8(context, cx, cy, x, y, alpha, user);
        math::plot_point8(context, cx, cy, x, y + 1, 255 - alpha, user);
    }
}
//---------------------------------------------------------------------------
//�����Բ�ϵĵ㣬ox oyΪԲ�ģ�a bΪ��Բˮƽ�ʹ�ֱ�뾶

void ellipse_points(std::vector<vec2i>& ls, int x1, int y1, int x2, int y2)
{
    using namespace math;
    //����ģʽ������һ����С���ͻ�ʧ��
    //if(a > 1024 || b > 1024)return ;

    ls.clear();

    float a = (x2 - x1) / 2.0f;
    float b = (y2 - y1) / 2.0f;//bС��0������£��㷨��ʧЧ

    //��������
    if(b < 0){
        b = -b;
        x1 += a;
        y1 -= b;
    }else{
        x1 += a;
        y1 += b;
    }

    float aa = a * a;
    float bb = b * b;
    //float aax2 = aa * 2;
    //float bbx2 = bb * 2;

    float p;
    int x = 0;
    int y = b;

    //ellipsePlotPoints(ox, oy, x, y);
    //����
    ls.push_back(vec2i(x1, y1 + b + 0.5f));
    ls.push_back(vec2i(x1, y1 - b));

    //��һ����
    //float t = sqrt(aa + bb);
    //while(x <= (aa / t) && y <= (bb / t))

    p = bb + aa * (-b + 0.25f);
    while (bb * (x + 1) < aa * (y - 0.5))
    //int px = aa * 2;
    //int py = 0;
    //while(bb * x < aa * y)
    {
        if(p < 0){
            //p += bb * (2 * x + 3);
            //p += bb * 2 * x + bb * 3;
            //p += bb * 2 * x + bb * 2 + bb;
            //p += bbx2 * x + bbx2 + bb;
            p += bb * (x + x + 3);
        }
        else{
            //p += bb * (2 * x + 3) + aa * (2 - 2 * y);
            //p += bb * 2 * x + bb * 3 + aa * 2 - aa * 2 * y;
            p += bb * (x + x + 3) + aa * (2 - y - y);
            --y;
        }
        ++x;
        //py += bb;
        internal::plot_point4(ls, x1, y1, x, y);
    }

    //�ڶ�����

    ls.push_back(vec2i(x1 - a, y1 ));
    ls.push_back(vec2i(x1 + a + 1, y1));

    //p = bb * (x + 0.5) * (x + 0.5) + aa * (y - 1) * (y - 1) - aa * bb;
    p = bb * (0.25 * x) + aa * (1 - (y * 2));
    while(y > 1)
    {
        if(p < 0){
            ++x;
            p += bb * (x + x + 2) + aa * (3 - y - y);
        }
        else{
            p += aa * (3 - y - y);
        }
        --y;
        internal::plot_point4(ls, x1, y1, x, y);
    }
}


//---------------------------------------------------------------------------
//��������

int hexagon_points(std::vector<vec2i>& ls, int x, int y, int size)
{
    ls.clear();

    if(size < 1){
        ls.push_back(vec2i(x, y));
        return 1;
    }

    int half = size / 2;
    int i, j;
    int n;

    //Ԥ�ȷ���ռ�
    ls.reserve(size * 6);

    //��������
    for(i=-half; i<=half; ++i){
        ls.push_back(vec2i(x - size, y + i));
        ls.push_back(vec2i(x + size, y + i));
    }

    //������������
    ls.push_back(vec2i(x, y + size));
    ls.push_back(vec2i(x, y - size));

    //�ĸ�б��
    for(i=1, j=0; i<size; ++i, ++j){
        n = j >> 1;
        n -= size;
        ++n;
        internal::plot_point4(ls, x, y, i, n);
    }

    return ls.size();
}

int hexagon_points_htz(std::vector<vec2i>& ls, int x, int y, int size)
{
    ls.clear();

    if(size < 1){
        ls.push_back(vec2i(x, y));
        return 1;
    }

    int half = size / 2;
    int i, j;
    int n;

    //Ԥ�ȷ���ռ�
    ls.reserve(size * 6);

    //��������
    for(i=-half; i<=half; ++i){
        ls.push_back(vec2i(x + i, y - size));
        ls.push_back(vec2i(x + i, y + size));
    }

    //������������
    ls.push_back(vec2i(x + size, y));
    ls.push_back(vec2i(x - size, y));

    //�ĸ�б��
    for(i=1, j=0; i<size; ++i, ++j){
        n = j >> 1;
        n -= size;
        ++n;
        internal::plot_point4(ls, x, y, n, i);
    }

    return ls.size();
}

//---------------------------------------------------------------------------
//���˱���
//�������и�˱��α߳���sin(45)*R

void draw_octagon(std::vector<vec2i>& ls, int x, int y, int size)
{
    int side = size * 0.70710678f + 0.5f;
    int n;

    ls.clear();

    //����
    ls.push_back(vec2i(x - 0, y - size));
    ls.push_back(vec2i(x - 0, y + size));
    ls.push_back(vec2i(x - size, y - 0));
    ls.push_back(vec2i(x + size, y - 0));

    //б�ߣ����ظ�����
    n = side - 1;
    internal::plot_point4(ls, x, y, n, n);

    for(int i = 1; i < side; ++i){
        //22.5�� 0.3826834323650897717284599840304
        n = i * 1696 / 4096;
        //n = i * 424 / 1024;//424Ҳ�����
        n -= size;
        ++n;
        internal::plot_point4(ls, x, y, i, n);
        internal::plot_point4(ls, x, y, n, i);
    }
}

//���˱���(ƽ��)
//a = sqrt(2) / 2
//a = sqrt(a) = 0.84089641525371454303112547623321
//��������α߳� s = a + sqrt(2) + a
//sqrt(2) / s = 0.45678638313705510397806219881721
void draw_octagon_hzt(std::vector<vec2i> &ls, int x, int y, int size, bool regular = false)
{
    //int half = size * 1870 / 4096;//0.45678638313705510397806219881721
    //0.45Ч���ã�����϶������
    int half = regular ?
        size * 1870 / 4096 :
        size / 2;//�������

    //���ߡ�����
    for(int i=-half; i<half + 1; ++i){
        ls.push_back(vec2i(x - i, y - size));
        ls.push_back(vec2i(x - i, y + size));
        ls.push_back(vec2i(x - size, y - i));
        ls.push_back(vec2i(x + size, y - i));
    }

    int n;
    for(int i=1; i<size - half; ++i){
        n = half + i;
        ls.push_back(vec2i(x - size + i, y - n));
        ls.push_back(vec2i(x + size - i, y - n));
        ls.push_back(vec2i(x - size + i, y + n));
        ls.push_back(vec2i(x + size - i, y + n));
    }

    //����϶
    if(!regular && (size + 1) & 1){
        half -= 1;
        for(int i=1; i<size - half; ++i){
            n = half + i + 0;
            ls.push_back(vec2i(x - size + i, y - n));
            ls.push_back(vec2i(x + size - i, y - n));
            ls.push_back(vec2i(x - size + i, y + n));
            ls.push_back(vec2i(x + size - i, y + n));
        }
    }
}

}//end namespace math
}//end namespace cgl

#endif //RASTERIZATION_HPP_20191118195817


/*
//���ֱ������ĵ�
int line_points(std::vector<vec2i>& ls, int x1, int y1, int x2, int y2)
{
    using namespace std;

    vec2i point;
    int wx = x2-x1;
    int wy = y2-y1;
    int offset;
    int dstp;
    int step;

    ls.clear();
    if(abs(wx) < abs(wy))
    {
        step = wy < 0 ? -1 : 1;
        if(wx){
            offset = ( wx << 16 ) / wy;
            offset *= step;
            dstp = x1<<16;
            dstp += 32767;//��������
            for(int i=y1; i!=y2; i+=step){
                point.x = dstp>>16;
                point.y = i;
                ls.push_back(point);
                dstp += offset;
            }
        }
        else{
            for(int i=y1; i!=y2; i+=step){
                point.x = x1;
                point.y = i;
                ls.push_back(point);
            }
        }
    }
    else //if(abs(wy) < abs(wx))
    {
        step = wx < 0 ? -1 : 1;
        if(wy){
            offset = ( wy << 16 ) / wx;
            offset *= step;
            dstp = y1<<16;
            dstp += 32767;
            for(int i=x1; i!=x2; i+=step){
                point.x = i;
                point.y = dstp>>16;
                ls.push_back(point);
                dstp += offset;
            }
        }
        else{
            for(int i=x1; i!=x2; i+=step){
                point.x = i;
                point.y = y1;
                ls.push_back(point);
            }
        }
    }

    point.x = x2;
    point.y = y2;
    ls.push_back(point);

    return ls.size();
}
*/

/*

int smooth_line(int x1, int y1, int x2, int y2)
{
    using namespace std;

    const static int bits = 16;
    const static int one = 1 << bits;

    vec2i point;
    int i;

    int dx = x2 - x1;
    int dy = y2 - y1;
    int x_step, y_step;
    int k = abs(dx);
    int alpha;

    if(k < abs(dy)){
        k = abs(dy);
        x_step = (dx << bits) / k;
        y_step = y1 < y2 ? one : -one;

        x1 <<= bits;
        y1 <<= bits;
        x1 += one >> 1;//��������
        y1 += one >> 1;

        for (i = 0; i < k; ++i){
            point.x = x1 >> bits;
            point.y = y1 >> bits;

            alpha = x1 & 0xFFFF;
//            alpha *= 255;
//            alpha >>= bits;
            alpha >>= 8;

            draw_pixel(Form1->Canvas, point.x, point.y, 255 - alpha);
            draw_pixel(Form1->Canvas, point.x + 1, point.y, alpha);

            x1 += x_step;
            y1 += y_step;
        }
    }
    else{
        if(k == 0){
            return 0;
        }

        x_step = x1 < x2 ? one : -one;
        y_step = (dy << bits) / k;

        x1 <<= bits;
        y1 <<= bits;
        x1 += one >> 1;//��������
        y1 += one >> 1;

        for (i = 0; i < k; ++i){
            point.x = x1 >> bits;
            point.y = y1 >> bits;

            alpha = y1 & 0xFFFF;
            alpha >>= 8;

            draw_pixel(Form1->Canvas, point.x, point.y, 255 - alpha);
            draw_pixel(Form1->Canvas, point.x, point.y + 1, alpha);

            x1 += x_step;
            y1 += y_step;
        }
    }

    return k;
}

//δ���
void smooth_ellipse(std::vector<vec3i>& ls, int x1, int y1, int x2, int y2)
{
    using namespace math;
    //����ģʽ������һ����С���ͻ�ʧ��
    //if(a > 1024 || b > 1024)return ;

    ls.clear();

    float a = (x2 - x1) / 2.0f;
    float b = (y2 - y1) / 2.0f;//bС��0������£��㷨��ʧЧ

    //��������
    if(b < 0){
        b = -b;
        x1 += a;
        y1 -= b;
    }else{
        x1 += a;
        y1 += b;
    }

    float aa = a * a;
    float bb = b * b;
    //float aax2 = aa * 2;
    //float bbx2 = bb * 2;
    float alpha = 1;

    float p;
    int x = 0;
    int y = b;

    //ellipsePlotPoints(ox, oy, x, y);
    //����
    ls.push_back(vec2i(x1, y1 + b + 0.5f));
    ls.push_back(vec2i(x1, y1 - b));

    //��һ����
    //float t = sqrt(aa + bb);
    //while(x <= (aa / t) && y <= (bb / t))

    p = bb + aa * (-b + 0.25f);
    while (bb * (x + 1) < aa * (y - 0.5))
    //int px = aa * 2;
    //int py = 0;
    //while(bb * x < aa * y)
    {
        if(p < 0){
            //p += bb * (2 * x + 3);
            //p += bb * 2 * x + bb * 3;
            //p += bb * 2 * x + bb * 2 + bb;
            //p += bbx2 * x + bbx2 + bb;
            p += bb * (x + x + 3);
        }
        else{
            //p += bb * (2 * x + 3) + aa * (2 - 2 * y);
            //p += bb * 2 * x + bb * 3 + aa * 2 - aa * 2 * y;
            p += bb * (x + x + 3) + aa * (2 - y - y);
            internal::plot_point(ls, x1 + x + 1, y1 + y, x / a);
            --y;
            internal::plot_point(ls, x1 + x, y1 + y, x / a);
        }
        ++x;
        //py += bb;
        //internal::plot_point4(ls, x1, y1, x, y, alpha);
        internal::plot_point(ls, x1 + x, y1 + y, 1 - x / a);
    }

    //�ڶ�����

    ls.push_back(vec2i(x1 - a, y1 ));
    ls.push_back(vec2i(x1 + a + 1, y1));

    //p = bb * (x + 0.5) * (x + 0.5) + aa * (y - 1) * (y - 1) - aa * bb;
    p = bb * (0.25 * x) + aa * (1 - (y * 2));
    while(y > 1)
    {
        if(p < 0){
            ++x;
            p += bb * (x + x + 2) + aa * (3 - y - y);
        }
        else{
            p += aa * (3 - y - y);
        }
        --y;
        //internal::plot_point4(ls, x1, y1, x, y, alpha);
        //internal::plot_point(ls, x1 + x, y1 + y, alpha);
    }
}

//Bresenham��Բ��
void draw_circle(pixelbuffer<uint32_t>& pixelbuf, int cx, int cy, int r, uint32_t c)
{
    int x = 0;
    int y = r;
    //int p = 1.25 - r;
    int p = 3 - 2 * r;
    math::plot_point8(pixelbuf, cx, cy, x, y, 0u);
    while(x < y){
        if(p < 0){
            p += 4 * x + 6;
        }
        else{
            p += 4 * ( x - y ) + 10;
            --y;
        }
        ++x;
        math::plot_point8(pixelbuf, cx, cy, x, y, c);
    }
}

//������Բ�����������عսǴ���������
void draw_circle(pixelbuffer<uint32_t>& pixelbuf, int cx, int cy, int r, uint32_t c)
{
    int x = 0; //x==1��Ҫ�����е�
    int y = r;
    int p = 0;
    while(x <= y){
        plot_point8(pixelbuf, cx, cy, x, y, 0x1f000000u);
        if(p <= 0){
            p += 2 * x + 1;
            ++x;
        }
        else{
            p -= 2 * y + 1;
            --y;
        }
    }
}

//���ٻ�Բ����û�г˷�����
void draw_circle(pixelbuffer<uint32_t>& pixelbuf, int cx , int cy , int r, uint32_t c)
{
    int x = 0;
    int y = r;
    int d = -r / 2;

    //CirclePlot(cx , cy , x , y);
    if(r & 1 == 0){
        while(x < y){
            x++;
            if(d < 0){
                d += x;
            }
            else{
                y--;
                d += x - y;
            }
            plot_point8(pixelbuf, cx, cy, x, y, 0x1f000000u);
        }
    }
    else{
        while(x < y){
            x++;
            if(d < 0){
              d += x + 1;
            }
            else{
              y--;
              d += x - y + 1;
            }
            plot_point8(pixelbuf, cx, cy, x, y, 0x1f000000u);
        }
    }
}

//��������Բ��alpha����ԭ��û������
void smooth_circle1(pixelbuffer<uint32_t>& pixelbuf, int cx, int cy, int r, uint32_t color)
{
    if(r == 0)return ;
	int x = 0;
	int y = r;
	int p = 1 - r;

	int dE  = 3;
	int dSE = -(y * 2)+5;
	int dS  = 0;
	double a = 1.0 / ((r << 1) * 1.4142);
    print(1 / a);

	//put_pixel(pixelbuf, cx, cy, x, y, color, 0.20);
	//put_pixel(pixelbuf, cx, cy, x, y - 1, color, 0.15);

    while(x < y){
		if(p < 0){
			p += dE;
			dE  += 2;
			dSE += 2;
		}
        else{
            p += dSE;

			dS += 1 - y * 2;
			dE += 2;
			dSE+= 4;
			--y;
		}
		dS += 1 + x * 2;
		++x;

//		put_pixel(pixelbuf, cx, cy, x, y, color, 1 - a * abs(dS));
		put_pixel(pixelbuf, cx, cy, x, y + 1, color, 1 - a * (dS+(y<<1)+1) );
//        put_pixel(pixelbuf, cx, cy, x, y, color, 1 - a * (dS-(y<<1)+1) );
		put_pixel(pixelbuf, cx, cy, x, y - 1, color, 1 + a * (dS-(y<<1)+1) );
	}
}


void draw_ellipse(pixelbuffer<uint32_t>& pixelbuf, int cx, int cy, int rx, int ry)
{
    int p; //���۲���

    int aa = rx * rx;
    int bb = ry * ry;

    //x��0���ӵ�45��λ��
    //ʹ��x��������Сyֵ��ͨ��б��Ϊ������õ�
    int minYofDeltaX = round(bb / sqrt(aa + bb));

    int x = 0;
    int y = ry;

    // ��������������߲����ĳ�ʼֵ
    p =  bb - aa * ry + aa / 4;

    while(minYofDeltaX < y){
        if(p < 0){
            //���p<0, ������һ����(x+1,y), ���Ҽ���
            //p += rx*rx*(3+2*x);
            p += bb * (x + x + 3);
        }
        else{
            //���P>=0, ������һ����(x+1,y-1), ���Ҽ���
            //p += rx*rx*(3+2*x) - 2*r1*r1*(y-1)
            --y;
            p += bb * (3 + 2 * x) - 2 * aa * ( y - 1);
        }
        ++x;
        math::plot_point4(pixelbuf, cx, cy, x, y, 0x1f000000u);
    }

    //p = bb * (x + 0.5) * (x + 0.5) + aa * (y - 1) * (y - 1) - aa * bb;
    // d = sqb * (x * x + x) + sqa * (y * y - y) - sqa * sqb;
    p = bb * x / 4 + aa * (1 - y - y);

    while(1 < y){
        if(p < 0){
            //P<0�������,��һ��Ŀ���Ϊ(x,y-1)�����Ҽ���
            ++x;
            p += bb * (x + x + 2) + aa * (3 - y - y);
        }
        else{
            //���p>=0������£���һ��Ŀ���Ϊ(x+1,y-1)�����Ҽ���
            p += aa * (3 - y - y);
        }
        --y;
        math::plot_point4(pixelbuf, cx, cy, x, y, 0x1fff0000u);
    }
}
*/