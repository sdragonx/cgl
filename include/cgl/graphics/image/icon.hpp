/*
 Copyright (c) 2005-2020 sdragonx (mail:sdragonx@foxmail.com)

 icon.hpp

 sdragon 2006-07-30 18:57:06

*/
#ifndef CGL_ICON_H_20060730185706
#define CGL_ICON_H_20060730185706

#include <windows.h>

namespace cgl{
namespace windows{
namespace gdi{

bool save_icon(
    HICON hIcon,        //图标句柄
    LPCSTR strFileName, //图标文件名称
    int nBit            //保存图标的颜色深度
    );

typedef struct tagICONHEADER
{
    WORD Reserved;
    WORD wType;
    WORD Count;
}ICONHEADER;

typedef struct tagICONINFOHEADER
{
    BYTE  bWidth;
    BYTE  bHeight;
    WORD  wColors;
    WORD  wReserved1;
    WORD  wReserved2;
    DWORD dwSize;
    DWORD dwOffset;
}ICONINFOHEADER;

int InitBmpInfoHeader(HBITMAP Bitmap, BITMAPINFOHEADER& BI, int nBit)
{
    int Bytes;
    DIBSECTION DS;

    DS.dsBmih.biSize = 0;
    Bytes = GetObject(Bitmap, sizeof(DS), &DS);
    if (( Bytes>=sizeof(DS.dsBm)+sizeof(DS.dsBmih) ) &&
        ( DS.dsBmih.biSize>=sizeof(DS.dsBmih) ) ){
        memcpy(&BI, &DS.dsBmih, sizeof(BITMAPINFOHEADER));
    }
    else{
        memset(&BI, 0, sizeof(BI));
        BI.biSize = sizeof(BI);
        BI.biWidth = DS.dsBm.bmWidth;
        BI.biHeight = DS.dsBm.bmHeight;
        BI.biBitCount = DS.dsBm.bmPlanes * DS.dsBm.bmBitsPixel;
    }
    if (nBit!=0){
            BI.biBitCount = nBit;
    }
    if (BI.biBitCount <= 8){
        BI.biClrUsed = 1<<BI.biBitCount;
    }
    BI.biPlanes = 1;
    if (BI.biClrImportant > BI.biClrUsed){
        BI.biClrImportant = BI.biClrUsed;
    }
    if (BI.biSizeImage == 0){
        BI.biSizeImage = ((BI.biWidth*BI.biBitCount+31) / 32) * 4 * BI.biHeight;
    }
    return 0;
}

static void GetDIBSizes(
    HBITMAP Bitmap,
    DWORD& InfoSize,
    DWORD& ImageSize,
    int nBit)
{
    BITMAPINFOHEADER BI;

    InitBmpInfoHeader(Bitmap, BI, nBit);
    InfoSize = sizeof(BITMAPINFOHEADER);
    if (BI.biBitCount > 8)
    {
        if (BI.biCompression & BI_BITFIELDS)
            InfoSize += 12;
    }
    else
    {
        InfoSize += sizeof(RGBQUAD) *
            (BI.biClrUsed!=0 ? BI.biClrUsed : (1 << BI.biBitCount));
    }
    ImageSize = BI.biSizeImage;
}

static void GetDIB(
    HBITMAP Bitmap,
    BITMAPINFO* BmpInfo,
    void* Bits,
    int nBit)
{
    HDC DC = CreateCompatibleDC(NULL);
    InitBmpInfoHeader(Bitmap, BmpInfo->bmiHeader, nBit);
    GetDIBits(DC, Bitmap, 0, BmpInfo->bmiHeader.biHeight, Bits,
        BmpInfo, DIB_RGB_COLORS);
    DeleteDC(DC);
}

bool save_icon(HICON hIcon, LPCTSTR strFileName, int nBit)
{
    if(!hIcon || !strFileName) return false;
    switch(nBit)
    {
        case 1:
        case 4:
        case 8:
        case 16:
        case 24:
        case 32:
            break;
        default:
            return false;
    }

    BOOL   rt;
    HANDLE hFile;
    DWORD  dwWrite;

    ICONHEADER     IconHeader = {0};
    ICONINFOHEADER IconInfoHeader = {0};
    ICONINFO       IconInfo;

    DWORD MaskInfoSize;
    DWORD ColorInfoSize;
    DWORD MaskBitsSize;
    DWORD ColorBitsSize;
    void *MaskBits;
    void *ColorBits;
    BITMAPINFO *MaskInfo;
    BITMAPINFO *ColorInfo;

    hFile = CreateFile(strFileName,
        GENERIC_WRITE, FILE_SHARE_READ,
        0, CREATE_ALWAYS, 0, 0);
    if (hFile==INVALID_HANDLE_VALUE) return false;

    GetIconInfo(hIcon, &IconInfo);
    GetDIBSizes(IconInfo.hbmMask, MaskInfoSize, MaskBitsSize, 1);
    GetDIBSizes(IconInfo.hbmColor, ColorInfoSize, ColorBitsSize, nBit);

    MaskInfo  = (BITMAPINFO*)malloc(MaskInfoSize);
    ColorInfo = (BITMAPINFO*)malloc(ColorInfoSize);
    MaskBits  = malloc(MaskBitsSize);
    ColorBits = malloc(ColorBitsSize);
    GetDIB(IconInfo.hbmMask, MaskInfo, MaskBits, 1);
    GetDIB(IconInfo.hbmColor, ColorInfo, ColorBits, nBit);
    IconHeader.wType = 1;//0x10001; 1==icon 2==ani
    IconHeader.Count = 1;
    rt &= WriteFile(hFile, &IconHeader, sizeof(IconHeader),
        &dwWrite, NULL);
    IconInfoHeader.bWidth = ColorInfo->bmiHeader.biWidth;
    IconInfoHeader.bHeight = ColorInfo->bmiHeader.biHeight;
    IconInfoHeader.wColors = ColorInfo->bmiHeader.biPlanes *
        ColorInfo->bmiHeader.biBitCount;
    IconInfoHeader.dwSize = ColorInfoSize + ColorBitsSize + MaskBitsSize;
    IconInfoHeader.dwOffset = sizeof(IconHeader) + sizeof(IconInfoHeader);
    rt &= WriteFile(hFile, &IconInfoHeader, sizeof(IconInfoHeader),
        &dwWrite, NULL);
    ColorInfo->bmiHeader.biHeight *= 2;
    rt &= WriteFile(hFile, ColorInfo, ColorInfoSize, &dwWrite, NULL);
    rt &= WriteFile(hFile, ColorBits, ColorBitsSize, &dwWrite, NULL);
    rt &= WriteFile(hFile, MaskBits, MaskBitsSize, &dwWrite, NULL);
    free(ColorInfo);
    free(MaskInfo);
    free(ColorBits);
    free(MaskBits);
    DeleteObject(IconInfo.hbmColor);
    DeleteObject(IconInfo.hbmMask);
    CloseHandle(hFile);
    return rt;
}

HICON load_icon(const TCHAR* filename)
{
    //return LoadImage(filename);
    return NULL;
}

}//end namespace gdi
}//end namespace windows
}//end namespace cgl

#endif //CGL_ICON_H_20060730185706
