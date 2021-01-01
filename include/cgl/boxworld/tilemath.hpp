/*

 tilemath.hpp

 sdragonx 2019-05-03 17:12:42

*/
#ifndef TILEMATH_HPP_20190503171242
#define TILEMATH_HPP_20190503171242

#include <cgl/public.h>
#include <cgl/vec.hpp>
#include <cgl/math.hpp>

//单位碰撞类型
//COLLISION_TYPE
const int COLLISION_BOOT = 1;
const int COLLISION_FLY  = 2;
const int COLLISION_RAY  = 4;
const int COLLISION_TELEPORT = 8;

//需要自己实现的函数
//只判断砖块是否可以通过
extern bool tilemap_is_through(void* instance, int x, int y, int mode);

//判断砖块是否透光
//extern bool tilemap_is_photic(int x, int y);

namespace boxworld{
using namespace cgl;

/*
int8
64x64   sizeof(TILE_POINT)=2  sizeof(TILE_CIRCLE)=130   sizeof(MAP_DATA)=520K    max_r = 90.5

uint8
128x128 sizeof(TILE_POINT)=2  sizeof(TILE_CIRCLE)=130   sizeof(MAP_DATA)=2.031M  max_r = 181

int16
256x256 sizeof(TILE_POINT)=4  sizeof(TILE_CIRCLE)=1028  sizeof(MAP_DATA)=64.25M  max_r = 362

tile.tp	使用	实际
64x64	262144  176800
128x128	2097152 1406272

路径图实际占用申请的大约0.67
*/

#pragma pack(push, 1)
template<typename T, size_t SIZE>
class tile_rule
{
public:
	typedef T value_type;
	typedef vec2<value_type> point;

	struct tile
	{
		value_type radius;	//半径
		value_type size;	//sizeof tp data
		point tp[SIZE];		//points to the center of circle
	};

private:
	static tile m_tiles[SIZE][SIZE];

public:
	static int init()
	{
		//初始化缓存数据
		std::vector<cgl::vec2i> ls;
		int r;
		float px, py;
		int total = 0;
		for(int y=0; y<SIZE; ++y){
			for(int x=0; x<SIZE; ++x){
				//计算半径
				px = x;
				py = y;
				//坐标修正
				px += 0.5f;
				py += 0.5f;
				float r = sqrt(px * px + py * py);
				m_tiles[y][x].radius = r;

				//计算圆心通往当前点需要经过的点位置
				math::line_points(ls, 0, 0, x, y);
				//得到的坐标中包含终点，要去掉终点坐标
				ls.pop_back();
				m_tiles[y][x].size = ls.size();

				for(size_t i=0; i<ls.size(); ++i){
					m_tiles[y][x].tp[i].x = ls[i].x;
					m_tiles[y][x].tp[i].y = ls[i].y;
				}
				total += ls.size();
			}
		}
		return 0;//m_tiles[0][0] = 0
	}

	static const tile& tiles(int x, int y)
	{
		static is_init = false;
		if(!is_init){
			init();
			is_init = true;
		}
		return m_tiles[y][x];
	}
};

//这样初始化不安全，第一行数据会被抹除为0
//template<typename T, size_t SIZE>
//tile_rule<T, SIZE>::tile tile_rule<T, SIZE>::m_tiles[SIZE][SIZE] = {tile_rule<T, SIZE>::init()};

template<typename T, size_t SIZE>
tile_rule<T, SIZE>::tile tile_rule<T, SIZE>::m_tiles[SIZE][SIZE] = {0};

#pragma pack(pop);

typedef tile_rule<int8_t, 64> tileRule;
//typedef tile_rule<uint8_t, 128> tileRule;


const tileRule::tile& tile_rules(int x, int y)
{
	if(x < 0)x = -x;
	if(y < 0)y = -y;
	if(x < 64 && y < 64){
		return tileRule::tiles(x, y);
	}
	return tileRule::tiles(0, 0);
}

int tilemap_radius(int x, int y)
{
	if(x < 0)x = -x;
	if(y < 0)y = -y;
	if(x < 64 && y < 64){
		return tileRule::tiles(x, y).radius;
	}
	else{
        return 0;
    }
}

//判断两点之间是否有障碍，最大操作数为半径64的圆
//速度比下面safe版的快很多，查表操作
bool tilemap_test_line(int ox, int oy, int x, int y, void* instance)
{
	int bx = x - ox;
	int by = y - oy;

	int sx = 1, sy = 1;//标记正负符号
	int mx, my;	//地图坐标

	if(bx < 0){
		bx = -bx;
		sx = -1;
	}
	if(by < 0){
		by = -by;
		sy = -1;
	}

	if(!tileRule::tiles(bx, by).size){//0,0 中心点
		return tilemap_is_through(instance, x, y, COLLISION_RAY);
	}

	const tileRule::tile& tc = tileRule::tiles(bx, by);

	for(int i = 0; i < tc.size; ++i){
		mx = tc.tp[i].x;
		mx *= sx;
		mx += ox;
		my = tc.tp[i].y;
		my *= sy;
		my += oy;
		if(!tilemap_is_through(instance, mx, my, COLLISION_RAY)){
			return false;
		}
	}
	return true;
}

//测试一条线上是否畅通
bool tilemap_test_line_safe(int ox, int oy, int x, int y, void* instance)
{
	//获得位置到中心的点集合
	std::vector<vec2i> ls;
	math::line_points(ls, ox, oy, x, y);
	ls.pop_back();//去掉终点坐标
	//和砖块进行碰撞，如果途中遇到砖块，这个光源位置就是暗的
	for(size_t i = 0; i < ls.size(); ++i){
		if(!tilemap_is_through(instance, ls[i].x, ls[i].y, COLLISION_FLY)){
			return false;
		}
	}

	return true;
}

}//end namespace boxworld

#endif //TILEMATH_HPP_20190503171242
