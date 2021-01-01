/*
 Copyright (c) 2005-2020 sdragonx (mail:sdragonx@foxmail.com)

 xdocument.hpp

 2017-01-04 10:46:41

 utf8 格式

 2018-8-16 19:55
 m_last，添加了这个变量。当数据量不大的时候，无所谓，数据量很大的时候，对性能影响是巨大的

 2019/5/3 02:10
 前几天做的，更改了整体结构，从basetree继承结构

................................................................................

 除非指明数据是字符串，否则不当做字符串处理

 exsample:
 {
    name="abc";//string
    age=32;//number
    image=<...>;//binary base64code
 }

................................................................................


void ls_node(TTreeNode* Node, xnode<char> node)
{
    TTreeNode* n = Node->Owner->AddChild(Node, "");
    String s = node.name().c_str();
    if(!node.value().empty()){
        s += " = ";
        s += node.value().c_str();
    }
    n->Text = s;

    xnode<char> p = node.first();
    while(p){
        ls_node(n, p);
        p = p.next();
    }
}

*/
#ifndef PSET_HPP_20170104104641
#define PSET_HPP_20170104104641

#include <cgl/memory/block.hpp>
//#include <cgl/hash/base64.hpp>
#include <cgl/string.hpp>
#include <cgl/tree.hpp>

namespace cgl{

template<typename T>
class xdocument;

const int xnode_unknown  = 0x0000;
const int xnode_object   = 0x0001;
const int xnode_array    = 0x0002;
const int xnode_string   = 0x0004;
const int xnode_binary   = 0x0008;
const int xnode_bool     = 0x0010;
const int xnode_int      = 0x0020;
const int xnode_float    = 0x0040;
const int xnode_hex      = 0x0080;
const int xnode_digit    = xnode_bool | xnode_int | xnode_float | xnode_hex;
const int xnode_attribute= xnode_array | xnode_string | xnode_digit;
const int xnode_comments = 0x1000;  //注释

const int xnode_any      = 0xFFFF;


template<typename T>
struct xnode_struct
{
    typedef cstring<T> string_type;
    int type;
    string_type name;
    string_type value;

    xnode_struct() :  type(xnode_unknown), name(), value()
    {
    }
};

template<typename T>
class xnode
{
public:
    typedef xnode this_type;
    typedef T char_type;
    typedef cstring<T> string_type;
    typedef basic_node< xnode_struct<T> > node_type;
    typedef node_type* pointer;
private:
    pointer m_node;
public:
    xnode() : m_node(null)
    {

    }

    xnode(pointer node) : m_node(node)
    {

    }

    this_type& operator=(pointer node)
    {
        m_node = node;
        return *this;
    }

    pointer handle()const { return  m_node; }
    bool is_null()const   { return !m_node; }
    operator bool()const  { return  m_node; }

    int type()const
    {
        return m_node ? m_node->value.type : xnode_unknown;
    }

    const string_type& name()const
    {
        return !is_null() ? m_node->value.name : string_type();
    }

    const string_type& value(const string_type& def_value = string_type())const
    {
        return !is_null() ? m_node->value.value : def_value;
    }

    void set_value(const string_type& value)
    {
        if(!is_null()){
            m_node->value.value = value;
        }
    }

    this_type& operator=(const string_type& value){
        this->set_value(value);
        return *this;
    }



public:// add
    pointer add(const string_type& name, const string_type& value, int type = xnode_string)
    {
        if(!is_null()){
            node_type *node = m_node->add();
            node->value.name = name;
            node->value.value = value;
            node->value.type = type;
            if(m_node->value.type != xnode_object || m_node->value.type != xnode_array){//change node type
                m_node->value.type = xnode_object;
            }
            return node;
        }
        return null;
    }

    this_type add_node(const string_type& name)
    {
        return this->add(name, string_type(), xnode_object);
    }

    this_type add_value(const string_type& name, const string_type& value)
    {
        return this->add(name, value, xnode_digit);
    }

    this_type add_string(const string_type& name, const string_type& value)
    {
        return this->add(name, value, xnode_string);
    }

    this_type add_comments(const string_type& text)
    {
        if(text.find("*/") != text.npos)throw "xnode add_comments";
        return this->add(string_type(), text, xnode_comments);
    }



public:// array 没有加入int64支持
    template<typename T>
    size_t detach(T* array, size_t size)
    {
        if(!is_null()){
            node_type* subnode = m_node->first();
            for(size_t i=0; i<size; ++i){
                if(subnode){
                    if(is_float<T>::value){
                        array[i] = subnode->value.value.to_decimal();
                    }
                    else{
                        array[i] = subnode->value.value.to_int();
                    }
                    subnode = subnode->next();
                }
                else{
                    array[i] = 0;
                }
            }
        }
        return 0;
    }

    template<typename T>
    size_t join(const T* array, size_t size)
    {
        if(!is_null()){
            this_type subnode;
            for(size_t i=0; i<size; ++i){
                if(is_float<T>::value){
                    subnode = this->add(string_t(), string_type(array[i]), xnode_digit);
                }
                else{
                    subnode = this->add(string_t(), string_type(int(array[i])), xnode_digit);
                }
            }
            m_node->value.type = xnode_array;
        }
        return 0;
    }



public:// delete
    int erase(this_type node)
    {
        if(!is_null()){
            #warning xdoc erase
            return m_node->erase(node.m_node);
        }
        return -1;
    }

    void clear()
    {
        if(!is_null()){
            m_node->clear();
        }
    }

//---------------------------------------------------------------------------
/*
    this_type operator[](const char_type* name)
    {
        this_type node = this->subnode(name);
        if(!node){
            node = this->add_node(name);
        }
        return node;
    }

    //array
    this_type operator[](const string_type& name)
    {
        this_type node = this->subnode(name);
        if(!node){
            node = this->add_node(name);
        }
        return node;
    }

    //value
    this_type operator()(const string_type& name)
    {
        this_type node = this->subvalue(name);
        if(!node){
            node = this->add_value(name, string_type());
        }
        return node;
    }

    this_type operator()(const char_type* name)
    {
        this_type node = this->subvalue(name);
        if(!node){
            node = this->add_value(name, string_type());
        }
        return node;
    }
*/
//---------------------------------------------------------------------------



public:// first, last, and find
    this_type first()const
    {
        if(!is_null()){
            return m_node->first();
        }
        return this_type();
    }


    this_type last()const
    {
        if(!is_null()){
            return m_node->last();
        }
        return this_type();
    }

    this_type find(int type)const
    {
        if(is_null()){
            return this_type();
        }

        pointer node = m_node->first();
        while(node){
            if(node->value.type & type){
                return node;
            }
            node = node->next();
        }
        return this_type();
    }

    this_type find(const string_type& name)const
    {
        if(is_null()){
            return this_type();
        }

        pointer node = m_node->first();
        while(node){
            if(node->value.name == name){
                return node;
            }
            node = node->next();
        }
        return this_type();
    }

    this_type find(const string_type& name, int type)const
    {
        if(!is_null()){
            pointer node = m_node->first();
            while(node){
                if((node->value.type & type) && (node->value.name == name)){
                    return node;
                }
                node = node->next();
            }
        }
        return this_type();
    }



public:// count

    int count()const
    {
        if(is_null()){
            return 0;
        }

        pointer node = m_node->first();
        int n = 0;
        while(node){
            ++n;
            node = node->next();
        }
        return n;
    }

    int count(int type)const
    {
        if(is_null()){
            return 0;
        }

        pointer node = m_node->first();
        int n = 0;
        while(node){
            if(node->value.type & type){
                ++n;
            }
            node = node->next();
        }
        return n;
    }

    int count(const string_type& name)const
    {
        if(is_null()){
            return 0;
        }

        pointer node = m_node->first();
        int n = 0;
        while(node){
            if(node->value.name == name){
                ++n;
            }
            node = node->next();
        }
        return n;
    }

    int count(const string_type& name, int type)const
    {
        if(is_null()){
            return 0;
        }

        pointer node = m_node->first();
        int n = 0;
        while(node){
            if((node->value.type & type) && (node->value.name == name)){
                ++n;
            }
            node = node->next();
        }
        return n;
    }



public:// previous

    this_type previous()const
    {
        if(!is_null()){
            return m_node->previous();
        }
        return null;
    }

    this_type previous(int type)const
    {
        if(is_null()){
            return null;
        }

        pointer node = m_node->previous();
        while(node){
            if(node->value.type & type){
                return node;
            }
            node = node->previous();
        }
        return null;
    }

    this_type previous(const string_type& name)const
    {
        if(is_null()){
            return null;
        }

        pointer node = m_node->previous();
        while(node){
            if(node->value.name == name){
                return node;
            }
            node = node->previous();
        }
        return null;
    }

    this_type previous(const string_type& name, int type)const
    {
        if(is_null()){
            return null;
        }

        pointer node = m_node->previous();
        while(node){
            if((node->value.type & type) && (node->value.name == name)){
                return node;
            }
            node = node->previous();
        }
        return null;
    }



public:// next()

    this_type next()const
    {
        if(!is_null()){
            return m_node->next();
        }
        return this_type();
    }

    this_type next(int type)const
    {
        if(is_null()){
            return this_type();
        }

        pointer node = m_node->next();
        while(node){
            if(node->value.type & type){
                return node;
            }
            node = node->next();
        }
        return this_type();
    }

    this_type next(const string_type& name)const
    {
        if(is_null()){
            return this_type();
        }

        pointer node = m_node->next();
        while(node){
            if(node->value.name == name){
                return node;
            }
            node = node->next();
        }
        return this_type();
    }

    this_type next(const string_type& name, int type)const
    {
        if(is_null()){
            return null;
        }

        pointer node = m_node->next();
        while(node){
            if((node->value.type & type) && (node->value.name == name)){
                return node;
            }
            node = node->next();
        }
        return null;
    }



public:// subnode type is xnode_object
    this_type subnode()const
    {
        return this->find(xnode_object);
    }

    this_type subnode(const string_type& name)const
    {
        return this->find(name, xnode_object);
    }

    int count_nodes()const
    {
        return this->count(xnode_object);
    }

    pointer previous_node()const
    {
        return this->previous(xnode_object);
    }

    pointer previous_node(const string_type& name)const
    {
        return this->previous(name, xnode_object);
    }

    this_type next_node()const
    {
        return this->next(xnode_object);
    }

    this_type next_node(const string_type& name)const
    {
        return this->next(name, xnode_object);
    }



public:// attribute type is xnode_attribute
    this_type attribute()const
    {
        return this->find(xnode_attribute);
    }

    this_type attribute(const string_type& name)const
    {
        return this->find(name, xnode_attribute);
    }

    int count_attributes()const
    {
        return this->count(xnode_attribute);
    }

    pointer previous_attribute()const
    {
        return this->previous(xnode_attribute);
    }

    pointer next_attribute()const
    {
        return this->previous(xnode_attribute);
    }



public:// auto read values
    float read_float(const string_type& name, float def = 0.0f)
    {
        xnode<char> n = node.attribute(name);
        if(n){
            return n.to_float();
        }
        return def;
    }

    int read_int(const string_type& name, int def = 0)
    {
        xnode<char> n = node.attribute(name);
        if(n){
            return n.to_int();
        }
        return def;
    }

    string_type read_string(const string_t& name, const string_type& def = string_type())
    {
        xnode<char> n = node.attribute(name);
        if(n){
            return n.value();
        }
        return def;
    }



public:// value cast
    bool to_bool(bool default_value = false)const
    {
        if(!is_null() && !m_node->value.value.empty()){
            return m_node->value.value.to_boolean();
        }
        return default_value;
    }

    int to_int(int default_value = 0)const
    {
        if(!is_null() && !m_node->value.value.empty()){
            return m_node->value.value.to_int();
        }
        return default_value;
    }

    float to_float(float default_value = 0.0f)const
    {
        if(!is_null() && !m_node->value.value.empty()){
            return m_node->value.value.to_decimal();
        }
        return default_value;
    }
};

template<typename T>
class xdocument
{
public:
    typedef T char_type;
    typedef cstring<T> string_type;
    TYPENAME typedef xnode<T>::node_type basic_node;
    typedef xnode<T> xnode_type;

private:
    const static int assign       = '=';
    const static int object_begin = '{';
    const static int object_end   = '}';
    const static int array_begin  = '[';
    const static int array_end    = ']';
    const static int binary_begin = '<';
    const static int binary_end   = '>';
    const static int string_begin = '"';
    const static int string_end   = '"';
    const static int comma        = ',';
    const static int endline      = ';';
    //const static char_type* comments_begin;
    //const static char_type* comments_end;
private:
    basic_node m_node;
public:
    xdocument():m_node()
    {
    }
    ~xdocument()
    {
        clear();
    }

    int open(const char* fname);
    int open(const wchar_t* fname);
    int open(std::FILE* f);

    int parse(const char_type* buf, size_t size = UINT32_MAX);

    int print(string_type& text, bool format = false);
    int save(const char* filename, bool format = false);
    int save(const wchar_t* filename, bool format = false);
    int save(std::FILE* f, bool format = false);

    void clear()
    {
        m_node.clear();
    }

    //return first object node
    xnode<T> subnode()
    {
        return xnode_type(&m_node).find(xnode_object);
        /*node_type* node = m_node.first();
        while(node){
            if(node->value.type == xnode_object){
                return node;
            }
            node = node->next();
        }
        return xnode<T>();
        */
    }

    basic_node* create(const string_type& name = string_type())
    {
        clear();
        char text[20] = {//xdocument code=utf8
            0x78, 0x64, 0x6F, 0x63, 0x75, 0x6D, 0x65, 0x6E, 0x74, 0x20,
            0x63, 0x6F, 0x64, 0x65, 0x3D, 0x75, 0x74, 0x66, 0x38, 0x00
        };
        xnode_type node = &m_node;
        node.add_comments(text);
        return node.add_node(name).handle();
    }

    void set_comments(const string_type& text)
    {
        node_type* node = get_comments();
        if(node){
            node->set_value(text);
        }
    }
private:
    inline void skip_space(const char_type* &begin, const char_type* end)
    {
        for( ; begin < end; ++begin){
            if(begin[0] != ' ' && begin[0] != '\t' && begin[0] != '\r' && begin[0] != '\n'){
                break;
            }
        }
    }

    basic_node* get_comments()
    {
        node_type* node = m_node.subnode();
        while(node){
            if(node->type() == node_type::node_comments){
                return node;
            }
            node = node->next();
        }
        return null;
    }

    const char_type* read_string(string_type& str, const char_type* begin, const char_type* end);
    const char_type* read_binary(string_type& str, const char_type* begin, const char_type* end);
    string_type to_base64(const string_type& str);

    int print(string_type& text, basic_node* node, bool format);

    int to_escape_string(string_type& dst, const string_type& str);
};

template<typename T>
int xdocument<T>::open(const char* fname)
{
    std::FILE* f = fopen(fname, "rb");
    if(f){
        this->open(f);
        std::fclose(f);
    }
    return 0;
}

template<typename T>
int xdocument<T>::open(const wchar_t* fname)
{
    FILE* f = _wfopen(fname, L"rb");
    if(f){
        this->open(f);
        fclose(f);
    }
    return 0;
}

template<typename T>
int xdocument<T>::open(STD::FILE* f)
{
    if(!f){
        return -1;
    }

    fseek(f, 0, SEEK_END);
    size_t size = ftell(f);
    fseek(f, 0, SEEK_SET);
    memblock<char> buf;
    buf.resize(size + 1);
    size = fread(buf.data(), 1, size, f);
    buf.data()[size] = 0;
    size_t offset = 0;
    if(3 < size){
        if(buf[0] == 0xEF && buf[1] == 0xBB && buf[2] == 0xBF){
            offset = 3;
        }
    }
    parse((char_type*)(buf.data() + offset), size - offset);
    return 0;
}

template<typename T>
const T* xdocument<T>::read_string(string_type& str, const T* begin, const T* end)
{
    for( ; begin < end; ++begin)
    {
        if(*begin == '\\'){
            ++begin;
            switch(*begin)
            {
            case 's':
                str.push_back(' ');
                break;
            case 'r':
                str.push_back('\r');
                break;
            case 'n':
                str.push_back('\n');
                break;
            case 't':
                str.push_back('\t');
                break;
            case '\\':
                str.push_back('\\');
                break;
            case '"':
                str.push_back('"');
                break;
            default:
                break;
            }
        }
        else if(*begin == '"'){
            break;
        }
        else{
            str.push_back(*begin);
        }
    }
    return begin;
}

#if 0
template<typename T>
const char* xdocument<T>::read_binary(xdocument::string_type& str, const char_type* begin, const char_type* end)
{
    for( ; begin<end; ++begin)
    {
        if(begin[0] != '>'){
            if(!isspace(begin[0])){//忽略掉空白
                str.push_back(begin[0]);
            }
        }
        else{
            //char* buffer = new char[str.length()];
            //size_t size = hash::base64_decode(buffer, str.c_str(), str.length());
            //str.assign(buffer, size);
            //safe_delete(buffer);
            return begin;
        }
    }
    return begin;
}

template<typename T>
xdocument<T>::string_type xdocument<T>::to_base64(const string_type& str)
{
    char* buf = new char[str.length()*2];
    size_t size = hash::base64_encode(buf, str.c_str(), str.length());
    cstring<char> s(buf, size);
    safe_delete(buf);
    return s;
}

#endif

template<typename T>
int xdocument<T>::parse(const char_type* text, size_t size)
{
    basic_node* node = &m_node;
    const char_type* begin = text;
    const char_type* end;
    const char_type* itr;
    string_type str;
    int result = 0;

    int read_type = xnode_unknown;

    this->clear();

    if(size == UINT32_MAX){
        size = cslen(text);
    }
    end = text + size;

    //check unicode BOM

    skip_space(begin, end);
    for(; begin < end; )
    {
        switch(begin[0])
        {
        case '/'://comment
        {
            if(begin[1] == '*'){
                begin += 2;
                for(itr = begin; itr<end-1; ++itr){
                    if(itr[0] == '*' && itr[1] == '/'){
                        break;
                    }
                }
                str.assign(begin, itr);
                if(itr[0] == '*'){
                    itr += 2;
                }
                else{
                    return --result;
                }
                begin = itr;
                xnode_type(node).add(string_type(), str, xnode_comments);
            }
            else{//parse error
                return --result;
            }
            str.clear();
            skip_space(begin, end);
            break;
        }

        case object_begin://{ push object
        {
            str.trim_right();
            node = xnode_type(node).add(str, string_type(), xnode_object);
            str.clear();
            ++begin;
            skip_space(begin, end);
            break;
        }

        case object_end://} pop object
        {
            node = node->parent();
            skip_space(++begin, end);
            read_type = xnode_unknown;//reset read state
            break;
        }

        //读取到=符号，此时数据类型未知
        case assign://= push value
        {
            str.trim_right();
            node = xnode_type(node).add(str, string_type(), xnode_unknown);
            str.clear();
            skip_space(++begin, end);
            break;
        }

        case endline://; pop value
        {
            /*
            if(node->value.type == xnode_string){// || node->type() == node_type::node_binary){
                //node->set_value(str);
                node->value.value = str;
                node = node->parent();
            }
            else if(node->value.type == xnode_array)
            {

            }
            */

            if(node->value.type != xnode_object && node->value.type != xnode_array){
                if(read_type == xnode_string){
                    node->value.type = xnode_string;
                    node->value.value = str;
                    node = node->parent();
                }
                else if(read_type == xnode_unknown){
                    node->value.type = xnode_digit;
                    //if(str as int); as float; like bool
                    node->value.value = str;
                    node = node->parent();
                }
            }

            read_type = xnode_unknown;//reset read state

            str.clear();
            skip_space(++begin, end);
            break;
        }
        case string_begin://"
        {
            read_type = xnode_string;
            begin = read_string(str, ++begin, end);
            skip_space(++begin, end);
            break;
        }

        //array suppert ---------------
        case array_begin://[ push array
        {
            node->value.type = xnode_array;
            skip_space(++begin, end);
            break;
        }
        case array_end://] pop array
        {
            str.trim_right();
            if(!str.empty()){// , "str" ]
                xnode_type(node).add(string_type(), str, read_type);
                str.clear();
                read_type = xnode_unknown;//reset read state
            }
            node = node->parent();
            //move_endline
            skip_space(++begin, end);
            break;
        }
        case comma://, 目前，只能存在数组里面
        {
            if(node->value.type == xnode_array){
                xnode_type(node).add(string_type(), str, read_type);//xnode_string);
                str.clear();
                read_type = xnode_unknown;//reset read state
            }
            else{
                str.push_back(begin[0]);
            }
            skip_space(++begin, end);
            break;
        }
        //array suppert ---------------

        #if 0
        /*
        case binary_begin://< binary data
        {
            node->m_type = node_type::node_binary;
            str.clear();
            begin = read_binary(str, ++begin, end);
            skip_space(++begin, end);
            break;
        }
        */
        #endif
        default:
            str.push_back(begin[0]);
            ++begin;
            break;
        }
    }

    return result;
}

template<typename _char_t>
int xdocument<_char_t>::to_escape_string(string_type& dst, const string_type& str)
{
    for(size_t i=0; i<str.length(); ++i)
    {
        switch(str[i])
        {
        case ' ':
            dst.push_back('\\');
            dst.push_back('s');
            break;
        case '\r':
            dst.push_back('\\');
            dst.push_back('r');
            break;
        case '\n':
            dst.push_back('\\');
            dst.push_back('n');
            break;
        case '\t':
            dst.push_back('\\');
            dst.push_back('t');
            break;
        case '\\':
            dst.push_back('\\');
            dst.push_back('\\');
            break;
        case '"':
            dst.push_back('\\');
            dst.push_back('"');
            break;
        default:
            dst.push_back(str[i]);
            break;
        }
    }
    return 0;
}

template<typename T>
int xdocument<T>::print(string_type& text, basic_node* node, bool format)
{
    if(!node)return -1;

    static const T nextline[]        = {'\r', '\n', '\0'};
    static const T comments_begin[]    = {'/', '*', '\0'};
    static const T comments_end[]    = {'*', '/', '\0'};
    static const T space = '\t';

    int level = 0;

    basic_node* subnode;

    while(node){
        switch(node->value.type)
        {
        case xnode_object:
            if(format)text.append(space, level);
            text.append(node->value.name);
            text.push_back(object_begin);
            if(format)text.append(nextline);
            //print subnode
            if(node->first()){
                ++level;
                node = node->first();
                continue;
            }
            else{//end node
                if(format)text.append(space, level);
                text.push_back(object_end);
                if(format)text.append(nextline);
            }
            break;
        case xnode_array:
            if(format)text.append(space, level);
            text.append(node->value.name);
            text.push_back(assign);
            text.push_back(array_begin);
            subnode = node->first();
            while(subnode){
                if(subnode->value.type == xnode_string)text.push_back(string_begin);
                //text.append(to_escape(n->value.value));
                //to_escape_string(text, node->value.value);
                text.append(subnode->value.value);
                if(subnode->value.type == xnode_string)text.push_back(string_end);
                if(subnode->next()){
                    text.push_back(comma);
                }
                subnode = subnode->next();
            }
            text.push_back(array_end);
            text.push_back(endline);
            if(format)text.append(nextline);
            break;

        #if 0
        /*
        case node_binary:
            if(format)text.append(space);
            text.append(node->name());
            text.append(assign);
            text.append(binary_begin);
            text.append(to_base64(node->value()));
            text.append(binary_end);
            text.append(endline);
            if(format)text.append(newline);
            break;
        */
        #endif
        case xnode_string:
        //case xnode_binary:
        case xnode_bool:
        case xnode_int:
        case xnode_float:
        case xnode_hex:
        case xnode_digit:
            if(format)text.append(space, level);
            text.append(node->value.name);
            if(node->value.value.empty()){
                text.push_back(assign);
                text.push_back(endline);
                if(format)text.append(nextline);
            }
            else{
                text.push_back(assign);
                if(node->value.type == xnode_string)text.push_back(string_begin);
                to_escape_string(text, node->value.value);
                if(node->value.type == xnode_string)text.push_back(string_end);
                text.push_back(endline);
                if(format)text.append(nextline);
            }
            break;
        case xnode_comments:
            if(format)text.append(space, level);
            text.append(comments_begin);
            text.append(node->value.value);
            text.append(comments_end);
            if(format)text.append(nextline);
            break;
        default:
            break;
        }

        if(node->next()){
            node = node->next();
        }
        else{//end object
            do{
                --level;
                if(level < 0)break;
                node = node->parent();
                if(format)text.append(space, level);
                text.push_back(object_end);
                if(format)text.append(nextline);
                if(level <= 0){
                    break;
                }
            }while(node && !node->next());

            //if(node){
                node = node->next();
            //}
        }
    }//end while

    return text.length();
}

template<typename T>
int xdocument<T>::print(xdocument<T>::string_type& text, bool format)
{
    return print(text, m_node.first(), format);
}

template<typename T>
int xdocument<T>::save(const char* filename, bool format)
{
    STD::FILE* f = STD::fopen(filename, "wb");
    save(f, format);
    fclose(f);
    return 0;
}

template<typename T>
int xdocument<T>::save(const wchar_t* filename, bool format)
{
    FILE* f = _wfopen(filename, L"wb");
    save(f, format);
    fclose(f);
    return 0;
}

template<typename T>
int xdocument<T>::save(STD::FILE* f, bool format)
{
    string_type text;
    print(text, format);
    /*
    if(BOM){
        //writer.out(llvm::UNI_UTF8_BOM, 3);
    }
    */
    fwrite(text.c_str(), sizeof(T), text.length(), f);
    return 0;
}

};

#endif
