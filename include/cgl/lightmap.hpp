/*

 lightmap.hpp

 sdragonx 2018-08-20 13:36:28


*/
#ifndef LIGHTMAP_HPP_20180820133628
#define LIGHTMAP_HPP_20180820133628

#include <cgl/array2d.hpp>
#include <cgl/math.hpp>
#include <cgl/graphics/draw_context.hpp>
#include "tilemath.hpp"

namespace cgl{

//
//基于砖块的光照映射图
//

template<size_t LIGHT_MAP_TILE_SIZE = 10>
class lightmap
{
public:
	array2d<float> m_map;

public:
	void* instance;
	bool against; //禁止光照？？？

	lightmap() : m_map()
	{
        against = true;
    }

	void init(size_t x, size_t y)
	{
		m_map.resize(x, y);
	}

	void reset()
	{
		m_map.fill(0.0f);
	}

	//光线衰减
	void damping()
	{
		for(int y=0; y<m_map.size_y(); ++y){
			for(int x=0; x<m_map.size_x(); ++x){
				m_map[y][x] *= 0.9f;
				if(m_map[y][x] < 0.0f){
					m_map[y][x] = 0;
				}
			}
		}
	}

	//添加一个光源，输入的是世界坐标
	void append_light(int wx, int wy, int size = 5)
	{
		//int mx = (wx + LIGHT_MAP_TILE_SIZE - 1)/LIGHT_MAP_TILE_SIZE;
		//int my = (wy + LIGHT_MAP_TILE_SIZE - 1)/LIGHT_MAP_TILE_SIZE;
		//世界坐标转换block坐标，为光源圆心
		int bx = wx / LIGHT_MAP_TILE_SIZE;
		int by = wy / LIGHT_MAP_TILE_SIZE;

		//范围约束
		int r = size * LIGHT_MAP_TILE_SIZE;
		int x1 = clamp(bx - size, m_map.size_x());
		int y1 = clamp(by - size, m_map.size_y());
		int x2 = clamp(bx + size + 0, m_map.size_x());
		int y2 = clamp(by + size + 0, m_map.size_y());

		int p1, p2;
		float d;
		//std::vector<vec2i> ls;

		for(int y = y1; y< y2; ++y)
		for(int x = x1; x< x2; ++x)
		{
			/*
			bool test = true;

			//获得位置到中心的点集合
			math::line_points(ls, mx, my, x, y);
			//和砖块进行碰撞，如果途中遇到砖块，这个光源位置就是暗的
			for(size_t m = 0; m < ls.size(); ++m){
				if(is_wall(ls[m].x, ls[m].y)){//map[ls[m].y][ls[m].x].value){
					test = false;
					break;
				}
			}

			if(!test){//测试失败，砖块为暗
				//m_map[y][x] = 0;
				continue;
			}
			*/
			if(against && !boxworld::tilemap_test_line(bx, by, x, y, instance)){
				continue;
			}

			p1 = x * LIGHT_MAP_TILE_SIZE + LIGHT_MAP_TILE_SIZE / 2;//偏移半个方格的像素
			p1 = wx - p1;
			p1 = pow(p1);
			p2 = y * LIGHT_MAP_TILE_SIZE + LIGHT_MAP_TILE_SIZE / 2;
			p2 = wy - p2;
			p2 = pow(p2);
			d = sqrt(float(p1 + p2));//根据距离，计算亮度
			d = r - d;
			if(d >= r ) d = r;
			if(d < 0) d = 0;
			//m_map[y][x] += (d / r) * 0.5f;
			m_map[y][x] += d / r;
		}
	}

	float operator()(size_t x, size_t y)
	{
		return m_map[y][x];
	}

	void drawtile(graphics::draw_context& dc, int x, int y, int image,
		float u1 = 0.0f, float v1 = 0.0f, float u2 = 1.0f, float v2 = 1.0f)
	{
		int vx = x * LIGHT_MAP_TILE_SIZE;
		int vy = y * LIGHT_MAP_TILE_SIZE;
		dc.color = blend(color::white, m_map[y][x]);
		dc.draw_image(image, vx, vy, LIGHT_MAP_TILE_SIZE, LIGHT_MAP_TILE_SIZE, u1, v1, u2, v2);
	}

	void fill_shadow(graphics::draw_context& dc, int x, int y)
	{
        vtx3f v;
		int vx, vy;

		vx = x * LIGHT_MAP_TILE_SIZE;
		vy = y * LIGHT_MAP_TILE_SIZE;

		dc.color = blend(color::white, m_map[y][x]);
		dc.fill_rect(vx, vy, LIGHT_MAP_TILE_SIZE, LIGHT_MAP_TILE_SIZE);
	}

private:
	size_t clamp(int size, size_t max)
	{
		if(size < 0){
			size = 0;
		}
		return size < max ? size : max;
	}

	inline int pow(int n)
	{
		return n * n;
	}

	vec4ub blend(vec4ub color, float a)
	{
		color.red *= a;
		color.green *= a;
		color.blue *= a;
		return color;
	}
};

//
//基于顶点的光照映射图
//

template<typename T, size_t LIGHT_MAP_TILE_SIZE = 16>
class vertex_lightmap
{
public:
	typedef T value_type;

	int width;
	int height;
	//array2d<float> m_map;
	array2d<value_type> m_map;
	//float ambient;	//环境光
	int ambient;

public:
	void* instance;
	bool against;	//是否启用光照碰撞，墙不透光

	vertex_lightmap() :
		width(),
		height(),
		m_map(),
		instance(),
		against(true)
	{
	}

	void init(int x, int y)
	{
		m_map.resize(x + 1, y + 1);
		width = x;
		height = y;
	}

	void reset(value_type value = 0.0f)
	{
		m_map.fill(value);
		ambient = value;
	}

	bool is_valid(int x, int y)const
	{
        return x >= 0 && y >= 0 && x < width && y < height;
	}

	//光线衰减
	void damping()
	{
		for(int y=0; y<m_map.size_y(); ++y){
			for(int x=0; x<m_map.size_x(); ++x){
				dumping(m_map[y][x]);
			}
		}
		dumping(ambient);
	}

	float luminance(int x, int y)const
	{
		if(is_valid(x, y)){
			return m_map[y][x];
		}
		return 0.0f;
	}

	/*
	float reverse_luminance(int x, int y)const
	{
		if(is_valid(x, y)){
			return 1.0f - m_map[y][x];
		}
		return 1.0f - ambient;
	}
	*/

	int reverse_luminance(int x, int y)const
	{
		if(is_valid(x, y)){
			return 255 - m_map[y][x];
		}
		return 255 - ambient;
	}

	float operator()(size_t x, size_t y)const
	{
		return m_map[y][x];
	}

	int get_alpha(int x, int y)
	{
		float l = m_map[y][x];
		int n = 255 * l;
		if(n > 255)n = 255;
		return n;
	}

	//和light混合
	int get_alpha(int x, int y, float light)
	{
		light += m_map[y][x];
		int n = 255 * light;
		if(n > 255)n = 255;
		return n;
	}

	void set_alpha(int x, int y, int alpha)
	{
		m_map[y][x] = alpha / 255.0f;
	}

	//世界坐标
	void append_light(int wx, int wy, int size)
	{
		const int dmin = size;
		const int dmax = dmin + 1;
		const int length = dmin * LIGHT_MAP_TILE_SIZE;
		//中心点
		//这里不用位置修正
		int bx = wx / LIGHT_MAP_TILE_SIZE;
		int by = wy / LIGHT_MAP_TILE_SIZE;

		int vx, vy;
		int a, b;
		int c;

		vec4ub color = color::white;

		//范围约束
		int x1 = clamp(bx - size, width);
		int y1 = clamp(by - size, height);
		int x2 = clamp(bx + size + 1, width);
		int y2 = clamp(by + size + 1, height);

		for(int y = y1; y < y2; ++y)
		for(int x = x1; x < x2; ++x){
			vx = x;
			vy = y;

			//位置修正。绘制的矩形是个斜菱形，阴影有半个矩形的误差
			if(vx > bx)--vx;
			if(vy > by)--vy;

//			if(vx + bx)++vx;
//			if(vy + by)++vy;

//			if(!is_valid(vx, vy)){
//				continue;
//			}

			//光照碰撞
			if(against && !boxworld::tilemap_test_line(bx, by, vx, vy, instance)){
				continue;
			}

			a = x * LIGHT_MAP_TILE_SIZE;// + LIGHT_MAP_TILE_SIZE / 2;
			a = wx - a;
			a *= a;
			b = y * LIGHT_MAP_TILE_SIZE;// + LIGHT_MAP_TILE_SIZE / 2;
			b = wy - b;
			b *= b;

			/*
			c = std::sqrt(float(a + b));
			if(c < length){//根据距离计算亮度
				c /= length;
				c = 1.0f - c;

				m_map[y][x] += c;
				if(m_map[y][x] > 1.0f) m_map[y][x] = 1.0f;
			}
			*/
			c = math::sqrti32(a + b);
			if(c < length){//根据距离计算亮度
				c = length - c;
				c = c * 255 / length;
				c += m_map[y][x];
				if(c > 255)c = 255;
				m_map[y][x] = c;
			}
		}
	}

	//生成可以绘制的4个顶点
//	void draw_cell(int x, int y)
//	{
//		vec4ub a, b;
//		vec4ub c, d;
//	}

	#ifdef DRAW_CONTEXT_HPP_20151111123500

	/*
	绘制：
	dc.color = color::white;
	bind_texture(...);
	lightmap.drawtile(dc, x, y, tex_tile);
	}
	*/

	//ok
	void drawtile(graphics::draw_context& dc, int x, int y, int image,
		float u1 = 0.0f, float v1 = 0.0f, float u2 = 1.0f, float v2 = 1.0f)
	{
        vtx3f v;
		int vx, vy;

		vx = x * LIGHT_MAP_TILE_SIZE;
		vy = y * LIGHT_MAP_TILE_SIZE;

		dc.bind_texture(image);
		v.color = dc.color;

		dc.begin(CGL_QUADS);
		v.x = vx;
		v.y = vy;
		//v.color.alpha = get_alpha(x, y);
		v.color = blend(dc.color, luminance(x, y));
		v.u = u1;
		v.v = v1;
		dc.vertex(v);

		v.x = vx + LIGHT_MAP_TILE_SIZE;
		v.y = vy;
		//v.color.alpha = get_alpha(x + 1, y);
		v.color = blend(dc.color, luminance(x + 1, y));
		v.u = u2;
		v.v = v1;
		dc.vertex(v);

		v.x = vx + LIGHT_MAP_TILE_SIZE;
		v.y = vy + LIGHT_MAP_TILE_SIZE;
		//v.color.alpha = get_alpha(x + 1, y + 1);
		v.color = blend(dc.color, luminance(x + 1, y + 1));
		v.u = u2;
		v.v = v2;
		dc.vertex(v);

		v.x = vx;
		v.y = vy + LIGHT_MAP_TILE_SIZE;
		//v.color.alpha = get_alpha(x, y + 1);
		v.color = blend(dc.color, luminance(x, y + 1));
		v.u = u1;
		v.v = v2;
		dc.vertex(v);

		dc.end();
	}

	void fill_shadow(graphics::draw_context& dc, int x, int y)
	{
        vtx3f v;
		int vx, vy;

		vx = x * LIGHT_MAP_TILE_SIZE;
		vy = y * LIGHT_MAP_TILE_SIZE;

		v.color = dc.color;
		int alpha = dc.color.alpha;

		dc.begin(CGL_QUADS);
		v.x = vx;
		v.y = vy;
		v.color.alpha = (alpha * reverse_luminance(x, y)) >> 8;
		dc.vertex(v);

		v.x = vx + LIGHT_MAP_TILE_SIZE;
		v.y = vy;
		v.color.alpha = (alpha * reverse_luminance(x + 1, y)) >> 8;
		dc.vertex(v);

		v.x = vx + LIGHT_MAP_TILE_SIZE;
		v.y = vy + LIGHT_MAP_TILE_SIZE;
		v.color.alpha = (alpha * reverse_luminance(x + 1, y + 1)) >> 8;
		dc.vertex(v);

		v.x = vx;
		v.y = vy + LIGHT_MAP_TILE_SIZE;
		v.color.alpha = (alpha * reverse_luminance(x, y + 1)) >> 8;
		dc.vertex(v);

		dc.end();
	}

	//两个光照图混合
	void drawtile(graphics::draw_context& dc, int x, int y, int image, const vertex_lightmap& lmap)
	{
        vtx3f v;
		int vx, vy;

		vx = x * BLOCK_SIZE;
		vy = y * BLOCK_SIZE;

		dc.bind_texture(image);
		v.color = dc.color;

		dc.begin(CGL_TRIANGLE_FAN);
		v.x = vx;
		v.y = vy;
		v.color.alpha = get_alpha(x, y, lmap.luminance(x, y));
		dc.vertex(v);

		v.x = vx + BLOCK_SIZE;
		v.y = vy;
		v.color.alpha = get_alpha(x + 1, y, lmap.luminance(x + 1, y));
		dc.vertex(v);

		v.x = vx + BLOCK_SIZE;
		v.y = vy + BLOCK_SIZE;
		v.color.alpha = get_alpha(x + 1, y + 1, lmap.luminance(x + 1, y + 1));
		dc.vertex(v);

		v.x = vx;
		v.y = vy + BLOCK_SIZE;
		v.color.alpha = get_alpha(x, y + 1, lmap.luminance(x, y + 1));
		dc.vertex(v);

		dc.end();
    }

	void paint(graphics::draw_context& dc)
	{
		vtx3f v;
		int vx, vy;

		if(m_map.empty())return ;

		dc.bind_texture(0);
		v.color = dc.color;
		for(int y=0; y < height; ++y)
		for(int x=0; x < width; ++x)
		{
			vx = x * BLOCK_SIZE;
			vy = y * BLOCK_SIZE;

			dc.begin(CGL_QUADS);
			v.x = vx;
			v.y = vy;
			v.color.alpha = get_alpha(x, y);
			dc.vertex(v);

			v.x = vx + BLOCK_SIZE;
			v.y = vy;
			v.color.alpha = get_alpha(x + 1, y);
			dc.vertex(v);

			v.x = vx + BLOCK_SIZE;
			v.y = vy + BLOCK_SIZE;
			v.color.alpha = get_alpha(x + 1, y + 1);
			dc.vertex(v);

			v.x = vx;
			v.y = vy + BLOCK_SIZE;
			v.color.alpha = get_alpha(x, y + 1);
			dc.vertex(v);

			dc.end();
        }
	}

	#endif //DRAW_CONTEXT_HPP_20151111123500

private:
	size_t clamp(int size, size_t max)
	{
		if(size < 0){
			size = 0;
		}
		return size < max ? size : max;
	}

	vec4ub blend(vec4ub color, float a)
	{
		color.red *= a;
		color.green *= a;
		color.blue *= a;
		return color;
	}

	inline void dumping(float& value)
	{
		value *= 0.9f;
		if(value < 0.0f){
			value = 0;
		}
	}

	template<typename T>
	inline void dumping(T& value)
	{
		if(value < 16){
			value = 0;
		}
		else{
			value -= 16;
		}
	}
};


}//end namespace cgl

#endif //LIGHTMAP_HPP_20180820133628
