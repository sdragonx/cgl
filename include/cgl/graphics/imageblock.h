/*

 imagediv.h

 sdragonx 2016-07-06 09:47:44

 图块

*/
#ifndef IMAGEDIV_H_20160706094744
#define IMAGEDIV_H_20160706094744

#include <cgl/public.h>
#include <vector>
//#include <cgl/graphics/draw_context.h>

_CGL_BEGIN

const int IMAGE_ALIGN_LEFT     = 0x01;
const int IMAGE_ALIGN_RIGHT    = 0x02;
const int IMAGE_ALIGN_CENTER_H = 0x03;	//horizontal水平居中
const int IMAGE_ALIGN_TOP      = 0x04;
const int IMAGE_ALIGN_BOTTOM   = 0x08;
const int IMAGE_ALIGN_CENTER_V = 0x0C;	//vertical垂直居中
const int IMAGE_ALIGN_CENTER   = 0x0F;

const int IMAGE_FLIP_X = 0x0100;
const int IMAGE_FLIP_Y = 0x0200;

const int IMAGE_MIN_FILTER_NEAREST = 0x010000;
const int IMAGE_MIN_FILTER_LINEAR  = 0x020000;	//default
const int IMAGE_MAG_FILTER_NEAREST = 0x040000;
const int IMAGE_MAG_FILTER_LINEAR  = 0x080000;	//default

//segment
//fragment
//teximage

#pragma pack(push, 1)
class teximage
{
public:
	intptr_t image;
	uint16_t x, y, width, height;
	float u1, v1, u2, v2;
public:
	teximage():image(), x(), y(), width(), height(), u1(), v1(), u2(), v2()
	{
	}

	teximage& operator=(const teximage& div)
	{
		image = div.image;
		x = div.x;
		y = div.y;
		width = div.width;
		height = div.height;
		u1 = div.u1;
		v1 = div.v1;
		u2 = div.u2;
		v2 = div.v2;
		return *this;
	}
	bool is_null()const
	{
		return !image;
	}

	//分割图片
	void split(std::vector<teximage>& ls, int x, int y, int maxframe)
	{
		teximage b;
		float w = width/x;
		float h = height/y;
		ls.clear();
		for(int i=0; i<y; ++i)
		{
			for(int j=0; j<x; ++j)
			{
				b.image = image;
				b.width = w;
				b.height = h;
				b.u1 = u1 + (u2-u1)/x*j;
				b.u2 = u1 + (u2-u1)/x*(j+1);
				b.v1 = v1 + (v2-v1)/y*i;
				b.v2 = v1 + (v2-v1)/y*(i+1);
				if(i*x+j < maxframe){
					ls.push_back(b);
				}
				else{
                    break;
                }
			}
		}
	}
};
#pragma pack(pop)


/*
int draw_image(draw_context& dc, const imageblock& image, int x, int y, int width, int height, int param)
{
	return 0;
}
*/

_CGL_END

#endif //IMAGEDIV_H_20160706094744
