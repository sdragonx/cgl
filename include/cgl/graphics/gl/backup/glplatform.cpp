//---------------------------------------------------------------------------
#pragma hdrstop
#include "glplatform.hpp"
#include "imagemgr.hpp"
#include "basefont.hpp"

_CGL_BEGIN

static OpenGL_Platform opengl = OpenGL_Platform();
//glImageManager& gl_platform<>::images = singleton<glImageManager>::instance();//glImageManager();
glImageManager OpenGL_Platform::images = glImageManager();
glTexState OpenGL_Platform::texture = glTexState();


bool isGLExtensionSupported(const char *extension)
{
	const char *extensions;
	const char *start;
	const char *where, *terminator;
	extensions = NULL;
	/* Extension names should not have spaces. */
	where = strchr(extension, ' ');
	if (where || *extension == '\0')
		return false;

	/* It takes a bit of care to be fool-proof about parsing the
	OpenGL extensions string. Don't be fooled by sub-strings,
	etc. */
	extensions = glGetString(GL_EXTENSIONS);
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

OpenGL_Platform::OpenGL_Platform()
{
	m_hwnd = NULL;
	m_winstyle = 0;
	m_fullscreen = false;
}


OpenGL_Platform::~OpenGL_Platform()
{
	this->shutdown();
}

int glxSetPixelFormat(HDC hDC)
{
	PIXELFORMATDESCRIPTOR pfd = {0};
	int pixel_id = 0;

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

	pixel_id = ChoosePixelFormat(hDC, &pfd);

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


int OpenGL_Platform::create(HWND hwnd)//, bool doublebuffer)
{
	HDC hDC = NULL;
	HGLRC hGLRC = NULL;

	m_hwnd = hwnd;
	hDC = GetDC(hwnd);
	m_winstyle = GetWindowLong(hwnd, GWL_STYLE);

	if(glxSetPixelFormat(hDC)<0)
	{
        return -1;
    }

	//opengl启动
	hGLRC = wglCreateContext(hDC);
	wglMakeCurrent(hDC, hGLRC);

	//初始化glew
	glewInit();

	//设置默认状态

	//启用纹理映射
	glEnable(GL_TEXTURE_2D);
	//纹理映射效果
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//启用alpha混合
	glEnable(GL_ALPHA_TEST);

	//打开混合
	glEnable(GL_BLEND);
	//alpha混合函数
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//优化透视
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	//平滑过滤
	glShadeModel(GL_SMOOTH);//GL_FLAT GL_SMOOTH

	//启用剪裁测试
	glEnable(GL_SCISSOR_TEST);

	//glEnable(GL_LINE_SMOOTH);//线反走样//有问题

	//激活抖动
	//glEnable(GL_DITHER);
	//glDisable(GL_DITHER);


	glEnable(GL_DEPTH_TEST);//深度测试
	glClearDepth(1.0);//设置清除深度缓冲区所用的值
	glDepthFunc(GL_LESS);//设置深度测试函数
	glDepthFunc(GL_LEQUAL);

	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);//设置多边形显示模式为正反面都是填充显示
    glPolygonMode(GL_FRONT, GL_FILL);//GL_POINT GL_LINE GL_FILL
	glPolygonMode(GL_BACK, GL_LINE);//GL_POINT GL_LINE GL_FILL

	//glFrontFace(GL_CW);//设置正面方式[GL_CCW|GL_CW]，默认GL_CCW
	//glEnable(GL_CULL_FACE);
	//glCullFace(GL_BACK);//设置舍弃面

	/*
	glFrontFace(GL_CCW);//设置正面方式
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);//设置舍弃面


	//用OpenGL实现支持的最大各异向程度设置最大各异向程度参数
  glTexParameterf(GL_TEXTURE_2D, 0x84FE, 16);
  // 注意：这里的 xxx 代表 各向异性过滤 的异向程度参数
  // 你可以设置为：4 , 8 , 16 等。这里也可以在 InitGL() 驱动 OpenGL 函数里写上
  //int LrgSupAni;
  //glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &LrgSupAni);
  //那么 xxx 就得小于或等于 LrgSupAni。



    GLfloat LightAmbient[]= { 0.5f, 0.5f, 0.5f, 1.0f }; //环境光参数 ( 新增 )
    GLfloat LightDiffuse[]= { 1.0f, 1.0f, 1.0f, 1.0f }; // 漫射光参数 ( 新增 )
    GLfloat LightPosition[]= { 3.0f, 2.0f, 0.0f, 1.0f }; // 光源位置 ( 新增 )

	glLightfv(GL_LIGHT1, GL_AMBIENT, LightAmbient); // 设置环境光
	glLightfv(GL_LIGHT1, GL_DIFFUSE, LightDiffuse); // 设置漫射光
	glLightfv(GL_LIGHT1, GL_POSITION,LightPosition); // 光源位置
	glEnable(GL_LIGHT1); // 启用一号光源

	FilterMode = GL_LINEAR;	//设置纹理的插值模式

	*/

	//初始化基础字体
	base_font_init();

    glGetIntegerv(GL_VIEWPORT, m_viewport.data);

    return 0;
}


void OpenGL_Platform::shutdown()
{
	if(this->is_enabled())
	{
		HGLRC hGLRC = wglGetCurrentContext();
		HDC   hDC   = wglGetCurrentDC();
		//释放字体资源
		base_font_free();
		//释放所有纹理
		images.close_all();
		//释放opengl设备
		if(hGLRC)
		{
			wglMakeCurrent(NULL, NULL);
			wglDeleteContext(wglGetCurrentContext());
		}
		if(hDC)
		{
			if(m_hwnd)ReleaseDC(m_hwnd, hDC);
		}
		//恢复窗口模式
		if(m_fullscreen)
		{
			ChangeDisplaySettings(NULL, 0);
		}
	}
}


int OpenGL_Platform::display_mode(int width, int height, int colorbits, bool fullscreen)
{
	RECT rc = {0};
	if(!this->is_enabled())
	{
		return -1;
	}
	m_fullscreen = fullscreen;
	if(fullscreen)
	{
		DEVMODE devmode;  // 设备模式
		memset(&devmode, 0, sizeof(DEVMODE));	// 确保内存分配
		devmode.dmSize = sizeof(DEVMODE); 		// Devmode 结构的大小
		devmode.dmPelsWidth  = width;		// 所选屏幕宽度
		devmode.dmPelsHeight = height;  	// 所选屏幕高度
		devmode.dmBitsPerPel = colorbits;	// 每象素所选的色彩深度
		devmode.dmFields=DM_BITSPERPEL|DM_PELSWIDTH|DM_PELSHEIGHT;
		m_winstyle = GetWindowLong(m_hwnd, GWL_STYLE);
		SetWindowLong(m_hwnd, GWL_STYLE, WS_POPUP);
		ChangeDisplaySettings(&devmode, CDS_FULLSCREEN);
		SetWindowPos(m_hwnd, HWND_TOPMOST, 0, 0, width, height, SWP_SHOWWINDOW);
	}
	else
	{
		ChangeDisplaySettings(NULL, 0);
		SetWindowLong(m_hwnd, GWL_STYLE, m_winstyle);
		SetWindowPos(m_hwnd, HWND_NOTOPMOST, 0, 0, width, height, SWP_SHOWWINDOW);
	}
	ShowWindow(m_hwnd, SW_SHOW);
	GetClientRect(m_hwnd, &rc);
	viewport(0, 0, rc.right - rc.left, rc.bottom - rc.top);
	return 0;
}
//---------------------------------------------------------------------------


void OpenGL_Platform::begin_paint()
{
	//assert(enabled());
	//this->init_matrix();
	glLoadIdentity();
	glScissor(m_viewport.x, m_viewport.y, m_viewport.width, m_viewport.height);
}


void OpenGL_Platform::end_paint()
{
	assert(this->is_enabled());
    //this->flush();
	SwapBuffers(wglGetCurrentDC());
}


void OpenGL_Platform::push_2d(int left, int top, int width, int height)
{
	//glPushAttrib(GL_TRANSFORM_BIT|GL_VIEWPORT_BIT);
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	glMatrixMode(GL_PROJECTION);
    glPushMatrix();
	glLoadIdentity();

	//直角坐标系
	//glOrtho(left, width-1, top, height-1, 0, INT_MAX);
	//十字坐标系
	//glOrtho(-width/2, width/2, -height/2, height/2, 0, INT_MAX);//

	//windows坐标系
	//glOrtho(left, width-1, height-1, top, -1.0, 1000000.0);
	glOrtho(left, left+width-1, top+height-1, top, 0, INT_MAX);

	//glOrtho(0, width-1, -height+1, 0, 0, INT_MAX);
	//glTranslatef(0, -height+1, 0);

	//重新设置正面，默认GL_CCW
	//glFrontFace(GL_CW);

	//反转屏幕
	//glScalef(1.0f, -1.0f, 1.0f);
	//glTranslatef(0.0f, -height, 0.0f);

	glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
	glDisable(GL_DEPTH_TEST);//关闭深度测试
	glDisable(GL_CULL_FACE); //关闭面剔除
}


void OpenGL_Platform::pop_2d()
{
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
    glPopAttrib();
}

//3D模式，第一人称视觉

void OpenGL_Platform::lookat(float atx, float aty, float atz, float anglex, float angley, float anglez)
{
	glRotatef(anglez, 0.0f, 0.0f, 1.0f);
    glRotatef(angley, 0.0f, 1.0f, 0.0f);
    glRotatef(anglex, 1.0f, 0.0f, 0.0f);

	glTranslatef(atx, aty, atz);//这里移动的是负值，坐标系才正确
}
//---------------------------------------------------------------------------


void OpenGL_Platform::viewport(int left, int top, int width, int height)
{
	if(this->is_enabled())
	{
		glViewport(left, top, width, height);
		//glViewport(0,0,0.1,0.025);
		glMatrixMode(GL_PROJECTION);//设置矩阵模式为投影矩阵
		glLoadIdentity();//初始化投影矩阵
		// 计算窗口的外观比例
		gluPerspective(60.0f,GLfloat(width)/GLfloat(height), 0.0001f, 10000.0f);
		//glFrustum(0.0f, 64.0f, 48.0f, 0.0f, 1.0f, 100.0f);
		glMatrixMode(GL_MODELVIEW);//设置矩阵模式为modelview矩阵
		glLoadIdentity();//重置OCS，使之与ECS重合

        m_viewport.left = left;
        m_viewport.top = top;
        m_viewport.width = width;
        m_viewport.height = height;
	}
}
//---------------------------------------------------------------------------

//管理的纹理函数
GLuint glxImage2D(long width, long height, GLenum format,
	GLenum src_format, GLenum src_type, const GLubyte *src_data,
	GLenum filter)
{
    return opengl.images.create(width, height, format, src_format, src_type, src_data, filter);
}

GLuint glxLoadImage(const wchar_t* fname, GLenum format)
{
	return opengl.images.open(fname, format);
}

void glxDeleteImage(GLuint image)
{
    return opengl.images.close(image);
}

void glxBindImage2D(GLuint image)
{
    return opengl.images.bind_image2D(image);
}

GLuint glxGetImage(GLuint image)
{
    return opengl.images.current();
}

/*
OpenGL 的绘图函数：
*/
//void DrawRect(Rectf);
//void DrawRect(Rectf, Rectf);

//glImage
//int glPaintImage(x, y, teximage, x, y, w, h)
//尽量回避glBegin和glEnd组合

inline void glxRectf(float x1, float y1, float x2, float y2, float u1, float v1, float u2, float v2)
{
	glBegin(GL_QUADS);
		glTexCoord2f(u1, v2); glVertex2f(x1, y1);
		glTexCoord2f(u2, v2); glVertex2f(x2, y1);
		glTexCoord2f(u2, v1); glVertex2f(x2, y2);
		glTexCoord2f(u1, v1); glVertex2f(x1, y2);
	glEnd();
}

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
