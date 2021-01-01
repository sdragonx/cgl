/*
 Copyright (c) 2005-2020 sdragonx (mail:sdragonx@foxmail.com)

 algorithm.hpp

 less_pointer
 less_string

 find_last_of
 find_last_not_of

 find_all

 is_exist
 delete_all
 dispose_all

 2007-02-28 22:42:46

*/
#ifndef ALGORITHM_HPP_20070228224246
#define ALGORITHM_HPP_20070228224246

#include <algorithm>
#include <iterator>
#include <map>
#include <utility>      //std::swap
#include <vector>
#include <cgl/public.h>

namespace cgl{

//---------------------------------------------------------------------------

//比较两个指针指向内容位置的大小
template<typename T>
bool less_pointer(const T* left, const T* right)
{
    return (*left) < (*right);
}

template<typename T>
bool less_string(const T* left, const T* right)
{
    return 0 < cscmp(left, right);
}

template<typename T>
bool string_is_equal(const T* left, const T* right)
{
    return cscmp(left, right) == 0;
}

template<typename T>
bool string_is_equali(const T* left, const T* right)
{
    return csicmp(left, right) == 0;
}
//---------------------------------------------------------------------------

//template<typename It, typename T>
//inline binsearch(It begin, It end, T value)
//{
//    return std::lower_bound(begin, end, value);
//}

template<typename It, typename findIt>
inline It find_last_of(It begin, It end, findIt findBegin, findIt findEnd)
{
    CGL_ASSERT(begin<=end && findBegin<=findEnd);
    typedef std::reverse_iterator<It> iterator;
    return std::find_first_of(iterator(end), iterator(begin), findBegin, findEnd).base() - 1;
}

template <class It, class findIt, class opt_func>
inline It find_last_of(It begin, It end, findIt findBegin, findIt findEnd, opt_func comp)
{
    CGL_ASSERT(begin<=end && findBegin<=findEnd && comp);
    typedef std::reverse_iterator<It> iterator;
    return std::find_first_of(iterator(end), iterator(begin), findBegin, findEnd, comp).base() - 1;
}

template<typename It, typename T>
inline It find_first_not_of(It begin, It end, const T& value)
{
    CGL_ASSERT(begin<=end);
    for(It i(begin); i!=end; ++i){
        if(*i != value){
            return i;
        }
    }
    return end;
}

template<typename It, typename findIt>
inline It find_first_not_of(It begin, It end, findIt findBegin, findIt findEnd)
{
    CGL_ASSERT(begin<=end && findBegin<=findEnd);
    for(It i=begin; i != end; ++i){
        if(std::find(findBegin, findEnd, *i) == findEnd){
            return i;
        }
    }
    return end;
}

template<typename It, typename T>
inline It find_last_not_of(It begin, It end, const T& value)
{
    CGL_ASSERT(begin<=end);
    --begin;
    for(It i = end-1; i!=begin; --i){
        if(*i != value){
            return i;
        }
    }
    return end;
}

template<typename It, typename findIt>
inline It find_last_not_of(It begin, It end, findIt findBegin, findIt findEnd)
{
    CGL_ASSERT(begin<=end && findBegin<=findEnd);
    typedef std::reverse_iterator<It> iterator;
    for(iterator i(end); i != iterator(begin); ++i){
        if(std::find(findBegin, findEnd, *i) == findEnd){
            return i.base() - 1;
        }
    }
    return end;
}

//---------------------------------------------------------------------------
//find_all<ANY, T>
//编译不过去

//??
/*
template<typename T, typename Co>
TYPENAME Co::iterator find_all(Co& ls, const T& value)
{
    return std::find(ls.begin(), ls.end(), value);
}
*/

template<typename ANY, typename T>
TYPENAME ANY::iterator find_all(ANY& ls, const T& value)
{
    return std::find<TYPENAME ANY::iterator>(ls.begin(), ls.end(), value);
}

template<typename ANY, typename T>
TYPENAME ANY::const_iterator find_all(const ANY& ls, const T& value)
{
    return std::find<TYPENAME ANY::const_iterator>(ls.begin(), ls.end(), value);
}

/*
template<class T, class Alloc>
std::vector<T, Alloc>::iterator find_all(std::vector<T, Alloc>& ls, const T& value)
{
    return std::find(ls.begin(), ls.end(), value);
}

template<class T, class Alloc>
TYPENAME std::vector<T, Alloc>::const_iterator find_all(std::vector<T, Alloc>& ls, const T& value)
{
    return std::find(ls.begin(), ls.end(), value);
}
*/

//判断值是否存在std::map中
template<class Key, class T, class Proc, class Alloc>
TYPENAME std::map<Key, T, Proc, Alloc>::iterator
    find_all(std::map<Key, T, Proc, Alloc>& map, const Key& id)
{
    return map.find(id);
}

template<class Key, class T, class Proc, class Alloc>
TYPENAME std::map<Key, T, Proc, Alloc>::const_iterator
    find_all(const std::map<Key, T, Proc, Alloc>& map, const Key& id)
{
    return map.find(id);
}

//---------------------------------------------------------------------------
//is_exists<ANY, T>
//判断值是否存在

template<typename ANY, typename T>
bool is_exists(const ANY& ls, const T& value)
{
    return find_all<ANY, T>(ls, value) != ls.end();
}

template<class Key, class T, class Proc, class Alloc>
bool is_exists(const std::map<Key, T, Proc, Alloc>& map, const Key& id)
{
    return map.find(id) != map.end();
}

//---------------------------------------------------------------------------
//delete_all<ANY>
//删除列表中的所有指针

template<class ANY>
void delete_all(ANY& ls)
{
    TYPENAME ANY::iterator i = ls.begin();
    for( ; i != ls.end(); ++i){
        delete (*i);
        (*i) = null;
    }
}

//释放std::map中的指针
template<class Key, class T, class Proc, class Alloc>
void delete_all(std::map<Key, T, Proc, Alloc>& map)
{
    TYPENAME std::map<Key, T, Proc, Alloc>::iterator i;
    for(i = map.begin(); i != map.end(); ++i){
        delete i->second;
        i->second = null;
    }
}

//---------------------------------------------------------------------------
//dispose_all<ANY>
//清理列表中的所有元素

template<class ANY>
void dispose_all(ANY& ls)
{
    TYPENAME ANY::iterator i = ls.begin();
    for( ; i != ls.end(); ++i){
        (*i)->dispose();
    }
}

//释放std::map中的对象(cgl only)
template<class Key, class T, class Proc, class Alloc>
void dispose_all(std::map<Key, T, Proc, Alloc>& map)
{
    TYPENAME std::map<Key, T, Proc, Alloc>::iterator i;
    for(i = map.begin(); i != map.end(); ++i){
        i->second.dispose();
    }
}

//---------------------------------------------------------------------------
// find_obj<ls, T>
// 查找列表，成功返回指针，失败返回null
// find_ptr find_obj find_unit

template<typename ANY, typename T>
T* find_obj(ANY& ls, const T& value)
{
    TYPENAME ANY::iterator i = find_all(ls, value);
    if(i != ls.end()){
        return &(*i);
    }
    return null;
}

//在std::map中查找值，没有找到返回null
template<class Key, class T, class Proc, class Alloc>
T* find_obj(std::map<Key, T, Proc, Alloc>& obj, const Key& id)
{
    TYPENAME std::map<Key, T, Proc, Alloc>::iterator i = obj.find(id);
    if(i != obj.end()){
        return &i->second;
    }
    return null;
}



/*
template<class T, class Alloc>
bool is_exist(const std::vector<T, Alloc>& obj, const T& value)
{
    return std::find(obj.begin(), obj.end(), value) != obj.end();
}

//判断值是否存在std::map中
template<class Key, class T, class Proc, class Alloc>
bool is_exist(const std::map<Key, T, Proc, Alloc>& map, const Key& id)
{
    return map.find(id) != map.end();
}
*/
/*
// std::vector

template<class T, class Alloc>
bool is_exist(const std::vector<T, Alloc>& ls, const T& value)
{
    return std::find(ls.begin(), ls.end(), value) != ls.end();
}

template<class T, class Alloc>
void delete_all(std::vector<T, Alloc>& ls)
{
    for(size_t i=0; i<ls.size(); ++i){
        delete ls[i];
        ls[i] = null;
    }
}

template<class T, class Alloc>
void dispose_all(std::vector<T, Alloc>& ls)
{
    for(size_t i=0; i<ls.size(); ++i){
        ls[i].dispose();
    }
}
*/

}//end namespace cgl

#endif //ALGORITHM_HPP_20070228224246
