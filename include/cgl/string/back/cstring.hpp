/*

 stringx.hxx

 sdragonx 2015-09-01 12:53:52

 内存分配函数由new替换成malloc之后，速度已经超过std::string了 2015-9-5

 join('c', array[]);
*/
#ifndef STRINGX_HXX_20150901125352
#define STRINGX_HXX_20150901125352

#include <iterator>
#include <utility>	//std::swap
#include <cgl/public.h>
#include <cgl/algorithm.hpp>
#include <cgl/memory.hpp>
#include <cgl/stdlib.hpp>
#include <cgl/string/stringsplit.hpp>
#include <cgl/std/string.h>
#include <cgl/std/type_traits.hpp>
#include <cgl/std/char_traits.h>
#include <cgl/string/code_convert.h>

#ifndef CGL_PLATFORM_GNUC
#pragma option push -b -a8 -pc -Vx- -Ve- -w-inl -w-aus -w-sig
#endif

namespace cgl{

//#define CGL_USE_TSTRING

//默认32个字符缓冲区，效率比std::string要快一点，再大就稍微慢了
#ifndef CGL_STRING_CACHE_SIZE
#define CGL_STRING_CACHE_SIZE 16
#endif

//#define CGL_STRING_USE_REALLOC
//#define CGL_STRING_DEBUG

template<typename _char_t,
	size_t _Cx = CGL_STRING_CACHE_SIZE,
	class _Ax = malloc_allocator<_char_t> >
class cstring
{
public:
	typedef cstring<_char_t, _Cx, _Ax> this_type;
	typedef _char_t value_type, char_type;
	typedef _Ax allocator_type;

	typedef char_traits<char_type> traits_type;

	typedef typename _Ax::reference reference;
	typedef typename _Ax::const_reference const_reference;
	typedef typename _Ax::difference_type difference_type;

	typedef char_type* pointer;
	typedef const char_type* const_pointer;
	typedef char_type* iterator;
	typedef const char_type* const_iterator;
	typedef std::reverse_iterator<iterator> reverse_iterator;
	typedef std::reverse_iterator<const_iterator> const_reverse_iterator;

	typedef size_t size_type;
	const static size_type npos = TEXT_MAX;

	enum{
		nullchar = '\0',
		CACHE_SIZE = _Cx < CGL_STRING_CACHE_SIZE ? CGL_STRING_CACHE_SIZE/sizeof(char_type) : _Cx/sizeof(char_type)
	};

	//memory allocator
	static allocator_type allocator;
private:
	//data struct
	union _buf_t{
		char_type  cache[CACHE_SIZE];
		char_type* data;
		_buf_t():data(null){/*void*/}
	}m_buf;
	size_type m_capacity;
	size_type m_size;
public:
	cstring():m_buf(), m_capacity(0), m_size(0) { this->endsize(0); }

	//compiler default
	cstring(const this_type& str):m_buf(), m_capacity(0), m_size(0) { this->assign(str); }

	template<typename C, size_t B, class A>
	cstring(const cstring<C, B, A>& str, size_type pos = 0, size_type count=npos)
		:m_buf(), m_capacity(0), m_size(0)
	{ this->assign(str, pos, count); }

	template<typename CH>
	cstring(const CH* str, size_type pos = npos) : m_buf(), m_capacity(0), m_size(0)
	{ this->assign(str, pos); }

	cstring(const_reference ch):m_buf(), m_capacity(0), m_size(0)
	{ this->assign(ch); }

	//difference std::string(size_type, char_type)
	cstring(const_reference ch, size_type count):m_buf(), m_capacity(0), m_size(0)
	{ this->assign(ch, count); }

	cstring(const_iterator first, const_iterator last):m_buf(), m_capacity(0), m_size(0)
	{ this->assign(first, last); }

	//格式化初始化
	cstring(size_t buf_size, const value_type* param, ...)
	{
		va_list body;
		size_t n;
		va_start(body, param);
		this->reserve(buf_size);
		n = csprintf(this->buffer(), buf_size, param, body);
		va_end(body);
		this->endsize(n);
	}

	//数值转换
	explicit cstring(int32_t n, int radix = 10):m_buf(), m_capacity(0), m_size(0)
	{ this->assign(n, radix); }

	explicit cstring(int64_t n, int radix = 10):m_buf(), m_capacity(0), m_size(0)
	{ this->assign(n, radix); }

	explicit cstring(float n, int dec = 0, int precision = 6):m_buf(), m_capacity(0), m_size(0)
	{ this->assign(n, dec, precision); }

	explicit cstring(double n, int dec = 0, int precision = 6):m_buf(), m_capacity(0), m_size(0)
	{ this->assign(n, dec, precision); }

	explicit cstring(long double n, int dec = 0, int precision = 6):m_buf(), m_capacity(0), m_size(0)
	{ this->assign(n, dec, precision); }

	explicit cstring(bool value):m_buf(), m_capacity(0), m_size(0)
	{ this->assign(value); }

	~cstring() { this->dispose(); }

//operator =
	//compiler default
	this_type& operator=(const this_type& str) { return assign(str); }

	template<typename C, size_t B, class A>
	this_type& operator=(const cstring<C, B, A>& str) { return this->assign(str.c_str(), str.length()); }
    
	this_type& operator=(const char& ch)		{ return this->assign(&ch, 1); }
    this_type& operator=(const wchar_t& ch)		{ return this->assign(&ch, 1); }
	this_type& operator=(const char8_t& ch)		{ return this->assign(&ch, 1); }
	this_type& operator=(const char16_t& ch)	{ return this->assign(&ch, 1); }
	this_type& operator=(const char32_t& ch)	{ return this->assign(&ch, 1); }
	this_type& operator=(const char* str)		{ return this->assign(str); }
    this_type& operator=(const wchar_t* str)	{ return this->assign(str); }
	this_type& operator=(const char8_t* str)	{ return this->assign(str); }
	this_type& operator=(const char16_t* str)	{ return this->assign(str); }
	this_type& operator=(const char32_t* str)	{ return this->assign(str); }

//operator +=
	this_type& operator+=(const_reference ch) { return this->append(ch); }
	this_type& operator+=(const_pointer str) { return this->append(str); }
	this_type& operator+=(const this_type& str) { return this->append(str); }

//iterator and reference
	inline const_pointer c_str()const { return this->buffer(); }
	//inline const_pointer data()const { return this->buffer(); }
	inline reference operator[](size_type pos) { return this->buffer()[pos]; }
	inline const_reference operator[](size_type pos)const { return this->buffer()[pos]; }

	inline iterator begin() { return this->buffer(); }
	inline iterator end() { return this->begin()+this->length(); }
	inline const_iterator begin()const { return this->buffer(); }
	inline const_iterator end()const { return this->begin()+this->length(); }
	inline reverse_iterator rbegin() { return reverse_iterator(this->end()); }
	inline reverse_iterator rend() { return reverse_iterator(this->begin()); }
	inline const_reverse_iterator rbegin()const { return const_reverse_iterator(this->end()); }
	inline const_reverse_iterator rend()const { return const_reverse_iterator(this->begin()); }

	inline reference front() { return *this->begin(); }
	inline reference back() { return *(this->end()-1); }
	inline const_reference front()const { return *this->begin(); }
	inline const_reference back()const { return *(this->end()-1); }

//property & method
	void clear() { this->endsize(0); }
	bool empty()const { return !this->length(); }
	size_type size()const { return m_size; }
	size_type length()const { return m_size; }
	size_type capacity()const { return this->is_cached() ? CACHE_SIZE : m_capacity; }

	size_type resize(size_type size, const char_type& ch = nullchar)
	{
		if(size < this->size()){
			this->erase(size, this->size());
		}
		else{
			this->append(ch, size-this->size());
		}
		return this->size();
	}
	//分配内存，并对齐到16字节
	size_type reserve(size_type size)
	{
		return this->allocate(size);
	}

//assign
	this_type& assign(const_reference ch, size_type n = 1)
	{
		this->clear();
		this->reserve(n);
		traits_type::assign(this->buffer(), n, ch);
		this->endsize(n);
		return *this;
	}

    template<typename CH>
	this_type& assign(const CH* str, size_type n = npos)
	{
		if(::cgl::is_same<value_type, CH>::value){
			return _assign(const_pointer(str), n);
		}
		else{
			this->clear();
			if(!str){
				return *this;
			}
			if(npos == n){
				n = cslen(str);
			}
			if(!n){
				return *this;
			}
			int bufsize = code_convert<char_type>((char_type*)null, 0, str, n);
			this->reserve(bufsize);
			n = code_convert<char_type>(this->buffer(), bufsize, str, n);
			this->endsize(n);
			return *this;
		}
	}

	inline this_type& assign(const_iterator first, const_iterator last)
	{
        if(first < last){
    		return this->assign(&*first, last-first);
        }
        return *this;
	}

	//compiler default
	this_type& assign(const this_type& str)
	{
		return this->_assign(str.c_str(), str.length());
	}

	template<typename C, size_t B, class A>
	inline this_type& assign(const cstring<C, B, A>& str, size_type pos=0, size_type count=npos)
	{
		if(count == npos)count=str.length();
		return this->assign(str.c_str() + pos, count);
	}

//numeric cast
	inline this_type& assign(int32_t n, int radix = 10)
	{
		if(radix<4)
			this->reserve(32);
		else
			this->reserve(16);
		tostr(this->buffer(), n, radix);
		this->endsize(cslen(this->buffer()));
		return *this;
	}

	inline this_type& assign(int64_t n, int radix = 10)
	{
		this->reserve(65);
		tostr(this->buffer(), n, radix);
		this->endsize(cslen(this->buffer()));
		return *this;
	}

	inline this_type& assign(float n, int dec = 0, int precision = 6)
	{
		return this->assign(double(n), dec, precision);
	}

	inline this_type& assign(double n, int dec = 0, int precision = 6)
	{
		char_type buf[64] = {0};
		format_float(buf, 64, n, dec, precision);
		return this->assign(buf);
	}

	inline this_type& assign(long double n, int dec = 0, int precision = 6)
	{
		return this->assign(double(n), dec, precision);
	}

	inline this_type& assign(bool value)
	{
		tostr(this->buffer(), value);
		return *this;
	}

//append
	this_type& append(const_reference ch)
	{
		this->expand(1);
		*this->end() = ch;
		this->endsize(this->size()+1);
		return *this;
	}

	this_type& append(const_reference ch, size_type count)
	{
		if(count){
			size_type p = this->size();
			this->expand(count);
			traits_type::assign(this->buffer()+p, count, ch);
			this->endsize(this->size()+count);
		}
		return *this;
	}

	this_type& append(const_pointer str)
	{
		return this->append(str, cslen(str));
	}

	this_type& append(const_pointer str, size_type count)
	{
		if(!str || !count){
			return *this;
		}

		if(this->contains(str)){//append self
			if(count==npos || this->end() < str+count){
				count = this->end()-str;
			}
		}
		else{//append other
			if(count == npos){
				count = cslen(str);
			}
		}
		this->expand(count);
		traits_type::copy(buffer() + size(), str, count);
		this->endsize(this->length()+count);
		return *this;
	}
	this_type& append(const_iterator first, const_iterator last)
	{
		if(first < last){
			return append(&*first, last-first);
		}
		else{
            return *this;
        }
	}
	this_type& append(const this_type& str, size_type n = npos)
	{
		return this->append(str.buffer(), n == npos ? str.length() : n);
	}
//erase
	this_type& erase(size_type pos, size_type size = npos)
	{
        //out of range
        if(this->size() < pos){
        }
        else{
            this->erase(this->begin() + pos, this->begin() + pos + std::min(size, this->size() - pos));
        }
        return *this;
	}

    iterator erase(iterator where)
    {
        return this->erase(where, where+1);
    }

	iterator erase(iterator first, iterator last)
	{
        if(this->empty() || last <= first){
			return end();
		}

        //out of range
        if(this->begin() > last || this->end() < first){
            return end();
        }

        if(first < this->begin()) first = this->begin();
        if(this->end() < last) last = this->end();

		if(last < this->end()){//in the front or middle
			traits_type::move(first, last, this->end() - last);
			this->endsize(this->size() - (last - first));
        }
		else{//in the back
			this->endsize(this->size() - (last - first));
		}
		return first;
	}

//insert
	size_type insert(size_type pos, size_type count, const_reference ch)
	{
		if(0 < count){
			if(pos > this->size())return npos;
			this->expand(count);
			traits_type::move(this->buffer() + pos + count, this->buffer() + pos, this->size() - pos);
			traits_type::assign(this->buffer() + pos, count, ch);
		}
        return pos;
	}

	size_type insert(size_type pos, const_pointer str, size_type count = npos)
	{
		if(!str || this->size() < pos || !count){
			return npos;
        }

		if(pos<this->size()){//in the front or middle
			if(this->contains(str)){
				//src first and last
            	size_type first = str-this->buffer();
                if(count == npos || count > this->size()-first)count = this->size()-first;
                size_type last = first+count;

				this->expand(count);
				pointer dst_begin = this->buffer()+pos;
				pointer dst_end = dst_begin+count;
                traits_type::move(dst_end, dst_begin, this->size()-pos);
				if(first<pos){
					if(pos<last){
						traits_type::copy(dst_begin, this->buffer()+first, pos-first);
						traits_type::copy(dst_begin+pos-first, dst_end, last-pos);
					}
					else{
						traits_type::copy(dst_begin, this->buffer()+first, count);
					}
				}
				else{
					traits_type::move(dst_begin, this->buffer()+last, count);
				}
				this->endsize(this->size()+count);
            }
			else{//insert other string
            	if(count == npos)count=cslen(str);
				this->expand(count);
				traits_type::move(this->buffer()+pos+count, this->buffer()+pos, this->size()-pos);
				traits_type::copy(this->buffer()+pos, str, count);
				this->endsize(this->size()+count);
            }
		}
		else{//in the back
			this->append(str, count);
		}
		return pos;
	}

	size_type insert(const_iterator where, const_iterator src_begin, const_iterator src_end)
	{
		return this->insert(where-this->begin(), &*src_begin, src_end-src_begin);
	}

	size_type insert(size_t where, const this_type& str, size_type start = 0, size_type n = npos)
	{
    	return this->insert(where, str.buffer()+start, n);
	}

//replace
	size_type replace(size_type pos, size_type n, const char_type& ch, size_type count = 1)
	{
		if(this->size() < pos){
			return npos;
		}

		if(this->size() == pos){
			this->append(ch, count);
			return pos;
		}

		if(0 == n){
			return this->insert(pos, count, ch);
		}

		if(pos+n >= this->size()){
			this->erase(pos, n);
			this->append(ch, count);
			return pos;
		}

		if(0==count){
			this->erase(pos, n);
			return pos;
		}

		if(n < count)this->expand(count - n);
		traits_type::move(this->buffer() + pos + count, this->buffer() + pos + n, this->size() - pos - n);
		traits_type::assign(this->buffer() + pos, ch, count);
		this->endsize(this->size() - n + count);
		return pos;
	}
	size_type replace(size_type pos, size_type n, const_pointer str, size_type str_length)
	{
		if(this->size() < pos){
			return npos;
		}

		if(pos == this->size()){
			this->append(str, str_length);
			return pos;
		}

		if(0 == n){
        	return this->insert(pos, str, str_length);
		}

		if(pos+n >= this->size()){
			this->erase(pos, n);
			this->append(str, str_length);
			return pos;
		}

		if(!str || 0 == str_length){
			return this->erase(pos, n);
		}

		//待优化
		if(str_length == npos)str_length = cslen(str);
		if(this->contains(str)){
			this_type s(str, str_length);
			return this->replace(pos, n, s.c_str(), str_length);
		}
		else{
			if(n<str_length)this->expand(str_length - n);
			traits_type::move(this->buffer()+pos+str_length, this->buffer()+pos+n, this->size()-pos-n);
			traits_type::copy(this->buffer()+pos, str, str_length);
			this->endsize(this->size()-n+str_length);
		}
		return pos;
	}

	size_type replace(size_type pos, size_type len, const this_type& str)
	{
        return this->replace(pos, len, str.c_str(), str.length());
	}

//push_back pop_back
	void push_back(const char_type& ch)
	{
		this->append(ch);
	}
	//这里有问题，在多字符模式下面，正确删除一个字符，可能需要一些操作
	#if 0
	void pop_back()
	{
		if(length() && length()!=nopos){
			erase(length()-1, 1);
		}
	}
	#endif

//find
	size_type find(const char_type& ch, size_type pos = 0)const
	{
		const_iterator p = std::find(this->begin()+pos, this->end(), ch);
		return (p != this->end()) ? p-this->begin() : npos;
	}

	size_type find(const_pointer str, size_type pos = 0)const
	{
		//const_pointer p = csstr(this->c_str(), _str);
		return this->find(str, str + cslen(str));
	}

	size_type find(const_iterator find_begin, const_iterator find_end, size_type pos = 0)const
	{
		const_iterator p = std::search(this->begin() + pos, this->end(), find_begin, find_end);
		return p != this->end() ? p-this->begin() : npos;
	}

	size_type find(const this_type& str, size_type pos = 0)const
	{
		return this->find(str.begin(), str.end(), pos);
	}

//find
	size_type rfind(const_pointer str, size_t len = npos)
	{
		if(len == npos)len = cslen(str);
		if(this->size()<len)return npos;
		const_pointer itr = this->end()-len-1;
		for( ; this->begin() < itr; --itr){
			if(!this->compare(itr, str, len)){
				return itr-this->begin();
			}
		}
		return npos;
	}

	//find_first_of
	size_type find_first_of(const_reference ch)const
	{
		return this->find(ch);
	}

	size_type find_first_of(const_iterator find_begin, const_iterator find_end)const
	{
		const_iterator p = std::find_first_of(this->begin(), this->end(), find_begin, find_end);
		return p != this->end() ? p-this->begin() : npos;
	}

	size_type find_first_of(const_pointer str)const
	{
		return this->find_first_of(str, str+cslen(str));
	}

	size_type find_first_of(const_pointer str, size_t pos)const
	{
		const_iterator p = std::find_first_of(this->begin()+pos, this->end(), str, str+cslen(str));
		return p != this->end() ? p-this->begin() : npos;
	}

	//find_last_of
	size_type find_last_of(const_reference ch)const
	{
		const_iterator itr = std::find(this->rbegin(), this->rend(), ch).base()-1;
		return itr!=this->end() ? itr - this->begin() : npos;
	}

	size_type find_last_of(const_pointer str)const
	{
		return this->find_last_of(str, str+cslen(str));
	}

	size_type find_last_of(const_iterator find_begin, const_iterator find_end)const
	{
		const_iterator itr = std::find_first_of(this->rbegin(), this->rend(),
			find_begin, find_end).base()-1;
		return itr != this->end() ? itr-this->begin() : npos;
	}

	//find_first_not_of
	size_type find_first_not_of(const_reference ch, size_type pos = 0)const
	{
		const_iterator itr = cgl::find_first_not_of(this->begin()+pos, this->end(), ch);
		return itr != this->end() ? itr-this->begin() : npos;
	}

	size_type find_first_not_of(const_pointer str, size_type pos = 0)const
	{
		//if(len == npos)len = cslen(str);
		return this->find_first_not_of(str, str+cslen(str), pos);
	}

	size_type find_first_not_of(const_iterator find_begin, const_iterator find_end, size_type pos = 0)const
	{
		const_iterator itr = cgl::find_first_not_of(this->begin()+pos, this->end(), find_begin, find_end);
		return itr != this->end() ? itr-this->begin() : npos;
	}

	//find_last_not_of
	size_type find_last_not_of(const_reference value)const
	{
		const_iterator itr = cgl::find_last_not_of(this->begin(), this->end(), value);
		return itr != this->end() ? itr-this->begin() : npos;
	}
	size_type find_last_not_of(const_iterator find_begin, const_iterator find_end)const
	{
		return ::cgl::find_last_not_of(this->begin(), this->end(), find_begin, find_end) - this->begin();
	}
	size_type find_last_not_of(const_pointer str, size_type len = npos)const
	{
		if(len == npos)len = cslen(str);
		return this->find_last_not_of(str, str+len);
	}

	size_type find_last_not_of(const this_type& str, size_type len = npos)const
	{
		if(len == npos)len = str.length();
		return this->find_last_not_of(str.begin(), str.begin()+len);
	}

//compare
	int compare(const_pointer str)const
	{
		return traits_type::compare(this->buffer(), str, this->length());
	}
	int compare(const_pointer str, size_type count)const
	{
		return traits_type::compare(this->buffer(), str, count);
	}
	int compare(const this_type& str)const
	{
		return traits_type::compare(this->buffer(), str.c_str(), str.length());
	}
	int compare(const this_type& str, size_type count)const
	{
		return traits_type::compare(this->buffer(), str.c_str(), count);
	}

	int icompare(const_pointer str)const
	{
		return csicmp(this->buffer(), str, this->length()+1);
	}
	int icompare(const_pointer str, size_type count)const
	{
		return csicmp(this->buffer(), str, ++count);
	}
	int icompare(const this_type& str)const
	{
		return csicmp(this->buffer(), str.c_str(), this->length()+1);
	}
	int icompare(const this_type& str, size_type count)const
	{
		return csicmp(this->buffer(), str.c_str(), ++count);
	}

//sub string
	this_type substr(size_type pos, size_t n)const
	{
		if(pos < this->size()){
			if(pos + n >= this->size()){
				n = this->size() - pos - 1;
			}
			return this_type(this->buffer()+pos, n);
		}
		else{
			return this_type();
		}
	}

	/*
	stringref substr(size_type pos, size_t n)const
	*/
//copy data to c style string
	size_type copy(pointer str, size_type n, size_type offset = 0)
	{
		if(!str)return 0;
		if(0==n || this->length()<offset){
			str[0] = nullchar;
			return 0;
		}
		if(this->length()-offset < n){
			n = this->length()-offset;
		}
		traits_type::copy(str, this->buffer(), n);
		return n;
	}

//space() return is_space() characters
	static const char_type* space()
	{
		const static char_type str[] = {' ', '\t', '\r', '\n', '\f', '\v', '\0'};
		return str;
	}

//to_lower to_upper
	this_type& tolower()
	{
		std::transform(this->begin(), this->end(), this->begin(), std::tolower);
		return *this;
	}
	this_type& toupper()
	{
		std::transform(this->begin(), this->end(), this->begin(), std::toupper);
		return *this;
	}

//trim
	void trim_left(const this_type& trimmer = space())
	{
		this->erase(0, this->find_first_not_of(trimmer.begin(), trimmer.end()));
	}
	void trim_right(const this_type& trimmer = space())
	{
		//this->erase(this->find_last_not_of(trimmer)+1);
		this->endsize(this->find_last_not_of(trimmer.begin(), trimmer.end())+1);
	}
	void trim(const this_type& trimmer = space())
	{
		this->trim_right(trimmer);
		this->trim_left(trimmer);
	}

//format
	int format(size_t buf_size, const value_type* fmt, ...)
	{
		using namespace std;
		int result = 0;
		va_list body;
		va_start(body, fmt);
		this->reserve(buf_size);
		result = tvalue<_char_t>(vsnprintf, vsnwprintf)(this->buffer(), buf_size, fmt, body);
		va_end(body);
		this->endsize(result);
		return result;
	}

//string split
	    template<typename container>
        size_type split(container& ls, char_type spliter, bool repeat = true)const
        {
            return stringsplit(ls, c_str(), length(), spliter, repeat);
        }

        template<typename container>
        size_type split(container& ls, const this_type& spliter, bool repeat = true)const
        {
            return stringsplit(ls, *this, c_str(), length(), spliter.c_str(), spliter.length(), repeat);
        }

//to numeric
	int		to_int(int radix = 10)const		{ return cstol<value_type>(this->c_str(), null, radix); }
	int		to_int64(int radix = 10)const	{ return cstoll<value_type>(this->c_str(), null, radix); }
	double	to_decimal()const				{ return cstof(this->c_str()); }
	bool	to_boolean()const				{ return cstobool(this->c_str()); }

//swap
	void swap(this_type& other)//no throw function
	{
		if(this!=&other){
			std::swap(m_buf, other.m_buf);
			std::swap(m_size, other.m_size);
			std::swap(m_capacity, other.m_capacity);
		}
	}
protected:
	//是否使用cache
	inline bool is_cached()const { return !m_capacity; }

	this_type& _assign(const_pointer str, size_type count = npos)
	{
		if(!str || !count){//clear
			this->clear();
			return *this;
		}

		if(this->contains(str)){//substr 这个没用
			if(count != npos){
				this->erase(str-this->buffer()+count, npos);
			}
			if(this->buffer() != str){
				this->erase(0, str-this->buffer());
			}
		}
		else
		{
			//this->clear();
			if(count == npos){//get str length
				count = cslen(str);
				if(!count){
					this->clear();
					return *this;
				}
			}
			this->reserve(count);
			traits_type::copy(this->buffer(), str, count);
			this->endsize(count);
		}
		return *this;
	}

	//
	size_type allocate(size_type size)
	{
		if(this->capacity()<size)
		{
			++size;
			//对齐到16个字符
			//size_t n = 16/sizeof(char_type)-1;
			const static int n = 15;
			size = size + n;
			size &= ~n;

			pointer pstr = this->allocator.allocate(size);
			if(pstr){
				traits_type::copy(pstr, this->buffer(), this->length());
				pstr[this->length()] = nullchar;
				if(!this->is_cached())
				{
					this->allocator.deallocate(m_buf.data, this->length());
				}
				m_buf.data = pstr;
				m_capacity = size;
			}
		}
		return this->capacity();
	}

	//扩展使用空间
	inline void expand(size_type n)
	{
		n += this->length();
		size_type m = CACHE_SIZE;
		if(m<n){
			while(m-1<n){
				if(m<4096){
					m = m*2;
				}
				else{
					m*=1.6;
				}
			}
			this->reserve(m-1);
		}
	}

	//set string end pos
	inline void endsize(size_type n)
	{
		this->buffer()[m_size = n] = nullchar;
	}

    //判断str是否在字符串内部
	bool contains(const_pointer str)
	{
		return str >= this->begin() && str < this->end();
	}

	inline const_pointer buffer()const { return this->is_cached() ? m_buf.cache : m_buf.data; }
	inline pointer buffer() { return this->is_cached() ? m_buf.cache : m_buf.data; }

	//free memory
	void dispose()
	{
		if(!this->is_cached())
		{
			this->allocator.deallocate(m_buf.data, m_size);
		}
		m_buf.data = null;
		m_size = m_capacity = 0;
	}
};

//allocator declaration
template<typename T, size_t _Cx, class _Ax>
_Ax cstring<T, _Cx, _Ax>::allocator = _Ax();

//---------------------------------------------------------------------------
//erase

//---------------------------------------------------------------------------
//operator +
template<typename T, size_t C, class A>
cstring<T, C, A> operator+(const cstring<T, C, A>& left, const cstring<T, C, A>& right)
{
	cstring<T, C, A> str;
	str.reserve(left.length()+right.length());
	str.append(left);
	str.append(right);
	return str;
}

template<typename T, size_t C, class A>
cstring<T, C, A> operator+(const cstring<T, C, A>& left, const T* right)
{
	cstring<T, C, A> str;
	size_t len = cslen(right);
	str.reserve(left.length()+len);
	str.append(left);
	str.append(right, len);
	return str;
}

template<typename T, size_t C, class A>
cstring<T, C, A> operator+(const T* left, const cstring<T, C, A>& right)
{
	cstring<T, C, A> str;
	size_t len = cslen(left);
	str.reserve(len+right.length());
	str.append(left, len);
	str.append(right);
	return str;
}

template<typename T, size_t C, class A>
cstring<T, C, A> operator+(const T& left, const cstring<T, C, A>& right)
{
	cstring<T, C, A> str;
	str.reserve(1+right.length());
	str.append(left, 1);
	str.append(right);
	return str;
}

template<typename T, size_t C, class A>
cstring<T, C, A> operator+(const cstring<T, C, A>& left, const T& right)
{
	cstring<T, C, A> str;
	str.reserve(1+left.length());
	str.append(left, left.length());
	str.append(right);
	return str;
}

//---------------------------------------------------------------------------
//operator <
template<typename T, size_t C, class A>
bool operator<(const cstring<T, C, A>& left, const cstring<T, C, A>& right)
{
	return char_traits<T>::compare(left.c_str(), right.c_str(), left.length()) < 0;
}

template<typename T, size_t C, class A>
bool operator<(const cstring<T, C, A>& left, const T* right)
{
	return char_traits<T>::compare(left.c_str(), right, left.length()) < 0;
}

template<typename T, size_t C, class A>
bool operator<(const T* left, const cstring<T, C, A>& right)
{
	return char_traits<T>::compare(left, right.c_str(), right.length()) < 0;
}

//---------------------------------------------------------------------------
//operator ==
template<typename T, size_t C, class A>
bool operator==(const cstring<T, C, A>& left, const cstring<T, C, A>& right)
{
	return !char_traits<T>::compare(left.c_str(), right.c_str(), left.length());
}

template<typename T, size_t C, class A>
bool operator==(const cstring<T, C, A>& left, const T* right)
{
	return !char_traits<T>::compare(left.c_str(), right, left.length());
}

template<typename T, size_t C, class A>
bool operator==(const T* left, const cstring<T, C, A>& right)
{
	return !char_traits<T>::compare(left, right.c_str(), right.length());
}
//operator !=
template<typename T, size_t C, class A>
bool operator!=(const cstring<T, C, A>& left, const cstring<T, C, A>& right)
{
	return char_traits<T>::compare(left.c_str(), right.c_str(), left.length());
}

template<typename T, size_t C, class A>
bool operator!=(const cstring<T, C, A>& left, const T* right)
{
	return char_traits<T>::compare(left.c_str(), right, left.length());
}

template<typename T, size_t C, class A>
bool operator!=(const T* left, const cstring<T, C, A>& right)
{
	return char_traits<T>::compare(left, right.c_str(), right.length());
}

//---------------------------------------------------------------------------

typedef cstring<tchar_t>  tstring_t;
typedef cstring<char>     string_t;
typedef cstring<wchar_t>  wstring_t;
typedef cstring<char8_t>  string8_t;
typedef cstring<char16_t> string16_t;
typedef cstring<char32_t> string32_t;

//---------------------------------------------------------------------------
//swap
template<typename _char_t, size_t _Cx, class _Ax>
inline void swap(cgl::cstring<_char_t, _Cx, _Ax>& left, cgl::cstring<_char_t, _Cx, _Ax>& right)
{
	return left.swap(right);
}

}//end namespace cgl

#ifndef CGL_PLATFORM_GNUC
#pragma option pop
#endif

#endif //STRINGX_HXX_20150901125352


