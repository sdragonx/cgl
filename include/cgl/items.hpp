/*

 items.hpp

 sdragonx 2019-06-09 14:09:19

*/
#ifndef ITEMS_HPP_20190609140919
#define ITEMS_HPP_20190609140919

#include "public.hpp"
#include "unit.hpp"
#include "item.hpp"

namespace boxworld{

class boxItem : public boxUnit
{
public:
	int count;

	boxItem(boxUnits<boxUnit>* list) : boxUnit(list)
	{
		scale = 0.5f;
	}

	int classtype()const { return CLASS_ITEM; }

	int update()
	{
		return 0;
	}

	void paint(graphics::draw_context& dc, vec4ub color, vec4ub vision_color, int param = 0)
	{
		float w, h;
		w = image.image->width * scale;
		h = image.image->height * scale;

		if(debug.show_bounds){
			dc.color = color::green;
			dc.point_image(engine.image.ring, position.x, position.z, h * 2, h);
		}

		dc.color = color::white;
		dc.point_image(image.image, position.x, position.z, w, h);
	}
};

class boxItems : public tilemap<boxUnit*>
{
public:
	boxUniWorld* const World;

public:
	boxItems(boxUniWorld* world);

	void resize(size_t x, size_t y);

	void clear();

	void random_item();

	//查找一个物品
	boxUnit* find(int x, int y, int range);
	vec2i find_space(int x, int y, int range);

	void dropitem(int x, int y, boxUnit* item);
	void dropitem(int x, int y, const string_t& name);

	bool is_storage(int x, int y);
	bool is_space(int x, int y);

	void print(draw_context& dc)
	{
		dc.color = color::blue;
		dc.color.alpha = 128;
		for(size_t y=0; y<size_y(); ++y)
		for(size_t x=0; x<size_x(); ++x){
			if(tile(x, y)){
				dc.fill_rect(x * WORLD_TILE_SIZE + 32, y * WORLD_TILE_SIZE + 32, HALF_TILE_SIZE, HALF_TILE_SIZE);
            }
		}
	}

protected:
	bool check_item(int x, int y);

	void select(std::vector<boxUnit*>& ls, int x, int y, int range);
};

}//end namespace boxworld

//#include "items.cpp"

#endif //ITEMS_HPP_20190609140919
