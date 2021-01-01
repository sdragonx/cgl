/*
 Copyright (c) 2005-2020 sdragonx (mail:sdragonx@foxmail.com)

 hashmap.hpp

 2018-08-09 14:51:42

 0 is invalid hashvalue, hashmap[0] is invalid value;

 2018/10/20 10:46

*/
#ifndef HASHMAP_HPP_20180809145142
#define HASHMAP_HPP_20180809145142

#include <cgl/public.h>
#include <cgl/algorithm.hpp>
#include <cgl/string.hpp>
#include <cgl/hash.hpp>

#ifndef CGL_HASHMAP_SEED
#define CGL_HASHMAP_SEED 0
#endif

namespace cgl{

//hash map + array
template<typename T>
class hashmap
{
public:
    typedef uint64_t key_type;
    typedef T value_type;
    typedef char char_type;
    typedef cstring<char_type> string_type;
    typedef std::map<key_type, value_type> container_type;
    typedef TYPENAME container_type::iterator iterator;
    typedef TYPENAME container_type::const_iterator const_iterator;

private:
    typedef std::vector<iterator> list_type;
    typedef TYPENAME list_type::iterator list_iterator;
    container_type m_map;
    list_type m_ls;

public:
    hashmap():m_map(), m_ls()
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

    iterator begin(){ return m_map.begin(); }
    iterator end(){ return m_map.end(); }
    const_iterator begin()const { return m_map.begin(); }
    const_iterator end()const { return m_map.end(); }

    void clear()
    {
        m_map.clear();
        m_ls.clear();
    }

    //get a hash key
    static key_type hash(const string_type& str, int seed = CGL_HASHMAP_SEED)
    {
        return hash::mmhash64(str.c_str(), str.length() * sizeof(char_type), seed);
    }

    static key_type hash(const char_type* str, size_t length, int seed = CGL_HASHMAP_SEED)
    {
        return hash::mmhash64(str, length * sizeof(char_type), seed);
    }

    //sequential traversal 
    value_type& operator[](size_t id)
    {
        return m_ls[id].second;
    }

    const value_type& operator[](size_t id)const
    {
        return m_ls[id]->second;
    }

    bool exists(const key_type& key)const
    {
        return m_map.find(key) != m_map.end();
    }

    bool exists(const string_type& str)const
    {
        return this->exists(this->hash(str));
    }

    value_type* find(const key_type& key)
    {
        iterator itr = m_map.find(key);
        if(itr != m_map.end()){
            return &itr->second;
        }
        return null;
    }

    value_type* find(const string_type& name)
    {
        return this->find(this->hash(name));
    }

    int insert(const key_type& key, const value_type& value)
    {
        iterator itr = m_map.find(key);
        if(itr != m_map.end()){
            itr->second = value;
        }
        else{
            itr = m_map.insert(std::make_pair(key, value)).first;
            m_ls.push_back(itr);
        }
        return 0;
    }

    int insert(const string_type& name, const value_type& value)
    {
        key_type id = this->hash(name);
        return this->insert(id, value);
    }

    void erase(key_type key)
    {
        iterator itr = m_map.find(key);
        if(itr != m_map.end()){
            m_map.erase(itr);
            m_ls.erase(find_all(m_ls, itr));
        }
    }

    void erase(const string_type& name)
    {
        this->erase(this->hash(name));
    }

};

}//end namespace cgl

#endif //HASHMAP_HPP_20180809145142
