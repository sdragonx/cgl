/*
 Copyright (c) 2005-2020 sdragonx (mail:sdragonx@foxmail.com)

 ilimage.hpp

 2016-03-14 14:58:04

 ilBindImage()之后，Images、Layers等就会重置

 ilGetInteger(IL_CUR_IMAGE)

*/

#ifndef ILIMAGE_HPP_20160314145804
#define ILIMAGE_HPP_20160314145804 1

#include <cgl/string/cstring.hpp>

#if !defined(_UNICODE) || !defined(UNICODE)
#define CGL_OPENIL_UNICODE
#define _UNICODE
#endif

#include <il/il.h>
#include <il/ilu.h>
//#include <il/ilut.h>

#if defined(CGL_PLATFORM_MSVC) || defined(CGL_PLATFORM_CBUILDER)
  #pragma comment(lib, "DevIL.lib")
#endif

namespace cgl{
namespace graphics{

const char* iluGetTypeName(ILenum type)
{
    switch(type){
    case IL_TYPE_UNKNOWN:
        return "未知格式";
    case IL_BMP:        return "BMP";
    case IL_CUT:        return "CUT";
    case IL_DOOM:       return "DOOM";
    case IL_DOOM_FLAT:  return "DOOM_FLAT";
    case IL_ICO:        return "ICO";
    case IL_JPG:        return "JPG";
    //case IL_JFIF:       return "JFIF";
    case IL_ILBM:       return "ILBM";
    case IL_PCD:        return "PCD";
    case IL_PCX:        return "PCX";
    case IL_PIC:        return "PIC";
    case IL_PNG:        return "PNG";
    case IL_PNM:        return "PNM";
    case IL_SGI:        return "SGI";
    case IL_TGA:        return "TGA";
    case IL_TIF:        return "TIF";
    case IL_CHEAD:      return "CHEAD";
    case IL_RAW:        return "RAW";
    case IL_MDL:        return "MDL";
    case IL_WAL:        return "WAL";
    case IL_LIF:        return "LIF";
    case IL_MNG:        return "MNG";
    //case IL_JNG:        return "JNG";
    case IL_GIF:        return "GIF";
    case IL_DDS:        return "DDS";
    case IL_DCX:        return "DCX";
    case IL_PSD:        return "PSD";
    case IL_EXIF:       return "EXIF";
    case IL_PSP:        return "PSP";
    case IL_PIX:        return "PIX";
    case IL_PXR:        return "PXR";
    case IL_XPM:        return "XPM";
    case IL_HDR:        return "HDR";
    case IL_ICNS:       return "ICNS";
    case IL_JP2:        return "JP2";
    case IL_EXR:        return "EXR";
    case IL_WDP:        return "WDP";
    case IL_VTF:        return "VTF";
    case IL_WBMP:       return "WBMP";
    case IL_SUN:        return "SUN";
    case IL_IFF:        return "IFF";
    case IL_TPL:        return "TPL";
    case IL_FITS:       return "FITS";
    case IL_DICOM:      return "DICOM";
    case IL_IWI:        return "IWI";
    case IL_BLP:        return "BLP";
    case IL_FTX:        return "FTX";
    case IL_ROT:        return "ROT";
    case IL_TEXTURE:    return "TEXTURE";
    case IL_DPX:        return "DPX";
    case IL_UTX:        return "UTX";
    case IL_MP3:        return "MP3";
    case IL_KTX:        return "KTX";
    default:
        return "未知新格式";
    }
}

class ilimage;

template<int ID, int SIZE_MODE, ILAPI ILboolean ILAPIENTRY (*activator)(ILuint )>
class ilelements
{
protected:
    friend class ilimage;

    ILuint handle;

public:
    ILuint id()
    {
        return ilGetInteger(ID);
    }

    ILboolean active(ILuint n)
    {
        if(handle){
            ilBindImage(handle);
            return activator(n);
        }
        return IL_FALSE;
    }

    ILuint size()const
    {
        if(handle){
            ilBindImage(handle);
            return ilGetInteger(SIZE_MODE);
        }
        return 0;
    }

};

class ilimage
{
public:
    typedef ilimage this_type;
    typedef wchar_t char_type;

public:
    union{
        ilelements<IL_ACTIVE_FACE,   IL_NUM_FACES,   ilActiveFace>   faces;
        ilelements<IL_ACTIVE_IMAGE,  IL_NUM_IMAGES,  ilActiveImage>  images;
        ilelements<IL_ACTIVE_LAYER,  IL_NUM_LAYERS,  ilActiveLayer>  layers;
        ilelements<IL_ACTIVE_MIPMAP, IL_NUM_MIPMAPS, ilActiveMipmap> mipmaps;
    };

public:
    ilimage()
    {
        set_handle(0);
    }

    ilimage(const char_type* filename)
    {
        set_handle(0);
        this->open(filename);
    }

    ilimage(const ilimage& image)
    {
        set_handle(0);
        *this = image;
    }

    virtual ~ilimage()
    {
        this->close();
    }

    ILuint handle()const
    {
        return images.handle;
    }

    //creates an ugly 64x64 black and yellow checkerboard image.
    bool default_image()
    {
        this->bind_always();
        return ilDefaultImage() == IL_TRUE;
    }
    
    bool TexImage(ILuint Width, ILuint Height, ILuint Depth, ILubyte NumChannels, ILenum Format, ILenum Type, void* Data)
    {
        this->bind_always();
        return ilTexImage(Width, Height, Depth, NumChannels, Format, Type, Data) == IL_TRUE;
    }

    bool create(int width, int height, int format, void* data = null)
    {
        int NumChannels;
        int pf;
        switch(format){
        case CGL_RGB:
            NumChannels = 3;
            pf = IL_RGB;
            break;
        case CGL_RGBA:
            NumChannels = 4;
            pf = IL_RGBA;
            break;
        case CGL_BGR:
            NumChannels = 3;
            pf = IL_BGR;
            break;
        case CGL_BGRA:
            NumChannels = 4;
            pf = IL_BGRA;
            break;
        default:
            CGL_ASSERT("unknown pixelformat.");
            return false;
        }
        return TexImage(width, height, 1, NumChannels, pf, IL_UNSIGNED_BYTE, data);
    }
    
    bool open(const char_type* filename)
    {
        this->bind_always();
        //return ilLoadImage(cstring<tchar_t>(filename).c_str());
        return ilLoadImage(filename) == IL_TRUE;
    }
    
    bool open(const char_type* filename, ILenum type)
    {
        this->bind_always();
        //return ilLoad(type, cstring<tchar_t>(filename).c_str());
        return ilLoad(type, filename) == IL_TRUE;
    }

    bool open_memory(const void* data, size_t size)
    {
        this->bind_always();
        return ilLoadL(IL_TYPE_UNKNOWN, data, size) == IL_TRUE;
    }
    
    bool save(const char_type* filename)
    {
        if(this->is_open()){
            this->bind();
            //return ilSaveImage(cstring<tchar_t>(filename).c_str());
            return ilSaveImage(filename) == IL_TRUE;
        }
        return false;
    }
    
    bool save(const char_type* filename, ILenum type)
    {
        if(this->is_open()){
            this->bind();
            //return ilSave(type, cstring<tchar_t>(filename).c_str());
            return ilSave(type, filename) == IL_TRUE;
        }
        return false;
    }

    bool is_open()const
    {
        return this->handle() != 0;
    }

    bool is_null()const
    {
        return this->handle() == 0;
    }

    void close()
    {
        if(this->is_open()){
            ilDeleteImages(1, &get_handle());
            this->set_handle(0);
        }
    }

    void bind()const
    {
        if(this->handle() != ILuint(ilGetInteger(IL_CUR_IMAGE))){
            ilBindImage(handle());
        }
    }

    void bind(ILuint image)
    {
        if(this->handle() == image){
            return;
        }
        this->close();  // Should we delete it?
        set_handle(image);
        this->bind();
    }

    //控制权移交
    void bind(this_type& image)
    {
        if(this->handle() == image.handle()){
            return ;
        }
        this->close();
        this->set_handle(image.release());
        this->bind();
    }

    //释放句柄，但不删除
    ILuint release()
    {
        ILuint image = handle();
        this->set_handle(0);
        return image;
    }

    //复制图像 handle << image
    bool copy(ILuint image)
    {
        this->bind_always();
        return ilCopyImage(image) == IL_TRUE;
    }

    this_type clone()
    {
        this_type image;
        if(this->is_open()){
            image.copy(this->handle());
        }
        return image;
    }

    bool bitblt(int x, int y, const ilimage& image, int sx, int sy, int sw, int sh)
    {
        if(this->is_open()){
            this->bind();
            //不管用？？
            return ilBlit(image.handle(), x, y, 0, sx, sy, 0, sw, sh, 0);
        }
        return IL_FALSE;
    }

    //
    // 图像信息
    //

    ILuint width()const { return get_int(IL_IMAGE_WIDTH); }
    ILuint height()const { return get_int(IL_IMAGE_HEIGHT); }
    ILuint depth()const { return get_int(IL_IMAGE_DEPTH); }
    ILubyte pixelbytes()const { return get_int(IL_IMAGE_BYTES_PER_PIXEL); }
    ILubyte pixelbits()const { return get_int(IL_IMAGE_BITS_PER_PIXEL); }
    ILenum format()const { return get_int(IL_IMAGE_FORMAT); }
    ILenum type()const { return get_int(IL_IMAGE_TYPE); }
    ILenum origin()const { return get_int(IL_IMAGE_ORIGIN); }

    //
    // 图像数据
    //

    ILubyte *data(void)
    {
        if(this->is_open()){
        this->bind();
            return ilGetData();
        }
        return 0;    
    }

    //图像数据大小
    ILuint size()const { return get_int(IL_IMAGE_SIZE_OF_DATA); }

    //
    // 调色板
    //

    ILenum palette_type()const { return get_int(IL_PALETTE_TYPE); }

    ILubyte *palette(void)
    {
        if(this->is_open()){
            this->bind();
            return ilGetPalette();
        }
        return 0;
    }

    ILuint palette_size()const { return get_int(IL_PALETTE_SIZE); }

    //
    // 图像操作
    //

    void clear(ILclampf red, ILclampf green, ILclampf blue, ILclampf alpha)
    {
        if(this->is_open()){
            this->bind();
            ilClearColour(red, green, blue, alpha);
            ilClearImage();
        }
    }

    bool convert(ILenum NewFormat)
    {
        if(this->is_open()){
            this->bind();
            return ilConvertImage(NewFormat, IL_UNSIGNED_BYTE) == IL_TRUE;
        }
        return false;
    }

    bool resize(ILuint Width, ILuint Height, ILuint Depth = 0)
    {
        if(this->is_open()){
            this->bind();
            //iluImageParameter(ILU_FILTER, ILU_NEAREST);
            //iluImageParameter(ILU_FILTER, ILU_SCALE_BOX);
            return iluScale(Width, Height, Depth);
        }
        return false;
    }

    bool flip(bool x, bool y)
    {
        if(this->is_open()){
            this->bind();
            if(x)iluMirror();
            if(y)iluFlipImage();
            return true;
        }
        return false;
    }

    bool rotate(float angle)
    {
        if(this->is_open()){
            this->bind();
            return iluRotate(angle);
        }
        return false;
    }

    //马赛克化
    bool mosaic(int pixelsize)
    {
        if(this->is_open()){
            this->bind();
            return iluPixelize(pixelsize);
        }
        return false;
    }

    //模糊
    bool blur(int sigma)
    {
        if(this->is_open()){
            this->bind();
            return iluBlurAvg(sigma);
            //return iluBlurGaussian(sigma);
        }
        return false;
    }

    bool gamma(float value)
    {
        if(this->is_open()){
            this->bind();
            return iluGammaCorrect(value);
        }
        return false;
    }

    //jpg config
    //图像品质
    void set_jpg_quality(int value = 80)
    {
        ilSetInteger(IL_JPG_QUALITY, value);
    }

    //png config
    ILenum alpha_index()const
    {
        if(this->is_open()){
            this->bind();
            return ilGetInteger(IL_PNG_ALPHA_INDEX);
        }
        return 0;
    }

    //操作符重载，复制图片
    this_type& operator=(ILuint image)
    {
        this->copy(image);
        return *this;
    }

    this_type& operator=(const this_type& image)
    {
        this->copy(image.handle());
        return *this;
    }

protected:
    void set_handle(ILuint image)
    {
        images.handle = image;
    }

    ILuint& get_handle()
    {
        return images.handle;
    }

    void gen_bind()
    {
        if(!this->is_open()){
            ilGenImages(1, &get_handle());
        }
        this->bind();
    }

    void bind_always()
    {
        this->is_open() ? this->bind() : this->gen_bind();    
    }

    ILuint get_int(ILenum e)const
    {
        if(this->is_open()){
            this->bind();
            return ilGetInteger(e);
        }
        return 0;
    }

private:
    static bool ilStartUp();
    static bool ilStartedUp;
};

// ensure that init is called exactly once
bool ilimage::ilStartUp()
{
    ilInit();
    //iluInit();
    //ilutInit();
    return true;
}

bool ilimage::ilStartedUp = ilStartUp();

}//end namespace graphics
}//end namespace cgl

#ifdef CGL_OPENIL_UNICODE
#undef _UNICODE
#undef CGL_OPENIL_UNICODE
#endif

#endif //ILIMAGE_HPP_20160314145804


/*
class images_type : public elements
{
public:
    images_type(ilimage* image) : elements(image) { }

    ILuint id()
    {
        return ilGetInteger(IL_ACTIVE_IMAGE);
    }

    ILboolean activeImage(ILuint n)
    {
        if(image){
            this->bind();
            return ilActiveImage(n);
        }
        return IL_FALSE;
    }

    ILuint images_count()const
    {
        if(this->is_open()){
            this->bind();
            return ilGetInteger(IL_NUM_IMAGES);
        }
        return 0;
    }

};


//图层管理
    ILboolean activeLayer(ILuint n)
    {
        if(this->is_open()){
            this->bind();
            return ilActiveLayer(n);
        }
        return IL_FALSE;
    }

    ILboolean activeMipmap(ILuint n)
    {
        if(this->is_open()){
            this->bind();
            return ilActiveImage(n);
        }
        return IL_FALSE;
    }


    ILuint num_layers()const
    {
        if(this->is_open()){
            this->bind();
            return ilGetInteger(IL_NUM_LAYERS);
        }
        return 0;
    }

    ILuint num_mipmaps()const
    {
        if(this->is_open()){
            this->bind();
            return ilGetInteger(IL_NUM_MIPMAPS);
        }
        return 0;
    }

*/