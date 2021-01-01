/*
 Copyright (c) 2005-2020 sdragonx (mail:sdragonx@foxmail.com)

 array2.hpp

 2006-10-08 22:26:44

 ������ά����ĳ���array2<T>::array_t��C�Ĵ�ָ��ģʽ��һ��Ҫʹ��
   delete_array2<T>()ɾ��������Ч��
 ��vector<T>�����������Ч��Ҫ��һЩ

 2018/8/20 13:25

*/
#ifndef CGL_ARRAY2_HPP_20061008222644
#define CGL_ARRAY2_HPP_20061008222644

#include <vector>

namespace cgl{

/*C���Ķ�ά����*/
template<typename T>
void allocate_array2(T** &v, int size_x, int size_y)
{
    v = new T*[size_y];
    for(int y=0; y < size_y; ++y)
        v[y] = new T[size_x];
}

template<typename T>
void allocate_array2(T** &v, int size_x, int size_y, const T& val)
{
    v = new T*[size_y];
    for(int y=0; y < size_y; ++y)
    {
        v[y] = new T[size_x];
        for(int x=0; x < size_x; ++x)
            v[y][x] = val;
    }
}

template<typename T>
void delete_array2(T** &v, int size_y)
{
    for(int y=0; y < size_y; ++y)
        delete[] v[y];
    delete[] v;
    v = NULL;
}

/*vector<T>���Ķ�̬����*/
template<typename T>
void new_array2(std::vector<std::vector<T> > &v, int size_x, int size_y)
{
    v.resize(size_y);
    for(int y=0; y < size_y; ++y){
        v[y].resize(size_x);
    }

    //����2������3
    //�ٶȲ�࣬��size_y���޴�ʱ���������ַ����ٶȻ�����һЩ
    //v.resize(size_x, std::vector<T>(size_y));
    //v.swap( std::vector<std::vector<T> >(size_x, std::vector<T>(size_y)) );
    //ֱ�ӷ�ֵ���ٶȷǳ�����ֱ�ӳ�ʼ�����ٶ�Ҳ�Ƿǳ���
    //v = std::vector<std::vector<T> >(size_x, std::vector<T>(size_y))
}

template<typename T>
void new_array2(std::vector<std::vector<T> > &v, int size_x, int size_y, const T& val)
{
    v.resize(size_y);
    for(int y=0; y < size_y; ++y)
        v[y].resize(size_x, val);
}

template<typename T>
void delete_array2(std::vector<std::vector<T> > &v)
{
    std::vector<std::vector<T> > tmp;
    v.swap(tmp);
}

//�����ά�����
template<typename T, int sizeX, int sizeY>
T total(T mat[sizeY][sizeX])
{
    T n = T();
    for(size_t y=0; y < sizeY; ++y)
        for(size_t x=0; x < sizeX; ++x)
            n += mat[y][x];

    return n;
}

/*
** vector<T>�Ķ�ά����
*/

template<typename T>
class array2
{
public:
    typedef array2 this_type;
    typedef T value_type;
    typedef std::vector<T> line_type;
    typedef std::vector<line_type> container_type;

private:
    container_type m_data;

public:
    array2() : m_data(){/*void*/}

    array2(const this_type& v) : m_data(v.m_data){/*void*/}

    array2(size_t x, size_t y) : m_data()
    {
        this->resize(x, y);
    }

    array2(size_t x, size_t y, const T& value) : m_data()
    {
        this->resize(x, y, value);
    }

    //
    // �ռ����
    //

    bool empty()const
    {
        return m_data.empty() || m_data[0].empty();
    }

    size_t size()const
    {
        return size_x() * size_y();
    }

    size_t size_x()const
    {
        return empty() ? 0 : m_data[0].size();
    }

    size_t size_y()const
    {
        return m_data.size();
    }

    void resize(size_t x, size_t y)
    {
        new_array2<T>(m_data, x, y);
    }

    void resize(size_t x, size_t y, const value_type& value)
    {
        new_array2<T>(m_data, x, y, value);
    }

    void clear()
    {
        delete_array2<T>(m_data);
    }

    //
    // ������
    //

    line_type& operator[](int y)
    {
        return m_data[y];
    }

    const line_type& operator[](int y)const
    {
        return m_data[y];
    }

    this_type& operator=(const this_type& other)
    {
        m_data = other.m_data;
        return *this;
    }

    //
    // ����
    //

    bool is_valid(size_t x, size_t y)const
    {
        return x < size_x() && y < size_y();
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

/*
array2d



class array2d
{
public:
    typedef array2d this_type;
    typedef int value_type;
    typedef value_type* pointer;


private:
    pointer m_data;
    size_t m_size;
    size_t m_width;
    size_t m_height;

public:
    array2d() : m_data(), m_size(), m_width(), m_height()
    {

    }

    ~array2d()
    {
        this->dispose();
    }

    bool empty()const
    {
        return this->size() == 0;
    }

    size_t size()const
    {
        return m_width * m_height;
    }

    void resize(size_t x, size_t y)
    {
        if(x * y < m_size){
            if(x == m_width){//line size not change
                m_height = y;
                return ;
            }
            else{
                this_type temp;
                temp.bind(*this);
                temp.m_width = x;
                temp.m_height = y;
                copy(temp);
                release();
                swap(temp);
            }
        }
        else{
            this->reserve(x, y);
        }
        m_width = x;
        m_height = y;
    }

    void reserve(size_t x, size_t y)
    {
        if(m_size < x * y){
            this_type temp;
            temp.allocate(x, y);
            copy(temp);
            swap(temp);
        }
    }

    void clear()
    {
        m_width = m_height = 0;
    }

    void dispose()
    {
        if(m_data){
            delete []m_data;
            this->release();
        }
    }

    pointer begin()
    {
        return &m_data[0];
    }

    pointer end()
    {
        return &m_data[m_width * m_height];
    }

    void fill(const value_type& value)
    {
        std::fill(this->begin(), this->end(), value);
    }

    pointer line(int y)
    {
        return &m_data[y * m_height];
    }

    pointer operator[](int y)
    {
        return &m_data[y * m_width];
    }

    void copy(this_type& other)
    {
        for(size_t y=0; y<other.m_height; ++y){
            std::copy(line(y), line(y) + other.m_width, other.line(y));
        }
    }

    void swap(this_type& other)
    {
        std::swap(m_data, other.m_data);
        std::swap(m_size, other.m_size);
        std::swap(m_width , other.m_width);
        std::swap(m_height, other.m_height);
    }

private:
    void allocate(int x, int y)
    {
        if(m_data){
            delete []m_data;
        }
        m_data = new value_type[x * y];
    }

    void bind(this_type& other)
    {
        this->dispose();
        m_data = other.m_data;
        m_size = other.m_size;
        m_width = other.m_width;
        m_height = other.m_height;
    }

    void release()
    {
        m_data = null;
        m_size = m_width = m_height = 0;
    }
};


*/

//
// for_each
//
// void function(x, y, T& value)
//
template<typename T>
void for_each(array2<T>& obj, void(*pfn)(int , int , T& ))
{
    for (int y=0; y<obj.size_y(); ++y){
        for (int x=0; x<obj.size_x(); ++x){
            pfn(x, y, obj[y][x]);
        }
    }
}

}// end namespace cgl

#endif //CGL_ARRAY2_HPP_20061008222644