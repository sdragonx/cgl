/*
 Copyright (c) 2005-2020 sdragonx (mail:sdragonx@foxmail.com)

 script.hpp

 2019-06-01 04:49:36

*/
#ifndef SCRIPT_HPP_20190601044936
#define SCRIPT_HPP_20190601044936

#include <cgl/string.hpp>

namespace cgl{

int parseAlign(const string_t& str)
{
    if(str.empty()){
        return 0;
    }
    int align = 0;
    if(!str.icompare("center")){
        align = CGL_CENTER;
    }
    else if(!str.icompare("top")){
        align = CGL_TOP|CGL_HORIZONTAL;//上居中
    }
    else if(!str.icompare("left")){
        align = CGL_LEFT|CGL_VERTICAL;//左居中
    }
    else if(!str.icompare("right")){
        align = CGL_RIGHT|CGL_VERTICAL;//右居中
    }
    else if(!str.icompare("bottom")){
        align = CGL_BOTTOM|CGL_HORIZONTAL;//下居中
    }
    return align;
}

}//end namespace cgl

#endif //SCRIPT_HPP_20190601044936
