/*

 boxworld.hpp

 sdragonx 2017-02-10 14:42:18

 2019/6/2 22:53

 physics

 一个轻量级的物理引擎，基于tile地形
 body的坐标是body bounds的中心

 float mass = density * (size.x * size.y);
 float inertia = mass * (size.x * size.x + size.y * size.y);

 刚体卡到死角还是无法解决啊

*/
#ifndef BOXWORLD_HPP_20170210144218
#define BOXWORLD_HPP_20170210144218

#include <set>
#include <cgl/public.h>
#include <cgl/array.hpp>
#include <cgl/vec.hpp>
#include <cgl/math/collision.hpp>
#include "tilemath.hpp"

namespace boxworld{
extern int WORLD_TILE_SIZE;
extern float INV_TILE_SIZE;
}

template<typename T>
inline void set_attrib(T& att, T value)
{
	att |= value;
}

template<typename T>
inline void reset_attrib(T& att, T value)
{
	att &= ~value;
}

namespace cgl{
//namespace physics{

using boxworld::WORLD_TILE_SIZE;
using boxworld::INV_TILE_SIZE;

vec2i get_tile(float x, float y)
{
    int tx = round(x);
    int ty = round(y);
    tx /= WORLD_TILE_SIZE;
    ty /= WORLD_TILE_SIZE;
    return vec2i(tx, ty);
}

inline vec2i get_tile(const vec2f& position)
{
    return get_tile(position.x, position.y);
}

//刚体和砖块碰撞修正值
#ifndef WORLD_TEST_FIX
#define WORLD_TEST_FIX  10  //(WORLD_TILE_SIZE/10) 1？
#endif

class boxBody;
class boxTile;
class boxWorld;

//圆形冲击力
void impulse_wave(boxWorld* world,
	int x, int y,	//位置，世界坐标
	int size,		//范围，圆形
	float force);	//威力

int body_repulsive(boxWorld* world, boxBody* body, float force);

class boxBody
{
public:
	enum TYPE
	{
		BODY_BOX = 0,	//默认box
		BODY_BALL,		//这个目前效果不好
		BODY_BULLET		//子弹，遇到碰撞就消失，判断是否消失
	};

	enum ATTRIBUTE
	{
		STATE_STATIC    = 1,	//静态对象，不参与碰撞
		STATE_SLEEP     = 2,	//是否休眠
		STATE_REPULSION = 4,	//是否启用互斥
        STATE_REVISE    = 0x80  //两个刚体纠缠到一块，进行修正
	};

	friend class boxWorld;
	friend class boxTile;

private:
	byte_t m_shape;		//0 圆形 1 矩形
	byte_t m_attrib;	//是否启用互斥
	vec2b  m_test;		//碰撞向量
	vec2f  m_position; 	//位置
	vec2f  m_size;     	//大小
	vec2f  m_velocity; 	//线速度
	vec2f  m_impulse;  	//冲力
	//float m_mass;		//质量
	float  m_restitution;//弹力。发生碰撞之后，速度向量的反弹比率 0~1(默认0，不反弹)
	float  m_friction;	//摩擦力。发生碰撞后，向量速度的衰减0~1(0为没有摩擦力，默认0.5)
	//angular_velocity	//角速度

	boxWorld* m_world;
	boxTile*  m_tile;
	boxBody*  m_next;

public://user
	float  scale;		//力度缩放
	void*  data;		//用户数据

public:
	boxBody();
	~boxBody();

	boxWorld* world()const { return m_world; }
	boxBody* next()const { return m_next; }

	vec4f bounds()const;

	int shape()const { return m_shape; }
	void set_shape(int value) { m_shape = value; }

	//线性力，叠加速度，跳跃
	vec2f velocity()const { return m_velocity; }
	void append_force(float x, float y);
	void set_force(float x, float y);

	//float force()const{ return (m_velocity + m_impulse).length(); }

	//瞬时冲力，行走
	vec2f impulse()const { return m_impulse; }
	void set_impulse(float x, float y);

	//弹力
	float restitution()const;
	void set_restitution(float value);

	//摩擦力
	float friction()const;
	void set_friction(float value);

	vec2f position()const;
	void set_position(float x, float y);

	vec2f size()const;
	void set_size(float x, float y);

	bool is_sleep()const { return m_attrib & STATE_SLEEP; };
	void sleep() { m_attrib |= STATE_SLEEP; }
	void weakup() { m_attrib &= ~STATE_SLEEP; }

	bool is_repulsion()const{ return m_attrib & STATE_REPULSION; }
	void set_repulsion(bool value)
	{
		m_attrib = value ? (m_attrib | STATE_REPULSION) : (m_attrib & (~STATE_REPULSION));
	}

	bool is_static()const { return m_attrib & STATE_STATIC; }
	void set_static(bool value);

    void set_revise(bool value)
    {
        value ? m_attrib |= STATE_REVISE : m_attrib &= ~STATE_REVISE;
    }

    bool is_revise()const
    {
        return m_attrib & STATE_REVISE;
    }

	//返回最后一次碰撞向量
	vec2b test()const { return m_test; }
};

class boxTile
{
protected:
	friend class boxWorld;
	boxBody* m_bodys;

public://user
	int16_t const x;//tile位置
	int16_t const y;
	//int water;  //水量
	//int8_t temp;   //温度
	//int is_through; //是否可以通过0为无法通过 is_through(x, y, void*data);
	//float photic; //是否透光。透光度
	//int value;

	//vec2f vec;	//地板产生的作用力，走上去会向某个方向推进
	//float scale;	//地板对力的缩放

	int value;
	void* data;

public:
	boxTile();
	boxTile(int _x, int _y);
	~boxTile();

	void addBody(boxBody* body);
	void eraseBody(boxBody* body);
	boxBody* bodys()const { return m_bodys; }

	boxBody* last()const
	{
		boxBody* body = m_bodys;
		while(body){
			if(body->next()){
				body = body->next();
			}
			else{
                break;
            }
		}
		return body;
	}

	/*
	void sort()
	{
		if(!m_bodys || !m_bodys->next()){
			return ;
		}

		boxBody* bodys = m_bodys;
		boxBody* body = bodys->next();
		boxBody* prev;
		boxBody* next;
		boxBody* temp;
		bodys->m_next = null;
		while(body){
			prev = null;
			next = bodys;

			while(true){
				if(body->position().y < next->position().y){
					if(next->next()){
						prev = next;
						next = next ->next();
					}
					else{//末尾
						next->m_next = body;
						body = body->next();
						next->m_next->m_next = null;
						break;
					}
				}
				else{
					if(prev){
						temp = prev->next();
						prev->m_next = body;
						body = body->next();
						prev->m_next = temp;
						break;
					}
					else{//first
						temp = body;
						body = body->next();
						temp->m_next = bodys;
						bodys = temp;
						break;
					}
				}
			}
		}
		m_bodys = bodys;
	}
    */

	//void on_remove();   //如果这个砖块被敲掉了，执行一下这个

	const boxTile& operator=(const boxTile& tile);
};

class boxWorld
{
public:
	typedef std::set<boxBody*>::iterator iterator;
	typedef std::set<boxBody*>::const_iterator const_iterator;

	friend class boxBody;

	vec2f gravity;		//世界重力
	bool auto_sleep;	//是否允许刚体自动休眠 default = true
	bool body_test;		//开启刚体碰撞测试mass?

	void* instance;

	//碰撞回调函数。bodyA是主动刚体，bodyB是被动刚体(可以是null)，tile是和bodyA进行碰撞的砖块
	//返回值为-1，表示bodyA被删除掉了
	int (*on_collision)(boxBody* bodyA, boxBody* bodyB, vec2i tile);

public:
	array2<boxTile> m_map;
	std::set<boxBody*> bodyList;
	std::set<boxBody*> bodyStatic;
	std::set<boxBody*> bullets;

public:
	boxWorld() :
        gravity(0.0f, 0.0f),
		auto_sleep(true),
		body_test(true),
		instance(),
		on_collision(),

		m_map(),
		bodyList(),
		bodyStatic(),
		bullets()
	{
	}

	~boxWorld()
	{
		this->dispose();
	}

	int create(int width, int height);
	void dispose();

	size_t size_x()const { return m_map.size_x(); }
	size_t size_y()const { return m_map.size_y(); }

//	int width()const { return size_x() * WORLD_TILE_SIZE; }
//	int height()const { return size_y() * WORLD_TILE_SIZE; }

	bool is_valid(int x, int y)const
	{
		return m_map.is_valid(x, y);
    }

	boxTile* tile(int x, int y)
	{
		if(!is_valid(x, y)){
			return null;
		}
		return &m_map[y][x];
	}

	const boxTile* tile(int x, int y)const
	{
		if(!is_valid(x, y)){
			return null;
		}
		return &m_map[y][x];
	}

	/*
	int is_through(int x, int y)const
	{
		const boxTile *t = this->at(x, y);
		return t ? t->is_through : 0;
	}

	int set_through(int x, int y, int value)
	{
		boxTile *t = this->at(x, y);
		if(t)t->is_through = value;
		return 0;
	}
	*/

	boxBody* createBody(int x, int y, int w, int h)
	{
		boxBody *body = new boxBody;
		body->m_world = this;
		body->set_position(x, y);
		body->m_size = vec2f(w, h);
		bodyList.insert(body);
		return body;
	}

	//for循环中使用这个iterator
	iterator deleteBody(boxBody* body)
	{
		if(body->m_tile)body->m_tile->eraseBody(body);
		iterator itr;
		if(body->is_static()){
			itr = bodyStatic.find(body);
			++itr;
			bodyStatic.erase(body);
			delete body;
		}
		else{
			itr = bodyList.find(body);
			++itr;
			bodyList.erase(body);
			delete body;
		}
		return itr;
	}

	//创建子弹
	boxBody* createBullet(int x, int y, int w, int h)
	{
		boxBody *body = new boxBody;
		body->m_world = this;
		body->set_shape(boxBody::BODY_BULLET);
		body->set_restitution(1.0f);
		body->set_friction(0);
		body->set_position(x, y);
		body->m_size = vec2f(w, h);
		bullets.insert(body);
		return body;
	}

	//for循环中使用这个iterator
	iterator deleteBullet(boxBody* body)
	{
		iterator itr = bullets.find(body);
		++itr;
		bullets.erase(body);
		delete body;
		return itr;
	}

	int bodyCount()const
	{
		return bodyList.size();
	}

	iterator bodyBegin()
	{
		return bodyList.begin();
	}

	iterator bodyEnd()
	{
		return bodyList.end();
	}

	/*bug
	boxBody* bodys(int index)
	{
		iterator itr = bodyList.begin();
		while(0<index)++itr;
		return *itr;
	}
	*/

	void update(float time);

	int query_bodys(std::vector<boxBody*> &ls, int x, int y, int width, int height);

	int select(std::vector<boxBody*> &ls, int x, int y, int width, int height, int shape = -1);
private:
	//碰撞测试，返回碰撞向量
	vec2b collisionTest(boxBody& body);
	vec2b circle_collisionTest(boxBody& body);

	void execute_force(boxBody& body, const vec2f& vec);
};

//---------------------------------------------------------------------------
//boxBody

boxBody::boxBody() :
	m_shape(BODY_BOX),
	m_attrib(STATE_REPULSION),
	m_test(),
	m_position(), m_size(), m_velocity(), m_impulse(),
	m_restitution(0.0f),
	m_friction(0.50f),
	m_world(null), m_tile(null), m_next(null),
	scale(1.0f),
	data(null)
{
}

boxBody::~boxBody()
{
	if(m_tile)m_tile->eraseBody(this);
}

vec4f boxBody::bounds()const
{
	return vec4f(m_position.x - m_size.x, m_position.y - m_size.y, m_size.x + m_size.x, m_size.y + m_size.y);
}

//线性力，跳跃
void boxBody::append_force(float x, float y)
{
	this->weakup();
	m_velocity.offset(x, y);//叠加速度
}

void boxBody::set_force(float x, float y)
{
	this->weakup();
	m_velocity = vec2f(x, y);
}

//瞬时冲力，行走
void boxBody::set_impulse(float x, float y)
{
	this->weakup();
	m_impulse += vec2f(x, y);
}

float boxBody::restitution()const
{
	return m_restitution;
}

void boxBody::set_restitution(float value)
{
	if(value < 0.0f)value = 0.0f;
	if(value > 1.0f)value = 1.0f;
	m_restitution = value;
}

float boxBody::friction()const
{
	return 1.0f - m_friction;
}

void boxBody::set_friction(float value)
{
	if(value < 0.0f)value = 0.0f;
	if(value > 1.0f)value = 1.0f;
	m_friction = 1.0f - value;
}

vec2f boxBody::position()const
{
	return m_position;
}

void boxBody::set_position(float x, float y)
{
	if(m_world){
		if(m_tile){
			m_tile->eraseBody(this);
		}
        vec2i t = get_tile(x, y);
		if(m_shape != BODY_BULLET){
			boxTile* tile = m_world->tile(t.x, t.y);
			if(tile)tile->addBody(this);
		}
	}

	m_position = vec2f(x, y);
}

vec2f boxBody::size()const
{
	return m_size;
}

void boxBody::set_size(float x, float y)
{
	m_size.x = x;
	m_size.y = y;
}

void boxBody::set_static(bool value)
{
	if(is_static() == value){
		return ;
	}

	boxWorld::iterator itr;
	if(value){
		m_attrib = m_attrib | STATE_STATIC;
		itr = m_world->bodyList.find(this);
		m_world->bodyList.erase(itr);
		m_world->bodyStatic.insert(this);
	}
	else{
		m_attrib = m_attrib & (~STATE_STATIC);
		itr = m_world->bodyStatic.find(this);
		m_world->bodyStatic.erase(itr);
		m_world->bodyList.insert(this);
    }
}

//---------------------------------------------------------------------------
//boxTile

boxTile::boxTile() :
	x(), y(),
	m_bodys(null),
	//is_through(0),
	data(null)
{
}

boxTile::boxTile(int _x, int _y) :
	x(_x), y(_y),
	m_bodys(null),
	//is_through(0),
	data(null)
{
}

boxTile::~boxTile()
{
}

void boxTile::eraseBody(boxBody* body)
{
	if(!m_bodys)return ;
	if(m_bodys == body){
		m_bodys = m_bodys->m_next;
	}
	else{
		boxBody* t = m_bodys;
		while(t->m_next && t->m_next != body){
			t = t->m_next;
		}

		if(t->m_next == body){//找到
			t->m_next = body->m_next;
		}
	}
	body->m_tile = null;
	body->m_next = null;
}

void boxTile::addBody(boxBody* body)
{
	if(!m_bodys){
		m_bodys = body;
		body->m_tile = this;
		return ;
	}

	if(m_bodys == body){
		return ;
	}
	else{
		boxBody* t = m_bodys;
		while(t->m_next){
			if(t->m_next == body){//已经存在
				return ;
			}
			t = t->m_next;
		}

		//加入列表
		t->m_next = body;
		body->m_tile = this;
	}
}

const boxTile& boxTile::operator=(const boxTile& tile)
{
	m_bodys = tile.m_bodys;
	const_cast<int16_t&>(x) = tile.x;
	const_cast<int16_t&>(y) = tile.y;
	//is_through = tile.is_through;
	//value = tile.value;
	data = tile.data;
	return *this;
}

//---------------------------------------------------------------------------
//boxWorld
int boxWorld::create(int width, int height)
{
	this->dispose();

	m_map.resize(width, height);
	for(size_t y=0; y<m_map.size_y(); ++y){
		for(size_t x=0; x<m_map.size_x(); ++x){
			m_map[y][x] = boxTile(x, y);
		}
	}
	return 0;
}

void boxWorld::dispose()
{
	if(!bodyList.empty()){
		for(iterator itr = bodyList.begin(); itr!=bodyList.end(); ++itr){
			delete *itr;
		}
		bodyList.clear();
	}

	if(!bodyStatic.empty()){
		for(iterator itr = bodyStatic.begin(); itr!=bodyStatic.end(); ++itr){
			delete *itr;
		}
		bodyStatic.clear();
	}

	if(!bullets.empty()){
		for(iterator itr = bullets.begin(); itr!=bullets.end(); ++itr){
			delete *itr;
		}
		bullets.clear();
	}
}

//这个是检测中心点，不精确
int boxWorld::query_bodys(std::vector<boxBody*> &ls, int x, int y, int width, int height)
{
	vec4f rc(x, y, width, height);
	rc = rc & vec4f(0, 0, this->size_x() * WORLD_TILE_SIZE, this->size_y() * WORLD_TILE_SIZE);

	int x1 = round(rc.x * INV_TILE_SIZE);
	int y1 = round(rc.y * INV_TILE_SIZE);
	int x2 = round((rc.x + rc.width  + WORLD_TILE_SIZE - 1) * INV_TILE_SIZE);
	int y2 = round((rc.y + rc.height + WORLD_TILE_SIZE - 1) * INV_TILE_SIZE);

	boxBody* body;
	ls.clear();
	for(int y=y1; y<y2; ++y){
		for(int x=x1; x<x2; ++x){
			body = this->tile(x, y)->m_bodys;
			while(body){
				if(rc.contains(body->m_position)){
					ls.push_back(body);
				}
				body = body->m_next;
			}
		}
	}
	return ls.size();
}

//矩形检测
int boxWorld::select(std::vector<boxBody*> &ls, int x, int y, int width, int height, int shape)
{
	vec4f rc(x, y, width, height);
	rc = rc & vec4f(0, 0, this->size_x() * WORLD_TILE_SIZE, this->size_y() * WORLD_TILE_SIZE);

	int x1 = round(rc.x * INV_TILE_SIZE);
	int y1 = round(rc.y * INV_TILE_SIZE);
	int x2 = round((rc.x + rc.width  + WORLD_TILE_SIZE - 1) * INV_TILE_SIZE);
	int y2 = round((rc.y + rc.height + WORLD_TILE_SIZE - 1) * INV_TILE_SIZE);

	x1 -= 1;//向周围扩展一个砖块的范围
	y1 -= 1;
	x2 += 1;
	y2 += 1;

//	if(x1 < 0)x1 = 0;
//	if(y1 < 0)y1 = 0;
//	if(x1 >= this->width())x1 = this->width() - 1;
//	if(x2 >= this->height())y2 = this->height() - 1;

	boxTile* tile;
	boxBody* body;
	ls.clear();
	for(int y=y1; y<=y2; ++y){
		for(int x=x1; x<=x2; ++x){
			tile = this->tile(x, y);
			if(tile){
				body = tile->m_bodys;
				while(body){
					if(rc.intersect(body->bounds())){
						if(shape < 0){
							ls.push_back(body);
						}
						else if(body->shape() == shape){
							ls.push_back(body);
                        }
					}
					body = body->m_next;
				}
            }
		}
	}
	return ls.size();
}


//时间目前不管用
//圆形碰撞测试
void boxWorld::update(float time)
{
	boxBody* body;
	for(iterator itr = bodyList.begin(); itr!=bodyList.end(); ++itr)
	{
		body = *itr;
//		if(body->is_static()){
//			continue;
//		}

		if(body->is_sleep()){
			continue;
		}

        //刚体碰撞
		if(body_test && body->is_repulsion()){
			if(body_repulsive(this, *itr, 0.20f) < 0){
				//itr = this->deleteBody(*itr);
				//continue;
			}
		}

		vec2i last = get_tile(body->m_position);// vec2i(body->m_position * INV_TILE_SIZE);

        //和砖块碰撞
		collisionTest(*body);
		/*//圆形碰撞不稳定
		if(body->m_type & (boxBody::BODY_BOX|boxBody::BODY_BULLET)){
			collisionTest(*body);
		}
		else{
			circle_collisionTest(*body);
		}
		*/

		//自动休眠
		if(auto_sleep && body->m_velocity.is_null()){
			body->sleep();
		}

		//更新位置
		vec2i now = get_tile(body->m_position);// vec2i(body->m_position * INV_TILE_SIZE);
		now.clamp(vec4i(1, 1, this->size_x() - 2, this->size_y() - 2));
		if(last != now)
		{
			boxTile* tile = this->tile(now.x, now.y);
			if(tile){//这里经常会跳出屏幕
				if(body->m_tile)body->m_tile->eraseBody(body);
				tile->addBody(body);
			}
		}
	}

	//处理子弹
	for(iterator itr = bullets.begin(); itr!=bullets.end(); )
	{
		body = *itr;
		if(body_repulsive(this, *itr, 0.20f) < 0){
			//itr = this->deleteBody(*itr);
			//continue;
		}
		collisionTest(*body);
		++itr;
	}
}

//---------------------------------------------------------------------------
//
// 碰撞测试函数
//
// 和墙壁碰撞
//

//力修正
void boxWorld::execute_force(boxBody& body, const vec2f& vec)
{
	//弹力
	if(body.m_test.x)body.m_velocity.x = -vec.x * body.m_restitution;
	if(body.m_test.y)body.m_velocity.y = -vec.y * body.m_restitution;

	//碰撞摩擦力
	if(body.m_test.y)body.m_velocity.x *= body.m_friction;
	if(body.m_test.x)body.m_velocity.y *= body.m_friction;

	//地面摩擦力。
	//air resistance空气阻力
	//2d俯视视觉可以考虑为地面摩擦力，根据地板类型修正阻值
	body.m_velocity.x *= body.m_friction;
	body.m_velocity.y *= body.m_friction;

	//清空冲力
	body.m_impulse = vec2f();

	//修正一下位置
//	if(body.m_position.x < WORLD_TILE_SIZE)body.m_position.x = WORLD_TILE_SIZE;
//	if(body.m_position.y < WORLD_TILE_SIZE)body.m_position.y = WORLD_TILE_SIZE;
//	if(body.m_position.x >= width() * WORLD_TILE_SIZE - WORLD_TILE_SIZE)body.m_position.x = width() * WORLD_TILE_SIZE - WORLD_TILE_SIZE;
//	if(body.m_position.y >= height() * WORLD_TILE_SIZE - WORLD_TILE_SIZE)body.m_position.y = height() * WORLD_TILE_SIZE - WORLD_TILE_SIZE;
}

vec2b boxWorld::collisionTest(boxBody& body)
{
	const float INV_TILE_SIZE = 1.0f / WORLD_TILE_SIZE;
	vec2f &pos = body.m_position;
	vec2b &test = body.m_test;	//碰撞方向
	void* data = body.shape() == boxBody::BODY_BULLET ? 0 : body.data;

	test = vec2b();

	int bx, by; //砖块位置
	int width = body.m_size.width;
	int height = body.m_size.height;
	int mode = body.shape() == boxBody::BODY_BOX ? COLLISION_BOOT : COLLISION_FLY;

	//速度向量 = 线性速度+世界重力+瞬时冲力

	//线性速度叠加世界重力
    if(!body.is_revise()){//未在修正状态
    	body.m_velocity += gravity * body.scale;
    }

	//速度向量初始化为线性速度
	vec2f vec = body.m_velocity;
	//叠加瞬时冲力
	vec += body.m_impulse * body.scale;

	//速度约束，防止穿墙
	//vec.clamp(-WORLD_TILE_SIZE / 2, -WORLD_TILE_SIZE / 2, WORLD_TILE_SIZE / 2, WORLD_TILE_SIZE /2 );//不管用

	int begin, end;
	bool n = true;
	bool t;

	//x方向检测
	bx = pos.x + vec.x;
	bx += vec.x < 0.0f ? -width: width;
	bx *= INV_TILE_SIZE;

	//位置要修正，不然会反方向穿越
	begin = (pos.y - height + WORLD_TEST_FIX) * INV_TILE_SIZE;
	end = (pos.y + height - WORLD_TEST_FIX) * INV_TILE_SIZE;

	//检测左上右上两个点
	for(int i=begin; i<=end; ++i){
		t = tilemap_is_through(instance, bx, i, mode);
		if(!t){
			if(on_collision){
				on_collision(&body, null, vec2i(bx, i));
			}
		}
		n = n && t;
	}

	if(!n){
		if(vec.x < 0.0f){
//			pos.x = (bx + 1) * WORLD_TILE_SIZE + width;
			//pos.x += (bx + 1) * WORLD_TILE_SIZE + width - pos.x;
			test.x = -1;
		}
		else{//右边碰撞
			//pos.x = bx * WORLD_TILE_SIZE - width;
//			pos.x -= pos.x - (bx * WORLD_TILE_SIZE-width);
			test.x = 1;
		}
	}
	else{
		pos.x += vec.x;
	}

	/*
	if(!is_zero(vec.x)){
		pos.x += vec.x;
		//偏移几个像素，不然y方向可能会卡住
		bx = static_cast<int>((pos.x + (vec.x < 0.0f ? -width - WORLD_TEST_FIX : width + WORLD_TEST_FIX)) * INV_TILE_SIZE);
		by = static_cast<int>(pos.y * INV_TILE_SIZE);

		if(!tilemap_is_through(bx, by, data)){
			if(vec.x < 0.0f){
				pos.x = (bx + 1) * WORLD_TILE_SIZE + width + WORLD_TEST_FIX;
				//pos.x += (bx + 1) * WORLD_TILE_SIZE + width + WORLD_TEST_FIX - pos.x;
				test.x = -1;
			}
			else{//右边碰撞
				pos.x = bx * WORLD_TILE_SIZE-width - WORLD_TEST_FIX;
				//pos.x -= pos.x - (bx * WORLD_TILE_SIZE-width - WORLD_TEST_FIX);
				test.x = 1;
			}
		}
	}
	*/

	//y方向检测
	//width /= 2.0f;	//横版角色宽度修正，不然1格宽的沟掉不下去
	n = true;
	by = pos.y + vec.y;
	by += vec.y < 0.0f ? -height: height;
	by *= INV_TILE_SIZE;
	//检测左上右上两个点
	begin = (pos.x - width + WORLD_TEST_FIX) * INV_TILE_SIZE;
	end = (pos.x + width - WORLD_TEST_FIX) * INV_TILE_SIZE;
	for(int i=begin; i<=end; ++i){
		t = tilemap_is_through(instance, i, by, mode);
		if(!t){
			if(on_collision){
				on_collision(&body, null, vec2i(bx, i));
			}
		}
		n = n && t;
	}
//	pos.y += vec.y;
	if(n){//可通过
		pos.y += vec.y;
	}
	else{
		if(vec.y < 0){
			test.y = -1;
//			pos.y = (by+1) * WORLD_TILE_SIZE + height;
			//pos.y += (by+1) * WORLD_TILE_SIZE + height - pos.y;
		}
		else{
			test.y = 1;
//			pos.y = by * WORLD_TILE_SIZE - height;
//			pos.y += -pos.y + by * WORLD_TILE_SIZE - height;
		}
	}



	#if 0
	if(vec.y < 0.0f)
	{//向上测试
		//bx = ;	//英雄是个长方形
		by = (pos.y - height + vec.y) * INV_TILE_SIZE;
		//检测左上右上两个点
		bx1 = (pos.x - width) * INV_TILE_SIZE;
		bx2 = (pos.x + width) * INV_TILE_SIZE;
		for(int i=bx1; i<=bx2; ++i){
			n = n && tilemap_is_through(i, by, data);
		}
		if(n){
			pos.y += vec.y;
		}
		else{
			pos.y += (by+1) * WORLD_TILE_SIZE + height - pos.y;
			test.y = -1;
		}
		/*
		if(!tilemap_is_through((pos.x - width) * INV_TILE_SIZE, by, body.data) ||
			!tilemap_is_through((pos.x + width) * INV_TILE_SIZE, by, body.data))
		{
			pos.y = (by+1) * WORLD_TILE_SIZE + height;
			//pos.y += (by+1) * WORLD_TILE_SIZE + height - pos.y;
			test.y = -1;
		}
		else{
			pos.y += vec.y;
		}
		*/
	}
	else
	{//向下测试
		by = (pos.y + height + vec.y) * INV_TILE_SIZE;

		bx1 = (pos.x - width) * INV_TILE_SIZE;
		bx2 = (pos.x + width) * INV_TILE_SIZE;
		for(int i=bx1; i<=bx2; ++i){
			n = n && tilemap_is_through(i, by, data);
		}
		if(n){
			pos.y += vec.y;
		}
		else{
			pos.y -= pos.y - (by * WORLD_TILE_SIZE - height);
			test.y = 1;
		}

		/*
		if(!tilemap_is_through((pos.x - width) * INV_TILE_SIZE, by, body.data) ||
			!tilemap_is_through((pos.x + width) * INV_TILE_SIZE, by, body.data) )
		{
			pos.y = by * WORLD_TILE_SIZE - height;
			//pos.y -= pos.y - (by * WORLD_TILE_SIZE - height);
			test.y = 1;
		}
		else{
			pos.y += vec.y;
		}
		*/
	}
	#endif

	execute_force(body, vec);

	//返回碰撞向量
	return test;
}


//
// 圆形刚体tile地图碰撞、移动
//
// 很不稳定，刚体经常卡在角落里
//

vec2b boxWorld::circle_collisionTest(boxBody& body)
{
	const float INV_TILE_SIZE = 1.0f / WORLD_TILE_SIZE;
	vec2f &pos = body.m_position;
	vec2b &test = body.m_test;	//碰撞方向
	void* data = body.data;
	int mode = body.shape() == boxBody::BODY_BOX ? COLLISION_BOOT : COLLISION_FLY;

	test = vec2b();

	int bx, by; //砖块位置
	int width = body.m_size.width;
	int height = body.m_size.height;

	//速度向量 = 线性速度+世界重力+瞬时冲力

	//线性速度叠加世界重力
	body.m_velocity += gravity * body.scale;

	//速度向量初始化为线性速度
	vec2f vec = body.m_velocity;
	//叠加瞬时冲力
	vec += body.m_impulse * body.scale;

	vec4f tile;
	vec2f p;
	int begin, end;
	bool n = true;

	//x方向检测
	bx = pos.x + vec.x;
	bx += vec.x < 0.0f ? -width: width;
	bx *= INV_TILE_SIZE;

	//位置要修正，不然会反方向穿越
	begin = (pos.y - height + WORLD_TEST_FIX) * INV_TILE_SIZE;
	end = (pos.y + height - WORLD_TEST_FIX) * INV_TILE_SIZE;

	//检测左上右上两个点
	for(int i=begin; i<=end; ++i){
		bool m = tilemap_is_through(instance, bx, i, mode);
		if(!m){
			tile = vec4f(bx * WORLD_TILE_SIZE, i * WORLD_TILE_SIZE, WORLD_TILE_SIZE, WORLD_TILE_SIZE);
			if(math::circle_collision<float>(tile, body.m_position, width, p)){
				n = false;
				break;
			}
		}
	}

	if(n){
		pos.x += vec.x;
	}
	else{
	   	if(vec.x < 0.0f){
			//这里遇到障碍之后的移动，会造成刚体闪烁
			//为了防止闪烁，这里暂时就直接不移动
			//vec.x += width + p.x;
			//pos.x += vec.x;// width + p.x;
//			pos.x = (bx + 1) * WORLD_TILE_SIZE + width;
//			pos.x += (bx + 1) * WORLD_TILE_SIZE + width - pos.x;
			test.x = -1;
		}
		else{//右边碰撞
			//vec -= width - p.x;
			//pos.x += vec.x; width - p.x;
//			pos.x = bx * WORLD_TILE_SIZE - width;
//			pos.x -= pos.x - (bx * WORLD_TILE_SIZE-width);
			test.x = 1;
		}
	}

	//y方向检测
	//width /= 2.0f;	//横版角色宽度修正，不然1格宽的沟掉不下去
	n = true;
	by = pos.y + vec.y;
	by += vec.y < 0.0f ? -height: height;
	by *= INV_TILE_SIZE;
	//检测左上右上两个点
	begin = (pos.x - width + WORLD_TEST_FIX) * INV_TILE_SIZE;
	end = (pos.x + width - WORLD_TEST_FIX) * INV_TILE_SIZE;
	for(int i=begin; i<=end; ++i){
		//n = n && tilemap_is_through(i, by, body.data);
		bool m = tilemap_is_through(instance, i, by, mode);
		if(!m){
			tile = vec4f(i * WORLD_TILE_SIZE, by * WORLD_TILE_SIZE, WORLD_TILE_SIZE, WORLD_TILE_SIZE);
			if(math::circle_collision<float>(tile, body.m_position, width, p)){
				n = false;
				break;
			}
		}
	}

	if(n){//可通过
		pos.y += vec.y;
	}
	else{
		if(vec.y < 0){
			test.y = -1;
			//vec.y += width + p.y;
			//pos.y += vec.y;// width + p.y;
//			pos.y = (by+1) * WORLD_TILE_SIZE + height;
//			pos.y += (by+1) * WORLD_TILE_SIZE + height - pos.y;
		}
		else{
			test.y = 1;
			//vec -= width - p.y;
			//pos.y += vec.y;//width - p.y;
//			pos.y = by * WORLD_TILE_SIZE - height;
//			pos.y += -pos.y + by * WORLD_TILE_SIZE - height;
		}
	}

	execute_force(body, vec);

	//返回碰撞向量
	return test;
}


//---------------------------------------------------------------------------
//在某个位置添加一个圆形冲击力，距离中心位置越近威力越强
//force为正值，表现为爆炸冲击力
//force如果为负值，表现为吸引力
//force最大值为[1|-1] * WORLD_TILE_SIZE，超过会穿墙。。。
void impulse_wave(boxWorld* world,
	int x, int y,	//位置，世界坐标
	int size,		//范围，圆形
	float force)	//威力
{
	std::vector<boxBody*> ls;
	world->select(ls, x - size, y - size, size + size, size + size);
	vec2f p(x, y);
	vec2f vec;
	vec2i cb(x / WORLD_TILE_SIZE, y / WORLD_TILE_SIZE);//中心点
	vec2i block;
	float length;

	float inv_size = 1.0f / size;
	float inv_length;

	for(size_t i=0; i<ls.size(); ++i){
		if(ls[i]->is_static()){
			continue;
		}

		vec = p - ls[i]->position();
		length = vec.length();
		if(length > size){
			continue;
		}
		inv_length = 1.0f / length;
		block = ls[i]->position();
		block /= WORLD_TILE_SIZE;
		//if(boxworld::tilemap_test_line(cb.x, cb.y, block.x, block.y)){//无障碍
			vec *= inv_length;//normalize
			vec *= 1.0 - length * inv_size;
			vec *= force;
			ls[i]->set_force(-vec.x, -vec.y);
		//}
	}
}

//---------------------------------------------------------------------------
//新子弹
//b = world.createBody(last.x + random(0), last.y + random(0), 10, 10);
//b->set_restitution(0.90);
//b->set_friction(0);
//b->data = (void*)1;
//b->type = 1;
//p = vec2i(mouse.x - last.x, mouse.y - last.y);
//p *= 0.1;
//b->append_force(p.x, p.y);
//子弹事件处理
//	for(boxWorld::iterator itr = world.bodyBegin(); itr != world.bodyEnd(); ++itr )
//	{
//		if(bullet_test(ls, &world, *itr++, 10)){}
//	}

//子弹测试
//不传导给其他刚体任何力，只碰撞
bool bullet_test(std::vector<boxBody*> del, boxWorld* world, boxBody* body, float force)
{
    std::vector<boxBody*> ls;
	float size = body->size().x * 2;
	vec2f p(body->position().x, body->position().y);

	float scale = 1;
	world->select(ls, p.x - size *scale, p.y - size * scale, (size + size) * scale, (size + size)* scale);

	vec4i bodyBounds = body->bounds();
	vec4i rc;
	vec2i result;
	vec2i v = vec2i(body->position().x - body->velocity().x, body->position().y - body->velocity().y);

	for(size_t i=0; i<ls.size(); ++i){
		if(ls[i] == body || ls[i]->data){
			continue;
		}
		rc = ls[i]->bounds();

		if(math::line_collision(rc, vec2i(body->position().x, body->position().y), v, result)){
			ls[i]->weakup();
			//ls[i]->data = (void*)255;
			body->set_force(0, 0);
			del.push_back(ls[i]);
			del.push_back(body);
			return false;

		}
	}
	return true;
}

//---------------------------------------------------------------------------
//互斥
//force 威力
//圆形范围测试
//刚体之间如何互斥，并且抵消重力是个问题

void on_collision123(boxBody* bodyA, boxBody* bodyB)
{
	//没有解决distance==0的情况，sqrt会报错
	vec2f vec(bodyB->position() - bodyA->position());
	float distance = vec.length();
	float sumRadius = bodyA->size().x + bodyB->size().x;
	float overlap = sumRadius - distance;// fabs(xA-xB);//计算重叠部分

	float inv_length = 1.0f / distance;
	vec *= inv_length;//normalize
	//vec *= 1.0 - distance * inv_size;//距离越远，力度越小
	vec *= overlap;// * 0.1;
	//ls[i]->set_force(-vec.x, -vec.y);
	//ball.append_force(-vec.x, -vec.y);//附加力的方法
	//append_force(vec.x, vec.y);
	if(bodyA->shape() != boxBody::BODY_BULLET){
		bodyA->append_force(vec.x, vec.y);
	}

	bodyA->append_force(vec.x, vec.y);
}

int body_repulsive(boxWorld* world, boxBody* body, float force)
{
	if(!body->is_repulsion()){
		return 0;
	}

	static std::vector<boxBody*> ls;
	float size = body->size().x;
	vec2f p(body->position().x, body->position().y);
	vec2f vec;
	float length;

    ls.clear();

	//碰撞检测范围
	world->select(ls, p.x - size, p.y - size, size + size, size + size, boxBody::BODY_BOX);

	//test force = 0.20f在无重力情况下，比较自然

	//force = 2.0f;

	//float inv_size = 1.0f / size;
	float inv_length;

    bool is_rep = false;

	for(size_t i=0; i<ls.size(); ++i){
		if(ls[i]->is_static()){
			continue;
		}
		if(!ls[i]->is_repulsion()){
			continue;
		}
		vec = p - ls[i]->position();
		length = vec.length();
		size = body->size().x + ls[i]->size().x;
		if(length > size || body == ls[i]){
			continue;
		}

        if(body->is_revise()){
            continue;
        }
		//距离太近，就不会施加力
		/*
		else if(length < 1.0){//
		//这里不是太好
			if(1 && body->velocity().x && body->velocity().y){
	//			vec.x = body->test().x * force;
	//			vec.y = body->test().y * force;
				inv_length = body->velocity().length();
				inv_length = 1.0 / inv_length;
				vec = body->velocity();
				vec *= inv_length;
				//vec = body->velocity();
				vec *= force;
			}
			else{
				vec.x = math::random(-force, force);
				vec.y = math::random(-force, force);
			}
//			ls[i]->append_force(vec.x, vec.y);
//			body->append_force(-vec.x, -vec.y);
			body->set_impulse(vec.x, vec.y);//效果不是太好
			ls[i]->set_impulse(-vec.x, -vec.y);
			continue;
		}
		/*/

		else if(length < 1.0f){//距离太近，会导致normal失败
			//随机方向的向量
			vec.x = math::random(-force, force);
			vec.y = math::random(-force, force);
			vec *= 0.001f;//0.001f

			if(body->shape() != boxBody::BODY_BULLET){
				ls[i]->append_force(vec.x, vec.y);
				body->append_force(-vec.x, -vec.y);
			}
			if(world->on_collision){
				if(world->on_collision(body, ls[i], vec2i()) < 0){
					return -1;
				}
			}
//			body->set_impulse(vec.x, vec.y);//效果不是太好
//			ls[i]->set_impulse(-vec.x, -vec.y);
            body->set_revise(true);
            is_rep = true;
			continue;
		}
		//*/

		inv_length = 1.0f / length;
		vec *= inv_length;//normalize
		//vec *= 1.0 - length * inv_size;//距离越远，力度越小
		vec *= force;
		//ls[i]->set_force(-vec.x, -vec.y);
		if(body->shape() != boxBody::BODY_BULLET){
			ls[i]->append_force(-vec.x, -vec.y);//附加力的方法
			body->append_force(vec.x, vec.y);
		}
		if(world->on_collision){
			if(world->on_collision(body, ls[i], vec2i()) < 0){
				return -1;
			}
		}
        body->set_revise(true);
        is_rep = true;

//		body->set_impulse(vec.x, vec.y);//冲力方法
//		ls[i]->set_impulse(-vec.x, -vec.y);
	}

    if(!is_rep){
        body->set_revise(false);
    }

	return 0;
}

//---------------------------------------------------------------------------

//}//end namespace physics
}//end namespace cgl

#endif //BOXWORLD_HPP_20170210144218