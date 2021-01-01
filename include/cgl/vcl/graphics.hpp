/*
 Copyright (c) 2005-2020 sdragonx (mail:sdragonx@foxmail.com)

 vcl.graphics.hpp

 2020-04-23 12:42:17

*/
#ifndef VCL_GRAPHICS_HPP_20200423124217
#define VCL_GRAPHICS_HPP_20200423124217

#include <cgl/public.h>
#include <cgl/graphics/image.hpp>
#include <cgl/graphics/effect/convolution.hpp>

namespace cgl{
namespace vcl{

typedef Graphics::TBitmap* PTBitmap;

//
// TBitmap
//

Graphics::TBitmap* TBitmap_Load(String file)
{
    Graphics::TBitmap* bmp = new Graphics::TBitmap();
    bmp->LoadFromFile(file);
    return bmp;
}

void TBitmap_Resize(Graphics::TBitmap* bmp, int width, int height, TPixelFormat pf = pf32bit)
{
    bmp->PixelFormat = pf;
    bmp->SetSize(width, height);
}

int TBitmap_LoadImage(Graphics::TBitmap* bmp, const void* data, size_t size)
{
    #ifdef CGL_USING_ILIMAGE
    graphics::ilimage image;

    image.open_memory(data, size);
    if(!image.is_open())
    {//�ļ���ʧ��
        image.close();
        return -1;
    }

    //ת��ΪBGRA���ظ�ʽ
    if(image.format() != IL_BGRA){
        //image.convert(IL_BGRA);
    }
    if(image.origin() != IL_ORIGIN_LOWER_LEFT){
        image.flip(false, true);
    }

    bmp->PixelFormat = pf32bit;
    bmp->SetSize(image.width(), image.height());

    memcpy(bmp->ScanLine[image.height()-1], image.data(), image.size());

    image.close();
    #else
    int w, h, channels;
    //����ͼƬת����RGBA��ʽ
    memblock<byte_t> buf;
    byte_t *img = stbi_load_from_memory(static_cast<const byte_t*>(data), size, &w, &h, &channels, 4);
    if(img){
        memswap(img, img + w * h * 4, w * 4);
        bmp->PixelFormat = pf32bit;
        bmp->SetSize(w, h);
        memcpy(bmp->ScanLine[h-1], img, w * h * 4);
        stbi_image_free(img);
    }
    #endif
    return 0;
}

int TBitmap_LoadImage(Graphics::TBitmap* bmp, String filename)
{
    #ifdef CGL_USING_ILIMAGE
    graphics::ilimage image;

    image.open(filename.c_str());
    if(!image.is_open())
    {//�ļ���ʧ��
        image.close();
        return -1;
    }

    //ת��ΪBGRA���ظ�ʽ
    if(image.format() != IL_BGRA){
        //image.convert(IL_BGRA);
    }
    if(image.origin() != IL_ORIGIN_LOWER_LEFT){
        image.flip(false, true);
    }

    int w = image.width();
    int h = image.height();

    bmp->PixelFormat = pf32bit;
    bmp->SetSize(w, h);//image.width(), image.height());

    memcpy(bmp->ScanLine[image.height()-1], image.data(), image.size());

    image.close();
    #else
    int w, h, channels;
    //����ͼƬת����RGBA��ʽ
    memblock<byte_t> buf;
    io::load_file(buf, filename.c_str());
    byte_t *img = stbi_load_from_memory(buf.data(), buf.size(), &w, &h, &channels, 4);
    if(img){
        memswap(img, img + w * h * 4, w * 4);
        bmp->PixelFormat = pf32bit;
        bmp->SetSize(w, h);
        memcpy(bmp->ScanLine[h-1], img, w * h * 4);
        stbi_image_free(img);
    }
    #endif
    return 0;
}

//
// ����
//

int TBitmap_Convolution(Graphics::TBitmap* bmpDest,
    Graphics::TBitmap* bmpSrc,
    const graphics::convolution<int>::kernel& mat)
{
    if(!bmpDest || !bmpSrc){
        return -1;
    }

    Graphics::TBitmap* temp;
    bool is_allcate = false;
    TPixelFormat format = bmpSrc->PixelFormat;

    if(format == pf24bit || format == pf32bit){
        format = bmpSrc->PixelFormat;
        if(bmpDest != bmpSrc){
            temp = bmpSrc;
        }
        else{
            temp = new Graphics::TBitmap();
            temp->Assign(bmpSrc);
            is_allcate = true;
        }
    }
    else{
        format = pf24bit;
        temp = new Graphics::TBitmap();
        temp->Assign(bmpSrc);
        temp->PixelFormat = format;
        is_allcate = true;
    }

    bmpDest->PixelFormat = temp->PixelFormat;
    bmpDest->SetSize(temp->Width, temp->Height);

    graphics::convolution<int> cv;
    if(format == pf24bit){
        graphics::pixelbuffer<vec3ub> source, dest;
        source.assign(temp);
        dest.assign(bmpDest);
        cv.execute(dest, source, mat);
    }
    else if(format == pf32bit){
        graphics::pixelbuffer<vec4ub> source, dest;
        source.assign(temp);
        dest.assign(bmpDest);
        cv.execute(dest, source, mat);
    }

    if(is_allcate){
        delete temp;
    }

    return 0;
}

//
//TImage
//

//TImageͼ��ı��С
//32λ ���ͼ��16k X 8k����
void TImage_Resize(TImage* Image, int width, int height, TPixelFormat pixelformat = pf32bit)
{
    Image->Width = width;
    Image->Height = height;
    Image->Picture->Bitmap->PixelFormat = pixelformat;
    Image->Picture->Bitmap->SetSize(width, height);
}

//TImageͼ��ı䵽��ǰ�ؼ���С
void TImage_Resize(TImage* Image, TPixelFormat pixelformat = pf32bit)
{
    Image->Picture->Bitmap->PixelFormat = pixelformat;
    Image->Picture->Bitmap->Width = Image->Width;
    Image->Picture->Bitmap->Height = Image->Height;
}

//��ʼ��TImageΪwindows��ɫ��
void TImage_InitColorpad(TImage* image, int width, int height)
{
    TImage_Resize(image, width, height, pf32bit);

    for(int y=0; y < height; ++y){
        DWORD* line = (DWORD*)image->Picture->Bitmap->ScanLine[y];
        for(int x=0; x < width; ++x){
            DWORD c = graphics::colorpad_color(width, height, x, y);
            line[x] = RedBlueSwap(c);
        }
    }
}

int TImage_LoadImage(TImage* Image, const void* data, size_t size)
{
    Graphics::TBitmap* bmp = Image->Picture->Bitmap;
    TBitmap_LoadImage(bmp, data, size);
    Image->Width = bmp->Width;
    Image->Height = bmp->Height;
    Image->Repaint();
    return 0;
}

int TImage_LoadImage(TImage* Image, String filename)
{
    Graphics::TBitmap* bmp = Image->Picture->Bitmap;
    TBitmap_LoadImage(bmp, filename);
    Image->Width = bmp->Width;
    Image->Height = bmp->Height;
    Image->Repaint();
    return 0;
}

}//end namespace vcl
}//end namespace cgl

#endif //VCL_GRAPHICS_HPP_20200423124217