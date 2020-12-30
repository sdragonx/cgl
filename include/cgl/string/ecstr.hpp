/*
 Copyright (c) 2005-2020 sdragonx (mail:sdragonx@foxmail.com)

 ecstr.h

 2015-02-23 14:39:28
 2020-01-31 23:39

 c风格字符串转换转义字符串，去掉其中的空格字符
 escape charactor string
 转义字符串转换

转义字符  意义        ASCII码值（十进制）
\a        响铃(BEL)       007
\b        退格(BS)        008
\f        换页(FF)        012
\n        换行(LF)        010
\r        回车(CR)        013
\t        水平制表(HT)    009
\v        垂直制表(VT)    011
\\        反斜线字符'\'   092
\'        单引号          039
\"        双引号          034
\?        问号            063
\0        空字符(NUL)     000
\ddd      八进制数字符    1到3位八进制
\xhh      十六进制字符    十六进制

*/
#ifndef ECSTR_H_20150223143928
#define ECSTR_H_20150223143928

#include <cgl/string/cstring.hpp>

namespace cgl{

template<typename _char_t>
inline void encode_char(cstring<_char_t>& dest, _char_t ch, bool hex_mode = true)
{
    switch(ch){
    case '\0':
        dest.push_back('\\');
        dest.push_back('0');
        break;
    case ' ':
        dest.push_back('\\');
        dest.push_back('s');
        break;
    case '\r':
        dest.push_back('\\');
        dest.push_back('r');
        break;
    case '\n':
        dest.push_back('\\');
        dest.push_back('n');
        break;
    case '\t':
        dest.push_back('\\');
        dest.push_back('t');
        break;
    //gnuc
    #if 0
    case '\p':
        dest.push_back('\\');
        dest.push_back('p');
        break;
    #endif
    case '\\':
        dest.push_back('\\');
        dest.push_back('\\');
        break;
    case '\"':
        dest.push_back('\\');
        dest.push_back('\"');
        break;
    case '\'':
        dest.push_back('\\');
        dest.push_back('\'');
        break;
    default:
        if(isprint(ch)){
            dest.push_back(ch);
        }
        else{//不可打印字符
            byte_t buf[2];
            tohex8(buf, ch);

            //十六进制转义字符
            dest.push_back('\\');
            dest.push_back('x');
            dest.push_back(buf[0]);
            dest.push_back(buf[1]);
        }
        break;
    }
}

template<typename _char_t>
int to_escape(cstring<_char_t>& dest, const cstring<_char_t>& str)
{
    dest.clear();
    for(size_t i=0; i<str.length(); ++i){
        encode_char(dest, str[i]);
    }
    return 0;
}

template<typename _char_t>
cstring<_char_t> from_escape(const cstring<_char_t>& str)
{
    cstring<_char_t> temp;
    temp.reserve(str.length());

    TYPENAME cstring<_char_t>::const_iterator begin = str.begin();
    TYPENAME cstring<_char_t>::const_iterator end = str.end();
    for( ; begin != end; ++begin){
        if(*begin != '\\'){
            temp.push_back(*begin);
            continue;
        }
        ++begin;
        if(begin >= end){
            break;
        }
        switch(*begin)
        {
        case 's':
            temp.push_back(' ');
            break;
        case 'r':
            temp.push_back('\r');
            break;
        case 'n':
            temp.push_back('\n');
            break;
        case 't':
            temp.push_back('\t');
            break;
        #if 0
        case 'p':
            temp.push_back('\p');
            break;
        #endif
        case '\\':
            temp.push_back('\\');
            break;
        case '\"':
            temp.push_back('\"');
            break;
        case '\'':
            temp.push_back('\'');
            break;
        case '0':
            temp.push_back('\0');
            break;
        default:
            break;
        }
    }
    return temp;
}

}//end namespace cgl

#endif //ECSTR_H_20150223143928