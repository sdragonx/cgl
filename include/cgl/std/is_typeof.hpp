/*
 Copyright (c) 2005-2020 sdragonx (mail:sdragonx@foxmail.com)

 is_typeof.hpp

 2020-03-03 22:31:32


void make_function()
{
    int n = 32;
    String s;
    String tml;
    String Tx;
    char buf[1024];
    const char* name = "is_typeof";

    mmlog.clear();

    //T0, T1, T2, ...
    for(int i=0; i<n; ++i){
        s = "T" + IntToStr(i);
        if(i != n - 1)s += ", ";
        Tx += s;
    }

    //主函数
    //typename T0 = empty_t, ...
    for(int i=0; i<n; ++i){
        snprintf(buf, 1024, "typename T%-2i = empty_t", i);
        s = buf;
        if(i != n - 1){
            s += ", ";
            if(i > 0 && (i + 1) % 4 == 0)s += "\r\n    ";
        }
        tml += s;
    }

    mmlog<<"template<typename T, \r\n    " << tml << ">" << endline;
    mmlog<<"struct " << name << " : public false_type { };\r\n" << endline;

    //特化
    tml = String();
    for(int i=0; i<n; ++i){
        snprintf(buf, 1024, "typename T%-2i", i);
        s = buf;
        if(i != n - 1){
            s += ", ";
            if(i > 0 && (i + 1) % 4 == 0)s += "\r\n         ";
        }
        tml += s;
    }

    for(int i=0; i<n; ++i){
        s = "T" + IntToStr(i);
        mmlog<<"template<" << tml << ">" << endline;
        mmlog<<"struct " << name << "<" << s << ", " << Tx << "> : public true_type { };\r\n" << endline;
    }
}

*/
#ifndef IS_TYPEOF_HPP_20200303223132
#define IS_TYPEOF_HPP_20200303223132

template<typename T, 
    typename T0  = empty_t, typename T1  = empty_t, typename T2  = empty_t, typename T3  = empty_t, 
    typename T4  = empty_t, typename T5  = empty_t, typename T6  = empty_t, typename T7  = empty_t, 
    typename T8  = empty_t, typename T9  = empty_t, typename T10 = empty_t, typename T11 = empty_t, 
    typename T12 = empty_t, typename T13 = empty_t, typename T14 = empty_t, typename T15 = empty_t, 
    typename T16 = empty_t, typename T17 = empty_t, typename T18 = empty_t, typename T19 = empty_t, 
    typename T20 = empty_t, typename T21 = empty_t, typename T22 = empty_t, typename T23 = empty_t, 
    typename T24 = empty_t, typename T25 = empty_t, typename T26 = empty_t, typename T27 = empty_t, 
    typename T28 = empty_t, typename T29 = empty_t, typename T30 = empty_t, typename T31 = empty_t>
struct is_typeof : public false_type { };

template<typename T0 , typename T1 , typename T2 , typename T3 , 
         typename T4 , typename T5 , typename T6 , typename T7 , 
         typename T8 , typename T9 , typename T10, typename T11, 
         typename T12, typename T13, typename T14, typename T15, 
         typename T16, typename T17, typename T18, typename T19, 
         typename T20, typename T21, typename T22, typename T23, 
         typename T24, typename T25, typename T26, typename T27, 
         typename T28, typename T29, typename T30, typename T31>
struct is_typeof<T0, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31> : public true_type { };

template<typename T0 , typename T1 , typename T2 , typename T3 , 
         typename T4 , typename T5 , typename T6 , typename T7 , 
         typename T8 , typename T9 , typename T10, typename T11, 
         typename T12, typename T13, typename T14, typename T15, 
         typename T16, typename T17, typename T18, typename T19, 
         typename T20, typename T21, typename T22, typename T23, 
         typename T24, typename T25, typename T26, typename T27, 
         typename T28, typename T29, typename T30, typename T31>
struct is_typeof<T1, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31> : public true_type { };

template<typename T0 , typename T1 , typename T2 , typename T3 , 
         typename T4 , typename T5 , typename T6 , typename T7 , 
         typename T8 , typename T9 , typename T10, typename T11, 
         typename T12, typename T13, typename T14, typename T15, 
         typename T16, typename T17, typename T18, typename T19, 
         typename T20, typename T21, typename T22, typename T23, 
         typename T24, typename T25, typename T26, typename T27, 
         typename T28, typename T29, typename T30, typename T31>
struct is_typeof<T2, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31> : public true_type { };

template<typename T0 , typename T1 , typename T2 , typename T3 , 
         typename T4 , typename T5 , typename T6 , typename T7 , 
         typename T8 , typename T9 , typename T10, typename T11, 
         typename T12, typename T13, typename T14, typename T15, 
         typename T16, typename T17, typename T18, typename T19, 
         typename T20, typename T21, typename T22, typename T23, 
         typename T24, typename T25, typename T26, typename T27, 
         typename T28, typename T29, typename T30, typename T31>
struct is_typeof<T3, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31> : public true_type { };

template<typename T0 , typename T1 , typename T2 , typename T3 , 
         typename T4 , typename T5 , typename T6 , typename T7 , 
         typename T8 , typename T9 , typename T10, typename T11, 
         typename T12, typename T13, typename T14, typename T15, 
         typename T16, typename T17, typename T18, typename T19, 
         typename T20, typename T21, typename T22, typename T23, 
         typename T24, typename T25, typename T26, typename T27, 
         typename T28, typename T29, typename T30, typename T31>
struct is_typeof<T4, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31> : public true_type { };

template<typename T0 , typename T1 , typename T2 , typename T3 , 
         typename T4 , typename T5 , typename T6 , typename T7 , 
         typename T8 , typename T9 , typename T10, typename T11, 
         typename T12, typename T13, typename T14, typename T15, 
         typename T16, typename T17, typename T18, typename T19, 
         typename T20, typename T21, typename T22, typename T23, 
         typename T24, typename T25, typename T26, typename T27, 
         typename T28, typename T29, typename T30, typename T31>
struct is_typeof<T5, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31> : public true_type { };

template<typename T0 , typename T1 , typename T2 , typename T3 , 
         typename T4 , typename T5 , typename T6 , typename T7 , 
         typename T8 , typename T9 , typename T10, typename T11, 
         typename T12, typename T13, typename T14, typename T15, 
         typename T16, typename T17, typename T18, typename T19, 
         typename T20, typename T21, typename T22, typename T23, 
         typename T24, typename T25, typename T26, typename T27, 
         typename T28, typename T29, typename T30, typename T31>
struct is_typeof<T6, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31> : public true_type { };

template<typename T0 , typename T1 , typename T2 , typename T3 , 
         typename T4 , typename T5 , typename T6 , typename T7 , 
         typename T8 , typename T9 , typename T10, typename T11, 
         typename T12, typename T13, typename T14, typename T15, 
         typename T16, typename T17, typename T18, typename T19, 
         typename T20, typename T21, typename T22, typename T23, 
         typename T24, typename T25, typename T26, typename T27, 
         typename T28, typename T29, typename T30, typename T31>
struct is_typeof<T7, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31> : public true_type { };

template<typename T0 , typename T1 , typename T2 , typename T3 , 
         typename T4 , typename T5 , typename T6 , typename T7 , 
         typename T8 , typename T9 , typename T10, typename T11, 
         typename T12, typename T13, typename T14, typename T15, 
         typename T16, typename T17, typename T18, typename T19, 
         typename T20, typename T21, typename T22, typename T23, 
         typename T24, typename T25, typename T26, typename T27, 
         typename T28, typename T29, typename T30, typename T31>
struct is_typeof<T8, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31> : public true_type { };

template<typename T0 , typename T1 , typename T2 , typename T3 , 
         typename T4 , typename T5 , typename T6 , typename T7 , 
         typename T8 , typename T9 , typename T10, typename T11, 
         typename T12, typename T13, typename T14, typename T15, 
         typename T16, typename T17, typename T18, typename T19, 
         typename T20, typename T21, typename T22, typename T23, 
         typename T24, typename T25, typename T26, typename T27, 
         typename T28, typename T29, typename T30, typename T31>
struct is_typeof<T9, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31> : public true_type { };

template<typename T0 , typename T1 , typename T2 , typename T3 , 
         typename T4 , typename T5 , typename T6 , typename T7 , 
         typename T8 , typename T9 , typename T10, typename T11, 
         typename T12, typename T13, typename T14, typename T15, 
         typename T16, typename T17, typename T18, typename T19, 
         typename T20, typename T21, typename T22, typename T23, 
         typename T24, typename T25, typename T26, typename T27, 
         typename T28, typename T29, typename T30, typename T31>
struct is_typeof<T10, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31> : public true_type { };

template<typename T0 , typename T1 , typename T2 , typename T3 , 
         typename T4 , typename T5 , typename T6 , typename T7 , 
         typename T8 , typename T9 , typename T10, typename T11, 
         typename T12, typename T13, typename T14, typename T15, 
         typename T16, typename T17, typename T18, typename T19, 
         typename T20, typename T21, typename T22, typename T23, 
         typename T24, typename T25, typename T26, typename T27, 
         typename T28, typename T29, typename T30, typename T31>
struct is_typeof<T11, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31> : public true_type { };

template<typename T0 , typename T1 , typename T2 , typename T3 , 
         typename T4 , typename T5 , typename T6 , typename T7 , 
         typename T8 , typename T9 , typename T10, typename T11, 
         typename T12, typename T13, typename T14, typename T15, 
         typename T16, typename T17, typename T18, typename T19, 
         typename T20, typename T21, typename T22, typename T23, 
         typename T24, typename T25, typename T26, typename T27, 
         typename T28, typename T29, typename T30, typename T31>
struct is_typeof<T12, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31> : public true_type { };

template<typename T0 , typename T1 , typename T2 , typename T3 , 
         typename T4 , typename T5 , typename T6 , typename T7 , 
         typename T8 , typename T9 , typename T10, typename T11, 
         typename T12, typename T13, typename T14, typename T15, 
         typename T16, typename T17, typename T18, typename T19, 
         typename T20, typename T21, typename T22, typename T23, 
         typename T24, typename T25, typename T26, typename T27, 
         typename T28, typename T29, typename T30, typename T31>
struct is_typeof<T13, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31> : public true_type { };

template<typename T0 , typename T1 , typename T2 , typename T3 , 
         typename T4 , typename T5 , typename T6 , typename T7 , 
         typename T8 , typename T9 , typename T10, typename T11, 
         typename T12, typename T13, typename T14, typename T15, 
         typename T16, typename T17, typename T18, typename T19, 
         typename T20, typename T21, typename T22, typename T23, 
         typename T24, typename T25, typename T26, typename T27, 
         typename T28, typename T29, typename T30, typename T31>
struct is_typeof<T14, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31> : public true_type { };

template<typename T0 , typename T1 , typename T2 , typename T3 , 
         typename T4 , typename T5 , typename T6 , typename T7 , 
         typename T8 , typename T9 , typename T10, typename T11, 
         typename T12, typename T13, typename T14, typename T15, 
         typename T16, typename T17, typename T18, typename T19, 
         typename T20, typename T21, typename T22, typename T23, 
         typename T24, typename T25, typename T26, typename T27, 
         typename T28, typename T29, typename T30, typename T31>
struct is_typeof<T15, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31> : public true_type { };

template<typename T0 , typename T1 , typename T2 , typename T3 , 
         typename T4 , typename T5 , typename T6 , typename T7 , 
         typename T8 , typename T9 , typename T10, typename T11, 
         typename T12, typename T13, typename T14, typename T15, 
         typename T16, typename T17, typename T18, typename T19, 
         typename T20, typename T21, typename T22, typename T23, 
         typename T24, typename T25, typename T26, typename T27, 
         typename T28, typename T29, typename T30, typename T31>
struct is_typeof<T16, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31> : public true_type { };

template<typename T0 , typename T1 , typename T2 , typename T3 , 
         typename T4 , typename T5 , typename T6 , typename T7 , 
         typename T8 , typename T9 , typename T10, typename T11, 
         typename T12, typename T13, typename T14, typename T15, 
         typename T16, typename T17, typename T18, typename T19, 
         typename T20, typename T21, typename T22, typename T23, 
         typename T24, typename T25, typename T26, typename T27, 
         typename T28, typename T29, typename T30, typename T31>
struct is_typeof<T17, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31> : public true_type { };

template<typename T0 , typename T1 , typename T2 , typename T3 , 
         typename T4 , typename T5 , typename T6 , typename T7 , 
         typename T8 , typename T9 , typename T10, typename T11, 
         typename T12, typename T13, typename T14, typename T15, 
         typename T16, typename T17, typename T18, typename T19, 
         typename T20, typename T21, typename T22, typename T23, 
         typename T24, typename T25, typename T26, typename T27, 
         typename T28, typename T29, typename T30, typename T31>
struct is_typeof<T18, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31> : public true_type { };

template<typename T0 , typename T1 , typename T2 , typename T3 , 
         typename T4 , typename T5 , typename T6 , typename T7 , 
         typename T8 , typename T9 , typename T10, typename T11, 
         typename T12, typename T13, typename T14, typename T15, 
         typename T16, typename T17, typename T18, typename T19, 
         typename T20, typename T21, typename T22, typename T23, 
         typename T24, typename T25, typename T26, typename T27, 
         typename T28, typename T29, typename T30, typename T31>
struct is_typeof<T19, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31> : public true_type { };

template<typename T0 , typename T1 , typename T2 , typename T3 , 
         typename T4 , typename T5 , typename T6 , typename T7 , 
         typename T8 , typename T9 , typename T10, typename T11, 
         typename T12, typename T13, typename T14, typename T15, 
         typename T16, typename T17, typename T18, typename T19, 
         typename T20, typename T21, typename T22, typename T23, 
         typename T24, typename T25, typename T26, typename T27, 
         typename T28, typename T29, typename T30, typename T31>
struct is_typeof<T20, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31> : public true_type { };

template<typename T0 , typename T1 , typename T2 , typename T3 , 
         typename T4 , typename T5 , typename T6 , typename T7 , 
         typename T8 , typename T9 , typename T10, typename T11, 
         typename T12, typename T13, typename T14, typename T15, 
         typename T16, typename T17, typename T18, typename T19, 
         typename T20, typename T21, typename T22, typename T23, 
         typename T24, typename T25, typename T26, typename T27, 
         typename T28, typename T29, typename T30, typename T31>
struct is_typeof<T21, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31> : public true_type { };

template<typename T0 , typename T1 , typename T2 , typename T3 , 
         typename T4 , typename T5 , typename T6 , typename T7 , 
         typename T8 , typename T9 , typename T10, typename T11, 
         typename T12, typename T13, typename T14, typename T15, 
         typename T16, typename T17, typename T18, typename T19, 
         typename T20, typename T21, typename T22, typename T23, 
         typename T24, typename T25, typename T26, typename T27, 
         typename T28, typename T29, typename T30, typename T31>
struct is_typeof<T22, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31> : public true_type { };

template<typename T0 , typename T1 , typename T2 , typename T3 , 
         typename T4 , typename T5 , typename T6 , typename T7 , 
         typename T8 , typename T9 , typename T10, typename T11, 
         typename T12, typename T13, typename T14, typename T15, 
         typename T16, typename T17, typename T18, typename T19, 
         typename T20, typename T21, typename T22, typename T23, 
         typename T24, typename T25, typename T26, typename T27, 
         typename T28, typename T29, typename T30, typename T31>
struct is_typeof<T23, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31> : public true_type { };

template<typename T0 , typename T1 , typename T2 , typename T3 , 
         typename T4 , typename T5 , typename T6 , typename T7 , 
         typename T8 , typename T9 , typename T10, typename T11, 
         typename T12, typename T13, typename T14, typename T15, 
         typename T16, typename T17, typename T18, typename T19, 
         typename T20, typename T21, typename T22, typename T23, 
         typename T24, typename T25, typename T26, typename T27, 
         typename T28, typename T29, typename T30, typename T31>
struct is_typeof<T24, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31> : public true_type { };

template<typename T0 , typename T1 , typename T2 , typename T3 , 
         typename T4 , typename T5 , typename T6 , typename T7 , 
         typename T8 , typename T9 , typename T10, typename T11, 
         typename T12, typename T13, typename T14, typename T15, 
         typename T16, typename T17, typename T18, typename T19, 
         typename T20, typename T21, typename T22, typename T23, 
         typename T24, typename T25, typename T26, typename T27, 
         typename T28, typename T29, typename T30, typename T31>
struct is_typeof<T25, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31> : public true_type { };

template<typename T0 , typename T1 , typename T2 , typename T3 , 
         typename T4 , typename T5 , typename T6 , typename T7 , 
         typename T8 , typename T9 , typename T10, typename T11, 
         typename T12, typename T13, typename T14, typename T15, 
         typename T16, typename T17, typename T18, typename T19, 
         typename T20, typename T21, typename T22, typename T23, 
         typename T24, typename T25, typename T26, typename T27, 
         typename T28, typename T29, typename T30, typename T31>
struct is_typeof<T26, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31> : public true_type { };

template<typename T0 , typename T1 , typename T2 , typename T3 , 
         typename T4 , typename T5 , typename T6 , typename T7 , 
         typename T8 , typename T9 , typename T10, typename T11, 
         typename T12, typename T13, typename T14, typename T15, 
         typename T16, typename T17, typename T18, typename T19, 
         typename T20, typename T21, typename T22, typename T23, 
         typename T24, typename T25, typename T26, typename T27, 
         typename T28, typename T29, typename T30, typename T31>
struct is_typeof<T27, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31> : public true_type { };

template<typename T0 , typename T1 , typename T2 , typename T3 , 
         typename T4 , typename T5 , typename T6 , typename T7 , 
         typename T8 , typename T9 , typename T10, typename T11, 
         typename T12, typename T13, typename T14, typename T15, 
         typename T16, typename T17, typename T18, typename T19, 
         typename T20, typename T21, typename T22, typename T23, 
         typename T24, typename T25, typename T26, typename T27, 
         typename T28, typename T29, typename T30, typename T31>
struct is_typeof<T28, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31> : public true_type { };

template<typename T0 , typename T1 , typename T2 , typename T3 , 
         typename T4 , typename T5 , typename T6 , typename T7 , 
         typename T8 , typename T9 , typename T10, typename T11, 
         typename T12, typename T13, typename T14, typename T15, 
         typename T16, typename T17, typename T18, typename T19, 
         typename T20, typename T21, typename T22, typename T23, 
         typename T24, typename T25, typename T26, typename T27, 
         typename T28, typename T29, typename T30, typename T31>
struct is_typeof<T29, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31> : public true_type { };

template<typename T0 , typename T1 , typename T2 , typename T3 , 
         typename T4 , typename T5 , typename T6 , typename T7 , 
         typename T8 , typename T9 , typename T10, typename T11, 
         typename T12, typename T13, typename T14, typename T15, 
         typename T16, typename T17, typename T18, typename T19, 
         typename T20, typename T21, typename T22, typename T23, 
         typename T24, typename T25, typename T26, typename T27, 
         typename T28, typename T29, typename T30, typename T31>
struct is_typeof<T30, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31> : public true_type { };

template<typename T0 , typename T1 , typename T2 , typename T3 , 
         typename T4 , typename T5 , typename T6 , typename T7 , 
         typename T8 , typename T9 , typename T10, typename T11, 
         typename T12, typename T13, typename T14, typename T15, 
         typename T16, typename T17, typename T18, typename T19, 
         typename T20, typename T21, typename T22, typename T23, 
         typename T24, typename T25, typename T26, typename T27, 
         typename T28, typename T29, typename T30, typename T31>
struct is_typeof<T31, T0, T1, T2, T3, T4, T5, T6, T7, T8,T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31> : public true_type { };

#endif //IS_TYPEOF_HPP_20200303223132
