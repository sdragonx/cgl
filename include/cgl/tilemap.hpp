/*
 Copyright (c) 2005-2020 sdragonx (mail:sdragonx@foxmail.com)

 tilemap.hpp

 2019-06-08 19:53:03

*/
#ifndef TILEMAP_HPP_20190608195303
#define TILEMAP_HPP_20190608195303

#include <cgl/array2d.hpp>
#include <cgl/algorithm/perlin.hpp>
#include <cgl/graphics/color.hpp>
#include <cgl/math/random.hpp>

namespace cgl{

using namespace graphics;

static vec4ub tilemap_color[16] = {
    color::teal,
    color::cyan,    //ˮ
    color::cyan,
    //color::cyan,
    color::darkGreen,    //����
    color::forestGreen,
    color::forestGreen,    //���ĵ�����
    color::olive,
    color::olive,
    color::olive,
    color::olive,    //����
    color::silver,    //�ұ�
    color::darkGray,//ɽ
    color::dimGray,    //��ɽ
    color::dimGray,
    color::snow,
    color::snow,    //ѩɽ
};

template<typename T>
class tilemap
{
public:
    typedef T value_type;

protected:
    array2d<value_type> m_map;

public:
    tilemap() : m_map()
    {

    }

    ~tilemap()
    {
        this->dispose();
    }

    void resize(size_t x, size_t y)
    {
        m_map.resize(x, y);
    }

    virtual void dispose()
    {
        m_map.clear();
    }

    void clear()
    {
        m_map.fill(value_type());
    }

    size_t size_x()const
    {
        return m_map.size_x();
    }

    size_t size_y()const
    {
        return m_map.size_y();
    }

    void fill(const value_type& value)
    {
        m_map.fill(value);
    }

    bool is_valid(size_t x, size_t y)const
    {
        return m_map.is_valid(x, y);
    }

    value_type& tile(int x, int y)
    {
        return m_map[y][x];
    }

    void set_tile(size_t x, size_t y, const value_type& value)
    {
        if(is_valid(x, y)){
            m_map[y][x] = value;
        }
    }

    array2d<value_type>::line& operator[](size_t y)
    {
        return m_map[y];
    }
};

template<typename T>
void random_map(tilemap<T>& map, int width, int height, int seed, int round = 6, float persistance = 0.5f)
{
    array2d<float> base;
    array2d<float> noise;

    perlin_noise_init(base, width, height, seed);

    //GenerateSmoothNoise(m, base, round);
    GeneratePerlinNoise(noise, base, round, persistance);

    map.resize(width, height);
    for (int y = 0; y < height; ++y){
        for (int x = 0; x < width; ++x){
            map[y][x] = noise[y][x] * 16;
        }
    }
}

}//end namespace cgl

#endif //TILEMAP_HPP_20190608195303
