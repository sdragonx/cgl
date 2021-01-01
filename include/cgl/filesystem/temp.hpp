/*
 Copyright (c) 2005-2020 sdragonx (mail:sdragonx@foxmail.com)

 temp.hpp

 2020-05-26 10:52:10

*/
#ifndef TEMP_HPP_20200526105210
#define TEMP_HPP_20200526105210

#include <cgl/public.h>
#include <cgl/string.hpp>
#include <cgl/filesystem.hpp>
#include <cgl/filesystem/path.hpp>
#include <cgl/math/random.hpp>

namespace cgl{
namespace filesystem{

string_t tempname()
{
    char buf[32];

    do{
        uint32_t n = math::random(UINT16_MAX);
        std::snprintf(buf, 32, "file%04x.tmp", n);
    }while(filesystem::file_exists<char>(buf));
    return buf;
}

}//end namespace filesystem
}//end namespace cgl

#endif //TEMP_HPP_20200526105210



