
#include <cgl/xdoc.hpp>
#include "../engine.hpp"
#include "../world.hpp"

namespace boxworld{

boxEngine::boxEngine(uniframe& framework) :
	Luna(&framework),
	m_worldList(),
	plants(),
	items()
{
	m_gameIsPause = false;
	m_gameSpeed = 1;	//游戏速度 1

	m_clock = 0;
	m_lastClock = std::clock();

	memset(&image, 0, sizeof(image));
}

void boxEngine::dispose()
{
	//删除所有世界
	for(size_t i=0; i<worldCount(); ++i){
		delete getWorld(i);
	}
	m_worldList.clear();
}

void boxEngine::viewport(int x, int y, int width, int height)
{
	//for(...
	boxUniWorld* world = getWorld();
	if(world){
		world->camera.init(width, height, world->width(), world->height(), WORLD_TILE_SIZE);
		world->camera.touch(0, 0);
	}

	screen.width = width;
	screen.height = height;
}

void boxEngine::init()
{
	//other
	plants.load(m_resPath + "plants.xd");
	items.load(m_resPath + "items.xd");

	//title
	tileset_LoadAtlas("base", "砖块64.png");
	tileset_LoadAtlas("wall", "wall.png", rimworld_loadorder);
	tileset_LoadAtlas("rock", "gfx/tiles/Rock_Atlas.png", rimworld_loadorder);

	image.ring = loadImage("gfx/ring.png");
	image.cut = loadImage("gfx/icons/CutPlants.png");

	//火
	teximage* fire = loadImage("gfx/effect/fire0.png", true, false);
	image.fire.load(fire, 4, 3);

	//水渍
	image.spatter[0] = loadImage("gfx/effect/SpatterA.png");
	image.spatter[1] = loadImage("gfx/effect/SpatterB.png");
	image.spatter[2] = loadImage("gfx/effect/SpatterC.png");

	//植物
	image.tex_grass[0] = engine.loadImage("gfx/plants/grass0.png");
	image.tex_grass[1] = engine.loadImage("gfx/plants/grass1.png");
	image.tex_grass[2] = engine.loadImage("gfx/plants/grass2.png");

	//天气
	image.fog = engine.loadImage("gfx/effect/FogWorld.png");
	image.rain = engine.loadImage("gfx/effect/RainWorldOverlay.png");

	image.clouds[0] = engine.loadImage("gfx/effect/cloud0.png");
	image.clouds[1] = engine.loadImage("gfx/effect/cloud1.png");
	image.clouds[2] = engine.loadImage("gfx/effect/cloud2.png");
	image.clouds[3] = engine.loadImage("gfx/effect/cloud3.png");

	//魔法
	image.lighting[0] = engine.loadImage("gfx/effect/lighting0.png");
	image.lighting[1] = engine.loadImage("gfx/effect/lighting1.png");

	teximage* tempBot = loadImage("gfx/effect/lightbot.png", true, false);
	tempBot->split(image.lightingBot, 4, 4);

	image.lightingBot1 = engine.loadImage("gfx/effect/lightning_2.png");

	image.halo = engine.loadImage("gfx/effect/halo.png");
	image.wing = engine.loadImage("gfx/effect/wing.png");

	//地下背景图片
	teximage* tempFog = loadImage("shadow.png", true, false);
	tempFog->split(image.shadow, 4, 4);
}

void boxEngine::setTileSize(int size)
{
	WORLD_TILE_SIZE = size;
	HALF_TILE_SIZE = size / 2;
	INV_TILE_SIZE = 1.0f / size;

	//32格播放声音
	SOUND_POS_SCALE = 1.0 / 32.0f / WORLD_TILE_SIZE;
}

void boxEngine::setResourcePath(const string_t& path)
{
	m_resPath = path;
}

string_t boxEngine::resourcePath()const
{
	return m_resPath;
}

int boxEngine::loadTexture(const string_t& tex, int format)
{
	return Luna->gl.textures.open((m_resPath + tex).c_str(), GLenum(format), true);//, &m_texSize);
}

teximage* boxEngine::loadImage(const string_t& filename, bool can_pack, bool fix)
{
	return Luna->images.open(m_resPath + filename, can_pack, fix);
}

//---------------------------------------------------------------------------
// input

void boxEngine::mousedown(int x, int y, int button)
{
	boxUniWorld* world = getWorld();
	if(world){
		world->on_mousedown(x, y, button);
	}
}

void boxEngine::mouseup(int x, int y, int button)
{
	boxUniWorld* world = getWorld();
	if(world){
		world->on_mouseup(x, y, button);
	}
}

void boxEngine::mousemove(int x, int y, int button)
{
	boxUniWorld* world = getWorld();
	if(world){
		world->on_mousemove(x, y, button);
	}
}

void boxEngine::doubleclick(int x, int y, int button)
{
	boxUniWorld* world = getWorld();
	if(world){
		world->on_doubleclick(x, y, button);
	}
}

void boxEngine::mousewheel(int y)
{
	boxUniWorld* world = getWorld();
	if(world){
		world->on_mousewheel(y);
	}
}

void boxEngine::keydown(int key)
{
	boxUniWorld* world = getWorld();
	if(world){
		world->on_keyboard();

		world->selector.keydown_default(key);
	}
	keyboard.set(key);
}

void boxEngine::keyup(int key)
{
	keyboard.reset(key);
}

//---------------------------------------------------------------------------
// config

int boxEngine::saveConfig(const string_t& filename)
{
	xdocument<char> doc;
	doc.create("engine");
	xnode<char> node = doc.subnode();
	node.add_value("WORLD_TILE_SIZE", WORLD_TILE_SIZE);
	doc.save(filename.c_str(), true);
	return 0;
}

int boxEngine::loadConfig(const string_t& filename)
{
	xdocument<char> doc;
	doc.open(filename.c_str());

	return -1;
}


//---------------------------------------------------------------------------
// engine

void boxEngine::pause(bool pause)
{
	if(pause){

	}
	else{
		m_lastClock = std::clock();
	}
	m_gameIsPause = pause;
}

bool boxEngine::isPause()const
{
	return m_gameIsPause;
}

//游戏暂停的时候，时间戳是0
int boxEngine::clock()
{
	if(!isPause()){
		m_clock += std::clock() - m_lastClock;
		m_lastClock = std::clock();
	}
	return m_clock;
}

void boxEngine::gameSpeed(int speed)
{
	m_gameSpeed = speed;
}

void boxEngine::update()
{
	time += m_gameSpeed;
	for(size_t i=0; i<m_worldList.size(); ++i){
		m_worldList[i]->update();
	}
}


//---------------------------------------------------------------------------
// world

void camera_on_lookat(void* instance, float x, float y, float z)
{
	engine.Luna->audio.listener.set_position(x * engine.sound_position_scale(), y * engine.sound_position_scale(), 0.0f);
	boxUniWorld* world = (boxUniWorld*)instance;
	world->calc_draw_range();
}

boxUniWorld* boxEngine::getWorld()
{
	return m_currentWorld;
}

boxUniWorld* boxEngine::getWorld(size_t index)
{
    return m_worldList[index];
}

void boxEngine::setWorld(boxUniWorld* world)
{
    m_currentWorld = world;
}

boxUniWorld* boxEngine::createWorld()
{
	boxUniWorld* world = new boxUniWorld(this);
	world->camera.on_lookat = camera_on_lookat;
	world->camera.zoom_min = WORLD_ZOOM_MIN;
	world->camera.zoom_max = WORLD_ZOOM_MAX;

	m_worldList.push_back(world);
	if(!m_currentWorld){
		m_currentWorld = world;
	}
	return world;
}

void boxEngine::eraseWorld(boxUniWorld* world)
{
	worldContainer::iterator itr = std::find(m_worldList.begin(), m_worldList.end(), world);
	if(itr != m_worldList.end()){
		m_worldList.erase(itr);
	}
	if(m_currentWorld == world){
		if(!m_worldList.empty()){
			m_currentWorld = m_worldList[0];
		}
		else{
            m_currentWorld = null;
        }
    }
}

void boxEngine::paintWorld(graphics::glcontext& gl)
{
	boxUniWorld* world = getWorld();

	if(!world){
		CGL_LOG("boxEngine : error on paintWorld.");
		return ;
	}

	world->camera.push_view(gl);

	graphics::draw_context dc(gl);

	world->paint(dc);
	world->units.paint(dc);

	/*
	for(size_t i=0; i<engine.worldCount(); ++i){
		dc.init();
		engine.getWorld(i)->paint(dc, 0);
		engine.getWorld(i)->units.paint(dc);
		luna.gl.draw(dc);
	}
	*/

	gl.draw(dc);

	world->camera.pop_view(gl);
}


//void boxEngine::playsound(const string_t& filename)
//{
//	UniFrame->audio.playsound(m_resPath + filename);
//}

float boxEngine::sound_position_scale()const
{
	return SOUND_POS_SCALE;
}

int sound_position(int sound, float x, float y, float z)
{
	if(sound){
		x *= engine.sound_position_scale();
		y *= engine.sound_position_scale();
		z = 0.0f;
		float v[3] = {x, y, z};
		alSourcefv(sound, AL_POSITION, v);
	}
	return sound;
}

int playsound(float x, float y, float z, const string_t& filename)
{
	ALuint sound = luna.audio.playsound(engine.resourcePath() + filename);
	sound_position(sound, x, y, z);
	return sound;
}

int playsound(vec2f pos, const string_t& filename)
{
	return playsound(pos.x, pos.y, 0.0f, filename);
}

int playsound(vec3f pos, const string_t& filename)
{
	return playsound(pos.x, pos.z, 0.0f, filename);
}

//在20格范围内播放声音
//30 x 20
bool playsound(al::alsound& sound, float x, float y, const string_t& filename)
{
	boxUniWorld* world = engine.getWorld();
	if(!world){
		return false;
	}

	vec3f p = world->camera.position;
	vec4f vp(p.x - WORLD_TILE_SIZE * 15, p.y - WORLD_TILE_SIZE * 10, WORLD_TILE_SIZE * 30, WORLD_TILE_SIZE * 20);
	/*
	vp = world.camera.viewport();
	vp.x -= vp.width * 0.5f;
	vp.y -= vp.height * 0.5f;
	vp.width *= 2.0f;
	vp.height *= 2.0f;
	*/

	if(vp.contains(x, y)){
		sound.bind_buffer(luna.audio.buffers.open(engine.resourcePath() + filename));
		sound.play();
		sound_position(sound.handle(), x, y, 0.0f);
		//print(filename);
		return true;
	}
	return false;
}


}//end namespace boxworld
