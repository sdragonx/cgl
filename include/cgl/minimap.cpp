#include "world.hpp"
#include "minimap.hpp"

namespace boxworld{

//---------------------------------------------------------------------------
//minimap

void boxMinimap::on_mousedown(ui::object* sender, int button, int x, int y)
{
	if(World){
		vec2i vp;
		minimap_touch(width() - 8, height() - 8, World->width(), World->height(), x - 4, y - 4, vp);
		World->camera.lookat(vp.x, vp.y);
	}
}

void boxMinimap::on_mousemove(ui::object* sender, int button, int x, int y)
{
	if(button == mouse_left){
		on_mousedown(sender, button, x, y);
	}
}

void boxMinimap::on_paint(ui::object* sender, draw_context* dc)
{
	ui::widget::on_paint(sender, dc);

	if(!World){
		return ;
	}

	vec4i map(0, 0, MAP_SIZE, MAP_SIZE);
	vec4f vp = World->camera.viewport;
	calc_minimap(width() - 8, height() - 8, map, vp);
	map.x += 4;
	map.y += 4;
	dc->color = color::white;
	dc->draw_image(texture, map);
	dc->color = color::green;
	vp.x += 4;
	vp.y += 4;
	dc->draw_rect(vp);

	vec2f p;
	const boxUnits<boxUnit>& units = World->units;
	dc->pen_width(3.0);
	for(size_t i=0; i<units.size(); ++i){
		p = vec2f(units[i]->position.x, units[i]->position.z);
		p *= INV_TILE_SIZE;
		p *= vec2f(map.width, map.height);
		p /= vec2f(MAP_SIZE, MAP_SIZE);
		p += vec2f(map.x, map.y);
		dc->color = group_color(units[i]->group);
		dc->draw_point(p.x, p.y);
	}
}

void boxMinimap::update_map()
{
	size_t size_x = World->size_x();
	size_t size_y = World->size_y();

	if(image.is_null()){
		image.create(size_x, size_y, CGL_RGB, false, null);
	}

	vec3ub c;
	vec3ub* p;
	for(size_t y=0; y<size_y; ++y){
		p = (vec3ub*)image.scanline(y);
		for(size_t x=0; x<size_x; ++x){
			int t = World->tile(x, y)->value;
			if(t){
				p[x] = grounds.tiles(t)->color;
			}
			else{
				p[x] = color::skyBlue;
			}
		}
	}

	if(texture){
		engine.Luna->gl.delete_texture(texture);
		texture = 0;
	}
	texture = luna.gl.create_texture(size_x, size_y, CGL_RGB, CGL_RGB, CGL_UNSIGNED_BYTE, image.data());
}

}//end namespace boxworld
