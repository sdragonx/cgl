/*
 Copyright (c) 2005-2020 sdragonx (mail:sdragonx@foxmail.com)

 envlist

 2018-07-16 00:09:37

*/
#ifndef ENVLIST_20180716000937
#define ENVLIST_20180716000937

#include <cgl/public.h>

namespace cgl{

template<typename _char_t>
class env_list
{
public:
    typedef cstring<_char_t> string_type;
private:
    std::map<string_type, string_type> m_env;
    string_type m_null;
public:
    int set(const string_type& name, const string_type& value)
    {
        if(value.empty()){
            m_env.erase(name);
        }
        else{
            m_env[name] = value;
        }
        return 0;
    }

    const string_type& value(const string_type& name)
    {
        TYPENAME std::map<string_type, string_type>::iterator itr =
            m_env.find(name);
        if(itr != m_env.end()){
            return itr->second;
        }
        else{
            return m_null;
        }
    }

    const string_type& operator[](const string_type& name)
    {
        return this->value(name);
    }
};

}//end namespace cgl

#endif //ENVLIST_20180716000937
