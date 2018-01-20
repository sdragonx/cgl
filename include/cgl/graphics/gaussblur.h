/*

 gaussblur.h

 sdragonx 2014-08-01 02:16:50

*/
#ifndef GAUSSBLUR_H_20140801
#define GAUSSBLUR_H_20140801

#pragma option -w-8004

#include <stdint.h>
#include <cmath>
#include <algorithm>

typedef uint8_t byte_t;

namespace cgl{

bool check_rect(int width, int height, int& left, int& top, int& right, int& bottom)
{
	if(!left && !top && !right && !bottom)
	{//不进行剪裁
		left   = 0;
		top    = 0;
		right  = width;
		bottom = height;
		return true;
	}

	if(!(right-left) || !(bottom-top))
	{//0面积矩形
		return false;
	}

	if(bottom <= 0)
	{//如果bottom小于0，反转上下
		int h  = -bottom - top;
		bottom = height - top;
		top    = bottom - h;
	}
	if(right <= 0)
	{//反转左右
		int w = -right - left;
		right = width - left;
		left  = right - w;
	}

	left   = std::max(left,   0);
	top    = std::max(top,    0);
	right  = std::min(right,  width);
	bottom = std::min(bottom, height);

	return true;
}


//快速高斯模糊函数，by sdragonx 2014-08-01
int gauss_blur(
	byte_t* image,	//位图数据
	int linebytes,	//位图行字节数，BMP数据在windows中是4字节对齐的。否则在处理非二次幂的图像时会有偏差
	int width,		//位图宽度
	int height,		//位图高度
	int cbyte,		//颜色通道数量
	int left,		//滤镜剪裁区域，都为0是不剪裁
	int top,
	int right,
	int bottom,
	float sigma		//高斯系数
	)
{
	int x = 0, y = 0, n = 0;
	int channel = 0;
	int srcline = 0, dstline = 0;
	int channelsize = 0;
	int bufsize = 0;
	float *w1 = NULL, *w2 = NULL, *imgbuf = NULL;
	int time = 0;

	#ifdef _DEBUG
	#if defined(_INC_WINDOWS)
		time = GetTickCount();
	#elif defined(_CLOCK_T)
		time  = clock();
	#endif
	#endif

	if(!check_rect(width, height, left, top, right, bottom))
	{
		return -1;
	}

	//剪裁宽度和高度
	int rc_width  = right - left;
	int rc_height = bottom - top;

	channelsize = rc_width*rc_height;
	bufsize     = rc_width > rc_height ? rc_width + 4 : rc_height + 4;

	w1 = (float*)malloc(bufsize * sizeof(float));
	if(!w1)
	{
		return -1;
	}
	w2 = (float*)malloc(bufsize * sizeof(float));
	if(!w2)
	{
		free(w1);
		return -1;
	}
	imgbuf = (float*)malloc(channelsize * sizeof(float));
	if(!imgbuf)
	{
		free(w1);
		free(w2);
		return -1;
	}

//----------------计算高斯核---------------------------------------//
	float q  = 0;
	float q2 = 0, q3 = 0;
	float b0 = 0, b1 = 0, b2 = 0, b3 = 0;
	float B  = 0;

	if (sigma >= 2.5f)
	{
		q = 0.98711f * sigma - 0.96330f;
	}
	else if ((sigma >= 0.5f) && (sigma < 2.5f))
	{
		q = 3.97156f - 4.14554f * (float) std::sqrt (1.0f - 0.26891f * sigma);
	}
	else
	{
		q = 0.1147705018520355224609375f;
	}

	q2 = q * q;
	q3 = q * q2;
	b0 = (1.57825f+ (2.44413f*q)+(1.4281f *q2)+(0.422205f*q3));
	b1 = (         (2.44413f*q)+(2.85619f*q2)+(1.26661f* q3));
	b2 = (                     -((1.4281f*q2)+(1.26661f* q3)));
	b3 = (                                    (0.422205f*q3));
	B = 1.0f-((b1+b2+b3)/b0);

	b1 /= b0;
	b2 /= b0;
	b3 /= b0;
//----------------计算高斯核结束---------------------------------------//

	// 处理图像的多个通道
	for (channel = 0; channel < cbyte; ++channel)
	{

		// 获取一个通道的所有像素值，并预处理
		srcline = top*linebytes + left*cbyte + channel;
		dstline = 0;
		for(y=0; y<rc_height; ++y, srcline+=linebytes, dstline+=rc_width)
		{
			for(x=0, n=0; x<rc_width; ++x, n+=cbyte)
			{
				(imgbuf+dstline)[x] = float((image+srcline)[n]);
			}
		}

		//for (int x=0; x<rc_width; ++x)
		for (x=0; x<rc_width; ++x)
		{//横向处理

			w1[0] = (imgbuf + x)[0];
			w1[1] = (imgbuf + x)[0];
			w1[2] = (imgbuf + x)[0];

			for (y=0, n=0; y<rc_height; ++y, n+=rc_width)
			{
				w1[y+3] = B*(imgbuf + x)[n] + (b1*w1[y+2] + b2*w1[y+1] + b3*w1[y+0]);
			}

			w2[rc_height+0]= w1[rc_height+2];
			w2[rc_height+1]= w1[rc_height+1];
			w2[rc_height+2]= w1[rc_height+0];

			for (int y=rc_height-1, n=y*rc_width; y>=0; --y, n-=rc_width)
			{
				(imgbuf + x)[n] = w2[y] = B*w1[y+3] + (b1*w2[y+1] + b2*w2[y+2] + b3*w2[y+3]);
			}
		}//横向处理

		srcline = 0;
		dstline = top*linebytes + left*cbyte + channel;
		for (y=0; y<rc_height; ++y, srcline+=rc_width, dstline+=linebytes)
		{//纵向处理

			//取当前行数据
			w1[0] = (imgbuf + srcline)[0];
			w1[1] = (imgbuf + srcline)[0];
			w1[2] = (imgbuf + srcline)[0];

			//正方向横向处理3个点的数据
			for (x=0; x<rc_width; ++x)
			{
				w1[x+3] = B*(imgbuf + srcline)[x] + (b1*w1[x+2] + b2*w1[x+1] + b3*w1[x+0]);
			}

			w2[rc_width+0]= w1[rc_width+2];
			w2[rc_width+1]= w1[rc_width+1];
			w2[rc_width+2]= w1[rc_width+0];

			//反方向处理
			for (x=rc_width-1, n=x*cbyte; x>=0; --x, n-=cbyte)
			{
				//(imgbuf + srcline)[x] = w2[x] = B*w1[x+3] + (b1*w2[x+1] + b2*w2[x+2] + b3*w2[x+3]);
				(image + dstline)[n] = w2[x] = B*w1[x+3] + (b1*w2[x+1] + b2*w2[x+2] + b3*w2[x+3]);
			}
		}//纵向处理

		/*
		//存储处理完毕的通道
		for(int y=0; y<rc_height; ++y)
		{
			int dstline = (y+top)*linebytes + left*cbyte;
			int srcline = y*rc_width;
			for (int x=0, n=channel; x<rc_width; ++x, n+=cbyte)
			{
				(image + dstline)[n] = (imgbuf + srcline)[x];
					//byte_edge((imgbuf + srcline)[x]);

			}
		}//存储循环
		//*/
	}//通道循环

	free (w1);
	free (w2);
	free(imgbuf);

	#ifdef _DEBUG
	#if defined(_INC_WINDOWS)
		return GetTickCount() - time;
	#elif defined(_CLOCK_T)
		return clock() - time;
	#endif
	#else
		return 0;
	#endif
}

int gauss_blur(
	byte_t* image,	//位图数据
	int linebytes,	//位图行字节数，BMP数据在windows中是4字节对齐的。否则在处理非二次幂的图像时会有偏差
	int width,		//位图宽度
	int height,		//位图高度
	int cbyte,		//颜色通道数量
	float sigma		//高斯系数
	)
{
	return gauss_blur(image, linebytes, width, height, cbyte, 0, 0, 0, 0, sigma);
}

#if 0
//不带剪裁的版本
int gauss_blur(
	byte_t* image,	//位图数据
	int linebytes,	//位图行字节数，BMP数据在windows中是4字节对齐的。否则在处理非二次幂的图像时会有偏差
	int width,		//位图宽度
	int height,		//位图高度
	int cbyte,		//颜色通道数量
	float sigma		//高斯系数
	)
{
	int x = 0, y = 0, n = 0;
	int channel = 0;
	int srcline = 0, dstline = 0;
	int channelsize = width*height;
	int bufsize = width > height ? width + 4 : height + 4;
	float *w1 = NULL, *w2 = NULL, *imgbuf = NULL;
	int time = 0;

	#if defined(_INC_WINDOWS)
		time = GetTickCount();
	#elif defined(_CLOCK_T)
		time  = clock();
	#endif

	w1 = (float*)malloc(bufsize * sizeof(float));
	if(!w1)
	{
		return -1;
	}
	w2 = (float*)malloc(bufsize * sizeof(float));
	if(!w2)
	{
		free(w1);
		return -1;
	}
	imgbuf = (float*)malloc(channelsize * sizeof(float));
	if(!imgbuf)
	{
		free(w1);
		free(w2);
		return -1;
	}

//----------------计算高斯核---------------------------------------//
	float q  = 0;
	float q2 = 0, q3 = 0;
	float b0 = 0, b1 = 0, b2 = 0, b3 = 0;
	float B  = 0;

	if (sigma >= 2.5f)
	{
		q = 0.98711f * sigma - 0.96330f;
	}
	else if ((sigma >= 0.5f) && (sigma < 2.5f))
	{
		q = 3.97156f - 4.14554f * (float) sqrt (1.0f - 0.26891f * sigma);
	}
	else
	{
		q = 0.1147705018520355224609375f;
	}

	q2 = q * q;
	q3 = q * q2;
	b0 = (1.57825+ (2.44413f*q)+(1.4281f *q2)+(0.422205f*q3));
	b1 = (         (2.44413f*q)+(2.85619f*q2)+(1.26661f* q3));
	b2 = (                     -((1.4281f*q2)+(1.26661f* q3)));
	b3 = (                                    (0.422205f*q3));
	B = 1.0-((b1+b2+b3)/b0);

	b1 /= b0;
	b2 /= b0;
	b3 /= b0;
//----------------计算高斯核结束---------------------------------------//

    // 处理图像的多个通道
	for (channel = 0; channel < cbyte; ++channel)
	{
		// 获取一个通道的所有像素值，并预处理
		for(y=0; y<height; ++y)
		{
			srcline = y*linebytes;
			dstline = y*width;
			for(x=0, n=channel; x<width; ++x, n+=cbyte)
			{
				(imgbuf+dstline)[x] = float((image+srcline)[n]);
			}
		}


		for (int x=0; x<width; ++x)
		{//横向处理

			w1[0] = (imgbuf + x)[0];
			w1[1] = (imgbuf + x)[0];
			w1[2] = (imgbuf + x)[0];

			for (y=0, n=0; y<height; ++y, n+=width)
			{
				w1[y+3] = B*(imgbuf + x)[n] + (b1*w1[y+2] + b2*w1[y+1] + b3*w1[y+0]);
			}

			w2[height+0]= w1[height+2];
			w2[height+1]= w1[height+1];
			w2[height+2]= w1[height+0];

			for (int y=height-1, n=y*width; y>=0; --y, n-=width)
			{
				//保存数据到缓存
				(imgbuf + x)[n] = w2[y] = B*w1[y+3] + (b1*w2[y+1] + b2*w2[y+2] + b3*w2[y+3]);
			}
		}//横向处理

		for (y=0, srcline=0, dstline=0; y<height; ++y, srcline+=width, dstline+=linebytes)
		{//纵向处理

			//取当前行数据
			w1[0] = (imgbuf + srcline)[0];
			w1[1] = (imgbuf + srcline)[0];
			w1[2] = (imgbuf + srcline)[0];

			//正方向横向处理3个点的数据
			for (x=0; x<width ; ++x)
			{
				w1[x+3] = B*(imgbuf + srcline)[x] + (b1*w1[x+2] + b2*w1[x+1] + b3*w1[x+0]);
			}

			w2[width+0]= w1[width+2];
			w2[width+1]= w1[width+1];
			w2[width+2]= w1[width+0];

			//反方向处理
			for (x=width-1, n=x*cbyte+channel; x>=0; --x, n-=cbyte)
			{
				//处理保存数据到缓存
				//(imgbuf + dstline)[x] = w2[x] = B*w1[x+3] + (b1*w2[x+1] + b2*w2[x+2] + b3*w2[x+3]);

				//存储返回数据
				(image + dstline)[n] = w2[x] = B*w1[x+3] + (b1*w2[x+1] + b2*w2[x+2] + b3*w2[x+3]);
			}

		}//纵向处理

		/*
		//存储处理完毕的通道
		for(int y=0; y<height; y++)
		{
			int dstline = y*linebytes;
			int srcline = y*width;
			for (int x=0; x<width; x++)
			{
				(image + dstline)[x * cbyte + channel] = (imgbuf + srcline)[x];
					//byte_edge((imgbuf + srcline)[x]-1);

			}
		}//存储循环
		//*/
	}//通道循环

	free(w1);
	w1=NULL;
	free(w2);
	w2=NULL;
	free(imgbuf);
	imgbuf=NULL;

	#if defined(_INC_WINDOWS)
		return GetTickCount() - time;
	#elif defined(_CLOCK_T)
		return clock() - time;
	#else
		return 0;
	#endif
}
#endif

}// end namespace cgl

#endif //GAUSSBLUR_H_20140801
