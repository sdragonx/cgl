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
// 下面这两个函数，只是剪裁目标图像有效区域？？？
//

//判断矩形区域是否有效
inline bool cliprect(vec4i& dst, int dw, int dh, vec4i& src, int sw, int sh)
{
    //src = src & vec4i(0, 0, sw, sh);
    src.offset(dst.x, dst.y);
    //dst = dst & vec4i(0, 0, dw, dh);
    dst = dst & src;
    return dst.reversed();
}

//left = top = right = bottom = 0 不需要剪裁
//返回false，区域无效
bool cliprect(int x, int y, int width, int height, int& left, int& top, int& right, int& bottom)
{
    //是否需要剪裁
    if(left == 0 && top == 0 && right == 0 && bottom == 0){
        left   = 0;
        top    = 0;
        right  = width;
        bottom = height;
        return true;
    }

    //是否无效矩形
    if((right - left == 0) || (bottom - top == 0)){
        return false;
    }

    //修正矩形
    if(right < left){
        std::swap(left, right);
    }

    if(bottom < top){
        std::swap(top, bottom);
    }

    //无效矩形
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
// 非缩放的图片剪裁
//
// image    图片类型
// x        目标矩形
// y
// width
// height
// sx, sy   源图片起始位置
// sw, sh   源图片大小
//
// 返回值   绘图区域有效返回true，否则返回false
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
// scaled_cliprect 缩放剪裁
//
// dest     目标绘制矩形。剪裁后范围在[0, dw]-[0, dh]之内
// dw, dh   目标图像大小
// source   源图像矩形，初始[0, 0]-[sw, sh]。使用float，定点数不可避免精度损失
// sw, sh   源图像大小
//
// 返回值   绘图区域有效返回true，否则返回false
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
