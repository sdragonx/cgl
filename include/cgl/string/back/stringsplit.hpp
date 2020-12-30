/*

 stringsplit.hpp

 sdragonx 2006-06-16 00:43:16
 revise  2016.07.18 19:04

*/

#ifndef STRINGSPLIT_HPP_200606161656
#define STRINGSPLIT_HPP_200606161656

#include <algorithm>

namespace cgl{

template<typename char_type, template<typename> class string_type, template<typename> class container>
size_t stringsplit(
	container< string_type<char_type> >& ls,
	typename string_type<char_type>::const_iterator begin,
	typename string_type<char_type>::const_iterator end,
	char_type spliter,
	bool repeat = true)
{
    if(end <= begin)
	{
    	return 0;
	}

    typename string_type<char_type>::const_iterator first = begin;
    typename string_type<char_type>::const_iterator second;
    
    for( ; first<end; )
    {
        second = std::find<string_type<char_type>::const_iterator>(first, end, spliter);
        if(first == second){
            if(repeat)ls.push_back(string_type<char_type>());
        }
        else{
            ls.push_back(string_type<char_type>(first, second));
    	}
        first = second+1;
    }
    if(repeat)
    {
        if(second == end-1){
        	ls.push_back(string_type<char_type>());
        }
    }
    return ls.size();
}

template<typename char_type, template<typename> class string_type, template<typename> class container>
size_t stringsplit(
	container< string_type<char_type> >& ls,
    typename string_type<char_type>::const_iterator begin,
    typename string_type<char_type>::const_iterator end,
    typename string_type<char_type>::const_iterator spliter_begin,
    typename string_type<char_type>::const_iterator spliter_end,
    bool repeat = true)
{
    if(end <= begin || spliter_end<=spliter_begin)
	{
    	return 0;
    }

    typename string_type<char_type>::const_iterator first = begin;
    typename string_type<char_type>::const_iterator second;
    
    for( ; first<end; )
    {
        second = std::find_first_of<string_type<char_type>::const_iterator>(first, end, spliter_begin, spliter_end);
        if(first == second){
            if(repeat)ls.push_back(string_type<char_type>());
        }
        else{
        	ls.push_back(string_type<char_type>(first, second));
        }
        first = second+1;
    }
    if(repeat)
    {
        if(second == end-1){
        	ls.push_back(string_type<char_type>());
        }
    }
    return ls.size();
}

#if 0
//stringsplit(ls, const char*, size, char, bool)
template<typename char_type, template<typename> class string_type, template<typename> class container>
size_t stringsplit(
    container< string_type<char_type> >& ls,
	const char_type* str,
    size_t length,
    const char_type& spliter,
    bool repeat = true)
{
    return stringsplit(ls, str, str+length, spliter, repeat);
}

//stringsplit(ls, const char*, size, const char*, size, bool)
template<typename char_type, template<typename> class string_type, template<typename> class container>
size_t stringsplit(
    container< string_type<char_type> >& ls,
	const char_type* str,
    size_t length,
    const char_type* spliter,
    size_t splength,
    bool repeat = true)
{
    typename string_type<char_type>::const_iterator begin(str),
    typename string_type<char_type>::const_iterator end(str + length),
    typename string_type<char_type>::const_iterator spliter_begin(spliter),
    typename string_type<char_type>::const_iterator spliter_end(spliter + splength),
    return stringsplit(ls, begin, end, spliter_begin, spliter_end, repeat);
}
#endif

//stringsplit(ls, string, char, bool)
template<typename char_type, template<typename> class string_type, template<typename> class container>
inline size_t stringsplit(
    container< string_type<char_type> > &ls,
	const string_type<char_type>& str,
    const char_type& spliter,
    bool repeat = true)
{
    return stringsplit(ls, str.begin(), str.end(), spliter, repeat);
}

//stringsplit(ls, string, string, bool)
template<typename char_type, template<typename> class string_type, template<typename> class container>
inline size_t stringsplit(
    container< string_type<char_type> >& ls,
	const string_type<char_type>& str,
    const string_type<char_type>& spliter,
    bool repeat = true)
{
    return stringsplit(ls, str.begin(), str.end(), spliter.begin(), spliter.end(), repeat);
}



}; // end namespace cgl;

#endif //STRINGSPLIT_HPP_200606161656
