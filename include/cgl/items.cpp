#include "items.hpp"

namespace boxworld{

boxItems::boxItems(boxUniWorld* world) : tilemap<boxUnit*>(), World(world)
{
}

void boxItems::resize(size_t x, size_t y)
{
	this->clear();
	tilemap<boxUnit*>::resize(x, y);
	this->fill(null);
}

void boxItems::clear()
{
	for(size_t y=0; y<size_y(); ++y)
	for(size_t x=0; x<size_x(); ++x){
		if(tile(x, y)){
			World->units.erase(tile(x, y));
			tile(x, y) = null;
		}
	}
}

void boxItems::random_item()
{
	boxItem* item;

	this->clear();

	vec2i p;
	for(size_t y=0; y<size_y(); ++y)
	for(size_t x=0; x<size_x(); ++x){
		if(random(100) < 10){
			p = find_space(x, y, 1024);
			if(p.is_null())continue;
			const ITEM* i = engine.items[math::random(engine.items.size())];
			if(!i)continue;
			item = World->units.create<boxItem>(0, 0, 64, UNIT_GROUP_ITEM);
			item->setTile(p.x, p.y);
			item->id = genID(i->name);
			item->image = i->images[math::random(i->images.size())];
			tile(p.x, p.y) = item;
		}
	}
}


bool boxItems::is_storage(int x, int y)
{
	vec4i rect(30, 30, 4, 4);
	return rect.contains(x, y);
}

bool boxItems::check_item(int x, int y)
{
	if(!is_valid(x, y)){
		return false;
	}

	if(is_storage(x, y)){
		return false;
	}

	boxUnit* obj = tile(x, y);

	if(!obj){
		return false;
	}

	if(obj->target){
		return false;
	}

	return true;

	// && !is_storage(x, y) && is_not_allowedType(storage, id);
}

//六角形搜索
void boxItems::select(std::vector<boxUnit*>& ls, int x, int y, int range)
{
	std::vector<vec2i> ps;
	math::hexagon_points(ps, x, y, range);
	//clip rect
	ls.clear();
	for(size_t i=0; i<ps.size(); ++i){
		if(check_item(ps[i].x, ps[i].y)){
			int px = ps[i].x;
			int py = ps[i].y;
			boxUnit* unit = tile(px, py);
			//CGL_LOG("find item %i, %i = %f, %f", px, py, unit->position.x * INV_TILE_SIZE, unit->position.z * INV_TILE_SIZE);
			ls.push_back(tile(ps[i].x, ps[i].y));
		}
	}
}

//查找一个物品
//这里能优化，A星查找
boxUnit* boxItems::find(int x, int y, int range)
{
	std::vector<boxUnit*> ls;
	for(int i=0; i<range; ++i){
		select(ls, x, y, i);
		if(!ls.empty()){
			return ls.front();
		}
	}

	return null;

	/*
	//检查脚下
	if(check_item(x, y)){
		return tile(x, y);
	}

	int x1, y1, x2, y2;
	//方形范围查找
	for(int r=1; r<range; ++r){
		x1 = x - r;
		x2 = x + r + 1;
		y1 = y - r;
		y2 = y + r + 1;
		for(int i=x1; i<x2; ++i){
			if(check_item(i, y1)){
				return tile(i, y1);
			}
			if(check_item(i, y2)){
				return tile(i, y2);
			}
		}
		++y1; --y2;
		for(int i=y1; i<y2; ++i){
			if(check_item(x1, i)){
				return tile(x1, i);
			}
			if(check_item(x2, i)){
				return tile(x2, i);
			}
		}
	}

	return null;
	*/
}

bool boxItems::is_space(int x, int y)
{
	if(!is_valid(x, y)){
		return false;
	}

	if(is_storage(x, y)){
		//return false;
	}

	if(tile(x, y)){
		return false;
	}

	if(!tilemap_is_through(World, x, y, COLLISION_BOOT)){
		return false;
	}
	return true;
}

vec2i boxItems::find_space(int x, int y, int range)
{
	std::vector<vec2i> ps;
	for(int r = 0; r < range; ++r){
		math::hexagon_points(ps, x, y, r);
		for(size_t i=0; i<ps.size(); ++i){
			if(is_space(ps[i].x, ps[i].y)){
				return ps[i];
			}
		}
	}
	return vec2i();
}

void boxItems::dropitem(int x, int y, boxUnit* item)
{
	x /= WORLD_TILE_SIZE;
	y /= WORLD_TILE_SIZE;
	vec2i p = find_space(x, y, 64);
	tile(p.x, p.y) = item;
	item->setTile(p.x, p.y);
	item->scale = 0.5f;
	item->target = null;
	item->ai.animation = ANIMATION_IDLE;
}

//掉落生成物品
void boxItems::dropitem(int x, int y, const string_t& name)
{
	x /= WORLD_TILE_SIZE;
	y /= WORLD_TILE_SIZE;
	vec2i p = find_space(x, y, 64);

	int64_t id = genID(name);
	const ITEM* i = engine.items.find(id);

	if(!i){
		return ;
	}

	boxUnit* item = World->units.create<boxItem>(0, 0, 64, UNIT_GROUP_ITEM);
	item->id = id;
	item->image = i->images[math::random(i->images.size())];
	item->scale = 0.5f;
	item->target = null;

	item->setTile(p.x, p.y);
	tile(p.x, p.y) = item;
}


}//end namespace boxworld
