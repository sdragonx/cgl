#include "lightning.hpp"
#include "world.hpp"
#include <cgl/thread/mutex.hpp>

namespace boxworld{

void boxLightning::push(float x, float y)
{
	const char* sounds[4] = {
		"sfx/Lightning_1a.ogg",
		"sfx/Lightning_1b.ogg",
		"sfx/Lightning_1c.ogg",
		"sfx/Lightning_1d.ogg"
	};

	mutex_t mutex;
	auto_lock lock(mutex);

	//playsound(sound, x, y, sounds[random(4)]);
	int s = engine.Luna->audio.playsound(engine.resourcePath() + sounds[random(4)]);
	sound_position(s, x, y, 0);
	impulse_wave(&World->m_world, x, y, WORLD_TILE_SIZE*4, 100);

	vec2i t = pt2map(x, y);
	if(tilemap_is_through(World, t.x, t.y, COLLISION_BOOT)){
		World->fireLayer.tile(t.x, t.y).x = 1;// >fireLayer[t.y][t.x].x = 1;
	}
	World->lightLayer.reset(255);

	teximage* img = engine.image.lightingBot1;
	LIGHTNING *lightning = new LIGHTNING;
	for(int i=0; i<16; ++i){
		lightning->push(x, y, 64, vec4ub(196, 128, 255, 255), 1, 0, 64,
			img->u1, img->v1, img->u2, img->v2);
		x += math::random(-256, 256);
		y -= 512 + math::random(-256, 256);
	}
	lightning->alpha = 255;
	lightning->value = random(16);
	m_lightnings.push_back(lightning);
}


}//