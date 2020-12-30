//---------------------------------------------------------------------------
#pragma hdrstop
#include "platform.hpp"

_CGL_BEGIN

bool IsExtensionSupported(const char *extension)
{
	const char *extensions = 0;
	const char *start;
	const char *where, *terminator;

	/* Extension names should not have spaces. */
	where = strchr(extension, ' ');
	if (where || *extension == '\0')
		return false;

	/* It takes a bit of care to be fool-proof about parsing the
	OpenGL extensions string. Don't be fooled by sub-strings,
	etc. */
	extensions = reinterpret_cast<const char*>(glGetString(GL_EXTENSIONS));
	start = extensions;
	for (;;)
	{
		where = strstr(start, extension );
		if ( !where ) break;

		terminator = where + strlen(extension);
		if ( (where == start || *(where - 1) == ' ') && (*terminator == ' ' || *terminator == '\0') )
			return true;
		start = terminator;
	}

	return false;
}

int opengl_setPixelFormat(HDC hDC)
{
	PIXELFORMATDESCRIPTOR pfd = {0};

    pfd.nSize = sizeof(pfd);
    pfd.nVersion = 1;
	pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 32;
	pfd.cRedBits = 8;
	//pfd.cRedShift = 16;
	pfd.cGreenBits = 8;
	//pfd.cGreenShift = 8;
	pfd.cBlueBits = 8;
	//pfd.cBlueShift = 0;
	pfd.cAlphaBits = 8;
	//pfd.cAlphaShift = 0;
	//pfd.cAccumBits = 64;
	//pfd.cAccumRedBits = 16;
	//pfd.cAccumGreenBits = 16;
	//pfd.cAccumBlueBits = 16;
	//pfd.cAccumAlphaBits = 0;
	pfd.cDepthBits = 16;
	//pfd.cStencilBits = 8;
	pfd.cAuxBuffers = 0;
	pfd.iLayerType = PFD_MAIN_PLANE;
	pfd.bReserved = 0;
	pfd.dwLayerMask = 0;
	pfd.dwVisibleMask = 0;
	pfd.dwDamageMask = 0;

	int pixel_id = ChoosePixelFormat(hDC, &pfd);

	if(!pixel_id)
	{
		//Let's choose a default index.
		pixel_id = 1;
		if(FALSE == DescribePixelFormat(hDC, pixel_id, sizeof(PIXELFORMATDESCRIPTOR), &pfd))
		{
			return -1;
		}
	}
	if(FALSE == SetPixelFormat(hDC, pixel_id, &pfd))
	{
		return -2;
	}
	return 0;
}


/*
inline void glxRecti(int x1, int y1, int x2, int y2, int u1, int v1, int u2, int v2)
{
	float width  = 0;
	float height = 0;
	float fu1, fv1, fu2, fv2;
	glGetTexLevelParameterfv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &width);
	glGetTexLevelParameterfv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &height);
	if(u2 == INT_MAX)u2 = width;
	if(v2 == INT_MAX)v2 = height;
	fu1 = u1/width;
	fv1 = v1/height;
	fu2 = u2/width;
	fv2 = v2/height;
	return glxRectf(x1, y1, x2, y2, fu1, fv1, fu2, fv2);
}

//直角坐标系
inline void draw_rect(float x1, float y1, float x2, float y2, float u1, float v1, float u2, float v2)
{
    //x1,y2   x2,y2
	//
    //x1,y1   x2,y1
    struct vtx{
    	GLfloat vertices[3];
        GLubyte colours[4];
		GLfloat uv[2];
    }vtxLList[] =
	{
		{ {x1, y1, 0.0f}, {255, 255, 0, 255}, {u1, v1} },
		{ {x2, y1, 0.0f}, {255, 255, 0, 255}, {u2, v1} },
		{ {x1, y2, 0.0f}, {255, 255, 0, 255}, {u1, v2} },
		{ {x2, y2, 0.0f}, {255, 255, 0, 255}, {u2, v2} },
    };

	glEnableClientState(GL_VERTEX_ARRAY);
	//glEnableClientState(GL_COLOR_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glVertexPointer(3, GL_FLOAT, sizeof(vtx), vtxLList[0].vertices);
	glColorPointer(4, GL_UNSIGNED_BYTE, sizeof(vtx), vtxLList[0].colours);
    glTexCoordPointer(2, GL_FLOAT, sizeof(vtx), vtxLList[0].uv);

    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_COLOR_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
}

//直角坐标系 GL_TRIANGLES模式
inline void draw_rect2(float x1, float y1, float x2, float y2, float u1, float v1, float u2, float v2)
{
    //x1,y2   x2,y2
    //
	//x1,y1   x2,y1
    struct vtx{
    	GLfloat vertices[3];
        GLubyte colours[4];
	    GLfloat uv[2];
	}vtxLList[] =
    {
    	{ {x1, y1, 0.0f}, {255, 0, 0, 255}, {u1, v1} },//a
        { {x2, y1, 0.0f}, {255, 0, 0, 255}, {u2, v1} },//b
        { {x1, y2, 0.0f}, {255, 0, 0, 255}, {u1, v2} },//c
		{ {x2, y1, 0.0f}, {255, 0, 0, 255}, {u2, v1} },//b
        { {x1, y2, 0.0f}, {255, 0, 0, 255}, {u1, v2} },//c
        { {x2, y2, 0.0f}, {255, 0, 0, 255}, {u2, v2} },//d
    };

	glEnableClientState(GL_VERTEX_ARRAY);
    //glEnableClientState(GL_COLOR_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);

    glVertexPointer(3, GL_FLOAT, sizeof(vtx), vtxLList[0].vertices);
	glColorPointer(4, GL_UNSIGNED_BYTE, sizeof(vtx), vtxLList[0].colours);
    glTexCoordPointer(2, GL_FLOAT, sizeof(vtx), vtxLList[0].uv);

    glDrawArrays(GL_TRIANGLES, 0, 6);

	glDisableClientState(GL_VERTEX_ARRAY);
    //glDisableClientState(GL_COLOR_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
}

//windows坐标系
inline void ui_draw_rect(float x1, float y1, float x2, float y2, float u1, float v1, float u2, float v2)
{
	GLint viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);
	return draw_rect2(x1, viewport[3]-y2, x2, viewport[3]-y1, u1, v1, u2, v2);
}


//UI坐标系 CW
inline void draw_rect3(float x1, float y1, float x2, float y2, float u1, float v1, float u2, float v2)
{
    //x1,y2   x2,y2
    //
    //x1,y1   x2,y1
	struct vtx{
    	GLfloat vertices[3];
        GLubyte colours[4];
	    GLfloat uv[2];
    }vtxLList[] =
	{
		{ {x1, y1, 0.0f}, {255, 0, 0, 255}, {u1, v2} },
		{ {x2, y1, 0.0f}, {255, 0, 0, 255}, {u2, v2} },
		{ {x1, y2, 0.0f}, {255, 0, 0, 255}, {u1, v1} },
		{ {x2, y2, 0.0f}, {255, 0, 0, 255}, {u2, v1} },
	};

	glEnableClientState(GL_VERTEX_ARRAY);
    //glEnableClientState(GL_COLOR_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

    glVertexPointer(3, GL_FLOAT, sizeof(vtx), vtxLList[0].vertices);
	glColorPointer(4, GL_UNSIGNED_BYTE, sizeof(vtx), vtxLList[0].colours);
	glTexCoordPointer(2, GL_FLOAT, sizeof(vtx), vtxLList[0].uv);

	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
}

*/

//---------------------------------------------------------------------------
//古老的函数，怀念一下
#if 0

/*
OpenGL 的纹理相关函数
*/
//GLuint MatrixToTexture(GLubyte *data, long width, long height, DWORD flags);//把一个数组转换成纹理
//GLuint MatrixToMipmaps(const void *data, long width, long height, DWORD flags);//把一个数组转换成MipMap纹理
//GLuint LoadTexture(const char* fn);//加载一张24位色的位图
//GLuint LoadAlphaTexture(const char* fn);


#include <cstdio>

GLuint MatrixToTexture(GLubyte *data, long width, long height, DWORD flags)
{
	GLuint tex = 0;		//纹理的标识
	DWORD  colorbit = 0;

	switch(flags)
	{
	case GL_RED:
	case GL_GREEN:
	case GL_BLUE:
        colorbit = GL_RGBA;
		break;
	case GL_ALPHA:
        colorbit = GL_ALPHA8;
        break;
	case GL_RGB:
    case GL_BGR:
		colorbit = 3;
		break;
    case GL_RGBA:
    case GL_BGRA:
    //case GL_ABGR_EXT:
		colorbit = 4;
		break;
	default:
        return 0;
	}

	glGenTextures(		//为纹理分配内存
	1,					//多少个纹理？
	&tex);				//把其标识赋值于tex

	glBindTexture(		//设置当前纹理
	GL_TEXTURE_2D,		//是二维纹理还是一维纹理？
	tex);				//使用用哪个纹理？

	//设置纹理缩放的插值方式
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glTexImage2D(		//为当前纹理设置像素数据
	GL_TEXTURE_2D,		//是二维纹理还是一维纹理？
	0,					//纹理的细节等级(level of detail)
	colorbit,			//纹理的每个像素包含几种颜色成份
	width,				//图像的宽度
	height,				//图像的高度
	0,					//纹理的边界宽度(只能是0或1)
	flags,				//图像的格式
	GL_UNSIGNED_BYTE,	//图像的数据类型
	data);				//图像的像素数组

	return tex;			//返回生成纹理的标识
}

GLuint MatrixToMipmaps(const void *data, long width, long height, DWORD flags)
{
    GLuint tex = 0;
    DWORD  iformat;

    switch(flags)
    {
	case GL_RED:
	case GL_GREEN:
	case GL_BLUE:
        iformat = GL_RGBA;
		break;
    case GL_ALPHA:
        iformat = GL_ALPHA8;
        break;
	case GL_RGB:
    case GL_BGR:
		iformat = 3;
		break;
    case GL_RGBA:
    case GL_BGRA:
    //case GL_ABGR_EXT:
		iformat = 4;
		break;
	default:
        return 0;
    }

    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);
    gluBuild2DMipmaps(GL_TEXTURE_2D, iformat, width, height, flags, GL_UNSIGNED_BYTE, data);

    return tex;
}

GLuint LoadTexture(const char* fn)
{//载入24位BMP位图
	GLuint tex = 0;
	BITMAPFILEHEADER head = {0};
	BITMAPINFOHEADER info = {0};
	BYTE *bmpdata = NULL;

	FILE *f = fopen(fn, "rb");
	if(f == NULL)
	{
		fclose(f);
		f = NULL;
		return 0;
	}
	fread(&head, sizeof(head), 1, f);
	if(head.bfType!=0x4d42)
	{
		fclose(f);
		return 0;	//文件类型判断
	}
	fseek(f, sizeof(head), SEEK_SET);
	fread(&info, sizeof(info), 1, f);
	if(info.biBitCount != 24)
	{
		fclose(f);
		return 0;
	}

	//读取位图数据
	bmpdata = new BYTE[info.biSizeImage];
	fseek(f, head.bfOffBits, SEEK_SET);
	fread(bmpdata, info.biSizeImage, 1, f);
	fclose(f);

	//创建纹理
	glGenTextures(1,&tex);
	//绑定纹理
	glBindTexture(GL_TEXTURE_2D,tex);
	//设置纹理过滤质量，GL_NEAREST 最近像素过滤  GL_LINEAR 线性过滤
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	//设置opengl像素数据读取格式
	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, info.biWidth, info.biHeight, 0, GL_BGR,
		GL_UNSIGNED_BYTE, bmpdata);
	delete []bmpdata;
	return tex;
}

//---------------------------------------------------------------------------

GLuint LoadAlphaTexture(const char* fn)
{//从24位BMP位图中载入Alpha信息，灰色图像
	GLuint tex;
	BITMAPFILEHEADER head;
	BITMAPINFOHEADER info;

	FILE *f = fopen(fn, "rb");
		if(f == NULL)return 0;
	fread(&head, sizeof(head), 1, f);
		if(head.bfType!=0x4d42)return 0;	//文件类型判断
	fseek(f, sizeof(head), SEEK_SET);
	fread(&info, sizeof(info), 1, f);

	BYTE *bmpdata = new BYTE[info.biSizeImage];
	fseek(f, head.bfOffBits, SEEK_SET);
	fread(bmpdata, info.biSizeImage, 1, f);	//读取位图数据
	BYTE *alpha = new BYTE[info.biSizeImage/3];
	for(int i=0; i<info.biSizeImage/3; ++i)
		alpha[i] = bmpdata[i+i+i];
	delete []bmpdata;

	tex = MatrixToTexture(alpha, info.biWidth, info.biHeight, GL_ALPHA);
	delete []alpha;
	return tex;
}


const char* pfragment = "#version 100\n uniform sampler2D tex;\n void main(){ vec4 color = texture2D(tex,gl_TexCoord[0].st); gl_FragColor = color; }";
const char *vShader = "#version 100\n void main(){ gl_TexCoord[0] = gl_MultiTexCoord0; gl_Position = ftransform(); }";
#endif
//---------------------------------------------------------------------------

_CGL_END
