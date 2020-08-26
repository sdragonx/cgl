/*
 Copyright (c) 2005-2020 sdragonx (mail:sdragonx@foxmail.com)

 matrix.hpp

 2020-02-15 01:24:15

*/
#ifndef MATRIX_HPP_20200215012415
#define MATRIX_HPP_20200215012415

#include <cgl/public.h>

namespace cgl{


//
// matrix<T, X, Y>
//

template <typename T, size_t X, size_t Y>
class matrix// : public array< array<T, X>, Y>
{
public:
    typedef matrix this_type;
    typedef T                                     value_type;
    typedef value_type&                           reference;
    typedef const value_type&                     const_reference;
    typedef value_type*                           iterator;
    typedef const value_type*                     const_iterator;
    typedef value_type*                           pointer;
    typedef const value_type*                     const_pointer;
    typedef size_t                                size_type;
    typedef ptrdiff_t                             difference_type;
    typedef std::reverse_iterator<iterator>       reverse_iterator;
    typedef std::reverse_iterator<const_iterator> const_reverse_iterator;

    enum{
        sizeX = X,
        sizeY = Y,
        count = X * Y
    };

public:
    union{
        value_type m_mat[Y][X];
        value_type m_data[count];
    };

public:
    size_type size()const
    {
        return count;
    }

    size_type max_size()const
    {
        return count;
    }

    bool empty()const
    {
        return !count;
    }

    value_type* data()
    {
        return m_data;
    }

    const value_type* data()const
    {
        return m_data;
    }

    iterator begin()
    {
        return iterator(m_data);
    }

    const_iterator begin()const
    {
        return const_iterator(m_data);
    }

    iterator end()
    {
        return iterator(m_data + X);
    }

    const_iterator end()const
    {
        return const_iterator(m_data + X);
    }

    reverse_iterator rbegin()
    {
        return reverse_iterator(end());
    }

    const_reverse_iterator rbegin()const
    {
        return const_reverse_iterator(end());
    }

    reverse_iterator rend()
    {
        return reverse_iterator(begin());
    }

    const_reverse_iterator rend()const
    {
        return const_reverse_iterator(begin());
    }

    reference front()
    {
        return m_data[0][0];
    }

    const_reference front()const
    {
        return m_data[0][0];
    }

    reference back()
    {
        return m_data[size() > 0 ? size()-1 : 0];
    }

    const_reference back()const
    {
        return m_data[size() > 0 ? size()-1 : 0];
    }

    reference operator[](size_type n)
    {
        return m_data[n];
    }

    const_reference operator[](size_type n)const
    {
        return m_data[n];
    }

    reference at(size_type n)
    {
        if(n >= X)throw("array::at");
        return m_data[n];
    }

    const_reference at(size_type n)const
    {
        if (n >= X)throw("array::at");
        return m_data[n];
    }

    void fill(const value_type& value)
    {
        std::fill_n(m_data, X, value);
    }

    void swap(matrix& other)
    {
        std::swap_ranges(m_data, m_data + X, other.m_data);
    }
};

}//end namespace cgl

#endif //MATRIX_HPP_20200215012415
