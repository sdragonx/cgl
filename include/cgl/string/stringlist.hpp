/*
 Copyright (c) 2005-2020 sdragonx (mail:sdragonx@foxmail.com)

 stringlist.hpp

 2020-02-19 17:16:00

*/
#ifndef STRINGLIST_HPP_20200219171600
#define STRINGLIST_HPP_20200219171600

#include <cgl/public.h>

namespace cgl{

//
// string list find
//
// stringlist_find("abc;123;456", "123");
//
bool stringlist_find(
    const char* begin, const char* end,
    const char* find_begin, const char* find_end,
    char split = ';')
{
    const char* p = std::search(begin, end, find_begin, find_end);
    if(p == end){
        return false;
    }
    size_t size = find_end - find_begin;
    if(end - begin == size){
        return true;
    }
    else if(p == begin){
        return *(p + size) == split;
    }
    else if(p + size == end){
        return *(p - 1) == split;
    }
    else{
        return (*(p + size) == split) && (*(p - 1) == split);
    }
    return false;
}

bool stringlist_find(const char* str, const char* list)
{
    return find_string(list, list + strlen(list), str, str + strlen(str));
}

template<typename T>
class stringlist : public std::vector< cstring<T> >
{
public:
    void sort()
    {
    }
};

}//end namespace cgl

#endif //STRINGLIST_HPP_20200219171600
