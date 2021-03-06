/*
 Copyright (c) 2005-2020 sdragonx (mail:sdragonx@foxmail.com)

 bitset.hpp

 2016-03-31 22:24:02

 相比标准库的bitset，这个可以存储无限位的数据，并且能够方便的以二进制形式保存到文件中

*/
#ifndef BITSET_HPP_20160331222402
#define BITSET_HPP_20160331222402

#include <cstdlib>

namespace cgl{

//设置数据位数据
uint32_t bit_set(byte_t* data, size_t x, size_t bits, uint32_t value)
{
    uint32_t mask;
    if(bits == 32){
        reinterpret_cast<uint32_t*>(data)[x] = value;
    }
    else if(bits == 24){
        mask = 0x00FFFFFF;
        data += x*3;
        uint32_t* p = reinterpret_cast<uint32_t*>(data);
        *p &= ~mask;
        value &= mask;
        *p |= value;
    }
    else if(bits == 16){
        reinterpret_cast<uint16_t*>(data)[x] = value;
    }
    else if(bits == 8){
        data[x] = value;
    }
    else if(bits < 8){
        mask = (1<<bits)-1;
        x *= bits;
        data += x/8;
        value &= mask;
        x &= 7;
        value <<= x;
        mask <<= x;
        mask = ~mask;
        byte_t n = data[0];
        n &= mask;
        n |= value;
        data[0] = n;
    }
    return 0;
}

uint32_t bit_get(const byte_t* data, int x, int bits)
{
    uint32_t mask;
    if(bits == 32){
        return reinterpret_cast<const uint32_t*>(data)[x];
    }
    else if(bits == 24){
        mask = 0x00FFFFFF;
        data += x*3;
        const uint32_t* p = reinterpret_cast<const uint32_t*>(data);
        return *p & mask;
    }
    else if(bits == 16){
        return reinterpret_cast<const uint16_t*>(data)[x];
    }
    else if(bits == 8){
        return data[x];
    }
    else if(bits < 8){
        mask = (1<<bits)-1;
        x *= bits;
        data += x/8;
        x &= 7;
        return (data[0]>>x) & mask;
    }
    else{
        assert(0);
        return 0;
    }
}

//sdragonx 2020-02-10 01:01:33
template<size_t BITSIZE, typename T = byte_t>
struct bitset
{
public:
    typedef T type;

    const static int type_size = sizeof(T);
    const static int type_bits = type_size * 8;
    const static int type_mask = type_bits - 1;
    const static int data_size = BITSIZE / type_bits + (BITSIZE & type_mask ? 1 : 0);    //需要数据长度
    const static int byte_size = data_size * type_size;
    /*
    enum{
        type_size = sizeof(T),
        type_bits = type_size * 8,
        type_mask = type_bits - 1,
        data_size = BITSIZE / type_bits + (BITSIZE & type_mask ? 1 : 0),
        byte_size = data_size * type_size
    };
    */
    type data[data_size];

public:
    bitset()
    {
        this->reset();
    }

    size_t size()const
    {
        return BITSIZE;
    }

    bool bit(size_t id)const
    {
        return (block(id) >> (id & type_mask)) & 0x01;
    }

    bool operator[](size_t id)const
    {
        return this->bit(id);
    }

    void set()
    {
        memset(data, 0xFF, byte_size);
    }

    bool set(size_t id)
    {
        type& b = block(id);
        b |= type(1) << (id & type_mask);
        return true;
    }

    void reset()
    {
        memset(data, 0, byte_size);
    }

    bool reset(size_t id)
    {
        type& b = block(id);
        b &= ~(type(1) << (id & type_mask));
        return false;
    }

    void print(char *buf)
    {
        for(size_t i=0; i<this->size(); ++i){
            *buf++ = this->bit(i) ? '1' : '0';
        }
        *buf = '\0';
    }

private:
    //get the data block
    inline type& block(size_t id)
    {
        return data[id / type_bits];
    }

    inline const type& block(size_t id)const
    {
        return data[id / type_bits];
    }
};

template<size_t X, size_t Y>
class bitset2d
{
public:
    typedef bitset<X> line;

    line lines[Y];
public:
    bitset2d()
    {

    }

    size_t size_x()const
    {
        return X;
    }

    size_t size_y()const
    {
        return Y;
    }

    size_t size()const
    {
        return X * Y;
    }

    bool bit(size_t x, size_t y)const
    {
        return lines[y].bit(x);
    }

    const line& operator[](size_t y)const
    {
        return lines[y];
    }

    void set()
    {
        memset(lines[0].data, 0xFF, line::byte_size * Y);
    }

    bool set(size_t x, size_t y)
    {
        lines[y].set(x);
        return true;
    }

    void reset()
    {
        memset(lines[0].data, 0x00, line::byte_size * Y);
    }

    bool reset(size_t x, size_t y)
    {
        lines[y].reset(x);
        return false;
    }

};

}//end namespace cgl

#endif //BITSET_HPP_20160331222402

/*
//1 2 4 8
template<int bits>
int bit_read(const void* in, size_t pos)
{
    typedef byte_t type;
    const int type_bits = sizeof(type) * 8;
    const type type_mask = type_bits - 1;
    const type mask = (1 << bits) - 1;

    struct _3{
        char d[3];
    };

    const type* data = reinterpret_cast<const type*>(in);
    pos *= bits;
    return (data[pos / type_bits ] >> (pos & type_mask)) & mask;
}


template<size_t BITSIZE>
struct bitset
{
public:
    //需要数据长度
    const static size_t byte_size = BITSIZE / 8 + (BITSIZE & 0x07 ? 1 : 0);

    byte_t data[byte_size];

public:
    bitset()
    {
        this->reset();
    }

    size_t size()const
    {
        return BITSIZE;
    }

    bool bit(size_t id)const
    {
        return (data[id / 8] >> (id & 0x07)) & 0x01;
    }

    bool operator[](size_t id)const
    {
        return this->bit(id);
    }

    void set()
    {
        memset(data, 0xFF, byte_size);
    }

    bool set(size_t id)
    {
        byte_t& b = data[id / 8];
        b |= 1 << (id & 0x07);
        return true;
    }

    void reset()
    {
        memset(data, 0, byte_size);
    }

    bool reset(size_t id)
    {
        byte_t& b = data[id / 8];
        b &= ~(1 << (id & 0x07));
        return false;
    }
};
*/