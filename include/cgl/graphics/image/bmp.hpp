/*
 Copyright (c) 2005-2020 sdragonx (mail:sdragonx@foxmail.com)

 bitmap.hpp

 2020-02-14 16:00:31

*/
#ifndef BITMAP_HPP_20200214160031
#define BITMAP_HPP_20200214160031

#include <cgl/io/binfile.hpp>

#ifndef _WINGDI_

#pragma pack(push, 1)

typedef struct tagBITMAP
{
    int32_t  bmType;
    int32_t  bmWidth;
    int32_t  bmHeight;
    int32_t  bmWidthBytes;
    uint16_t bmPlanes;
    uint16_t bmBitsPixel;
    void*    bmBits;
}BITMAP;

//BMP文件 BITMAPFILEHEADER + BITMAPINFOHEADER + PALETTE + DATA
typedef struct tagBITMAPFILEHEADER
{
    uint16_t bfType;
    uint32_t bfSize;
    uint16_t bfReserved1;
    uint16_t bfReserved2;
    uint32_t bfOffBits;
}BITMAPFILEHEADER;

typedef struct tagBITMAPINFOHEADER
{
    uint32_t biSize;
    int32_t  biWidth;
    int32_t  biHeight;
    uint16_t biPlanes;
    uint16_t biBitCount;
    uint32_t biCompression;
    uint32_t biSizeImage;
    int32_t  biXPelsPerMeter;
    int32_t  biYPelsPerMeter;
    uint32_t biClrUsed;
    uint32_t biClrImportant;
}BITMAPINFOHEADER;

typedef struct tagRGBQUAD
{
    uint8_t rgbBlue;
    uint8_t rgbGreen;
    uint8_t rgbRed;
    uint8_t rgbReserved;
}RGBQUAD;

typedef struct tagBITMAPINFO
{
    BITMAPINFOHEADER    bmiHeader;
    RGBQUAD             bmiColors[1];
}BITMAPINFO;

#pragma pack(pop)

#endif//_WINGDI_

namespace cgl{
namespace graphics{

//2020-02-18 02:20:10
int bmp_save(io::unifile* file, int width, int height, int pixelbits, int stride, void* data, void* palette)
{
    BITMAPFILEHEADER bmFile;
    BITMAPINFOHEADER bmInfo;
    RGBQUAD pal[256] = {0};
    int palsize = (pixelbits < 15) ? 4 * (1 << pixelbits) : 0;
    int linebytes;

    //计算行宽
    if(pixelbits < 8){
        int pixels = 8 / pixelbits;
        linebytes = (width + pixels - 1) / pixels;
    }
    else{
        linebytes = width * (pixelbits / 8);
    }

    linebytes += 3;
    linebytes &= ~3;

    //填充bmpinfo结构
    bmInfo.biSize          = le_conv32(sizeof(BITMAPINFOHEADER));
    bmInfo.biWidth         = le_conv32(width);
    bmInfo.biHeight        = le_conv32(height);
    bmInfo.biPlanes        = le_conv16(1);
    bmInfo.biBitCount      = le_conv16(pixelbits);
    bmInfo.biCompression   = 0;
    bmInfo.biSizeImage     = le_conv32(linebytes * height);
    bmInfo.biXPelsPerMeter = 0;
    bmInfo.biYPelsPerMeter = 0;
    bmInfo.biClrUsed       = le_conv32(palsize);
    bmInfo.biClrImportant  = 0;

    //填充文件头
    bmFile.bfType      = le_conv16(0x4D42);   // 'BM' WINDOWS_BITMAP_SIGNATURE
    bmFile.bfSize      = le_conv32(sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + bmInfo.biSizeImage);
    bmFile.bfReserved1 = 0;
    bmFile.bfReserved2 = 0;
    bmFile.bfOffBits   = le_conv32(sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * palsize);

    //写入数据
    file->seek(0, SEEK_SET);
    file->write(&bmFile, sizeof(BITMAPFILEHEADER));
    file->write(&bmInfo, sizeof(BITMAPINFOHEADER));

    //写入调色板数据
    if(palsize){
        file->write(pal, sizeof(RGBQUAD) * palsize);
    }

    //写入图像数据
    //如果源数据未对齐。。。再更新吧
    file->write(data, linebytes * height);
    return 0;
}

}//end namespace
}//end namespace cgl

#endif //BITMAP_HPP_20200214160031
