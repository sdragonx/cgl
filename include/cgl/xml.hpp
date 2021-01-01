/*
 Copyright (c) 2005-2020 sdragonx (mail:sdragonx@foxmail.com)

 xml.h

 2016-06-28 11:01:17

 rapidxml库的使用是有陷阱的，rapidxml不主动去申请字符串。
 下面的二次封装，name必须是字符串常量

*/
#ifndef XML_H_20160628110117
#define XML_H_20160628110117

#pragma once
#define RAPIDXML_NO_STREAMS
//#define RAPIDXML_NO_EXCEPTIONS

#include <cstdio>

#include <cgl/external/rapidxml/rapidxml.hpp>
//#include <rapidxml/rapidxml_utils.hpp>
#include <cgl/external/rapidxml/rapidxml_print.hpp>

#include <cgl/public.h>
#include <cgl/io.hpp>
#include <cgl/string.hpp>
#include <cgl/vec.hpp>
#include <cgl/vec_utils.hpp>

namespace cgl{

//extern void show_message(const string_t& text);

//读取属性值
template<typename T>
const T* xml_readstring(const rapidxml::xml_node<T>* node, const T* name,
    const T* defvalue = null)
{
    rapidxml::xml_attribute<T>* attrib = null;
    if(node)
    {
        attrib = node->first_attribute(name);
        if(attrib)
        {
            return attrib->value();
        }
    }
    return defvalue;
}

template<typename T, typename C>
T xml_readvalue(const rapidxml::xml_node<C>* node, const C* name, const T& defvalue = T())
{
    rapidxml::xml_attribute<C>* attrib = null;
    if(node)
    {
        attrib = node->first_attribute(name);
        if(attrib)
        {
            return string_cast<T>(attrib->value());
        }
    }
    return defvalue;
}

/*
vec2s xml_readvec2s(rapidxml::xml_node<>* node, const char* attrib_name)
{
    vec2s v = vec2s();
    if(node)
    {
        rapidxml::xml_attribute<>* attrib = node->first_attribute(attrib_name);
        if(attrib)
        {
            sscanf(attrib->value(), "%h,%h", &v.x, &v.y);
        }
    }
    return v;
}
*/

//创建node attribute
template<typename T, typename C>
T xml_node_value(const rapidxml::xml_node<C>* node, const T& defvalue = T())
{
    if(node && node->value())
    {
        return string_cast<T>(node->value());
    }
    return defvalue;
}


template<typename T>
rapidxml::xml_node<T>* xml_append_node(rapidxml::xml_node<T> *node,
    const T* name, const T* value = null)
{
    rapidxml::xml_node<T>* subnode = node->document()->allocate_node(
        rapidxml::node_element,
        name,
        value ? node->document()->allocate_string(value) : value);
    node->append_node(subnode);
    return subnode;
}

template<typename T>
rapidxml::xml_attribute<T>* xml_append_attrib(rapidxml::xml_node<T> *node, const T* name, const T* value = null)
{
    rapidxml::xml_attribute<T>* attrib =
        node->document()->allocate_attribute(
        name,
        value ? node->document()->allocate_string(value) : value);
    node->append_attribute(attrib);
    return attrib;
}

//一个写文件器
class xml_writer
{
public:
    std::FILE* io;

    int open(const char* filename)
    {
        io = fopen(filename, "w");
        return 0;
    }

    int open(const wchar_t* filename)
    {
        io = _wfopen(filename, L"w");
        return 0;
    }

    void close()
    {
        if(io)fclose(io), io = null;
    }

    xml_writer& operator=(char ch)
    {
        fputc(ch, io);
        return *this;
    }

    xml_writer& operator*()
    {
        return *this;
    }

    xml_writer& operator++()
    {
        return *this;
    }

    xml_writer& operator++(int)
    {
        return *this;
    }
};


template<typename _char_t>
class xml_attribute
{
public:
    typedef _char_t char_type;
    typedef xml_attribute this_type;

private:
    rapidxml::xml_attribute<char_type>* m_attrib;

public:
    xml_attribute():m_attrib(null) { /*void*/ }
    xml_attribute(rapidxml::xml_attribute<char_type>* attrib):m_attrib(attrib) { /*void*/ }

    bool is_null()const { return !m_attrib; }
    operator bool()const { return m_attrib; }

    const char_type* name()const
    {
        return m_attrib ? m_attrib->name() : 0;
    }

    const char_type* value(const char_type* default_value = null)const
    {
        if(!this->is_null())
        {
            return m_attrib->value();
        }
        else{
            return default_value;
        }
    }

    void set_value(const char_type* value)
    {
        if(!this->is_null()){
            m_attrib->value(m_attrib->document()->allocate_string(value));
        }
    }

    bool to_bool(bool default_value = false)const
    {
        if(!this->is_null()){
            return string_cast<bool>(m_attrib->value());
        }
        else{
            return default_value;
        }
    }

    int to_int(int default_value = 0)const
    {
        if(!this->is_null()){
            return string_cast<int>(m_attrib->value());
        }
        else{
            return default_value;
        }
    }

    float to_float(float default_value = 0.0f)const
    {
        if(!this->is_null()){
            return cstof(m_attrib->value());
        }
        else{
            return default_value;
        }
    }

    vec2i to_vec2i(int x = 0, int y = 0)const
    {
        if(!this->is_null()){
            return to_vec2<int>(m_attrib->value());
        }
        else{
            return vec2i(x, y);
        }
    }

    vec2f to_vec2f(float x = 0.0f, float y = 0.0f)const
    {
        if(!this->is_null()){
            return to_vec2<float>(m_attrib->value());
        }
        else{
            return vec2f(x, y);
        }
    }

    this_type next(const char* name = NULL)const
    {
        return m_attrib ? m_attrib->next_attribute(name) : NULL;
    }
};

template<typename _char_t>
class xml_node
{
public:
    typedef _char_t char_type;
    typedef xml_node<char_type> this_type;
    typedef xml_attribute<char_type> attribute_type;

private:
    rapidxml::xml_node<char_type>* m_node;

public:
    xml_node():m_node(null) { /*void*/ }
    xml_node(rapidxml::xml_node<char_type>* node):m_node(node) { /*void*/ }

    bool is_null()const { return !m_node; }
    operator bool()const { return m_node; }

    const char_type* name()const
    {
        return is_null() ? 0 : m_node->name();
    }

    const char_type* value(const char_type* default_value = null)const
    {
        return is_null() ? default_value : m_node->value();
    }

    void set_value(const char_type* value)
    {
        if(m_node){
            m_node->value(m_node->document()->allocate_string(value));
        }
    }

    bool to_bool(bool default_value = false)const
    {
        if(!this->is_null()){
            return string_cast<bool>(m_node->value());
        }
        else{
            return default_value;
        }
    }

    int to_int(int default_value = 0)const
    {
        if(!this->is_null()){
            return string_cast<int>(m_node->value());
        }
        else{
            return default_value;
        }
    }

    float to_float(float default_value = 0.0f)const
    {
        if(!this->is_null()){
            return string_cast<float>(m_node->value());
        }
        else{
            return default_value;
        }
    }

    vec2i to_vec2i(int x = 0, int y = 0)const
    {
        if(!this->is_null()){
            return to_vec2i(m_node->value());
        }
        else{
            return vec2i(x, y);
        }
    }

    vec2f to_vec2f(float x = 0.0f, float y = 0.0f)const
    {
        if(!this->is_null()){
            return to_vec2f(m_node->value());
        }
        else{
            return vec2f(x, y);
        }
    }

    //
    // attribute
    //

    xml_attribute<char_type> attribute(const char_type* name)
    {
        return m_node ? attribute_type(m_node->first_attribute(name)) : attribute_type();
    }

    const xml_attribute<char_type> attribute(const char_type* name)const
    {
        return m_node ? attribute_type(m_node->first_attribute(name)) : attribute_type();
    }

    xml_attribute<char_type> set_attribute(const char_type* name, const char_type* value = null)
    {
        if(m_node){
            xml_attribute<char_type> attrib = this->attribute(name);
            if(attrib){
                attrib.set_value(value);
                return attrib;
            }
            else{
                rapidxml::xml_attribute<char_type>* pa = this->allocate_attribute(name, value);
                if(pa){
                    m_node->append_attribute(pa);
                    return xml_attribute<char_type>(pa);
                }
            }
        }
        return xml_attribute<char_type>();
    }

    xml_attribute<char_type> set_attribute(const char_type* name, int value)
    {
        char buf[32];
        tostr(buf, 32, value, 10);
        return this->set_attribute(name, buf);
    }

    xml_attribute<char_type> set_attribute(const char_type* name, float value)
    {
        char buf[32];
        format_float(buf, 32, value);
        return this->set_attribute(name, buf);
    }

    xml_attribute<char_type> set_attribute(const char_type* name, bool value)
    {
        return this->set_attribute(name, value ? T_TEXT(char_type, "true") : T_TEXT(char_type, "false"));
    }

    xml_attribute<char_type> set_attribute(const char_type* name, vec2i value)
    {
        char_type buf[32];
        tostr<char_type>(buf, 32, value, 10);
        return this->set_attribute(name, buf);
    }

    xml_attribute<char_type> set_attribute(const char_type* name, vec2f value)
    {
        char buf[64];
        vec2_to<char>(buf, 64, value);
        return this->set_attribute(name, buf);
    }
//node
    this_type subnode(const char_type* name = null)
    {
        return m_node ? this_type(m_node->first_node(name)) : this_type();
    }

    const this_type subnode(const char_type* name = null)const
    {
        return m_node ? this_type(m_node->first_node(name)) : this_type();
    }

    this_type push_front(const char_type* name, const char_type* value = null)
    {
        if(m_node){
            rapidxml::xml_node<char_type>* node = this->allocate_node(name, value);
            if(node){
                m_node->prepend_node(node);
                return this_type(node);
            }
        }
        return xml_node<char_type>();
    }

    this_type push_back(const char_type* name, const char_type* value = null)
    {
        if(m_node){
            rapidxml::xml_node<char_type>* node = this->allocate_node(name, value);
            if(node){
                m_node->append_node(node);
                return xml_node<char_type>(node);
            }
        }
        return xml_node<char_type>();
    }

    this_type prev(const char_type* name = null)const
    {
        return is_null() ? this_type() : this_type(m_node->previous_sibling(name));
    }

    this_type next(const char_type* name = null)const
    {
        return is_null() ? this_type() : this_type(m_node->next_sibling(name));
    }

//type
    int type()const
    {
        return is_null() ? m_node->type() : 0;
    }

    bool is_element()const
    {
        return this->type() == rapidxml::node_element;
    }

    bool is_comment()const
    {
        return this->type() == rapidxml::node_comment;
    }

//count
    size_t count(const char* node_name)const
    {
        size_t n = 0;
        xml_node<char_type> node = this->subnode(node_name);
        while(node){
            ++n;
            node = node.next(node_name);
        }
        return n;
    }
private:
    rapidxml::xml_node<char_type>* allocate_node(const char_type* name, const char_type* value)
    {
        return m_node->document()->allocate_node(
            rapidxml::node_element,
            name,//m_node->document()->allocate_string(name),
            value ? m_node->document()->allocate_string(value) : value);
    }
    rapidxml::xml_attribute<char_type>* allocate_attribute(const char_type* name, const char_type* value)
    {
        return m_node->document()->allocate_attribute(
            name,//m_node->document()->allocate_string(name),
            value ? m_node->document()->allocate_string(value) : value);
    }
};

template<typename _char_t>
class xml_document
{
public:
    typedef _char_t char_type;
    typedef xml_node<char_type> node_type;

private:
    rapidxml::xml_document<char_type> m_document;
    cstring<_char_t> m_buf;

public:
    xml_document():m_document(), m_buf()
    {

    }
    xml_document(const char_type* file_name):m_document()
    {
        this->open(file_name);
    }
    ~xml_document()
    {
        this->close();
    }

    void create()
    {
        this->close();
        m_document.append_node(m_document.allocate_node(rapidxml::node_pi,
            T_TEXT(char_type, "xml version=\"1.0\" encoding=\"utf-8\"")));
    }

    int open(const char_type* filename)
    {
        io::binfile f;
        f.open(filename, io_read, sh_share);
        return on_load(&f);
    }

    int open(const wchar_t* filename)
    {
        io::binfile f;
        f.open(filename, io_read, sh_share);
        return on_load(&f);
    }

    int open_memory(const char* data, size_t size)
    {
        if(sizeof(char_type)!=1){
            m_buf.resize(size);
            code_convert<char_type, char8_t>(&m_buf.front(), m_buf.length(), (char8_t*)data, size);
        }
        else{
            m_buf.resize(size);
            memcpy(&m_buf.front(), data, size);
        }
        this->parse(&m_buf.front());
        return 0;
    }

    //转换成utf8码保存
    int save(const char_type* filename)
    {
        #if 0
        cstring<char_type> text;
        cstring<utf8_t> textu8;
        rapidxml::print(std::back_inserter(text), m_document, 0);
        textu8.reserve(text.length()*(sizeof(char_type)+1));
        int n = code_convert<utf8_t>(&textu8.front(), textu8.capacity(), text.c_str(), text.length());
        file<char_type> f;
        f.open(file_name, io_write, sh_share);
        f.resize(0);
        f.seek(0, seek_begin);
        f.write(&textu8.front(), n);
        #endif

        xml_writer w;
        w.open(filename);
        rapidxml::print(w, m_document, 0);
        w.close();
        return 0;
    }

    int save(const wchar_t* filename)
    {
        xml_writer w;
        w.open(filename);
        rapidxml::print(w, m_document, 0);
        w.close();
        return 0;
    }

    void close()
    {
        m_document.clear();
        m_buf.clear();
    }

    bool is_open()const
    {
        return !m_buf.empty();
    }

    xml_node<char_type> subnode(const char_type* name = null)
    {
        return m_document.first_node(name);
    }

    xml_node<char_type> push_front(const char_type* name, const char_type* value = null)
    {
        rapidxml::xml_node<char_type>* node = this->allocate_node(name, value);
        if(node){
            m_document.prepend_node(node);
            return xml_node<char_type>(node);
        }
        return xml_node<char_type>();
    }

    xml_node<char_type> push_back(const char_type* name, const char_type* value = null)
    {
        rapidxml::xml_node<char_type>* node = this->allocate_node(name, value);
        if(node){
            m_document.append_node(node);
            return xml_node<char_type>(node);
        }
        return xml_node<char_type>();
    }

//count
    size_t count(const char* node_name)const
    {
        size_t n = 0;
        xml_node<char_type> node = this->subnode(node_name);
        while(node){
            ++n;
            node = node.next(node_name);
        }
        return n;
    }

private:
    int on_load(io::unifile* f)
    {
        if(!f->is_open()){
            return CGL_ERROR;
        }

        int size = f->size();
        f->seek(0, seek_begin);

        if(sizeof(char_type)!=1){
            cstring<char> textu8;
            textu8.resize(size);
            f->read(&textu8.front(), size);
            m_buf.resize(textu8.length() + 1);
            code_convert<char_type, char8_t>(&m_buf.front(), m_buf.length(), (char8_t*)textu8.c_str(), size);
        }
        else{
            m_buf.resize(size + 1);
            f->read(&m_buf.front(), size);
        }
        f->close();

        this->parse(&m_buf.front());
        return 0;
    }

    void parse(_char_t* data)
    {
        try{
            m_document.parse<0>(data);
        }
        catch(rapidxml::parse_error err){
            //show_message(err.what());
            //show_message(err.where<wchar_t>());
        }
    }

    rapidxml::xml_node<char_type>* allocate_node(const char_type* name, const char_type* value)
    {
        return m_document.allocate_node(
            rapidxml::node_element,
            name,//m_document.allocate_string(name),
            value ? m_document.allocate_string(value) : value);
    }
};

}//end namespace cgl

#endif //XML_HXX_20160628110117

