/*
 Copyright (c) 2005-2020 sdragonx (mail:sdragonx@foxmail.com)

 tempfile.hpp

 2020-08-13 23:46:26

*/
#ifndef TEMPFILE_HPP_20200813234626
#define TEMPFILE_HPP_20200813234626

#include "stdfile.hpp"

namespace cgl{
namespace io{

class tempfile : public unifile
{
protected:
	fsize_t used_memory;
	fsize_t max_memory;

public:
	tempfile() : unifile()
	{
		this->open();
	}

	tempfile(const std::string&) : unifile()
	{
		this->open();
	}

	~tempfile()
	{
		this->close();
	}

	int open()
	{
		#ifdef _MSC_VER
		tmpfile_s(&m_stdio);
		#else
		m_stdio = tmpfile();
		#endif
		return 0;
	}

	void setAutoRemove(bool)
	{

	}
};

}//end namespace io
}//end namespace cgl

#endif //TEMPFILE_HPP_20200813234626
