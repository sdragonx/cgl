/*

 vectype.h

 sdragonx 2015-09-09 20:32:28

 revise sdragonx 2017/2/4

*/
#ifndef CGL_VECTYPE_H_20150909203228
#define CGL_VECTYPE_H_20150909203228

#include <algorithm>
#include <float.h>

namespace cgl{

#define VEC2_OPERATION(op)\
	template<typename X>\
	this_type operator op(const X& value)\
	{\
		return this_type(x op value, y op value);\
	}\
	template<typename X>\
	this_type operator op(const vec2<X>& v)\
	{\
		return this_type(x op v.x, y op v.y);\
	}\
	template<typename X>\
	const this_type& operator op=(const X& value)\
	{\
		x op= value; y op= value;\
		return *this;\
	}\
	template<typename X>\
	const this_type& operator op=(const vec2<X>& v)\
	{\
		x op= v.x; y op= v.y;\
		return *this;\
	}

#define VEC3_OPERATION(op)\
	template<typename X>\
	this_type operator op(const X& value)\
	{\
		return this_type(x op value, y op value, z op value);\
	}\
	template<typename X>\
	this_type operator op(const vec2<X>& v)\
	{\
		return this_type(x op v.x, y op v.y, z);\
	}\
	template<typename X>\
	this_type operator op(const vec3<X>& v)\
	{\
		return this_type(x op v.x, y op v.y, z op v.z);\
	}\
	template<typename X>\
	const this_type& operator op=(const X& value)\
	{\
		x op= value; y op= value; z op= value;\
		return *this;\
	}\
	template<typename X>\
	const this_type& operator op=(const vec2<X>& v)\
	{\
		x op= v.x; y op= v.y;\
		return *this;\
	}\
	template<typename X>\
	const this_type& operator op=(const vec3<X>& v)\
	{\
		x op= v.x; y op= v.y; z op= v.z;\
		return *this;\
	}

#define VEC4_OPERATION(op)\
	template<typename X>\
	this_type operator op(const X& value)\
	{\
		return this_type(x op value, y op value, z op value, w op value);\
	}\
	template<typename X>\
	this_type operator op(const vec2<X>& v)\
	{\
		return this_type(x op v.x, y op v.y, z, w);\
	}\
	template<typename X>\
	this_type operator op(const vec3<X>& v)\
	{\
		return this_type(x op v.x, y op v.y, z op v.z, w);\
	}\
	template<typename X>\
	this_type operator op(const vec4<X>& v)\
	{\
		return this_type(x op v.x, y op v.y, z op v.z, w op v.w);\
	}\
	template<typename X>\
	const this_type& operator op=(const X& value)\
	{\
		x op= value; y op= value; z op= value; w op= value;\
		return *this;\
	}\
	template<typename X>\
	const this_type& operator op=(const vec2<X>& v)\
	{\
		x op= v.x; y op= v.y;\
		return *this;\
	}\
	template<typename X>\
	const this_type& operator op=(const vec3<X>& v)\
	{\
		x op= v.x; y op= v.y; z op= v.z;\
		return *this;\
	}\
	template<typename X>\
	const this_type& operator op=(const vec4<X>& v)\
	{\
		x op= v.x; y op= v.y; z op= v.z; w op= v.w;\
		return *this;\
	}

#pragma pack(push, 1)

template<typename T>class vec2;
template<typename T>class vec3;
template<typename T>class vec4;

template<typename T>
class vec2
{
public:
	typedef vec2<T> this_type;
	typedef T value_type;
	union{
		value_type data[2];
		struct{ value_type x, y; };
		struct{ value_type u, v; };
		struct{ value_type s, t; };
		struct{ value_type w, h; };
		struct{ value_type width, height; };
		struct{ value_type begin, end; };
		struct{ value_type start, size; };
	};
public:
	vec2():x(), y(){ /*void*/ }

	vec2(const value_type& _x, const value_type& _y):x(_x), y(_y) { /*void*/ }

	template<typename X>
	vec2(const vec2<X>& v):x(v.x), y(v.y) { /*void*/ }

	operator bool()const { return !x && !y; }

	this_type operator -()const { return this_type(-x, -y); }

	bool operator==(const this_type& v)const
	{
		return (x == v.x) && (y == v.y);
	}
	bool operator!=(const this_type& v)const
	{
		return (x != v.x) || (y != v.y);
	}

	bool operator<(const this_type& v)const
	{
		return x < v.x || y < v.y;
	}

	VEC2_OPERATION(+)
	VEC2_OPERATION(-)
	VEC2_OPERATION(*)
	VEC2_OPERATION(/)
public:
	value_type length_square()const
	{
		return x*x + y*y;
	}

	value_type length()const
	{
		return std::sqrt(static_cast<double>(length_square()));
	}

	value_type distance(const value_type& _x, const value_type& _y)const
	{
		return this_type(x-_x, y-_y).length();
	}

	value_type distance(const this_type& p)const
	{
		return this->operator-(p).length();
	}

	const this_type& offset(const value_type& _x, const value_type& _y)
	{
		x += _x;
		y += _y;
		return *this;
	}

	const this_type& offset(const vec2<T>& v)
	{
		return this->operator+=(v);
	}

	bool inside(const vec4<T>& rect)const
	{
		return rect.contains(x, y);
	}
};

template<typename T>
class vec3
{
public:
	typedef vec3<T> this_type;
	typedef T value_type;
	union{
		value_type data[3];
		struct{ value_type x, y, z; };
		struct{ value_type begin, end, current; };
		struct{ value_type start, size, step; };
		struct{ value_type r, g, b; };
		struct{ value_type red, green, blue; };
	};
public:
	vec3():x(), y(), z(){ /*void*/ }

	vec3(const value_type& _x, const value_type& _y, const value_type& _z = value_type())
		:x(_x), y(_y), z(_z) { /*void*/ }

	template<typename X>
	vec3(const vec2<X> v, const value_type& _z = value_type())
		:x(v.x), y(v.y), z(_z) { /*void*/}

	template<typename X>
	vec3(const vec3<X>& v):x(v.x), y(v.y), z(v.z){ /*void*/ }

	operator bool()const { return x && y && z; }

	this_type operator -()const { return this_type(-x, -y, -z); }

	bool operator==(const this_type& v)const
	{
		return (x == v.x) && (y == v.y) && (z == v.z);
	}

	bool operator!=(const this_type& v)const
	{
		return (x != v.x) || (y != v.y) || (z != v.z);
	}

	bool operator<(const this_type& v)const
	{
		return x < v.x || y < v.y || z < v.z;
	}

	VEC3_OPERATION(+)
	VEC3_OPERATION(-)
	VEC3_OPERATION(*)
	VEC3_OPERATION(/)
public:
	value_type length_square()const
	{
		return x*x + y*y + z*z;
	}

	value_type length()const
	{
		return std::sqrt(static_cast<double>(length_square()));
	}

	value_type distance(const value_type& _x, const value_type& _y)const
	{
		return this_type(x-_x, y-_y).length();
	}

	value_type distance(const value_type& _x, const value_type& _y, const value_type& _z)const
	{
		return this_type(x-_x, y-_y, z-_z).length();
	}

	value_type distance(const vec2<T>& v)const
	{
		return this->operator-(v).length();
	}

	value_type distance(const this_type& v)const
	{
		return this->operator-(v).length();
	}

	const this_type& offset(const value_type& _x, const value_type& _y)
	{
		x += _x;
		y += _y;
		return *this;
	}

	const this_type& offset(const value_type& _x, const value_type& _y, const value_type& _z)
	{
		x += _x;
		y += _y;
		z += _z;
		return *this;
	}

	const this_type& offset(const vec2<T>& v)
	{
		return this->operator+=(v);
	}

	const this_type& offset(const this_type& v)
	{
		return this->operator+=(v);
	}


	//点积
	value_type dot_product(const this_type& v)const
	{
		return x * v.x + y * v.y + z * v.z;
	}

	//乘积
	this_type cross_product(const this_type& v)const
	{
		return this_type(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x);
	}

	//归一化
	void normalize()
	{
		float m = 1.0f/(length() + FLT_EPSILON);
		x *= m;
		y *= m;
		z *= m;
	}
};

template<typename T>
class vec4
{
public:
	typedef vec4<T> this_type;
	typedef T value_type;
	union{
		value_type data[4];
		struct{
			value_type x, y;
			union{
				struct{ value_type z, w; };
				struct{ value_type width, height; };
			};
		};
		struct{ value_type left, top, right, bottom; };
		struct{ value_type r, g, b, a; };
		struct{ value_type red, green, blue, alpha; };
	};
public:
	vec4():x(), y(), z(), w(){ /*void*/ }

	vec4(const value_type& _x, const value_type& _y, 
		const value_type& _z = value_type(),
		const value_type& _w = value_type())
		:x(_x), y(_y), z(_z), w(_w) { /*void*/ }

	template<typename X>
	vec4(const vec2<X>& v,
		const value_type& _z = value_type(),
		const value_type& _w = value_type())
		:x(v.x), y(v.y), z(_z), w(_w) { /*void*/ }

	template<typename X>
	vec4(const vec2<X>& v1, const vec2<X>& v2)
		:x(v1.x), y(v1.y), z(v2.x), w(v2.y) { /*void*/ }

	template<typename X>
	vec4(const vec3<X> v, const value_type& _w = value_type())
		:x(v.x), y(v.y), z(v.y), w(_w) { /*void*/ }

	template<typename X>
	vec4(const vec4<X>& v):x(v.x), y(v.y), z(v.z), w(v.w) { /*void*/ }

	operator bool()const { return x && y && z && w; }

	this_type operator -()const { return this_type(-x, -y, -z, -w); }

	bool operator==(const this_type& v)const
	{
		return (x == v.x) && (y == v.y) && (z == v.z) && (w == v.w);
	}

	bool operator!=(const this_type& v)const
	{
		return (x != v.x) || (y != v.y) || (z != v.z) || (w != v.w);
	}

	bool operator<(const this_type& v)const
	{
		return x < v.x || y < v.y || z < v.z || w < v.w;
	}

	VEC4_OPERATION(+)
	VEC4_OPERATION(-)
	VEC4_OPERATION(*)
	VEC4_OPERATION(/)

public://向量操作
	value_type length_square()const
	{
		return x*x + y*y + z*z + w*w;
	}

	value_type length()const
	{
		return std::sqrt(static_cast<double>(length_sqare()));
	}

	value_type distance(const value_type& _x, const value_type& _y)
	{
		return this_type(x-_x, y-_y).length();
	}

	value_type distance(const value_type& _x, const value_type& _y, 
		const value_type& _z)const
	{
		return this_type(x-_x, y-_y, z-_z).length();
	}

	value_type distance(const value_type& _x, const value_type& _y, 
		const value_type& _z, const value_type& _w)const
	{
		return this_type(x-_x, y-_y, z-_z, w-_w).length();
	}

	value_type distance(const vec2<T>& v)const
	{
		return this->operator-(v).length();
	}

	value_type distance(const vec3<T>& v)const
	{
		return this->operator-(v).length();
	}

	value_type distance(const this_type& v)const
	{
		return this->operator-(v).length();
	}

	const this_type& offset(const value_type& _x, const value_type& _y)
	{
		x += _x;
		y += _y;
		return *this;
	}

	const this_type& offset(const value_type& _x, const value_type& _y, const value_type& _z)
	{
		x += _x;
		y += _y;
		z += _z;
		return *this;
	}

	const this_type& offset(const value_type& _x, const value_type& _y, const value_type& _z, const value_type& _w)
	{
		x += _x;
		y += _y;
		z += _z;
		w += _w;
		return *this;
	}

	const this_type& offset(const vec2<T>& v)
	{
		return this->operator+=(v);
	}

	const this_type& offset(const vec3<T>& v)
	{
		return this->operator+=(v);
	}

	const this_type& offset(const this_type& v)
	{
		return this->operator+=(v);
	}
public://rect操作
	value_type get_right()const
	{
		return x + width;
	}

	value_type get_bottom()const
	{
		return y + height;
	}

	/*
	void set_width(const value_type& width)
	{
		x2 = x1 + width;
	}

	void set_height(const value_type& width)
	{
		y2 = y1 + height;
	}
	*/

	//判断矩形是否不相交
	bool non_intersect(const this_type& rc)const
	{
		return x >= rc.get_right() || y >= rc.get_bottom() || get_right() <= rc.x || get_bottom() <= rc.y;
	}

	//判断矩形是否相交
	bool intersect(const this_type& rc)const
	{
		return !non_intersect(rc);
	}

	//判断矩形是否反转
	bool reversed()const
	{
		return width < 0 || height < 0;
	}

	//返回矩形相交区域或者
	this_type operator&(const this_type& rc)const
	{
		int max_x = std::max(x, rc.x);
		int max_y = std::max(y, rc.y);
		return this_type(
			max_x,
			max_y,
			std::min(get_right(), rc.get_right())-max_x,
			std::min(get_bottom(), rc.get_bottom())-max_y);
	}

	//获得包围两个矩形的区域AABB
	this_type operator|(const this_type& rc)
	{
		int min_x = std::min(x, rc.x);
		int min_y = std::min(y, rc.y);
		return this_type(
			min_x,
			min_y,
			std::max(get_right(), rc.get_right())-min_x,
			std::max(get_bottom(), rc.get_bottom())-min_y);
	}

	//面积
	value_type area()const
	{
		return width * height;
	}

	//矩形的面积是否为0
	bool empty_area()const
	{
		return is_zero(width) || is_zero(height);
	}

	//判断矩形面积是否相等
	bool same_area(const this_type& rc)const
	{
		return width == rc.width && height == rc.height;
	}

	//是否包含点
	bool contains(const value_type& _x, const value_type& _y)const
	{
		return _x >= x && _x <= get_right() && _y >= y && _y <= get_bottom();
	}

	bool contains(const vec2<T>& p)const
	{
		return contains(p.x, p.y);
	}

	//判断是否包含矩形
	bool contains(const this_type& rc)const
	{
		return x <= rc.x && y <= rc.y && get_right() >= rc.get_right() && get_bottom() >= rc.get_bottom();
	}

	//判断是否在rc内部
	bool inside(const this_type& rc)const
	{
		return rc.contain(*this);
	}

	//返回矩形中心点
	vec2<T> center()const
	{
		return vec2<T>(x + width/2, y + height/2);
	}


	//缩小
	const this_type& inflate(const value_type& size)
	{
		x += size;
		y += size;
		width  -= size*2;
		height -= size*2;
		return *this;
	};

	const this_type& inflate(const value_type& size_x, const value_type& size_y)
	{
		x += size_x;
		y += size_y;
		width  -= size_x*2;
		height -= size_y*2;
		return *this;
	};

	//放大
	const this_type& expand(const value_type& size)
	{
		x -= size;
		y -= size;
		width  += size*2;
		height += size*2;
		return *this;
	};

	const this_type& expand(const value_type& size_x, const value_type& size_y)
	{
		x1 -= size_x;
		y1 -= size_y;
		width  += size_x*2;
		height += size_y*2;
		return *this;
	};
};

#pragma pack(pop)

typedef vec2<int8_t> vec2b;
typedef vec3<int8_t> vec3b;
typedef vec4<int8_t> vec4b;

typedef vec2<uint8_t> vec2ub;
typedef vec3<uint8_t> vec3ub;
typedef vec4<uint8_t> vec4ub;

typedef vec2<short> vec2s;
typedef vec3<short> vec3s;
typedef vec4<short> vec4s;

typedef vec2<int> vec2i;
typedef vec3<int> vec3i;
typedef vec4<int> vec4i;

typedef vec2<float> vec2f;
typedef vec3<float> vec3f;
typedef vec4<float> vec4f;

} //end namespace cgl

#endif //CGL_VECTYPE_H_20150909203228
