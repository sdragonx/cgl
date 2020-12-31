/*

 boxworld_lightning.hpp

 sdragonx 2019-07-17 19:18:36

*/
#ifndef BOXWORLD_LIGHTNING_HPP_20190717191836
#define BOXWORLD_LIGHTNING_HPP_20190717191836

#include "public.hpp"
#include <cgl/graphics/draw.hpp>

namespace boxworld{


class boxLightning
{
public:
	typedef ribbons LIGHTNING;

	boxUniWorld* World;
	std::vector<LIGHTNING*> m_lightnings;

	boxLightning(boxUniWorld* world) : World(world), m_lightnings()
	{

	}

	~boxLightning()
	{
		this->dispose();
	}

	void dispose()
	{
		delete_all(m_lightnings);
		m_lightnings.clear();
    }

	void push(float x, float y);
	void update()
	{
		if(m_lightnings.empty()){
			return ;
		}

		for(size_t i = m_lightnings.size() - 1; i < m_lightnings.size(); --i){
			m_lightnings[i]->alpha -= 32;
			if(m_lightnings[i]->alpha < 0){
				delete m_lightnings[i];
				m_lightnings.erase(m_lightnings.begin() + i);
			}
			for(size_t j=0; j<m_lightnings[i]->vb.size(); ++j){
				m_lightnings[i]->vb[j].color.alpha = m_lightnings[i]->alpha;
			}
		}
    }

	void paint(glcontext& dc)
	{
		dc.blendfunc(GL_SRC_ALPHA, GL_ONE);
		for(size_t i=0; i<m_lightnings.size(); ++i){
			dc.color.alpha = m_lightnings[i]->alpha;
			dc.draw_image(engine.image.lightingBot[m_lightnings[i]->value],
				m_lightnings[i]->vb.back().x - 64, m_lightnings[i]->vb.back().y - 128, 256, 256);
			m_lightnings[i]->paint(dc, engine.image.lightingBot1->id);
			//m_lightnings[i]->paint(dc, engine.image.lightingBot1->image);
		}
		dc.blendfunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		dc.color.alpha = 255;
	}
};

}//end namespace boxworld

//#include "lightning.cpp"

#endif //BOXWORLD_LIGHTNING_HPP_20190717191836
