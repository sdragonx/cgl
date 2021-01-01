/*
 Copyright (c) 2005-2020 sdragonx (mail:sdragonx@foxmail.com)

 rawimage.hpp

 2020-11-25 07:34:00

*/
#ifndef RAWIMAGE_HPP_20201125073400
#define RAWIMAGE_HPP_20201125073400

#include <cgl/public.h>
#include <fstream>

namespace cgl{
namespace graphics{


//行数据
typedef std::vector<uint16_t> RAWline;

//RAWimage图像定义，二维动态数组
typedef std::vector<RAWline> RAWimage;

//统计数据
struct RAWstate
{
	typedef std::map<uint16_t, int> RAWhistogram;

	int min;                //最小值
	int max;                //最大值
	RAWhistogram histogram; //直方图数据
};

//RAWimage设置大小
void RAWimage_resize(RAWimage& a, size_t x, size_t y)
{
	a.resize(y);
	for (size_t i = 0; i<y; ++i) {
		a[i].resize(x);
	}
}

//加载RAW图像
bool RAWimage_load(RAWimage& image, RAWstate& state, const wchar_t* filename)
{
	std::ifstream f(filename, std::ios::binary);
	if (!f.is_open()) {
		//文件打开失败
		return false;
	}

	//获取文件大小
	f.seekg(0, std::ios::end);
	int size = static_cast<int>(f.tellg());

	//重新设置文件读取位置
	f.seekg(0, std::ios::beg);

	//假设图片是方的，16位宽，计算图片大小
	size = static_cast<int>(sqrt(size * 0.5));

	//更改输出图片大小
	RAWimage_resize(image, size, size);

	//行缓冲区
	RAWline buf(size);
	int n;

	//统计大小初始化
	state.min = INT_MAX;
	state.max = INT_MIN;

	for (int y = 0; y<size; ++y) {
		//每次读取一行数据
		f.read((char*)&buf[0], size * 2);
		for (int x = 0; x<size; ++x) {
			n = buf[x];
			//写入文件，这里宽和高反转了一下
			image[x][y] = n;
			//计算最大最小值
			if (n < state.min)state.min = n;
			if (n > state.max)state.max = n;

			//直方图
			state.histogram[n]++;
		}
	}

	f.close();

	return true;
}

//16位RAW图像转8位灰度
void RAWimage_gray256(RAWimage& out, const RAWimage& image, const RAWstate& state)
{
	if (image.empty()) {
		return;
	}

	//获取宽高
	int w = image[0].size();
	int h = image.size();

	//更改输出图像大小
	RAWimage_resize(out, w, h);

	//取最大值和最小值的差
	int his = state.max - state.min;
	int n;
	for (int y = 0; y<h; ++y) {
		for (int x = 0; x<w; ++x) {
			n = image[y][x];
			n -= state.min;
			n = n * 255 / his;//比例缩放到255范围之内
			out[y][x] = n;//写入输出数据
		}
	}
}

//这是读取一个骨头的CT
void RAWimage_gray256(RAWimage& out, const RAWimage& image)
{
	if (image.empty()) {
		return;
	}

	//获取宽高
	int w = image[0].size();
	int h = image.size();

	//更改输出图像大小
	RAWimage_resize(out, w, h);

	int n;
	for (int y = 0; y<h; ++y) {
		for (int x = 0; x<w; ++x) {
			n = image[y][x] & 0xFF;
            //亮度高于120的舍弃
			if(n > 120){
                n = 0;
            }
			out[y][x] = n;//写入输出数据
		}
	}
}

}//end namespace graphics
}//end namespace cgl

#endif //RAWIMAGE_HPP_20201125073400
