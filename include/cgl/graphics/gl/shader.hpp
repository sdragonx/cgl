/*
 Copyright (c) 2005-2020 sdragonx (mail:sdragonx@foxmail.com)

 shader.hpp

 2018-05-29 01:38:47

*/
#ifndef SHADER_HPP_20180529013847
#define SHADER_HPP_20180529013847

#include "gl.hpp"

namespace cgl{
namespace graphics{

// create a shader object, load the shader source, and compile the shader.
GLuint compile_shader(GLenum type, const char* name, const char *source)
{
    GLuint shader;

    // create the shader object
    shader = glCreateShader(type);

    if(!shader){
        CGL_LOG_ERROR("glCreateShader failed.");
        return 0;
    }

    // load the shader source
    glShaderSource(shader, 1, &source, NULL);

    // Compile the shader
    glCompileShader(shader);

    // check the compile status
    GLint compiled;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
    if(!compiled){
        GLint infoLen = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);
        if(infoLen > 1){
            char* infoLog = (char *) malloc(sizeof(char) * infoLen);
            glGetShaderInfoLog( shader, infoLen, NULL, infoLog );
            cstring<char> str(infoLog, infoLen);
            std::vector< cstring<char> > ls;
            str.split(ls, "\r\n");
            CGL_LOG_ERROR("glCompileShader failed : %s", name);
            for(size_t i=0; i<ls.size(); ++i){
                CGL_LOG_ERROR("%s", ls[i].c_str());
            }
            free(infoLog);
        }

        glDeleteShader(shader);
        return 0;
    }

    return shader;
}

GLuint compile_program(const char* vsSource, const char* fsSource)
{
    GLuint program;
    GLuint vs;
    GLuint fs;

    // load the vertex/fragment shaders
    vs = compile_shader(GL_VERTEX_SHADER, "vertex shader", vsSource);
    fs = compile_shader(GL_FRAGMENT_SHADER, "fragment shader", fsSource);

    // create the program object
    program = glCreateProgram();

    if(!program){
        CGL_LOG_ERROR("glprogram create failed.");
        return CGL_ERROR;
    }

    glAttachShader(program, vs);
    glAttachShader(program, fs);

    // link the program
    glLinkProgram(program);

    // check the link status
    GLint linked;
    glGetProgramiv( program, GL_LINK_STATUS, &linked );

    if(!linked){
        GLint infoLen = 0;
        glGetProgramiv( program, GL_INFO_LOG_LENGTH, &infoLen );
        if ( infoLen > 1 ){
            char* infoLog = (char*)malloc (sizeof(char) * infoLen );

            glGetProgramInfoLog ( program, infoLen, NULL, infoLog );
            CGL_LOG_ERROR("glprogram linking program : %s", infoLog);

            free ( infoLog );
        }
        glDeleteProgram(program);
        return CGL_ERROR;
    }

    // delete shader object
    glDetachShader(program, vs);
    glDeleteShader(vs);

    glDetachShader(program, fs);
    glDeleteShader(fs);

    return program;
}

}//end namespace graphics
}//end namespace cgl

#endif //SHADER_HPP_20180529013847
