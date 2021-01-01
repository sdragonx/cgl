/*

 tilemap.hpp

 sdragonx 2019-05-26 14:19:17

*/
#ifndef TILEMAP_HPP_20190526141917
#define TILEMAP_HPP_20190526141917

#include <cgl/array2d.hpp>
#include <cgl/frame.hpp>
#include <cgl/graphics/teximage.hpp>
#include <cgl/xdoc.hpp>
#include "engine.hpp"

extern cgl::uniframe luna;

namespace boxworld{

// x 1 x
// 8 0 2
// x 4 x
static struct TILE_DIRECT{
    int x, y;
	int value;
}tileDirect[4] = {{0, -1, 1}, {1, 0, 2}, {0, 1, 4}, {-1, 0, 8}};

//统一加载砖块排序
//char tileset_loadOrder[16]  = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
static char rimworld_loadorder[16] = {12, 13, 14, 15, 8, 9, 10, 11, 4, 5, 6, 7, 0, 1, 2, 3};//rimworld砖块是反着的

class boxTileSet
{
public:
	string_t name;
	vec4ub color;
	teximage* m_image;
	graphics::teximage m_ls[16];

	boxTileSet() : m_image() { /*void*/ }

	void load(const char* file, const char loadorder[16] = null)
	{
		vec2i size;
		m_image = engine.loadImage(file, true, false);
		if(loadorder){
			graphics::teximage ls[16];
			m_image->split(ls, 4, 4);
			for(int i=0; i<16; ++i){
				m_ls[i] = ls[loadorder[i]];
			}
		}
		else{
			m_image->split(m_ls, 4, 4);
		}
	}

	void dispose()
	{
		if(m_image){
			//
		}
	}

	const graphics::teximage* image(int index)const
	{
		//assert
		return &m_ls[index];
	}

	const graphics::teximage* operator[](int index)const
	{
		return this->image(index);
	}
};

int tileset_LoadAtlas(const char* name, const char* file, const char* loadorder = null);
int tileset_count();
int tileset_getIndex(const string_t& name);
string_t tileset_getName(int index);
vec4ub tileset_getColor(int index);

const graphics::teximage* tileset_getImage(int index, int style);
const graphics::teximage* tileset_getImage(const string_t& tileset, int style);

void draw_tile(graphics::draw_context& dc, int x, int y, int tile, int style);

class boxMap
{
public:
	struct tile_type
	{
		int tile;
		uint8_t style;

		tile_type() : tile(), style(){ /*void*/ }
	};
public:
	array2d<tile_type> m_map;

	boxMap() : m_map()
	{
	}

	size_t size_x()const
	{
		return m_map.size_x();
	}

	size_t size_y()const
	{
		return m_map.size_y();
	}

	void resize(int x, int y)
	{
		m_map.resize(x, y);
	}

	void clear()
	{
		if(!m_map.empty()){
			tile_type t;
			m_map.fill(t);
		}
	}

	bool is_valid(int x, int y)const
	{
		return size_t(x) < m_map.size_x() && size_t(y) < m_map.size_y();
	}

	int tile(int x, int y)const
	{
		if(is_valid(x, y)){
			return m_map[y][x].tile;
		}
		return 0;
	}

	int style(int x, int y)const
	{
		if(is_valid(x, y)){
			return m_map[y][x].style;
		}
		return 0;
    }

	//计算当前砖块的样式
	int calc_tile_style(int x, int y)
	{
		//if(!map[y][x])return 0;

		int value = 0;
		int bx, by;
		for(int i=0; i<4; ++i){
			bx = x + tileDirect[i].x;
			by = y + tileDirect[i].y;
			if(is_valid(bx, by)){
				if(m_map[by][bx].tile){
					value += tileDirect[i].value;
				}
			}
		}
		return value;
    }

	int set_tile(int x, int y, int tile)
	{
		if(!this->is_valid(x, y)){
			return -1;
		}

		if(this->tile(x, y) == tile){
			return -2;
		}

		int value = 0;
		if(tile){
			value = calc_tile_style(x, y);
			m_map[y][x].style = value;
		}
		else{
            m_map[y][x].style = 0;
        }

		m_map[y][x].tile = tile;

		//更新周围砖块
		//on_setTile
		int bx, by;
		for(int i=0; i<4; ++i){
			bx = x + tileDirect[i].x;
			by = y + tileDirect[i].y;
			if(is_valid(bx, by)){
				tile = m_map[y + tileDirect[i].y][x + tileDirect[i].x].tile;
				if(tile){
					value = calc_tile_style(bx, by);
					m_map[by][bx].style = value;
				}
			}
		}

		return 0;
	}

	//绘制一个地图
	void draw_map(int bx, int by, const char* map, int width, int height)
	{
		for(int y=0; y<height; ++y){
			for(int x=0; x<width; ++x){
				this->set_tile(x, y, map[x]);
			}
			map += height;
		}
	}

//---------------------------------------------------------------------------
//save and load

	int save(xnode<char> node)
	{
		std::vector<int> buf;
		buf.resize(m_map.size_x());

		//保存当前索引
		xnode<char> ls = node.add_node("tile");
		for(int i=0; i<tileset_count(); ++i){
			string_t s = tileset_getName(i);
			xnode<char> ts = ls.add_string(string_t(i), s);
			//node.add_value(string_t(i), s);
		}

		//保存地图
		for(size_t y=0; y<m_map.size_y(); ++y){
			xnode<char> node_y = node.add_node(string_t(int(y)));
			copy_line(&buf[0], y);
			node_y.join(&buf[0], m_map.size_x());
		}
		return 0;
	}

	int load(xnode<char> node)
	{
		std::vector<int> buf;
		buf.resize(m_map.size_x());

		//读取砖块索引
		std::map<int, string_t> store;
		xnode<char> ls = node.subnode("tile");
		if(ls){
			xnode<char> t = ls.first();
			store[0] = "null";
			while(t){
				store[t.name().to_int()] = t.value();
				t = t.next();
			};
		}
		else{
			return -1;
		}

		//加载地图
		int index;
		for(size_t y=0; y<m_map.size_y(); ++y){
			xnode<char> n = node.attribute(string_t(int(y)));
			if(!n.is_null()){
				n.detach<int>(&buf[0], m_map.size_x());
				for(size_t x=0; x<m_map.size_x(); ++x){
					string_t name = store[buf[x]];
					index = tileset_getIndex(name);
					this->set_tile(x, y, index);
				}
			}
		}
		return 0;
	}
private:
	void copy_line(int* p, int y)
	{
		for(size_t x=0; x<m_map.size_x(); ++x){
			p[x] = m_map[y][x].tile;
		}
	}

};

class boxFog
{
public:
	array2d<char> m_map;

	int width()
	{
		return m_map.size_x();
	}

	int height()
	{
		return m_map.size_y();
	}

	int resize(size_t w, size_t h)
	{
		m_map.resize(w, h);

		m_map.fill(0);
		return 0;
	}

	char& at(int x, int y)
	{
		return m_map[y][x];
    }

	void add(int x, int y, int n)
	{
		char& t = this->at(x, y);
		t |= n;
	}

	void sub(int x, int y, int n)
	{
		char& t = this->at(x, y);
		t &= ~n;
	}

	//更新迷雾区域
	void set(int x, int y)
	{
		if(!m_map.is_valid(x, y)){
			return ;
		}
		add(x,   y,   1);
		add(x+1, y,   2);
		add(x,	 y+1, 4);
		add(x+1, y+1, 8);
	}

	//更新迷雾区域
	void reset(int x, int y)
	{
		if(!m_map.is_valid(x, y)){
			return ;
		}
		sub(x,   y,   1);
		sub(x+1, y,   2);
		sub(x,	 y+1, 4);
		sub(x+1, y+1, 8);
	}

    //重置整个地图的阴影
	void reset_map_shadow(boxMap& map)
	{
		m_map.fill(0);
		for(size_t y=0; y<map.size_y(); ++y){
		for(size_t x=0; x<map.size_x(); ++x){
			if(map.tile(x, y) && map.tile(x + 1, y) &&
				map.tile(x, y + 1) && map.tile(x + 1, y + 1)){
				this->set(x, y);
			}
		}
		}
	}

	void fill_shadow(boxMap& map, int x, int y)
	{
		//四个角的砖块
		int t0 = map.tile(x - 1, y - 1);
		int t1 = map.tile(x + 1, y - 1);
		int t2 = map.tile(x - 1, y + 1);
		int t3 = map.tile(x + 1, y + 1);

		int style = map.style(x, y);
		switch(style)
		{
		case 3:
			if(t1)set(x, y - 1);
			break;
		case 6:
			if(t3)set(x, y);
			break;
		case 7:
			if(t1)set(x, y - 1);
			if(t3)set(x, y);
			break;
		case 9:
			if(t0)set(x - 1, y - 1);
			break;
		case 11:
			if(t0)set(x - 1, y - 1);
			if(t1)set(x, y - 1);
			break;
		case 12:
			if(t2)set(x - 1, y);
			break;
		case 13:
			if(t0)set(x - 1, y - 1);
			if(t2)set(x - 1, y);
			break;
		case 14:
			if(t2)set(x - 1, y);
			if(t3)set(x, y);
		case 15:
			if(t0)set(x - 1, y - 1);
			if(t1)set(x, y - 1);
			if(t2)set(x - 1, y);
			if(t3)set(x, y);
			break;
		}
	}

	void reset_shadow(int x, int y)
	{
		reset(x - 1, y - 1);
		reset(x - 0, y - 1);
		reset(x - 1, y - 0);
		reset(x - 0, y - 0);
	}

	void paint(draw_context& dc, const vec4f& viewport)
	{
		char t;

		//只绘制可视范围
		vec4i range = draw_range(width(), height(), viewport, WORLD_TILE_SIZE);
		float px, py;

		++range.ymax;
		if(range.ymax >= m_map.size_y()){
			range.ymax = m_map.size_y();
		}

		dc.color = vec4ub(0, 0, 0, 255);
		for(int y=range.ymin; y<range.ymax; ++y){
			py = y * WORLD_TILE_SIZE - 24;
			for(int x=range.xmin; x<range.xmax; ++x){
				px = x * WORLD_TILE_SIZE;
				t = this->at(x, y);
				if(!t)continue;
				t = 15 - t;
				dc.draw_image(&engine.image.shadow[t], px, py, WORLD_TILE_SIZE, WORLD_TILE_SIZE);
			}
		}
	}
};


}//end namespace boxworld

//#include "tilemap.cpp"

#endif //TILEMAP_HPP_20190526141917
