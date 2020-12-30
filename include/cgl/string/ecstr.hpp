/*
 Copyright (c) 2005-2020 sdragonx (mail:sdragonx@foxmail.com)

 ecstr.h

 2015-02-23 14:39:28
 2020-01-31 23:39

 c����ַ���ת��ת���ַ�����ȥ�����еĿո��ַ�
 escape charactor string
 ת���ַ���ת��

ת���ַ�  ����        ASCII��ֵ��ʮ���ƣ�
\a        ����(BEL)       007
\b        �˸�(BS)        008
\f        ��ҳ(FF)        012
\n        ����(LF)        010
\r        �س�(CR)        013
\t        ˮƽ�Ʊ�(HT)    009
\v        ��ֱ�Ʊ�(VT)    011
\\        ��б���ַ�'\'   092
\'        ������          039
\"        ˫����          034
\?        �ʺ�            063
\0        ���ַ�(NUL)     000
\ddd      �˽������ַ�    1��3λ�˽���
\xhh      ʮ�������ַ�    ʮ������

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
        else{//���ɴ�ӡ�ַ�
            byte_t buf[2];
            tohex8(buf, ch);

            //ʮ������ת���ַ�
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