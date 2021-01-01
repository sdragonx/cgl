/*
 Copyright (c) 2005-2020 sdragonx (mail:sdragonx@foxmail.com)

 io.hpp

 2012-04-26 23:14

*/
#ifndef IO_HPP_201204262314
#define IO_HPP_201204262314

#include <memory>
#include <cgl/public.h>
#include <cgl/io/zip_archive.hpp>
#include <cgl/io/memfile.hpp>
#include <cgl/io/binfile.hpp>
#include <cgl/io/zlibfile.hpp>
#include <cgl/io/filelink.hpp>

#include "io/file.hpp"
#include "io/filebuf.hpp"

//blockfile
//blobfile

namespace cgl{
namespace io{

int unifile::save(const char* fn)
{
    alone_ptr<unifile> f(new unifile);
    f->open(fn, io_write, sh_none);
    return this->save(f.get());
}

int unifile::save(const wchar_t* fn)
{
    alone_ptr<unifile> f(new unifile);
    f->open(fn, io_write, sh_none);
    return this->save(f.get());
}

}//end namespace io
}//end namespace cgl

//---------------------------------------------------------------------------
#endif //IO_HPP_201204262314

