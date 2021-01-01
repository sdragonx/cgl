/*
 Copyright (c) 2005-2020 sdragonx (mail:sdragonx@foxmail.com)

 spirit.hpp

 2019-07-16 15:08:38

*/
#ifndef SPIRIT_HPP_20190716150838
#define SPIRIT_HPP_20190716150838

#include "teximage.hpp"

namespace cgl{
namespace graphics{

class spirit
{
public:
    std::vector<teximage> m_images;
    size_t m_frame;
    size_t m_round;
    bool m_is_play;
    bool m_is_repeat;
    bool m_is_forward;

public:
    spirit() :
        m_images(),
        m_frame(),
        m_round(),
        m_is_play(true),
        m_is_repeat(true),
        m_is_forward(true)
    {

    }

    /*
    void load(const char* filename, int x, int y, int max = INT_MAX)
    {
        teximage* image = engine.loadImage(filename, true, false);
        image->split(m_images, x, y, max);
        m_frame = 0;
        m_round = 0;
    }
    */

    void load(teximage* image, int x, int y, int max = INT_MAX)
    {
        image->split(m_images, x, y, max);
        m_frame = 0;
        m_round = 0;
    }

    bool is_play()const
    {
        return m_is_play;
    }

    void play(bool repeat = true, bool forward = true)
    {
        m_is_repeat = repeat;
        m_is_forward = forward;
        m_is_play = true;
        m_round = 0;
    }

    void stop()
    {
        m_is_play = false;
    }

    void rewind()
    {
        m_frame = 0;
    }

    size_t frame()const
    {
        return m_frame;
    }

    void set_frame(size_t frame)
    {
        if(frame < m_images.size()){
            m_frame = frame;
        }
    }

    size_t round()const
    {
        return m_round;
    }

    void update()
    {
        if(!m_is_play){
            return ;
        }

        if(m_images.empty()){
            return ;
        }

        if(m_is_forward){
            if(m_frame + 1 >=  m_images.size()){
                ++m_round;
                if(m_is_repeat){
                    m_frame = 0;
                }
                else{
                    m_is_play = false;
                }
            }
            else{
                ++m_frame;
            }
        }
        else{
            if(m_frame > 0){
                --m_frame;
            }
            else{
                ++m_round;
                if(m_is_repeat){
                    m_frame = m_images.size() - 1;
                }
                else{
                    m_is_play = false;
                }
            }
        }
    }

    void paint(draw_context& dc, int x, int y, float scale)
    {
        float w = m_images[m_frame].width * scale * 0.5f;
        float h = m_images[m_frame].height * scale * 0.5f;
        dc.point_image(&m_images[m_frame], x, y - h * 0.5f, w, h);
    }
};


}//end namespace graphics
}//end namespace cgl

#endif //SPIRIT_HPP_20190716150838
