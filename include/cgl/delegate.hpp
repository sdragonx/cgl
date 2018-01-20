/*

 delegate.hpp

 委托类

 程序生成的代码，最大支持16个参数，支持__cdecl、__stdcall、__fastcall
 编辑器不支持C11也能用，c++builder 6.0测试可用
 为了通用性，只能这么暴力了

 sdragonx 2017-02-15 03:41:29

 sdragonx 1:05 2018/1/10 修订

 delegate<void (*)(...)> func;
 func.set_function(&class, &class::function);
 func.set_function(function);
 func(...);//执行

*/
#ifndef DELEGATE_HPP_20170215034129
#define DELEGATE_HPP_20170215034129

#ifndef NULL_HPP_20180114125304
	#define null NULL
#endif

namespace cgl{

template<typename T>
class delegator_base;

template<typename C, typename T>
class delegator;

template<typename T>
class delegate;

//---------------------------------------------------------------------------
// T (*)()

template<typename T>
class delegator_base<T (*)()>
{
public:
	virtual T invoke()=0;
	virtual delegator_base* clone()const=0;
};

template<typename classT, typename T>
class delegator<classT, T __cdecl (*)()> : public delegator_base<T (*)()>
{
public:
	typedef T __cdecl (classT::*function_type)();
	classT *_C;
	function_type _F;
	delegator(classT *c, function_type func):_C(c), _F(func){/*void*/}
	T invoke()
	{
		return (_C->*_F)();
	}
	delegator_base<T (*)()>* clone()const
	{
		return new delegator(*this);
	}
};

template<typename T>
class delegator<void, T __cdecl (*)()> : public delegator_base<T (*)()>
{
public:
	typedef T __cdecl (*function_type)();
	function_type _F;
	delegator(function_type func):_F(func){/*void*/}
	T invoke()
	{
		return (*_F)();
	}
	delegator_base<T (*)()>* clone()const
	{
		return new delegator(*this);
	}
};

template<typename classT, typename T>
class delegator<classT, T __stdcall (*)()> : public delegator_base<T (*)()>
{
public:
	typedef T __stdcall (classT::*function_type)();
	classT *_C;
	function_type _F;
	delegator(classT *c, function_type func):_C(c), _F(func){/*void*/}
	T invoke()
	{
		return (_C->*_F)();
	}
	delegator_base<T (*)()>* clone()const
	{
		return new delegator(*this);
	}
};

template<typename T>
class delegator<void, T __stdcall (*)()> : public delegator_base<T (*)()>
{
public:
	typedef T __stdcall (*function_type)();
	function_type _F;
	delegator(function_type func):_F(func){/*void*/}
	T invoke()
	{
		return (*_F)();
	}
	delegator_base<T (*)()>* clone()const
	{
		return new delegator(*this);
	}
};

template<typename classT, typename T>
class delegator<classT, T __fastcall (*)()> : public delegator_base<T (*)()>
{
public:
	typedef T __fastcall (classT::*function_type)();
	classT *_C;
	function_type _F;
	delegator(classT *c, function_type func):_C(c), _F(func){/*void*/}
	T invoke()
	{
		return (_C->*_F)();
	}
	delegator_base<T (*)()>* clone()const
	{
		return new delegator(*this);
	}
};

template<typename T>
class delegator<void, T __fastcall (*)()> : public delegator_base<T (*)()>
{
public:
	typedef T __fastcall (*function_type)();
	function_type _F;
	delegator(function_type func):_F(func){/*void*/}
	T invoke()
	{
		return (*_F)();
	}
	delegator_base<T (*)()>* clone()const
	{
		return new delegator(*this);
	}
};

template<typename T>
class delegate<T (*)()>
{
public:
	typedef T (*std_function)();
	typedef T __cdecl (*cdecl_function)();
	typedef T __stdcall (*stdcall_function)();
	typedef T __fastcall (*fastcall_function)();
	typedef delegate<std_function> this_type;
	delegator_base<std_function> *base;
	delegate():base(null){/*void*/};
	delegate(const this_type& d):base(d.base_clone()){/*void*/}
	~delegate()
	{
		this->destroy();
	}
	operator bool()const
	{
		return base;
	}
	T operator()()
	{
		return base->invoke();
	}
	void destroy()
	{
		if(base)delete base, base = null;
	}
	delegator_base<std_function>* base_clone()const
	{
		if(base)return base->clone(); else return null;
	}
	const this_type& operator=(const this_type& d)
	{
		this->destroy();
		base = d.base_clone();
		return *this;
	}
	template<typename classT>
	void set_function(classT *c, T __cdecl (classT::*func)())
	{
		this->destroy();
		base = new delegator<classT, cdecl_function>(c, func);
	}
	template<typename classT>
	void set_function(classT *c, T __stdcall (classT::*func)())
	{
		this->destroy();
		base = new delegator<classT, stdcall_function>(c, func);
	}
	template<typename classT>
	void set_function(classT *c, T __fastcall (classT::*func)())
	{
		this->destroy();
		base = new delegator<classT, fastcall_function>(c, func);
	}
	void set_function(cdecl_function func)
	{
		this->destroy();
		base = new delegator<void, cdecl_function>(func);
	}
	void set_function(stdcall_function func)
	{
		this->destroy();
		base = new delegator<void, stdcall_function>(func);
	}
	void set_function(fastcall_function func)
	{
		this->destroy();
		base = new delegator<void, fastcall_function>(func);
	}
};

//---------------------------------------------------------------------------
// T (*)(T0)

template<typename T, typename T0>
class delegator_base<T (*)(T0)>
{
public:
	virtual T invoke(T0)=0;
	virtual delegator_base* clone()const=0;
};

template<typename classT, typename T, typename T0>
class delegator<classT, T __cdecl (*)(T0)> : public delegator_base<T (*)(T0)>
{
public:
	typedef T __cdecl (classT::*function_type)(T0);
	classT *_C;
	function_type _F;
	delegator(classT *c, function_type func):_C(c), _F(func){/*void*/}
	T invoke(T0 p0)
	{
		return (_C->*_F)(p0);
	}
	delegator_base<T (*)(T0)>* clone()const
	{
		return new delegator(*this);
	}
};

template<typename T, typename T0>
class delegator<void, T __cdecl (*)(T0)> : public delegator_base<T (*)(T0)>
{
public:
	typedef T __cdecl (*function_type)(T0);
	function_type _F;
	delegator(function_type func):_F(func){/*void*/}
	T invoke(T0 p0)
	{
		return (*_F)(p0);
	}
	delegator_base<T (*)(T0)>* clone()const
	{
		return new delegator(*this);
	}
};

template<typename classT, typename T, typename T0>
class delegator<classT, T __stdcall (*)(T0)> : public delegator_base<T (*)(T0)>
{
public:
	typedef T __stdcall (classT::*function_type)(T0);
	classT *_C;
	function_type _F;
	delegator(classT *c, function_type func):_C(c), _F(func){/*void*/}
	T invoke(T0 p0)
	{
		return (_C->*_F)(p0);
	}
	delegator_base<T (*)(T0)>* clone()const
	{
		return new delegator(*this);
	}
};

template<typename T, typename T0>
class delegator<void, T __stdcall (*)(T0)> : public delegator_base<T (*)(T0)>
{
public:
	typedef T __stdcall (*function_type)(T0);
	function_type _F;
	delegator(function_type func):_F(func){/*void*/}
	T invoke(T0 p0)
	{
		return (*_F)(p0);
	}
	delegator_base<T (*)(T0)>* clone()const
	{
		return new delegator(*this);
	}
};

template<typename classT, typename T, typename T0>
class delegator<classT, T __fastcall (*)(T0)> : public delegator_base<T (*)(T0)>
{
public:
	typedef T __fastcall (classT::*function_type)(T0);
	classT *_C;
	function_type _F;
	delegator(classT *c, function_type func):_C(c), _F(func){/*void*/}
	T invoke(T0 p0)
	{
		return (_C->*_F)(p0);
	}
	delegator_base<T (*)(T0)>* clone()const
	{
		return new delegator(*this);
	}
};

template<typename T, typename T0>
class delegator<void, T __fastcall (*)(T0)> : public delegator_base<T (*)(T0)>
{
public:
	typedef T __fastcall (*function_type)(T0);
	function_type _F;
	delegator(function_type func):_F(func){/*void*/}
	T invoke(T0 p0)
	{
		return (*_F)(p0);
	}
	delegator_base<T (*)(T0)>* clone()const
	{
		return new delegator(*this);
	}
};

template<typename T, typename T0>
class delegate<T (*)(T0)>
{
public:
	typedef T (*std_function)(T0);
	typedef T __cdecl (*cdecl_function)(T0);
	typedef T __stdcall (*stdcall_function)(T0);
	typedef T __fastcall (*fastcall_function)(T0);
	typedef delegate<std_function> this_type;
	delegator_base<std_function> *base;
	delegate():base(null){/*void*/};
	delegate(const this_type& d):base(d.base_clone()){/*void*/}
	~delegate()
	{
		this->destroy();
	}
	operator bool()const
	{
		return base;
	}
	T operator()(T0 p0)
	{
		return base->invoke(p0);
	}
	void destroy()
	{
		if(base)delete base, base = null;
	}
	delegator_base<std_function>* base_clone()const
	{
		if(base)return base->clone(); else return null;
	}
	const this_type& operator=(const this_type& d)
	{
		this->destroy();
		base = d.base_clone();
		return *this;
	}
	template<typename classT>
	void set_function(classT *c, T __cdecl (classT::*func)(T0))
	{
		this->destroy();
		base = new delegator<classT, cdecl_function>(c, func);
	}
	template<typename classT>
	void set_function(classT *c, T __stdcall (classT::*func)(T0))
	{
		this->destroy();
		base = new delegator<classT, stdcall_function>(c, func);
	}
	template<typename classT>
	void set_function(classT *c, T __fastcall (classT::*func)(T0))
	{
		this->destroy();
		base = new delegator<classT, fastcall_function>(c, func);
	}
	void set_function(cdecl_function func)
	{
		this->destroy();
		base = new delegator<void, cdecl_function>(func);
	}
	void set_function(stdcall_function func)
	{
		this->destroy();
		base = new delegator<void, stdcall_function>(func);
	}
	void set_function(fastcall_function func)
	{
		this->destroy();
		base = new delegator<void, fastcall_function>(func);
	}
};

//---------------------------------------------------------------------------
// T (*)(T0, T1)

template<typename T, typename T0, typename T1>
class delegator_base<T (*)(T0, T1)>
{
public:
	virtual T invoke(T0, T1)=0;
	virtual delegator_base* clone()const=0;
};

template<typename classT, typename T, typename T0, typename T1>
class delegator<classT, T __cdecl (*)(T0, T1)> : public delegator_base<T (*)(T0, T1)>
{
public:
	typedef T __cdecl (classT::*function_type)(T0, T1);
	classT *_C;
	function_type _F;
	delegator(classT *c, function_type func):_C(c), _F(func){/*void*/}
	T invoke(T0 p0, T1 p1)
	{
		return (_C->*_F)(p0, p1);
	}
	delegator_base<T (*)(T0, T1)>* clone()const
	{
		return new delegator(*this);
	}
};

template<typename T, typename T0, typename T1>
class delegator<void, T __cdecl (*)(T0, T1)> : public delegator_base<T (*)(T0, T1)>
{
public:
	typedef T __cdecl (*function_type)(T0, T1);
	function_type _F;
	delegator(function_type func):_F(func){/*void*/}
	T invoke(T0 p0, T1 p1)
	{
		return (*_F)(p0, p1);
	}
	delegator_base<T (*)(T0, T1)>* clone()const
	{
		return new delegator(*this);
	}
};

template<typename classT, typename T, typename T0, typename T1>
class delegator<classT, T __stdcall (*)(T0, T1)> : public delegator_base<T (*)(T0, T1)>
{
public:
	typedef T __stdcall (classT::*function_type)(T0, T1);
	classT *_C;
	function_type _F;
	delegator(classT *c, function_type func):_C(c), _F(func){/*void*/}
	T invoke(T0 p0, T1 p1)
	{
		return (_C->*_F)(p0, p1);
	}
	delegator_base<T (*)(T0, T1)>* clone()const
	{
		return new delegator(*this);
	}
};

template<typename T, typename T0, typename T1>
class delegator<void, T __stdcall (*)(T0, T1)> : public delegator_base<T (*)(T0, T1)>
{
public:
	typedef T __stdcall (*function_type)(T0, T1);
	function_type _F;
	delegator(function_type func):_F(func){/*void*/}
	T invoke(T0 p0, T1 p1)
	{
		return (*_F)(p0, p1);
	}
	delegator_base<T (*)(T0, T1)>* clone()const
	{
		return new delegator(*this);
	}
};

template<typename classT, typename T, typename T0, typename T1>
class delegator<classT, T __fastcall (*)(T0, T1)> : public delegator_base<T (*)(T0, T1)>
{
public:
	typedef T __fastcall (classT::*function_type)(T0, T1);
	classT *_C;
	function_type _F;
	delegator(classT *c, function_type func):_C(c), _F(func){/*void*/}
	T invoke(T0 p0, T1 p1)
	{
		return (_C->*_F)(p0, p1);
	}
	delegator_base<T (*)(T0, T1)>* clone()const
	{
		return new delegator(*this);
	}
};

template<typename T, typename T0, typename T1>
class delegator<void, T __fastcall (*)(T0, T1)> : public delegator_base<T (*)(T0, T1)>
{
public:
	typedef T __fastcall (*function_type)(T0, T1);
	function_type _F;
	delegator(function_type func):_F(func){/*void*/}
	T invoke(T0 p0, T1 p1)
	{
		return (*_F)(p0, p1);
	}
	delegator_base<T (*)(T0, T1)>* clone()const
	{
		return new delegator(*this);
	}
};

template<typename T, typename T0, typename T1>
class delegate<T (*)(T0, T1)>
{
public:
	typedef T (*std_function)(T0, T1);
	typedef T __cdecl (*cdecl_function)(T0, T1);
	typedef T __stdcall (*stdcall_function)(T0, T1);
	typedef T __fastcall (*fastcall_function)(T0, T1);
	typedef delegate<std_function> this_type;
	delegator_base<std_function> *base;
	delegate():base(null){/*void*/};
	delegate(const this_type& d):base(d.base_clone()){/*void*/}
	~delegate()
	{
		this->destroy();
	}
	operator bool()const
	{
		return base;
	}
	T operator()(T0 p0, T1 p1)
	{
		return base->invoke(p0, p1);
	}
	void destroy()
	{
		if(base)delete base, base = null;
	}
	delegator_base<std_function>* base_clone()const
	{
		if(base)return base->clone(); else return null;
	}
	const this_type& operator=(const this_type& d)
	{
		this->destroy();
		base = d.base_clone();
		return *this;
	}
	template<typename classT>
	void set_function(classT *c, T __cdecl (classT::*func)(T0, T1))
	{
		this->destroy();
		base = new delegator<classT, cdecl_function>(c, func);
	}
	template<typename classT>
	void set_function(classT *c, T __stdcall (classT::*func)(T0, T1))
	{
		this->destroy();
		base = new delegator<classT, stdcall_function>(c, func);
	}
	template<typename classT>
	void set_function(classT *c, T __fastcall (classT::*func)(T0, T1))
	{
		this->destroy();
		base = new delegator<classT, fastcall_function>(c, func);
	}
	void set_function(cdecl_function func)
	{
		this->destroy();
		base = new delegator<void, cdecl_function>(func);
	}
	void set_function(stdcall_function func)
	{
		this->destroy();
		base = new delegator<void, stdcall_function>(func);
	}
	void set_function(fastcall_function func)
	{
		this->destroy();
		base = new delegator<void, fastcall_function>(func);
	}
};

//---------------------------------------------------------------------------
// T (*)(T0, T1, T2)

template<typename T, typename T0, typename T1, typename T2>
class delegator_base<T (*)(T0, T1, T2)>
{
public:
	virtual T invoke(T0, T1, T2)=0;
	virtual delegator_base* clone()const=0;
};

template<typename classT, typename T, typename T0, typename T1, typename T2>
class delegator<classT, T __cdecl (*)(T0, T1, T2)> : public delegator_base<T (*)(T0, T1, T2)>
{
public:
	typedef T __cdecl (classT::*function_type)(T0, T1, T2);
	classT *_C;
	function_type _F;
	delegator(classT *c, function_type func):_C(c), _F(func){/*void*/}
	T invoke(T0 p0, T1 p1, T2 p2)
	{
		return (_C->*_F)(p0, p1, p2);
	}
	delegator_base<T (*)(T0, T1, T2)>* clone()const
	{
		return new delegator(*this);
	}
};

template<typename T, typename T0, typename T1, typename T2>
class delegator<void, T __cdecl (*)(T0, T1, T2)> : public delegator_base<T (*)(T0, T1, T2)>
{
public:
	typedef T __cdecl (*function_type)(T0, T1, T2);
	function_type _F;
	delegator(function_type func):_F(func){/*void*/}
	T invoke(T0 p0, T1 p1, T2 p2)
	{
		return (*_F)(p0, p1, p2);
	}
	delegator_base<T (*)(T0, T1, T2)>* clone()const
	{
		return new delegator(*this);
	}
};

template<typename classT, typename T, typename T0, typename T1, typename T2>
class delegator<classT, T __stdcall (*)(T0, T1, T2)> : public delegator_base<T (*)(T0, T1, T2)>
{
public:
	typedef T __stdcall (classT::*function_type)(T0, T1, T2);
	classT *_C;
	function_type _F;
	delegator(classT *c, function_type func):_C(c), _F(func){/*void*/}
	T invoke(T0 p0, T1 p1, T2 p2)
	{
		return (_C->*_F)(p0, p1, p2);
	}
	delegator_base<T (*)(T0, T1, T2)>* clone()const
	{
		return new delegator(*this);
	}
};

template<typename T, typename T0, typename T1, typename T2>
class delegator<void, T __stdcall (*)(T0, T1, T2)> : public delegator_base<T (*)(T0, T1, T2)>
{
public:
	typedef T __stdcall (*function_type)(T0, T1, T2);
	function_type _F;
	delegator(function_type func):_F(func){/*void*/}
	T invoke(T0 p0, T1 p1, T2 p2)
	{
		return (*_F)(p0, p1, p2);
	}
	delegator_base<T (*)(T0, T1, T2)>* clone()const
	{
		return new delegator(*this);
	}
};

template<typename classT, typename T, typename T0, typename T1, typename T2>
class delegator<classT, T __fastcall (*)(T0, T1, T2)> : public delegator_base<T (*)(T0, T1, T2)>
{
public:
	typedef T __fastcall (classT::*function_type)(T0, T1, T2);
	classT *_C;
	function_type _F;
	delegator(classT *c, function_type func):_C(c), _F(func){/*void*/}
	T invoke(T0 p0, T1 p1, T2 p2)
	{
		return (_C->*_F)(p0, p1, p2);
	}
	delegator_base<T (*)(T0, T1, T2)>* clone()const
	{
		return new delegator(*this);
	}
};

template<typename T, typename T0, typename T1, typename T2>
class delegator<void, T __fastcall (*)(T0, T1, T2)> : public delegator_base<T (*)(T0, T1, T2)>
{
public:
	typedef T __fastcall (*function_type)(T0, T1, T2);
	function_type _F;
	delegator(function_type func):_F(func){/*void*/}
	T invoke(T0 p0, T1 p1, T2 p2)
	{
		return (*_F)(p0, p1, p2);
	}
	delegator_base<T (*)(T0, T1, T2)>* clone()const
	{
		return new delegator(*this);
	}
};

template<typename T, typename T0, typename T1, typename T2>
class delegate<T (*)(T0, T1, T2)>
{
public:
	typedef T (*std_function)(T0, T1, T2);
	typedef T __cdecl (*cdecl_function)(T0, T1, T2);
	typedef T __stdcall (*stdcall_function)(T0, T1, T2);
	typedef T __fastcall (*fastcall_function)(T0, T1, T2);
	typedef delegate<std_function> this_type;
	delegator_base<std_function> *base;
	delegate():base(null){/*void*/};
	delegate(const this_type& d):base(d.base_clone()){/*void*/}
	~delegate()
	{
		this->destroy();
	}
	operator bool()const
	{
		return base;
	}
	T operator()(T0 p0, T1 p1, T2 p2)
	{
		return base->invoke(p0, p1, p2);
	}
	void destroy()
	{
		if(base)delete base, base = null;
	}
	delegator_base<std_function>* base_clone()const
	{
		if(base)return base->clone(); else return null;
	}
	const this_type& operator=(const this_type& d)
	{
		this->destroy();
		base = d.base_clone();
		return *this;
	}
	template<typename classT>
	void set_function(classT *c, T __cdecl (classT::*func)(T0, T1, T2))
	{
		this->destroy();
		base = new delegator<classT, cdecl_function>(c, func);
	}
	template<typename classT>
	void set_function(classT *c, T __stdcall (classT::*func)(T0, T1, T2))
	{
		this->destroy();
		base = new delegator<classT, stdcall_function>(c, func);
	}
	template<typename classT>
	void set_function(classT *c, T __fastcall (classT::*func)(T0, T1, T2))
	{
		this->destroy();
		base = new delegator<classT, fastcall_function>(c, func);
	}
	void set_function(cdecl_function func)
	{
		this->destroy();
		base = new delegator<void, cdecl_function>(func);
	}
	void set_function(stdcall_function func)
	{
		this->destroy();
		base = new delegator<void, stdcall_function>(func);
	}
	void set_function(fastcall_function func)
	{
		this->destroy();
		base = new delegator<void, fastcall_function>(func);
	}
};

//---------------------------------------------------------------------------
// T (*)(T0, T1, T2, T3)

template<typename T, typename T0, typename T1, typename T2, typename T3>
class delegator_base<T (*)(T0, T1, T2, T3)>
{
public:
	virtual T invoke(T0, T1, T2, T3)=0;
	virtual delegator_base* clone()const=0;
};

template<typename classT, typename T, typename T0, typename T1, typename T2, typename T3>
class delegator<classT, T __cdecl (*)(T0, T1, T2, T3)> : public delegator_base<T (*)(T0, T1, T2, T3)>
{
public:
	typedef T __cdecl (classT::*function_type)(T0, T1, T2, T3);
	classT *_C;
	function_type _F;
	delegator(classT *c, function_type func):_C(c), _F(func){/*void*/}
	T invoke(T0 p0, T1 p1, T2 p2, T3 p3)
	{
		return (_C->*_F)(p0, p1, p2, p3);
	}
	delegator_base<T (*)(T0, T1, T2, T3)>* clone()const
	{
		return new delegator(*this);
	}
};

template<typename T, typename T0, typename T1, typename T2, typename T3>
class delegator<void, T __cdecl (*)(T0, T1, T2, T3)> : public delegator_base<T (*)(T0, T1, T2, T3)>
{
public:
	typedef T __cdecl (*function_type)(T0, T1, T2, T3);
	function_type _F;
	delegator(function_type func):_F(func){/*void*/}
	T invoke(T0 p0, T1 p1, T2 p2, T3 p3)
	{
		return (*_F)(p0, p1, p2, p3);
	}
	delegator_base<T (*)(T0, T1, T2, T3)>* clone()const
	{
		return new delegator(*this);
	}
};

template<typename classT, typename T, typename T0, typename T1, typename T2, typename T3>
class delegator<classT, T __stdcall (*)(T0, T1, T2, T3)> : public delegator_base<T (*)(T0, T1, T2, T3)>
{
public:
	typedef T __stdcall (classT::*function_type)(T0, T1, T2, T3);
	classT *_C;
	function_type _F;
	delegator(classT *c, function_type func):_C(c), _F(func){/*void*/}
	T invoke(T0 p0, T1 p1, T2 p2, T3 p3)
	{
		return (_C->*_F)(p0, p1, p2, p3);
	}
	delegator_base<T (*)(T0, T1, T2, T3)>* clone()const
	{
		return new delegator(*this);
	}
};

template<typename T, typename T0, typename T1, typename T2, typename T3>
class delegator<void, T __stdcall (*)(T0, T1, T2, T3)> : public delegator_base<T (*)(T0, T1, T2, T3)>
{
public:
	typedef T __stdcall (*function_type)(T0, T1, T2, T3);
	function_type _F;
	delegator(function_type func):_F(func){/*void*/}
	T invoke(T0 p0, T1 p1, T2 p2, T3 p3)
	{
		return (*_F)(p0, p1, p2, p3);
	}
	delegator_base<T (*)(T0, T1, T2, T3)>* clone()const
	{
		return new delegator(*this);
	}
};

template<typename classT, typename T, typename T0, typename T1, typename T2, typename T3>
class delegator<classT, T __fastcall (*)(T0, T1, T2, T3)> : public delegator_base<T (*)(T0, T1, T2, T3)>
{
public:
	typedef T __fastcall (classT::*function_type)(T0, T1, T2, T3);
	classT *_C;
	function_type _F;
	delegator(classT *c, function_type func):_C(c), _F(func){/*void*/}
	T invoke(T0 p0, T1 p1, T2 p2, T3 p3)
	{
		return (_C->*_F)(p0, p1, p2, p3);
	}
	delegator_base<T (*)(T0, T1, T2, T3)>* clone()const
	{
		return new delegator(*this);
	}
};

template<typename T, typename T0, typename T1, typename T2, typename T3>
class delegator<void, T __fastcall (*)(T0, T1, T2, T3)> : public delegator_base<T (*)(T0, T1, T2, T3)>
{
public:
	typedef T __fastcall (*function_type)(T0, T1, T2, T3);
	function_type _F;
	delegator(function_type func):_F(func){/*void*/}
	T invoke(T0 p0, T1 p1, T2 p2, T3 p3)
	{
		return (*_F)(p0, p1, p2, p3);
	}
	delegator_base<T (*)(T0, T1, T2, T3)>* clone()const
	{
		return new delegator(*this);
	}
};

template<typename T, typename T0, typename T1, typename T2, typename T3>
class delegate<T (*)(T0, T1, T2, T3)>
{
public:
	typedef T (*std_function)(T0, T1, T2, T3);
	typedef T __cdecl (*cdecl_function)(T0, T1, T2, T3);
	typedef T __stdcall (*stdcall_function)(T0, T1, T2, T3);
	typedef T __fastcall (*fastcall_function)(T0, T1, T2, T3);
	typedef delegate<std_function> this_type;
	delegator_base<std_function> *base;
	delegate():base(null){/*void*/};
	delegate(const this_type& d):base(d.base_clone()){/*void*/}
	~delegate()
	{
		this->destroy();
	}
	operator bool()const
	{
		return base;
	}
	T operator()(T0 p0, T1 p1, T2 p2, T3 p3)
	{
		return base->invoke(p0, p1, p2, p3);
	}
	void destroy()
	{
		if(base)delete base, base = null;
	}
	delegator_base<std_function>* base_clone()const
	{
		if(base)return base->clone(); else return null;
	}
	const this_type& operator=(const this_type& d)
	{
		this->destroy();
		base = d.base_clone();
		return *this;
	}
	template<typename classT>
	void set_function(classT *c, T __cdecl (classT::*func)(T0, T1, T2, T3))
	{
		this->destroy();
		base = new delegator<classT, cdecl_function>(c, func);
	}
	template<typename classT>
	void set_function(classT *c, T __stdcall (classT::*func)(T0, T1, T2, T3))
	{
		this->destroy();
		base = new delegator<classT, stdcall_function>(c, func);
	}
	template<typename classT>
	void set_function(classT *c, T __fastcall (classT::*func)(T0, T1, T2, T3))
	{
		this->destroy();
		base = new delegator<classT, fastcall_function>(c, func);
	}
	void set_function(cdecl_function func)
	{
		this->destroy();
		base = new delegator<void, cdecl_function>(func);
	}
	void set_function(stdcall_function func)
	{
		this->destroy();
		base = new delegator<void, stdcall_function>(func);
	}
	void set_function(fastcall_function func)
	{
		this->destroy();
		base = new delegator<void, fastcall_function>(func);
	}
};

//---------------------------------------------------------------------------
// T (*)(T0, T1, T2, T3, T4)

template<typename T, typename T0, typename T1, typename T2, typename T3, typename T4>
class delegator_base<T (*)(T0, T1, T2, T3, T4)>
{
public:
	virtual T invoke(T0, T1, T2, T3, T4)=0;
	virtual delegator_base* clone()const=0;
};

template<typename classT, typename T, typename T0, typename T1, typename T2, typename T3, typename T4>
class delegator<classT, T __cdecl (*)(T0, T1, T2, T3, T4)> : public delegator_base<T (*)(T0, T1, T2, T3, T4)>
{
public:
	typedef T __cdecl (classT::*function_type)(T0, T1, T2, T3, T4);
	classT *_C;
	function_type _F;
	delegator(classT *c, function_type func):_C(c), _F(func){/*void*/}
	T invoke(T0 p0, T1 p1, T2 p2, T3 p3, T4 p4)
	{
		return (_C->*_F)(p0, p1, p2, p3, p4);
	}
	delegator_base<T (*)(T0, T1, T2, T3, T4)>* clone()const
	{
		return new delegator(*this);
	}
};

template<typename T, typename T0, typename T1, typename T2, typename T3, typename T4>
class delegator<void, T __cdecl (*)(T0, T1, T2, T3, T4)> : public delegator_base<T (*)(T0, T1, T2, T3, T4)>
{
public:
	typedef T __cdecl (*function_type)(T0, T1, T2, T3, T4);
	function_type _F;
	delegator(function_type func):_F(func){/*void*/}
	T invoke(T0 p0, T1 p1, T2 p2, T3 p3, T4 p4)
	{
		return (*_F)(p0, p1, p2, p3, p4);
	}
	delegator_base<T (*)(T0, T1, T2, T3, T4)>* clone()const
	{
		return new delegator(*this);
	}
};

template<typename classT, typename T, typename T0, typename T1, typename T2, typename T3, typename T4>
class delegator<classT, T __stdcall (*)(T0, T1, T2, T3, T4)> : public delegator_base<T (*)(T0, T1, T2, T3, T4)>
{
public:
	typedef T __stdcall (classT::*function_type)(T0, T1, T2, T3, T4);
	classT *_C;
	function_type _F;
	delegator(classT *c, function_type func):_C(c), _F(func){/*void*/}
	T invoke(T0 p0, T1 p1, T2 p2, T3 p3, T4 p4)
	{
		return (_C->*_F)(p0, p1, p2, p3, p4);
	}
	delegator_base<T (*)(T0, T1, T2, T3, T4)>* clone()const
	{
		return new delegator(*this);
	}
};

template<typename T, typename T0, typename T1, typename T2, typename T3, typename T4>
class delegator<void, T __stdcall (*)(T0, T1, T2, T3, T4)> : public delegator_base<T (*)(T0, T1, T2, T3, T4)>
{
public:
	typedef T __stdcall (*function_type)(T0, T1, T2, T3, T4);
	function_type _F;
	delegator(function_type func):_F(func){/*void*/}
	T invoke(T0 p0, T1 p1, T2 p2, T3 p3, T4 p4)
	{
		return (*_F)(p0, p1, p2, p3, p4);
	}
	delegator_base<T (*)(T0, T1, T2, T3, T4)>* clone()const
	{
		return new delegator(*this);
	}
};

template<typename classT, typename T, typename T0, typename T1, typename T2, typename T3, typename T4>
class delegator<classT, T __fastcall (*)(T0, T1, T2, T3, T4)> : public delegator_base<T (*)(T0, T1, T2, T3, T4)>
{
public:
	typedef T __fastcall (classT::*function_type)(T0, T1, T2, T3, T4);
	classT *_C;
	function_type _F;
	delegator(classT *c, function_type func):_C(c), _F(func){/*void*/}
	T invoke(T0 p0, T1 p1, T2 p2, T3 p3, T4 p4)
	{
		return (_C->*_F)(p0, p1, p2, p3, p4);
	}
	delegator_base<T (*)(T0, T1, T2, T3, T4)>* clone()const
	{
		return new delegator(*this);
	}
};

template<typename T, typename T0, typename T1, typename T2, typename T3, typename T4>
class delegator<void, T __fastcall (*)(T0, T1, T2, T3, T4)> : public delegator_base<T (*)(T0, T1, T2, T3, T4)>
{
public:
	typedef T __fastcall (*function_type)(T0, T1, T2, T3, T4);
	function_type _F;
	delegator(function_type func):_F(func){/*void*/}
	T invoke(T0 p0, T1 p1, T2 p2, T3 p3, T4 p4)
	{
		return (*_F)(p0, p1, p2, p3, p4);
	}
	delegator_base<T (*)(T0, T1, T2, T3, T4)>* clone()const
	{
		return new delegator(*this);
	}
};

template<typename T, typename T0, typename T1, typename T2, typename T3, typename T4>
class delegate<T (*)(T0, T1, T2, T3, T4)>
{
public:
	typedef T (*std_function)(T0, T1, T2, T3, T4);
	typedef T __cdecl (*cdecl_function)(T0, T1, T2, T3, T4);
	typedef T __stdcall (*stdcall_function)(T0, T1, T2, T3, T4);
	typedef T __fastcall (*fastcall_function)(T0, T1, T2, T3, T4);
	typedef delegate<std_function> this_type;
	delegator_base<std_function> *base;
	delegate():base(null){/*void*/};
	delegate(const this_type& d):base(d.base_clone()){/*void*/}
	~delegate()
	{
		this->destroy();
	}
	operator bool()const
	{
		return base;
	}
	T operator()(T0 p0, T1 p1, T2 p2, T3 p3, T4 p4)
	{
		return base->invoke(p0, p1, p2, p3, p4);
	}
	void destroy()
	{
		if(base)delete base, base = null;
	}
	delegator_base<std_function>* base_clone()const
	{
		if(base)return base->clone(); else return null;
	}
	const this_type& operator=(const this_type& d)
	{
		this->destroy();
		base = d.base_clone();
		return *this;
	}
	template<typename classT>
	void set_function(classT *c, T __cdecl (classT::*func)(T0, T1, T2, T3, T4))
	{
		this->destroy();
		base = new delegator<classT, cdecl_function>(c, func);
	}
	template<typename classT>
	void set_function(classT *c, T __stdcall (classT::*func)(T0, T1, T2, T3, T4))
	{
		this->destroy();
		base = new delegator<classT, stdcall_function>(c, func);
	}
	template<typename classT>
	void set_function(classT *c, T __fastcall (classT::*func)(T0, T1, T2, T3, T4))
	{
		this->destroy();
		base = new delegator<classT, fastcall_function>(c, func);
	}
	void set_function(cdecl_function func)
	{
		this->destroy();
		base = new delegator<void, cdecl_function>(func);
	}
	void set_function(stdcall_function func)
	{
		this->destroy();
		base = new delegator<void, stdcall_function>(func);
	}
	void set_function(fastcall_function func)
	{
		this->destroy();
		base = new delegator<void, fastcall_function>(func);
	}
};

//---------------------------------------------------------------------------
// T (*)(T0, T1, T2, T3, T4, T5)

template<typename T, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5>
class delegator_base<T (*)(T0, T1, T2, T3, T4, T5)>
{
public:
	virtual T invoke(T0, T1, T2, T3, T4, T5)=0;
	virtual delegator_base* clone()const=0;
};

template<typename classT, typename T, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5>
class delegator<classT, T __cdecl (*)(T0, T1, T2, T3, T4, T5)> : public delegator_base<T (*)(T0, T1, T2, T3, T4, T5)>
{
public:
	typedef T __cdecl (classT::*function_type)(T0, T1, T2, T3, T4, T5);
	classT *_C;
	function_type _F;
	delegator(classT *c, function_type func):_C(c), _F(func){/*void*/}
	T invoke(T0 p0, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5)
	{
		return (_C->*_F)(p0, p1, p2, p3, p4, p5);
	}
	delegator_base<T (*)(T0, T1, T2, T3, T4, T5)>* clone()const
	{
		return new delegator(*this);
	}
};

template<typename T, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5>
class delegator<void, T __cdecl (*)(T0, T1, T2, T3, T4, T5)> : public delegator_base<T (*)(T0, T1, T2, T3, T4, T5)>
{
public:
	typedef T __cdecl (*function_type)(T0, T1, T2, T3, T4, T5);
	function_type _F;
	delegator(function_type func):_F(func){/*void*/}
	T invoke(T0 p0, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5)
	{
		return (*_F)(p0, p1, p2, p3, p4, p5);
	}
	delegator_base<T (*)(T0, T1, T2, T3, T4, T5)>* clone()const
	{
		return new delegator(*this);
	}
};

template<typename classT, typename T, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5>
class delegator<classT, T __stdcall (*)(T0, T1, T2, T3, T4, T5)> : public delegator_base<T (*)(T0, T1, T2, T3, T4, T5)>
{
public:
	typedef T __stdcall (classT::*function_type)(T0, T1, T2, T3, T4, T5);
	classT *_C;
	function_type _F;
	delegator(classT *c, function_type func):_C(c), _F(func){/*void*/}
	T invoke(T0 p0, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5)
	{
		return (_C->*_F)(p0, p1, p2, p3, p4, p5);
	}
	delegator_base<T (*)(T0, T1, T2, T3, T4, T5)>* clone()const
	{
		return new delegator(*this);
	}
};

template<typename T, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5>
class delegator<void, T __stdcall (*)(T0, T1, T2, T3, T4, T5)> : public delegator_base<T (*)(T0, T1, T2, T3, T4, T5)>
{
public:
	typedef T __stdcall (*function_type)(T0, T1, T2, T3, T4, T5);
	function_type _F;
	delegator(function_type func):_F(func){/*void*/}
	T invoke(T0 p0, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5)
	{
		return (*_F)(p0, p1, p2, p3, p4, p5);
	}
	delegator_base<T (*)(T0, T1, T2, T3, T4, T5)>* clone()const
	{
		return new delegator(*this);
	}
};

template<typename classT, typename T, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5>
class delegator<classT, T __fastcall (*)(T0, T1, T2, T3, T4, T5)> : public delegator_base<T (*)(T0, T1, T2, T3, T4, T5)>
{
public:
	typedef T __fastcall (classT::*function_type)(T0, T1, T2, T3, T4, T5);
	classT *_C;
	function_type _F;
	delegator(classT *c, function_type func):_C(c), _F(func){/*void*/}
	T invoke(T0 p0, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5)
	{
		return (_C->*_F)(p0, p1, p2, p3, p4, p5);
	}
	delegator_base<T (*)(T0, T1, T2, T3, T4, T5)>* clone()const
	{
		return new delegator(*this);
	}
};

template<typename T, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5>
class delegator<void, T __fastcall (*)(T0, T1, T2, T3, T4, T5)> : public delegator_base<T (*)(T0, T1, T2, T3, T4, T5)>
{
public:
	typedef T __fastcall (*function_type)(T0, T1, T2, T3, T4, T5);
	function_type _F;
	delegator(function_type func):_F(func){/*void*/}
	T invoke(T0 p0, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5)
	{
		return (*_F)(p0, p1, p2, p3, p4, p5);
	}
	delegator_base<T (*)(T0, T1, T2, T3, T4, T5)>* clone()const
	{
		return new delegator(*this);
	}
};

template<typename T, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5>
class delegate<T (*)(T0, T1, T2, T3, T4, T5)>
{
public:
	typedef T (*std_function)(T0, T1, T2, T3, T4, T5);
	typedef T __cdecl (*cdecl_function)(T0, T1, T2, T3, T4, T5);
	typedef T __stdcall (*stdcall_function)(T0, T1, T2, T3, T4, T5);
	typedef T __fastcall (*fastcall_function)(T0, T1, T2, T3, T4, T5);
	typedef delegate<std_function> this_type;
	delegator_base<std_function> *base;
	delegate():base(null){/*void*/};
	delegate(const this_type& d):base(d.base_clone()){/*void*/}
	~delegate()
	{
		this->destroy();
	}
	operator bool()const
	{
		return base;
	}
	T operator()(T0 p0, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5)
	{
		return base->invoke(p0, p1, p2, p3, p4, p5);
	}
	void destroy()
	{
		if(base)delete base, base = null;
	}
	delegator_base<std_function>* base_clone()const
	{
		if(base)return base->clone(); else return null;
	}
	const this_type& operator=(const this_type& d)
	{
		this->destroy();
		base = d.base_clone();
		return *this;
	}
	template<typename classT>
	void set_function(classT *c, T __cdecl (classT::*func)(T0, T1, T2, T3, T4, T5))
	{
		this->destroy();
		base = new delegator<classT, cdecl_function>(c, func);
	}
	template<typename classT>
	void set_function(classT *c, T __stdcall (classT::*func)(T0, T1, T2, T3, T4, T5))
	{
		this->destroy();
		base = new delegator<classT, stdcall_function>(c, func);
	}
	template<typename classT>
	void set_function(classT *c, T __fastcall (classT::*func)(T0, T1, T2, T3, T4, T5))
	{
		this->destroy();
		base = new delegator<classT, fastcall_function>(c, func);
	}
	void set_function(cdecl_function func)
	{
		this->destroy();
		base = new delegator<void, cdecl_function>(func);
	}
	void set_function(stdcall_function func)
	{
		this->destroy();
		base = new delegator<void, stdcall_function>(func);
	}
	void set_function(fastcall_function func)
	{
		this->destroy();
		base = new delegator<void, fastcall_function>(func);
	}
};

//---------------------------------------------------------------------------
// T (*)(T0, T1, T2, T3, T4, T5, T6)

template<typename T, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
class delegator_base<T (*)(T0, T1, T2, T3, T4, T5, T6)>
{
public:
	virtual T invoke(T0, T1, T2, T3, T4, T5, T6)=0;
	virtual delegator_base* clone()const=0;
};

template<typename classT, typename T, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
class delegator<classT, T __cdecl (*)(T0, T1, T2, T3, T4, T5, T6)> : public delegator_base<T (*)(T0, T1, T2, T3, T4, T5, T6)>
{
public:
	typedef T __cdecl (classT::*function_type)(T0, T1, T2, T3, T4, T5, T6);
	classT *_C;
	function_type _F;
	delegator(classT *c, function_type func):_C(c), _F(func){/*void*/}
	T invoke(T0 p0, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6)
	{
		return (_C->*_F)(p0, p1, p2, p3, p4, p5, p6);
	}
	delegator_base<T (*)(T0, T1, T2, T3, T4, T5, T6)>* clone()const
	{
		return new delegator(*this);
	}
};

template<typename T, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
class delegator<void, T __cdecl (*)(T0, T1, T2, T3, T4, T5, T6)> : public delegator_base<T (*)(T0, T1, T2, T3, T4, T5, T6)>
{
public:
	typedef T __cdecl (*function_type)(T0, T1, T2, T3, T4, T5, T6);
	function_type _F;
	delegator(function_type func):_F(func){/*void*/}
	T invoke(T0 p0, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6)
	{
		return (*_F)(p0, p1, p2, p3, p4, p5, p6);
	}
	delegator_base<T (*)(T0, T1, T2, T3, T4, T5, T6)>* clone()const
	{
		return new delegator(*this);
	}
};

template<typename classT, typename T, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
class delegator<classT, T __stdcall (*)(T0, T1, T2, T3, T4, T5, T6)> : public delegator_base<T (*)(T0, T1, T2, T3, T4, T5, T6)>
{
public:
	typedef T __stdcall (classT::*function_type)(T0, T1, T2, T3, T4, T5, T6);
	classT *_C;
	function_type _F;
	delegator(classT *c, function_type func):_C(c), _F(func){/*void*/}
	T invoke(T0 p0, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6)
	{
		return (_C->*_F)(p0, p1, p2, p3, p4, p5, p6);
	}
	delegator_base<T (*)(T0, T1, T2, T3, T4, T5, T6)>* clone()const
	{
		return new delegator(*this);
	}
};

template<typename T, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
class delegator<void, T __stdcall (*)(T0, T1, T2, T3, T4, T5, T6)> : public delegator_base<T (*)(T0, T1, T2, T3, T4, T5, T6)>
{
public:
	typedef T __stdcall (*function_type)(T0, T1, T2, T3, T4, T5, T6);
	function_type _F;
	delegator(function_type func):_F(func){/*void*/}
	T invoke(T0 p0, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6)
	{
		return (*_F)(p0, p1, p2, p3, p4, p5, p6);
	}
	delegator_base<T (*)(T0, T1, T2, T3, T4, T5, T6)>* clone()const
	{
		return new delegator(*this);
	}
};

template<typename classT, typename T, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
class delegator<classT, T __fastcall (*)(T0, T1, T2, T3, T4, T5, T6)> : public delegator_base<T (*)(T0, T1, T2, T3, T4, T5, T6)>
{
public:
	typedef T __fastcall (classT::*function_type)(T0, T1, T2, T3, T4, T5, T6);
	classT *_C;
	function_type _F;
	delegator(classT *c, function_type func):_C(c), _F(func){/*void*/}
	T invoke(T0 p0, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6)
	{
		return (_C->*_F)(p0, p1, p2, p3, p4, p5, p6);
	}
	delegator_base<T (*)(T0, T1, T2, T3, T4, T5, T6)>* clone()const
	{
		return new delegator(*this);
	}
};

template<typename T, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
class delegator<void, T __fastcall (*)(T0, T1, T2, T3, T4, T5, T6)> : public delegator_base<T (*)(T0, T1, T2, T3, T4, T5, T6)>
{
public:
	typedef T __fastcall (*function_type)(T0, T1, T2, T3, T4, T5, T6);
	function_type _F;
	delegator(function_type func):_F(func){/*void*/}
	T invoke(T0 p0, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6)
	{
		return (*_F)(p0, p1, p2, p3, p4, p5, p6);
	}
	delegator_base<T (*)(T0, T1, T2, T3, T4, T5, T6)>* clone()const
	{
		return new delegator(*this);
	}
};

template<typename T, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
class delegate<T (*)(T0, T1, T2, T3, T4, T5, T6)>
{
public:
	typedef T (*std_function)(T0, T1, T2, T3, T4, T5, T6);
	typedef T __cdecl (*cdecl_function)(T0, T1, T2, T3, T4, T5, T6);
	typedef T __stdcall (*stdcall_function)(T0, T1, T2, T3, T4, T5, T6);
	typedef T __fastcall (*fastcall_function)(T0, T1, T2, T3, T4, T5, T6);
	typedef delegate<std_function> this_type;
	delegator_base<std_function> *base;
	delegate():base(null){/*void*/};
	delegate(const this_type& d):base(d.base_clone()){/*void*/}
	~delegate()
	{
		this->destroy();
	}
	operator bool()const
	{
		return base;
	}
	T operator()(T0 p0, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6)
	{
		return base->invoke(p0, p1, p2, p3, p4, p5, p6);
	}
	void destroy()
	{
		if(base)delete base, base = null;
	}
	delegator_base<std_function>* base_clone()const
	{
		if(base)return base->clone(); else return null;
	}
	const this_type& operator=(const this_type& d)
	{
		this->destroy();
		base = d.base_clone();
		return *this;
	}
	template<typename classT>
	void set_function(classT *c, T __cdecl (classT::*func)(T0, T1, T2, T3, T4, T5, T6))
	{
		this->destroy();
		base = new delegator<classT, cdecl_function>(c, func);
	}
	template<typename classT>
	void set_function(classT *c, T __stdcall (classT::*func)(T0, T1, T2, T3, T4, T5, T6))
	{
		this->destroy();
		base = new delegator<classT, stdcall_function>(c, func);
	}
	template<typename classT>
	void set_function(classT *c, T __fastcall (classT::*func)(T0, T1, T2, T3, T4, T5, T6))
	{
		this->destroy();
		base = new delegator<classT, fastcall_function>(c, func);
	}
	void set_function(cdecl_function func)
	{
		this->destroy();
		base = new delegator<void, cdecl_function>(func);
	}
	void set_function(stdcall_function func)
	{
		this->destroy();
		base = new delegator<void, stdcall_function>(func);
	}
	void set_function(fastcall_function func)
	{
		this->destroy();
		base = new delegator<void, fastcall_function>(func);
	}
};

//---------------------------------------------------------------------------
// T (*)(T0, T1, T2, T3, T4, T5, T6, T7)

template<typename T, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7>
class delegator_base<T (*)(T0, T1, T2, T3, T4, T5, T6, T7)>
{
public:
	virtual T invoke(T0, T1, T2, T3, T4, T5, T6, T7)=0;
	virtual delegator_base* clone()const=0;
};

template<typename classT, typename T, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7>
class delegator<classT, T __cdecl (*)(T0, T1, T2, T3, T4, T5, T6, T7)> : public delegator_base<T (*)(T0, T1, T2, T3, T4, T5, T6, T7)>
{
public:
	typedef T __cdecl (classT::*function_type)(T0, T1, T2, T3, T4, T5, T6, T7);
	classT *_C;
	function_type _F;
	delegator(classT *c, function_type func):_C(c), _F(func){/*void*/}
	T invoke(T0 p0, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7)
	{
		return (_C->*_F)(p0, p1, p2, p3, p4, p5, p6, p7);
	}
	delegator_base<T (*)(T0, T1, T2, T3, T4, T5, T6, T7)>* clone()const
	{
		return new delegator(*this);
	}
};

template<typename T, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7>
class delegator<void, T __cdecl (*)(T0, T1, T2, T3, T4, T5, T6, T7)> : public delegator_base<T (*)(T0, T1, T2, T3, T4, T5, T6, T7)>
{
public:
	typedef T __cdecl (*function_type)(T0, T1, T2, T3, T4, T5, T6, T7);
	function_type _F;
	delegator(function_type func):_F(func){/*void*/}
	T invoke(T0 p0, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7)
	{
		return (*_F)(p0, p1, p2, p3, p4, p5, p6, p7);
	}
	delegator_base<T (*)(T0, T1, T2, T3, T4, T5, T6, T7)>* clone()const
	{
		return new delegator(*this);
	}
};

template<typename classT, typename T, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7>
class delegator<classT, T __stdcall (*)(T0, T1, T2, T3, T4, T5, T6, T7)> : public delegator_base<T (*)(T0, T1, T2, T3, T4, T5, T6, T7)>
{
public:
	typedef T __stdcall (classT::*function_type)(T0, T1, T2, T3, T4, T5, T6, T7);
	classT *_C;
	function_type _F;
	delegator(classT *c, function_type func):_C(c), _F(func){/*void*/}
	T invoke(T0 p0, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7)
	{
		return (_C->*_F)(p0, p1, p2, p3, p4, p5, p6, p7);
	}
	delegator_base<T (*)(T0, T1, T2, T3, T4, T5, T6, T7)>* clone()const
	{
		return new delegator(*this);
	}
};

template<typename T, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7>
class delegator<void, T __stdcall (*)(T0, T1, T2, T3, T4, T5, T6, T7)> : public delegator_base<T (*)(T0, T1, T2, T3, T4, T5, T6, T7)>
{
public:
	typedef T __stdcall (*function_type)(T0, T1, T2, T3, T4, T5, T6, T7);
	function_type _F;
	delegator(function_type func):_F(func){/*void*/}
	T invoke(T0 p0, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7)
	{
		return (*_F)(p0, p1, p2, p3, p4, p5, p6, p7);
	}
	delegator_base<T (*)(T0, T1, T2, T3, T4, T5, T6, T7)>* clone()const
	{
		return new delegator(*this);
	}
};

template<typename classT, typename T, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7>
class delegator<classT, T __fastcall (*)(T0, T1, T2, T3, T4, T5, T6, T7)> : public delegator_base<T (*)(T0, T1, T2, T3, T4, T5, T6, T7)>
{
public:
	typedef T __fastcall (classT::*function_type)(T0, T1, T2, T3, T4, T5, T6, T7);
	classT *_C;
	function_type _F;
	delegator(classT *c, function_type func):_C(c), _F(func){/*void*/}
	T invoke(T0 p0, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7)
	{
		return (_C->*_F)(p0, p1, p2, p3, p4, p5, p6, p7);
	}
	delegator_base<T (*)(T0, T1, T2, T3, T4, T5, T6, T7)>* clone()const
	{
		return new delegator(*this);
	}
};

template<typename T, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7>
class delegator<void, T __fastcall (*)(T0, T1, T2, T3, T4, T5, T6, T7)> : public delegator_base<T (*)(T0, T1, T2, T3, T4, T5, T6, T7)>
{
public:
	typedef T __fastcall (*function_type)(T0, T1, T2, T3, T4, T5, T6, T7);
	function_type _F;
	delegator(function_type func):_F(func){/*void*/}
	T invoke(T0 p0, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7)
	{
		return (*_F)(p0, p1, p2, p3, p4, p5, p6, p7);
	}
	delegator_base<T (*)(T0, T1, T2, T3, T4, T5, T6, T7)>* clone()const
	{
		return new delegator(*this);
	}
};

template<typename T, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7>
class delegate<T (*)(T0, T1, T2, T3, T4, T5, T6, T7)>
{
public:
	typedef T (*std_function)(T0, T1, T2, T3, T4, T5, T6, T7);
	typedef T __cdecl (*cdecl_function)(T0, T1, T2, T3, T4, T5, T6, T7);
	typedef T __stdcall (*stdcall_function)(T0, T1, T2, T3, T4, T5, T6, T7);
	typedef T __fastcall (*fastcall_function)(T0, T1, T2, T3, T4, T5, T6, T7);
	typedef delegate<std_function> this_type;
	delegator_base<std_function> *base;
	delegate():base(null){/*void*/};
	delegate(const this_type& d):base(d.base_clone()){/*void*/}
	~delegate()
	{
		this->destroy();
	}
	operator bool()const
	{
		return base;
	}
	T operator()(T0 p0, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7)
	{
		return base->invoke(p0, p1, p2, p3, p4, p5, p6, p7);
	}
	void destroy()
	{
		if(base)delete base, base = null;
	}
	delegator_base<std_function>* base_clone()const
	{
		if(base)return base->clone(); else return null;
	}
	const this_type& operator=(const this_type& d)
	{
		this->destroy();
		base = d.base_clone();
		return *this;
	}
	template<typename classT>
	void set_function(classT *c, T __cdecl (classT::*func)(T0, T1, T2, T3, T4, T5, T6, T7))
	{
		this->destroy();
		base = new delegator<classT, cdecl_function>(c, func);
	}
	template<typename classT>
	void set_function(classT *c, T __stdcall (classT::*func)(T0, T1, T2, T3, T4, T5, T6, T7))
	{
		this->destroy();
		base = new delegator<classT, stdcall_function>(c, func);
	}
	template<typename classT>
	void set_function(classT *c, T __fastcall (classT::*func)(T0, T1, T2, T3, T4, T5, T6, T7))
	{
		this->destroy();
		base = new delegator<classT, fastcall_function>(c, func);
	}
	void set_function(cdecl_function func)
	{
		this->destroy();
		base = new delegator<void, cdecl_function>(func);
	}
	void set_function(stdcall_function func)
	{
		this->destroy();
		base = new delegator<void, stdcall_function>(func);
	}
	void set_function(fastcall_function func)
	{
		this->destroy();
		base = new delegator<void, fastcall_function>(func);
	}
};

//---------------------------------------------------------------------------
// T (*)(T0, T1, T2, T3, T4, T5, T6, T7, T8)

template<typename T, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8>
class delegator_base<T (*)(T0, T1, T2, T3, T4, T5, T6, T7, T8)>
{
public:
	virtual T invoke(T0, T1, T2, T3, T4, T5, T6, T7, T8)=0;
	virtual delegator_base* clone()const=0;
};

template<typename classT, typename T, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8>
class delegator<classT, T __cdecl (*)(T0, T1, T2, T3, T4, T5, T6, T7, T8)> : public delegator_base<T (*)(T0, T1, T2, T3, T4, T5, T6, T7, T8)>
{
public:
	typedef T __cdecl (classT::*function_type)(T0, T1, T2, T3, T4, T5, T6, T7, T8);
	classT *_C;
	function_type _F;
	delegator(classT *c, function_type func):_C(c), _F(func){/*void*/}
	T invoke(T0 p0, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8)
	{
		return (_C->*_F)(p0, p1, p2, p3, p4, p5, p6, p7, p8);
	}
	delegator_base<T (*)(T0, T1, T2, T3, T4, T5, T6, T7, T8)>* clone()const
	{
		return new delegator(*this);
	}
};

template<typename T, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8>
class delegator<void, T __cdecl (*)(T0, T1, T2, T3, T4, T5, T6, T7, T8)> : public delegator_base<T (*)(T0, T1, T2, T3, T4, T5, T6, T7, T8)>
{
public:
	typedef T __cdecl (*function_type)(T0, T1, T2, T3, T4, T5, T6, T7, T8);
	function_type _F;
	delegator(function_type func):_F(func){/*void*/}
	T invoke(T0 p0, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8)
	{
		return (*_F)(p0, p1, p2, p3, p4, p5, p6, p7, p8);
	}
	delegator_base<T (*)(T0, T1, T2, T3, T4, T5, T6, T7, T8)>* clone()const
	{
		return new delegator(*this);
	}
};

template<typename classT, typename T, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8>
class delegator<classT, T __stdcall (*)(T0, T1, T2, T3, T4, T5, T6, T7, T8)> : public delegator_base<T (*)(T0, T1, T2, T3, T4, T5, T6, T7, T8)>
{
public:
	typedef T __stdcall (classT::*function_type)(T0, T1, T2, T3, T4, T5, T6, T7, T8);
	classT *_C;
	function_type _F;
	delegator(classT *c, function_type func):_C(c), _F(func){/*void*/}
	T invoke(T0 p0, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8)
	{
		return (_C->*_F)(p0, p1, p2, p3, p4, p5, p6, p7, p8);
	}
	delegator_base<T (*)(T0, T1, T2, T3, T4, T5, T6, T7, T8)>* clone()const
	{
		return new delegator(*this);
	}
};

template<typename T, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8>
class delegator<void, T __stdcall (*)(T0, T1, T2, T3, T4, T5, T6, T7, T8)> : public delegator_base<T (*)(T0, T1, T2, T3, T4, T5, T6, T7, T8)>
{
public:
	typedef T __stdcall (*function_type)(T0, T1, T2, T3, T4, T5, T6, T7, T8);
	function_type _F;
	delegator(function_type func):_F(func){/*void*/}
	T invoke(T0 p0, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8)
	{
		return (*_F)(p0, p1, p2, p3, p4, p5, p6, p7, p8);
	}
	delegator_base<T (*)(T0, T1, T2, T3, T4, T5, T6, T7, T8)>* clone()const
	{
		return new delegator(*this);
	}
};

template<typename classT, typename T, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8>
class delegator<classT, T __fastcall (*)(T0, T1, T2, T3, T4, T5, T6, T7, T8)> : public delegator_base<T (*)(T0, T1, T2, T3, T4, T5, T6, T7, T8)>
{
public:
	typedef T __fastcall (classT::*function_type)(T0, T1, T2, T3, T4, T5, T6, T7, T8);
	classT *_C;
	function_type _F;
	delegator(classT *c, function_type func):_C(c), _F(func){/*void*/}
	T invoke(T0 p0, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8)
	{
		return (_C->*_F)(p0, p1, p2, p3, p4, p5, p6, p7, p8);
	}
	delegator_base<T (*)(T0, T1, T2, T3, T4, T5, T6, T7, T8)>* clone()const
	{
		return new delegator(*this);
	}
};

template<typename T, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8>
class delegator<void, T __fastcall (*)(T0, T1, T2, T3, T4, T5, T6, T7, T8)> : public delegator_base<T (*)(T0, T1, T2, T3, T4, T5, T6, T7, T8)>
{
public:
	typedef T __fastcall (*function_type)(T0, T1, T2, T3, T4, T5, T6, T7, T8);
	function_type _F;
	delegator(function_type func):_F(func){/*void*/}
	T invoke(T0 p0, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8)
	{
		return (*_F)(p0, p1, p2, p3, p4, p5, p6, p7, p8);
	}
	delegator_base<T (*)(T0, T1, T2, T3, T4, T5, T6, T7, T8)>* clone()const
	{
		return new delegator(*this);
	}
};

template<typename T, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8>
class delegate<T (*)(T0, T1, T2, T3, T4, T5, T6, T7, T8)>
{
public:
	typedef T (*std_function)(T0, T1, T2, T3, T4, T5, T6, T7, T8);
	typedef T __cdecl (*cdecl_function)(T0, T1, T2, T3, T4, T5, T6, T7, T8);
	typedef T __stdcall (*stdcall_function)(T0, T1, T2, T3, T4, T5, T6, T7, T8);
	typedef T __fastcall (*fastcall_function)(T0, T1, T2, T3, T4, T5, T6, T7, T8);
	typedef delegate<std_function> this_type;
	delegator_base<std_function> *base;
	delegate():base(null){/*void*/};
	delegate(const this_type& d):base(d.base_clone()){/*void*/}
	~delegate()
	{
		this->destroy();
	}
	operator bool()const
	{
		return base;
	}
	T operator()(T0 p0, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8)
	{
		return base->invoke(p0, p1, p2, p3, p4, p5, p6, p7, p8);
	}
	void destroy()
	{
		if(base)delete base, base = null;
	}
	delegator_base<std_function>* base_clone()const
	{
		if(base)return base->clone(); else return null;
	}
	const this_type& operator=(const this_type& d)
	{
		this->destroy();
		base = d.base_clone();
		return *this;
	}
	template<typename classT>
	void set_function(classT *c, T __cdecl (classT::*func)(T0, T1, T2, T3, T4, T5, T6, T7, T8))
	{
		this->destroy();
		base = new delegator<classT, cdecl_function>(c, func);
	}
	template<typename classT>
	void set_function(classT *c, T __stdcall (classT::*func)(T0, T1, T2, T3, T4, T5, T6, T7, T8))
	{
		this->destroy();
		base = new delegator<classT, stdcall_function>(c, func);
	}
	template<typename classT>
	void set_function(classT *c, T __fastcall (classT::*func)(T0, T1, T2, T3, T4, T5, T6, T7, T8))
	{
		this->destroy();
		base = new delegator<classT, fastcall_function>(c, func);
	}
	void set_function(cdecl_function func)
	{
		this->destroy();
		base = new delegator<void, cdecl_function>(func);
	}
	void set_function(stdcall_function func)
	{
		this->destroy();
		base = new delegator<void, stdcall_function>(func);
	}
	void set_function(fastcall_function func)
	{
		this->destroy();
		base = new delegator<void, fastcall_function>(func);
	}
};

//---------------------------------------------------------------------------
// T (*)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9)

template<typename T, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9>
class delegator_base<T (*)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9)>
{
public:
	virtual T invoke(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9)=0;
	virtual delegator_base* clone()const=0;
};

template<typename classT, typename T, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9>
class delegator<classT, T __cdecl (*)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9)> : public delegator_base<T (*)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9)>
{
public:
	typedef T __cdecl (classT::*function_type)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9);
	classT *_C;
	function_type _F;
	delegator(classT *c, function_type func):_C(c), _F(func){/*void*/}
	T invoke(T0 p0, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8, T9 p9)
	{
		return (_C->*_F)(p0, p1, p2, p3, p4, p5, p6, p7, p8, p9);
	}
	delegator_base<T (*)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9)>* clone()const
	{
		return new delegator(*this);
	}
};

template<typename T, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9>
class delegator<void, T __cdecl (*)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9)> : public delegator_base<T (*)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9)>
{
public:
	typedef T __cdecl (*function_type)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9);
	function_type _F;
	delegator(function_type func):_F(func){/*void*/}
	T invoke(T0 p0, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8, T9 p9)
	{
		return (*_F)(p0, p1, p2, p3, p4, p5, p6, p7, p8, p9);
	}
	delegator_base<T (*)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9)>* clone()const
	{
		return new delegator(*this);
	}
};

template<typename classT, typename T, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9>
class delegator<classT, T __stdcall (*)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9)> : public delegator_base<T (*)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9)>
{
public:
	typedef T __stdcall (classT::*function_type)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9);
	classT *_C;
	function_type _F;
	delegator(classT *c, function_type func):_C(c), _F(func){/*void*/}
	T invoke(T0 p0, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8, T9 p9)
	{
		return (_C->*_F)(p0, p1, p2, p3, p4, p5, p6, p7, p8, p9);
	}
	delegator_base<T (*)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9)>* clone()const
	{
		return new delegator(*this);
	}
};

template<typename T, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9>
class delegator<void, T __stdcall (*)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9)> : public delegator_base<T (*)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9)>
{
public:
	typedef T __stdcall (*function_type)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9);
	function_type _F;
	delegator(function_type func):_F(func){/*void*/}
	T invoke(T0 p0, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8, T9 p9)
	{
		return (*_F)(p0, p1, p2, p3, p4, p5, p6, p7, p8, p9);
	}
	delegator_base<T (*)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9)>* clone()const
	{
		return new delegator(*this);
	}
};

template<typename classT, typename T, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9>
class delegator<classT, T __fastcall (*)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9)> : public delegator_base<T (*)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9)>
{
public:
	typedef T __fastcall (classT::*function_type)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9);
	classT *_C;
	function_type _F;
	delegator(classT *c, function_type func):_C(c), _F(func){/*void*/}
	T invoke(T0 p0, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8, T9 p9)
	{
		return (_C->*_F)(p0, p1, p2, p3, p4, p5, p6, p7, p8, p9);
	}
	delegator_base<T (*)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9)>* clone()const
	{
		return new delegator(*this);
	}
};

template<typename T, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9>
class delegator<void, T __fastcall (*)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9)> : public delegator_base<T (*)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9)>
{
public:
	typedef T __fastcall (*function_type)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9);
	function_type _F;
	delegator(function_type func):_F(func){/*void*/}
	T invoke(T0 p0, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8, T9 p9)
	{
		return (*_F)(p0, p1, p2, p3, p4, p5, p6, p7, p8, p9);
	}
	delegator_base<T (*)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9)>* clone()const
	{
		return new delegator(*this);
	}
};

template<typename T, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9>
class delegate<T (*)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9)>
{
public:
	typedef T (*std_function)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9);
	typedef T __cdecl (*cdecl_function)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9);
	typedef T __stdcall (*stdcall_function)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9);
	typedef T __fastcall (*fastcall_function)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9);
	typedef delegate<std_function> this_type;
	delegator_base<std_function> *base;
	delegate():base(null){/*void*/};
	delegate(const this_type& d):base(d.base_clone()){/*void*/}
	~delegate()
	{
		this->destroy();
	}
	operator bool()const
	{
		return base;
	}
	T operator()(T0 p0, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8, T9 p9)
	{
		return base->invoke(p0, p1, p2, p3, p4, p5, p6, p7, p8, p9);
	}
	void destroy()
	{
		if(base)delete base, base = null;
	}
	delegator_base<std_function>* base_clone()const
	{
		if(base)return base->clone(); else return null;
	}
	const this_type& operator=(const this_type& d)
	{
		this->destroy();
		base = d.base_clone();
		return *this;
	}
	template<typename classT>
	void set_function(classT *c, T __cdecl (classT::*func)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9))
	{
		this->destroy();
		base = new delegator<classT, cdecl_function>(c, func);
	}
	template<typename classT>
	void set_function(classT *c, T __stdcall (classT::*func)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9))
	{
		this->destroy();
		base = new delegator<classT, stdcall_function>(c, func);
	}
	template<typename classT>
	void set_function(classT *c, T __fastcall (classT::*func)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9))
	{
		this->destroy();
		base = new delegator<classT, fastcall_function>(c, func);
	}
	void set_function(cdecl_function func)
	{
		this->destroy();
		base = new delegator<void, cdecl_function>(func);
	}
	void set_function(stdcall_function func)
	{
		this->destroy();
		base = new delegator<void, stdcall_function>(func);
	}
	void set_function(fastcall_function func)
	{
		this->destroy();
		base = new delegator<void, fastcall_function>(func);
	}
};

//---------------------------------------------------------------------------
// T (*)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10)

template<typename T, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10>
class delegator_base<T (*)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10)>
{
public:
	virtual T invoke(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10)=0;
	virtual delegator_base* clone()const=0;
};

template<typename classT, typename T, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10>
class delegator<classT, T __cdecl (*)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10)> : public delegator_base<T (*)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10)>
{
public:
	typedef T __cdecl (classT::*function_type)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10);
	classT *_C;
	function_type _F;
	delegator(classT *c, function_type func):_C(c), _F(func){/*void*/}
	T invoke(T0 p0, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8, T9 p9, T10 p10)
	{
		return (_C->*_F)(p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10);
	}
	delegator_base<T (*)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10)>* clone()const
	{
		return new delegator(*this);
	}
};

template<typename T, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10>
class delegator<void, T __cdecl (*)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10)> : public delegator_base<T (*)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10)>
{
public:
	typedef T __cdecl (*function_type)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10);
	function_type _F;
	delegator(function_type func):_F(func){/*void*/}
	T invoke(T0 p0, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8, T9 p9, T10 p10)
	{
		return (*_F)(p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10);
	}
	delegator_base<T (*)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10)>* clone()const
	{
		return new delegator(*this);
	}
};

template<typename classT, typename T, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10>
class delegator<classT, T __stdcall (*)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10)> : public delegator_base<T (*)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10)>
{
public:
	typedef T __stdcall (classT::*function_type)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10);
	classT *_C;
	function_type _F;
	delegator(classT *c, function_type func):_C(c), _F(func){/*void*/}
	T invoke(T0 p0, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8, T9 p9, T10 p10)
	{
		return (_C->*_F)(p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10);
	}
	delegator_base<T (*)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10)>* clone()const
	{
		return new delegator(*this);
	}
};

template<typename T, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10>
class delegator<void, T __stdcall (*)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10)> : public delegator_base<T (*)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10)>
{
public:
	typedef T __stdcall (*function_type)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10);
	function_type _F;
	delegator(function_type func):_F(func){/*void*/}
	T invoke(T0 p0, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8, T9 p9, T10 p10)
	{
		return (*_F)(p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10);
	}
	delegator_base<T (*)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10)>* clone()const
	{
		return new delegator(*this);
	}
};

template<typename classT, typename T, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10>
class delegator<classT, T __fastcall (*)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10)> : public delegator_base<T (*)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10)>
{
public:
	typedef T __fastcall (classT::*function_type)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10);
	classT *_C;
	function_type _F;
	delegator(classT *c, function_type func):_C(c), _F(func){/*void*/}
	T invoke(T0 p0, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8, T9 p9, T10 p10)
	{
		return (_C->*_F)(p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10);
	}
	delegator_base<T (*)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10)>* clone()const
	{
		return new delegator(*this);
	}
};

template<typename T, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10>
class delegator<void, T __fastcall (*)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10)> : public delegator_base<T (*)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10)>
{
public:
	typedef T __fastcall (*function_type)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10);
	function_type _F;
	delegator(function_type func):_F(func){/*void*/}
	T invoke(T0 p0, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8, T9 p9, T10 p10)
	{
		return (*_F)(p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10);
	}
	delegator_base<T (*)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10)>* clone()const
	{
		return new delegator(*this);
	}
};

template<typename T, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10>
class delegate<T (*)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10)>
{
public:
	typedef T (*std_function)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10);
	typedef T __cdecl (*cdecl_function)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10);
	typedef T __stdcall (*stdcall_function)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10);
	typedef T __fastcall (*fastcall_function)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10);
	typedef delegate<std_function> this_type;
	delegator_base<std_function> *base;
	delegate():base(null){/*void*/};
	delegate(const this_type& d):base(d.base_clone()){/*void*/}
	~delegate()
	{
		this->destroy();
	}
	operator bool()const
	{
		return base;
	}
	T operator()(T0 p0, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8, T9 p9, T10 p10)
	{
		return base->invoke(p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10);
	}
	void destroy()
	{
		if(base)delete base, base = null;
	}
	delegator_base<std_function>* base_clone()const
	{
		if(base)return base->clone(); else return null;
	}
	const this_type& operator=(const this_type& d)
	{
		this->destroy();
		base = d.base_clone();
		return *this;
	}
	template<typename classT>
	void set_function(classT *c, T __cdecl (classT::*func)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10))
	{
		this->destroy();
		base = new delegator<classT, cdecl_function>(c, func);
	}
	template<typename classT>
	void set_function(classT *c, T __stdcall (classT::*func)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10))
	{
		this->destroy();
		base = new delegator<classT, stdcall_function>(c, func);
	}
	template<typename classT>
	void set_function(classT *c, T __fastcall (classT::*func)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10))
	{
		this->destroy();
		base = new delegator<classT, fastcall_function>(c, func);
	}
	void set_function(cdecl_function func)
	{
		this->destroy();
		base = new delegator<void, cdecl_function>(func);
	}
	void set_function(stdcall_function func)
	{
		this->destroy();
		base = new delegator<void, stdcall_function>(func);
	}
	void set_function(fastcall_function func)
	{
		this->destroy();
		base = new delegator<void, fastcall_function>(func);
	}
};

//---------------------------------------------------------------------------
// T (*)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11)

template<typename T, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11>
class delegator_base<T (*)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11)>
{
public:
	virtual T invoke(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11)=0;
	virtual delegator_base* clone()const=0;
};

template<typename classT, typename T, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11>
class delegator<classT, T __cdecl (*)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11)> : public delegator_base<T (*)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11)>
{
public:
	typedef T __cdecl (classT::*function_type)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11);
	classT *_C;
	function_type _F;
	delegator(classT *c, function_type func):_C(c), _F(func){/*void*/}
	T invoke(T0 p0, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8, T9 p9, T10 p10, T11 p11)
	{
		return (_C->*_F)(p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11);
	}
	delegator_base<T (*)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11)>* clone()const
	{
		return new delegator(*this);
	}
};

template<typename T, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11>
class delegator<void, T __cdecl (*)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11)> : public delegator_base<T (*)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11)>
{
public:
	typedef T __cdecl (*function_type)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11);
	function_type _F;
	delegator(function_type func):_F(func){/*void*/}
	T invoke(T0 p0, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8, T9 p9, T10 p10, T11 p11)
	{
		return (*_F)(p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11);
	}
	delegator_base<T (*)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11)>* clone()const
	{
		return new delegator(*this);
	}
};

template<typename classT, typename T, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11>
class delegator<classT, T __stdcall (*)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11)> : public delegator_base<T (*)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11)>
{
public:
	typedef T __stdcall (classT::*function_type)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11);
	classT *_C;
	function_type _F;
	delegator(classT *c, function_type func):_C(c), _F(func){/*void*/}
	T invoke(T0 p0, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8, T9 p9, T10 p10, T11 p11)
	{
		return (_C->*_F)(p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11);
	}
	delegator_base<T (*)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11)>* clone()const
	{
		return new delegator(*this);
	}
};

template<typename T, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11>
class delegator<void, T __stdcall (*)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11)> : public delegator_base<T (*)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11)>
{
public:
	typedef T __stdcall (*function_type)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11);
	function_type _F;
	delegator(function_type func):_F(func){/*void*/}
	T invoke(T0 p0, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8, T9 p9, T10 p10, T11 p11)
	{
		return (*_F)(p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11);
	}
	delegator_base<T (*)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11)>* clone()const
	{
		return new delegator(*this);
	}
};

template<typename classT, typename T, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11>
class delegator<classT, T __fastcall (*)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11)> : public delegator_base<T (*)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11)>
{
public:
	typedef T __fastcall (classT::*function_type)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11);
	classT *_C;
	function_type _F;
	delegator(classT *c, function_type func):_C(c), _F(func){/*void*/}
	T invoke(T0 p0, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8, T9 p9, T10 p10, T11 p11)
	{
		return (_C->*_F)(p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11);
	}
	delegator_base<T (*)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11)>* clone()const
	{
		return new delegator(*this);
	}
};

template<typename T, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11>
class delegator<void, T __fastcall (*)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11)> : public delegator_base<T (*)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11)>
{
public:
	typedef T __fastcall (*function_type)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11);
	function_type _F;
	delegator(function_type func):_F(func){/*void*/}
	T invoke(T0 p0, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8, T9 p9, T10 p10, T11 p11)
	{
		return (*_F)(p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11);
	}
	delegator_base<T (*)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11)>* clone()const
	{
		return new delegator(*this);
	}
};

template<typename T, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11>
class delegate<T (*)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11)>
{
public:
	typedef T (*std_function)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11);
	typedef T __cdecl (*cdecl_function)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11);
	typedef T __stdcall (*stdcall_function)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11);
	typedef T __fastcall (*fastcall_function)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11);
	typedef delegate<std_function> this_type;
	delegator_base<std_function> *base;
	delegate():base(null){/*void*/};
	delegate(const this_type& d):base(d.base_clone()){/*void*/}
	~delegate()
	{
		this->destroy();
	}
	operator bool()const
	{
		return base;
	}
	T operator()(T0 p0, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8, T9 p9, T10 p10, T11 p11)
	{
		return base->invoke(p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11);
	}
	void destroy()
	{
		if(base)delete base, base = null;
	}
	delegator_base<std_function>* base_clone()const
	{
		if(base)return base->clone(); else return null;
	}
	const this_type& operator=(const this_type& d)
	{
		this->destroy();
		base = d.base_clone();
		return *this;
	}
	template<typename classT>
	void set_function(classT *c, T __cdecl (classT::*func)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11))
	{
		this->destroy();
		base = new delegator<classT, cdecl_function>(c, func);
	}
	template<typename classT>
	void set_function(classT *c, T __stdcall (classT::*func)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11))
	{
		this->destroy();
		base = new delegator<classT, stdcall_function>(c, func);
	}
	template<typename classT>
	void set_function(classT *c, T __fastcall (classT::*func)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11))
	{
		this->destroy();
		base = new delegator<classT, fastcall_function>(c, func);
	}
	void set_function(cdecl_function func)
	{
		this->destroy();
		base = new delegator<void, cdecl_function>(func);
	}
	void set_function(stdcall_function func)
	{
		this->destroy();
		base = new delegator<void, stdcall_function>(func);
	}
	void set_function(fastcall_function func)
	{
		this->destroy();
		base = new delegator<void, fastcall_function>(func);
	}
};

//---------------------------------------------------------------------------
// T (*)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12)

template<typename T, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12>
class delegator_base<T (*)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12)>
{
public:
	virtual T invoke(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12)=0;
	virtual delegator_base* clone()const=0;
};

template<typename classT, typename T, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12>
class delegator<classT, T __cdecl (*)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12)> : public delegator_base<T (*)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12)>
{
public:
	typedef T __cdecl (classT::*function_type)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12);
	classT *_C;
	function_type _F;
	delegator(classT *c, function_type func):_C(c), _F(func){/*void*/}
	T invoke(T0 p0, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8, T9 p9, T10 p10, T11 p11, T12 p12)
	{
		return (_C->*_F)(p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12);
	}
	delegator_base<T (*)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12)>* clone()const
	{
		return new delegator(*this);
	}
};

template<typename T, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12>
class delegator<void, T __cdecl (*)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12)> : public delegator_base<T (*)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12)>
{
public:
	typedef T __cdecl (*function_type)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12);
	function_type _F;
	delegator(function_type func):_F(func){/*void*/}
	T invoke(T0 p0, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8, T9 p9, T10 p10, T11 p11, T12 p12)
	{
		return (*_F)(p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12);
	}
	delegator_base<T (*)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12)>* clone()const
	{
		return new delegator(*this);
	}
};

template<typename classT, typename T, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12>
class delegator<classT, T __stdcall (*)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12)> : public delegator_base<T (*)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12)>
{
public:
	typedef T __stdcall (classT::*function_type)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12);
	classT *_C;
	function_type _F;
	delegator(classT *c, function_type func):_C(c), _F(func){/*void*/}
	T invoke(T0 p0, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8, T9 p9, T10 p10, T11 p11, T12 p12)
	{
		return (_C->*_F)(p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12);
	}
	delegator_base<T (*)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12)>* clone()const
	{
		return new delegator(*this);
	}
};

template<typename T, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12>
class delegator<void, T __stdcall (*)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12)> : public delegator_base<T (*)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12)>
{
public:
	typedef T __stdcall (*function_type)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12);
	function_type _F;
	delegator(function_type func):_F(func){/*void*/}
	T invoke(T0 p0, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8, T9 p9, T10 p10, T11 p11, T12 p12)
	{
		return (*_F)(p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12);
	}
	delegator_base<T (*)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12)>* clone()const
	{
		return new delegator(*this);
	}
};

template<typename classT, typename T, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12>
class delegator<classT, T __fastcall (*)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12)> : public delegator_base<T (*)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12)>
{
public:
	typedef T __fastcall (classT::*function_type)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12);
	classT *_C;
	function_type _F;
	delegator(classT *c, function_type func):_C(c), _F(func){/*void*/}
	T invoke(T0 p0, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8, T9 p9, T10 p10, T11 p11, T12 p12)
	{
		return (_C->*_F)(p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12);
	}
	delegator_base<T (*)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12)>* clone()const
	{
		return new delegator(*this);
	}
};

template<typename T, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12>
class delegator<void, T __fastcall (*)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12)> : public delegator_base<T (*)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12)>
{
public:
	typedef T __fastcall (*function_type)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12);
	function_type _F;
	delegator(function_type func):_F(func){/*void*/}
	T invoke(T0 p0, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8, T9 p9, T10 p10, T11 p11, T12 p12)
	{
		return (*_F)(p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12);
	}
	delegator_base<T (*)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12)>* clone()const
	{
		return new delegator(*this);
	}
};

template<typename T, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12>
class delegate<T (*)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12)>
{
public:
	typedef T (*std_function)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12);
	typedef T __cdecl (*cdecl_function)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12);
	typedef T __stdcall (*stdcall_function)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12);
	typedef T __fastcall (*fastcall_function)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12);
	typedef delegate<std_function> this_type;
	delegator_base<std_function> *base;
	delegate():base(null){/*void*/};
	delegate(const this_type& d):base(d.base_clone()){/*void*/}
	~delegate()
	{
		this->destroy();
	}
	operator bool()const
	{
		return base;
	}
	T operator()(T0 p0, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8, T9 p9, T10 p10, T11 p11, T12 p12)
	{
		return base->invoke(p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12);
	}
	void destroy()
	{
		if(base)delete base, base = null;
	}
	delegator_base<std_function>* base_clone()const
	{
		if(base)return base->clone(); else return null;
	}
	const this_type& operator=(const this_type& d)
	{
		this->destroy();
		base = d.base_clone();
		return *this;
	}
	template<typename classT>
	void set_function(classT *c, T __cdecl (classT::*func)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12))
	{
		this->destroy();
		base = new delegator<classT, cdecl_function>(c, func);
	}
	template<typename classT>
	void set_function(classT *c, T __stdcall (classT::*func)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12))
	{
		this->destroy();
		base = new delegator<classT, stdcall_function>(c, func);
	}
	template<typename classT>
	void set_function(classT *c, T __fastcall (classT::*func)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12))
	{
		this->destroy();
		base = new delegator<classT, fastcall_function>(c, func);
	}
	void set_function(cdecl_function func)
	{
		this->destroy();
		base = new delegator<void, cdecl_function>(func);
	}
	void set_function(stdcall_function func)
	{
		this->destroy();
		base = new delegator<void, stdcall_function>(func);
	}
	void set_function(fastcall_function func)
	{
		this->destroy();
		base = new delegator<void, fastcall_function>(func);
	}
};

//---------------------------------------------------------------------------
// T (*)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13)

template<typename T, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13>
class delegator_base<T (*)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13)>
{
public:
	virtual T invoke(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13)=0;
	virtual delegator_base* clone()const=0;
};

template<typename classT, typename T, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13>
class delegator<classT, T __cdecl (*)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13)> : public delegator_base<T (*)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13)>
{
public:
	typedef T __cdecl (classT::*function_type)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13);
	classT *_C;
	function_type _F;
	delegator(classT *c, function_type func):_C(c), _F(func){/*void*/}
	T invoke(T0 p0, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8, T9 p9, T10 p10, T11 p11, T12 p12, T13 p13)
	{
		return (_C->*_F)(p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13);
	}
	delegator_base<T (*)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13)>* clone()const
	{
		return new delegator(*this);
	}
};

template<typename T, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13>
class delegator<void, T __cdecl (*)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13)> : public delegator_base<T (*)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13)>
{
public:
	typedef T __cdecl (*function_type)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13);
	function_type _F;
	delegator(function_type func):_F(func){/*void*/}
	T invoke(T0 p0, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8, T9 p9, T10 p10, T11 p11, T12 p12, T13 p13)
	{
		return (*_F)(p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13);
	}
	delegator_base<T (*)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13)>* clone()const
	{
		return new delegator(*this);
	}
};

template<typename classT, typename T, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13>
class delegator<classT, T __stdcall (*)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13)> : public delegator_base<T (*)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13)>
{
public:
	typedef T __stdcall (classT::*function_type)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13);
	classT *_C;
	function_type _F;
	delegator(classT *c, function_type func):_C(c), _F(func){/*void*/}
	T invoke(T0 p0, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8, T9 p9, T10 p10, T11 p11, T12 p12, T13 p13)
	{
		return (_C->*_F)(p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13);
	}
	delegator_base<T (*)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13)>* clone()const
	{
		return new delegator(*this);
	}
};

template<typename T, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13>
class delegator<void, T __stdcall (*)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13)> : public delegator_base<T (*)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13)>
{
public:
	typedef T __stdcall (*function_type)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13);
	function_type _F;
	delegator(function_type func):_F(func){/*void*/}
	T invoke(T0 p0, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8, T9 p9, T10 p10, T11 p11, T12 p12, T13 p13)
	{
		return (*_F)(p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13);
	}
	delegator_base<T (*)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13)>* clone()const
	{
		return new delegator(*this);
	}
};

template<typename classT, typename T, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13>
class delegator<classT, T __fastcall (*)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13)> : public delegator_base<T (*)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13)>
{
public:
	typedef T __fastcall (classT::*function_type)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13);
	classT *_C;
	function_type _F;
	delegator(classT *c, function_type func):_C(c), _F(func){/*void*/}
	T invoke(T0 p0, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8, T9 p9, T10 p10, T11 p11, T12 p12, T13 p13)
	{
		return (_C->*_F)(p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13);
	}
	delegator_base<T (*)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13)>* clone()const
	{
		return new delegator(*this);
	}
};

template<typename T, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13>
class delegator<void, T __fastcall (*)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13)> : public delegator_base<T (*)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13)>
{
public:
	typedef T __fastcall (*function_type)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13);
	function_type _F;
	delegator(function_type func):_F(func){/*void*/}
	T invoke(T0 p0, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8, T9 p9, T10 p10, T11 p11, T12 p12, T13 p13)
	{
		return (*_F)(p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13);
	}
	delegator_base<T (*)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13)>* clone()const
	{
		return new delegator(*this);
	}
};

template<typename T, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13>
class delegate<T (*)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13)>
{
public:
	typedef T (*std_function)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13);
	typedef T __cdecl (*cdecl_function)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13);
	typedef T __stdcall (*stdcall_function)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13);
	typedef T __fastcall (*fastcall_function)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13);
	typedef delegate<std_function> this_type;
	delegator_base<std_function> *base;
	delegate():base(null){/*void*/};
	delegate(const this_type& d):base(d.base_clone()){/*void*/}
	~delegate()
	{
		this->destroy();
	}
	operator bool()const
	{
		return base;
	}
	T operator()(T0 p0, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8, T9 p9, T10 p10, T11 p11, T12 p12, T13 p13)
	{
		return base->invoke(p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13);
	}
	void destroy()
	{
		if(base)delete base, base = null;
	}
	delegator_base<std_function>* base_clone()const
	{
		if(base)return base->clone(); else return null;
	}
	const this_type& operator=(const this_type& d)
	{
		this->destroy();
		base = d.base_clone();
		return *this;
	}
	template<typename classT>
	void set_function(classT *c, T __cdecl (classT::*func)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13))
	{
		this->destroy();
		base = new delegator<classT, cdecl_function>(c, func);
	}
	template<typename classT>
	void set_function(classT *c, T __stdcall (classT::*func)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13))
	{
		this->destroy();
		base = new delegator<classT, stdcall_function>(c, func);
	}
	template<typename classT>
	void set_function(classT *c, T __fastcall (classT::*func)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13))
	{
		this->destroy();
		base = new delegator<classT, fastcall_function>(c, func);
	}
	void set_function(cdecl_function func)
	{
		this->destroy();
		base = new delegator<void, cdecl_function>(func);
	}
	void set_function(stdcall_function func)
	{
		this->destroy();
		base = new delegator<void, stdcall_function>(func);
	}
	void set_function(fastcall_function func)
	{
		this->destroy();
		base = new delegator<void, fastcall_function>(func);
	}
};

//---------------------------------------------------------------------------
// T (*)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14)

template<typename T, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13, typename T14>
class delegator_base<T (*)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14)>
{
public:
	virtual T invoke(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14)=0;
	virtual delegator_base* clone()const=0;
};

template<typename classT, typename T, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13, typename T14>
class delegator<classT, T __cdecl (*)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14)> : public delegator_base<T (*)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14)>
{
public:
	typedef T __cdecl (classT::*function_type)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14);
	classT *_C;
	function_type _F;
	delegator(classT *c, function_type func):_C(c), _F(func){/*void*/}
	T invoke(T0 p0, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8, T9 p9, T10 p10, T11 p11, T12 p12, T13 p13, T14 p14)
	{
		return (_C->*_F)(p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14);
	}
	delegator_base<T (*)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14)>* clone()const
	{
		return new delegator(*this);
	}
};

template<typename T, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13, typename T14>
class delegator<void, T __cdecl (*)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14)> : public delegator_base<T (*)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14)>
{
public:
	typedef T __cdecl (*function_type)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14);
	function_type _F;
	delegator(function_type func):_F(func){/*void*/}
	T invoke(T0 p0, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8, T9 p9, T10 p10, T11 p11, T12 p12, T13 p13, T14 p14)
	{
		return (*_F)(p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14);
	}
	delegator_base<T (*)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14)>* clone()const
	{
		return new delegator(*this);
	}
};

template<typename classT, typename T, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13, typename T14>
class delegator<classT, T __stdcall (*)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14)> : public delegator_base<T (*)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14)>
{
public:
	typedef T __stdcall (classT::*function_type)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14);
	classT *_C;
	function_type _F;
	delegator(classT *c, function_type func):_C(c), _F(func){/*void*/}
	T invoke(T0 p0, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8, T9 p9, T10 p10, T11 p11, T12 p12, T13 p13, T14 p14)
	{
		return (_C->*_F)(p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14);
	}
	delegator_base<T (*)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14)>* clone()const
	{
		return new delegator(*this);
	}
};

template<typename T, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13, typename T14>
class delegator<void, T __stdcall (*)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14)> : public delegator_base<T (*)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14)>
{
public:
	typedef T __stdcall (*function_type)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14);
	function_type _F;
	delegator(function_type func):_F(func){/*void*/}
	T invoke(T0 p0, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8, T9 p9, T10 p10, T11 p11, T12 p12, T13 p13, T14 p14)
	{
		return (*_F)(p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14);
	}
	delegator_base<T (*)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14)>* clone()const
	{
		return new delegator(*this);
	}
};

template<typename classT, typename T, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13, typename T14>
class delegator<classT, T __fastcall (*)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14)> : public delegator_base<T (*)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14)>
{
public:
	typedef T __fastcall (classT::*function_type)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14);
	classT *_C;
	function_type _F;
	delegator(classT *c, function_type func):_C(c), _F(func){/*void*/}
	T invoke(T0 p0, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8, T9 p9, T10 p10, T11 p11, T12 p12, T13 p13, T14 p14)
	{
		return (_C->*_F)(p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14);
	}
	delegator_base<T (*)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14)>* clone()const
	{
		return new delegator(*this);
	}
};

template<typename T, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13, typename T14>
class delegator<void, T __fastcall (*)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14)> : public delegator_base<T (*)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14)>
{
public:
	typedef T __fastcall (*function_type)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14);
	function_type _F;
	delegator(function_type func):_F(func){/*void*/}
	T invoke(T0 p0, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8, T9 p9, T10 p10, T11 p11, T12 p12, T13 p13, T14 p14)
	{
		return (*_F)(p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14);
	}
	delegator_base<T (*)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14)>* clone()const
	{
		return new delegator(*this);
	}
};

template<typename T, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13, typename T14>
class delegate<T (*)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14)>
{
public:
	typedef T (*std_function)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14);
	typedef T __cdecl (*cdecl_function)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14);
	typedef T __stdcall (*stdcall_function)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14);
	typedef T __fastcall (*fastcall_function)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14);
	typedef delegate<std_function> this_type;
	delegator_base<std_function> *base;
	delegate():base(null){/*void*/};
	delegate(const this_type& d):base(d.base_clone()){/*void*/}
	~delegate()
	{
		this->destroy();
	}
	operator bool()const
	{
		return base;
	}
	T operator()(T0 p0, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8, T9 p9, T10 p10, T11 p11, T12 p12, T13 p13, T14 p14)
	{
		return base->invoke(p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14);
	}
	void destroy()
	{
		if(base)delete base, base = null;
	}
	delegator_base<std_function>* base_clone()const
	{
		if(base)return base->clone(); else return null;
	}
	const this_type& operator=(const this_type& d)
	{
		this->destroy();
		base = d.base_clone();
		return *this;
	}
	template<typename classT>
	void set_function(classT *c, T __cdecl (classT::*func)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14))
	{
		this->destroy();
		base = new delegator<classT, cdecl_function>(c, func);
	}
	template<typename classT>
	void set_function(classT *c, T __stdcall (classT::*func)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14))
	{
		this->destroy();
		base = new delegator<classT, stdcall_function>(c, func);
	}
	template<typename classT>
	void set_function(classT *c, T __fastcall (classT::*func)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14))
	{
		this->destroy();
		base = new delegator<classT, fastcall_function>(c, func);
	}
	void set_function(cdecl_function func)
	{
		this->destroy();
		base = new delegator<void, cdecl_function>(func);
	}
	void set_function(stdcall_function func)
	{
		this->destroy();
		base = new delegator<void, stdcall_function>(func);
	}
	void set_function(fastcall_function func)
	{
		this->destroy();
		base = new delegator<void, fastcall_function>(func);
	}
};

//---------------------------------------------------------------------------
// T (*)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15)

template<typename T, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13, typename T14, typename T15>
class delegator_base<T (*)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15)>
{
public:
	virtual T invoke(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15)=0;
	virtual delegator_base* clone()const=0;
};

template<typename classT, typename T, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13, typename T14, typename T15>
class delegator<classT, T __cdecl (*)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15)> : public delegator_base<T (*)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15)>
{
public:
	typedef T __cdecl (classT::*function_type)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15);
	classT *_C;
	function_type _F;
	delegator(classT *c, function_type func):_C(c), _F(func){/*void*/}
	T invoke(T0 p0, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8, T9 p9, T10 p10, T11 p11, T12 p12, T13 p13, T14 p14, T15 p15)
	{
		return (_C->*_F)(p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15);
	}
	delegator_base<T (*)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15)>* clone()const
	{
		return new delegator(*this);
	}
};

template<typename T, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13, typename T14, typename T15>
class delegator<void, T __cdecl (*)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15)> : public delegator_base<T (*)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15)>
{
public:
	typedef T __cdecl (*function_type)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15);
	function_type _F;
	delegator(function_type func):_F(func){/*void*/}
	T invoke(T0 p0, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8, T9 p9, T10 p10, T11 p11, T12 p12, T13 p13, T14 p14, T15 p15)
	{
		return (*_F)(p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15);
	}
	delegator_base<T (*)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15)>* clone()const
	{
		return new delegator(*this);
	}
};

template<typename classT, typename T, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13, typename T14, typename T15>
class delegator<classT, T __stdcall (*)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15)> : public delegator_base<T (*)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15)>
{
public:
	typedef T __stdcall (classT::*function_type)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15);
	classT *_C;
	function_type _F;
	delegator(classT *c, function_type func):_C(c), _F(func){/*void*/}
	T invoke(T0 p0, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8, T9 p9, T10 p10, T11 p11, T12 p12, T13 p13, T14 p14, T15 p15)
	{
		return (_C->*_F)(p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15);
	}
	delegator_base<T (*)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15)>* clone()const
	{
		return new delegator(*this);
	}
};

template<typename T, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13, typename T14, typename T15>
class delegator<void, T __stdcall (*)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15)> : public delegator_base<T (*)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15)>
{
public:
	typedef T __stdcall (*function_type)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15);
	function_type _F;
	delegator(function_type func):_F(func){/*void*/}
	T invoke(T0 p0, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8, T9 p9, T10 p10, T11 p11, T12 p12, T13 p13, T14 p14, T15 p15)
	{
		return (*_F)(p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15);
	}
	delegator_base<T (*)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15)>* clone()const
	{
		return new delegator(*this);
	}
};

template<typename classT, typename T, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13, typename T14, typename T15>
class delegator<classT, T __fastcall (*)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15)> : public delegator_base<T (*)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15)>
{
public:
	typedef T __fastcall (classT::*function_type)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15);
	classT *_C;
	function_type _F;
	delegator(classT *c, function_type func):_C(c), _F(func){/*void*/}
	T invoke(T0 p0, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8, T9 p9, T10 p10, T11 p11, T12 p12, T13 p13, T14 p14, T15 p15)
	{
		return (_C->*_F)(p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15);
	}
	delegator_base<T (*)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15)>* clone()const
	{
		return new delegator(*this);
	}
};

template<typename T, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13, typename T14, typename T15>
class delegator<void, T __fastcall (*)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15)> : public delegator_base<T (*)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15)>
{
public:
	typedef T __fastcall (*function_type)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15);
	function_type _F;
	delegator(function_type func):_F(func){/*void*/}
	T invoke(T0 p0, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8, T9 p9, T10 p10, T11 p11, T12 p12, T13 p13, T14 p14, T15 p15)
	{
		return (*_F)(p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15);
	}
	delegator_base<T (*)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15)>* clone()const
	{
		return new delegator(*this);
	}
};

template<typename T, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13, typename T14, typename T15>
class delegate<T (*)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15)>
{
public:
	typedef T (*std_function)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15);
	typedef T __cdecl (*cdecl_function)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15);
	typedef T __stdcall (*stdcall_function)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15);
	typedef T __fastcall (*fastcall_function)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15);
	typedef delegate<std_function> this_type;
	delegator_base<std_function> *base;
	delegate():base(null){/*void*/};
	delegate(const this_type& d):base(d.base_clone()){/*void*/}
	~delegate()
	{
		this->destroy();
	}
	operator bool()const
	{
		return base;
	}
	T operator()(T0 p0, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8, T9 p9, T10 p10, T11 p11, T12 p12, T13 p13, T14 p14, T15 p15)
	{
		return base->invoke(p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15);
	}
	void destroy()
	{
		if(base)delete base, base = null;
	}
	delegator_base<std_function>* base_clone()const
	{
		if(base)return base->clone(); else return null;
	}
	const this_type& operator=(const this_type& d)
	{
		this->destroy();
		base = d.base_clone();
		return *this;
	}
	template<typename classT>
	void set_function(classT *c, T __cdecl (classT::*func)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15))
	{
		this->destroy();
		base = new delegator<classT, cdecl_function>(c, func);
	}
	template<typename classT>
	void set_function(classT *c, T __stdcall (classT::*func)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15))
	{
		this->destroy();
		base = new delegator<classT, stdcall_function>(c, func);
	}
	template<typename classT>
	void set_function(classT *c, T __fastcall (classT::*func)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15))
	{
		this->destroy();
		base = new delegator<classT, fastcall_function>(c, func);
	}
	void set_function(cdecl_function func)
	{
		this->destroy();
		base = new delegator<void, cdecl_function>(func);
	}
	void set_function(stdcall_function func)
	{
		this->destroy();
		base = new delegator<void, stdcall_function>(func);
	}
	void set_function(fastcall_function func)
	{
		this->destroy();
		base = new delegator<void, fastcall_function>(func);
	}
};

} //end namespace cgl

#endif // DELEGATE_HPP_20170215034129
