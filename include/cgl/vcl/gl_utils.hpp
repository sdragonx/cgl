/*
 Copyright (c) 2005-2020 sdragonx (mail:sdragonx@foxmail.com)

 vcl_gl_utils.hpp

 2018-08-26 08:55:16

*/
#ifndef VCL_GL_UTILS_HPP_20180826085516
#define VCL_GL_UTILS_HPP_20180826085516

#include <cgl/graphics.hpp>
#include <vcl.h>

#ifndef GL_MIN
#define GL_MIN 0
#define GL_MAX 0
#endif

namespace cgl{

GLenum gluGetBlendFunc(int index)
{
    static const int blendfunc[] = {GL_ZERO, GL_ONE, GL_SRC_COLOR, GL_ONE_MINUS_SRC_COLOR, GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_DST_COLOR, GL_ONE_MINUS_DST_COLOR, GL_DST_ALPHA, GL_ONE_MINUS_DST_ALPHA, GL_SRC_ALPHA_SATURATE};
    return blendfunc[index];
}

GLenum gluGetBlendFunc(TComboBox* combobox)
{
    return gluGetBlendFunc(combobox->ItemIndex);
}

void gluInitBlendFunc(TComboBox* combobox, int mode = CGL_SRC_COLOR)
{
    static const TCHAR* blend = TEXT("GL_ZERO,GL_ONE,GL_SRC_COLOR,GL_ONE_MINUS_SRC_COLOR,GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA,GL_DST_COLOR,GL_ONE_MINUS_DST_COLOR,GL_DST_ALPHA,GL_ONE_MINUS_DST_ALPHA,GL_SRC_ALPHA_SATURATE");
    combobox->Items->Delimiter = ',';
    combobox->Items->DelimitedText = blend;
    combobox->ItemIndex = (mode == CGL_SRC_COLOR) ? 4 : 5;
}

GLenum gluGetBlendEquation(int index)
{
    int blendEquation[] = {GL_FUNC_ADD, GL_FUNC_SUBTRACT, GL_FUNC_REVERSE_SUBTRACT, GL_MIN, GL_MAX};
    return blendEquation[index];
}

void gluInitBlendEquation(TComboBox* combobox)
{
    static const TCHAR* bEquation = TEXT("GL_FUNC_ADD,GL_FUNC_SUBTRACT,GL_FUNC_REVERSE_SUBTRACT,GL_MIN,GL_MAX");
    combobox->Items->Delimiter = ',';
    combobox->Items->DelimitedText = bEquation;
    combobox->ItemIndex = 0;
}

}//end namespace cgl

#endif //VCL_GL_UTILS_HPP_20180826085516
