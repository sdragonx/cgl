/*
 Copyright (c) 2005-2020 sdragonx (mail:sdragonx@foxmail.com)

 clipbox.hpp

 2017-02-06 14:19:44

*/
#ifndef CLIPBOX_HPP_20170206141944
#define CLIPBOX_HPP_20170206141944

#include <cgl/vec.hpp>

namespace cgl{

//inline bool clip_x(int width, int& x)
//{
//    x = math::clamp(x, 0, width);
//}

//
// ����������������ֻ�Ǽ���Ŀ��ͼ����Ч���򣿣���
//

//�жϾ��������Ƿ���Ч
inline bool cliprect(vec4i& dst, int dw, int dh, vec4i& src, int sw, int sh)
{
    //src = src & vec4i(0, 0, sw, sh);
    src.offset(dst.x, dst.y);
    //dst = dst & vec4i(0, 0, dw, dh);
    dst = dst & src;
    return dst.reversed();
}

//left = top = right = bottom = 0 ����Ҫ����
//����false��������Ч
bool cliprect(int x, int y, int width, int height, int& left, int& top, int& right, int& bottom)
{
    //�Ƿ���Ҫ����
    if(left == 0 && top == 0 && right == 0 && bottom == 0){
        left   = 0;
        top    = 0;
        right  = width;
        bottom = height;
        return true;
    }

    //�Ƿ���Ч����
    if((right - left == 0) || (bottom - top == 0)){
        return false;
    }

    //��������
    if(right < left){
        std::swap(left, right);
    }

    if(bottom < top){
        std::swap(top, bottom);
    }

    //��Ч����
    if(left >= width || top >= height || right <=0 || bottom <= 0){
        return false;
    }

    left   = std::max(left,   0);
    top    = std::max(top,    0);
    right  = std::min(right,  width);
    bottom = std::min(bottom, height);

    return true;
}

//
// cliprect
//
// �����ŵ�ͼƬ����
//
// image    ͼƬ����
// x        Ŀ�����
// y
// width
// height
// sx, sy   ԴͼƬ��ʼλ��
// sw, sh   ԴͼƬ��С
//
// ����ֵ   ��ͼ������Ч����true�����򷵻�false
//
template<typename T>
bool cliprect(const T& image, int& x, int& y, int& width, int& height, int& sx, int &sy, int sw, int sh)
{
    if(x < 0){
        sx += -x;
        if(sx >= sw){
            return false;
        }
        width += x;
        if(width <= 0){
            return false;
        }
        x = 0;
    }

    if(x + width >= (int)image.width()){
        width = image.width() - x;
    }

    if(y < 0){
        sy += -y;
        if(sy >= sh){
            return false;
        }
        height += y;
        if(height <= 0){
            return false;
        }
        y = 0;
    }

    if(y + height >= (int)image.height()){
        height = image.height() - y;
    }

    width = std::min(width, int(sw - sx));
    height = std::min(height, int(sh - sy));

    return true;
}

//
// scaled_cliprect ���ż���
//
// dest     Ŀ����ƾ��Ρ����ú�Χ��[0, dw]-[0, dh]֮��
// dw, dh   Ŀ��ͼ���С
// source   Դͼ����Σ���ʼ[0, 0]-[sw, sh]��ʹ��float�����������ɱ��⾫����ʧ
// sw, sh   Դͼ���С
//
// ����ֵ   ��ͼ������Ч����true�����򷵻�false
//
bool scaled_cliprect(vec4i& dest, int dw, int dh, vec4f& source, int sw, int sh)
{
    if(dest.x >= dw || dest.y >= dh || dest.width == 0 || dest.height == 0){
        return false;
    }

    //assert(dw == 0 || dh == 0 || sw == 0 || sh == 0)

    //source = vec4i(0, 0, sw, sh);

    int org_w = dest.width;
    int org_h = dest.height;
    float n;

    if(dest.x < 0){
        n = -float(dest.x) * sw / org_w;
        source.x += n;
        source.width -= round(n);
        dest.width += dest.x;
        dest.x = 0;
    }

    if(dest.y < 0){
        n = -float(dest.y) * sh / org_h;
        source.y += n;
        source.height -= round(n);
        dest.height += dest.y;
        dest.y = 0;
    }

    n = dest.get_right();
    if(n >= dw){
        n -= dw;
        source.width -= n * sw / org_w;
        dest.width -= round(n);
    }

    n = dest.get_bottom();
    if(n >= dh){
        n -= dh;
        source.height -= n * sh / org_h;
        dest.height -= round(n);
    }

    return true;
}

}//end namespace cgl

#endif //CLIPBOX_HPP_20170206141944
