/*
 Copyright (c) 2005-2020 sdragonx (mail:sdragonx@foxmail.com)

 fps.hpp

 2018-01-08 06:31:24

*/
#ifndef FPS_HPP_20180108063124
#define FPS_HPP_20180108063124

#include <ctime>

namespace cgl{

int fps_stats()
{
    using namespace std;
    static int fps_total = 0;
    static int fps = 0;
    static int t = clock();
    ++fps;
    if(clock() - t > 1000){
        fps_total = fps;
        fps = 0;
        t = clock();
    }
    return fps_total;
}

}// end namespace cgl

#endif //FPS_HPP_20180108063124
