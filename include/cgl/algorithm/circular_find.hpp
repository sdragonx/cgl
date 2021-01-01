/*
 Copyright (c) 2005-2020 sdragonx (mail:sdragonx@foxmail.com)

 circular_find.hpp

 2019-07-25 19:56:26

 vec2s 1024x1024 = 4M

*/
#ifndef CIRCULAR_FIND_HPP_20190725195626
#define CIRCULAR_FIND_HPP_20190725195626

#include <cgl/stack.hpp>

namespace cgl{

class circular_find
{
public:
    const static int SIZE_X = 1024;
    const static int SIZE_Y = 1024;

    enum{
        OPEN_LIST = 1,
        CLOSE_LIST = 2
    };

    struct node
    {
        int16_t x, y;
        int g;

        node() : x(), y(), g() { }
        node(int _x, int _y) : x(_x), y(_y), g() { }
        bool operator<(const node& n)const
        {
            return n.g < g;
        }
    };

protected:
    char m_map[SIZE_Y][SIZE_X];
    binstack<node> m_openlist;

    inline int calc_g(int x, int y, int sx, int sy)
    {
        x = sx - x;
        y = sy - y;
        return x * x + y * y;
    }

public:
    inline bool is_invalid(int x, int y)const
    {
        if(x < 0 || x >= SIZE_X || y < 0 || y >= SIZE_Y){
            return true;
        }
        return false;
    }

    inline bool is_valid(int x, int y)const
    {
        return x >= 0 && x < SIZE_X && y >= 0 && y < SIZE_Y;
    }

    inline bool can_search(int x, int y)
    {
        if(!is_valid(x, y)){
            return false;
        }
        return m_map[y][x] == 0;
    }

    inline void search(int x, int y, int ox, int oy)
    {
        if(can_search(x, y)){
            node p(x, y);
            p.g = calc_g(x, y, ox, oy);
            m_openlist.push(p);
            m_map[y][x] = OPEN_LIST;
        }
    }

    int find(std::vector<vec2i>& ls, int x, int y, int size)
    {
        memset(m_map, 0, sizeof(m_map));
        m_openlist.clear();
        m_openlist.reserve(size);
        ls.clear();
        ls.reserve(size);

        search(x, y, x, y);

        node tile;
        int n = 0;
        while(!m_openlist.empty())
        {
            tile = m_openlist.top();
            m_openlist.pop();
            m_map[tile.y][tile.x] = CLOSE_LIST;
            ls.push_back(vec2i(tile.x, tile.y));

            ++n;
            if(n >= size){
//                final.x = tile.x;
//                final.y = tile.y;
                return n;
            }

            search(tile.x, tile.y - 1, x, y);
            search(tile.x, tile.y + 1, x, y);
            search(tile.x - 1, tile.y, x, y);
            search(tile.x + 1, tile.y, x, y);
        }
    }
};

}//end namespace cgl

#endif //CIRCULAR_FIND_HPP_20190725195626
