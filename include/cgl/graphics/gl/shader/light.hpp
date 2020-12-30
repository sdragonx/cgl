/*
 Copyright (c) 2005-2020 sdragonx (mail:sdragonx@foxmail.com)

 shader.light.hpp

 2020-04-08 23:54:57

*/
#ifndef LIGHT_SHADER_HPP_20200408235457
#define LIGHT_SHADER_HPP_20200408235457

#include <cgl/public.h>

namespace cgl{
namespace graphics{

//如何判断光源是在物体后面
const static char* light_vs = __stringify(

struct LIGHT
{
    vec4 position;
    vec4 direction;
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
    float shine;
};

//matrix
uniform mat4 u_mvp;
uniform mat4 u_modelview;
//uniform mat4 u_projection;
uniform vec4 u_camera;

//uniform vec4 ambient;//全局环境光
uniform LIGHT light;

//vertex
attribute vec4 a_position;
attribute vec4 a_color;
attribute vec2 a_uv;
attribute vec3 a_normal;

//varying
varying vec4 v_color;
varying vec2 v_uv;
varying vec4 v_light;

varying vec3 vtxPos;
varying vec3 lightPos;
varying vec3 viewPos;

vec4 calc_light(vec3 normal)
{
    vec3 lightColor = vec3(1.0, 1.0, 1.0);//光的颜色
    float ambientStrength = 0.0;
    float diffuseStrength = 0.5;
    float specularStrength = 0.5;//大于0.5就会变成聚光灯
    float specularShine = 4.0;    //控制聚光范围

    vec3 final;

    //全局环境光照
    vec3 ambient = lightColor * ambientStrength;

    //漫反射
    //vec3 normal = a_normal;
    vec3 lightDir = normalize(-light.position);//
//    vec3 lightDir = normalize(lightPos - vtxPos);//点和光的距离

    float NdotL = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = lightColor * (diffuseStrength * NdotL);
    //diffuse = clamp(diffuse, 0.0, 1.0);
    //diffuse = vec4(1.0) - diffuse;

    final = ambient + diffuse;

    if(NdotL > 0){//镜面光
        vec3 viewDir = normalize(viewPos - vtxPos);//点和相机距离

        //vec3 reflectDir = normalize(reflect(-lightDir, normal));
        //vec3 reflectDir = reflect(-lightDir, normal);
        //float spec = pow(max(dot(viewDir, reflectDir), 0.0), specularShine); //shininess

        vec3 halfVector = normalize(viewDir + lightDir);//半向量
        float NdotH = dot(normal.xyz,halfVector);//求点积
        float spec=max(0.0,pow(NdotH, specularShine));//光线衰减

        vec3 specular = lightColor * (specularStrength * spec);
        //specular = clamp(specular, 0.0, 1.0);

        final += specular;
    }

    return vec4(final, 1.0);
}

void main(void)
{
    gl_TexCoord[0] = vec4(a_uv, 0.0, 0.0);
    gl_TexCoord[1] = gl_MultiTexCoord1;

    v_uv = a_uv;
    v_color = a_color;

    gl_Position = u_mvp * a_position;

    lightPos = (u_modelview * light.position).xyz;
    vtxPos = (u_modelview * a_position).xyz;
    viewPos = (u_modelview * u_camera).xyz;

    vec3 vtxNormal = gl_NormalMatrix * a_normal;
    //------------------------------------

    v_light = calc_light(vtxNormal);
}

);

//---------------------------------------------------------------------------

const static char* light_fs = __stringify(

uniform int texIndex;
uniform sampler2D texImage[4];

varying vec4 v_color;
varying vec2 v_uv;

varying vec3 vtxPos;
varying vec3 lightPos;
varying vec3 viewPos;

varying vec4 v_light;

void main()
{
    vec4 color = vec4(1.0, 1.0, 1.0, 1.0);
    if(texIndex != 0){
        color = texture2D(texImage[0], v_uv);
    }
    gl_FragColor = color * v_light;
}

);

}//end namespace graphics
}//end namespace cgl

#endif //LIGHT_SHADER_HPP_20200408235457
