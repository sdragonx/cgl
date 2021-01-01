/*
 Copyright (c) 2005-2020 sdragonx (mail:sdragonx@foxmail.com)

 filename.hpp

 2015-02-25 14:32:54

 文件名查找比较

*/
#ifndef FILENAME_HPP_20150225143254
#define FILENAME_HPP_20150225143254

#include <cgl/string.hpp>

namespace cgl{
namespace filesystem{

/*
 模糊查找文件名matching
 例：
 bool b = fn_compare("abc.txt", "a?c.*");

 2010-09-20 16:01

 match
*/
template<typename _char_t>
bool fn_compare(const cstring<_char_t>& fname, const cstring<_char_t>& findstr)
{
    //const static cstring<_char_t> mark = "*?";
    _char_t mark[4] = { '*', '?' };
    if(fname.empty() || findstr.empty())
    {
        return false;
    }
    if(findstr.find_first_of(mark) == cstring<_char_t>::npos)
    {
        return fname.find(findstr) != cstring<_char_t>::npos;
    }
    else
    {
        size_t n = 0;
        for(size_t i = 0; i<findstr.size(); )
        {
            if(findstr[i] == '*')
            {
                i = findstr.find_first_not_of(mark, i);
                if(i == cstring<_char_t>::npos){
                    return true;
                }
                n = fname.find(findstr[i], n);
                if(n == cstring<_char_t>::npos){
                    return false;
                }
            }
            else if(findstr[i] == '?')
            {
                ++n;
                ++i;
            }
            else
            {
                //while(mark.find(findstr[i]) == cstring<_char_t>::npos)
                while(findstr[i] != '*' && findstr[i] != '?')
                {
                    if(!findstr[i]){
                        return true;
                    }
                    else if(fname[n++] != findstr[i++]){
                        return false;
                    }
                }
            }
        }
        return !(n < fname.size());
    }
}

//
// check_file_ext
//
// check_file_ext("*.bmp;*.jpg;*.png", "bmp");
// check_file_ext("*.bmp;*.jpg;*.png", ".bmp");
// check_file_ext("*.bmp;*.jpg;*.png", "*.bmp");
//
// 2020-02-19 17:19:16

template<typename _char_t>
bool check_file_ext(const _char_t* begin, const _char_t* end,
    const _char_t* find_begin, const _char_t* find_end, _char_t split = ';')
{
    const _char_t* p = std::search(begin, end, find_begin, find_end);
    if(p == end){
        return false;
    }
    size_t size = find_end - find_begin;
    if(size_t(end - begin) == size){//长度相等
        return true;
    }
    bool equal = true;
    if(begin < p){
        equal &= (*(p - 1) == split) || (*(p - 1) == '.') || (*(p - 1) == '*');
    }
    if(p + size < end){
        equal &= (*(p + size) == split);
    }

    return equal;
}

template<typename _char_t>
bool check_file_ext(const _char_t* list, const _char_t* ext)
{
    return check_file_ext(list, list + strlen(list), ext, ext + strlen(ext));
}

template<typename _char_t>
bool check_file_ext(const cstring<_char_t>& list, const cstring<_char_t>& ext)
{
    return check_file_ext(list.begin(), list.end(), ext.begin(), ext.end());
}

}//end namespace filesystem
}//end namespace cgl

#endif //FILENAME_HPP_20150225143254
