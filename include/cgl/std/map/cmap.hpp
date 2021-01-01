/*
 Copyright (c) 2005-2020 sdragonx (mail:sdragonx@foxmail.com)

 cmap.hpp

 2017-02-09 08:57:38

 使用一个Key，并可以索引查询

*/
#ifndef CMAP_HPP_20170209085738
#define CMAP_HPP_20170209085738

#include <map>
#include <vector>

namespace cgl{

template<typename KEY, typename T>
class cmap
{
public:
    typedef KEY key_type;
    typedef T value_type;
    typedef T* pointer;
    typedef std::map<key_type, value_type> container_type;
    typedef container_type::iterator iterator;
    typedef container_type::const_iterator const_iterator;

private:
    typedef std::vector<iterator> list_type;
    typedef list_type::iterator list_iterator;

    container_type m_map;
    list_type m_ls;

public:
    cmap() : m_map(), m_ls()
    {

    }

    bool empty()const
    {
        return m_map.empty();
    }

    size_t size()const
    {
        return m_map.size();
    }

    void reserve(size_t n)
    {
        m_map.reserve(n);
    }

    void resize(size_t n)
    {
        m_map.resize(n);
    }

    value_type& operator[](size_t index)
    {
        return m_map[m_ls[index]];
    }

    const value_type& operator[](size_t index)const
    {
        return m_map[m_ls[index]];
    }

    value_type& operator[](const key_type& key)
    {
        iterator itr = m_map.find(key);
        return itr->second;
    }

    const value_type& operator[](const key_type& key)const
    {
        const_iterator itr = m_map.find(key);
        return itr->second;
    }

    void clear()
    {
        m_map.clear();
        m_ls.clear();
    }

    bool exists(const key_type& key)const
    {
        return m_map.find(key) != m_map.end();
    }

    size_t indexof(const key_type& key)const
    {
        list_type::const_iterator itr = std::find(m_ls.begin(), m_ls.end(), m_map.find(key));
        if(itr != m_ls.end()){
            return itr - m_ls.begin();
        }
        return SIZE_MAX;
    }

    const key_type& keyof(int index)const
    {
        return m_ls[index]->first;
    }

    value_type* find(const key_type& key)
    {
        iterator itr = m_map.find(key);
        if(itr != m_map.end()){
            return &itr->second;
        }
        return null;
    }

    int insert(const key_type& key, const value_type& value)
    {
        iterator itr = m_map.find(key);
        //motify existed value
        if(itr != m_map.end()){
            itr->second = value;
        }
        else{
            //add new
            itr = m_map.insert(std::make_pair(key, value)).first;
            m_ls.push_back(itr);
        }
        return 0;
    }

    void erase(const key_type& key)
    {
        iterator itr = m_map.find(key);
        if(itr != m_map.end()){
            m_map.erase(itr);
            m_ls.erase(find_all(m_ls, itr));
        }
    }
};

}//end namespace cgl

#endif //CMAP_HPP_20170209085738