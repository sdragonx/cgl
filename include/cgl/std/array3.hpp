/*
 Copyright (c) 2005-2020 sdragonx (mail:sdragonx@foxmail.com)

 array3.hpp

 2020-10-22 01:56:55

*/
#ifndef ARRAY3_HPP_20201022015655
#define ARRAY3_HPP_20201022015655

#include <cgl/public.h>

namespace cgl{


template<typename T>
class array3
{
public:
    typedef array3 this_type;
    typedef T value_type;
    typedef std::vector<value_type> line_type;
    typedef std::vector<line_type> plane_type;
    typedef std::vector<plane_type> container_type;

private:
    container_type m_data;
    size_t m_size_x;
    size_t m_size_y;
    size_t m_size_z;

public:
    array3() : m_data(), m_size_x(), m_size_y(), m_size_z()
    {
    }

    //
    // 空间管理
    //

    bool empty()const
    {
        return m_data.empty() || m_data[0].empty() || m_data[0][0].empty();
    }

    size_t size()const
    {
        return m_size_x * m_size_y * m_size_z;
    }

    size_t size_x()const
    {
        return m_size_x;
    }

    size_t size_y()const
    {
        return m_size_y;
    }

    size_t size_z()const
    {
        return m_size_z;
    }

    void resize(size_t sizeX, size_t sizeY, size_t sizeZ)
    {
        m_data.resize(sizeZ);
        for(size_t z = 0; z < sizeZ; ++z){
            m_data[z].resize(sizeY);
            for(size_t y = 0; y < sizeY; ++y){
                m_data[z][y].resize(sizeX);
            }
        }

        m_size_x = sizeX;
        m_size_y = sizeY;
        m_size_z = sizeZ;
    }

    void resize(size_t sizeX, size_t sizeY, size_t sizeZ, const value_type& value)
    {
        m_data.resize(sizeZ);
        for(size_t z = 0; z < sizeZ; ++z){
            m_data[z].resize(sizeY);
            for(size_t y = 0; y < sizeY; ++y){
                m_data[z][y].resize(sizeX, value);
            }
        }

        m_size_x = sizeX;
        m_size_y = sizeY;
        m_size_z = sizeZ;
    }

    void clear()
    {
        m_data.clear();
    }

    //
    // 操作符
    //

    plane_type& operator[](size_t i)
    {
        return m_data[i];
    }

    const plane_type& operator[](size_t i)const
    {
        return m_data[i];
    }

    this_type& operator=(const this_type& other)
    {
        m_data = other.m_data;
        return *this;
    }

    //
    // 方法
    //

    bool is_valid(size_t x, size_t y, size_t z)
    {
        return x < size_x() && y < size_y() && z < size_z();
    }

    void fill(const T& value)
    {
        for(size_t y=0; y < m_data.size(); ++y){
            std::fill(m_data[y].begin(), m_data[y].end(), value);
        }
    }

    void swap(this_type& other)
    {
        m_data.swap(other.m_data);
    }
};

}//end namespace cgl

#endif //ARRAY3_HPP_20201022015655