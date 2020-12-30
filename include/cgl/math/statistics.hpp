/*
 Copyright (c) 2005-2020 sdragonx (mail:sdragonx@foxmail.com)

 statistics.hpp

 2020-11-09 02:10:55

 Í³¼ÆÑ§

*/
#ifndef STATISTICS_HPP_20201109021055
#define STATISTICS_HPP_20201109021055

#include <cgl/public.h>

namespace cgl{
namespace math{

template<typename T>
class basic_stat
{
public:
    typedef T value_type;

    value_type min_value;
    value_type max_value;
    value_type total;
    value_type count;

public:
    void add(const value_type& value)
    {
    }
};

}//end namespace math
}//end namespace cgl

#endif //STATISTICS_HPP_20201109021055
