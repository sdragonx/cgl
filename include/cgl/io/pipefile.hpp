/*
 Copyright (c) 2005-2020 sdragonx (mail:sdragonx@foxmail.com)

 pipefile.hpp

 2018-11-24 13:34:18

 class pipefile
 {
 public:
    pipefile();
    handle in();                ������
    handle out();               ������
    int create(int size);       �����ܵ� �ɹ�����0 ʧ�ܷ���-1
    void close();               �رչܵ�
    int query();                ��ѯ�ܵ��пɶ�ȡ���ݴ�С
    int read(void* data, size_t size);          ��ȡ����
    int write(const void* data, size_t size);   д������
 }

*/
#ifndef PIPEFILE_HPP_20181124133418
#define PIPEFILE_HPP_20181124133418

#include <cgl/public.h>

namespace cgl{

#ifdef CGL_PLATFORM_WINDOWS

class pipefile
{
private:
	HANDLE handle[2];

public:
    pipefile()
    {
        handle[0] = handle[1] = null;
    }

    HANDLE in()const
	{
		return handle[0];
	}

	HANDLE out()const
	{
		return handle[1];
	}

    //�����ܵ���0ΪĬ�ϻ�������С
    int create(DWORD buffer = 0)
    {
        this->close();
        SECURITY_ATTRIBUTES sa = {sizeof(SECURITY_ATTRIBUTES), NULL, TRUE};
        if (!CreatePipe(&handle[0], &handle[1], &sa, buffer)){
            return -1;
        }
        return 0;
    }

    void close_in()
    {
        if(handle[0]){
            CloseHandle(handle[0]);
            handle[0] = null;
        }
    }

    void close_out()
    {
        if(handle[1]){
            CloseHandle(handle[1]);
            handle[1] = null;
        }
    }

    void close()
	{
		if(handle[0]){ CloseHandle(handle[0]); handle[0] = null; }
        if(handle[1]){ CloseHandle(handle[1]); handle[1] = null; }
	}

    int query(size_t buffer = 1024)
    {
        DWORD size;
        PeekNamedPipe(in(), 0, buffer, &size, 0, 0);
        return size;
    }

	int read(void* data, size_t size)
	{
        DWORD dwSize;
		ReadFile(in(), data, size, &dwSize, NULL);
        return dwSize;
	}

	int write(const void* data, size_t size)
	{
		DWORD dwSize;
		WriteFile(out(), data, size, &dwSize, NULL);
        return dwSize;
	}

};

#else

class pipefile
{
private:
	int handle[2];

public:
	pipefile()
	{
		handle[0] = handle[1] = 0;
	}

    int in()const
	{
		return handle[0];
	}

	int out()const
	{
		return handle[1];
	}

	int create()
	{
        this->close();
		return pipe(handle);//0 ok; -1 failed
	}

	void close()
	{
		if(handle[0]){ ::close(handle[0]); handle[0] = 0; }
		if(handle[1]){ ::close(handle[1]); handle[1] = 0; }
	}

    int query()
    {
        int size;
        ioctl(in(), FIONREAD, &size);
        return size;
    }

	int read(void* data, size_t size)
	{
		return ::read(in(), data, size);
	}

	int write(const void* data, size_t size)
	{
		return ::write(out(), data, size);
	}
};

#endif

}//end namespace cgl

#endif //PIPEFILE_HPP_20181124133418