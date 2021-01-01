/*

 tilemath.hpp

 sdragonx 2019-05-03 17:12:42

*/
#ifndef TILEMATH_HPP_20190503171242
#define TILEMATH_HPP_20190503171242

#include <cgl/public.h>
#include <cgl/vec.hpp>
#include <cgl/math.hpp>

//��λ��ײ����
//COLLISION_TYPE
const int COLLISION_BOOT = 1;
const int COLLISION_FLY  = 2;
const int COLLISION_RAY  = 4;
const int COLLISION_TELEPORT = 8;

//��Ҫ�Լ�ʵ�ֵĺ���
//ֻ�ж�ש���Ƿ����ͨ��
extern bool tilemap_is_through(void* instance, int x, int y, int mode);

//�ж�ש���Ƿ�͸��
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

tile.tp	ʹ��	ʵ��
64x64	262144  176800
128x128	2097152 1406272

·��ͼʵ��ռ������Ĵ�Լ0.67
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
		value_type radius;	//�뾶
		value_type size;	//sizeof tp data
		point tp[SIZE];		//points to the center of circle
	};

private:
	static tile m_tiles[SIZE][SIZE];

public:
	static int init()
	{
		//��ʼ����������
		std::vector<cgl::vec2i> ls;
		int r;
		float px, py;
		int total = 0;
		for(int y=0; y<SIZE; ++y){
			for(int x=0; x<SIZE; ++x){
				//����뾶
				px = x;
				py = y;
				//��������
				px += 0.5f;
				py += 0.5f;
				float r = sqrt(px * px + py * py);
				m_tiles[y][x].radius = r;

				//����Բ��ͨ����ǰ����Ҫ�����ĵ�λ��
				math::line_points(ls, 0, 0, x, y);
				//�õ��������а����յ㣬Ҫȥ���յ�����
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

//������ʼ������ȫ����һ�����ݻᱻĨ��Ϊ0
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

//�ж�����֮���Ƿ����ϰ�����������Ϊ�뾶64��Բ
//�ٶȱ�����safe��Ŀ�ܶ࣬������
bool tilemap_test_line(int ox, int oy, int x, int y, void* instance)
{
	int bx = x - ox;
	int by = y - oy;

	int sx = 1, sy = 1;//�����������
	int mx, my;	//��ͼ����

	if(bx < 0){
		bx = -bx;
		sx = -1;
	}
	if(by < 0){
		by = -by;
		sy = -1;
	}

	if(!tileRule::tiles(bx, by).size){//0,0 ���ĵ�
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

//����һ�������Ƿ�ͨ
bool tilemap_test_line_safe(int ox, int oy, int x, int y, void* instance)
{
	//���λ�õ����ĵĵ㼯��
	std::vector<vec2i> ls;
	math::line_points(ls, ox, oy, x, y);
	ls.pop_back();//ȥ���յ�����
	//��ש�������ײ�����;������ש�飬�����Դλ�þ��ǰ���
	for(size_t i = 0; i < ls.size(); ++i){
		if(!tilemap_is_through(instance, ls[i].x, ls[i].y, COLLISION_FLY)){
			return false;
		}
	}

	return true;
}

}//end namespace boxworld

#endif //TILEMATH_HPP_20190503171242
