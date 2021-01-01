/*
 Copyright (c) 2005-2020 sdragonx (mail:sdragonx@foxmail.com)

 cpuid.hpp

 2020-04-29 11:21:29

     using namespace std;
    cout<<"����CPU��Ϣ����:"<<endl;
	cout<<"CPU �� Ƶ: "<<cpu_freq()<<" MHZ"<<endl;
	cout<<"CPU ������: "<<cpu_manu()<<endl;
	cout<<"CPU �� ��: "<<cpu_type()<<endl;

*/
#ifndef CPUID_HPP_20200429112129
#define CPUID_HPP_20200429112129

#include <cgl/public.h>

namespace cgl{
//namespace system{

void cpuid(int id, uint32_t& a, uint32_t& b, uint32_t& c, uint32_t& d)
{
    uint32_t x, y, z, w;
    #if defined(_WIN32) && !defined(__GNUC__)
        __asm{
            mov eax,id  //��������ֵ��eax
            cpuid       //ִ��cpuidָ��
            mov x,eax   //�Ĵ���ȡֵ
            mov y,ebx
            mov z,ecx
            mov w,edx
        }

    #elif defined(__GNUC__)
        __asm__(
            "cpuid"
            :"=a"(a),
            "=b"(b),
            "=c"(c),
            "=d"(d)
            :"a"(id)
        );
    #endif

    a = x;
    b = y;
    c = z;
    d = w;
}

inline void cpuid(int id, uint32_t info[4])
{
    cpuid(id, info[0], info[1], info[2], info[3]);
}

inline void cpuid(int id, __x128& info)
{
    cpuid(id, info.idata);
}

/*	��Intel Pentium���ϼ����CPU�У���һ����Ϊ��ʱ�����Time Stamp�����Ĳ�����
	����64λ�޷����������ĸ�ʽ����¼����CPU�ϵ�������������ʱ����������
	����Ŀǰ��CPU��Ƶ���ǳ��ߣ��������������Դﵽ���뼶�ļ�ʱ���ȡ�
	�����ȷ�����������ַ������޷�����ġ�
	��Pentium���ϵ�CPU�У��ṩ��һ������ָ��RDTSC��Read Time Stamp Counter��
	����ȡ���ʱ��������֣������䱣����EDX:EAX�Ĵ�������
*/
//��ȡCPUƵ��,��λ: MHZ
int cpu_freq()
{
	int start,over;
	_asm
	{
		RDTSC
		mov start,eax
	}
	Sleep(50);
	_asm
	{
		RDTSC
		mov over,eax
	}
	return (over-start)/50000;
}

/*��eax = 0��Ϊ������������Եõ�CPU����������Ϣ��
  cpuidָ��ִ���Ժ󣬻᷵��һ��12�ַ�����������Ϣ��
  ǰ�ĸ��ַ���ASC�밴��λ����λ����ebx���м��ĸ�����edx������ĸ��ַ�����ecx��
*/
//manufacturer
const char* cpu_manu()   //��ȡ��������Ϣ
{
	static char ID[13] = {0};
    __x128 info = {0};

    cpuid(0, info);
	memcpy(ID + 0, &info.ebx, 4); //��������Ϣ���Ƶ�����
	memcpy(ID + 4, &info.edx, 4);
	memcpy(ID + 8, &info.ecx, 4);

	return ID;
}

/*  ���ҵĵ����ϵ���Ҽ���ѡ�����ԣ������ڴ��ڵ����濴��һ��CPU����Ϣ��
	�����CPU���̱��ַ�����CPU���̱��ַ���Ҳ��ͨ��cpuid�õ��ġ�
	�����̱���ַ����ܳ�(48���ַ�)�����Բ�����һ��cpuidָ��ִ��ʱȫ���õ���
	����intel�����ֳ���3��������eax����������ֱ���0x80000002,0x80000003,0x80000004��
	ÿ�η��ص�16���ַ������մӵ�λ����λ��˳�����η���eax, ebx, ecx, edx��
	��ˣ�������ѭ���ķ�ʽ��ÿ��ִ�����Ժ󱣴�����Ȼ��ִ����һ��cpuid��
*/
const char* cpu_type()
{
	const uint32_t id = 0x80000002; //��0x80000002��ʼ,��0x80000004����
	static char CPUType[64] = {0};//�����洢CPU�ͺ���Ϣ

    __x128 info;

//    cpuid(0x80000000, info);
//    int n = info.eax;

	for(uint32_t i = 0 ; i < 4; ++i)
	{
		cpuid(id + i, info);
		//ÿ��ѭ������,������Ϣ������
		memcpy(CPUType + 16 * i +  0, &info.eax, 4);
		memcpy(CPUType + 16 * i +  4, &info.ebx, 4);
		memcpy(CPUType + 16 * i +  8, &info.ecx, 4);
		memcpy(CPUType + 16 * i + 12, &info.edx, 4);
	}

	return CPUType;
}

//}//end namespace system
}//end namespace cgl

#endif //CPUID_HPP_20200429112129