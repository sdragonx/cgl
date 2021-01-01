/*
 Copyright (c) 2005-2020 sdragonx (mail:sdragonx@foxmail.com)

 stdfile.hpp

 2018-09-20 14:23:35

*/
#ifndef STDFILE_HPP_20180920142335
#define STDFILE_HPP_20180920142335

#include "unifile.hpp"

namespace cgl{
namespace io{

class stdfile : public unifile
{
public:
    stdfile();
    ~stdfile();
    int open(const char* fn, int mode, int share);
    int open(const wchar_t* fn, int mode, int share);
    void close();
    bool is_open();
    fsize_t read(void* data, fsize_t size);
    fsize_t write(const void* data, fsize_t size);
    fsize_t seek(fsize_t pos, int whence);
    fsize_t tell();
    fsize_t size();
    fsize_t resize(fsize_t size);

	bool eof()
	{
		return feof(m_stdio);
	}


	fsize_t readLine(char* buffer, size_t size)
	{
		for(size_t i=0; i<size; ++i){
			buffer[i] = this->getc();
			if (buffer[i] == '\n') {
				buffer[i] == '\0';
				if (i != 0 && buffer[i - 1] == '\r') {
					buffer[i - 1] = '\0';
				}
				return i;
			}
			if (this->eof()) {
				return i;
			}
		}
		return FILE_ERROR;
	}


private:
    int open(const char* afn, const wchar_t* wfn, int mode, int share);
};

stdfile::stdfile() : unifile()
{

}

stdfile::~stdfile()
{
    this->close();
}

int stdfile::open(const char* fn, int mode, int share)
{
    return this->open(fn, null, mode, share);
}

int stdfile::open(const wchar_t* fn, int mode, int share)
{
    return this->open(null, fn, mode, share);
}

void stdfile::close()
{
    if(this->is_open())
    {
        fclose(m_stdio);
		m_stdio = null;
    }
}

bool stdfile::is_open()
{
    return m_stdio;
}

fsize_t stdfile::read(void* data, fsize_t size)
{
    CGL_ASSERT(this->is_open());
    return (fsize_t)fread(data, 1, size_t(size), m_stdio);
}

fsize_t stdfile::write(const void* data, fsize_t size)
{
    CGL_ASSERT(this->is_open());
    return (fsize_t)fwrite(data, 1, size_t(size), m_stdio);
}

fsize_t stdfile::seek(fsize_t pos, int whence)
{
    return (fsize_t)fseek(m_stdio, long(pos), whence);
}

fsize_t stdfile::tell()
{
    return (fsize_t)ftell(m_stdio);
}

fsize_t stdfile::size()
{
    fsize_t cur = this->tell();
    this->seek(0, seek_end);
    fsize_t n = this->tell();
    this->seek(cur, seek_begin);
    return n;
}

fsize_t stdfile::resize(fsize_t size)
{
    //return fchsizei64(m_stream, size);
	
	return 0;
}

int stdfile::open(const char* afn, const wchar_t* wfn, int mode, int share)
{
    this->close();

    char* amode;
    wchar_t* wmode;

    if(mode & io_create){
        if(mode & io_write){
            amode = "wb";
            wmode = L"wb";
        }
        else{
            amode = "rb";
            wmode = L"rb";
        }
    }
    else{
        if(mode & io_write){
            amode = "wb+";
            wmode = L"wb+";
        }
        else{
            amode = "rb+";
            wmode = L"rb+";
        }
    }

    if(afn)
    {
		#ifdef _MSC_VER
		fopen_s(&m_stdio, afn, amode);
		#else
		m_stdio = fopen(afn, amode);
		#endif
        unifile::m_name  = afn;
    }
    else if(wfn)
    {
		#ifdef _MSC_VER
		_wfopen_s(&m_stdio, wfn, wmode);
		#else
		m_stdio = _wfopen(wfn, wmode);
		#endif
        //unifile::m_name = wfn;
    }
    else
    {
        return FILE_ERROR;
    }

    if(this->is_open())
    {
        //if(mode & io_create)this->resize(0);
        if(mode & io_append)this->seek(0, seek_end);
        return FILE_SUCCESS;
    }
    else
    {
		m_stdio = null;
        unifile::m_name.clear();
        return FILE_ERROR;
    }
}

}//end namespace io
}//end namespace cgl

#endif //STDFILE_HPP_20180920142335
