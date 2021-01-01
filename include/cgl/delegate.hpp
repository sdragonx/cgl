/*
 Copyright (c) 2005-2020 sdragonx (mail:sdragonx@foxmail.com)

 delegate.hpp

 委托类

 程序生成的代码，最大支持16个参数，支持__cdecl、__stdcall、__fastcall
 编辑器不支持C11也能用，c++builder 6.0测试可用
 为了通用性，只能这么暴力了

 2017-02-15 03:41:29

 sdragonx 1:05 2018/1/10 修订

 2018/7/29 1:31 __stdcall __fastcall等调用方式，gcc出现重定义错误。。所以
 修改了，只支持一种或定义的调用方式，一般来说够用了
 gcc 7.2.0测试通过

*/
#ifndef DELEGATE_HPP_20170215034129
#define DELEGATE_HPP_20170215034129

namespace cgl{

template<typename T>
class delegator_base;

template<typename T>
class delegate;

//设定函数调用方式 __cdecl __stdcall __fastcall
#ifndef CGL_DELEGATE_CALL
    #define CGL_DELEGATE_CALL
#endif

//---------------------------------------------------------------------------
// T (*)()

template<typename T>
class delegator_base<T (*)()>
{
public:
    virtual ~delegator_base(){ /*void*/ }
    virtual T invoke()=0;
    virtual delegator_base* clone()const=0;
};

template<typename classT, typename T>
class class_delegatorT0: public delegator_base<T (*)()>
{
public:
    typedef T CGL_DELEGATE_CALL (classT::*function_type)();
    classT *_C;
    function_type _F;
    class_delegatorT0(classT *c, function_type func):_C(c), _F(func){/*void*/}
    T invoke()
    {
        return (_C->*_F)();
    }
    delegator_base<T (*)()>* clone()const
    {
        return new class_delegatorT0(*this);
    }
};

template<typename T>
class delegatorT0 : public delegator_base<T (*)()>
{
public:
    typedef T CGL_DELEGATE_CALL (*function_type)();
    function_type _F;
    delegatorT0(function_type func):_F(func){/*void*/}
    T invoke()
    {
        return (*_F)();
    }
    delegator_base<T (*)()>* clone()const
    {
        return new delegatorT0(*this);
    }
};

template<typename T>
class delegate<T (*)()>
{
public:
    typedef T (*function_type)();
    typedef delegate<function_type> this_type;
    delegator_base<function_type> *base;
    delegate():base(0){/*void*/};
    delegate(const this_type& d):base(d.base_clone()){/*void*/}
    ~delegate() { this->destroy(); }
    operator bool()const { return base; }
    void destroy() { if(base)delete base, base = 0; }
    delegator_base<function_type>* base_clone()const { if(base)return base->clone(); else return 0; }
    const this_type& operator=(const this_type& d)
    {
        this->destroy();
        base = d.base_clone();
        return *this;
    }
    T operator()()
    {
        return base->invoke();
    }
    template<typename classT>
    void set_function(classT *c, T CGL_DELEGATE_CALL (classT::*func)())
    {
        this->destroy();
        base = new class_delegatorT0<classT, T>(c, func);
    }
    void set_function(T CGL_DELEGATE_CALL (*func)())
    {
        this->destroy();
        base = new delegatorT0<T>(func);
    }
};

//---------------------------------------------------------------------------
// T (*)(T0)

template<typename T, typename T0>
class delegator_base<T (*)(T0)>
{
public:
    virtual ~delegator_base(){ /*void*/ }
    virtual T invoke(T0)=0;
    virtual delegator_base* clone()const=0;
};

template<typename classT, typename T, typename T0>
class class_delegatorT1: public delegator_base<T (*)(T0)>
{
public:
    typedef T CGL_DELEGATE_CALL (classT::*function_type)(T0);
    classT *_C;
    function_type _F;
    class_delegatorT1(classT *c, function_type func):_C(c), _F(func){/*void*/}
    T invoke(T0 p0)
    {
        return (_C->*_F)(p0);
    }
    delegator_base<T (*)(T0)>* clone()const
    {
        return new class_delegatorT1(*this);
    }
};

template<typename T, typename T0>
class delegatorT1 : public delegator_base<T (*)(T0)>
{
public:
    typedef T CGL_DELEGATE_CALL (*function_type)(T0);
    function_type _F;
    delegatorT1(function_type func):_F(func){/*void*/}
    T invoke(T0 p0)
    {
        return (*_F)(p0);
    }
    delegator_base<T (*)(T0)>* clone()const
    {
        return new delegatorT1(*this);
    }
};

template<typename T, typename T0>
class delegate<T (*)(T0)>
{
public:
    typedef T (*function_type)(T0);
    typedef delegate<function_type> this_type;
    delegator_base<function_type> *base;
    delegate():base(0){/*void*/};
    delegate(const this_type& d):base(d.base_clone()){/*void*/}
    ~delegate() { this->destroy(); }
    operator bool()const { return base; }
    void destroy() { if(base)delete base, base = 0; }
    delegator_base<function_type>* base_clone()const { if(base)return base->clone(); else return 0; }
    const this_type& operator=(const this_type& d)
    {
        this->destroy();
        base = d.base_clone();
        return *this;
    }
    T operator()(T0 p0)
    {
        return base->invoke(p0);
    }
    template<typename classT>
    void set_function(classT *c, T CGL_DELEGATE_CALL (classT::*func)(T0))
    {
        this->destroy();
        base = new class_delegatorT1<classT, T, T0>(c, func);
    }
    void set_function(T CGL_DELEGATE_CALL (*func)(T0))
    {
        this->destroy();
        base = new delegatorT1<T, T0>(func);
    }
};

//---------------------------------------------------------------------------
// T (*)(T0, T1)

template<typename T, typename T0, typename T1>
class delegator_base<T (*)(T0, T1)>
{
public:
    virtual ~delegator_base(){ /*void*/ }
    virtual T invoke(T0, T1)=0;
    virtual delegator_base* clone()const=0;
};

template<typename classT, typename T, typename T0, typename T1>
class class_delegatorT2: public delegator_base<T (*)(T0, T1)>
{
public:
    typedef T CGL_DELEGATE_CALL (classT::*function_type)(T0, T1);
    classT *_C;
    function_type _F;
    class_delegatorT2(classT *c, function_type func):_C(c), _F(func){/*void*/}
    T invoke(T0 p0, T1 p1)
    {
        return (_C->*_F)(p0, p1);
    }
    delegator_base<T (*)(T0, T1)>* clone()const
    {
        return new class_delegatorT2(*this);
    }
};

template<typename T, typename T0, typename T1>
class delegatorT2 : public delegator_base<T (*)(T0, T1)>
{
public:
    typedef T CGL_DELEGATE_CALL (*function_type)(T0, T1);
    function_type _F;
    delegatorT2(function_type func):_F(func){/*void*/}
    T invoke(T0 p0, T1 p1)
    {
        return (*_F)(p0, p1);
    }
    delegator_base<T (*)(T0, T1)>* clone()const
    {
        return new delegatorT2(*this);
    }
};

template<typename T, typename T0, typename T1>
class delegate<T (*)(T0, T1)>
{
public:
    typedef T (*function_type)(T0, T1);
    typedef delegate<function_type> this_type;
    delegator_base<function_type> *base;
    delegate():base(0){/*void*/};
    delegate(const this_type& d):base(d.base_clone()){/*void*/}
    ~delegate() { this->destroy(); }
    operator bool()const { return base; }
    void destroy() { if(base)delete base, base = 0; }
    delegator_base<function_type>* base_clone()const { if(base)return base->clone(); else return 0; }
    const this_type& operator=(const this_type& d)
    {
        this->destroy();
        base = d.base_clone();
        return *this;
    }
    T operator()(T0 p0, T1 p1)
    {
        return base->invoke(p0, p1);
    }
    template<typename classT>
    void set_function(classT *c, T CGL_DELEGATE_CALL (classT::*func)(T0, T1))
    {
        this->destroy();
        base = new class_delegatorT2<classT, T, T0, T1>(c, func);
    }
    void set_function(T CGL_DELEGATE_CALL (*func)(T0, T1))
    {
        this->destroy();
        base = new delegatorT2<T, T0, T1>(func);
    }
};

//---------------------------------------------------------------------------
// T (*)(T0, T1, T2)

template<typename T, typename T0, typename T1, typename T2>
class delegator_base<T (*)(T0, T1, T2)>
{
public:
    virtual ~delegator_base(){ /*void*/ }
    virtual T invoke(T0, T1, T2)=0;
    virtual delegator_base* clone()const=0;
};

template<typename classT, typename T, typename T0, typename T1, typename T2>
class class_delegatorT3: public delegator_base<T (*)(T0, T1, T2)>
{
public:
    typedef T CGL_DELEGATE_CALL (classT::*function_type)(T0, T1, T2);
    classT *_C;
    function_type _F;
    class_delegatorT3(classT *c, function_type func):_C(c), _F(func){/*void*/}
    T invoke(T0 p0, T1 p1, T2 p2)
    {
        return (_C->*_F)(p0, p1, p2);
    }
    delegator_base<T (*)(T0, T1, T2)>* clone()const
    {
        return new class_delegatorT3(*this);
    }
};

template<typename T, typename T0, typename T1, typename T2>
class delegatorT3 : public delegator_base<T (*)(T0, T1, T2)>
{
public:
    typedef T CGL_DELEGATE_CALL (*function_type)(T0, T1, T2);
    function_type _F;
    delegatorT3(function_type func):_F(func){/*void*/}
    T invoke(T0 p0, T1 p1, T2 p2)
    {
        return (*_F)(p0, p1, p2);
    }
    delegator_base<T (*)(T0, T1, T2)>* clone()const
    {
        return new delegatorT3(*this);
    }
};

template<typename T, typename T0, typename T1, typename T2>
class delegate<T (*)(T0, T1, T2)>
{
public:
    typedef T (*function_type)(T0, T1, T2);
    typedef delegate<function_type> this_type;
    delegator_base<function_type> *base;
    delegate():base(0){/*void*/};
    delegate(const this_type& d):base(d.base_clone()){/*void*/}
    ~delegate() { this->destroy(); }
    operator bool()const { return base; }
    void destroy() { if(base)delete base, base = 0; }
    delegator_base<function_type>* base_clone()const { if(base)return base->clone(); else return 0; }
    const this_type& operator=(const this_type& d)
    {
        this->destroy();
        base = d.base_clone();
        return *this;
    }
    T operator()(T0 p0, T1 p1, T2 p2)
    {
        return base->invoke(p0, p1, p2);
    }
    template<typename classT>
    void set_function(classT *c, T CGL_DELEGATE_CALL (classT::*func)(T0, T1, T2))
    {
        this->destroy();
        base = new class_delegatorT3<classT, T, T0, T1, T2>(c, func);
    }
    void set_function(T CGL_DELEGATE_CALL (*func)(T0, T1, T2))
    {
        this->destroy();
        base = new delegatorT3<T, T0, T1, T2>(func);
    }
};

//---------------------------------------------------------------------------
// T (*)(T0, T1, T2, T3)

template<typename T, typename T0, typename T1, typename T2, typename T3>
class delegator_base<T (*)(T0, T1, T2, T3)>
{
public:
    virtual ~delegator_base(){ /*void*/ }
    virtual T invoke(T0, T1, T2, T3)=0;
    virtual delegator_base* clone()const=0;
};

template<typename classT, typename T, typename T0, typename T1, typename T2, typename T3>
class class_delegatorT4: public delegator_base<T (*)(T0, T1, T2, T3)>
{
public:
    typedef T CGL_DELEGATE_CALL (classT::*function_type)(T0, T1, T2, T3);
    classT *_C;
    function_type _F;
    class_delegatorT4(classT *c, function_type func):_C(c), _F(func){/*void*/}
    T invoke(T0 p0, T1 p1, T2 p2, T3 p3)
    {
        return (_C->*_F)(p0, p1, p2, p3);
    }
    delegator_base<T (*)(T0, T1, T2, T3)>* clone()const
    {
        return new class_delegatorT4(*this);
    }
};

template<typename T, typename T0, typename T1, typename T2, typename T3>
class delegatorT4 : public delegator_base<T (*)(T0, T1, T2, T3)>
{
public:
    typedef T CGL_DELEGATE_CALL (*function_type)(T0, T1, T2, T3);
    function_type _F;
    delegatorT4(function_type func):_F(func){/*void*/}
    T invoke(T0 p0, T1 p1, T2 p2, T3 p3)
    {
        return (*_F)(p0, p1, p2, p3);
    }
    delegator_base<T (*)(T0, T1, T2, T3)>* clone()const
    {
        return new delegatorT4(*this);
    }
};

template<typename T, typename T0, typename T1, typename T2, typename T3>
class delegate<T (*)(T0, T1, T2, T3)>
{
public:
    typedef T (*function_type)(T0, T1, T2, T3);
    typedef delegate<function_type> this_type;
    delegator_base<function_type> *base;
    delegate():base(0){/*void*/};
    delegate(const this_type& d):base(d.base_clone()){/*void*/}
    ~delegate() { this->destroy(); }
    operator bool()const { return base; }
    void destroy() { if(base)delete base, base = 0; }
    delegator_base<function_type>* base_clone()const { if(base)return base->clone(); else return 0; }
    const this_type& operator=(const this_type& d)
    {
        this->destroy();
        base = d.base_clone();
        return *this;
    }
    T operator()(T0 p0, T1 p1, T2 p2, T3 p3)
    {
        return base->invoke(p0, p1, p2, p3);
    }
    template<typename classT>
    void set_function(classT *c, T CGL_DELEGATE_CALL (classT::*func)(T0, T1, T2, T3))
    {
        this->destroy();
        base = new class_delegatorT4<classT, T, T0, T1, T2, T3>(c, func);
    }
    void set_function(T CGL_DELEGATE_CALL (*func)(T0, T1, T2, T3))
    {
        this->destroy();
        base = new delegatorT4<T, T0, T1, T2, T3>(func);
    }
};

//---------------------------------------------------------------------------
// T (*)(T0, T1, T2, T3, T4)

template<typename T, typename T0, typename T1, typename T2, typename T3, typename T4>
class delegator_base<T (*)(T0, T1, T2, T3, T4)>
{
public:
    virtual ~delegator_base(){ /*void*/ }
    virtual T invoke(T0, T1, T2, T3, T4)=0;
    virtual delegator_base* clone()const=0;
};

template<typename classT, typename T, typename T0, typename T1, typename T2, typename T3, typename T4>
class class_delegatorT5: public delegator_base<T (*)(T0, T1, T2, T3, T4)>
{
public:
    typedef T CGL_DELEGATE_CALL (classT::*function_type)(T0, T1, T2, T3, T4);
    classT *_C;
    function_type _F;
    class_delegatorT5(classT *c, function_type func):_C(c), _F(func){/*void*/}
    T invoke(T0 p0, T1 p1, T2 p2, T3 p3, T4 p4)
    {
        return (_C->*_F)(p0, p1, p2, p3, p4);
    }
    delegator_base<T (*)(T0, T1, T2, T3, T4)>* clone()const
    {
        return new class_delegatorT5(*this);
    }
};

template<typename T, typename T0, typename T1, typename T2, typename T3, typename T4>
class delegatorT5 : public delegator_base<T (*)(T0, T1, T2, T3, T4)>
{
public:
    typedef T CGL_DELEGATE_CALL (*function_type)(T0, T1, T2, T3, T4);
    function_type _F;
    delegatorT5(function_type func):_F(func){/*void*/}
    T invoke(T0 p0, T1 p1, T2 p2, T3 p3, T4 p4)
    {
        return (*_F)(p0, p1, p2, p3, p4);
    }
    delegator_base<T (*)(T0, T1, T2, T3, T4)>* clone()const
    {
        return new delegatorT5(*this);
    }
};

template<typename T, typename T0, typename T1, typename T2, typename T3, typename T4>
class delegate<T (*)(T0, T1, T2, T3, T4)>
{
public:
    typedef T (*function_type)(T0, T1, T2, T3, T4);
    typedef delegate<function_type> this_type;
    delegator_base<function_type> *base;
    delegate():base(0){/*void*/};
    delegate(const this_type& d):base(d.base_clone()){/*void*/}
    ~delegate() { this->destroy(); }
    operator bool()const { return base; }
    void destroy() { if(base)delete base, base = 0; }
    delegator_base<function_type>* base_clone()const { if(base)return base->clone(); else return 0; }
    const this_type& operator=(const this_type& d)
    {
        this->destroy();
        base = d.base_clone();
        return *this;
    }
    T operator()(T0 p0, T1 p1, T2 p2, T3 p3, T4 p4)
    {
        return base->invoke(p0, p1, p2, p3, p4);
    }
    template<typename classT>
    void set_function(classT *c, T CGL_DELEGATE_CALL (classT::*func)(T0, T1, T2, T3, T4))
    {
        this->destroy();
        base = new class_delegatorT5<classT, T, T0, T1, T2, T3, T4>(c, func);
    }
    void set_function(T CGL_DELEGATE_CALL (*func)(T0, T1, T2, T3, T4))
    {
        this->destroy();
        base = new delegatorT5<T, T0, T1, T2, T3, T4>(func);
    }
};

//---------------------------------------------------------------------------
// T (*)(T0, T1, T2, T3, T4, T5)

template<typename T, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5>
class delegator_base<T (*)(T0, T1, T2, T3, T4, T5)>
{
public:
    virtual ~delegator_base(){ /*void*/ }
    virtual T invoke(T0, T1, T2, T3, T4, T5)=0;
    virtual delegator_base* clone()const=0;
};

template<typename classT, typename T, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5>
class class_delegatorT6: public delegator_base<T (*)(T0, T1, T2, T3, T4, T5)>
{
public:
    typedef T CGL_DELEGATE_CALL (classT::*function_type)(T0, T1, T2, T3, T4, T5);
    classT *_C;
    function_type _F;
    class_delegatorT6(classT *c, function_type func):_C(c), _F(func){/*void*/}
    T invoke(T0 p0, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5)
    {
        return (_C->*_F)(p0, p1, p2, p3, p4, p5);
    }
    delegator_base<T (*)(T0, T1, T2, T3, T4, T5)>* clone()const
    {
        return new class_delegatorT6(*this);
    }
};

template<typename T, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5>
class delegatorT6 : public delegator_base<T (*)(T0, T1, T2, T3, T4, T5)>
{
public:
    typedef T CGL_DELEGATE_CALL (*function_type)(T0, T1, T2, T3, T4, T5);
    function_type _F;
    delegatorT6(function_type func):_F(func){/*void*/}
    T invoke(T0 p0, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5)
    {
        return (*_F)(p0, p1, p2, p3, p4, p5);
    }
    delegator_base<T (*)(T0, T1, T2, T3, T4, T5)>* clone()const
    {
        return new delegatorT6(*this);
    }
};

template<typename T, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5>
class delegate<T (*)(T0, T1, T2, T3, T4, T5)>
{
public:
    typedef T (*function_type)(T0, T1, T2, T3, T4, T5);
    typedef delegate<function_type> this_type;
    delegator_base<function_type> *base;
    delegate():base(0){/*void*/};
    delegate(const this_type& d):base(d.base_clone()){/*void*/}
    ~delegate() { this->destroy(); }
    operator bool()const { return base; }
    void destroy() { if(base)delete base, base = 0; }
    delegator_base<function_type>* base_clone()const { if(base)return base->clone(); else return 0; }
    const this_type& operator=(const this_type& d)
    {
        this->destroy();
        base = d.base_clone();
        return *this;
    }
    T operator()(T0 p0, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5)
    {
        return base->invoke(p0, p1, p2, p3, p4, p5);
    }
    template<typename classT>
    void set_function(classT *c, T CGL_DELEGATE_CALL (classT::*func)(T0, T1, T2, T3, T4, T5))
    {
        this->destroy();
        base = new class_delegatorT6<classT, T, T0, T1, T2, T3, T4, T5>(c, func);
    }
    void set_function(T CGL_DELEGATE_CALL (*func)(T0, T1, T2, T3, T4, T5))
    {
        this->destroy();
        base = new delegatorT6<T, T0, T1, T2, T3, T4, T5>(func);
    }
};

//---------------------------------------------------------------------------
// T (*)(T0, T1, T2, T3, T4, T5, T6)

template<typename T, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
class delegator_base<T (*)(T0, T1, T2, T3, T4, T5, T6)>
{
public:
    virtual ~delegator_base(){ /*void*/ }
    virtual T invoke(T0, T1, T2, T3, T4, T5, T6)=0;
    virtual delegator_base* clone()const=0;
};

template<typename classT, typename T, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
class class_delegatorT7: public delegator_base<T (*)(T0, T1, T2, T3, T4, T5, T6)>
{
public:
    typedef T CGL_DELEGATE_CALL (classT::*function_type)(T0, T1, T2, T3, T4, T5, T6);
    classT *_C;
    function_type _F;
    class_delegatorT7(classT *c, function_type func):_C(c), _F(func){/*void*/}
    T invoke(T0 p0, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6)
    {
        return (_C->*_F)(p0, p1, p2, p3, p4, p5, p6);
    }
    delegator_base<T (*)(T0, T1, T2, T3, T4, T5, T6)>* clone()const
    {
        return new class_delegatorT7(*this);
    }
};

template<typename T, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
class delegatorT7 : public delegator_base<T (*)(T0, T1, T2, T3, T4, T5, T6)>
{
public:
    typedef T CGL_DELEGATE_CALL (*function_type)(T0, T1, T2, T3, T4, T5, T6);
    function_type _F;
    delegatorT7(function_type func):_F(func){/*void*/}
    T invoke(T0 p0, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6)
    {
        return (*_F)(p0, p1, p2, p3, p4, p5, p6);
    }
    delegator_base<T (*)(T0, T1, T2, T3, T4, T5, T6)>* clone()const
    {
        return new delegatorT7(*this);
    }
};

template<typename T, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
class delegate<T (*)(T0, T1, T2, T3, T4, T5, T6)>
{
public:
    typedef T (*function_type)(T0, T1, T2, T3, T4, T5, T6);
    typedef delegate<function_type> this_type;
    delegator_base<function_type> *base;
    delegate():base(0){/*void*/};
    delegate(const this_type& d):base(d.base_clone()){/*void*/}
    ~delegate() { this->destroy(); }
    operator bool()const { return base; }
    void destroy() { if(base)delete base, base = 0; }
    delegator_base<function_type>* base_clone()const { if(base)return base->clone(); else return 0; }
    const this_type& operator=(const this_type& d)
    {
        this->destroy();
        base = d.base_clone();
        return *this;
    }
    T operator()(T0 p0, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6)
    {
        return base->invoke(p0, p1, p2, p3, p4, p5, p6);
    }
    template<typename classT>
    void set_function(classT *c, T CGL_DELEGATE_CALL (classT::*func)(T0, T1, T2, T3, T4, T5, T6))
    {
        this->destroy();
        base = new class_delegatorT7<classT, T, T0, T1, T2, T3, T4, T5, T6>(c, func);
    }
    void set_function(T CGL_DELEGATE_CALL (*func)(T0, T1, T2, T3, T4, T5, T6))
    {
        this->destroy();
        base = new delegatorT7<T, T0, T1, T2, T3, T4, T5, T6>(func);
    }
};

//---------------------------------------------------------------------------
// T (*)(T0, T1, T2, T3, T4, T5, T6, T7)

template<typename T, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7>
class delegator_base<T (*)(T0, T1, T2, T3, T4, T5, T6, T7)>
{
public:
    virtual ~delegator_base(){ /*void*/ }
    virtual T invoke(T0, T1, T2, T3, T4, T5, T6, T7)=0;
    virtual delegator_base* clone()const=0;
};

template<typename classT, typename T, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7>
class class_delegatorT8: public delegator_base<T (*)(T0, T1, T2, T3, T4, T5, T6, T7)>
{
public:
    typedef T CGL_DELEGATE_CALL (classT::*function_type)(T0, T1, T2, T3, T4, T5, T6, T7);
    classT *_C;
    function_type _F;
    class_delegatorT8(classT *c, function_type func):_C(c), _F(func){/*void*/}
    T invoke(T0 p0, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7)
    {
        return (_C->*_F)(p0, p1, p2, p3, p4, p5, p6, p7);
    }
    delegator_base<T (*)(T0, T1, T2, T3, T4, T5, T6, T7)>* clone()const
    {
        return new class_delegatorT8(*this);
    }
};

template<typename T, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7>
class delegatorT8 : public delegator_base<T (*)(T0, T1, T2, T3, T4, T5, T6, T7)>
{
public:
    typedef T CGL_DELEGATE_CALL (*function_type)(T0, T1, T2, T3, T4, T5, T6, T7);
    function_type _F;
    delegatorT8(function_type func):_F(func){/*void*/}
    T invoke(T0 p0, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7)
    {
        return (*_F)(p0, p1, p2, p3, p4, p5, p6, p7);
    }
    delegator_base<T (*)(T0, T1, T2, T3, T4, T5, T6, T7)>* clone()const
    {
        return new delegatorT8(*this);
    }
};

template<typename T, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7>
class delegate<T (*)(T0, T1, T2, T3, T4, T5, T6, T7)>
{
public:
    typedef T (*function_type)(T0, T1, T2, T3, T4, T5, T6, T7);
    typedef delegate<function_type> this_type;
    delegator_base<function_type> *base;
    delegate():base(0){/*void*/};
    delegate(const this_type& d):base(d.base_clone()){/*void*/}
    ~delegate() { this->destroy(); }
    operator bool()const { return base; }
    void destroy() { if(base)delete base, base = 0; }
    delegator_base<function_type>* base_clone()const { if(base)return base->clone(); else return 0; }
    const this_type& operator=(const this_type& d)
    {
        this->destroy();
        base = d.base_clone();
        return *this;
    }
    T operator()(T0 p0, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7)
    {
        return base->invoke(p0, p1, p2, p3, p4, p5, p6, p7);
    }
    template<typename classT>
    void set_function(classT *c, T CGL_DELEGATE_CALL (classT::*func)(T0, T1, T2, T3, T4, T5, T6, T7))
    {
        this->destroy();
        base = new class_delegatorT8<classT, T, T0, T1, T2, T3, T4, T5, T6, T7>(c, func);
    }
    void set_function(T CGL_DELEGATE_CALL (*func)(T0, T1, T2, T3, T4, T5, T6, T7))
    {
        this->destroy();
        base = new delegatorT8<T, T0, T1, T2, T3, T4, T5, T6, T7>(func);
    }
};

//---------------------------------------------------------------------------
// T (*)(T0, T1, T2, T3, T4, T5, T6, T7, T8)

template<typename T, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8>
class delegator_base<T (*)(T0, T1, T2, T3, T4, T5, T6, T7, T8)>
{
public:
    virtual ~delegator_base(){ /*void*/ }
    virtual T invoke(T0, T1, T2, T3, T4, T5, T6, T7, T8)=0;
    virtual delegator_base* clone()const=0;
};

template<typename classT, typename T, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8>
class class_delegatorT9: public delegator_base<T (*)(T0, T1, T2, T3, T4, T5, T6, T7, T8)>
{
public:
    typedef T CGL_DELEGATE_CALL (classT::*function_type)(T0, T1, T2, T3, T4, T5, T6, T7, T8);
    classT *_C;
    function_type _F;
    class_delegatorT9(classT *c, function_type func):_C(c), _F(func){/*void*/}
    T invoke(T0 p0, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8)
    {
        return (_C->*_F)(p0, p1, p2, p3, p4, p5, p6, p7, p8);
    }
    delegator_base<T (*)(T0, T1, T2, T3, T4, T5, T6, T7, T8)>* clone()const
    {
        return new class_delegatorT9(*this);
    }
};

template<typename T, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8>
class delegatorT9 : public delegator_base<T (*)(T0, T1, T2, T3, T4, T5, T6, T7, T8)>
{
public:
    typedef T CGL_DELEGATE_CALL (*function_type)(T0, T1, T2, T3, T4, T5, T6, T7, T8);
    function_type _F;
    delegatorT9(function_type func):_F(func){/*void*/}
    T invoke(T0 p0, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8)
    {
        return (*_F)(p0, p1, p2, p3, p4, p5, p6, p7, p8);
    }
    delegator_base<T (*)(T0, T1, T2, T3, T4, T5, T6, T7, T8)>* clone()const
    {
        return new delegatorT9(*this);
    }
};

template<typename T, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8>
class delegate<T (*)(T0, T1, T2, T3, T4, T5, T6, T7, T8)>
{
public:
    typedef T (*function_type)(T0, T1, T2, T3, T4, T5, T6, T7, T8);
    typedef delegate<function_type> this_type;
    delegator_base<function_type> *base;
    delegate():base(0){/*void*/};
    delegate(const this_type& d):base(d.base_clone()){/*void*/}
    ~delegate() { this->destroy(); }
    operator bool()const { return base; }
    void destroy() { if(base)delete base, base = 0; }
    delegator_base<function_type>* base_clone()const { if(base)return base->clone(); else return 0; }
    const this_type& operator=(const this_type& d)
    {
        this->destroy();
        base = d.base_clone();
        return *this;
    }
    T operator()(T0 p0, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8)
    {
        return base->invoke(p0, p1, p2, p3, p4, p5, p6, p7, p8);
    }
    template<typename classT>
    void set_function(classT *c, T CGL_DELEGATE_CALL (classT::*func)(T0, T1, T2, T3, T4, T5, T6, T7, T8))
    {
        this->destroy();
        base = new class_delegatorT9<classT, T, T0, T1, T2, T3, T4, T5, T6, T7, T8>(c, func);
    }
    void set_function(T CGL_DELEGATE_CALL (*func)(T0, T1, T2, T3, T4, T5, T6, T7, T8))
    {
        this->destroy();
        base = new delegatorT9<T, T0, T1, T2, T3, T4, T5, T6, T7, T8>(func);
    }
};

//---------------------------------------------------------------------------
// T (*)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9)

template<typename T, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9>
class delegator_base<T (*)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9)>
{
public:
    virtual ~delegator_base(){ /*void*/ }
    virtual T invoke(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9)=0;
    virtual delegator_base* clone()const=0;
};

template<typename classT, typename T, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9>
class class_delegatorT10: public delegator_base<T (*)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9)>
{
public:
    typedef T CGL_DELEGATE_CALL (classT::*function_type)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9);
    classT *_C;
    function_type _F;
    class_delegatorT10(classT *c, function_type func):_C(c), _F(func){/*void*/}
    T invoke(T0 p0, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8, T9 p9)
    {
        return (_C->*_F)(p0, p1, p2, p3, p4, p5, p6, p7, p8, p9);
    }
    delegator_base<T (*)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9)>* clone()const
    {
        return new class_delegatorT10(*this);
    }
};

template<typename T, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9>
class delegatorT10 : public delegator_base<T (*)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9)>
{
public:
    typedef T CGL_DELEGATE_CALL (*function_type)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9);
    function_type _F;
    delegatorT10(function_type func):_F(func){/*void*/}
    T invoke(T0 p0, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8, T9 p9)
    {
        return (*_F)(p0, p1, p2, p3, p4, p5, p6, p7, p8, p9);
    }
    delegator_base<T (*)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9)>* clone()const
    {
        return new delegatorT10(*this);
    }
};

template<typename T, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9>
class delegate<T (*)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9)>
{
public:
    typedef T (*function_type)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9);
    typedef delegate<function_type> this_type;
    delegator_base<function_type> *base;
    delegate():base(0){/*void*/};
    delegate(const this_type& d):base(d.base_clone()){/*void*/}
    ~delegate() { this->destroy(); }
    operator bool()const { return base; }
    void destroy() { if(base)delete base, base = 0; }
    delegator_base<function_type>* base_clone()const { if(base)return base->clone(); else return 0; }
    const this_type& operator=(const this_type& d)
    {
        this->destroy();
        base = d.base_clone();
        return *this;
    }
    T operator()(T0 p0, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8, T9 p9)
    {
        return base->invoke(p0, p1, p2, p3, p4, p5, p6, p7, p8, p9);
    }
    template<typename classT>
    void set_function(classT *c, T CGL_DELEGATE_CALL (classT::*func)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9))
    {
        this->destroy();
        base = new class_delegatorT10<classT, T, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9>(c, func);
    }
    void set_function(T CGL_DELEGATE_CALL (*func)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9))
    {
        this->destroy();
        base = new delegatorT10<T, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9>(func);
    }
};

//---------------------------------------------------------------------------
// T (*)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10)

template<typename T, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10>
class delegator_base<T (*)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10)>
{
public:
    virtual ~delegator_base(){ /*void*/ }
    virtual T invoke(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10)=0;
    virtual delegator_base* clone()const=0;
};

template<typename classT, typename T, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10>
class class_delegatorT11: public delegator_base<T (*)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10)>
{
public:
    typedef T CGL_DELEGATE_CALL (classT::*function_type)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10);
    classT *_C;
    function_type _F;
    class_delegatorT11(classT *c, function_type func):_C(c), _F(func){/*void*/}
    T invoke(T0 p0, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8, T9 p9, T10 p10)
    {
        return (_C->*_F)(p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10);
    }
    delegator_base<T (*)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10)>* clone()const
    {
        return new class_delegatorT11(*this);
    }
};

template<typename T, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10>
class delegatorT11 : public delegator_base<T (*)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10)>
{
public:
    typedef T CGL_DELEGATE_CALL (*function_type)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10);
    function_type _F;
    delegatorT11(function_type func):_F(func){/*void*/}
    T invoke(T0 p0, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8, T9 p9, T10 p10)
    {
        return (*_F)(p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10);
    }
    delegator_base<T (*)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10)>* clone()const
    {
        return new delegatorT11(*this);
    }
};

template<typename T, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10>
class delegate<T (*)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10)>
{
public:
    typedef T (*function_type)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10);
    typedef delegate<function_type> this_type;
    delegator_base<function_type> *base;
    delegate():base(0){/*void*/};
    delegate(const this_type& d):base(d.base_clone()){/*void*/}
    ~delegate() { this->destroy(); }
    operator bool()const { return base; }
    void destroy() { if(base)delete base, base = 0; }
    delegator_base<function_type>* base_clone()const { if(base)return base->clone(); else return 0; }
    const this_type& operator=(const this_type& d)
    {
        this->destroy();
        base = d.base_clone();
        return *this;
    }
    T operator()(T0 p0, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8, T9 p9, T10 p10)
    {
        return base->invoke(p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10);
    }
    template<typename classT>
    void set_function(classT *c, T CGL_DELEGATE_CALL (classT::*func)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10))
    {
        this->destroy();
        base = new class_delegatorT11<classT, T, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10>(c, func);
    }
    void set_function(T CGL_DELEGATE_CALL (*func)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10))
    {
        this->destroy();
        base = new delegatorT11<T, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10>(func);
    }
};

//---------------------------------------------------------------------------
// T (*)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11)

template<typename T, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11>
class delegator_base<T (*)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11)>
{
public:
    virtual ~delegator_base(){ /*void*/ }
    virtual T invoke(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11)=0;
    virtual delegator_base* clone()const=0;
};

template<typename classT, typename T, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11>
class class_delegatorT12: public delegator_base<T (*)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11)>
{
public:
    typedef T CGL_DELEGATE_CALL (classT::*function_type)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11);
    classT *_C;
    function_type _F;
    class_delegatorT12(classT *c, function_type func):_C(c), _F(func){/*void*/}
    T invoke(T0 p0, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8, T9 p9, T10 p10, T11 p11)
    {
        return (_C->*_F)(p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11);
    }
    delegator_base<T (*)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11)>* clone()const
    {
        return new class_delegatorT12(*this);
    }
};

template<typename T, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11>
class delegatorT12 : public delegator_base<T (*)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11)>
{
public:
    typedef T CGL_DELEGATE_CALL (*function_type)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11);
    function_type _F;
    delegatorT12(function_type func):_F(func){/*void*/}
    T invoke(T0 p0, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8, T9 p9, T10 p10, T11 p11)
    {
        return (*_F)(p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11);
    }
    delegator_base<T (*)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11)>* clone()const
    {
        return new delegatorT12(*this);
    }
};

template<typename T, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11>
class delegate<T (*)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11)>
{
public:
    typedef T (*function_type)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11);
    typedef delegate<function_type> this_type;
    delegator_base<function_type> *base;
    delegate():base(0){/*void*/};
    delegate(const this_type& d):base(d.base_clone()){/*void*/}
    ~delegate() { this->destroy(); }
    operator bool()const { return base; }
    void destroy() { if(base)delete base, base = 0; }
    delegator_base<function_type>* base_clone()const { if(base)return base->clone(); else return 0; }
    const this_type& operator=(const this_type& d)
    {
        this->destroy();
        base = d.base_clone();
        return *this;
    }
    T operator()(T0 p0, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8, T9 p9, T10 p10, T11 p11)
    {
        return base->invoke(p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11);
    }
    template<typename classT>
    void set_function(classT *c, T CGL_DELEGATE_CALL (classT::*func)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11))
    {
        this->destroy();
        base = new class_delegatorT12<classT, T, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11>(c, func);
    }
    void set_function(T CGL_DELEGATE_CALL (*func)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11))
    {
        this->destroy();
        base = new delegatorT12<T, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11>(func);
    }
};

//---------------------------------------------------------------------------
// T (*)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12)

template<typename T, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12>
class delegator_base<T (*)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12)>
{
public:
    virtual ~delegator_base(){ /*void*/ }
    virtual T invoke(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12)=0;
    virtual delegator_base* clone()const=0;
};

template<typename classT, typename T, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12>
class class_delegatorT13: public delegator_base<T (*)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12)>
{
public:
    typedef T CGL_DELEGATE_CALL (classT::*function_type)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12);
    classT *_C;
    function_type _F;
    class_delegatorT13(classT *c, function_type func):_C(c), _F(func){/*void*/}
    T invoke(T0 p0, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8, T9 p9, T10 p10, T11 p11, T12 p12)
    {
        return (_C->*_F)(p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12);
    }
    delegator_base<T (*)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12)>* clone()const
    {
        return new class_delegatorT13(*this);
    }
};

template<typename T, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12>
class delegatorT13 : public delegator_base<T (*)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12)>
{
public:
    typedef T CGL_DELEGATE_CALL (*function_type)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12);
    function_type _F;
    delegatorT13(function_type func):_F(func){/*void*/}
    T invoke(T0 p0, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8, T9 p9, T10 p10, T11 p11, T12 p12)
    {
        return (*_F)(p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12);
    }
    delegator_base<T (*)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12)>* clone()const
    {
        return new delegatorT13(*this);
    }
};

template<typename T, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12>
class delegate<T (*)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12)>
{
public:
    typedef T (*function_type)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12);
    typedef delegate<function_type> this_type;
    delegator_base<function_type> *base;
    delegate():base(0){/*void*/};
    delegate(const this_type& d):base(d.base_clone()){/*void*/}
    ~delegate() { this->destroy(); }
    operator bool()const { return base; }
    void destroy() { if(base)delete base, base = 0; }
    delegator_base<function_type>* base_clone()const { if(base)return base->clone(); else return 0; }
    const this_type& operator=(const this_type& d)
    {
        this->destroy();
        base = d.base_clone();
        return *this;
    }
    T operator()(T0 p0, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8, T9 p9, T10 p10, T11 p11, T12 p12)
    {
        return base->invoke(p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12);
    }
    template<typename classT>
    void set_function(classT *c, T CGL_DELEGATE_CALL (classT::*func)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12))
    {
        this->destroy();
        base = new class_delegatorT13<classT, T, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12>(c, func);
    }
    void set_function(T CGL_DELEGATE_CALL (*func)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12))
    {
        this->destroy();
        base = new delegatorT13<T, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12>(func);
    }
};

//---------------------------------------------------------------------------
// T (*)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13)

template<typename T, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13>
class delegator_base<T (*)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13)>
{
public:
    virtual ~delegator_base(){ /*void*/ }
    virtual T invoke(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13)=0;
    virtual delegator_base* clone()const=0;
};

template<typename classT, typename T, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13>
class class_delegatorT14: public delegator_base<T (*)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13)>
{
public:
    typedef T CGL_DELEGATE_CALL (classT::*function_type)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13);
    classT *_C;
    function_type _F;
    class_delegatorT14(classT *c, function_type func):_C(c), _F(func){/*void*/}
    T invoke(T0 p0, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8, T9 p9, T10 p10, T11 p11, T12 p12, T13 p13)
    {
        return (_C->*_F)(p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13);
    }
    delegator_base<T (*)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13)>* clone()const
    {
        return new class_delegatorT14(*this);
    }
};

template<typename T, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13>
class delegatorT14 : public delegator_base<T (*)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13)>
{
public:
    typedef T CGL_DELEGATE_CALL (*function_type)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13);
    function_type _F;
    delegatorT14(function_type func):_F(func){/*void*/}
    T invoke(T0 p0, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8, T9 p9, T10 p10, T11 p11, T12 p12, T13 p13)
    {
        return (*_F)(p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13);
    }
    delegator_base<T (*)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13)>* clone()const
    {
        return new delegatorT14(*this);
    }
};

template<typename T, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13>
class delegate<T (*)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13)>
{
public:
    typedef T (*function_type)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13);
    typedef delegate<function_type> this_type;
    delegator_base<function_type> *base;
    delegate():base(0){/*void*/};
    delegate(const this_type& d):base(d.base_clone()){/*void*/}
    ~delegate() { this->destroy(); }
    operator bool()const { return base; }
    void destroy() { if(base)delete base, base = 0; }
    delegator_base<function_type>* base_clone()const { if(base)return base->clone(); else return 0; }
    const this_type& operator=(const this_type& d)
    {
        this->destroy();
        base = d.base_clone();
        return *this;
    }
    T operator()(T0 p0, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8, T9 p9, T10 p10, T11 p11, T12 p12, T13 p13)
    {
        return base->invoke(p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13);
    }
    template<typename classT>
    void set_function(classT *c, T CGL_DELEGATE_CALL (classT::*func)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13))
    {
        this->destroy();
        base = new class_delegatorT14<classT, T, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13>(c, func);
    }
    void set_function(T CGL_DELEGATE_CALL (*func)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13))
    {
        this->destroy();
        base = new delegatorT14<T, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13>(func);
    }
};

//---------------------------------------------------------------------------
// T (*)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14)

template<typename T, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13, typename T14>
class delegator_base<T (*)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14)>
{
public:
    virtual ~delegator_base(){ /*void*/ }
    virtual T invoke(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14)=0;
    virtual delegator_base* clone()const=0;
};

template<typename classT, typename T, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13, typename T14>
class class_delegatorT15: public delegator_base<T (*)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14)>
{
public:
    typedef T CGL_DELEGATE_CALL (classT::*function_type)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14);
    classT *_C;
    function_type _F;
    class_delegatorT15(classT *c, function_type func):_C(c), _F(func){/*void*/}
    T invoke(T0 p0, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8, T9 p9, T10 p10, T11 p11, T12 p12, T13 p13, T14 p14)
    {
        return (_C->*_F)(p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14);
    }
    delegator_base<T (*)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14)>* clone()const
    {
        return new class_delegatorT15(*this);
    }
};

template<typename T, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13, typename T14>
class delegatorT15 : public delegator_base<T (*)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14)>
{
public:
    typedef T CGL_DELEGATE_CALL (*function_type)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14);
    function_type _F;
    delegatorT15(function_type func):_F(func){/*void*/}
    T invoke(T0 p0, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8, T9 p9, T10 p10, T11 p11, T12 p12, T13 p13, T14 p14)
    {
        return (*_F)(p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14);
    }
    delegator_base<T (*)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14)>* clone()const
    {
        return new delegatorT15(*this);
    }
};

template<typename T, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13, typename T14>
class delegate<T (*)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14)>
{
public:
    typedef T (*function_type)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14);
    typedef delegate<function_type> this_type;
    delegator_base<function_type> *base;
    delegate():base(0){/*void*/};
    delegate(const this_type& d):base(d.base_clone()){/*void*/}
    ~delegate() { this->destroy(); }
    operator bool()const { return base; }
    void destroy() { if(base)delete base, base = 0; }
    delegator_base<function_type>* base_clone()const { if(base)return base->clone(); else return 0; }
    const this_type& operator=(const this_type& d)
    {
        this->destroy();
        base = d.base_clone();
        return *this;
    }
    T operator()(T0 p0, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8, T9 p9, T10 p10, T11 p11, T12 p12, T13 p13, T14 p14)
    {
        return base->invoke(p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14);
    }
    template<typename classT>
    void set_function(classT *c, T CGL_DELEGATE_CALL (classT::*func)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14))
    {
        this->destroy();
        base = new class_delegatorT15<classT, T, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14>(c, func);
    }
    void set_function(T CGL_DELEGATE_CALL (*func)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14))
    {
        this->destroy();
        base = new delegatorT15<T, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14>(func);
    }
};

//---------------------------------------------------------------------------
// T (*)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15)

template<typename T, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13, typename T14, typename T15>
class delegator_base<T (*)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15)>
{
public:
    virtual ~delegator_base(){ /*void*/ }
    virtual T invoke(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15)=0;
    virtual delegator_base* clone()const=0;
};

template<typename classT, typename T, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13, typename T14, typename T15>
class class_delegatorT16: public delegator_base<T (*)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15)>
{
public:
    typedef T CGL_DELEGATE_CALL (classT::*function_type)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15);
    classT *_C;
    function_type _F;
    class_delegatorT16(classT *c, function_type func):_C(c), _F(func){/*void*/}
    T invoke(T0 p0, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8, T9 p9, T10 p10, T11 p11, T12 p12, T13 p13, T14 p14, T15 p15)
    {
        return (_C->*_F)(p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15);
    }
    delegator_base<T (*)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15)>* clone()const
    {
        return new class_delegatorT16(*this);
    }
};

template<typename T, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13, typename T14, typename T15>
class delegatorT16 : public delegator_base<T (*)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15)>
{
public:
    typedef T CGL_DELEGATE_CALL (*function_type)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15);
    function_type _F;
    delegatorT16(function_type func):_F(func){/*void*/}
    T invoke(T0 p0, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8, T9 p9, T10 p10, T11 p11, T12 p12, T13 p13, T14 p14, T15 p15)
    {
        return (*_F)(p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15);
    }
    delegator_base<T (*)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15)>* clone()const
    {
        return new delegatorT16(*this);
    }
};

template<typename T, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13, typename T14, typename T15>
class delegate<T (*)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15)>
{
public:
    typedef T (*function_type)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15);
    typedef delegate<function_type> this_type;
    delegator_base<function_type> *base;
    delegate():base(0){/*void*/};
    delegate(const this_type& d):base(d.base_clone()){/*void*/}
    ~delegate() { this->destroy(); }
    operator bool()const { return base; }
    void destroy() { if(base)delete base, base = 0; }
    delegator_base<function_type>* base_clone()const { if(base)return base->clone(); else return 0; }
    const this_type& operator=(const this_type& d)
    {
        this->destroy();
        base = d.base_clone();
        return *this;
    }
    T operator()(T0 p0, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8, T9 p9, T10 p10, T11 p11, T12 p12, T13 p13, T14 p14, T15 p15)
    {
        return base->invoke(p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15);
    }
    template<typename classT>
    void set_function(classT *c, T CGL_DELEGATE_CALL (classT::*func)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15))
    {
        this->destroy();
        base = new class_delegatorT16<classT, T, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15>(c, func);
    }
    void set_function(T CGL_DELEGATE_CALL (*func)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15))
    {
        this->destroy();
        base = new delegatorT16<T, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15>(func);
    }
};

} //end namespace cgl

#endif // DELEGATE_HPP_20170215034129
