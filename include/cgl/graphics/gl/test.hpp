/*
 Copyright (c) 2005-2020 sdragonx (mail:sdragonx@foxmail.com)

 gltest.hpp

 2020-04-04 18:48:53

*/
#ifndef GLTEST_HPP_20200404184853
#define GLTEST_HPP_20200404184853

#include <cgl/public.h>
#include "shader.hpp"

namespace cgl{
namespace graphics{

void test_shader()
{
	GLint mTexture2DUniformLocation;

	const char* vertexShaderSource = __stringify(
		precision highp float;
		attribute vec4 position;
		varying vec2 texcoord;
		void main()
		{
			gl_Position = vec4(position.xy, 0.0, 1.0);
			texcoord = (position.xy * 0.5) + 0.5;
		});

	const char* fragmentShaderSource2D = __stringify(
		precision highp float;
		uniform sampler2D tex;
		varying vec2 texcoord;

		void main()
		{
			gl_FragColor = texture2D(tex, texcoord);
		});

	GLint program;
	program = compile_program(vertexShaderSource, fragmentShaderSource2D);
	mTexture2DUniformLocation = glGetUniformLocation(program, "tex");

	uint8_t textureInitData[16] = {
		255, 0,   0,   255,  // Red
		0,   255, 0,   255,  // Green
		0,   0,   255, 255,  // Blue
		255, 255, 0,   255   // Red + Green
	};

	// Create a simple RGBA texture
	GLuint tex = 0;
	glGenTextures(1, &tex);
	glBindTexture(GL_TEXTURE_2D, tex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 2, 2, 0, GL_RGBA, GL_UNSIGNED_BYTE,
				 textureInitData);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	// Draw a quad using the texture
	glClear(GL_COLOR_BUFFER_BIT);
	glUseProgram(program);
	glUniform1i(mTexture2DUniformLocation, 0);

	GLint positionLocation = glGetAttribLocation(program, "position");
	const GLfloat vertices[] =
	{
		-1.0f,  1.0f, 0.5f,
		-1.0f, -1.0f, 0.5f,
		 1.0f, -1.0f, 0.5f,
		-1.0f,  1.0f, 0.5f,
		 1.0f, -1.0f, 0.5f,
		 1.0f,  1.0f, 0.5f,
	};

	GLuint vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(positionLocation, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(positionLocation);
	glDrawArrays(GL_TRIANGLES, 0, 6);

	glUseProgram(0);
	glDeleteProgram(program);
	glDeleteTextures(1, &tex);
	glDeleteBuffers(1, &vbo);
}

}//end namespace graphics
}//end namespace cgl

#endif //GLTEST_HPP_20200404184853
