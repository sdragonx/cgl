/*
 Copyright (c) 2005-2020 sdragonx (mail:sdragonx@foxmail.com)

 math_base.hpp

 2018-10-28 21:57:07

*/
#ifndef MATH_BASE_HPP_20181028215707
#define MATH_BASE_HPP_20181028215707

#include <cgl/public.h>
#include <cgl/vec.hpp>

#include <algorithm>
#include <cstdlib>
#include <vector>

namespace cgl{
namespace math{

#ifndef M_PI
    #define M_PI    3.141592653589793238462 //acos(-1.0)
#endif

#define M_INV_PI    0.318309886183790671538

#ifndef M_PI_2
	#define M_PI_2  1.570796326794896619231
#endif

#ifndef M_PI_4 
	#define M_PI_4  0.785398163397448309616
#endif

#ifndef M_SQRT2
    #define M_SQRT2 1.414213562373095048802
#endif

#define M_RD        0.017453292519943295769    //弧度(radian)
#define M_INV_RD   57.295779513082320876798    //弧度的倒数(reciprocal) 1.0/M_RD

}//end namespace math
}//end namespace cgl

#endif //MATH_BASE_HPP_20181028215707
