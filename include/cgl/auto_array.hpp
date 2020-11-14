/*
 Copyright (c) 2005-2020 sdragonx (mail:sdragonx@foxmail.com)

 auto_array.hpp

 2019-06-06 17:57:11

 根据需要自动添加元素

*/
#ifndef AUTO_ARRAY_HPP_20190606175711
#define AUTO_ARRAY_HPP_20190606175711

#include <vector>

namespace cgl{

//
// auto_array
//

template<typename T>
class auto_array
{
public:
    typedef auto_array this_type;
    typedef T value_type;

private:
    std::vector<value_type> ls;

public:
    auto_array() : ls()
    {

    }

    void dispose()
    {
        ls.clear();
    }

    bool empty()const
    {
        return ls.empty();
    }

    size_t size()const
    {
        return ls.size();
    }

    void set(size_t index, const value_type& value)
    {
        this->allocate(index);
        ls[index] = value;
    }

    value_type& get(size_t index)
    {
        this->allocate(index);
        return ls[index];
    }

    const value_type& get(size_t index)const
    {
        if(index < ls.size()){
            return ls[index];
        }
        return single<value_type>::value;
    }

    value_type& operator[](size_t index)
    {
        return this->get(index);
    }

    const value_type& operator[](size_t index)const
    {
        return this->get(index);
    }

    void erase(size_t index)
    {
        if(index < ls.size()){
            ls.erase(ls.begin() + index);
        }
    }

    void swap(this_type& other)
    {
        ls.swap(other.ls);
    }

private:
    void allocate(size_t size)
    {
        if(ls.size() <= size){
            ls.resize(size + 1);
        }
    }
};

//
// auto_array2d dynamic_cell
//

template<typename T>
class auto_array2d
{
public:
    typedef T value_type;
    typedef auto_array<value_type> line;
    typedef std::vector<line> container;

protected:
    container lines;

public:
    auto_array2d() : lines()
    {

    }

    ~auto_array2d()
    {

    }

    void dispose()
    {
        lines.clear();
    }

    bool empty()const
    {
        return lines.empty();
    }

    size_t size()const
    {
        return lines.size();
    }

    size_t total()const
    {
        size_t n = 0;
        for(size_t y=0; y<lines.size(); ++y){
            n += lines[y].size();
        }
        return n;
    }

    void set(size_t x, size_t y, const value_type& value)
    {
        this->allocate(y);
        lines[y][x] = value;
    }

    value_type& get(size_t x, size_t y)
    {
        this->allocate(y);
        return lines[y][x];
    }

    const value_type& get(size_t x, size_t y)const
    {
        if(y < lines.size()){
            return lines[y][x];
        }
        return single<value_type>::value;
    }

    line& operator[](size_t y)
    {
        this->allocate(y);
        return lines[y];
    }

    const line& operator[](size_t y)const
    {
        if(y < lines.size()){
            return lines[y];
        }
        return single<line>::value;
    }

    void erase_row(size_t y)
    {
        if(y < lines.size()){
            lines.erase(lines.begin() + y);
        }
    }

    void erase_column(size_t x)
    {
        for(size_t i=0; i<lines.size(); ++i){
            lines[i].erase(x);
        }
    }

    void swap_row(size_t y1, size_t y2)
    {
        this->allocate(y1);
        this->allocate(y2);
        lines[y1].swap(lines[y2]);
    }

    void swap_column(size_t x1, size_t x2)
    {
        for(size_t i=0; i<lines.size(); ++i){
            if(x1 < lines[i].size() || x2 < lines[i].size()){
                std::swap(this->get(x1, i), this->get(x2, i));
            }
        }
    }

private:
    void allocate(size_t size)
    {
        if(lines.size() <= size){
            lines.resize(size + 1);
        }
    }
};

//auto_array3d

}//end namespace cgl

#endif //AUTO_ARRAY_HPP_20190606175711
