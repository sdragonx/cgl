/*
 Copyright (c) 2005-2020 sdragonx (mail:sdragonx@foxmail.com)

 for_each.hpp

 2020-01-25 08:54:47

*/
#ifndef FOR_EACH_HPP_20200125085447
#define FOR_EACH_HPP_20200125085447

#include <cgl/public.h>
#include <functional>

namespace cgl{

//std::function<bool (const int & , const int &)>

template <typename InputIterator, typename Function>
inline Function for_each(InputIterator first, InputIterator last, Function function)
{
    for(; first != last; ++first){
        function(*first);
    }
    return function;
}

template <typename Container, typename Function>
inline Function for_each(Container& container, Function function)
{
    TYPENAME container::iterator i = container.begin();
    for(; i != container.end(); ++i){
        function(*i);
    }
    return function;
}

}//end namespace cgl

#endif //FOR_EACH_HPP_20200125085447
