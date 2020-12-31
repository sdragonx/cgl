/*

 minimap.hpp

 sdragonx 2019-06-09 16:20:29

*/
#ifndef MINIMAP_HPP_20190609162029
#define MINIMAP_HPP_20190609162029

#include "public.hpp"

namespace boxworld{


//����С��ͼ���ڴ�С������С��ͼ������Ϣ
void calc_minimap(
	int width, int height,	//����С��ͼ��С
	vec4i& map,				//���������С���������꣩�����ص�ͼ��С
	vec4f& viewport)		//�����ӿ���Ϣ�������ӿ�λ��
{
	vec4i minimap(0, 0, width, height);
	int world_width = map.width;
	int world_height = map.height;
	map = minimap.align(map.width, map.height, CGL_CENTER, true);

	//�����ӿ�λ��
	viewport *= INV_TILE_SIZE;
	viewport.x = viewport.x * map.width / world_width;
	viewport.y = viewport.y * map.height / world_height;
	viewport.width = viewport.width * map.width / world_width;
	viewport.height = viewport.height * map.height / world_height;
	viewport.x += map.x;
	viewport.y += map.y;
}

//���С��ͼ�����������ӿ�λ��
void minimap_touch(
	int width, int height,	//С��ͼ��С
	int world_width,			//��ͼ��С
	int world_height,
	int x, int y,			//���λ��
	vec2i& vp)				//�����ӿ�λ��
{
	vec4i map(0, 0, width, height);
	map = map.align(world_width, world_height, CGL_CENTER, true);

	x -= map.x;
	y -= map.y;
	vp.x = x * world_width / map.width;
	vp.y = y * world_height / map.height;
}

//ʹ��һ��256x256���������ͼ����
// ÿ�ֵ���Ҫʹ��һ����ɫ
// ÿ����λҪʹ��һ����ɫ��ʶ
//
//

class boxMinimap : public ui::widget
{
private:
	boxUniWorld* World;
	int texture;
	pixelimage image;

public:
	boxMinimap() : ui::widget(), World(), texture(), image()
	{
		this->set_clipclient(true);
	}

	~boxMinimap()
	{

	}

	string_t classname()const
	{
		return "boxworld.minimap";
	}

	void dispose()
	{
		if(texture){
			engine.Luna->gl.delete_texture(texture);
			texture = 0;
		}
    }

	void on_create(ui::object* sender)
	{
		frame.constant_mode = vec4i(0, 0, ui::PARENT_WIDTH, 0);
		frame.constant = vec4i(0, 0, 0, 0);
	}

	void on_mousedown(ui::object* sender, int button, int x, int y);
	void on_mousemove(ui::object* sender, int button, int x, int y);
	void on_paint(ui::object* sender, draw_context* dc);

	void set_world(boxUniWorld* world)
	{
		if(World != world){
			World = world;
			this->update_map();
		}
	}

	void update_map();
};


}//end namespace boxworld

//#include "minimap.cpp"

#endif //MINIMAP_HPP_20190609162029
