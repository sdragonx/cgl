/*
 Copyright (c) 2005-2020 sdragonx (mail:sdragonx@foxmail.com)

 inifile.h

 2015-02-15 02:14:49

*/
#ifndef INIFILE_H_20150215021449
#define INIFILE_H_20150215021449

#include <fstream>
#include <cgl/string.hpp>

namespace cgl{

template<typename _char_t>
class inifile
{
public:
    //typedef cstring<_char_t> string_type;
    typedef std::string string_type;

    struct item_type
    {
        string_type name;
        string_type value;

        item_type() : name(), value() { /*void*/ }
        item_type(const string_type& n, const string_type& v) : name(n), value(v) { /*void*/ }
    };

    class section_type
    {
    public:
        friend class inifile;
        string_type name;
        std::vector<item_type> items;

    public:
        item_type& operator[](size_t index)
        {
            return items[index];
        }

        const item_type& operator[](size_t index)const
        {
            return items[index];
        }

        void add(const item_type& item)
        {
            items.push_back(item);
        }

        void add(const string_type& name, const string_type& value)
        {
            this->add(item_type(name, value));
        }

        size_t find(const string_type& name)
        {
            return find_all(items, name);
        }
    };

//    typedef std::vector<SECTION> sections;
//    typedef inilines::iterator  iterator;
//    typedef inilines::const_iterator  const_iterator;

    typedef std::vector<string_type> stringlist;

//private:
    string_type filename;
    std::vector<section_type> sections;

public:
    int open(const string_type& file)
    {
        std::basic_ifstream<_char_t> fs;
        string_type str;

        this->close();

        fs.open(file.c_str());
        if(!fs.is_open()){
            return -1;
        }
        filename = file;
        while(!fs.eof()){
            std::getline(fs, str);
            parse_line(str);
        }
        return 0;
    }

    int save(const string_type& file)
    {
        std::basic_ofstream<_char_t> fs;
        fs.open(file.c_str());
        if(!fs.is_open()){
            return -1;
        }
        for(iterator itr = lines.begin(); itr!=lines.end(); ++itr){
            switch(itr->type){
            case INI_NULL:
                fs<<std::endl;
                break;
            case INI_SECTION:
                //if(itr!=lines.begin())fs<<std::endl;//加空行
                fs<<'['<<itr->name.c_str()<<']'<<std::endl;
                break;
            case INI_VARIABLE:
                fs<<itr->name.c_str()<<'='<<itr->value.c_str()<<std::endl;
                break;
            case INI_COMMENT:
                fs<<itr->value.c_str()<<std::endl;
                break;
            default:
                break;
            }
        }
        return 0;
    }

    void close()
    {
        filename.clear();
        sections.clear();
        sections.resize(1);
    }

private:
    int parse_line(const string_type& line)
    {
        item_type item;

        string_type::const_iterator begin = line.begin();
        string_type::const_iterator end = line.end();

        begin = skip_space(begin, end);
        if(begin == end){
            sections.back().add(item);
            return 0;
        }

        if(*begin == '['){//小节
            for( ++begin; begin < end; ++begin){
                if(!isspace(*begin) && *begin != ']'){
                    item.name.push_back(*begin);
                }
                else{
                    break;
                }
            }
            begin = skip_space(begin, end);
            if(*begin == ']'){
                section_type s;
                s.name = item.name;
                sections.push_back(s);
                //sections.back().name == "abcd";//item.name;
            }
        }
        else if(*begin == ';' || *begin == '#'){//注释
            item.name = '#';
            item.value = string_type(begin, end);
            sections.back().add(item);
        }
        else{
            for( ; begin < end; ++begin){
                if(!isspace(*begin) && *begin != '='){
                    item.name.push_back(*begin);
                }
            }
            begin = skip_space(begin, end);
            if(*begin == '='){
                ++begin;
                begin = skip_space(begin, end);
                for( ; begin < end; ++begin){
                    item.value.push_back(*begin);
                }
            }
            sections.back().add(item);
        }

        return 0;
    }
};

}//end namespace cgl

#endif //INIFILE_H_20150215021449
