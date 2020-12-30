/*
 Copyright (c) 2005-2020 sdragonx (mail:sdragonx@foxmail.com)

 dds.hpp

 2020-05-25 17:58:39

*/
#ifndef DDS_HPP_20200525175839
#define DDS_HPP_20200525175839

#include <cgl/public.h>
#include <cgl/graphics/gl/gl.hpp>

#ifndef MAKEFOURCC
#define MAKEFOURCC(ch0, ch1, ch2, ch3) \
	((unsigned int)(unsigned char)(ch0) | ((unsigned int)(unsigned char)(ch1) << 8) |   \
	((unsigned int)(unsigned char)(ch2) << 16) | ((unsigned int)(unsigned char)(ch3) << 24 ))
#endif


#define DDS_MAGIC 0x20534444    // "DDS "

#define DDS_FOURCC      0x00000004  // DDPF_FOURCC
#define DDS_RGB         0x00000040  // DDPF_RGB
#define DDS_RGBA        0x00000041  // DDPF_RGB | DDPF_ALPHAPIXELS
#define DDS_LUMINANCE   0x00020000  // DDPF_LUMINANCE
#define DDS_LUMINANCEA  0x00020001  // DDPF_LUMINANCE | DDPF_ALPHAPIXELS
#define DDS_ALPHA       0x00000002  // DDPF_ALPHA
#define DDS_PAL8        0x00000020  // DDPF_PALETTEINDEXED8

#define DDS_HEADER_FLAGS_TEXTURE        0x00001007  // DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH | DDSD_PIXELFORMAT
#define DDS_HEADER_FLAGS_MIPMAP         0x00020000  // DDSD_MIPMAPCOUNT
#define DDS_HEADER_FLAGS_VOLUME         0x00800000  // DDSD_DEPTH
#define DDS_HEADER_FLAGS_PITCH          0x00000008  // DDSD_PITCH
#define DDS_HEADER_FLAGS_LINEARSIZE     0x00080000  // DDSD_LINEARSIZE

#define DDS_HEIGHT 0x00000002 // DDSD_HEIGHT
#define DDS_WIDTH  0x00000004 // DDSD_WIDTH

#define DDS_SURFACE_FLAGS_TEXTURE 0x00001000 // DDSCAPS_TEXTURE
#define DDS_SURFACE_FLAGS_MIPMAP  0x00400008 // DDSCAPS_COMPLEX | DDSCAPS_MIPMAP
#define DDS_SURFACE_FLAGS_CUBEMAP 0x00000008 // DDSCAPS_COMPLEX

#define DDS_CUBEMAP_POSITIVEX 0x00000600 // DDSCAPS2_CUBEMAP | DDSCAPS2_CUBEMAP_POSITIVEX
#define DDS_CUBEMAP_NEGATIVEX 0x00000a00 // DDSCAPS2_CUBEMAP | DDSCAPS2_CUBEMAP_NEGATIVEX
#define DDS_CUBEMAP_POSITIVEY 0x00001200 // DDSCAPS2_CUBEMAP | DDSCAPS2_CUBEMAP_POSITIVEY
#define DDS_CUBEMAP_NEGATIVEY 0x00002200 // DDSCAPS2_CUBEMAP | DDSCAPS2_CUBEMAP_NEGATIVEY
#define DDS_CUBEMAP_POSITIVEZ 0x00004200 // DDSCAPS2_CUBEMAP | DDSCAPS2_CUBEMAP_POSITIVEZ
#define DDS_CUBEMAP_NEGATIVEZ 0x00008200 // DDSCAPS2_CUBEMAP | DDSCAPS2_CUBEMAP_NEGATIVEZ

#define DDS_CUBEMAP_ALLFACES (DDS_CUBEMAP_POSITIVEX | DDS_CUBEMAP_NEGATIVEX |\
                              DDS_CUBEMAP_POSITIVEY | DDS_CUBEMAP_NEGATIVEY |\
                              DDS_CUBEMAP_POSITIVEZ | DDS_CUBEMAP_NEGATIVEZ)

#define DDS_CUBEMAP 0x00000200 // DDSCAPS2_CUBEMAP

#define DDS_FLAGS_VOLUME 0x00200000 // DDSCAPS2_VOLUME

#define FOURCC_DXT1 MAKEFOURCC('D','X','T','1')
#define FOURCC_DXT2 MAKEFOURCC('D','X','T','2')
#define FOURCC_DXT3 MAKEFOURCC('D','X','T','3')
#define FOURCC_DXT4 MAKEFOURCC('D','X','T','4')
#define FOURCC_DXT5 MAKEFOURCC('D','X','T','5')

#pragma pack(push, 1)

struct DDS_PIXELFORMAT
{
    uint32_t  size;         // size of structure
    uint32_t  flags;        // pixel format flags
    uint32_t  fourCC;       // (FOURCC code)
    uint32_t  RGBBitCount;  // how many bits per pixel
    uint32_t  RBitMask;     // mask for red bit
    uint32_t  GBitMask;     // mask for green bit
    uint32_t  BBitMask;     // mask for blue bit
    uint32_t  ABitMask;     // mask for alpha bit
};

struct DDS_HEADER
{
    uint32_t          size;
    uint32_t          flags;
    uint32_t          height;
    uint32_t          width;
    uint32_t          pitchOrLinearSize;    // Formless late-allocated optimized surface size
    uint32_t          depth; // only if DDS_HEADER_FLAGS_VOLUME is set in flags
    uint32_t          mipmapCount;
    uint32_t          alphaBitDepth;
    uint32_t          reserved1[10];
    DDS_PIXELFORMAT   ddspf;
    uint32_t          caps1;    // direct draw surface capabilities
    uint32_t          caps2;
    uint32_t          caps3;
    uint32_t          caps4;
    uint32_t          textureStage;
};

#pragma pack(pop)

namespace cgl{
namespace graphics{

//
// gluLoadCompressedTexture()
//
// 压缩纹理支持
//

#ifndef GL_COMPRESSED_RGB_S3TC_DXT1_EXT
    #define GL_COMPRESSED_RGB_S3TC_DXT1_EXT  0x83F0
    #define GL_COMPRESSED_RGBA_S3TC_DXT1_EXT 0x83F1
    #define GL_COMPRESSED_RGBA_S3TC_DXT3_EXT 0x83F2
    #define GL_COMPRESSED_RGBA_S3TC_DXT5_EXT 0x83F3
#endif

//dds图片数据
struct DDS_IMAGE_DATA
{
    int width;
    int height;
    int components;
    int format;
    int mipmaps;
    std::vector<byte_t> pixels;
};

//加载DDS文件
int gluLoadDDSImage( DDS_IMAGE_DATA& dds, const char *filename )
{
    DDS_HEADER ddsd;
    char filecode[4];
    FILE *pFile;
    int factor;
    int bufferSize;

    // Open the file
    io::binfile f;
    f.open(filename, io_read, sh_share);

    if( !f.is_open() ){
        CGL_LOG("loadDDSTextureFile couldn't find, or failed to load \"%s\"", filename);
        return CGL_ERROR;
    }

    // Verify the file is a true .dds file
    f.read( filecode, 4 );

    if( strncmp( filecode, "DDS ", 4 ) != 0 ){
        CGL_LOG("The file \"%s\" doesn't appear to be a valid .dds file!", filename);
        return CGL_ERROR;
    }

    // Get the surface descriptor
    f.read(&ddsd, sizeof(ddsd));
    memset( &dds, 0, sizeof(DDS_IMAGE_DATA) );

    //
    // This .dds loader supports the loading of compressed formats DXT1, DXT3
    // and DXT5.
    //

    switch( ddsd.ddspf.fourCC ){
    case FOURCC_DXT1:
        // DXT1's compression ratio is 8:1
        dds.format = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
        factor = 2;
        break;
    case FOURCC_DXT3:
        // DXT3's compression ratio is 4:1
        dds.format = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
        factor = 4;
        break;
    case FOURCC_DXT5:
        // DXT5's compression ratio is 4:1
        dds.format = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
        factor = 4;
        break;
    default:
        CGL_LOG("The file \"%s\" doesn't appear to be compressed "
            "using DXT1, DXT3, or DXT5!", filename );
        return CGL_ERROR;
    }

    //
    // How big will the buffer need to be to load all of the pixel data
    // including mip-maps?
    //

    if( ddsd.pitchOrLinearSize == 0 ){
        CGL_LOG("dwLinearSize is 0!");
    }

    if( ddsd.mipmapCount > 1 )
        bufferSize = ddsd.pitchOrLinearSize * factor;
    else
        bufferSize = ddsd.pitchOrLinearSize;

    dds.pixels.resize(bufferSize);
    f.read( &dds.pixels[0], bufferSize );

    // Close the file
    f.close();

    dds.width   = ddsd.width;
    dds.height  = ddsd.height;
    dds.mipmaps = ddsd.mipmapCount;

    if( ddsd.ddspf.fourCC == FOURCC_DXT1 )
        dds.components = 3;
    else
        dds.components = 4;

    return CGL_OK;
}

//生成纹理
GLuint gluLoadCompressedTexture( const char* filename )
{
    // NOTE: Unlike "lena.bmp", "lena.dds" actually contains its own mip-map
    // levels, which are also compressed.
    DDS_IMAGE_DATA dds;

    GLuint tex;

    if(gluLoadDDSImage( dds, filename ) != CGL_OK){
        return 0;
    }

    int nHeight     = dds.height;
    int nWidth      = dds.width;
    int nNumMipMaps = dds.mipmaps;

    int nBlockSize;

    if( dds.format == GL_COMPRESSED_RGBA_S3TC_DXT1_EXT )
        nBlockSize = 8;
    else
        nBlockSize = 16;

    glGenTextures( 1, &tex );
    glBindTexture( GL_TEXTURE_2D, tex );

    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

    int nSize;
    int nOffset = 0;

    // Load the mip-map levels

    for( int i = 0; i < nNumMipMaps; ++i ){
        if( nWidth  == 0 ) nWidth  = 1;
        if( nHeight == 0 ) nHeight = 1;

        nSize = ((nWidth+3)/4) * ((nHeight+3)/4) * nBlockSize;

        glCompressedTexImage2D ( GL_TEXTURE_2D,
            i,
            dds.format,
            nWidth,
            nHeight,
            0,
            nSize,
            &dds.pixels[nOffset] );

        nOffset += nSize;

        // Half the image size for the next mip-map level...
        nWidth  = (nWidth  / 2);
        nHeight = (nHeight / 2);
    }

    return tex;
}

}//end namespace graphics
}//end namespace cgl

#endif //DDS_HPP_20200525175839
