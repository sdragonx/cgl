/*
 Copyright (c) 2005-2020 sdragonx (mail:sdragonx@foxmail.com)

 stringsplit.hpp

 2006-06-16 00:43:16
 revise 2018/7/28 19:42

 function list:

 size_t stringsplit(container, const char_type*, size_t, char_type, bool)
 size_t stringsplit(container, const char_type*, size_t, const char_type*, size_t, bool)

 ���Ǹ�ģ�庯����֧�ֱ�׼������ĸ����������ͱ�׼�ַ�����
 ��θ��£�Ϊ��ƽ̨�����ԣ�������ֻʣ�������ĺ�����
 ������3�������������ͨ�� (gcc7.2.0��c++builder6.0��C++Builder2010)

 ���磺
 std::string s = "abc,,123";
 std::vector<std::string> ls;
 stringsplit(ls, s.c_str(), s.length(), ',', true);
 ���repeat == true��s�ͱ��ָ��Ϊ3���ַ������ڶ����ַ����ǿյ�
 ���repeat == false��s�ͱ��ָ��2���ַ���

 Ҳ֧���������������磺
 std::list<std::string> ls;
 stringsplit(ls, s.c_str(), s.size(), ',', true);

 std::deque<std::string> ls;
 stringsplit(ls, s.c_str(), s.size(), ',', true);

 ��������������������C�ַ�����ʽд�ģ�Ҳ֧���������͵��ַ�����
 ֻҪ����ַ����й��캯��String(const char*, size_t length)����

 const char* s = "a,b,,c";
 std::vector<std::string> ls;
 stringsplit(ls, s, strlen(s), ',', true);

*/

#ifndef STRINGSPLIT_HPP_200606161656
#define STRINGSPLIT_HPP_200606161656

#include <algorithm>

namespace cgl{

//
// size_t stringsplit<char_type>(container, const char_type*, size_t, char_type, bool)
//
template<typename char_type, class container>
size_t stringsplit(
    container& ls,
    const char_type* str,
    size_t size,
    char_type spliter,
    bool repeat = true)
{
    typedef const char_type* const_iterator;
    typedef typename container::value_type string_type;

    const_iterator begin = str;
    const_iterator end = begin + size;
    const_iterator first = begin;
    const_iterator second;

    for( ; first<end; )
    {
        second = std::find<const_iterator>(first, end, spliter);
        if(first == second){
            if(repeat)ls.push_back(string_type());
        }
        else{
            ls.push_back(string_type(first, second - first));
        }
        first = second+1;
    }
    if(repeat)
    {
        if(second == end-1){
            ls.push_back(string_type());
        }
    }
    return ls.size();
}

//
// size_t stringsplit<char_type>(container, const char_type*, size_t, const char_type*, size_t, bool)
//
template<typename char_type, typename container>
size_t stringsplit(container& ls,
    const char_type* str,
    size_t size,
    const char_type* spliter,
    size_t spliter_size,
    bool repeat = true)
{
    typedef typename container::value_type string_type;
    typedef const char_type* const_iterator;

    const_iterator end = str + size;
    const_iterator first = str;
    const_iterator second;

    for( ; first<end; )
    {
        second = std::find_first_of<const_iterator>(first, end, spliter, spliter + spliter_size);
        if(first == second){
            if(repeat)ls.push_back(string_type());
        }
        else{
            ls.push_back(string_type(first, second));
        }
        first = second+1;
    }
    if(repeat)
    {
        if(second == end-1){
            ls.push_back(string_type());
        }
    }
    return ls.size();
}

/*

������չ������Ϊ�ο�
�е�ƽ̨std::iterator��һ���࣬�е����char*ָ��
����ƽ̨ʵ�ַ�ʽ��һ������ʱ�޷�ͳһ


//
// size_t stringsplit<string_type>(container, const_iterator, const_iterator, char_type, bool)
//
template<typename string_type, typename container>
size_t stringsplit(
    container& ls,
    typename string_type::const_iterator begin,
    typename string_type::const_iterator end,
    typename string_type::value_type spliter,
    bool repeat = true)
{
    return stringsplit(ls, &*begin, end - begin, spliter, repeat);
}

//
// size_t stringsplit<string_type>(container, string_type, char, bool)
//
template<typename string_type, typename container>
size_t stringsplit(
    container& ls,
    const string_type& str,
    typename string_type::value_type spliter,
    bool repeat = true)
{
    return stringsplit(ls, str.c_str(), str.size(), spliter, repeat);
}

//
// size_t stringsplit<string_type>(container, const_iterator, const_iterator, const_iterator, const_iterator, bool)
//
template<typename string_type, typename container>
size_t stringsplit(
    container& ls,
    typename string_type::const_iterator begin,
    typename string_type::const_iterator end,
    typename string_type::const_iterator spliter_begin,
    typename string_type::const_iterator spliter_end,
    bool repeat = true)
{
    return stringsplit(ls, &*begin, end - begin, &*spliter_begin, spliter_end - spliter_begin, repeat);
}

//
// size_t stringsplit<string_type>(container, string_type, string_type, bool)
//
template<typename string_type, typename container>
size_t stringsplit(
    container& ls,
    const string_type& str,
    const string_type& spliter,
    bool repeat = true)
{
    return stringsplit(ls, str.c_str(), str.size(), spliter.c_str(), spliter.size(), repeat);
}
*/

}; // end namespace cgl;

#endif //STRINGSPLIT_HPP_200606161656
