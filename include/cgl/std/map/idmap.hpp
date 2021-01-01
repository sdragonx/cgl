/*
 Copyright (c) 2005-2020 sdragonx (mail:sdragonx@foxmail.com)

 idmap.hpp

 2018-01-09 23:33:43

 using int for random index, 0 is invalid index. idmap[0] is invalid value.

 2018/10/20 8:46

 std::RAND_MAX == int16¾¯¸æ

*/
#ifndef IDMAP_HPP_20180109233343
#define IDMAP_HPP_20180109233343

#include <cgl/public.h>
#include <cgl/math/random.hpp>

#include <map>

namespace cgl{

template<typename T>
class idmap
{
public:
    typedef int key_type;
    typedef T value_type;

    typedef TYPENAME std::map<key_type, value_type> container_type;
    typedef TYPENAME container_type::iterator iterator;
    typedef TYPENAME container_type::const_iterator const_iterator;

private:
    container_type m_map;
    static value_type m_none;

public:
    idmap() : m_map() { /*void*/ }
    ~idmap() { /*void*/ }

    key_type allocate()
    {
        int index;
        do{
            index = 1 + math::random<int>(RAND_MAX);
        }while(this->exists(index));
        m_map[index] = value_type();
        return index;
    }

    size_t size()const
    {
        return m_map.size();
    }

    void clear()
    {
        m_map.clear();
    }

    iterator begin()            { return m_map.begin(); }
    const_iterator begin()const { return m_map.begin(); }
    iterator end()              { return m_map.end();   }
    const_iterator end()const   { return m_map.end();   }

    bool exists(int index)const
    {
        return  m_map.find(index) != m_map.end();
    }

    value_type& operator[](int index)
    {
        iterator itr = m_map.find(index);
        if(itr != m_map.end()){
            return itr->second;
        }
        else{
            return m_none;
        }
    }

    const value_type& operator[](int index)const
    {
        iterator itr = m_map.find(index);
        if(itr != m_map.end()){
            return itr->second;
        }
        else{
            return m_none;
        }
    }

    void erase(int index)
    {
        if(this->exists(index)){
            m_map.erase(index);
        }
    }
};

template<typename T>
T idmap<T>::m_none = T();

}//end namespace cgl

#endif //IDMAP_HPP_20180109233343

