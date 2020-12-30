/*
 Copyright (c) 2005-2020 sdragonx (mail:sdragonx@foxmail.com)

 copy_image.hpp

 2020-04-02 23:23:47

 copy_image(dest, source, 0x00FF0000)   masked_copy channel_copy

*/
#ifndef COPY_IMAGE_HPP_20200402232347
#define COPY_IMAGE_HPP_20200402232347

#include <cgl/graphics/clipbox.hpp>
#include <cgl/graphics/pixel/pixelbuffer.hpp>
#include <cgl/graphics/pixel/pixel_traits.hpp>
#include <cgl/math/matrix/mat2.hpp>

namespace cgl{
namespace graphics{

//
// bitblt
//
template<typename T>
int bitblt(pixelbuffer<T>& dest,
    int dx, int dy, int width, int height,
    const pixelbuffer<T>& source,
    int sx, int sy,
    int param = CGL_COPY)
{
    if(!cliprect(dest, dx, dy, width, height, sx, sy, source.width(), source.height())){
        return -1;
    }

    for(int y=0; y<height; ++y){
        memcpy(dest[dy + y] + dx, source[sy + y] + sx, width * sizeof(T));
//        for(int x=0; x<width; ++x){
//            dest[dy + y][dx + x] = source[sy + y][sx + x];
//        }
    }

    return 0;
}


//
// copy_image 通用图像复制函数，带缩放
//
// dest         目标图像
// x            绘图区域
// y
// width
// height
// source       源图像
// pixeltraits  取色器
//
// 返回值 成功返回0，失败返回错误码
//
template<typename T>
int copy_image(pixelbuffer<T>& dest,
    int x, int y, int width, int height,
    const pixelbuffer<T>& source,
    pixel_traits<T>::getter getter)
{
    vec4i dest_rect(x, y, width, height);
    vec4f source_rect(0, 0, source.width(), source.height());
    if(!scaled_cliprect(dest_rect, dest.width(), dest.height(),
        source_rect, source.width(), source.height())){
        return -1;
    }

    //精度有损失
    //int step_x = round(source_rect.width * PIXEL_FIXED_ONE) /  dest_rect.width;
    //int step_y = round(source_rect.height * PIXEL_FIXED_ONE) / dest_rect.height;
    int step_x = source.width() * PIXEL_FIXED_ONE /  width;
    int step_y = source.height() * PIXEL_FIXED_ONE / height;

    uint32_t sx;
    uint32_t sy = round(source_rect.y * PIXEL_FIXED_ONE);
    uint32_t sx_org = round(source_rect.x * PIXEL_FIXED_ONE);
    for(int y=dest_rect.y; y<dest_rect.get_bottom(); ++y){
        sx = sx_org;
        for(int x=dest_rect.x; x<dest_rect.get_right(); ++x){
            dest[y][x] = getter(source, sx, sy);
            sx += step_x;
        }
        sy += step_y;
    }

    return 0;
}

//
// copy_image 相较于上一个函数，增加uv变量
//

inline int fixed_uv_mod(int value, int size)
{
    size <<= PIXEL_FIXED_BITS;
    return value < 0 ? -value % size : value % size;
}

template<typename T>
int copy_image(pixelbuffer<T>& dest,
    int x, int y, int width, int height,
    const pixelbuffer<T>& source,
    float u1, float v1, float u2, float v2,
    pixel_traits<T>::getter getter)
{
    int sw = source.width() * (u2 - u1);
    int sh = source.height() * (v2 - v1);
    vec4i dest_rect(x, y, width, height);
    vec4f source_rect(u1 * source.width(), v1 * source.height(), sw, sh);
    if(!scaled_cliprect(dest_rect, dest.width(), dest.height(),
        source_rect, sw, sh)){
        return -1;
    }

    int step_x = sw * PIXEL_FIXED_ONE / width;
    int step_y = sh * PIXEL_FIXED_ONE / height;

    int sx;
    int sy = round(source_rect.y * PIXEL_FIXED_ONE);
    int sx_org = round(source_rect.x * PIXEL_FIXED_ONE);
    int u, v;
    for(int y=dest_rect.y; y<dest_rect.get_bottom(); ++y){
        sx = sx_org;
        v = fixed_uv_mod(sy, source.height());
        for(int x=dest_rect.x; x<dest_rect.get_right(); ++x){
            u = fixed_uv_mod(sx, source.width());
            dest[y][x] = getter(source, u, v);
            sx += step_x;
        }
        sy += step_y;
    }
    return 0;
}

template<typename T>
int copy_image(pixelbuffer<T>& dest,
    int x, int y, int width, int height,
    const pixelbuffer<T>& source,
    pixel_traits<T>::getter getter, pixel_traits<T>::function pfn)
{
    vec4i dest_rect(x, y, width, height);
    vec4f source_rect(0, 0, source.width(), source.height());
    if(!scaled_cliprect(dest_rect, dest.width(), dest.height(),
        source_rect, source.width(), source.height())){
        return -1;
    }

    //精度有损失
    //int step_x = round(source_rect.width * PIXEL_FIXED_ONE) /  dest_rect.width;
    //int step_y = round(source_rect.height * PIXEL_FIXED_ONE) / dest_rect.height;
    int step_x = source.width() * PIXEL_FIXED_ONE /  width;
    int step_y = source.height() * PIXEL_FIXED_ONE / height;

    uint32_t sx;
    uint32_t sy = round(source_rect.y * PIXEL_FIXED_ONE);
    uint32_t sx_org = round(source_rect.x * PIXEL_FIXED_ONE);
    for(int y=dest_rect.y; y<dest_rect.get_bottom(); ++y){
        sx = sx_org;
        for(int x=dest_rect.x; x<dest_rect.get_right(); ++x){
            dest[y][x] = pfn(dest[y][x], getter(source, sx, sy));
            sx += step_x;
        }
        sy += step_y;
    }

    return 0;
}

//
// scaled_blt 临近取值缩放图像
// stretch
int scaled_blt(pixelbuffer<uint32_t>& dest, int x, int y, int width, int height, const pixelbuffer<uint32_t>& source)
{
    return copy_image<uint32_t>(dest, x, y, width, height, source, nearest_color);
}

//
// bilinear_blt 双线性插值缩放图像
//
int bilinear_blt(pixelbuffer<uint32_t>& dest, int x, int y, int width, int height, const pixelbuffer<uint32_t>& source)
{
    return copy_image<uint32_t>(dest, x, y, width, height, source, bilinear_color);
}

//
// transform_blt 使用矩阵变换绘制图片
//
//反锯齿不会做
void transform_blt(
    pixelbuffer<uint32_t>& dest,
    const pixelbuffer<uint32_t>& source,
    const mat2f& mat,
    const mat2f& inv_mat)
{
    int sx, sy;

    fixed_mat m;
    m.assign(mat);

    box2i bounds;
    m.transform(0, 0, sx, sy);
    bounds.append(sx>>16, sy>>16);
    m.transform(source.width(), 0, sx, sy);
    bounds.append(sx>>16, sy>>16);
    m.transform(source.width(), source.height(), sx, sy);
    bounds.append(sx>>16, sy>>16);
    m.transform(0, source.height(), sx, sy);
    bounds.append(sx>>16, sy>>16);

    bounds &= box2i(0, 0, dest.width(), dest.height());

    dest.draw_rect(bounds.min_x, bounds.min_y, bounds.max_x - bounds.min_x, bounds.max_y - bounds.min_y, 0xff000000);

    m.assign(inv_mat);
    box2i source_box(0, 0, source.width() * 65536, source.height() * 65536);
    for(int y=bounds.min_y; y<bounds.max_y; ++y){
        for(int x=bounds.min_x; x<bounds.max_x; ++x){
            m.transform(x, y, sx, sy);
            if(!source_box.contains(sx, sy))continue;
            dest.set_pixel(x, y, bilinear_color(source, sx, sy));
        }
    }
}


inline int fixed_uv_mod2(int value, int size)
{
    size <<= PIXEL_FIXED_BITS;
    return value < 0 ? (value % size) + 65535 : value % size;
}

//uv有问题，不能镜像
void transform_blt(
    pixelbuffer<uint32_t>& dest,
    const pixelbuffer<uint32_t>& source,
    float u1, float v1, float u2, float v2,
    const mat2f& mat, const mat2f& inv_mat)
{
    int sx, sy;

    fixed_mat m;
    m.assign(mat);

    box2i bounds;
    m.transform(0, 0, sx, sy);
    bounds.append(sx>>16, sy>>16);
    m.transform(source.width(), 0, sx, sy);
    bounds.append(sx>>16, sy>>16);
    m.transform(source.width(), source.height(), sx, sy);
    bounds.append(sx>>16, sy>>16);
    m.transform(0, source.height(), sx, sy);
    bounds.append(sx>>16, sy>>16);

    bounds &= box2i(0, 0, dest.width(), dest.height());

    dest.draw_rect(bounds.min_x, bounds.min_y, bounds.max_x - bounds.min_x, bounds.max_y - bounds.min_y, 0xff000000);

    m.assign(inv_mat);

    int sw = u2 - u1;
    int sh = v2 - v1;

//    int step_u = abs(sw) * 65536 / source.width();
//    int step_v = abs(sh) * 65536 / source.height();

    int org_u = u1 * source.width();
    int org_v = v1 * source.height();

    box2i source_box(0, 0, source.width() * 65536, source.height() * 65536);

    for(int y=bounds.min_y; y<bounds.max_y; ++y){
        for(int x=bounds.min_x; x<bounds.max_x; ++x){
            m.transform(x, y, sx, sy);
            if(!source_box.contains(sx, sy)){
                continue;
            }
            sx = org_u + sx * sw;
            sy = org_v + sy * sh;
            sx = fixed_uv_mod2(sx, source.width());
            sy = fixed_uv_mod2(sy, source.height());
            dest.set_pixel(x, y, bilinear_color(source, sx, sy));
        }
    }
}


//
// transform_blt 使用矩阵变换绘制图片
//
//反锯齿边角有问题，在缩放的图片里面，反锯齿边界和缩放比例宽度一样
void transform_blt_aa1(
    pixelbuffer<uint32_t>& dest,
    const pixelbuffer<uint32_t>& source,
    const mat2f& mat,
    const mat2f& inv_mat)
{
    int sx, sy;

    fixed_mat m;
    m.assign(mat);

    box2i bounds;
    m.transform(0, 0, sx, sy);
    bounds.append(sx>>16, sy>>16);
    m.transform(source.width(), 0, sx, sy);
    bounds.append(sx>>16, sy>>16);
    m.transform(source.width(), source.height(), sx, sy);
    bounds.append(sx>>16, sy>>16);
    m.transform(0, source.height(), sx, sy);
    bounds.append(sx>>16, sy>>16);

    bounds &= box2i(0, 0, dest.width(), dest.height());

    //dest.draw_rect(bounds.min_x, bounds.min_y, bounds.max_x - bounds.min_x, bounds.max_y - bounds.min_y, 0xff000000);

    m.assign(inv_mat);
    box2i source_box(0, 0, source.width() * 65536, source.height() * 65536);
    uint32_t color;
    int alpha;
    for(int y=bounds.min_y; y<bounds.max_y; ++y){
        for(int x=bounds.min_x; x<bounds.max_x; ++x){
            m.transform(x, y, sx, sy);
            if(!source_box.contains(sx, sy))continue;

            color = bilinear_color(source, sx, sy);

            if((sx & 0xFFFF0000) == 0x00000){//边界
                alpha = (sx & 0xFFFF) / 256;
                dest.blend(x, y, color, alpha);
            }
            else if(int(sx & 0xFFFF0000) == source_box.max_x - 65536){
                alpha = 256 - (sx & 0xFFFF) / 256;
                dest.blend(x, y, color, alpha);
            }
            else if((sy & 0xFFFF0000) == 0x00000){//边界
                alpha = (sy & 0xFFFF) / 256;
                dest.blend(x, y, color, alpha);
            }
            else if(int(sy & 0xFFFF0000) == source_box.max_y - 65536){
                alpha = 256 - (sy & 0xFFFF) / 256;
                dest.blend(x, y, color, alpha);
            }
            else{
                //dest.set_pixel(x, y, 0x000000ff);
                dest.set_pixel(x, y, color);
            }
        }
    }
}

//
// tile_image 平铺图像
//
// 这个函数比GDIPlus快
//

template<typename T>
void tile_image(pixelbuffer<T> &dest,
    vec4i bounds,
    const pixelbuffer<T>& image,
    pixel_traits<T>::getter getter = nearest_color)
{
    //移动过到起始位置
    while(0 < bounds.x){
        bounds.x -= bounds.width;
    }
    while(0 < bounds.y){
        bounds.y -= bounds.height;
    }

    //平铺
    for(int y=bounds.y; y<dest.height(); y+=bounds.height){
        for(int x=bounds.x; x<dest.width(); x+=bounds.width){
            copy_image<T>(dest, x, y, bounds.width, bounds.height, image, getter, source_alpha);
        }
    }
}

}//end namespace graphics
}//end namespace cgl

#endif //COPY_IMAGE_HPP_20200402232347
