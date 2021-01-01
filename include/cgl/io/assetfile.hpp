/*
 Copyright (c) 2005-2020 sdragonx (mail:sdragonx@foxmail.com)

 assetfile.hpp

 2018-12-01 01:11:17

 安卓资源文件读取

*/
#ifndef ASSETFILE_HPP_20181201011117
#define ASSETFILE_HPP_20181201011117

#ifdef CGL_PLATFORM_ANDROID

#include <android/asset_manager.h>
#include "base.hpp"
#include "unifile.hpp"

namespace cgl{
namespace io{

class assetfile : public unifile
{
public:
    assetfile() : unifile()
    {
    }

    ~assetfile()
    {
        this->close();
    }

    int open(AAssetManager* am, const char* filename, int mode)
    {
        unifile::m_handle = AAssetManager_open(am, filename, mode);
        unifile::m_name = filename;
        return 0;
    }

    void close()
    {
        if(m_handle){
            AAsset* asset = (AAsset*)unifile::m_handle;
            AAsset_close(asset);
            unifile::m_handle = null;
        }
    }

    bool is_open()const
    {
        return unifile::m_handle;
    }

    fsize_t seek(fsize_t pos, int whence)
    {
        AAsset* asset = (AAsset*)unifile::m_handle;
        return AAsset_seek(asset, pos, whence);
        //return AAsset_seek64(asset, pos, whence);
    }

    fsize_t tell()
    {
        return this->seek(0, SEEK_CUR);
    }

    fsize_t read(void* data, fsize_t size)
    {
        AAsset* asset = (AAsset*)unifile::m_handle;
        return AAsset_read(asset, data, size);
    }

    fsize_t size()
    {
        AAsset* asset = (AAsset*)unifile::m_handle;
        return AAsset_getLength(asset);
    }

};

}//end namespace io
}//end namespace cgl

#endif//CGL_PLATFORM_ANDROID

#endif //ASSETFILE_HPP_20181201011117
