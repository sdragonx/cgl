/*
 Copyright (c) 2005-2020 sdragonx (mail:sdragonx@foxmail.com)

 dir.hpp

 2015-07-29 14:59:36

*/
#ifndef DIR_HPP_20150729145936
#define DIR_HPP_20150729145936

#include <stdlib.h>

namespace cgl{
namespace filesystem{

//把带有环境变量的路径转换成普通路径，类似winapi ExpandEnvironmentStrings
template<typename _char_t>
_char_t* expand_env_string(const _char_t* path, _char_t* output, size_t size)
{
    _char_t* env_path = null;
    size_t   env_size = 0;
    _char_t  env_name[1024] = {0};
    _char_t* env = 0;

    const _char_t* path_end = path+cslen(path);
    _char_t* pout = output;

    --size;
    for( ; path<path_end, size; ++path)
    {
        if(path[0] == '%'){
            if(env){
                *env = '\0';
                env_path = tvalue<_char_t>(getenv, _wgetenv)(env_name);
                env_size = std::min<size_t>(cslen(env_path), size);
                csncpy(pout, env_path, env_size);
                pout += env_size;
                size -= env_size;
                env = null;
            }
            else{
                env = env_name;
            }
        }
        else if(env){
            *env++ = *path;
        }
        else{
            *pout++ = *path;
            --size;
        }
    }
    *pout = '\0';
    return output;
}

}//end namespace filesystem
}//end namespace cgl

#endif //DIR_HPP_20150729145936
