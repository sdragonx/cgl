/*
 Copyright (c) 2005-2020 sdragonx (mail:sdragonx@foxmail.com)

 shader.basic.hpp

 2020-04-08 23:48:30

 lowp mediump highp

 names:
 texture_id
 texture_layers
 texture_uv

 vertex

 lights

*/
#ifndef BASIC_SHADER_HPP_20200408234830
#define BASIC_SHADER_HPP_20200408234830

#include <cgl/public.h>

namespace cgl{
namespace graphics{


#ifdef CGL_PLATFORM_GLES123
//300 core

const static char* basic_vs = __stringify(

#version 300 es\n

precision mediump float;

//#ifdef GL_ES\n
//precision mediump float;
//#else\n
//#version 130\n
//precision lowp float;
//#endif\n

//matrix
uniform mat4 u_mvp;

//vertex
//attribute vec4 a_position;
in vec4 a_position;
//attribute vec4 a_color;
//attribute vec2 a_uv;
//attribute vec3 a_normal;

//varying
//varying vec4 v_color;
//varying vec2 v_uv;

void main(void){
    gl_Position = a_position;// u_mvp * a_position;
//    v_color = a_color;
//    v_uv = a_uv;
}
);


const static char* basic_fs = __stringify(

#version 300 es\n

//#ifdef GL_ES\n
precision mediump float;
//#endif\n

//texture
uniform int texIndex;
uniform sampler2D texImage[4];

//varying vec4 v_color;
//varying vec2 v_uv;

out vec4 my_FragColor;

void main(void)
{
    vec4 color = vec4(1.0, 0.0, 0.0, 1.0);
    if(texIndex != 0){
        //color = texture2D(texImage[0], v_uv);
    }

    my_FragColor = color;// * v_color;
}
);

#else

//normal

const static char* basic_vs = __stringify(

//#version 300 \n

precision mediump float;

//matrix
uniform mat4 u_mvp;

//vertex
attribute vec4 a_position;
attribute vec4 a_color;
attribute vec2 a_uv;
attribute vec3 a_normal;

//varying
varying vec4 v_color;
varying vec2 v_uv;

void main(void){
    gl_Position = u_mvp * a_position;
    v_color = a_color;
    v_uv = a_uv;
}
);


const static char* basic_fs = __stringify(

//#version 300 \n
precision mediump float;

//texture
uniform int texIndex;
uniform sampler2D texImage[4];

varying vec4 v_color;
varying vec2 v_uv;

void main(void)
{
    vec4 color = vec4(1.0, 0.0, 0.0, 1.0);
    if(texIndex != 0){
        color = texture2D(texImage[0], v_uv);
    }

    gl_FragColor = color * v_color;
}
);

#endif

}//end namespace graphics
}//end namespace cgl

#endif //BASIC_SHADER_HPP_20200408234830