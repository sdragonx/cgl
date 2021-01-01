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


//������
typedef std::vector<uint16_t> RAWline;

//RAWimageͼ���壬��ά��̬����
typedef std::vector<RAWline> RAWimage;

//ͳ������
struct RAWstate
{
	typedef std::map<uint16_t, int> RAWhistogram;

	int min;                //��Сֵ
	int max;                //���ֵ
	RAWhistogram histogram; //ֱ��ͼ����
};

//RAWimage���ô�С
void RAWimage_resize(RAWimage& a, size_t x, size_t y)
{
	a.resize(y);
	for (size_t i = 0; i<y; ++i) {
		a[i].resize(x);
	}
}

//����RAWͼ��
bool RAWimage_load(RAWimage& image, RAWstate& state, const wchar_t* filename)
{
	std::ifstream f(filename, std::ios::binary);
	if (!f.is_open()) {
		//�ļ���ʧ��
		return false;
	}

	//��ȡ�ļ���С
	f.seekg(0, std::ios::end);
	int size = static_cast<int>(f.tellg());

	//���������ļ���ȡλ��
	f.seekg(0, std::ios::beg);

	//����ͼƬ�Ƿ��ģ�16λ������ͼƬ��С
	size = static_cast<int>(sqrt(size * 0.5));

	//�������ͼƬ��С
	RAWimage_resize(image, size, size);

	//�л�����
	RAWline buf(size);
	int n;

	//ͳ�ƴ�С��ʼ��
	state.min = INT_MAX;
	state.max = INT_MIN;

	for (int y = 0; y<size; ++y) {
		//ÿ�ζ�ȡһ������
		f.read((char*)&buf[0], size * 2);
		for (int x = 0; x<size; ++x) {
			n = buf[x];
			//д���ļ��������͸߷�ת��һ��
			image[x][y] = n;
			//���������Сֵ
			if (n < state.min)state.min = n;
			if (n > state.max)state.max = n;

			//ֱ��ͼ
			state.histogram[n]++;
		}
	}

	f.close();

	return true;
}

//16λRAWͼ��ת8λ�Ҷ�
void RAWimage_gray256(RAWimage& out, const RAWimage& image, const RAWstate& state)
{
	if (image.empty()) {
		return;
	}

	//��ȡ���
	int w = image[0].size();
	int h = image.size();

	//�������ͼ���С
	RAWimage_resize(out, w, h);

	//ȡ���ֵ����Сֵ�Ĳ�
	int his = state.max - state.min;
	int n;
	for (int y = 0; y<h; ++y) {
		for (int x = 0; x<w; ++x) {
			n = image[y][x];
			n -= state.min;
			n = n * 255 / his;//�������ŵ�255��Χ֮��
			out[y][x] = n;//д���������
		}
	}
}

//���Ƕ�ȡһ����ͷ��CT
void RAWimage_gray256(RAWimage& out, const RAWimage& image)
{
	if (image.empty()) {
		return;
	}

	//��ȡ���
	int w = image[0].size();
	int h = image.size();

	//�������ͼ���С
	RAWimage_resize(out, w, h);

	int n;
	for (int y = 0; y<h; ++y) {
		for (int x = 0; x<w; ++x) {
			n = image[y][x] & 0xFF;
            //���ȸ���120������
			if(n > 120){
                n = 0;
            }
			out[y][x] = n;//д���������
		}
	}
}

}//end namespace graphics
}//end namespace cgl

#endif //RAWIMAGE_HPP_20201125073400
