/*
 Copyright (c) 2005-2020 sdragonx (mail:sdragonx@foxmail.com)

 dual_map.hpp

 2017-02-09 08:56:27

 使用双重key，两个key都必须是唯一的。支持数组遍历

*/
#ifndef DUAL_MAP_HPP_20170209085627
#define DUAL_MAP_HPP_20170209085627

#include <map>
#include <vector>

namespace cgl{

template<typename ID, typename KEY, typename T>
class dual_map
{
public:
    typedef ID first_key;
    typedef KEY second_key;
    typedef T value_type;

    typedef std::map<first_key, value_type> container_type;
    typedef container_type::iterator iterator;
    typedef container_type::const_iterator const_iterator;

    const static size_t invalid_index = SIZE_MAX;

private:
    typedef std::map<second_key, first_key> keymap_type;
    typedef keymap_type::iterator key_iterator;
    container_type m_map;
    keymap_type m_keymap;
    std::vector<iterator> m_ls;

public:
    dual_map() : m_map(), m_keymap(), m_ls()
    {

    }

    size_t size()const
    {
        return m_map.size();
    }

    iterator begin()
    {
        return m_map.begin();
    }

    const_iterator begin()const
    {
        return m_map.begin();
    }

    iterator end()
    {
        return m_map.end();
    }

    const_iterator end()const
    {
        return m_map.end();
    }

    value_type& operator[](const first_key& key)
    {
        iterator itr = m_map.find(key);
        return itr->second;
    }

    value_type& operator[](const second_key& key)
    {
        key_iterator itr = m_keymap.find(key);
        return itr->second->second;
    }

    value_type& operator[](size_t index)
    {
        return m_ls[index]->second;
    }

    value_type* find(const first_key& key)
    {
        iterator itr = m_map.find(key);
        if(itr != m_map.end()){
            return &itr->second;
        }
        return null;
    }

    value_type* find(const second_key& key)
    {
        key_iterator itr = m_keymap.find(key);

        if(itr != m_keymap.end()){
            return itr->second->second;
        }
        return null;
    }

    //两个Key必须都不存在才能插入
    int insert(const first_key& key1, const second_key& key2, const value_type& value)
    {
        iterator itr1 = m_map.find(key1);
        key_iterator itr2 = m_keymap.find(key2);

        if(itr1 == m_map.end() && itr2 == m_keymap.end()){
            itr1 = m_map.insert(std::make_pair(key1, value)).first;
            m_keymap[key2] = key1;
            m_ls.push_back(itr1);
            return 0;
        }
        else{
            return -1;
        }
    }

    void erase(size_t index)
    {
        if(index < m_ls.size()){
            iterator itr = m_ls[index];
            m_map.erase(itr);
            m_keymap.erase(find_all(m_keymap, itr->first));
            m_ls.erase(m_ls.begin() + index);
        }
    }
};

}//end namespace cgl

#endif //DUAL_MAP_HPP_20170209085627

