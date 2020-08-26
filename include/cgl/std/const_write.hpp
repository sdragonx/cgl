/*
 Copyright (c) 2005-2020 sdragonx (mail:sdragonx@foxmail.com)

 const_write.hpp

 modify const variable

 2018-06-30 02:49:16
 2018-10-30 10:15

*/
#ifndef CONST_WRITE_H_20180630024916
#define CONST_WRITE_H_20180630024916

namespace cgl{

template<typename T>
inline T& const_write(const T& value)
{
    return const_cast<T&>(value);
}

}//end namespace cgl

#endif //CONST_WRITE_H_20180630024916
