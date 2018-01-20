/*

 cool.hpp

 "一生二，二生三，三生万物"，无三不成理，这就是事物的三态性。
 实例：
 int a = 1;
 int b = 2;
 cool c;
 c = compare(a, b)

 sdragonx 2010-04-10 09:31:05
 revise   2014-7-11 14:37

*/
#ifndef COOL_HPP_20100410093105
#define COOL_HPP_20100410093105

namespace cgl{

class __sign
{
public:
    const static int plus  =  1;
    const static int minus = -1;
    const static int zero  =  0;
public:
    __sign():value(zero)
    {
    }

    template<typename T>
    __sign(const T& n):value(int(n))
    {
        if(!is_zero())
        {
            if(is_plus())
                value = plus;
            else
                value = minus;
        }
    }

    template<typename T>
    const __sign& operator=(const T& n)
    {
        value = __sign(n).value;
        return *this;
	}
    template<typename T>operator T()const
    {
        return T(value);
    }
    /*取消这些大小的比较
    bool operator> (const int& n)const{ return value>n;  }
    bool operator>=(const int& n)const{ return value>=n; }
    bool operator< (const int& n)const{ return value<n;  }
    bool operator<=(const int& n)const{ return value<=n; }
    */
    bool operator! ()            const{ return is_zero(); }

    template<typename T>
    bool operator==(const T& n)const{ return value==__sign(n).value; }

    template<typename T>
    bool operator!=(const T& n)const{ return value!=__sign(n).value; }

    bool is_zero()const  { return !value;     }
    bool is_plus()const  { return value>zero; }
    bool is_minus()const { return value<zero; }
    bool is_upper()const { return is_plus();  }
    bool is_lower()const { return is_minus(); }
    bool is_right()const { return is_plus();  }
    bool is_left()const  { return is_minus(); }
    bool is_equal()const { return is_zero();  }
    bool is_null()const  { return is_zero();  }
private:
    int value;
};

typedef __sign sign, sign_t, cool, cool_t;

} //end namespace cgl

#endif //COOL_HPP_20100410093105
